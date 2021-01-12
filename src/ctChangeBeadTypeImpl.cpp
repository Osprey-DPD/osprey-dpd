/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctChangeBeadTypeImpl.cpp: implementation of the ctChangeBeadTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctChangeBeadTypeImpl.h"
#include "ctChangeBeadType.h"
#include "SimBox.h"
#include "IModifySimStateBeadTypes.h"
#include "Bead.h"
#include "CNTCell.h"   // Needed for the RNG
#include "CurrentState.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "LogctChangeBeadType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctChangeBeadTypeImpl::ctChangeBeadTypeImpl()
{
}

ctChangeBeadTypeImpl::~ctChangeBeadTypeImpl()
{

}

// Command handler function to assign a new type and string identifier, 
// (or name) to the beads in a simple bead command target. This creates 
// the new bead type as one greater than the previous largest bead type, 
// and automatically assigns a new name to the new type. This distinguishes it 
// from the similar command ctChangedNamedBeadType which requires a 
// user-defined name. The new name has the following format:
//
// e.g., oldName = H
//       newname = H-12345678-1
//
// where "12345678" is a randomly-generated string of integers, and the final 
// index is a counter that indicates how many types have been created using
// the given random string. This allows for future modifications so that the 
// command can be applied to a composite target and all the bead types in the
// target given different but related names, so that they can be easily
// identified.

void ctChangeBeadTypeImpl::ChangeBeadType(const xxCommand* const pCommand)
{
	const ctChangeBeadType* const pCmd = dynamic_cast<const ctChangeBeadType*>(pCommand);

	const zString label = pCmd->GetTargetLabel();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(label);
        const long oldType = pCmdTarget->GetCurrentBeadType();
	const long newType = pSimBox->GetBeadTypeTotal();

        const zString oldName = pSimBox->GetBeadNameFromType(oldType);

	// Now we have the target, check it is not composite nor empty, that we can
    // add a new bead type pair to it (which means it cannot be a polymer target)
    // and that the current bead type is valid and the new bead name is unique, 
    // then get the beads contained in it

	BeadVector vBeads = pCmdTarget->GetBeads();

	if(pCmdTarget && !pCmdTarget->IsComposite() && pCmdTarget->AddBeadTypePair(newType, oldType) && 
        oldType >= 0 && vBeads.size() > 0)
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

        // Create a new name for the bead type based on its old name and 
        /// a randomly-generated string. We use the dash as the separator
        // to distinguish the passed-in prefix.

        const zString newName = CCNTCell::GetRandomString(oldName, "-");

		// Note that we pass the oldType parameter to the IModifyBeadTypes()
		// interface object because the new type is just one greater than the
		// total number of bead types but we want to copy the interaction
		// parameters for the old type into the new type.

		pSimBox->IModifyBeadTypes()->AddNewNamedType(oldType, newType, newName);

		// and propagate the changes to the CMonitor

		pSimBox->UpdateBeadTypes();

		new CLogctChangeBeadType(pSimBox->GetCurrentTime(), label, oldName, newName, oldType, newType);
    }
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
