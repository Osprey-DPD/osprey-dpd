/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctChangeBondTypeImpl.cpp: implementation of the ctChangeBondTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctChangeBondTypeImpl.h"
#include "ctChangeBondType.h"
#include "SimBox.h"
#include "IModifySimStateBondTypes.h"
#include "Bond.h"
#include "CNTCell.h"   // Needed for the RNG
#include "CurrentState.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "LogctChangeBondType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctChangeBondTypeImpl::ctChangeBondTypeImpl()
{
}

ctChangeBondTypeImpl::~ctChangeBondTypeImpl()
{

}

// Function to implement a ctChangeBondType command to a command target.
// A set of polymers, which have a specified type, that lie within a 
// certain geometric region of the SimBox are stored in a CCommandTarget. 
// Inclusion of the polymers in the target is determined by their head bead's
// coordinates, tail bead's or both. Commands can then be directed at the 
// polymers.
//
// This command changes the type of all instances of a specified bond type
// that are in the target polymers to a new value determined as one greater 
// than the highest bond type currently assigned. This ensures that the bond 
// types are still consecutive integers.

void ctChangeBondTypeImpl::ChangeBondType(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctChangeBondType* const pCmd = dynamic_cast<const ctChangeBondType*>(pCommand);

	const zString label = pCmd->GetTargetLabel();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(label);

	// Now we have the target, check it is not composite and get the 
	// bonds contained in it

	if(pCmdTarget && !pCmdTarget->IsComposite())
	{
		// Fake the bond name and types for now

		const zString oldName = "test";
		const long oldType    = -1;
		const long newType    = -1;

		new CLogctChangeBondType(pSimBox->GetCurrentTime(), label, oldName, oldType, newType);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}

#endif
}
