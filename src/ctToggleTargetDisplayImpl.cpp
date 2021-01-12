/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctToggleTargetDisplayImpl.cpp: implementation of the ctToggleTargetDisplayImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctToggleTargetDisplayImpl.h"
#include "ctToggleTargetDisplay.h"
#include "SimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "Bead.h"
#include "LogctTargetDisplay.h"
#include "LogctEmptyTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctToggleTargetDisplayImpl::ctToggleTargetDisplayImpl()
{
}

ctToggleTargetDisplayImpl::~ctToggleTargetDisplayImpl()
{

}

// Command handler to toggle the display status of all beads contained in a command
// target. It applies recursively to all targets within a composite target, and
// sets each bead's visibility to the oppositate state. Note that the beads do not
// all have to belong to the same polymer type or be related in any way other than
// that they are all contained in the same target.

void ctToggleTargetDisplayImpl::ToggleTargetDisplay(const xxCommand* const pCommand)
{
	const ctToggleTargetDisplay* const pCmd = dynamic_cast<const ctToggleTargetDisplay*>(pCommand);

	const zString	targetLabel	= pCmd->GetTargetLabel();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode*   pCmdTarget = pSimBox->GetCommandTarget(targetLabel);

	if(pCmdTarget)
	{
		// Now we have the command target we loop over all beads contained in it 
		// and toggle their visibility status. Note that if the target is composite, 
		// the GetBeads() function recurses into all its contained targets.
		// Also note that if the target is empty we issue a warning message but not
		// an error.

		BeadVector vTargetBeads = pCmdTarget->GetBeads();

		if(!vTargetBeads.empty())
		{
			for(BeadVectorIterator iterBead=vTargetBeads.begin(); iterBead!=vTargetBeads.end(); iterBead++)
			{
				(*iterBead)->SetVisible(!(*iterBead)->GetVisible());
			}

			const bool bVisible = vTargetBeads.front()->GetVisible();

			new CLogctTargetDisplay(pSimBox->GetCurrentTime(), targetLabel, bVisible);
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
