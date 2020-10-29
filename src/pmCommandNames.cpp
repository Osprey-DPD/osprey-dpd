/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmCommandNames.cpp: implementation of the pmCommandNames class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmCommandNames.h"
#include "InputData.h"
#include "xxCommand.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmCommandNames::m_Type = "CommandNames";

const zString pmCommandNames::GetType()
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
	mpmMessage* Create() {return new pmCommandNames();}

	const zString id = pmCommandNames::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store the names of all commands that are scheduled for execution in
// this message so that P0 can send them to PN preparatory to executing them
// at the appropriate times.

pmCommandNames::pmCommandNames() : mpmMessage()
{
    m_Names.clear();
}

pmCommandNames::pmCommandNames(const pmCommandNames& oldMessage) : mpmMessage(oldMessage)
{
    m_Names.clear();

    for(cStringSequenceIterator is=oldMessage.m_Names.begin(); is!=oldMessage.m_Names.end(); is++)
    {
        m_Names.push_back(*is);
    }
}

pmCommandNames::~pmCommandNames()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmCommandNames::put(zOutStream& os) const
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

zInStream& pmCommandNames::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmCommandNames::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmCommandNames::GetMessage() const
{
	return new pmCommandNames(*this);
}

// ****************************************
// Function to copy the names of commands that are scheduled for execution
// from P0 to PN so that they can be ready to receive the command data at 
// the appropriate times.

void pmCommandNames::SetMessageData(const CInputData *const pData)
{
    for(cCommandIterator ic=pData->GetCommands().begin(); ic!=pData->GetCommands().end(); ic++)
    {
        const zString name = (*ic)->GetCommandType();
        m_Names.push_back(name);    
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// As we allow an empty vector of names, there is nothing to validate here.

bool pmCommandNames::Validate()
{
    bool bSuccess = true;

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmCommandNames::SendAllP()
{
#if SimMPS == SimulationEnabled
    // Because we don't know how many commands are defined, and hence how
    // many characters we need to send, we use MPI_Pack/MPI_Unpack instead of 
    // building our own datatype.

    // We allow for up to 20000 characters for now: this is approximately
    // 400 x 50 characters commands.

    char buffer[20000];
    int  position;
    position = 0;

    long total = m_Names.size();

    MPI_Pack(&total, 1, MPI_LONG, buffer, 20000, &position, MPI_COMM_WORLD);

    // Now iterate over each command name and pack the number of characters
    // and the name itself into the message one by one.

    char cname[50];

    for(cStringSequenceIterator is=m_Names.begin(); is!=m_Names.end(); is++)
    {
        const zString name = *is;
        strcpy(cname, name.c_str());
        long length = name.size()+1;

        MPI_Pack(&length, 1, MPI_LONG, buffer, 20000, &position, MPI_COMM_WORLD);
        MPI_Pack(&cname, length, MPI_CHAR, buffer, 20000, &position, MPI_COMM_WORLD);
    }

    // and send it to all the other processors: note that we assume that
    // the sending processor is P0, so we start the loop at processor rank 1.

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(buffer, 20000, MPI_PACKED, m_ProcId, 0, MPI_COMM_WORLD);
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

void pmCommandNames::Receive()
{
#if SimMPS == SimulationEnabled

    char buffer[20000];
    int  position;

    long total;
    long length = 0;
    char cname[100];
    zString name;

    int tag = 0;
    MPI_Status status;

    MPI_Recv(buffer, 20000, MPI_PACKED, 0, tag, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 20000, &position, &total, 1, MPI_LONG, MPI_COMM_WORLD);

    for(long i=0; i<total; i++)
    {
        MPI_Unpack(buffer, 20000, &position, &length, 1, MPI_LONG, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 20000, &position, &cname, length, MPI_CHAR, MPI_COMM_WORLD);

        name = cname;
        m_Names.push_back(name);
    }

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmCommandNames::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmCommandNames::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled


#endif
}
