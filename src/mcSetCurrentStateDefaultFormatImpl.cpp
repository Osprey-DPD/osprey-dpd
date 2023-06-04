/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetCurrentStateDefaultFormatImpl.cpp: implementation of the mcSetCurrentStateDefaultFormatImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetCurrentStateDefaultFormatImpl.h"
#include "mcSetCurrentStateDefaultFormat.h"
#include "Monitor.h"
#include "LogSetCurrentStateDefaultFormat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetCurrentStateDefaultFormatImpl::mcSetCurrentStateDefaultFormatImpl()
{
}

mcSetCurrentStateDefaultFormatImpl::~mcSetCurrentStateDefaultFormatImpl()
{

}

// Command handler function to change the default format for current state output
// to the specified type. We change the format on all processors, but only log the execution on P0.

void mcSetCurrentStateDefaultFormatImpl::SetCurrentStateDefaultFormat(const xxCommand* const pCommand)
{
	const mcSetCurrentStateDefaultFormat* const pCmd = dynamic_cast<const mcSetCurrentStateDefaultFormat*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	// The following if statements should be replaced by use of a 
	// map<zString,CCurrentStateFormat*>. If the format is unrecognised,
	// we log a "command failed" message.

#if EnableParallelCommands == SimMPSEnabled

	const zString format = pCmd->GetFormat();

	if(format == "Povray")
	{
		pMon->m_DefaultCurrentStateFormat = "Povray";

        if(xxParallelBase::GlobalGetRank() == 0)
        {
		    new CLogSetCurrentStateDefaultFormat(pMon->GetCurrentTime(), pMon->m_DefaultCurrentStateFormat);
        }
	}
	else if(format == "PovrayWithBonds")
	{
		pMon->m_DefaultCurrentStateFormat = "PovrayWithBonds";

        if(xxParallelBase::GlobalGetRank() == 0)
        {
		    new CLogSetCurrentStateDefaultFormat(pMon->GetCurrentTime(), pMon->m_DefaultCurrentStateFormat);
        }
	}
	else if(format == "Amira")
	{
		pMon->m_DefaultCurrentStateFormat = "Amira";

        if(xxParallelBase::GlobalGetRank() == 0)
        {
		    new CLogSetCurrentStateDefaultFormat(pMon->GetCurrentTime(), pMon->m_DefaultCurrentStateFormat);
        }
	}
	else if(format == "Paraview")
	{
		pMon->m_DefaultCurrentStateFormat = "Paraview";

        if(xxParallelBase::GlobalGetRank() == 0)
        {
		    new CLogSetCurrentStateDefaultFormat(pMon->GetCurrentTime(), pMon->m_DefaultCurrentStateFormat);
        }
	}
	else
	{
        if(xxParallelBase::GlobalGetRank() == 0)
        {
		     new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
        }
	}
	
#else

	const zString format = pCmd->GetFormat();

	if(format == "Povray")
	{
		pMon->m_DefaultCurrentStateFormat = "Povray";

		new CLogSetCurrentStateDefaultFormat(pMon->GetCurrentTime(), pMon->m_DefaultCurrentStateFormat);
	}
	else if(format == "PovrayWithBonds")
	{
		pMon->m_DefaultCurrentStateFormat = "PovrayWithBonds";

		new CLogSetCurrentStateDefaultFormat(pMon->GetCurrentTime(), pMon->m_DefaultCurrentStateFormat);
	}
	else if(format == "Amira")
	{
		pMon->m_DefaultCurrentStateFormat = "Amira";

		new CLogSetCurrentStateDefaultFormat(pMon->GetCurrentTime(), pMon->m_DefaultCurrentStateFormat);
	}
	else if(format == "Paraview")
	{
		pMon->m_DefaultCurrentStateFormat = "Paraview";

		new CLogSetCurrentStateDefaultFormat(pMon->GetCurrentTime(), pMon->m_DefaultCurrentStateFormat);
	}
    else if(format == "SolventFree")
    {
        pMon->m_DefaultCurrentStateFormat = "SolventFree";
        
            new CLogSetCurrentStateDefaultFormat(pMon->GetCurrentTime(), pMon->m_DefaultCurrentStateFormat);
    }
    else if(format == "SolventFreeAndPovray")
    {
        pMon->m_DefaultCurrentStateFormat = "SolventFreeAndPovray";
            
            new CLogSetCurrentStateDefaultFormat(pMon->GetCurrentTime(), pMon->m_DefaultCurrentStateFormat);
    }
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}

#endif
}

