/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpsEdge.cpp: implementation of the mpsEdge class.
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
// Constructor for the border class that represents the linear Edge between
// four adjacent processors. We pass in the size of the Space in lx,ly,lz so that
// it can copy the necessary CNT cells from the main array. But we store 
// only the size of the border region after the copying. An Edge is external if
// either of its defining Faces are external. 
//
// We also store the set of CNT cells that comprise this Edge plus its two Corners,
// and the set of external CNT cells that wrap this Edge that are created in the
// mpsFace class. We rely on the fact that the two Face instances that meet at an 
// Edge have already been created before we enter this constructor, so that the 
// external CNT cells have already been connected up. But we still need to create 
// new external cells for the diagonal strip of cells adjacent to this Edge and its
// Corners.

mpsEdge::mpsEdge(mpsSimBox* const pSimBox, mpsFace* const pF1, mpsFace* const pF2,
                 long lx, long ly, long lz, long xc, long yc, long zc, long xn, long yn, long zn, 
                 CNTCellVector vAllCNTCells) : mpsBorder(pSimBox, lx, ly, lz, xc, yc, zc),
                                               m_pFace1(pF1), m_pFace2(pF2),
                                               m_XN(xn), m_YN(yn), m_ZN(zn)
{
    // Clear the containers of CNT cells and external CNT cells belonging to this 
    // Edge AND its associated two Corners.

    m_vECCells.clear();
    m_vExternalECCells.clear();

    // Store pointers to the CNT cells within the Edge according to its normal vector.
    // Note that we do not check the validity of the normal components: the calling 
    // routine must guarantee this for us. The Face instances are responsible for
    // creating and connecting the external CNT cells, here we just copy pointers
    // from the complete set of CNT cells in the SimBox, and trust that the Faces
    // have already connected them up.
    //

    long extIndex   = 0;
    long nnIndex    = 0;
    long nnIntIndex = 0;
    long alpha, beta, gamma;
    CCNTCell* pOldCell;
    CCNTCell* pNewCell;

    if(m_ZN == -1)
    {
        if(m_XN == 1)
        {
            if(m_YN == 1) // TR edge
            {
                for(long iz = 0; iz < m_LZ; iz++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(m_LX-1,m_LY-1,iz));

					// Store the Edge cells alone
					if((iz > 0 && iz < m_LZ-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					// and next the Edge/Corner cells. We know that the two Faces 
					// that meet at this Edge occur in the order T then R in the 
					// constructor argument list, and similarly for all other Edges.

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(1,1,0);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"TR: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long iz = 0; iz < m_LZ; iz++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(m_LX-1,m_LY-1,iz));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(alpha == 1 && beta == 1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(iz, 0, 0, m_LZ, 0, 0, gamma, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout <<"TR: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                              std::cout <<"TR: adding force loop cell with alpha, beta, gamma,extIndex,nnIntIndex,pNewCell " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
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
            else if(m_YN == 0) // BR edge
            {
                for(long iz = 0; iz < m_LZ; iz++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(m_LX-1,0,iz));

					// Store the Edge cells alone
					if((iz > 0 && iz < m_LZ-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(1,-1,0);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"BR: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long iz = 0; iz < m_LZ; iz++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(m_LX-1,0,iz));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(alpha == 1 && beta == -1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(iz, 0, 0, m_LZ, 0, 0, gamma, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout <<"BR: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                                std::cout <<"BR: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndex, pNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
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
        else if(m_XN == 0)
        {
            if(m_YN == 1) // TL edge
            {
                for(long iz = 0; iz < m_LZ; iz++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(0,m_LY-1,iz));

					// Store the Edge cells alone
					if((iz > 0 && iz < m_LZ-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(-1,1,0);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"TL: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long iz = 0; iz < m_LZ; iz++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(0,m_LY-1,iz));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(alpha == -1 && beta == 1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(iz, 0, 0, m_LZ, 0, 0, gamma, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout << "TL cell " << pCell->GetId() << " adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                                std::cout << "TL cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndex,nnIntIndex,pNewCell " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
                                            }
                                            pCell->SetIntNNCellIndex(nnIntIndex, pNewCell);
                                        }
		                                nnIntIndex++;
	                                }
                                }
                            }
                        }
/*						
						    std::cout << zEndl << "TL Edge has neighbour cells for iz " << iz << zEndl;
							for(long ii=0; ii<27; ii++)
							{
							    std::cout << pCell->GetNNCell(ii)->GetId() << " ";
							}	
						    std::cout << zEndl << "and force loop cells" << zEndl;
							
							for(long jj=0; jj<13; jj++)
							{
							    std::cout << pCell->GetIntNNCell(jj)->GetId() << " " ;
							}
							std::cout << zEndl;
*/
						
				    }
				}
            }
            else if(m_YN == 0) // BL edge
            {
                for(long iz = 0; iz < m_LZ; iz++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(0,0,iz));

					// Store the Edge cells alone
					if((iz > 0 && iz < m_LZ-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(-1,-1,0);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"BL: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long iz = 0; iz < m_LZ; iz++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(0,0,iz));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(alpha == -1 && beta == -1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(iz, 0, 0, m_LZ, 0, 0, gamma, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout << "BL: cell " << pCell->GetId() << " adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                              std::cout << "BL: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndex " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << zEndl;
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
    }
    else if(m_ZN == 0)
    {
        if(m_YN == -1)
        {
            if(m_XN == 1) // DR edge
            {
                for(long iy = 0; iy < m_LY; iy++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(m_LX-1,iy,0));

					// Store the Edge cells alone
					if((iy > 0 && iy < m_LY-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(1,0,-1);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"DR: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long iy = 0; iy < m_LY; iy++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(m_LX-1,iy,0));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(alpha == 1 && gamma == -1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(iy, 0, 0, m_LY, 0, 0, beta, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
 //                                         std::cout <<"DR: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                              std::cout <<"DR: adding force loop cell with alpha, beta, gamma,extIndex " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << zEndl;
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
            else if(m_XN == 0) // DL edge
            {
                for(long iy = 0; iy < m_LY; iy++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(0,iy,0));

					// Store the Edge cells alone
					if((iy > 0 && iy < m_LY-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(-1,0,-1);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"DL: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long iy = 0; iy < m_LY; iy++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(0,iy,0));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(alpha == -1 && gamma == -1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(iy, 0, 0, m_LY, 0, 0, beta, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout <<"DL: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                              std::cout <<"DL: adding force loop cell with alpha, beta, gamma,extIndex " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << zEndl;
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
        else if(m_XN == -1)
        {
            if(m_YN == 1) // DT edge
            {
                for(long ix = 0; ix < m_LX; ix++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,m_LY-1,0));

					// Store the Edge cells alone
					if((ix > 0 && ix < m_LX-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(0,1,-1);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"DT: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long ix = 0; ix < m_LX; ix++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,m_LY-1,0));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(beta == 1 && gamma == -1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(ix, 0, 0, m_LX, 0, 0, alpha, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout <<"DT: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                              std::cout <<"DT: adding force loop cell with alpha, beta, gamma,extIndex " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << zEndl;
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
            else if(m_YN == 0)  // DB edge
            {
                for(long ix = 0; ix < m_LX; ix++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,0,0));

					// Store the Edge cells alone
					if((ix > 0 && ix < m_LX-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(0,-1,-1);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"DB: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long ix = 0; ix < m_LX; ix++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,0,0));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(beta == -1 && gamma == -1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(ix, 0, 0, m_LX, 0, 0, alpha, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout <<"DB: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                              std::cout <<"DB: adding force loop cell with alpha, beta, gamma,extIndex " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << zEndl;
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
    }
    else if(m_ZN == 1)
    {
        if(m_YN == -1)
        {
            if(m_XN == 1) // UR edge
            {
                for(long iy = 0; iy < m_LY; iy++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(m_LX-1,iy,m_LZ-1));

					// Store the Edge cells alone
					if((iy > 0 && iy < m_LY-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(1,0,1);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"UR: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long iy = 0; iy < m_LY; iy++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(m_LX-1,iy,m_LZ-1));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(alpha == 1 && gamma == 1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(iy, 0, 0, m_LY, 0, 0, beta, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout <<"UR: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                              std::cout <<"UR: adding force loop cell with alpha, beta, gamma,extIndex " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << zEndl;
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
            else if(m_XN == 0) // UL edge
            {
                for(long iy = 0; iy < m_LY; iy++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(0,iy,m_LZ-1));

					// Store the Edge cells alone
					if((iy > 0 && iy < m_LY-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(-1,0,1);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"UL: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long iy = 0; iy < m_LY; iy++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(0,iy,m_LZ-1));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(alpha == -1 && gamma == 1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(iy, 0, 0, m_LY, 0, 0, beta, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout <<"UL: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                              std::cout <<"UL: adding force loop cell with alpha, beta, gamma,extIndex " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << zEndl;
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
        else if(m_XN == -1)
        {
            if(m_YN == 1) // UT edge
            {
                for(long ix = 0; ix < m_LX; ix++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,m_LY-1,m_LZ-1));

					// Store the Edge cells alone
					if((ix > 0 && ix < m_LX-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(0,1,1);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"UT: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long ix = 0; ix < m_LX; ix++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,m_LY-1,m_LZ-1));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(beta == 1 && gamma == 1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(ix, 0, 0, m_LX, 0, 0, alpha, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout <<"UT: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
//                                              std::cout <<"UT: adding force loop cell with alpha, beta, gamma,extIndex " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << zEndl;
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
            else if(m_YN == 0)  // UB edge
            {
                for(long ix = 0; ix < m_LX; ix++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,0,m_LZ-1));

					// Store the Edge cells alone
					if((ix > 0 && ix < m_LX-1))
					{
						m_vCNTCells.push_back(pCell);
					}

					if(IsExternal())
					{
						m_vECCells.push_back(pCell);

						nnIndex  = m_pSimBox->MapCellToNNIndex(0,-1,1);
						pOldCell = pCell->GetNNCell(nnIndex);
						pNewCell = new CExternalCNTCell(pOldCell);
						m_vExternalECCells.push_back(pNewCell);
						if(IsProcessZero())
						{
//						    std::cout <<"UB: creating external cell with, nnIndex, pOldCell->id, pNewCell->id " << nnIndex << " " << pOldCell->GetId() << " " << pNewCell->GetId() << zEndl;
						}
					}
                }
				
				// Now iterate over the EC cells again and connect them to each other's external cells to allow the force loop
				// to update the bead-bead forces.
				
				if(IsExternal())
				{
                    for(long ix = 0; ix < m_LX; ix++)
                    {
                        CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,0,m_LZ-1));
				
				        nnIntIndex = 0;
                        for(gamma = -1; gamma < 2; gamma++)
                        {
                            for(beta = -1; beta < 2; beta++)
                            {
                                for(alpha = -1; alpha < 2; alpha++)
                                {
                                    pNewCell = 0;
                                    if(beta == -1 && gamma == 1)
                                    {
                                        nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                        extIndex = GetCellIndexFromCoords(ix, 0, 0, m_LX, 0, 0, alpha, 0, 0);
                                        pNewCell = m_vExternalECCells.at(extIndex);
                                        pCell->SetNNCellIndex(nnIndex, pNewCell);
                                        if(IsProcessZero())
                                        {
//                                          std::cout <<"UB: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                        }
                                    }
                                    // Check if the neighbour cell should be added to the force loop array
                                    if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                                {
                                        if(pNewCell)
                                        {
                                            if(IsProcessZero())
                                            {
 //                                             std::cout <<"UB: adding force loop cell with alpha, beta, gamma,extIndex " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << zEndl;
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
    }

}

// Copy constructor

mpsEdge::mpsEdge(const mpsEdge& oldBorder) : mpsBorder(oldBorder),
                                             m_pFace1(oldBorder.m_pFace1),
                                             m_pFace2(oldBorder.m_pFace2),
                                             m_XN(oldBorder.m_XN),
                                             m_YN(oldBorder.m_YN),
                                             m_ZN(oldBorder.m_ZN)
{

}
                           
// Destructor

mpsEdge::~mpsEdge()
{
    // mpsBorder instances only copy CNTCell pointers so we don't destroy them here

}

// An Edge is external if either of its Faces is external.

bool mpsEdge::IsExternal() const
{
    return (m_pFace1->IsExternal() || m_pFace2->IsExternal());
}

// Function to update the positions of the beads in this Border region if it
// does not need to communicate with other processors. Which regions this applies
// to depends on the simulation Space geometry.

void mpsEdge::UpdatePos()
{
	for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdatePos();
	} 
}

// Function to update the positions of the beads in an Edge region that needs
// to communicate with other processors. 

// An Edge region communicates with 3 other processors.

void mpsEdge::UpdateSharedPos()
{
    std::cout << "Error in mpsEdge - UpdateSharedPos" << zEndl;
}

// PVF to calculate the non-bonded, bead-bead forces for all beads in this Border
// region that do NOT depend on beads owned by adjacent processors. This class
// requires that the mpsFace class calculates forces between beads in a Face and
// beads in this Edge instance. We just add the Edge bead forces not calculated
// by the Face.
// 
// 1 Calculate internal forces between bead pairs in this Edge, that is, between
//   beads in the same Edge cell or adjacent Edge cells, but not an Edge cell and
//   an adjacent Face cell.
//
// 2    "         "       "    between beads in this Edge and its two Corners

void mpsEdge::UpdateForce()
{
    // Internal bead-bead pairs in the Edge: first we calculate forces between
    // beads both of which are within an Edge cell, then we add forces for pairs
    // that are in adjacent Edge cells, but only in one direction so that we do not
    // double count the forces.

	for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdateForce();
	} 

}

// PVF to calculate the non-bonded bead-bead forces for all beads in this Border
// region that depend on the coordinates of beads owned by adjacent processors.
// We also have to store the random forces so that we can send them to the 
// originating processor later.
//
// This function must be called after all bead coordinates have been received from
// all adjacent processors and stored in the CExternalCNTCell instances created in
// the constructor, and after the corresponding mpsFace function has been called. 
// It has to perform the following tasks:
//
// 1 Calculate internal forces between bead pairs in this Edge, that is, between
//   beads in the same Edge cell or adjacent Edge cells but not an Edge cell and
//   and adjacent Face cell.
//
// 2    "         "       "    between beads in this Edge and its two Corners
//
// 3 Calculate all external forces between beads in this Edge and all 
//   CExternalCNTCells adjacent to the Edge
//   

void mpsEdge::CalculateSharedForce()
{
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        (*iterCell)->UpdateForceP();		
    } 
}

// Local version of the CalculateSharedForce() for one of the Edges that includes its particular missing bead-bead interactions.

void mpsEdge::CalculateSharedForceUT()
{	
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        (*iterCell)->UpdateForceP();
		
		// Add in the missing interactions in the +X-Z direction
		
		BeadList lBeads = (*iterCell)->GetBeads();
		
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
		    ((*iterCell)->GetNNCell(6))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(7))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(15))->UpdateForceBetweenCells(true,*iterBead);
		}
    } 
}

// Local version of the CalculateSharedForce() for one of the Edges that includes its particular missing bead-bead interactions.

void mpsEdge::CalculateSharedForceUR()
{	
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        (*iterCell)->UpdateForceP();
		
		// Add in the missing interactions in the +X-Z direction
		
		BeadList lBeads = (*iterCell)->GetBeads();
		
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
		    ((*iterCell)->GetNNCell(2))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(5))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
		}
    } 
}

// Local version of the CalculateSharedForce() for one of the Edges that includes its particular missing bead-bead interactions.

void mpsEdge::CalculateSharedForceDT()
{	
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        (*iterCell)->UpdateForceP();
		
		// Add in the missing interactions in the +X-Z direction
		
		BeadList lBeads = (*iterCell)->GetBeads();
		
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
		    ((*iterCell)->GetNNCell(6))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(7))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(15))->UpdateForceBetweenCells(true,*iterBead);
		}
    } 
}

// Local version of the CalculateSharedForce() for one of the Edges that includes its particular missing bead-bead interactions.

void mpsEdge::CalculateSharedForceTR()
{	
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        (*iterCell)->UpdateForceP();
		
		// Add in the missing interactions in the +X-Z direction
		
		BeadList lBeads = (*iterCell)->GetBeads();
		
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
		    ((*iterCell)->GetNNCell(2))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(5))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(6))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(7))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(15))->UpdateForceBetweenCells(true,*iterBead);
		}
    } 
}

// Local version of the CalculateSharedForce() for one of the Edges that includes its particular missing bead-bead interactions.

void mpsEdge::CalculateSharedForceDR()
{	
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        (*iterCell)->UpdateForceP();
		
		// Add in the missing interactions in the +X-Z direction
		
		BeadList lBeads = (*iterCell)->GetBeads();
		
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
		    ((*iterCell)->GetNNCell(2))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(5))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
		}
    } 
}


// Local version of the CalculateSharedForce() for one of the Edges that includes its particular missing bead-bead interactions.

void mpsEdge::CalculateSharedForceBR()
{	
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        (*iterCell)->UpdateForceP();
		
		// Add in the missing interactions in the +X-Z direction
		
		BeadList lBeads = (*iterCell)->GetBeads();
		
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
		    ((*iterCell)->GetNNCell(2))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(5))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(18))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(19))->UpdateForceBetweenCells(true,*iterBead);
		}
    } 
}

// Local version of the CalculateSharedForce() for one of the Edges that includes its particular missing bead-bead interactions.

void mpsEdge::CalculateSharedForceTL()
{	
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        (*iterCell)->UpdateForceP();
		
		// Add in the missing interactions in the +X-Z direction
		
		BeadList lBeads = (*iterCell)->GetBeads();
		
		for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
		    ((*iterCell)->GetNNCell(6))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(7))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(8))->UpdateForceBetweenCells(true,*iterBead);
		    ((*iterCell)->GetNNCell(15))->UpdateForceBetweenCells(true,*iterBead);
		}
    } 
}


void mpsEdge::UpdateSharedForce()
{
    if(GetRank() == 0 && m_XN == -1 && m_YN == 1 && m_ZN == 0)
	{
        std::cout << "Proc " << GetRank() << " inside UpdateSharedForce for Edge " << m_XN << " " << m_YN << " " << m_ZN << zEndl;
	}
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        if(GetRank() == 0 && m_XN == -1 && m_YN == 1 && m_ZN == 0)
	    {
            std::cout << "Proc " << GetRank() << " calling UpdateForce for Edge cell  " << (*iterCell)->GetId() << " that contains beads " << zEndl;
			BeadList lBeads = (*iterCell)->GetBeads();
			for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
			{
			    std::cout << "Cell " << (*iterCell)->GetId() << " contains bead " << (*iterBead)->GetId() << zEndl;
			}
        }	 
		   
        (*iterCell)->UpdateForce();
    } 
}

