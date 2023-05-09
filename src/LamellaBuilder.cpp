/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LamellaBuilder.cpp: implementation of the CLamellaBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimStdlibFlags.h"
#include "LamellaBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"

#include "CNTCell.h"			// needed for random number generator
#include "xxParallelBase.h"     // needed to get processor rank

#include "LogState.h"
#include "LogBuilderError.h"	// needed to log errors assembling the initial state

#include <random>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLamellaBuilder::CLamellaBuilder(long polymer, long x, long y, long z, double centre, double thickness, 
								 bool blinear, double upperFraction,
								 bool bPolymerise, long polyType, double polyFraction, long polyPosition,
								 double springConstant, double unstretchedLength,
								 double area, double l1, double l2, double bilayercentre, 
								 double lowerHead, double upperHead) : m_PolymerType(polymer),
																	  m_X(x), m_Y(y), m_Z(z),
																	  m_Centre(centre),
																	  m_Thickness(thickness),
																	  m_bLinearise(blinear),
																	  m_UpperFraction(upperFraction),
																	  m_bPolymerise(bPolymerise),
																	  m_PolymerisedType(polyType),
																	  m_PolymerisedFraction(polyFraction),
																	  m_PolymerisedPosition(polyPosition),
																	  m_SpringConstant(springConstant),
																	  m_UnstretchedLength(unstretchedLength),
																	  m_BilayerArea(area),
																	  m_Bilayer1Length(l1),
																	  m_Bilayer2Length(l2),
																	  m_BilayerCentre(bilayercentre),
																	  m_LowerHead(lowerHead),
																	  m_UpperHead(upperHead),
																	  m_PolymerTotal(0),
																	  m_PolymerSize(0),
																	  m_LowerFreeLength(0.0),
																	  m_UpperFreeLength(0.0),
																	  m_bMonolayer(false)
{
	m_vBilayer.clear();
	m_vPolymerisedUpperPolymers.clear();
	m_vPolymerisedLowerPolymers.clear();
	m_vPolymerisedBonds.clear();

	for(short i=0; i<2; i++)
	{
		m_MonolayerTotal[i]		= 0;
		m_MonolayerFirstPolymer[i]	= 0;
		m_LatticeSize[i]		= 0;		// No of vertices in monolayer lattices	
		m_MonolayerSign[i]		= 0.0;
		m_MonolayerHead[i]		= 0.0;
		m_HexSpace[i]			= 0.0;		// Hexagonal lattice spacings
		m_HexCAxisSpace[i]		= 0.0;		// Bead spacing in lattice normal direction
		m_Hexe1X[i]			= 0.0;
		m_Hexe1Y[i]			= 0.0;		// Components of hexagonal lattice vectors
		m_Hexe2X[i]			= 0.0;
		m_Hexe2Y[i]			= 0.0;
	}
	

}

CLamellaBuilder::~CLamellaBuilder()
{

}

// Virtual function to assemble beads into a lamella initial configuration.
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
// size of the SimBox.
//
// The tail bead of each lamella polymer is position in the mid-plane and the
// head beads are fixed at a given distance above and below this plane. The
// other beads in the polymers are then randomly positioned about the head-tail
// axis. This is probably not an equilibrium organisation but it is the simplest
// that we can do.
//
// If the initial lamella is polymerised (as indicated by m_bPolymerise) we create
// new bonds with the next available bond type and connect the specified beads
// in the polymer (m_PolymerisedPosition) by Hookean springs with given spring
// constant and unstretched length (m_SpringConstant, m_UnstretchedLength). The 
// fraction of polymers connected by such bonds is specified by m_PolymerisedFraction.
// To be precise, this is the fraction of bonds that are created to connect the
// polymers.

