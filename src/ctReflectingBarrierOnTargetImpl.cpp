/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctReflectingBarrierOnTargetImpl.cpp: implementation of the ctReflectingBarrierOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctReflectingBarrierOnTargetImpl.h"
#include "ctReflectingBarrierOnTarget.h"
#include "SimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "taReflectingBarrier.h"
#include "LogctReflectingBarrierOnTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctReflectingBarrierOnTargetImpl::ctReflectingBarrierOnTargetImpl()
{
}

ctReflectingBarrierOnTargetImpl::~ctReflectingBarrierOnTargetImpl()
{

}

// Command handler function apply a create a reflecting barrier at a given position along an axis
// that prevents all beads in the command target from crossing it from either side.
// Note that the barrier may be placed at the origin (mag = 0) and the opposite end (mag = 1).
// Only the three major axes may be specified as the normal, and this has been checked in the calling routine.

void ctReflectingBarrierOnTargetImpl::ReflectingBarrierOnTarget(const xxCommand* const pCommand)
{
	const ctReflectingBarrierOnTarget* const pCmd = dynamic_cast<const ctReflectingBarrierOnTarget*>(pCommand);

	const zString targetLabel = pCmd->GetTargetLabel();
	const zString decLabel    = pCmd->GetDecLabel();

	const long xn = pCmd->GetXNormal();
	const long yn = pCmd->GetYNormal();
	const long zn = pCmd->GetZNormal();

	const double mag = pCmd->GetMagnitude();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(targetLabel);

	// Now we have the target, create a taReflectingBarrier decorator object
	// and wrap the target with it to enable the force to be applied to the
	// target's beads. Note that because we use a Composite pattern for the
	// command targets, we don't need to know whether it is composite or
	// simple (or a bead or polymer target) as the GetBeads() function just returns all beads in the target.

	if(pCmdTarget)
	{
		CCommandTargetNode* pActiveTarget = pSimBox->GetActiveCommandTargetOutermost(targetLabel);

		// If the target is already active, remove it before adding it back
		// using the new decorator: we just check if the label of the
		// outermost decorator instance differs from the target's label.
		// Note that because we now know that pCmdTarget exists, we don't 
		// have to check that pActiveTarget is non-NULL as it must either be
		// a pointer to a decorator instance or equal to pCmdTarget.

		if(pActiveTarget->GetLabel() != targetLabel)
		{
			pSimBox->m_ActiveCommandTargets.remove(pActiveTarget);
		}

		// Wrap the active target in a new force decorator instance
		// and add it to the active command targets container.

		taReflectingBarrier* const pDec = new taReflectingBarrier(decLabel, pActiveTarget, xn, yn, zn, mag);

		pSimBox->m_ActiveCommandTargets.push_back(pDec);

		// Log sucessful execution of the command

		new CLogctReflectingBarrierOnTarget(pSimBox->GetCurrentTime(), targetLabel, decLabel, xn, yn, zn, mag);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
