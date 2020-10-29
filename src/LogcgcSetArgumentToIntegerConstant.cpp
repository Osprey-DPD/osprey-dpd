/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogcgcSetArgumentToIntegerConstant.cpp: implementation of the CLogcgcSetArgumentToIntegerConstant class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogcgcSetArgumentToIntegerConstant.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentToIntegerConstant& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<Name>SetArgumentToIntegerConstant</Name>" << zEndl;
	os << "<CommandGroupName>" << rMsg.m_GroupName    << "</CommandGroupName>" << zEndl;
	os << "<CommandName>"      << rMsg.m_CommandName  << "</CommandName>"      << zEndl;
	os << "<CommandNo>"        << rMsg.m_CommandNo    << "</CommandNo>"        << zEndl;
	os << "<ArgumentName>"     << rMsg.m_ArgName      << "</ArgumentName>"     << zEndl;
	os << "<ArgumentValue>"    << rMsg.m_Value        << "</ArgumentValue>"    << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
    os << "Command " << rMsg.m_CommandName << " (index " << rMsg.m_CommandNo << ") " << "in group " << rMsg.m_GroupName << zEndl;
	os << "setting argument " << rMsg.m_ArgName << "  to integer constant " << rMsg.m_Value; 

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogcgcSetArgumentToIntegerConstant::CLogcgcSetArgumentToIntegerConstant(long time, const zString groupName,
                                                   const zString cmdName, long cmdNo, const zString argName, 
                                                   long value) : CLogInfoMessage(time), 
                                                   m_GroupName(groupName), m_CommandName(cmdName),
                                                   m_ArgName(argName), m_CommandNo(cmdNo),
                                                   m_Value(value)
                                                   
{
}

CLogcgcSetArgumentToIntegerConstant::~CLogcgcSetArgumentToIntegerConstant()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogcgcSetArgumentToIntegerConstant::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << (*this);
}