bool CLamellaBuilder::Assemble(CInitialState& riState)
{
	// ****************************************
	// Position the wall beads first because they cannot move and may be 
	// bonded to beads in the bulk whose positions can be arranged so 
	// that their bonds are not too large initially.

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}

	// Copy the polymers in the CInitialState into local vectors for ease of access.
	// First we count the number of polymers preceeding the ones we want using 
	// their type to cycle through the GetPolymerTotalForType() function. Once we
	// have the start of the polymers we are interested in, and we know how many
	// there are, we can copy that part of the polymer vector into a local
	// vector.

	long firstPolymer = 0;

	// does the loop execute if m_PolymerType =0?

	for(long ip=0; ip<m_PolymerType; ip++)
	{
		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

    m_PolymerTotal = riState.GetPolymerTotalForType(m_PolymerType);

	long lastPolymer = firstPolymer + m_PolymerTotal;

	// Copy the polymer pointers for the required polymers into local storage.
	// We use a back_inserter to avoid having to manually resize the container.

	PolymerVector bilayerPolymers = riState.GetPolymers();

	copy(bilayerPolymers.begin()+firstPolymer, bilayerPolymers.begin()+lastPolymer, back_inserter(m_vBilayer));

	// Get the number of beads in all polymers in the bilayer. Note that the
	// size of these polymers includes the head and tail. Wall polymers would
	// fail here because their head is not counted in their size. Because
	// there is only one kind of polymer used in the bilayer we don' have to check
	// which type comes first.

	m_PolymerSize = m_vBilayer.at(0)->GetSize();

	// ****************************************
	// Calculate the number of polymers in each monolayer from the fraction in the
	// upper one. We have the correspdonence
	//
	//	0 = upper monolayer
	//	1 = lower monolayer

	m_MonolayerSign[0] =  1.0;	// Sign needed to assign correct coordinates to beads
	m_MonolayerSign[1] = -1.0;	// in polymers in the upper and lower monolayers

	m_MonolayerTotal[0] = static_cast<long>(m_UpperFraction*m_PolymerTotal);
	m_MonolayerTotal[1] = m_PolymerTotal - m_MonolayerTotal[0];

	// Store a flag showing if the user has specified an asymmetry parameter
	// of 1 or 0, indicating that all the polymers are in one monolayer. Then
	// set the lattice size from the number of polymers. If all the polymers
	// are in one monolayer, we adjust the m_UpperHead, m_LowerHead parameters
	// to reflect this.

	if(m_MonolayerTotal[0] == 0)	// All polymers in lower monolayer	
	{
		m_bMonolayer				= true;
		m_MonolayerFirstPolymer[0]	= 0;
		m_MonolayerFirstPolymer[1]	= 0;
		m_LatticeSize[0]			= 0;
		m_LatticeSize[1]			= m_MonolayerTotal[1];			
		m_MonolayerHead[0]			= 0.0;
		m_MonolayerHead[1]			= m_LowerHead;
		m_UpperHead					= m_BilayerCentre;
	}
	else if (m_MonolayerTotal[1] == 0)	// All polymers in upper monolayer
	{
		m_bMonolayer				= true;
		m_MonolayerFirstPolymer[0]	= 0;
		m_MonolayerFirstPolymer[1]	= 0;
		m_LatticeSize[0]			= m_MonolayerTotal[0];		
		m_LatticeSize[1]			= 0;
		m_MonolayerHead[0]			= m_UpperHead;
		m_MonolayerHead[1]			= 0.0;
		m_LowerHead					= m_BilayerCentre;
	}
	else
	{
		m_bMonolayer				= false;
		m_MonolayerFirstPolymer[0]	= 0;
		m_MonolayerFirstPolymer[1]	= m_MonolayerTotal[0];
		m_LatticeSize[0]			= m_MonolayerTotal[0];		
		m_LatticeSize[1]			= m_MonolayerTotal[1];
		m_MonolayerHead[0]			= m_UpperHead;
		m_MonolayerHead[1]			= m_LowerHead;
	}

	// ****************************************
	// Calculate the free space available between the membrane and the wall.
	// If no wall exists the functions just return 0 for its width.
	// If all the polymers are in one monolayer, we adjust the free space
	// to the centre of the "bilayer".

	if(m_X == 1)
	{
		m_LowerFreeLength = m_LowerHead - riState.GetWallXWidth();
		m_UpperFreeLength = riState.GetSimBoxXLength() - m_UpperHead - riState.GetWallXWidth();
	}
	else if(m_Y == 1)
	{
		m_LowerFreeLength = m_LowerHead - riState.GetWallYWidth();
		m_UpperFreeLength = riState.GetSimBoxYLength() - m_UpperHead - riState.GetWallYWidth();
	}
	else if(m_Z == 1)
	{
		m_LowerFreeLength = m_LowerHead - riState.GetWallZWidth();
		m_UpperFreeLength = riState.GetSimBoxZLength() - m_UpperHead - riState.GetWallZWidth();
	}
	// ****************************************
	// Calculate the lattice size for a hexagonal lattice that organises
	// the polymers in each monolayer. We have to take into account 
	// the walls, the orientation of the bilayer and the number of 
	// polymers in each  monolayer to calculate the spacing.
	// Get unit vectors in e1 and e2 directions (NB. 1.1547 = 2/sqrt(3)) and the
	// number of vertices in the lattice.
	// We shrink the lattice vectors by 0.999 and offset the first sites by 0.001 to
	// prevent any sites going outside the SimBox. This occurred with a box size
	// of 20**3 in the test run 11024, changelist 462.

	if(m_bMonolayer)
	{
		if(m_MonolayerTotal[0] == 0)	// All in lower monolayer
		{
			m_HexSpace[0]	= 0.0;
			m_HexSpace[1]	= 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[1]));
		}
		else
		{
			m_HexSpace[0]	= 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[0]));
			m_HexSpace[1]	= 0.0;
		}
	}
	else
	{
		m_HexSpace[0]	= 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[0]));
		m_HexSpace[1]	= 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[1]));
	}

	m_Hexe1X[0]		= m_HexSpace[0];			// e1 lattice vector
	m_Hexe1Y[0]		= 0.0;
	m_Hexe2X[0]		= 0.5*m_HexSpace[0];		// e2 lattice vector
	m_Hexe2Y[0]		= 0.866025*m_HexSpace[0];

	m_Hexe1X[1]		= m_HexSpace[1];			// e1 lattice vector
	m_Hexe1Y[1]		= 0.0;
	m_Hexe2X[1]		= 0.5*m_HexSpace[1];		// e2 lattice vector
	m_Hexe2Y[1]		= 0.866025*m_HexSpace[1];
	
	// For linear and branched polymers we count the number of beads between 
	// the head and tail (inclusive) and use this to define the spacing in 
	// the C direction. The spacing is defined as the monolayer thickness
	// divided by m_PolymerSize-1. 
	// We don't know whether the head or tail will be encountered first in the 
	// polymer string so we set a flag, bCount, to indicate when we should count
	// beads. It is reset to null when both head and tail have been counted.
	// The other beads in the polymer are arranged randomly around the head-tail
	// axis.
	//
	// NOTE. This algorithm won't work if the tail bead is found first in the
	// polymer shape string. It will arrange the beads as if the head were first,
	// resulting in the beads between the head and tail having their order
	// reversed.

	const BeadVector&  vBeads = m_vBilayer.at(0)->GetBeads();
	const CBead*		pcHead = dynamic_cast<CBead*>(m_vBilayer.at(0)->GetHead());
	const CBead*		pcTail = dynamic_cast<CBead*>(m_vBilayer.at(0)->GetTail());

	const CBead*		 pEnd = NULL;
	long counter			  = 0;
	bool bCount				  = false;

	long dummyCount = 0;

	for(cBeadVectorIterator pBead=vBeads.begin();pBead!=vBeads.end(); pBead++)
	{
		dummyCount++;

		if(*pBead == pcHead && !bCount)
		{
			bCount = true;
			pEnd   = pcTail;
			counter++;
		}
		else if(*pBead == pcTail && !bCount)
		{
			bCount = true;
			pEnd   = pcHead;
			counter++;
		}
		else if(*pBead == pEnd)
		{
			bCount = false;
			counter++;
		}
		else if(bCount)
		{
			counter++;
		}
	}

	m_HexCAxisSpace[0] = 0.5*m_Thickness/static_cast<double>(counter-1);
	m_HexCAxisSpace[1] = m_HexCAxisSpace[0];

	// ****************************************
	// Loop over the number of vertices in the lattices. If all the polymers are
	// in one monolayer we just loop once; if both monolayers are needed the
	// loop executes twice. We have to take care with the orientation of the 
	// polymers in each monolayer and use the m_MonolayerSign[] variable to
	// adjust the coordinate in the direction of the bilayer normal when setting
	// bead coordinates. Polymer head coordinates are positioned
	// directly above (or below) the tails using the m_Thickness parameter.
	//
	// The hexp[] coordinates are those of the vertices in the hexagonal
	// lattice used to site the polymers. They are copied to xp[] so that
	// we can translate the coordinates for those polymers that cross the 
	// SimBox boundaries without affecting the lattice iteration. The hexp[]
	// coordinates are translated only when they cross the hexagonal lattice
	// boundaries and this starts a new row in the lattice. Translating the
	// xp[] coordinates for a polymer does not start a new row, it just moves
	// the polymer back within the SimBox.
	//
	// Note that the xp[] coordinates are without regard to the bilayer 
	// orientation, that is, they are in-plane coordinates. We convert them 
	// to X,Y,Z coordinates later. Also, we set the copies of the bead coordinates
	// using SetunPBCXPos(), SetInitialXPos() later.

	double hexp[3];		// Hexagonal lattice coords
	double xp[3];		// Head bead coords
	double xp1[3];		// Other polymer beads coords

	// Store the length of the rows in the hexagonal lattices for each monolayer.
	// We have to store them separately because there may be different numbers of
	// polymers in each monolayer depending on how the user specified the fractions.

	long rowLength[2];

	// Use a local counter to keep track of the number of polymerised bonds created 
	// so that we can set their ids appropriately. We start counting at the number
	// of bonds already created.

	long polyBondCounter = riState.GetBondTotal();

	// Create a template bond for use in creating all polymerised bonds.
	// We get the current number of bond types from the CInitialState
	// object and create a new type to be used as a template for all
	// polymerised bonds. The template is stored in the CInitialState
	// just as the normal bond types.

	const long polyBondType = riState.GetBondTypeTotal();

	CBond* pPolyBondTemplate = NULL;

	if(m_bPolymerise)
	{
		riState.SetPolymerisedState(true);
		pPolyBondTemplate = new CBond(polyBondType, m_SpringConstant, m_UnstretchedLength);
		riState.AddBondType(pPolyBondTemplate);
	}

	for(short monolayerIndex = 0; monolayerIndex < 2; monolayerIndex++)
	{
		hexp[0] = 0.001*m_Hexe1X[monolayerIndex];	// We use an offset to avoid positions coinciding with boundaries
		hexp[1] = 0.001*m_Hexe2Y[monolayerIndex];
		hexp[2] = 0.0;

		xp[0]	= hexp[0];
		xp[1]	= hexp[1];
		xp[2]	= hexp[2];

		xp1[0]	= 0.0;
		xp1[1]	= 0.0;
		xp1[2]	= 0.0;

		rowLength[monolayerIndex] = 0;

		long siteCounter = 0;			// Used to check no of sites per row
		long rowCounter  = 0;			// Used to set coordinates of first site in each row

		for(long iv=m_MonolayerFirstPolymer[monolayerIndex]; iv<m_MonolayerFirstPolymer[monolayerIndex]+m_LatticeSize[monolayerIndex]; iv++)
		{
			// We still use the variable upperPoly to avoid having to change its name

			CPolymer* upperPoly = m_vBilayer.at(iv);

			// If the initial state is polymerised, store the polymer pointers in
			// vectors so that we can attach them by newly-created bonds to 
			// their neighbours later. We also store counters showing what the 
			// (i,j) coordinates of the polymers are for use in assigning bonds.

			if(m_bPolymerise)
			{
				if(monolayerIndex == 0)
				{
					m_vPolymerisedUpperPolymers.push_back(upperPoly);

				}
				else
				{
					m_vPolymerisedLowerPolymers.push_back(upperPoly);
				}
			}

			// Position the beads in the polymers around the head and tail axis.
			// We have to take account of the orientation of the bilayer now.

			if(m_X == 1)
			{
				upperPoly->GetTail()->SetXPos(m_BilayerCentre + m_MonolayerSign[monolayerIndex]*0.2);
				upperPoly->GetTail()->SetYPos(xp[0]);	// shift tails slightly to avoid
				upperPoly->GetTail()->SetZPos(xp[1]);	// creating large forces

				upperPoly->GetHead()->SetXPos(m_MonolayerHead[monolayerIndex]);
				upperPoly->GetHead()->SetYPos(xp[0]);
				upperPoly->GetHead()->SetZPos(xp[1]);

				// Assign coordinates to other beads in the polymers wrapping them around
				// the head-tail axis. If the user has selected the linearise option then 
				// the beads are given coordinates on a straight line joining the polymer
				// head and tail, otherwise they are positioned somwhat randomly about the
				// head-tail axis. If the polymers are in addition linear, the initial 
				// configuration will consist of exactly straight polymers, but if the 
				// polymers are branched only the beads between the head and tail will
				// be straightened.  We check that the random
				// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
				// We don't have to check for their being less than 0 because we only
				// add a random component, and don't subtract it.

				double beadCounter = 1.0;

				for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
				{
					if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
					{
						if(m_bLinearise)
						{
							xp1[0] = upperPoly->GetHead()->GetXPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
							xp1[1] = xp[0];
							xp1[2] = xp[1];
							beadCounter += 1.0;
						}
						else
						{
							xp1[0] = m_BilayerCentre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();
							xp1[1] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();
							xp1[2] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();

							if(xp1[1] > riState.GetSimBoxYLength())
								xp1[1] -= 0.5*m_Hexe1X[monolayerIndex];

							if(xp1[2] > riState.GetSimBoxZLength())
								xp1[2] -= 0.5*m_Hexe2Y[monolayerIndex];
						}

						(*iterBead2)->SetXPos(xp1[0]);
						(*iterBead2)->SetYPos(xp1[1]);
						(*iterBead2)->SetZPos(xp1[2]);
					}
				}
			}
			else if(m_Y == 1)
			{
				upperPoly->GetTail()->SetXPos(xp[1]);	// shift tails slightly to avoid
				upperPoly->GetTail()->SetYPos(m_BilayerCentre + m_MonolayerSign[monolayerIndex]*0.2);
				upperPoly->GetTail()->SetZPos(xp[0]);	// creating large forces

				upperPoly->GetHead()->SetXPos(xp[1]);
				upperPoly->GetHead()->SetYPos(m_MonolayerHead[monolayerIndex]);
				upperPoly->GetHead()->SetZPos(xp[0]);

				// Assign coordinates to other beads in the polymers wrapping them around
				// the head-tail axis. If the user has selected the linearise option then 
				// the beads are given coordinates on a straight line joining the polymer
				// head and tail, otherwise they are positioned somwhat randomly about the
				// head-tail axis. If the polymers are in addition linear, the initial 
				// configuration will consist of exactly straight polymers, but if the 
				// polymers are branched only the beads between the head and tail will
				// be straightened.  We check that the random
				// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
				// We don't have to check for their being less than 0 because we only
				// add a random component, and don't subtract it.

				double beadCounter = 1.0;

				for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
				{
					if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
					{
						if(m_bLinearise)
						{
							xp1[0] = xp[1];
							xp1[1] = upperPoly->GetHead()->GetYPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
							xp1[2] = xp[0];
							beadCounter += 1.0;
						}
						else
						{
							xp1[0] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();
							xp1[1] = m_BilayerCentre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();
							xp1[2] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();

							if(xp1[0] > riState.GetSimBoxXLength())
								xp1[0] -= 0.5*m_Hexe2Y[monolayerIndex];

							if(xp1[2] > riState.GetSimBoxZLength())
								xp1[2] -= 0.5*m_Hexe1X[monolayerIndex];
						}

						(*iterBead2)->SetXPos(xp1[0]);
						(*iterBead2)->SetYPos(xp1[1]);
						(*iterBead2)->SetZPos(xp1[2]);
					}
				}
			}
			else if(m_Z == 1)
			{
				upperPoly->GetTail()->SetXPos(xp[0]);	// shift tails slightly to avoid
				upperPoly->GetTail()->SetYPos(xp[1]);	// creating large forces
				upperPoly->GetTail()->SetZPos(m_BilayerCentre + m_MonolayerSign[monolayerIndex]*0.2);

				upperPoly->GetHead()->SetXPos(xp[0]);
				upperPoly->GetHead()->SetYPos(xp[1]);
				upperPoly->GetHead()->SetZPos(m_MonolayerHead[monolayerIndex]);

				// Assign coordinates to other beads in the polymers wrapping them around
				// the head-tail axis. If the user has selected the linearise option then 
				// the beads are given coordinates on a straight line joining the polymer
				// head and tail, otherwise they are positioned somwhat randomly about the
				// head-tail axis. If the polymers are in addition linear, the initial 
				// configuration will consist of exactly straight polymers, but if the 
				// polymers are branched only the beads between the head and tail will
				// be straightened.  We check that the random
				// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
				// We don't have to check for their being less than 0 because we only
				// add a random component, and don't subtract it.

				double beadCounter = 1.0;

				for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
				{
					if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
					{
						if(m_bLinearise)
						{
							xp1[0] = xp[0];
							xp1[1] = xp[1];
							xp1[2] = upperPoly->GetHead()->GetZPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
							beadCounter += 1.0;
						}
						else
						{
							xp1[0] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();
							xp1[1] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();
							xp1[2] = m_BilayerCentre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();

							if(xp1[0] > riState.GetSimBoxXLength())
								xp1[0] -= 0.5*m_Hexe1X[monolayerIndex];

							if(xp1[1] > riState.GetSimBoxYLength())
								xp1[1] -= 0.5*m_Hexe2Y[monolayerIndex];
						}

						(*iterBead2)->SetXPos(xp1[0]);
						(*iterBead2)->SetYPos(xp1[1]);
						(*iterBead2)->SetZPos(xp1[2]);
					}
				}
			}

			// Update the coordinates for the next vertex. We make two checks:
			// if the next hexp[0] coordinate is greater than the hexagonal lattice 
			// length we translate to the next row, but if not, and it is 
			// greater than the SimBox width, we translate back in the same row.
			// We have to do the test for the hexagonal lattice first so that
			// the first row is correctly handled. Thw widths of both lattices
			// coincide here. Note that if we translate a vertex we increment
			// the y coordinate as well.

			siteCounter++;

			hexp[0] += m_Hexe1X[monolayerIndex];

			if(hexp[0] > m_Bilayer1Length + static_cast<double>(rowCounter)*m_Hexe2X[monolayerIndex])
			{
				rowCounter++;

				hexp[0] = static_cast<double>(rowCounter)*m_Hexe2X[monolayerIndex];
				hexp[1] += m_Hexe2Y[monolayerIndex];

				xp[0] = hexp[0];
				xp[1] = hexp[1];

				if(rowLength[monolayerIndex] == 0)
					rowLength[monolayerIndex] = siteCounter;

				// If we have run out of area it is an error so abort the run

				if(hexp[1] >= m_Bilayer2Length)
				{
					new CLogBuilderError(0, "lamella lattice outside SimBox");
					return false;
				}
			}
			else if(hexp[0] > m_Bilayer1Length)
			{
				xp[0] = hexp[0] - m_Bilayer1Length;
			}
			else
			{
				xp[0] = hexp[0];
				xp[1] = hexp[1];
			}
		}		// end of loop over polymers

		// If the initial state is polymerised we create bonds to bind together
		// adjacent polymers in each monolayer. We use the vector of polymers 
		// for the current monolayer to identify which polymers are neighbours
		// to each other in the hexagonal lattices defining each monolayer.
		// Because the user can specify different numbers of polymers in each
		// monolayer, these lattices may not have the same size. Hence we have to
		// calculate the row size, and polymer indices separately.
		// For each vertex in the lattice we create bonds to connect it to its
		// neighbours to the right, upper right and upper left. This ensures
		// that all are connected without duplication or missing any bonds.

		// The indices are calculated mod rowLength[monolayerIndex]. But after we
		// have calculated the indices using the whole lattice size, we can
		// adjust them to ignore those bonds that would cross the PBCs and 
		// those that would connect to vertices that do not contain a polymer. 
		// We do this by storing a flag that shows whether the neighbour vertex
		// was translated before creating the bonds.
		// If the bond is not to be created we give it an index -1. Then we 
		// check the indices before creating the new bonds.

		if(m_bPolymerise)
		{
			// Empty the vector of polymerised bonds for each monolayer.

			m_vPolymerisedBonds.clear();

			CPolymer*  centralPoly		= NULL;
			CPolymer*  rightPoly		= NULL;
			CPolymer*  upperRightPoly	= NULL;
			CPolymer*  upperLeftPoly	= NULL;

			long polymerIndex			= 0;
			long rightIndex				= 0;
			long upperRightIndex		= 0;
			long upperLeftIndex			= 0;

			long rowIndex = 0;

			for(long iv=m_MonolayerFirstPolymer[monolayerIndex]; iv<m_MonolayerFirstPolymer[monolayerIndex]+m_LatticeSize[monolayerIndex]; iv++)
			{
				// Now we have to check that there are actually polymers at each
				// vertex of the lattice. It may arise that some vertices are
				// empty because the user has specified a box size and area per
				// lipid that leads to the creation of too few polymers to cover
				// the lattice. If this occurs, the m_LatticeSize[] values will
				// be less than the number of vertices. 
				// We check that indices only connect adjacent vertices and not 
				// those that cross via the PBCs nor empty ones. Notice that the
				// checks on the upper right and left sites depend on whether the
				// row is even or odd.
				// The flags here indicate whether the bonds will cross
				// the boundaries of the hexagonal lattice not the SimBox.

				rightIndex = polymerIndex + 1;
				if(rightIndex == (rowIndex+1)*rowLength[monolayerIndex])
				{
					rightIndex -= rowLength[monolayerIndex];
				}
					
				upperRightIndex	= polymerIndex + rowLength[monolayerIndex];

				upperLeftIndex	= upperRightIndex - 1;
				if(upperLeftIndex == (rowIndex+1)*rowLength[monolayerIndex]-1)
				{
					upperLeftIndex += rowLength[monolayerIndex];
				}

				// Correct the top row indices for the lattice size including
				// the rightIndex in case some vertices are empty

				if(rightIndex >=m_LatticeSize[monolayerIndex])
				{
					rightIndex -= m_LatticeSize[monolayerIndex];
				}

				if(upperRightIndex >=m_LatticeSize[monolayerIndex])
				{
					upperRightIndex -= m_LatticeSize[monolayerIndex];
				}

				if(upperLeftIndex >= m_LatticeSize[monolayerIndex])
				{
					upperLeftIndex  -= m_LatticeSize[monolayerIndex];
				}

				// Unfortunately, the polymers are stored in vectors that cannot be
				// accessed using the monolayer index. So we have to use an if
				// statement to assign bonds to the two monolayers.
				// We only add polymerised bonds whose length is less than half
				// the SimBox size to avoid bonds crossing the
				// SimBox boundaries and having their length miscalculated
				// by the CBond::AddForce() method that uses unPBC coordinates
				// to calculate bond lengths. We use the CBond::CalculateLength()
				// method because the usual GetLength() depends on the unPBC 
				// coordinates and they have not been assigned yet (it happens at
				// the end of this routine).
				// If the bond is not added to the CInitialState's vAllPolymerisedBonds 
				// vector it must be deleted here to avoid a memory leak as the
				// CInitialState object is responsible for deleting polymerised bonds.

				if(monolayerIndex == 0)	// Upper monolayer
				{
					centralPoly		= m_vPolymerisedUpperPolymers.at(polymerIndex);
					CBead* pHead	=    (centralPoly->GetBeads()).at(m_PolymerisedPosition);

					rightPoly			= m_vPolymerisedUpperPolymers.at(rightIndex);
					CBead* pRightTail	= (rightPoly->GetBeads()).at(m_PolymerisedPosition);
					CBond* pRightBond	= new CBond(*pPolyBondTemplate);

					pRightBond->SetId(polyBondCounter++);
					pRightBond->SetBeads(pHead, pRightTail);
					pRightBond->SetBeadIndex(pHead->GetId(), pRightTail->GetId());

					if(pRightBond->CalculateLength() < 0.5*m_Bilayer1Length &&
					   pRightBond->CalculateLength() < 0.5*m_Bilayer2Length)
					{
						m_vPolymerisedBonds.push_back(pRightBond);		
					}
					else
						delete pRightBond;

					upperRightPoly			= m_vPolymerisedUpperPolymers.at(upperRightIndex);
					CBead* pUpperRightTail	= (upperRightPoly->GetBeads()).at(m_PolymerisedPosition);
					CBond* pUpperRightBond	= new CBond(*pPolyBondTemplate);

					pUpperRightBond->SetId(polyBondCounter++);
					pUpperRightBond->SetBeads(pHead, pUpperRightTail);
					pUpperRightBond->SetBeadIndex(pHead->GetId(), pUpperRightTail->GetId());

					if(pUpperRightBond->CalculateLength() < 0.5*m_Bilayer1Length &&
					   pUpperRightBond->CalculateLength() < 0.5*m_Bilayer2Length)
					{
						m_vPolymerisedBonds.push_back(pUpperRightBond);
					}
					else
						delete pUpperRightBond;

					upperLeftPoly			= m_vPolymerisedUpperPolymers.at(upperLeftIndex);
					CBead* pUpperLeftTail	= (upperLeftPoly->GetBeads()).at(m_PolymerisedPosition);
					CBond* pUpperLeftBond	= new CBond(*pPolyBondTemplate);

					pUpperLeftBond->SetId(polyBondCounter++);
					pUpperLeftBond->SetBeads(pHead, pUpperLeftTail);
					pUpperLeftBond->SetBeadIndex(pHead->GetId(), pUpperLeftTail->GetId());

					if(pUpperLeftBond->CalculateLength() < 0.5*m_Bilayer1Length &&
					   pUpperLeftBond->CalculateLength() < 0.5*m_Bilayer2Length)
					{
						m_vPolymerisedBonds.push_back(pUpperLeftBond);
					}
					else
						delete pUpperLeftBond;		
				}
				else	// Lower monolayer
				{
					centralPoly		= m_vPolymerisedLowerPolymers.at(polymerIndex);
					CBead* pHead	=    (centralPoly->GetBeads()).at(m_PolymerisedPosition);

					rightPoly			= m_vPolymerisedLowerPolymers.at(rightIndex);
					CBead* pRightTail	= (rightPoly->GetBeads()).at(m_PolymerisedPosition);
					CBond* pRightBond	= new CBond(*pPolyBondTemplate);

					pRightBond->SetId(polyBondCounter++);
					pRightBond->SetBeads(pHead, pRightTail);
					pRightBond->SetBeadIndex(pHead->GetId(), pRightTail->GetId());

					if(pRightBond->CalculateLength() < 0.5*m_Bilayer1Length &&
					   pRightBond->CalculateLength() < 0.5*m_Bilayer2Length)
					{
						m_vPolymerisedBonds.push_back(pRightBond);		
					}
					else
						delete pRightBond;

					upperRightPoly			= m_vPolymerisedLowerPolymers.at(upperRightIndex);
					CBead* pUpperRightTail	= (upperRightPoly->GetBeads()).at(m_PolymerisedPosition);
					CBond* pUpperRightBond	= new CBond(*pPolyBondTemplate);

					pUpperRightBond->SetId(polyBondCounter++);
					pUpperRightBond->SetBeads(pHead, pUpperRightTail);
					pUpperRightBond->SetBeadIndex(pHead->GetId(), pUpperRightTail->GetId());

					if(pUpperRightBond->CalculateLength() < 0.5*m_Bilayer1Length &&
					   pUpperRightBond->CalculateLength() < 0.5*m_Bilayer2Length)
					{
						m_vPolymerisedBonds.push_back(pUpperRightBond);
					}
					else
						delete pUpperRightBond;

					upperLeftPoly			= m_vPolymerisedLowerPolymers.at(upperLeftIndex);
					CBead* pUpperLeftTail	= (upperLeftPoly->GetBeads()).at(m_PolymerisedPosition);
					CBond* pUpperLeftBond	= new CBond(*pPolyBondTemplate);

					pUpperLeftBond->SetId(polyBondCounter++);
					pUpperLeftBond->SetBeads(pHead, pUpperLeftTail);
					pUpperLeftBond->SetBeadIndex(pHead->GetId(), pUpperLeftTail->GetId());

					if(pUpperLeftBond->CalculateLength() < 0.5*m_Bilayer1Length &&
					   pUpperLeftBond->CalculateLength() < 0.5*m_Bilayer2Length)
					{
						m_vPolymerisedBonds.push_back(pUpperLeftBond);
					}
					else
						delete pUpperLeftBond;		
				}

				polymerIndex++;

				if(polymerIndex == (rowIndex+1)*rowLength[monolayerIndex])
					rowIndex++;

			}	// End of loop over polymers being polymerised

			// Remove polymerised bonds from each monolayer so that the 
			// user-defined fraction of bonds is left. We apply a random 
			// shuffle to the bond vector and then delete bonds starting from 
			// the front until the desired fraction remains. The random shuffle
			// prevents the removed bonds being contiguous.
			//
			// The number of bonds deleted is just 
			// 
			//		(1-m_PolymerisedFraction)*m_vPolymerisedBonds.size()
			//
			// and this is done for each monolayer. Note that this assumes the
			// vector is emptied between monolayers. This results in the same
			// fraction being removed from each monolayer, but the actual
			// number of bonds removed may be different if the sizes of the two
			// monolayers are different.
			// We have to delete the bonds before removing their pointers from the
			// vector because we cannot change the vector's size while iterating
			// through it.

			// Remove random_shuffle to make it compile on c++17
			std::minstd_rand rng(rand());
			std::shuffle(m_vPolymerisedBonds.begin(), m_vPolymerisedBonds.end(), rng);

			const long removeNo = static_cast<long>((1.0 - m_PolymerisedFraction)*m_vPolymerisedBonds.size());

			if(removeNo > 0)
			{
				BondVectorIterator lastBondRemoved;
				long numberRemoved = 0;

				for(BondVectorIterator iterBond=m_vPolymerisedBonds.begin(); iterBond!=m_vPolymerisedBonds.end(); iterBond++)
				{
					if(numberRemoved < removeNo)
					{
						lastBondRemoved = iterBond;
						delete *iterBond;
						numberRemoved++;
					}
				}

				// Now erase the elements from the vector. Note that we have to 
				// increment the iterator to the end of the sequence we want to
				// erase before we call the erase function.

//				BondVectorIterator lastOne = m_vPolymerisedBonds.erase(m_vPolymerisedBonds.begin(), ++lastBondRemoved);
				m_vPolymerisedBonds.erase(m_vPolymerisedBonds.begin(), ++lastBondRemoved);
			}

			// Copy the polymerised bonds for this monolayer to the CInitialState 
			// object, storing them in the polymerised bond vector and the
			// general bond vector.

			for(BondVectorIterator iterBond=m_vPolymerisedBonds.begin(); iterBond!=m_vPolymerisedBonds.end(); iterBond++)
			{
				riState.AddPolymerisedBond(*iterBond);
				riState.AddPolymerisedBondtoAllBonds(*iterBond);
			}
		}	// End of polymerisation algorithm

	}	// End of loop over monolayers

	// Update the CInitialState's record of bonds by counting all bond types again

	riState.CalculateBondTypeTotals();

	// ****************************************
	// Now fill the spaces between the lamella and the walls randomly
	// with the remaining polymers. We calculate the free space available 
	// between the lamella and the walls (if any), and use an isRandom object 
	// to fill space with the remaining polymers in the simulation. 
	// We pass a vector of the coordinates of the excluded regions
	// so that the polymers can be assigned coordinates in the free regions.
	// We start the measurements at the lower box face in the direction of the 
	// bilayer normal. The side on which a polymer is placed alternates
	// from one to the next. We also check that bonded beads are not so far
	// apart that they generate large bond forces. But we only do this for 
	// the non-bilayer polymers. The user specifies how stretched the bilayer
	// polymers are using the m_Thickness parameter and if they set a value 
	// that causes the bonds to be stretched that is a choice we allow.

	// Store the polymer types that don't make up the bilayer

	zLongVector nonBilayerPolymerTypes;

	nonBilayerPolymerTypes.clear();

	for(long polyType=0; polyType<riState.GetPolymerTypeTotal(); polyType++)
	{
		if(polyType != m_PolymerType)
		{
			nonBilayerPolymerTypes.push_back(polyType);
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

bool CLamellaBuilder::AssembleP(CInitialState& riState)
{
    bool bSuccess = true;
	
#if EnableParallelSimBox == SimMPSEnabled

	// ****************************************
	// Copy the polymers in the CInitialState into local vectors for ease of access.
	// First we count the number of polymers preceeding the ones we want using 
	// their type to cycle through the GetPolymerTotalForType() function. Once we
	// have the start of the polymers we are interested in, and we know how many
	// there are, we can copy that part of the polymer vector into a local
	// vector.

	long firstPolymer = 0;

	// does the loop execute if m_PolymerType =0?

	for(long ip=0; ip<m_PolymerType; ip++)
	{
		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

    m_PolymerTotal = riState.GetPolymerTotalForType(m_PolymerType);
	
	long lastPolymer = firstPolymer + m_PolymerTotal;
	
	
	// If the bilayer passes through the current SimBox, we position the polymers into a bilayer.
	
	if(m_PolymerTotal > 0)
	{		
	// Copy the polymer pointers for the required polymers into local storage.
	// We use a back_inserter to avoid having to manually resize the container.

	PolymerVector bilayerPolymers = riState.GetPolymers();

	copy(bilayerPolymers.begin()+firstPolymer, bilayerPolymers.begin()+lastPolymer, back_inserter(m_vBilayer));

	// Get the number of beads in all polymers in the bilayer. Note that the
	// size of these polymers includes the head and tail. Wall polymers would
	// fail here because their head is not counted in their size. Because
	// there is only one kind of polymer used in the bilayer we don' have to check
	// which type comes first.

	m_PolymerSize = m_vBilayer.at(0)->GetSize();

	// ****************************************
	// Calculate the number of polymers in each monolayer from the fraction in the
	// upper one. We have the correspdonence
	//
	//	0 = upper monolayer
	//	1 = lower monolayer

	m_MonolayerSign[0] =  1.0;	// Sign needed to assign correct coordinates to beads
	m_MonolayerSign[1] = -1.0;	// in polymers in the upper and lower monolayers

	m_MonolayerTotal[0] = static_cast<long>(m_UpperFraction*m_PolymerTotal);
	m_MonolayerTotal[1] = m_PolymerTotal - m_MonolayerTotal[0];

	// Store a flag showing if the user has specified an asymmetry parameter
	// of 1 or 0, indicating that all the polymers are in one monolayer. Then
	// set the lattice size from the number of polymers. If all the polymers
	// are in one monolayer, we adjust the m_UpperHead, m_LowerHead parameters
	// to reflect this.
	
	// Note that we have to translate the bilayer centre and upper and lower extent into the local SimBox from the whole simulation Space

	if(m_MonolayerTotal[0] == 0)	// All polymers in lower monolayer	
	{
		while (m_BilayerCentre > 0.0)
		{
		    m_BilayerCentre -= riState.GetSimBoxXLength();
		
		}
		m_BilayerCentre += riState.GetSimBoxXLength();
		m_UpperHead      = m_BilayerCentre + 0.5*m_Thickness;
		m_LowerHead      = m_BilayerCentre - 0.5*m_Thickness;
		
		m_bMonolayer				= true;
		m_MonolayerFirstPolymer[0]	= 0;
		m_MonolayerFirstPolymer[1]	= 0;
		m_LatticeSize[0]			= 0;
		m_LatticeSize[1]			= m_MonolayerTotal[1];			
		m_MonolayerHead[0]			= 0.0;
		m_MonolayerHead[1]			= m_LowerHead;
		m_UpperHead					= m_BilayerCentre;
	}
	else if (m_MonolayerTotal[1] == 0)	// All polymers in upper monolayer
	{
		while (m_BilayerCentre > 0.0)
		{
		    m_BilayerCentre -= riState.GetSimBoxYLength();
		
		}
		m_BilayerCentre += riState.GetSimBoxYLength();
		m_UpperHead      = m_BilayerCentre + 0.5*m_Thickness;
		m_LowerHead      = m_BilayerCentre - 0.5*m_Thickness;
		
		m_bMonolayer				= true;
		m_MonolayerFirstPolymer[0]	= 0;
		m_MonolayerFirstPolymer[1]	= 0;
		m_LatticeSize[0]			= m_MonolayerTotal[0];		
		m_LatticeSize[1]			= 0;
		m_MonolayerHead[0]			= m_UpperHead;
		m_MonolayerHead[1]			= 0.0;
		m_LowerHead					= m_BilayerCentre;
	}
	else
	{
		// Translate the bilayer into a single SimBox from the whole simulation Space
		
		while (m_BilayerCentre > 0.0)
		{
		    m_BilayerCentre -= riState.GetSimBoxZLength();
		
		}
		m_BilayerCentre += riState.GetSimBoxZLength();
		m_UpperHead      = m_BilayerCentre + 0.5*m_Thickness;
		m_LowerHead      = m_BilayerCentre - 0.5*m_Thickness;
	   
		m_bMonolayer				= false;
		m_MonolayerFirstPolymer[0]	= 0;
		m_MonolayerFirstPolymer[1]	= m_MonolayerTotal[0];
		m_LatticeSize[0]			= m_MonolayerTotal[0];		
		m_LatticeSize[1]			= m_MonolayerTotal[1];
		m_MonolayerHead[0]			= m_UpperHead;
		m_MonolayerHead[1]			= m_LowerHead;
	}

	// ****************************************
	// Calculate the free space available between the membrane and the wall.
	// If no wall exists the functions just return 0 for its width.
	// If all the polymers are in one monolayer, we adjust the free space
	// to the centre of the "bilayer".

	if(m_X == 1)
	{
		m_LowerFreeLength = m_LowerHead;
		m_UpperFreeLength = riState.GetSimBoxXLength() - m_UpperHead;
	}
	else if(m_Y == 1)
	{
		m_LowerFreeLength = m_LowerHead;
		m_UpperFreeLength = riState.GetSimBoxYLength() - m_UpperHead;
	}
	else if(m_Z == 1)
	{
		m_LowerFreeLength = m_LowerHead;
		m_UpperFreeLength = riState.GetSimBoxZLength() - m_UpperHead;
	}
	
//    std::cout << "********************" << zEndl;
//	std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " starting to assemble " << m_PolymerTotal << " " << m_vBilayer.size() << " polymers into a lamella structure" << zEndl;
//	std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " Data are " << m_PolymerType << " " << m_X << " " << m_Y << " " << m_Z << " " << m_Centre << " " << m_Thickness << " " << m_bLinearise << " " << m_UpperFraction << zEndl;
//	std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " Lamella data: " << m_Bilayer1Length << " " << m_Bilayer2Length << " " << m_BilayerArea << " " << m_BilayerCentre << " " << m_LowerHead << " " << m_UpperHead << zEndl;
//	std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " Polymer data: " << m_PolymerTotal << " " << m_PolymerSize << " " << m_LowerFreeLength << " " << m_UpperFreeLength << zEndl;
//    std::cout << "********************" << zEndl;
	
	// ****************************************
	// Calculate the lattice size for a hexagonal lattice that organises
	// the polymers in each monolayer. We have to take into account 
	// the walls, the orientation of the bilayer and the number of 
	// polymers in each  monolayer to calculate the spacing.
	// Get unit vectors in e1 and e2 directions (NB. 1.1547 = 2/sqrt(3)) and the
	// number of vertices in the lattice.
	// We shrink the lattice vectors by 0.999 and offset the first sites by 0.001 to
	// prevent any sites going outside the SimBox. This occurred with a box size
	// of 20**3 in the test run 11024, changelist 462.

	if(m_bMonolayer)
	{
		if(m_MonolayerTotal[0] == 0)	// All in lower monolayer
		{
			m_HexSpace[0]	= 0.0;
			m_HexSpace[1]	= 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[1]));
		}
		else
		{
			m_HexSpace[0]	= 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[0]));
			m_HexSpace[1]	= 0.0;
		}
	}
	else
	{
		m_HexSpace[0]	= 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[0]));
		m_HexSpace[1]	= 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[1]));
	}

	m_Hexe1X[0]		= m_HexSpace[0];			// e1 lattice vector
	m_Hexe1Y[0]		= 0.0;
	m_Hexe2X[0]		= 0.5*m_HexSpace[0];		// e2 lattice vector
	m_Hexe2Y[0]		= 0.866025*m_HexSpace[0];

	m_Hexe1X[1]		= m_HexSpace[1];			// e1 lattice vector
	m_Hexe1Y[1]		= 0.0;
	m_Hexe2X[1]		= 0.5*m_HexSpace[1];		// e2 lattice vector
	m_Hexe2Y[1]		= 0.866025*m_HexSpace[1];
	
	// For linear and branched polymers we count the number of beads between 
	// the head and tail (inclusive) and use this to define the spacing in 
	// the C direction. The spacing is defined as the monolayer thickness
	// divided by m_PolymerSize-1. 
	// We don't know whether the head or tail will be encountered first in the 
	// polymer string so we set a flag, bCount, to indicate when we should count
	// beads. It is reset to null when both head and tail have been counted.
	// The other beads in the polymer are arranged randomly around the head-tail
	// axis.
	//
	// NOTE. This algorithm won't work if the tail bead is found first in the
	// polymer shape string. It will arrange the beads as if the head were first,
	// resulting in the beads between the head and tail having their order
	// reversed.

	const BeadVector&  vBeads = m_vBilayer.at(0)->GetBeads();
	const CBead*		pcHead = dynamic_cast<CBead*>(m_vBilayer.at(0)->GetHead());
	const CBead*		pcTail = dynamic_cast<CBead*>(m_vBilayer.at(0)->GetTail());

	const CBead*		 pEnd = NULL;
	long counter			  = 0;
	bool bCount				  = false;

	long dummyCount = 0;

	for(cBeadVectorIterator pBead=vBeads.begin();pBead!=vBeads.end(); pBead++)
	{
		dummyCount++;

		if(*pBead == pcHead && !bCount)
		{
			bCount = true;
			pEnd   = pcTail;
			counter++;
		}
		else if(*pBead == pcTail && !bCount)
		{
			bCount = true;
			pEnd   = pcHead;
			counter++;
		}
		else if(*pBead == pEnd)
		{
			bCount = false;
			counter++;
		}
		else if(bCount)
		{
			counter++;
		}
	}

	m_HexCAxisSpace[0] = 0.5*m_Thickness/static_cast<double>(counter-1);
	m_HexCAxisSpace[1] = m_HexCAxisSpace[0];

	// ****************************************
	// Loop over the number of vertices in the lattices. If all the polymers are
	// in one monolayer we just loop once; if both monolayers are needed the
	// loop executes twice. We have to take care with the orientation of the 
	// polymers in each monolayer and use the m_MonolayerSign[] variable to
	// adjust the coordinate in the direction of the bilayer normal when setting
	// bead coordinates. Polymer head coordinates are positioned
	// directly above (or below) the tails using the m_Thickness parameter.
	//
	// The hexp[] coordinates are those of the vertices in the hexagonal
	// lattice used to site the polymers. They are copied to xp[] so that
	// we can translate the coordinates for those polymers that cross the 
	// SimBox boundaries without affecting the lattice iteration. The hexp[]
	// coordinates are translated only when they cross the hexagonal lattice
	// boundaries and this starts a new row in the lattice. Translating the
	// xp[] coordinates for a polymer does not start a new row, it just moves
	// the polymer back within the SimBox.
	//
	// Note that the xp[] coordinates are without regard to the bilayer 
	// orientation, that is, they are in-plane coordinates. We convert them 
	// to X,Y,Z coordinates later. Also, we set the copies of the bead coordinates
	// using SetunPBCXPos(), SetInitialXPos() later.

	double hexp[3];		// Hexagonal lattice coords
	double xp[3];		// Head bead coords
	double xp1[3];		// Other polymer beads coords

	// Store the length of the rows in the hexagonal lattices for each monolayer.
	// We have to store them separately because there may be different numbers of
	// polymers in each monolayer depending on how the user specified the fractions.

	long rowLength[2];

	for(short monolayerIndex = 0; monolayerIndex < 2; monolayerIndex++)
	{
		hexp[0] = 0.001*m_Hexe1X[monolayerIndex];	// We use an offset to avoid positions coinciding with boundaries
		hexp[1] = 0.001*m_Hexe2Y[monolayerIndex];
		hexp[2] = 0.0;

		xp[0]	= hexp[0];
		xp[1]	= hexp[1];
		xp[2]	= hexp[2];

		xp1[0]	= 0.0;
		xp1[1]	= 0.0;
		xp1[2]	= 0.0;

		rowLength[monolayerIndex] = 0;

		long siteCounter = 0;			// Used to check no of sites per row
		long rowCounter  = 0;			// Used to set coordinates of first site in each row

		for(long iv=m_MonolayerFirstPolymer[monolayerIndex]; iv<m_MonolayerFirstPolymer[monolayerIndex]+m_LatticeSize[monolayerIndex]; iv++)
		{
			// We still use the variable upperPoly to avoid having to change its name

			CPolymer* upperPoly = m_vBilayer.at(iv);

			// If the initial state is polymerised, store the polymer pointers in
			// vectors so that we can attach them by newly-created bonds to 
			// their neighbours later. We also store counters showing what the 
			// (i,j) coordinates of the polymers are for use in assigning bonds.

			if(m_bPolymerise)
			{
				if(monolayerIndex == 0)
				{
					m_vPolymerisedUpperPolymers.push_back(upperPoly);

				}
				else
				{
					m_vPolymerisedLowerPolymers.push_back(upperPoly);
				}
			}

			// Position the beads in the polymers around the head and tail axis.
			// We have to take account of the orientation of the bilayer now.

			if(m_X == 1)
			{
				upperPoly->GetTail()->SetXPos(m_BilayerCentre + m_MonolayerSign[monolayerIndex]*0.2);
				upperPoly->GetTail()->SetYPos(xp[0]);	// shift tails slightly to avoid
				upperPoly->GetTail()->SetZPos(xp[1]);	// creating large forces

				upperPoly->GetHead()->SetXPos(m_MonolayerHead[monolayerIndex]);
				upperPoly->GetHead()->SetYPos(xp[0]);
				upperPoly->GetHead()->SetZPos(xp[1]);

				// Assign coordinates to other beads in the polymers wrapping them around
				// the head-tail axis. If the user has selected the linearise option then 
				// the beads are given coordinates on a straight line joining the polymer
				// head and tail, otherwise they are positioned somwhat randomly about the
				// head-tail axis. If the polymers are in addition linear, the initial 
				// configuration will consist of exactly straight polymers, but if the 
				// polymers are branched only the beads between the head and tail will
				// be straightened.  We check that the random
				// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
				// We don't have to check for their being less than 0 because we only
				// add a random component, and don't subtract it.

				double beadCounter = 1.0;

				for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
				{
					if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
					{
						if(m_bLinearise)
						{
							xp1[0] = upperPoly->GetHead()->GetXPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
							xp1[1] = xp[0];
							xp1[2] = xp[1];
							beadCounter += 1.0;
						}
						else
						{
							xp1[0] = m_BilayerCentre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();
							xp1[1] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();
							xp1[2] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();

							if(xp1[1] > riState.GetSimBoxYLength())
								xp1[1] -= 0.5*m_Hexe1X[monolayerIndex];

							if(xp1[2] > riState.GetSimBoxZLength())
								xp1[2] -= 0.5*m_Hexe2Y[monolayerIndex];
						}

						(*iterBead2)->SetXPos(xp1[0]);
						(*iterBead2)->SetYPos(xp1[1]);
						(*iterBead2)->SetZPos(xp1[2]);
					}
				}
			}
			else if(m_Y == 1)
			{
				upperPoly->GetTail()->SetXPos(xp[1]);	// shift tails slightly to avoid
				upperPoly->GetTail()->SetYPos(m_BilayerCentre + m_MonolayerSign[monolayerIndex]*0.2);
				upperPoly->GetTail()->SetZPos(xp[0]);	// creating large forces

				upperPoly->GetHead()->SetXPos(xp[1]);
				upperPoly->GetHead()->SetYPos(m_MonolayerHead[monolayerIndex]);
				upperPoly->GetHead()->SetZPos(xp[0]);

				// Assign coordinates to other beads in the polymers wrapping them around
				// the head-tail axis. If the user has selected the linearise option then 
				// the beads are given coordinates on a straight line joining the polymer
				// head and tail, otherwise they are positioned somwhat randomly about the
				// head-tail axis. If the polymers are in addition linear, the initial 
				// configuration will consist of exactly straight polymers, but if the 
				// polymers are branched only the beads between the head and tail will
				// be straightened.  We check that the random
				// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
				// We don't have to check for their being less than 0 because we only
				// add a random component, and don't subtract it.

				double beadCounter = 1.0;

				for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
				{
					if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
					{
						if(m_bLinearise)
						{
							xp1[0] = xp[1];
							xp1[1] = upperPoly->GetHead()->GetYPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
							xp1[2] = xp[0];
							beadCounter += 1.0;
						}
						else
						{
							xp1[0] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();
							xp1[1] = m_BilayerCentre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();
							xp1[2] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();

							if(xp1[0] > riState.GetSimBoxXLength())
								xp1[0] -= 0.5*m_Hexe2Y[monolayerIndex];

							if(xp1[2] > riState.GetSimBoxZLength())
								xp1[2] -= 0.5*m_Hexe1X[monolayerIndex];
						}

						(*iterBead2)->SetXPos(xp1[0]);
						(*iterBead2)->SetYPos(xp1[1]);
						(*iterBead2)->SetZPos(xp1[2]);
					}
				}
			}
			else if(m_Z == 1)
			{
				upperPoly->GetTail()->SetXPos(xp[0]);	// shift tails slightly to avoid
				upperPoly->GetTail()->SetYPos(xp[1]);	// creating large forces
				upperPoly->GetTail()->SetZPos(m_BilayerCentre + m_MonolayerSign[monolayerIndex]*0.2);

				upperPoly->GetHead()->SetXPos(xp[0]);
				upperPoly->GetHead()->SetYPos(xp[1]);
				upperPoly->GetHead()->SetZPos(m_MonolayerHead[monolayerIndex]);

				// Assign coordinates to other beads in the polymers wrapping them around
				// the head-tail axis. If the user has selected the linearise option then 
				// the beads are given coordinates on a straight line joining the polymer
				// head and tail, otherwise they are positioned somwhat randomly about the
				// head-tail axis. If the polymers are in addition linear, the initial 
				// configuration will consist of exactly straight polymers, but if the 
				// polymers are branched only the beads between the head and tail will
				// be straightened.  We check that the random
				// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
				// We don't have to check for their being less than 0 because we only
				// add a random component, and don't subtract it.

				double beadCounter = 1.0;

				for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
				{
					if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
					{
						if(m_bLinearise)
						{
							xp1[0] = xp[0];
							xp1[1] = xp[1];
							xp1[2] = upperPoly->GetHead()->GetZPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
							beadCounter += 1.0;
						}
						else
						{
							xp1[0] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();
							xp1[1] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();
							xp1[2] = m_BilayerCentre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();

							if(xp1[0] > riState.GetSimBoxXLength())
								xp1[0] -= 0.5*m_Hexe1X[monolayerIndex];

							if(xp1[1] > riState.GetSimBoxYLength())
								xp1[1] -= 0.5*m_Hexe2Y[monolayerIndex];
						}

						(*iterBead2)->SetXPos(xp1[0]);
						(*iterBead2)->SetYPos(xp1[1]);
						(*iterBead2)->SetZPos(xp1[2]);
					}
				}
			}

			// Update the coordinates for the next vertex. We make two checks:
			// if the next hexp[0] coordinate is greater than the hexagonal lattice 
			// length we translate to the next row, but if not, and it is 
			// greater than the SimBox width, we translate back in the same row.
			// We have to do the test for the hexagonal lattice first so that
			// the first row is correctly handled. Thw widths of both lattices
			// coincide here. Note that if we translate a vertex we increment
			// the y coordinate as well.

			siteCounter++;

			hexp[0] += m_Hexe1X[monolayerIndex];

			if(hexp[0] > m_Bilayer1Length + static_cast<double>(rowCounter)*m_Hexe2X[monolayerIndex])
			{
				rowCounter++;

				hexp[0] = static_cast<double>(rowCounter)*m_Hexe2X[monolayerIndex];
				hexp[1] += m_Hexe2Y[monolayerIndex];

				xp[0] = hexp[0];
				xp[1] = hexp[1];

				if(rowLength[monolayerIndex] == 0)
					rowLength[monolayerIndex] = siteCounter;

				// If we have run out of area it is an error so abort the run

				if(hexp[1] >= m_Bilayer2Length)
				{
					new CLogBuilderError(0, "lamella lattice outside SimBox");
					return false;
				}
			}
			else if(hexp[0] > m_Bilayer1Length)
			{
				xp[0] = hexp[0] - m_Bilayer1Length;
			}
			else
			{
				xp[0] = hexp[0];
				xp[1] = hexp[1];
			}
		}		// end of loop over polymers


	}	// End of loop over monolayers

	// Update the CInitialState's record of bonds by counting all bond types again

	riState.CalculateBondTypeTotals();
		
	// ****************************************
	// Now fill the spaces between the lamella and the walls randomly
	// with the remaining polymers. We calculate the free space available 
	// between the lamella and the walls (if any), and use an isRandom object 
	// to fill space with the remaining polymers in the simulation. 
	// We pass a vector of the coordinates of the excluded regions
	// so that the polymers can be assigned coordinates in the free regions.
	// We start the measurements at the lower box face in the direction of the 
	// bilayer normal. The side on which a polymer is placed alternates
	// from one to the next. We also check that bonded beads are not so far
	// apart that they generate large bond forces. But we only do this for 
	// the non-bilayer polymers. The user specifies how stretched the bilayer
	// polymers are using the m_Thickness parameter and if they set a value 
	// that causes the bonds to be stretched that is a choice we allow.

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

