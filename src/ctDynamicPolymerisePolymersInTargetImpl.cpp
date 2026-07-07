/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctDynamicPolymerisePolymersInTargetImpl.cpp: implementation of the ctDynamicPolymerisePolymersInTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctDynamicPolymerisePolymersInTargetImpl.h"
#include "ctDynamicPolymerisePolymersInTarget.h"
#include "SimBox.h"
#include "IModifySimStateBondTypes.h"
#include "Bond.h"
#include "CNTCell.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "taPolymerisePolymers.h"
#include "LogctDynamicPolymerisePolymersInTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctDynamicPolymerisePolymersInTargetImpl::ctDynamicPolymerisePolymersInTargetImpl()
{
}

ctDynamicPolymerisePolymersInTargetImpl::~ctDynamicPolymerisePolymersInTargetImpl()
{

}

// Command handler function to dynamically polymerise a given bead in a given polymer type
// when they approach within a specified distance.

void ctDynamicPolymerisePolymersInTargetImpl::DynamicPolymerisePolymersInTarget(const xxCommand* const pCommand)
{
	const ctDynamicPolymerisePolymersInTarget* const pCmd = dynamic_cast<const ctDynamicPolymerisePolymersInTarget*>(pCommand);

	const zString targetLabel = pCmd->GetTargetLabel();
	const zString decLabel    = pCmd->GetDecLabel();

	const long polyType           = pCmd->GetPolymerType();
	const zLongVector vBeadTypes  = pCmd->GetBeadTypes();
	const long maxBonds           = pCmd->GetMaxBonds();
	const double maxDistance      = pCmd->GetMaxDistance();
	const double springConst      = pCmd->GetSpringConstant();
	const double unstrLength      = pCmd->GetUnStretchedLength();
	const double bondPrefactor    = pCmd->GetBondPrefactor();
	const double bondBarrier      = pCmd->GetBondBarrier();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(targetLabel);

	// Now we have the target, create a  decorator object
	// and wrap the target with it to enable the polymers to be dynamically polymerised
    // using a new bond type we create here and pass to the decorator. This is done
    // because we need access to the ISimState instance, which is only accessible to
    // CSimBox and this implementation instance.

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
        
        // Create a template bond for use in creating polymerised bonds, if any are made.
        // We get the current number of bond types from the CInitialState
        // object and create a new type to be used as a template for all
        // polymerised bonds. The template is stored in the CInitialState
        // just as the normal bond types using the interface class
        // IModifySimStateBondTypes. Note that I had to make this class a friend
        // of CSimState to allow this.

        const long polyBondType = pSimBox->GetBondTypeTotal();    // used to create new bond type

    #if SimIdentifier == MD
        m_sprConst *= (pSimBox->GetLengthScale()*pSimBox->GetLengthScale()/pSimBox->GetEnergyScale());
        m_unStrLength /= pSimBox->GetLengthScale();
    #endif
        
        CBond* pPolyBondTemplate = new CBond(polyBondType, springConst, unstrLength);

        // Create a new unique name for the bond type and store the template bond
        // for later use. The bond name is constructed using the target's (unique)
        // label, a random string, and a counter. The latter index can be used
        // in the future if many bond types are constructed for the same target.
        // We use a dash as the separator to distinguish the target name
        // and the random string. A sample dynamic bond name is,
        //
        // TargetLabel-12345678-1

        const zString polyBondName = CCNTCell::GetRandomString(decLabel, "-");

        pSimBox->IModifyBondTypes()->AddNewNamedType(pPolyBondTemplate, polyBondType, polyBondName);

		// Wrap the active target in a new  decorator instance
		// and add it to the active command targets container.
                
		taPolymerisePolymers* const pDec = new taPolymerisePolymers(decLabel, pActiveTarget, pSimBox, pPolyBondTemplate, polyType, vBeadTypes, maxBonds, maxDistance, springConst, unstrLength, bondPrefactor, bondBarrier);

		pSimBox->m_ActiveCommandTargets.push_back(pDec);
        
		// Log sucessful execution of the command

		new CLogctDynamicPolymerisePolymersInTarget(pSimBox->GetCurrentTime(), targetLabel, decLabel, polyBondName, polyType, vBeadTypes, maxBonds,
		                                            maxDistance, springConst, unstrLength, bondPrefactor, bondBarrier);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
