/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmExtendedPolymerProcessorCount.cpp: implementation of the pmExtendedPolymerProcessorCount class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmExtendedPolymerProcessorCount.h"
#include "mpuExtendedPolymer.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmExtendedPolymerProcessorCount::m_Type = "pmExtendedPolymerProcessorCount";

const zString pmExtendedPolymerProcessorCount::GetType()
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
	mpmMessage* Create() {return new pmExtendedPolymerProcessorCount();}

	const zString id = pmExtendedPolymerProcessorCount::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmExtendedPolymerProcessorCount::pmExtendedPolymerProcessorCount() : mpmMessage()
{
    m_vPolymerIds.clear();
}

pmExtendedPolymerProcessorCount::pmExtendedPolymerProcessorCount(const pmExtendedPolymerProcessorCount& oldMessage) : mpmMessage(oldMessage)
{
    // We do a shallow copy of the message data
    m_vPolymerIds = oldMessage.m_vPolymerIds;
}

pmExtendedPolymerProcessorCount::~pmExtendedPolymerProcessorCount()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmExtendedPolymerProcessorCount::put(zOutStream& os) const
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

zInStream& pmExtendedPolymerProcessorCount::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmExtendedPolymerProcessorCount::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmExtendedPolymerProcessorCount::GetMessage() const
{
	return new pmExtendedPolymerProcessorCount(*this);
}

// ****************************************
// Function to store the ids of all extended polymers owned by the sending processor.

void pmExtendedPolymerProcessorCount::SetMessageData(LongExtendedPolymerMap& mExtPolymers)
{
    m_vPolymerIds.clear();
	
	for(cLongExtendedPolymerIterator iterExtPoly=mExtPolymers.begin(); iterExtPoly!=mExtPolymers.end(); iterExtPoly++)
	{
	    const mpuExtendedPolymer* const pPoly = iterExtPoly->second;
		
		m_vPolymerIds.push_back(pPoly->GetId());
	}
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
//All polymer ids must be positive.

bool pmExtendedPolymerProcessorCount::Validate()
{
    bool bSuccess = true;
	
    for(zLongVectorIterator in=m_vPolymerIds.begin(); in!= m_vPolymerIds.end(); in++)
	{
	    if(*in <= 0)
		{
	        bSuccess = false;
	        std::cout << "Error in pmExtendedPolymerProcessorCount: negative extended polymer id on proc " << m_SenderRank << zEndl;		
		}
	}
	
	return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmExtendedPolymerProcessorCount::SendP0()
{
#if SimMPS == SimulationEnabled
    // Send the payload from owning processor to P0. We assume there will never be more than 1000 polymers.
	// the number of entries is passed as the first array element so that the receiving processor knows how many data items
	// to unpack.

	if(!IsProcessZero())
	{		
        char buffer[1000];
        int  position;
        position = 0;

        long polymerTotal = m_vPolymerIds.size();

		MPI_Pack(&polymerTotal, 1, MPI_LONG, buffer, 1000, &position, MPI_COMM_WORLD);

        for(long i=0; i<polymerTotal; i++)
        {
            long id = m_vPolymerIds.at(i);
            MPI_Pack(&id, 1, MPI_LONG, buffer, 1000, &position, MPI_COMM_WORLD);
        }
		
        MPI_Send(buffer, 1000, MPI_PACKED, m_ProcId, 0, MPI_COMM_WORLD);
	}
	else
    {
		// This branch should not be seen	
		std::cout << "Error: Processor 0 trying to call SendP0() " << zEndl;
    }
#endif
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmExtendedPolymerProcessorCount::SendAllP()
{
#if SimMPS == SimulationEnabled
    // Send the payload from P0 to all other processors PN.

#endif
}

// Wrapper for the Mpi function:
// MPI_Recv(message, length, MPI_CHAR, source, tag, MPI_COMM_WORLD);

// We retrieve the data from the message and fill up this instance's member variables. 
// We do not check for validity here as we assume that the sending object has 
// already done it. Note that this function does not propagate the data out into 
// the code, it only fills up the receiving message instance. Each messaging class 
// is responsible for providing further functions to pass the data to the rest of the code.

void pmExtendedPolymerProcessorCount::Receive()
{
#if SimMPS == SimulationEnabled
 
	m_ReceiverRank = GetRank();

    char buffer[1000];
    int position;

    MPI_Status status;
	
//	std::cout << "Proc " << m_ReceiverRank << " about to receive polymers " << zEndl;

    MPI_Recv(buffer, 1000, MPI_PACKED, MPI_ANY_SOURCE, m_Tag, MPI_COMM_WORLD, &status);

	m_SenderRank = status.MPI_SOURCE;

//	std::cout << "Proc " << m_ReceiverRank << " received polymers from " << m_SenderRank << zEndl;

    // Unpack the data into member variables, after ensuring the receiving container is empty
	
	m_vPolymerIds.clear();
	
	long total, id;
	total = 0;
	id    = 0;

    position = 0;
    MPI_Unpack(buffer, 1000, &position, &total, 1, MPI_LONG, MPI_COMM_WORLD);
	
	for(long i=0; i<total; i++)
	{
        MPI_Unpack(buffer, 1000, &position, &id, 1, MPI_LONG, MPI_COMM_WORLD);
		m_vPolymerIds.push_back(id);
	}

//	std::cout << "Proc " << m_ReceiverRank << " exiting receive" << zEndl;

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmExtendedPolymerProcessorCount::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmExtendedPolymerProcessorCount::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}


