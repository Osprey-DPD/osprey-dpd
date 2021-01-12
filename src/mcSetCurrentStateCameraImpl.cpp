/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetCurrentStateCameraImpl.cpp: implementation of the mcSetCurrentStateCameraImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetCurrentStateCameraImpl.h"
#include "mcSetCurrentStateCamera.h"
#include "Monitor.h"
#include "SimState.h"
#include "LogCurrentStateCamera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetCurrentStateCameraImpl::mcSetCurrentStateCameraImpl()
{
}

mcSetCurrentStateCameraImpl::~mcSetCurrentStateCameraImpl()
{

}

// Command handler to set the coordinates of the camera and target in the
// CCurrentState snapshot file. The coordinates are specified as multiples of
// the SimBox size and converted to actual coordinates before passing them
// to the CCurrentState.

void mcSetCurrentStateCameraImpl::SetCurrentStateCamera(const xxCommand* const pCommand)
{
	const mcSetCurrentStateCamera* const pCmd = dynamic_cast<const mcSetCurrentStateCamera*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

    // We reject the command and log a command failed message if the camera and target
    // coordinates are identical as this is forbidden by Povray.

#if EnableParallelCommands == SimMPSEnabled
    if(!(pCmd->GetCamera(0) == pCmd->GetTarget(0) &&
         pCmd->GetCamera(1) == pCmd->GetTarget(1) &&
         pCmd->GetCamera(2) == pCmd->GetTarget(2)))
    {
	    pMon->m_Camera[0] = pCmd->GetCamera(0)*pMon->m_pSimState->GetSimSpaceXLength();
	    pMon->m_Camera[1] = pCmd->GetCamera(1)*pMon->m_pSimState->GetSimSpaceYLength();
	    pMon->m_Camera[2] = pCmd->GetCamera(2)*pMon->m_pSimState->GetSimSpaceZLength();

	    pMon->m_Target[0] = pCmd->GetTarget(0)*pMon->m_pSimState->GetSimSpaceXLength();
	    pMon->m_Target[1] = pCmd->GetTarget(1)*pMon->m_pSimState->GetSimSpaceYLength();
	    pMon->m_Target[2] = pCmd->GetTarget(2)*pMon->m_pSimState->GetSimSpaceZLength();

        if(xxParallelBase::GlobalGetRank() == 0 && pCmd->IsExecutionLogged())
        {
		    new CLogCurrentStateCamera(pMon->GetCurrentTime(), 
											    pMon->m_Camera[0], pMon->m_Camera[1], pMon->m_Camera[2],
											    pMon->m_Target[0], pMon->m_Target[1], pMon->m_Target[2]);
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
    if(!(pCmd->GetCamera(0) == pCmd->GetTarget(0) &&
         pCmd->GetCamera(1) == pCmd->GetTarget(1) &&
         pCmd->GetCamera(2) == pCmd->GetTarget(2)))
    {
	    pMon->m_Camera[0] = pCmd->GetCamera(0)*pMon->m_SimBoxXLength;
	    pMon->m_Camera[1] = pCmd->GetCamera(1)*pMon->m_SimBoxYLength;
	    pMon->m_Camera[2] = pCmd->GetCamera(2)*pMon->m_SimBoxZLength;

	    pMon->m_Target[0] = pCmd->GetTarget(0)*pMon->m_SimBoxXLength;
	    pMon->m_Target[1] = pCmd->GetTarget(1)*pMon->m_SimBoxYLength;
	    pMon->m_Target[2] = pCmd->GetTarget(2)*pMon->m_SimBoxZLength;

	    if(pCmd->IsExecutionLogged())
	    {
		    new CLogCurrentStateCamera(pMon->GetCurrentTime(), 
											    pMon->m_Camera[0], pMon->m_Camera[1], pMon->m_Camera[2],
											    pMon->m_Target[0], pMon->m_Target[1], pMon->m_Target[2]);
	    }	
    }
    else
    {
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
    }
#endif
}
