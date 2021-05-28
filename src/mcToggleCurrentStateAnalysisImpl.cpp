/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcToggleCurrentStateAnalysisImpl.cpp: implementation of the mcToggleCurrentStateAnalysisImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcToggleCurrentStateAnalysisImpl.h"
#include "mcToggleCurrentStateAnalysis.h"
#include "Monitor.h"
#include "LogToggleCurrentStateAnalysis.h"
#include "LogCommandForbiddenInParallelRun.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcToggleCurrentStateAnalysisImpl::mcToggleCurrentStateAnalysisImpl()
{
}

mcToggleCurrentStateAnalysisImpl::~mcToggleCurrentStateAnalysisImpl()
{

}

// Command handler functionto toggle on or off storage and analysis of the 
// current state snap shots created during the simulation. Snapshots are 
// always written to file, this command only toggles whether the current 
// states are saved in the Monitor and analysed at the end of a run.
//
// The default is not to save the snapshots.
// This command is disabled in the parallel code because the datasets may 
// be enormous.

void mcToggleCurrentStateAnalysisImpl::ToggleCurrentStateAnalysis(const xxCommand* const pCommand)
{
    CMonitor* pMon = dynamic_cast<CMonitor*>(this);

#if SimMPS == SimulationDisabled
	pMon->m_bCurrentStateAnalysis = !pMon->m_bCurrentStateAnalysis;

	// This command cannot fail

	new CLogToggleCurrentStateAnalysis(pMon->GetCurrentTime(), pMon->m_bCurrentStateAnalysis);
#elif SimMPS == SimulationEnabled
    const mcToggleCurrentStateAnalysis* const pCmd = dynamic_cast<const mcToggleCurrentStateAnalysis*>(pCommand);
    
	new CLogCommandForbiddenInParallelRun(pMon->GetCurrentTime(), pCmd);
#endif
}

