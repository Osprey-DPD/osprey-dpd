/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpsCorner.cpp: implementation of the mpsCorner class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "mpsFace.h"
#include "mpsEdge.h"
#include "mpsCorner.h"
#include "mpsSimBox.h"

// ****************************************
// Global members 



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor for the border class that represents the single corner cell that
// links 8 processors. This is the only Border region for which the number of cells
// is independent of the size of the SimBox.

mpsCorner::mpsCorner(mpsSimBox* const pSimBox, mpsFace* const pF1, mpsFace* const pF2, mpsFace* const pF3, 
                     long i, long j, long k, long lx, long ly, long lz, 
                     CCNTCell* const pCell) : mpsBorder(pSimBox,lx,ly,lz,1,1,1),
                     m_pFace1(pF1), m_pFace2(pF2), m_pFace3(pF3),
                     m_i(i), m_j(j), m_k(k)
{
    m_vCNTCells.push_back(pCell);
	m_vExternalCNTCells.clear();
		
	long nnIndex;
	long nnIntIndex;
    long alpha, beta, gamma;
	CCNTCell* pOldCell;
	CCNTCell* pNewCell;

    // Now assign the single external cell 
    if(m_i == m_LX-1)
	{
	  if(m_j == m_LY-1)
	  {
	    if(m_k == m_LZ-1)
		{
		    // UTR corner
			nnIndex  = m_pSimBox->MapCellToNNIndex(1,1,1);
		    pOldCell = pCell->GetNNCell(nnIndex);
		    pNewCell = new CExternalCNTCell(pOldCell);
	        m_vExternalCNTCells.push_back(pNewCell);
			pCell->SetNNCellIndex(nnIndex, pNewCell);
		  
			if(IsProcessZero())
			{
//				std::cout <<"UTR: creating external cell with nnIndex, pCell->id, pNewCell->id " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
			}
			
			// Corners are always external
			
			nnIntIndex = 0;
			for(gamma = -1; gamma < 2; gamma++)
			{
				for(beta = -1; beta < 2; beta++)
				{
					for(alpha = -1; alpha < 2; alpha++)
					{
						// Check if the neighbour cell should be added to the force loop array
						if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
						{
							if(alpha == 1 && beta == 1 && gamma == 1)
							{
							    if(IsProcessZero())
							    {
//									std::cout <<"UTR: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma, pNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
							    }
							    pCell->SetIntNNCellIndex(nnIntIndex, pNewCell);
							}
							nnIntIndex++;
						}
					}
				}
			}		
		}
		else
		{
		    // DTR corner
            nnIndex = m_pSimBox->MapCellToNNIndex(1,1,-1);
		    pOldCell = pCell->GetNNCell(nnIndex);
		    pNewCell = new CExternalCNTCell(pOldCell);
	        m_vExternalCNTCells.push_back(pNewCell);
			pCell->SetNNCellIndex(nnIndex, pNewCell);
		  
			if(IsProcessZero())
			{
//				std::cout <<"DTR: creating external cell with nnIndex, pCell->id, pNewCell->id " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
			}
			
			// Corners are always external
			
			nnIntIndex = 0;
			for(gamma = -1; gamma < 2; gamma++)
			{
				for(beta = -1; beta < 2; beta++)
				{
					for(alpha = -1; alpha < 2; alpha++)
					{
						// Check if the neighbour cell should be added to the force loop array
						if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
						{
							if(alpha == 1 && beta == 1 && gamma == -1)
							{
							    if(IsProcessZero())
							    {
//									std::cout <<"DTR: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndex, pNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
							    }
							    pCell->SetIntNNCellIndex(nnIntIndex, pNewCell);
							}
							nnIntIndex++;
						}
					}
				}
			}				
		}
	  }
	  else
	  {
	    if(m_k == m_LZ-1)
		{
		    // UBR corner
            nnIndex = m_pSimBox->MapCellToNNIndex(1,-1,1);
		    pOldCell = pCell->GetNNCell(nnIndex);
		    pNewCell = new CExternalCNTCell(pOldCell);
	        m_vExternalCNTCells.push_back(pNewCell);
			pCell->SetNNCellIndex(nnIndex, pNewCell);
		  
			if(IsProcessZero())
			{
//				std::cout <<"UBR: creating external cell with nnIndex, pCell->id, pNewCell->id " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
			}
			// Corners are always external
			
			nnIntIndex = 0;
			for(gamma = -1; gamma < 2; gamma++)
			{
				for(beta = -1; beta < 2; beta++)
				{
					for(alpha = -1; alpha < 2; alpha++)
					{
						// Check if the neighbour cell should be added to the force loop array
						if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
						{
							if(alpha == 1 && beta == -1 && gamma == 1)
							{
							    if(IsProcessZero())
							    {
//									std::cout <<"UBR: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndex, pNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
							    }
							    pCell->SetIntNNCellIndex(nnIntIndex, pNewCell);
							}
							nnIntIndex++;
						}
					}
				}
			}				
		}
		else
		{
		    // DBR corner
            nnIndex = m_pSimBox->MapCellToNNIndex(1,-1,-1);
		    pOldCell = pCell->GetNNCell(nnIndex);
		    pNewCell = new CExternalCNTCell(pOldCell);
	        m_vExternalCNTCells.push_back(pNewCell);
			pCell->SetNNCellIndex(nnIndex, pNewCell);
		  
			if(IsProcessZero())
			{
//				std::cout <<"DBR: creating external cell with nnIndex, pCell->id, pNewCell->id " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
			}
			// Corners are always external
			
			nnIntIndex = 0;
			for(gamma = -1; gamma < 2; gamma++)
			{
				for(beta = -1; beta < 2; beta++)
				{
					for(alpha = -1; alpha < 2; alpha++)
					{
						// Check if the neighbour cell should be added to the force loop array
						if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
						{
							if(alpha == 1 && beta == -1 && gamma == -1)
							{
							    if(IsProcessZero())
							    {
//									std::cout <<"DBR: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndex, pNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
							    }
							    pCell->SetIntNNCellIndex(nnIntIndex, pNewCell);
							}
							nnIntIndex++;
						}
					}
				}
			}				
		}
	  }
	}
	else
	{
	  if(m_j == m_LY-1)
	  {
	    if(m_k == m_LZ-1)
		{
		    // UTL corner
            nnIndex = m_pSimBox->MapCellToNNIndex(-1,1,1);
		    pOldCell = pCell->GetNNCell(nnIndex);
		    pNewCell = new CExternalCNTCell(pOldCell);
	        m_vExternalCNTCells.push_back(pNewCell);
			pCell->SetNNCellIndex(nnIndex, pNewCell);
		  
			if(IsProcessZero())
			{
//				std::cout <<"UTL: creating external cell with nnIndex, pCell->id, pNewCell->id " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
			}
			// Corners are always external
			
			nnIntIndex = 0;
			for(gamma = -1; gamma < 2; gamma++)
			{
				for(beta = -1; beta < 2; beta++)
				{
					for(alpha = -1; alpha < 2; alpha++)
					{
						// Check if the neighbour cell should be added to the force loop array
						if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
						{
							if(alpha == -1 && beta == 1 && gamma == 1)
							{
							    if(IsProcessZero())
							    {
//									std::cout <<"UTL: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndex, pNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
							    }
							    pCell->SetIntNNCellIndex(nnIntIndex, pNewCell);
							}
							nnIntIndex++;
						}
					}
				}
			}		
		}
		else
		{
		    // DTL corner
            nnIndex = m_pSimBox->MapCellToNNIndex(-1,1,-1);
		    pOldCell = pCell->GetNNCell(nnIndex);
		    pNewCell = new CExternalCNTCell(pOldCell);
	        m_vExternalCNTCells.push_back(pNewCell);
			pCell->SetNNCellIndex(nnIndex, pNewCell);
		  
			if(IsProcessZero())
			{
//				std::cout <<"DTL: creating external cell with nnIndex, pCell->id, pNewCell->id " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
			}
			// Corners are always external
			
			nnIntIndex = 0;
			for(gamma = -1; gamma < 2; gamma++)
			{
				for(beta = -1; beta < 2; beta++)
				{
					for(alpha = -1; alpha < 2; alpha++)
					{
						// Check if the neighbour cell should be added to the force loop array
						if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
						{
							if(alpha == -1 && beta == 1 && gamma == -1)
							{
							    if(IsProcessZero())
							    {
//									std::cout <<"DTL: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndex, pNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
							    }
							    pCell->SetIntNNCellIndex(nnIntIndex, pNewCell);
							}
							nnIntIndex++;
						}
					}
				}
			}				
		}
	  }
	  else
	  {
	    if(m_k == m_LZ-1)
		{
		    // UBL corner
            nnIndex = m_pSimBox->MapCellToNNIndex(-1,-1,1);
		    pOldCell = pCell->GetNNCell(nnIndex);
		    pNewCell = new CExternalCNTCell(pOldCell);
	        m_vExternalCNTCells.push_back(pNewCell);
			pCell->SetNNCellIndex(nnIndex, pNewCell);
		  
			if(IsProcessZero())
			{
//				std::cout <<"UBL: creating external cell with nnIndex, pCell->id, pNewCell->id " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
			}
			// Corners are always external
			
			nnIntIndex = 0;
			for(gamma = -1; gamma < 2; gamma++)
			{
				for(beta = -1; beta < 2; beta++)
				{
					for(alpha = -1; alpha < 2; alpha++)
					{
						// Check if the neighbour cell should be added to the force loop array
						if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
						{
							if(alpha == -1 && beta == -1 && gamma == 1)
							{
							    if(IsProcessZero())
							    {
//									std::cout <<"UBL: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndex, pNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
							    }
							    pCell->SetIntNNCellIndex(nnIntIndex, pNewCell);
							}
							nnIntIndex++;
						}
					}
				}
			}				
		}
		else
		{
		    // DBL corner
            nnIndex = m_pSimBox->MapCellToNNIndex(-1,-1,-1);
		    pOldCell = pCell->GetNNCell(nnIndex);
		    pNewCell = new CExternalCNTCell(pOldCell);
	        m_vExternalCNTCells.push_back(pNewCell);
			pCell->SetNNCellIndex(nnIndex, pNewCell);
		  
			if(IsProcessZero())
			{
//				std::cout <<"DBL: creating external cell with nnIndex, pCell->id, pNewCell->id " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
			}
			// Corners are always external
			
			nnIntIndex = 0;
			for(gamma = -1; gamma < 2; gamma++)
			{
				for(beta = -1; beta < 2; beta++)
				{
					for(alpha = -1; alpha < 2; alpha++)
					{
						// Check if the neighbour cell should be added to the force loop array
						if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
						{
							if(alpha == -1 && beta == -1 && gamma == -1)
							{
							    if(IsProcessZero())
							    {
//									std::cout <<"DBL: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndex, pNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
							    }
							    pCell->SetIntNNCellIndex(nnIntIndex, pNewCell);
							}
							nnIntIndex++;
						}
					}
				}
			}
		}
	  }
	}
	
	
}

