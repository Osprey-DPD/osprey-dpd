/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
 // mpsPlanarZSimBox.cpp: implementation of the mpsPlanarZSimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mpsPlanarZSimBox.h"


long mpsPlanarZSimBox::netTotal = 0;
long mpsPlanarZSimBox::RFaceDifference = 0;
long mpsPlanarZSimBox::LFaceDifference = 0;
long mpsPlanarZSimBox::TFaceDifference = 0;
long mpsPlanarZSimBox::BFaceDifference = 0;
long mpsPlanarZSimBox::TREdgeDifference = 0;
long mpsPlanarZSimBox::BLEdgeDifference = 0;
long mpsPlanarZSimBox::TLEdgeDifference = 0;
long mpsPlanarZSimBox::BREdgeDifference = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// This class represents the parallel SimBox for processors with a
// planar arrangement in Space for which only the X,Y dimensions contain
// more than one processor. This means that in the Z dimension each 
// processor only has to communicate with itself.

mpsPlanarZSimBox::mpsPlanarZSimBox(long px, long py, long pz, long lx, long ly, long lz,
                                   double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
								   PolymerVector& rvBulkPolymers) : mpsSimBox(2, 3, px, py, pz, lx, ly, lz, dx, dy, dz, rvCNTCells, rvBulkPolymers)
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

	// Set tags for each message to distinguish different instances of the same 
	// message class. Note that the corner diagonal messages were added last, hence
	// their tags are the highest. I left this in to show that these messages
	// we missed in the original code.
	 
	m_Msg1L.SetTag(1);
	m_Msg1B.SetTag(2);
	m_Msg1TL.SetTag(3);
	m_Msg1BL.SetTag(4);
	m_Msg1DB.SetTag(5);
	m_Msg1UL.SetTag(6);
	m_Msg1UB.SetTag(7);
	m_Msg1DL.SetTag(8);
    m_Msg1DBL.SetTag(9);
	m_Msg1UTL.SetTag(10);
	m_Msg1UBL.SetTag(11);
    m_Msg1DTL.SetTag(12);

	m_Msg2R.SetTag(13);
	m_Msg2T.SetTag(14);
	m_Msg2BR.SetTag(15);
	m_Msg2TR.SetTag(16);
	m_Msg2UT.SetTag(17);
	m_Msg2DR.SetTag(18);
	m_Msg2DT.SetTag(19);
	m_Msg2UR.SetTag(20);
	m_Msg2UTR.SetTag(21);
    m_Msg2DBR.SetTag(22);
    m_Msg2DTR.SetTag(23);
    m_Msg2UBR.SetTag(24);

	m_Msg3R.SetTag(25);
	m_Msg3T.SetTag(26);
	m_Msg4R.SetTag(27);
	m_Msg4T.SetTag(28);
	 
	m_Msg3TR.SetTag(29);
	m_Msg3TL.SetTag(30);
	m_Msg4TR.SetTag(31);
	m_Msg4TL.SetTag(32);
	
	// Additional messages for Edges and Corners that cross Z boundaries.
	
	m_Msg3UT.SetTag(33);
	m_Msg3UB.SetTag(34);
	m_Msg3UR.SetTag(35);
	m_Msg3UL.SetTag(36);
	m_Msg4UT.SetTag(37);
	m_Msg4UB.SetTag(38);
	m_Msg4UR.SetTag(39);
	m_Msg4UL.SetTag(40);
	
    m_Msg3UTR.SetTag(41);	
    m_Msg3UTL.SetTag(42);	
    m_Msg3UBR.SetTag(43);	
    m_Msg3UBL.SetTag(44);	
    m_Msg4UTR.SetTag(45);	
    m_Msg4UTL.SetTag(46);	
    m_Msg4UBR.SetTag(47);	
    m_Msg4UBL.SetTag(48);	
	
	
    // Set the pids of the adjacent processors. Note that the other pids will have illegal values that are set in the base class.

	UFacePid  = GetNeighbourPidFromCoordinateDifferences(0,0,1);
    DFacePid  = GetNeighbourPidFromCoordinateDifferences(0,0,-1);
    RFacePid  = GetNeighbourPidFromCoordinateDifferences(1,0,0);
    LFacePid  = GetNeighbourPidFromCoordinateDifferences(-1,0,0);
    TFacePid  = GetNeighbourPidFromCoordinateDifferences(0,1,0);
    BFacePid  = GetNeighbourPidFromCoordinateDifferences(0,-1,0);

    TREdgePid = GetNeighbourPidFromCoordinateDifferences(1,1,0);
    TLEdgePid = GetNeighbourPidFromCoordinateDifferences(-1,1,0);
    BREdgePid = GetNeighbourPidFromCoordinateDifferences(1,-1,0);
    BLEdgePid = GetNeighbourPidFromCoordinateDifferences(-1,-1,0);

    UREdgePid = GetNeighbourPidFromCoordinateDifferences(1,0,1);
    ULEdgePid = GetNeighbourPidFromCoordinateDifferences(-1,0,1);
    UTEdgePid = GetNeighbourPidFromCoordinateDifferences(0,1,1);
    UBEdgePid = GetNeighbourPidFromCoordinateDifferences(0,-1,1);

    DREdgePid = GetNeighbourPidFromCoordinateDifferences(1,0,-1);
    DLEdgePid = GetNeighbourPidFromCoordinateDifferences(-1,0,-1);
    DTEdgePid = GetNeighbourPidFromCoordinateDifferences(0,1,-1);
    DBEdgePid = GetNeighbourPidFromCoordinateDifferences(0,-1,-1);

    UTRCornerPid = GetNeighbourPidFromCoordinateDifferences(1,1,1);
    UTLCornerPid = GetNeighbourPidFromCoordinateDifferences(-1,1,1);
    UBRCornerPid = GetNeighbourPidFromCoordinateDifferences(1,-1,1);
    UBLCornerPid = GetNeighbourPidFromCoordinateDifferences(-1,-1,1);
    DTRCornerPid = GetNeighbourPidFromCoordinateDifferences(1,1,-1);
    DTLCornerPid = GetNeighbourPidFromCoordinateDifferences(-1,1,-1);
    DBRCornerPid = GetNeighbourPidFromCoordinateDifferences(1,-1,-1);
    DBLCornerPid = GetNeighbourPidFromCoordinateDifferences(-1,-1,-1);
	
	
	// Set the initial containers for empty bead instances in each message class needed in this geometry.
	
	pmSendPlanarBeads::InitialiseEmptyBeadContainer();
	pmSendPlanarEdgeBeads::InitialiseEmptyBeadContainer();
	pmSendCubicCornerBeads::InitialiseEmptyBeadContainer();


// Zero the debug counters

    m_LCoordCounter = 0;
    m_BCoordCounter = 0;
   
    m_RForceCounter = 0;
    m_TForceCounter = 0;

   m_UTLCoordCounter = 0;
   m_DTLCoordCounter = 0;
   m_DBRForceCounter = 0;
   m_UBRForceCounter = 0;

/*	if(IsProcessZero())
	{
    std::cout << "Debug check on all CNT neighbours for force calculation" << zEndl;
	
    for(long iz = 0; iz < m_LZ; iz++)
    {
        for(long iy = 0; iy < m_LY; iy++)
        {
            for(long ix = 0; ix < m_LX; ix++)
            {
                CCNTCell* pCell = m_rvCNTCells.at(GetCellIndexFromCoords(ix,iy,iz));
				
				long xIndex = pCell->GetBLXIndex();
				long yIndex = pCell->GetBLYIndex();
				long zIndex = pCell->GetBLZIndex();
				
				
				for(short int in=0; in<13; in++)
				{
				    CCNTCell* pNNCell = pCell->GetIntNNCell(in);
										std::cout << "Cell " << pCell->GetId() << " with indices " << ix << " " << iy << " "<< iz << " and BLX,BLY,BLZ indices " << xIndex << " " << yIndex << " "<< zIndex << " has " << in << " NN cell " << pNNCell->GetId() << " with indices " << pNNCell->GetBLXIndex() << " " <<  pNNCell->GetBLYIndex() << " " << pNNCell->GetBLZIndex() << zEndl;
				}
            }
        }
    }
	}
*/	
}

// Copy constructor

mpsPlanarZSimBox::mpsPlanarZSimBox(const mpsPlanarZSimBox& oldSimBox) : mpsSimBox(oldSimBox),
                                   m_Msg1L(oldSimBox.m_Msg1L),
                                   m_Msg1B(oldSimBox.m_Msg1B),
                                   m_Msg1TL(oldSimBox.m_Msg1TL),
                                   m_Msg1BL(oldSimBox.m_Msg1BL),
                                   m_Msg1DB(oldSimBox.m_Msg1DB),
                                   m_Msg1UL(oldSimBox.m_Msg1UL),
                                   m_Msg1UB(oldSimBox.m_Msg1UB),
                                   m_Msg1DL(oldSimBox.m_Msg1DL),
                                   m_Msg1DBL(oldSimBox.m_Msg1DBL),
                                   m_Msg1UTL(oldSimBox.m_Msg1UTL),
                                   m_Msg1UBL(oldSimBox.m_Msg1UBL),
                                   m_Msg1DTL(oldSimBox.m_Msg1DTL),
                                   m_Msg2R(oldSimBox.m_Msg2R),
                                   m_Msg2T(oldSimBox.m_Msg2T),
                                   m_Msg2BR(oldSimBox.m_Msg2BR),
                                   m_Msg2TR(oldSimBox.m_Msg2TR),
                                   m_Msg2UT(oldSimBox.m_Msg2UT),
                                   m_Msg2DR(oldSimBox.m_Msg2DR),
                                   m_Msg2DT(oldSimBox.m_Msg2DT),
                                   m_Msg2UR(oldSimBox.m_Msg2UR),
                                   m_Msg2UTR(oldSimBox.m_Msg2UTR),
                                   m_Msg2DBR(oldSimBox.m_Msg2DBR),
                                   m_Msg2DTR(oldSimBox.m_Msg2DTR),
                                   m_Msg2UBR(oldSimBox.m_Msg2UBR),
                                   m_Msg3R(oldSimBox.m_Msg3R),
                                   m_Msg3T(oldSimBox.m_Msg3T),
                                   m_Msg4R(oldSimBox.m_Msg4R),
                                   m_Msg4T(oldSimBox.m_Msg4T),
                                   m_Msg3TR(oldSimBox.m_Msg3TR),
                                   m_Msg3TL(oldSimBox.m_Msg3TL),
                                   m_Msg4TR(oldSimBox.m_Msg4TR),
                                   m_Msg4TL(oldSimBox.m_Msg4TL)
{
}


mpsPlanarZSimBox::~mpsPlanarZSimBox()
{
    // Write out the statistics on beads crossing between processors.
	// Note that there are 4*GetWorld() counters for a planar geometry.
	
	    double sum  = static_cast<double>(m_vBeadCrossing.at(4*GetRank()));
		double sum2 = static_cast<double>(m_vBeadCrossing2.at(4*GetRank()));
		
	    double average  = sum / static_cast<double>(m_BeadCrossingCounter);
		double variance = (sum2 / static_cast<double>(m_BeadCrossingCounter)) - average*average;
		
	    std::cout << "Processor " << GetRank() << " recorded a RL bead transfer average/variance of " << average << " " << variance << " after " << m_BeadCrossingCounter << " calls" << zEndl;

	    sum  = static_cast<double>(m_vBeadCrossing.at(4*GetRank()+1));
	    sum2 = static_cast<double>(m_vBeadCrossing2.at(4*GetRank()+1));
		
	    average  = sum / static_cast<double>(m_BeadCrossingCounter);
		variance = (sum2 / static_cast<double>(m_BeadCrossingCounter)) - average*average;
		
	    std::cout << "Processor " << GetRank() << " recorded a TB bead transfer average/variance of " << average << " " << variance << " after " << m_BeadCrossingCounter << " calls" << zEndl;

	    sum  = static_cast<double>(m_vBeadCrossing.at(4*GetRank()+2));
	    sum2 = static_cast<double>(m_vBeadCrossing2.at(4*GetRank()+2));
		
	    average  = sum / static_cast<double>(m_BeadCrossingCounter);
		variance = (sum2 / static_cast<double>(m_BeadCrossingCounter)) - average*average;
		
	    std::cout << "Processor " << GetRank() << " recorded a TR-BL bead transfer average/variance of " << average << " " << variance << " after " << m_BeadCrossingCounter << " calls" << zEndl;

	    sum  = static_cast<double>(m_vBeadCrossing.at(4*GetRank()+3));
	    sum2 = static_cast<double>(m_vBeadCrossing2.at(4*GetRank()+3));
		
	    average  = sum / static_cast<double>(m_BeadCrossingCounter);
		variance = (sum2 / static_cast<double>(m_BeadCrossingCounter)) - average*average;
		
	    std::cout << "Processor " << GetRank() << " recorded a TL-BR bead transfer average/variance of " << average << " " << variance << " after " << m_BeadCrossingCounter << " calls" << zEndl;
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
// This is a planar Space so we connect the four adjacent processors in the
// Z plane of the Space.

void mpsPlanarZSimBox::PartitionSpace()
{
    // Copy the Bulk CNT cells into a second array to make iterating over internal cells
    // efficient. This includes all cells except those on the borders of the Space.
    // For this class, the Borders are just the layer of CNT cells at the Z ends
    // of the processor's Space.

    m_vBulkCNTCells.clear();
	
    for(long iz = 1; iz < m_LZ-1; iz++)
    {
        for(long iy = 1; iy < m_LY-1; iy++)
        {
            for(long ix = 1; ix < m_LX-1; ix++)
            {
                m_vBulkCNTCells.push_back(m_rvCNTCells.at(GetCellIndexFromCoords(ix,iy,iz)));
            }
        }
    }

    // Create the external CNT cells that wrap the Face/Edge/Corner regions between
    // processors. These are called "external" Border regions. For this case, we need 
    // four layers of external cells adjacent to R,L,T,B faces of the processor's Space. 
    // Note that this means the R Face external cells actually wrap the L Face cells,
    // and vice versa, and similarly for the T,B Faces.
    //
    // Each Border region has the following external connectedions:
    //
    // Linear:  Face   - 1 external side
    //          Edge   - 1 external side, 1 external diagonal,  1 internal side, 
    //          Corner - 1 external side, 3 external diagonals, 2 internal sides, 1 internal diagonal
    //
    // Planar:  Face   - 1 external side
    //          Edge   - 2 external sides, 1 external diagonal
    //          Corner - 2 external sides, 1 external diagonal, 1 internal side, 3 internal diagonals
    //
    // Cubic:   Face   - 1 external side
    //          Edge   - 2 external sides, 1 external diagonal
    //          Corner - 3 external sides, 3 external diagonals
    //
    // :
    // Now assign the Faces, Edges and Corners in that order.
    // Each Face instance receives the whole CNT cell array, the size of the Space, 
    // the dimensions of the Face region and its normal vector, and the pid of the 
    // adjacent processor. This allows each Face to send messages to the adjacent processor. 
    // A Face is either internal or external depending on the geometry of the simulation
    // Space. It stores a single flag showing its internal/external status. An Edge
    // derives its external status from its two Faces, and a Corner from its three Faces.
    // We also define the normals to Edges and Corners from those of their contained Faces.

    // The constructors also replace pointers to CNT cells 
    // in those Border regions that cross the PBCs in the directions of adjacent 
    // processors by pointers to newly-created CExternalCNTCells so 
    // that forces and bead movements of beads in these cells can be updated.

    m_pU = new mpsFace(this, false,  m_LX, m_LY, m_LZ, m_LX-2, m_LY-2, 1,  0,  0,  1, m_rvCNTCells);
    m_pD = new mpsFace(this, false,  m_LX, m_LY, m_LZ, m_LX-2, m_LY-2, 1,  0,  0, -1, m_rvCNTCells);
    m_pR = new mpsFace(this, true,   m_LX, m_LY, m_LZ, 1, m_LY-2, m_LZ-2,  1,  0,  0, m_rvCNTCells);
    m_pL = new mpsFace(this, true,   m_LX, m_LY, m_LZ, 1, m_LY-2, m_LZ-2, -1,  0,  0, m_rvCNTCells);
    m_pT = new mpsFace(this, true,   m_LX, m_LY, m_LZ, m_LX-2, 1, m_LZ-2,  0,  1,  0, m_rvCNTCells);
    m_pB = new mpsFace(this, true,   m_LX, m_LY, m_LZ, m_LX-2, 1, m_LZ-2,  0, -1,  0, m_rvCNTCells);
  
    m_pUR  = new mpsEdge(this, m_pU, m_pR, m_LX, m_LY, m_LZ, 1, m_LY-2, 1,  1, -1,  1, m_rvCNTCells);
    m_pUL  = new mpsEdge(this, m_pU, m_pL, m_LX, m_LY, m_LZ, 1, m_LY-2, 1,  0, -1,  1, m_rvCNTCells);
    m_pUT  = new mpsEdge(this, m_pU, m_pT, m_LX, m_LY, m_LZ, m_LX-2, 1, 1, -1,  1,  1, m_rvCNTCells);
    m_pUB  = new mpsEdge(this, m_pU, m_pB, m_LX, m_LY, m_LZ, m_LX-2, 1, 1, -1,  0,  1, m_rvCNTCells);

    m_pDR  = new mpsEdge(this, m_pD, m_pR, m_LX, m_LY, m_LZ, 1, m_LY-2, 1,  1, -1,  0, m_rvCNTCells);
    m_pDL  = new mpsEdge(this, m_pD, m_pL, m_LX, m_LY, m_LZ, 1, m_LY-2, 1,  0, -1,  0, m_rvCNTCells);
    m_pDT  = new mpsEdge(this, m_pD, m_pT, m_LX, m_LY, m_LZ, m_LX-2, 1, 1, -1,  1,  0, m_rvCNTCells);
    m_pDB  = new mpsEdge(this, m_pD, m_pB, m_LX, m_LY, m_LZ, m_LX-2, 1, 1, -1,  0,  0, m_rvCNTCells);

    m_pTR = new mpsEdge(this, m_pT, m_pR, m_LX, m_LY, m_LZ, 1, 1, m_LZ-2,  1,  1, -1, m_rvCNTCells);
    m_pBR = new mpsEdge(this, m_pB, m_pR, m_LX, m_LY, m_LZ, 1, 1, m_LZ-2,  1,  0, -1, m_rvCNTCells);
    m_pTL = new mpsEdge(this, m_pT, m_pL, m_LX, m_LY, m_LZ, 1, 1, m_LZ-2,  0,  1, -1, m_rvCNTCells);
    m_pBL = new mpsEdge(this, m_pB, m_pL, m_LX, m_LY, m_LZ, 1, 1, m_LZ-2,  0,  0, -1, m_rvCNTCells);

    m_pUTR = new mpsCorner(this, m_pU, m_pT, m_pR, m_LX-1, m_LY-1, m_LZ-1, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(m_LX-1,m_LY-1,m_LZ-1)));
    m_pUTL = new mpsCorner(this, m_pU, m_pT, m_pL, 0, m_LY-1, m_LZ-1, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(0,m_LY-1,m_LZ-1)));
    m_pUBR = new mpsCorner(this, m_pU, m_pB, m_pR, m_LX-1, 0, m_LZ-1, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(m_LX-1,0,m_LZ-1)));
    m_pUBL = new mpsCorner(this, m_pU, m_pB, m_pL, 0, 0, m_LZ-1, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(0,0,m_LZ-1)));

    m_pDTR = new mpsCorner(this, m_pD, m_pT, m_pR, m_LX-1, m_LY-1, 0, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(m_LX-1,m_LY-1,0)));
    m_pDTL = new mpsCorner(this, m_pD, m_pT, m_pL, 0, m_LY-1, 0, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(0,m_LY-1,0)));
    m_pDBR = new mpsCorner(this, m_pD, m_pB, m_pR, m_LX-1, 0, 0, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(m_LX-1,0,0)));
    m_pDBL = new mpsCorner(this, m_pD, m_pB, m_pL, 0, 0, 0, m_LX, m_LY, m_LZ, m_rvCNTCells.at(GetCellIndexFromCoords(0,0,0)));    

}


