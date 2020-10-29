/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmBondPairData.cpp: implementation of the pmBondPairData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "pmBondPairData.h"
#include "InputData.h"
#include "BondPair.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmBondPairData::m_Type = "BondPairData";

// Total number of types created

long pmBondPairData::m_TypeTotal = 0;


const zString pmBondPairData::GetType()
{
	return m_Type;
}

long pmBondPairData::GetTypeTotal()
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
	mpmMessage* Create() {return new pmBondPairData();}

	const zString id = pmBondPairData::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// The total number of bondpair types is stored in a static member variable,
// and we use its current value to initialise the actual bondpair type for this message.

pmBondPairData::pmBondPairData() : mpmMessage(), 
                           m_Name1(""), m_Name2(""), m_Name3(""),
                           m_BondPairType(pmBondPairData::m_TypeTotal++),
                           m_Modulus(0.0), m_Phi0(0.0)
{
}

pmBondPairData::pmBondPairData(const pmBondPairData& oldMessage) : mpmMessage(oldMessage),
                       m_Name1(oldMessage.m_Name1),
                       m_Name2(oldMessage.m_Name2),
                       m_Name3(oldMessage.m_Name3),
                       m_BondPairType(oldMessage.m_BondPairType),
                       m_Modulus(oldMessage.m_Modulus),
                       m_Phi0(oldMessage.m_Phi0)
{

}

pmBondPairData::~pmBondPairData()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmBondPairData::put(zOutStream& os) const
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

zInStream& pmBondPairData::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmBondPairData::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmBondPairData::GetMessage() const
{
	return new pmBondPairData(*this);
}

// ****************************************
// Function to copy all the required data from the CInputData object
// into the message. 

void pmBondPairData::SetMessageData(const CInputData *const pData)
{
    if(pData->GetBondPairHeadTypesMap().find(m_BondPairType) != pData->GetBondPairHeadTypesMap().end())
    {
        long headType = (*pData->GetBondPairHeadTypesMap().find(m_BondPairType)).second;
        m_Name1 = (*pData->GetBeadTypesMap().find(headType)).second;
    }

    if(pData->GetBondPairMiddleTypesMap().find(m_BondPairType) != pData->GetBondPairMiddleTypesMap().end())
    {
        long middleType = (*pData->GetBondPairMiddleTypesMap().find(m_BondPairType)).second;
        m_Name2 = (*pData->GetBeadTypesMap().find(middleType)).second;
    }

    if(pData->GetBondPairTailTypesMap().find(m_BondPairType) != pData->GetBondPairTailTypesMap().end())
    {
        long tailType = (*pData->GetBondPairTailTypesMap().find(m_BondPairType)).second;
        m_Name3 = (*pData->GetBeadTypesMap().find(tailType)).second;
    }

    m_Modulus = (pData->GetBondPairTypes()).at(m_BondPairType)->GetModulus();
    m_Phi0    = (pData->GetBondPairTypes()).at(m_BondPairType)->GetPhi0();

}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmBondPairData::Validate()
{
    bool bSuccess = true;

    if(m_Name1.empty() || m_Name2.empty() || m_Name3.empty() || m_Modulus < 0 || m_Phi0 < 0.0)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmBondPairData::SendAllP()
{
#if SimMPS == SimulationEnabled

    // Because we don't know how many bond types are defined, and hence how
    // many parameters we need to send, we use MPI_Pack/MPI_Unpack instead of 
    // building our own datatype.

    // We allow for up to 10 bond types for now: this mean we have

    char buffer[1000];
    int  position;

    char headName[100];
    char middleName[100];
    char tailName[100];

    long headNameLength   = m_Name1.size()+1;
    long middleNameLength = m_Name2.size()+1;
    long tailNameLength   = m_Name3.size()+1;

    strcpy(headName, m_Name1.c_str());
    strcpy(middleName, m_Name2.c_str());
    strcpy(tailName, m_Name3.c_str());

    position = 0;
    MPI_Pack(&headNameLength, 1, MPI_LONG, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&headName, headNameLength, MPI_CHAR, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&middleNameLength, 1, MPI_LONG, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&middleName, middleNameLength, MPI_CHAR, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&tailNameLength, 1, MPI_LONG, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&tailName, tailNameLength, MPI_CHAR, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_Modulus, 1, MPI_DOUBLE, buffer, 1000, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_Phi0, 1, MPI_DOUBLE, buffer, 1000, &position, MPI_COMM_WORLD);

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

void pmBondPairData::Receive()
{
#if SimMPS == SimulationEnabled

    char buffer[1000];
    int  position;

    char headName[100];
    char middleName[100];
    char tailName[100];

    long headNameLength   = 0;
    long middleNameLength = 0;
    long tailNameLength   = 0;


    int tag = 0;
    MPI_Status status;

    MPI_Recv(buffer, 1000, MPI_PACKED, 0, tag, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 1000, &position, &headNameLength,    1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &headName, headNameLength, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &middleNameLength,    1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &middleName, middleNameLength, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &tailNameLength,    1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &tailName, tailNameLength, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &m_Modulus, 1, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 1000, &position, &m_Phi0, 1, MPI_DOUBLE, MPI_COMM_WORLD);

    m_Name1 = headName;
    m_Name2 = middleName;
    m_Name3 = tailName;


#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmBondPairData::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmBondPairData::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled



#endif
}
