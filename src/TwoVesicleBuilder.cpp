/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// TwoVesicleBuilder.cpp: implementation of the CTwoVesicleBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "TwoVesicleBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"


#include "CNTCell.h"			// needed for random number generator


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTwoVesicleBuilder::CTwoVesicleBuilder(zLongVector polymerTypes1,  zLongVector polymerTypes2, 
									   zLongVector interiorTypes1, zLongVector interiorTypes2, 
						  			   zDoubleVector outerFractions1, zDoubleVector outerFractions2,
											 const double xc[2], const double yc[2], const double zc[2],  
											 const double outerRadius[2], const double innerRadius[2],
											 const double thickness[2], const bool bPatches1[2],
											 const bool bPatches2[2]) : m_PolymerTypes1(polymerTypes1),
																		m_PolymerTypes2(polymerTypes2),
																		m_InteriorTypes1(interiorTypes1),
																		m_InteriorTypes2(interiorTypes2),
																		m_OuterFractions1(outerFractions1),
																		m_OuterFractions2(outerFractions2)
{
	for(short int i=0; i<2; i++)
	{
		m_XC[i]		= xc[i];
		m_YC[i]		= yc[i]; 
		m_ZC[i]		= zc[i];
		m_OuterRadius[i]	= outerRadius[i];
		m_InnerRadius[i]	= innerRadius[i];
		m_Thickness[i]		= thickness[i];
		m_bPatches1[i]		= bPatches1[i];
		m_bPatches2[i]		= bPatches2[i];
	}

}

CTwoVesicleBuilder::~CTwoVesicleBuilder()
{

}


// Virtual function to assemble polymers into a two-vesicle initial configuration.
// 
// The CInitialState object is responsible for creating the beads, bonds and
// polymers from the types defined by the CInputData object, via the function
// CreatePolymers(). This builder sets the coordinates of the beads so
// as to create the required two-vesicle configuration. The polymers that 
// compose the vesicles have already been created using a bulk concentration 
// specified by the user. 
//
// The number of polymers whose heads lie on the inner and outer surfaces of the
// vesicles are not necessarily the same. They are typically calculated on the 
// assumption that they have a common area-per-headgroup. Given the surface areas 
// of each monolayer we calculate the number of polymers and then position them 
// independently. Note that the user can set the vesicle radius (as long as it lies
// within the SimBox and does not overlap any wall present) so that polymers
// may be squashed or stretched beyond their preferred length.
// First we position the outer monolayer polymers so that their heads lie 
// roughly on the vertices of a hexagonal lattice covering the outer surface, and 
// their tails terminate on the surface of a sphere with radius m_BilayerRadius.
// This has been determined from the user-defined outer radius and bilayer thickness.
// Then we repeat the procedure for the inner monolayer
//

