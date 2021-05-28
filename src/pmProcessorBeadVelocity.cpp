/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmProcessorBeadVelocity.cpp: implementation of the pmProcessorBeadVelocity class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimNumericFlags.h"
#include "pmProcessorBeadVelocity.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmProcessorBeadVelocity::m_Type = "ProcessorBeadVelocity";

const zString pmProcessorBeadVelocity::GetType()
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
	mpmMessage* Create() {return new pmProcessorBeadVelocity();}

	const zString id = pmProcessorBeadVelocity::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pmProcessorBeadVelocity::pmProcessorBeadVelocity() : mpmMessage(), m_BeadTotal(0),
                                                     m_XVel(0.0), m_YVel(0.0), m_ZVel(0.0)
{
	m_Tag = 1;  // Tag stored in the base class.

	m_vBeadTotal.clear();
	m_vXVelTotal.clear();
	m_vYVelTotal.clear();
	m_vZVelTotal.clear();
}

pmProcessorBeadVelocity::pmProcessorBeadVelocity(const pmProcessorBeadVelocity& oldMessage) : mpmMessage(oldMessage),
                                                     m_XVel(oldMessage.m_XVel),
                                                     m_YVel(oldMessage.m_YVel),
                                                     m_ZVel(oldMessage.m_ZVel)
{
	m_vBeadTotal = oldMessage.m_vBeadTotal;
	m_vXVelTotal = oldMessage.m_vXVelTotal;
	m_vYVelTotal = oldMessage.m_vYVelTotal;
	m_vZVelTotal = oldMessage.m_vZVelTotal;

}

pmProcessorBeadVelocity::~pmProcessorBeadVelocity()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmProcessorBeadVelocity::put(zOutStream& os) const
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

zInStream& pmProcessorBeadVelocity::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmProcessorBeadVelocity::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmProcessorBeadVelocity::GetMessage() const
{
	return new pmProcessorBeadVelocity(*this);
}

// Function to set the message data. In this case, it is the total number of beads
// in a processor's Space and the velocity of all of its beads. Note that the
// data are validated in the Validate() function not here. Also, P0 MUST set its own
// data before receiving messages from PN.

void pmProcessorBeadVelocity::SetMessageData(long beadTotal, double xv, double yv, double zv)
{
	m_BeadTotal = beadTotal;
	m_XVel    = xv;
	m_YVel    = yv;
	m_ZVel    = zv;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We just check that the sender rank is valid and that there are some beads in the
// processor's Space. If the bead total is zero, the  velocity will be undefined.

bool pmProcessorBeadVelocity::Validate()
{
    bool bSuccess = true;

    if(m_SenderRank < 0 || m_SenderRank >= GetWorld() || m_BeadTotal < 0)
    {
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for MPI_Send() sent to Processor 0. In this class, each PN sends a
// message to P0 containing the number of beads in its Space and their  velocity.
//
// MPI_Send(message, length, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

void pmProcessorBeadVelocity::SendP0()
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

        // and send it to P0. The destination parameter is hardwired to 0 here.
		// And the tag is stored in the base class.
		
        MPI_Send(&m_BeadTotal, 1, MsgDataType, 0, m_Tag, MPI_COMM_WORLD);
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

void pmProcessorBeadVelocity::Receive()
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

		ZeroVel();

		m_vBeadTotal.push_back(m_BeadTotal);
		m_vXVelTotal.push_back(m_XVel);
		m_vYVelTotal.push_back(m_YVel);
		m_vZVelTotal.push_back(m_ZVel);

// Collect data from all PN in turn: this message is only received by P0 which
// knows its own bead total without a message. The tag parameter must be the same
// as in the send routine.
				
        MPI_Status status;

        for(long source = 1; source < GetWorld(); source++)
		{
			MPI_Recv(&m_BeadTotal, 1, MsgDataType, source, m_Tag, MPI_COMM_WORLD, &status);

			m_vBeadTotal.push_back(m_BeadTotal);
			m_vXVelTotal.push_back(m_XVel);
			m_vYVelTotal.push_back(m_YVel);
			m_vZVelTotal.push_back(m_ZVel);
		}
    }

#endif
}

// Stub function for the overloaded Receive function to avoid compiler warnings on the Hermit1.hww.de platform.

void pmProcessorBeadVelocity::Receive(long procId)
{
#if SimMPS == SimulationEnabled

#endif
}

// PVF to assemble the disparate data items into a struct suitable for sending via 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

// This message consists of a single long and three doubles.

void pmProcessorBeadVelocity::BuildDerivedType(MsgDataTypePtr pMsgDataType)
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

    MPI_Get_address(&m_XVel, &address);
    displacements[1] = address - startAddress;

    MPI_Get_address(&m_YVel, &address);
    displacements[2] = address - startAddress;

    MPI_Get_address(&m_ZVel, &address);
    displacements[3] = address - startAddress;

    // Build the derived datatype and commit it

    MPI_Type_create_struct(dataTotal, block_lengths, displacements, typelist, pMsgDataType);
    MPI_Type_commit(pMsgDataType);

#endif

}

// Accessor functions for P0 to get the data passed from all the PN.

long pmProcessorBeadVelocity::GetBeadTotal(long pn) const
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

double pmProcessorBeadVelocity::GetXVel(long pn) const
{
	if(pn >= 0 && pn < GetWorld())
	{
		return m_vXVelTotal.at(pn);
	}
	else
	{
		return 0;
	}
}

double pmProcessorBeadVelocity::GetYVel(long pn) const
{
	if(pn >= 0 && pn < GetWorld())
	{
		return m_vYVelTotal.at(pn);
	}
	else
	{
		return 0;
	}
}

double pmProcessorBeadVelocity::GetZVel(long pn) const
{
	if(pn >= 0 && pn < GetWorld())
	{
		return m_vZVelTotal.at(pn);
	}
	else
	{
		return 0;
	}
}

// Function to clear all the containers preparatory to storing a new set of data.

void pmProcessorBeadVelocity::ZeroVel()
{
	m_vBeadTotal.clear();
	m_vXVelTotal.clear();
	m_vYVelTotal.clear();
	m_vZVelTotal.clear();
}

// Function to return the total number of beads in the processor World including P0.

long pmProcessorBeadVelocity::GetWorldBeadTotal() const
{
	return accumulate(m_vBeadTotal.begin(), m_vBeadTotal.end(), 0);
}

// Functions to return the total centre of mass velocity in the processor World.

double pmProcessorBeadVelocity::GetWorldXVel() const
{
	return accumulate(m_vXVelTotal.begin(), m_vXVelTotal.end(), 0.0);
}

double pmProcessorBeadVelocity::GetWorldYVel() const
{
	return accumulate(m_vYVelTotal.begin(), m_vYVelTotal.end(), 0.0);
}

double pmProcessorBeadVelocity::GetWorldZVel() const
{
	return accumulate(m_vZVelTotal.begin(), m_vZVelTotal.end(), 0.0);
}
