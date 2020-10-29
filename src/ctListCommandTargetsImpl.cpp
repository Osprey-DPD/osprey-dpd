/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctListCommandTargetsImpl.cpp: implementation of the ctListCommandTargetsImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctListCommandTargetsImpl.h"
#include "ctListCommandTargets.h"
#include "SimBox.h"
#include "CommandTargetNode.h"
#include "LogctListCommandTargets.h"
#include "LogTextMessage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctListCommandTargetsImpl::ctListCommandTargetsImpl()
{
}

ctListCommandTargetsImpl::~ctListCommandTargetsImpl()
{

}

// Command handler write a list of all current command targets to the log file.
// It has no arguments, but assembles the id, type and label for each target
// into a sequence of strings that are then written to the log file.

void ctListCommandTargetsImpl::ListCommandTargets(const xxCommand* const pCommand)
{
//	const ctListCommandTargets* const pCmd = dynamic_cast<const ctListCommandTargets*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

    CommandTargetSequence targets = pSimBox->GetCommandTargets();
    const long targetTotal = targets.size();

    if(targetTotal > 0)
    {
        StringSequence targetList;
        targetList.clear();
        zString id, type, label;

        for(CommandTargetIterator iterTarget=targets.begin(); iterTarget!=targets.end(); iterTarget++)
        {
            id    = pSimBox->ToString((*iterTarget)->GetId());
            type  = (*iterTarget)->GetTargetType();
            label = (*iterTarget)->GetLabel();

            targetList.push_back(id);
            targetList.push_back(type);
            targetList.push_back(label);
        }

	    CLogctListCommandTargets* pMsg = new CLogctListCommandTargets(pSimBox->GetCurrentTime(), targetTotal, targetList);
    }
    else
    {
        CLogTextMessage* pWarning = new CLogTextMessage(pSimBox->GetCurrentTime(), "No command targets exist");
    }
}
