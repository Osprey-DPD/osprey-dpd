/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmISRandom.cpp: implementation of the pmISRandom class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmISRandom.h"
#include "InitialStateRandom.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 
// Note that this idenfier must be the same as the one that appears in the
// CDF or the message will not be created.

const zString pmISRandom::m_Type = "random";

const zString pmISRandom::GetType()
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
	mpmMessage* Create() {return new pmISRandom();}

	const zString id = pmISRandom::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store the parameters needed to define the specified initial state here.
// A random state actually needs no parameters, but we pass a constant string
// just to allow the message to have a payload. Note that we have to convert the
// zString into a c-style string for MPI.

pmISRandom::pmISRandom() : mpmInitialState("random")
{
}

pmISRandom::pmISRandom(const pmISRandom& oldMessage) : mpmInitialState(oldMessage)
{
}

pmISRandom::~pmISRandom()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmISRandom::put(zOutStream& os) const
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

zInStream& pmISRandom::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmISRandom::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmISRandom::GetMessage() const
{
	return new pmISRandom(*this);
}

// ****************************************
// Function to copy all the required data from the CInitialStateData-derived object
// into the message. Note that each concrete message class knows the type of
// ISD that it expects, so it can perform the dynamic_cast safely.

void pmISRandom::SetMessageData(const CInitialStateData* const pISD)
{
    // No data required for the random initial state
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// This IS has no data, so we just return true.

bool pmISRandom::Validate()
{
    bool bSuccess = true;

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmISRandom::SendAllP()
{
#if SimMPS == SimulationEnabled

    char stringName[100];
    strcpy(stringName, GetISType().c_str());
    long nameLength = GetISType().size()+1;

    // and send it to all the other processors: note that we assume that
    // the sending processor is P0, so we start the loop at processor rank 1.

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(stringName, nameLength, MPI_CHAR, m_ProcId, 0, MPI_COMM_WORLD);
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

void pmISRandom::Receive()
{
#if SimMPS == SimulationEnabled

    char stringName[100];

    int tag = 0;
    MPI_Status  status;
    m_SenderRank = 0;

    MPI_Recv(stringName, 100, MPI_CHAR, m_SenderRank, tag, MPI_COMM_WORLD, &status);

    // Convert the c-stype string into a zString

    zString type = stringName;
    SetISType(type);

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmISRandom::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmISRandom::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
    // Not needed for this class
}
