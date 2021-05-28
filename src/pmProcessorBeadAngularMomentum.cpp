/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmProcessorBeadAngularMomentum.cpp: implementation of the pmProcessorBeadAngularMomentum class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimNumericFlags.h"
#include "pmProcessorBeadAngularMomentum.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmProcessorBeadAngularMomentum::m_Type = "ProcessorBeadAngularMomentum";

const zString pmProcessorBeadAngularMomentum::GetType()
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
	mpmMessage* Create() {return new pmProcessorBeadAngularMomentum();}

	const zString id = pmProcessorBeadAngularMomentum::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pmProcessorBeadAngularMomentum::pmProcessorBeadAngularMomentum() : mpmMessage(), m_BeadTotal(0),
                                                         m_XAngMom(0.0), m_YAngMom(0.0), m_ZAngMom(0.0)
{
	m_vBeadTotal.clear();
	m_vXAngMom.clear();
	m_vYAngMom.clear();
	m_vZAngMom.clear();
}

pmProcessorBeadAngularMomentum::pmProcessorBeadAngularMomentum(const pmProcessorBeadAngularMomentum& oldMessage) : mpmMessage(oldMessage),
                                                     m_XAngMom(oldMessage.m_XAngMom),
                                                     m_YAngMom(oldMessage.m_YAngMom),
                                                     m_ZAngMom(oldMessage.m_ZAngMom)
{
	m_vBeadTotal = oldMessage.m_vBeadTotal;
	m_vXAngMom   = oldMessage.m_vXAngMom;
	m_vYAngMom   = oldMessage.m_vYAngMom;
	m_vZAngMom   = oldMessage.m_vZAngMom;

}

pmProcessorBeadAngularMomentum::~pmProcessorBeadAngularMomentum()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmProcessorBeadAngularMomentum::put(zOutStream& os) const
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

zInStream& pmProcessorBeadAngularMomentum::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmProcessorBeadAngularMomentum::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmProcessorBeadAngularMomentum::GetMessage() const
{
	return new pmProcessorBeadAngularMomentum(*this);
}

// Function to set the message data. In this case, it is the total number of beads
// in a processor's Space and the CM velocity of all of its beads. Note that the
// data are validated in the Validate() function not here. Also, P0 MUST set its own
// data before receiving messages from PN.

