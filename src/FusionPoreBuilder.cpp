/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// FusionPoreBuilder.cpp: implementation of the CFusionPoreBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "FusionPoreBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"

#include "CNTCell.h"			// needed for random number generator



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFusionPoreBuilder::CFusionPoreBuilder(zLongVector polymerTypes1,	zLongVector polymerTypes2,
							long x, long y, long z, const double centre[2], const double thickness[2], 
							bool bLinear, zDoubleVector upperFractions,
							double area, double l1, double l2, 
							const double bilayercentre[2], 
							const double lowerHead[2], const double upperHead[2]) : m_PolymerTypes1(polymerTypes1),
																  m_PolymerTypes2(polymerTypes2),
																  m_X(x), m_Y(y), m_Z(z),
																  m_bLinearise(bLinear),
																  m_UpperFractions(upperFractions),
																  m_BilayerArea(area),
																  m_Bilayer1Length(l1),
																  m_Bilayer2Length(l2)
{
	for(short int i=0; i<2; i++)
	{
		m_Centre[i]			= centre[i];
		m_Thickness[i]		= thickness[i];
		m_BilayerCentre[i]	= bilayercentre[i];
		m_LowerHead[i]		= lowerHead[i];
		m_UpperHead[i]		= upperHead[i];

	}
}

CFusionPoreBuilder::~CFusionPoreBuilder()
{

}

// Virtual function to assemble beads into an initial configuration with 
// two parallel bilayers located at specified positions with a fusion pore
// connecting them.
// 
// We use the CBuilder enclosed objects, isBilayer, isRandom, to construct 
// two parallel bilayers at the specified locations and fill the gaps between
// them with the remaining polymer types. We do not provide any checks that 
// the bilayers don't overlap each other: that is the user's responsibility. 
// However, they are prevented from overlapping the walls by the CBuilder routines.

