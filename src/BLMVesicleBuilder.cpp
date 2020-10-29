/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// BLMVesicleBuilder.cpp: implementation of the CBLMVesicleBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "BLMVesicleBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"
#include "PolymerCrossLink.h"

#include "CNTCell.h"			// Needed for random number generator
#include "LogSimErrorTrace.h"   // Needed to issue error messages



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBLMVesicleBuilder::CBLMVesicleBuilder(const zLongVector&				blmPolymerTypes,
									   const zLongVector&				blmReservoirTypes,
									   const zDoubleVector&				blmUpperFractions,
									   const PolymerCrossLinkVector&	blmCrossLinks,
									   long blmx, long blmy, long blmz,
									   double blmCentre, double blmThickness,
									   bool bblmLinear, const bool bblmPatches[2], bool bblmPolymerise,
									   double area, double l1, double l2, double bilayerCentre, 
									   double upperHead, double lowerHead,
									   const zLongVector&				vPolymerTypes,
									   const zLongVector&				vInteriorTypes,
									   const zLongVector&				vExteriorTypes,
									   const zDoubleVector&				vOuterFractions,
									   const PolymerCrossLinkVector&	vCrossLinks,
									   double vxc, double vyc, double vzc,
									   double vxcentre, double vycentre, double vzcentre,
									   double vOuterRadius, double vInnerRadius, double vThickness,
									   const bool bvPatches[2],
									   bool bvPolymerise) : m_BLMPolymerTypes(blmPolymerTypes),
															m_BLMReservoirTypes(blmReservoirTypes),
															m_BLMUpperFractions(blmUpperFractions),
															m_BLMCrossLinks(blmCrossLinks),
															m_BLMX(blmx), m_BLMY(blmy), m_BLMZ(blmz),
															m_BLMCentre(blmCentre),
															m_BLMThickness(blmThickness),
															m_bBLMLinearise(bblmLinear),
															m_bBLMPolymerise(bblmPolymerise),
															m_BilayerArea(area),
															m_Bilayer1Length(l1),
															m_Bilayer2Length(l2),
															m_BilayerCentre(bilayerCentre),
															m_BilayerUpperHead(upperHead),
															m_BilayerLowerHead(lowerHead),
															m_bBLMMonolayer(false),
															m_BLMPolymerTotal(0),
															m_BLMUpperTotal(0),
															m_BLMLowerTotal(0),
															m_VPolymerTypes(vPolymerTypes),
															m_VInteriorTypes(vInteriorTypes),
															m_VExteriorTypes(vExteriorTypes),
															m_VOuterFractions(vOuterFractions),
															m_VCrossLinks(vCrossLinks),
															m_VXC(vxc), m_VYC(vyc), m_VZC(vzc),
															m_VesicleXCentre(vxcentre), 
															m_VesicleYCentre(vycentre), 
															m_VesicleZCentre(vzcentre),
															m_VOuterRadius(vOuterRadius),
															m_VInnerRadius(vInnerRadius),
															m_VThickness(vThickness),
															m_bVPolymerise(bvPolymerise)
{
	m_bBLMPatches[0] = bblmPatches[0];
	m_bBLMPatches[1] = bblmPatches[1];

	m_bVPatches[0] = bvPatches[0];
	m_bVPatches[1] = bvPatches[1];
}

CBLMVesicleBuilder::~CBLMVesicleBuilder()
{

}

// Virtual function to assemble polymers into the bilayer-vesicle initial state. 
//
// First, the vesicle is assembled out of the polymers that occur in the 
// m_VPolymerTypes vector, and its interior is filled with the
// polymers that occur in the m_VInteriorTypes vector. The exterior of the
// vesicle is then filled from the SimBox boundary to the bilayer monolayer
// closest to the vesicle with the polymers occurring in m_VExteriorTypes.
// Next, the bilayer is assembled using the polymers in m_BLMPolymerTypes
// and the space behind the bilayer (furthest from the vesicle) filled with
// the polymers specified in the m_BLMReservoirTypes vector.
//
// 