bool CLamellaBuilder::AssembleRandomP(CInitialState& riState, const zDoubleVector exRegions)
{
    bool bSuccess = true;
	
#if EnableParallelSimBox == SimMPSEnabled

	zLongVector nonBilayerPolymerTypes;

	nonBilayerPolymerTypes.clear();

	for(long polyType=0; polyType<riState.GetPolymerTypeTotal(); polyType++)
	{
		if(polyType != m_PolymerType)
		{
			nonBilayerPolymerTypes.push_back(polyType);
		}	
	}

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
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	// Randomly position the beads in all non-bilayer polymers in the empty regions
	
	double xp[3];				// Coordinates to assign to a bead
	xp[0]        = 0.0;
	xp[1]        = 0.0;
	xp[2]        = 0.0;
	double width = 0.0;			// region width used several times below
	double freeLength = 0.0;	// total width of all free regions

	// Get the total length of the free regions. Note that the size of the
	// exRegions container must be even.

    long unsigned int index = 0;
    
	for(index=1; index<exRegions.size()/2; index++)
	{
		width = exRegions.at(2*index) - exRegions.at(2*index-1);
		freeLength += width;
	}

	zDoubleVector freeRegionRatio;	// Cumulative occupation prob for each region

	double cumulativeProb = 0.0;

	for(index=1; index<exRegions.size()/2; index++)
	{
		width = exRegions.at(2*index) - exRegions.at(2*index-1);

		const double prob = width/freeLength;

		cumulativeProb += prob;

		freeRegionRatio.push_back(cumulativeProb);
	}

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

			for(long unsigned int nindex=0; nindex<freeRegionRatio.size(); nindex++)
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