bool CTwoVesicleBuilder::Assemble(CInitialState &riState)
{
	long index = 0;	// counter used everywhere below
	cPolymerVectorIterator iterPoly;
	cBeadVectorIterator    iterBead;

	// ****************************************
	// Position the wall beads first because they cannot move and may be 
	// bonded to beads in the bulk whose positions can be arranged so 
	// that their bonds are not too large initially.

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}

	// Create the first vesicle

	isVesicle vesicle1(m_PolymerTypes1, m_OuterFractions1, m_XC[0], m_YC[0], m_ZC[0], m_OuterRadius[0], 
					  m_InnerRadius[0], m_Thickness[0], m_bPatches1);

	vesicle1.Assemble(riState);

	// now the second vesicle

	isVesicle vesicle2(m_PolymerTypes2, m_OuterFractions2, m_XC[1], m_YC[1], m_ZC[1], m_OuterRadius[1], 
					  m_InnerRadius[1], m_Thickness[1], m_bPatches2);

	vesicle2.Assemble(riState);

	// Calculate the free space available within the vesicles, and between the 
	// vesicles and the walls, and fill the vesicles with their interior polymers,
	// and the remainder of the simBox with the remaining polymers in the 
	// simulation. We assign coordinates randomly to the beads and then move 
	// them if they overlap with the vesicle volume.

	// Store the polymer types that don't make up the vesicles or their interiors. 
	// Because there are four lists of polymers that occur in the vesicles/interiors,
	// we concatenate them into a single list before checking for non-membership of 
	// the list. This avoids having to do some complicated logic for testing if 
	// a polymer is not in either list. We have already checked that the vesicle
	// and interior polymer lists are all disjoint.

	zLongVector vintPolymerTypes;
	zLongVector nonVesiclePolymerTypes;

	vintPolymerTypes.clear();
	nonVesiclePolymerTypes.clear();

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		if(find(m_PolymerTypes1.begin(), m_PolymerTypes1.end(), ip) != m_PolymerTypes1.end() ||
		   find(m_PolymerTypes2.begin(), m_PolymerTypes2.end(), ip) != m_PolymerTypes2.end() ||
		   find(m_InteriorTypes1.begin(), m_InteriorTypes1.end(), ip) != m_InteriorTypes1.end() ||
		   find(m_InteriorTypes2.begin(), m_InteriorTypes2.end(), ip) != m_InteriorTypes2.end())
		{
			vintPolymerTypes.push_back(ip);
		}
		else
		{
			nonVesiclePolymerTypes.push_back(ip);
		}
	}

	// Copy the polymers in the CInitialState into local vectors for ease of access.
	// The non-vesicle, non-interior polymers are stored in vExteriorPolymers, and the
	// interior polymers in vInteriorXPolymers.

	PolymerVector vPolymers = riState.GetPolymers();
	PolymerVector vInteriorPolymers1;
	PolymerVector vInteriorPolymers2;
	PolymerVector vExteriorPolymers;

	vInteriorPolymers1.clear();
	vInteriorPolymers2.clear();
	vExteriorPolymers.clear();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		lastPolymer += riState.GetPolymerTotalForType(ip);

		// Copy the polymer pointers for the required polymers into local storage.
		// We use a back_inserter to avoid having to manually resize the container.

		if(find(m_InteriorTypes1.begin(), m_InteriorTypes1.end(), ip) != m_InteriorTypes1.end())
		{
			copy(vPolymers.begin()+firstPolymer, vPolymers.begin()+lastPolymer, back_inserter(vInteriorPolymers1));
		}
		else if(find(m_InteriorTypes2.begin(), m_InteriorTypes2.end(), ip) != m_InteriorTypes2.end())
		{
			copy(vPolymers.begin()+firstPolymer, vPolymers.begin()+lastPolymer, back_inserter(vInteriorPolymers2));
		}
		else if(find(nonVesiclePolymerTypes.begin(), nonVesiclePolymerTypes.end(), ip) != nonVesiclePolymerTypes.end())
		{
			copy(vPolymers.begin()+firstPolymer, vPolymers.begin()+lastPolymer, back_inserter(vExteriorPolymers));
		}


		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	// Exclude any CNT cells that contain wall beads. We just mark the cells whose
	// indices are within the walls. We first set a flag in a vector showing that
	// the corresponding index is for a free CNT cell, next we exclude all cells
    // that lie within the wall regions, and finally we copy all the 
	// free CNT cell indeices into another vector to allow them to be randomly accessed
	// using a second index into the vector.

	const long CNTXNo = riState.GetCNTXCellNo();
	const long CNTYNo = riState.GetCNTYCellNo();
	const long CNTZNo = riState.GetCNTZCellNo();

	zLongVector vAllFreeCNTCells(CNTXNo*CNTYNo*CNTZNo);

	for(long unsigned int j=0; j<vAllFreeCNTCells.size(); j++)
	{
		vAllFreeCNTCells.at(j) = 1;
	}

	long xCellId, yCellId, zCellId;

	if(riState.GetWallXCellNo() != 0)	// X wall
	{
		for(xCellId=0; xCellId<riState.GetWallXCellNo(); xCellId++)
		{
			for(yCellId=0; yCellId<CNTYNo; yCellId++)
			{
				for(zCellId=0; zCellId<CNTZNo; zCellId++)
				{
					index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
					vAllFreeCNTCells.at(index) = 0;
          
					index = CNTXNo*(CNTYNo*zCellId + yCellId) + CNTXNo-xCellId-1;
					vAllFreeCNTCells.at(index) = 0;
				}
			}
		}
	}

	if(riState.GetWallYCellNo() != 0)	// Y wall
	{
		for(yCellId=0; yCellId<riState.GetWallYCellNo(); yCellId++)
		{
			for(xCellId=0; xCellId<CNTXNo; xCellId++)
			{
				for(zCellId=0; zCellId<CNTZNo; zCellId++)
				{
					index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
					vAllFreeCNTCells.at(index) = 0;

					index = CNTXNo*(CNTYNo*zCellId + CNTYNo-yCellId-1) + xCellId;
					vAllFreeCNTCells.at(index) = 0;
				}
			}
		}
	}

	if(riState.GetWallZCellNo() != 0)	// Z wall
	{
		for(zCellId=0; zCellId<riState.GetWallZCellNo(); zCellId++)
		{
			for(xCellId=0; xCellId<CNTXNo; xCellId++)
			{
				for(yCellId=0; yCellId<CNTYNo; yCellId++)
				{
					index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
					vAllFreeCNTCells.at(index) = 0;
					index = CNTXNo*(CNTYNo*(CNTZNo-zCellId-1) + yCellId) + xCellId;
					vAllFreeCNTCells.at(index) = 0;
				}
			}
		}
	}

	// Now exclude the CNT cells that contain beads belonging to polymers
	// composing the vesicle membranes

	const double CNTXCellWidth = riState.GetCNTXCellWidth();
	const double CNTYCellWidth = riState.GetCNTYCellWidth();
	const double CNTZCellWidth = riState.GetCNTZCellWidth();

	long ix, iy, iz;

	for(iterPoly=riState.GetPolymers().begin(); iterPoly!=riState.GetPolymers().end(); iterPoly++)
	{
		if(find(m_PolymerTypes1.begin(), m_PolymerTypes1.end(), (*iterPoly)->GetType()) != m_PolymerTypes1.end() ||
		   find(m_PolymerTypes2.begin(), m_PolymerTypes2.end(), (*iterPoly)->GetType()) != m_PolymerTypes2.end())
		{
			for(iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
				ix = static_cast<long>((*iterBead)->GetXPos()/CNTXCellWidth);
				iy = static_cast<long>((*iterBead)->GetYPos()/CNTYCellWidth);

#if SimDimension == 2
				iz = 0;
#elif SimDimension == 3
				iz = static_cast<long>((*iterBead)->GetZPos()/CNTZCellWidth);		
#endif

				index = CNTXNo*(CNTYNo*iz+iy) + ix;

				vAllFreeCNTCells.at(index) = 0;
			}
		}
	}

	// Copy the free CNT cells into two other vectors to hold the cells interior to
	// the two vesicles

	zLongVector vAllFreeInterior1CNTCells;
	zLongVector vAllFreeInterior2CNTCells;

	copy(vAllFreeCNTCells.begin(), vAllFreeCNTCells.end(), back_inserter(vAllFreeInterior1CNTCells));
	copy(vAllFreeCNTCells.begin(), vAllFreeCNTCells.end(), back_inserter(vAllFreeInterior2CNTCells));

	// and remove those cells not within the vesicle interiors. We also remove
	// the interior cells from the vAllFreeCNTCells vector so that we can partition
	// polymers in the inner and outer volumes independently. This means that
	// a given CNT cell can only occur in one or the other vector but not both.
	// We need the coordinates of the vesicle centres in real units, not as a fraction
	// of the SimBox size.

	const double VXC1 = m_XC[0]*riState.GetSimBoxXLength();
	const double VYC1 = m_YC[0]*riState.GetSimBoxYLength();
	const double VZC1 = m_ZC[0]*riState.GetSimBoxZLength();

	const double VXC2 = m_XC[1]*riState.GetSimBoxXLength();
	const double VYC2 = m_YC[1]*riState.GetSimBoxYLength();
	const double VZC2 = m_ZC[1]*riState.GetSimBoxZLength();

	long interiorSum1 = 0;
	long interiorSum2 = 0;

	for(xCellId=0; xCellId<riState.GetCNTXCellNo(); xCellId++)
	{
		for(yCellId=0; yCellId<riState.GetCNTYCellNo(); yCellId++)
		{
			for(zCellId=0; zCellId<riState.GetCNTZCellNo(); zCellId++)
			{
				index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;

				double xcd1 = static_cast<double>(xCellId)*CNTXCellWidth - VXC1;
				double ycd1 = static_cast<double>(yCellId)*CNTYCellWidth - VYC1;
				double zcd1 = static_cast<double>(zCellId)*CNTZCellWidth - VZC1;

				double xcd2 = static_cast<double>(xCellId)*CNTXCellWidth - VXC2;
				double ycd2 = static_cast<double>(yCellId)*CNTYCellWidth - VYC2;
				double zcd2 = static_cast<double>(zCellId)*CNTZCellWidth - VZC2;

				double rcd1 = xcd1*xcd1 + ycd1*ycd1 + zcd1*zcd1;
				double rcd2 = xcd2*xcd2 + ycd2*ycd2 + zcd2*zcd2;

				// If a cell is outside each vesicle remove it from the
				// appropriate interior free cells, whereas if it is inside
				// a vesicle remove it from the exterior cells and the other vesicle's
				// interior

				if(rcd1 > m_InnerRadius[0]*m_InnerRadius[0])
				{
					vAllFreeInterior1CNTCells.at(index) = 0;
				}
				else
				{
					interiorSum1++;
					vAllFreeCNTCells.at(index) = 0;
					vAllFreeInterior2CNTCells.at(index) = 0;
				}

				if(rcd2 > m_InnerRadius[1]*m_InnerRadius[1])
				{
					vAllFreeInterior2CNTCells.at(index) = 0;
				}
				else
				{
					interiorSum2++;
					vAllFreeCNTCells.at(index) = 0;
					vAllFreeInterior1CNTCells.at(index) = 0;
				}
			}
		}
	}

	// Now copy the remaining free CNT cell indices to a second vector
	// for the exterior and vesicle interior spaces

	zLongVector vFreeCNTCells;
	zLongVector vFreeInterior1CNTCells;
	zLongVector vFreeInterior2CNTCells;

	for(long j=0; j<CNTXNo*CNTYNo*CNTZNo; j++)
	{
		if(vAllFreeCNTCells.at(j) == 1)
			vFreeCNTCells.push_back(j);

		if(vAllFreeInterior1CNTCells.at(j) == 1)
			vFreeInterior1CNTCells.push_back(j);

		if(vAllFreeInterior2CNTCells.at(j) == 1)
			vFreeInterior2CNTCells.push_back(j);
	}

	// Now assign coordinates to the beads in non-vesicle polymers among
	// the free CNT cells

	double xp[3];
	xp[0]        = 0.0;
	xp[1]        = 0.0;
	xp[2]        = 0.0;

	const long freeCells		  = vFreeCNTCells.size();
	const long freeInterior1Cells = vFreeInterior1CNTCells.size();
	const long freeInterior2Cells = vFreeInterior2CNTCells.size();

	long rCellIndex;
	long rCNTCellIndex;
	bool bFirstBead;
	double xFirstBead = 0.0;
    double yFirstBead = 0.0;
    double zFirstBead = 0.0;

	for(iterPoly=riState.GetPolymers().begin(); iterPoly!=riState.GetPolymers().end(); iterPoly++)
	{
		if(find(vintPolymerTypes.begin(), vintPolymerTypes.end(), (*iterPoly)->GetType()) == vintPolymerTypes.end())
		{
		    bFirstBead = true;
			
			for(iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
			    if(bFirstBead)
				{
				    bFirstBead = false;
				
				    // Pick the cell containing the first bead randomly from those outside the vesicle. This avoids 
					// the new algorithm breaking the assignment of positions for single-bead polymers such as water.
					
				    rCellIndex = static_cast<long>(freeCells*CCNTCell::GetRandomNo());
				    if(rCellIndex == freeCells)
					    rCellIndex = 0;

				    rCNTCellIndex = vFreeCNTCells.at(rCellIndex); // cells outside  vesicle

				    ix = rCNTCellIndex%CNTXNo;
				    iy = ((rCNTCellIndex-ix)/CNTXNo)%CNTYNo;
				    iz = ((rCNTCellIndex-ix)/CNTXNo - iy)/CNTYNo;

				    xp[0] = riState.GetCNTXCellWidth()*(ix + CCNTCell::GetRandomNo());
				    xp[1] = riState.GetCNTYCellWidth()*(iy + CCNTCell::GetRandomNo());
				    xp[2] = riState.GetCNTZCellWidth()*(iz + CCNTCell::GetRandomNo());

				    (*iterBead)->SetXPos(xp[0]);
				    (*iterBead)->SetYPos(xp[1]);
				    (*iterBead)->SetZPos(xp[2]);
					
					xFirstBead = xp[0];
					yFirstBead = xp[1];
					zFirstBead = xp[2];
				}
				else
				{
				    // Position the polymer's subsequent beads randomly but close to the first bead
					
				    xp[0] = xFirstBead + riState.GetCNTXCellWidth()*(CCNTCell::GetRandomNo());
				    xp[1] = yFirstBead + riState.GetCNTYCellWidth()*(CCNTCell::GetRandomNo());
				    xp[2] = zFirstBead + riState.GetCNTZCellWidth()*(CCNTCell::GetRandomNo());
					
					// Ensure that subsequent beads are not placed outside the free volume of the SimBox, ie inside any walls or
					// outside the whole box.
					
					if(xp[0] < riState.GetWallXWidth())
					{
					    xp[0] = riState.GetWallXWidth() + riState.GetCNTXCellWidth()*CCNTCell::GetRandomNo();
					}
					else if(xp[0] > (riState.GetSimBoxXLength() - riState.GetWallXWidth()))
					{
					    xp[0] = riState.GetSimBoxXLength() - riState. GetWallXWidth() - riState.GetCNTXCellWidth()*CCNTCell::GetRandomNo();
					}
					(*iterBead)->SetXPos(xp[0]);					
					
					
					if(xp[1] < riState.GetWallYWidth())
					{
					    xp[1] = riState.GetWallYWidth() + riState.GetCNTYCellWidth()*CCNTCell::GetRandomNo();
					}
					else if(xp[1] > (riState.GetSimBoxYLength() - riState.GetWallYWidth()))
					{
					    xp[1] = riState.GetSimBoxYLength() - riState. GetWallYWidth() - riState.GetCNTYCellWidth()*CCNTCell::GetRandomNo();
					}
				    (*iterBead)->SetYPos(xp[1]);
					
					if(xp[2] < riState.GetWallZWidth())
					{
					    xp[2] = riState.GetWallZWidth() + riState.GetCNTZCellWidth()*CCNTCell::GetRandomNo();
					}
					else if(xp[2] > (riState.GetSimBoxZLength() - riState.GetWallZWidth()))
					{
					    xp[2] = riState.GetSimBoxZLength() - riState. GetWallZWidth() - riState.GetCNTZCellWidth()*CCNTCell::GetRandomNo();
					}
				    (*iterBead)->SetZPos(xp[2]);
				}
			}
		}
		else if(find(m_InteriorTypes1.begin(), m_InteriorTypes1.end(), (*iterPoly)->GetType()) != m_InteriorTypes1.end())
		{
			// Interior of first vesicle
			for(iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
				rCellIndex = static_cast<long>(freeInterior1Cells*CCNTCell::GetRandomNo());
					if(rCellIndex == freeInterior1Cells)
						rCellIndex = 0;
				rCNTCellIndex = vFreeInterior1CNTCells.at(rCellIndex);

				ix = rCNTCellIndex%CNTXNo;
				iy = ((rCNTCellIndex-ix)/CNTXNo)%CNTYNo;
				iz = ((rCNTCellIndex-ix)/CNTXNo - iy)/CNTYNo;

				long check = CNTXNo*(CNTYNo*iz+iy)+ix;

				if(check != rCNTCellIndex)
				{
					std::cout << "Error assigning vesicle interior 1 coordinates" << zEndl;
				}

				xp[0] = riState.GetCNTXCellWidth()*(ix + CCNTCell::GetRandomNo());
				xp[1] = riState.GetCNTYCellWidth()*(iy + CCNTCell::GetRandomNo());
				xp[2] = riState.GetCNTZCellWidth()*(iz + CCNTCell::GetRandomNo());

				(*iterBead)->SetXPos(xp[0]);
				(*iterBead)->SetYPos(xp[1]);
				(*iterBead)->SetZPos(xp[2]);
			}
		}
		else if(find(m_InteriorTypes2.begin(), m_InteriorTypes2.end(), (*iterPoly)->GetType()) != m_InteriorTypes2.end())
		{
			// Interior of second vesicle
			for(iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
				rCellIndex = static_cast<long>(freeInterior2Cells*CCNTCell::GetRandomNo());
					if(rCellIndex == freeInterior1Cells)
						rCellIndex = 0;
				rCNTCellIndex = vFreeInterior2CNTCells.at(rCellIndex);

				ix = rCNTCellIndex%CNTXNo;
				iy = ((rCNTCellIndex-ix)/CNTXNo)%CNTYNo;
				iz = ((rCNTCellIndex-ix)/CNTXNo - iy)/CNTYNo;

				long check = CNTXNo*(CNTYNo*iz+iy)+ix;

				if(check != rCNTCellIndex)
				{
					std::cout << "Error assigning vesicle interior 2 coordinates" << zEndl;
				}

				xp[0] = riState.GetCNTXCellWidth()*(ix + CCNTCell::GetRandomNo());
				xp[1] = riState.GetCNTYCellWidth()*(iy + CCNTCell::GetRandomNo());
				xp[2] = riState.GetCNTZCellWidth()*(iz + CCNTCell::GetRandomNo());

				(*iterBead)->SetXPos(xp[0]);
				(*iterBead)->SetYPos(xp[1]);
				(*iterBead)->SetZPos(xp[2]);
			}
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

	// Copy the coordinates of all beads into the other bead arrays used to ensure
	// that analysis of diffusion etc, is calculated from the initial bead positions.

	AssignUnPBCCoords(riState);

	// ****************************************
	// Set the velocities of the beads from a 3d Maxwell distribution 
	// with a mean  value and variance fixed from the temperature. This applies 
	// to all beads not just the ones in the vesicle. We indicate that a 3d
	// distribution is required by passing in a zero normal vector.

	AssignMaxwellVelocities(riState, 0, 0, 0);

	return true;
}
