/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctTranslateTargetToPlaneImpl.cpp: implementation of the ctTranslateTargetToPlaneImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctTranslateTargetToPlaneImpl.h"
#include "ctTranslateTargetToPlane.h"
#include "SimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "taPlanarAnchorForce.h"
#include "LogctTranslateTargetToPlane.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctTranslateTargetToPlaneImpl::ctTranslateTargetToPlaneImpl()
{
}

ctTranslateTargetToPlaneImpl::~ctTranslateTargetToPlaneImpl()
{

}

// Command handler function to translate all beads in a command target so that they lie within a specified plane 
// whose normal is in the Z direction.

void ctTranslateTargetToPlaneImpl::TranslateTargetToPlane(const xxCommand* const pCommand)
{
	const ctTranslateTargetToPlane* const pCmd = dynamic_cast<const ctTranslateTargetToPlane*>(pCommand);

	const zString targetLabel = pCmd->GetTargetLabel();
	const zString decLabel    = pCmd->GetDecLabel();

	const long xn = pCmd->GetXNormal();
	const long yn = pCmd->GetYNormal();
	const long zn = pCmd->GetZNormal();

	const double xc = pCmd->GetXCentre();
	const double yc = pCmd->GetYCentre();
	const double zc = pCmd->GetZCentre();
	
	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Convert relative coordinates to absolute ones

//	double fxc = xc*pSimBox->GetSimBoxXLength();
//	double fyc = yc*pSimBox->GetSimBoxYLength();
//	double fzc = zc*pSimBox->GetSimBoxZLength();

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(targetLabel);

	// Now we have the target, create a taPlanarAnchorForce decorator object 
	// and wrap the target with it to enable the force to be applied to the
	// target's beads. Note that because we use a Composite pattern for the
	// command targets, we don't need to know whether it is composite or
	// simple as the GetBeads() function just returns all beads in the target.
	// However, the data on bead type, bead name and original bead type
	// is not applicable to composite targets because they may contain
	// a multitude of bead types.

	if(pCmdTarget)
	{
		const long originalBeadType    = pCmdTarget->GetOriginalBeadType();
		const zString beadName         = pSimBox->GetBeadNameFromType(originalBeadType);

	        new CLogctTranslateTargetToPlane(pSimBox->GetCurrentTime(), targetLabel, decLabel, xn, yn, zn, xc, yc, zc);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
