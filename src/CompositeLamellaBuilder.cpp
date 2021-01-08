/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CompositeLamellaBuilder.cpp: implementation of the CCompositeLamellaBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"

#include "CompositeLamellaBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"
#include "PolymerCrossLink.h"

#include "CNTCell.h"			// needed for random number generator



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompositeLamellaBuilder::CCompositeLamellaBuilder(const zLongVector& polymerTypes, 
							 const zDoubleVector& upperFractions,
							 const PolymerCrossLinkVector& crossLinks,
							 long x, long y, long z, double centre, double thickness, 
							 const bool bPatches[2], bool bPolymerise, 
							 double area, double l1, double l2, double bilayercentre, 
							 double upperHead, double lowerHead, bool bLinear) : m_PolymerTypes(polymerTypes),
											m_UpperFractions(upperFractions),
											m_CrossLinks(crossLinks),
											m_X(x), m_Y(y), m_Z(z),
											m_Centre(centre),
											m_Thickness(thickness),
											m_bLinearise(bLinear),
											m_bPolymerise(bPolymerise),
											m_BilayerArea(area),
											m_Bilayer1Length(l1),
											m_Bilayer2Length(l2),
											m_BilayerCentre(bilayercentre),
											m_UpperHead(upperHead),
											m_LowerHead(lowerHead),
											m_bMonolayer(false),
											m_PolymerTotal(0),
											m_UpperTotal(0),
											m_LowerTotal(0)
{
	m_bPatches[0] = bPatches[0];
	m_bPatches[1] = bPatches[1];	
}

CCompositeLamellaBuilder::~CCompositeLamellaBuilder()
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

bool CCompositeLamellaBuilder::Assemble(CInitialState& riState)
{
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

	for(long unsigned int iPolymer=0; iPolymer<m_PolymerTypes.size(); iPolymer++)
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
		m_UpperHead		= m_BilayerCentre;
	}
	else if(m_LowerTotal == 0)
	{
		m_bMonolayer	= true;
		m_LowerHead		= m_BilayerCentre;
	}
	else
	{
		m_bMonolayer	= false;
	}

	// Check the contents of the cross-links

