/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctConstantSpecificAreaGhostImpl.cpp: implementation of the ctConstantSpecificAreaGhostImpl class.
//
//////////////////////////////////////////////////////////////////////
  
#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ctConstantSpecificAreaGhostImpl.h"
#include "ctConstantSpecificAreaGhost.h"
#include "taConstantSpecificAreaGhost.h"
#include "SimBox.h"
#include "Bead.h"
#include "CommandTarget.h"
#include "CommandTargetPolymer.h"
#include "LogctConstantSpecificAreaGhost.h"
#include "LogTextMessage.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctConstantSpecificAreaGhostImpl::ctConstantSpecificAreaGhostImpl()
{

}

ctConstantSpecificAreaGhostImpl::~ctConstantSpecificAreaGhostImpl()
{

}


void ctConstantSpecificAreaGhostImpl::ConstantSpecificArea(const xxCommand* const pCommand)
{
	const ctConstantSpecificAreaGhost* const pCmd = dynamic_cast<const ctConstantSpecificAreaGhost*>(pCommand);

	const zString	targetLabel         = pCmd->GetReservoirTargetName();
	const zString	decLabel	        = pCmd->GetDecLabel();
	const zString	membraneLabel       = pCmd->GetMembraneTargetName();
    
    std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " ConstantSpecificAreaGhostImpl has targets " << targetLabel << " " << membraneLabel << " " << decLabel << zEndl;

        const zString	beadName            = pCmd->GetBeadName();

	const long      sampleRate	        = pCmd->GetSampleRate();	 
	const double      targetAN	        = pCmd->GetTargetAN();	 

    // Get a CSimBox pointer and convert bead labels to types

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

        const long start = pSimBox->GetCurrentTime();
        const long end = pSimBox->GetTotalTime();

        long beadType = pSimBox->GetBeadTypeFromName(beadName);

	// Get the ghost reservoir and membrane targets from the target list

	CCommandTargetNode* pCmdTarget      = pSimBox->GetCommandTarget(targetLabel);
	CCommandTargetNode* pMembraneTarget = pSimBox->GetCommandTarget(membraneLabel);

	// Now we have the target, create a taConstantSpecificAreaGhost decorator object 
	// and wrap the target with it to enable the area algorithm to work.

	if(pCmdTarget && pMembraneTarget)
	{
		CCommandTargetNode* pActiveTarget = pSimBox->GetActiveCommandTargetOutermost(targetLabel);

		// If the target is already active, remove it before adding it back
		// using the new decorator: we just check if the label of the
		// outermost decorator instance differs from the target's label.
		// Note that because we now know that pCmdTarget exists, we don't 
		// have to check that pActiveTarget is non-NULL as it must either be
		// a pointer to a decorator instance or equal to pCmdTarget.

		if(pActiveTarget->GetLabel() != targetLabel)
		{
		    pSimBox->m_ActiveCommandTargets.remove(pActiveTarget);
		}

		// Wrap the active target in a new area decorator instance
		// and add it to the active command targets container. 
        // We pass in an ISimState pointer so that the decorator can issue commands.

        ISimState* const pISimState = pSimBox;

        taConstantSpecificAreaGhost*  pDec = new taConstantSpecificAreaGhost(decLabel, pActiveTarget, pMembraneTarget, 
                                                pISimState, beadType, start, end, sampleRate, targetAN);


	pSimBox->m_ActiveCommandTargets.push_back(pDec);

	// Log sucessful execution of the command
        

#if EnableParallelSimBox == SimMPSEnabled
        if(xxParallelBase::GlobalGetRank() == 0)
        {
            new CLogctConstantSpecificAreaGhost(pSimBox->GetCurrentTime(), membraneLabel, targetLabel, decLabel, 
                                                                                        beadName, sampleRate, targetAN);
        }
    }
	else
	{
        if(xxParallelBase::GlobalGetRank() == 0)
        {
            new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
            
        }
    }
#else
            new CLogctConstantSpecificAreaGhost(pSimBox->GetCurrentTime(), membraneLabel, targetLabel, decLabel, beadName, sampleRate, targetAN);
    }
    else
    {
         new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
    }
#endif


}


