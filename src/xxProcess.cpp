/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxProcess.cpp: implementation of the xxProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "xxProcess.h"
#include "xxProcessState.h"

#include "aaRegionToType.h"
#include "Analysis.h"
#include "xxEvent.h"
#include "xxCommand.h"
#include "xxMessage.h"

#if EnableModifiableProcess == SimProcessEnabled
// Commands implementing the IModifyProcess interface

#include "pcSetIrreversibleUnmodifiable.h"
#include "pcSetModifiable.h"
#include "pcSetUnmodifiable.h"
#endif

// Messages issued as a result of process logic

#include "LogCommandFailed.h"
#include "LogpcProcessModificationForbidden.h"
#include "LogpcToggleProcessModificationStatus.h"


// STL using declarations



// **********************************************************************
// Global Functions.
//
// Function to write out the process data to file. We pass the
// stream output operator to the contained xxProcess-derived object using
// its put() function. This is because the << and >> operators cannot be
// treated polymporphically.
//

zOutStream& operator<<(zOutStream& os, const xxProcess& process)
{
	return process.put(os);
}

// Function to read the process data from file. Because the name has
// already been read we only need to call the derived class' get() function 
// to fill in its specific data.

zInStream& operator>>(zInStream& is, xxProcess& process)
{
	return process.get(is);
}

// Static member variable holding the number of processes created.

long xxProcess::m_ProcessTotal = 0;

long xxProcess::GetProcessTotal()
{
	return m_ProcessTotal;
}
// Static member function to reset the number of processes to zero.

