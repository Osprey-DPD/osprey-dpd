/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccStartCommandDocumentProcessImpl.cpp: implementation of the ccStartCommandDocumentProcessImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccStartCommandDocumentProcessImpl.h"
#include "ccStartCommandDocumentProcess.h"
#include "SimBox.h"
#include "ISimulation.h"
#include "IExperimentAccessControl.h"
#include "prCommandDocument.h"
#include "LogStartCommandDocumentProcess.h"
#include "LogExperimentNotAccessible.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccStartCommandDocumentProcessImpl::ccStartCommandDocumentProcessImpl()
{
}

ccStartCommandDocumentProcessImpl::~ccStartCommandDocumentProcessImpl()
{

}

// Command handler to start the process that allows command documents to be 
// imported into a running experiment. In order for this process to be created,
// the simulation must be running within an experiment. If the command is issued
// more than once, a warning message is logged and the command is ignored.

void ccStartCommandDocumentProcessImpl::StartCommandDocumentProcess(const xxCommand* const pCommand)
{
	const ccStartCommandDocumentProcess* const pCmd = dynamic_cast<const ccStartCommandDocumentProcess*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Note that if the simulation is running stand-alone its IExperimentAccessControl
	// interface is valid and its IsExperimentAccessible() function should be called
	// to see if the experiment is present. If the simulation is running stand-alone
	// we issue a warning message to log that the user has issued commands that
	// require the simulation to be part of an experiment.

	ISimState* pISimState = pSimBox;

	ISimulation* pISim = pISimState->GetISimulation();

	IExperimentAccessControl* pEAC = pISim->GetIExperimentAccessControl();

	if(pEAC->IsExperimentAccessible())
	{
		// Check to see if the process is already running

		if(true)
		{
			prCommandDocument* pProcess = new prCommandDocument();

			pProcess->InternalValidateData(pISimState);

			// We have to call the process' ValidateData() function as it creates
			// the process' associated process state file.

			pISimState->AddProcess(pProcess);

			 new CLogStartCommandDocumentProcess(pSimBox->GetCurrentTime());
		}
		else
		{
			 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
		}
	}
	else
	{
		 new CLogExperimentNotAccessible(pSimBox->GetCurrentTime());
	}
}
