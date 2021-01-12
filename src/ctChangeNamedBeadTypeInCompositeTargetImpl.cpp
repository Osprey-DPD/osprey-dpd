/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctChangeNamedBeadTypeInCompositeTargetImpl.cpp: implementation of the ctChangeNamedBeadTypeInCompositeTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctChangeNamedBeadTypeInCompositeTargetImpl.h"
#include "ctChangeNamedBeadTypeInCompositeTarget.h"
#include "SimBox.h"
#include "IModifySimStateBeadTypes.h"
#include "Bead.h"
#include "CurrentState.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "LogctChangeNamedBeadTypeInCompositeTarget.h"
#include "LogWarningTextMessage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctChangeNamedBeadTypeInCompositeTargetImpl::ctChangeNamedBeadTypeInCompositeTargetImpl()
{
}

ctChangeNamedBeadTypeInCompositeTargetImpl::~ctChangeNamedBeadTypeInCompositeTargetImpl()
{

}

// Command handler function to assign a new type and string identifier, 
// (or name) to the beads in a composite command target. This creates the 
// new bead type as one greater than the previous largest bead type, and 
// associates the user-supplied name with the new type. If the new name is 
// the same as an existing bead type's name, the command fails. 
//
// This command is similar to the ctChangeNamedBeadType command, but it operates
// on composite targets only. It allows a set of distinct targets to have their
// beads all changed to the same (new) type.  In order to be able 
// undo the change, each simple bead target stores the old type/new type pairs;
// other target types ignore the command but return a flag showing that
// the command was invoked on a composite target that contained targets other
// than just simple bead targets. If any of the contained targets are themselves 
// composite, the command propagates into them as well.
//
// Because the contained targets may contain beads of different types, there is no
// unique "oldType" whose bead-bead interaction paramters can be copied into 
// the new type. Hence, we use the values associated with the first simple bead
// target in the composite target.

void ctChangeNamedBeadTypeInCompositeTargetImpl::ChangeNamedBeadTypeInCompositeTarget(const xxCommand* const pCommand)
{
	const ctChangeNamedBeadTypeInCompositeTarget* const pCmd = dynamic_cast<const ctChangeNamedBeadTypeInCompositeTarget*>(pCommand);

	const zString label   = pCmd->GetTargetLabel();
	const zString newName = pCmd->GetNewBeadName();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(label);

	// Now we have the target, check that it is composite, not empty, contains at
    // least one simple bead target and that the new bead name is unique, 
    // then tell it to change the type of the beads in its contained targets.
    // We include the test for the simple bead target so that if the command
    // is sent to a polymer target (or a composite target containing only 
    // non-simple bead targets) it is safely rejected.

	if(pCmdTarget && pCmdTarget->IsComposite() && pCmdTarget->GetTargetTotal() > 0 && pCmdTarget->IsSimpleBeadTargetInComposite()  &&
       !pSimBox->IModifyBeadTypes()->IsBeadNameInMap(newName))
	{
	// Change the type of the beads and propagate the changes to the 
	// rest of the simulation. The new type is just one greater than
	// the highest type in use. We use the IModifySimStateBeadTypes
	// interface class to safely alter the CSimState and pass the changes 
	// to the rest of the simulation. The beads' display ids are set to their
        // new type using the current state's static member functions.
        // Because the target may contain beads of different types, we use the
        // type of the first simple bead target found to define the old type.
        // This determines the bead-bead interaction parameters that are used to
        // define the new type's interactions; but these may be changed to any
        // value by subsequent commands. If the target contains beads of different
        // types, the old type that appears in the log message will not apply to
        // all the types, but this has only cosmetic effects.

        const long oldType = pCmdTarget->GetFirstBeadType();
	const long newType = pSimBox->GetBeadTypeTotal();

        // If the composite target contains a target that is not a simple bead
        // target nor a composite target, we issue a warning log message, but 
        // continue to execute the change to all valid bead targets.

        if(!pCmdTarget->ChangeNamedBeadType(newType))
        {
            const zString warningMsg = "Warning: composite target ignoring attempt to change the type of a contained non-simple bead target's beads";
            new CLogWarningTextMessage(pSimBox->GetCurrentTime(), warningMsg);
        }

        // Note that we pass the oldType parameter to the IModifyBeadTypes()
		// interface object because the new type is just one greater than the
		// total number of bead types but we want to copy the interaction
		// parameters for the old type into the new type.

		pSimBox->IModifyBeadTypes()->AddNewNamedType(oldType, newType, newName);

		// and propagate the changes to the CMonitor

		pSimBox->UpdateBeadTypes();

        // Note that because there may be more than one type of bead in the targets
        // we cannot pass values for the old name and type, so we use placeholders.

		new CLogctChangeNamedBeadTypeInCompositeTarget(pSimBox->GetCurrentTime(), label, "various", newName, oldType, newType);
	}
	else
	{
		new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}

}
