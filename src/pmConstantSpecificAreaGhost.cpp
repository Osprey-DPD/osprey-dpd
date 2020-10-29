/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmConstantSpecificAreaGhost.cpp: implementation of the pmConstantSpecificAreaGhost class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmConstantSpecificAreaGhost.h"
#include "ctConstantSpecificAreaGhost.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmConstantSpecificAreaGhost::m_Type = "ConstantSpecificAreaGhost";

const zString pmConstantSpecificAreaGhost::GetType()
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
	mpmMessage* Create() {return new pmConstantSpecificAreaGhost();}

	const zString id = pmConstantSpecificAreaGhost::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmConstantSpecificAreaGhost::pmConstantSpecificAreaGhost() : mpmCommandWrapper(), 
															m_TargetName(""), m_GhostLipidName(""), m_RealLipidName(""),
                                                                               m_DecLabel(""),
                                                                               m_BeadName(""),
                                                                               m_SampleRate(0), m_TargetAN(0.0),m_AreaMethodConst(0.0) 
{
}

pmConstantSpecificAreaGhost::pmConstantSpecificAreaGhost(const pmConstantSpecificAreaGhost& oldMessage) : mpmCommandWrapper(oldMessage),
                                     m_TargetName(oldMessage.m_TargetName),
                                     m_GhostLipidName(oldMessage.m_GhostLipidName),
									 m_RealLipidName(oldMessage.m_RealLipidName),
									 m_DecLabel(oldMessage.m_DecLabel),
									 m_BeadName(oldMessage.m_BeadName),
									 m_SampleRate(oldMessage.m_SampleRate),
 									 m_TargetAN(oldMessage.m_TargetAN),
 
                                    m_AreaMethodConst(oldMessage.m_AreaMethodConst)
{
}

pmConstantSpecificAreaGhost::~pmConstantSpecificAreaGhost()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmConstantSpecificAreaGhost::put(zOutStream& os) const
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

zInStream& pmConstantSpecificAreaGhost::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmConstantSpecificAreaGhost::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmConstantSpecificAreaGhost::GetMessage() const
{
	return new pmConstantSpecificAreaGhost(*this);
}

// ****************************************
// Function to copy all the required data for the command into the message.
// Because each command message wraps a known command we can extract the
// data using dynamic cast.