/*	for(cPolymerCrossLinkIterator iterLink=m_CrossLinks.begin(); iterLink!=m_CrossLinks.end(); iterLink++)
	{
		const zString name	= (*iterLink)->GetName();
		double fraction		= (*iterLink)->GetFraction();
		long position		= (*iterLink)->GetPosition();
		long type			= (*iterLink)->GetType();
		double spring		= (*iterLink)->GetSpringConstant();
		double length		= (*iterLink)->GetUnstretchedLength();
	}
*/

	// Create a bilayer

	isBilayer bilayer(m_PolymerTypes, m_UpperFractions,
					  m_X, m_Y, m_Z, m_BilayerCentre, m_Thickness,
					  m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
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

	for(long int iPolymer=0; iPolymer<riState.GetPolymerTypeTotal(); iPolymer++)
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

// Virtual function to assemble beads into a lamella initial configuration for
// a parallel simulation. It differs from the above serial function in that 
// each processor has to position beads within its own space. Depending on the 
// number of processors assigned to each dimension, and the location of the lamella,
// not all processors need to assemble membrane polymers; some of them will only
// need to position the solvent polymers and any solute polymers. We check to see if the
// owning processor has any lamella polymers before we try and arrange them in the bilayer configuration.
//
// Note that the centre, upper and lower planes of the membrane are defined with respect to the whole Space not
// a single SimBox, so we have to correct for this when assigning the heads and tails of the molecules to the membrane.
//
// Note also that we do not allow the parallel lamella state to be polymerised.

bool CCompositeLamellaBuilder::AssembleP(CInitialState& riState)
{
    bool bSuccess = true;
	
#if EnableParallelSimBox == SimMPSEnabled
	// Check if all polymers are in one monolayer. We just see if the sum 
	// of all the polymers in either monolayer is zero. This is obtained from
	// the fractions of each polymer type and the number thereof. We also store
	// the number of polymers of all types in each monolayer for use in the
	// isBilayer constructor.

	m_PolymerTotal	= 0;
	m_UpperTotal	= 0;
	m_LowerTotal	= 0;

	for(long unsigned int iPolymer=0; iPolymer<m_PolymerTypes.size(); iPolymer++)
	{
		long polyNo = riState.GetPolymerTotalForType(m_PolymerTypes.at(iPolymer));
		long upperNo = static_cast<long>(m_UpperFractions.at(iPolymer)*polyNo);
		
		m_PolymerTotal	+= polyNo;
		m_UpperTotal	+= upperNo;
		m_LowerTotal	+= (polyNo - upperNo);
	}

	// If the bilayer passes through the current SimBox, we position the polymers into a bilayer: note that it is hard-wired for a Z normal at the moment.
	
	if(m_PolymerTotal > 0)
	{		
	// If either monolayer is empty recalculate the free space outside the
	// bilayer to be filled with non-bilayer polymers.

	if(m_UpperTotal == 0)			// Upper monolayer empty
	{
		m_bMonolayer	= true;
		m_UpperHead		= m_BilayerCentre;
	}
	else if(m_LowerTotal == 0)
	{
		m_bMonolayer	= true;
		m_LowerHead		= m_BilayerCentre;
	}
	else
	{
		m_bMonolayer	= false;
				
		while (m_BilayerCentre > 0.0)
		{
		    m_BilayerCentre -= riState.GetSimBoxZLength();
		
		}
		m_BilayerCentre += riState.GetSimBoxZLength();
		m_UpperHead      = m_BilayerCentre + 0.5*m_Thickness;
		m_LowerHead      = m_BilayerCentre - 0.5*m_Thickness;
	}

	// Create a bilayer
	
	isBilayer bilayer(m_PolymerTypes, m_UpperFractions,
					  m_X, m_Y, m_Z, m_BilayerCentre, m_Thickness,
					  m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
					  m_UpperHead, m_LowerHead, m_UpperTotal, m_LowerTotal,
					  m_bMonolayer, m_bLinearise, m_bPatches);

	bilayer.Assemble(riState);

	// Calculate the free space available between the membrane and the simulation box boundary
	// (Note that we do not allows a wall in the parallel IS), and
	// use an isRandom object to fill it with the remaining polymers in the
	// simulation. We pass a vector of the coordinates of the excluded regions
	// so that the polymers can be assigned coordinates in the free regions.
	// We start the measurements at the lower box face in the direction of the 
	// bilayer normal.

	// Store the polymer types that don't make up the bilayer

	zLongVector nonBilayerPolymerTypes;

	nonBilayerPolymerTypes.clear();

	for(long int iPolymer=0; iPolymer<riState.GetPolymerTypeTotal(); iPolymer++)
	{
		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), iPolymer) == m_PolymerTypes.end())
		{
			nonBilayerPolymerTypes.push_back(iPolymer);
		}	
	}

//    std::cout << "********************" << zEndl;
//	std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " starting to assemble " << m_PolymerTotal << " polymers into a lamella structure" << zEndl;
//	std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " Data are " << m_PolymerTypes.size() << " " << m_X << " " << m_Y << " " << m_Z << " " << m_Centre << " " << m_Thickness << " " << m_bLinearise << " " << m_UpperFractions.size() << zEndl;
//	std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " Lamella data: " << m_Bilayer1Length << " " << m_Bilayer2Length << " " << m_BilayerArea << " " << m_BilayerCentre << " " << m_LowerHead << " " << m_UpperHead << zEndl;
//    std::cout << "********************" << zEndl;

	// Specify the bilayer volume as an excluded region so that no solvent beads are positioned within it - note no Walls are allowed here
	// but we have to enter values for the (non-existent) excluded regions as the original algorithm used them.
	
	zDoubleVector exRegions;
	exRegions.clear();

	if(m_X == 1)
	{
	    exRegions.push_back(0.0);
	    exRegions.push_back(0.0);
	    exRegions.push_back(m_LowerHead);
	    exRegions.push_back(m_UpperHead);
	    exRegions.push_back(riState.GetSimBoxXLength());
	    exRegions.push_back(riState.GetSimBoxXLength());
	}
	else if(m_Y == 1)
	{
	    exRegions.push_back(0.0);
	    exRegions.push_back(0.0);
	    exRegions.push_back(m_LowerHead);
	    exRegions.push_back(m_UpperHead);
	    exRegions.push_back(riState.GetSimBoxYLength());
	    exRegions.push_back(riState.GetSimBoxYLength());
	}
	else if(m_Z == 1)
	{
	    exRegions.push_back(0.0);
	    exRegions.push_back(0.0);
	    exRegions.push_back(m_LowerHead);
	    exRegions.push_back(m_UpperHead);
	    exRegions.push_back(riState.GetSimBoxZLength());
	    exRegions.push_back(riState.GetSimBoxZLength());
	}
		
	if(!AssembleRandomP(riState, exRegions))
	{
	    bSuccess = false;
	    std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " error - unable to fill empty regions with non-bilayer polymers" << zEndl;
	}

	}
	else    // Branch for processors not containing any part of the bilayer
	{
	    zDoubleVector exRegions;
	    exRegions.clear();

	    if(m_X == 1)
	    {
	        exRegions.push_back(0.0);
	        exRegions.push_back(0.0);
	        exRegions.push_back(riState.GetSimBoxXLength());
	        exRegions.push_back(riState.GetSimBoxXLength());
    	}
	    else if(m_Y == 1)
	    {
	        exRegions.push_back(0.0);
	        exRegions.push_back(0.0);
	        exRegions.push_back(riState.GetSimBoxYLength());
	        exRegions.push_back(riState.GetSimBoxYLength());
	    }
	    else if(m_Z == 1)
	    {
	        exRegions.push_back(0.0);
	        exRegions.push_back(0.0);
	        exRegions.push_back(riState.GetSimBoxZLength());
	        exRegions.push_back(riState.GetSimBoxZLength());
	    }
	
	    if(!AssembleRandomP(riState, exRegions))
	    {
	        bSuccess = false;
	        std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " error - unable to fill empty SimBox with polymers" << zEndl;
	    }
	}

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


