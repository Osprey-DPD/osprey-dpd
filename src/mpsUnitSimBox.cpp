/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpsUnitSimBox.cpp: implementation of the mpsUnitSimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mpsUnitSimBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// This class represents the parallel SimBox running on a single processor.
// This case is a bit pathological, and will probably run slower than the
// equivalent serial code due to the uneccessary overhead of checking
// unused parallel functions. It is included here for completeness.
// We assign a geometry of 0 to allow us to distinguish this case from all others.

mpsUnitSimBox::mpsUnitSimBox(long lx, long ly, long lz, double dx, double dy, double dz, 
                             CNTCellVector&  rvCNTCells,
							 PolymerVector& rvBulkPolymers) : mpsSimBox(0, 0, 1, 1, 1, lx, ly, lz, dx, dy, dz, rvCNTCells, rvBulkPolymers)
{
    // Store the pids of the containing processor's nearest neighbours for use
    // in message passing. Each processor's pid is its rank in the World.
    // If the assembly function fails, we set a flag in the base class
    // indicating that the simulation must be aborted.

    if(ConnectNeighbours())
    {
        // Now partition the processor's Space into regions and store the pid of
        // the processors adjacent to each Face of this processor. This function 
        // must be called AFTER ConnectNeighbours() as it assumes that the pids
        // have already been assigned.

        PartitionSpace();
    }
    else
    {
        SetAssemblyFailed();
    }
}

// Copy constructor

mpsUnitSimBox::mpsUnitSimBox(const mpsUnitSimBox& oldSimBox) : mpsSimBox(oldSimBox)
{

}


mpsUnitSimBox::~mpsUnitSimBox()
{

}

// PVF to calculate the forces on all beads in the Border regions of the processor's
// SimBox. For this class, which has only a single processor, we calculate
// the forces using the serial code that employs the CNTCell network and pbcs.

void mpsUnitSimBox::UpdateBorderPos()
{
    // This function needs to loop over the Border regions even thought they
    // are all internal so that we can check that the internal cells are correctly
    // updated.
}

void mpsUnitSimBox::UpdateBorderForce()
{
    // This function needs to loop over the Border regions even thought they
    // are all internal so that we can check that the internal cells are correctly
    // updated.
}

// PVF to store pointers to the processor's CNT cells according to 
// whether they belong to the Bulk, Face, Edge or Corner regions and to connect
// up all Border cells to the appropriate CExternalCNTCell instances according
// to their position within the Border.
//
// Bulk    = all CNT cells not on the borders of the processor's Space
// Faces   = planar area of cells on the faces of the Space
// Edges   = linear sequence of cells on the edges of the Space
// Corners = single cells on the corners of the Space
//
// Only the Bulk cells are stored directly here, the other cells are stored
// in regions that are derived from mpsBorder. Face regions are special:
// we store the pid of the adjacent processor in them and the id of the adjacent
// Face region.
//
// This is a linear Space so we connect the two adjacent processors along the
// Z axis of the Space and add the CNT cells in the X,Y directions that are not
// in the Z Border regions to the Bulk cells. This allows us to iterate over all
// CNT cells that do not require messaging in one loop.