// Copy constructor

mpsCorner::mpsCorner(const mpsCorner& oldBorder) : mpsBorder(oldBorder),
                                                   m_pFace1(oldBorder.m_pFace1),
                                                   m_pFace2(oldBorder.m_pFace2),
                                                   m_pFace3(oldBorder.m_pFace3),
                                                   m_i(oldBorder.m_i), m_j(oldBorder.m_j), m_k(oldBorder.m_k)
{
    m_vCNTCells = oldBorder.m_vCNTCells;
}
                           
// Destructor

mpsCorner::~mpsCorner()
{
    // mpsBorder instances only copy CNTCell pointers so we don't destroy them here
}

// A Corner is external if any of its Faces are external.

bool mpsCorner::IsExternal() const
{
    return (m_pFace1->IsExternal() || m_pFace2->IsExternal()  || m_pFace3->IsExternal());
}

// Function to update the positions of the beads in this Border region if it
// does not need to communicate with other processors. Which regions this applies
// to depends on the simulation Space geometry.

void mpsCorner::UpdatePos()
{
    m_vCNTCells.front()->UpdatePos();
}

// Function to update the positions of the beads in a Corner region. A Corner
// contains a single CNT cell, which only contains O(1) beads at typical
// densities, so we just we call a dedicated CCNTCell function, UpdatePosCorner()
// for the single cell, passing in a pointer to the mpsBorder base class that 
// gives the CNT cell access to the mpsSimBox and allows it to add any beads 
// that change processors to the Border's lost bead map.

