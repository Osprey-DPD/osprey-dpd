/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogcgtCommandInstantiationFailed.cpp: implementation of the CLogcgtCommandInstantiationFailed class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogcgtCommandInstantiationFailed.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogcgtCommandInstantiationFailed& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<Name>CommandInstantiationFailed</Name>" << zEndl;
	os << "<CommandGroupName>" << rMsg.m_GroupName   << "</CommandGroupName>" << zEndl;
	os << "<CommandName>"      << rMsg.m_CommandName << "</CommandName>"      << zEndl;
	os << "<ArgumentName>"     << rMsg.m_ArgName     << "</ArgumentName>"     << zEndl;
	os << "<ArgumentType>"     << rMsg.m_ArgType     << "</ArgumentType>"     << zEndl;
	os << "<ArgumentSource>"   << rMsg.m_ArgSource   << "</ArgumentSource>"   << zEndl;
    os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;


#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Failed to instantiate command " << rMsg.m_CommandName << " in command group "  << rMsg.m_GroupName << zEndl;
    if(!rMsg.m_ArgType.empty() && !rMsg.m_ArgSource.empty())
    {
        os << "Check argument " << rMsg.m_ArgName << " with expected type " << rMsg.m_ArgType << " obtained from " << rMsg.m_ArgSource;
    }
    else if(rMsg.m_ArgType.empty())
    {
        os << "Check argument " << rMsg.m_ArgName << " with unspecified type";
    }
    else
    {
        os << "Check argument " << rMsg.m_ArgName << " with expected type " << rMsg.m_ArgType << " but unspecified source";
    }

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogcgtCommandInstantiationFailed::CLogcgtCommandInstantiationFailed(long time, const zString groupName, const zString cmdName,
                                                   const zString argName, const zString argType, const zString argSource) : CLogWarningMessage(time), 
                                                   m_GroupName(groupName),
                                                   m_CommandName(cmdName),
                                                   m_ArgName(argName),
                                                   m_ArgType(argType),
                                                   m_ArgSource(argSource)

{
}

CLogcgtCommandInstantiationFailed::~CLogcgtCommandInstantiationFailed()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogcgtCommandInstantiationFailed::Serialize(zOutStream& os) const
{
	CLogWarningMessage::Serialize(os);

	os << (*this);
}