// Function to copy the bead coordinates into the Border region's external
// CNT cells so that their non-bonded forces with beads in the current Corner
// can be calculated.
//
// An Edge adds the bead to the "index" nearest-neighbour cell of Edge cell "ix".

void mpsEdge::AddBeadToCell(long index, long ix, long id, long type, 
                            double x, double y, double z, double vx, double vy, double vz)
{
    dynamic_cast<CExternalCNTCell*>(m_vCNTCells.at(ix)->GetNNCell(index))->AddBeadToCell(id,type,x,y,z,vx,vy,vz);
}

// Specialised function to copy bead coordinates into the Edge's 
// external CNT cells when it controls Edge/Corner cells. It is similar to the
// above function except that it uses both Edge and Corner cells.

void mpsEdge::AddBeadToECCell(long index, long ix, long id, long type, 
                            double x, double y, double z, double vx, double vy, double vz)
{
    dynamic_cast<CExternalCNTCell*>(m_vExternalECCells.at(ix))->AddBeadToCell(id,type,x,y,z,vx,vy,vz);
}

// Function to return the container of CNT cells belonging to this Edge AND its
// adjacent two Corners. This is distinct from the set of CNT cells that 
// belong just to the Edge that are returned by the base class function GetCNTCells().

CNTCellVector& mpsEdge::GetECCells()
{
	return m_vECCells;
}

