/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// FreeLamellaBuilder.cpp: implementation of the CFreeLamellaBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "FreeLamellaBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"
#include "PolymerCrossLink.h"

#include "CNTCell.h"			// needed for random number generator



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFreeLamellaBuilder::CFreeLamellaBuilder(const zLongVector& polymerTypes, 
							 const zDoubleVector& upperFractions,
							 const PolymerCrossLinkVector& crossLinks,
							 const bool bPatches[2], bool bPolymerise, bool bLinear,
							 long x, long y, long z, const double centre[3], 
							 double thickness, double area, double l1, double l2, 
							 double bilayer1centre, double bilayer2centre, double bilayer3centre, 
							 double upperHead, double lowerHead) : m_PolymerTypes(polymerTypes),
											m_UpperFractions(upperFractions),
											m_CrossLinks(crossLinks),
											m_bPolymerise(bPolymerise),
											m_bLinearise(bLinear),
											m_X(x), m_Y(y), m_Z(z),
											m_Thickness(thickness),
											m_BilayerArea(area),
											m_Bilayer1Length(l1),
											m_Bilayer2Length(l2),
											m_Bilayer1Centre(bilayer1centre),
											m_Bilayer2Centre(bilayer2centre),
											m_Bilayer3Centre(bilayer3centre),
											m_UpperHead(upperHead),
											m_LowerHead(lowerHead),
											m_bMonolayer(false),
											m_PolymerTotal(0),
											m_UpperTotal(0),
											m_LowerTotal(0)
{
	m_bPatches[0] = bPatches[0];
	m_bPatches[1] = bPatches[1];
	m_Centre[0]   = centre[0];
	m_Centre[1]   = centre[1];
	m_Centre[2]   = centre[2];
}

CFreeLamellaBuilder::~CFreeLamellaBuilder()
{

}

// Virtual function to assemble beads into a lamella initial configuration.
// This differs from the CLamellaBuilder class in allowing multiple polymers 
// to be assembled in the bilayer. We just add together the polymers
// composing the bilayer in a single vector. All polymers will be
// stretched to the same head-tail distance but their in-plane coordinates
// are randomised. The fraction of each polymer type that is in each monolayer
// is specified.
// 
// The CInitialState object is responsible for creating the beads, bonds and
// polymers from the types defined by the CInputData object, via the function
// CreatePolymers(). This builder sets the coordinates of the beads so
// as to create the required lamella configuration. The polymers that compose the 
// lamella have already been created using a bulk concentration specified by
// the user. We don't know that this will contain enough polymers to cover the
// whole SimBox lateral area, but we try and distribute them uniformly anyway.
// The polymers are positioned at the lattice points of a regular hexagonal 
// lattice whose spacing is determined by the number of polymers and the
// size of the SimBox. The lattice spacing may be different for each monolayer
// if there are different numbers of polymers in each.
//
// The tail bead of each lamella polymer is positioned in the mid-plane and the
// head beads are fixed at a given distance above and below this plane. The
// other beads in the polymers are then either randomly positioned about the 
// head-tail axis, or positioned in a straight line between the head and tail
// if the linearise flag is set. This is probably not an equilibrium organisation 
// but it is the simplest that we can do.
//