#endif

    return bSuccess;
}

// Protected function to fill up empty regions of the SimBox with the remaining polymers left over once the bilayer has been assembled.
// Any regions that should be excluded from receiving polymers are specified as an adjacent pair of values in the exRegions argument along
// the given axis.

bool CCompositeLamellaBuilder::AssembleRandomP(CInitialState& riState, const zDoubleVector exRegions)
{
    bool bSuccess = true;
	
#if EnableParallelSimBox == SimMPSEnabled

//    std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " starting to fill up the empty spaces with solvent: total polymer types is " << riState.GetPolymerTypeTotal() << " exRegions size is " << exRegions.size() << zEndl;

	zLongVector nonBilayerPolymerTypes;

	nonBilayerPolymerTypes.clear();


	for(long polyType=0; polyType<riState.GetPolymerTypeTotal(); polyType++)
	{
		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), polyType) == m_PolymerTypes.end())
		{
//			std::cout << "Nonbilayer types adding " << polyType << zEndl;
			nonBilayerPolymerTypes.push_back(polyType);
		}	
	}

//    std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " found " << nonBilayerPolymerTypes.size() << " non-bilayer polymer types" << zEndl;
	
	PolymerVector vAllPolymers = riState.GetPolymers();  // All polymers in SimBox
	
	PolymerVector vRandomPolymers;			// Polymers used to fill non-bilayer regions
	vRandomPolymers.clear();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		lastPolymer += riState.GetPolymerTotalForType(ip);

		if(find(nonBilayerPolymerTypes.begin(), nonBilayerPolymerTypes.end(), ip) != nonBilayerPolymerTypes.end())
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(vAllPolymers.begin()+firstPolymer, vAllPolymers.begin()+lastPolymer, back_inserter(vRandomPolymers));
//			std::cout <<  "Proc " << xxParallelBase::GlobalGetRank() << " has added polymers of type " << ip << " to the vRandomPolymers container, that now has " << vRandomPolymers.size() << " polymers" << zEndl;
			
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}
	
//	std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " has " << vAllPolymers.size() << " total polymers, of which " << vRandomPolymers.size() << " are solvent" << zEndl;

	// Randomly position the beads in all non-bilayer polymers in the empty regions
	
	double xp[3];				// Coordinates to assign to a bead
	xp[0]        = 0.0;
	xp[1]        = 0.0;
	xp[2]        = 0.0;
	long index   = 0;			// counter used several times below
	double width = 0.0;			// region width used several times below
	double freeLength = 0.0;	// total width of all free regions

	// Get the total length of the free regions. Note that the size of the
	// exRegions container must be even.

	for(index=1; index<exRegions.size()/2; index++)
	{
		width = exRegions.at(2*index) - exRegions.at(2*index-1);
		freeLength += width;
	}

	zDoubleVector freeRegionRatio;	// Cumulative occupation probability for each region

	double cumulativeProb = 0.0;

	for(index=1; index<exRegions.size()/2; index++)
	{
		width = exRegions.at(2*index) - exRegions.at(2*index-1);

		const double prob = width/freeLength;

		cumulativeProb += prob;

		freeRegionRatio.push_back(cumulativeProb);
	}
	
