/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctSpringForceBetweenTargetsImpl.cpp: implementation of the ctSpringForceOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctSpringForceBetweenTargetsImpl.h"
#include "ctSpringForceBetweenTargets.h"
#include "SimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "taSpringForceBetweenTargets.h"
#include "LogctSpringForceBetweenTargets.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctSpringForceBetweenTargetsImpl::ctSpringForceBetweenTargetsImpl()
{
}

ctSpringForceBetweenTargetsImpl::~ctSpringForceBetweenTargetsImpl()
{

}

// Command handler function apply a Hookean spring force to all beads in the specifed
// targets either pulling them together or pushing them apart depending on the sign of the force. 
// The magnitude of the force depends on the separation of the two targets' centre of mass positions. 
// The force is derived from a harmonic potential, and the same magnitude force is applied to all beads 
// in each target.

void ctSpringForceBetweenTargetsImpl::SpringForceBetweenTargets(const xxCommand* const pCommand)
{
	const ctSpringForceBetweenTargets* const pCmd = dynamic_cast<const ctSpringForceBetweenTargets*>(pCommand);

	const zString target1Label = pCmd->GetTargetLabel();  
	const zString target2Label = pCmd->GetTarget2Label();
	const zString decLabel     = pCmd->GetDecLabel();


	const double keff   = pCmd->GetMagnitude();
    const double deltax = pCmd->GetDeltaX();
    const double period = pCmd->GetPeriod();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);


	// Get the primary and secondary command targets from the target list

	CCommandTargetNode* pCmdTarget1 = pSimBox->GetCommandTarget(target1Label);
	CCommandTargetNode* pCmdTarget2 = pSimBox->GetCommandTarget(target2Label);
    
	// Now we have the target, create a taSpringForceBetweenTargets decorator object 
	// and wrap the first target with it to enable the force to be applied to the
	// target's beads. Note that because we use a Composite pattern for the
	// command targets, we don't need to know whether it is composite or
	// simple as the GetBeads() function just returns all beads in the target.
	// However, the data on bead type, bead name and original bead type
	// is not applicable to composite targets because they may contain
	// a multitude of bead types.

	if(pCmdTarget1 && pCmdTarget2)
	{
		const long originalBeadType    = pCmdTarget1->GetOriginalBeadType();
		const zString beadName         = pSimBox->GetBeadNameFromType(originalBeadType);

		CCommandTargetNode* pActiveTarget = pSimBox->GetActiveCommandTargetOutermost(target1Label);

		// If the target is already active, remove it before adding it back
		// using the new decorator: we just check if the label of the
		// outermost decorator instance differs from the target's label.
		// Note that because we now know that pCmdTarget exists, we don't 
		// have to check that pActiveTarget is non-NULL as it must either be
		// a pointer to a decorator instance or equal to pCmdTarget.

		if(pActiveTarget->GetLabel() != target1Label)
		{
			pSimBox->m_ActiveCommandTargets.remove(pActiveTarget);
		}

		// Wrap the active target in a new force decorator instance
		// and add it to the active command targets container.

		taSpringForceBetweenTargets* const pDec = new taSpringForceBetweenTargets(decLabel, pActiveTarget, pCmdTarget2, keff, deltax, period);

		pSimBox->m_ActiveCommandTargets.push_back(pDec);

		// Log sucessful execution of the command

		new CLogctSpringForceBetweenTargets(pSimBox->GetCurrentTime(), 
                                                target1Label, target2Label, decLabel, keff, deltax, period);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
