/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmPanCurrentStateCamera.cpp: implementation of the pmPanCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmPanCurrentStateCamera.h"
#include "mcPanCurrentStateCamera.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmPanCurrentStateCamera::m_Type = "PanCurrentStateCamera";

const zString pmPanCurrentStateCamera::GetType()
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
	mpmMessage* Create() {return new pmPanCurrentStateCamera();}

	const zString id = pmPanCurrentStateCamera::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmPanCurrentStateCamera::pmPanCurrentStateCamera() : mpmCommandWrapper(), m_bInitialFlag(false),
																		  m_bFinalFlag(false),
																		  m_Duration(0),
																		  m_FrameRate(0),
																		  m_StepsPerFrame(0),
																		  m_PanAngle(0.0)

{
	for(long i=0; i<3; i++)
	{
	    m_Normal[i] = 0;
		m_Camera[i]	= 0.0;
		m_Target[i]	= 0.0;
	}

}

pmPanCurrentStateCamera::pmPanCurrentStateCamera(const pmPanCurrentStateCamera& oldMessage) : mpmCommandWrapper(oldMessage),
                                                                                              m_bInitialFlag(oldMessage.m_bInitialFlag),
																		                      m_bFinalFlag(oldMessage.m_bFinalFlag),
																							  m_Duration(oldMessage.m_Duration),
																		                      m_FrameRate(oldMessage.m_FrameRate),
																							  m_StepsPerFrame(oldMessage.m_StepsPerFrame),
																		                      m_PanAngle(oldMessage.m_PanAngle)
																		
{
	for(long i=0; i<3; i++)
	{
	    m_Normal[i] = oldMessage.m_Normal[i];
		m_Camera[i]	= oldMessage.m_Camera[i];
		m_Target[i]	= oldMessage.m_Target[i];
	}
}

pmPanCurrentStateCamera::~pmPanCurrentStateCamera()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmPanCurrentStateCamera::put(zOutStream& os) const
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

zInStream& pmPanCurrentStateCamera::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmPanCurrentStateCamera::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmPanCurrentStateCamera::GetMessage() const
{
	return new pmPanCurrentStateCamera(*this);
}

// ****************************************
// Function to copy all the required data for the command into the message.
// Because each command message wraps a known command we can extract the
// data using dynamic cast.

void pmPanCurrentStateCamera::SetMessageData(const xxCommand* const pCommand)
{
	const mcPanCurrentStateCamera* const pCmd = dynamic_cast<const mcPanCurrentStateCamera*>(pCommand);

    if(pCmd)
    {		
	    m_bInitialFlag   = pCmd->GetJumpToInitialFlag();
		m_bFinalFlag     = pCmd->GetStayAtFinalFlag();
		
		// Note that the command class has already converted the pan angle from degrees to radians.
		
		m_Duration       = pCmd->GetDuration();
		m_FrameRate      = pCmd->GetFrameRate();
		m_StepsPerFrame  = pCmd->GetStepsPerFrame();
		m_PanAngle       = pCmd->GetPanAngle();
		
	    for(long i=0; i<3; i++)
	    {
            m_Normal[i] = pCmd->GetNormal(i);
		    m_Camera[i]	= pCmd->GetCamera(i);
		    m_Target[i]	= pCmd->GetTarget(i);
	    }
    }
    else
    {
	    m_bInitialFlag   = false;
		m_bFinalFlag     = false;
		m_Duration       = 0;
		m_FrameRate      = 0;
		m_StepsPerFrame  = 1;
		m_PanAngle       = 0.0;
		
	    for(long i=0; i<3; i++)
	    {
		    m_Camera[i]	= 0.0;
		    m_Target[i]	= 0.0;
	    }
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmPanCurrentStateCamera::Validate()
{
    bool bSuccess = true;

    // The camera and target positions must not be identical.

    if(m_Camera[0] == m_Target[0] && m_Camera[1] == m_Target[1] &&
       m_Camera[2] == m_Target[2])
    {
        bSuccess = false;
    }
	else if(m_Normal[0] == 0 && m_Normal[1] == 0 && m_Normal[2] == 0)
	{
        bSuccess = false;
	}
	else if(m_Duration == 0 || m_FrameRate == 0 || m_StepsPerFrame == 0)
	{
        bSuccess = false;
	}

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmPanCurrentStateCamera::SendAllP()
{
#if SimMPS == SimulationEnabled

    int position;
	position = 0;
	
    MPI_Pack(&m_bInitialFlag,    1, MPI_LONG,   m_Buffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_bFinalFlag,      1, MPI_LONG,   m_Buffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_Duration,        1, MPI_LONG,   m_Buffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_FrameRate,       1, MPI_LONG,   m_Buffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_StepsPerFrame,   1, MPI_LONG,   m_Buffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_PanAngle,        1, MPI_DOUBLE, m_Buffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Normal,           3, MPI_LONG,   m_Buffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Camera,           3, MPI_DOUBLE, m_Buffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Target,           3, MPI_DOUBLE, m_Buffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
	
    // and send it to all the other processors: note that we assume that
    // the sending processor is P0, so we start the loop at processor rank 1.

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(m_Buffer, m_MaxMsgBuffer, MPI_PACKED, m_ProcId, 0, MPI_COMM_WORLD);
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

void pmPanCurrentStateCamera::Receive()
{
#if SimMPS == SimulationEnabled

    int  position;

    MPI_Status status;
	
    MPI_Recv(m_Buffer, m_MaxMsgBuffer, MPI_PACKED, 0, m_Tag, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(m_Buffer, m_MaxMsgBuffer, &position, &m_bInitialFlag,    1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(m_Buffer, m_MaxMsgBuffer, &position, &m_bFinalFlag,      1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(m_Buffer, m_MaxMsgBuffer, &position, &m_Duration,        1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(m_Buffer, m_MaxMsgBuffer, &position, &m_FrameRate,       1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(m_Buffer, m_MaxMsgBuffer, &position, &m_StepsPerFrame,   1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(m_Buffer, m_MaxMsgBuffer, &position, &m_PanAngle,        1, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(m_Buffer, m_MaxMsgBuffer, &position, m_Normal,           3, MPI_LONG,   MPI_COMM_WORLD);
    MPI_Unpack(m_Buffer, m_MaxMsgBuffer, &position, m_Camera,           3, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(m_Buffer, m_MaxMsgBuffer, &position,&m_Target,           3, MPI_DOUBLE, MPI_COMM_WORLD);
	
    // Now instantiate the concrete command class using the internal constructor that
    // takes the command's data as arguments. We log the message.   

    m_pCommand = new mcPanCurrentStateCamera(m_SimTime, true, m_bInitialFlag, m_bFinalFlag, m_Duration, m_FrameRate, m_StepsPerFrame, m_PanAngle, m_Normal, m_Camera, m_Target);
	
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmPanCurrentStateCamera::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmPanCurrentStateCamera::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}


