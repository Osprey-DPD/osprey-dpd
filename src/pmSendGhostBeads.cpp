/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSendGhostBeads.cpp: implementation of the pmSendGhostBeads class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSendGhostBeads.h"
#include "Bead.h"
#include "CurrentState.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSendGhostBeads::m_Type = "SendGhostBeads";

const zString pmSendGhostBeads::GetType()
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
	mpmMessage* Create() {return new pmSendGhostBeads();}

	const zString id = pmSendGhostBeads::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We create a fixed number of empty bead instances here so that they are
// ready for use by the first message instance created. As we do not know
// how many beads are likely to be transferred per message until we have
// averaged over some messages, we just create a typical number set by the
// static member m_InitialEmptyBeads.
//
// Note that the pointers to the MPI_Request objects are NOT owned by this 
// message class, but are created/destroyed by the calling object.

pmSendGhostBeads::pmSendGhostBeads() : mpmMessage(), m_pSendRequest(0),
                                         m_pRecRequest(0), m_VisibleFlags(0),
                                         m_RecTotal(0), m_pBead(0)
{
    for(long i=0; i<m_MaxMsgBeads; i++)
    {
        m_Ids[i]          = 0;
        m_Types[i]        = 0;
        m_X[i]            = 0.0;
        m_Y[i]            = 0.0;
        m_Z[i]            = 0.0;
        m_UnPBCX[i]       = 0.0;
        m_UnPBCY[i]       = 0.0;
        m_UnPBCZ[i]       = 0.0;
    }

    m_lBeads.clear();
    m_lRecBeads.clear();
    m_lEmptyBeads.clear();

    // Create the initial set of empty beads used to receive incoming bead
    // coordinates.

    for(long j=0; j<m_InitialEmptyBeads; j++)
    {
        m_lEmptyBeads.push_back(new CBead());
    }
}

// Copy constructor.

pmSendGhostBeads::pmSendGhostBeads(const pmSendGhostBeads& oldMessage) : mpmMessage(oldMessage),
                                               m_pSendRequest(oldMessage.m_pSendRequest),
                                               m_pRecRequest(oldMessage.m_pRecRequest),
                                               m_RecTotal(oldMessage.m_RecTotal),
											   m_pBead(oldMessage.m_pBead)
{
	// We omit the radius parameter for now as it is hard-wired to 0.5 for all beads.

    for(long i=0; i<m_MaxMsgBeads; i++)
    {
        m_Ids[i]		  = oldMessage.m_Ids[i];
        m_Types[i]		  = oldMessage.m_Types[i];
        m_X[i]			  = oldMessage.m_X[i];
        m_Y[i]			  = oldMessage.m_Y[i];
        m_Z[i]			  = oldMessage.m_Z[i];
        m_UnPBCX[i]       = oldMessage.m_UnPBCX[i];
        m_UnPBCY[i]       = oldMessage.m_UnPBCY[i];
        m_UnPBCZ[i]       = oldMessage.m_UnPBCZ[i];
    }

    m_lBeads      = oldMessage.m_lBeads;
    m_lRecBeads   = oldMessage.m_lRecBeads;
    m_lEmptyBeads = oldMessage.m_lEmptyBeads;
}

// Destructor. If any empty bead instances exist we delete them here as they are not
// included the initial state's bead set.

pmSendGhostBeads::~pmSendGhostBeads()
{
	// I have to determine who actually owns the beads before I delete them here as it 
	// currently causes a crash.

//    if(!m_lEmptyBeads.empty())
//    {
//        for(BeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lEmptyBeads.end(); iterBead++)
//        {
//            delete *iterBead;
//        }
//        m_lEmptyBeads.clear();
//    }
}

// Private helper function to create a batch of empty bead instances. We just
// append the new instances to the empty bead container.

