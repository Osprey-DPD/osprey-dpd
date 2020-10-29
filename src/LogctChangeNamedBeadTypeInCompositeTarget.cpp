/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctChangeNamedBeadTypeInCompositeTarget.cpp: implementation of the CLogctChangeNamedBeadTypeInCompositeTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctChangeNamedBeadTypeInCompositeTarget.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctChangeNamedBeadTypeInCompositeTarget& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>ChangeNamedBeadTypeInCompositeTarget</Name>"  << zEndl;
	os << "<Text>" << zEndl;
	os << "<CommandTargetName>" << rMsg.m_CmdTarget << "</CommandTargetName>" << zEndl;
	os << "<OldName>"           << rMsg.m_OldName   << "</OldName>"  << zEndl;
	os << "<OldType>"           << rMsg.m_OldType   << "</OldType>"  << zEndl;
	os << "<NewName>"           << rMsg.m_NewName   << "</NewName>"  << zEndl;
	os << "<NewType>"           << rMsg.m_NewType   << "</NewType>"  << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Composite command target " << rMsg.m_CmdTarget << " has beads "
	   << rMsg.m_OldName << " (type " << rMsg.m_OldType 
	   << ") changed to type " 	<< rMsg.m_NewName << " (type " << rMsg.m_NewType <<  ")";


#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogctChangeNamedBeadTypeInCompositeTarget::CLogctChangeNamedBeadTypeInCompositeTarget(long time, const zString cmdTarget, 
													 const zString oldName, const zString newName, 
													long oldType, long newType) : CLogConstraintMessage(time), 
													m_CmdTarget(cmdTarget), 
													m_OldName(oldName), m_NewName(newName), 
													m_OldType(oldType), m_NewType(newType)
{

}

CLogctChangeNamedBeadTypeInCompositeTarget::~CLogctChangeNamedBeadTypeInCompositeTarget()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctChangeNamedBeadTypeInCompositeTarget::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << (*this);
}

