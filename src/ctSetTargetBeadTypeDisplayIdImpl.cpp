/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctSetTargetBeadTypeDisplayIdImpl.cpp: implementation of the ctSetTargetBeadTypeDisplayIdImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctSetTargetBeadTypeDisplayIdImpl.h"
#include "ctSetTargetBeadTypeDisplayId.h"
#include "SimBox.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "CurrentState.h"
#include "Bead.h"
#include "LogctTargetBeadTypeDisplayId.h"
#include "LogctEmptyTarget.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctSetTargetBeadTypeDisplayIdImpl::ctSetTargetBeadTypeDisplayIdImpl()
{
}

ctSetTargetBeadTypeDisplayIdImpl::~ctSetTargetBeadTypeDisplayIdImpl()
{

}

// Command handler to set the displayId of all beads of the specified type in
// the selected target to a given value. A value of -1 resets the beads to use 
// their type as their display id.

void ctSetTargetBeadTypeDisplayIdImpl::SetTargetBeadTypeDisplayId(const xxCommand* const pCommand)
{
	const ctSetTargetBeadTypeDisplayId* const pCmd = dynamic_cast<const ctSetTargetBeadTypeDisplayId*>(pCommand);

	const zString	targetLabel = pCmd->GetTargetLabel();
        const long     beadType    = pCmd->GetBeadType();
	const long     displayId    = pCmd->GetDisplayId();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode*   pCmdTarget = pSimBox->GetCommandTarget(targetLabel);

	if(pCmdTarget)
	{
		// Now we have the command target we loop over all beads contained in it 
		// and set their displayId appropriately. Note that if the target is composite, 
		// the GetBeads() function recurses into all its contained targets.
		// Also note that if the target is empty we issue a warning message but not
		// an error.

		BeadVector vTargetBeads = pCmdTarget->GetBeads();

		if(!vTargetBeads.empty())
		{
			if(displayId == -1)
			{
				for(BeadVectorIterator iterBead=vTargetBeads.begin(); iterBead!=vTargetBeads.end(); iterBead++)
				{
					if((*iterBead)->GetType() == beadType)
					{
                        (*iterBead)->SetVisible(pSimBox->GetISimBox()->GetMonitor()->IsBeadTypeVisible((*iterBead)->GetType()));
					    CCurrentState::SetBeadDisplayId((*iterBead)->GetId(), (*iterBead)->GetType());
					}
				}
			}
			else
			{
				for(BeadVectorIterator iterBead=vTargetBeads.begin(); iterBead!=vTargetBeads.end(); iterBead++)
				{
					if((*iterBead)->GetType() == beadType)
					{
                        (*iterBead)->SetVisible();
						CCurrentState::SetBeadDisplayId((*iterBead)->GetId(), displayId);
					}
				}
			}

			new CLogctTargetBeadTypeDisplayId(pSimBox->GetCurrentTime(), targetLabel, beadType, displayId);
		}
		else
		{
			new CLogctEmptyTarget(pSimBox->GetCurrentTime(), targetLabel);
		}
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