bool CBLMVesicleBuilder::Assemble(CInitialState& riState)
{
    bool bSuccess = true;

	// Declare some counters used everywhere below

	unsigned long j	= 0;
	long ip			= 0;
	long index		= 0;	
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

	// Create a vesicle

	isVesicle vesicle(m_VPolymerTypes, m_VOuterFractions, m_VXC, m_VYC, m_VZC, m_VOuterRadius, 
					  m_VInnerRadius, m_VThickness, m_bVPatches);

	vesicle.Assemble(riState);

	// Calculate the free space available within the vesicle, and between the 
	// vesicle and the walls, and fill it with the remaining polymers in the 
	// simulation. We assign coordinates randomly to the beads and then move 
	// them if they overlap with the vesicle volume.

	// Store the polymer types that don't make up the vesicle and are not specified
	// as being in the vesicle interior, and also those that are marked as 
	// occupying the vesicle exterior. These do not include those that make up
	// the bilayer and may not include those in the bilayer reservoir. Note
	// that the reservoir may contain the same polymer types that occur in the
	// vesicle interior and exterior.

	zLongVector nonVesiclePolymerTypes;

	nonVesiclePolymerTypes.clear();

	for(ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		if(find(m_VPolymerTypes.begin(),  m_VPolymerTypes.end(),  ip) == m_VPolymerTypes.end() &&
		   find(m_VInteriorTypes.begin(), m_VInteriorTypes.end(), ip) == m_VInteriorTypes.end() &&
		   find(m_VExteriorTypes.begin(), m_VExteriorTypes.end(), ip) == m_VExteriorTypes.end())
		{
			nonVesiclePolymerTypes.push_back(ip);
		}
	}

	// Copy the polymers in the CInitialState into local vectors for ease of access.
	// The non-vesicle non-interior polymers are stored in vRandomPolymers, and the
	// interior polymers in vInteriorPolymers and exterior polymers in
	// vExteriorPolymers.
	// We loop over all types of polymer adding them to a local vector if
	// their type occurs in the data for a random region.

	PolymerVector vPolymers = riState.GetPolymers();
	PolymerVector vInteriorPolymers;
	PolymerVector vExteriorPolymers;
	PolymerVector vReservoirPolymers;

	vInteriorPolymers.clear();
	vExteriorPolymers.clear();
	vReservoirPolymers.clear();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		lastPolymer += riState.GetPolymerTotalForType(ip);

		// Copy the polymer pointers for the required polymers into local storage.
		// We use a back_inserter to avoid having to manually resize the container.
		// There are three sorts of polymer: those in the vesicle interior, those
		// in the vesicle exterior and the remainder that are also not in the vesicle,
		// that is, the remainder are those polymers in the bilayer and possibly
		// the reservoir if the reservoir and the vesicle exterior do not contain
		// the same types of polymer.

		if(find(m_VInteriorTypes.begin(), m_VInteriorTypes.end(), ip) != m_VInteriorTypes.end())
		{
			copy(vPolymers.begin()+firstPolymer, vPolymers.begin()+lastPolymer, back_inserter(vInteriorPolymers));
		}
		else if(find(m_VExteriorTypes.begin(), m_VExteriorTypes.end(), ip) != m_VExteriorTypes.end())
		{
			copy(vPolymers.begin()+firstPolymer, vPolymers.begin()+lastPolymer, back_inserter(vExteriorPolymers));
		}
		else if(find(m_BLMReservoirTypes.begin(), m_BLMReservoirTypes.end(), ip) != m_BLMReservoirTypes.end())
		{
			copy(vPolymers.begin()+firstPolymer, vPolymers.begin()+lastPolymer, back_inserter(vReservoirPolymers));
		}


		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	// Exclude any CNT cells that contain wall beads and the region of the
	// SimBox containing the bilayer and reservoir. We just mark the cells whose
	// indices are within the walls and bilayer regions. We first set a flag 
	// in a vector showing whether the corresponding index is for a free CNT cell, 
	// then we collect all the free CNT cells into another vector to allow them 
	// to be randomly accessed using an index into the vector.
	// Because the wall and bilayer must have the same orientation, we use the
	// wall's normal to exclude the bilayer/reservoir cells as well.

	const long CNTXNo = riState.GetCNTXCellNo();
	const long CNTYNo = riState.GetCNTYCellNo();
	const long CNTZNo = riState.GetCNTZCellNo();

	const double CNTXCellWidth = riState.GetCNTXCellWidth();
	const double CNTYCellWidth = riState.GetCNTYCellWidth();
	const double CNTZCellWidth = riState.GetCNTZCellWidth();


	zLongVector vAllFreeCNTCells(CNTXNo*CNTYNo*CNTZNo);

	for(j=0; j<vAllFreeCNTCells.size(); j++)
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
	else if(riState.GetWallYCellNo() != 0)	// Y wall
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
	else if(riState.GetWallZCellNo() != 0)	// Z wall
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

	// Now exclude cells containing the bilayer and reservoir. This depends on 
	// the relative positions of the bilayer and vesicle centre. 

	if(m_BLMX == 1)
	{
		if(m_BilayerCentre < m_VesicleXCentre)		// Bilayer is below vesicle
		{
			const long CNTXLimit = static_cast<long>((m_BilayerUpperHead + 0.5*CNTXCellWidth)/CNTXCellWidth);

			for(xCellId=0; xCellId!=CNTXLimit; xCellId++)
			{
				for(yCellId=0; yCellId!=CNTYNo; yCellId++)
				{
					for(zCellId=0; zCellId!=CNTZNo; zCellId++)
					{
						index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
                        if(index < riState.GetCNTCellTotal())
                        {
                            vAllFreeCNTCells.at(index) = 0;
                        }
                        else
                        {
                            bSuccess = false;
                        }
					}
				}
			}
		}
		else		// bilayer is above vesicle
		{
			const long CNTXLimit = static_cast<long>((m_BilayerLowerHead - 0.5*CNTXCellWidth)/CNTXCellWidth);

			for(xCellId=CNTXLimit; xCellId!=CNTXNo; xCellId++)
			{
				for(yCellId=0; yCellId!=CNTYNo; yCellId++)
				{
					for(zCellId=0; zCellId!=CNTZNo; zCellId++)
					{
						index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
                        if(index < riState.GetCNTCellTotal())
                        {
                            vAllFreeCNTCells.at(index) = 0;
                        }
                        else
                        {
                            bSuccess = false;
                        }
					}
				}
			}
		}
	}
	else if( m_BLMY == 1)
	{
		if(m_BilayerCentre < m_VesicleYCentre)	// Bilayer is below vesicle
		{
			const long CNTYLimit = static_cast<long>((m_BilayerUpperHead + 0.5*CNTYCellWidth)/CNTYCellWidth);

			for(yCellId=0; yCellId!=CNTYLimit; yCellId++)
			{
				for(xCellId=0; xCellId!=CNTXNo; xCellId++)
				{
					for(zCellId=0; zCellId!=CNTZNo; zCellId++)
					{
						index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
                        if(index < riState.GetCNTCellTotal())
                        {
                            vAllFreeCNTCells.at(index) = 0;
                        }
                        else
                        {
                            bSuccess = false;
                        }
					}
				}
			}
		}
		else		// bilayer is above vesicle
		{
			const long CNTYLimit = static_cast<long>((m_BilayerLowerHead - 0.5*CNTYCellWidth)/CNTYCellWidth);

			for(yCellId=CNTYLimit; yCellId!=CNTYNo; yCellId++)
			{
				for(xCellId=0; xCellId!=CNTXNo; xCellId++)
				{
					for(zCellId=0; zCellId!=CNTZNo; zCellId++)
					{
						index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
                        if(index < riState.GetCNTCellTotal())
                        {
                            vAllFreeCNTCells.at(index) = 0;
                        }
                        else
                        {
                            bSuccess = false;
                        }
					}
				}
			}
		}
	}
	else if(m_BLMZ == 1)
	{
		if(m_BilayerCentre < m_VesicleZCentre)
		{
			// Bilayer is below the vesicle (i.e., has a smaller z coordinate) 
			// so exclude all CNT cells from the SimBox boundary
			// to the bilayer upper monolayer head coordinate.
			// We round up the integer index to ensure that the randomly-placed
			// polymers do not overlap the bilayer.

			const long CNTZLimit = static_cast<long>((m_BilayerUpperHead + 0.5*CNTZCellWidth)/CNTZCellWidth);

			for(zCellId=0; zCellId!=CNTZLimit; zCellId++)
			{
				for(xCellId=0; xCellId!=CNTXNo; xCellId++)
				{
					for(yCellId=0; yCellId!=CNTYNo; yCellId++)
					{
						index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
                        if(index < riState.GetCNTCellTotal())
                        {
                            vAllFreeCNTCells.at(index) = 0;
                        }
                        else
                        {
                            bSuccess = false;
                        }
					}
				}
			}
		}
		else
		{
			// Bilayer is above the vesicle (i.e., has a larger z coordinate) 
			// so exclude all CNT cells from the bilayer lower 
			// monolayer head coordinate to the SimBox boundary.

			const long CNTZLimit = static_cast<long>((m_BilayerLowerHead - 0.5*CNTZCellWidth)/CNTZCellWidth);

			for(zCellId=CNTZLimit; zCellId!=CNTZNo; zCellId++)
			{
				for(xCellId=0; xCellId!=CNTXNo; xCellId++)
				{
					for(yCellId=0; yCellId!=CNTYNo; yCellId++)
					{
						index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;
                        if(index < riState.GetCNTCellTotal())
                        {
                            vAllFreeCNTCells.at(index) = 0;
                        }
                        else
                        {
                            bSuccess = false;
                        }
					}
				}
			}
		}
	}

    // Abort the run on error
    if(!bSuccess)
    {
        new CLogSimErrorTrace(0, "Error in bilayer/vesicle builder (unable to assign bilayer polymer coordinates: stopping run");
        return bSuccess;
    }

	// Copy the free CNT cells into another vector,

	zLongVector vAllFreeInteriorCNTCells;

	copy(vAllFreeCNTCells.begin(), vAllFreeCNTCells.end(), back_inserter(vAllFreeInteriorCNTCells));

	// and remove those cells not within the vesicle interior. We also remove
	// the interior cells from the vAllFreeCNTCells vector so that we can partition
	// polymers in the inner and outer volumes independently. This means that
	// a given CNT cell can only occur in one or the other vector but not both.
	// We need the coordinates of the vesicle centre in real units, not as a fraction
	// of the SimBox size.

	long exteriorSum = 0;
	long interiorSum = 0;

	for(xCellId=0; xCellId<riState.GetCNTXCellNo(); xCellId++)
	{
		for(yCellId=0; yCellId<riState.GetCNTYCellNo(); yCellId++)
		{
			for(zCellId=0; zCellId<riState.GetCNTZCellNo(); zCellId++)
			{
				index = CNTXNo*(CNTYNo*zCellId + yCellId) + xCellId;

				double xcd = static_cast<double>(xCellId)*CNTXCellWidth - m_VesicleXCentre;
				double ycd = static_cast<double>(yCellId)*CNTYCellWidth - m_VesicleYCentre;
				double zcd = static_cast<double>(zCellId)*CNTZCellWidth - m_VesicleZCentre;

				double rcd2 = xcd*xcd + ycd*ycd + zcd*zcd;

				if(rcd2 > m_VInnerRadius*m_VInnerRadius)
				{
					exteriorSum++;
					vAllFreeInteriorCNTCells.at(index) = 0;
				}
				else
				{
					interiorSum++;
					vAllFreeCNTCells.at(index) = 0;
				}
			}
		}
	}

	// Exclude the CNT cells, including interior ones, that contain polymers 
	// making up the vesicle

	long ix, iy, iz;

	for(iterPoly=riState.GetPolymers().begin(); iterPoly!=riState.GetPolymers().end(); iterPoly++)
	{
		if(bSuccess && find(m_VPolymerTypes.begin(), m_VPolymerTypes.end(), (*iterPoly)->GetType()) != m_VPolymerTypes.end())
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

                if(index < riState.GetCNTCellTotal())
                {
				    vAllFreeCNTCells.at(index) = 0;
				    vAllFreeInteriorCNTCells.at(index) = 0;
                }
                else
                {
                    bSuccess = false;
                }
			}
		}
	}

    // Abort the run on error
    if(!bSuccess)
    {
        new CLogSimErrorTrace(0, "Error in bilayer/vesicle builder (vesicle radius too large for box): stopping run");
        return bSuccess;
    }

	// Now copy the remaining free CNT cell indices to the vFreeCNTCells and
	// vFreeInteriorCNTCells vectors, and store the number of such free cells
	// for use in assigning coordinates to the interior polymers.

	zLongVector vFreeCNTCells;
	zLongVector vFreeInteriorCNTCells;

	for(j=0; j<CNTXNo*CNTYNo*CNTZNo; j++)
	{
		if(vAllFreeCNTCells.at(j) == 1)
			vFreeCNTCells.push_back(j);

		if(vAllFreeInteriorCNTCells.at(j) == 1)
			vFreeInteriorCNTCells.push_back(j);
	}

	const long freeCells		 = vFreeCNTCells.size();
	const long freeInteriorCells = vFreeInteriorCNTCells.size();

	// Now position the beads belonging to the bilayer and the reservoir space
	// behind it.

	// Check if all the bilayer polymers are in one monolayer. We just see if 
	// the sum of all the polymers in either monolayer is zero. This is obtained from
	// the fractions of each polymer type and the number thereof. We also store
	// the number of polymers of all types in each monolayer for use in the
	// isBilayer constructor.

	m_BLMPolymerTotal	= 0;
	m_BLMUpperTotal		= 0;
	m_BLMLowerTotal		= 0;

	for(unsigned long iPolymer=0; iPolymer<m_BLMPolymerTypes.size(); iPolymer++)
	{
		long polyNo = riState.GetPolymerTotalForType(m_BLMPolymerTypes.at(iPolymer));
		long upperNo = static_cast<long>(m_BLMUpperFractions.at(iPolymer)*polyNo);

		m_BLMPolymerTotal	+= polyNo;
		m_BLMUpperTotal		+= upperNo;
		m_BLMLowerTotal		+= (polyNo - upperNo);
	}

	// If either monolayer is empty recalculate the free space outside the
	// bilayer to be filled with non-bilayer polymers.

	if(m_BLMUpperTotal == 0)			// Upper monolayer empty
	{
		m_bBLMMonolayer		= true;
		m_BilayerUpperHead	= m_BilayerCentre;
	}
	else if(m_BLMLowerTotal == 0)
	{
		m_bBLMMonolayer		= true;
		m_BilayerLowerHead	= m_BilayerCentre;
	}
	else
	{
		m_bBLMMonolayer	= false;
	}

	// Check the contents of the cross-links
