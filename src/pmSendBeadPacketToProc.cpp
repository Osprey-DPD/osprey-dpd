/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSendBeadPacketToProc.cpp: implementation of the pmSendBeadPacketToProc class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSendBeadPacketToProc.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSendBeadPacketToProc::m_Type = "SendBeadPacketToProc";

const zString pmSendBeadPacketToProc::GetType()
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
	mpmMessage* Create() {return new pmSendBeadPacketToProc();}

	const zString id = pmSendBeadPacketToProc::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.
// Note that we store invalid values here so that the caller has to replace them
// with valid values otherwise the Validate() function will trap the error.

pmSendBeadPacketToProc::pmSendBeadPacketToProc() : mpmMessage(), m_SenderId(-1)
{
}

pmSendBeadPacketToProc::pmSendBeadPacketToProc(const pmSendBeadPacketToProc& oldMessage) : mpmMessage(oldMessage),
                                             m_SenderId(oldMessage.m_SenderId)
{
}

pmSendBeadPacketToProc::~pmSendBeadPacketToProc()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSendBeadPacketToProc::put(zOutStream& os) const
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

zInStream& pmSendBeadPacketToProc::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSendBeadPacketToProc::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSendBeadPacketToProc::GetMessage() const
{
	return new pmSendBeadPacketToProc(*this);
}

// Function to store the procId of the processor that creates this message.

void pmSendBeadPacketToProc::SetSenderId(long pid)
{
	if(pid >= 0 && pid < GetWorld())
	{
		m_SenderId = pid;
	}
	else
	{
		m_SenderId = -1; // error value will be trapped by Validate() 
	}
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We just check that the senderId and the neighbour pids are within the valid ranges.

bool pmSendBeadPacketToProc::Validate()
{
    bool bSuccess = true;

    if(m_SenderId < 0 || m_SenderId >= GetWorld())
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for MPI_Send() sent to processor procId. In this class, each PN sends a
// message to P0 containing a list of its nearest neighbours.
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmSendBeadPacketToProc::Send(long procId)
{
#if SimMPS == SimulationEnabled

//    if(!IsProcessZero() && procId == 0)
 //   {
//        long dest = 0;   // Local variables used by MPI_Send, MPI_Recv
//        long tag  = 1;

//		dest = procId;
		
        // First, we have to fill the m_NNProcId array with the ids of our
        // nearest neighbours. But note that this array should include the id
        // of the processor itself as well as its neighbours.

//        long data[28];
//        data[0] = m_SenderId;
//        for(long i=1; i<28; i++)
//        {
//            data[i] = m_NNProcId[i-1];
//        }

//        MPI_Send(&data, 28, MPI_LONG, dest, tag, MPI_COMM_WORLD);
//	}
//    else
//    {
		// This branch should not be seen	
//		std::cout << "Error: processor " << m_ProcId << " trying to call Send() " << zEndl;
//    }

#endif
}

// Wrapper for the Mpi function:
// MPI_Recv(message, length, MPI_CHAR, source, tag, MPI_COMM_WORLD, status);

// This function is called by  P0 to receive the nearest neighbour data from PN.

void pmSendBeadPacketToProc::Receive()
{
#if SimMPS == SimulationEnabled
/*
    // Only P0 should respond to this message, and it writes the data to the logfile.

//    if(IsProcessZero())
//    {
//        long tag    = 1; // Local variables used by MPI_Send, MPI_Recv
//        MPI_Status status;
//        long data[28];
//		for(long j=0; j<28; j++)
//		{
//			data[j] = 0;
//		}

// Collect data from P0 and all PN in turn

//		long currentTime = IGlobalSimBox::Instance()->GetCurrentTime();
//		long currentTime = 0;
				
//        for(long source = 0; source < GetWorld() ; source++)
//		{
//		    if(source > 0)
//			{
//				MPI_Recv(&data, 28, MPI_LONG, source, tag, MPI_COMM_WORLD, &status);

				// Unpack the data into the sending processor's own id and those
				// of its nearest neighbours.

//				m_SenderId = data[0];

//				for(long i=0; i<27; i++)
//				{
//					m_NNProcId[i] = data[i+1];
//				}
			}
		
//			CLogpmSimBoxNeighbourList* pMsg1 = new CLogpmSimBoxNeighbourList(currentTime, m_SenderId, m_NNProcId);
//		}
//    }
*/
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmSendBeadPacketToProc::Receive(long procId)
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

void pmSendBeadPacketToProc::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}

