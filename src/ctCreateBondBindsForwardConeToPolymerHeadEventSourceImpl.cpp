/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl.cpp: implementation of the ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl.h"
#include "acCreateBondBindsForwardConeToPolymerHeadEventSource.h"
#include "taBondBindsToPolymerEventSource.h"
#include "aevBondBindsForwardConeToPolymerHead.h"
#include "SimBox.h"
#include "aeActiveSimBox.h"
#include "aeActiveCellNetwork.h"
#include "CommandTarget.h"
#include "CommandTargetACNFreePolymers.h"
#include "LogacACNCreateEventSource.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl::ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl()
{
}

ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl::~ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl()
{

}

// Command handler function to wrap an ACN target in an event source decorator (ESD)
// so that events controlled by the ACN can broadcast their state for analysis.
// Note that information flows in the following direction:
//
// Event --> ACN --> Event Source Decorator --> Event Analysis Decorator
//
// so the decorators do not need to pass information back to the ACN, and are
// prevented from doing so.

void ctCreateBondBindsForwardConeToPolymerHeadEventSourceImpl::CreateBondBindsForwardConeToPolymerHeadEventSource(const xxCommand* const pCommand)
{
	const acCreateBondBindsForwardConeToPolymerHeadEventSource* const pCmd = dynamic_cast<const acCreateBondBindsForwardConeToPolymerHeadEventSource*>(pCommand);

	const zString targetLabel = pCmd->GetACNTargetId();
	const zString decLabel    = pCmd->GetEventSourceId();


	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(targetLabel);

	// Now we have the target, create a taBondBindsToPolymer
    // decorator object and wrap the target with it, and also store a pointer to
    // the decorator in the ACN so that it can forward event broadcast information
    // to the event source.

	if(pCmdTarget)
	{
		CCommandTargetNode* pActiveTarget = pSimBox->GetActiveCommandTargetOutermost(targetLabel);

        // If the target is already active, remove it before adding it back
		// using the new decorator: we just check if the label of the
		// outermost decorator instance differs from the target's label.
		// Note that because we now know that pCmdTarget exists, we don't 
		// have to check that pActiveTarget is non-NULL as it must either be
		// a pointer to a decorator instance or equal to pCmdTarget.
        // We cast the actual target to its real type so that we can access
        // its associated ACN: note that it is essential that we cast the "target"
        // and not one of its wrapping decorators as they won't have the correct type,
        // and the cast will fail.

		if(pActiveTarget->GetLabel() != targetLabel)
		{
			pSimBox->m_ActiveCommandTargets.remove(pActiveTarget);
		}

        const CCommandTargetACNFreePolymers* const pPolymerTarget = dynamic_cast<CCommandTargetACNFreePolymers*>(pCmdTarget);

        const zString acnType   = pPolymerTarget->GetACNType();
        const zString eventType = aevBondBindsForwardConeToPolymerHead::GetType();
           
        // Wrap the active target in a new event source instance
		// and add it to the active command targets container.

		taBondBindsToPolymerEventSource* const pDec = new taBondBindsToPolymerEventSource(decLabel, pActiveTarget);

		pSimBox->m_ActiveCommandTargets.push_back(pDec);

        // Pass the newly-created ESD to the ACN so that it can broadcast event
        // data as required.

	    aeActiveSimBox* const pShadowSimBox = pSimBox->GetActiveSimBox();

	    if(pShadowSimBox && pShadowSimBox->IsACNPresent(acnType))
	    {
            aeActiveCellNetwork* const pACN = pShadowSimBox->GetACNFromType(acnType);

            pACN->AddEventSource(eventType, pDec);

		    // Log sucessful execution of the command

		    new CLogacACNCreateEventSource(pSimBox->GetCurrentTime(), acnType, eventType, decLabel);
        }
        else
	    {
		     new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	    }
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
