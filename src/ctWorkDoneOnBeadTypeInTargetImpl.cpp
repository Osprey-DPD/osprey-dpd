/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctWorkDoneOnBeadTypeInTargetImpl.cpp: implementation of the ctWorkDoneOnBeadTypeInTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctWorkDoneOnBeadTypeInTargetImpl.h"
#include "ctWorkDoneOnBeadTypeInTarget.h"
#include "SimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "Bead.h"
#include "LogctWorkDoneOnBeadTypeInTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctWorkDoneOnBeadTypeInTargetImpl::ctWorkDoneOnBeadTypeInTargetImpl()
{
}

ctWorkDoneOnBeadTypeInTargetImpl::~ctWorkDoneOnBeadTypeInTargetImpl()
{

}

// Command handler to calculate the work done on a set of beads in a command target.
// Only beads of the specified type are included in the sum of F.dx, and the
// calculation uses the stored values of the beads' position coordinate increments
// since the last time step and the current force.

void ctWorkDoneOnBeadTypeInTargetImpl::WorkDoneOnBeadTypeInTarget(const xxCommand* const pCommand)
{
	const ctWorkDoneOnBeadTypeInTarget* const pCmd = dynamic_cast<const ctWorkDoneOnBeadTypeInTarget*>(pCommand);

	const zString   label = pCmd->GetTargetLabel();
	const long soughtType = pCmd->GetBeadType();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(label);

	// Now we have the target, check it is not composite nor empty before applying
	// the command to it

	if(pCmdTarget && !pCmdTarget->IsComposite() && pCmdTarget->GetBeadTotalForType(soughtType) > 0)
	{
		BeadVector vBeads = pCmdTarget->GetBeads();

		const zString beadName = pSimBox->GetBeadNameFromType(soughtType);
		const long beadTotal   = vBeads.size();

		// Calculate the work done as the sum over all beads in the target of F.dx

		double workDone  = 0.0;

		for(BeadVectorIterator iterBead = vBeads.begin(); iterBead != vBeads.end(); iterBead++)
		{
			const double fdx = (*iterBead)->GetXForce()*(*iterBead)->GetdXPos();
			const double fdy = (*iterBead)->GetYForce()*(*iterBead)->GetdYPos();
			const double fdz = (*iterBead)->GetZForce()*(*iterBead)->GetdZPos();

			workDone += (fdx + fdy + fdz);

			std::cout << (*iterBead)->GetdXPos()  << " " << (*iterBead)->GetdYPos()  << " " << (*iterBead)->GetdZPos()  << zEndl;
			std::cout << (*iterBead)->GetOldXForce()  << " " << (*iterBead)->GetOldYForce()  << " " << (*iterBead)->GetOldZForce()  << zEndl;
			std::cout << (*iterBead)->GetXForce()  << " " << (*iterBead)->GetYForce()  << " " << (*iterBead)->GetZForce()  << zEndl;
			std::cout << fdx << " " << fdy << " " << fdz << " work done = " << workDone << zEndl;
	}

		new CLogctWorkDoneOnBeadTypeInTarget(pSimBox->GetCurrentTime(), label, beadName, soughtType, beadTotal, workDone);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);

	}
}
