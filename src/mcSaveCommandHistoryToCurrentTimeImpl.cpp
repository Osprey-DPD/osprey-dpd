/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveCommandHistoryToCurrentTimeImpl.cpp: implementation of the mcSaveCommandHistoryToCurrentTimeImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveCommandHistoryToCurrentTimeImpl.h"
#include "mcSaveCommandHistoryToCurrentTime.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "CommandFile.h"
#include "LogSaveCommandHistoryToCurrentTime.h"
#include "aaRegionToType.h"	// Needed for aaCommandExecutionTimeGreater() function object


// STL using declarations

	using std::remove_if;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveCommandHistoryToCurrentTimeImpl::mcSaveCommandHistoryToCurrentTimeImpl()
{
}

mcSaveCommandHistoryToCurrentTimeImpl::~mcSaveCommandHistoryToCurrentTimeImpl()
{

}

// Handler function to write a list of those commands in the current simulation
// whose execution times are earlier than the current command 
// to a command document so that they can be imported into subsequent runs.
// The name of the document is automatically generated:
//
//  prefix.runId.xml
//
// where, "prefix" defines the name of the command history file and is returned
// by the xxBase::GetCHPrefix() function. If the EnableXMLCommands flag is disabled,
// the ".xml" suffix is not added to the file name.
//
// We keep the protocolId for future use when the Experiment is plumbed in.

void mcSaveCommandHistoryToCurrentTimeImpl::SaveCommandHistoryToCurrentTime(const xxCommand* const pCommand)
{
	const mcSaveCommandHistoryToCurrentTime* const pCmd = dynamic_cast<const mcSaveCommandHistoryToCurrentTime*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	const ISimBox* const pISimBox = pMon->GetISimBox();

	const zString protocolId	= pISimBox->GetInitialStateType();
	const zString runId			= pISimBox->GetRunId();

	zString extension = ".con." + pMon->ToString(pMon->GetCurrentTime());

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	extension += ".xml";

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	extension += ".dat";

#endif

	// Construct a CCommandFile instance passing it the essential information
	// to identify the command document and a container of all commands. 
	// We create a new container that holds only those commands whose execution times
	// are less than or equal to the current time. This requires that commands in
	// the original container be ordered.

	// Note that because we are inside the implementation of a command there will
	// always be at least one command in the container. Finally, call its
	// Serialize function to write the data to file and issue a message to 
	// indicate the successful, or otherwise, execution of the command.

	CommandSequence allCommands;
	allCommands = pISimBox->GetCommands();

	CommandIterator lastCommand = remove_if(allCommands.begin(), allCommands.end(), aaCommandExecutionTimeGreater(pMon->GetCurrentTime()));

	CommandSequence executedCommands;
	copy(allCommands.begin(), lastCommand, back_inserter(executedCommands));


	CCommandFile commandFile(pISimBox, protocolId, runId, extension, executedCommands);

	if(commandFile.Serialize())
	{
		new CLogSaveCommandHistoryToCurrentTime(pMon->GetCurrentTime(), commandFile.GetFileName());
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}

}
