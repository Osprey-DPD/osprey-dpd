/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectACNFreeBondsAsTargetImpl.cpp: implementation of the ccSelectACNFreeBondsAsTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectACNFreeBondsAsTargetImpl.h"
#include "ccSelectACNFreeBondsAsTarget.h"
#include "SimBox.h"
#include "aeActiveSimBox.h"
#include "aefActinNetwork.h"
#include "aeActivePolymer.h"
#include "aeActiveBond.h"
#include "LogSelectACNFreeBondsAsTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectACNFreeBondsAsTargetImpl::ccSelectACNFreeBondsAsTargetImpl()
{
}

ccSelectACNFreeBondsAsTargetImpl::~ccSelectACNFreeBondsAsTargetImpl()
{

}

// Command handler to wrap all free bonds belonging to the specified ACN
// in a command target. We use a special ACN bond command target that
// is given a pointer to the ACN so that it can retrieve the latest
// set of active bonds as needed.

void ccSelectACNFreeBondsAsTargetImpl::SelectACNFreeBondsAsTarget(const xxCommand* const pCommand)
{
#if EnableACNCommand == SimCommandEnabled

	const ccSelectACNFreeBondsAsTarget* const pCmd = dynamic_cast<const ccSelectACNFreeBondsAsTarget*>(pCommand);

	const zString	targetLabel	= pCmd->GetTargetLabel();
	const zString	acnType	= pCmd->GetACNType();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Check that the shadow SimBox and the specified active network exist, if 
	// they do not we issue a command failed message. We do not check that any 
	// active polymers exist as they may be created after this command executes. 
	// This is not an error condition.

	aeActiveSimBox* const pShadowSimBox = pSimBox->GetActiveSimBox();

	if(pShadowSimBox && pShadowSimBox->IsACNPresent(acnType))
	{
		// Create and store a CCommandTargetACNFreeBonds target to hold the 
		// polymers and receive commands.

		if(pSimBox->CreateCommandTargetACNFreeBonds(targetLabel, acnType))
		{
			 new CLogSelectACNFreeBondsAsTarget(pSimBox->GetCurrentTime(), targetLabel, acnType);
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
#endif

}
