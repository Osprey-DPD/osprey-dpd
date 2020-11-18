/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Domain2d.cpp: implementation of the CDomain2d class.
//
//////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimNumericFlags.h"
#include "Domain2d.h"
#include "aaRegionToType.h"		// includes CNTCell.h


// STL using declarations



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDomain2d::CDomain2d(zLongVector headTypes, zLongVector tailTypes, 
					 bool bMonolayers[]) : m_HeadBeadTypes(headTypes),
										   m_TailBeadTypes(tailTypes),
										   m_RaftTotal(0),
										   m_MeanRaftCellSize(0.0),
										   m_MeanRaftPolymerSize(0.0),
										   m_MeanRaftArea(0.0),
										   m_MeanRaftPerimeter(0.0)
{
	m_bMonolayers[0] = bMonolayers[0];	// Upper monolayer
	m_bMonolayers[1] = bMonolayers[1];	// Lower monolayer

	m_Rafts.Clear();
	m_vCellInRaft.clear();
	m_vCellTotal.clear();

}

CDomain2d::~CDomain2d()
{

}

// Function to scan the monolayer and see if the specified polymers have formed
// rafts by checking the positions of their head beads. The set of head bead types
// that define the rafts must be disjoint from the bead types in the remaining 
// polymers to avoid counting non-raft polymers.
//
// Note that the monolayers may be either planar, as in a CCompositeBilayer, or
// spherical, as in a CVesicle, or there may be only one monolayer (not implemented yet).
// The algorithm works on the CNT cell structure and defines a raft as all CNT cells
// that contain raft polymer head beads and their immediate neighbour cells. This allows
// a layer of empty CNT cells around each raft to provide some flexibility to defining
// polymers that arwe "near" each other.

