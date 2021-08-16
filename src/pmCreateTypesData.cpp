/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmCreateTypesData.cpp: implementation of the pmCreateTypesData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "pmCreateTypesData.h"
#include "Polymer.h"
#include "AbstractBead.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmCreateTypesData::m_Type = "CreateTypesData";


const zString pmCreateTypesData::GetType()
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
	mpmMessage* Create() {return new pmCreateTypesData();}

	const zString id = pmCreateTypesData::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// The total number of polymer types is stored in a static member variable,
// and we use its current value to initialise the actual polymer type for this message.

pmCreateTypesData::pmCreateTypesData() : mpmMessage(), m_BeadTotal(0), m_BondTotal(0),
                                         m_BondPairTotal(0), m_PolymerTotal(0)
{
    m_vPolymerTypeTotals.clear();

    for(long i = 0; i < 1000; i++)
    {
        m_Array[i] = 0;
    }
}

pmCreateTypesData::pmCreateTypesData(const pmCreateTypesData& oldMessage) : mpmMessage(oldMessage),
                                     m_BeadTotal(oldMessage.m_BeadTotal),
                                     m_BondTotal(oldMessage.m_BondTotal),
                                     m_BondPairTotal(oldMessage.m_BondPairTotal),
                                     m_PolymerTotal(oldMessage.m_PolymerTotal),
                                     m_vPolymerTypeTotals(oldMessage.m_vPolymerTypeTotals)

{
    for(long i = 0; i < 1000; i++)
    {
        m_Array[i] = oldMessage.m_Array[i];
    }
}

pmCreateTypesData::~pmCreateTypesData()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmCreateTypesData::put(zOutStream& os) const
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

zInStream& pmCreateTypesData::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmCreateTypesData::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmCreateTypesData::GetMessage() const
{
	return new pmCreateTypesData(*this);
}

// ****************************************
// Function to copy the numbers of each polymer type in the message.

void pmCreateTypesData::SetMessageData(long beadTotal, long bondTotal, long bondPairTotal, long polymerTotal,
                        zLongVector vPolymerTypeTotals)
{
    m_BeadTotal     = beadTotal;
    m_BondTotal     = bondTotal;
    m_BondPairTotal = bondPairTotal;
    m_PolymerTotal  = polymerTotal;
    
	m_vPolymerTypeTotals.clear();
    m_vPolymerTypeTotals = vPolymerTypeTotals;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We check that there are at least some polymers to be created, and that none of the
// totals are negative. There may be zero bonds and bondpairs but there has to be 
// a non-zero number of beads and polymers. Note that zero is allowed for some polymers 
// but not all, and the first type defined must be non-zero. We also have to check that 
// the number of polymer types does not exceed the fixed-size array used to hold the data for the
// MPI message.

bool pmCreateTypesData::Validate()
{
    bool bSuccess = true;

    if(m_BeadTotal <= 0 || m_BondTotal < 0 || m_BondPairTotal < 0 || m_PolymerTotal <= 0)
    {
        bSuccess = false;
    }
    if(m_vPolymerTypeTotals.empty())
    {
        bSuccess = false;
    }
    else if(m_vPolymerTypeTotals.size() > 1000)
    {
        bSuccess = false;
    }
    else if(m_vPolymerTypeTotals.at(0) == 0)
    {
        bSuccess = false;
    }
    else
    {
        long total = 0;
        for(czLongVectorIterator ciz = m_vPolymerTypeTotals.begin(); ciz!=m_vPolymerTypeTotals.end(); ciz++)
        {
            if(*ciz < 0)
            {
                bSuccess = false;
            }
            else
            {
                total += *ciz;
            }
        }
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, procId, tag, MPI_COMM_WORLD);

void pmCreateTypesData::Send(long procId)
{
#if SimMPS == SimulationEnabled

    m_SenderRank   = GetRank();    // Rank of the sending processor
    m_ReceiverRank = procId;       // Rank of the receiving processor
	
    const long polymerTypeTotal = m_vPolymerTypeTotals.size();

    // We pass the number of beads, bonds, bondpairs and polymers of all types 
    // as the first four pieces of data, then the number of polymer types and
    // after that the numbers of each type of polymer.

    m_Array[0] = m_BeadTotal;
    m_Array[1] = m_BondTotal;
    m_Array[2] = m_BondPairTotal;
    m_Array[3] = m_PolymerTotal;
    m_Array[4] = polymerTypeTotal;

    for(long i = 0; i < polymerTypeTotal; i++)
    {
        m_Array[i+5] = m_vPolymerTypeTotals.at(i);
    }

	MPI_Send(m_Array, 1000, MPI_LONG, m_ReceiverRank, GetTag(), MPI_COMM_WORLD);

#endif
}



// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

void pmCreateTypesData::SendP0()
{
#if SimMPS == SimulationEnabled

    const long polymerTypeTotal = m_vPolymerTypeTotals.size();

    // We pass the number of beads, bonds, bondpairs and polymers of all types 
    // as the first four pieces of data, then the number of polymer types and
    // after that the numbers of each type of polymer.

    m_Array[0] = m_BeadTotal;
    m_Array[1] = m_BondTotal;
    m_Array[2] = m_BondPairTotal;
    m_Array[3] = m_PolymerTotal;
    m_Array[4] = polymerTypeTotal;
	
    for(long i = 0; i < polymerTypeTotal; i++)
    {
        m_Array[i+5] = m_vPolymerTypeTotals.at(i);
    }

	if(!IsProcessZero())
	{
		m_SenderRank = GetRank();
		m_ReceiverRank = 0;

	    MPI_Send(m_Array, 1000, MPI_LONG, 0, GetTag(), MPI_COMM_WORLD);
	}

#endif
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmCreateTypesData::SendAllP()
{
#if SimMPS == SimulationEnabled

    const long polymerTypeTotal = m_vPolymerTypeTotals.size();

    // We pass the number of beads, bonds, bondpairs and polymers of all types 
    // as the first four pieces of data, then the number of polymer types and
    // after that the numbers of each type of polymer.

    m_Array[0] = m_BeadTotal;
    m_Array[1] = m_BondTotal;
    m_Array[2] = m_BondPairTotal;
    m_Array[3] = m_PolymerTotal;
    m_Array[4] = polymerTypeTotal;

    for(long i = 0; i < polymerTypeTotal; i++)
    {
        m_Array[i+5] = m_vPolymerTypeTotals.at(i);
    }

    for(m_ProcId=1; m_ProcId<GetWorld(); m_ProcId++)
    {
        MPI_Send(m_Array, 1000, MPI_LONG, m_ProcId, 0, MPI_COMM_WORLD);
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

void pmCreateTypesData::Receive()
{
#if SimMPS == SimulationEnabled

    MPI_Status status;

    MPI_Recv(m_Array, 1000, MPI_LONG, MPI_ANY_SOURCE, GetTag(), MPI_COMM_WORLD, &status);
	
	m_SenderRank = status.MPI_SOURCE;

    // Unpack the data into member variables

    m_BeadTotal     = m_Array[0];
    m_BondTotal     = m_Array[1];
    m_BondPairTotal = m_Array[2];
    m_PolymerTotal  = m_Array[3];

    const long polymerTypeTotal = m_Array[4];
	
	// Make sure we clear the receiving container before adding new data to it!
	
	m_vPolymerTypeTotals.clear();
    for(long i = 0; i < polymerTypeTotal; i++)
    {
        m_vPolymerTypeTotals.push_back(m_Array[i+5]);
    }
	

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmCreateTypesData::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmCreateTypesData::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled



#endif
}