// Function to return the container of external CNT cells along this Edge AND its
// adjacent two Corners. Note that this function only returns the external cells that
// wrap the Edge/Corner DIAGONAl cells: not the cells along the Edge's two faces.
// These are included in the appropriate Face's GetExternalFECCells() function.

CNTCellVector& mpsEdge::GetExternalECCells()
{
    return m_vExternalECCells;
}

// Function to return the set of all beads in the CNT cells belonging to this
// Edge and its associated Corners. This is not the same as the external
// CNT Cells, for which a function is provided below.

BeadList mpsEdge::GetECBeads() const
{
    BeadList lBeads;
    lBeads.clear();

    for(cCNTCellIterator citerCell=m_vECCells.begin(); citerCell!=m_vECCells.end(); citerCell++)
    {
        BeadList ltb;
        ltb = (*citerCell)->GetBeads();
        copy(ltb.begin(), ltb.end(), back_inserter(lBeads));
    }

    return lBeads;
}

// Function to return the set of all beads in the external CNT cells adjacent to this
// Edge and its associated two Corners. This function assembles the bead
// pointers in a list and then removes them from all the cells. Hence, it must only
// be called ONCE per timestep as all calls subsequent to the first will return
// an empty list.

BeadList mpsEdge::GetExternalECBeads()
{
    BeadList lBeads;
    lBeads.clear();

    for(CNTCellIterator iterCell=m_vExternalECCells.begin(); iterCell!=m_vExternalECCells.end(); iterCell++)
    {
        BeadList ltb;
        ltb = (*iterCell)->GetBeads();
//		(*iterCell)->RemoveAllBeadsFromCell();
        copy(ltb.begin(), ltb.end(), back_inserter(lBeads));
    }
		
    return lBeads;
}

