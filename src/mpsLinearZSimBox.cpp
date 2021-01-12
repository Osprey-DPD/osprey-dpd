/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpsLinearZSimBox.cpp: implementation of the mpsLinearZSimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mpsLinearZSimBox.h"
#include "pmProcessorBeadCMVelocity.h"
#include "pmProcessorBeadAngularMomentum.h"

long mpsLinearZSimBox::UFaceDifference = 0;
long mpsLinearZSimBox::DFaceDifference = 0;
long mpsLinearZSimBox::netTotal = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// This class represents the parallel SimBox for processors with a
// linear arrangement in Space for which only the Z dimension contains
// more than one processor. This means that in the X, Y dimensions each 
// processor only has to communicate with itself.

mpsLinearZSimBox::mpsLinearZSimBox(long px, long py, long pz, long lx, long ly, long lz,
                                   double dx, double dy, double dz, CNTCellVector&  rvCNTCells,
								   PolymerVector& rvBulkPolymers) : mpsSimBox(1, 3, px, py, pz, lx, ly, lz, dx, dy, dz, rvCNTCells, rvBulkPolymers)
{	
    // Store the pids of the containing processor's nearest neighbours for use
    // in message passing. Each processor's pid is its rank in the World.
    // If the assembly function fails, we set a flag in the base class
    // indicating that the simulation must be aborted.
    //
    // Statistics are collected on the number of beads that pass between processors
	// in the Z direction. The counters are initialised in the base class and
	// updated in the UpdateBorderPos() function. Results are written out in this
	// class' destructor. For the linear geometry, there is only one counter.

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
	
	 // Set tags for each message to distinguish different instances of the same message class
	 
	 m_Msg1.SetTag(1);
	 m_Msg2.SetTag(2);
	 m_Msg3.SetTag(3);
	 m_Msg4.SetTag(4);

    // Set the pids of the two adjacent processors used in a linear geometry. Note that the other pids will have an illegal value
	// that is set in the base class.

    UFacePid = GetNeighbourPidFromCoordinateDifferences(0,0,1);
    DFacePid = GetNeighbourPidFromCoordinateDifferences(0,0,-1);

}

// Copy constructor

mpsLinearZSimBox::mpsLinearZSimBox(const mpsLinearZSimBox& oldSimBox) : mpsSimBox(oldSimBox),
                                   m_Msg1(oldSimBox.m_Msg1), m_Msg2(oldSimBox.m_Msg2),
                                   m_Msg3(oldSimBox.m_Msg3), m_Msg4(oldSimBox.m_Msg4)
{
}


