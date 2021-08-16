/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSimBoxNeighbourResponse.cpp: implementation of the pmSimBoxNeighbourResponse class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSimBoxNeighbourResponse.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSimBoxNeighbourResponse::m_Type = "SimBoxNeighbourResponse";

const zString pmSimBoxNeighbourResponse::GetType()
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
	mpmMessage* Create() {return new pmSimBoxNeighbourResponse();}

	const zString id = pmSimBoxNeighbourResponse::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmSimBoxNeighbourResponse::pmSimBoxNeighbourResponse() : mpmMessage(), m_SenderId(0)
{
    for(long i = 0; i < 27; i++)
    {
        m_NNProcId[i] = 0;
    }
}

pmSimBoxNeighbourResponse::pmSimBoxNeighbourResponse(const pmSimBoxNeighbourResponse& oldMessage) : mpmMessage(oldMessage),
                                             m_SenderId(oldMessage.m_SenderId)
{
    for(long i = 0; i < 27; i++)
    {
        m_NNProcId[i] = oldMessage.m_NNProcId[i];
    }
}

pmSimBoxNeighbourResponse::~pmSimBoxNeighbourResponse()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSimBoxNeighbourResponse::put(zOutStream& os) const
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

zInStream& pmSimBoxNeighbourResponse::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSimBoxNeighbourResponse::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSimBoxNeighbourResponse::GetMessage() const
{
	return new pmSimBoxNeighbourResponse(*this);
}

// ****************************************
// Function to copy all the required data from the CInputData object
// into the message.
// The only data this message carries is the id of the sending processor.

void pmSimBoxNeighbourResponse::SetMessageData(long senderId)
{
    m_SenderId = senderId;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmSimBoxNeighbourResponse::Validate()
{
    bool bSuccess = true;

    if(m_SenderId < 0 || m_SenderId >= GetWorld())
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for MPI_Send() sent to a specific processor.
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmSimBoxNeighbourResponse::Send(long procId)
{
    // Send a message to P0 with the sender id and a list of its 
    // nearest neighbour ids. This makes 27 longs.

#if SimMPS == SimulationEnabled

    if(IsProcessZero())
    {
        Receive();
    }
    else
    {
        long dest = 0;   // Local variables used by MPI_Send, MPI_Recv
        long tag  = 0;
 
        long data[27];
        data[0] = m_SenderId;
        for(long i=1; i<27; i++)
        {
            data[i] = m_NNProcId[i-1];
        }

        MPI_Send(&data, 27, MPI_LONG, dest, tag, MPI_COMM_WORLD);
    }


#elif SimStubMPS == SimulationEnabled
    std::cout << "Leaving Send on proc " << GetRank() << zEndl;
#endif

}

// Wrapper for the Mpi function:
// MPI_Recv(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

// Function called when P0 receives a message from a processor that contains
// a list of all of its neighbouring processors' ids plus its own.

void pmSimBoxNeighbourResponse::Receive()
{
#if SimMPS == SimulationEnabled

    // Only P0 should respond to this message, and it writes the data to the logfile.

    if(IsProcessZero())
    {
        long source = 0;   // Local variables used by MPI_Send, MPI_Recv
        long tag    = 0;
        MPI_Status status;
        long data[27];

        MPI_Recv(data, 27, MPI_LONG, source, tag, MPI_COMM_WORLD, &status);

        m_SenderId = data[0];

        for(long i=0; i<26; i++)
        {
            m_NNProcId[i] = data[i+1];
        }



    }

#elif SimStubMPS == SimulationEnabled
    std::cout << "Leaving Receive on proc " << GetRank() << zEndl;
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.
void pmSimBoxNeighbourResponse::Receive(long procId)
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

void pmSimBoxNeighbourResponse::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}

// Function to return the processor id of one of the sender's nearest neighbours.
// If the argument is out of range, we return the sender's id.

long pmSimBoxNeighbourResponse::GetNNProcId(long i) const
{
    if(i >= 0 && i < 27)
    {
        return m_NNProcId[i];
    }
    else
    {
        return GetSenderId();
    }
}