bool CFreeLamellaBuilder::Assemble(CInitialState& riState)
{
	long iBead		= 0;	// counter used everywhere below
	long iPolymer	= 0;	// counter used everywhere below

	// ****************************************
	// Position the wall beads first because they cannot move and may be 
	// bonded to beads in the bulk whose positions can be arranged so 
	// that their bonds are not too large initially.

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}

	// Check if all polymers are in one monolayer. We just see if the sum 
	// of all the polymers in either monolayer is zero. This is obtained from
	// the fractions of each polymer type and the number thereof. We also store
	// the number of polymers of all types in each monolayer for use in the
	// isBilayer constructor.

	m_PolymerTotal	= 0;
	m_UpperTotal	= 0;
	m_LowerTotal	= 0;

	for(iPolymer=0; iPolymer<m_PolymerTypes.size(); iPolymer++)
	{
		long polyNo = riState.GetPolymerTotalForType(m_PolymerTypes.at(iPolymer));
		long upperNo = static_cast<long>(m_UpperFractions.at(iPolymer)*polyNo);

		m_PolymerTotal	+= polyNo;
		m_UpperTotal	+= upperNo;
		m_LowerTotal	+= (polyNo - upperNo);
	}

	// If either monolayer is empty recalculate the free space outside the
	// bilayer to be filled with non-bilayer polymers.

	if(m_UpperTotal == 0)			// Upper monolayer empty
	{
		m_bMonolayer	= true;
		m_UpperHead		= m_Bilayer3Centre;
	}
	else if(m_LowerTotal == 0)
	{
		m_bMonolayer	= true;
		m_LowerHead		= m_Bilayer3Centre;
	}
	else
	{
		m_bMonolayer	= false;
	}

	// Check the contents of the cross-links

	for(cPolymerCrossLinkIterator iterLink=m_CrossLinks.begin(); iterLink!=m_CrossLinks.end(); iterLink++)
	{
		const zString name	= (*iterLink)->GetName();
		double fraction		= (*iterLink)->GetFraction();
		long position		= (*iterLink)->GetPosition();
		long type			= (*iterLink)->GetType();
		double spring		= (*iterLink)->GetSpringConstant();
		double length		= (*iterLink)->GetUnstretchedLength();
	}


	// Create a bilayer

	isFreeBilayer bilayer(m_PolymerTypes, m_UpperFractions,
					  m_X, m_Y, m_Z, m_Thickness,
					  m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
					  m_Bilayer1Centre, m_Bilayer2Centre, m_Bilayer3Centre, 
					  m_UpperHead, m_LowerHead, m_UpperTotal, m_LowerTotal,
					  m_bMonolayer, m_bLinearise, m_bPatches);


	bilayer.Assemble(riState);

	// Calculate the free space available between the membrane and the wall, and
	// use an isRandom object to fill it with the remaining polymers in the
	// simulation. We pass a vector of the coordinates of the excluded regions
	// so that the polymers can be assigned coordinates in the free regions.
	// We start the measurements at the lower box face in the direction of the 
	// bilayer normal.

	// Store the polymer types that don't make up the bilayer

	zLongVector nonBilayerPolymerTypes;

	nonBilayerPolymerTypes.clear();

	for(iPolymer=0; iPolymer<riState.GetPolymerTypeTotal(); iPolymer++)
	{
		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), iPolymer) == m_PolymerTypes.end())
		{
			nonBilayerPolymerTypes.push_back(iPolymer);
		}	
	}

	zDoubleVector exRegions;

	exRegions.push_back(0.0);

	if(m_X == 1)
	{
		exRegions.push_back(riState.GetWallXWidth());
		exRegions.push_back(m_LowerHead);
		exRegions.push_back(m_UpperHead);
		exRegions.push_back(riState.GetSimBoxXLength() - riState.GetWallXWidth());
		exRegions.push_back(riState.GetSimBoxXLength());
	}
	else if(m_Y == 1)
	{
		exRegions.push_back(riState.GetWallYWidth());
		exRegions.push_back(m_LowerHead);
		exRegions.push_back(m_UpperHead);
		exRegions.push_back(riState.GetSimBoxYLength() - riState.GetWallYWidth());
		exRegions.push_back(riState.GetSimBoxYLength());
	}
	else if(m_Z == 1)
	{
		exRegions.push_back(riState.GetWallZWidth());
		exRegions.push_back(m_LowerHead);
		exRegions.push_back(m_UpperHead);
		exRegions.push_back(riState.GetSimBoxZLength() - riState.GetWallZWidth());
		exRegions.push_back(riState.GetSimBoxZLength());
	}

	// Fill the space between the bilayer and SimBox walls with randomly-positioned
	// polymers

	isRandom random(nonBilayerPolymerTypes, exRegions, m_X, m_Y, m_Z);

	random.Assemble(riState);

	// Copy the coordinates of all beads into the other bead arrays used to ensure
	// that analysis of diffusion etc, is calculated from the initial bead positions.

	AssignUnPBCCoords(riState);

	// ****************************************
	// Set the in-plane velocities of the beads from a 2d Maxwell distribution 
	// with a mean  value and variance fixed from the temperature, but with a
	// normal component of velocity to zero. This applies to all beads not just
	// the ones in the bilayer. This may prevent penetrations of the new bilayer
	// by large random velocity beads. The bilayer normal is indicated to the routine
	// by the arguments so that it can zero the appropriate component.

	AssignMaxwellVelocities(riState, m_X, m_Y, m_Z);

	return true;
}