void mpsUnitSimBox::PartitionSpace()
{
    // Copy the Bulk CNT cells into a second array to make iterating over internal cells
    // efficient. This includes all cells except those on the borders of the Space.
    // For this class, the Borders are just the layer of CNT cells at the Z ends
    // of the processor's Space.

    m_vBulkCNTCells.clear();

    long bulkCounter = 0;

    for(long iz = 1; iz < m_LZ-1; iz++)
    {
        for(long iy = 1; iy < m_LY-1; iy++)
        {
            for(long ix = 1; ix < m_LX-1; ix++)
            {
                bulkCounter++;
                m_vBulkCNTCells.push_back(m_rvCNTCells.at(GetCellIndexFromCoords(ix,iy,iz)));
            }
        }
    }

    std::cout << "Counted " << bulkCounter << " internal cells" << zEndl;

    // Now assign the Faces, Edges and Corners; we pass in the whole CNT cell array, 
    // the size of the Space, the dimensions of the Face region and its normal vector, 
    // and the pid of the adjacent processor. This allows each Face to send messages 
    // to the adjacent processor.  The constructors also replace pointers to CNT cells 
    // in those Border regions that cross the PBCs in the directions of adjacent 
    // processors by pointers to newly-created CExternalCNTCells so 
    // that forces and bead movements of beads in these cells can be updated.

    m_pR = new mpsFace(this, false, m_LX, m_LY, m_LZ, 1, m_LY-2, m_LZ-2,  1,  0,  0, m_rvCNTCells);
    m_pL = new mpsFace(this, false, m_LX, m_LY, m_LZ, 1, m_LY-2, m_LZ-2, -1,  0,  0, m_rvCNTCells);
    m_pT = new mpsFace(this, false, m_LX, m_LY, m_LZ, m_LX-2, 1, m_LZ-2,  0,  1,  0, m_rvCNTCells);
    m_pB = new mpsFace(this, false, m_LX, m_LY, m_LZ, m_LX-2, 1, m_LZ-2,  0, -1,  0, m_rvCNTCells);
    m_pD = new mpsFace(this, false, m_LX, m_LY, m_LZ, m_LX-2, m_LY-2, 1,  0,  0, -1, m_rvCNTCells);
    m_pU = new mpsFace(this, false, m_LX, m_LY, m_LZ, m_LX-2, m_LY-2, 1,  0,  0,  1, m_rvCNTCells);

    // Next assign the edges; similar to the Faces, we pass in the whole CNT cell array,
    // the size of the Space and the Edge dimensions. We identify the Edges by the
    // coordinates of their column setting the component of their normal vector 
    // -1 to distinguish it from the other components.
/*
    m_pTR = new mpsEdge(this, false, m_LX, m_LY, m_LZ, 1, 1, m_LZ-2,  1,  1, -1, m_rvCNTCells);
    m_pBR = new mpsEdge(this, false, m_LX, m_LY, m_LZ, 1, 1, m_LZ-2,  1,  0, -1, m_rvCNTCells);
    m_pTL = new mpsEdge(this, false, m_LX, m_LY, m_LZ, 1, 1, m_LZ-2,  0,  1, -1, m_rvCNTCells);
    m_pBL = new mpsEdge(this, false, m_LX, m_LY, m_LZ, 1, 1, m_LZ-2,  0,  0, -1, m_rvCNTCells);
    m_pDR = new mpsEdge(this, false, m_LX, m_LY, m_LZ, 1, m_LY-2, 1,  1, -1,  0, m_rvCNTCells);
    m_pDL = new mpsEdge(this, false, m_LX, m_LY, m_LZ, 1, m_LY-2, 1,  0, -1,  0, m_rvCNTCells);
    m_pDT = new mpsEdge(this, false, m_LX, m_LY, m_LZ, m_LX-2, 1, 1, -1,  1,  0, m_rvCNTCells);
    m_pDB = new mpsEdge(this, false, m_LX, m_LY, m_LZ, m_LX-2, 1, 1, -1,  0,  0, m_rvCNTCells);
    m_pUR = new mpsEdge(this, false, m_LX, m_LY, m_LZ, 1, m_LY-2, 1,  1, -1,  1, m_rvCNTCells);
    m_pUL = new mpsEdge(this, false, m_LX, m_LY, m_LZ, 1, m_LY-2, 1,  0, -1,  1, m_rvCNTCells);
    m_pUT = new mpsEdge(this, false, m_LX, m_LY, m_LZ, m_LX-2, 1, 1, -1,  1,  1, m_rvCNTCells);
    m_pUB = new mpsEdge(this, false, m_LX, m_LY, m_LZ, m_LX-2, 1, 1, -1,  0,  1, m_rvCNTCells);

//    m_pDTR = new mpsCorner(this, false, 0, m_LY-1, m_LZ-1, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(0,m_LY-1,m_LZ-1)));
//    m_pDTL = new mpsCorner(this, false, 0, m_LY-1, 0, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(0,m_LY-1,0)));
//    m_pDBR = new mpsCorner(this, false, 0, 0, m_LZ-1, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(0,0,m_LZ-1)));
//    m_pDBL = new mpsCorner(this, false, 0, 0, 0, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(0,0,0)));    
//    m_pUTR = new mpsCorner(this, false, m_LX-1, m_LY-1, m_LZ-1, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(m_LX-1,m_LY-1,m_LZ-1)));
//    m_pUTL = new mpsCorner(this, false, m_LX-1, m_LY-1, 0, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(m_LX-1,m_LY-1,0)));
//    m_pUBR = new mpsCorner(this, false, m_LX-1, 0, m_LZ-1, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(m_LX-1,0,m_LZ-1)));
//    m_pUBL = new mpsCorner(this, false, m_LX-1, 0, 0, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(m_LX-1,0,0)));

*/
}



