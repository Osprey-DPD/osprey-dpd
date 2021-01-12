/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prCommandDocument.cpp: implementation of the prCommandDocument class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "prCommandDocument.h"
#include "SimState.h"
#include "ISimulation.h"
#include "IExperimentAccessControl.h"
#include "IDocumentHandler.h"
#include "ICommandDocument.h"

#include "ISimBox.h"
#include "InputData.h"
#include "xxCommandObject.h"

// Commands implementing the IModifyProcess interface

#include "pcSetIrreversibleUnmodifiable.h"
#include "pcSetModifiable.h"
#include "pcSetUnmodifiable.h"

// Commands implementing the IModifyCommandDocument interface

#include "pcCommandDocumentGetPendingDocTotal.h"
#include "pcCommandDocumentGetPendingDocs.h"
#include "pcCommandDocumentAddDocument.h"

// Messages issued as a result of process logic

#include "LogCommandFailed.h"
#include "LogpcProcessModificationForbidden.h"
#include "LogpcToggleProcessModificationStatus.h"
#include "LogpcCommandDocumentGetPendingDocTotal.h"
#include "LogpcCommandDocumentGetPendingDocs.h"
#include "LogpcCommandDocumentAddDocument.h"
#include "LogCommandDocumentImported.h"
#include "LogCommandDocumentImportFailed.h"



// STL using declarations

				
//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.

const zString prCommandDocument::m_Type = "CommandDocument";

const zString prCommandDocument::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{

	xxProcess* Create() {return new prCommandDocument();}

	const zString id = prCommandDocument::GetType();

	// This process should only be created internally

	const bool bRegistered = acfProcessFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

prCommandDocument::prCommandDocument() : m_bIsModifiable(true),
										 m_bNeverModifiable(false),
										 m_DocumentName("")
										 

{
	m_Documents.clear();
}

prCommandDocument::~prCommandDocument()
{
}

// Member functions to write/read the data specific to the process.
//
// This process can only be created by command.

zOutStream& prCommandDocument::put(zOutStream& os) const
{
	return os;
}

// Function to read the data needed by the process

zInStream& prCommandDocument::get(zInStream& is)
{
	return is;
}


// Non-static function to return the type of the process

const zString prCommandDocument::GetProcessType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current process.

xxProcess* prCommandDocument::GetProcess() const
{
	return new prCommandDocument(*this);
}

// Function to update the state of the process. This process reads a user-specified
// file for commands that are to be added to the command queue.
//
// The sequence of steps is as follows:
//
//  1   Ensure that the simulation is running as part of an experiment
//      with access to the experiment's documents otherwise ignore the process
//
//  2	Check to see if any command documents have been submitted and, if so,
//      that the next command document exists, if not issue a warning message
//
//	3	Call the experiment's IDocumentHandler interface and pass the 
//	    command document's file name to it to parse the XML document. 
//      If the import fails, issue a warning message
//

void prCommandDocument::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
#if EnableCommandDocumentProcess == SimProcessEnabled

	ISimulation* pSim = rSimState.GetISimulation();

	IExperimentAccessControl* pEAC = pSim->GetIExperimentAccessControl();

	if(pEAC->IsExperimentAccessible() && !m_Documents.empty())
	{
		for(cStringSequenceIterator citerDoc=m_Documents.begin(); citerDoc!=m_Documents.end(); citerDoc++)
		{
			m_DocumentName = *citerDoc;

			IDocumentHandler* pDocHandle = pEAC->GetIDocumentHandler();

			ICommandDocument* pComDoc = pDocHandle->GetICommandDocument();

			if(xxFile::DoesFileExist(m_DocumentName) && pComDoc->ReadCommandDocument(m_DocumentName))
			{
				 new CLogCommandDocumentImported(pISimBox->GetCurrentTime(), GetPid(), m_DocumentName);
			}
			else
			{
				 new CLogCommandDocumentImportFailed(pISimBox->GetCurrentTime(), GetPid(), m_DocumentName);
			}
		}

		// Empty the document queue now that they have all been imported or the
		// import failed. This prevents failed imports being repeated

		m_Documents.clear();
		m_DocumentName = "";
	}

#endif
}

// Function to check that the user-specified data is valid and, if so, 
// to find the aggregates and events that are used by the process.

// An xxProcessState is created to hold the data that the process wants
// to write to file. Data is passed as a series of CTimeSeriesData objects
// as used for CHistoryState.

