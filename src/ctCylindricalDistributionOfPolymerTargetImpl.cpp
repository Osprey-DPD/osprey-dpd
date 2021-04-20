/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctCylindricalDistributionOfPolymerTargetImpl.cpp: implementation of the ctCylindricalDistributionOfPolymerTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctCylindricalDistributionOfPolymerTargetImpl.h"
#include "ctCylindricalDistributionOfPolymerTarget.h"
#include "SimBox.h"
#include "CommandTargetPolymer.h"
#include "CommandTargetComposite.h"
#include "taCylindricalDistribution.h"
#include "LogctRadialDistribution.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctCylindricalDistributionOfPolymerTargetImpl::ctCylindricalDistributionOfPolymerTargetImpl()
{
}

ctCylindricalDistributionOfPolymerTargetImpl::~ctCylindricalDistributionOfPolymerTargetImpl()
{

}

// Command handler function to calculate the radial distribution about a fixed axis of all polymers contained in
// the specified polymer target. It creates a decorator that repeatedly calculates the CM of each polymer and
// creates a histogram of the results.

void ctCylindricalDistributionOfPolymerTargetImpl::CylindricalDistributionOfPolymerTarget(const xxCommand* const pCommand)
{
	const ctCylindricalDistributionOfPolymerTarget* const pCmd = dynamic_cast<const ctCylindricalDistributionOfPolymerTarget*>(pCommand);

	const zString targetLabel = pCmd->GetTargetLabel();
	const zString decLabel    = pCmd->GetDecLabel();

    const long start   = pCmd->GetStartTime();
    const long end     = pCmd->GetEndTime();
    const long samples = pCmd->GetSamples();

	const long xn = pCmd->GetXNormal();
	const long yn = pCmd->GetYNormal();
	const long zn = pCmd->GetZNormal();

	const double xc = pCmd->GetXCentre();
	const double yc = pCmd->GetYCentre();  // Fractions of SimBox size
	const double zc = pCmd->GetZCentre();
 
    const double shellWidth = pCmd->GetShellWidth();
    const double outerRadius = pCmd->GetOuterRadius();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Convert relative coordinates to absolute ones

	double fxc = xc*pSimBox->GetSimBoxXLength();
	double fyc = yc*pSimBox->GetSimBoxYLength();  // Absolute units
	double fzc = zc*pSimBox->GetSimBoxZLength();
 
	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(targetLabel);
    CCommandTargetPolymer* pPolymerTarget = dynamic_cast<CCommandTargetPolymer*>(pCmdTarget);

	// Now we have the target, create a taCylindricalDistribution decorator object
	// and wrap the target with it to enable the polymers to be binned into a histogram according to their CM.

    if(pPolymerTarget && pPolymerTarget->GetPolymerTotal() > 0 && start >= pSimBox->GetCurrentTime())
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

        taCylindricalDistribution* const pDec = new taCylindricalDistribution(decLabel, pActiveTarget, start, end, samples,
                                               xn, yn, zn, fxc, fyc, fzc, shellWidth, outerRadius);
		pSimBox->m_ActiveCommandTargets.push_back(pDec);

		// Log sucessful execution of the command

		new CLogctRadialDistribution(pSimBox->GetCurrentTime(), targetLabel, decLabel,
												"cylindrical", start, end, samples, xn, yn, zn, xc, yc, zc, shellWidth, outerRadius);
	}
	else
	{
		new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
