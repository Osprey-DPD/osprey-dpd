/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveProtocolFileImpl.cpp: implementation of the mcSaveProtocolFileImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveProtocolFileImpl.h"

#if EnableMonitorCommand == SimCommandEnabled
  #include "mcSaveProtocolFile.h"
#endif

#include "Monitor.h"
#include "ISimBox.h"
#include "ProtocolFile.h"
#include "LogSaveProtocolFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveProtocolFileImpl::mcSaveProtocolFileImpl()
{
}

mcSaveProtocolFileImpl::~mcSaveProtocolFileImpl()
{

}



void mcSaveProtocolFileImpl::SaveProtocolFile(const xxCommand* const pCommand)
{
#if EnableMonitorCommand == SimCommandEnabled
	const mcSaveProtocolFile* const pCmd = dynamic_cast<const mcSaveProtocolFile*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	const ISimBox* const pISimBox = pMon->GetISimBox();

	zString simType;

#if SimIdentifier == BD
	simType = "bd";
#elif SimIdentifier == DPD
	simType = "dpd";
#elif SimIdentifier == MD
	simType = "md";
#endif

	const zString protocolId	= pISimBox->GetInitialStateType();
	const zString runId			= pISimBox->GetRunId();
	const zString title			= pISimBox->GetTitle();
	const zString date			= pISimBox->GetDate();
	const zString comment		= pISimBox->GetComment();

	// Constructy a CProtocolFile instance passing it the essential information
	// to identify the protocol; then set its protected member variables using the
	// friend status of the mcSaveProtocolFileImpl class. Finally, we call its
	// Serialize function to write the data to file and issue a message to 
	// indicate the successful, or otherwise, execution of the command.

	CProtocolFile protocol(pISimBox, simType, protocolId, runId, title, date, comment);


	if(protocol.Serialize())
	{
		new CLogSaveProtocolFile(pMon->GetCurrentTime(), protocolId, runId);
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}
#endif
}
