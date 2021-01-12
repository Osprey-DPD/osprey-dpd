/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcIssueRunCompleteFractionImpl.cpp: implementation of the mcIssueRunCompleteFractionImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcIssueRunCompleteFractionImpl.h"
#include "mcIssueRunCompleteFraction.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "LogRunCompleteFraction.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcIssueRunCompleteFractionImpl::mcIssueRunCompleteFractionImpl()
{
}

mcIssueRunCompleteFractionImpl::~mcIssueRunCompleteFractionImpl()
{

}

// Command handler to output a message to the log file showing the percentage
// of the total simulation that is complete. The percentage is expressed as 
// an integer only. 
//
// If the CMonitor::m_RunCompleteInterval parameter is 0, the current time is 
// used to calculate the completed percentage and it is output immediately.
// This command then has no further effect.
//
// If the m_RunCompleteInterval parameter is non-zero, it means that the user 
// has issued an mcSetRunCompleteInterval command so we use the specified 
// percentage to determine when to issue the log message and schedule a new 
// command for the next interval. If the user subsequently issues another 
// mcSetRunCompleteInterval command it will change the stored interval and issue 
// another one of these commands. The original command will still be in the queue 
// however, and will be executed at the original time.
//
// If the user extends the totoal simulation time, this changes the percentage 
// of the run that is complete at a given time, but does not affect the sequence 
// of commands. They will continue to be executed until the simulation ends or
// the user disables the feature.
//
// The sequence of commands can be disabled by using the mcRemoveRunCompleteInterval
// command. This sets the interval to 0 and it is executed only once.

void mcIssueRunCompleteFractionImpl::IssueRunCompleteFraction(const xxCommand* const pCommand)
{
	const mcIssueRunCompleteFraction* const pCmd = dynamic_cast<const mcIssueRunCompleteFraction*>(pCommand);

	CMonitor* pMon = dynamic_cast<CMonitor*>(this);

	const long interval			= pMon->m_RunCompleteInterval;
	const long currentTime		= pMon->GetCurrentTime();
	const long totalTime		= pMon->GetTotalTime();
	const long percentComplete	= 100*currentTime/totalTime;

	// Log the time completed now and, if required, schedule the command to 
	// execute again when the next "interval" is complete. Note that the
	// intervals are measured from the start of the run, not the time the
	// command is executed. Commands that would be executed after the run
	// is complete are not logged.

	if(interval == 0)
	{
		 new CLogRunCompleteFraction(pMon->GetCurrentTime(), percentComplete);
	}
	else if(interval > 0 && interval <= 100)
	{
		 new CLogRunCompleteFraction(pMon->GetCurrentTime(), percentComplete);

		long lastInterval	= (100*currentTime/(interval*totalTime))*interval;

		long nextInterval	= lastInterval + interval;
		long nextTime		= nextInterval*totalTime/100;

		if(nextTime <= totalTime)
		{
			mcIssueRunCompleteFraction* pNextCmd = new mcIssueRunCompleteFraction(nextTime);
			pMon->GetISimBox()->AddCommand(pNextCmd);
		}
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}

}