mpsLinearZSimBox::~mpsLinearZSimBox()
{
    // Write out the statistics on beads crossing between processors.
	// There is only a single counter per processor for a linear geometry.
	
	double sum  = static_cast<double>(m_vBeadCrossing.at(0));
	double sum2 = static_cast<double>(m_vBeadCrossing2.at(0));
	
	double average  = sum / static_cast<double>(m_BeadCrossingCounter);
	double variance = (sum2 / static_cast<double>(m_BeadCrossingCounter)) - average*average;
	
	std::cout << "Processor " << GetRank() << " recorded a U-D bead transfer average/variance of " << average << " " << variance << " after " << m_BeadCrossingCounter << " calls" << zEndl;
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

void mpsLinearZSimBox::PartitionSpace()
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
    // two layers of external cells adjacent to the U and D faces of the processor's Space. 
    // Note that this means the U Face external cells actually wrap the D Face cells,
    // and vice versa.
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

    m_pU = new mpsFace(this, true,  m_LX, m_LY, m_LZ, m_LX-2, m_LY-2, 1,  0,  0,  1, m_rvCNTCells);
    m_pD = new mpsFace(this, true,  m_LX, m_LY, m_LZ, m_LX-2, m_LY-2, 1,  0,  0, -1, m_rvCNTCells);
    m_pR = new mpsFace(this, false, m_LX, m_LY, m_LZ, 1, m_LY-2, m_LZ-2,  1,  0,  0, m_rvCNTCells);
    m_pL = new mpsFace(this, false, m_LX, m_LY, m_LZ, 1, m_LY-2, m_LZ-2, -1,  0,  0, m_rvCNTCells);
    m_pT = new mpsFace(this, false, m_LX, m_LY, m_LZ, m_LX-2, 1, m_LZ-2,  0,  1,  0, m_rvCNTCells);
    m_pB = new mpsFace(this, false, m_LX, m_LY, m_LZ, m_LX-2, 1, m_LZ-2,  0, -1,  0, m_rvCNTCells);
  
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
// The linear Z SimBox case requires the +- Z Face/Edge/Corners to communicate with
// adjacent processors, but the R,T,L,B Faces and their associated Edges do not.
// Note that Corners are external for all simulation Space geometries.

void mpsLinearZSimBox::UpdateBorderPos()
{
#if SimMPS == SimulationEnabled

	// Debugcheck on the number of beads in the parallel simbox
	
/*	long beadTotal = 0;
	
    for(long iz = 0; iz < m_LZ; iz++)
    {
        for(long iy = 0; iy < m_LY; iy++)
        {
            for(long ix = 0; ix < m_LX; ix++)
            {
                beadTotal += m_rvCNTCells.at(GetCellIndexFromCoords(ix,iy,iz))->CellBeadTotal();
            }
        }
    }
	
	std::cout << "Proc " << GetRank() << " has bead total " << beadTotal << zEndl;
*/	
    // First we move beads in regions where they may move to an another processor

    m_pU->UpdateSharedPos();
    m_pD->UpdateSharedPos();
	    
    // Messages 3/4 - Non-blocking MPI_Isend to U/D Face processor and MPI_Irecv from
    //             D/U Face processor of pmSendLinearBeads containing 
    //             bead instances in external cells adjacent to the U Face/Corner/Edge regions



    // Collect the bead instances to send to U face proc: they are all the beads
    // in the external cells adjacent to the current processor's U face plus its 
    // adjacent Edges and Corners. 
    // Note that a linear SimBox only needs to send beads from the current processor 
    // as the simulation Space is periodic in the transverse directions.
    // The message unpacks the bead instances to package their coordinates in
    // the message payload.

	BeadList  lUFaceBeads;
	lUFaceBeads.clear();
	lUFaceBeads = m_pU->GetExternalFECBeads();

	BeadList  lDFaceBeads;
	lDFaceBeads.clear();
	lDFaceBeads = m_pD->GetExternalFECBeads();

	BeadList  lBeads;
	lBeads.clear();

	lBeads = m_pUR->GetExternalECBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lUFaceBeads));
	lBeads = m_pUT->GetExternalECBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lUFaceBeads));
	lBeads = m_pUL->GetExternalECBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lUFaceBeads));
	lBeads = m_pUB->GetExternalECBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lUFaceBeads));
	lBeads = m_pUTR->GetExternalBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lUFaceBeads));
	lBeads = m_pUTL->GetExternalBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lUFaceBeads));
	lBeads = m_pUBR->GetExternalBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lUFaceBeads));
	lBeads = m_pUBL->GetExternalBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lUFaceBeads));

	lBeads = m_pDR->GetExternalECBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lDFaceBeads));
	lBeads = m_pDT->GetExternalECBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lDFaceBeads));
	lBeads = m_pDL->GetExternalECBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lDFaceBeads));
	lBeads = m_pDB->GetExternalECBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lDFaceBeads));
	lBeads = m_pDTR->GetExternalBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lDFaceBeads));
	lBeads = m_pDTL->GetExternalBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lDFaceBeads));
	lBeads = m_pDBR->GetExternalBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lDFaceBeads));
	lBeads = m_pDBL->GetExternalBeads();
	copy(lBeads.begin(), lBeads.end(), back_inserter(lDFaceBeads));

    m_Msg3.SetMessageData(lUFaceBeads);
    m_Msg4.SetMessageData(lDFaceBeads);
	
	  

