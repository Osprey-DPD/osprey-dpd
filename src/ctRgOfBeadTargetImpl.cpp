/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctRgOfBeadTargetImpl.cpp: implementation of the ctRgOfBeadTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctRgOfBeadTargetImpl.h"
#include "ctRgOfBeadTarget.h"
#include "taRgBead.h"
#include "SimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "CommandTarget.h"
#include "ctRemoveCommandTargetActivity.h"
#include "LogctRgBeadTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctRgOfBeadTargetImpl::ctRgOfBeadTargetImpl()
{
}

ctRgOfBeadTargetImpl::~ctRgOfBeadTargetImpl()
{

}
// Command handler to calculate the CM and Rg for all Beads in a Bead command target.
//
// The target may be simple or composite, and may contain different types
// of Beads, they are all included in the calculation.

void ctRgOfBeadTargetImpl::RgBeadTarget(const xxCommand* const pCommand)
{
	const ctRgOfBeadTarget* const pCmd = dynamic_cast<const ctRgOfBeadTarget*>(pCommand);

	const zString	targetLabel	= pCmd->GetTargetLabel();
	const zString	decLabel	= pCmd->GetDecLabel();
	const long		start		= pCmd->GetStartTime();
	const long		end			= pCmd->GetEndTime();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list 

	CCommandTargetNode*   pCmdTarget = pSimBox->GetCommandTarget(targetLabel);
    CCommandTarget* pBeadTarget = dynamic_cast<CCommandTarget*>(pCmdTarget);
    
    // Only create the decorator if we really have a Bead target (and not a polymer target for example),
    // it is non-empty, and the time interval over which to calculate the MSD is later than the execution time
    // of this command.

	if(pBeadTarget && pBeadTarget->GetBeadTotal() > 0 && start >= pSimBox->GetCurrentTime())
	{
        const long BeadTotal = pBeadTarget->GetBeadTotal();
        
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

		// Wrap the target in the decorator and add it to the active command 
        // targets container.

		taRgBead* const pDec = new taRgBead(decLabel, pActiveTarget, start, end);
		pSimBox->m_ActiveCommandTargets.push_back(pDec);
        
		// Note that the same message is used to log the start of the calculation (no distances specified
		// in the constructor), and the end of the calculation (last parameters = CM and Rg)

		new CLogctRgBeadTarget(pSimBox->GetCurrentTime(), targetLabel, decLabel, start, end, BeadTotal);

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
