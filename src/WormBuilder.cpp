/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// WormBuilder.cpp: implementation of the CWormBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "WormBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"

#include "CNTCell.h"			// needed for random number generator


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWormBuilder::CWormBuilder(const zLongVector& polymerTypes, 
							const PolymerCrossLinkVector& crossLinks,
							long xn, long yn, long zn, 
							double xc, double yc, double zc, 
							double radius, double halfLength, 
							bool bPatches, bool bPolymerise) : m_PolymerTypes(polymerTypes),
															    m_CrossLinks(crossLinks),
											  					m_XN(xn), m_YN(yn), m_ZN(zn),
											  					m_XC(xc), m_YC(yc), m_ZC(zc),
																m_Radius(radius),
																m_HalfLength(halfLength),
																m_bPatches(bPatches),
																m_XCentre(0.0),
																m_YCentre(0.0),
																m_ZCentre(0.0)
												  
{
}

CWormBuilder::~CWormBuilder()
{

}


// Virtual function to assemble polymers into a wormlike micelle initial configuration.
// 
// The CInitialState object is responsible for creating the beads, bonds and
// polymers from the types defined by the CInputData object, via the function
// CreatePolymers(). This builder sets the coordinates of the beads so
// as to create the required tubular worm configuration. The polymers that 
// compose the worm have already been created using a bulk concentration 
// specified by the user. 
//
// Note that the user can set the worm radius and half length (as long 
// as the worm lies within the SimBox and does not overlap any wall present) 
// so that polymers may be squashed or stretched beyond their preferred length.
// First we position the outer worm polymers so that their heads lie 
// roughly on the surface of a cylinder with the specified radius and their tails
// terminate along the worm axis.

bool CWormBuilder::Assemble(CInitialState &riState)
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

	// Create a worm micelle

	isWormMicelle micelle(m_PolymerTypes, m_XN, m_YN, m_ZN, m_XC, m_YC, m_ZC, m_Radius, 
						  m_HalfLength, m_bPatches, m_bPolymerise);

	micelle.Assemble(riState);

	// Calculate the free space between the micelle and the walls, and fill it 
	// with the remaining polymers in the simulation. We assign coordinates randomly 
	// to the beads and then move them if they overlap with the micelle volume.

	// Store the polymer types that don't make up the micelle 

	zLongVector nonVesiclePolymerTypes;

	nonVesiclePolymerTypes.clear();

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) == m_PolymerTypes.end())
		{
			nonVesiclePolymerTypes.push_back(ip);
		}	
	}

	// Copy the polymers in the CInitialState into local vectors for ease of access.
	// The non-vesicle non-interior polymers are stored in vRandomPolymers, and the
	// interior polymers in vInteriorPolymers.
	// We loop over all types of polymer adding them to a local vector if
	// their type occurs in the data for a random region.

	PolymerVector vPolymers = riState.GetPolymers();
	PolymerVector vRandomPolymers;

	vRandomPolymers.clear();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		lastPolymer += riState.GetPolymerTotalForType(ip);

		if(find(nonVesiclePolymerTypes.begin(), nonVesiclePolymerTypes.end(), ip) != nonVesiclePolymerTypes.end())
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(vPolymers.begin()+firstPolymer, vPolymers.begin()+lastPolymer, back_inserter(vRandomPolymers));
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

	m_XCentre = m_XC*riState.GetSimBoxXLength();
	m_YCentre = m_YC*riState.GetSimBoxYLength();
	m_ZCentre = m_ZC*riState.GetSimBoxZLength();

	const double CNTXCellWidth = riState.GetCNTXCellWidth();
	const double CNTYCellWidth = riState.GetCNTYCellWidth();
	const double CNTZCellWidth = riState.GetCNTZCellWidth();

	// Exclude all cells that lie within the expected cylindrical region

	double rcd2 = 0.0;

	for(xCellId=0; xCellId<riState.GetCNTXCellNo(); xCellId++)
	{
		for(yCellId=0; yCellId<riState.GetCNTYCellNo(); yCellId++)
		{
			for(zCellId=0; zCellId<riState.GetCNTZCellNo(); zCellId++)
			{
				index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;

				const double xcd = static_cast<double>(xCellId)*CNTXCellWidth - m_XCentre;
				const double ycd = static_cast<double>(yCellId)*CNTYCellWidth - m_YCentre;
				const double zcd = static_cast<double>(zCellId)*CNTZCellWidth - m_ZCentre;

				// Calculate the distance of the cell from the cylinder axis

				if(m_XN == 1)
				{
					rcd2 = ycd*ycd + zcd*zcd;
				}
				else if(m_YN == 1)
				{
					rcd2 = xcd*xcd + zcd*zcd;
				}
				else
				{
					rcd2 = xcd*xcd + ycd*ycd;
				}

				if(rcd2 < m_Radius*m_Radius)
				{
					vAllFreeCNTCells.at(index) = 0;
				}
			}
		}
	}

	// Now exclude the CNT cells that actually contain polymers making up the micelle

	long ix, iy, iz;

	for(iterPoly=riState.GetPolymers().begin(); iterPoly!=riState.GetPolymers().end(); iterPoly++)
	{
		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), (*iterPoly)->GetType()) != m_PolymerTypes.end())
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

	// Now copy the remaining free CNT cell indices to the vFreeCNTCells 

	zLongVector vFreeCNTCells;

	for(long j=0; j<CNTXNo*CNTYNo*CNTZNo; j++)
	{
		if(vAllFreeCNTCells.at(j) == 1)
			vFreeCNTCells.push_back(j);
	}

	const long freeCells = vFreeCNTCells.size();

	// Assign coordinates to the beads of non-worm polymers among
	// the free CNT cells. Cells are chosen randomly for each bead which
	// may lead to some cells containing more than the average number of beads
	// (set by the overall bead density), and this may lead to large forces
	// initially. In DPD this is not a problem but MD may need an MC equilibration
	// segment to reduce these initial transients to a reasonable level.

	double xp[3];
	xp[0]        = 0.0;
	xp[1]        = 0.0;
	xp[2]        = 0.0;

	long rCellIndex;
	long rCNTCellIndex;
	bool bFirstBead;
	double xFirstBead = 0.0;
    double yFirstBead = 0.0;
    double zFirstBead = 0.0;

	for(iterPoly=riState.GetPolymers().begin(); iterPoly!=riState.GetPolymers().end(); iterPoly++)
	{
		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), (*iterPoly)->GetType()) == m_PolymerTypes.end())
		{
		    bFirstBead = true;
			
			for(iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
			    if(bFirstBead)
				{
				    bFirstBead = false;
				
				    // Pick the cellcontaining the first bead randomly from those outside the vesicle. This avoids the new
					// algorithm breaking the assignment of positions for single-bead polymers such as water.
					
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
	}

	// Copy the coordinates of all beads into the other bead arrays used to ensure
	// that analysis of diffusion etc, is calculated from the initial bead positions.

	AssignUnPBCCoords(riState);

	// ****************************************
	// Set the in-plane velocities of the beads from a 3d Maxwell distribution 
	// with a mean  value and variance fixed from the temperature. This applies 
	// to all beads not just the ones in the vesicle. We indicate that a 3d
	// distribution is required by passing in a zero normal vector.

	AssignMaxwellVelocities(riState, 0, 0, 0);

	return true;
}
