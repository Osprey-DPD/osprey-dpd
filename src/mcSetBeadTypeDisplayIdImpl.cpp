/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetBeadTypeDisplayIdImpl.cpp: implementation of the mcSetBeadTypeDisplayIdImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetBeadTypeDisplayIdImpl.h"
#include "mcSetBeadTypeDisplayId.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "SimState.h"
#include "CurrentState.h"
#include "Bead.h"
#include "LogBeadDisplayId.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetBeadTypeDisplayIdImpl::mcSetBeadTypeDisplayIdImpl()
{
}

mcSetBeadTypeDisplayIdImpl::~mcSetBeadTypeDisplayIdImpl()
{

}

// Command handler to change the display id for the given bead type so that
// all such beads are drawn with the same colour. Colours are associated
// with display ids in the CCurrentStateFormat class. Specifying a value 
// of -1 resets all beads to use their type as their displayId.
//
// We check that the specified bead type is allowed before modifying the
// CMonitor, and issue a command failed message if it is not.

void mcSetBeadTypeDisplayIdImpl::SetBeadTypeDisplayId(const xxCommand* const pCommand)
{
	const mcSetBeadTypeDisplayId* const pCmd = dynamic_cast<const mcSetBeadTypeDisplayId*>(pCommand);

	const long beadType = pCmd->GetBeadType();
	const long displayId   = pCmd->GetDisplayId();	

	CMonitor* pMon = dynamic_cast<CMonitor*>(this);

	CSimState* const psState = pMon->m_pSimState;

	const long    beadTotal = psState->GetInitialState().GetBeadTotalForType(beadType);
	
	if(beadType >= 0 && beadTotal > 0)
	{
		const zString beadName  = psState->GetAnalysisState().GetBeadNameFromType(beadType);

		AbstractBeadVector vAllBeads = pMon->GetISimBox()->GetBeads();

		for(cAbstractBeadVectorIterator citerBead=vAllBeads.begin(); citerBead!=vAllBeads.end(); citerBead++)
		{
			if((*citerBead)->GetType() == beadType)
			{
				if(displayId == -1)
				{
					CCurrentState::SetBeadDisplayId((*citerBead)->GetId(), (*citerBead)->GetType());
				}
				else
				{
					CCurrentState::SetBeadDisplayId((*citerBead)->GetId(), displayId);
				}
			}
		}

		new CLogBeadDisplayId(pMon->GetCurrentTime(), beadName, beadType, displayId);
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}
}
