/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctListAllCommandTargetActivitiesImpl.cpp: implementation of the ctListAllCommandTargetActivitiesImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctListAllCommandTargetActivitiesImpl.h"
#include "ctListAllCommandTargetActivities.h"
#include "SimBox.h"
#include "CommandTargetNode.h"
#include "LogctListAllCommandTargetActivities.h"
#include "LogTextMessage.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctListAllCommandTargetActivitiesImpl::ctListAllCommandTargetActivitiesImpl()
{
}

ctListAllCommandTargetActivitiesImpl::~ctListAllCommandTargetActivitiesImpl()
{

}

// Command handler write a list of all the decorators that wrap all targets
// to the log file. First, it stores each target's id and type, and then it
// assembles a list of the labels of all decorators that wrap each target.
// Each target's data is written on a separate line. The list of  decorators 
// ends with the target's own label, as this is the innermost
// decorator label. If a target has no activities, just its own label is 
// written to the log file.

void ctListAllCommandTargetActivitiesImpl::ListAllCommandTargetActivities(const xxCommand* const pCommand)
{
//	const ctListAllCommandTargetActivities* const pCmd = dynamic_cast<const ctListAllCommandTargetActivities*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

    CommandTargetSequence targets = pSimBox->GetCommandTargets();
    const long targetTotal = targets.size();

    zArray2dString decList;  // Container of all targets' decorator details
    decList.clear();

    if(targetTotal > 0)
    {
        StringSequence targetList;  // Container holding each target's details
        zString id, type;

        for(CommandTargetIterator iterTarget=targets.begin(); iterTarget!=targets.end(); iterTarget++)
        {
            targetList.clear();
            id    = pSimBox->ToString((*iterTarget)->GetId());
            type  = (*iterTarget)->GetTargetType();

            targetList.push_back(id);
            targetList.push_back(type);

            const long decTotal = (*iterTarget)->GetDecoratorTotal();

            if(decTotal > 0)
            {
                const CCommandTargetNode* const pOutermost = (*iterTarget)->GetOutermostDecorator();

                targetList.push_back(pOutermost->GetLabel());

                CCommandTargetNode*  pInner = pOutermost->GetInnerDecorator();
                while(pInner)
                {
                    targetList.push_back(pInner->GetLabel());
                    pInner = pInner->GetInnerDecorator();
                }
            }
            else
            {
                // This target has no decorators so we just output its own label

                targetList.push_back((*iterTarget)->GetLabel());
            }

            decList.push_back(targetList);
        }

        new CLogctListAllCommandTargetActivities(pSimBox->GetCurrentTime(), decList);
    }
    else
    {
        new CLogTextMessage(pSimBox->GetCurrentTime(), "No command targets exist");
    }
}
