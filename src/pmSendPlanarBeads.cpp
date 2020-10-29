/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSendPlanarBeads.cpp: implementation of the pmSendPlanarBeads class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSendPlanarBeads.h"
#include "Bead.h"
#include "Polymer.h"
#include "CurrentState.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static container for empty bead instances used by all instances of this message class.
// We make it static because the parallel SimBox needs multiple instances of the message for its Faces/Edges/Corners.

BeadList  pmSendPlanarBeads::m_lEmptyBeads;

// Static member variable containing the identifier for this message. 

const zString pmSendPlanarBeads::m_Type = "SendPlanarBeads";

const zString pmSendPlanarBeads::GetType()
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
	mpmMessage* Create() {return new pmSendPlanarBeads();}

	const zString id = pmSendPlanarBeads::GetType();

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

pmSendPlanarBeads::pmSendPlanarBeads() : mpmMessage(), m_pSendRequest(0),
                                         m_pRecRequest(0), 
                                         m_RecTotal(0), m_pBead(0)
{
    for(long i=0; i<m_MaxMsgBeads; i++)
    {
        m_Ids[i]          = 0;
        m_DisplayIds[i]   = 0;
        m_Visibility[i]   = 0;
        m_Types[i]        = 0;
        m_Locations[i]    = 0;
        m_FirstBondIds[i] = 0;
        m_FirstBPIds[i]   = 0;
        m_PolyIds[i]      = 0;
        m_PolyTypes[i]    = 0;
        m_Radius[i]       = 0.0;    // Radius parameter not used yet
        m_X[i]            = 0.0;
        m_Y[i]            = 0.0;
        m_Z[i]            = 0.0;
        m_VX[i]           = 0.0;
        m_VY[i]           = 0.0;
        m_VZ[i]           = 0.0;
        m_OldVX[i]        = 0.0;
        m_OldVY[i]        = 0.0;
        m_OldVZ[i]        = 0.0;
        m_OldFX[i]        = 0.0;
        m_OldFY[i]        = 0.0;
        m_OldFZ[i]        = 0.0;
/*
        m_OldX[i]         = 0.0;
        m_OldY[i]         = 0.0;
        m_OldZ[i]         = 0.0;
        m_UnPBCX[i]       = 0.0;
        m_UnPBCY[i]       = 0.0;
        m_UnPBCZ[i]       = 0.0;
        m_InitialX[i]     = 0.0;
        m_InitialY[i]     = 0.0;
        m_InitialZ[i]     = 0.0;
        m_dX[i]           = 0.0;
        m_dY[i]           = 0.0;
        m_dZ[i]           = 0.0;  */
    }

    m_lBeads.clear();
    m_lRecBeads.clear();
    m_vBeadLocations.clear();
    m_vFirstBondIds.clear();
    m_vFirstBPIds.clear();
    m_vPolymerIds.clear();
	m_vPolymerTypes.clear();
	
}

// Copy constructor.

