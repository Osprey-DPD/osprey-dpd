/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmDouble3d.cpp: implementation of the pmDouble3d class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmDouble3d.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmDouble3d::m_Type = "pmDouble3d";

const zString pmDouble3d::GetType()
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
	mpmMessage* Create() {return new pmDouble3d();}

	const zString id = pmDouble3d::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmDouble3d::pmDouble3d() : mpmMessage()
{
	m_Value[0] = 0.0;
	m_Value[1] = 0.0;
	m_Value[2] = 0.0;
}

pmDouble3d::pmDouble3d(const pmDouble3d& oldMessage) : mpmMessage(oldMessage)
{
	m_Value[0] = oldMessage.m_Value[0];
	m_Value[1] = oldMessage.m_Value[1];
	m_Value[2] = oldMessage.m_Value[2];
}

pmDouble3d::~pmDouble3d()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmDouble3d::put(zOutStream& os) const
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

zInStream& pmDouble3d::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmDouble3d::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmDouble3d::GetMessage() const
{
	return new pmDouble3d(*this);
}

// ****************************************
// Function to store three real numbers in the message payload.
// We allow all values: positive, negative and zero.

void pmDouble3d::SetMessageData(double x, double y, double z)
{
    m_Value[0] = x;
    m_Value[1] = y;
    m_Value[2] = z;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// There is no validation to be done for this message, so we just return true.

bool pmDouble3d::Validate()
{
    return true;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmDouble3d::SendP0()
{
#if SimMPS == SimulationEnabled
    // Send the payload from owning processor to P0.

	if(!IsProcessZero())
	{
		m_SenderRank = GetRank();
		m_ReceiverRank = 0;

		MPI_Send(&m_Value, 3, MPI_DOUBLE, 0, m_Tag, MPI_COMM_WORLD);
	}
	else
    {
		// This branch should not be seen	
		std::cout << "Error: Processor 0 trying to call SendP0() " << zEndl;
    }

#endif
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmDouble3d::SendAllP()
{
#if SimMPS == SimulationEnabled

	m_SenderRank = GetRank();

    // Send the payload from the owning processor to all others but not itself.

    for(m_ProcId=0; m_ProcId<GetWorld(); m_ProcId++)
    {
        if(m_ProcId != m_SenderRank)
        {
            MPI_Send(&m_Value, 3, MPI_DOUBLE, m_ProcId, m_Tag, MPI_COMM_WORLD);
        }
    }
#endif
}

// Wrapper for the Mpi function:
// MPI_Recv(message, length, MPI_CHAR, source, tag, MPI_COMM_WORLD);

// We retrieve the data from the message and fill up this instance's member variables. 
// We do not check for validity here as we assume that the sending object has 
// already done it. Note that this function does not propagate the data out into 
// the code, it only fills up the receiving message instance. Each messaging class 
// is responsible for providing further functions to pass the data to the rest of the code.

void pmDouble3d::Receive()
{
#if SimMPS == SimulationEnabled
    MPI_Status status;

	m_ReceiverRank = GetRank();

	// The rank of the sending processor is obtained from the status parameter

	MPI_Recv(&m_Value, 3, MPI_DOUBLE, MPI_ANY_SOURCE, m_Tag, MPI_COMM_WORLD, &status);

	m_SenderRank = status.MPI_SOURCE;
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmDouble3d::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}


// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmDouble3d::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}
