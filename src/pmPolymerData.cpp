/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmPolymerData.cpp: implementation of the pmPolymerData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "pmPolymerData.h"
#include "InputData.h"
#include "Polymer.h"
#include "AbstractBead.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmPolymerData::m_Type = "PolymerData";

// Total number of types created

long pmPolymerData::m_TypeTotal = 0;


const zString pmPolymerData::GetType()
{
	return m_Type;
}

long pmPolymerData::GetTypeTotal()
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
	mpmMessage* Create() {return new pmPolymerData();}

	const zString id = pmPolymerData::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// The total number of polymer types is stored in a static member variable,
// and we use its current value to initialise the actual polymer type for this message.

pmPolymerData::pmPolymerData() : mpmMessage(), 
                           m_Name(""), m_Shape(""),
                           m_HeadName(""),m_TailName(""),
                           m_PolymerType(pmPolymerData::m_TypeTotal++),
                           m_Fraction(0.0)
{
}

pmPolymerData::pmPolymerData(const pmPolymerData& oldMessage) : mpmMessage(oldMessage),
                       m_Name(oldMessage.m_Name),
                       m_Shape(oldMessage.m_Shape),
                       m_HeadName(oldMessage.m_HeadName),
                       m_TailName(oldMessage.m_TailName),
                       m_PolymerType(oldMessage.m_PolymerType),
                       m_Fraction(oldMessage.m_Fraction)
{

}

pmPolymerData::~pmPolymerData()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmPolymerData::put(zOutStream& os) const
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

zInStream& pmPolymerData::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmPolymerData::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmPolymerData::GetMessage() const
{
	return new pmPolymerData(*this);
}

// ****************************************
// Function to copy all the required data from the CInputData object
// into the message. 

void pmPolymerData::SetMessageData(const CInputData *const pData)
{
    if(pData->GetPolymerTypesMap().find(m_PolymerType) != pData->GetPolymerTypesMap().end())
    {
	    m_Name = (*pData->GetPolymerTypesMap().find(m_PolymerType)).second;
    }

    m_Shape    = pData->GetPolymerTypeShape(m_PolymerType);
    m_Fraction = pData->GetPolymerTypeFraction(m_PolymerType);

    // Now get the polymer's head and tail bead names if they have been specified;
    // if not, we leave them blank.

    long headType = (pData->GetPolymerTypes()).at(m_PolymerType)->GetHead()->GetType();
    long tailType = (pData->GetPolymerTypes()).at(m_PolymerType)->GetTail()->GetType();

    const zString headName = (*pData->GetBeadTypesMap().find(headType)).second;
    const zString tailName = (*pData->GetBeadTypesMap().find(tailType)).second;

    if(!headName.empty() && !tailName.empty())
    {
        m_HeadName = headName;
        m_TailName = tailName;
    }
    else
    {
        m_HeadName = "";
        m_TailName = "";
    }
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.

bool pmPolymerData::Validate()
{
    bool bSuccess = true;

    // We don't check for the head and tail bead names being empty as this is
    // allowed.

    if(m_Name.empty() || m_Shape.empty() || m_Fraction < 0 || m_Fraction > 1.0)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmPolymerData::SendAllP()
{
#if SimMPS == SimulationEnabled

    // Because we don't know how many polymer types are defined, and hence how
    // many parameters we need to send, we use MPI_Pack/MPI_Unpack instead of 
    // building our own datatype.
	// Note that polymer shape strings can be of arbitrary length, but we have to use fixed length message buffers.
	// We use a large buffer size and hope that this is sufficient. But if many polymers are defined that all have complicated
	// shapes, we may need to increase it.


    char buffer[10000];
    int  position;

    char name[1000];
    char shape[1000];
    char headName[1000];
    char tailName[1000];

    long nameLength  = m_Name.size()+1;
    long shapeLength = m_Shape.size()+1;
    long headLength  = m_HeadName.size()+1;
    long tailLength  = m_TailName.size()+1;

    strcpy(name,  m_Name.c_str());
    strcpy(shape, m_Shape.c_str());
    strcpy(headName, m_HeadName.c_str());
    strcpy(tailName, m_TailName.c_str());

    // Note that we send the polymer's head and tail bead names even if they
    // are not specified. We trap the empty strings later.
    position = 0;
    MPI_Pack(&nameLength,   1,      MPI_LONG, buffer,   10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&name, nameLength,     MPI_CHAR, buffer,   10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&shapeLength,   1,     MPI_LONG, buffer,   10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&shape, shapeLength,   MPI_CHAR, buffer,   10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&m_Fraction, 1,        MPI_DOUBLE, buffer, 10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&headLength,   1,      MPI_LONG, buffer,   10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&headName, headLength, MPI_CHAR, buffer,   10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&tailLength,   1,      MPI_LONG, buffer,   10000, &position, MPI_COMM_WORLD);
    MPI_Pack(&tailName, tailLength, MPI_CHAR, buffer,   10000, &position, MPI_COMM_WORLD);

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

void pmPolymerData::Receive()
{
#if SimMPS == SimulationEnabled

    char buffer[10000];
    int  position;

    char name[1000];
    char shape[1000];
    char headName[1000];
    char tailName[1000];

    long nameLength  = 0;
    long shapeLength = 0;
    long headLength  = 0;
    long tailLength  = 0;


    int tag = 0;
    MPI_Status status;

    MPI_Recv(buffer, 1000, MPI_PACKED, 0, tag, MPI_COMM_WORLD, &status);

    // Unpack the data into member variables

    position = 0;
    MPI_Unpack(buffer, 10000, &position, &nameLength, 1,        MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &name, nameLength,     MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &shapeLength, 1,       MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &shape, shapeLength,   MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &m_Fraction, 1,        MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &headLength, 1,        MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &headName, headLength, MPI_CHAR, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &tailLength, 1,        MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(buffer, 10000, &position, &tailName, tailLength, MPI_CHAR, MPI_COMM_WORLD);
   
    
    m_Name     = name;
    m_Shape    = shape;
    m_HeadName = headName;
    m_TailName = tailName;

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmPolymerData::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmPolymerData::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled



#endif
}