pmSendPlanarBeads::pmSendPlanarBeads(const pmSendPlanarBeads& oldMessage) : mpmMessage(oldMessage),
                                               m_pSendRequest(oldMessage.m_pSendRequest),
                                               m_pRecRequest(oldMessage.m_pRecRequest),
                                               m_RecTotal(oldMessage.m_RecTotal),
											   m_pBead(oldMessage.m_pBead)
{
	// We omit the radius parameter for now as it is hard-wired to 0.5 for all beads.

    for(long i=0; i<m_MaxMsgBeads; i++)
    {
        m_Ids[i]		  = oldMessage.m_Ids[i];
        m_DisplayIds[i]	  = oldMessage.m_DisplayIds[i];
        m_Visibility[i]	  = oldMessage.m_Visibility[i];
        m_Types[i]		  = oldMessage.m_Types[i];
        m_Locations[i]	  = oldMessage.m_Locations[i];
        m_FirstBondIds[i] = oldMessage.m_FirstBondIds[i];
        m_FirstBPIds[i]   = oldMessage.m_FirstBPIds[i];
        m_PolyIds[i]      = oldMessage.m_PolyIds[i];
        m_PolyTypes[i]    = oldMessage.m_PolyTypes[i];
        m_Radius[i]       = oldMessage.m_Radius[i];
        m_X[i]			  = oldMessage.m_X[i];
        m_Y[i]			  = oldMessage.m_Y[i];
        m_Z[i]			  = oldMessage.m_Z[i];
        m_VX[i]			  = oldMessage.m_VX[i];
        m_VY[i]			  = oldMessage.m_VY[i];
        m_VZ[i]			  = oldMessage.m_VZ[i];
        m_OldVX[i]        = oldMessage.m_OldVX[i];
        m_OldVY[i]        = oldMessage.m_OldVY[i];
        m_OldVZ[i]        = oldMessage.m_OldVZ[i];
        m_OldFX[i]        = oldMessage.m_OldFX[i];
        m_OldFY[i]        = oldMessage.m_OldFY[i];
        m_OldFZ[i]        = oldMessage.m_OldFZ[i];
/*		
        m_OldX[i]         = oldMessage.m_OldX[i];
        m_OldY[i]         = oldMessage.m_OldY[i];
        m_OldZ[i]         = oldMessage.m_OldZ[i];
        m_UnPBCX[i]       = oldMessage.m_UnPBCX[i];
        m_UnPBCY[i]       = oldMessage.m_UnPBCY[i];
        m_UnPBCZ[i]       = oldMessage.m_UnPBCZ[i];
        m_InitialX[i]     = oldMessage.m_InitialX[i];
        m_InitialY[i]     = oldMessage.m_InitialY[i];
        m_InitialZ[i]     = oldMessage.m_InitialZ[i];
        m_dX[i]           = oldMessage.m_dX[i];
        m_dY[i]           = oldMessage.m_dY[i];
        m_dZ[i]           = oldMessage.m_dZ[i];  */
    }

    m_lBeads           = oldMessage.m_lBeads;
    m_lRecBeads        = oldMessage.m_lRecBeads;
	m_vBeadLocations   = oldMessage.m_vBeadLocations;
	m_vFirstBondIds    = oldMessage.m_vFirstBondIds;
	m_vFirstBPIds      = oldMessage.m_vFirstBPIds;
	m_vPolymerIds      = oldMessage.m_vPolymerIds;
	m_vPolymerTypes    = oldMessage.m_vPolymerTypes;
	
}

// Destructor. If any empty bead instances exist we delete them here as they are not
// included the initial state's bead set.

pmSendPlanarBeads::~pmSendPlanarBeads()
{
	// I have to determine who actually owns the beads before I delete them here as it 
	// currently causes a crash.

    if(!m_lEmptyBeads.empty())
    {
//	    std::cout << "pmSendPlanarBeads destroying " << m_lEmptyBeads.size() << " empty bead instances" << zEndl;
		
        for(BeadListIterator iterBead=m_lEmptyBeads.begin(); iterBead!=m_lEmptyBeads.end(); iterBead++)
        {
            delete *iterBead;
        }
        m_lEmptyBeads.clear();
    }
}

// Static function to empty the container of empty bead instances used when beads arrive in this processor's Space.
// Because the container is static, we do not call it in the constructor, but the owning class has to call it.

void pmSendPlanarBeads::InitialiseEmptyBeadContainer()
{
    m_lEmptyBeads.clear();
	
    for(long j=0; j<m_InitialEmptyBeads; j++)
    {
        m_lEmptyBeads.push_back(new CBead());
    }
}


// Private helper function to create a batch of empty bead instances. We just
// append the new instances to the empty bead container.

bool pmSendPlanarBeads::CreateEmptyBeads()
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

zOutStream& pmSendPlanarBeads::put(zOutStream& os) const
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

zInStream& pmSendPlanarBeads::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSendPlanarBeads::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSendPlanarBeads::GetMessage() const
{
	return new pmSendPlanarBeads(*this);
}

// ****************************************
// Function to store the set of beads whose coordinates form the message payload.
// The payload contains the data needed by the receiving processor in order to 
// be able to calculate the non-bonded forces between its own beads and those in
// the sending processor.