// Function to empty the external CNT cells before adding new beads to them.

void mpsEdge::ClearExternalECCells()
{
    for(CNTCellIterator iterCell=m_vExternalECCells.begin(); iterCell!=m_vExternalECCells.end(); iterCell++)
    {
//	    if(IsProcessZero())
//	        std::cout << "Edge " << m_XN << " " << m_YN << " " << m_ZN << " is clearing cell " << (*iterCell)->GetId() << zEndl;
        dynamic_cast<CExternalCNTCell*>(*iterCell)->Clear();
    }
}

// Function to remove all bead pointers from the external CNT cells.

void mpsEdge::RemoveBeadsFromExternalECCells()
{
    for(CNTCellIterator iterCell=m_vExternalECCells.begin(); iterCell!=m_vExternalECCells.end(); iterCell++)
    {
		(*iterCell)->RemoveAllBeadsFromCell();
    }
}

// Function to calculate extended bond forces between beads in this Edge's EC cells and its adjacent external cells that wrap the Border cells for a
// neighbouring processor. The algorithm currently assumes that bonded beads are always in adjacent cells - so bond lengths must not be longer 
// than the CNT cell width.

void mpsEdge::CalculateSharedBondForce()
{
    for(CNTCellIterator iterCell=m_vExternalECCells.begin(); iterCell!=m_vExternalECCells.end(); iterCell++)
    {
		dynamic_cast<CExternalCNTCell*>(*iterCell)->UpdateBondForceBetweenCells(m_pSimBox);
	}
}



