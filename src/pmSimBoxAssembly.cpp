/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSimBoxAssembly.cpp: implementation of the pmSimBoxAssembly class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSimBoxAssembly.h"
#include "LogpmSimBoxAssemblyFailed.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSimBoxAssembly::m_Type = "SimBoxAssembly";

const zString pmSimBoxAssembly::GetType()
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
	mpmMessage* Create() {return new pmSimBoxAssembly();}

	const zString id = pmSimBoxAssembly::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmSimBoxAssembly::pmSimBoxAssembly() : mpmMessage(), m_bFailed(false)
{
}

pmSimBoxAssembly::pmSimBoxAssembly(const pmSimBoxAssembly& oldMessage) : mpmMessage(oldMessage),
                                                                         m_bFailed(oldMessage.m_bFailed)
{
}

pmSimBoxAssembly::~pmSimBoxAssembly()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSimBoxAssembly::put(zOutStream& os) const
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

zInStream& pmSimBoxAssembly::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSimBoxAssembly::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSimBoxAssembly::GetMessage() const
{
	return new pmSimBoxAssembly(*this);
}

// Function to store a flag showing if the parallel SimBox assembly failed or not.

void pmSimBoxAssembly::SetAssemblyFailed(bool bFailed)
{
    m_bFailed = bFailed;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We just check that the senderId and the neighbour pids are within the valid ranges.

bool pmSimBoxAssembly::Validate()
{
    bool bSuccess = true;

    if(m_SenderRank < 0 || m_SenderRank >= GetWorld())
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for MPI_Send() sent to Processor 0. In this class, each PN sends a
// message to P0 containing its rank and a flag showing if its parallel SimBox
// assembled correctly.
//
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmSimBoxAssembly::SendP0()
{
#if SimMPS == SimulationEnabled

    if(!IsProcessZero())
    {
        long dest = 0;   // Local variables used by MPI_Send, MPI_Recv
        long tag  = 1;

		dest = 0;
		
        long data[2];
        data[0] = m_SenderRank;

        if(GetAssemblyFailed())
        {
            data[1] = 1;
        }
        else
        {
            data[1] = 0;
        }

        MPI_Send(&data, 2, MPI_LONG, dest, tag, MPI_COMM_WORLD);
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

// This function is called by  P0 to receive the nearest neighbour data from PN.

void pmSimBoxAssembly::Receive()
{
#if SimMPS == SimulationEnabled

    // Only P0 should respond to this message, and it writes the data to the logfile.

    if(IsProcessZero())
    {
        long tag    = 1; // Local variables used by MPI_Send, MPI_Recv
        MPI_Status status;
        long data[2];

        long currentTime = 0;  // We are still at time zero here

// Collect data from P0 and all PN in turn
				
        for(long source = 1; source < GetWorld() ; source++)
		{
			MPI_Recv(&data, 2, MPI_LONG, source, tag, MPI_COMM_WORLD, &status);

			m_SenderRank = data[0];

            if(data[1] == 1)
            {
                m_bFailed = true;

			    new CLogpmSimBoxAssemblyFailed(currentTime, m_SenderRank);
            }
		
		}
    }

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmSimBoxAssembly::Receive(long procId)
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

void pmSimBoxAssembly::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}