//    std::cout << "Processor " << GetRank() << " has U-D bead total " << m_Msg3.GetSentBeadTotal()-m_Msg4.GetSentBeadTotal() << zEndl;


    // and start the non-blocking send. Note that the above function retrieves 
    // beads that are in the owning processor's Space not a neighbouring processo.
    // The latter beads are returned by GetDFaceNeighbourBeads().
    // Simultaneously, we use the same message to receive beads from the U Face
    // processor that has itself sent the message to its D Face processor, which is
    // the current processor.

    MPI_Request  sendRequest3;
    MPI_Request  receiveRequest3;

    if(m_Msg3.Validate())
    {
        m_Msg3.ISend(UFacePid, &sendRequest3);
        m_Msg3.IReceive(DFacePid, &receiveRequest3);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg3 failed to validate its data" << zEndl;
	}
    
    MPI_Request  sendRequest4;
    MPI_Request  receiveRequest4;

    if(m_Msg4.Validate())
    {
        m_Msg4.ISend(DFacePid, &sendRequest4);
		m_Msg4.IReceive(UFacePid, &receiveRequest4);
    }
	else
	{
		std::cout << "Proc " << GetRank() << " Msg4 failed to validate its data" << zEndl;
	}
 
   // Now remove the old bead pointers from the external cells belonging to the Faces' FEC cells.
   
	m_pU->RemoveBeadsFromExternalFECCells();
	m_pD->RemoveBeadsFromExternalFECCells();
	m_pR->RemoveBeadsFromExternalFECCells();
	m_pL->RemoveBeadsFromExternalFECCells();
	m_pT->RemoveBeadsFromExternalFECCells();
	m_pB->RemoveBeadsFromExternalFECCells();
	
   m_pUR->RemoveBeadsFromExternalECCells();
   m_pUT->RemoveBeadsFromExternalECCells();
   m_pUL->RemoveBeadsFromExternalECCells();
   m_pUB->RemoveBeadsFromExternalECCells();
   m_pDR->RemoveBeadsFromExternalECCells();
   m_pDT->RemoveBeadsFromExternalECCells();
   m_pDL->RemoveBeadsFromExternalECCells();
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


   // Now move beads in regions that do not border adjacent processors

    m_pR->UpdatePos();
    m_pT->UpdatePos();
    m_pL->UpdatePos();
    m_pB->UpdatePos();

    m_pTR->UpdatePos();
    m_pTL->UpdatePos();
    m_pBR->UpdatePos();
    m_pBL->UpdatePos();

    // Messages 3/4 - Wait
    // D/U Face processor: Blocking MPI_Wait to get the bead instances from 
    // the D/F face processor.

    m_Msg3.Wait(&receiveRequest3);
    m_Msg3.Wait(&sendRequest3);

    // Now we retrieve the newly-filled bead instances from the message and
	// add them to the D Face's CNT cells. Note that we actually use the complete
	// set of CNT cells not just the Face's cells.

	BeadList lDBeads = m_Msg3.GetRecBeads();

