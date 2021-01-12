/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSavePovrayCurrentStateImpl.cpp: implementation of the mcSavePovrayCurrentStateImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSavePovrayCurrentStateImpl.h"
#include "mcSavePovrayCurrentState.h"
#include "Monitor.h"
#include "PovrayFormat.h"
#include "LogSaveCurrentState.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSavePovrayCurrentStateImpl::mcSavePovrayCurrentStateImpl()
{
}

mcSavePovrayCurrentStateImpl::~mcSavePovrayCurrentStateImpl()
{

}

// Function to write a current state to file in the Povray format. The scene
// data required to set up the output must be set before executing this command
// or the default values will be used.
//
// Note that current states that are written as the result of commands
// are not saved or included in the current state analysis.
//

void mcSavePovrayCurrentStateImpl::SavePovrayCurrentState(const xxCommand* const pCommand)
{
	const mcSavePovrayCurrentState* const pCmd = dynamic_cast<const mcSavePovrayCurrentState*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

#if EnableParallelCommands == SimMPSEnabled

	CPovrayFormat* pFormat = new CPovrayFormat(pMon->m_SimBoxXLength, pMon->m_SimBoxYLength, pMon->m_SimBoxZLength,
											   pMon->m_bDisplayBox, pMon->m_BeadTypeSize, pMon->m_Camera, pMon->m_Target,
											   pMon->m_vLightX, pMon->m_vLightY, pMon->m_vLightZ);
											   
    // Only P0 logs the command execution, other processors simply execute the command.
	
    if(xxParallelBase::GlobalGetRank() == 0)
	{
        if(pMon->InternalSaveCurrentState(pFormat))
	    {
		    if(pCmd->IsExecutionLogged())
			{
		         new CLogSaveCurrentState(pMon->GetCurrentTime());
			}
	    }
	    else
	    {
		     new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	    }
	}
	else
	{
	    pMon->InternalSaveCurrentState(pFormat);
	}
	
#else

	CPovrayFormat* pFormat = new CPovrayFormat(pMon->m_SimBoxXLength, pMon->m_SimBoxYLength, pMon->m_SimBoxZLength,
											   pMon->m_bDisplayBox, pMon->m_BeadTypeSize, pMon->m_Camera, pMon->m_Target,
											   pMon->m_vLightX, pMon->m_vLightY, pMon->m_vLightZ);

    if(pMon->InternalSaveCurrentState(pFormat))
	{
		 new CLogSaveCurrentState(pMon->GetCurrentTime());
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}
	
#endif
}
