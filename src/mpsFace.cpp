/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpsFace.cpp: implementation of the mpsFace class.
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
// Constructor for the border class that represents the planar face between
// two adjacent processors. We pass the size of the Space in lx,ly,lz 
// so that it can copy the necessary CNT cells from the main array. 
// A Face is responsible for creating external CNT cells for itself and its
// four Edges and Corners, but it only manipulates the coordinates of beads 
// in its own external cells. The Edge and Corner instances retrieve their external
// cells in their constructors by copying pointers from their Face instances.

mpsFace::mpsFace(mpsSimBox* const pSimBox, bool bExternal, long lx, long ly, long lz, 
                 long xc, long yc, long zc, long xn, long yn, long zn,  
                 CNTCellVector vAllCNTCells) : mpsBorder(pSimBox, lx, ly, lz, xc, yc, zc),
                                           m_bExternal(bExternal),
                                           m_NNProcId(0), m_NNFaceId(0),
                                           m_XN(xn), m_YN(yn), m_ZN(zn)
{
    // Clear the containers of CNT cells and external CNT cells belonging to this 
    // Face AND its associated Edges and Corners.

    m_vFECCells.clear();
    m_vExternalFECCells.clear();

    // Store pointers to the CNT cells within the Face according to its normal vector.
    // Note that we do not check that only one of X,Y or Z are non-zero: the calling 
    // routine must guarantee this for us.

    long extIndex   = 0;
    long nnIndex    = 0;
    long nnIntIndex = 0;
    long alpha, beta, gamma;
    CCNTCell* pOldCell;
    CCNTCell* pNewCell;

    if(m_XN == 1)  // R face
    {
        m_NNProcId = m_pSimBox->GetNeighbourPidFromCoordinateDifferences(1, 0, 0); 
        m_NNFaceId = 1; // adjacent to Left face

        // First iterate over all cells in this FEC and store the Face's cells
        // and create external cells if this Face is external.

        for(long iz = 0; iz < m_LZ; iz++)
        {
			for(long iy = 0; iy < m_LY; iy++)
			{
                CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(m_LX-1,iy,iz));

                // All cells with x = Lx-1 except corners and edges
                if((iy > 0 && iy < m_LY-1) && (iz > 0 && iz < m_LZ-1))
                {
                    m_vCNTCells.push_back(pCell);
                }

                if(IsExternal())
                {
                    nnIndex  = m_pSimBox->MapCellToNNIndex(1,0,0);
                    pOldCell = pCell->GetNNCell(nnIndex);
                    pNewCell = new CExternalCNTCell(pOldCell);
                    m_vFECCells.push_back(pCell);
                    m_vExternalFECCells.push_back(pNewCell);
                }
            }
        }

        // Now iterate over the FEC cells again and connect the cells to each other's
        // external cells to allow the force loop to update bead-bead forces.

        if(IsExternal())
        {
			for(long iz = 0; iz < m_LZ; iz++)
			{
				for(long iy = 0; iy < m_LY; iy++)
				{
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(m_LX-1,iy,iz));

                    // We increment the intNNIndex counter for all cells but only use it
                    // if a neighbouring cell has actually been replaced.

                    nnIntIndex = 0;
                    for(gamma = -1; gamma < 2; gamma++)
                    {
                        for(beta = -1; beta < 2; beta++)
                        {
                            for(alpha = -1; alpha < 2; alpha++)
                            {
                                pNewCell = 0;
                                if(alpha == 1)
                                {
                                    nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                    extIndex = GetCellIndexFromCoords(iy, iz, 0, m_LY, m_LZ, 0, beta, gamma, 0);
                                    pNewCell = m_vExternalFECCells.at(extIndex);
                                    pCell->SetNNCellIndex(nnIndex, pNewCell);
                                    if(IsProcessZero())
                                    {
//                                        std::cout <<"R: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                    }
                                }
                                // Check if the neighbour cell should be added to the force loop array
                                if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                            {
                                    if(pNewCell)
                                    {
                                        if(IsProcessZero())
                                        {
//                                            std::cout <<"R: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndexpNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
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
    else if(m_XN == -1)  // L face
    {
        m_NNProcId = m_pSimBox->GetNeighbourPidFromCoordinateDifferences(-1, 0, 0);
        m_NNFaceId = 0; // adjacent to Right face

        for(long iz = 0; iz < m_LZ; iz++)
        {
			for(long iy = 0; iy < m_LY; iy++)
			{
                CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(0,iy,iz));

                // All cells with x = 0 except corners and edges
                if((iy > 0 && iy < m_LY-1) && (iz > 0 && iz < m_LZ-1))
                {
                    m_vCNTCells.push_back(pCell);
                }

                if(IsExternal())
                {
                    nnIndex  = m_pSimBox->MapCellToNNIndex(-1,0,0);
                    pOldCell = pCell->GetNNCell(nnIndex);
                    pNewCell = new CExternalCNTCell(pOldCell);
//					std::cout << "L Face wrapping external cell: " << nnIndex << " " << pOldCell->GetId() << zEndl;
                    m_vFECCells.push_back(pCell);
                    m_vExternalFECCells.push_back(pNewCell);
                }
            }
        }

        // Now iterate over the FEC cells again and connect the cells to each other's
        // external cells to allow the force loop to update bead-bead forces.

        if(IsExternal())
        {
			for(long iz = 0; iz < m_LZ; iz++)
			{
				for(long iy = 0; iy < m_LY; iy++)
				{
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(0,iy,iz));

                    // We increment the intNNIndex counter for all cells but only use it
                    // if a neighbouring cell has actually been replaced.

//					std::cout << "**********" << zEndl;
                    nnIntIndex = 0;
                    for(gamma = -1; gamma < 2; gamma++)
                    {
                        for(beta = -1; beta < 2; beta++)
                        {
                            for(alpha = -1; alpha < 2; alpha++)
                            {
                                pNewCell = 0;
                                if(alpha == -1)
                                {
                                    nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                    extIndex = GetCellIndexFromCoords(iy, iz, 0, m_LY, m_LZ, 0, beta, gamma, 0);
                                    pNewCell = m_vExternalFECCells.at(extIndex);
                                    pCell->SetNNCellIndex(nnIndex, pNewCell);
                                    if(IsProcessZero())
                                    {
//                                        std::cout <<"L: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                    }
                                }
                                // Check if the neighbour cell should be added to the force loop array
                                if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                            {
                                    if(pNewCell)
                                    {
                                        if(IsProcessZero())
                                        {
//                                            std::cout <<"L: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndexpNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
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
    else if(m_YN == 1)  // T face
    {
        m_NNProcId = m_pSimBox->GetNeighbourPidFromCoordinateDifferences(0, 1, 0);
        m_NNFaceId = 3; // adjacent to Bottom face

        for(long iz = 0; iz < m_LZ; iz++)
        {
			for(long ix = 0; ix < m_LX; ix++)
			{
                CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,m_LY-1,iz));

                // All cells with y = Ly-1 except corners and edges
                if((ix > 0 && ix < m_LX-1) && (iz > 0 && iz < m_LZ-1))
                {
                    m_vCNTCells.push_back(pCell);
                }

                if(IsExternal())
                {
                    nnIndex  = m_pSimBox->MapCellToNNIndex(0,1,0);
                    pOldCell = pCell->GetNNCell(nnIndex);
                    pNewCell = new CExternalCNTCell(pOldCell);
//					std::cout << "T Face wrapping external cell: " << nnIndex << " " << pOldCell->GetId() << zEndl;
                    m_vFECCells.push_back(pCell);
                    m_vExternalFECCells.push_back(pNewCell);
                }
            }
        }

        // Now iterate over the FEC cells again and connect the cells to each other's
        // external cells to allow the force loop to update bead-bead forces.

        if(IsExternal())
        {
			for(long iz = 0; iz < m_LZ; iz++)
			{
				for(long ix = 0; ix < m_LX; ix++)
				{
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,m_LY-1,iz));

                    // We increment the intNNIndex counter for all cells but only use it
                    // if a neighbouring cell has actually been replaced.

                    nnIntIndex = 0;
                    for(gamma = -1; gamma < 2; gamma++)
                    {
                        for(beta = -1; beta < 2; beta++)
                        {
                            for(alpha = -1; alpha < 2; alpha++)
                            {
                                pNewCell = 0;
                                if(beta == 1)
                                {
                                    nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                    extIndex = GetCellIndexFromCoords(ix, iz, 0, m_LX, m_LZ, 0, alpha, gamma, 0);
                                    pNewCell = m_vExternalFECCells.at(extIndex);
                                    pCell->SetNNCellIndex(nnIndex, pNewCell);
                                    if(IsProcessZero())
                                    {
//                                        std::cout <<"T: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                    }
                                }
                                // Check if the neighbour cell should be added to the force loop array
                                if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                            {
                                    if(pNewCell)
                                    {
                                        if(IsProcessZero())
                                        {
//                                            std::cout <<"T: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndexpNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
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
    else if(m_YN == -1)  // B face
    {
        m_NNProcId = m_pSimBox->GetNeighbourPidFromCoordinateDifferences(0, -1, 0);
        m_NNFaceId = 2; // adjacent to Top face

        for(long iz = 0; iz < m_LZ; iz++)
        {
			for(long ix = 0; ix < m_LX; ix++)
			{
                CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,0,iz));

                // All cells with y = 0 except corners and edges
                if((ix > 0 && ix < m_LX-1) && (iz > 0 && iz < m_LZ-1))
                {
                    m_vCNTCells.push_back(pCell);
                }

                if(IsExternal())
                {
                    nnIndex  = m_pSimBox->MapCellToNNIndex(0,-1,0);
                    pOldCell = pCell->GetNNCell(nnIndex);
                    pNewCell = new CExternalCNTCell(pOldCell);
//					std::cout << "B Face wrapping external cell: " << nnIndex << " " << pOldCell->GetId() << zEndl;
                    m_vFECCells.push_back(pCell);
                    m_vExternalFECCells.push_back(pNewCell);
                }
            }
        }

        // Now iterate over the FEC cells again and connect the cells to each other's
        // external cells to allow the force loop to update bead-bead forces.

        if(IsExternal())
        {
			for(long iz = 0; iz < m_LZ; iz++)
			{
				for(long ix = 0; ix < m_LX; ix++)
				{
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,0,iz));

                    // We increment the intNNIndex counter for all cells but only use it
                    // if a neighbouring cell has actually been replaced.

                    nnIntIndex = 0;
                    for(gamma = -1; gamma < 2; gamma++)
                    {
                        for(beta = -1; beta < 2; beta++)
                        {
                            for(alpha = -1; alpha < 2; alpha++)
                            {
                                pNewCell = 0;
                                if(beta == -1)
                                {
                                    nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                    extIndex = GetCellIndexFromCoords(ix, iz, 0, m_LX, m_LZ, 0, alpha, gamma, 0);
                                    pNewCell = m_vExternalFECCells.at(extIndex);
                                    pCell->SetNNCellIndex(nnIndex, pNewCell);
                                    if(IsProcessZero())
                                    {
//                                        std::cout <<"B: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                    }
                                }
                                // Check if the neighbour cell should be added to the force loop array
                                if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                            {
                                    if(pNewCell)
                                    {
                                        if(IsProcessZero())
                                        {
//                                            std::cout <<"B: cell " << pCell->GetId() << " adding force loop cell with alpha, beta, gamma,extIndexpNewCell->GetId() " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << " " << pNewCell->GetId() << zEndl;
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
    else if(m_ZN == 1)  // U face
    {
        m_NNProcId = m_pSimBox->GetNeighbourPidFromCoordinateDifferences(0, 0, 1);
        m_NNFaceId = 5; // adjacent to Down face

        // First iterate over all cells in this FEC and store the Face's cells
        // and create external cells if this Face is external.

        for(long iy = 0; iy < m_LY; iy++)
        {
            for(long ix = 0; ix < m_LX; ix++)
            {
                CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,iy,m_LZ-1));

                // All cells with z = Lz-1 except corners and edges
                if((ix > 0 && ix < m_LX-1) && (iy > 0 && iy < m_LY-1))
                {
                    m_vCNTCells.push_back(pCell);
                }

                if(IsExternal())
                {
                    nnIndex  = m_pSimBox->MapCellToNNIndex(0,0,1);
                    pOldCell = pCell->GetNNCell(nnIndex);
                    pNewCell = new CExternalCNTCell(pOldCell);
                    m_vFECCells.push_back(pCell);
                    m_vExternalFECCells.push_back(pNewCell);
                }
            }
        }

        // Now iterate over the FEC cells again and connect the cells to each other's
        // external cells to allow the force loop to update bead-bead forces.

        if(IsExternal())
        {
            for(long iy = 0; iy < m_LY; iy++)
            {
                for(long ix = 0; ix < m_LX; ix++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,iy,m_LZ-1));

                    // We increment the intNNIndex counter for all cells but only use it
                    // if a neighbouring cell has actually been replaced.

                    nnIntIndex = 0;
                    for(gamma = -1; gamma < 2; gamma++)
                    {
                        for(beta = -1; beta < 2; beta++)
                        {
                            for(alpha = -1; alpha < 2; alpha++)
                            {
                                pNewCell = 0;
                                if(gamma == 1)
                                {
                                    nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                    extIndex = GetCellIndexFromCoords(ix, iy, 0, m_LX, m_LY, 0, alpha, beta, 0);
                                    pNewCell = m_vExternalFECCells.at(extIndex);
                                    pCell->SetNNCellIndex(nnIndex, pNewCell);
                                    if(IsProcessZero())
                                    {
//                                        std::cout <<"U: adding external cell with alpha, beta, gamma, extIndex, nnIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                    }
                                }
                                // Check if the neighbour cell should be added to the force loop array
                                if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                            {
                                    if(pNewCell)
                                    {
                                        if(IsProcessZero())
                                        {
//                                            std::cout <<"U: adding force loop cell with alpha, beta, gamma,extIndex " << alpha << " " << beta << " " << gamma << " " << extIndex << " " << nnIntIndex << zEndl;
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
    else if(m_ZN == -1)  // D face
    {
        m_NNProcId = m_pSimBox->GetNeighbourPidFromCoordinateDifferences(0, 0, -1);
        m_NNFaceId = 4; // adjacent to Up face

        // First iterate over all cells in this FEC and store the Face's cells
        // and create external cells if this Face is external.

        for(long iy = 0; iy < m_LY; iy++)
        {
            for(long ix = 0; ix < m_LX; ix++)
            {
                CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,iy,0));

                // All cells with z = 0 except corners and edges
                if((ix > 0 && ix < m_LX-1) && (iy > 0 && iy < m_LY-1))
                {
                    m_vCNTCells.push_back(pCell);
                }

                if(IsExternal())
                {
                    nnIndex  = m_pSimBox->MapCellToNNIndex(0,0,-1);
                    pOldCell = pCell->GetNNCell(nnIndex);
                    pNewCell = new CExternalCNTCell(pOldCell);
                    m_vFECCells.push_back(pCell);
                    m_vExternalFECCells.push_back(pNewCell);
                }
            }
        }

        // Now iterate over the FEC cells again and connect the cells to each other
        // to allow the force loop to update bead-bead forces.

        if(IsExternal())
        {
            for(long iy = 0; iy < m_LY; iy++)
            {
                for(long ix = 0; ix < m_LX; ix++)
                {
                    CCNTCell* pCell = vAllCNTCells.at(GetCellIndexFromCoords(ix,iy,0));

                    // We increment the intNNIndex counter for all cells but only use it
                    // if a neighbouring cell has actually been replaced.

                    nnIntIndex = 0;
                    for(gamma = -1; gamma < 2; gamma++)
                    {
                        for(beta = -1; beta < 2; beta++)
                        {
                            for(alpha = -1; alpha < 2; alpha++)
                            {
                                pNewCell = 0;
                                if(gamma == -1)
                                {
                                    nnIndex  = m_pSimBox->MapCellToNNIndex(alpha,beta,gamma);
                                    extIndex = GetCellIndexFromCoords(ix, iy, 0, m_LX, m_LY, 0, alpha, beta, 0);
                                    pNewCell = m_vExternalFECCells.at(extIndex);
                                    pCell->SetNNCellIndex(nnIndex, pNewCell);
                                    if(IsProcessZero())
                                    {
//                                        std::cout <<"D: adding external cell with alpha, beta, gamma, nnIndex, extIndex, pCell->id, pNewCell->id " << alpha << " " << beta << " " << gamma << " " << nnIndex << " " << extIndex << " " << pCell->GetId() << " " << pNewCell->GetId() << zEndl;
                                    }
                                }
                                // Check if the neighbour cell should be added to the force loop array
                                if( gamma==1 || (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
	                            {
                                    if(pNewCell)
                                    {
                                        if(IsProcessZero())
                                        {
//                                            std::cout <<"D: adding force loop cell with alpha, beta, gamma,nnIndex " << alpha << " " << beta << " " << gamma << " " << nnIndex << " " << nnIntIndex << zEndl;
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
	
//	std::cout << "Face " << m_XN << " " << m_YN << " " << m_ZN << " has internal/external cell totals " << m_vCNTCells.size() << "  " << m_vFECCells.size() << "  " << m_vExternalFECCells.size() << zEndl;
}


// Copy constructor

mpsFace::mpsFace(const mpsFace& oldBorder) : mpsBorder(oldBorder),
                                             m_bExternal(oldBorder.m_bExternal),
                                             m_NNProcId(oldBorder.m_NNProcId), 
                                             m_NNFaceId(oldBorder.m_NNFaceId),
                                             m_XN(oldBorder.m_XN),
                                             m_YN(oldBorder.m_YN),
                                             m_ZN(oldBorder.m_ZN),
                                             m_vFECCells(oldBorder.m_vFECCells),
                                             m_vExternalFECCells(oldBorder.m_vExternalFECCells)
{
    // Note that the beads are not copied.
}
                           
// Destructor

mpsFace::~mpsFace()
{
    // Face instances own their external CNT Cells and those of their adjacent Edges and
    // Corners for the linear SimBox geometries, so we destroy them here. Note that 
    // these instances are not the same as the SimBox CNT cells that are copied into 
    // the Faces, but have been created in the
    // constructor to wrap the layer of cells belonging to the adjacent processors.

    if(!m_vExternalFECCells.empty())
    {
        for(CNTCellIterator iterCell=m_vExternalFECCells.begin(); iterCell!=m_vExternalFECCells.end(); iterCell++)
        {
            delete *iterCell;
        }
            m_vExternalFECCells.clear();
    }

}

// PVF showing if this Border region spans two or more processors. A Face is external
// if it was created with its external flag set to true.

bool mpsFace::IsExternal() const
{
    return m_bExternal;
}

// Function to update the positions of the beads in this Border region if it
// does not need to communicate with other processors. Which regions this applies
// to depends on the simulation Space geometry.

void mpsFace::UpdatePos()
{
	for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdatePos();
	} 
}

// Function to update the positions of the beads in this Border region if it needs
// to communicate with other processors. A Face instance only needs to communicate 
// with one other processor.
//
// We loop over all beads in the Face, and check for movement in the following
// order:
//
// 1 Bead moves within Face
// 2 Bead moves back into Bulk
// 3 Bead moves to neighbouring processor
//
// Class 3 beads are collected in the lost bead map, stored in the Border base class,
// This stores the beads using their destination processor's pid as the key into
// the map.

void mpsFace::UpdateSharedPos()
{
    // We loop over all the cells in this Face/Edge/Corner region and collect 
    // the beads that need to be moved to a neighbouring processor. We call the 
    // normal UpdatePos function that moves beads into the cells' external
    // neighbours' wrapped CAbstractCell instances. Then we retrieve them and
    // package them into the message that is sent to the adjacent processor.

	for(CNTCellIterator iterCell=m_vFECCells.begin(); iterCell!=m_vFECCells.end(); iterCell++)
	{
		(*iterCell)->UpdatePos();
	} 
}

// PVF to calculate the non-bonded, bead-bead forces for all beads in this Border
// region that do NOT depend on beads owned by adjacent processors. By definition,
// this means the Face must be periodically connected to the opposite Face in the 
// same processor. The CCNTCell class applies the PBCs in the Face's normal direction.

void mpsFace::UpdateForce()
{
	for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdateForce();
	} 
}

// PVF to calculate the non-bonded bead-bead forces for all beads in this Border
// region that may depend on the coordinates of beads owned by adjacent processors.
// We also have to store the random forces so that we can send them to the 
// originating processor later.
//
// We get the direction of the adjacent processors from the Face normal so that
// we know in which direction the Face's CNT cells have to access bead coordinates
// from the data stored in the mpsSimBox. In the other two directions, bead pointers
// can be obtained from the usual CNT cell network. Beads owned by the adjacent 
// processor that are in the Border regions adjacent to this Face are passed to this
// function in a container so that we can loop over their interactions with beads 
// in this Face. Note that this container MUST include ALL  beads that potentially
// can interact with any beads in this Face AND its adjacent Edges and Corners.
// The set of such beads depends on the geometry of the simulation Space. It is
// simplest for the linear SimBox as it then only contains the adjacent processor's
// Face and adjacent Edges and Corners.
//
// This function must be called after all bead coordinates have been received from
// all adjacent processors and stored in the CExternalCNTCell instances created in
// the constructor. It has to perform the following tasks:
//
// 1 Calculate internal forces between bead pairs in this processor's Face
//
// 2    "          "       "    between beads in this Face and its adjacent Edges
//
// 3    "          "       "    between beads in this Face and its Corners 
//
// 4 Calculate all external forces between beads in this processor's Face and all 
//   CExternalCNTCells adjacent to the Face
//   
// The procedure is that a Face calculates bead-bead forces for itself and those 
// bead pairs that span the Face and its adjacent Edges and Corners. Then the
// same function is called for each Edge adjacent to the Face to add the bead-bead
// forces for pairs within the Edges and Edge-Corners. Finally, the same function 
// is called for each Corner adjacent to the Face to add in the (few) bead-bead
// forces for beads contained in the Corner. In this way, all bead-bead forces
// are collected and summed for beads in or adjacent to the Face.

void mpsFace::CalculateSharedForce()
{
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        (*iterCell)->UpdateForceP();
    } 
}

// Local version of the CalculateSharedForce() for the T Face that includes the missing bead-bead interactions.

void mpsFace::CalculateSharedForceT()
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

// Local version of the CalculateSharedForce() for the R Face that includes the missing bead-bead interactions.

void mpsFace::CalculateSharedForceR()
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

// PVF to calculate the forces between beads in cells belonging to this Face that are in addition to forces transmitted from
// the adjacent processor.

void mpsFace::UpdateSharedForce()
{
    for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
    {
        (*iterCell)->UpdateForce();
    } 
}

// Functions to copy the bead coordinates into the Border region's external
// CNT cells so that their non-bonded forces with beads in the current Corner
// can be calculated. Three versions are provided for Faces with X,Y,Z normals.
// This avoids having to pass extra arguments or make tests on the Face normal.
//
// A Face adds the bead to the "index" nearest-neighbour cell of the cell 
// identified by the 2d indices "ix, iy". Note that these do not necessarily
// correspond to the x,y axes of the SimBox, and their range is limited to the
// size of the Face not the size of the SimBox in the plane of the Face.


void mpsFace::AddBeadToXCell(long index, long ix, long iy, long id, long type, 
                             double x, double y, double z, double vx, double vy, double vz)
{
    dynamic_cast<CExternalCNTCell*>(m_vCNTCells.at(GetCellIndexFromCoords(ix,iy,0,GetCNTYCellNo(),GetCNTZCellNo()))->GetNNCell(index))->AddBeadToCell(id,type,x,y,z,vx,vy,vz);

}

void mpsFace::AddBeadToYCell(long index, long ix, long iy, long id, long type, 
                             double x, double y, double z, double vx, double vy, double vz)
{
    dynamic_cast<CExternalCNTCell*>(m_vCNTCells.at(GetCellIndexFromCoords(ix,iy,0,GetCNTZCellNo(),GetCNTXCellNo()))->GetNNCell(index))->AddBeadToCell(id,type,x,y,z,vx,vy,vz);

}

void mpsFace::AddBeadToZCell(long index, long ix, long iy, long id, long type, 
                             double x, double y, double z, double vx, double vy, double vz)
{
    dynamic_cast<CExternalCNTCell*>(m_vCNTCells.at(GetCellIndexFromCoords(ix,iy,0,GetCNTXCellNo(),GetCNTYCellNo()))->GetNNCell(index))->AddBeadToCell(id,type,x,y,z,vx,vy,vz);

}

// Specialised function to copy bead coordinates into the Face's external CNT cells when
// it controls Face/Edge/Corner cells. It is similar to the above function, but is used 
// in the linear SimBox geometry, and perhaps others yet to be implemented.

void mpsFace::AddBeadToFECXCell(long index, long ix, long iy, long id, long type, 
                                double x, double y, double z, double vx, double vy, double vz)
{
//    if(x < 0 || y < 0 || z < 0)
//	{
//      std::cout << "FECX " << index << " " << id << " " << ix << " "<< iy  << " " << x << " " << y << " " << z <<  zEndl;
//	}
//	else
	{	
      dynamic_cast<CExternalCNTCell*>(m_vExternalFECCells.at(GetCellIndexFromCoords(ix,iy,0,GetLY(),GetLZ())))->AddBeadToCell(id,type,x,y,z,vx,vy,vz);
    }	
}

// Specialised function to copy bead coordinates into the Face's external CNT cells when
// it controls Face/Edge/Corner cells. It is similar to the above function, but is used 
// in the linear SimBox geometry, and perhaps others yet to be implemented.

void mpsFace::AddBeadToFECYCell(long index, long ix, long iy, long id, long type, 
                                double x, double y, double z, double vx, double vy, double vz)
{
//    if(x < 0 || y < 0 || z < 0)
//	{
//      std::cout << "FECY " << index << " " << id << " " << ix << " "<< iy  << " " << x << " " << y << " " << z << zEndl;
//	}
//    else
	{
      dynamic_cast<CExternalCNTCell*>(m_vExternalFECCells.at(GetCellIndexFromCoords(ix,iy,0,GetLX(),GetLZ())))->AddBeadToCell(id,type,x,y,z,vx,vy,vz);
    }
}

// Specialised function to copy bead coordinates into the Face's external CNT cells when
// it controls Face/Edge/Corner cells. It is similar to the above function, but is used 
// in the linear SimBox geometry, and perhaps others yet to be implemented.

void mpsFace::AddBeadToFECZCell(long index, long ix, long iy, long id, long type, 
                                double x, double y, double z, double vx, double vy, double vz)
{
//    if(x < 0 || y < 0 || z < 0)
//	{
//      std::cout << "FECZ " << index << " " << id << " " << ix << " "<< iy  << " " << x << " " << y << " " << z << zEndl;
//	}
//    else 
	{
      dynamic_cast<CExternalCNTCell*>(m_vExternalFECCells.at(GetCellIndexFromCoords(ix,iy,0,GetLX(),GetLY())))->AddBeadToCell(id,type,x,y,z,vx,vy,vz);
	}
}

// Function to return the set of all beads in the CNT cells belonging to this
// Face and its associated Edges and Corners. This is not the same as the external
// CNT Cells, for which a function is provided below.

BeadList mpsFace::GetFECBeads() const
{
    BeadList lBeads;
    lBeads.clear();

    for(cCNTCellIterator citerCell=m_vFECCells.begin(); citerCell!=m_vFECCells.end(); citerCell++)
    {
        BeadList ltb;
        ltb = (*citerCell)->GetBeads();
        copy(ltb.begin(), ltb.end(), back_inserter(lBeads));
    }

    return lBeads;
}

// Function to return the set of all beads in the external CNT cells adjacent to this
// Face and its associated Edges and Corners. This function assembles the bead
// pointers in a list and then removes them from all the cells. Hence, it must only
// be called ONCE per timestep as all calls subsequent to the first will return
// an empty list.

BeadList mpsFace::GetExternalFECBeads()
{
    BeadList lBeads;
    lBeads.clear();

    for(CNTCellIterator iterCell=m_vExternalFECCells.begin(); iterCell!=m_vExternalFECCells.end(); iterCell++)
    {
        BeadList ltb;
        ltb = (*iterCell)->GetBeads();
//		(*iterCell)->RemoveAllBeadsFromCell();
        copy(ltb.begin(), ltb.end(), back_inserter(lBeads));
    }

    return lBeads;
}

BeadList mpsFace::GetExternalFBeads()
{
    BeadList lBeads;
    lBeads.clear();

    for(CNTCellIterator iterCell=m_vExternalCNTCells.begin(); iterCell!=m_vExternalCNTCells.end(); iterCell++)
    {
        BeadList ltb;
        ltb = (*iterCell)->GetBeads();
//		(*iterCell)->RemoveAllBeadsFromCell();
        copy(ltb.begin(), ltb.end(), back_inserter(lBeads));
    }

    return lBeads;
}

// Function to return the container of CNT cells belonging to this Face AND its
// adjacent Edges and Corners. This is distinct from the set of CNT cells that 
// belong just to the Face that are returned by the base class function GetCNTCells().

CNTCellVector& mpsFace::GetFECCells()
{
    return m_vFECCells;
}

// Function to return the container of external CNT cells in this Face AND its
// adjacent Edges and Corners. This is distinct from the external cells belonging
// just to the Face that are returned by the base class function GetExternalCNTCells().

CNTCellVector& mpsFace::GetExternalFECCells()
{
//    std::cout << "Face " << m_XN << " " << m_YN << " " << m_ZN << " has " << m_vExternalFECCells.size() << " external FEC cells" << zEndl;
    return m_vExternalFECCells;
}

// Function to empty the external CNT cells before adding new beads to them.

void mpsFace::ClearExternalFECCells()
{
    for(CNTCellIterator iterCell=m_vExternalFECCells.begin(); iterCell!=m_vExternalFECCells.end(); iterCell++)
    {
//	    if(IsProcessZero())
//	        std::cout << "Face " << m_XN << " " << m_YN << " " << m_ZN << " is clearing cell " << (*iterCell)->GetId() << zEndl;
        dynamic_cast<CExternalCNTCell*>(*iterCell)->Clear();
    }
}

// Function to remove all bead pointers from the external CNT cells.

void mpsFace::RemoveBeadsFromExternalFECCells()
{
    for(CNTCellIterator iterCell=m_vExternalFECCells.begin(); iterCell!=m_vExternalFECCells.end(); iterCell++)
    {
		(*iterCell)->RemoveAllBeadsFromCell();
    }
}

// Function to calculate extended bond forces between beads in this Face's FEC cells and its adjacent external cells that wrap the Border cells for a
// neighbouring processor. The algorithm currently assumes that bonded beads are always in adjacent cells - so bond lengths must not be longer 
// than the CNT cell width.

void mpsFace::CalculateSharedBondForce()
{
    for(CNTCellIterator iterCell=m_vExternalFECCells.begin(); iterCell!=m_vExternalFECCells.end(); iterCell++)
    {
		dynamic_cast<CExternalCNTCell*>(*iterCell)->UpdateBondForceBetweenCells(m_pSimBox);
	}
}



