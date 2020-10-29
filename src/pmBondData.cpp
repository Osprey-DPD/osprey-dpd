/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmBondData.cpp: implementation of the pmBondData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "pmBondData.h"
#include "InputData.h"
#include "Bond.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmBondData::m_Type = "BondData";

// Total number of types created

long pmBondData::m_TypeTotal = 0;


const zString pmBondData::GetType()
{
	return m_Type;
}

long pmBondData::GetTypeTotal()
{
	return m_TypeTotal;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// message class but a global function hidden in the namespace.

namespace
{
	mpmMessage* Create() {return new pmBondData();}

	const zString id = pmBondData::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// The total number of bond types is stored in a static member variable,
// and we use its current value to initialise the actual bond type for this message.

pmBondData::pmBondData() : mpmMessage(), 
                           m_HeadName(""),m_TailName(""),
                           m_BondType(pmBondData::m_TypeTotal++),
                           m_SprConst(0.0), m_UnStrLen(0.0)
{
}

pmBondData::pmBondData(const pmBondData& oldMessage) : mpmMessage(oldMessage),
                       m_HeadName(oldMessage.m_HeadName),
                       m_TailName(oldMessage.m_TailName),
                       m_BondType(oldMessage.m_BondType),
                       m_SprConst(oldMessage.m_SprConst),
                       m_UnStrLen(oldMessage.m_UnStrLen)
{

}

pmBondData::~pmBondData()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmBondData::put(zOutStream& os) const
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

zInStream& pmBondData::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmBondData::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmBondData::GetMessage() const
{
	return new pmBondData(*this);
}

// ****************************************
// Function to copy all the required data from the CInputData object
// into the message. 

void pmBondData::SetMessageData(const CInputData *const pData)
{
    // We need the string identifiers for the bond's head and tail beads,
    // so we get their types and then use the bead names map to get the 
    // corresponding names.

    if(pData->GetBondHeadTypesMap().find(m_BondType) != pData->GetBondHeadTypesMap().end())
    {
        long headType = (*pData->GetBondHeadTypesMap().find(m_BondType)).second;
        m_HeadName    = (*pData->GetBeadTypesMap().find(headType)).second;
    }

    if(pData->GetBondTailTypesMap().find(m_BondType) != pData->GetBondTailTypesMap().end())
    {
        long tailType = (*pData->GetBondTailTypesMap().find(m_BondType)).second;
        m_TailName    = (*pData->GetBeadTypesMap().find(tailType)).second;
    }

    m_SprConst = (pData->GetBondTypes()).at(m_BondType)->GetSprConst();
    m_UnStrLen = (pData->GetBondTypes()).at(m_BondType)->GetUnStrLength();

}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmBondData::Validate()
{
    bool bSuccess = true;

    if(m_HeadName.empty() || m_TailName.empty() || m_SprConst < 0 || m_UnStrLen < 0.0)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmBondData::SendAllP()
{
#if SimMPS == SimulationEnabled

    // Because we don't know how many bond types are defined, and hence how
    // many parameters we need to send, we use MPI_Pack/MPI_Unpack instead of 
    // building our own datatype.

    // We allow for up to 10 bond types for now: this mean we have

    char buffer[1000];
    int  position;

    char headName[100];
    char tailName[100];

    long headNameLength = m_HeadName.size()+1;
    long tailNameLength = m_TailName.size()+1;

    strcpy(headName, m_HeadName.c_str());
    strcpy(tailName, m_TailName.c_str());

    position = 0;
    MPI_Pack(&headNameLength,   1, MPI_LONG, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&headName, headNameLength, MPI_CHAR, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&tailNameLength,   1, MPI_LONG, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&tailName, tailNameLength, MPI_CHAR, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_SprConst, 1, MPI_DOUBLE, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_UnStrLen, 1, MPI_DOUBLE, buffer, 1000, &position, MPI_COMM_WORLD);

    // and send it to all the other processors: note that we assume that
    // the sending processor is P0, so we start the loop at processor rank 1.

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(buffer, 1000, MPI_PACKED, m_ProcId, 0, MPI_COMM_WORLD);
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

void pmBondData::Receive()
{
#if SimMPS == SimulationEnabled

    char buffer[1000];
    int  position;

    char headName[100];
    char tailName[100];
    long headNameLength = 0;
    long tailNameLength = 0;


    int tag = 0;
    MPI_Status status;

    MPI_Recv(buffer, 1000, MPI_PACKED, 0, tag, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 1000, &position, &headNameLength,    1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &headName, headNameLength, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &tailNameLength,    1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &tailName, tailNameLength, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &m_SprConst, 1, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &m_UnStrLen, 1, MPI_DOUBLE, MPI_COMM_WORLD);

    m_HeadName = headName;
    m_TailName = tailName;

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmBondData::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmBondData::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled



#endif
}
