/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmProcessorBeadTotal.cpp: implementation of the pmProcessorBeadTotal class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimNumericFlags.h"
#include "pmProcessorBeadTotal.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmProcessorBeadTotal::m_Type = "ProcessorBeadTotal";

const zString pmProcessorBeadTotal::GetType()
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
	mpmMessage* Create() {return new pmProcessorBeadTotal();}

	const zString id = pmProcessorBeadTotal::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmProcessorBeadTotal::pmProcessorBeadTotal() : mpmMessage(), m_BeadTotal(0)
{
	m_vBeadTotal.clear();
}

pmProcessorBeadTotal::pmProcessorBeadTotal(const pmProcessorBeadTotal& oldMessage) : mpmMessage(oldMessage),
                                           m_BeadTotal(oldMessage.m_BeadTotal)
{
	m_vBeadTotal = oldMessage.m_vBeadTotal;
}

pmProcessorBeadTotal::~pmProcessorBeadTotal()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmProcessorBeadTotal::put(zOutStream& os) const
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

zInStream& pmProcessorBeadTotal::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmProcessorBeadTotal::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmProcessorBeadTotal::GetMessage() const
{
	return new pmProcessorBeadTotal(*this);
}

// Function to zero the total bead counter. This is used by P0 to start a new round of
// data collection on each time step.

void pmProcessorBeadTotal::ZeroBeadTotal()
{
	m_vBeadTotal.clear();
}

// Function to set the message data. In this case, it is only the number of beads
// in the owning processor's Space. Note that this includes both the bulk CNT cells
// and all Border cells.

void pmProcessorBeadTotal::SetMessageData(long beadTotal)
{
	m_BeadTotal = beadTotal;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We just check that the senderId and the neighbour pids are within the valid ranges.

bool pmProcessorBeadTotal::Validate()
{
    bool bSuccess = true;

    if(m_SenderRank < 0 || m_SenderRank >= GetWorld() || m_BeadTotal < 0)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for MPI_Send() sent to Processor 0. In this class, each PN sends a
// message to P0 containing the number of beads in its Space.
//
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmProcessorBeadTotal::SendP0()
{
#if SimMPS == SimulationEnabled

    if(!IsProcessZero())
    {
        long dest = 0;   // Local variables used by MPI_Send, MPI_Recv
        long tag  = 1;

		dest = 0;
		
        MPI_Send(&m_BeadTotal, 1, MPI_LONG, dest, tag, MPI_COMM_WORLD);
	}
    else
    {
		// This branch should not be seen	
		std::cout << "Error: Processor 0 trying to call SendP0() " << zEndl;
    }

#endif
}

// Wrapper for the Mpi function:
// MPI_Recv(message, length, MPI_CHAR, source, tag, MPI_COMM_WORLD, status);

// This function is called by  P0 to receive the bead total from PN.

void pmProcessorBeadTotal::Receive()
{
#if SimMPS == SimulationEnabled

    // Only P0 should respond to this message.

    if(IsProcessZero())
    {
		// Clear the containers and store P0 data first - this assumes that P0 has
		// already stored its own data in this object.

		ZeroBeadTotal();
		m_vBeadTotal.push_back(m_BeadTotal);

// Collect data from all PN in turn: this message is only received by P0 which
// knows its own bead total without a message.
				
        long tag = 1; // Local variables used by MPI_Send, MPI_Recv
        MPI_Status status;

        for(long source = 1; source < GetWorld() ; source++)
		{
			MPI_Recv(&m_BeadTotal, 1, MPI_LONG, source, tag, MPI_COMM_WORLD, &status);
			m_vBeadTotal.push_back(m_BeadTotal);
		}
    }

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmProcessorBeadTotal::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

// Because this message only consists of a single long, we don't create a new
// type but use the MPI_LONG type instead. Hence, this is a do-nothing function.

void pmProcessorBeadTotal::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}

// Accessor functions for P0 to get the data passed from all the PN.

long pmProcessorBeadTotal::GetBeadTotal(long pn) const
{
	if(pn >= 0 && pn < GetWorld())
	{
		return m_vBeadTotal.at(pn);
	}
	else
	{
		return 0;
	}
}

long pmProcessorBeadTotal::GetWorldBeadTotal() const
{
	return accumulate(m_vBeadTotal.begin(), m_vBeadTotal.end(), 0);
}
