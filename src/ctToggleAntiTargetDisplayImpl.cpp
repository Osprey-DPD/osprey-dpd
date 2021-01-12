/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctToggleAntiTargetDisplayImpl.cpp: implementation of the ctToggleAntiTargetDisplayImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "ctToggleAntiTargetDisplayImpl.h"
#include "ctToggleAntiTargetDisplay.h"
#include "SimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "Bead.h"
#include "LogctAntiTargetDisplay.h"
#include "LogctEmptyTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctToggleAntiTargetDisplayImpl::ctToggleAntiTargetDisplayImpl()
{
}

ctToggleAntiTargetDisplayImpl::~ctToggleAntiTargetDisplayImpl()
{

}

// Command handler to toggle the display status of all beads NOT contained in a 
// command target. 

void ctToggleAntiTargetDisplayImpl::ToggleAntiTargetDisplay(const xxCommand* const pCommand)
{
	const ctToggleAntiTargetDisplay* const pCmd = dynamic_cast<const ctToggleAntiTargetDisplay*>(pCommand);

	const zString	targetLabel	= pCmd->GetTargetLabel();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode*   pCmdTarget = pSimBox->GetCommandTarget(targetLabel);

	// Now we have the command target, we set the visibility of all beads NOT in 
	// the target to the opposite state of those beads contained in the target. 
	// That is, if the target is currently visible, the remaining beads are made  
	// invisible; if the target is invisible, the remaining beads are made visible. 
	// The display status assigned is independent of the previous display status 
	// of any non-target beads. Becaue the SimBox stores beads using their 
	// base class pointers whereas targets use CBead pointers we do two loops, first
	// setting all beads to the anti-target visibility status and then resetting
	// the target beads back to their original status.

	if(pCmdTarget)
	{
		if(pCmdTarget->GetBeadTotal() > 0)
		{
			BeadVector vTargetBeads = pCmdTarget->GetBeads();
			const bool bTargetVisible = vTargetBeads.front()->GetVisible();

		        AbstractBeadVector vAllBeads = pSimBox->GetAllBeadsInCNTCells();
			for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
			{
				(*iterBead)->SetVisible(!bTargetVisible);
			}

			for(BeadVectorIterator iterBead2=vTargetBeads.begin(); iterBead2!=vTargetBeads.end(); iterBead2++)
			{
				(*iterBead2)->SetVisible(bTargetVisible);
			}

			new CLogctAntiTargetDisplay(pSimBox->GetCurrentTime(), targetLabel, !bTargetVisible);
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
