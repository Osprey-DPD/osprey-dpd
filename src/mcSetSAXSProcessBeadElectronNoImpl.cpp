/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetSAXSProcessBeadElectronNoImpl.cpp: implementation of the mcSetSAXSProcessBeadElectronNoImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetSAXSProcessBeadElectronNoImpl.h"
#include "mcSetSAXSProcessBeadElectronNo.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "SimState.h"
#include "CurrentState.h"
#include "Bead.h"
#include "aaRegionToType.h"
#include "prSAXS.h"
#include "LogSetSAXSProcessBeadElectronNo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetSAXSProcessBeadElectronNoImpl::mcSetSAXSProcessBeadElectronNoImpl()
{
}

mcSetSAXSProcessBeadElectronNoImpl::~mcSetSAXSProcessBeadElectronNoImpl()
{

}

// Command handler to assign anumber of electrons to a bead type typically to be used in the calculation of a SAXS scattering function.
// We use a simple model in which N electrons (N may be fractional) are positioned at the centre of the beads of the specified type.
// The scattering function then depends on this number, more elctrons leads to greater scattering.
//
// We check that the specified bead type exists before modifying the target process. If the process or bead type don't exist, we exit with a
// command failed message.

void mcSetSAXSProcessBeadElectronNoImpl::SetSAXSProcessBeadElectronNo(const xxCommand* const pCommand)
{
	const mcSetSAXSProcessBeadElectronNo* const pCmd = dynamic_cast<const mcSetSAXSProcessBeadElectronNo*>(pCommand);

    const long    pid      = pCmd->GetProcessId();
	const zString beadName = pCmd->GetBeadName();
	const double  eno      = pCmd->GetEno();

	CMonitor* pMon = dynamic_cast<CMonitor*>(this);

	CSimState* const psState = pMon->m_pSimState;

	const long beadType  = psState->GetAnalysisState().GetBeadTypeFromName(beadName);
	const long beadTotal = psState->GetInitialState().GetBeadTotalForType(beadType);
    
    // Check if the target process exists. Its name is made of the string "SAXS" and the counter pid that is incremented for each
    // process created in the run. We check that the specified process is of the correct type.
    
    ProcessSequence vProcesses = pMon->GetISimBox()->GetProcesses();
    ProcessIterator iterProc = find_if(vProcesses.begin(), vProcesses.end(), aaGetProcessId(pid));
        
	if(beadType >= 0 && beadTotal > 0 && iterProc!= vProcesses.end())
	{
        prSAXS* pSAXS = dynamic_cast<prSAXS*>(*iterProc);
        
        if(pSAXS && pSAXS->SetBeadTypeElectronNo(beadType, eno))
        {
            const zString procName = prSAXS::GetType() + pMon->ToString(pid);
                   
            new CLogSetSAXSProcessBeadElectronNo(pMon->GetCurrentTime(), procName, beadName, beadType, beadTotal, eno);
        }
        else
        {
            new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
        }
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}
}
