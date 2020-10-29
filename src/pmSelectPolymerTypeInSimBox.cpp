/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSelectPolymerTypeInSimBox.cpp: implementation of the pmSelectPolymerTypeInSimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSelectPolymerTypeInSimBox.h"
#include "ccSelectPolymerTypeInSimBox.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSelectPolymerTypeInSimBox::m_Type = "SelectPolymerTypeInSimBox";

const zString pmSelectPolymerTypeInSimBox::GetType()
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
	mpmMessage* Create() {return new pmSelectPolymerTypeInSimBox();}

	const zString id = pmSelectPolymerTypeInSimBox::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmSelectPolymerTypeInSimBox::pmSelectPolymerTypeInSimBox() : mpmCommandWrapper(), m_TargetLabel(""), m_PolymerName("")
{
}

pmSelectPolymerTypeInSimBox::pmSelectPolymerTypeInSimBox(const pmSelectPolymerTypeInSimBox& oldMessage) : mpmCommandWrapper(oldMessage),
                                         m_TargetLabel(oldMessage.m_TargetLabel),
										 m_PolymerName(oldMessage.m_PolymerName)
{
}

pmSelectPolymerTypeInSimBox::~pmSelectPolymerTypeInSimBox()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSelectPolymerTypeInSimBox::put(zOutStream& os) const
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

zInStream& pmSelectPolymerTypeInSimBox::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSelectPolymerTypeInSimBox::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSelectPolymerTypeInSimBox::GetMessage() const
{
	return new pmSelectPolymerTypeInSimBox(*this);
}

// ****************************************
// Function to copy all the required data for the command into the message.
// Because each command message wraps a known command we can extract the
// data using dynamic cast.

void pmSelectPolymerTypeInSimBox::SetMessageData(const xxCommand* const pCommand)
{
	const ccSelectPolymerTypeInSimBox* const pCmd = dynamic_cast<const ccSelectPolymerTypeInSimBox*>(pCommand);

    if(pCmd)
    {
	    m_TargetLabel = pCmd->GetTargetLabel();
	    m_PolymerName = pCmd->GetPolymerName();
    }
    else
    {
        m_TargetLabel = "";
        m_PolymerName = "";
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmSelectPolymerTypeInSimBox::Validate()
{
    bool bSuccess = true;

    if(m_TargetLabel.empty() || m_PolymerName.empty() )
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSelectPolymerTypeInSimBox::SendAllP()
{
#if SimMPS == SimulationEnabled

    // We don't know how long the target label is nor the polymer name, but we allow 1000 chars each
    // (actually, 1000 minus the size of a long)

    char buffer[2000];
    int  position;
    position = 0;

    // First pack the size of the string holding the bead's name and then
    // pack the name itself into the message

    char cnameTarget[1000];
    strcpy(cnameTarget, m_TargetLabel.c_str());
    long lengthTarget = m_TargetLabel.size()+1;

    char cnamePolymer[1000];
    strcpy(cnamePolymer, m_PolymerName.c_str());
    long lengthPolymer = m_PolymerName.size()+1;

    MPI_Pack(&lengthTarget,  1,             MPI_LONG, buffer, 2000, &position, MPI_COMM_WORLD);
    MPI_Pack(&cnameTarget,   lengthTarget,  MPI_CHAR, buffer, 2000, &position, MPI_COMM_WORLD);
    MPI_Pack(&lengthPolymer,  1,            MPI_LONG, buffer, 2000, &position, MPI_COMM_WORLD);
    MPI_Pack(&cnamePolymer,  lengthPolymer, MPI_CHAR, buffer, 2000, &position, MPI_COMM_WORLD);

    // and send it to all the other processors: note that we assume that
    // the sending processor is P0, so we start the loop at processor rank 1.

    SetTag(0);

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(buffer, 2000, MPI_PACKED, m_ProcId, m_Tag, MPI_COMM_WORLD);
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

void pmSelectPolymerTypeInSimBox::Receive()
{
#if SimMPS == SimulationEnabled

    char buffer[2000];
    int  position;

    long lengthTarget = 0;
    char cnameTarget[1000];

    long lengthPolymer = 0;
    char cnamePolymer[1000];

    SetTag(0);
    MPI_Status status;

    MPI_Recv(buffer, 2000, MPI_PACKED, 0, m_Tag, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 2000, &position, &lengthTarget,  1,             MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 2000, &position, &cnameTarget,   lengthTarget,  MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 2000, &position, &lengthPolymer, 1,             MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 2000, &position, &cnamePolymer,  lengthPolymer, MPI_CHAR, MPI_COMM_WORLD);

    m_TargetLabel = cnameTarget;
	m_PolymerName = cnamePolymer;

    // Now instantiate the concrete command class using the internal constructor that
    // takes the command's data as arguments

    m_pCommand = new ccSelectPolymerTypeInSimBox(m_SimTime, m_TargetLabel, m_PolymerName);

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmSelectPolymerTypeInSimBox::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmSelectPolymerTypeInSimBox::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}


