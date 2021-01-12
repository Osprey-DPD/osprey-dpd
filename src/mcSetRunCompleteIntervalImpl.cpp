/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetRunCompleteIntervalImpl.cpp: implementation of the mcSetRunCompleteIntervalImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetRunCompleteIntervalImpl.h"
#include "mcSetRunCompleteInterval.h"
#include "mcIssueRunCompleteFraction.h"
#include "ISimBox.h"
#include "Monitor.h"
#include "LogSetRunCompleteInterval.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetRunCompleteIntervalImpl::mcSetRunCompleteIntervalImpl()
{
}

mcSetRunCompleteIntervalImpl::~mcSetRunCompleteIntervalImpl()
{

}

// Command handler to set the interval at which "mcIssueRunCompleteFraction" commands
// are issued. The interval is expressed as a percentage of the total run time,but
// can only be an integer. Its value is stored in the CMonitor class and may be 
// changed by subsequent commands or if the total simulation time is extended by 
// command. Once it has set the interval, it issues an mcIssueRunCompleteFraction
// command to output the percentage of the total simulation time that is complete.

void mcSetRunCompleteIntervalImpl::SetRunCompleteInterval(const xxCommand* const pCommand)
{
	const mcSetRunCompleteInterval* const pCmd = dynamic_cast<const mcSetRunCompleteInterval*>(pCommand);

	const long interval = pCmd->GetInterval();

	CMonitor* pMon = dynamic_cast<CMonitor*>(this);

	const long currentTime = pMon->GetCurrentTime();
	const long totalTime   = pMon->GetTotalTime();
	const long intervalTime = interval*totalTime/100;

	// If the interval is not an integer in [1,100] the command fails; the command
	// also fails if the interval is less than 1 time step as may happen if the
	// run is short and the interval is small. Note that specifying the same 
	// interval as already exists does not cause a failure but the command will 
	// have no apparent effect.

	if(interval >= 1 && interval <= 100 && intervalTime >0)
	{
		// Only start the logging command sequence if this is the first time that 
		// logging is turned on: subsequent commands simply change the logging interval.
		// This prevents more than one sequene of logging commands being active.
		
		if(pMon->m_RunCompleteInterval == 0)
		{
			long lastInterval	= (100*currentTime/(interval*totalTime))*interval;
			long nextInterval	= lastInterval + interval;
			long nextTime		= nextInterval*totalTime/100;
			
			mcIssueRunCompleteFraction* pNextCmd = new mcIssueRunCompleteFraction(nextTime);
			pMon->GetISimBox()->AddCommand(pNextCmd);
		}

		pMon->m_RunCompleteInterval = interval;

		new CLogSetRunCompleteInterval(pMon->GetCurrentTime(), interval);
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}
}
