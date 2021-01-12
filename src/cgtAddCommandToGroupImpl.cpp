/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgtAddCommandToGroupImpl.cpp: implementation of the cgtAddCommandToGroupImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgtAddCommandToGroupImpl.h"
#include "cgtAddCommandToGroup.h"
#include "tguCommandGroup.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "LogcgtAddCommandToGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgtAddCommandToGroupImpl::cgtAddCommandToGroupImpl()
{
}

cgtAddCommandToGroupImpl::~cgtAddCommandToGroupImpl()
{
}

// Command handler to add a command to an existing command group.

void cgtAddCommandToGroupImpl::AddCommandToGroup(const xxCommand* const pCommand)
{
#if EnableCommandGroups == SimCommandEnabled

	const cgtAddCommandToGroup* const pCmd = dynamic_cast<const cgtAddCommandToGroup*>(pCommand);

	const zString  groupName = pCmd->GetCommandGroupName();	    // Name of command group
	const zString  cmdName	 = pCmd->GetCommandName();	        // Name of command to add
    StringSequence argNames  = pCmd->GetArguments();            // Command's arguments

    CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

    // Retrieve the named command group and pass it the name of the command
    // and the placeholder names of its arguments. There must be exactly the same
    // number of placeholder names as arguments. The group stores the command
    // information until such time as it needs to instantiate the command
    // with appropriate argument balues.

    tguCommandGroup* const pGroup = pSimBox->GetCommandGroup(groupName);

	if(pGroup)
	{
        pGroup->AddCommand(cmdName, argNames);

		new CLogcgtAddCommandToGroup(pSimBox->GetCurrentTime(), groupName, cmdName, argNames);
	}
	else
	{
		new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}

#endif
}

