/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctReflectingBarrierOnTarget.cpp: implementation of the CLogctReflectingForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctReflectingBarrierOnTarget.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctReflectingBarrierOnTarget& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>ReflectingBarrierOnTarget</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<DecoratorName>" << rMsg.m_DecLabel    << "</DecoratorName>" << zEndl;
	os << "<TargetName>"    << rMsg.m_TargetLabel << "</TargetName>"    << zEndl;
	os << rMsg.m_X << " " << rMsg.m_Y << " " << rMsg.m_Z << "  " << rMsg.m_Magnitude << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Reflecting barrier activity " << rMsg.m_DecLabel << " applied to command target " << rMsg.m_TargetLabel;
	os << " with parameters " << rMsg.m_X << " " << rMsg.m_Y << " " << rMsg.m_Z << "  " << rMsg.m_Magnitude;

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogctReflectingBarrierOnTarget::CLogctReflectingBarrierOnTarget(long time,
									const zString targetlabel, const zString decLabel, 
									long xn, long yn, long zn, 
									double mag) : CLogConstraintMessage(time),
									m_TargetLabel(targetlabel),
									m_DecLabel(decLabel),
									m_X(xn), m_Y(yn), m_Z(zn),
									m_Magnitude(mag)
{

}

CLogctReflectingBarrierOnTarget::~CLogctReflectingBarrierOnTarget()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctReflectingBarrierOnTarget::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << (*this);
}

