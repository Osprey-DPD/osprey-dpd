/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogShowModifiableProcesses.cpp: implementation of the CLogShowModifiableProcesses class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogShowModifiableProcesses.h"
#include "xxProcess.h"


//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogShowModifiableProcesses& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>ShowModifiableProcesses</Name>" << zEndl;
	os << "<Text>" << zEndl;

	if(rMsg.m_ProcessTotal > 0)
	{
		os << "<ProcessTotal>" << rMsg.m_ProcessTotal << "</ProcessTotal>" << zEndl;

		for(cProcessIterator citerProc=rMsg.m_Processes.begin(); citerProc!=rMsg.m_Processes.end(); citerProc++)
		{
			const zString type = (*citerProc)->GetProcessType();
			const zString pid  = (*citerProc)->GetPid();

			os << "<Name>" << type << "</Name>" << zEndl;
			os << "<Pid>"  << pid  << "</Pid>" << zEndl;
		}	
	}
	else
	{
		os << "<ProcessTotal>0</ProcessTotal>" << zEndl;
	}
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	if(rMsg.m_ProcessTotal > 0)
	{
		os << "Running processes are: " << zEndl;

		for(cProcessIterator citerProc=rMsg.m_Processes.begin(); citerProc!=rMsg.m_Processes.end(); citerProc++)
		{
			const zString type = (*citerProc)->GetProcessType();
			const zString pid  = (*citerProc)->GetPid();

			os << type << " " << pid << zEndl;
		}
	}
	else
	{
		os << "There are no running processes" << zEndl;
	}

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogShowModifiableProcesses::CLogShowModifiableProcesses(long time, ProcessSequence vProcesses) : CLogInfoMessage(time),
											m_ProcessTotal(vProcesses.size()),
											m_Processes(vProcesses)
{

}

CLogShowModifiableProcesses::~CLogShowModifiableProcesses()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogShowModifiableProcesses::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << (*this);
}