/*
	for(cPolymerCrossLinkIterator iterLink=m_BLMCrossLinks.begin(); iterLink!=m_BLMCrossLinks.end(); iterLink++)
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

	isBilayer bilayer(m_BLMPolymerTypes, m_BLMUpperFractions,
					  m_BLMX, m_BLMY, m_BLMZ, m_BilayerCentre, m_BLMThickness,
					  m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
					  m_BilayerUpperHead, m_BilayerLowerHead, m_BLMUpperTotal, m_BLMLowerTotal,
					  m_bBLMMonolayer, m_bBLMLinearise, m_bBLMPatches);


	bilayer.Assemble(riState);

	// Calculate the free space available in the reservoir between the bilayer 
	// and the SimBox boundary (or wall if there is one), and use an isRandom 
	// object to fill it with the reservoir polymers. We pass a vector of the 
	// coordinates of the excluded regions so that the polymers can be assigned 
	// coordinates in the free regions.The excluded regions are counted from 
	// the lower SimBox face in the direction of the bilayer normal.

	zDoubleVector exRegions;

	if(m_BLMX == 1)
	{
		if(m_BilayerCentre < m_VesicleXCentre)
		{
			exRegions.push_back(0.0);
			exRegions.push_back(riState.GetWallXWidth());
			exRegions.push_back(m_BilayerLowerHead);
			exRegions.push_back(riState.GetSimBoxXLength());
		}
		else
		{
			exRegions.push_back(0.0);
			exRegions.push_back(m_BilayerUpperHead);
			exRegions.push_back(riState.GetSimBoxXLength() - riState.GetWallXWidth());
			exRegions.push_back(riState.GetSimBoxXLength());
		}
	}
	else if(m_BLMY == 1)
	{
		if(m_BilayerCentre < m_VesicleYCentre)
		{
			exRegions.push_back(0.0);
			exRegions.push_back(riState.GetWallYWidth());
			exRegions.push_back(m_BilayerLowerHead);
			exRegions.push_back(riState.GetSimBoxYLength());
		}
		else
		{
			exRegions.push_back(0.0);
			exRegions.push_back(m_BilayerUpperHead);
			exRegions.push_back(riState.GetSimBoxYLength() - riState.GetWallYWidth());
			exRegions.push_back(riState.GetSimBoxYLength());
		}
	}
	else if(m_BLMZ == 1)
	{
		if(m_BilayerCentre < m_VesicleZCentre)
		{
			// Bilayer is to the left of the vesicle (i.e., has a smaller 
			// z coordinate) so the reservoir extends from the SimBox
			// boundary at Z = 0 to the lower monolayer head position

			exRegions.push_back(0.0);
			exRegions.push_back(riState.GetWallZWidth());
			exRegions.push_back(m_BilayerLowerHead);
			exRegions.push_back(riState.GetSimBoxZLength());
		}
		else
		{
			// Bilayer is to the right of the vesicle (i.e., has a larger
			// z coordinate) so the reservoir extends from the bilayer upper 
			// monolayer head coordinate to the SimBox boundary.

			exRegions.push_back(0.0);
			exRegions.push_back(m_BilayerUpperHead);
			exRegions.push_back(riState.GetSimBoxZLength() - riState.GetWallZWidth());
			exRegions.push_back(riState.GetSimBoxZLength());
		}
	}

	// Fill the reservoir space with randomly-positioned polymers

	isRandom random(m_BLMReservoirTypes, exRegions, m_BLMX, m_BLMY, m_BLMZ);

	random.Assemble(riState);

	// Assign coordinates to the beads of non-vesicle polymers among
	// the free CNT cells. First we place the polymers that are specified
	// as being within the vesicle interior, restricting their bead coordinates
	// to lie with the inner radius of the vesicle, then we place the remaining 
	// polymers outside the vesicle. Cells are chosen randomly for each bead which
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
		if(find(m_VInteriorTypes.begin(), m_VInteriorTypes.end(), (*iterPoly)->GetType()) != m_VInteriorTypes.end())
		{
			for(iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
				rCellIndex = static_cast<long>(freeInteriorCells*CCNTCell::GetRandomNo());
				if(rCellIndex == freeInteriorCells)
					rCellIndex = 0;

				rCNTCellIndex = vFreeInteriorCNTCells.at(rCellIndex); // cells inside vesicle

				ix = rCNTCellIndex%CNTXNo;
				iy = ((rCNTCellIndex-ix)/CNTXNo)%CNTYNo;
				iz = ((rCNTCellIndex-ix)/CNTXNo - iy)/CNTYNo;

                xp[0] = riState.GetCNTXCellWidth()*(ix + CCNTCell::GetRandomNo());
				xp[1] = riState.GetCNTYCellWidth()*(iy + CCNTCell::GetRandomNo());
				xp[2] = riState.GetCNTZCellWidth()*(iz + CCNTCell::GetRandomNo());

				(*iterBead)->SetXPos(xp[0]);
				(*iterBead)->SetYPos(xp[1]);
				(*iterBead)->SetZPos(xp[2]);
			}
		}
		else if(find(m_VExteriorTypes.begin(), m_VExteriorTypes.end(), (*iterPoly)->GetType()) != m_VExteriorTypes.end())
		{
		    bFirstBead = true;
			
			for(iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
			    if(bFirstBead)
				{
				    bFirstBead = false;
				
				    // Pick the cellcontaining the first bead randomly from those outside the vesicle. This avoids 
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

		// Check that pairs of beads that are bonded together are not too far apart 
		// that they generate huge bond forces. This arises because their positions 
		// are just randomly selected from the free space and for boxes of 
		// 20 x 20 x 20 or more this can lead to large separations. If they are 
		// further apart than "rbond", move them along the line joining their centres 
		// so that they are only unit distance apart.

		if(find(m_VPolymerTypes.begin(), m_VPolymerTypes.end(), (*iterPoly)->GetType()) == m_VPolymerTypes.end())
		{
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
	// Set the velocities of the beads from a 3d Maxwell distribution 
	// with a mean  value and variance fixed from the temperature. This applies 
	// to all beads not just the ones in the vesicle. We indicate that a 3d
	// distribution is required by passing in a zero normal vector.

	AssignMaxwellVelocities(riState, 0, 0, 0);

	return bSuccess;
}
