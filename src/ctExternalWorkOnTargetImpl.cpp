/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctExternalWorkOnTargetImpl.cpp: implementation of the ctExternalWorkOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctExternalWorkOnTargetImpl.h"
#include "ctExternalWorkOnTarget.h"
#include "taExternalWork.h"
#include "taForceDecorator.h"
#include "SimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "ctRemoveCommandTargetActivity.h"
#include "LogctExternalWorkOnTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctExternalWorkOnTargetImpl::ctExternalWorkOnTargetImpl()
{
}

ctExternalWorkOnTargetImpl::~ctExternalWorkOnTargetImpl()
{

}

// Command handler to calculate the work done on a set of beads in a command target.
// Only beads of the specified type are included in the sum of F.dx, and the
// calculation uses the stored values of the beads' position coordinate increments
// since the last time step and the current force.

void ctExternalWorkOnTargetImpl::ExternalWorkOnTarget(const xxCommand* const pCommand)
{
	const ctExternalWorkOnTarget* const pCmd = dynamic_cast<const ctExternalWorkOnTarget*>(pCommand);

	const zString	targetLabel	= pCmd->GetTargetLabel();
	const zString	forceLabel	= pCmd->GetForceLabel();
	const zString	decLabel	= pCmd->GetDecLabel();
	const long		start		= pCmd->GetStartTime();
	const long		end			= pCmd->GetEndTime();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list and the force decorator whose
	// work is to be measured

	CCommandTargetNode*   pCmdTarget = pSimBox->GetCommandTarget(targetLabel);
	taForceDecorator*   pForceTarget = dynamic_cast<taForceDecorator*>(pSimBox->GetCommandTargetActivity(forceLabel));

	// Now we have the command target and the force decorator we can create the
	// taExternalWork instance to do the calculation of the work done. Note
	// that it is only beads contained in the pForceTarget wrapper that contribute 
	// to the work, not all the beads in the pCmdTarget (although they may be the same).

	if(pCmdTarget && pForceTarget)
	{
		const long beadType         = pForceTarget->GetCurrentBeadType();
		const long originalBeadType = pForceTarget->GetOriginalBeadType();
		const zString beadName      = pSimBox->GetBeadNameFromType(originalBeadType);
		const long beadTotal		= pForceTarget->GetBeadTotal();

		// If the target is already active, remove it before adding it back
		// using the new decorator: we just check if the label of the
		// outermost decorator instance differs from the target's label.
		// Note that because we now know that pCmdTarget exists, we don't 
		// have to check that pActiveTarget is non-NULL as it must either be
		// a pointer to a decorator instance or equal to pCmdTarget.

		CCommandTargetNode* pActiveTarget = pSimBox->GetActiveCommandTargetOutermost(targetLabel);

		if(pActiveTarget->GetLabel() != targetLabel)
		{
			pSimBox->m_ActiveCommandTargets.remove(pActiveTarget);
		}

		// Wrap the active target in a new constant force decorator instance
		// and add it to the active command targets container.

		taExternalWork* const pDec = new taExternalWork(decLabel, pActiveTarget, pForceTarget, start, end);
		pSimBox->m_ActiveCommandTargets.push_back(pDec);

		// Note that the same message is used to log the start of the calculation (4th argument = true),
		// and the end of the calculation (4th argument = false, last parameter = work done)

		new CLogctExternalWorkOnTarget(pSimBox->GetCurrentTime(), targetLabel, forceLabel, decLabel, 
												start, end, beadName, beadType, beadTotal);

		// Now the command has been created to execute during the specified interval,
		// issue a command to remove its associated activity at the time step after
		// its final measurement.

		pSimBox->AddCommand(new ctRemoveCommandTargetActivity(end+1, decLabel));
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
