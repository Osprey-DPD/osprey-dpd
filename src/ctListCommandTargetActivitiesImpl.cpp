/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctListCommandTargetActivitiesImpl.cpp: implementation of the ctListCommandTargetActivitiesImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctListCommandTargetActivitiesImpl.h"
#include "ctListCommandTargetActivities.h"
#include "SimBox.h"
#include "CommandTargetNode.h"
#include "LogctListCommandTargetActivities.h"
#include "LogTextMessage.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctListCommandTargetActivitiesImpl::ctListCommandTargetActivitiesImpl()
{
}

ctListCommandTargetActivitiesImpl::~ctListCommandTargetActivitiesImpl()
{

}

// Command handler write a list of all decorators that wrap a specified target
// to the log file. It takes the target's label as an argument and assembles the  
// id, type and label of each decorator instance into a sequence of strings 
// that are then written to the log file. If the target does not exist, it writes
// a "command failed" message to the log file, while if the target has no 
// decorators, it just writes an informational message.

void ctListCommandTargetActivitiesImpl::ListCommandTargetActivities(const xxCommand* const pCommand)
{
	const ctListCommandTargetActivities* const pCmd = dynamic_cast<const ctListCommandTargetActivities*>(pCommand);

    const zString targetLabel = pCmd->GetTargetLabel();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

    CCommandTargetNode* const pTarget = pSimBox->GetCommandTarget(targetLabel);

    if(pTarget)
    {
        const long decTotal = pTarget->GetDecoratorTotal();

        if(decTotal > 0)
        {
            StringSequence decList;
            decList.clear();

            const CCommandTargetNode* const pOutermost = pTarget->GetOutermostDecorator();
            CCommandTargetNode*  pInner     = pOutermost->GetInnerDecorator();

            decList.push_back(pOutermost->GetLabel());

            while(pInner)
            {
                decList.push_back(pInner->GetLabel());
                pInner = pInner->GetInnerDecorator();
            }

            // Remove the last entry as this is the target's own label
            decList.pop_back();

    	    new CLogctListCommandTargetActivities(pSimBox->GetCurrentTime(), targetLabel, decList);
        }
        else
        {
            const zString msg = "Command target " + targetLabel + " has no activities";
            new CLogTextMessage(pSimBox->GetCurrentTime(), msg);
        }

    }
    else
    {
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
    }
}