// ****************************************
// PVF to update the positions of beads in the Border regions. Depending on the
// simulation Space geometry, some of the Borders do not need to communicate with
// other processors while some do.  For both types of region, we call the serial
// CCNTCell function to move the beads into their adjacent cells. FEC cells that border
// another processor collect beads in the external cell wrapper instances, and from
// there we collect the beads and package them into messages for sending to the 
// adjacent processors.
//
// The planar Z SimBox case requires the X,Y Face/Edge/Corners to communicate with
// adjacent processors, but the U,D Faces do not. Note that Corners are external 
// for all simulation Space geometries. There is an added complication now because
// each Edge (and its two Corners) can send beads to one of three adjacent processors.
// We have to be careful when retrieving beads from an Edge's external cells because the
// retrieval function clears the Edge's container after returning the beads, and we need 
// to send the beads to all three processors depending on their coordinates.

void mpsPlanarZSimBox::UpdateBorderPos()
{
#if SimMPS == SimulationEnabled

//    std::cout << "****************************************" << zEndl;
//    std::cout  << "Processor " << GetRank() << " has entered mpsPlanarZSimBox's UpdateBorderPos with neighbours (R,L,T,B) " << RFacePid << " " << LFacePid << " " << TFacePid << " " << BFacePid << zEndl;
//    std::cout  << "and diagonal processors (TR,BR,TL,BL) " << TREdgePid << " " << BREdgePid << " " << TLEdgePid << " " << BLEdgePid << zEndl;

/*	if(IsProcessZero())
	{
	    CNTCellIterator iterCell;
        CNTCellVector  lRFaceCells    = m_pR->GetFECCells();
        CNTCellVector  lRFaceExtCells = m_pR->GetExternalFECCells();
	
		std::cout << "R direction has index " << MapCellToNNIndex(1,0,0) << zEndl;
			
	    for(iterCell=lRFaceCells.begin(); iterCell!=lRFaceCells.end(); iterCell++)
        {
		    std::cout << "R Face has cells " << (*iterCell)->GetId() << zEndl;
			
			// Now write out all the neighbours to this cell in the R direction: this requires looping over all 
			// the nearest neighbours and selecting those in the R direction.
			
			for(short int j=0; j<27; j++)
			{
			    long nindex = (*iterCell)->GetNNCell(j)->GetId();
				std::cout << "Cell " << (*iterCell)->GetId() << " has " << j << " nearest neighbour " << nindex << zEndl;
			}
			
        
        }
	    for(iterCell=lRFaceExtCells.begin(); iterCell!=lRFaceExtCells.end(); iterCell++)
        {
		    std::cout << "R Face has external cells " << (*iterCell)->GetId() << zEndl;
        
        }


        CNTCellVector  lTREdgeCells    = m_pTR->GetECCells();
        CNTCellVector  lTREdgeExtCells = m_pTR->GetExternalECCells();
		
	    for(iterCell=lTREdgeCells.begin(); iterCell!=lTREdgeCells.end(); iterCell++)
        {
		    std::cout << "TR Edge has cells " << (*iterCell)->GetId() << zEndl;
		}
		
	    for(iterCell=lTREdgeExtCells.begin(); iterCell!=lTREdgeExtCells.end(); iterCell++)
        {
		    std::cout << "TR Edge has external cells " << (*iterCell)->GetId() << zEndl;
		}
		
	}  */
	
	if(!CheckBeadIds())
	{
	    std::cout << "Proc " << GetRank() << "failed bead id validation entering UpdateBorderPos " << zEndl;
	}
		
    m_pU->UpdatePos();
    m_pD->UpdatePos();
    m_pR->UpdatePos();
    m_pL->UpdatePos();
    m_pT->UpdatePos();
    m_pB->UpdatePos();
	
	m_pTR->UpdatePos();
	m_pTL->UpdatePos();
	m_pBR->UpdatePos();
	m_pBL->UpdatePos();
	
	m_pUR->UpdatePos();
	m_pUL->UpdatePos();
	m_pUT->UpdatePos();
	m_pUB->UpdatePos();
	
	m_pDR->UpdatePos();
	m_pDL->UpdatePos();
	m_pDT->UpdatePos();
	m_pDB->UpdatePos();
	
	m_pUTR->UpdatePos();
	m_pUTL->UpdatePos();
	m_pUBR->UpdatePos();
	m_pUBL->UpdatePos();
	
	m_pDTR->UpdatePos();
	m_pDTL->UpdatePos();
	m_pDBR->UpdatePos();
	m_pDBL->UpdatePos();
	

	if(!CheckBeadIds())
	{
	    std::cout << "Proc " << GetRank() << "failed bead id validation after updating R, L< T, B Faces " << zEndl;
	}
	    
    // Now collect the bead instances to send to R, L face procs
		
	BeadList  lRBeads;
	lRBeads.clear();
	lRBeads = m_pR->GetExternalFECBeads();
	
	BeadList  lLBeads;
	lLBeads.clear();
	lLBeads = m_pL->GetExternalFECBeads();
			    	
    m_Msg3R.SetMessageData(lRBeads);
	m_Msg4R.SetMessageData(lLBeads);
	
    MPI_Request  sendRequest3R;
    MPI_Request  receiveRequest3R;

    if(m_Msg3R.Validate())
    {
        m_Msg3R.ISend(RFacePid, &sendRequest3R);
        m_Msg3R.IReceive(LFacePid, &receiveRequest3R);
	    MoveOwningPolymersToExtended(RFacePid, lRBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3R failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4R;
    MPI_Request  receiveRequest4R;

    if(m_Msg4R.Validate())
    {
        m_Msg4R.ISend(LFacePid, &sendRequest4R);
		m_Msg4R.IReceive(RFacePid, &receiveRequest4R);
	    MoveOwningPolymersToExtended(LFacePid, lLBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg4R failed to validate its data" << zEndl;
	}
    
	// and for the T, B face procs
	
	BeadList  lTBeads;
	lTBeads.clear();
	lTBeads = m_pT->GetExternalFECBeads();
	
	BeadList  lBBeads;
	lBBeads.clear();
	lBBeads = m_pB->GetExternalFECBeads();
	
    m_Msg3T.SetMessageData(lTBeads);
    m_Msg4T.SetMessageData(lBBeads);
	
    // and start the non-blocking send. Note that the above function retrieves 
    // beads that are in the owning processor's Space not a neighbouring processo.
    // The latter beads are returned by GetDFaceNeighbourBeads().
    // Simultaneously, we use the same message to receive beads from the U Face
    // processor that has itself sent the message to its D Face processor, which is
    // the current processor.

    MPI_Request  sendRequest3T;
    MPI_Request  receiveRequest3T;

    if(m_Msg3T.Validate())
    {
        m_Msg3T.ISend(TFacePid, &sendRequest3T);
        m_Msg3T.IReceive(BFacePid, &receiveRequest3T);
	    MoveOwningPolymersToExtended(TFacePid, lTBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3T failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4T;
    MPI_Request  receiveRequest4T;

    if(m_Msg4T.Validate())
    {
        m_Msg4T.ISend(BFacePid, &sendRequest4T);
		m_Msg4T.IReceive(TFacePid, &receiveRequest4T);
	    MoveOwningPolymersToExtended(BFacePid, lBBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg4T failed to validate its data" << zEndl;
	}

   // Now we have to deal with beads in the diagonal external cells adjacent to the Edge/Corner regions 
   // that may move to the diagonal neighbouring processor. Unlike the planar geometry, we cannot assume that the
   // processors above and below the owning processor's plane are the same. Hence, we have to send distinct messages to all
   // diagonal processors:  there are 12 of these: 
   //
   // 4 in-plane diagonals: TR, TL, BR, BL 
   
	BeadList  lTRBeads;
	lTRBeads.clear();
	lTRBeads = m_pTR->GetExternalECBeads();
	
	BeadList  lBLBeads;
	lBLBeads.clear();
	lBLBeads = m_pBL->GetExternalECBeads();
			
    m_Msg3TR.SetMessageData(lTRBeads);
    m_Msg4TR.SetMessageData(lBLBeads);
	
    MPI_Request  sendRequest3TR;
    MPI_Request  receiveRequest3TR;

    if(m_Msg3TR.Validate())
    {
        m_Msg3TR.ISend(TREdgePid, &sendRequest3TR);
        m_Msg3TR.IReceive(BLEdgePid, &receiveRequest3TR);
	    MoveOwningPolymersToExtended(TREdgePid, lTRBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3TR failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4TR;
    MPI_Request  receiveRequest4TR;

    if(m_Msg4TR.Validate())
    {
        m_Msg4TR.ISend(BLEdgePid, &sendRequest4TR);
		m_Msg4TR.IReceive(TREdgePid, &receiveRequest4TR);
	    MoveOwningPolymersToExtended(BLEdgePid, lBLBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg4TR failed to validate its data" << zEndl;
	}
   
   // Now the opposite diagonal edge
   
	BeadList  lTLBeads;
	lTLBeads.clear();
	lTLBeads = m_pTL->GetExternalECBeads();
	
	BeadList  lBRBeads;
	lBRBeads.clear();
	lBRBeads = m_pBR->GetExternalECBeads();
		   
	m_Msg3TL.SetMessageData(lTLBeads);
    m_Msg4TL.SetMessageData(lBRBeads);
	
    // and start the non-blocking send. Note that the above function retrieves 
    // beads that are in the owning processor's Space not a neighbouring processor.
    // The latter beads are returned by GetDFaceNeighbourBeads().
    // Simultaneously, we use the same message to receive beads from the U Face
    // processor that has itself sent the message to its D Face processor, which is
    // the current processor.

    MPI_Request  sendRequest3TL;
    MPI_Request  receiveRequest3TL;

    if(m_Msg3TL.Validate())
    {
        m_Msg3TL.ISend(TLEdgePid, &sendRequest3TL);
        m_Msg3TL.IReceive(BREdgePid, &receiveRequest3TL);
	    MoveOwningPolymersToExtended(TLEdgePid, lTLBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3TL failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4TL;
    MPI_Request  receiveRequest4TL;

    if(m_Msg4TL.Validate())
    {
        m_Msg4TL.ISend(BREdgePid, &sendRequest4TL);
		m_Msg4TL.IReceive(TLEdgePid, &receiveRequest4TL);
	    MoveOwningPolymersToExtended(BREdgePid, lBRBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " m_Msg4TL failed to validate its data" << zEndl;
	}
	
    // 8 U,D-direction diagonals: UT, UB, UR, UL, DT, DB, DR, DL
	
	// UT
	BeadList  lUTBeads;
	lUTBeads.clear();
	lUTBeads = m_pUT->GetExternalECBeads();
	
	BeadList  lDBBeads;
	lDBBeads.clear();
	lDBBeads = m_pDB->GetExternalECBeads();
		   
	m_Msg3UT.SetMessageData(lUTBeads);
    m_Msg4UT.SetMessageData(lDBBeads);
	
    MPI_Request  sendRequest3UT;
    MPI_Request  receiveRequest3UT;

    if(m_Msg3UT.Validate())
    {
        m_Msg3UT.ISend(UTEdgePid, &sendRequest3UT);
        m_Msg3UT.IReceive(DBEdgePid, &receiveRequest3UT);
	    MoveOwningPolymersToExtended(UTEdgePid, lUTBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3UT failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4UT;
    MPI_Request  receiveRequest4UT;

    if(m_Msg4UT.Validate())
    {
        m_Msg4UT.ISend(DBEdgePid, &sendRequest4UT);
		m_Msg4UT.IReceive(UTEdgePid, &receiveRequest4UT);
	    MoveOwningPolymersToExtended(DBEdgePid, lDBBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " m_Msg4UT failed to validate its data" << zEndl;
	}
	
	// UB
	BeadList  lUBBeads;
	lUBBeads.clear();
	lUBBeads = m_pUB->GetExternalECBeads();
	
	BeadList  lDTBeads;
	lDTBeads.clear();
	lDTBeads = m_pDT->GetExternalECBeads();
		   
	m_Msg3UB.SetMessageData(lUBBeads);
    m_Msg4UB.SetMessageData(lDTBeads);
	
    MPI_Request  sendRequest3UB;
    MPI_Request  receiveRequest3UB;

    if(m_Msg3UB.Validate())
    {
        m_Msg3UB.ISend(UBEdgePid, &sendRequest3UB);
        m_Msg3UB.IReceive(DTEdgePid, &receiveRequest3UB);
	    MoveOwningPolymersToExtended(UBEdgePid, lUBBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3UB failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4UB;
    MPI_Request  receiveRequest4UB;

    if(m_Msg4UB.Validate())
    {
        m_Msg4UB.ISend(DTEdgePid, &sendRequest4UB);
		m_Msg4UB.IReceive(UBEdgePid, &receiveRequest4UB);
	    MoveOwningPolymersToExtended(DTEdgePid, lDTBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " m_Msg4UB failed to validate its data" << zEndl;
	}
	
	// UR
	
	BeadList  lURBeads;
	lURBeads.clear();
	lURBeads = m_pUR->GetExternalECBeads();
	
	BeadList  lDLBeads;
	lDLBeads.clear();
	lDLBeads = m_pDL->GetExternalECBeads();
		   
	m_Msg3UR.SetMessageData(lURBeads);
    m_Msg4UR.SetMessageData(lDLBeads);
	
    MPI_Request  sendRequest3UR;
    MPI_Request  receiveRequest3UR;

    if(m_Msg3UR.Validate())
    {
        m_Msg3UR.ISend(UREdgePid, &sendRequest3UR);
        m_Msg3UR.IReceive(DLEdgePid, &receiveRequest3UR);
	    MoveOwningPolymersToExtended(UREdgePid, lURBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3UR failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4UR;
    MPI_Request  receiveRequest4UR;

    if(m_Msg4UR.Validate())
    {
        m_Msg4UR.ISend(DLEdgePid, &sendRequest4UR);
		m_Msg4UR.IReceive(UREdgePid, &receiveRequest4UR);
	    MoveOwningPolymersToExtended(DLEdgePid, lDLBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " m_Msg4UR failed to validate its data" << zEndl;
	}
	
	
	// UL
	
	BeadList  lULBeads;
	lULBeads.clear();
	lULBeads = m_pUL->GetExternalECBeads();
	
	BeadList  lDRBeads;
	lDRBeads.clear();
	lDRBeads = m_pDR->GetExternalECBeads();
		   
	m_Msg3UL.SetMessageData(lULBeads);
    m_Msg4UL.SetMessageData(lDRBeads);
	
    MPI_Request  sendRequest3UL;
    MPI_Request  receiveRequest3UL;

    if(m_Msg3UL.Validate())
    {
        m_Msg3UL.ISend(ULEdgePid, &sendRequest3UL);
        m_Msg3UL.IReceive(DREdgePid, &receiveRequest3UL);
	    MoveOwningPolymersToExtended(ULEdgePid, lULBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3UL failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4UL;
    MPI_Request  receiveRequest4UL;

    if(m_Msg4UL.Validate())
    {
        m_Msg4UL.ISend(DREdgePid, &sendRequest4UL);
		m_Msg4UL.IReceive(ULEdgePid, &receiveRequest4UL);
	    MoveOwningPolymersToExtended(DREdgePid, lDRBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " m_Msg4UL failed to validate its data" << zEndl;
	}
	
    // Now for the corner processors: these are not the same as the edge processors in 3d
	
    // UTR
	
	BeadList  lUTRBeads;
	lUTRBeads.clear();
	lUTRBeads = m_pUTR->GetExternalBeads();
	
	BeadList  lDBLBeads;
	lDBLBeads.clear();
	lDBLBeads = m_pDBL->GetExternalBeads();
		
    m_Msg3UTR.SetMessageData(lUTRBeads);
    m_Msg4UTR.SetMessageData(lDBLBeads);
	
    MPI_Request  sendRequest3UTR;
    MPI_Request  receiveRequest3UTR;

    if(m_Msg3UTR.Validate())
    {
        m_Msg3UTR.ISend(UTRCornerPid, &sendRequest3UTR);
        m_Msg3UTR.IReceive(DBLCornerPid, &receiveRequest3UTR);
	    MoveOwningPolymersToExtended(UTRCornerPid, lUTRBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3UTR failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4UTR;
    MPI_Request  receiveRequest4UTR;

    if(m_Msg4UTR.Validate())
    {
        m_Msg4UTR.ISend(DBLCornerPid, &sendRequest4UTR);
		m_Msg4UTR.IReceive(UTRCornerPid, &receiveRequest4UTR);
	    MoveOwningPolymersToExtended(DBLCornerPid, lDBLBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg4UTR failed to validate its data" << zEndl;
	}
			
    // UTL
	
	BeadList  lUTLBeads;
	lUTLBeads.clear();
	lUTLBeads = m_pUTL->GetExternalBeads();
	
	BeadList  lDBRBeads;
	lDBRBeads.clear();
	lDBRBeads = m_pDBR->GetExternalBeads();
		
    m_Msg3UTL.SetMessageData(lUTLBeads);
    m_Msg4UTL.SetMessageData(lDBRBeads);
	
    MPI_Request  sendRequest3UTL;
    MPI_Request  receiveRequest3UTL;

    if(m_Msg3UTL.Validate())
    {
        m_Msg3UTL.ISend(UTLCornerPid, &sendRequest3UTL);
        m_Msg3UTL.IReceive(DBRCornerPid, &receiveRequest3UTL);
	    MoveOwningPolymersToExtended(UTLCornerPid, lUTLBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3UTL failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4UTL;
    MPI_Request  receiveRequest4UTL;

    if(m_Msg4UTL.Validate())
    {
        m_Msg4UTL.ISend(DBRCornerPid, &sendRequest4UTL);
		m_Msg4UTL.IReceive(UTLCornerPid, &receiveRequest4UTL);
	    MoveOwningPolymersToExtended(DBRCornerPid, lDBRBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg4UTL failed to validate its data" << zEndl;
	}
			
    // UBR
	
	BeadList  lUBRBeads;
	lUBRBeads.clear();
	lUBRBeads = m_pUBR->GetExternalBeads();
	
	BeadList  lDTLBeads;
	lDTLBeads.clear();
	lDTLBeads = m_pDTL->GetExternalBeads();
		
    m_Msg3UBR.SetMessageData(lUBRBeads);
    m_Msg4UBR.SetMessageData(lDTLBeads);
	
    MPI_Request  sendRequest3UBR;
    MPI_Request  receiveRequest3UBR;

    if(m_Msg3UBR.Validate())
    {
        m_Msg3UBR.ISend(UBRCornerPid, &sendRequest3UBR);
        m_Msg3UBR.IReceive(DTLCornerPid, &receiveRequest3UBR);
	    MoveOwningPolymersToExtended(UBRCornerPid, lUBRBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3UBR failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4UBR;
    MPI_Request  receiveRequest4UBR;

    if(m_Msg4UBR.Validate())
    {
        m_Msg4UBR.ISend(DTLCornerPid, &sendRequest4UBR);
		m_Msg4UBR.IReceive(UBRCornerPid, &receiveRequest4UBR);
	    MoveOwningPolymersToExtended(DTLCornerPid, lDTLBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg4UBR failed to validate its data" << zEndl;
	}
			
    // UBL
	
	BeadList  lUBLBeads;
	lUBLBeads.clear();
	lUBLBeads = m_pUBL->GetExternalBeads();
	
	BeadList  lDTRBeads;
	lDTRBeads.clear();
	lDTRBeads = m_pDTR->GetExternalBeads();
		
    m_Msg3UBL.SetMessageData(lUBLBeads);
    m_Msg4UBL.SetMessageData(lDTRBeads);
	
    MPI_Request  sendRequest3UBL;
    MPI_Request  receiveRequest3UBL;

    if(m_Msg3UBL.Validate())
    {
        m_Msg3UBL.ISend(UBLCornerPid, &sendRequest3UBL);
        m_Msg3UBL.IReceive(DTRCornerPid, &receiveRequest3UBL);
	    MoveOwningPolymersToExtended(UBLCornerPid, lUBLBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3UBL failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4UBL;
    MPI_Request  receiveRequest4UBL;

    if(m_Msg4UBL.Validate())
    {
        m_Msg4UBL.ISend(DTRCornerPid, &sendRequest4UBL);
		m_Msg4UBL.IReceive(UBLCornerPid, &receiveRequest4UBL);
	    MoveOwningPolymersToExtended(DTRCornerPid, lDTRBeads);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg4UBL failed to validate its data" << zEndl;
	}
	
    // Now remove the old bead pointers from the external cells belonging to the Face/Edge/Corners cells.
      
	m_pU->RemoveBeadsFromExternalFECCells();
	m_pD->RemoveBeadsFromExternalFECCells();
	m_pR->RemoveBeadsFromExternalFECCells();
	m_pL->RemoveBeadsFromExternalFECCells();
	m_pT->RemoveBeadsFromExternalFECCells();
	m_pB->RemoveBeadsFromExternalFECCells();
	 
    m_pUR->RemoveBeadsFromExternalECCells();
    m_pUL->RemoveBeadsFromExternalECCells();
    m_pUT->RemoveBeadsFromExternalECCells();
    m_pUB->RemoveBeadsFromExternalECCells();
    m_pDR->RemoveBeadsFromExternalECCells();
    m_pDL->RemoveBeadsFromExternalECCells();
    m_pDT->RemoveBeadsFromExternalECCells();
    m_pDB->RemoveBeadsFromExternalECCells();
    m_pTR->RemoveBeadsFromExternalECCells();
    m_pTL->RemoveBeadsFromExternalECCells();
    m_pBR->RemoveBeadsFromExternalECCells();
    m_pBL->RemoveBeadsFromExternalECCells();
   
    m_pUTR->RemoveBeadsFromExternalCells();
    m_pUTL->RemoveBeadsFromExternalCells();
    m_pUBR->RemoveBeadsFromExternalCells();
    m_pUBL->RemoveBeadsFromExternalCells();
    m_pDTR->RemoveBeadsFromExternalCells();
    m_pDTL->RemoveBeadsFromExternalCells();
    m_pDBR->RemoveBeadsFromExternalCells();
    m_pDBL->RemoveBeadsFromExternalCells();
   
   

    // R, L Faces	
    m_Msg3R.Wait(&sendRequest3R);
    m_Msg3R.Wait(&receiveRequest3R);

	BeadList lLRecBeads = m_Msg3R.GetRecBeads();

    BeadListIterator iterBead;  // Iterator used several times below
	
	for(iterBead=lLRecBeads.begin(); iterBead!=lLRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		
		
		if( ix != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3R bead has invalid id " << ix << " " << iy << " " << iz << " " << (*iterBead)->GetId() << " " << (*iterBead)->GetXPos()  << " " << (*iterBead)->GetYPos()  << " " << (*iterBead)->GetZPos() << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}
/*	
    in = 0;
	vPolymerIdVector = m_Msg3R.GetPolymerIds();
	std::cout <<  "****************************************" << zEndl;
	std::cout <<" Proc " << GetRank() << " R Border has vector sizes " << lLRecBeads.size() << " " << vPolymerIdVector.size() << zEndl;
	for(iterBead2=lLRecBeads.begin(); iterBead2!=lLRecBeads.end(); iterBead2++)
	{
	    std::cout << "R border for proc " << GetRank() << " receiving from " << LFacePid << " bead/polymer pair " << (*iterBead2)->GetId() << " " << vPolymerIdVector.at(in) << zEndl;
	    ++in;
	} */
	
	MoveOwningPolymersToBulk(LFacePid, lLRecBeads, m_Msg3R.GetBeadLocations(), m_Msg3R.GetFirstBondIds(), m_Msg3R.GetFirstBondPairIds(), m_Msg3R.GetPolymerIds(), m_Msg3R.GetPolymerTypes());

    m_Msg4R.Wait(&sendRequest4R);
    m_Msg4R.Wait(&receiveRequest4R);

    // Now we retrieve the newly-filled bead instances from the message and
	// add them to the R Face's CNT cells

	BeadList lRRecBeads = m_Msg4R.GetRecBeads();

	for(iterBead=lRRecBeads.begin(); iterBead!=lRRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != m_LX-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4R bead has invalid id " << ix << " " << iy << " " << iz << " " << (*iterBead)->GetId() << " " << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}
/*
    in = 0;
	vPolymerIdVector = m_Msg4R.GetPolymerIds();
	std::cout <<  "****************************************" << zEndl;
	std::cout <<" Proc " << GetRank() << " L Border has vector sizes " << lRRecBeads.size() << " " << vPolymerIdVector.size() << zEndl;
	for(iterBead2=lRRecBeads.begin(); iterBead2!=lRRecBeads.end(); iterBead2++)
	{
	    std::cout << "L border for proc " << GetRank() << " receiving from " << RFacePid << " bead/polymer pair " << (*iterBead2)->GetId() << " " << vPolymerIdVector.at(in) << zEndl;
	    ++in;
	}  */
	
	MoveOwningPolymersToBulk(RFacePid, lRRecBeads, m_Msg4R.GetBeadLocations(), m_Msg4R.GetFirstBondIds(), m_Msg4R.GetFirstBondPairIds(), m_Msg4R.GetPolymerIds(), m_Msg4R.GetPolymerTypes());


    // T, B Faces
    m_Msg3T.Wait(&sendRequest3T);
    m_Msg3T.Wait(&receiveRequest3T);

	BeadList lBRecBeads = m_Msg3T.GetRecBeads();

	for(iterBead=lBRecBeads.begin(); iterBead!=lBRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(iy != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3T bead has invalid id " << ix << " " << iy << " " << iz << " " << (*iterBead)->GetId() << " " << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}
/*
    in = 0;
	vPolymerIdVector = m_Msg3T.GetPolymerIds();
	std::cout <<  "****************************************" << zEndl;
	std::cout <<" Proc " << GetRank() << " T Border has vector sizes " << lBRecBeads.size() << " " << vPolymerIdVector.size() << zEndl;
	for(iterBead2=lBRecBeads.begin(); iterBead2!=lBRecBeads.end(); iterBead2++)
	{
	    std::cout << "T border for proc " << GetRank() << " receiving from " << BFacePid << " bead/polymer pair " << (*iterBead2)->GetId() << " " << vPolymerIdVector.at(in) << zEndl;
	    ++in;
	}  */
	
	MoveOwningPolymersToBulk(BFacePid, lBRecBeads, m_Msg3T.GetBeadLocations(), m_Msg3T.GetFirstBondIds(), m_Msg3T.GetFirstBondPairIds(), m_Msg3T.GetPolymerIds(), m_Msg3T.GetPolymerTypes());

    m_Msg4T.Wait(&sendRequest4T);
    m_Msg4T.Wait(&receiveRequest4T);

    // Now we retrieve the newly-filled bead instances from the message and
	// add them to the T Face's CNT cells

	BeadList lTRecBeads = m_Msg4T.GetRecBeads();

	for(iterBead=lTRecBeads.begin(); iterBead!=lTRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(iy != m_LY-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4T bead has invalid id " << ix << " " << iy << " " << iz << " " << (*iterBead)->GetId() << " " << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

/*
    in = 0;
	vPolymerIdVector = m_Msg4T.GetPolymerIds();
	std::cout <<  "****************************************" << zEndl;
	std::cout <<" Proc " << GetRank() << " B Border has vector sizes " << lTRecBeads.size() << " " << vPolymerIdVector.size() << zEndl;
	for(iterBead2=lTRecBeads.begin(); iterBead2!=lTRecBeads.end(); iterBead2++)
	{
	    std::cout << "B border for proc " << GetRank() << " receiving from " << TFacePid << " bead/polymer pair " << (*iterBead2)->GetId() << " " << vPolymerIdVector.at(in) << zEndl;
	    ++in;
	}  */
	
	MoveOwningPolymersToBulk(TFacePid, lTRecBeads, m_Msg4T.GetBeadLocations(), m_Msg4T.GetFirstBondIds(), m_Msg4T.GetFirstBondPairIds(), m_Msg4T.GetPolymerIds(), m_Msg4T.GetPolymerTypes());

// Now the beads receive from the Edge diagonal processors

    // TR, BL Edges
    m_Msg3TR.Wait(&sendRequest3TR);
    m_Msg3TR.Wait(&receiveRequest3TR);

	BeadList l3TRRecBeads = m_Msg3TR.GetRecBeads();

	for(iterBead=l3TRRecBeads.begin(); iterBead!=l3TRRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != 0 || iy != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3TR bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(BLEdgePid, l3TRRecBeads, m_Msg3TR.GetBeadLocations(), m_Msg3TR.GetFirstBondIds(), m_Msg3TR.GetFirstBondPairIds(), m_Msg3TR.GetPolymerIds(), m_Msg3TR.GetPolymerTypes());

    m_Msg4TR.Wait(&sendRequest4TR);
    m_Msg4TR.Wait(&receiveRequest4TR);

	BeadList l4TRRecBeads = m_Msg4TR.GetRecBeads();

	for(iterBead=l4TRRecBeads.begin(); iterBead!=l4TRRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != m_LX-1 || iy != m_LY-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4TR bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(TREdgePid, l4TRRecBeads, m_Msg4TR.GetBeadLocations(), m_Msg4TR.GetFirstBondIds(), m_Msg4TR.GetFirstBondPairIds(), m_Msg4TR.GetPolymerIds(), m_Msg4TR.GetPolymerTypes());


    // TL, BR Edges

    m_Msg3TL.Wait(&sendRequest3TL);
    m_Msg3TL.Wait(&receiveRequest3TL);

	BeadList l3TLRecBeads = m_Msg3TL.GetRecBeads();

	for(iterBead=l3TLRecBeads.begin(); iterBead!=l3TLRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != m_LX-1 || iy != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3TL bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(BREdgePid, l3TLRecBeads, m_Msg3TL.GetBeadLocations(), m_Msg3TL.GetFirstBondIds(), m_Msg3TL.GetFirstBondPairIds(), m_Msg3TL.GetPolymerIds(), m_Msg3TL.GetPolymerTypes());

    m_Msg4TL.Wait(&sendRequest4TL);
    m_Msg4TL.Wait(&receiveRequest4TL);

	BeadList l4TLRecBeads = m_Msg4TL.GetRecBeads();

	for(iterBead=l4TLRecBeads.begin(); iterBead!=l4TLRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != 0 || iy != m_LY-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4TL bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(TLEdgePid, l4TLRecBeads, m_Msg4TL.GetBeadLocations(), m_Msg4TL.GetFirstBondIds(), m_Msg4TL.GetFirstBondPairIds(), m_Msg4TL.GetPolymerIds(), m_Msg4TL.GetPolymerTypes());


    // UT, DB Edges
    m_Msg3UT.Wait(&sendRequest3UT);
    m_Msg3UT.Wait(&receiveRequest3UT);

	BeadList l3UTRecBeads = m_Msg3UT.GetRecBeads();

	for(iterBead=l3UTRecBeads.begin(); iterBead!=l3UTRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(iy != 0 || iz != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3UT bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(DBEdgePid, l3UTRecBeads, m_Msg3UT.GetBeadLocations(), m_Msg3UT.GetFirstBondIds(), m_Msg3UT.GetFirstBondPairIds(), m_Msg3UT.GetPolymerIds(), m_Msg3UT.GetPolymerTypes());

    m_Msg4UT.Wait(&sendRequest4UT);
    m_Msg4UT.Wait(&receiveRequest4UT);

	BeadList l4UTRecBeads = m_Msg4UT.GetRecBeads();

	for(iterBead=l4UTRecBeads.begin(); iterBead!=l4UTRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(iy != m_LY-1 || iz != m_LZ-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4UT bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(UTEdgePid, l4UTRecBeads, m_Msg4UT.GetBeadLocations(), m_Msg4UT.GetFirstBondIds(), m_Msg4UT.GetFirstBondPairIds(), m_Msg4UT.GetPolymerIds(), m_Msg4UT.GetPolymerTypes());


    // UB, DT Edges
    m_Msg3UB.Wait(&sendRequest3UB);
    m_Msg3UB.Wait(&receiveRequest3UB);

	BeadList l3UBRecBeads = m_Msg3UB.GetRecBeads();

	for(iterBead=l3UBRecBeads.begin(); iterBead!=l3UBRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(iy != m_LY-1 || iz != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3UB bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(DTEdgePid, l3UBRecBeads, m_Msg3UB.GetBeadLocations(), m_Msg3UB.GetFirstBondIds(), m_Msg3UB.GetFirstBondPairIds(), m_Msg3UB.GetPolymerIds(), m_Msg3UB.GetPolymerTypes());

    m_Msg4UB.Wait(&sendRequest4UB);
    m_Msg4UB.Wait(&receiveRequest4UB);

	BeadList l4UBRecBeads = m_Msg4UB.GetRecBeads();

	for(iterBead=l4UBRecBeads.begin(); iterBead!=l4UBRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(iy != 0 || iz != m_LZ-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4UB bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(UBEdgePid, l4UBRecBeads, m_Msg4UB.GetBeadLocations(), m_Msg4UB.GetFirstBondIds(), m_Msg4UB.GetFirstBondPairIds(), m_Msg4UB.GetPolymerIds(), m_Msg4UB.GetPolymerTypes());
	
	
    // UR, DL Edges
    m_Msg3UR.Wait(&sendRequest3UR);
    m_Msg3UR.Wait(&receiveRequest3UR);

	BeadList l3URRecBeads = m_Msg3UR.GetRecBeads();

	for(iterBead=l3URRecBeads.begin(); iterBead!=l3URRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != 0 || iz != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3UR bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(DLEdgePid, l3URRecBeads, m_Msg3UR.GetBeadLocations(), m_Msg3UR.GetFirstBondIds(), m_Msg3UR.GetFirstBondPairIds(), m_Msg3UR.GetPolymerIds(), m_Msg3UR.GetPolymerTypes());

    m_Msg4UR.Wait(&sendRequest4UR);
    m_Msg4UR.Wait(&receiveRequest4UR);

	BeadList l4URRecBeads = m_Msg4UR.GetRecBeads();

	for(iterBead=l4URRecBeads.begin(); iterBead!=l4URRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != m_LX-1 || iz != m_LZ-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4UR bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(UREdgePid, l4URRecBeads, m_Msg4UR.GetBeadLocations(), m_Msg4UR.GetFirstBondIds(), m_Msg4UR.GetFirstBondPairIds(), m_Msg4UR.GetPolymerIds(), m_Msg4UR.GetPolymerTypes());



    // UL, DR Edges
    m_Msg3UL.Wait(&sendRequest3UL);
    m_Msg3UL.Wait(&receiveRequest3UL);

	BeadList l3ULRecBeads = m_Msg3UL.GetRecBeads();

	for(iterBead=l3ULRecBeads.begin(); iterBead!=l3ULRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != m_LX-1 || iz != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3UL bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(DREdgePid, l3ULRecBeads, m_Msg3UL.GetBeadLocations(), m_Msg3UL.GetFirstBondIds(), m_Msg3UL.GetFirstBondPairIds(), m_Msg3UL.GetPolymerIds(), m_Msg3UL.GetPolymerTypes());


    m_Msg4UL.Wait(&sendRequest4UL);
    m_Msg4UL.Wait(&receiveRequest4UL);

	BeadList l4ULRecBeads = m_Msg4UL.GetRecBeads();

	for(iterBead=l4ULRecBeads.begin(); iterBead!=l4ULRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != 0 || iz != m_LZ-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4UL bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(ULEdgePid, l4ULRecBeads, m_Msg4UL.GetBeadLocations(), m_Msg4UL.GetFirstBondIds(), m_Msg4UL.GetFirstBondPairIds(), m_Msg4UL.GetPolymerIds(), m_Msg4UL.GetPolymerTypes());
	
    // Now the corners

    // UTR, DBL
    m_Msg3UTR.Wait(&sendRequest3UTR);
    m_Msg3UTR.Wait(&receiveRequest3UTR);

	BeadList l3UTRRecBeads = m_Msg3UTR.GetRecBeads();

	for(iterBead=l3UTRRecBeads.begin(); iterBead!=l3UTRRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != 0 || iy != 0 ||  iz != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3UTR bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(DBLCornerPid, l3UTRRecBeads, m_Msg3UTR.GetBeadLocations(), m_Msg3UTR.GetFirstBondIds(), m_Msg3UTR.GetFirstBondPairIds(), m_Msg3UTR.GetPolymerIds(), m_Msg3UTR.GetPolymerTypes());


    m_Msg4UTR.Wait(&sendRequest4UTR);
    m_Msg4UTR.Wait(&receiveRequest4UTR);

	BeadList l4UTRRecBeads = m_Msg4UTR.GetRecBeads();

	for(iterBead=l4UTRRecBeads.begin(); iterBead!=l4UTRRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != m_LX-1 || iy != m_LY-1 || iz != m_LZ-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4UTR bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(UTRCornerPid, l4UTRRecBeads, m_Msg4UTR.GetBeadLocations(), m_Msg4UTR.GetFirstBondIds(), m_Msg4UTR.GetFirstBondPairIds(), m_Msg4UTR.GetPolymerIds(), m_Msg4UTR.GetPolymerTypes());


    // UTL, DBR
    m_Msg3UTL.Wait(&sendRequest3UTL);
    m_Msg3UTL.Wait(&receiveRequest3UTL);

	BeadList l3UTLRecBeads = m_Msg3UTL.GetRecBeads();

	for(iterBead=l3UTLRecBeads.begin(); iterBead!=l3UTLRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != m_LX-1 || iy != 0 ||  iz != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3UTL bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(DBRCornerPid, l3UTLRecBeads, m_Msg3UTL.GetBeadLocations(), m_Msg3UTL.GetFirstBondIds(), m_Msg3UTL.GetFirstBondPairIds(), m_Msg3UTL.GetPolymerIds(), m_Msg3UTL.GetPolymerTypes());


    m_Msg4UTL.Wait(&sendRequest4UTL);
    m_Msg4UTL.Wait(&receiveRequest4UTL);

	BeadList l4UTLRecBeads = m_Msg4UTL.GetRecBeads();

	for(iterBead=l4UTLRecBeads.begin(); iterBead!=l4UTLRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != 0 || iy != m_LY-1 || iz != m_LZ-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4UTL bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(UTLCornerPid, l4UTLRecBeads, m_Msg4UTL.GetBeadLocations(), m_Msg4UTL.GetFirstBondIds(), m_Msg4UTL.GetFirstBondPairIds(), m_Msg4UTL.GetPolymerIds(), m_Msg4UTL.GetPolymerTypes());


    // UBR, DTL
    m_Msg3UBR.Wait(&sendRequest3UBR);
    m_Msg3UBR.Wait(&receiveRequest3UBR);

	BeadList l3UBRRecBeads = m_Msg3UBR.GetRecBeads();

	for(iterBead=l3UBRRecBeads.begin(); iterBead!=l3UBRRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != 0 || iy != m_LY-1 ||  iz != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3UBR bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(DTLCornerPid, l3UBRRecBeads, m_Msg3UBR.GetBeadLocations(), m_Msg3UBR.GetFirstBondIds(), m_Msg3UBR.GetFirstBondPairIds(), m_Msg3UBR.GetPolymerIds(), m_Msg3UBR.GetPolymerTypes());


    m_Msg4UBR.Wait(&sendRequest4UBR);
    m_Msg4UBR.Wait(&receiveRequest4UBR);

	BeadList l4UBRRecBeads = m_Msg4UBR.GetRecBeads();

	for(iterBead=l4UBRRecBeads.begin(); iterBead!=l4UBRRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != m_LX-1 || iy != 0 || iz != m_LZ-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4UBR bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(UBRCornerPid, l4UBRRecBeads, m_Msg4UBR.GetBeadLocations(), m_Msg4UBR.GetFirstBondIds(), m_Msg4UBR.GetFirstBondPairIds(), m_Msg4UBR.GetPolymerIds(), m_Msg4UBR.GetPolymerTypes());


    // UBL, DTR
    m_Msg3UBL.Wait(&sendRequest3UBL);
    m_Msg3UBL.Wait(&receiveRequest3UBL);

	BeadList l3UBLRecBeads = m_Msg3UBL.GetRecBeads();

	for(iterBead=l3UBLRecBeads.begin(); iterBead!=l3UBLRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != m_LX-1 || iy != m_LY-1 ||  iz != 0 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg3UBL bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(DTRCornerPid, l3UBLRecBeads, m_Msg3UBL.GetBeadLocations(), m_Msg3UBL.GetFirstBondIds(), m_Msg3UBL.GetFirstBondPairIds(), m_Msg3UBL.GetPolymerIds(), m_Msg3UBL.GetPolymerTypes());


    m_Msg4UBL.Wait(&sendRequest4UBL);
    m_Msg4UBL.Wait(&receiveRequest4UBL);

	BeadList l4UBLRecBeads = m_Msg4UBL.GetRecBeads();

	for(iterBead=l4UBLRecBeads.begin(); iterBead!=l4UBLRecBeads.end(); iterBead++)
	{
		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		if(ix != 0 || iy != 0 || iz != m_LZ-1 || (*iterBead)->GetId() < 0)
		{
		    std::cout << "Proc " << GetRank() << " Msg4UBL bead has invalid id " << ix << " " << iy << " " << iz << zEndl;
		}

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	MoveOwningPolymersToBulk(UBLCornerPid, l4UBLRecBeads, m_Msg4UBL.GetBeadLocations(), m_Msg4UBL.GetFirstBondIds(), m_Msg4UBL.GetFirstBondPairIds(), m_Msg4UBL.GetPolymerIds(), m_Msg4UBL.GetPolymerTypes());


	
    // ****************************************
	// Collect statistics on the number of beads moving between processors in
	// all directions.
	// 0 - R - L
	// 1 - T - B
	// 2 - TR - BL
	// 3 - TL - BR

     RFaceDifference = (m_Msg3R.GetSentBeadTotal()-m_Msg3R.GetRecBeadTotal());
	 LFaceDifference = (m_Msg4R.GetSentBeadTotal()-m_Msg4R.GetRecBeadTotal());
	 TFaceDifference = (m_Msg3T.GetSentBeadTotal()-m_Msg3T.GetRecBeadTotal());
	 BFaceDifference = (m_Msg4T.GetSentBeadTotal()-m_Msg4T.GetRecBeadTotal());
	 TREdgeDifference = (m_Msg3TR.GetSentBeadTotal()-m_Msg3TR.GetRecBeadTotal());
	 BLEdgeDifference = (m_Msg4TR.GetSentBeadTotal()-m_Msg4TR.GetRecBeadTotal());
	 TLEdgeDifference = (m_Msg3TL.GetSentBeadTotal()-m_Msg3TL.GetRecBeadTotal());
	 BREdgeDifference = (m_Msg4TL.GetSentBeadTotal()-m_Msg4TL.GetRecBeadTotal());
	 
	netTotal += (RFaceDifference + LFaceDifference + TFaceDifference + BFaceDifference + TREdgeDifference + BLEdgeDifference + TLEdgeDifference + BREdgeDifference);
	
	m_BeadCrossingCounter++;
	const long valueRL  = m_Msg3R.GetSentBeadTotal()-m_Msg4R.GetSentBeadTotal();
	const long valueTB  = m_Msg3T.GetSentBeadTotal()-m_Msg4T.GetSentBeadTotal();
	const long valueTRBL = m_Msg3TR.GetSentBeadTotal()-m_Msg4TR.GetSentBeadTotal();
	const long valueTLBR = m_Msg3TL.GetSentBeadTotal()-m_Msg4TL.GetSentBeadTotal();

	m_vBeadCrossing.at(4*GetRank())    += valueRL;
	m_vBeadCrossing.at(4*GetRank()+1)  += valueTB;
	m_vBeadCrossing.at(4*GetRank()+2)  += valueTRBL;
	m_vBeadCrossing.at(4*GetRank()+3)  += valueTLBR;

	m_vBeadCrossing2.at(4*GetRank())   += valueRL*valueRL;
	m_vBeadCrossing2.at(4*GetRank()+1) += valueTB*valueTB;
	m_vBeadCrossing2.at(4*GetRank()+2) += valueTRBL*valueTRBL;
	m_vBeadCrossing2.at(4*GetRank()+3) += valueTLBR*valueTLBR;

	// **********
	// Calculate the number of beads in the whole simulation Space, and the 
	// World angular momentum and CM velocity for all processors
	// and write them to the log file.

	++localBeadTotalTimer;
	if(localBeadTotalTimer == 1000)
	{
	    localBeadTotalTimer = 0;
//	    LogWorldBeadTotal();
//	    LogWorldAngularMomentum();
//	    LogWorldCMVel();
		
	    // debug output on the number of beads moving across processor boundaries	
        std::cout << "Proc " << GetRank() << " has sent-received beads = " << netTotal << zEndl;
        std::cout << "Proc " << GetRank() << " has sent/received R, L beads = " << m_Msg3R.GetSentBeadTotal() << " " << m_Msg3R.GetRecBeadTotal()  << " " << m_Msg4R.GetSentBeadTotal() << " " << m_Msg4R.GetRecBeadTotal()   << zEndl;
        std::cout << "Proc " << GetRank() << " has sent/received T, B beads = " << m_Msg3T.GetSentBeadTotal() << " " << m_Msg3T.GetRecBeadTotal()  << " " << m_Msg4T.GetSentBeadTotal() << " " << m_Msg4T.GetRecBeadTotal()   << zEndl;
	
        std::cout << "Proc " << GetRank() << " has sent/received TR, BL beads = " << m_Msg3TR.GetSentBeadTotal() << " " << m_Msg3TR.GetRecBeadTotal()  << " " << m_Msg4TR.GetSentBeadTotal() << " " << m_Msg4TR.GetRecBeadTotal()   << zEndl;
        std::cout << "Proc " << GetRank() << " has sent/received TL, BR beads = " << m_Msg3TL.GetSentBeadTotal() << " " << m_Msg3TL.GetRecBeadTotal()  << " " << m_Msg4TL.GetSentBeadTotal() << " " << m_Msg4TL.GetRecBeadTotal()   << zEndl;
	}
	

#endif

}

// PVF to update the forces on beads within the CNT Cells in the Border regions 
// of the SimBox that may need to communicate with adjacent processors.
//
// For a planar geometry, each Face in the simulation Space's plane has to send
// bead forces/coordinates and bead to its adjacent processors. Additionally, 
// the four Edges whose normal is parallel to the simulation Space normal must
// send data to the processors connected along the diagonals of the owning processor.
// Each Face/Edge sends bead coordinates outwards to an adjacent processor and
// receives bead forces in return.
//
// The coding of the messages is as follows:
//
// m_Msg1X and m_Msg1XX for sending forces from Faces and Edges respectively,
// where X = R,L,T,B and XX = TR, TL, BR, BL)
//
// m_Msg2X, m_Msg2XX for receiving coordinates from Faces and Edges respectively.
//
// Note that the message tags are set in the constructor and left unchanged.

void mpsPlanarZSimBox::UpdateBorderForce()
{
#if SimMPS == SimulationEnabled
	
	/*
	CNTCellVector rTLCells   = m_pTL->GetECCells();
	CNTCellIterator icell;
	for(icell=rTLCells.begin(); icell!=rTLCells.end(); icell++)
	{
	    std::cout << "TL Edge cell: " << (*icell)->GetId() << " << has neighbours" << zEndl;
		
							for(long ii=0; ii<27; ii++)
							{
							    std::cout << (*icell)->GetNNCell(ii)->GetId() << " ";
							}	
						    std::cout << zEndl << "and force loop cells" << zEndl;
							
							for(long jj=0; jj<13; jj++)
							{
							    std::cout << (*icell)->GetIntNNCell(jj)->GetId() << " " ;
							}
							std::cout << zEndl;
		
	}
	std::cout << zEndl;
	
	*/


//    std::cout  << "Processor " << GetRank() << " has entered mpsPlanarZSimBox's UpdateBorderForce with neighbours (R,L,T,B) " << RFacePid << " " << LFacePid << " " << TFacePid << " " << BFacePid << zEndl;
//    std::cout  << "and diagonal processors (TR,BR,TL,BL) " << TREdgePid << " " << BREdgePid << " " << TLEdgePid << " " << BLEdgePid << zEndl;
		
/*	CNTCellVector rFaceCells = m_pR->GetExternalFECCells();
	CNTCellVector rTRCells   = m_pTR->GetExternalECCells();
	CNTCellVector rBRCells   = m_pBR->GetExternalECCells();
	CNTCellVector rUTRCell   = m_pUTR->GetExternalCNTCells();
	CNTCellVector rDTRCell   = m_pDTR->GetExternalCNTCells();
	CNTCellVector rUBRCell   = m_pUBR->GetExternalCNTCells();
	CNTCellVector rDBRCell   = m_pDBR->GetExternalCNTCells();
	
	
	
	if(IsProcessZero())
	{
	CNTCellIterator icell;
	
	for(icell=rFaceCells.begin(); icell!=rFaceCells.end(); icell++)
	{
	    std::cout << "R Face cells: " << (*icell)->GetId() << zEndl;
	}
	std::cout << zEndl;
	
	for(icell=rTRCells.begin(); icell!=rTRCells.end(); icell++)
	{
	    std::cout << "TR Edge cells: " << (*icell)->GetId() << zEndl;
	}
	std::cout << zEndl;
	
	for(icell=rBRCells.begin(); icell!=rBRCells.end(); icell++)
	{
	    std::cout << "BR Edge cells: " << (*icell)->GetId() << zEndl;
	}
	
	std::cout << zEndl;
	
	for(icell=rUTRCell.begin(); icell!=rUTRCell.end(); icell++)
	{
	    std::cout << "UTR Corner cells: " << (*icell)->GetId() << zEndl;
	}
	std::cout << zEndl;
	
	for(icell=rDTRCell.begin(); icell!=rDTRCell.end(); icell++)
	{
	    std::cout << "DTR Corner cells: " << (*icell)->GetId() << zEndl;
	}
	std::cout << zEndl;
	
	for(icell=rUBRCell.begin(); icell!=rUBRCell.end(); icell++)
	{
	    std::cout << "UBR Corner cells: " << (*icell)->GetId() << zEndl;
	}
	std::cout << zEndl;
	
	for(icell=rDBRCell.begin(); icell!=rDBRCell.end(); icell++)
	{
	    std::cout << "DBR Corner cells: " << (*icell)->GetId() << zEndl;
	}
	}
*/	
	// ****************************************
    // Message 1 - Non-blocking MPI_Isend to L,B Face processors and MPI_Irecv from
    //             R,T Face processors of pmSendPlanarBeadCoords containing 
    //             bead coordinates in our own L,B Faces and adjacent Border regions

    // First empty the external CNT cells before adding new bead coordinates to them. 
    // For this planar SimBox we need to clear the R, T Faces and their adjacent BR, TR Edges and Corners.
	
    m_pU->ClearExternalFECCells();
    m_pD->ClearExternalFECCells();
    m_pT->ClearExternalFECCells();
    m_pB->ClearExternalFECCells();
    m_pR->ClearExternalFECCells();
    m_pL->ClearExternalFECCells();
	
    m_pUT->ClearExternalECCells();
    m_pUR->ClearExternalECCells();
    m_pUB->ClearExternalECCells();
    m_pUL->ClearExternalECCells();
    m_pDT->ClearExternalECCells();
    m_pDR->ClearExternalECCells();
    m_pDB->ClearExternalECCells();
    m_pDL->ClearExternalECCells();
    m_pTR->ClearExternalECCells();
    m_pBR->ClearExternalECCells();
    m_pTL->ClearExternalECCells();
    m_pBL->ClearExternalECCells();
	
	m_pUTR->ClearExternalCells();
	m_pUTL->ClearExternalCells();
	m_pUBR->ClearExternalCells();
	m_pUBL->ClearExternalCells();
	m_pDTR->ClearExternalCells();
	m_pDTL->ClearExternalCells();
	m_pDBR->ClearExternalCells();
    m_pDBL->ClearExternalCells();
		

    MPI_Request  sendRequest1L;
    MPI_Request  receiveRequest1L;

    m_Msg1L.SetMessageData(GetFaceEdgeCornerBeads(*m_pL));

    m_LCoordCounter += m_Msg1L.GetSentBeadTotal();
//	std::cout << "Proc " << GetRank() << " has accumulated " << m_LCoordCounter << " bead coords to proc in L direction with pid " << LFacePid << zEndl;

    if(m_Msg1L.Validate())
    {
        m_Msg1L.ISend(LFacePid, &sendRequest1L);
        m_Msg1L.IReceive(RFacePid, &receiveRequest1L);
    }
	else
	{
	    std::cout << "Msg1L failed first validation" << zEndl;
	}

    // Message 1L - Wait
    // L Face processor: Blocking MPI_Wait of pmSendLinearBeadCoords to L Face processor
    // and from R Face processor that complete the non-blocking Send/Receive calls above.

    if(m_Msg1L.Validate())
    {
//	    std::cout << " **********  Msg1L about to execute its Wait function" << zEndl;
		
        m_Msg1L.Wait(&sendRequest1L);
        m_Msg1L.Wait(&receiveRequest1L);
//	    std::cout << " **********  Msg1L finished executing its Wait function" << zEndl;

        // **********
        // Pass the coordinates of the beads to the external CNT Cells that wrap
        // the R Face processor's Border cells. 

        // First we have to translate the bead coordinates into the owning processor's
        // Space. Because the linear SimBox only receives beads from a single adjacent
        // processor, we can treat the beads as belonging to a single Face and its
        // adjacent four Edges and Corners.

        TranslateBeadCoordsInNegativeX();
    }
	else
	{
	    std::cout << "Msg1L failed second validation" << zEndl;
	}


    MPI_Request  sendRequest1B;
    MPI_Request  receiveRequest1B;
	
    m_Msg1B.SetMessageData(GetFaceEdgeCornerBeads(*m_pB));
	
    m_BCoordCounter += m_Msg1B.GetSentBeadTotal();
//	std::cout << "Proc " << GetRank() << " has accumulated " << m_BCoordCounter << " bead coords to proc in B direction with pid " << BFacePid << zEndl;

    if(m_Msg1B.Validate())
    {
        m_Msg1B.ISend(BFacePid, &sendRequest1B);
        m_Msg1B.IReceive(TFacePid, &receiveRequest1B);
    }
	else
	{
	    std::cout << "Msg1B failed first validation" << zEndl;
	}

    // Message 1B - Wait
    // B Face processor: Blocking MPI_Wait of pmSendLinearBeadCoords to B Face processor
    // and from T Face processor that complete the non-blocking Send/Receive calls above.

    if(m_Msg1B.Validate())
    {
        m_Msg1B.Wait(&sendRequest1B);
        m_Msg1B.Wait(&receiveRequest1B);

        // **********
        // Pass the coordinates of the beads to the external CNT Cells that wrap
        // the T Face processor's Border cells. 

        // First we have to translate the bead coordinates into the owning processor's
        // Space. Because the linear SimBox only receives beads from a single adjacent
        // processor, we can treat the beads as belonging to a single Face and its
        // adjacent four Edges and Corners.

        TranslateBeadCoordsInNegativeY();
    }
	else
	{
	    std::cout << "Msg1B failed second validation" << zEndl;
	}

	// ********************
	// Now send/receive the Edge's beads to the diagonal processors in the
	// TL,BL directions

    m_Msg1TL.SetMessageData(GetEdgeCornerBeads(*m_pTL));
	
    // and start the non-blocking send. Note that the above function retrieves 
    // beads that are in the owning processor's Space not a neighbouring processor.

    MPI_Request  sendRequest1TL;
    MPI_Request  receiveRequest1TL;

    if(m_Msg1TL.Validate())
    {
        m_Msg1TL.ISend(TLEdgePid, &sendRequest1TL);
        m_Msg1TL.IReceive(BREdgePid, &receiveRequest1TL);
    }
	else
	{
	    std::cout << "Msg1TL failed first validation" << zEndl;
	}


    // Message 1TL - Wait
    // TL Edge processor: Blocking MPI_Wait of pmSendLinearBeadCoords to the
	// processor along the TL diagonal, and from the BR Face processor that 
	// complete the non-blocking Send/Receive calls above.

    if(m_Msg1TL.Validate())
    {
        m_Msg1TL.Wait(&sendRequest1TL);
        m_Msg1TL.Wait(&receiveRequest1TL);

        // **********
        // Pass the coordinates of the beads to the external CNT Cells that wrap
        // the BR Edge processor's Border cells. 

        // First we have to translate the bead coordinates into the owning 
		// processor's Space. As the beads are coming from the BR Edge processor,
		// we have to translate them negatively in X and positively in Y.

        TranslateBeadCoordsInNegXPosY();
    }
	else
	{
	    std::cout << "Msg1TL failed second validation" << zEndl;
	}
	
	
    MPI_Request  sendRequest1BL;
    MPI_Request  receiveRequest1BL;

    m_Msg1BL.SetMessageData(GetEdgeCornerBeads(*m_pBL));
	
    if(m_Msg1BL.Validate())
    {
        m_Msg1BL.ISend(BLEdgePid, &sendRequest1BL);
        m_Msg1BL.IReceive(TREdgePid, &receiveRequest1BL);
    }
	else
	{
	    std::cout << "Msg1BL failed first validation" << zEndl;
	}

    if(m_Msg1BL.Validate())
    {
        m_Msg1BL.Wait(&sendRequest1BL);
        m_Msg1BL.Wait(&receiveRequest1BL);

        TranslateBeadCoordsInNegXNegY();
    }
	else
	{
	    std::cout << "Msg1BL failed second validation" << zEndl;
	}
	
	
    MPI_Request  sendRequest1DB;
    MPI_Request  receiveRequest1DB;

    m_Msg1DB.SetMessageData(GetEdgeCornerBeads(*m_pDB));

    if(m_Msg1DB.Validate())
    {
        m_Msg1DB.ISend(BFacePid, &sendRequest1DB);
        m_Msg1DB.IReceive(TFacePid, &receiveRequest1DB);
    }
	else
	{
	    std::cout << "Msg1DB failed first validation" << zEndl;
	}

    if(m_Msg1DB.Validate())
    {
        m_Msg1DB.Wait(&sendRequest1DB);
        m_Msg1DB.Wait(&receiveRequest1DB);

        TranslateBeadCoordsInNegYNegZ();
    }
	else
	{
	    std::cout << "Msg1DB failed second validation" << zEndl;
	}
	
	
    MPI_Request  sendRequest1UL;
    MPI_Request  receiveRequest1UL;

    m_Msg1UL.SetMessageData(GetEdgeCornerBeads(*m_pUL));

    if(m_Msg1UL.Validate())
    {
        m_Msg1UL.ISend(LFacePid, &sendRequest1UL);
        m_Msg1UL.IReceive(RFacePid, &receiveRequest1UL);
    }
	else
	{
	    std::cout << "Msg1UL failed first validation" << zEndl;
	}

    if(m_Msg1UL.Validate())
    {
        m_Msg1UL.Wait(&sendRequest1UL);
        m_Msg1UL.Wait(&receiveRequest1UL);

        TranslateBeadCoordsInNegXPosZ();
    }
	else
	{
	    std::cout << "Msg1UL failed second validation" << zEndl;
	}

	
    MPI_Request  sendRequest1UB;
    MPI_Request  receiveRequest1UB;

    m_Msg1UB.SetMessageData(GetEdgeCornerBeads(*m_pUB));

    if(m_Msg1UB.Validate())
    {
        m_Msg1UB.ISend(BFacePid, &sendRequest1UB);
        m_Msg1UB.IReceive(TFacePid, &receiveRequest1UB);
    }
	else
	{
	    std::cout << "Msg1UB failed first validation" << zEndl;
	}

    if(m_Msg1UB.Validate())
    {
        m_Msg1UB.Wait(&sendRequest1UB);
        m_Msg1UB.Wait(&receiveRequest1UB);

        TranslateBeadCoordsInNegYPosZ();
    }
	else
	{
	    std::cout << "Msg1UB failed second validation" << zEndl;
	}
	
	
    MPI_Request  sendRequest1DL;
    MPI_Request  receiveRequest1DL;

    m_Msg1DL.SetMessageData(GetEdgeCornerBeads(*m_pDL));

    if(m_Msg1DL.Validate())
    {
        m_Msg1DL.ISend(LFacePid, &sendRequest1DL);
        m_Msg1DL.IReceive(RFacePid, &receiveRequest1DL);
    }
	else
	{
	    std::cout << "Msg1DL failed first validation" << zEndl;
	}

    if(m_Msg1DL.Validate())
    {
        m_Msg1DL.Wait(&sendRequest1DL);
        m_Msg1DL.Wait(&receiveRequest1DL);

        TranslateBeadCoordsInNegXNegZ();
    }
	else
	{
	    std::cout << "Msg1DL failed second validation" << zEndl;
	}
	
	// ********************
	// Now send/receive the Corner's bead coordinates to the corner diagonal 
	// processors in the DBL, DBR, DTR, DTL directions. Note that for the 
	// planar geometry, these are the same as the BL, BR, TR and TL processors.

    MPI_Request  sendRequest1DBL;
    MPI_Request  receiveRequest1DBL;

    m_Msg1DBL.SetMessageData(GetCornerBeads(*m_pDBL));

    if(m_Msg1DBL.Validate())
    {
        m_Msg1DBL.ISend(BLEdgePid, &sendRequest1DBL);
        m_Msg1DBL.IReceive(TREdgePid, &receiveRequest1DBL);
    }
	else
	{
	    std::cout << "Msg1DBL failed first validation" << zEndl;
	}

    if(m_Msg1DBL.Validate())
    {
        m_Msg1DBL.Wait(&sendRequest1DBL);
        m_Msg1DBL.Wait(&receiveRequest1DBL);

        TranslateBeadCoordsInNegXNegYNegZ();
    }
	else
	{
	    std::cout << "Msg1DBL failed second validation" << zEndl;
	}



    MPI_Request  sendRequest1UTL;
    MPI_Request  receiveRequest1UTL;

	m_Msg1UTL.SetMessageData(GetCornerBeads(*m_pUTL));

//    m_UTLCoordCounter += m_Msg1UTL.GetSentBeadTotal();
//	std::cout << "Proc " << GetRank() << " has accumulated " << m_UTLCoordCounter << " bead coords to proc in UTL direction with pid " << TLEdgePid << zEndl;
	
    if(m_Msg1UTL.Validate())
    {
        m_Msg1UTL.ISend(TLEdgePid, &sendRequest1UTL);
        m_Msg1UTL.IReceive(BREdgePid, &receiveRequest1UTL);
    }
	else
	{
	    std::cout << "Msg1UTL failed first validation" << zEndl;
	}

    if(m_Msg1UTL.Validate())
    {
        m_Msg1UTL.Wait(&sendRequest1UTL);
        m_Msg1UTL.Wait(&receiveRequest1UTL);

        TranslateBeadCoordsInNegXPosYPosZ();
    }
	else
	{
	    std::cout << "Msg1UTL failed second validation" << zEndl;
	}


    MPI_Request  sendRequest1UBL;
    MPI_Request  receiveRequest1UBL;

	m_Msg1UBL.SetMessageData(GetCornerBeads(*m_pUBL));

    if(m_Msg1UBL.Validate())
    {
        m_Msg1UBL.ISend(BLEdgePid, &sendRequest1UBL);
        m_Msg1UBL.IReceive(TREdgePid, &receiveRequest1UBL);
    }
	else
	{
	    std::cout << "Msg1UBL failed first validation" << zEndl;
	}

    if(m_Msg1UBL.Validate())
    {
        m_Msg1UBL.Wait(&sendRequest1UBL);
        m_Msg1UBL.Wait(&receiveRequest1UBL);

        TranslateBeadCoordsInNegXNegYPosZ();
    }
	else
	{
	    std::cout << "Msg1UBL failed second validation" << zEndl;
	}


    MPI_Request  sendRequest1DTL;
    MPI_Request  receiveRequest1DTL;

	m_Msg1DTL.SetMessageData(GetCornerBeads(*m_pDTL));

//    m_DTLCoordCounter += m_Msg1DTL.GetSentBeadTotal();
//	std::cout << "Proc " << GetRank() << " has accumulated " << m_DTLCoordCounter << " bead coords to proc in DTL direction with pid " << TLEdgePid << zEndl;
	
    if(m_Msg1DTL.Validate())
    {
        m_Msg1DTL.ISend(TLEdgePid, &sendRequest1DTL);
        m_Msg1DTL.IReceive(BREdgePid, &receiveRequest1DTL);
    }
	else
	{
	    std::cout << "Msg1DTL failed first validation" << zEndl;
	}

    if(m_Msg1DTL.Validate())
    {
        m_Msg1DTL.Wait(&sendRequest1DTL);
        m_Msg1DTL.Wait(&receiveRequest1DTL);

        TranslateBeadCoordsInNegXPosYNegZ();
    }
	else
	{
	    std::cout << "Msg1DTL failed second validation" << zEndl;
	}


    // **********
    // Calculate forces between beads in the U,T,R Faces and their adjacent 
	// Edge and Corner Regions including those interactions that span the 
	// processor boundaries. The calculations in the CalculateSharedForce() 
	// function DO depend on coordinates received from  other processors, 
	// and it stores forces that must be sent back to other processors.
    //
    // **********

    m_pU->UpdateForce();  // The U,D Faces are special as they are wholly internal
	m_pD->UpdateForce();
	
    m_pT->CalculateSharedForceT();	
	m_pB->CalculateSharedForce();
    m_pR->CalculateSharedForceR();
	m_pL->CalculateSharedForce();
	
    m_pUT->CalculateSharedForceUT();
	m_pUB->CalculateSharedForce();		
    m_pUR->CalculateSharedForceUR();
	m_pUL->CalculateSharedForce();
	
	m_pDT->CalculateSharedForceDT();
    m_pDB->CalculateSharedForce();
    m_pDR->CalculateSharedForceDR();
	m_pDL->CalculateSharedForce();

    m_pTR->CalculateSharedForceTR();
    m_pTL->CalculateSharedForceTL();
    m_pBR->CalculateSharedForceBR();
    m_pBL->CalculateSharedForce();
	
    m_pUTR->CalculateSharedForceUTR();
    m_pUTL->CalculateSharedForceUTL();
    m_pUBR->CalculateSharedForceUBR();
    m_pUBL->CalculateSharedForce();
    m_pDTR->CalculateSharedForceDTR();   
    m_pDTL->CalculateSharedForceDTL();
    m_pDBR->CalculateSharedForceDBR();
    m_pDBL->CalculateSharedForce();
	
	
	// Calculate bonded forces between beads in the owning processor's Border regions and their adjacent external cells. Note that this only works if ALL
	// bond lengths are less than the size of the CNT cells so that all bonded beads lie in adjacent CNT cells. We only calculate the forces for the Border
	// regions that normally calculate non-bonded bead forces to avoid double-counting. Each Border iterates over all of its external CNT cells and checks
	// all the beads therein to see if they are bonded to beads in the Border's own cells. 	
	
	m_ExtendedBondForceCounter = 0; 
	
	m_pT->CalculateSharedBondForce();
	m_pR->CalculateSharedBondForce();
	m_pBR->CalculateSharedBondForce();
	m_pTR->CalculateSharedBondForce();
	m_pUT->CalculateSharedBondForce();
	m_pDR->CalculateSharedBondForce();
	m_pDT->CalculateSharedBondForce();
	m_pUR->CalculateSharedBondForce();
	m_pUTR->CalculateSharedBondForce();
	m_pDBR->CalculateSharedBondForce();
	m_pDTR->CalculateSharedBondForce();
	m_pUBR->CalculateSharedBondForce();
	
	// Pass the extended bond force counter to P0 for summation to check the total number of bonds that all processors have counted
	
/*	m_ExtendedBondCounter = m_mmExtendedBonds.size();
	
	if(!IsProcessZero())
	{
		m_MsgExtForceCounter.SetSenderRank(GetRank());
	    m_MsgExtForceCounter.SetMessageData(m_ExtendedBondCounter, m_ExtendedBondForceCounter);
		
		if(m_MsgExtForceCounter.Validate())
		{
		    m_MsgExtForceCounter.SendP0();		
		}
		else
		{
	        std::cout << "ERROR on Proc " << GetRank() << " negative ext bond force counter " << m_ExtendedBondForceCounter << " out of " << m_ExtendedBondCounter << " stored extended bonds" << zEndl;
		}
	}
	else
	{
	    // Accumulate the counters for all processors including this one
		
		long bondTotal = 0;
		long efCounter = 0;
		for(long source=1; source<GetWorld(); source++)
		{
		    m_MsgExtForceCounter.Receive();
		
		    bondTotal = m_MsgExtForceCounter.GetBondTotal();
		    efCounter = m_MsgExtForceCounter.GetForceCounter();
			
            m_ExtendedBondCounter += bondTotal;
		    m_ExtendedBondForceCounter += efCounter;
        }
		
		// Check that the number of extended bond force calculations matches the number of extended bonds. Note that each bond is stored twice in
		// the map (once by its head bead and once by its tail bead), and each bond is held by two processors. Hence, there should be 4 times as  
		// many extended bonds as forces.
		
		if(4*m_ExtendedBondForceCounter != m_ExtendedBondCounter)
		{
		    std::cout << "ERROR on Proc " << GetRank() << " finished with extended bond force total " << 4*m_ExtendedBondForceCounter << " out of " << m_ExtendedBondCounter << zEndl;
        }
	}

*/

	// ****************************************
    // Message 2 - Non-blocking MPI_Irecv from L Face processor and MPI_Isend 
    //             to R Face of pmSendPlanarBeadForces containing calculated
    //             forces to add to adjacent processor's beads


    MPI_Request  sendRequest2R;
    MPI_Request  receiveRequest2R;

    m_Msg2R.SetMessageData(GetFaceEdgeCornerExternalCells(*m_pR));
	
    if(m_Msg2R.Validate())
    {
        m_Msg2R.ISend(RFacePid, &sendRequest2R);
        m_Msg2R.IReceive(LFacePid, &receiveRequest2R);

        // Message 2 - Wait
        // L Face processor: Blocking MPI_Wait of pmSendLinearBeadForces from L Face processor
        // that completes the non-blocking Send call above.

        m_Msg2R.Wait(&sendRequest2R);
        m_Msg2R.Wait(&receiveRequest2R);

        // **********
        // Accumulate the forces on beads in the external CNT Cells that wrap
        // the L Face processor's Border cells. 
		
//	std::cout << "Processor " << GetRank() << "  has sent coord total " << m_Msg1.GetSentBeadTotal()  << " to proc " << DFacePid << " and received coord total " << m_Msg1.GetRecBeadTotal() << " from proc " << UFacePid << zEndl;
//	std::cout << "Processor " << GetRank() << "  has sent force total " << m_Msg2.GetSentForceTotal() << " to proc " << UFacePid << " and received force total " << m_Msg2.GetRecForceTotal() << " from proc " << DFacePid << zEndl;

        TranslateBeadForcesInPositiveX();
	}
    else
    {
        std::cout << "Error in Msg2R: validation of forces failed" << zEndl;
    }


    MPI_Request  sendRequest2T;
    MPI_Request  receiveRequest2T;

    m_Msg2T.SetMessageData(GetFaceEdgeCornerExternalCells(*m_pT));
	
    if(m_Msg2T.Validate())
    {
        m_Msg2T.ISend(TFacePid, &sendRequest2T);
        m_Msg2T.IReceive(BFacePid, &receiveRequest2T);

		// ****************************************
        // Message 2 - Wait
        // D Face processor: Blocking MPI_Wait of pmSendPlanarBeadForces from D Face processor
        // that completes the non-blocking Send call above.

        m_Msg2T.Wait(&sendRequest2T);
        m_Msg2T.Wait(&receiveRequest2T);

        // **********
        // Accumulate the forces on beads in the external CNT Cells that wrap
        // the U Face processor's Border cells. 
		
//	std::cout << "Processor " << GetRank() << "  has sent coord total " << m_Msg1.GetSentBeadTotal()  << " to proc " << DFacePid << " and received coord total " << m_Msg1.GetRecBeadTotal() << " from proc " << UFacePid << zEndl;
//	std::cout << "Processor " << GetRank() << "  has sent force total " << m_Msg2.GetSentForceTotal() << " to proc " << UFacePid << " and received force total " << m_Msg2.GetRecForceTotal() << " from proc " << DFacePid << zEndl;

        TranslateBeadForcesInPositiveY();
    }
    else
    {
        std::cout << "Error in Msg2T: validation of forces failed" << zEndl;
    }

	// ********************
	// Now send/receive the Edge's forces to the diagonal processors in the
	// BR, TR directions to match the coordinates message sent in the TL, BL 
	// directions above.

    MPI_Request  sendRequest2BR;
    MPI_Request  receiveRequest2BR;

    m_Msg2BR.SetMessageData(GetEdgeCornerExternalCells(*m_pBR));
	
	
    if(m_Msg2BR.Validate())
    {
        m_Msg2BR.ISend(BREdgePid, &sendRequest2BR);
        m_Msg2BR.IReceive(TLEdgePid, &receiveRequest2BR);

        m_Msg2BR.Wait(&sendRequest2BR);
        m_Msg2BR.Wait(&receiveRequest2BR);

        TranslateBeadForcesInPosXNegY();
    }
    else
    {
        std::cout << "Error in Msg2BR: validation of forces failed" << zEndl;
    }


    MPI_Request  sendRequest2TR;
    MPI_Request  receiveRequest2TR;

    m_Msg2TR.SetMessageData(GetEdgeCornerExternalCells(*m_pTR));
		
    if(m_Msg2TR.Validate())
    {
        m_Msg2TR.ISend(TREdgePid, &sendRequest2TR);
        m_Msg2TR.IReceive(BLEdgePid, &receiveRequest2TR);

        m_Msg2TR.Wait(&sendRequest2TR);
        m_Msg2TR.Wait(&receiveRequest2TR);

        TranslateBeadForcesInPosXPosY();
    }
    else
    {
        std::cout << "Error in Msg2TR: validation of forces failed" << zEndl;
    }


    MPI_Request  sendRequest2UT;
    MPI_Request  receiveRequest2UT;

    m_Msg2UT.SetMessageData(GetEdgeCornerExternalCells(*m_pUT));
	
    if(m_Msg2UT.Validate())
    {
        m_Msg2UT.ISend(TFacePid, &sendRequest2UT);
        m_Msg2UT.IReceive(BFacePid, &receiveRequest2UT);

        m_Msg2UT.Wait(&sendRequest2UT);
        m_Msg2UT.Wait(&receiveRequest2UT);

        TranslateBeadForcesInPosYPosZ();
    }
    else
    {
        std::cout << "Error in Msg2UT: validation of forces failed" << zEndl;
    }


    MPI_Request  sendRequest2DR;
    MPI_Request  receiveRequest2DR;

    m_Msg2DR.SetMessageData(GetEdgeCornerExternalCells(*m_pDR));
	
    if(m_Msg2DR.Validate())
    {
        m_Msg2DR.ISend(RFacePid, &sendRequest2DR);
        m_Msg2DR.IReceive(LFacePid, &receiveRequest2DR);

        m_Msg2DR.Wait(&sendRequest2DR);
        m_Msg2DR.Wait(&receiveRequest2DR);

        TranslateBeadForcesInPosXNegZ();
    }
    else
    {
        std::cout << "Error in Msg2DR: validation of forces failed" << zEndl;
    }


    MPI_Request  sendRequest2DT;
    MPI_Request  receiveRequest2DT;

    m_Msg2DT.SetMessageData(GetEdgeCornerExternalCells(*m_pDT));
	
    if(m_Msg2DT.Validate())
    {
        m_Msg2DT.ISend(TFacePid, &sendRequest2DT);
        m_Msg2DT.IReceive(BFacePid, &receiveRequest2DT);

        m_Msg2DT.Wait(&sendRequest2DT);
        m_Msg2DT.Wait(&receiveRequest2DT);

        TranslateBeadForcesInPosYNegZ();
    }
    else
    {
        std::cout << "Error in Msg2DT: validation of forces failed" << zEndl;
    }


    MPI_Request  sendRequest2UR;
    MPI_Request  receiveRequest2UR;

    m_Msg2UR.SetMessageData(GetEdgeCornerExternalCells(*m_pUR));
	
    if(m_Msg2UR.Validate())
    {
        m_Msg2UR.ISend(RFacePid, &sendRequest2UR);
        m_Msg2UR.IReceive(LFacePid, &receiveRequest2UR);

        m_Msg2UR.Wait(&sendRequest2UR);
        m_Msg2UR.Wait(&receiveRequest2UR);

        TranslateBeadForcesInPosXPosZ();
    }
    else
    {
        std::cout << "Error in Msg2UR: validation of forces failed" << zEndl;
    }

	// ********************
	// Now send/receive the Corner's forces to the corner diagonal processors in the
	// UTR, UTL, UBL, UBR directions to match the coordinates message sent in the 
	// DBL, DBR, DTR, DTL messages above.

    MPI_Request  sendRequest2UTR;
    MPI_Request  receiveRequest2UTR;

    m_Msg2UTR.SetMessageData(GetCornerExternalCells(*m_pUTR));
	
    if(m_Msg2UTR.Validate())
    {
        m_Msg2UTR.ISend(TREdgePid, &sendRequest2UTR);
        m_Msg2UTR.IReceive(BLEdgePid, &receiveRequest2UTR);

        m_Msg2UTR.Wait(&sendRequest2UTR);
        m_Msg2UTR.Wait(&receiveRequest2UTR);

        TranslateBeadForcesInPosXPosYPosZ();
    }
    else
    {
        std::cout << "Error in Msg2UTR: validation of forces failed" << zEndl;
    }


    MPI_Request  sendRequest2DBR;
    MPI_Request  receiveRequest2DBR;

    m_Msg2DBR.SetMessageData(GetCornerExternalCells(*m_pDBR));
	
//    m_DBRForceCounter += m_Msg2DBR.GetSentForceTotal();
//	std::cout << "Proc " << GetRank() << " has  accumulated " << m_DBRForceCounter << " forces to proc in DBR direction with pid " << BREdgePid << zEndl;

    if(m_Msg2DBR.Validate())
    {
        m_Msg2DBR.ISend(BREdgePid, &sendRequest2DBR);
        m_Msg2DBR.IReceive(TLEdgePid, &receiveRequest2DBR);

        m_Msg2DBR.Wait(&sendRequest2DBR);
        m_Msg2DBR.Wait(&receiveRequest2DBR);

        TranslateBeadForcesInPosXNegYNegZ();
    }
    else
    {
        std::cout << "Error in Msg2DBR: validation of forces failed" << zEndl;
    }

    MPI_Request  sendRequest2DTR;
    MPI_Request  receiveRequest2DTR;

    m_Msg2DTR.SetMessageData(GetCornerExternalCells(*m_pDTR));
	
    if(m_Msg2DTR.Validate())
    {
        m_Msg2DTR.ISend(TREdgePid, &sendRequest2DTR);
        m_Msg2DTR.IReceive(BLEdgePid, &receiveRequest2DTR);

        m_Msg2DTR.Wait(&sendRequest2DTR);
        m_Msg2DTR.Wait(&receiveRequest2DTR);

        TranslateBeadForcesInPosXPosYNegZ();
    }
    else
    {
        std::cout << "Error in Msg2DTR: validation of forces failed" << zEndl;
    }


    MPI_Request  sendRequest2UBR;
    MPI_Request  receiveRequest2UBR;

    m_Msg2UBR.SetMessageData(GetCornerExternalCells(*m_pUBR));
	
//    m_UBRForceCounter += m_Msg2UBR.GetSentForceTotal();
//	std::cout << "Proc " << GetRank() << " has  accumulated " << m_UBRForceCounter << " forces to proc in UBR direction with pid " << BREdgePid << zEndl;

    if(m_Msg2UBR.Validate())
    {
        m_Msg2UBR.ISend(BREdgePid, &sendRequest2UBR);
        m_Msg2UBR.IReceive(TLEdgePid, &receiveRequest2UBR);

        m_Msg2UBR.Wait(&sendRequest2UBR);
        m_Msg2UBR.Wait(&receiveRequest2UBR);

        TranslateBeadForcesInPosXNegYPosZ();
    }
    else
    {
        std::cout << "Error in Msg2UBR: validation of forces failed" << zEndl;
    }

#endif
}

// ****************************************
// Private helper function to translate the coordinates of beads received by
// this processor from its neighbour in the R direction, and to bin them
// into the appropriate CNT cells for storing in the R Face and its adjacent
// Edges and Corners. 
// It must be called first as it clears the external cells for the R,T Faces and adjacent regions that are
// used by all the subsequent TranslateBeadCoordsInXXX functions.

void mpsPlanarZSimBox::TranslateBeadCoordsInNegativeX()
{
    zLongVector&   rBeadIds   = m_Msg1L.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1L.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1L.GetX();
    zDoubleVector& rBeadY     = m_Msg1L.GetY();
    zDoubleVector& rBeadZ     = m_Msg1L.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1L.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1L.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1L.GetVZ();

    // Shift the beads into the slice of external cells adjacent to the 
    // owning processor's +X Face and then bin them into the Face's CNT cells 
    // and its adjacent Edges and Corners using their y,z coordinates to 
    // determine which cell they belong to. 

    // For this planar SimBox, the index identifying the adjacent external cell
    // is always the same.

    const long index = GetPidIndexFromCoordinateDifferences(1,0,0);

    const long total = m_Msg1L.GetRecBeadTotal();
	
//	std::cout << "Proc " << GetRank() << " is translating " << total << " bead coords in -X with tag " << m_Msg1L.GetTag() << zEndl;

    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

		if(x > m_DX)
		{
		    std::cout << "TranslateBeads in -X error; initial X coord > 1:  " << index << " " << rBeadIds.at(i)  << zEndl;
		    std::cout << "original coords are:      " << x << " " << y << " "<< z <<  zEndl;
		    std::cout << "original velocities are:  " << rBeadVX.at(i) << " " << rBeadVY.at(i) << " "<<  rBeadVZ.at(i) <<  zEndl;
		}
		

        x +=GetSimBoxXLength();
        rBeadX.at(i) = x;

        const long iy = static_cast<long>(y/GetCNTYCellWidth());
        const long iz = static_cast<long>(z/GetCNTZCellWidth());
		
		if(iy < 0 || iz < 0)
		{
		    std::cout << "TranslateBeads in -X error:  " << index << " " << rBeadIds.at(i) << " " << iy << " " << iz << zEndl;
		    std::cout << "shifted x coords are:  " << x << " " << y << " "<< z <<  zEndl;
		    std::cout << "(SimBox X Length = " << GetSimBoxXLength() << " )" << zEndl;
		}
		
        m_pR->AddBeadToFECXCell(index, iy, iz, rBeadIds.at(i), rBeadTypes.at(i), 
                                rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
                                rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

// Private helper function to translate the coordinates of beads received by
// this processor from its neighbour in the T direction, and to bin them
// into the appropriate CNT cells for storing in the T Face and its adjacent
// Edges and Corners. 

void mpsPlanarZSimBox::TranslateBeadCoordsInNegativeY()
{
    zLongVector&   rBeadIds   = m_Msg1B.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1B.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1B.GetX();
    zDoubleVector& rBeadY     = m_Msg1B.GetY();
    zDoubleVector& rBeadZ     = m_Msg1B.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1B.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1B.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1B.GetVZ();

    // Shift the beads into the slice of external cells adjacent to the 
    // owning processor's +Y Face and then bin them into the Face's CNT cells 
    // and its adjacent Edges and Corners using their x,z coordinates to 
    // determine which cell they belong to. 

    // For this planar SimBox, the index identifying the adjacent external cell
    // is always the same.

    const long index = GetPidIndexFromCoordinateDifferences(0,1,0);

    const long total = m_Msg1B.GetRecBeadTotal();
	
//	std::cout << "Proc " << GetRank() << " is translating " << total << " bead coords in -Y with tag " << m_Msg1B.GetTag() << zEndl;

    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

		if(y > m_DY)
		{
		    std::cout << "TranslateBeads in -Y error; initial Y coord > 1:  " << index << " " << rBeadIds.at(i)  << zEndl;
		    std::cout << "original coords are:      " << x << " " << y << " "<< z <<  zEndl;
		    std::cout << "original velocities are:  " << rBeadVX.at(i) << " " << rBeadVY.at(i) << " "<<  rBeadVZ.at(i) <<  zEndl;
		}
		
        y +=GetSimBoxYLength();
        rBeadY.at(i) = y;

        const long ix = static_cast<long>(x/GetCNTXCellWidth());
        const long iz = static_cast<long>(z/GetCNTZCellWidth());

		if(ix < 0 || iz < 0)
		{
		    std::cout << "TranslateBeads in -Y error:  " << ix << " " << iz << zEndl;
		}
		
		if(GetRank() == 0)
		{
//		    std::cout << "Proc " << GetRank() << " is passing bead with id " << rBeadIds.at(i) << " down to T Face with indices " << ix << " " << iz << " " << x << " " << y << " " << z << zEndl;
		
		}
		
        m_pT->AddBeadToFECYCell(index, ix, iz, rBeadIds.at(i), rBeadTypes.at(i), 
                                rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
                                rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

// Function to collect the forces on all beads in the external cells of this processor's
// L Face. First we get the container of cells, then we extract the
// forces on all beads in the cells in the order that they occur. This means that the
// order of the bead forces is the same as that of the original beads in the cells.

void mpsPlanarZSimBox::TranslateBeadForcesInPositiveX()
{
//    const long total = m_Msg2R.GetRecForceTotal();

    BeadList       lBeads = GetFaceEdgeCornerBeads(*m_pL);
    zLongVector&   rIds   = m_Msg2R.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2R.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2R.GetFX();
    zDoubleVector& rFY    = m_Msg2R.GetFY();
    zDoubleVector& rFZ    = m_Msg2R.GetFZ();

//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +X for " << lBeads.size() << zEndl;	
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

// Function to collect the forces on all beads in the external cells of this processor's
// B Face. First we get the container of cells, then we extract the
// forces on all beads in the cells in the order that they occur. This means that the
// order of the bead forces is the same as that of the original beads in the cells.

void mpsPlanarZSimBox::TranslateBeadForcesInPositiveY()
{
//    const long total = m_Msg2T.GetRecForceTotal();

    BeadList       lBeads = GetFaceEdgeCornerBeads(*m_pB);
    zLongVector&   rIds   = m_Msg2T.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2T.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2T.GetFX();
    zDoubleVector& rFY    = m_Msg2T.GetFY();
    zDoubleVector& rFZ    = m_Msg2T.GetFZ();

//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +Y for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

// ****************************************
// Private helper function to translate the coordinates of beads received by
// this processor from its diagonal neighbour in the BR direction, and to 
// bin them into the appropriate CNT cells for storing in the BR Edge and its 
// adjacent Corners. 

void mpsPlanarZSimBox::TranslateBeadCoordsInNegXPosY()
{
    zLongVector&   rBeadIds   = m_Msg1TL.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1TL.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1TL.GetX();
    zDoubleVector& rBeadY     = m_Msg1TL.GetY();
    zDoubleVector& rBeadZ     = m_Msg1TL.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1TL.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1TL.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1TL.GetVZ();


    const long index = GetPidIndexFromCoordinateDifferences(1,-1,0);

    const long total = m_Msg1TL.GetRecBeadTotal();
	
//	std::cout << "Proc " << GetRank() << " is translating " << total << " bead coords in -X+Y with index " << index << zEndl;

    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

        x +=GetSimBoxXLength();
        rBeadX.at(i) = x;

        y -=GetSimBoxYLength();
        rBeadY.at(i) = y;

        const long iz = static_cast<long>(z/GetCNTZCellWidth());

//        std::cout << "-X+Y:  bead " << rBeadIds.at(i) << " " << x << " " << y << " " << z << zEndl;
        if(iz < 0)
		{
		    std::cout << "TranslateBeads in -X+Y error:  " << iz << zEndl;
            std::cout << rBeadIds.at(i) << " " << iz << " " << x << " " << y << " " << z << zEndl;
		}
		
        m_pBR->AddBeadToECCell(index, iz, rBeadIds.at(i), rBeadTypes.at(i), 
							   rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
							   rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

// Private helper function to translate the coordinates of beads received by
// this processor from its diagonal neighbour in the TR direction, and to 
// bin them into the appropriate CNT cells for storing in the TR Edge and its 
// adjacent Corners. 

void mpsPlanarZSimBox::TranslateBeadCoordsInNegXNegY()
{
    zLongVector&   rBeadIds   = m_Msg1BL.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1BL.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1BL.GetX();
    zDoubleVector& rBeadY     = m_Msg1BL.GetY();
    zDoubleVector& rBeadZ     = m_Msg1BL.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1BL.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1BL.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1BL.GetVZ();


    const long index = GetPidIndexFromCoordinateDifferences(1,1,0);

    const long total = m_Msg1BL.GetRecBeadTotal();
	
//	std::cout << "Proc " << GetRank() << " is translating " << total << " bead coords in -X-Y with index " << index << zEndl;

    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

        x +=GetSimBoxXLength();
        rBeadX.at(i) = x;

        y +=GetSimBoxYLength();
        rBeadY.at(i) = y;

        const long iz = static_cast<long>(z/GetCNTZCellWidth());

        if(iz < 0)
		{
		    std::cout << "TranslateBeads in -X-Y error:  " << iz << zEndl;
            std::cout << rBeadIds.at(i) << " " << iz << " " << x << " " << y << " " << z << zEndl;
		}
			
        m_pTR->AddBeadToECCell(index, iz, rBeadIds.at(i), rBeadTypes.at(i), 
							   rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
							   rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

// Private helper function to translate the coordinates of beads received by
// this processor from its diagonal neighbour in the UT direction, and to 
// bin them into the appropriate CNT cells for storing in the UT Edge and its 
// adjacent Corners. 

void mpsPlanarZSimBox::TranslateBeadCoordsInNegYNegZ()
{
    zLongVector&   rBeadIds   = m_Msg1DB.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1DB.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1DB.GetX();
    zDoubleVector& rBeadY     = m_Msg1DB.GetY();
    zDoubleVector& rBeadZ     = m_Msg1DB.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1DB.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1DB.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1DB.GetVZ();


    const long index = GetPidIndexFromCoordinateDifferences(0,1,1);

    const long total = m_Msg1DB.GetRecBeadTotal();
	
//	std::cout << "Proc " << GetRank() << " is translating " << total << " bead coords in -X-Y with index " << index << zEndl;

    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

        y +=GetSimBoxYLength();
        rBeadY.at(i) = y;

        z +=GetSimBoxZLength();
        rBeadZ.at(i) = z;

        const long ix = static_cast<long>(x/GetCNTXCellWidth());

        if(ix < 0)
		{
		    std::cout << "TranslateBeads in -Y-Z error:  " << ix << zEndl;
            std::cout << rBeadIds.at(i) << " " << ix << " " << x << " " << y << " " << z << zEndl;
		}
			
        m_pUT->AddBeadToECCell(index, ix, rBeadIds.at(i), rBeadTypes.at(i), 
							   rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
							   rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

// Private helper function to translate the coordinates of beads received by
// this processor from its diagonal neighbour in the UL direction, and to 
// bin them into the appropriate CNT cells for storing in the UL Edge and its 
// adjacent Corners. 

void mpsPlanarZSimBox::TranslateBeadCoordsInNegXPosZ()
{
    zLongVector&   rBeadIds   = m_Msg1UL.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1UL.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1UL.GetX();
    zDoubleVector& rBeadY     = m_Msg1UL.GetY();
    zDoubleVector& rBeadZ     = m_Msg1UL.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1UL.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1UL.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1UL.GetVZ();

    const long index = GetPidIndexFromCoordinateDifferences(1,0,-1);

    const long total = m_Msg1UL.GetRecBeadTotal();
	
//	std::cout << "Proc " << GetRank() << " is translating " << total << " bead coords in -X+Z with index " << index << zEndl;
			
    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

        x +=GetSimBoxXLength();
        rBeadX.at(i) = x;

        z -=GetSimBoxZLength();
        rBeadZ.at(i) = z;

        const long iy = static_cast<long>(y/GetCNTYCellWidth());

        if(iy < 0)
		{
		    std::cout << "TranslateBeads in -X+Z error:  " << iy << zEndl;
            std::cout << rBeadIds.at(i) << " " << iy << " " << x << " " << y << " " << z << zEndl;
		}
			
        m_pDR->AddBeadToECCell(index, iy, rBeadIds.at(i), rBeadTypes.at(i), 
							   rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
							   rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

// Private helper function to translate the coordinates of beads received by
// this processor from its diagonal neighbour in the UT direction, and to 
// bin them into the appropriate CNT cells for storing in the UT Edge and its 
// adjacent Corners. 

void mpsPlanarZSimBox::TranslateBeadCoordsInNegYPosZ()
{
    zLongVector&   rBeadIds   = m_Msg1UB.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1UB.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1UB.GetX();
    zDoubleVector& rBeadY     = m_Msg1UB.GetY();
    zDoubleVector& rBeadZ     = m_Msg1UB.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1UB.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1UB.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1UB.GetVZ();

    const long index = GetPidIndexFromCoordinateDifferences(0,1,-1);

    const long total = m_Msg1UB.GetRecBeadTotal();
	
    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

        y +=GetSimBoxYLength();
        rBeadY.at(i) = y;

        z -=GetSimBoxZLength();
        rBeadZ.at(i) = z;

        const long ix = static_cast<long>(x/GetCNTXCellWidth());

        if(ix < 0)
		{
		    std::cout << "TranslateBeads in -+Y-Z error:  " << ix << zEndl;
            std::cout << rBeadIds.at(i) << " " << ix << " " << x << " " << y << " " << z << zEndl;
		}
			
        m_pDT->AddBeadToECCell(index, ix, rBeadIds.at(i), rBeadTypes.at(i), 
							   rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
							   rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}


// Private helper function to translate the coordinates of beads received by
// this processor from its diagonal neighbour in the UB direction, and to 
// bin them into the appropriate CNT cells for storing in the UB Edge and its 
// adjacent Corners. 

void mpsPlanarZSimBox::TranslateBeadCoordsInNegXNegZ()
{
    zLongVector&   rBeadIds   = m_Msg1DL.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1DL.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1DL.GetX();
    zDoubleVector& rBeadY     = m_Msg1DL.GetY();
    zDoubleVector& rBeadZ     = m_Msg1DL.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1DL.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1DL.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1DL.GetVZ();

    const long index = GetPidIndexFromCoordinateDifferences(1,0,1);

    const long total = m_Msg1DL.GetRecBeadTotal();
	
    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

		x +=GetSimBoxXLength();
        rBeadX.at(i) = x;

        z +=GetSimBoxZLength();
        rBeadZ.at(i) = z;

        const long iy = static_cast<long>(y/GetCNTYCellWidth());

        if(iy < 0)
		{
		    std::cout << "TranslateBeads in +X-Z error:  " << iy << zEndl;
            std::cout << rBeadIds.at(i) << " " << iy << " " << x << " " << y << " " << z << zEndl;
		}
			
        m_pUR->AddBeadToECCell(index, iy, rBeadIds.at(i), rBeadTypes.at(i), 
							   rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
							   rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

void mpsPlanarZSimBox::TranslateBeadCoordsInNegXNegYNegZ()
{
    zLongVector&   rBeadIds   = m_Msg1DBL.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1DBL.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1DBL.GetX();
    zDoubleVector& rBeadY     = m_Msg1DBL.GetY();
    zDoubleVector& rBeadZ     = m_Msg1DBL.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1DBL.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1DBL.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1DBL.GetVZ();

    const long index = GetPidIndexFromCoordinateDifferences(1,1,1);

    const long total = m_Msg1DBL.GetRecBeadTotal();
	
    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

        x +=GetSimBoxXLength();
        rBeadX.at(i) = x;

        y +=GetSimBoxYLength();
        rBeadY.at(i) = y;

        z +=GetSimBoxZLength();
        rBeadZ.at(i) = z;
		
        m_pUTR->AddBeadToCell(index, rBeadIds.at(i), rBeadTypes.at(i), 
							   rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
							   rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

void mpsPlanarZSimBox::TranslateBeadCoordsInNegXPosYPosZ()
{
    zLongVector&   rBeadIds   = m_Msg1UTL.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1UTL.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1UTL.GetX();
    zDoubleVector& rBeadY     = m_Msg1UTL.GetY();
    zDoubleVector& rBeadZ     = m_Msg1UTL.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1UTL.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1UTL.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1UTL.GetVZ();

    const long index = GetPidIndexFromCoordinateDifferences(1,-1,-1);

    const long total = m_Msg1UTL.GetRecBeadTotal();
	
//	std::cout << "Proc " << GetRank() << " is translating " << total << " bead coords in -X+Y+Z with index " << index << zEndl;
	
    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

        x +=GetSimBoxXLength();
        rBeadX.at(i) = x;

        y -=GetSimBoxYLength();
        rBeadY.at(i) = y;

        z -=GetSimBoxZLength();
        rBeadZ.at(i) = z;
		
        m_pDBR->AddBeadToCell(index, rBeadIds.at(i), rBeadTypes.at(i), 
							   rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
							   rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

// Private helper function to translate the coordinates of beads received by
// this processor from its corner diagonal neighbour in the UBL direction, 
// and to bin them into the CNT cell for storing in the UBL Corner.

void mpsPlanarZSimBox::TranslateBeadCoordsInNegXNegYPosZ()
{
    zLongVector&   rBeadIds   = m_Msg1UBL.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1UBL.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1UBL.GetX();
    zDoubleVector& rBeadY     = m_Msg1UBL.GetY();
    zDoubleVector& rBeadZ     = m_Msg1UBL.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1UBL.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1UBL.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1UBL.GetVZ();

    const long index = GetPidIndexFromCoordinateDifferences(1,1,-1);

    const long total = m_Msg1UBL.GetRecBeadTotal();
	
    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

        x +=GetSimBoxXLength();
        rBeadX.at(i) = x;

        y +=GetSimBoxYLength();
        rBeadY.at(i) = y;

        z -=GetSimBoxZLength();
        rBeadZ.at(i) = z;
		
        m_pDTR->AddBeadToCell(index, rBeadIds.at(i), rBeadTypes.at(i), 
							   rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
							   rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

void mpsPlanarZSimBox::TranslateBeadCoordsInNegXPosYNegZ()
{
    zLongVector&   rBeadIds   = m_Msg1DTL.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1DTL.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1DTL.GetX();
    zDoubleVector& rBeadY     = m_Msg1DTL.GetY();
    zDoubleVector& rBeadZ     = m_Msg1DTL.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1DTL.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1DTL.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1DTL.GetVZ();

    const long index = GetPidIndexFromCoordinateDifferences(1,-1,1);

    const long total = m_Msg1DTL.GetRecBeadTotal();
	
//	std::cout << "Proc " << GetRank() << " is translating " << total << " bead coords in -X+Y-Z with index " << index << zEndl;
	
    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);
		
//		std::cout << "UBR receiving bead " << rBeadIds.at(i) << zEndl;

        x +=GetSimBoxXLength();
        rBeadX.at(i) = x;

        y -=GetSimBoxYLength();
        rBeadY.at(i) = y;

        z +=GetSimBoxZLength();
        rBeadZ.at(i) = z;
		
        m_pUBR->AddBeadToCell(index, rBeadIds.at(i), rBeadTypes.at(i), 
							   rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
							   rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}


// Function to collect the forces on all beads in the external cells of this 
// processor's TL Edge. First we get the container of cells, then we extract the
// forces on all beads in the cells in the order that they occur. This means 
// that the order of the bead forces is the same as that of the original beads 
// in the cells.

void mpsPlanarZSimBox::TranslateBeadForcesInPosXNegY()
{
//    const long total = m_Msg2BR.GetRecForceTotal();

    BeadList       lBeads = GetEdgeCornerBeads(*m_pTL);
    zLongVector&   rIds   = m_Msg2BR.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2BR.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2BR.GetFX();
    zDoubleVector& rFY    = m_Msg2BR.GetFY();
    zDoubleVector& rFZ    = m_Msg2BR.GetFZ();

//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +X-Y for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

// Function to collect the forces on all beads in the external cells of this 
// processor's BL Edge. First we get the container of cells, then we extract the
// forces on all beads in the cells in the order that they occur. This means 
// that the order of the bead forces is the same as that of the original beads 
// in the cells.

void mpsPlanarZSimBox::TranslateBeadForcesInPosXPosY()
{
//    const long total = m_Msg2TR.GetRecForceTotal();

    BeadList       lBeads = GetEdgeCornerBeads(*m_pBL);
    zLongVector&   rIds   = m_Msg2TR.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2TR.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2TR.GetFX();
    zDoubleVector& rFY    = m_Msg2TR.GetFY();
    zDoubleVector& rFZ    = m_Msg2TR.GetFZ();

//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +X+Y for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

void mpsPlanarZSimBox::TranslateBeadForcesInPosYPosZ()
{
//    const long total = m_Msg2UT.GetRecForceTotal();

    BeadList       lBeads = GetEdgeCornerBeads(*m_pDB);
    zLongVector&   rIds   = m_Msg2UT.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2UT.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2UT.GetFX();
    zDoubleVector& rFY    = m_Msg2UT.GetFY();
    zDoubleVector& rFZ    = m_Msg2UT.GetFZ();

//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +Y+Z for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

void mpsPlanarZSimBox::TranslateBeadForcesInPosXNegZ()
{
//    const long total = m_Msg2DR.GetRecForceTotal();

    BeadList       lBeads = GetEdgeCornerBeads(*m_pUL);
    zLongVector&   rIds   = m_Msg2DR.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2DR.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2DR.GetFX();
    zDoubleVector& rFY    = m_Msg2DR.GetFY();
    zDoubleVector& rFZ    = m_Msg2DR.GetFZ();

//     std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +X-Z for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

void mpsPlanarZSimBox::TranslateBeadForcesInPosYNegZ()
{
//    const long total = m_Msg2DT.GetRecForceTotal();

    BeadList       lBeads = GetEdgeCornerBeads(*m_pUB);
    zLongVector&   rIds   = m_Msg2DT.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2DT.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2DT.GetFX();
    zDoubleVector& rFY    = m_Msg2DT.GetFY();
    zDoubleVector& rFZ    = m_Msg2DT.GetFZ();

//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +Y-Z for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

void mpsPlanarZSimBox::TranslateBeadForcesInPosXPosZ()
{
//    const long total = m_Msg2UR.GetRecForceTotal();

    BeadList       lBeads = GetEdgeCornerBeads(*m_pDL);
    zLongVector&   rIds   = m_Msg2UR.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2UR.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2UR.GetFX();
    zDoubleVector& rFY    = m_Msg2UR.GetFY();
    zDoubleVector& rFZ    = m_Msg2UR.GetFZ();

//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +X+Z for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

void mpsPlanarZSimBox::TranslateBeadForcesInPosXPosYPosZ()
{
//    const long total = m_Msg2UTR.GetRecForceTotal();

    BeadList       lBeads = GetCornerBeads(*m_pDBL);
    zLongVector&   rIds   = m_Msg2UTR.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2UTR.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2UTR.GetFX();
    zDoubleVector& rFY    = m_Msg2UTR.GetFY();
    zDoubleVector& rFZ    = m_Msg2UTR.GetFZ();
	
//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +X+Y+Z for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

void mpsPlanarZSimBox::TranslateBeadForcesInPosXNegYNegZ()
{
//    const long total = m_Msg2DBR.GetRecForceTotal();
	
    BeadList       lBeads = GetCornerBeads(*m_pUTL);
    zLongVector&   rIds   = m_Msg2DBR.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2DBR.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2DBR.GetFX();
    zDoubleVector& rFY    = m_Msg2DBR.GetFY();
    zDoubleVector& rFZ    = m_Msg2DBR.GetFZ();

//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +X-Y-Z for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

void mpsPlanarZSimBox::TranslateBeadForcesInPosXPosYNegZ()
{
//    const long total = m_Msg2DTR.GetRecForceTotal();
	
    BeadList       lBeads = GetCornerBeads(*m_pUBL);
    zLongVector&   rIds   = m_Msg2DTR.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2DTR.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2DTR.GetFX();
    zDoubleVector& rFY    = m_Msg2DTR.GetFY();
    zDoubleVector& rFZ    = m_Msg2DTR.GetFZ();
	
//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +X+Y-Z for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

void mpsPlanarZSimBox::TranslateBeadForcesInPosXNegYPosZ()
{
//    const long total = m_Msg2UBR.GetRecForceTotal();

    BeadList       lBeads = GetCornerBeads(*m_pDTL);
    zLongVector&   rIds   = m_Msg2UBR.GetBeadIds();
    zLongVector&   rFCs   = m_Msg2UBR.GetForceCounter();
    zDoubleVector& rFX    = m_Msg2UBR.GetFX();
    zDoubleVector& rFY    = m_Msg2UBR.GetFY();
    zDoubleVector& rFZ    = m_Msg2UBR.GetFZ();

//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces in +X-Y+Z for " << lBeads.size() << zEndl;
    AddExternalForcesToBeads(lBeads, rIds, rFCs, rFX, rFY, rFZ);
}

