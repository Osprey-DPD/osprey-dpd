/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccRemoveForminBondImpl.cpp: implementation of the ccRemoveForminBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccRemoveForminBondImpl.h"
#include "ccRemoveForminBond.h"
#include "aeForminBond.h"
#include "SimBox.h"
#include "IModifySimStatePolymers.h"
#include "LogActiveBondRemoved.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccRemoveForminBondImpl::ccRemoveForminBondImpl()
{
}

ccRemoveForminBondImpl::~ccRemoveForminBondImpl()
{

}

// Command handler to remove active bonds of the type aeFormin. These may wrap 
// several types of polymer if the user has issued several ccAddForminBond commands
// so the name of the polymers whose active bonds are to be removed is specified 
// as part of the command.  This allows several different polymer types to be 
// wrapped by the same kind of active bond but still removed independently.

void ccRemoveForminBondImpl::RemoveForminBond(const xxCommand* const pCommand)
{
#if EnableACNCommand == SimCommandEnabled

	const ccRemoveForminBond* const pCmd = dynamic_cast<const ccRemoveForminBond*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	const zString headMonomerName	= pCmd->GetHeadMonomerName();
	const zString tailMonomerName	= pCmd->GetTailMonomerName();

	const long headMonomerType	= pSimBox->GetPolymerTypeFromName(headMonomerName);
	const long tailMonomerType	= pSimBox->GetPolymerTypeFromName(tailMonomerName);

	// not implemented yet... Note that when it is implemented we cannot 
	// assume that there are no active bonds present after we have removed
	// the Formin ones, as there may be other ACNs defined. Only if all of themn
	// are destroyed should we rest the CSimBox flag.

	if(true)
	{
		 new CLogActiveBondRemoved(pSimBox->GetCurrentTime(), "Formin", headMonomerName, tailMonomerName,
																						headMonomerType, tailMonomerType);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
#endif
}

