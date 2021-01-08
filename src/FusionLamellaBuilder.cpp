/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// FusionLamellaBuilder.cpp: implementation of the CFusionLamellaBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "FusionLamellaBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"

#include "CNTCell.h"			// needed for random number generator


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFusionLamellaBuilder::CFusionLamellaBuilder(zLongVector polymerTypes1,	zLongVector polymerTypes2,
							long x, long y, long z, 
							const double centre[2], const double thickness[2], 
							bool bLinear, const zDoubleVector upperFractions,
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
		m_Centre[i]		= centre[i];
		m_Thickness[i]		= thickness[i];
		m_bPatches[i]		= false;		// unused flags needed by CBuilder::isBilayer
		m_BilayerCentre[i]	= bilayercentre[i];
		m_UpperHead[i]		= upperHead[i];
		m_LowerHead[i]		= lowerHead[i];
		m_bMonolayer[i]	= false;
		m_PolymerTotal[i]	= 0;
		m_UpperTotal[i]	= 0;
		m_LowerTotal[i]	= 0;
	}
}

CFusionLamellaBuilder::~CFusionLamellaBuilder()
{

}

// Virtual function to assemble beads into an initial configuration with 
// two parallel bilayers located at specified positions. 
// 
// We use the CBuilder enclosed objects, isBilayer, isRandom, to construct 
// two parallel bilayers at the specified locations and fill the gaps between
// them with the remaining polymer types. We do not provide any checks that 
// the bilayers don't overlap each other: that is the user's responsibility. 
// However, they are prevented from overlapping the walls by the CBuilder routines.

bool CFusionLamellaBuilder::Assemble(CInitialState& riState)
{
	// ****************************************
	// Position the wall beads first because they cannot move and may be 
	// bonded to beads in the bulk whose positions can be arranged so 
	// that their bonds are not too large initially.

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}

	// Check to see if all the polymers in either bilayer are in one monolayer.
	// We do the lower bilayer first but check the upper monolayer in each bilayer first.

	m_PolymerTotal[0]	= 0;	// Lower bilayer in SimBox
	m_UpperTotal[0]	= 0;	// Upper monolayer in lower bilayer
	m_LowerTotal[0]	= 0;	// Lower monolayer in lower bilayer

	for(long unsigned int iPolymer=0; iPolymer<m_PolymerTypes1.size(); iPolymer++)
	{
		long polyNo = riState.GetPolymerTotalForType(m_PolymerTypes1.at(iPolymer));
		long upperNo = static_cast<long>(m_UpperFractions.at(iPolymer)*polyNo);

		m_PolymerTotal[0]	+= polyNo;
		m_UpperTotal[0]	+= upperNo;
		m_LowerTotal[0]	+= (polyNo - upperNo);
	}

	m_PolymerTotal[1]	= 0;	// Upper bilayer in SimBox
	m_UpperTotal[1]	= 0;	// Upper monolayer in upper bilayer
	m_LowerTotal[1]	= 0;	// Lower monolayer in upper bilayer

	for(long unsigned int iPolymer=0; iPolymer<m_PolymerTypes2.size(); iPolymer++)
	{
		long polyNo = riState.GetPolymerTotalForType(m_PolymerTypes2.at(iPolymer));
		long upperNo = static_cast<long>(m_UpperFractions.at(iPolymer)*polyNo);

		m_PolymerTotal[1]	+= polyNo;
		m_UpperTotal[1]	+= upperNo;
		m_LowerTotal[1]	+= (polyNo - upperNo);
	}

	// If either monolayer in either bilayer is empty recalculate the free space 
	// outside the bilayer to be filled with non-bilayer polymers.

	for(unsigned int i=0; i<2; i++)
	{
		if(m_UpperTotal[i] == 0)	// Upper monolayer empty
		{
			m_bMonolayer[i] = true;
			m_UpperHead[i]	= m_BilayerCentre[i];
		}
		else if(m_LowerTotal[i] == 0)
		{
			m_bMonolayer[i] = true;
			m_LowerHead[i]	= m_BilayerCentre[i];
		}
		else
		{
			m_bMonolayer[i] = false;
		}
	}

	// Create the lower bilayer using a CBuilder::isBilayer object

	isBilayer bilayer1(m_PolymerTypes1, m_UpperFractions,
					  m_X, m_Y, m_Z, m_BilayerCentre[0], m_Thickness[0],
					  m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
					  m_UpperHead[0], m_LowerHead[0], m_UpperTotal[0], m_LowerTotal[0],
					  m_bMonolayer[0], m_bLinearise, m_bPatches);


	bilayer1.Assemble(riState);

	// Create the upper bilayer

	isBilayer bilayer2(m_PolymerTypes2, m_UpperFractions,
					  m_X, m_Y, m_Z, m_BilayerCentre[1], m_Thickness[1],
					  m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
					  m_UpperHead[1], m_LowerHead[1], m_UpperTotal[1], m_LowerTotal[1],
					  m_bMonolayer[1], m_bLinearise, m_bPatches);


	bilayer2.Assemble(riState);

	// Calculate the free space available between the bilayers and the walls, 
	// or the SimBox boundaries if no wall is present. Then use a CBuilder::isRandom object 
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

	// Copy the coordinates of all beads into the other bead arrays used to ensure
	// that analysis of diffusion etc, is calculated from the initial bead positions.

	AssignUnPBCCoords(riState);

	// ****************************************
	// Set the in-plane velocities of all the beads from a Maxwell distribution 
	// with a mean  value and variance fixed from the temperature, but with a
	// normal component of velocity to zero. This applies to all beads not just
	// the ones in the bilayer. This may prevent penetrations of the new bilayer
	// by large random velocity beads.

	AssignMaxwellVelocities(riState, m_X, m_Y, m_Z);

	return true;
}

