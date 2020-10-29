/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSetDisplayBeadRange.cpp: implementation of the pmSetDisplayBeadRange class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSetDisplayBeadRange.h"
#include "mcSetDisplayBeadRange.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSetDisplayBeadRange::m_Type = "SetDisplayBeadRange";

const zString pmSetDisplayBeadRange::GetType()
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
	mpmMessage* Create() {return new pmSetDisplayBeadRange();}

	const zString id = pmSetDisplayBeadRange::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.
// Note that we set the axis to the z axis by default.

pmSetDisplayBeadRange::pmSetDisplayBeadRange() : mpmCommandWrapper(), 
                                                 m_X(1), m_Y(2), m_Z(3),
											     m_AxisNo(m_Z), 
											     m_MinFraction(0.0),
											     m_MaxFraction(1.0)

{
}

pmSetDisplayBeadRange::pmSetDisplayBeadRange(const pmSetDisplayBeadRange& oldMessage) : mpmCommandWrapper(oldMessage),
											     m_X(oldMessage.m_X), 
											     m_Y(oldMessage.m_Y), 
											     m_Z(oldMessage.m_Z), 
											     m_AxisNo(oldMessage.m_AxisNo), 
											     m_MinFraction(oldMessage.m_MinFraction),
											     m_MaxFraction(oldMessage.m_MaxFraction)
{
}

pmSetDisplayBeadRange::~pmSetDisplayBeadRange()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSetDisplayBeadRange::put(zOutStream& os) const
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

zInStream& pmSetDisplayBeadRange::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSetDisplayBeadRange::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSetDisplayBeadRange::GetMessage() const
{
	return new pmSetDisplayBeadRange(*this);
}

// ****************************************
// Function to copy all the required data for the command into the message.
// Because each command message wraps a known command we can extract the
// data using dynamic cast.

void pmSetDisplayBeadRange::SetMessageData(const xxCommand* const pCommand)
{
	const mcSetDisplayBeadRange* const pCmd = dynamic_cast<const mcSetDisplayBeadRange*>(pCommand);

    if(pCmd)
    {
        m_AxisNo      = GetAxisNo(pCmd->GetAxis());
		m_MinFraction = pCmd->GetMinFraction();
		m_MaxFraction = pCmd->GetMaxFraction();
    }
    else
    {
        m_AxisNo      = m_Z;
		m_MinFraction = 0.0;
		m_MaxFraction = 1.0;
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmSetDisplayBeadRange::Validate()
{
    bool bSuccess = true;

    if((m_AxisNo != m_X && m_AxisNo != m_Y && m_AxisNo != m_Z) ||
	    m_MinFraction < 0.0 || m_MaxFraction > 1.0 || m_MinFraction > m_MaxFraction)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSetDisplayBeadRange::SendAllP()
{
#if SimMPS == SimulationEnabled

    // The data consist of 1 integer and two doubles, so we pack them
    // sequentially in the buffer and send them to all the other processors: 
	// note that we assume that the sending processor is P0, so we start the 
	// loop at processor rank 1.

    char buffer[100];
    int  position;
	position = 0;

    MPI_Pack(&m_AxisNo,      1, MPI_LONG,   buffer, 100, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_MinFraction, 1, MPI_DOUBLE, buffer, 100, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_MaxFraction, 1, MPI_DOUBLE, buffer, 100, &position, MPI_COMM_WORLD);

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(buffer, 100, MPI_PACKED, m_ProcId, 0, MPI_COMM_WORLD);
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

void pmSetDisplayBeadRange::Receive()
{
#if SimMPS == SimulationEnabled

    char buffer[100];
    int  position;

    int tag = 0;
    MPI_Status status;

    MPI_Recv(buffer, 100, MPI_PACKED, 0, tag, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 100, &position, &m_AxisNo,      1, MPI_LONG,   MPI_COMM_WORLD);
    MPI_Unpack(buffer, 100, &position, &m_MinFraction, 1, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 100, &position, &m_MaxFraction, 1, MPI_DOUBLE, MPI_COMM_WORLD);

    // Now instantiate the concrete command class using the internal constructor that
    // takes the command's data as arguments
	
    m_pCommand = new mcSetDisplayBeadRange(m_SimTime, GetAxis(), m_MinFraction, m_MaxFraction);

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmSetDisplayBeadRange::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmSetDisplayBeadRange::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}


// Function to return the string name of the axis given the integer value
// stored locally.

const zString pmSetDisplayBeadRange::GetAxis() const
{
    if(m_AxisNo == m_X)
		return "x";
	else if(m_AxisNo == m_Y)
		return "y";
	else
		return "z";
}

// Function to return the integer id of the axis given its string identifier.

long pmSetDisplayBeadRange::GetAxisNo(const zString axis) const
{
    if(axis == "x")
		return m_X;
	else if(axis == "y")
		return m_Y;
	else
		return m_Z;
}