void xxProcess::ZeroProcessTotal()
{
	m_ProcessTotal = 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// A process must be given times to start and stop executing when it is created. 
// The SimBox will send it UpdateState() messages  at each time step to check 
// if it should perform its operations. 
// It stores a unique id labelling the order in which processes were created,
// and has a static member that holds the total number of processes created. 
// Static member functions are provided to query this value and to reset it to 
// zero between simulations. A process can be identified by its id only if
// previously-created processes are known. 
//
// The process data is written to a file with the name: prefix.<process name>m.nnn 
// where "prefix" is the string returned by the global function xxBase::GetPSPrefix(),
// and "m" is the unique id given to each process when it is created. But this 
// is provided by the xxProcessState class because we do not have access to the
// runId and other data when the xxProcessObject creates processes. The
// xxProcessState object is created in the derived class' ValidateData() functions
// because then we have access to the CInputData object.
//
// Processes are created with their validity set to true: the derived classes' 
// get() routines must set the validity to false if an error occurs while 
// reading a process' data. The m_bInitialise flag indicates if the process
// has been initialised or not: it is used to perform once-off calculations
// the first time the process is executed.

// Default constructor used by IModifyProcess interface class

// note that we fake the user-assigned pid for now and make it a string version
// of the id itself

xxProcess::xxProcess() : m_id(++xxProcess::m_ProcessTotal), 
						m_pid(ToString(m_id)),
						m_Start(0), m_End(0), 
						m_bProcessValid(true),
						m_bInitialise(true), 
						m_bIsModifiable(true),
						m_bNeverModifiable(false),
						m_pState(0)
{
}

xxProcess::xxProcess(bool bValid) : m_id(++xxProcess::m_ProcessTotal), 
									m_pid(ToString(m_id)),
									m_Start(0), m_End(0), 
									m_bProcessValid(bValid),
									m_bInitialise(true), 
									m_bIsModifiable(true),
						            m_bNeverModifiable(false),
									m_pState(0)
{
}

// Copying a process does not change its id. Also the same xxProcessState
// object pointer is passed to the new xxProcess.

xxProcess::xxProcess(const xxProcess& oldProcess) : xxBase(oldProcess), 
                                                    m_id(oldProcess.m_id), 
													m_pid(oldProcess.m_pid),
													m_Start(oldProcess.m_Start),
													m_End(oldProcess.m_End),
													m_bProcessValid(oldProcess.m_bProcessValid),
													m_bInitialise(oldProcess.m_bInitialise),
													m_bIsModifiable(oldProcess.m_bIsModifiable),
													m_bNeverModifiable(oldProcess.m_bNeverModifiable),
													m_pState(oldProcess.m_pState)
{
}

xxProcess::~xxProcess()
{
	// Delete the xxProcessState if it exists

	if(m_pState)
	{
		delete m_pState;
		m_pState = NULL;
	}
}

// Empty put() function as the data can be output using the
// putASCIIStartTag(), putASCIIEndTags() functions and their XML equivalents.

zOutStream& xxProcess::put(zOutStream& os) const
{
	return os;
}



// The "Process" keyword and the "Type" keyword and its value have already been 
// read in CInputDPDFile as they determine which xxProcess-derived object 
// to create, we fill in the general process data here and call the derived 
// class' get() function to fill in its specific data.
//
// Note that we cannot read in the aggregate's type here because different 
// processes may need more than one aggregate. We leave it to the derived 
// classes to deal with their aggregates.

zInStream& xxProcess::get(zInStream& is)
{
// Read the start and end times

	zString token;
	long start, end;

	is >> token;
	if(!is.good() || token != "Times")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> start >> end;
		if(!is.good() || start < 0 || end < start)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Data was read successfully so copy it into member variables

	SetStartTime(start);
	SetEndTime(end);

	return is;
}

// Protected functions to allow derived classes to set their own data.

void xxProcess::SetStartTime(long start)
{
	m_Start = start;
}

void xxProcess::SetEndTime(long end)
{
	m_End = end;
}

// An error on read results in the m_bProcessValid flag being set to false so
// that the read class can take action.

void xxProcess::SetDataValid(bool bValid)
{
	m_bProcessValid = bValid;
}

// Protected function to allow derived classes to store an xxProcessState pointer
// and serialize their data to file.

void xxProcess::SetState(xxProcessState *pState)
{
	m_pState = pState;
}

// Public function to tell the enclosed xxProcessState object to
// write its data to the process state file. If there is no state object,
// because the process was internally created for example, we just return true.

bool xxProcess::SaveState()	const
{
	if(m_pState)
	{
		return m_pState->Serialize();
	}
	else
		return true;
}

// Public function to allow processes that are created internally as
// the result of commands to validate their data and create their
// associated process state object. We pass the ISimState interface
// to give the process access to the simulation data.

// This is a do-nothing function that should be overridden by any process
// that is to be internally created.

bool xxProcess::InternalValidateData(const ISimState* const pISimState)
{
	return true;
}

// **********************************************************************
// Protected functions to allow a process to modify/initialise its events.
//
// These functions can only be called by derived classes so that only an event 
// (via its base class function) or its parent process can modify events, 
// and only a process can activate an event.
//
// The event is identified by its (unique) integer id.

void xxProcess::ActivateEvent(long id)
{
	EventIterator iterEvent = find_if(m_Events.begin(), m_Events.end(), aaGetEventId(id));

	if(iterEvent != m_Events.end())
		(*iterEvent)->ActivateEvent();
}

void xxProcess::InActivateEvent(long id)
{
	EventIterator iterEvent = find_if(m_Events.begin(), m_Events.end(), aaGetEventId(id));

	if(iterEvent != m_Events.end())
		(*iterEvent)->InActivateEvent();
}

// Protected function to allow a process to remove an event from the SimBox's
// event sequence. This does not actually delete the xxEvent itself, it only
// removes the SimBox's pointer to it. It can later be restored by the process.
// An event cannot delete itself, only its parent process can do so. The event
// is removed from the event sequence using the command ccRemoveEvent targetted
// to the SimBox.
// 
// The event is identified by its (unique) integer id.

void xxProcess::DeleteEvent(long id)
{

}

// Function to add an event to the processes event sequence.
// It sets the start and end times for the event to watch for its 
// particular change in an aggregate before adding it.
// Note that it cannot be added to the CAnalysisState nor the CSimBox
// because there is no access to them from a process.

void xxProcess::AddEvent(xxEvent* pEvent, long start, long end)
{
	pEvent->SetStartTime(start);
	pEvent->SetEndTime(end);


	m_Events.push_back(pEvent);
}

// ****************************************
// Implementation of IModifyProcess interface

bool xxProcess::InternalIsModifiable() const
{
	return (!m_bNeverModifiable && m_bIsModifiable);
}

void xxProcess::InternalSetModifiable(const xxCommand* const pCommand)
{
#if EnableModifiableProcess == SimProcessEnabled

	const pcSetModifiable* const pCmd = dynamic_cast<const pcSetModifiable*>(pCommand);

	const zString pid = pCmd->GetPid();

	if(!m_bNeverModifiable)
	{
		m_bIsModifiable = true;

		CLogpcToggleProcessModificationStatus* pMsg = new CLogpcToggleProcessModificationStatus(pCmd->GetExecutionTime(), GetPid(), m_bIsModifiable);
	}
	else
	{
		CLogCommandFailed* pMsg = new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}

#endif
}

void xxProcess::InternalSetUnmodifiable(const xxCommand* const pCommand)
{
#if EnableModifiableProcess == SimProcessEnabled

    const pcSetUnmodifiable* const pCmd = dynamic_cast<const pcSetUnmodifiable*>(pCommand);

	const zString pid = pCmd->GetPid();

	if(!m_bNeverModifiable)
	{
		m_bIsModifiable = false;

		CLogpcToggleProcessModificationStatus* pMsg = new CLogpcToggleProcessModificationStatus(pCmd->GetExecutionTime(), GetPid(), m_bIsModifiable);
	}
	else
	{
		CLogCommandFailed* pMsg = new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}

#endif
}

// Function to allow the process to prevent any further modifications to itself.
// Note that a warning is logged if this command is sent more than once to the
// same process.

void xxProcess::InternalSetIrreversibleUnmodifiable(const xxCommand* const pCommand)
{
#if EnableModifiableProcess == SimProcessEnabled

	const pcSetIrreversibleUnmodifiable* const pCmd = dynamic_cast<const pcSetIrreversibleUnmodifiable*>(pCommand);

	const zString pid = pCmd->GetPid();

	if(!m_bNeverModifiable && m_bIsModifiable)
	{
		m_bNeverModifiable = true;
		m_bIsModifiable    = false;

		CLogpcProcessModificationForbidden* pMsg = new CLogpcProcessModificationForbidden(pCmd->GetExecutionTime(), pid);
	}
	else
	{
		CLogCommandFailed* pMsg = new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}

#endif
}

