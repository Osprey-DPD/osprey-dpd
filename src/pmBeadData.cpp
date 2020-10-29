/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmBeadData.cpp: implementation of the pmBeadData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "pmBeadData.h"
#include "InputData.h"
#include "Bead.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmBeadData::m_Type = "BeadData";

// Total number of types created

long pmBeadData::m_TypeTotal = 0;


const zString pmBeadData::GetType()
{
	return m_Type;
}

long pmBeadData::GetTypeTotal()
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
	mpmMessage* Create() {return new pmBeadData();}

	const zString id = pmBeadData::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.
// The total number of bead types is stored in a static member variable,
// and we use its previous value to initialise the actual bead type for this message.

pmBeadData::pmBeadData() : mpmMessage(), 
                           m_BeadName(""),m_BeadType(pmBeadData::m_TypeTotal++),
                           m_BeadRadius(0.0), m_LGRadius(0.0)
{
    m_vConsInt.clear();
    m_vDissInt.clear();
    m_vLGInt.clear();
}

pmBeadData::pmBeadData(const pmBeadData& oldMessage) : mpmMessage(oldMessage),
                       m_BeadName(oldMessage.m_BeadName),
                       m_BeadType(oldMessage.m_BeadType),
                       m_BeadRadius(oldMessage.m_BeadRadius),
                       m_LGRadius(oldMessage.m_LGRadius),
                       m_vConsInt(oldMessage.m_vConsInt),
                       m_vDissInt(oldMessage.m_vDissInt),
                       m_vLGInt(oldMessage.m_vLGInt)
{

}

pmBeadData::~pmBeadData()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmBeadData::put(zOutStream& os) const
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

zInStream& pmBeadData::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmBeadData::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmBeadData::GetMessage() const
{
	return new pmBeadData(*this);
}

// ****************************************
// Function to copy all the required data from the CInputData object
// into the message. 

void pmBeadData::SetMessageData(const CInputData *const pData)
{
    if(pData->GetBeadTypesMap().find(m_BeadType) != pData->GetBeadTypesMap().end())
    {
	    m_BeadName = (*pData->GetBeadTypesMap().find(m_BeadType)).second;
    }

    m_BeadRadius = (pData->GetBeadTypes()).at(m_BeadType)->GetRadius();
 
#if EnableDPDLG == ExperimentEnabled
    m_LGRadius = pData->GetBeadTypes().at(m_BeadType).GetLGRadius();
#endif

    // Now the conservative and dissipative interactions. We just want
    // the row in the vector corresponding to the bead type's interactions
    // with itself and those types previously defined, which just have
    // lower types.

    zArray2dDouble  vvConsInt = *pData->GetDPDBeadConsIntArray();
    zArray2dDouble  vvDissInt = *pData->GetDPDBeadDissIntArray();

#if EnableDPDLG == ExperimentEnabled
    zArray2dDouble  vvLGInt = *pData->GetDPDBeadLGIntArray();
#endif

	for(long j=0; j<m_TypeTotal; j++)
	{
		m_vConsInt.push_back(vvConsInt.at(m_BeadType).at(j));
		m_vDissInt.push_back(vvDissInt.at(m_BeadType).at(j));

#if EnableDPDLG == ExperimentEnabled
		m_vLGInt.push_back(vvLGInt.at(m_BeadType).at(j));
#endif
	}
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmBeadData::Validate()
{
    bool bSuccess = true;

    if(m_BeadName.empty() || m_BeadType < 0 || m_BeadRadius < 0.0)
    {
        bSuccess = false;
    }
#if EnableDPDLG == ExperimentEnabled
    else if(m_LGRadius < 0.0)
    {
        bSuccess = false;
    }
#endif

    // Check the bead-bead interactions: note that the conservative parameters
    // are allowed to be negative, but not the dissipative or LG parameters.

	for(long j=0; j<m_BeadType; j++)
	{
        if(m_vDissInt.at(j) < 0.0)
        {
            bSuccess = false;
        }

#if EnableDPDLG == ExperimentEnabled
        if(m_vLGInt.at(j) < 0.0)
        {
            bSuccess = false;
        }
#endif
	}

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmBeadData::SendAllP()
{
#if SimMPS == SimulationEnabled

    // Because we don't know how many bead types are defined, and hence how
    // many conservative/dissipative interaction parameters we need to send,
    // we use MPI_Pack/MPI_Unpack instead of building our own datatype.

    // We allow for up to 10 bead types for now: this mean we have
    // 
    // 10*type + 10*radius + 90 *consInt + 90 * dissInt

    // not counting the names

    char buffer[10000];
    int  position;

    char name[1000];
    strcpy(name, m_BeadName.c_str());
    long nameLength = m_BeadName.size() + 1;

    position = 0;
    MPI_Pack(&nameLength,   1,  MPI_LONG, buffer,   10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&name, nameLength, MPI_CHAR, buffer,   10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_BeadType,   1,  MPI_LONG, buffer,   10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_BeadRadius, 1,  MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);

    for(long i=0; i<m_TypeTotal; i++)
    {
        double cons = m_vConsInt.at(i);
        double diss = m_vDissInt.at(i);

        MPI_Pack(&cons, 1, MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
        MPI_Pack(&diss, 1, MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
    }

    // and send it to all the other processors: note that we assume that
    // the sending processor is P0, so we start the loop at processor rank 1.

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(buffer, 10000, MPI_PACKED, m_ProcId, 0, MPI_COMM_WORLD);
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

void pmBeadData::Receive()
{
#if SimMPS == SimulationEnabled

    char buffer[10000];
    int position;

    char name[1000];
    long nameLength = 0;


    int tag = 0;
    MPI_Status status;

    MPI_Recv(buffer, 10000, MPI_PACKED, 0, tag, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 10000, &position, &nameLength, 1,    MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &name, nameLength, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_BeadType, 1,    MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_BeadRadius, 1,  MPI_DOUBLE, MPI_COMM_WORLD);

    m_BeadName = name;

    double cons = 0.0;
    double diss = 0.0;

    for(long i=0; i<m_TypeTotal; i++)
    {
        MPI_Unpack(buffer, 10000, &position, &cons, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(buffer, 10000, &position, &diss, 1, MPI_DOUBLE, MPI_COMM_WORLD);

        m_vConsInt.push_back(cons);
        m_vDissInt.push_back(diss);
    }


#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmBeadData::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmBeadData::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled


#endif
}
