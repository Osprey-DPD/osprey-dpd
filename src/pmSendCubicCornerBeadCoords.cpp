/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSendCubicCornerBeadCoords.cpp: implementation of the pmSendCubicCornerBeadCoords class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pmSendCubicCornerBeadCoords.h"
#include "Bead.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSendCubicCornerBeadCoords::m_Type = "SendCubicCornerBeadCoords";

const zString pmSendCubicCornerBeadCoords::GetType()
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
	mpmMessage* Create() {return new pmSendCubicCornerBeadCoords();}

	const zString id = pmSendCubicCornerBeadCoords::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.
// Note that the pointers to the MPI_Request objects are NOT owned by this 
// message class, but are created/destroyed by the calling object.

pmSendCubicCornerBeadCoords::pmSendCubicCornerBeadCoords() : mpmMessage(), m_pSendRequest(0),
                                                   m_pRecRequest(0), m_RecTotal(0)
{
    for(long i=0; i<m_MaxMsgBeads; i++)
    {
        m_Ids[i]   = 0;
        m_Types[i] = 0;
        m_X[i]     = 0.0;
        m_Y[i]     = 0.0;
        m_Z[i]     = 0.0;
        m_VX[i]    = 0.0;
        m_VY[i]    = 0.0;
        m_VZ[i]    = 0.0;
    }

    m_lBeads.clear();
    m_vIds.clear();
    m_vTypes.clear();
    m_vLX.clear();
    m_vLY.clear();
    m_vLZ.clear();
    m_vVX.clear();
    m_vVY.clear();
    m_vVZ.clear();
}

pmSendCubicCornerBeadCoords::pmSendCubicCornerBeadCoords(const pmSendCubicCornerBeadCoords& oldMessage) : mpmMessage(oldMessage),
                                               m_pSendRequest(oldMessage.m_pSendRequest),
                                               m_pRecRequest(oldMessage.m_pRecRequest),
                                               m_RecTotal(oldMessage.m_RecTotal)
{
    for(long i=0; i<m_MaxMsgBeads; i++)
    {
        m_Ids[i]   = oldMessage.m_Ids[i];
        m_Types[i] = oldMessage.m_Types[i];
        m_X[i]     = oldMessage.m_X[i];
        m_Y[i]     = oldMessage.m_Y[i];
        m_Z[i]     = oldMessage.m_Z[i];
        m_VX[i]    = oldMessage.m_VX[i];
        m_VY[i]    = oldMessage.m_VY[i];
        m_VZ[i]    = oldMessage.m_VZ[i];
    }

    m_lBeads = oldMessage.m_lBeads;
    m_vIds   = oldMessage.m_vIds;
    m_vTypes = oldMessage.m_vTypes;
    m_vLX    = oldMessage.m_vLX;
    m_vLY    = oldMessage.m_vLY;
    m_vLZ    = oldMessage.m_vLZ;
    m_vVX    = oldMessage.m_vVX;
    m_vVY    = oldMessage.m_vVY;
    m_vVZ    = oldMessage.m_vVZ;
}

pmSendCubicCornerBeadCoords::~pmSendCubicCornerBeadCoords()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSendCubicCornerBeadCoords::put(zOutStream& os) const
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

