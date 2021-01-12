/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcCommentImpl.cpp: implementation of the mcCommentImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcCommentImpl.h"
#include "mcComment.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "LogTextMessage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcCommentImpl::mcCommentImpl()
{
}

mcCommentImpl::~mcCommentImpl()
{

}

// Command handler to output an arbitrary text message to the log file.
// This allows keywords, parameters and comments to be embedded in the log file
// for later use in searching and retrieving the results.  We allow empty strings, 
// and just write a sequence of asterisks to act as a separator line.

void mcCommentImpl::Comment(const xxCommand* const pCommand)
{
	const mcComment* const pCmd = dynamic_cast<const mcComment*>(pCommand);

	CMonitor* pMon = dynamic_cast<CMonitor*>(this);

    const zString msg = pCmd->GetMessage();

	if(msg != "")
	{
         new CLogTextMessage(pMon->GetCurrentTime(), msg);
	}
	else
	{
         new CLogTextMessage(pMon->GetCurrentTime(), "**********");
	}
}
