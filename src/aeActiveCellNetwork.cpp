/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeActiveCellNetwork.cpp: implementation of the aeActiveCellNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimMathFlags.h"
#include "aeActiveCellNetwork.h"
#include "aeCNTCell.h"
#include "CNTCell.h"
#include "aeActiveBond.h"
#include "aeActivePolymer.h"
#include "aevActiveEvent.h"
#include "aevPolymerForms.h"
#include "aevPolymerDissolves.h"
#include "aevBondBindsToPolymerHead.h"
#include "aevBondBindsToPolymerTail.h"
#include "aevBondHydrolysesATP.h"
#include "aevBondReleasesPi.h"
#include "aevBondPhosphorylation.h"
#include "IActiveSimBox.h"
#include "IACNAccessControl.h"
#include "aeActiveBond1dProfile.h"
#include "taEventSourceDecorator.h"
#include "LogTextMessage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aeActiveCellNetwork::aeActiveCellNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn,
					double xw, double yw, double zw) : m_pIACNAccess(0),
							m_PolymerFormsTotal(0), m_PolymerDissolvesTotal(0), 
							m_pShadow(pShadow),
							m_pPolymerFormsEvent(0),
						        m_pPolymerDissolvesEvent(0),
							m_pBondBindsToPolymerHeadEvent(0),
														  m_pBondBindsToPolymerTailEvent(0),
														  m_pBondUnbindsFromPolymerHeadEvent(0),
														  m_pBondUnbindsFromPolymerTailEvent(0),
														  m_CNTXCellNo(xn),
														  m_CNTYCellNo(yn),
														  m_CNTZCellNo(zn),
														  m_CNTXYCellTotal(xn*yn),
														  m_CNTXCellWidth(xw),
														  m_CNTYCellWidth(yw),
														  m_CNTZCellWidth(zw),
														  m_bUpdateFreeBondDisplay(false),
														  m_bUpdateFreePolymerDisplay(false),
														  m_bFreeBondVisible(true),
														  m_bFreePolymerVisible(true),
														  m_bFreePolymerVisibleMinLength(false),
														  m_FreePolymerDisplayMinLength(0),
														  m_bRestrictPolymers(false),
                                                          m_MaxPolymers(0),
														  m_bPhantomBonds(false),
                                                          m_ConstantFreeBondNumber(0),
                                                          m_PhantomBondsPerEvent(0),
														  m_MeanFilamentSize(0.0),
														  m_MeanSqFilamentSize(0.0),
														  m_SDevFilamentSize(0.0),
														  m_MeanFilamentLength(0.0),
														  m_MeanSqFilamentLength(0.0),
														  m_SDevFilamentLength(0.0),
														  m_MeanFilamentLatSqDisp(0.0),
														  m_VarFilamentLatSqDisp(0.0),
                                                          m_bCumulativeEventStatistics(true)
{

#if SimDimension == 2

	m_CNTCellTotal	= m_CNTXCellNo*m_CNTYCellNo;

#elif SimDimension == 3

	m_CNTCellTotal	= m_CNTXCellNo*m_CNTYCellNo*m_CNTZCellNo;

#endif

	// ****************************************
	// Ensure the local containers are empty

	for(short int i=0; i<3; i++)
	{
		m_MeanFilamentCM[i]     = 0.0;
		m_MeanFilamentSqDisp[i] = 0.0;
		m_MeanFilamentHead[i]   = 0.0;
		m_MeanFilamentTail[i]   = 0.0;
		m_VarFilamentSqDisp[i]  = 0.0;
		m_VarFilamentHead[i]    = 0.0;
		m_VarFilamentTail[i]    = 0.0;
	}

	m_Events.clear();
	m_FreeActiveBonds.clear();
        m_FreePhantomBonds.clear();
	m_FreeActivePolymers.clear();
	m_DensityFields.clear();
        m_EventSuccessCounters.clear();
        m_EventFailureCounters.clear();

    // Make sure the event (name, period) and event source maps are empty

    m_mEventExecutionPeriodFromName.clear();
    m_mEventSourceFromType.clear();

	// Construct the CNT cell network and partition the active bonds amongst the cells.
	// We use a static member function of the aeCNTCell class to allow the cells to 
	// access their size without needed to store the data in every cell instance.
	// This function must be called before MakeCNTCells().

	m_vCNTCells.clear();
	m_vCNTCells.reserve(m_CNTCellTotal);

	aeCNTCell::SetSimBoxLengths(m_CNTXCellNo,    m_CNTYCellNo,    m_CNTZCellNo,
								m_CNTXCellWidth, m_CNTYCellWidth, m_CNTZCellWidth);

	MakeCNTCells();
}