void CDomain2d::UpdateState(CSimState &rSimState, const ISimBox *const pISimBox)
{
	// Ensure all containers are emptied before starting the new analysis

	m_Rafts.DeleteAll();
	m_vCellInRaft.clear();
	m_vCellTotal.clear();

	// Set the cell-in-raft flags to -1 so that we can check when cells are first added to rafts

	m_vCellInRaft.resize(pISimBox->GetCNTCellTotal(), -1);
	 m_vCellTotal.resize(pISimBox->GetCNTCellTotal(), 0);

	// Iterate over all CNT cells checking if each one contains raft polymers.
	//
	// Note rafts and cells use zero-based indexing

    long i = 0; // Counter used several times below
	long cellsInRafts = 0;	// Holds the number of cells belonging to all rafts

	for(cCNTCellIterator cIterCell=pISimBox->GetCNTCells().begin(); cIterCell!=pISimBox->GetCNTCells().end(); cIterCell++)
	{
		CCNTCell* const pCell    = *cIterCell;
		const long cellId	     = pCell->GetId();
		const long cellRaftIndex = m_vCellInRaft.at(cellId);
		const BeadList lBeads    = pCell->GetBeads();

		// Set a flag for the current cell showing if it contains raft polymers.
		// We have to check if beads corresponding to the head of each raft polymer
		// occur in the cell before checking neighbour cells.

		bool bCellHasRaftPolymers = false;

//		for(czLongVectorIterator cIterBeadType=m_HeadBeadTypes.begin(); cIterBeadType!=m_HeadBeadTypes.end(); cIterBeadType++)
		for(czLongVectorIterator cIterBeadType=m_TailBeadTypes.begin(); cIterBeadType!=m_TailBeadTypes.end(); cIterBeadType++)
		{
			cBeadListIterator iterBead = find_if(lBeads.begin(), lBeads.end(), aaBeadType(*cIterBeadType));

			if(iterBead!=lBeads.end())	
			{
				bCellHasRaftPolymers = true;
				long beadNo = count_if(lBeads.begin(), lBeads.end(), aaBeadType(*cIterBeadType));
				m_vCellTotal.at(cellId) += beadNo;
			}
		}

			// ****************************************
			// cell contains raft polymers 

		if(bCellHasRaftPolymers)
		{
			// If the cell is already in a raft add all of its nearest neighbours to the 
			// raft. If any of them are already in a different raft signal an error.

			if(cellRaftIndex != -1)
			{
				CRaft* const pOldRaft = m_Rafts.GetRaft(cellRaftIndex);

				for(i=0; i<27; i++)
				{
					CCNTCell* const pAdjCell	= pCell->GetNNCell(i);
					const long adjCellId		= pAdjCell->GetId();
					const long adjCellRaftIndex = m_vCellInRaft.at(adjCellId);

					if(adjCellRaftIndex == -1)
					{
						m_vCellInRaft.at(adjCellId) = cellRaftIndex;
						pOldRaft->AddCell(pAdjCell);
						cellsInRafts++;
					}
					else if(adjCellRaftIndex != cellRaftIndex)
					{
						std::cout << "Error - neighbour cell already in a raft" << zEndl;
					}
				}
			}
			else	// cell is not in a raft so check its neighbours for a raft
			{
				bool bNNCellNotInRaft = true;

				long j = 0;
				while(bNNCellNotInRaft && j<27)
				{
					CCNTCell* const pAdjCell    = pCell->GetNNCell(j++);
					const long adjCellId	    = pAdjCell->GetId();
					const long adjCellRaftIndex = m_vCellInRaft.at(adjCellId);

					if(adjCellRaftIndex != -1)
					{
						bNNCellNotInRaft = false;
						CRaft* const pOldRaft = m_Rafts.GetRaft(adjCellRaftIndex);

						pOldRaft->AddCell(pCell);
						m_vCellInRaft.at(cellId) = adjCellRaftIndex;
						cellsInRafts++;
					}
				}

				// If no neighbours were in a raft, create a new raft for the current 
				// cell and add all of its neighbours

				if(bNNCellNotInRaft)
				{
					const long newRaftIndex = m_Rafts.GetRaftTotal(); // Zero-based indexing!

					CRaft* const pRaft = new CRaft(m_HeadBeadTypes, m_TailBeadTypes);
					m_Rafts.AddRaft(pRaft);
					pRaft->AddCell(pCell);
					m_vCellInRaft.at(cellId) = newRaftIndex;

					cellsInRafts++;

					for(i=0; i<27; i++)
					{
						CCNTCell* const pAdjCell    = pCell->GetNNCell(i);
						const long adjCellId	    = pAdjCell->GetId();
						const long adjCellRaftIndex = m_vCellInRaft.at(adjCellId);

						if(adjCellRaftIndex == -1)
						{
							pRaft->AddCell(pAdjCell);
							m_vCellInRaft.at(adjCellId) = newRaftIndex;
							cellsInRafts++;
						}
						else if(adjCellId != cellId)	// Exclude self-comparison
						{
						    std::cout << "Error - neighbour cell already in a raft" << zEndl;
						}
					}
				}
			}
		}
		else	// ****************************************
				// cell does not contain raft polymers, check its neighbour cells to see
				// if any belong to a raft and contain raft polymers
		{
			bool bNNCellNotInRaft = true;

			long k = 0;
			while(bNNCellNotInRaft && k<27)
			{
				CCNTCell* const pAdjCell	= pCell->GetNNCell(k++);
				const long adjCellId		= pAdjCell->GetId();
				const long adjCellRaftIndex = m_vCellInRaft.at(adjCellId);

				if(adjCellRaftIndex != -1 && m_vCellTotal.at(adjCellId) > 0)
				{
					bNNCellNotInRaft = false;
					CRaft* const pOldRaft = m_Rafts.GetRaft(adjCellRaftIndex);

					pOldRaft->AddCell(pCell);
					m_vCellInRaft.at(cellId) = adjCellRaftIndex;
					cellsInRafts++;
				}
			}
		}
	}

	// Analyse the raft properties using the CRaftSet's 
	// CalculateMeanRaftXXX() functions. These recalculate the raft 
	// properties as opposed to the inline GetMeanRaftXXX() functions
	// that only return a cached value. 

	m_RaftTotal				= m_Rafts.GetRaftTotal();
	m_MeanRaftCellSize		= m_Rafts.CalculateMeanRaftCellSize();
	m_MeanRaftPolymerSize	= m_Rafts.CalculateMeanRaftPolymerSize();
	m_MeanRaftArea			= m_Rafts.CalculateMeanRaftArea();
	m_MeanRaftPerimeter		= m_Rafts.CalculateMeanRaftPerimeter();

}



