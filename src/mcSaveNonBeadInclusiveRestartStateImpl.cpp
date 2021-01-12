/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveNonBeadInclusiveRestartStateImpl.cpp: implementation of the mcSaveNonBeadInclusiveRestartStateImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveNonBeadInclusiveRestartStateImpl.h"
#include "mcSaveNonBeadInclusiveRestartState.h"
#include "Monitor.h"
#include "LogSaveNonBeadInclusiveRestartState.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveNonBeadInclusiveRestartStateImpl::mcSaveNonBeadInclusiveRestartStateImpl()
{
}

mcSaveNonBeadInclusiveRestartStateImpl::~mcSaveNonBeadInclusiveRestartStateImpl()
{

}

// Command handler function to save an inclusive restart state to file without 
// the bead coordinates. This allows the dynamically-created data, such as 
// command targets and polymerised bonds, to be checked without a lot of bead data 
// obscuring the comparison. It is intended mainly for debug purposes.

void mcSaveNonBeadInclusiveRestartStateImpl::SaveNonBeadInclusiveRestartStateCmd(const xxCommand* const pCommand)
{
//	const mcSaveNonBeadInclusiveRestartState* const pCmd = dynamic_cast<const mcSaveNonBeadInclusiveRestartState*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);
	
	pMon->SaveNonBeadInclusiveRestartState();

	// This command cannot fail

	new CLogSaveNonBeadInclusiveRestartState(pMon->GetCurrentTime());
}

