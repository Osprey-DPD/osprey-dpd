/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSendLinearBeads.cpp: implementation of the pmSendLinearBeads class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSendLinearBeads.h"
#include "Bead.h"
#include "Polymer.h"
#include "CurrentState.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSendLinearBeads::m_Type = "SendLinearBeads";

const zString pmSendLinearBeads::GetType()
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
	mpmMessage* Create() {return new pmSendLinearBeads();}

	const zString id = pmSendLinearBeads::GetType();

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

pmSendLinearBeads::pmSendLinearBeads() : mpmMessage(), m_pSendRequest(0),
                                         m_pRecRequest(0), m_VisibleFlags(0),
                                         m_RecTotal(0), m_pBead(0), m_pPolymer(0)
{
    for(long i=0; i<m_MaxMsgBeads; i++)
    {
        m_Ids[i]          = 0;
        m_DisplayIds[i]   = 0;
        m_Visibility[i]   = 0;
        m_Types[i]        = 0;
        m_PolymerIds[i]   = 0;
        m_PolymerTypes[i] = 0;
        m_BeadOrder[i]    = 0;
        m_Radius[i]       = 0.0;
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
        m_UnPBCX[i]       = 0.0;
        m_UnPBCY[i]       = 0.0;
        m_UnPBCZ[i]       = 0.0;
        m_InitialX[i]     = 0.0;
        m_InitialY[i]     = 0.0;
        m_InitialZ[i]     = 0.0;
        m_dX[i]           = 0.0;
        m_dY[i]           = 0.0;
        m_dZ[i]           = 0.0;
    }

    m_lBeads.clear();
    m_lRecBeads.clear();
    m_lEmptyBeads.clear();
    m_lRecPolymers.clear();
    m_lEmptyPolymers.clear();

    // Create the initial set of empty beads and polymers  used to receive 
	// incoming bead coordinates.

    for(long j=0; j<m_InitialEmptyBeads; j++)
    {
        m_lEmptyBeads.push_back(new CBead());
        m_lEmptyPolymers.push_back(new CPolymer());
    }
}

// Copy constructor.

pmSendLinearBeads::pmSendLinearBeads(const pmSendLinearBeads& oldMessage) : mpmMessage(oldMessage),
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
        m_PolymerIds[i]   = oldMessage.m_PolymerIds[i];
        m_PolymerTypes[i] = oldMessage.m_PolymerTypes[i];
        m_BeadOrder[i]    = oldMessage.m_BeadOrder[i];
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
        m_UnPBCX[i]       = oldMessage.m_UnPBCX[i];
        m_UnPBCY[i]       = oldMessage.m_UnPBCY[i];
        m_UnPBCZ[i]       = oldMessage.m_UnPBCZ[i];
        m_InitialX[i]     = oldMessage.m_InitialX[i];
        m_InitialY[i]     = oldMessage.m_InitialY[i];
        m_InitialZ[i]     = oldMessage.m_InitialZ[i];
        m_dX[i]           = oldMessage.m_dX[i];
        m_dY[i]           = oldMessage.m_dY[i];
        m_dZ[i]           = oldMessage.m_dZ[i];
    }

    m_lBeads         = oldMessage.m_lBeads;
    m_lRecBeads      = oldMessage.m_lRecBeads;
    m_lEmptyBeads    = oldMessage.m_lEmptyBeads;
    m_lRecPolymers   = oldMessage.m_lRecPolymers;
    m_lEmptyPolymers = oldMessage.m_lEmptyPolymers;
}

// Destructor. If any empty bead instances exist we delete them here as they are not
// included the initial state's bead set.

pmSendLinearBeads::~pmSendLinearBeads()
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

bool pmSendLinearBeads::CreateEmptyBeads()
{
    for(long j=0; j<m_InitialEmptyBeads; j++)
    {
        m_lEmptyBeads.push_back(new CBead());
    }

	return true;
}

// Private helper function to create a batch of empty polymers instances. We just
// append the new instances to the empty polymer container.

