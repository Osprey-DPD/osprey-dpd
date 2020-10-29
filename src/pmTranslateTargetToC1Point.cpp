/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmTranslateTargetToC1Point.cpp: implementation of the pmTranslateTargetToC1Point class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmTranslateTargetToC1Point.h"
#include "ctTranslateTargetToC1Point.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmTranslateTargetToC1Point::m_Type = "TranslateTargetToC1Point";

const zString pmTranslateTargetToC1Point::GetType()
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
	mpmMessage* Create() {return new pmTranslateTargetToC1Point();}

	const zString id = pmTranslateTargetToC1Point::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmTranslateTargetToC1Point::pmTranslateTargetToC1Point() : mpmCommandWrapper(), m_Label("")
{
    m_Centre[0] = 0.0;
    m_Centre[1] = 0.0;
    m_Centre[2] = 0.0;
}

pmTranslateTargetToC1Point::pmTranslateTargetToC1Point(const pmTranslateTargetToC1Point& oldMessage) : mpmCommandWrapper(oldMessage),
													                                                   m_Label(oldMessage.m_Label)
{
    m_Centre[0] = oldMessage.m_Centre[0];
    m_Centre[1] = oldMessage.m_Centre[1];
    m_Centre[2] = oldMessage.m_Centre[2];
}

pmTranslateTargetToC1Point::~pmTranslateTargetToC1Point()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//
// m_Label - string identifier for the target being moved
// m_Centre[3]  - coordinates of the point to which the target is being moved.

zOutStream& pmTranslateTargetToC1Point::put(zOutStream& os) const
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

zInStream& pmTranslateTargetToC1Point::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmTranslateTargetToC1Point::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmTranslateTargetToC1Point::GetMessage() const
{
	return new pmTranslateTargetToC1Point(*this);
}

// ****************************************
// Function to copy all the required data for the command into the message.
// Because each command message wraps a known command we can extract the
// data using dynamic cast.

void pmTranslateTargetToC1Point::SetMessageData(const xxCommand* const pCommand)
{
	const ctTranslateTargetToC1Point* const pCmd = dynamic_cast<const ctTranslateTargetToC1Point*>(pCommand);

    if(pCmd)
    {
	    m_Label     = pCmd->GetTargetLabel();
        m_Centre[0] = pCmd->GetXCentre();
        m_Centre[1] = pCmd->GetYCentre();
        m_Centre[2] = pCmd->GetZCentre();
    }
    else
    {
	    m_Label     = "";
        m_Centre[0] = 0.0; // This illegal value is caught by the Validate function below
        m_Centre[1] = 0.0;
        m_Centre[2] = 0.0; 
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmTranslateTargetToC1Point::Validate()
{
    bool bSuccess = true;

    // The target should not be moved exactly to a processor boundary.
	
    if( m_Label.empty() || 
	     m_Centre[0] <= 0.0 ||  m_Centre[1] <= 0.0 ||  m_Centre[2] <= 0.0 || 
		 m_Centre[0] >= 1.0 ||  m_Centre[1] >= 1.0 ||  m_Centre[2] >= 1.0)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmTranslateTargetToC1Point::SendAllP()
{
#if SimMPS == SimulationEnabled

	if(IsProcessZero())
	{
        char buffer[1100];
        int  position;

        char name[1000];
        strcpy(name, m_Label.c_str());
        long nameLength = m_Label.size() + 1;

        position = 0;
        MPI_Pack(&nameLength,   1,  MPI_LONG,   buffer, 1100, &position, MPI_COMM_WORLD);
        MPI_Pack(&name, nameLength, MPI_CHAR,   buffer, 1100, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_Centre[0],  1,  MPI_DOUBLE, buffer, 1100, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_Centre[1],  1,  MPI_DOUBLE, buffer, 1100, &position, MPI_COMM_WORLD);
        MPI_Pack(&m_Centre[2],  1,  MPI_DOUBLE, buffer, 1100, &position, MPI_COMM_WORLD);

        SetTag(0);

        for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
        {
            MPI_Send(buffer, 1100, MPI_PACKED, m_ProcId, m_Tag, MPI_COMM_WORLD);
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

void pmTranslateTargetToC1Point::Receive()
{
#if SimMPS == SimulationEnabled

    SetTag(0);
    MPI_Status status;

	m_ReceiverRank = GetRank();

    char buffer[1100];
    int position;

    char name[1100];
    long nameLength = 0;
	
    MPI_Recv(buffer, 1100, MPI_PACKED, 0, m_Tag, MPI_COMM_WORLD, &status);

	m_SenderRank = status.MPI_SOURCE;
	
    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 1100, &position, &nameLength, 1,    MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1100, &position, &name, nameLength, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1100, &position, &m_Centre[0], 1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1100, &position, &m_Centre[1], 1,  MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1100, &position, &m_Centre[2], 1,  MPI_DOUBLE, MPI_COMM_WORLD);

    m_Label = name;

    // Now instantiate the concrete command class using the internal constructor that
    // takes the command's data as arguments and log the execution.

    m_pCommand = new ctTranslateTargetToC1Point(m_SimTime, m_Label, m_Centre[0], m_Centre[1], m_Centre[2]);
	
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmTranslateTargetToC1Point::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmTranslateTargetToC1Point::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}