//    std::cout <<  "Proc " << xxParallelBase::GlobalGetRank() << " has freeLength = " << freeLength << " out of a Z length of " << riState.GetSimBoxZLength() << " and freeRegionRatio has size " << freeRegionRatio.size() << zEndl;


	// Assign the beads to the free regions in proportion to the widths of 
	// the regions. This results in an approximately uniform density.
	// All beads in a given polymer must be in the same free region and not
	// straddle the membrane. I moved the selection of the actual free region into
    // the outer polymer loop not the inner bead loop as the latter only works for
    // polymers containing a single bead.

	double rindex     = 0.0;
	bool bNotAssigned = true;

	for(cPolymerVectorIterator iterPoly=vRandomPolymers.begin(); iterPoly!=vRandomPolymers.end(); iterPoly++)
	{
			rindex = CCNTCell::GetRandomNo();
			bNotAssigned = true;

			for(long nindex=0; nindex<freeRegionRatio.size(); nindex++)
			{
				if(bNotAssigned && rindex <= freeRegionRatio.at(nindex))
				{
					bNotAssigned = false;
					index = nindex;
				}
			}

			if(bNotAssigned)
				index = 0;

			width = exRegions.at(2*index+2) - exRegions.at(2*index+1);
						
			for(cBeadVectorIterator iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
				if(m_X == 1)
				{
					xp[0] = exRegions.at(2*index+1) + width*CCNTCell::GetRandomNo();

					xp[1] = 0.95*riState.GetSimBoxYLength()*CCNTCell::GetRandomNo();
					xp[2] = 0.95*riState.GetSimBoxZLength()*CCNTCell::GetRandomNo();
				}
				else if(m_Y == 1)
				{
					xp[0] = 0.95*riState.GetSimBoxXLength()*CCNTCell::GetRandomNo();

					xp[1] = exRegions.at(2*index+1) + width*CCNTCell::GetRandomNo();

					xp[2] = 0.95*riState.GetSimBoxZLength()*CCNTCell::GetRandomNo();
				}
				else if(m_Z == 1)
				{
					xp[0] = 0.95*riState.GetSimBoxXLength()*CCNTCell::GetRandomNo();
					xp[1] = 0.95*riState.GetSimBoxYLength()*CCNTCell::GetRandomNo();
						
					xp[2] = exRegions.at(2*index+1) + width*CCNTCell::GetRandomNo();
				}

				(*iterBead)->SetXPos(xp[0]);
				(*iterBead)->SetYPos(xp[1]);
				(*iterBead)->SetZPos(xp[2]);
			}

			// Check that pairs of beads that are bonded together are not too far apart 
			// that they generate huge bond forces. This arises because their positions 
			// are just randomly selected from the free space and for boxes of 
			// 20 x 20 x 20 or more this can lead to large separations. If they are 
			// further apart than "rbond", move them along the line joining their centres 
			// so that they are only unit distance apart.

			const double rbond = 1.0;
			double rmag;
			double rdiff[3], runit[3];

			for(cBondVectorIterator iterBond=(*iterPoly)->GetBonds().begin(); iterBond!= (*iterPoly)->GetBonds().end(); iterBond++)
			{
				rdiff[0] = (*iterBond)->GetTail()->GetXPos() - (*iterBond)->GetHead()->GetXPos();
				rdiff[1] = (*iterBond)->GetTail()->GetYPos() - (*iterBond)->GetHead()->GetYPos();
				rdiff[2] = (*iterBond)->GetTail()->GetZPos() - (*iterBond)->GetHead()->GetZPos();

				rmag = sqrt(rdiff[0]*rdiff[0] + rdiff[1]*rdiff[1] + rdiff[2]*rdiff[2]);

				runit[0] = rdiff[0]/rmag;
				runit[1] = rdiff[1]/rmag;
				runit[2] = rdiff[2]/rmag;

				if( fabs(rdiff[0]) > rbond )
				{
					(*iterBond)->GetTail()->SetXPos((*iterBond)->GetHead()->GetXPos()+runit[0]);
				}

				if( fabs(rdiff[1]) > rbond )
				{
					(*iterBond)->GetTail()->SetYPos((*iterBond)->GetHead()->GetYPos()+runit[1]);
				}

				if( fabs(rdiff[2]) > rbond )
				{
					(*iterBond)->GetTail()->SetZPos((*iterBond)->GetHead()->GetZPos()+runit[2]);
				}
			}
		}

#endif
 
    return bSuccess;
}




