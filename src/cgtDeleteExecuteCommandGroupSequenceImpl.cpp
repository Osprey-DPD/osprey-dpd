/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgtDeleteExecuteCommandGroupSequenceImpl.cpp: implementation of the cgtDeleteExecuteCommandGroupSequenceImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgtDeleteExecuteCommandGroupSequenceImpl.h"
#include "cgtDeleteExecuteCommandGroupSequence.h"
#include "SimBox.h"
#include "tguCommandGroup.h"
#include "LogcgtDeleteExecuteCommandGroupSequence.h"
#include "LogcgtInactiveCommandGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgtDeleteExecuteCommandGroupSequenceImpl::cgtDeleteExecuteCommandGroupSequenceImpl()
{
}

cgtDeleteExecuteCommandGroupSequenceImpl::~cgtDeleteExecuteCommandGroupSequenceImpl()
{
}

// Command handler to remove the sequence execution pattern from a named command group.
// The group is not destroyed, but only has its execution pattern deleted, and is
// is left in an inactive state. The active state of the group is irrelevant for
// this command.

void cgtDeleteExecuteCommandGroupSequenceImpl::DeleteExecuteCommandGroupSequence(const xxCommand* const pCommand)
{
#if EnableCommandGroups == SimCommandEnabled

	const cgtDeleteExecuteCommandGroupSequence* const pCmd = dynamic_cast<const cgtDeleteExecuteCommandGroupSequence*>(pCommand);

	const zString groupName	= pCmd->GetCommandGroupName();	// Name of command group

    
    CSimBox*   const pSimBox    = dynamic_cast<CSimBox*>(this);

// Retrieve the named group and delete its sequence execution pattern. The only
// failure mode for this command is if the named group does not exist. If the
// group did not have a sequence execution pattern, the delete command is ignored.

    tguCommandGroup* const pGroup = pSimBox->GetCommandGroup(groupName);

	if(pGroup)
	{
        if(pSimBox->RemoveActiveCommandGroup(groupName))
        {
		    new CLogcgtDeleteExecuteCommandGroupSequence(pSimBox->GetCurrentTime(), groupName);
        }
        else
        {
		    new CLogcgtInactiveCommandGroup(pSimBox->GetCurrentTime(), groupName);
        }
	}
	else
	{
		new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
  
#endif
}