zInStream& pmSendCubicCornerBeadCoords::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSendCubicCornerBeadCoords::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSendCubicCornerBeadCoords::GetMessage() const
{
	return new pmSendCubicCornerBeadCoords(*this);
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

void pmSendCubicCornerBeadCoords::SetMessageData(BeadList lBeads)
{
    m_lBeads = lBeads;
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We check that the number of beads to be sent will fit into the fixed-size arrays
// used by MPI and that the beads' position coordinates are not negative.

bool pmSendCubicCornerBeadCoords::Validate()
{
    bool bSuccess = true;

    if(GetSentBeadTotal() > 0)
    {
        if(GetSentBeadTotal() < m_MaxMsgBeads)
        {
            for(cBeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); iterBead++)
            {
                if((*iterBead)->GetXPos() < 0.0 || (*iterBead)->GetYPos() < 0.0 || (*iterBead)->GetZPos() < 0.0)
                {
		            std::cout << "Proc " << GetRank() << " pmSendCubicCornerBeadCoords: bead has negative coords: " <<  (*iterBead)->GetId() << " " << (*iterBead)->GetXPos() << " " << (*iterBead)->GetYPos() << " " << (*iterBead)->GetZPos() << zEndl;
                    bSuccess = false;
                }
            }
        }
        else
        {
		    std::cout << "pmSendCubicCornerBeadCoords exceeded bead limit: " << GetSentBeadTotal() << " < " << m_MaxMsgBeads << zEndl;
            bSuccess = false;
        }
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Isend(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSendCubicCornerBeadCoords::ISend(long procId, MsgRequestPtr pRequest)
{
#if SimMPS == SimulationEnabled

    m_pSendRequest = pRequest;

    // Store the bead coordinates in local arrays for the MPI call

    long i = 0;

    for(cBeadListIterator iterBead=m_lBeads.begin(); iterBead!=m_lBeads.end(); iterBead++)
    {
       m_Ids[i]   = (*iterBead)->GetId();
       m_Types[i] = (*iterBead)->GetType();
       m_X[i]     = (*iterBead)->GetXPos();
       m_Y[i]     = (*iterBead)->GetYPos();
       m_Z[i]     = (*iterBead)->GetZPos();
       m_VX[i]    = (*iterBead)->GetXMom();
       m_VY[i]    = (*iterBead)->GetYMom();
       m_VZ[i]    = (*iterBead)->GetZMom();
       ++i;
    }


    // First, pack the number of beads

    int  position;
    position = 0;
    MPI_Pack(&i, 1, MPI_LONG, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);

    // Now pack the data into the message payload

    MPI_Pack(m_Ids,   i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Types, i, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_X,     i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Y,     i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_Z,     i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_VX,    i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_VY,    i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_VZ,    i, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);

    MPI_Isend(m_SendBuffer, m_MaxMsgBuffer, MPI_PACKED, procId, m_Tag, MPI_COMM_WORLD, pRequest);
#endif
}

// Wrapper for the Mpi function:
// MPI_Irecv(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSendCubicCornerBeadCoords::IReceive(long procId, MsgRequestPtr pRequest)
{
#if SimMPS == SimulationEnabled

    m_pRecRequest = pRequest;

    MPI_Irecv(m_RecBuffer, m_MaxMsgBuffer, MPI_PACKED, procId, m_Tag, MPI_COMM_WORLD, pRequest);
#endif
}

// We retrieve the data from the message and fill up this instance's member variables. 
// We do not check for validity here as we assume that the sending object has 
// already done it. Note that this function does not propagate the data out into 
// the code, it only fills up the receiving message instance. Each messaging class 
// is responsible for providing further functions to pass the data to the rest of the code.

bool pmSendCubicCornerBeadCoords::Wait(MsgRequestPtr pRequest)
{
    bool bSuccess = false;

#if SimMPS == SimulationEnabled

    MPI_Status status;

    MPI_Wait(pRequest, &status);

    // If this is the receive call unpack the data

    if(pRequest == m_pSendRequest)
    {
        bSuccess = true;
//        std::cout << "Processor " << GetRank() << " has executed Wait and sent " << GetSentBeadTotal() << " bead coords" << zEndl;
    }
    else if(pRequest == m_pRecRequest)
    {
        bSuccess = true;

        int  position;
        position = 0;
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, &m_RecTotal, 1, MPI_LONG, MPI_COMM_WORLD);

//        std::cout << "Processor " << GetRank() << " has executed Wait and received " << GetRecBeadTotal() << " bead coords" << zEndl;

        // Now unpack the message payload

        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Ids,   m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Types, m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_X,     m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Y,     m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Z,     m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_VX,    m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_VY,    m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_VZ,    m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);

        // Now copy the data into local storage for retrieval, but clear the containers first

        m_vIds.clear();
        m_vTypes.clear();
        m_vLX.clear();
        m_vLY.clear();
        m_vLZ.clear();
        m_vVX.clear();
        m_vVY.clear();
        m_vVZ.clear();

        for(long i=0; i<m_RecTotal; i++)
        {
            m_vIds.push_back(m_Ids[i]);
            m_vTypes.push_back(m_Types[i]);
            m_vLX.push_back(m_X[i]);
            m_vLY.push_back(m_Y[i]);
            m_vLZ.push_back(m_Z[i]);
            m_vVX.push_back(m_VX[i]);
            m_vVY.push_back(m_VY[i]);
            m_vVZ.push_back(m_VZ[i]);
        }
    }
#endif

    return bSuccess;
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmSendCubicCornerBeadCoords::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}




