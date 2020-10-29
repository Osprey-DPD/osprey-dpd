/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmString.cpp: implementation of the pmString class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "pmString.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 
// Because "String" is too generic, we prefix the type identifier with "pm".

const zString pmString::m_Type = "pmString";

const zString pmString::GetType()
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
	mpmMessage* Create() {return new pmString();}

	const zString id = pmString::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pmString::pmString() : mpmMessage(), m_Value("")
{
}

pmString::pmString(const pmString& oldMessage) : mpmMessage(oldMessage),
                                                 m_Value(oldMessage.m_Value)
{

}

pmString::~pmString()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmString::put(zOutStream& os) const
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

zInStream& pmString::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmString::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmString::GetMessage() const
{
	return new pmString(*this);
}

// ****************************************
// Function to copy all the required data from the CInputData object
// into the message. 

void pmString::SetMessageData(const zString value)
{
    m_Value = value;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmString::Validate()
{
    bool bSuccess = true;

    if(m_Value.empty())
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmString::SendAllP()
{
#if SimMPS == SimulationEnabled

	m_SenderRank = GetRank();

    // Because we don't know the length of the string, we have to set a fixed
    // upper bound.

    char buffer[1000];
    int  position;

    char name[100];
    strcpy(name, m_Value.c_str());
    long nameLength = m_Value.size()+1;

    position = 0;
    MPI_Pack(&nameLength,   1, MPI_LONG, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&name, nameLength, MPI_CHAR, buffer, 1000, &position, MPI_COMM_WORLD);

    // Send the payload from the owning processor to all others but not itself.

    for(m_ProcId=0; m_ProcId<GetWorld(); m_ProcId++)
    {
        if(m_ProcId != m_SenderRank)
        {
            MPI_Send(buffer, 1000, MPI_PACKED, m_ProcId, m_Tag, MPI_COMM_WORLD);
        }
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

void pmString::Receive()
{
#if SimMPS == SimulationEnabled

    char buffer[1000];
    int position;

    char name[100];
    long nameLength = 0;

    MPI_Status status;
    
	m_ReceiverRank = GetRank();

	// The rank of the sending processor is obtained from the status parameter

    MPI_Recv(buffer, 1000, MPI_PACKED, MPI_ANY_SOURCE, m_Tag, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 1000, &position, &nameLength,    1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &name, nameLength, MPI_CHAR, MPI_COMM_WORLD);

    m_Value = name;

	m_SenderRank = status.MPI_SOURCE;
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmString::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmString::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled

#endif
}
