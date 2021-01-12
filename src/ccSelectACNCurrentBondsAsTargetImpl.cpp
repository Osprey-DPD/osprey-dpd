/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectACNCurrentBondsAsTargetImpl.cpp: implementation of the ccSelectACNCurrentBondsAsTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectACNCurrentBondsAsTargetImpl.h"
#include "ccSelectACNCurrentBondsAsTarget.h"
#include "SimBox.h"
#include "aeActiveSimBox.h"
#include "aefActinNetwork.h"
#include "aeActivePolymer.h"
#include "aeActiveBond.h"
#include "LogSelectACNCurrentBondsAsTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectACNCurrentBondsAsTargetImpl::ccSelectACNCurrentBondsAsTargetImpl()
{
}

ccSelectACNCurrentBondsAsTargetImpl::~ccSelectACNCurrentBondsAsTargetImpl()
{

}

// Command handler to wrap all free bonds belonging to the specified ACN
// in a command target. We use a polymer command target so that each active
// bond, which wraps a CPolymer, is stored independently of all the others.
// Note that this command creates a target that is not updated: it only holds
// those free bonds that exist when it is created. To create a target that
// continually updates its contained bonds, use the 
// ccSelectACNFreeBondsAsTarget command.

void ccSelectACNCurrentBondsAsTargetImpl::SelectACNCurrentBondsAsTarget(const xxCommand* const pCommand)
{
#if EnableACNCommand == SimCommandEnabled

	const ccSelectACNCurrentBondsAsTarget* const pCmd = dynamic_cast<const ccSelectACNCurrentBondsAsTarget*>(pCommand);

	const zString	targetLabel	= pCmd->GetTargetLabel();
	const zString	acnType	= pCmd->GetACNType();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Check that the shadow SimBox and the specified active network exist, if 
	// they do not we issue a command failed message. As this type of target
	// does not update its contents, we check that some bonds of the specified
	// type exist otherwise we issue a command failed message.

	aeActiveSimBox* const pShadowSimBox = pSimBox->GetActiveSimBox();

	aeActiveCellNetwork* const pACN = pShadowSimBox->GetACNFromType(acnType);

	if(pShadowSimBox && pACN && pACN->GetFreeBondTotal() > 0)
	{
		// Now we have the ACN, collect its free bonds, extract all the monomers
		// in them and store them in a CCommandTargetPolymer. Because active bonds
		// consist simply of two CPolymers (one as their head and one as their tail)
		// they can be stored in a CCommandTargetPolymer target. Note that free active
        // bonds only contain a single CPolymer as their tail.

		ActiveBondList lFreeActiveBonds;
		lFreeActiveBonds.clear();
		lFreeActiveBonds = pShadowSimBox->GetfActinNetwork()->GetFreeBonds();

		const long freeBondTotal = lFreeActiveBonds.size();

		PolymerVector vTargetPolymers;
		vTargetPolymers.clear();

		for(ActiveBondListIterator iterBond = lFreeActiveBonds.begin(); iterBond != lFreeActiveBonds.end(); iterBond++)
		{
			vTargetPolymers.push_back((*iterBond)->GetTailMonomer());
		}

		const long polymerTotal = vTargetPolymers.size();
		const long polymerType  = vTargetPolymers.front()->GetType();
		const zString polymerName = pSimBox->GetPolymerNameFromType(polymerType);

		// Create and store a CCommandTargetPolymer to hold the polymers and 
		// receive commands.

		if(pSimBox->CreateCommandTargetPolymer(targetLabel, polymerType, vTargetPolymers))
		{
			 new CLogSelectACNCurrentBondsAsTarget(pSimBox->GetCurrentTime(), targetLabel, acnType, polymerName, polymerType, freeBondTotal, polymerTotal);
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