//       std::cout << "Proc " << GetRank() << " about to allocate D Face beads" << lDBeads.size() << zEndl;


    BeadListIterator iterBead;  // Iterator used several times below

	for(iterBead=lDBeads.begin(); iterBead!=lDBeads.end(); iterBead++)
	{
//		std::cout << "Processor " << GetRank() << " assigning D Face bead with id/coords " << (*iterBead)->GetId() << " " << (*iterBead)->GetXPos() << " " << (*iterBead)->GetYPos() << " " << (*iterBead)->GetZPos() << " " << (*iterBead)->GetOldXForce() << " " << (*iterBead)->GetOldYForce() << " " << (*iterBead)->GetOldZForce() << zEndl;

		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

    m_Msg4.Wait(&receiveRequest4);
    m_Msg4.Wait(&sendRequest4);

    // Now we retrieve the newly-filled bead instances from the message and
	// add them to the U Face's CNT cells

	BeadList lUBeads = m_Msg4.GetRecBeads();


//       std::cout << "Proc " << GetRank() << " about to allocate U Face beads" << lUBeads.size() << zEndl;

	for(iterBead=lUBeads.begin(); iterBead!=lUBeads.end(); iterBead++)
	{
//		std::cout << "Processor " << GetRank() << " assigning U Face bead with id/coords " << (*iterBead)->GetId() << " " << (*iterBead)->GetXPos() << " " << (*iterBead)->GetYPos() << " " << (*iterBead)->GetZPos() << (*iterBead)->GetOldXForce() << " " << (*iterBead)->GetOldYForce() << " " << (*iterBead)->GetOldZForce()<< zEndl;

		const long ix = static_cast<long>((*iterBead)->GetXPos()/GetCNTXCellWidth());
		const long iy = static_cast<long>((*iterBead)->GetYPos()/GetCNTYCellWidth());
		const long iz = static_cast<long>((*iterBead)->GetZPos()/GetCNTZCellWidth());		

		m_rvCNTCells[GetCellIndexFromCoords(ix,iy,iz)]->AddBeadtoCell(*iterBead);
	}

	++localBeadTotalTimer;
	if(localBeadTotalTimer == 100)
	{
	    localBeadTotalTimer = 0;
				
		// Calculate the total CM velocity and angular momentum for all PN including P0. The results are stored in the base class.
		
	    const long beadTotal1 = CalculateCMVel();
	    const long beadTotal2 = CalculateAngularMomentum();
		
	    // Now create message instances to pass PN data to P0 for accumulation
	
	    pmProcessorBeadCMVelocity       pmBeadCMV;
	    pmProcessorBeadAngularMomentum  pmBeadAM;

	    pmBeadCMV.ZeroCMVel();
	    pmBeadAM.ZeroAngMom();
		
	    if(IsProcessZero())
	    {		
		    if(beadTotal1 != beadTotal2)
		    {
		        std::cout << "**** Error on P0 - bead totals not equal for CM velocity and Ang Momeuntum calculations ****" << zEndl;
		    }
		
		    // Store P0 data first and then receive results from other PN
		
		    pmBeadCMV.SetMessageData(beadTotal1, m_CMVel[0], m_CMVel[1], m_CMVel[2]);
		    pmBeadAM.SetMessageData(beadTotal2, m_AngMom[0], m_AngMom[1], m_AngMom[2]);

            if(pmBeadCMV.Validate() && pmBeadAM.Validate())
		    {
                 pmBeadCMV.Receive();
                 pmBeadAM.Receive();
			 
			     // Now write out the data for the World
			    double vx = pmBeadCMV.GetWorldXCMVel();
			    double vy = pmBeadCMV.GetWorldYCMVel();
			    double vz = pmBeadCMV.GetWorldZCMVel();
			    double cmag = sqrt(vx*vx +vy*vy + vz*vz);
			 
			    std::cout << "****************" << zEndl;
		        std::cout << "World CM vel is " << pmBeadCMV.GetWorldBeadTotal() << " " << cmag << " " << vx << " " << vy << " " << vz  << zEndl;

				for(long pi=0; pi<GetWorld(); pi++)
				{
				    std::cout << "Processor " << pi << " has bead total and total velocity " << pmBeadCMV.GetBeadTotal(pi) << " " <<  pmBeadCMV.GetXVel(pi) << " " << pmBeadCMV.GetYVel(pi) << " " << pmBeadCMV.GetZVel(pi) << zEndl;					
				}

		    }
		    else
		    {
		        std::cout << "**** Error on P0 - CM vel or Ang Mom validation failed **** " << beadTotal1 << " " << beadTotal2 << zEndl;
		    }
	    }
	    else
	    {
		    // Send owning processor's bead total and CM vel and ang mom to P0

		    if(beadTotal1 != beadTotal2)
		    {
		        std::cout << "**** Error on PN " << GetRank() << " - bead totals not equal for CM velocity and Ang Momeuntum calculations ****" << zEndl;
		    }
				
		    pmBeadCMV.SetMessageData(beadTotal1, m_CMVel[0], m_CMVel[1], m_CMVel[2]);
		    pmBeadAM.SetMessageData(beadTotal2, m_AngMom[0], m_AngMom[1], m_AngMom[2]);

            if(pmBeadCMV.Validate() && pmBeadAM.Validate())
		    {
                 pmBeadCMV.SendP0();
                 pmBeadAM.SendP0();
	        }
		    else
		    {
		        std::cout << "**** Error on PN " << GetRank() << " - CM vel or Ang Mom validation failed **** " << beadTotal1 << " " << beadTotal2 << zEndl;
		    }
	    }
    }
	


     UFaceDifference = (m_Msg3.GetSentBeadTotal()-m_Msg3.GetRecBeadTotal());
	 DFaceDifference = (m_Msg4.GetSentBeadTotal()-m_Msg4.GetRecBeadTotal());
	 netTotal += (UFaceDifference + DFaceDifference);
	
//    std::cout << "Proc " << GetRank() << " has sent-received beads = " << netTotal << zEndl;




	m_BeadCrossingCounter++;
	const long value = m_Msg3.GetSentBeadTotal()-m_Msg4.GetSentBeadTotal();
	m_vBeadCrossing.at(0)  += value;
	m_vBeadCrossing2.at(0) += value*value;

#endif
}