bool prCommandDocument::ValidateData(const CInputData &riData)
{
	SetState(new xxProcessState(xxBase::GetPSPrefix() + GetProcessType() + ToString(GetId()) + "." + riData.GetRunId(), GetStartTime(), GetEndTime()));

	return true;
}
// Function to allow the process data to be validated when it is created internally.
// This function should not be called if the ValidateData() function has
// already been called.
//
// As this process requires no data for its creation, and has no state to 
// serialise, we do not create a state object for it.
 
bool prCommandDocument::InternalValidateData(const ISimState* const pISimState)
{
	return true;
}

// ****************************************
// Implementation of IModifyProcess interface

bool prCommandDocument::IsModifiable() const
{
	return (!m_bNeverModifiable && m_bIsModifiable);
}

void prCommandDocument::SetModifiable(const xxCommand* const pCommand)
{
	const pcSetModifiable* const pCmd = dynamic_cast<const pcSetModifiable*>(pCommand);

	const zString pid = pCmd->GetPid();

	if(!m_bNeverModifiable)
	{
		m_bIsModifiable = true;

		 new CLogpcToggleProcessModificationStatus(pCmd->GetExecutionTime(), GetPid(), m_bIsModifiable);
	}
	else
	{
		 new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}
}

void prCommandDocument::SetUnmodifiable(const xxCommand* const pCommand)
{
	const pcSetUnmodifiable* const pCmd = dynamic_cast<const pcSetUnmodifiable*>(pCommand);

	const zString pid = pCmd->GetPid();

	if(!m_bNeverModifiable)
	{
		m_bIsModifiable = false;

		 new CLogpcToggleProcessModificationStatus(pCmd->GetExecutionTime(), GetPid(), m_bIsModifiable);
	}
	else
	{
		 new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}
}

// Function to allow the process to prevent any further modifications to itself.
// Note that a warning is logged if this command is sent more than once to the
// same process.

void prCommandDocument::SetIrreversibleUnmodifiable(const xxCommand* const pCommand)
{
	const pcSetIrreversibleUnmodifiable* const pCmd = dynamic_cast<const pcSetIrreversibleUnmodifiable*>(pCommand);

	const zString pid = pCmd->GetPid();

	if(!m_bNeverModifiable && m_bIsModifiable)
	{
		m_bNeverModifiable = true;
		m_bIsModifiable    = false;

		 new CLogpcProcessModificationForbidden(pCmd->GetExecutionTime(), pid);
	}
	else
	{
		 new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}

}

// ****************************************
// Implementation of IModifyCommandDocument interface

// This function adds the name of a command document to the pending documents list
// if the process is modifiable.
// 
// Note that the document is only actually imported when the UpdateState function 
// is called next. 

void prCommandDocument::AddDocument(const xxCommand* const pCommand)
{
#if EnableCommandDocumentProcess == SimProcessEnabled

	const pcCommandDocumentAddDocument* const pCmd = dynamic_cast<const pcCommandDocumentAddDocument*>(pCommand);

	const zString docName = pCmd->GetDocumentName();

	if(IsModifiable() && !docName.empty())
	{
		m_Documents.push_back(docName);

		 new CLogpcCommandDocumentAddDocument(pCmd->GetExecutionTime(), GetPid(), docName);
	}
	else
	{
		 new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}
#endif
}

// Function to return the number of command documents waiting to be imported.
// Note that we allow this command to be executed even if the process is marked 
// as unmodifiable. This is because it does not change the process state.

void prCommandDocument::GetPendingDocumentTotal(const xxCommand* const pCommand) const
{
#if EnableCommandDocumentProcess == SimProcessEnabled

	const pcCommandDocumentGetPendingDocTotal* const pCmd = dynamic_cast<const pcCommandDocumentGetPendingDocTotal*>(pCommand);

	// This command cannot fail

	 new CLogpcCommandDocumentGetPendingDocTotal(pCmd->GetExecutionTime(), GetPid(), m_Documents.size());
#endif
}

// Function to return the names of command documents waiting to be imported.
// Note that we allow this command to be executed even if the process is marked 
// as unmodifiable. This is because it does not change the process state.

void prCommandDocument::GetPendingDocuments(const xxCommand* const pCommand) const
{
#if EnableCommandDocumentProcess == SimProcessEnabled
	const pcCommandDocumentGetPendingDocs* const pCmd = dynamic_cast<const pcCommandDocumentGetPendingDocs*>(pCommand);

	// This command cannot fail

	 new CLogpcCommandDocumentGetPendingDocs(pCmd->GetExecutionTime(), GetPid(), m_Documents);
#endif
}

