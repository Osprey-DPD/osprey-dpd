/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pmSendCubicCornerBeadForces.cpp: implementation of the pmSendCubicCornerBeadForces class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "pmSendCubicCornerBeadForces.h"
#include "ExternalCNTCell.h"
#include "Bead.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this message. 

const zString pmSendCubicCornerBeadForces::m_Type = "SendCubicCornerBeadForces";

const zString pmSendCubicCornerBeadForces::GetType()
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
	mpmMessage* Create() {return new pmSendCubicCornerBeadForces();}

	const zString id = pmSendCubicCornerBeadForces::GetType();

	const bool bRegistered = acfParallelMessageFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store all parameters for all simulation types here, but conditionally
// compile the functions that access them depending on the SimIdentifier flag.
// Note that the pointers to the MPI_Request objects are NOT owned by this 
// message class, but are created/destroyed by the calling object.

pmSendCubicCornerBeadForces::pmSendCubicCornerBeadForces() : mpmMessage(), m_pSendRequest(0), m_pRecRequest(0), m_SentForceTotal(0), m_RecTotal(0)
                                                   
{
    for(long i=0; i<m_MaxMsgBeads; i++)
    {
        m_Ids[i]          = 0;
        m_ForceCounter[i] = 0;
        m_FX[i]           = 0.0;
        m_FY[i]           = 0.0;
        m_FZ[i]           = 0.0;
    }

    m_vIds.clear();
    m_vForceCounter.clear();
    m_vFX.clear();
    m_vFY.clear();
    m_vFZ.clear();
}

pmSendCubicCornerBeadForces::pmSendCubicCornerBeadForces(const pmSendCubicCornerBeadForces& oldMessage) : mpmMessage(oldMessage),
                                               m_pSendRequest(oldMessage.m_pSendRequest), m_pRecRequest(oldMessage.m_pRecRequest),
                                               m_SentForceTotal(oldMessage.m_SentForceTotal), m_RecTotal(oldMessage.m_RecTotal)
                                               
{
    for(long i=0; i<m_MaxMsgBeads; i++)
    {
        m_Ids[i]          = oldMessage.m_Ids[i];
        m_ForceCounter[i] = oldMessage.m_Ids[i];
        m_FX[i]           = oldMessage.m_FX[i];
        m_FY[i]           = oldMessage.m_FY[i];
        m_FZ[i]           = oldMessage.m_FZ[i];
    }

    m_vIds          = oldMessage.m_vIds;
    m_vForceCounter = oldMessage.m_vForceCounter;
    m_vFX           = oldMessage.m_vFX;
    m_vFY           = oldMessage.m_vFY;
    m_vFZ           = oldMessage.m_vFZ;
}

pmSendCubicCornerBeadForces::~pmSendCubicCornerBeadForces()
{
}

// Member functions to read/write the data specific to the message.
//
// Arguments
// *********
//

zOutStream& pmSendCubicCornerBeadForces::put(zOutStream& os) const
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

zInStream& pmSendCubicCornerBeadForces::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the message

const zString pmSendCubicCornerBeadForces::GetMessageType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current message.

const mpmMessage* pmSendCubicCornerBeadForces::GetMessage() const
{
	return new pmSendCubicCornerBeadForces(*this);
}

// ****************************************
// Function to store the set of bead-bead forces that form the message payload.
// The payload contains the data needed by the receiving processor in order to 
// be able to add the non-bonded forces between its own beads and those in
// the sending processor.
//
// We actually pass in the set of external CNT Cells that contain the forces 
// that form the message payload. This reduces the number of arguments we have to pass.

void pmSendCubicCornerBeadForces::SetMessageData(const CNTCellVector& vExtCells)
{
    // First clear the local containers

    m_vIds.clear();
    m_vForceCounter.clear();
    m_vFX.clear();
    m_vFY.clear();
    m_vFZ.clear();

    for(cCNTCellIterator citerCell=vExtCells.begin(); citerCell!=vExtCells.end(); citerCell++)
    {
        const CExternalCNTCell* const pCell = dynamic_cast<const CExternalCNTCell*>(*citerCell);

        zLongVector   eid = pCell->GetBeadIds();
        zLongVector   efc = pCell->GetForceCounter();
        zDoubleVector efx = pCell->GetXForces();
        zDoubleVector efy = pCell->GetYForces();
        zDoubleVector efz = pCell->GetZForces();

        copy(eid.begin(), eid.end(), back_inserter(m_vIds));
        copy(efc.begin(), efc.end(), back_inserter(m_vForceCounter));
        copy(efx.begin(), efx.end(), back_inserter(m_vFX));
        copy(efy.begin(), efy.end(), back_inserter(m_vFY));
        copy(efz.begin(), efz.end(), back_inserter(m_vFZ));
    }

    m_SentForceTotal = m_vIds.size();  // Local counter used in the MPI send function wrapper
}