bool pmSendLinearBeads::CreateEmptyPolymers()
{
    for(long j=0; j<m_InitialEmptyBeads; j++)
    {
        m_lEmptyPolymers.push_back(new CPolymer());
    }

	return true;
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSendLinearBeads::put(zOutStream& os) const
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

zInStream& pmSendLinearBeads::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSendLinearBeads::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSendLinearBeads::GetMessage() const
{
	return new pmSendLinearBeads(*this);
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

void pmSendLinearBeads::SetMessageData(BeadList lBeads)
{
//    std::cout << "Processor " << GetRank() << " loading up " << lBeads.size() << " beads" << zEndl;

    m_lBeads = lBeads;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We check that the number of beads to be sent will fit into the fixed-size arrays
// used by MPI, that the bead id and its owning polymer's id are non-negative 
// (indicating that they are both valid, and not empty instances) and that the 
// beads' position coordinates are not negative. If these conditions are met, we
// strip out the bead from its owning polymer and recycle the polymer instance, setting
// some flags to show whether the bead is the first or only bead in the polymer,
// or whether the polymer is extended.

bool pmSendLinearBeads::Validate()
{
    bool bSuccess = true;

#if SimMPS == SimulationEnabled

    if(GetSentBeadTotal() > 0)
    {
        if(GetSentBeadTotal() < m_MaxMsgBeads)
        {
            for(cBeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); iterBead++)
            {
                if((*iterBead)->GetId() < 0 ||!(*iterBead)->GetPolymer() || (*iterBead)->GetPolymer()->GetId() < 0 || 
				   (*iterBead)->GetRadius() == 0.0 || (*iterBead)->GetXPos() < 0.0 || (*iterBead)->GetYPos() < 0.0 || (*iterBead)->GetZPos() < 0.0)
                {
			        std::cout << "Error in pmSendLinearBeads: negative bead/polymer id or coordinate" << zEndl;
					std::cout << (*iterBead)->GetId() << " " << (*iterBead)->GetRadius() << " " << (*iterBead)->GetPolymer()->GetId() << " " << (*iterBead)->GetXPos() << " " << (*iterBead)->GetYPos() << " " << (*iterBead)->GetZPos() << zEndl;
                    bSuccess = false;
                }
            }
        }
        else
        {
			std::cout << "Error in pmSendLinearBeads: too many beads to send" << zEndl;
            bSuccess = false;
        }
    }
#endif

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Isend(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSendLinearBeads::ISend(long procId, MsgRequestPtr pRequest)
{
#if SimMPS == SimulationEnabled

    m_pSendRequest = pRequest;

    // Store the bead coordinates in local arrays for the MPI call and add the
    // newly-empty instances to the local empty beads store. We set the empty 
	// bead's id to a negative value to indicate that this is not a valid bead
	// instance. We do the same for the polymer instances that contained the beads.
	// But this is more complicated. Single-bead polymers are simply emptied and
	// stored for later use. Polymers with more than one bead have to be checked to
	// replace the moving bead instance by a ghost bead, so that we can continue to
	// calculate bonded forces between beads owned by different processors.

    long i = 0;

    for(cBeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); iterBead++)
    {
	   CPolymer* pPolymer = (*iterBead)->GetPolymer();

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
       m_Types[i]	     = (*iterBead)->GetType();
       m_PolymerIds[i]   = pPolymer->GetId();
       m_PolymerTypes[i] = pPolymer->GetType();
       m_BeadOrder[i]    = GetBeadOrderFromPolymer(pPolymer);

//     m_Radius[i]       = (*iterBead)->GetRadius();  // Hardwired to 0.5 for now

       m_X[i]		     = (*iterBead)->GetXPos();
       m_Y[i]		     = (*iterBead)->GetYPos();
       m_Z[i]		     = (*iterBead)->GetZPos();
       m_VX[i]		     = (*iterBead)->GetXMom();
       m_VY[i]		     = (*iterBead)->GetYMom();
       m_VZ[i]		     = (*iterBead)->GetZMom();

       m_OldVX[i]        = (*iterBead)->GetOldXMom();
       m_OldVY[i]        = (*iterBead)->GetOldYMom();
       m_OldVZ[i]        = (*iterBead)->GetOldZMom();
       m_OldFX[i]        = (*iterBead)->GetOldXForce();
       m_OldFY[i]        = (*iterBead)->GetOldYForce();
       m_OldFZ[i]        = (*iterBead)->GetOldZForce();

       m_UnPBCX[i]       = (*iterBead)->GetunPBCXPos();
       m_UnPBCY[i]       = (*iterBead)->GetunPBCYPos();
       m_UnPBCZ[i]       = (*iterBead)->GetunPBCZPos();
       m_InitialX[i]     = (*iterBead)->GetInitialXPos();
       m_InitialY[i]     = (*iterBead)->GetInitialYPos();
       m_InitialZ[i]     = (*iterBead)->GetInitialZPos();
	   m_dX[i]           = (*iterBead)->GetdXPos();
	   m_dY[i]           = (*iterBead)->GetdYPos();
	   m_dZ[i]           = (*iterBead)->GetdZPos();

	   (*iterBead)->SetId(-1);
	   m_lEmptyBeads.push_back(*iterBead);

	   // If the polymer has only a single-bead or this is the last bead in the polymer
	   // to be passed, we recycle the empty polymer instance. Otherwise, we replace
	   // the bead with a ghost bead and leave the polymer in the sending processor.

	   if(pPolymer->GetSize() == 1 || pPolymer->GetSize() == m_BeadOrder[i])
	   {
			pPolymer->SetId(-1);
	   }
	   else
	   {

	   }

       ++i;
    }

    // First, pack the number of beads

    int  position;
    position = 0;
    MPI_Pack(&i, 1, MPI_LONG, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);

    // Now pack the data into the message payload: we don't send the bead radii as they
	// are all hardwired to 0.5 for now. If we later allow beads to have different radii,
	// we need to send this data as well.

    MPI_Pack(m_Ids,           i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_DisplayIds,    i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Visibility,    i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Types,         i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_PolymerIds,    i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_PolymerTypes,  i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_BeadOrder,     i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_X,             i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Y,             i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Z,             i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_VX,            i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_VY,            i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_VZ,            i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldVX,         i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldVY,         i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldVZ,         i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldFX,         i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldFY,         i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_OldFZ,         i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_UnPBCX,        i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_UnPBCY,        i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_UnPBCZ,        i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_InitialX,      i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_InitialY,      i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_InitialZ,      i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_dX,            i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_dY,            i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_dZ,            i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);

    m_ErrorCode = MPI_Isend(m_SendBuffer, m_MaxMsgBuffer, MPI_PACKED, procId, m_Tag, MPI_COMM_WORLD, pRequest);
	
	char error_message[MPI_MAX_ERROR_STRING];
	int errorMessageLength;
		
	if(m_ErrorCode != MPI_SUCCESS)
	{
	    std::cout << "Proc " << GetRank() << " error" << zEndl;
		MPI_Error_string(m_ErrorCode, error_message, &errorMessageLength);
		
		std::cout << "Proc " << GetRank() << " has MPI error in pmSendLinearBeads::ISend " << error_message << zEndl;
		MPI_Abort(MPI_COMM_WORLD, -1);
	}
 
#endif
}

// Wrapper for the Mpi function:
// MPI_Irecv(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSendLinearBeads::IReceive(long procId, MsgRequestPtr pRequest)
{
#if SimMPS == SimulationEnabled

    m_pRecRequest = pRequest;

    m_ErrorCode = MPI_Irecv(m_RecBuffer, m_MaxMsgBuffer, MPI_PACKED, procId, m_Tag, MPI_COMM_WORLD, pRequest);
	
	char error_message[MPI_MAX_ERROR_STRING];
	int errorMessageLength;
		
	if(m_ErrorCode != MPI_SUCCESS)
	{
	    std::cout << "Proc " << GetRank() << " error" << zEndl;
		MPI_Error_string(m_ErrorCode, error_message, &errorMessageLength);
		
		std::cout << "Proc " << GetRank() << " has MPI error in pmSendLinearBeads::IReceive " << error_message << zEndl;
		MPI_Abort(MPI_COMM_WORLD, -1);
	}

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

bool pmSendLinearBeads::Wait(MsgRequestPtr pRequest)
{
    bool bSuccess = false;

#if SimMPS == SimulationEnabled

    MPI_Status status;

    m_ErrorCode = MPI_Wait(pRequest, &status);

	char error_message[MPI_MAX_ERROR_STRING];
	int errorMessageLength;
		
	if(m_ErrorCode != MPI_SUCCESS)
	{
	    std::cout << "Proc " << GetRank() << " error" << zEndl;
		MPI_Error_string(m_ErrorCode, error_message, &errorMessageLength);
		
		std::cout << "Proc " << GetRank() << " has MPI error in pmSendLinearBeads::Wait " << error_message << zEndl;
		MPI_Abort(MPI_COMM_WORLD, -1);
	}
	
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
//			std::cout << "Processor " << GetRank() << " has executed Wait and received " << GetRecBeadTotal() << " beads" << zEndl;
		}

        // Now unpack the message payload

        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Ids,           m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_DisplayIds,    m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Visibility,    m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Types,         m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_PolymerIds,    m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_PolymerTypes,  m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_BeadOrder,     m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_X,             m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Y,             m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Z,             m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_VX,            m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_VY,            m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_VZ,            m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldVX,         m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldVY,         m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldVZ,         m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldFX,         m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldFY,         m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_OldFZ,         m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_UnPBCX,        m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_UnPBCY,        m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_UnPBCZ,        m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_InitialX,      m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_InitialY,      m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_InitialZ,      m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_dX,            m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_dY,            m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_dZ,            m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);

        // and copy the data into empty bead instances held by this message instance.
		// If there are not sufficient empty bead instances available, we create 
		// another batch before starting to fill them up. If the memory allocation fails
		// then we return false to indicate that the transfer failed.

		while(m_RecTotal > GetEmptyBeadTotal() && bSuccess)
		{
			std::cout << "Processor " << GetRank()  << " has to create new empty beads for " << m_RecTotal << " received instances" << zEndl;

            bSuccess = CreateEmptyBeads();
		}

		if(bSuccess)
		{
			m_lRecBeads.clear();

			for(long i=0; i<m_RecTotal; i++)
			{
				m_pBead = m_lEmptyBeads.back();
				m_lEmptyBeads.pop_back();

				m_pBead->SetId(m_Ids[i]);
				CCurrentState::SetBeadDisplayId(m_Ids[i], m_DisplayIds[i]);
				m_pBead->SetVisible(m_Visibility[i]);
				m_pBead->SetType(m_Types[i]);
				m_pBead->SetRadius(0.5);    // Hardwired to 0.5 for now
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
				m_pBead->SetunPBCXPos(m_UnPBCX[i]);
				m_pBead->SetunPBCYPos(m_UnPBCY[i]);
				m_pBead->SetunPBCZPos(m_UnPBCZ[i]);
				m_pBead->SetInitialXPos(m_InitialX[i]);
				m_pBead->SetInitialYPos(m_InitialY[i]);
				m_pBead->SetInitialZPos(m_InitialZ[i]);
				m_pBead->SetdXPos(m_dX[i]);
				m_pBead->SetdYPos(m_dY[i]);
				m_pBead->SetdZPos(m_dZ[i]);

				// Now set the bead's owning polymer data


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

void pmSendLinearBeads::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}



// Function to return the set of new bead instances created from the data
// received in this message class.

BeadList pmSendLinearBeads::GetRecBeads()
{
	return m_lRecBeads;
}

// Function to return the set of new polymers instances created from the data
// received in this message class.


PolymerList pmSendLinearBeads::GetRecPolymers()
{
    return m_lRecPolymers;
}

// Private helper function to calculate how many beads in this polymer will have been
// replaced by ghost beads when the current bead is trnsferred. The result is the 
// m_BeadOrder[] parameter associated with the bead currently being passed.
// A single-bead polymer automatically has the order 1 as the only bead to be passed 
// must be the first. For extended polymers the order is defined as one plus the 
// number of beads that have already been replaced by ghost beads. Note that the return
// value cannot exceed the size of the polymer.

long pmSendLinearBeads::GetBeadOrderFromPolymer(CPolymer* const pPolymer) const
{
	long order = 1;

	if(pPolymer->GetSize() > 1)
	{
		BeadVector vBeads = pPolymer->GetBeads();

		for(cBeadVectorIterator citerBead=vBeads.begin(); citerBead!=vBeads.end(); citerBead++)
		{
//			if(dynamic_cast<const CGhostBead*>(*citerBead))
			{
                ++order;
			}
		}

	}

	return order;
}

// Private helper function to return true if the bead currently being passed is
// either the first or last in the polymer. This allows the message class to
// put it into a fresh empty polymer instance.

bool pmSendLinearBeads::IsBeadFirstOrLastInPolymer() const
{
	return false;
}

bool pmSendLinearBeads::ReplaceBeadByGhostInPolymer()
{
    return false;	
}