aeActiveCellNetwork::~aeActiveCellNetwork()
{
	// Delete the exemplar events used by this network

	if(m_pPolymerFormsEvent)
	{
		delete m_pPolymerFormsEvent;
	}

	if(m_pPolymerDissolvesEvent)
	{
		delete m_pPolymerDissolvesEvent;
	}

	if(m_pBondBindsToPolymerHeadEvent)
	{
		delete m_pBondBindsToPolymerHeadEvent;
	}

	if(m_pBondBindsToPolymerTailEvent)
	{
		delete m_pBondBindsToPolymerTailEvent;
	}

	if(m_pBondUnbindsFromPolymerHeadEvent)
	{
		delete m_pBondUnbindsFromPolymerHeadEvent;
	}

	if(m_pBondUnbindsFromPolymerTailEvent)
	{
		delete m_pBondUnbindsFromPolymerTailEvent;
	}

	// Delete all events owned by this network: note that active events
	// are transient so they are not stored in the CAnalysisState object.
	// Also note that dependent events are destroyed in their containing
	// event so we don't try and delete them here.

	if(!m_Events.empty())
	{
		for(ActiveEventListIterator iterEvent=m_Events.begin(); iterEvent!=m_Events.end(); iterEvent++)
		{
			if(!(*iterEvent)->IsDependent())
			{
				delete *iterEvent;
			}
		} 
		m_Events.clear();
	}

	// Delete all CNT cells created in the MakeCNTCells function. 

	if(!m_vCNTCells.empty())
	{
		for(aeCNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
		{
			delete (*iterCell);
		} 
		m_vCNTCells.clear();
	}

	// Delete the active bonds and polymers that may have been created by command.
	// Note that the bonds that have been joined into polymers are destroyed by
	// their containing polymers. Also, as all of the free active bonds in the 
    // the m_FreePhantomBonds container are also in the m_FreeActiveBonds container
    // we must not delete its contents.

	if(!m_FreeActiveBonds.empty())
	{
		for(ActiveBondListIterator iterBond=m_FreeActiveBonds.begin(); iterBond!=m_FreeActiveBonds.end(); iterBond++)
		{
			delete *iterBond;
		} 
		m_FreeActiveBonds.clear();
	}

	if(!m_FreeActivePolymers.empty())
	{
		for(ActivePolymerListIterator iterPolymer=m_FreeActivePolymers.begin(); iterPolymer!=m_FreeActivePolymers.end(); iterPolymer++)
		{
			delete *iterPolymer;
		} 
		m_FreeActivePolymers.clear();
	}

	// Delete any density fields created during analysis of the spatial 
	// distribution  of the active bonds and polymers

	if(!m_DensityFields.empty())
	{
		for(DensityFieldIterator iterDens=m_DensityFields.begin(); iterDens!=m_DensityFields.end(); iterDens++)
		{
			delete *iterDens;
		} 
		m_DensityFields.clear();
	}
}

// Function to assemble aeCNTCells into a network that spans the whole CSimBox,
// and allows dynamically-created active bonds to be tracked as they evolve.
// Note that the number of cells in each dimension does not have to be the same
// as the number of CNT cells in the CSimBox.

void aeActiveCellNetwork::MakeCNTCells()
{
	long index, coords[3];
	bool flag;

	aeCNTCell* pCell;

	for( long k=0; k<m_CNTZCellNo; k++ )
	{
		for( long j=0; j<m_CNTYCellNo; j++ )
		{
			for( long i=0; i<m_CNTXCellNo; i++ )
			{
				index = m_CNTXCellNo*(m_CNTYCellNo*k+j) + i;
				coords[0] = i;
				coords[1] = j;
				coords[2] = k;

				if( i==0 || i==m_CNTXCellNo-1 ||
					j==0 || j==m_CNTYCellNo-1 ||
					k==0 || k==m_CNTZCellNo-1 )
				{
					flag = true;
				}
				else
				{
					flag = false;
				}

				pCell = new aeCNTCell(index, coords, flag);
				m_vCNTCells.push_back(pCell);
			}
		}
	}

	// Having created the cells we now link them into the nearest-neighbour
	// cell network. We map the coordinates of the cell (i,j,k) plus the change due
	// to a bead crossing cell boundaries (alpha,beta,gamma) into the 1D function 
	// given by the cell id's. There are 27 different directions (including the 
	// case of no change in the cell holding the bead) and each must be 
	// calculated for each cell. We use a helper function that maps the set
	// (gamma,beta,alpha) into [0,26] to allow storage as a 1d array of indices.
	//
	// We also store a separate array of cell pointers for those NN cells to be
	// used in calculating forces between beads. 

	for(aeCNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		long nnIntIndex = 0;

		for( long gamma=-1; gamma<2; gamma++ )
		{
			for( long  beta=-1;  beta<2;  beta++ )
			{
				for( long alpha=-1; alpha<2; alpha++ )
				{
					long newIndex = MapCelltoIndex( (*iterCell)->GetBLXIndex(), 
											   (*iterCell)->GetBLYIndex(), 
											   (*iterCell)->GetBLZIndex(), 
												alpha, beta, gamma );
					aeCNTCell* pNNCell  = m_vCNTCells.at(newIndex);

					// map nn index to cell pointer
					long nnIndex  = MapCelltoNNIndex( alpha, beta, gamma );
					(*iterCell)->SetNNCellIndex( nnIndex, pNNCell );

					// map nn index to cell pointer for interacting cells.
					// These are those cells to the right and above the current 
					// cell in 2d and equivalently in 3d.

					if( gamma==1 ||
					   (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
					{
						(*iterCell)->SetIntNNCellIndex( nnIntIndex, pNNCell );
						nnIntIndex++;
					}
				}
			}
		}
	} 

}

// Function to map the 3D cell coordinates (i,j,k) into a 1D index function when
// a bead moves from cell (i,j,k) to (i+alpha,j+beta,k+gamma). The index function
// returns an integer that corresponds to the label of the cell in the direction
// defined by the set (alpha,beta,gamma) taking the size of the simulation box,
// and the periodicity of the box into account. The order of storing the cells
// is X,Y,Z, that is, a plane of XY cells is filled first following by the next
// plane in the +Z direction.
//
// Also note that this function works for 2D and 3D simulations.

long aeActiveCellNetwork::MapCelltoIndex(long i, long j, long k, long alpha, long beta, long gamma) const
{
	long oldIndex, newIndex;

	oldIndex = i + j*m_CNTXCellNo + k*m_CNTXYCellTotal;

// X test
	newIndex = oldIndex + alpha;
	if( newIndex >= m_CNTXCellNo*(1 + j + k*m_CNTYCellNo) )
		newIndex -= m_CNTXCellNo;
	else if( newIndex < m_CNTXCellNo*(j + k*m_CNTYCellNo) )
		newIndex += m_CNTXCellNo;

// Y test
	newIndex += beta*m_CNTXCellNo;
	if( newIndex >= (1 + k)*m_CNTXYCellTotal )
		newIndex -= m_CNTXYCellTotal;
	else if( newIndex < k*m_CNTXYCellTotal )
		newIndex += m_CNTXYCellTotal;

// Z test
#if SimDimension == 3

	newIndex += gamma*m_CNTXYCellTotal;
	if( newIndex >= m_CNTCellTotal )
		newIndex -= m_CNTCellTotal;
	else if( newIndex < 0 )
		newIndex += m_CNTCellTotal;

#endif

	return newIndex;
}

// Function to map the set (alpha,beta,gamma) into a 1D function that gives
// the array index of an NN cell in 2D or 3D.

long aeActiveCellNetwork::MapCelltoNNIndex(long alpha, long beta, long gamma) const
{
	long newIndex;

#if SimDimension == 2

	newIndex = 4 + alpha + 3*beta;

#elif SimDimension == 3

	newIndex = 13 + alpha + 3*beta + 9*gamma;

#endif

	return newIndex;
}

// Function to assign the newly-created active bonds to the appropriate
// cells using the coordinates of their tail monomer's head bead to determine
// which cell they belong to.
//
// This routine should only be called ONCE after creating active bonds as it
// insert the newly-created bonds into the m_FreeActiveBonds container. 

void aeActiveCellNetwork::AssignActiveBondsToCells(ActiveBondList lFreeActiveBonds)
{
	for(ActiveBondListIterator iterBond=lFreeActiveBonds.begin(); iterBond!=lFreeActiveBonds.end(); iterBond++)
	{
		long ix = static_cast<long>((*iterBond)->GetTailMonomer()->GetHead()->GetXPos()/m_CNTXCellWidth);
		long iy = static_cast<long>((*iterBond)->GetTailMonomer()->GetHead()->GetYPos()/m_CNTYCellWidth);

#if SimDimension == 2
		long iz = 0;
#elif SimDimension == 3
		long iz = static_cast<long>((*iterBond)->GetTailMonomer()->GetHead()->GetZPos()/m_CNTZCellWidth);		
#endif

		const long index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;
		m_vCNTCells[index]->AddActiveBondToCell(*iterBond);
	} 

	// Now store the free active bonds locally so that we can iterate over them

	m_FreeActiveBonds.clear();
	copy(lFreeActiveBonds.begin(), lFreeActiveBonds.end(), back_inserter(m_FreeActiveBonds));
}

// Function to assign free active bonds to the appropriate CNT cell based on the 
// coordinates of the tail monomer's head bead. An active bond always contains 
// a tail monomer, but free active bonds have no head monomer whereas those that 
// are bound into an active polymer also have a head monomer.
//
// This routine does not take account of bonds that are organised into active polymers. 
// It iterates over all the CNT cells, and the bonds contained in each cell, checking
// if each bond should be moved to a neighbouring cell. Note that there is no 
// restriction that bonds can only move to an adjacent cell: they can be transported 
// to any cell in the active SimBox.
//
// We also update the display status of free active bonds and polymers if required.
// It is performed here to avoid every derived class having to implement it, but is
// only performed if necessary to save execution time. Next, any active events
// belonging to this ACN are updated. Finally, we calculate the
// forces due to the bonds between monomers bound into polymers.

void aeActiveCellNetwork::UpdateActiveBondsInCells()
{
	for(aeCNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		const long cellId = (*iterCell)->GetId();

		ActiveBondList lBonds = (*iterCell)->GetBonds();

		for(ActiveBondListIterator iterBond=lBonds.begin(); iterBond!=lBonds.end(); iterBond++)
		{
			long ix = static_cast<long>((*iterBond)->GetTailMonomer()->GetHead()->GetXPos()/m_CNTXCellWidth);
			long iy = static_cast<long>((*iterBond)->GetTailMonomer()->GetHead()->GetYPos()/m_CNTYCellWidth);

	#if SimDimension == 2
			long iz = 0;
	#elif SimDimension == 3
			long iz = static_cast<long>((*iterBond)->GetTailMonomer()->GetHead()->GetZPos()/m_CNTZCellWidth);		
	#endif
	  
			const long index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;

			// Only move the bead if it has changed cells

			if(index != cellId)
			{
				(*iterCell)->RemoveActiveBondFromCell(*iterBond);

				m_vCNTCells[index]->AddActiveBondToCell(*iterBond);
			}
		} 
	}


	// Now update the active events belonging to this ACN: we do this before
	// updating the display status so that any new filaments become visible

	UpdateEvents();

	// Update the display status of bonds and polymers if required

	if(GetFreeBondDisplayUpdateStatus())
	{
		UpdateFreeBondDisplayStatus();
	}

	if(GetFreePolymerDisplayUpdateStatus())
	{
		UpdateFreePolymerDisplayStatus();
	}

	// Finally, iterate over all free active polymers to calculate the forces 
	// that bind the monomers into polymers. These forces are internal to the 
	// polymers and do not affect free monomers or other polymers.

	for(ActivePolymerListIterator iterPoly=m_FreeActivePolymers.begin(); iterPoly!=m_FreeActivePolymers.end(); iterPoly++)
	{
		(*iterPoly)->AddForce();
	}
}

// Function to return a pointer to an active cell given a set of (x,y,z) coordinates.

aeCNTCell* aeActiveCellNetwork::GetCellFromIndex(double x, double y, double z) const
{
	const long ix = static_cast<long>(x/m_CNTXCellWidth);
	const long iy = static_cast<long>(y/m_CNTYCellWidth);

	#if SimDimension == 2
		const long iz = 0;
	#elif SimDimension == 3
		const long iz = static_cast<long>(z/m_CNTZCellWidth);		
	#endif

	const long index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;

	return m_vCNTCells[index];
}

// Function to return a pointer to an active cell that contains a specified
// active bond.  We use the coordinates of the bond's tail monomer's head bead
// to identify which cell the bond belongs to.

aeCNTCell* aeActiveCellNetwork::GetCellFromBond(const aeActiveBond *const pBond) const
{
	const double x = pBond->GetTailHeadBead()->GetXPos();
	const double y = pBond->GetTailHeadBead()->GetYPos();
	const double z = pBond->GetTailHeadBead()->GetZPos();

	const long ix = static_cast<long>(x/m_CNTXCellWidth);
	const long iy = static_cast<long>(y/m_CNTYCellWidth);

	#if SimDimension == 2
		const long iz = 0;
	#elif SimDimension == 3
		const long iz = static_cast<long>(z/m_CNTZCellWidth);		
	#endif

	const long index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;

	return m_vCNTCells[index];
}

// ****************************************
// Analysis functions that are independent of the filament type.

// The number of monomers bound into filaments is just the sum
//
//   Nbound = sum over l ( nl * l )
//
// where nl is the number of filaments of length l, and the sum is taken over
// 2,3... infinity. Instead of summing to infinity, the upper limit is set
// by the maximum number of active bonds that may be in each filament.
// This is a property of the aeActivePolymer-derived classes. In fact,
// we just sum over all filaments without ordering them according to their
// length as it is simpler.

long aeActiveCellNetwork::GetBoundBondTotal() const
{
	if(m_FreeActivePolymers.empty())
	{
		return 0;
	}
	else
	{
		long sum = 0;

		for(cActivePolymerListIterator citerPoly=m_FreeActivePolymers.begin(); citerPoly!=m_FreeActivePolymers.end(); citerPoly++)
		{
			sum += (*citerPoly)->GetSize();
		}
		return sum;
	}
}

// The total number of monomers is made up of those bound into filaments 
// and those free in solution.

long aeActiveCellNetwork::GetMonomerTotal() const
{
	long sum = 0;

	sum = GetBoundBondTotal() + GetFreeBondTotal();

	return sum;
}

double aeActiveCellNetwork::GetFreeBondFraction() const
{
	const double freeTotal  = static_cast<double>(GetFreeBondTotal());
	const double boundTotal = static_cast<double>(GetBoundBondTotal());

	double freeBondFraction = 0.0;

	if(boundTotal > 0)
	{
		freeBondFraction = freeTotal/(freeTotal + boundTotal);
	}
	else
	{
		freeBondFraction = 1.0;
	}

	return freeBondFraction;
}

double aeActiveCellNetwork::GetBoundBondFraction() const
{
	return 1.0 - GetFreeBondFraction();
}

// Function to calculate the generic properties of the filaments without regard
// to their actual type.
//
// The mean filament size is just the sum over all filaments of the
// fraction of filaments with n monomers times n. To simplify
// the sum, we just sum over each filament separately instead of sorting 
// them first.
//
// The filament length is measured from the position of the first bond's tail bead 
// to the last bond's tail bead. This gives the actual end-to-end separation, not
// the contour length of the filament. Note that we use the PBC coordinates
// to calculate the distance and apply tests to check if a filament has crossed
// the SimBox boundaries. We cannot use the non-PBC coordinates for dynamic filaments
// because the coordinates of the monomers that are bound in the filament may not
// be in the same periodic image of the SimBox.

double aeActiveCellNetwork::CalculateFilamentProperties()
{
	m_MeanFilamentSize   = 0.0;
	m_MeanSqFilamentSize = 0.0;
	m_SDevFilamentSize   = 0.0;

	m_MeanFilamentLength   = 0.0;
	m_MeanSqFilamentLength = 0.0;
	m_SDevFilamentLength   = 0.0;

	if(GetFilamentTotal() > 0)
	{
		for(cActivePolymerListIterator citerPoly=m_FreeActivePolymers.begin(); citerPoly!=m_FreeActivePolymers.end(); citerPoly++)
		{
			m_MeanFilamentSize += (*citerPoly)->GetSize();
			m_MeanSqFilamentSize += (*citerPoly)->GetSize()*(*citerPoly)->GetSize();
			
			const CAbstractBead* const pFirstBead = (*citerPoly)->GetTailBond()->GetTailTailBead();
			const CAbstractBead* const pLastBead  = (*citerPoly)->GetHeadBond()->GetTailTailBead();
			

			double dx = pLastBead->GetXPos() - pFirstBead->GetXPos();
			double dy = pLastBead->GetYPos() - pFirstBead->GetYPos();
			double dz = pLastBead->GetZPos() - pFirstBead->GetZPos();

//            std::cout << "bead pointers " << pFirstBead << " " << pLastBead << " " << dx << " " << dy << " " << dz << zEndl;

			if( dx > GetIActiveSimBox()->GetHalfSimBoxXLength() )
				dx = dx - GetIActiveSimBox()->GetSimBoxXLength();
			else if( dx < -GetIActiveSimBox()->GetHalfSimBoxXLength() )
				dx = dx + GetIActiveSimBox()->GetSimBoxXLength();

			if( dy > GetIActiveSimBox()->GetHalfSimBoxYLength() )
				dy = dy - GetIActiveSimBox()->GetSimBoxYLength();
			else if( dy < -GetIActiveSimBox()->GetHalfSimBoxYLength() )
				dy = dy + GetIActiveSimBox()->GetSimBoxYLength();

#if SimDimension == 3
			if( dz > GetIActiveSimBox()->GetHalfSimBoxZLength() )
				dz = dz - GetIActiveSimBox()->GetSimBoxZLength();
			else if( dz < -GetIActiveSimBox()->GetHalfSimBoxZLength() )
				dz = dz + GetIActiveSimBox()->GetSimBoxZLength();
#else
			dz = 0.0;
#endif

			const double sep = sqrt(dx*dx + dy*dy + dz*dz);
			
			m_MeanFilamentLength += sep;
			m_MeanSqFilamentLength += sep*sep;
		}

		m_MeanFilamentSize     /= static_cast<double>(GetFilamentTotal());
		m_MeanSqFilamentSize   /= static_cast<double>(GetFilamentTotal());
		m_MeanFilamentLength   /= static_cast<double>(GetFilamentTotal());
		m_MeanSqFilamentLength /= static_cast<double>(GetFilamentTotal());

		if(m_MeanSqFilamentSize > m_MeanFilamentSize*m_MeanFilamentSize)
		{
			m_SDevFilamentSize = sqrt(m_MeanSqFilamentSize - m_MeanFilamentSize*m_MeanFilamentSize);
		}

		if(m_MeanSqFilamentLength > m_MeanFilamentLength*m_MeanFilamentLength)
		{
			m_SDevFilamentLength = sqrt(m_MeanSqFilamentLength - m_MeanFilamentLength*m_MeanFilamentLength);
		}
	}


	return m_MeanFilamentSize;
}

// Function to calculate the distribution of transverse displacements of the
// CM, head monomers and tail monomers, and all monomers of all filaments 
// for use in calculating their bending stiffness when  they are placed 
// in a cylindrically-symmetric potential. The histogram of the displacements 
// allows the filaments' bending stiffness to be determined.
//
// To represent the filaments' head and tail monomers we use the H, M, and T beads
// of each tail monomer (ie, CPolymer) of the terminal active bonds. We know these
// are defined for any molecular architecture of the monomers.
// 
// Note that we do not check that the filaments have constant length, but just 
// use the first and last monomers. For accurate use of this function, 
// the filaments should have a constant length. For simplicity, we also do not 
// check that the beads belonging to head and tail monomers do not overlap the PBCs. 
// For the calculation of the bending stiffness, the filament ought to be aligned 
// near the centre of the SimBox, and its ends should not be too close the 
// SimBox boundaries.

void aeActiveCellNetwork::CalculateFilamentInCylindricalPotentialProperties()
{
	if(GetFilamentTotal() > 0)
	{
		short int i = 0; // Counter used several times below

		// Set all observables to zero each time this function is called

		m_MeanFilamentLatSqDisp = 0.0;
		m_VarFilamentLatSqDisp  = 0.0;

		double meanHeadBeads[3];
		double meanTailBeads[3];

		for(i=0; i<3; i++)
		{
			m_MeanFilamentCM[i]     = 0.0;
			m_MeanFilamentSqDisp[i] = 0.0;
			m_MeanFilamentHead[i]   = 0.0;
			m_MeanFilamentTail[i]   = 0.0;
			m_VarFilamentSqDisp[i]  = 0.0;
			m_VarFilamentHead[i]    = 0.0;
			m_VarFilamentTail[i]    = 0.0;

			meanHeadBeads[i] = 0.0;
			meanTailBeads[i] = 0.0;
		}

		double meanPolymerLatRSQ = 0.0; // Running average over all polymers
		double varPolymerLatRSQ = 0.0;  // Running average over all polymers

		for(cActivePolymerListIterator citerPoly=m_FreeActivePolymers.begin(); citerPoly!=m_FreeActivePolymers.end(); citerPoly++)
		{
			const CPolymer* pHeadMonomer = (*citerPoly)->GetHeadBond()->GetTailMonomer();
			const CPolymer* pTailMonomer = (*citerPoly)->GetTailBond()->GetTailMonomer();

			const CAbstractBead* const pHeadHBead = pHeadMonomer->GetHead();
			const CAbstractBead* const pHeadMBead = pHeadMonomer->GetHeadAdjacentBead();
			const CAbstractBead* const pHeadTBead = pHeadMonomer->GetTail();
			const CAbstractBead* const pTailHBead = pTailMonomer->GetHead();
			const CAbstractBead* const pTailMBead = pTailMonomer->GetHeadAdjacentBead();
			const CAbstractBead* const pTailTBead = pTailMonomer->GetTail();

			meanHeadBeads[0] += pHeadHBead->GetXPos() + pHeadMBead->GetXPos() + pHeadTBead->GetXPos();
			meanHeadBeads[1] += pHeadHBead->GetYPos() + pHeadMBead->GetYPos() + pHeadTBead->GetYPos();
			meanHeadBeads[2] += pHeadHBead->GetZPos() + pHeadMBead->GetZPos() + pHeadTBead->GetZPos();

			meanTailBeads[0] += pTailHBead->GetXPos() + pTailMBead->GetXPos() + pTailTBead->GetXPos();
			meanTailBeads[1] += pTailHBead->GetYPos() + pTailMBead->GetYPos() + pTailTBead->GetYPos();
			meanTailBeads[2] += pTailHBead->GetZPos() + pTailMBead->GetZPos() + pTailTBead->GetZPos();

			for(i=0; i<3; i++)
			{
				meanHeadBeads[i] = 0.33333333*meanHeadBeads[i];
				meanTailBeads[i] = 0.33333333*meanTailBeads[i];

				m_MeanFilamentHead[i] += meanHeadBeads[i];
				m_MeanFilamentTail[i] += meanTailBeads[i];

				m_VarFilamentHead[i] += meanHeadBeads[i]*meanHeadBeads[i];
				m_VarFilamentTail[i] += meanTailBeads[i]*meanTailBeads[i];
			}

			// Now calculate the mean lateral displacement averaged over all 
			// monomers in one filament, and then average over all filaments.
			// Note that we expect this routine to be most useful for a single
			// filament, but we make it generally applicable.

			double monomerLatRSQ = 0.0; // Mean square lateral displacement of one monomer
			double monoCM[3];           // CM of a single monomer
			double filamentCM[3];        // Mean CM of all monomers/filament
			double filamentRSQ[3];       // Mean square displacement of all monomers/filament

			for(i=0; i<3; i++)
			{
				monoCM[i]      = 0.0;
				filamentCM[i]  = 0.0;
				filamentRSQ[i] = 0.0;
			}

			ActiveBondList localBonds = (*citerPoly)->GetBonds();

			for(cActiveBondListIterator citerBond2=localBonds.begin(); citerBond2!=localBonds.end(); citerBond2++)
			{
				const aeActiveBond* pBond = *citerBond2;

				const CAbstractBead* const pHBead = pBond->GetTailMonomer()->GetHead();
				const CAbstractBead* const pMBead = pBond->GetTailMonomer()->GetHeadAdjacentBead();
				const CAbstractBead* const pTBead = pBond->GetTailMonomer()->GetTail();

				monoCM[0] = 0.33333333*(pHBead->GetXPos() + pMBead->GetXPos() + pTBead->GetXPos());
				monoCM[1] = 0.33333333*(pHBead->GetYPos() + pMBead->GetYPos() + pTBead->GetYPos());
				monoCM[2] = 0.33333333*(pHBead->GetZPos() + pMBead->GetZPos() + pTBead->GetZPos());

				// Calculate the squared displacements and add them to the 
				// running totals. NOTE that in calculating the lateral displacement
				// we assume the filament is aligned along the Z axis!

				filamentCM[0] += monoCM[0];
				filamentCM[1] += monoCM[1];
				filamentCM[2] += monoCM[2];

				filamentRSQ[0] += monoCM[0]*monoCM[0];
				filamentRSQ[1] += monoCM[1]*monoCM[1];
				filamentRSQ[2] += monoCM[2]*monoCM[2];

                monomerLatRSQ = monoCM[0]*monoCM[0] + monoCM[1]*monoCM[1];
			}

			for(i=0; i<3; i++)
			{
				filamentCM[i] /= static_cast<double>((*citerPoly)->GetSize());
				filamentRSQ[i] /= static_cast<double>((*citerPoly)->GetSize());

				m_MeanFilamentCM[i]     +=  filamentCM[i];
				m_MeanFilamentSqDisp[i] += (filamentRSQ[i] - filamentCM[i]*filamentCM[i]);
				m_VarFilamentSqDisp[i]  += ((filamentRSQ[i] - filamentCM[i]*filamentCM[i])*(filamentRSQ[i] - filamentCM[i]*filamentCM[i]));
			}

			meanPolymerLatRSQ += monomerLatRSQ;
			varPolymerLatRSQ  += monomerLatRSQ*monomerLatRSQ;
		}

		const double invnfil = 1.0/m_FreeActivePolymers.size();

		for(i=0; i<3; i++)
		{
			m_MeanFilamentCM[i]     = invnfil*m_MeanFilamentCM[i];
			m_MeanFilamentSqDisp[i] = invnfil*m_MeanFilamentSqDisp[i];
            m_VarFilamentSqDisp[i]  = invnfil*m_VarFilamentSqDisp[i];

			m_MeanFilamentHead[i] = invnfil*m_MeanFilamentHead[i];
			m_MeanFilamentTail[i] = invnfil*m_MeanFilamentTail[i];
			m_VarFilamentHead[i]  = invnfil*m_VarFilamentHead[i];
			m_VarFilamentTail[i]  = invnfil*m_VarFilamentTail[i];

			m_VarFilamentHead[i] -= m_MeanFilamentHead[i]*m_MeanFilamentHead[i];
			m_VarFilamentTail[i] -= m_MeanFilamentTail[i]*m_MeanFilamentTail[i];
		}

		m_MeanFilamentLatSqDisp = invnfil*meanPolymerLatRSQ;  
		m_VarFilamentLatSqDisp  = invnfil*varPolymerLatRSQ;  
	}
}

// Function to check if the calling instance can be granted access to the ACN's
// internal state. If the derived class implements the IACNAccessControlInterface
// request is passed down to it, otherwise access is denied.

bool aeActiveCellNetwork::IsAccessControlEnabled() const
{
	if(m_pIACNAccess)
	{
		return m_pIACNAccess->IsAccessControlEnabled();
	}
	else
		return false;
}

// Protected function to allow derived classes access to the IActiveSimBox interface
// of the aeActiveSimBox. This is used when an ACN wants to gain access to another
// ACN so that they can interact.

IActiveSimBox* aeActiveCellNetwork::GetIActiveSimBox()
{
	return m_pShadow;
}

// Protected function to allow ACNs to toggle on and off the display of their 
// free bonds in current state snapshots. The current display status is stored
// in a private flag in this class.

void aeActiveCellNetwork::UpdateFreeBondDisplayStatus()
{
	for(ActiveBondListIterator iterBond=m_FreeActiveBonds.begin(); iterBond!=m_FreeActiveBonds.end(); iterBond++)
	{
		(*iterBond)->SetVisible(m_bFreeBondVisible);
	}
}

// Protected function to allow ACNs to toggle the display of their free polymers
// (containing bound bonds) in current state snapshots on and off. The current 
// display status is stored in a private flag in this class.
// Each of the local flags is checked to see if its associated test should be 
// performed on the polymers.

void aeActiveCellNetwork::UpdateFreePolymerDisplayStatus()
{
	for(ActivePolymerListIterator iterPoly=m_FreeActivePolymers.begin(); iterPoly!=m_FreeActivePolymers.end(); iterPoly++)
	{
		bool bGlobalVisible = m_bFreePolymerVisible;

		if(bGlobalVisible && m_bFreePolymerVisibleMinLength)
		{
			bGlobalVisible = ((*iterPoly)->GetSize() >= m_FreePolymerDisplayMinLength);
		}

		// Add further polymer tests here

		(*iterPoly)->SetVisible(bGlobalVisible);
	}
}

// Protected function to set the display status of free polymers according to
// their length, ie, number of monomers. Note that a value of 0 for the 
// minimum length means that the test is turned off. Negative values are
// forbidden.

void aeActiveCellNetwork::SetFreePolymerDisplayByMinLength(long minSize)
{
	if(minSize > 0)
	{
		m_bFreePolymerVisibleMinLength = true;
		m_FreePolymerDisplayMinLength = minSize;
	}
	else
	{
		m_bFreePolymerVisibleMinLength = false;
		m_FreePolymerDisplayMinLength = 0;
	}

	UpdateFreePolymerDisplayStatus();
}

// Function to add a bond to an ACN's free bond container and update its
// display status if required.

void aeActiveCellNetwork::AddFreeBond(aeActiveBond* const pBond)
{
	if(GetFreeBondDisplayUpdateStatus())
	{
		pBond->SetVisible(GetFreeBondDisplayStatus());
	}

	m_FreeActiveBonds.push_back(pBond);
}

// Function to remove a bond from the ACN's free bond container when it binds to 
// an active polymer. If the ACN is set to check the display status of its bonds, 
// it first updates the bond's display status to that appropriate for bound bonds. 
// It takes an aeActiveBond pointer as argument and uses the STL remove() function.
// This can be compared with the RemoveFreeBondByIterator function that uses
// iterators to erase a free bond. 

void aeActiveCellNetwork::RemoveFreeBond(aeActiveBond* const pBond)
{
	if(GetFreeBondDisplayUpdateStatus())
	{
		pBond->SetVisible(GetFreePolymerDisplayStatus());
	}

	m_FreeActiveBonds.remove(pBond);
}

// Second function to remove a bond from the ACN's free bond container and update
// its display status. This function takes an ActiveBondListIterator and returns
// an iterator to the next element in the container. Because the iterator is
// invalidated after the erase() function is called, we update the bond's display
// status prior to removing the bond from the container.

ActiveBondListIterator aeActiveCellNetwork::RemoveFreeBondByIterator(ActiveBondListIterator iterBond)
{
	if(GetFreeBondDisplayUpdateStatus())
	{
		(*iterBond)->SetVisible(GetFreePolymerDisplayStatus());
	}

	ActiveBondListIterator nextBond = m_FreeActiveBonds.erase(iterBond);

	return nextBond;
}

// Function to add a polymer to an ACN's free polymer container, and update
// its display status if required.

void aeActiveCellNetwork::AddFreePolymer(aeActivePolymer* const pPolymer)
{
//    std::cout << "Adding a new free polymer " << pPolymer << " " << pPolymer->GetId() << " " << pPolymer->GetSize() << " " << pPolymer->GetHeadBond()->GetId() << " " << pPolymer->GetTailBond()->GetId() <<  " previous total " << m_FreeActivePolymers.size() << zEndl;
	
	if(GetFreePolymerDisplayUpdateStatus())
	{
//	    std::cout << "Updating display status for free polymer " << pPolymer->GetId() << zEndl;
		pPolymer->SetVisible(GetFreePolymerDisplayStatus());
	}

	m_FreeActivePolymers.push_back(pPolymer);
}

// Function to remove a polymer from the ACN's free polymer container when 
// it dissociates.. It takes an aeActivePolymer pointer as argument and 
// uses the STL remove() function. We do not update its display status as the
// calling routine determines if the polymer is dissolved into its component
// bonds, in which case the polymer is destroyed, or it is modified in some
// other way.

void aeActiveCellNetwork::RemoveFreePolymer(aeActivePolymer* const pPolymer)
{
	m_FreeActivePolymers.remove(pPolymer);
}

// Function to add the calculation of a 1-dimensional density profile of an 
// active bond species along a specified direction through the SimBox. Note 
// that we assume all the input data has been validated before this routine is 
// called. Hence, we do not check for a commensurate sample and analysis period, 
// valid normal vector, etc. This function turns on the analysis and increments
// the counter for the density fields so that the results are accumulated
// during the analysis time period.

void aeActiveCellNetwork::Add1dDensityField(long start, long end, 
										    long sample, long analysis,
										    long nx, long ny, long nz, long sliceTotal,
										    double lowerCoord, double upperCoord)
{
	// Check that the current time is still before the temporal range for analysis.
	// Note that because we have ensured that end >= start, we don't have to check
	// if the current time is less than the end time.
	// We don't check the sample and analysis periods for now.

	if(GetCurrentTime() <= start)
	{
		// Create the CDensityField1d class to calculate the profile and pass it a
		// vector of the active bonds that are to be analysed. Note that this
		// includes all bonds of the specified type, even if not all will not 
		// contribute to the profile because of a restricted coordinate range.
		// We pass in the runId and current time to label the output file.

		aeActiveBond1dProfile* p1dProfile = new aeActiveBond1dProfile(GetRunId(), GetCurrentTime(), GetACNType(), m_FreeActiveBonds, start, end, sample, nx, ny, nz, sliceTotal, GetSimBoxXLength(), GetSimBoxYLength(), GetSimBoxZLength(), lowerCoord, upperCoord);

		m_DensityFields.push_back(p1dProfile);
	}
}

void aeActiveCellNetwork::Add2dDensityField()
{

}

void aeActiveCellNetwork::Add3dDensityField()
{

}

// Function to calculate the density profiles of active bonds during the 
// time period specified in the profile commands. The calculation is done
// until the simulation time is equal to the end time. We do not include
// the final time step as this leads to n+1 samples instead of n, where n is the
// interval end - start.
//
// We iterate over all profiles and check if the current time falls within
// their analysis times. Once its end time is passed, a profile is removed
// from the analysis by removing it from the profiles container. When there
// are no profiles in the container, this function is no longer called.

void aeActiveCellNetwork::CalculateDensityFields()
{
	DensityFieldIterator iterDens=m_DensityFields.begin();

	while(iterDens != m_DensityFields.end())
	{
		if(GetCurrentTime() < (*iterDens)->GetEndTime())
		{
			if(GetCurrentTime() >= (*iterDens)->GetStartTime())
			{
				(*iterDens)->Sample();

				(*iterDens)->Serialize();
			}

			iterDens++;
		}
		else
		{
			iterDens = m_DensityFields.erase(iterDens);
		}
	}

}

// ****************************************
// Functions relating to the handling of active events.

// Function to create an instance of a named event type. It is used by
// derived classes to instantiate the events they need. The newly-created
// events are stored in the ACN's event container. If the type is unrecognised,
// we return a NULL pointer without storing it in the events container.

aevActiveEvent* aeActiveCellNetwork::AddEvent(const zString type)
{
	aevActiveEvent* const pEvent = acfActiveEventFactory::Instance()->Create(type);

	if(pEvent)
	{
		m_Events.push_back(pEvent);
	}

	return pEvent;
}

// Function to remove an event from the ACN's container and destroy the event
// instance.

void aeActiveCellNetwork::DeleteEvent(aevActiveEvent* pEvent)
{
	RemoveEvent(pEvent);

	delete pEvent;
}

// Function to remove an event from the ACN's event container without destroying
// the event itself.

void aeActiveCellNetwork::RemoveEvent(aevActiveEvent* pEvent)
{
	ActiveEventListIterator iterEvent = find(m_Events.begin(), m_Events.end(), pEvent);

	if(iterEvent != m_Events.end())
	{
		m_Events.erase(iterEvent);
	}
}

// Protected function to allow concrete ACN classes to update the state of 
// their events. This only applies to active events that are owned by an ACN.
//
// The ACN iterates over its events allowing each one to check if their
// pre-conditions are satisfied and to take the appropriate action. The 
// aevActiveEvent::Execute() function returns a bool so we can see if an event 
// should continue executing or be destroyed.

void aeActiveCellNetwork::UpdateEvents()
{
	if(!m_Events.empty())
	{
		ActiveEventListIterator iterEvent=m_Events.begin();

		while(iterEvent != m_Events.end())
		{
			if((*iterEvent)->Execute(m_pShadow))
			{
				// Event is still active so move to the next one

				iterEvent++;
			}
			else if(!(*iterEvent)->IsDependent())
			{
				// When the event has achieved its aim we delete it, but only
				// if it is not dependent on (i.e., controlled by) another event

				delete *iterEvent;
				iterEvent = m_Events.erase(iterEvent);
			}
		} 
	}
}

// Function to return the event instance that is being used by the ACN to
// form active polymers out of its monomers. This allows the event's parameters
// to be modified by command.

const zString aeActiveCellNetwork::GetInternalPolymerFormsEventType() const
{
	if(m_pPolymerFormsEvent)
	{
		return m_pPolymerFormsEvent->GetEventType();
	}
	else
	{
		return "";
	}
}

const zString aeActiveCellNetwork::GetInternalPolymerDissolvesEventType() const
{
	if(m_pPolymerDissolvesEvent)
	{
		return m_pPolymerDissolvesEvent->GetEventType();
	}
	else
	{
		return "";
	}
}

const zString aeActiveCellNetwork::GetInternalBondBindsToPolymerHeadEventType() const
{
	if(m_pBondBindsToPolymerHeadEvent)
	{
		return m_pBondBindsToPolymerHeadEvent->GetEventType();
	}
	else
	{
		return "";
	}
}

const zString aeActiveCellNetwork::GetInternalBondBindsToPolymerTailEventType() const
{
	if(m_pBondBindsToPolymerTailEvent)
	{
		return m_pBondBindsToPolymerTailEvent->GetEventType();
	}
	else
	{
		return "";
	}
}

const zString aeActiveCellNetwork::GetInternalBondUnbindsFromPolymerHeadEventType() const
{
	if(m_pBondUnbindsFromPolymerHeadEvent)
	{
		return m_pBondUnbindsFromPolymerHeadEvent->GetEventType();
	}
	else
	{
		return "";
	}
}

const zString aeActiveCellNetwork::GetInternalBondUnbindsFromPolymerTailEventType() const
{
	if(m_pBondUnbindsFromPolymerTailEvent)
	{
		return m_pBondUnbindsFromPolymerTailEvent->GetEventType();
	}
	else
	{
		return "";
	}
}


aevActiveEvent* aeActiveCellNetwork::GetInternalPolymerFormsEvent()
{
	return m_pPolymerFormsEvent;
}

aevActiveEvent* aeActiveCellNetwork::GetInternalPolymerDissolvesEvent()
{
	return m_pPolymerDissolvesEvent;
}

aevActiveEvent*  aeActiveCellNetwork::GetInternalBondBindsToPolymerHeadEvent()
{
	return m_pBondBindsToPolymerHeadEvent;
}

aevActiveEvent*   aeActiveCellNetwork::GetInternalBondBindsToPolymerTailEvent()
{
	return m_pBondBindsToPolymerTailEvent;
}

aevActiveEvent*   aeActiveCellNetwork::GetInternalBondUnbindsFromPolymerHeadEvent()
{
	return m_pBondUnbindsFromPolymerHeadEvent;
}

aevActiveEvent*   aeActiveCellNetwork::GetInternalBondUnbindsFromPolymerTailEvent()
{
	return m_pBondUnbindsFromPolymerTailEvent;
}

void aeActiveCellNetwork::SetInternalPolymerFormsEvent(aevActiveEvent* const pEvent)
{
	if(m_pPolymerFormsEvent)
	{
		delete m_pPolymerFormsEvent;
		m_pPolymerFormsEvent = 0;
	}

	m_pPolymerFormsEvent = pEvent;
}

void aeActiveCellNetwork::SetInternalPolymerDissolvesEvent(aevActiveEvent* const pEvent)
{
	if(m_pPolymerDissolvesEvent)
	{
		delete m_pPolymerDissolvesEvent;
		m_pPolymerDissolvesEvent = 0;
	}

	m_pPolymerDissolvesEvent = pEvent;
}

void aeActiveCellNetwork::SetInternalBondBindsToPolymerHeadEvent(aevActiveEvent* const pEvent)
{
	if(m_pBondBindsToPolymerHeadEvent)
	{
		delete m_pBondBindsToPolymerHeadEvent;
		m_pBondBindsToPolymerHeadEvent = 0;
	}

	m_pBondBindsToPolymerHeadEvent = pEvent;
}

void aeActiveCellNetwork::SetInternalBondBindsToPolymerTailEvent(aevActiveEvent* const pEvent)
{
	if(m_pBondBindsToPolymerTailEvent)
	{
		delete m_pBondBindsToPolymerTailEvent;
		m_pBondBindsToPolymerTailEvent = 0;
	}

	m_pBondBindsToPolymerTailEvent = pEvent;
}

void aeActiveCellNetwork::SetInternalBondUnbindsFromPolymerHeadEvent(aevActiveEvent* const pEvent)
{
	if(m_pBondUnbindsFromPolymerHeadEvent)
	{
		delete m_pBondUnbindsFromPolymerHeadEvent;
		m_pBondUnbindsFromPolymerHeadEvent = 0;
	}

	m_pBondUnbindsFromPolymerHeadEvent = pEvent;
}

void aeActiveCellNetwork::SetInternalBondUnbindsFromPolymerTailEvent(aevActiveEvent* const pEvent)
{
	if(m_pBondUnbindsFromPolymerTailEvent)
	{
		delete m_pBondUnbindsFromPolymerTailEvent;
		m_pBondUnbindsFromPolymerTailEvent = 0;
	}

	m_pBondUnbindsFromPolymerTailEvent = pEvent;
}


// Command handler function to restrict the number of polymers that the ACN
// will create using aevPolymerForms events. It automatically updates the
// flag indicating the restriction is in effect. Note that 0 is not an
// allowed maximum as it is used to turn off the restriction.

void aeActiveCellNetwork::SetMaxPolymers(long max)
{
	if(max > 0)
	{
		m_bRestrictPolymers = true;
		m_MaxPolymers = max;
	}
	else
	{
		m_bRestrictPolymers = false;
		m_MaxPolymers = 0;
	}
}

// Command handler function to enable the ACN to keep the concentration of free
// bonds constant. It does this by setting a "phantom bond" flag for a given fraction
// of all the active bonds of the specified type. Thereafter, whenever a free bond
// binds to a polymer via a bond-on event, it converts a phantom bond to an active free
// bond, and when a bond leaves a polymer it converts an active free bond back to a
// phantom. The number of free bonds whose phantom flag is toggled per event is 
// user-definable. If the constant fraction is zero, this disables the phantom bonds and
// sets all free bonds back to being active. Note that any bond in a polymer is, by
// definition, active.
//
// If the command is issued more than once, each successive invocation first sets all
// bonds active and then imposes the new constant fraction. If the user specifies a
// constant fraction larger than the previous value, or if there are not enough free
// bonds to satisfy the constant fraction condition, all of the free bonds are set
// active and no phantoms are created. Conversely, if the user specifies that all the 
// free bonds should be phantoms, this effectively prevents any polymers from growing
// further as there are no more phantoms. If an active bond subsequently detaches from
// a filament, it may become active or phantom depending on the fraction specified.

void aeActiveCellNetwork::SetConstantFreeBonds(double constantFraction, long phantomsPerEvent)
{
	if(constantFraction > 0.0 && phantomsPerEvent > 0)
	{
		m_bPhantomBonds = true;
        m_ConstantFreeBondNumber = static_cast<long>(constantFraction*GetMonomerTotal());
		m_PhantomBondsPerEvent = phantomsPerEvent;

        // Convert the required number of bonds to phantoms. Note that this is a fraction
        // of the TOTAL number of active bonds not the fraction of those that are currently
        // free. If some bonds are already in polymers, and there are not enough
        // free bonds left to be converted to the required number of phantoms, then
        // the actual fraction of active free bonds will not be that specified.

        long counter = GetMonomerTotal() - m_ConstantFreeBondNumber;

        m_FreePhantomBonds.clear();
        for(ActiveBondListIterator iterBond=m_FreeActiveBonds.begin(); iterBond!=m_FreeActiveBonds.end(); iterBond++)
	    {
            if(counter > 0)
            {
                counter--;
		        (*iterBond)->SetPhantom(true);
                m_FreePhantomBonds.push_back(*iterBond);
            }
        }

//        std::cout << "Activating phantoms:  total, free, phantom = " << GetMonomerTotal() << "  " << m_ConstantFreeBondNumber << "  " << GetPhantomBondTotal() << zEndl;
	}
	else
	{
		m_bPhantomBonds = false;
        m_ConstantFreeBondNumber = GetMonomerTotal();
		m_PhantomBondsPerEvent = 0;
        
        // Iterate over all bonds (both free and those in polymers) and set their
        // phantom flag to false. Note that we should not need to iterate over those
        // bonds in polymers as they must be active by definition; but we do so for now
        // as a debug check.

        for(ActiveBondListIterator iterBond=m_FreeActiveBonds.begin(); iterBond!=m_FreeActiveBonds.end(); iterBond++)
	    {
		    (*iterBond)->SetPhantom(false);
        }

        for(ActivePolymerListIterator iterPoly=m_FreeActivePolymers.begin(); iterPoly!=m_FreeActivePolymers.end(); iterPoly++)
	    {
            ActiveBondList lBonds = (*iterPoly)->GetBonds();

            for(ActiveBondListIterator iterBond1=lBonds.begin(); iterBond1!=lBonds.end(); iterBond1++)
	        {
    		    (*iterBond1)->SetPhantom(false);
            }
        }
	}
}

// Command handler function to enable the ACN to cancel the constant concentration 
// of free bonds set by any previous command. We reset the phantom flag for all bonds
//.to false. To avoid any possible errors, we iterate over the m_FreeActiveBonds
// container instead of the m_FreePhantomBonds container. This ensures that if any
// active bond is marked as a phantom but accidentally left out of the phantom bonds
// container, it is still reset to active. Note that the phantom bonds container
// contains a subset of the pointers in the active bond container.

void aeActiveCellNetwork::CancelConstantFreeBonds()
{
	m_bPhantomBonds = false;
    m_ConstantFreeBondNumber = 0;
	m_PhantomBondsPerEvent = 0;

    for(ActiveBondListIterator iterBond=m_FreeActiveBonds.begin(); iterBond!=m_FreeActiveBonds.end(); iterBond++)
    {
        (*iterBond)->SetPhantom(false);
    }

    m_FreePhantomBonds.clear();
}

// Function to allow aevPolymerForms events to reduce the event total when
// they are destroyed.

void aeActiveCellNetwork::DecrementPolymerFormsEventTotal()
{
	m_PolymerFormsTotal--;
}

// Virtual function to allow derived classes to attach a process that can
// serialise their data to file. Note that an attached process is still
// owned by the CAnalysisState instance, not by the ACN. Hence, the ACNs
// must not delete their processes.

void aeActiveCellNetwork::AttachProcess(xxProcess* pProcess)
{
	// Do nothing body for now until all ACNS implement it and it becomes a PVF
}


// Function to retrieve the ACN's event counters for output to file.
// Each event keeps track of how many times its activation succeeds and fails.
// The ACN can either accumulate these statistics continuously or output
// a time series of the data by resetting the event counters to zero between
// samples.  For time series output, we clear all counters before the sample
// so that the new data fills them up anew. Because we don't know in advance 
// how many events are being managed, and if active polymers assemble and
// disassemble the number of events varies, doing it this way ensures the 
// counters always have the appropriate size. However, it means that the output 
// may have a variable number of data points.
// 
// If the statistics are being accumulated, the number of events must be constant.
// If the number of events changes, we reset the counters and start accumulating
// data again.
//
// The calling routine needs to know in advance the order of the events to be 
// able to use the data. For example, the aevPolymerForms event
// automatically creates events for the binding and unbinding of monomers to
// the head and tail of the active polymer. Each active polymer contains
// at least 4 events: bond on and off at each end.

void aeActiveCellNetwork::CalculateEventCounters()
{
    // Reset event counters if the appropriate flag indicates that 
    // time series are required instead of cumulative statistics.

    if(IsCumulativeEventStatisticsOn())
    {
        // If the number of events has changed, zero the local counters 
        // and start accumulating the statistics anew.
        // This is necessary because new filaments forming, or old ones
        // disintegrating, changes the number of events being sampled.

        if(GetEventTotal() != static_cast<long>(m_EventSuccessCounters.size()))
        {
           ResetACNEventCounters();

           for(cActiveEventListIterator iterEvent=m_Events.begin(); iterEvent!=m_Events.end(); iterEvent++)
           {
               m_EventSuccessCounters.push_back((*iterEvent)->GetSuccessCounter());
               m_EventFailureCounters.push_back((*iterEvent)->GetFailureCounter());
           }
        }
        else
        {
            // Accumulate statistics
            long eventNo = 0;
            for(cActiveEventListIterator iterEvent=m_Events.begin(); iterEvent!=m_Events.end(); iterEvent++)
            {
                m_EventSuccessCounters.at(eventNo) += ((*iterEvent)->GetSuccessCounter());
                m_EventFailureCounters.at(eventNo) += ((*iterEvent)->GetFailureCounter());
                eventNo++;
            }
        }
    }
    else
    {
        // Time series data: empty the local event counters between samples

        ResetACNEventCounters();

        for(cActiveEventListIterator iterEvent=m_Events.begin(); iterEvent!=m_Events.end(); iterEvent++)
        {
            m_EventSuccessCounters.push_back((*iterEvent)->GetSuccessCounter());
            m_EventFailureCounters.push_back((*iterEvent)->GetFailureCounter());
        }
    }

    // Reset the event counters in both cases as we store the accumulated data here
    // in the ACN.

    ResetAllEventCounters();
}

// Function to return the number of successful activations of a specified event.

long aeActiveCellNetwork::GetEventSuccessCounter(long i) const
{
    if(i >= 0 && i < static_cast<long>(m_EventSuccessCounters.size()))
    {
        return m_EventSuccessCounters.at(i);
    }
    else
    {
        return 0;
    }
}

// Function to return the number of failed activations of a specified event.

long aeActiveCellNetwork::GetEventFailureCounter(long i) const
{
    if(i >= 0 && i < static_cast<long>(m_EventFailureCounters.size()))
    {
        return m_EventFailureCounters.at(i);
    }
    else
    {
        return 0;
    }
}

// Function to return a specific event. Because events are stored in a list,
// we have to iterate through the list to find the required event.

aevActiveEvent* aeActiveCellNetwork::GetEvent(long i)
{
    if(i >= 0 && i < GetEventTotal())
    {
        ActiveEventListIterator iterEvent = m_Events.begin();

        for(long j=0; j<i; j++)
        {
            iterEvent++;
        }

        return *iterEvent;
    }
    else
    {
        return 0;
    }
}

// Command handler function to toggle the event statistics output between
// displaying the cumulative statistics on successes/failures (default case)
// and a time series showing the number of successes/failures in each
// sampling period. In both cases, it sets the local counters and all 
// the event counters to zero to start a new sampling regime.
//
// The function returns the new value of the flag.

bool aeActiveCellNetwork::ToggleCumulativeEventStatisticsHandler()
{
   m_bCumulativeEventStatistics = !m_bCumulativeEventStatistics;

   ResetAllEventCounters();
   ResetACNEventCounters();

   return m_bCumulativeEventStatistics;
}

// Function to reset the success/failure counters for all events to zero.
// This is used by associated processes that want to get a time series of 
// event data instead of accumulating the total numbers. This function
// does not check the status of the cumulative event statistics flag, so 
// it can be used in unusual situations to force the counters to zero.

void aeActiveCellNetwork::ResetAllEventCounters()
{
    for(ActiveEventListIterator iterEvent = m_Events.begin(); iterEvent!=m_Events.end(); iterEvent++)
    {
        (*iterEvent)->ResetAllCounters();
    }
}

// Function to reset the local ACN event counters. This function can be used
// by processes and other analysis routines that want to sample the event
// statistics as a time series instead of accumulating the data. The command
// acToggleCumulativeEventStatistics is used to switch between a time series
// of event stats and accumulation.  Note that when it is in time series mode,
// the ACN does not zero its local stats counters, but it continue to zero
// the event counters.

void aeActiveCellNetwork::ResetACNEventCounters()
{
    m_EventSuccessCounters.clear();
    m_EventFailureCounters.clear();
}

// Function to check the state of all monomers (free and bound) and ensure that
// their internal states are consistent. Especially, we check that the 
// polymerized flag is set appropriate, and that the head bond of free monomers
// is empty.
// 
// Any discrepancy found results in a log message being issued.

bool aeActiveCellNetwork::CheckMonomers()
{
    // Check free monomers first
/*
    zString message;

	for(ActiveBondListIterator iterBond=m_FreeActiveBonds.begin(); iterBond!=m_FreeActiveBonds.end(); iterBond++)
	{

        aeActiveBond* pHeadAdj = (*iterBond)->GetHeadAdjacentBond();
        aeActiveBond* pTailAdj = (*iterBond)->GetTailAdjacentBond();

	CPolymer* pHead = (*iterBond)->GetHeadMonomer();
	CPolymer* pTail = (*iterBond)->GetTailMonomer();

        CAbstractBead* pHHBead = (*iterBond)->GetHeadHeadBead();
        CAbstractBead* pHTBead = (*iterBond)->GetHeadTailBead();
        CAbstractBead* pTHBead = (*iterBond)->GetTailHeadBead();
        CAbstractBead* pTTBead = (*iterBond)->GetTailTailBead();


//        if(pHead && pTail)
//        {
//        long headNo = (*iterBond)->CountHeadAdjacentBonds();
//        long tailNo = (*iterBond)->CountTailAdjacentBonds();
//            message = "Bond " + ToString(id) + " " + ToString(headNo) + " " + ToString(tailNo);
//          new CLogTextMessage(GetCurrentTime(), message);
//        }  
//       std::cout << "Bond: " << id << " " + headNo + " " + tailNo + " " + pHead + "  " + pTail;


	}

    // Now check all filaments

	if(GetFilamentTotal() > 0)
	{
		for(cActivePolymerListIterator citerPoly=m_FreeActivePolymers.begin(); citerPoly!=m_FreeActivePolymers.end(); citerPoly++)
		{
			const CAbstractBead* const pFirstBead = (*citerPoly)->GetTailBond()->GetTailTailBead();
			const CAbstractBead* const pLastBead  = (*citerPoly)->GetHeadBond()->GetTailTailBead();
        	}
	}
*/
    return true;
}

// Function to return a pointer to an event given its name or NULL if the event
// is not defined for this ACN. It is mainly accessed from the IModifyActiveCellNetwork
// interface command in response to a user command.

aevActiveEvent* aeActiveCellNetwork::GetInternalEventFromType(const zString eventType)
{
    aevActiveEvent* pEvent = 0;

    for(ActiveEventListIterator iterEvent = m_Events.begin(); iterEvent != m_Events.end(); iterEvent++)
    {
        if((*iterEvent)->GetEventType() == eventType)
            pEvent = *iterEvent;
    }

    return pEvent;
}

// Function to delete an ATP-dependent event that wraps a specified bond.
// Note that this only works if there at most a single event of the given
// type wrapping the bond whose id is supplied.

bool aeActiveCellNetwork::DeleteATPDependentEventWrappingBond(long bondId)
{
    bool bEventDestroyed = false;

	for(ActiveEventListIterator iterEvent = m_Events.begin(); iterEvent != m_Events.end(); iterEvent++)
    {
	       if(dynamic_cast<aevBondHydrolysesATP*>(*iterEvent))
	       {
	            aevBondHydrolysesATP* pActualEvent = dynamic_cast<aevBondHydrolysesATP*>(*iterEvent);
                if(pActualEvent->GetBondId() == bondId)
                {
                    RemoveEvent(pActualEvent);
                    bEventDestroyed = true;
                    return bEventDestroyed;
                }
           }
           else if(dynamic_cast<aevBondReleasesPi*>(*iterEvent))
	       {
                aevBondReleasesPi* pActualEvent = dynamic_cast<aevBondReleasesPi*>(*iterEvent);
                if(pActualEvent->GetBondId() == bondId)
                {
                    RemoveEvent(pActualEvent);
                    bEventDestroyed = true;
                     return bEventDestroyed;
               }
           }
           else if(dynamic_cast<aevBondPhosphorylation*>(*iterEvent))
	       {
                aevBondPhosphorylation* pActualEvent = dynamic_cast<aevBondPhosphorylation*>(*iterEvent);
                if(pActualEvent->GetBondId() == bondId)
                {
                    RemoveEvent(pActualEvent);
                    bEventDestroyed = true;
                    return bEventDestroyed;
                }
           }
    }

    return bEventDestroyed;
}

// Function to set the period with which a named event type attempts to execute.
// This reduces the sampling frequency of the event and allows a larger 
// probability to be used in the test. We store the (event name, execution period) 
// in a map, and all events of the same type have the same period.
// If the event already has a period set, we erase it before inserting the
// new pair.  Note that a value of 0 means an event executes every time-step.

void aeActiveCellNetwork::SetInternalEventExecutionPeriod(const zString eventType, long period)
{
    StringLongIterator iterPeriod;

    iterPeriod = m_mEventExecutionPeriodFromName.find(eventType);

    if(iterPeriod!= m_mEventExecutionPeriodFromName.end())
    {
        m_mEventExecutionPeriodFromName.erase(iterPeriod);
    }

    m_mEventExecutionPeriodFromName.insert(zPairSL(eventType,period));

    // Now that the new period has been stored, and will be used for newly-created
    // events, modify existing events of the specified type.

    UpdateEventPeriodByType(eventType);
}

// Function to return an event type's execution period given its name.
// If the event type is not found, we return 0 so that the event is always executed.

long aeActiveCellNetwork::GetInternalEventExecutionPeriodFromName(const zString eventType) const
{
    cStringLongIterator citerPeriod;

    citerPeriod = m_mEventExecutionPeriodFromName.find(eventType);

    if(citerPeriod == m_mEventExecutionPeriodFromName.end())
    {
        return 0;
    }
    else
    {
        return (*citerPeriod).second;
    }
}

// Private helper function to update the execution period of a specific event type
// when the user issues the SetEventExecutionPeriod command. The command first
// stores the (eventType, period) pair in a map (via SetInternalEventExecutionPeriod()
// above) which then calls this function to update all events of the specified type 
// and the prototype event that is stored in the ACN instance. Note that this function
// must be called after the SetInternalEventExecutionPeriod() otherwise the map
// will not contain the new execution period.
//
// Because we don't know what type of event is being modified, we have to iterate
// over all events and change each one whose type matches "eventType".

void aeActiveCellNetwork::UpdateEventPeriodByType(const zString eventType)
{
    const long newPeriod = GetInternalEventExecutionPeriodFromName(eventType);

	if(!m_Events.empty())
	{
        for(ActiveEventListIterator iterEvent=m_Events.begin(); iterEvent!=m_Events.end(); iterEvent++)
        {
            if((*iterEvent)->GetEventType() == eventType)
            {
                (*iterEvent)->ResetExecutionPeriod(newPeriod);
            }
        }
	}

    // Now update the stored prototype events. We have to compare the type of each
    // of these with the specified type. This is ugly, but there is no other way.

    if(eventType == m_pPolymerFormsEvent->GetEventType())
    {
        m_pPolymerFormsEvent->ResetExecutionPeriod(newPeriod);
    }
    else if(eventType == m_pPolymerDissolvesEvent->GetEventType())
    {
        m_pPolymerDissolvesEvent->ResetExecutionPeriod(newPeriod);
    }
    else if(eventType == m_pBondBindsToPolymerHeadEvent->GetEventType())
    {
        m_pBondBindsToPolymerHeadEvent->ResetExecutionPeriod(newPeriod);
    }
    else if(eventType == m_pBondBindsToPolymerTailEvent->GetEventType())
    {
        m_pBondBindsToPolymerTailEvent->ResetExecutionPeriod(newPeriod);
    }
    else if(eventType == m_pBondUnbindsFromPolymerHeadEvent->GetEventType())
    {
        m_pBondUnbindsFromPolymerHeadEvent->ResetExecutionPeriod(newPeriod);
    }
    else if(eventType == m_pBondUnbindsFromPolymerTailEvent->GetEventType())
    {
        m_pBondUnbindsFromPolymerTailEvent->ResetExecutionPeriod(newPeriod);
    }
}

// Function used by events contained in the ACN to broadcast their state to 
// event source/analysis decorators for analysis. All events owned by the ACN
// call the same function and their type is used to determine which event source
// decorators pass on the message. We pass a pointer to the complete event instead
// of just its alphanumeric type as we may want to pass more data later.

void aeActiveCellNetwork::BroadcastEventState(const aevActiveEvent* const pEvent)
{
    const zString type = pEvent->GetEventType();

//    std::cout << "ACN broadcasting the state for event type " << type << " at time " << GetIActiveSimBox()->GetCurrentTime() << zEndl;

    // The ACN now iterates over all event source decorators defined for the given
    // event type and forwards its information for analysis.

    StringEventSourceIterator iterESD = m_mEventSourceFromType.find(type);

    if(iterESD != m_mEventSourceFromType.end())
    {
        while(iterESD != m_mEventSourceFromType.upper_bound(type))
        {
            iterESD->second->BroadcastEventState(pEvent);
            iterESD++;
        }
    }
}

// Function that stores a new event source decorator in response to a user command.
// All events of the specified type are modified to broadcast their state when
// their success criteria are satisfied. Note that the calling function is
// responsible for obtaining the type of the event class and creating the
// appropriate event source decorator. Here we just iterate over all events 
// owned by the ACN modifying those whose type matches that specified in the 
// arguments and store the decorator instance.
// If the function executes successfully, we return true, otherwise return false.
//
bool aeActiveCellNetwork::AddEventSource(const zString eventType, taEventSourceDecorator* const pSource)
{
    // First set a flag in all instances of the specified event type so that
    // they broadcast their success state

	if(!m_Events.empty())
	{
		for(ActiveEventListIterator iterEvent=m_Events.begin(); iterEvent!=m_Events.end(); iterEvent++)
        {
//            std::cout <<"found event of type " << (*iterEvent)->GetEventType() << " and comparing to " << eventType << zEndl;

            if((*iterEvent)->GetEventType() == eventType)
            {
                (*iterEvent)->SetBroadcastState(true);
            }
		} 

        // Now that the events will broadcast their state, create an entry in a map
        // that uses the event type as the key and an event source decorator instance
        // as the value. Because there may be more than one event source wrapping 
        // an ACN, we use a multimap to hold <eventType, taEventSourceDecorator*> pairs.

//        std::cout <<"Size of event source map before addition " << m_mEventSourceFromType.size() << zEndl;
        m_mEventSourceFromType.insert(zPairSESD(eventType, pSource));
//        std::cout <<"Size of event source map after addition " << m_mEventSourceFromType.size() << zEndl;
	}
    else
    {
        return false;
    }

    return true;
}

// Function to remove an event source decorator in response to a user command.
// We reset the events' broadcast state but do not destroy the events. Then
// we destroy the event source decorator.
// NOTE What do we do if more than one event source decorator has been created 
// for the same event type? We need to ref count them so that we only reset the
// events' broadcast flag when all of them have been destroyed.

bool aeActiveCellNetwork::RemoveEventSource(const zString eventType, taEventSourceDecorator* const pSource)
{
	if(!m_Events.empty())
	{
		for(ActiveEventListIterator iterEvent=m_Events.begin(); iterEvent!=m_Events.end(); iterEvent++)
        {
            if((*iterEvent)->GetEventType() == eventType)
            {
                (*iterEvent)->SetBroadcastState(false);
            }
		} 

        // Now remove all values that correspond to the "eventType" key in the multimap

//        std::cout <<"Size of event source map before erase " << m_mEventSourceFromType.size() << zEndl;
        m_mEventSourceFromType.erase(eventType);
//        std::cout <<"Size of event source map after erase " << m_mEventSourceFromType.size() << zEndl;

	}
    else
    {
        return false;
    }

    return true;
}

// Overridden VF to allow this ACN to keep the number of free bonds constant.
// When a bond detaches from an active polymer, we have to decrease the number of
// free active bonds by one to maintain their number constant. Currently,this is 
// hardwired to use modify a single bond only. Because the bond unbinding events
// use the ACN function AddFreeBond(), which just uses push_back on the passed-in
// bond pointer, we know the bond that has just been released is at the end of the
// container. We toggle its phantom flag to true so that it does not immediately 
// rebind and this keeps the number of active bonds constant.

void aeActiveCellNetwork::ConvertFreeBondsToPhantom()
{
    if(GetPhantomBondsAllowed())
    {
        ActiveBondListIterator iterBond = m_FreeActiveBonds.end();
        iterBond--;
        
        (*iterBond)->SetPhantom(true);
        m_FreePhantomBonds.push_back(*iterBond);

//        std::cout << "converting to phantom (bound, free, phantom totals) " << GetBoundBondTotal() << "  " << GetFreeBondTotal() << "  " << GetPhantomBondTotal() << zEndl;
    }
}

// Function to allow this ACN to keep the number of free bonds constant.
// When a bond attaches to an active polymer, we have to increase the number of
// free active bonds by one to maintain their number constant. We randomly
// select a phantom bond and toggle its phantom flag.
// Although we would like to be able to toggle an arbitrary number of bonds, 
// we hardwire it to one for now as it is most efficient.

void aeActiveCellNetwork::ConvertPhantomBondsToFree()
{
    if(GetPhantomBondsAllowed() && GetPhantomBondTotal() > 0)
    {
            // Find a random phantom bond

            long unsigned int index = static_cast<long>(CCNTCell::GetRandomNo()*m_FreePhantomBonds.size());
            if(index == m_FreePhantomBonds.size())
                index--;

            ActiveBondListIterator iterBond = m_FreePhantomBonds.begin();

            while(index > 0)
            {
                index--;
                iterBond++;
            }
            
            (*iterBond)->SetPhantom(false);
            m_FreePhantomBonds.erase(iterBond);

//        std::cout << "converting to active (bound, free, phantom totals) " << GetBoundBondTotal() << "  " << GetFreeBondTotal() << "  " << GetPhantomBondTotal() << zEndl;
    }
}

// Function used by derived classes to convert all currently-free active bonds
// to phantoms. We copy the bonds from the active bonds container into
// the phantom bonds container, setting their phantom flag, but do not remove them 
// from the original container. Their phantom state must be checked using the flag.
// 
void aeActiveCellNetwork::SetAllFreeBondsPhantom()
{
    m_FreePhantomBonds.clear();

    for(ActiveBondListIterator iterBond=m_FreeActiveBonds.begin(); iterBond!=m_FreeActiveBonds.end(); iterBond++)
    {
        (*iterBond)->SetPhantom(true);
        m_FreePhantomBonds.push_back(*iterBond);
    }
}

// Function used by derived classes to convert all currently-phantom bonds to their 
// active state. Because the phantom bonds container only holds copies of the
// bonds, we do not need to move the pointers back to the free active bonds container,
// but just toggle the bonds' phantom flag and then clear the container.

void aeActiveCellNetwork::SetAllPhantomBondsActive()
{
    for(ActiveBondListIterator iterBond=m_FreePhantomBonds.begin(); iterBond!=m_FreePhantomBonds.end(); iterBond++)
    {
        (*iterBond)->SetPhantom(false);
    }

    m_FreePhantomBonds.clear();
}