// ****************************************
// Pure virtual function to check that the data are valid prior to sending a message.
// We simply check that the number of force terns to be sent will fit into the 
// fixed-size arrays used by MPI.

bool pmSendCubicCornerBeadForces::Validate()
{
    bool bSuccess = true;

    if(m_SentForceTotal > m_MaxMsgBeads || m_SentForceTotal != static_cast<long>(m_vFX.size()) ||
       m_vFX.size() != m_vFY.size() || m_vFX.size() != m_vFZ.size() || m_vFY.size() != m_vFZ.size())
    {
	    std::cout << "Proc " << GetRank() << " pmSendCubicCornerBeadForces failed validation with " << m_SentForceTotal << " force terms" << zEndl;
        bSuccess = false;
    }

    return bSuccess;
}

// Wrapper for the MPI function:
// MPI_Isend(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSendCubicCornerBeadForces::ISend(long procId, MsgRequestPtr pRequest)
{
#if SimMPS == SimulationEnabled

    m_pSendRequest = pRequest;

    for(long i=0; i<m_SentForceTotal; i++)
    {
       m_Ids[i]          =  m_vIds.at(i);
       m_ForceCounter[i] = m_vForceCounter.at(i);
       m_FX[i]           = m_vFX.at(i);
       m_FY[i]           = m_vFY.at(i);
       m_FZ[i]           = m_vFZ.at(i);
    }


    // First, pack the number of force terms

    int  position;
    position = 0;
    MPI_Pack(&m_SentForceTotal, 1, MPI_LONG, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);

    // Now pack the data into the message payload

    MPI_Pack(m_Ids,          m_SentForceTotal, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_ForceCounter, m_SentForceTotal, MPI_LONG,   m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_FX,           m_SentForceTotal, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_FY,           m_SentForceTotal, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);
    MPI_Pack(m_FZ,           m_SentForceTotal, MPI_DOUBLE, m_SendBuffer, m_MaxMsgBuffer, &position, MPI_COMM_WORLD);

    MPI_Isend(m_SendBuffer, m_MaxMsgBuffer, MPI_PACKED, procId, m_Tag, MPI_COMM_WORLD, pRequest);
#endif
}

// Wrapper for the Mpi function:
// MPI_Irecv(message, length, MPI_CHAR, m_ProcId, tag, MPI_COMM_WORLD);

void pmSendCubicCornerBeadForces::IReceive(long procId, MsgRequestPtr pRequest)
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

bool pmSendCubicCornerBeadForces::Wait(MsgRequestPtr pRequest)
{
    bool bSuccess = false;

#if SimMPS == SimulationEnabled

    MPI_Status status;

    MPI_Wait(pRequest, &status);

    // If this is the receive call unpack the data

    if(pRequest == m_pSendRequest)
    {
        bSuccess = true;
//        std::cout << "Processor " << GetRank() << " has executed Wait and sent " << GetSentForceTotal() << " forces" << zEndl;
    }
    else if(pRequest == m_pRecRequest)
    {
        bSuccess = true;

        int  position;
        position = 0;
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, &m_RecTotal, 1, MPI_LONG, MPI_COMM_WORLD);

//        std::cout << "Processor " << GetRank() << " has executed Wait and received " << GetRecForceTotal() << " forces" << zEndl;

        // Now unpack the message payload

        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_Ids,          m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_ForceCounter, m_RecTotal, MPI_LONG,   MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_FX,           m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_FY,           m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(m_RecBuffer, m_MaxMsgBuffer, &position, m_FZ,           m_RecTotal, MPI_DOUBLE, MPI_COMM_WORLD);

        // Now copy the data into local storage for retrieval after clearing the containers

        m_vIds.clear();
        m_vForceCounter.clear();
        m_vFX.clear();
        m_vFY.clear();
        m_vFZ.clear();

        for(long i=0; i<m_RecTotal; i++)
        {
            m_vIds.push_back(m_Ids[i]);
            m_vForceCounter.push_back(m_ForceCounter[i]);
            m_vFX.push_back(m_FX[i]);
            m_vFY.push_back(m_FY[i]);
            m_vFZ.push_back(m_FZ[i]);
        }
    }
#endif

    return bSuccess;
}

// PVF to assemble the disparate data items into a struct suitable for sending to 
// the MPI messaging functions. All the data must be in the calling object, and the
// data type is not usable outside this class.
// The SimMPS flag must be defined for this function to work, otherwise it does nothing.

void pmSendCubicCornerBeadForces::BuildDerivedType(MsgDataTypePtr pMsgDataType)
{
}