// ****************************************
// PVF to update the forces on beads within the CNT cells in the Border regions
// of the SimBox that may need to communicate with adjacent processors. Which
// Border regions need to communicate with adjacent processors depends on the 
// geometry of the processor Space, which is encoded in the mpsSimBox-derived class.
// Internal Border regions just calculate their forces, External ones have 
// to send/receive messages.
//
// Each Border region type (Face, Edge and Corner) is adjacent to a fixed set of other
// Border regions in neighbouring processors that it has to search to calculate
// all short-ranged non-bonded forces. Each processor "owns" its Border regions
// only in the UTR directions, and it is sent the coordinates of all beads in the 
// adjacent processors' regions that impinge on these UTR Borders. It also sends
// the coordinates of beads in its own DBL Border regions to the appropriate 
// adjacent processors. It then uses the neighbouring processors' coordinates to
// calculate all forces on its own beads, and stores the calculated random forces 
// ready to send them back to the neighbouring processors. Once it has updated 
// the local forces on all of its own beads, and added in the random forces passed
// in from adjacent processors, it sends its own random forces back to the 
// neighbouring processors so that they can update their own bead forces.
//
// In order to optimise the calculations, we apply different algorithms depending
// on the geometry and normal vector of the arrangement of processors.
// 
// Linear: only the Faces in the normal direction need messages
//         only the Edges perpendicular to the normal need messages, and then
//         only in the normal direction
//         all Corners need messages but only in the normal direction
//
// Planar: only the Faces perpendicular to the normal need messages
//         Edges in the normal direction need messages in the transverse directions
//         Edges perpendicular to the normal need messages in the other transverse
//         direction only
//
// Cubic:  all Faces need messages in their own normal direction
//         all Edges need messages in the directions perpendicular to their normal
//         all Corners need messages in all directions
//
//
// For the linear Z SimBox, only the Border regions along the Z axis have to 
// communicate with the two adjacent processorsn.
// 
// Z Face (U)  Edges (UT, UB, UR, UL) and Corners (UTR, UTL, UBR, UBL)
// 
// and the opposite ones: (D), (DT, DB, DR, DL) and (DTR, DTL, DBR, DBL)
// 
// X,Y Faces (R,L,T,B) and Z Edges (TR, TL, BR, BL) are internal.
// 
// All Corners need to communicate with the adjacent processors in Z direction

