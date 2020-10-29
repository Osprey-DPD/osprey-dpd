/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSetTimeStepSize.cpp: implementation of the pmSetTimeStepSize class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSetTimeStepSize.h"
#include "ccSetTimeStepSize.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSetTimeStepSize::m_Type = "SetTimeStepSize";

const zString pmSetTimeStepSize::GetType()
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
	mpmMessage* Create() {return new pmSetTimeStepSize();}

	const zString id = pmSetTimeStepSize::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmSetTimeStepSize::pmSetTimeStepSize() : mpmCommandWrapper(), m_dt(0.0)
{
}

pmSetTimeStepSize::pmSetTimeStepSize(const pmSetTimeStepSize& oldMessage) : mpmCommandWrapper(oldMessage),
                                       m_dt(oldMessage.m_dt)
{
}

pmSetTimeStepSize::~pmSetTimeStepSize()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSetTimeStepSize::put(zOutStream& os) const
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

zInStream& pmSetTimeStepSize::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSetTimeStepSize::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSetTimeStepSize::GetMessage() const
{
	return new pmSetTimeStepSize(*this);
}

// ****************************************
// Function to copy all the required data for the command into the message.
// Because each command message wraps a known command we can extract the
// data using dynamic cast.

void pmSetTimeStepSize::SetMessageData(const xxCommand* const pCommand)
{
	const ccSetTimeStepSize* const pCmd = dynamic_cast<const ccSetTimeStepSize*>(pCommand);

    if(pCmd)
    {
        m_dt = pCmd->GetTimeStep();
    }
    else
    {
        m_dt = 0.0; // This illegal value is caught by the Validate function below
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmSetTimeStepSize::Validate()
{
    bool bSuccess = true;

    // The integration step size

    if(m_dt <= 0.0)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSetTimeStepSize::SendAllP()
{
#if SimMPS == SimulationEnabled

	if(IsProcessZero())
	{
        SetTag(0);

        for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
        {
            MPI_Send(&m_dt, 1, MPI_DOUBLE, m_ProcId, m_Tag, MPI_COMM_WORLD);
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

void pmSetTimeStepSize::Receive()
{
#if SimMPS == SimulationEnabled

    SetTag(0);
    MPI_Status status;

	m_ReceiverRank = GetRank();

    MPI_Recv(&m_dt, 1, MPI_DOUBLE, 0, m_Tag, MPI_COMM_WORLD, &status);

	m_SenderRank = status.MPI_SOURCE;

    // Now instantiate the concrete command class using the internal constructor that
    // takes the command's data as arguments and log the execution.

    m_pCommand = new ccSetTimeStepSize(m_SimTime, true, m_dt);
	
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmSetTimeStepSize::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmSetTimeStepSize::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}

