/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetSamplePeriodImpl.cpp: implementation of the mcSetSamplePeriodImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetSamplePeriodImpl.h"
#include "mcSetSamplePeriod.h"
#include "Monitor.h"
#include "LogSetSamplePeriod.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetSamplePeriodImpl::mcSetSamplePeriodImpl()
{
}

mcSetSamplePeriodImpl::~mcSetSamplePeriodImpl()
{

}

// Command handler function to set the number of timesteps between data samples
// used in the calculating time-averaged data.

void mcSetSamplePeriodImpl::SetSamplePeriod(const xxCommand* const pCommand)
{
	const mcSetSamplePeriod* const pCmd = dynamic_cast<const mcSetSamplePeriod*>(pCommand);

	const long newPeriod = pCmd->GetPeriod();

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

    // We reject the command and log a command failed message if the new period
	// is not a positive integer, but this is checked in the CMonitor instance.

#if EnableParallelCommands == SimMPSEnabled
    if(pMon->InternalSetSamplePeriod(newPeriod))
    {
        if(xxParallelBase::GlobalGetRank() == 0 && pCmd->IsExecutionLogged())
        {
		new CLogSetSamplePeriod(pMon->GetCurrentTime(), newPeriod);
        }
    }
    else
    {
        if(xxParallelBase::GlobalGetRank() == 0)
        {
		new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
        }
        else
        {
            // send a pmParallelCommandFailed error message back to P0 so that it can
            // log a CLogParallelCommandFailed message to the log file.
        }
    }
#else
    if(pMon->InternalSetSamplePeriod(newPeriod))
    {
	    if(pCmd->IsExecutionLogged())
	    {
		new CLogSetSamplePeriod(pMon->GetCurrentTime(), newPeriod);
	    }	
    }
    else
    {
	        new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
    }
#endif
}
