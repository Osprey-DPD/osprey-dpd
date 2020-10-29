/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogState.cpp: implementation of the CLogState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimXMLFlags.h"
#include "LogState.h"
#include "InputData.h"
#include "LogMessage.h"


//////////////////////////////////////////////////////////////////////
// Static member variable and function definitions
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Static member variable holding a pointer to the single instance of CLogState.
// Note that it does not get assigned a value until the user creates the
// instance. 

CLogState* CLogState::m_pInstance = NULL;


// Static member function to add a newly-created message to the message sequence
// by calling the class member function AddMessage. This allows any object
// to add a message to the CLogState.

void CLogState::StaticAddMessage(xxMessage* const pMsg)
{
	m_pInstance->AddMessage(pMsg);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// The log state file's name depends on whether it is XML-enabled or not,
// so we use two different constructor signatures to allow for this. If it
// is XML-enabled, we write out wrapper tags <LogState></LogState> as the first
// and last elements in the file.

#if EnableXMLCommands == SimXMLEnabled

CLogState::CLogState(const CInputData& rData) : xxState(xxBase::GetLSPrefix() + rData.GetRunId() + ".xml", true, 0, rData.GetRunId())
{
	// First write the xml and stylesheet PIs: note that the version and
	// stylesheet name are hardwired here.

    if(IsProcessZero())
    {
	    m_outStream << "<?xml version=\"1.0\" ?>" << zEndl;
	    m_outStream << "<?xml-stylesheet type=\"text/css\" href=\"logstate.css\" ?>" << zEndl;

	    // now the top-level LogState element with its runId attribute

	    m_outStream << "<LogState runId = \"" << rData.GetRunId() << "\">" << zEndl;
    }

#elif EnableXMLCommands == SimXMLDisabled

    CLogState::CLogState(const CInputData& rData) : xxState(xxBase::GetLSPrefix() + rData.GetRunId(), true, 0, rData.GetRunId())
{

#endif

	// Store the single CLogState object in a static member so that it can be
	// accessed by any class, and have messages added to it.

	CLogState::m_pInstance = this;

	// Create enough memory for the default number of messages. Any more and memory
	// must be reallocated. Note that the unwritten message sequence holds the
	// messages until they have been serialized when their pointers are removed.
	// But we only delete the CLogMessage objects via the m_vMessages container.

	m_vMessages.reserve(1000);
	m_vUnWrittenMessages.reserve(1000);

}

CLogState::~CLogState()
{
	// Dump all remaining messages to file before destruction. We have to distinguish between the serial and parallel codes as only P0 can write to file, so all 
    // other PN must send their messages to P0 prior to writing.

#if EnableParallelMonitor == SimMPSEnabled
    
	m_pInstance->SerializeP();
        
#else
    
	m_pInstance->Serialize();

#endif
    
    // If the log state is XML-enabled, add the closing tag
    
#if EnableXMLCommands == SimXMLEnabled
    if(IsProcessZero())
    {
	    m_outStream << "</LogState>" << zEndl;
    }
#endif
        
	// Clear the static pointer to the singleton CLogState object preparatory to
	// doing another simulation.

	m_pInstance = NULL;

	// Delete created messages

	for(MessageIterator iterMsg=m_vMessages.begin(); iterMsg!=m_vMessages.end(); iterMsg++)
	{
		delete (*iterMsg);
	}

	m_vMessages.clear();
	m_vUnWrittenMessages.clear();
}

long CLogState::GetMessageTotal()
{
	return m_vMessages.size();
}

long CLogState::GetUnWrittenMessageTotal()
{
	return m_vUnWrittenMessages.size();
}


// Function to write out the logging messages to file. We iterate over the 
// unwritten message container calling the CLogMessage::operator<<() for each 
// one. Once a message has been serialized, we remove it from the container. 
// Note that it is still held in the m_vMessages container until the CLogState
// object is destroyed at the end of the simulation.
//

bool CLogState::Serialize()
{
	if(m_IOFlag && m_bOpenFlag)	
	{
		if(GetUnWrittenMessageTotal() > 0)
		{
			for(MessageIterator iterMsg=m_vUnWrittenMessages.begin(); iterMsg!=m_vUnWrittenMessages.end(); iterMsg++)
			{
				m_outStream << (**iterMsg);
			}

			m_vUnWrittenMessages.clear();
		}
	}
	else
		return false;

	return true;
}

// Parallel version of the above function to write out the logging messages to file.
// We use the scheme that only P0 actually writes to a file, and all other PN send their set of 
// buffered messages to P0. 
//
// Note that any messages left unwritten at the end of a run are serialized in the destructor.

bool CLogState::SerializeP()
{
#if EnableParallelMonitor == SimMPSEnabled

	// If we are P0 receive the message buffers from all PN and add them to the unwritten message buffer.

	if(IsProcessZero())
	{
	
	    // Here we insert code to receive messages from PN
		
	    if(m_IOFlag && m_bOpenFlag)	
	    {
		    if(GetUnWrittenMessageTotal() > 0)
		    {
			    for(MessageIterator iterMsg=m_vUnWrittenMessages.begin(); iterMsg!=m_vUnWrittenMessages.end(); iterMsg++)
			    {
				    m_outStream << (**iterMsg);
			    }

			    m_vUnWrittenMessages.clear();
		    }
	    }
	    else
		    return false;
	}
	else
	{
        // If we are PN, send the current message buffer to P0
	
	    // For now we ignore all messages from PN: we assume that any message that P0 writes to file applies to all PN as well.
	
	
	
	}

#endif

	return true;
}


// Member function to add a message to the CLogState's message list. It can only
// be called by the static member function StaticAddMessage().

void CLogState::AddMessage(xxMessage *const pMsg)
{
	m_vMessages.push_back(pMsg);
	m_vUnWrittenMessages.push_back(pMsg);
}
