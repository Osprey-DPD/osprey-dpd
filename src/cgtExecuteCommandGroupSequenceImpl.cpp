/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgtExecuteCommandGroupSequenceImpl.cpp: implementation of the cgtExecuteCommandGroupSequenceImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgtExecuteCommandGroupSequenceImpl.h"
#include "cgtExecuteCommandGroupSequence.h"
#include "ISimState.h"
#include "SimBox.h"
#include "tguCommandGroup.h"
#include "LogcgtExecuteCommandGroupSequence.h"
#include "LogcgtInactiveCommandGroup.h"
#include "LogcgtExecuteCommandGroupFailed.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgtExecuteCommandGroupSequenceImpl::cgtExecuteCommandGroupSequenceImpl()
{
}

cgtExecuteCommandGroupSequenceImpl::~cgtExecuteCommandGroupSequenceImpl()
{
}

// Command handler to execute a command group repeatedly with a specified period. 
// The group is not deleted after execution but left in an inactive state.
// We pass the data to the command group to create its sequential execution pattern,
// and then add the group to the CSimBox's active command groups. This causes it
// to be executed the specified number of times.

void cgtExecuteCommandGroupSequenceImpl::ExecuteCommandGroupSequence(const xxCommand* const pCommand)
{
#if EnableCommandGroups == SimCommandEnabled

	const cgtExecuteCommandGroupSequence* const pCmd = dynamic_cast<const cgtExecuteCommandGroupSequence*>(pCommand);

	const zString groupName	= pCmd->GetCommandGroupName();	// Name of command group
    const long    total     = pCmd->GetTotal();             // Number of times to execute gorup
    const long    period    = pCmd->GetPeriod();            // Number of steps between executions
    
    CSimBox*   const pSimBox    = dynamic_cast<CSimBox*>(this);
    ISimState* const pISimState = pSimBox;

    // Retrieve the named group and execute it repeatedly; we pass in the ISimState
    // interface pointer so the group can add its commands to the list of commands
    // to be executed. If the group does not exist we issue a standard "command failed"
    // message; if the group is inactive we issue an "inactive command group" message; 
    // and if the execution of the group's commands fails, we issue a more
    // specific message.

    tguCommandGroup* const pGroup = pSimBox->GetCommandGroup(groupName);

	if(pGroup)
	{
        if(pGroup->ExecuteSequence(pISimState, total, period))
        {
            if(pSimBox->AddActiveCommandGroup(groupName))
            {
		        new CLogcgtExecuteCommandGroupSequence(pSimBox->GetCurrentTime(), groupName, total, period);
            }
            else
            {
		        new CLogcgtInactiveCommandGroup(pSimBox->GetCurrentTime(), groupName);
            }
        }
        else
        {
		    new CLogcgtExecuteCommandGroupFailed(pSimBox->GetCurrentTime(), groupName);
        }
	}
	else
	{
		new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}

#endif
}