bool pmSendGhostBeads::CreateEmptyBeads()
{
    for(long j=0; j<m_InitialEmptyBeads; j++)
    {
        m_lEmptyBeads.push_back(new CBead());
    }

	return true;
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSendGhostBeads::put(zOutStream& os) const
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

zInStream& pmSendGhostBeads::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSendGhostBeads::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSendGhostBeads::GetMessage() const
{
	return new pmSendGhostBeads(*this);
}

// ****************************************
// Function to store the set of beads whose coordinates form the message payload.
// The payload contains the data needed by the receiving processor in order to 
// be able to calculate the non-bonded forces between its own beads and those in
// the sending processor. This data is:
// 
// Bead id, type and interaction radius (radius not included yet)
// Bead position coordinates
// Bead velocity coordinates

void pmSendGhostBeads::SetMessageData(BeadList lBeads)
{
    m_lBeads = lBeads;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We check that the number of beads to be sent will fit into the fixed-size arrays
// used by MPI, that the bead id is non-negative (indicating that the bead is valid,
// and not an empty bead instance) and that the beads' position coordinates are 
// not negative.

bool pmSendGhostBeads::Validate()
{
    bool bSuccess = true;

    if(GetSentBeadTotal() > 0)
    {
        if(GetSentBeadTotal() < m_MaxMsgBeads)
        {
            for(cBeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); iterBead++)
            {
                if((*iterBead)->GetId()   < 0   ||
				   (*iterBead)->GetXPos() < 0.0 || (*iterBead)->GetYPos() < 0.0 || (*iterBead)->GetZPos() < 0.0)
                {
				    std::cout << "Proc " << GetRank() << " has " << GetSentBeadTotal() << " beads and pmSendGhostBeads validation failed with: " << (*iterBead)->GetId() << " " << (*iterBead)->GetXPos() << " " <<  (*iterBead)->GetYPos() << " " <<  (*iterBead)->GetZPos() << zEndl;
                    bSuccess = false;
                }
            }
        }
        else
        {
	        std::cout << "Proc " << GetRank() << " pmSendGhostBeads failed validation with " << GetSentBeadTotal() << " beads" << zEndl;
            bSuccess = false;
        }
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Isend(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSendGhostBeads::ISend(long procId, MsgRequestPtr pRequest)
{
#if SimMPS == SimulationEnabled

    m_pSendRequest = pRequest;

    // Store the bead coordinates in local arrays for the MPI call and add the
    // newly-empty instance to the local empty beads store. We set the empty 
	// bead's id to a negative value to indicate that this is not a valid bead
	// instance.

    long i = 0;

    for(cBeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); iterBead++)
    {
       m_Ids[i]		    = (*iterBead)->GetId();
       m_DisplayIds[i]	= CCurrentState::GetBeadDisplayId((*iterBead)->GetId());
	   if((*iterBead)->GetVisible())
	   {
		   m_Visibility[i]	= 1;
	   }
	   else
	   {
		   m_Visibility[i]	= 0;
	   }
       m_Types[i]	    = (*iterBead)->GetType();
       m_X[i]		    = (*iterBead)->GetXPos();
       m_Y[i]		    = (*iterBead)->GetYPos();
       m_Z[i]		    = (*iterBead)->GetZPos();
       m_UnPBCX[i]      = (*iterBead)->GetunPBCXPos();
       m_UnPBCY[i]      = (*iterBead)->GetunPBCYPos();
       m_UnPBCZ[i]      = (*iterBead)->GetunPBCZPos();

	   (*iterBead)->SetId(-1);
	   m_lEmptyBeads.push_back(*iterBead);
       ++i;
    }


    // First, pack the number of beads

    int  position;
    position = 0;
    MPI_Pack(&i, 1, MPI_LONG, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);

    // Now pack the data into the message payload

    MPI_Pack(m_Ids,        i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Types,      i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_X,          i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Y,          i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Z,          i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_UnPBCX,     i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_UnPBCY,     i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_UnPBCZ,     i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);

    MPI_Isend(m_SendBuffer, m_MaxMsgBuffer, MPI_PACKED, procId, m_Tag, MPI_COMM_WORLD, pRequest);
 
#endif
}

// Wrapper for the Mpi function:
// MPI_Irecv(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSendGhostBeads::IReceive(long procId, MsgRequestPtr pRequest)
{
#if SimMPS == SimulationEnabled

    m_pRecRequest = pRequest;

    MPI_Irecv(m_RecBuffer, m_MaxMsgBuffer, MPI_PACKED, procId, m_Tag, MPI_COMM_WORLD, pRequest);
#endif
}

// We retrieve the bead coordinates from the message and fill up as many empty bead
// instances as we need from the empty bead store. If we run out of these instances,
// we create a new batch.
//  
// We do not check for validity here as we assume that the sending object has 
// already done it. Note that this function does not propagate the data out into 
// the code, it only fills up the receiving message instance. Each messaging class 
// is responsible for providing further functions to pass the data to the rest of the code.

bool pmSendGhostBeads::Wait(MsgRequestPtr pRequest)
{
    bool bSuccess = false;

#if SimMPS == SimulationEnabled

    MPI_Status status;

    MPI_Wait(pRequest, &status);

    // If this is the receive call unpack the data

    if(pRequest == m_pSendRequest)
    {
        bSuccess = true;
		if(GetSentBeadTotal() > 0)
		{
//			std::cout << "Processor " << GetRank() << " has executed Wait and sent " << GetSentBeadTotal() << " beads" << zEndl;
		}
	}
    else if(pRequest == m_pRecRequest)
    {
        bSuccess = true;

        int  position;
        position = 0;
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, &m_RecTotal, 1, MPI_LONG, MPI_COMM_WORLD);

		if(GetRecBeadTotal() > 0)
		{
//			std::cout << "Processor " << GetRank() << " has executed Wait and received " << GetRecBeadTotal() << " planar Face beads, and has " << GetEmptyBeadTotal() << " empty beads" << zEndl;
		}

        // Now unpack the message payload

        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Ids,        m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Types,      m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_X,          m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Y,          m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Z,          m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_UnPBCX,     m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_UnPBCY,     m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_UnPBCZ,     m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);

        // and copy the data into empty bead instances held by this message instance.
		// If there are not sufficient empty bead instances available, we create 
		// another batch before starting to fill them up. If the memory allocation fails
		// then we return false to indicate that the transfer failed.

		while(m_RecTotal > GetEmptyBeadTotal() && bSuccess)
		{
			bSuccess = CreateEmptyBeads();
			std::cout << "Processor " << GetRank()  << " has to create new empty beads (Face) for " << m_RecTotal << " received instances, now holding " << GetEmptyBeadTotal() << zEndl;
	    }

		if(bSuccess)
		{
			m_lRecBeads.clear();

			for(long i=0; i<m_RecTotal; i++)
			{
				m_pBead = m_lEmptyBeads.back();
				m_lEmptyBeads.pop_back();

				m_pBead->SetId(m_Ids[i]);
				m_pBead->SetNotMovable();
				m_pBead->SetType(m_Types[i]);
				m_pBead->SetXPos(m_X[i]);
				m_pBead->SetYPos(m_Y[i]);
				m_pBead->SetZPos(m_Z[i]);
				m_pBead->SetunPBCXPos(m_UnPBCX[i]);
				m_pBead->SetunPBCYPos(m_UnPBCY[i]);
				m_pBead->SetunPBCZPos(m_UnPBCZ[i]);

				m_lRecBeads.push_back(m_pBead);
			}
		}    
    }
#endif

    return bSuccess;
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmSendGhostBeads::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}



