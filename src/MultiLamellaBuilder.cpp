/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// MultiLamellaBuilder.cpp: implementation of the CMultiLamellaBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "MultiLamellaBuilder.h"

#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiLamellaBuilder::CMultiLamellaBuilder(const zLongVector polymerTypes, long x, long y, long z, 
					 const zDoubleVector centres, const zDoubleVector upperFractions, 
					 bool bLinear, double thickness, double area, double l1, double l2) : m_PolymerTypes(polymerTypes),
																		m_X(x), m_Y(y), m_Z(z),
																		m_BilayerCentres(centres),
																		m_UpperFractions(upperFractions),
																		m_bLinearise(bLinear),
																		m_Thickness(thickness),
																		m_BilayerArea(area),
																		m_Bilayer1Length(l1),
																		m_Bilayer2Length(l2),
																		m_LamellaTotal(centres.size())
{
	m_LowerTotals.clear();
	m_UpperTotals.clear();
}

CMultiLamellaBuilder::~CMultiLamellaBuilder()
{

}

// Virtual function to assemble beads into an initial configuration with 
// multiple parallel lamellae located at specified positions. The polymers 
// are distributed randomly amongst the lamellae, with the same upper and lower
// monolayer fractions. If either monolayer is empty the build is aborted.
// 
// We use the CBuilder enclosed objects, isMultiBilayer, isRandom, to construct 
// the lamellae at the specified locations and fill the gaps between
// them with the remaining polymer types. We do not provide any checks that 
// the lamellae don't overlap each other: that is the user's responsibility. 
// However, they are prevented from overlapping the walls by the CBuilder routines.

bool CMultiLamellaBuilder::Assemble(CInitialState& riState)
{
	// ****************************************
	// Position the wall beads first because they cannot move and may be 
	// bonded to beads in the bulk whose positions can be arranged so 
	// that their bonds are not too large initially.

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}

	// Check to see if all the polymers in the bilayers are in one monolayer: 
	// this is not allowed to abort the build if so.

	long sumUpperTotal = 0;
	long sumLowerTotal = 0;

	for(long unsigned int iPolymer=0; iPolymer<m_PolymerTypes.size(); iPolymer++)
	{
		long polyNo = riState.GetPolymerTotalForType(m_PolymerTypes.at(iPolymer));
		long upperNo = static_cast<long>(m_UpperFractions.at(iPolymer)*polyNo);

		sumUpperTotal += upperNo;
		sumLowerTotal += (polyNo - upperNo);
	}

	if(sumUpperTotal == 0 || sumLowerTotal == 0)
	{
		return false;
	}

	// If there is more than one lamella, calculate the number of polymers in each,
	// and if the number of polymers is not exactly divisible by the 
	// number of lamellae, distribute the excess throughout the lamella so that
	// there are equal numbers of extra sites in each: this may mean some empty sites
	// in some lamella. But these density fluctuations will be eliminated in the
	// equilibration phase of the simulation.

	if(m_LamellaTotal > 1)
	{
		long firstUpperTotal = sumUpperTotal/m_LamellaTotal;
		long firstLowerTotal = sumLowerTotal/m_LamellaTotal;

		long potentialUpperTotal = firstUpperTotal*m_LamellaTotal;
		long potentialLowerTotal = firstLowerTotal*m_LamellaTotal;

		if(sumUpperTotal > potentialUpperTotal)
		{
			long diff = (sumUpperTotal - potentialUpperTotal)/m_LamellaTotal;

			if(diff > 0)
			{
				firstUpperTotal += diff;
			}
			else
			{
				firstUpperTotal += 1;
			}
		}

		if(sumLowerTotal > potentialLowerTotal)
		{
			long diff = (sumLowerTotal - potentialLowerTotal)/m_LamellaTotal;

			if(diff > 0)
			{
				firstLowerTotal += diff;
			}
			else
			{
				firstLowerTotal += 1;
			}
		}

		// Now copy the numbers of polymers to be placed in each lamella into
		// the containers.

		for(long lam=0; lam<m_LamellaTotal; lam++)
		{
			m_UpperTotals.push_back(firstUpperTotal);
			m_LowerTotals.push_back(firstLowerTotal);
		}

	}
	else
	{
		m_UpperTotals.push_back(sumUpperTotal);
		m_LowerTotals.push_back(sumLowerTotal);
	}

	// Create the bilayers using a CBuilder::isMultiBilayer object: we take into
	// account the cases in which either monolayer is empty by reassiging
	// the head coordinates. First we divide up the total number of polymers
	// equally amongst the lamellae.

	isMultiBilayer bilayer1(m_PolymerTypes, m_UpperFractions,
						  m_X, m_Y, m_Z, m_BilayerCentres, m_Thickness,
						  m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
						  m_UpperTotals, m_LowerTotals, m_bLinearise);

	bilayer1.Assemble(riState);

	// Calculate the free space available between the bilayers and the walls, 
	// or the SimBox boundaries if no wall is present. Then use a CBuilder::isRandom object 
	// to fill it with the remaining polymers in the simulation. The excluded regions
	// are identified by their start and end coordinates, starting at the bottom 
	// face of the SimBox in the direction of the bilayer normal.

	// Store the polymer types that don't make up the bilayers

	zLongVector nonBilayerPolymerTypes;

	nonBilayerPolymerTypes.clear();

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		if( find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) == m_PolymerTypes.end())
		{
			nonBilayerPolymerTypes.push_back(ip);
		}	
	}

	zDoubleVector exRegions;

	exRegions.push_back(0.0);

	// Check for a wall and exclude the wall regions if present: note that
	// we include the wall regions, giving them a thickness of zero, even
	// if there is no wall because the number of free regions is counted
	// assuming the wall is present. Each lamella contributes two excluded
	// regions to the box.

	if(m_X == 1)
	{
		exRegions.push_back(riState.GetWallXWidth());

		for(long unsigned int lam=0; lam<m_BilayerCentres.size(); lam++)
		{
			exRegions.push_back(m_BilayerCentres.at(lam) - 0.5*m_Thickness);
			exRegions.push_back(m_BilayerCentres.at(lam) + 0.5*m_Thickness);
		}

		exRegions.push_back(riState.GetSimBoxXLength() - riState.GetWallXWidth());
		exRegions.push_back(riState.GetSimBoxXLength());
	}
	else if(m_Y == 1)
	{
		exRegions.push_back(riState.GetWallYWidth());

		for(long unsigned int lam=0; lam<m_BilayerCentres.size(); lam++)
		{
			exRegions.push_back(m_BilayerCentres.at(lam) - 0.5*m_Thickness);
			exRegions.push_back(m_BilayerCentres.at(lam) + 0.5*m_Thickness);
		}

		exRegions.push_back(riState.GetSimBoxYLength() - riState.GetWallYWidth());
		exRegions.push_back(riState.GetSimBoxYLength());
	}
	else if(m_Z == 1)
	{
		exRegions.push_back(riState.GetWallZWidth());

		for(long unsigned int lam=0; lam<m_BilayerCentres.size(); lam++)
		{
			exRegions.push_back(m_BilayerCentres.at(lam) - 0.5*m_Thickness);
			exRegions.push_back(m_BilayerCentres.at(lam) + 0.5*m_Thickness);
		}

		exRegions.push_back(riState.GetSimBoxZLength() - riState.GetWallZWidth());
		exRegions.push_back(riState.GetSimBoxZLength());
	}

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