void pmConstantSpecificAreaGhost::SetMessageData(const xxCommand* const pCommand)
{
	const ctConstantSpecificAreaGhost* const pCmd = dynamic_cast<const ctConstantSpecificAreaGhost*>(pCommand);

    if(pCmd)
    {


            m_TargetName            = pCmd->GetMembraneTargetName();
            m_DecLabel	            = pCmd->GetDecLabel();
            m_BeadName              = pCmd->GetBeadName();
            m_SampleRate	        = pCmd->GetSampleRate();	 
            m_TargetAN	            = pCmd->GetTargetAN();

    }
    else
    {



       m_TargetName      = ""; 
       m_DecLabel        = "";
       m_BeadName        = "";


       m_SampleRate      = 0;
       m_TargetAN        = 0.0;
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We use the same values as are used in the original command class.

bool pmConstantSpecificAreaGhost::Validate()
{
    bool bSuccess = true;

    if(m_TargetName.empty() || m_GhostLipidName.empty() || m_RealLipidName.empty()|| m_DecLabel.empty() || m_BeadName.empty())
    {
        bSuccess = false;
    }
	else if(m_SampleRate < 1 || m_TargetAN < 0.01)
	{
        bSuccess = false;
	}

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmConstantSpecificAreaGhost::SendAllP()
{
#if SimMPS == SimulationEnabled

    // We don't know how long the target label, polymer name decorator label and bead name are, but we allow 1000 chars each
    // (actually, 1000 minus the size of a long); and 1000 more for the numeric data.

    char buffer[5000];
    int  position;
    position = 0;

    // First pack the size of the string holding the bead's name and then
    // pack the name itself into the message

    char cnameTargetName[1000];
    strcpy(cnameTargetName, m_TargetName.c_str());
    long lengthTargetName = m_TargetName.size()+1;

    char cnameGhostLipidName[1000];
    strcpy(cnameGhostLipidName, m_GhostLipidName.c_str());
    long lengthGhostLipidName = m_GhostLipidName.size()+1;

    char cnameRealLipidName[1000];
    strcpy(cnameRealLipidName, m_RealLipidName.c_str());
    long lengthRealLipidName = m_RealLipidName.size()+1;

    char cnameDecLabel[1000];
    strcpy(cnameDecLabel, m_DecLabel.c_str());
    long lengthDecLabel = m_DecLabel.size()+1;

    char cnameBeadName[1000];
    strcpy(cnameBeadName, m_BeadName.c_str());
    long lengthBeadName = m_BeadName.size()+1;

    MPI_Pack(&lengthTargetName,  1,                     MPI_LONG, buffer, 5000, &position, MPI_COMM_WORLD);
    MPI_Pack(&cnameTargetName,   lengthTargetName,  MPI_CHAR, buffer, 5000, &position, MPI_COMM_WORLD);
    MPI_Pack(&lengthGhostLipidName, 1,                     MPI_LONG, buffer, 5000, &position, MPI_COMM_WORLD);
    MPI_Pack(&cnameGhostLipidName,  lengthGhostLipidName, MPI_CHAR, buffer, 5000, &position, MPI_COMM_WORLD);
    MPI_Pack(&lengthRealLipidName, 1,                     MPI_LONG, buffer, 5000, &position, MPI_COMM_WORLD);
    MPI_Pack(&cnameRealLipidName,  lengthRealLipidName, MPI_CHAR, buffer, 5000, &position, MPI_COMM_WORLD);
    MPI_Pack(&lengthDecLabel,        1,                     MPI_LONG, buffer, 5000, &position, MPI_COMM_WORLD);
    MPI_Pack(&cnameDecLabel,         lengthDecLabel,        MPI_CHAR, buffer, 5000, &position, MPI_COMM_WORLD);

    MPI_Pack(&lengthBeadName,        1,                     MPI_LONG, buffer, 5000, &position, MPI_COMM_WORLD);
    MPI_Pack(&cnameBeadName,         lengthBeadName,        MPI_CHAR, buffer, 5000, &position, MPI_COMM_WORLD);

    MPI_Pack(&m_SampleRate,          1,                     MPI_LONG, buffer, 5000, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_TargetAN,            1,                     MPI_DOUBLE, buffer, 5000, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_AreaMethodConst,            1,                     MPI_DOUBLE, buffer, 5000, &position, MPI_COMM_WORLD);

    // and send it to all the other processors: note that we assume that
    // the sending processor is P0, so we start the loop at processor rank 1.

    SetTag(0);

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(buffer, 5000, MPI_PACKED, m_ProcId, m_Tag, MPI_COMM_WORLD);
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



void pmConstantSpecificAreaGhost::Receive()
{
#if SimMPS == SimulationEnabled


    char buffer[5000];
    int  position;



    long lengthTargetName = 0;
    char cnameTargetName[1000];

    long lengthGhostLipidName = 0;
    char cnameGhostLipidName[1000];

    long lengthRealLipidName = 0;
    char cnameRealLipidName[1000];

    long lengthDecLabel = 0;
	char cnameDecLabel[1000];

	long lengthBeadName = 0;
    char cnameBeadName[1000];

    SetTag(0);
    MPI_Status status;

    MPI_Recv(buffer, 5000, MPI_PACKED, 0, m_Tag, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 5000, &position, &lengthTargetName,  1,                     MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &cnameTargetName,   lengthTargetName,  MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &lengthGhostLipidName, 1,                     MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &cnameGhostLipidName,  lengthGhostLipidName, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &lengthRealLipidName, 1,                     MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &cnameRealLipidName,  lengthRealLipidName, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &lengthDecLabel,        1,                     MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &cnameDecLabel,         lengthDecLabel,        MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &lengthBeadName,        1,                     MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &cnameBeadName,         lengthBeadName,        MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &m_SampleRate,          1,                     MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &m_TargetAN,            1,                     MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 5000, &position, &m_AreaMethodConst,            1,                     MPI_DOUBLE, MPI_COMM_WORLD);
	m_TargetName  = cnameTargetName;
	m_GhostLipidName = cnameGhostLipidName;
	m_RealLipidName = cnameRealLipidName;
	m_DecLabel            = cnameDecLabel;
	m_BeadName            = cnameBeadName;
    
    // Now instantiate the concrete command class using the internal constructor that
    // takes the command's data as arguments

//    m_pCommand = new ctConstantSpecificAreaGhost(m_SimTime, m_TargetName, m_GhostLipidName , m_RealLipidName, m_DecLabel, m_BeadName, m_SampleRate, m_TargetAN, m_AreaMethodConst);
   
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmConstantSpecificAreaGhost::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmConstantSpecificAreaGhost::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}


