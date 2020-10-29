/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmExtendedBondForceCount.cpp: implementation of the pmExtendedBondForceCount class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmExtendedBondForceCount.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmExtendedBondForceCount::m_Type = "pmExtendedBondForceCount";

const zString pmExtendedBondForceCount::GetType()
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
	mpmMessage* Create() {return new pmExtendedBondForceCount();}

	const zString id = pmExtendedBondForceCount::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.

pmExtendedBondForceCount::pmExtendedBondForceCount() : mpmMessage(), m_BondTotal(0), m_ForceCounter(0)
{
}

pmExtendedBondForceCount::pmExtendedBondForceCount(const pmExtendedBondForceCount& oldMessage) : mpmMessage(oldMessage),
                                                    m_BondTotal(oldMessage.m_BondTotal),
                                                    m_ForceCounter(oldMessage.m_ForceCounter)
{
}

pmExtendedBondForceCount::~pmExtendedBondForceCount()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmExtendedBondForceCount::put(zOutStream& os) const
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

zInStream& pmExtendedBondForceCount::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmExtendedBondForceCount::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmExtendedBondForceCount::GetMessage() const
{
	return new pmExtendedBondForceCount(*this);
}

// ****************************************
// Function to store the number of extended bonds that span the owning processor's boundaries whose force has been calculated by the owning processor.
// Because only one processor of the pair spanned by the bond does the calculation, the sum of all the bond forces calculated by all processors should
// equal the number of extended bonds. But note that the extended bond map in mpsSimBox holds two entries for each bond as it makes them accessible by
// their head and tail ghost bead ids.

void pmExtendedBondForceCount::SetMessageData(long bondTotal, long forceCounter)
{
    m_BondTotal = bondTotal;
	m_ForceCounter = forceCounter;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We allow any non-negative integer for the value.

bool pmExtendedBondForceCount::Validate()
{
    bool bSuccess = true;
    if(m_BondTotal < 0 || m_ForceCounter < 0)
	{
	    bSuccess = false;
	    std::cout << "Error in pmSendExtendedBondForceCount: negative extended bond total and/or force counter " << m_BondTotal <<  " " << m_ForceCounter << zEndl;
	}
	
	return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmExtendedBondForceCount::SendP0()
{
#if SimMPS == SimulationEnabled
    // Send the payload from owning processor to P0.

	if(!IsProcessZero())
	{
	    long data[2];
		data[0] = m_BondTotal;
		data[1] = m_ForceCounter;
		
		MPI_Send(data, 2, MPI_LONG, 0, m_Tag, MPI_COMM_WORLD);
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

void pmExtendedBondForceCount::SendAllP()
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

void pmExtendedBondForceCount::Receive()
{
#if SimMPS == SimulationEnabled
    MPI_Status status;
    long data[2];
 
	m_ReceiverRank = GetRank();

	// The rank of the sending processor is obtained from the
	// status parameter

	MPI_Recv(data, 2, MPI_LONG, MPI_ANY_SOURCE, m_Tag, MPI_COMM_WORLD, &status);

	m_SenderRank = status.MPI_SOURCE;
    m_BondTotal    = data[0];
	m_ForceCounter = data[1];
#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmExtendedBondForceCount::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmExtendedBondForceCount::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}