void pmProcessorBeadAngularMomentum::SetMessageData(long beadTotal, double xrv, double yrv, double zrv)
{
	m_BeadTotal = beadTotal;
	m_XAngMom   = xrv;
	m_YAngMom   = yrv;
	m_ZAngMom   = zrv;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We just check that the sender rank is valid and that there are some beads in the
// processor's Space. If the bead total is zero, the CM velocity will be undefined.

bool pmProcessorBeadAngularMomentum::Validate()
{
    bool bSuccess = true;

    if(m_SenderRank < 0 || m_SenderRank >= GetWorld() || m_BeadTotal < 0)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for MPI_Send() sent to Processor 0. In this class, each PN sends a
// message to P0 containing the number of beads in its Space and their total angular 
// momentum.
//
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmProcessorBeadAngularMomentum::SendP0()
{
#if SimMPS == SimulationEnabled

    if(!IsProcessZero())
    {
		// Declare a local variable that is an MPI_Datatype to hold the message;  
		// note that if MPI is not compiled in, this data type defaults 
		// to a long.

		MPI_Datatype  MsgDataType;

		// Pack the data into a derived MPI data type 

		BuildDerivedType(&MsgDataType);

        // and send it to all the other processors: note that we assume that
        // the sending processor is P0, so we start the loop at processor rank 1.

        long dest = 0;   // Local variables used by MPI_Send, MPI_Recv
        long tag  = 1;
		
        MPI_Send(&m_BeadTotal, 1, MsgDataType, dest, tag, MPI_COMM_WORLD);
	}
    else
    {
		// This branch should not be seen	
		std::cout << "Error: Processor 0 trying to call SendP0() " << zEndl;
    }

#endif
}

// Wrapper for the Mpi function:
// MPI_Recv(message, length, MPI_CHAR, source, tag, MPI_COMM_WORLD, status);

// This function is called by  P0 to receive the bead total from PN.

void pmProcessorBeadAngularMomentum::Receive()
{
#if SimMPS == SimulationEnabled

// Only P0 should respond to this message.

    if(IsProcessZero())
    {
		MPI_Datatype  MsgDataType;

// Pack the data into a derived MPI data type

		BuildDerivedType(&MsgDataType);

// Clear the containers and store P0 data first - this assumes that P0 has already 
// stored its own data in this object.

		ZeroAngMom();

		m_vBeadTotal.push_back(m_BeadTotal);
		m_vXAngMom.push_back(m_XAngMom);
		m_vYAngMom.push_back(m_YAngMom);
		m_vZAngMom.push_back(m_ZAngMom);

// Collect data from all PN in turn: this message is only received by P0 which
// knows its own bead total without a message.
				
        long tag = 1; // Local variables used by MPI_Send, MPI_Recv
        MPI_Status status;

        for(long source = 1; source < GetWorld(); source++)
		{
			MPI_Recv(&m_BeadTotal, 1, MsgDataType, source, tag, MPI_COMM_WORLD, &status);

			m_vBeadTotal.push_back(m_BeadTotal);
			m_vXAngMom.push_back(m_XAngMom);
			m_vYAngMom.push_back(m_YAngMom);
			m_vZAngMom.push_back(m_ZAngMom);
		}
    }

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmProcessorBeadAngularMomentum::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending via 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

// This message consists of a single long and three doubles.

void pmProcessorBeadAngularMomentum::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
#if SimMPS == SimulationEnabled

    // Store the number of elements of each type in the blocks

    const long dataTotal = 4;

    int block_lengths[4];

    for(long i=0; i<dataTotal; i++)
    {
        block_lengths[i] = 1;  
    }

    // Define the types of the blocks

    MPI_Datatype typelist[4];

    typelist[0] = MPI_LONG;
    typelist[1] = MPI_DOUBLE;
    typelist[2] = MPI_DOUBLE;
    typelist[3] = MPI_DOUBLE;

    // Define the displacement of each element from the start of the new type

    MPI_Aint displacements[4];

    // Local variables used to calculate the start of each element
    MPI_Aint startAddress;
    MPI_Aint address;

    MPI_Get_address(&m_BeadTotal, &startAddress);
    displacements[0] = 0;

    MPI_Get_address(&m_XAngMom, &address);
    displacements[1] = address - startAddress;

    MPI_Get_address(&m_YAngMom, &address);
    displacements[2] = address - startAddress;

    MPI_Get_address(&m_ZAngMom, &address);
    displacements[3] = address - startAddress;

    // Build the derived datatype and commit it

    MPI_Type_create_struct(dataTotal, block_lengths, displacements, typelist, pMsgDataType);
    MPI_Type_commit(pMsgDataType);

#endif

}

// Accessor functions for P0 to get the data passed from all the PN.

long pmProcessorBeadAngularMomentum::GetBeadTotal(long pn) const
{
	if(pn >= 0 && pn < GetWorld())
	{
		return m_vBeadTotal.at(pn);
	}
	else
	{
		return 0;
	}
}

double pmProcessorBeadAngularMomentum::GetXAngMom(long pn) const
{
	if(pn >= 0 && pn < GetWorld())
	{
		return m_vXAngMom.at(pn);
	}
	else
	{
		return 0;
	}
}

double pmProcessorBeadAngularMomentum::GetYAngMom(long pn) const
{
	if(pn >= 0 && pn < GetWorld())
	{
		return m_vYAngMom.at(pn);
	}
	else
	{
		return 0;
	}
}

double pmProcessorBeadAngularMomentum::GetZAngMom(long pn) const
{
	if(pn >= 0 && pn < GetWorld())
	{
		return m_vZAngMom.at(pn);
	}
	else
	{
		return 0;
	}
}

// Function to clear all the containers preparatory to storing a new set of data.

void pmProcessorBeadAngularMomentum::ZeroAngMom()
{
	m_vBeadTotal.clear();
	m_vXAngMom.clear();
	m_vYAngMom.clear();
	m_vZAngMom.clear();
}

long pmProcessorBeadAngularMomentum::GetWorldBeadTotal() const
{
	return accumulate(m_vBeadTotal.begin(), m_vBeadTotal.end(), 0);
}

double pmProcessorBeadAngularMomentum::GetWorldXAngMom() const
{
	return accumulate(m_vXAngMom.begin(), m_vXAngMom.end(), 0.0);
}

double pmProcessorBeadAngularMomentum::GetWorldYAngMom() const
{
	return accumulate(m_vYAngMom.begin(), m_vYAngMom.end(), 0.0);
}

double pmProcessorBeadAngularMomentum::GetWorldZAngMom() const
{
	return accumulate(m_vZAngMom.begin(), m_vZAngMom.end(), 0.0);
}