void pmSendPlanarBeads::SetMessageData(BeadList lBeads)
{
    m_lBeads = lBeads;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We check that the number of beads to be sent will fit into the fixed-size arrays
// used by MPI, that the bead id is non-negative (indicating that the bead is valid,
// and not an empty bead instance) and that the beads' position coordinates are 
// not negative.

bool pmSendPlanarBeads::Validate()
{
    bool bSuccess = true;

#if EnableParallelSimBox == SimMPSEnabled

    if(GetSentBeadTotal() > 0)
    {
        if(GetSentBeadTotal() < m_MaxMsgBeads)
        {
            for(cBeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); iterBead++)
            {
                if((*iterBead)->GetId() < 0 || !(*iterBead)->GetPolymer() || (*iterBead)->GetPolymer()->GetId() < 0 || 
				   (*iterBead)->GetXPos() < 0.0 || (*iterBead)->GetYPos() < 0.0 || (*iterBead)->GetZPos() < 0.0)
                {
				    if((*iterBead)->GetPolymer())
					{
			            std::cout << "Error in pmSendPlanarBeads: negative bead/polymer id or coordinate " << (*iterBead)->GetId() << " " << (*iterBead)->GetType() << " " << (*iterBead)->GetPolymer()->GetId() << " " << (*iterBead)->GetPolymer()->GetType() << " " << (*iterBead)->GetPolymer()->GetSize() << zEndl;
                    }
					else
					{
			            std::cout << "Error in pmSendPlanarBeads: null owning polymer pointer, negative bead/polymer id or coordinate " << (*iterBead)->GetId() << " " << (*iterBead)->GetType() << " " << (*iterBead)->GetPolymer() << zEndl;
					}
					std::cout << (*iterBead)->GetXPos() << " " << (*iterBead)->GetYPos() << " " << (*iterBead)->GetZPos() << zEndl;
                    bSuccess = false;
				}
            }
        }
        else
        {
	        std::cout << "Proc " << GetRank() << " pmSendPlanarBeads failed validation with " << GetSentBeadTotal() << " beads and limit " << m_MaxMsgBeads << zEndl;
            bSuccess = false;
        }
    }

#endif

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Isend(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSendPlanarBeads::ISend(long procId, MsgRequestPtr pRequest)
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
       m_Ids[i]		      = (*iterBead)->GetId();
       m_DisplayIds[i]	  = CCurrentState::GetBeadDisplayId((*iterBead)->GetId());
	   if((*iterBead)->GetVisible())
	   {
		   m_Visibility[i]	= 1;
	   }
	   else
	   {
		   m_Visibility[i]	= 0;
	   }
       m_Types[i]	      = (*iterBead)->GetType();
       m_Locations[i]	  = (*iterBead)->GetPolymer()->GetBeadLocation(*iterBead);   // NB A value of -1 is an error: it means the polymer has no beads

       if(m_Locations[i] == -1)
	   {
	       std::cout << "ERROR in pmSendPlanarBeads: bead " << (*iterBead)->GetId() << " has -1 location in polymer " << (*iterBead)->GetPolymer()->GetId() << zEndl;
	   }


       m_FirstBondIds[i]  = (*iterBead)->GetPolymer()->GetFirstBondId();
       m_FirstBPIds[i]    = (*iterBead)->GetPolymer()->GetFirstBondPairId();
       m_PolyIds[i]	      = (*iterBead)->GetPolymer()->GetId();
       m_PolyTypes[i]	  = (*iterBead)->GetPolymer()->GetType();
       m_X[i]		      = (*iterBead)->GetXPos();
       m_Y[i]		      = (*iterBead)->GetYPos();
       m_Z[i]		      = (*iterBead)->GetZPos();
       m_VX[i]		      = (*iterBead)->GetXMom();
       m_VY[i]		      = (*iterBead)->GetYMom();
       m_VZ[i]		      = (*iterBead)->GetZMom();
       m_OldVX[i]         = (*iterBead)->GetOldXMom();
       m_OldVY[i]         = (*iterBead)->GetOldYMom();
       m_OldVZ[i]         = (*iterBead)->GetOldZMom();
       m_OldFX[i]         = (*iterBead)->GetOldXForce();
       m_OldFY[i]         = (*iterBead)->GetOldYForce();
       m_OldFZ[i]         = (*iterBead)->GetOldZForce();
/*
       m_OldX[i]          = (*iterBead)->GetOldXPos();
       m_OldY[i]          = (*iterBead)->GetOldYPos();
       m_OldZ[i]          = (*iterBead)->GetOldZPos();

       m_UnPBCX[i]        = (*iterBead)->GetunPBCXPos();
       m_UnPBCY[i]        = (*iterBead)->GetunPBCYPos();
       m_UnPBCZ[i]        = (*iterBead)->GetunPBCZPos();
       m_InitialX[i]      = (*iterBead)->GetInitialXPos();
       m_InitialY[i]      = (*iterBead)->GetInitialYPos();
       m_InitialZ[i]      = (*iterBead)->GetInitialZPos();
	   m_dX[i]            = (*iterBead)->GetdXPos();
	   m_dY[i]            = (*iterBead)->GetdYPos();
	   m_dZ[i]            = (*iterBead)->GetdZPos();  */

//	   (*iterBead)->SetId(-1);
	   m_lEmptyBeads.push_back(*iterBead);
       ++i;
    }
	
    // First, pack the number of beads

    int  position;
    position = 0;
	
    MPI_Pack(&i, 1, MPI_LONG, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);

    // Now pack the data into the message payload

    MPI_Pack(m_Ids,            i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_DisplayIds,     i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Visibility,     i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Types,          i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Locations,      i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_FirstBondIds,   i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_FirstBPIds,     i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_PolyIds,        i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_PolyTypes,      i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_X,              i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Y,              i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Z,              i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_VX,             i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_VY,             i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_VZ,             i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldVX,          i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldVY,          i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldVZ,          i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldFX,          i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldFY,          i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldFZ,          i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
/*
    MPI_Pack(m_OldX,           i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldY,           i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldZ,           i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_UnPBCX,         i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_UnPBCY,         i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_UnPBCZ,         i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_InitialX,       i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_InitialY,       i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_InitialZ,       i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_dX,             i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_dY,             i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_dZ,             i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);  */

    MPI_Isend(m_SendBuffer, m_MaxMsgBuffer, MPI_PACKED, procId, m_Tag, MPI_COMM_WORLD, pRequest);
 
#endif
}

// Wrapper for the Mpi function:
// MPI_Irecv(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSendPlanarBeads::IReceive(long procId, MsgRequestPtr pRequest)
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

bool pmSendPlanarBeads::Wait(MsgRequestPtr pRequest)
{
    bool bSuccess = false;

#if SimMPS == SimulationEnabled

    MPI_Status status;

    MPI_Wait(pRequest, &status);

    // If this is the receive call unpack the data

    if(pRequest == m_pSendRequest)
    {
//		std::cout << "Processor " << GetRank() << "executing Wait for a send request" << zEndl;
        bSuccess = true;
		if(GetSentBeadTotal() > 0)
		{
//			std::cout << "Processor " << GetRank() << " has executed Wait and sent " << GetSentBeadTotal() << " beads" << zEndl;
		}
	}
    else if(pRequest == m_pRecRequest)
    {
//		std::cout << "Processor " << GetRank() << "executing Wait for a receive request" << zEndl;
        bSuccess = true;

        int  position;
        position = 0;
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, &m_RecTotal, 1, MPI_LONG, MPI_COMM_WORLD);

		if(GetRecBeadTotal() > 0)
		{
//			std::cout << "Processor " << GetRank() << " has executed Wait and received " << GetRecBeadTotal() << " planar Face beads, and has " << GetEmptyBeadTotal() << " empty beads" << zEndl;
		}

        // Now unpack the message payload

        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Ids,            m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_DisplayIds,     m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Visibility,     m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Types,          m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Locations,      m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_FirstBondIds,   m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_FirstBPIds,     m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_PolyIds,        m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_PolyTypes,      m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_X,              m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Y,              m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Z,              m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_VX,             m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_VY,             m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_VZ,             m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldVX,          m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldVY,          m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldVZ,          m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldFX,          m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldFY,          m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldFZ,          m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
/*		
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldX,           m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldY,           m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldZ,           m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_UnPBCX,         m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_UnPBCY,         m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_UnPBCZ,         m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_InitialX,       m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_InitialY,       m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_InitialZ,       m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_dX,             m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_dY,             m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_dZ,             m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
*/
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
			m_vBeadLocations.clear();
			m_vFirstBondIds.clear();
			m_vFirstBPIds.clear();
			m_vPolymerIds.clear();
			m_vPolymerTypes.clear();

			for(long i=0; i<m_RecTotal; i++)
			{
				m_pBead = m_lEmptyBeads.back();
				m_lEmptyBeads.pop_back();
				
				m_vBeadLocations.push_back(m_Locations[i]);
				m_vFirstBondIds.push_back(m_FirstBondIds[i]);
				m_vFirstBPIds.push_back(m_FirstBPIds[i]);
				m_vPolymerIds.push_back(m_PolyIds[i]);
				m_vPolymerTypes.push_back(m_PolyTypes[i]);
				
				if(m_PolyIds[i] < 0 || m_PolyTypes[i] < 0)
				{
				    std::cout << "Error in pmSendPlanarBeads, polymer id < 0 " << m_Ids[i] << " " << m_PolyIds[i] << " " << m_PolyTypes[i] << zEndl;
				}

                m_pBead->SetOwningPolymer(0);
				m_pBead->SetId(m_Ids[i]);
				CCurrentState::SetBeadDisplayId(m_Ids[i], m_DisplayIds[i]);
				m_pBead->SetVisible(m_Visibility[i]);
				m_pBead->SetNotMovable();
				m_pBead->SetType(m_Types[i]);
				m_pBead->SetRadius(0.5);        // Radius is hardwired to 0.5 for now
 				m_pBead->SetXPos(m_X[i]);
				m_pBead->SetYPos(m_Y[i]);
				m_pBead->SetZPos(m_Z[i]);
				m_pBead->SetXMom(m_VX[i]);
				m_pBead->SetYMom(m_VY[i]);
				m_pBead->SetZMom(m_VZ[i]);
				m_pBead->SetXForce(0.0);
				m_pBead->SetYForce(0.0);
				m_pBead->SetZForce(0.0);
				m_pBead->SetOldXMom(m_OldVX[i]);
				m_pBead->SetOldYMom(m_OldVY[i]);
				m_pBead->SetOldZMom(m_OldVZ[i]);
				m_pBead->SetOldXForce(m_OldFX[i]);
				m_pBead->SetOldYForce(m_OldFY[i]);
				m_pBead->SetOldZForce(m_OldFZ[i]);
/*				
				m_pBead->SetOldXPos(m_OldX[i]);
				m_pBead->SetOldYPos(m_OldY[i]);
				m_pBead->SetOldZPos(m_OldZ[i]);
				m_pBead->SetunPBCXPos(m_UnPBCX[i]);
				m_pBead->SetunPBCYPos(m_UnPBCY[i]);
				m_pBead->SetunPBCZPos(m_UnPBCZ[i]);
				m_pBead->SetInitialXPos(m_InitialX[i]);
				m_pBead->SetInitialYPos(m_InitialY[i]);
				m_pBead->SetInitialZPos(m_InitialZ[i]);
				m_pBead->SetdXPos(m_dX[i]);
				m_pBead->SetdYPos(m_dY[i]);
				m_pBead->SetdZPos(m_dZ[i]);  */

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

void pmSendPlanarBeads::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}



// Function to return the set of new bead instances created from the data
// received in this message class.

BeadList pmSendPlanarBeads::GetRecBeads()
{
	return m_lRecBeads;
}

// Function to return a bead's zero-indexed location in its owning polymer counting from the first bead in the polymer's shape.

const zLongVector& pmSendPlanarBeads::GetBeadLocations() const
{
    return m_vBeadLocations;
}

// Function to return the set of ids of the first bonds in the polymers.

const zLongVector& pmSendPlanarBeads::GetFirstBondIds() const
{
    return m_vFirstBondIds;
}

// Function to return the set of ids of the first stiff bonds in the polymers.

const zLongVector& pmSendPlanarBeads::GetFirstBondPairIds() const
{
    return m_vFirstBPIds;
}

// Function to return the set of unique polymer ids for the concrete polymers that own the beads that are being passed
// by this message class.

const zLongVector& pmSendPlanarBeads::GetPolymerIds() const
{
    return m_vPolymerIds;
}

// Function to return the set of polymer types for the concrete polymers that own the beads that are being passed
// by this message class.

const zLongVector& pmSendPlanarBeads::GetPolymerTypes() const
{
    return m_vPolymerTypes;
}