void mpsLinearZSimBox::UpdateBorderForce()
{
#if SimMPS == SimulationEnabled

//    std::cout  << "Processor " << GetRank() << " has entered mpsLinearZSimBox's UpdateBorderForce with neighbours " << DFacePid << " " << UFacePid << zEndl;

    // Message 1 - Non-blocking MPI_Isend to D Face processor and MPI_Irecv from
    //             U Face processor of pmSendLinearBeadCoords containing 
    //             bead coordinates in our own D Face and adjacent Border regions


    // First empty the external CNT cells before adding new beads to them. 
    // For this linear SimBox we need to clear the U Face and its adjacent Edges and Corners.

    m_pU->ClearExternalFECCells();
    m_pD->ClearExternalFECCells();
    m_pR->ClearExternalFECCells();
    m_pL->ClearExternalFECCells();
    m_pT->ClearExternalFECCells();
    m_pB->ClearExternalFECCells();

    m_pUR->ClearExternalECCells();
    m_pUL->ClearExternalECCells();
    m_pUT->ClearExternalECCells();
    m_pUB->ClearExternalECCells();
    m_pDR->ClearExternalECCells();
    m_pDL->ClearExternalECCells();
    m_pDT->ClearExternalECCells();
    m_pDB->ClearExternalECCells();
    m_pTR->ClearExternalECCells();
    m_pTL->ClearExternalECCells();
    m_pBR->ClearExternalECCells();
    m_pBL->ClearExternalECCells();

	m_pUTR->ClearExternalCells();
	m_pUTL->ClearExternalCells();
	m_pUBR->ClearExternalCells();
	m_pUBL->ClearExternalCells();
	m_pDTR->ClearExternalCells();
	m_pDTL->ClearExternalCells();
	m_pDBR->ClearExternalCells();
	m_pDBL->ClearExternalCells();

    // Collect the bead coordinates to send to D face proc: they are all the beads
    // in the current processor's D face plus its adjacent Edges and Corners. 
    // Note that a linear SimBox only needs to send beads from the current processor 
    // as the simulation Space is periodic in the transverse directions.
    // The message unpacks the bead instances to package their coordinates in
    // the message payload.

    BeadList lub = m_pU->GetFECBeads();
	BeadList ldb = m_pD->GetFECBeads();
				
//	std::cout << "Inside: UpdateBorderForce:  Processor " << GetRank() << " first has U/D FEC  bead totals " << lub.size() << " " << ldb.size() << zEndl;
	
    m_Msg1.SetMessageData(GetFaceEdgeCornerBeads(*m_pD));

    // and start the non-blocking send. Note that the above function retrieves 
    // beads that are in the owning processor's Space not a neighbouring processo.
    // The latter beads are returned by GetDFaceNeighbourBeads().
    // Simultaneously, we use the same message to receive beads from the U Face
    // processor that has itself sent the message to its D Face processor, which is
    // the current processor.

    MPI_Request  sendRequest1;
    MPI_Request  receiveRequest1;

    if(m_Msg1.Validate())
    {
        m_Msg1.ISend(DFacePid, &sendRequest1);
        m_Msg1.IReceive(UFacePid, &receiveRequest1);
    }
	else
	{
	    std::cout << "Msg1 failed first validation" << zEndl;
	}

    // **********
    // Calculate the internal forces between beads in the internal R, L, T, B Faces 
    // and their internal edges while waiting for the adjacent processors to reply.
    // Note that the UpdateForce() function does not depend on inter-processor comms.
    // **********

    m_pR->UpdateForce();
    m_pT->UpdateForce();
    m_pL->UpdateForce();
    m_pB->UpdateForce();

    m_pTR->UpdateForce();
    m_pTL->UpdateForce();
    m_pBR->UpdateForce();
    m_pBL->UpdateForce();

    // Message 1 - Wait
    // D Face processor: Blocking MPI_Wait of pmSendLinearBeadCoords to D Face processor
    // and from U Face processor that complete the non-blocking Send/Receive calls above.

    if(m_Msg1.Validate())
    {
        m_Msg1.Wait(&sendRequest1);
        m_Msg1.Wait(&receiveRequest1);

        // **********
        // Pass the coordinates of the beads to the external CNT Cells that wrap
        // the U Face processor's Border cells. 

        // First we have to translate the bead coordinates into the owning processor's
        // Space. Because the linear SimBox only receives beads from a single adjacent
        // processor, we can treat the beads as belonging to a single Face and its
        // adjacent four Edges and Corners.

        TranslateBeadCoordsInNegativeZ();


        // **********
        // Calculate forces between beads in our U Face and its 
        // adjacent Regions including those in the adjacent processor whose
        // bead coordinates have been received in pMsg1, and store the forces 
        // that need to sent back to the U face processor and received from the
        // D Face processor.
        // Note that the CalculateSharedForce() function DOES depend on other processors.
        //
        // **********

        m_pU->CalculateSharedForce();

        m_pUR->CalculateSharedForce();
        m_pUT->CalculateSharedForce();
        m_pUL->CalculateSharedForce();
        m_pUB->CalculateSharedForce();

        m_pUTR->CalculateSharedForce();
        m_pUTL->CalculateSharedForce();
        m_pUBL->CalculateSharedForce();
        m_pUBR->CalculateSharedForce();
    }
	else
	{
	    std::cout << "Msg1 failed second validation" << zEndl;
	}

    // Message 2 - Non-blocking MPI_Irecv from D Face processor and MPI_Isend 
    //             to U Face of pmSendLinearBeadForces containing calculated
    //             forces to add to adjacent processor's beads



    m_Msg2.SetMessageData(GetFaceEdgeCornerExternalCells(*m_pU));
	
    MPI_Request  sendRequest2;
    MPI_Request  receiveRequest2;

    if(m_Msg2.Validate())
    {
        m_Msg2.ISend(UFacePid, &sendRequest2);
        m_Msg2.IReceive(DFacePid, &receiveRequest2);

        // Message 2 - Wait
        // D Face processor: Blocking MPI_Wait of pmSendLinearBeadForces from D Face processor
        // that completes the non-blocking Send call above.

        m_Msg2.Wait(&sendRequest2);
        m_Msg2.Wait(&receiveRequest2);

        // **********
        // Accumulate the forces on beads in the external CNT Cells that wrap
        // the U Face processor's Border cells. 
		
//	std::cout << "Processor " << GetRank() << "  has sent coord total " << m_Msg1.GetSentBeadTotal()  << " to proc " << DFacePid << " and received coord total " << m_Msg1.GetRecBeadTotal() << " from proc " << UFacePid << zEndl;
//	std::cout << "Processor " << GetRank() << "  has sent force total " << m_Msg2.GetSentForceTotal() << " to proc " << UFacePid << " and received force total " << m_Msg2.GetRecForceTotal() << " from proc " << DFacePid << zEndl;

        TranslateBeadForcesInPositiveZ();

        // **********
        // Add forces to beads in our D Face and adjacent Border regions.
        // **********

        m_pD->UpdateSharedForce();

        m_pDR->UpdateSharedForce();
        m_pDT->UpdateSharedForce();
        m_pDL->UpdateSharedForce();
        m_pDB->UpdateSharedForce();

        m_pDTR->UpdateSharedForce();
        m_pDTL->UpdateSharedForce();
        m_pDBL->UpdateSharedForce();
        m_pDBR->UpdateSharedForce();
    }
	else
	{
	    std::cout << "Msg2 failed validation" << zEndl;
	}

#endif
}