// A Corner region communicates with 7 other processors, so the tests we need 
// to do are quite different from the Face region that only communicates with 
// one other processor.


void mpsCorner::UpdateSharedPos()
{
    std::cout << "Error in mpsCorner - UpdateSharedPos" << zEndl;
}

// PVF to calculate the non-bonded, bead-bead forces for all beads in this Border
// region that do NOT depend on beads owned by adjacent processors. 

void mpsCorner::UpdateForce()
{
    m_vCNTCells.front()->UpdateForce();
}


// PVF to calculate the non-bonded bead-bead forces for all beads in this Border
// region that depend on the coordinates of beads owned by adjacent processors.
// We also have to store the random forces so that we can send them to the 
// originating processor later.
//
// This function must be called after all bead coordinates have been received from
// all adjacent processors and stored in the CExternalCNTCell instances created in
// the constructor, and after the corresponding mpsFace, mpsEdge functions have
// been called.  It has to perform the following tasks:
//
// 1 Calculate internal forces between bead pairs in this Corner
//
// 2 Calculate all external forces between beads in this Corner and all 
//   CExternalCNTCells adjacent to the Corner
//   

void mpsCorner::CalculateSharedForce()
{
/*    if(m_i == m_LX-1 && m_j == m_LY-1 && m_k == m_LZ-1)
	{
	    std::cout << "Proc " << GetRank() << " UTR Corner calling UpdateForceP" << zEndl;
	 
	    long localBeadTotal = 0;
		  		
		CCNTCell* pCell = m_vCNTCells.front()->GetIntNNCell(12);  // We only want the corner's external cell
			
		CExternalCNTCell* pExtCell = dynamic_cast<CExternalCNTCell*>(pCell);
			
		if(pExtCell)
		{
			localBeadTotal = pCell->CellBeadTotal();			
		}
		else
		{
		    std::cout << "UTR dynamic cast failed" << zEndl;
			
		}
		std::cout << "UTR has total neighbouring beads " << localBeadTotal << zEndl;
	}
*/	  
	    
    m_vCNTCells.front()->UpdateForceP();
}


