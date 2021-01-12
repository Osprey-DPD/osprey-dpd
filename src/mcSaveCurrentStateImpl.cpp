/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveCurrentStateImpl.cpp: implementation of the mcSaveCurrentStateImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveCurrentStateImpl.h"
#include "mcSaveCurrentState.h"
#include "Monitor.h"
#include "LogSaveCurrentState.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveCurrentStateImpl::mcSaveCurrentStateImpl()
{
}

mcSaveCurrentStateImpl::~mcSaveCurrentStateImpl()
{

}

// Command handler function to save a current state to file.
// Because there is already a CMonitor function called SaveCurrentState,
// and we want to be able to log execution of commands, we have
// added "Cmd" to the end of this handler function.

void mcSaveCurrentStateImpl::SaveCurrentStateCmd(const xxCommand* const pCommand)
{
	const mcSaveCurrentState* const pCmd = dynamic_cast<const mcSaveCurrentState*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

#if EnableParallelMonitor == SimMPSEnabled

    // We have to call the parallel-aware function in the Monitor so that P0 can collect the current stata snapshot data
	// from all processors. Only P0 logs execution of the command if required.
	
	std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " about to call SaveCurrentStateP at time " << pMon->GetCurrentTime() << zEndl;
	
	pMon->SaveCurrentStateP();

	std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " finished calling  SaveCurrentStateP at time " << pMon->GetCurrentTime() << zEndl;

	if(xxParallelBase::GlobalGetRank() == 0 && pCmd->IsExecutionLogged())
	{
		 new CLogSaveCurrentState(pMon->GetCurrentTime());
	}
	
#else
	pMon->SaveCurrentState();

	if(pCmd->IsExecutionLogged())
	{
		 new CLogSaveCurrentState(pMon->GetCurrentTime());
	}
#endif
}