bool CFusionPoreBuilder::Assemble(CInitialState& riState)
{
	long iBead;

	// ****************************************
	// Position the wall beads first because they cannot move and may be 
	// bonded to beads in the bulk whose positions can be arranged so 
	// that their bonds are not too large initially.

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}

	// Create the first bilayer: note that the m_BilayerCentre[] gives the
	// coordinates in bead diameters wherease the m_Centre[] are fractions.

	isBilayer bilayer1(m_PolymerTypes1, m_X, m_Y, m_Z, 
					   m_BilayerCentre[0], m_Thickness[0], 
					   m_bLinearise, m_UpperFractions, 
					   m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
					   m_LowerHead[0], m_UpperHead[0]);

	bilayer1.Assemble(riState);

	// Create the second bilayer

	isBilayer bilayer2(m_PolymerTypes2, m_X, m_Y, m_Z, 
					   m_BilayerCentre[1], m_Thickness[1], 
					   m_bLinearise, m_UpperFractions, 
					   m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
					   m_LowerHead[1], m_UpperHead[1]);

	bilayer2.Assemble(riState);

	// Calculate the free space available between the bilayers and the walls, 
	// or the SimBox boundaries if no wall is present. Then use an isRandom object 
	// to fill it with the remaining polymers in the simulation. The excluded regions
	// are identified by their start and end coordinates, starting at the bottom 
	// face of the SimBox in the direction of the bilayer normal.

	// Store the polymer types that don't make up the two bilayers

	zLongVector nonBilayerPolymerTypes;

	nonBilayerPolymerTypes.clear();

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		if( find(m_PolymerTypes1.begin(), m_PolymerTypes1.end(), ip) == m_PolymerTypes1.end() &&
			find(m_PolymerTypes2.begin(), m_PolymerTypes2.end(), ip) == m_PolymerTypes2.end())
		{
			nonBilayerPolymerTypes.push_back(ip);
		}	
	}

	zDoubleVector exRegions;

	exRegions.push_back(0.0);		// Bottom SimBox face

	if(m_X == 1)
	{
		exRegions.push_back(riState.GetWallXWidth());
		exRegions.push_back(m_LowerHead[0]);
		exRegions.push_back(m_UpperHead[0]);
		exRegions.push_back(m_LowerHead[1]);
		exRegions.push_back(m_UpperHead[1]);
		exRegions.push_back(riState.GetSimBoxXLength() - riState.GetWallXWidth());
		exRegions.push_back(riState.GetSimBoxXLength());	// Top SimBox face
	}
	else if(m_Y == 1)
	{
		exRegions.push_back(riState.GetWallYWidth());
		exRegions.push_back(m_LowerHead[0]);
		exRegions.push_back(m_UpperHead[0]);
		exRegions.push_back(m_LowerHead[1]);
		exRegions.push_back(m_UpperHead[1]);
		exRegions.push_back(riState.GetSimBoxYLength() - riState.GetWallYWidth());
		exRegions.push_back(riState.GetSimBoxYLength());
	}
	else if(m_Z == 1)
	{
		exRegions.push_back(riState.GetWallZWidth());
		exRegions.push_back(m_LowerHead[0]);
		exRegions.push_back(m_UpperHead[0]);
		exRegions.push_back(m_LowerHead[1]);
		exRegions.push_back(m_UpperHead[1]);
		exRegions.push_back(riState.GetSimBoxZLength() - riState.GetWallZWidth());
		exRegions.push_back(riState.GetSimBoxZLength());
	}

	// Fill the space between the bilayers and the walls with randomly-positioned
	// polymers

	isRandom random(nonBilayerPolymerTypes, exRegions, m_X, m_Y, m_Z);

	random.Assemble(riState);


	// ****************************************
	// Set the in-plane velocities of all the beads from a Maxwell distribution 
	// with a mean  value and variance fixed from the temperature, but with a
	// normal component of velocity to zero. This applies to all beads not just
	// the ones in the bilayer. This may prevent penetrations of the new bilayer
	// by large random velocity beads.

	// The bead velocities are selected randomly from a much larger vector 
	// of velocities taken from a Maxwell distribution with unit mean and variance.
	// If the number of values needed exceeds 50000 then the actual number required
	// is used. Not all of the beads are in the membrane but this does not matter.

	zDoubleVector rvelDist = MaxwellDist(riState.GetBeadTotal());

	zDoubleVector velXDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector velYDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector velZDist(riState.GetBeadTotal(), 0.0);

	long index;
	double vmag, vtheta, vphi;
	double vp[3];
	double vcm[3];

	vcm[0] = 0.0;
	vcm[1] = 0.0;
	vcm[2] = 0.0;

	for(iBead=0; iBead<riState.GetBeadTotal(); iBead++)
	{
		index	= static_cast<long>(rvelDist.size()*CCNTCell::GetRandomNo());
		vmag	= rvelDist.at(index);
		vtheta	= acos(1.0-2.0*CCNTCell::GetRandomNo());
		vphi	= xxBase::m_globalTwoPI*CCNTCell::GetRandomNo();

		vp[0]   = vmag*sin(vtheta)*cos(vphi);
		vp[1]   = vmag*sin(vtheta)*sin(vphi);
		vp[2]   = 0.0;

		vcm[0] += vp[0];
		vcm[1] += vp[1];
		vcm[2] += vp[2];

		velXDist.at(iBead) = vp[0];
		velYDist.at(iBead) = vp[1];
		velZDist.at(iBead) = vp[2];
	}

	double vmean[3], v2mean[3], var[3];

	for(short i=0; i<3; i++)
	{
		vcm[i]	   /= static_cast<double>(riState.GetBeadTotal());
		vmean[i]	= 0.0;
		v2mean[i]	= 0.0;
		var[i]		= 0.0;
	}

	// remove CM velocity from bead velocities

	double vtotal = 0.0;

	for(iBead=0; iBead<riState.GetBeadTotal(); iBead++)
	{
		velXDist.at(iBead)	-= vcm[0];
		velYDist.at(iBead)  -= vcm[1];
		velZDist.at(iBead)  -= vcm[2];

		vtotal = vtotal + velXDist.at(iBead)*velXDist.at(iBead) +
						  velYDist.at(iBead)*velYDist.at(iBead) +
						  velZDist.at(iBead)*velZDist.at(iBead);
	}

	vtotal = sqrt(0.5*vtotal/static_cast<double>(riState.GetBeadTotal()));

	// finally normalize the velocities to the required temperature, 
	// calculate the mean and variance, and assign the bead velocities

	for(iBead=0; iBead<riState.GetBeadTotal(); iBead++)
	{
		velXDist.at(iBead)	= sqrt(riState.GetkT())*velXDist.at(iBead)/vtotal;
		velYDist.at(iBead)	= sqrt(riState.GetkT())*velYDist.at(iBead)/vtotal;
		velZDist.at(iBead)  = 0.0;

		(riState.GetBeads()).at(iBead)->SetXMom(velXDist.at(iBead));
		(riState.GetBeads()).at(iBead)->SetYMom(velYDist.at(iBead));
		(riState.GetBeads()).at(iBead)->SetZMom(velZDist.at(iBead));

		vmean[0] += velXDist.at(iBead);
		vmean[1] += velYDist.at(iBead);
		vmean[2] += velZDist.at(iBead);

		v2mean[0] += velXDist.at(iBead)*velXDist.at(iBead);
		v2mean[1] += velYDist.at(iBead)*velYDist.at(iBead);
		v2mean[2] += velZDist.at(iBead)*velZDist.at(iBead);
	}

	for(short j=0; j<3; j++)
	{
		vmean[j]  =  vmean[j]/static_cast<double>(riState.GetBeadTotal());
		v2mean[j] = v2mean[j]/static_cast<double>(riState.GetBeadTotal());
		var[j]    = v2mean[j] - vmean[j]*vmean[j];
	}


#ifdef TraceOn
	Trace("Initial vel distn");
	TraceVector("Mean Vel      = ", vmean[0],  vmean[1],  vmean[2]);
	TraceVector("Mean Sq Vel   = ", v2mean[0], v2mean[1], v2mean[2]);
	TraceVector("Var Vel       = ", var[0],    var[1],    var[2]);
	Trace("********************");
#endif

	return true;
}

