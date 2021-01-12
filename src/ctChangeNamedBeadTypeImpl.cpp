/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctChangeNamedBeadTypeImpl.cpp: implementation of the ctChangeNamedBeadTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctChangeNamedBeadTypeImpl.h"
#include "ctChangeNamedBeadType.h"
#include "SimBox.h"
#include "IModifySimStateBeadTypes.h"
#include "Bead.h"
#include "CurrentState.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "LogctChangeNamedBeadType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctChangeNamedBeadTypeImpl::ctChangeNamedBeadTypeImpl()
{
}

ctChangeNamedBeadTypeImpl::~ctChangeNamedBeadTypeImpl()
{

}

// Command handler function to assign a new type and string identifier, 
// (or name) to the beads in a command target. This creates the new bead 
// type as one greater than the previous largest bead type, and associates 
// the user-supplied name with the new type. If the new name is the same 
// as an existing bead type's name, the command fails.

void ctChangeNamedBeadTypeImpl::ChangeNamedBeadType(const xxCommand* const pCommand)
{
	const ctChangeNamedBeadType* const pCmd = dynamic_cast<const ctChangeNamedBeadType*>(pCommand);

	const zString label   = pCmd->GetTargetLabel();
	const zString newName = pCmd->GetNewBeadName();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list, and exit if it does not
    // exist or is composite (see the related command ChangeNamedBeadTypeInCompositeTarget
    // for the composite case).

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(label);

    if(pCmdTarget && !pCmdTarget->IsComposite())
    {
	    // Now we have the target, check it is not composite nor empty, that we can
        // add a new bead type pair to it (which means it cannot be a polymer target)
        // and that the current bead type is valid and the new bead name is unique, 
        // then get the beads contained in it

        const long oldType = pCmdTarget->GetCurrentBeadType();
	    const long newType = pSimBox->GetBeadTypeTotal();

        const zString oldName = pSimBox->GetBeadNameFromType(oldType);

        BeadVector vBeads = pCmdTarget->GetBeads(); 

	    if(oldType >= 0 && vBeads.size() > 0 && !pSimBox->IModifyBeadTypes()->IsBeadNameInMap(newName) &&
           pCmdTarget->AddBeadTypePair(newType, oldType))
	    {
		    // Change the type of the beads and propagate the changes to the 
		    // rest of the simulation. The new type is just one greater than
		    // the highest type in use. We use the IModifySimStateBeadTypes
		    // interface class to safely alter the CSimState and pass the changes 
		    // to the rest of the simulation. The beads' display ids are set to their
            // new type using the current state's static member functions.

		    for(BeadVectorIterator iterBead = vBeads.begin(); iterBead != vBeads.end(); iterBead++)
		    {
			    (*iterBead)->SetType(newType);
                (*iterBead)->SetVisible();
                CCurrentState::SetBeadDisplayId((*iterBead)->GetId(), newType);
		    }

		    // Note that we pass the oldType parameter to the IModifyBeadTypes()
		    // interface object because the new type is just one greater than the
		    // total number of bead types but we want to copy the interaction
		    // parameters for the old type into the new type.

		    pSimBox->IModifyBeadTypes()->AddNewNamedType(oldType, newType, newName);

		    // and propagate the changes to the CMonitor

		    pSimBox->UpdateBeadTypes();

		    new CLogctChangeNamedBeadType(pSimBox->GetCurrentTime(), label, oldName, newName, oldType, newType);
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