// Local version of the CalculateSharedForce() for one of the Corners that includes its particular missing bead-bead interactions.

void mpsCorner::CalculateSharedForceUTR()
{	
    m_vCNTCells.front()->UpdateForceP();
		
	// Add in the missing interactions
		
	BeadList lBeads =  m_vCNTCells.front()->GetBeads();
		
	for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	{
		(m_vCNTCells.front()->GetNNCell(2))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(5))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(6))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(7))->UpdateForceBetweenCells(true,*iterBead);  
		(m_vCNTCells.front()->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);  
		(m_vCNTCells.front()->GetNNCell(15))->UpdateForceBetweenCells(true,*iterBead);
	}
}

// Local version of the CalculateSharedForce() for one of the Corners that includes its particular missing bead-bead interactions.

void mpsCorner::CalculateSharedForceUBR()
{	
    m_vCNTCells.front()->UpdateForceP();
		
	// Add in the missing interactions
		
	BeadList lBeads =  m_vCNTCells.front()->GetBeads();
		
	for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	{
		(m_vCNTCells.front()->GetNNCell(2))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(5))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
	}
}

// Local version of the CalculateSharedForce() for one of the Corners that includes its particular missing bead-bead interactions.

void mpsCorner::CalculateSharedForceDTR()
{	
    m_vCNTCells.front()->UpdateForceP();
		
	// Add in the missing interactions
		
	BeadList lBeads =  m_vCNTCells.front()->GetBeads();
		
	for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	{
		(m_vCNTCells.front()->GetNNCell(2))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(5))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(6))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(7))->UpdateForceBetweenCells(true,*iterBead);  
		(m_vCNTCells.front()->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);  
		(m_vCNTCells.front()->GetNNCell(15))->UpdateForceBetweenCells(true,*iterBead);
	}
}


