/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgtCreateCommandGroupImpl.cpp: implementation of the cgtCreateCommandGroupImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgtCreateCommandGroupImpl.h"
#include "cgtCreateCommandGroup.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "LogcgtCreateCommandGroup.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgtCreateCommandGroupImpl::cgtCreateCommandGroupImpl()
{
}

cgtCreateCommandGroupImpl::~cgtCreateCommandGroupImpl()
{
}

// Command handler to create a command group that wraps a set of commands that are 
// to be executed together.

void cgtCreateCommandGroupImpl::InternalCreateCommandGroup(const xxCommand* const pCommand)
{
#if EnableCommandGroups == SimCommandEnabled

	const cgtCreateCommandGroup* const pCmd = dynamic_cast<const cgtCreateCommandGroup*>(pCommand);

	const zString groupName	= pCmd->GetCommandGroupName();	 // Name of command group

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);


	// Create and store an empty tguCommandGroup that can store command instances.

	if(pSimBox->CreateCommandGroup(groupName))
	{
		new CLogcgtCreateCommandGroup(pSimBox->GetCurrentTime(), groupName);
	}
	else
	{
		new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}

#endif
}

