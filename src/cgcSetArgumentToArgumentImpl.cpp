/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgcSetArgumentToArgumentImpl.cpp: implementation of the cgcSetArgumentToArgumentImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgcSetArgumentToArgumentImpl.h"
#include "cgcSetArgumentToArgument.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "tguCommandGroup.h"
#include "LogcgcSetArgumentToArgument.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgcSetArgumentToArgumentImpl::cgcSetArgumentToArgumentImpl()
{
}

cgcSetArgumentToArgumentImpl::~cgcSetArgumentToArgumentImpl()
{
}

// Command handler to link a destination command's argument value to that of
// another argument in a possibly-different command. This allows two or more 
// commands to have arguments whose values are guaranteed to be the same.

void cgcSetArgumentToArgumentImpl::SetArgumentToArgument(const xxCommand* const pCommand)
{
#if EnableCommandGroups == SimCommandEnabled

	const cgcSetArgumentToArgument* const pCmd = dynamic_cast<const cgcSetArgumentToArgument*>(pCommand);

	const zString groupName	      = pCmd->GetCommandGroupName();	    // Name of command group
	const long    destCmdIndex    = pCmd->GetDestinationCommandIndex();	// Index of the destination command
	const zString destArgName	  = pCmd->GetDestinationArgumentName();	// Placeholder name of the argument being replaced
	const long    sourceCmdIndex  = pCmd->GetSourceCommandIndex();	    // Index of the source command 
	const zString sourceArgName	  = pCmd->GetSourceArgumentName();	    // Placeholder name of the argument being copied

    
    CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);


    tguCommandGroup* const pGroup = pSimBox->GetCommandGroup(groupName);

	if(pGroup)
	{
        // Note the order of the arguments is: destination first, then source.
        // This is similar to the commands that set an argument to a constant value
        // where the argument comes first and its value second.

        pGroup->SetArgumentToArgument(destCmdIndex, destArgName, sourceCmdIndex, sourceArgName);

        const zString destCmdName   = pGroup->GetCommandName(destCmdIndex);
        const zString sourceCmdName = pGroup->GetCommandName(sourceCmdIndex);

		new CLogcgcSetArgumentToArgument(pSimBox->GetCurrentTime(), groupName, destCmdName, destCmdIndex, destArgName, sourceCmdName, sourceCmdIndex, sourceArgName);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
 
#endif
}