// Local version of the CalculateSharedForce() for one of the Corners that includes its particular missing bead-bead interactions.

void mpsCorner::CalculateSharedForceDBR()
{	
    m_vCNTCells.front()->UpdateForceP();
		
	// Add in the missing interactions
		
	BeadList lBeads =  m_vCNTCells.front()->GetBeads();
		
	for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	{
		(m_vCNTCells.front()->GetNNCell(2))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(5))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(19))->UpdateForceBetweenCells(true,*iterBead);
	}
}

// Local version of the CalculateSharedForce() for one of the Corners that includes its particular missing bead-bead interactions.

void mpsCorner::CalculateSharedForceUTL()
{	
    m_vCNTCells.front()->UpdateForceP();
		
	// Add in the missing interactions
		
	BeadList lBeads =  m_vCNTCells.front()->GetBeads();
		
	for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	{
		(m_vCNTCells.front()->GetNNCell(6))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(7))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(15))->UpdateForceBetweenCells(true,*iterBead);
	}
}

// Local version of the CalculateSharedForce() for one of the Corners that includes its particular missing bead-bead interactions.

void mpsCorner::CalculateSharedForceDTL()
{	
    m_vCNTCells.front()->UpdateForceP();
		
	// Add in the missing interactions
		
	BeadList lBeads =  m_vCNTCells.front()->GetBeads();
		
	for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
	{
		(m_vCNTCells.front()->GetNNCell(6))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(7))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
		(m_vCNTCells.front()->GetNNCell(15))->UpdateForceBetweenCells(true,*iterBead);
	}
}


void mpsCorner::UpdateSharedForce()
{
    m_vCNTCells.front()->UpdateForce();
}

void mpsCorner::AddBeadToCell(long index, long id, long type, double x, double y, double z, double vx, double vy, double vz)
{
    dynamic_cast<CExternalCNTCell*>(m_vExternalCNTCells.front())->AddBeadToCell(id,type,x,y,z,vx,vy,vz);
}

// Function to return the set of all beads in the single external CNT cell diagonally connected to this
// Corner. This function assembles the bead pointers in a list. Even though there is only a single external cell for a Corner region,
// we perform a loop to keep the algorithm the same as for Edges and Faces.

BeadList mpsCorner::GetExternalBeads()
{
    BeadList lBeads;
    lBeads.clear();

    for(CNTCellIterator iterCell=m_vExternalCNTCells.begin(); iterCell!=m_vExternalCNTCells.end(); iterCell++)
    {
        BeadList ltb;
        ltb = (*iterCell)->GetBeads();
        copy(ltb.begin(), ltb.end(), back_inserter(lBeads));
    }
		
    return lBeads;
}

// Function to remove all coordinates from the lone external CNT cell belonging to this corner.

void mpsCorner::ClearExternalCells()
{
    dynamic_cast<CExternalCNTCell*>(m_vExternalCNTCells.front())->Clear();
}

// Function to remove all bead pointers from the lone external CNT cell belonging to this Corner.
// We perform a loop in case we later extend the code to include more than one external cell.

void mpsCorner::RemoveBeadsFromExternalCells()
{
    dynamic_cast<CExternalCNTCell*>(m_vExternalCNTCells.front())->RemoveAllBeadsFromCell();
}

// Function to calculate extended bond forces between beads in this Corner's single external cell and its adjacent external cells that wrap the Border cells for a
// neighbouring processor.  It is called only by the U,T,R Faces to avoid double-counting the bond forces. The algorithm currently assumes that bonded
// beads are always in adjacent cells - so bond lengths must not be longer than the CNT cell width.

void mpsCorner::CalculateSharedBondForce()
{
    dynamic_cast<CExternalCNTCell*>(m_vExternalCNTCells.front())->UpdateBondForceBetweenCells(m_pSimBox);
}


