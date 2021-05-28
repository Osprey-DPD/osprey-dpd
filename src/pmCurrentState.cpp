/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmCurrentState.cpp: implementation of the pmCurrentState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "pmCurrentState.h"
#include "CurrentState.h"
#include "Bead.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmCurrentState::m_Type = "CurrentState";


const zString pmCurrentState::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// message class but a global function hidden in the namespace.

namespace
{
	mpmMessage* Create() {return new pmCurrentState();}

	const zString id = pmCurrentState::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// This message class is used to send the bead coordinates from PN to P0.
// The maximum number of beads per message is defined by the constant
// pmCurrentStateMAXBEADS. The data required for each bead is:
//
// Bead Id      - long
// Bead Type    - long
// Bead radius  - double
// Bead X coord - double
// Bead Y coord - double
// Bead Z coord - double
//
//

pmCurrentState::pmCurrentState() : mpmMessage(), m_pcState(0), m_Counter(0)
{
    // Ensure all arrays are zero.

    for(long i=0; i<pmCurrentStateMAXBEADS; i++)
    {
        m_Id[i]       = 0;
        m_BeadType[i] = 0;   // We have to avoid a naming conflict with the message type
        m_Radius[i]   = 0.0;
        m_X[i]        = 0.0;
        m_Y[i]        = 0.0;
        m_Z[i]        = 0.0;
    }
}

pmCurrentState::pmCurrentState(const pmCurrentState& oldMessage) : mpmMessage(oldMessage),
                               m_pcState(oldMessage.m_pcState),
                               m_Counter(oldMessage.m_Counter)
{
    for(long i=0; i<pmCurrentStateMAXBEADS; i++)
    {
        m_Id[i]       = oldMessage.m_Id[i];
        m_BeadType[i] = oldMessage.m_BeadType[i];
        m_Radius[i]   = oldMessage.m_Radius[i];
        m_X[i]        = oldMessage.m_X[i];
        m_Y[i]        = oldMessage.m_Y[i];
        m_Z[i]        = oldMessage.m_Z[i];
    }
}

pmCurrentState::~pmCurrentState()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmCurrentState::put(zOutStream& os) const
{
#if EnableXMLParallelMessagess == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<MessageId>"    << GetId()   << "</MessageId>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLParallelMessagess == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << GetId();
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& pmCurrentState::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmCurrentState::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmCurrentState::GetMessage() const
{
	return new pmCurrentState(*this);
}

// ****************************************
// Function to store a pointer to a CCurrentState instance owned by P0
// to which bead data will be passed for serialization. This function
// must NOT be called by any PN.

void pmCurrentState::SetCurrentStatePointer(CCurrentState* const pcState)
{
    m_pcState = pcState;
}

// Function to add a bead's data to the message. We only add a bead if it
// satisfies the following conditions:
//
// Bead display conditions
// ***********************
//
// Bead visibility flag is true
// 
// This avoids sending beads to P0 that are not going to be written to the
// current state snapshot file anyway.
//
// Note the zero-indexing of the arrays: the m_Counter index is set to zero
// in the constructor. We don't allow more beads to be added than the
// maximum to avoid array bounds errors.

void pmCurrentState::AddBead(const CAbstractBead* const pBead)
{
    // Note that we pass the bead's "display id" not its actual id.
    // And we shift the bead coordinates into the whole simulation Space
    // from each processor's local coordinate system.

    if(m_Counter < pmCurrentStateMAXBEADS && pBead->GetVisible())
    {
        m_Id[m_Counter]       = CCurrentState::GetBeadDisplayId(pBead->GetId());
        m_BeadType[m_Counter] = pBead->GetType();
        m_Radius[m_Counter]   = pBead->GetRadius();
        m_X[m_Counter]        = pBead->GetShiftedXPos();
        m_Y[m_Counter]        = pBead->GetShiftedYPos();
        m_Z[m_Counter]        = pBead->GetShiftedZPos();

        m_Counter++;
    }
}

// Overloaded function to add a bead's data directly: it is missing the
// visibility flag and the bead radius.

void pmCurrentState::AddBead(long id, long type, double x, double y, double z)
{
    if(m_Counter < pmCurrentStateMAXBEADS)
    {
        m_Id[m_Counter]       = CCurrentState::GetBeadDisplayId(id);
        m_BeadType[m_Counter] = type;
        m_X[m_Counter]        = x;
        m_Y[m_Counter]        = y;
        m_Z[m_Counter]        = z;

        m_Counter++;
    }
}

// Function to extract a bead's data from the message. The current value of 
// the m_Counter index determines which bead is extracted.

void pmCurrentState::GetBead(long *pid, long *pType, double *pX, double *pY, double *pZ)
{
    if(m_Counter >= 0)
    {
        *pid   = m_Id[m_Counter];
        *pType = m_BeadType[m_Counter];
        *pX    = m_X[m_Counter];
        *pY    = m_Y[m_Counter];
        *pZ    = m_Z[m_Counter];

        --m_Counter;
    }
}

// Overloaded function to add a bead's data directly: it is missing the
// visibility flag.

void pmCurrentState::AddBead(long id, long type, double radius, double x, double y, double z)
{
    if(m_Counter < pmCurrentStateMAXBEADS)
    {
        m_Id[m_Counter]       = CCurrentState::GetBeadDisplayId(id);
        m_BeadType[m_Counter] = type;
        m_Radius[m_Counter]   = radius;
        m_X[m_Counter]        = x;
        m_Y[m_Counter]        = y;
        m_Z[m_Counter]        = z;

        m_Counter++;
    }
}

// Function to extract a bead's data from the message. The current value of 
// the m_Counter index determines which bead is extracted.

void pmCurrentState::GetBead(long *pid, long *pType, double *pRadius, double *pX, double *pY, double *pZ)
{
    if(m_Counter >= 0)
    {
        *pid     = m_Id[m_Counter];
        *pType   = m_BeadType[m_Counter];
        *pRadius = m_Radius[m_Counter];
        *pX      = m_X[m_Counter];
        *pY      = m_Y[m_Counter];
        *pZ      = m_Z[m_Counter];

        --m_Counter;
    }
}


// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We check that the total number of beads is within the allowed range, and 
// that the bead display id and type are non-negative, and that the coordinates 
// are greater than zero for all beads. We do NOT validate the current state
// pointer as this is only used on P0, and is not set before passing bead data
// from PN to P0.

bool pmCurrentState::Validate()
{
    bool bSuccess = true;

    if(m_Counter < 0 || m_Counter > pmCurrentStateMAXBEADS)
    {
        bSuccess = false;
    }
    else
    {
        long i = 0;
        while(bSuccess && i < m_Counter)
        {
            if(m_Id[i] < 0  || m_BeadType[i] < 0 || m_Radius[i] <= 0.0 ||
               m_X[i] < 0.0 || m_Y[i] < 0.0 || m_Z[i] < 0.0)
            {
                bSuccess = false;
            }
            i++;
        }

    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmCurrentState::SendP0()
{
#if SimMPS == SimulationEnabled

    if(!IsProcessZero())
    {
        MPI_Datatype  MsgDataType;

        BuildDerivedType(&MsgDataType);

        long dest = 0;   // Local variables used by MPI_Send, MPI_Recv
		
	    // First send the number of beads whose coordinates are to be written
		
        long tag  = 0;
		MPI_Send(&m_Counter, 1, MPI_LONG, dest, tag, MPI_COMM_WORLD);
		
		// Now send the bead data
		
		tag = 1;
        MPI_Send(&m_Id[0], 1, MsgDataType, dest, tag, MPI_COMM_WORLD);
    }
    else
    {
		// This branch should not be seen	
		std::cout << "Error: Processor 0 trying to call SendP0() " << zEndl;
    }

#endif
}

// Wrapper for the Mpi function:
// MPI_Recv(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

// We retrieve the data from the message and fill up this instance's member variables. 
// We do not check for validity here as we assume that the sending object has 
// already done it. Note that this function does not propagate the data out into 
// the code, it only fills up the receiving message instance. Each messaging class 
// is responsible for providing further functions to pass the data to the rest of the code.

void pmCurrentState::Receive()
{
#if SimMPS == SimulationEnabled

    // Only P0 should respond to this message, and it writes the data to 
    // its current state instance.

    if(IsProcessZero())
    {
        int tag = 0;
        MPI_Status status;

// Collect data from P0 and all PN in turn
						
        for(long source = 1; source < GetWorld() ; source++)
		{			
			// First get the number of beads
			
			tag = 0;
			MPI_Recv(&m_Counter, 1, MPI_LONG, source, tag, MPI_COMM_WORLD, &status);
			
			// and define the derived type using the number of data items
						
            MPI_Datatype  MsgDataType;

            BuildDerivedType(&MsgDataType);
			
			// Now receive the bead data
			
			tag = 1;
            MPI_Recv(&m_Id[0], 1, MsgDataType, source, tag, MPI_COMM_WORLD, &status);
			
            // Pass the bead coordinates to the current state instance - checking
            // that it exists first
		
            if(m_pcState && !m_pcState->SerializePN(m_Counter, m_Id, m_BeadType, m_Radius, m_X, m_Y, m_Z))
            {
                ErrorTrace("Error writing PN data in CMonitor::SaveCurrentStateP");
            }
		}
    }

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmCurrentState::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmCurrentState::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled
		
    // Store the number of elements of each type in the blocks: for simplicity,
    // we divide the data into 6 groups: ids, types, radii and X, Y, Z coordinates.
    // The type of the elements in the first two groups is long,
    // and the last four are doubles. There are m_Counter elements in each
    // of these groups. 

    // Build the derived datatype and commit it

    int block_lengths[6];

    for(long i = 0; i < 6; i++)
    {
        block_lengths[i] = m_Counter;  
    }

    MPI_Datatype typelist[6];

    typelist[0] = MPI_LONG;
    typelist[1] = MPI_LONG;
    typelist[2] = MPI_DOUBLE;
    typelist[3] = MPI_DOUBLE;
    typelist[4] = MPI_DOUBLE;
    typelist[5] = MPI_DOUBLE;

    MPI_Aint displacements[6];
    
    // Local variables used to calculate the start of each element

    MPI_Aint startAddress;
    MPI_Aint address;

    MPI_Get_address(&m_Id[0], &startAddress);
    displacements[0] = 0;

    MPI_Get_address(&m_BeadType[0], &address);
	displacements[1] = address - startAddress;

    MPI_Get_address(&m_Radius[0], &address);
	displacements[2] = address - startAddress;

    MPI_Get_address(&m_X[0], &address);
	displacements[3] = address - startAddress;

    MPI_Get_address(&m_Y[0], &address);
	displacements[4] = address - startAddress;

    MPI_Get_address(&m_Z[0], &address);
	displacements[5] = address - startAddress;

    MPI_Type_create_struct(6, block_lengths, displacements, typelist, pMsgDataType);
    MPI_Type_commit(pMsgDataType);

#endif
}