// Private helper function to translate the coordinates of beads received by
// this processor from its neighbour in the +Z direction, and to bin them
// into the appropriate CNT cells for storing in the U Face and its adjacent
// Edges and Corners. For this SimBox we only have to add the Z dimension
// of the SimBox and bin in the X,Y dimensions. This places the beads in the
// layer of external CNT cells to the right of the +Z Face of the owning SimBox.

void mpsLinearZSimBox::TranslateBeadCoordsInNegativeZ()
{
    zLongVector&   rBeadIds   = m_Msg1.GetBeadIds();
    zLongVector&   rBeadTypes = m_Msg1.GetBeadTypes();
    zDoubleVector& rBeadX     = m_Msg1.GetX();
    zDoubleVector& rBeadY     = m_Msg1.GetY();
    zDoubleVector& rBeadZ     = m_Msg1.GetZ();
    zDoubleVector& rBeadVX    = m_Msg1.GetVX();
    zDoubleVector& rBeadVY    = m_Msg1.GetVY();
    zDoubleVector& rBeadVZ    = m_Msg1.GetVZ();

    // Shift the beads into the slice of external cells adjacent to the 
    // owning processor's +Z Face and then bin them into the Face's CNT cells 
    // and its adjacent Edges and Corners using their x,y coordinates to 
    // determine which cell they belong to. Note that it is only for this case
    // that the two coordinates in the plane of the Face are equal to x and y.

    // For this linear SimBox, the index identifying the adjacent external cell
    // is always the same.

    const long index = GetPidIndexFromCoordinateDifferences(0,0,1);

    const long total = m_Msg1.GetRecBeadTotal();
	
//	std::cout << "Proc " << GetRank() << " is translating " << total << " bead coords" << zEndl;

    for(long i=0; i<total; i++)
    {
        double x = rBeadX.at(i);
        double y = rBeadY.at(i);
        double z = rBeadZ.at(i);

        z +=GetSimBoxZLength();
        rBeadZ.at(i) = z;

        const long ix = static_cast<long>(x/GetCNTXCellWidth());
        const long iy = static_cast<long>(y/GetCNTYCellWidth());
//        const long iz = GetCNTZCellNo()-1;

        m_pU->AddBeadToFECZCell(index, ix, iy, rBeadIds.at(i), rBeadTypes.at(i), 
                              rBeadX.at(i),   rBeadY.at(i), rBeadZ.at(i),
                              rBeadVX.at(i),  rBeadVY.at(i), rBeadVZ. at(i));
    }
}

// Function to collect the forces on all beads in the external cells of this processor's
// D Face. First we get the container of cells, then we extract the
// forces on all beads in the cells in the order that they occur. This means that the
// order of the bead forces is the same as that of the original beads in the cells.
//
// The name of this function is a misnomer; it is chosen so that it matches the
// TranslateBeadCoordsInNegativeZ function above.

void mpsLinearZSimBox::TranslateBeadForcesInPositiveZ()
{
//    const long total = m_Msg2.GetRecForceTotal();
//    std::cout << "Proc " << GetRank() << " is accumulating " << total << " forces" << zEndl;

    BeadList       lBeads = GetFaceEdgeCornerBeads(*m_pD);
    zLongVector&   rIds   = m_Msg2.GetBeadIds();
    zDoubleVector& rFX    = m_Msg2.GetFX();
    zDoubleVector& rFY    = m_Msg2.GetFY();
    zDoubleVector& rFZ    = m_Msg2.GetFZ();

    AddExternalForcesToBeads(lBeads, rIds, rFX, rFY, rFZ);
}
