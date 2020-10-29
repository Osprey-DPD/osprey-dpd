/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctRemoveCommandTargetActivity.cpp: implementation of the CLogctRemoveCommandTargetActivity class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctRemoveCommandTargetActivity.h"


//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctRemoveCommandTargetActivity& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>RemoveCommandTargetActivity</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<ActivityName>"  << rMsg.m_ActivityLabel  << "</ActivityName>" << zEndl;
	os << "<TargetName>"    << rMsg.m_TargetLabel    << "</TargetName>" << zEndl;
	os << "<ActivityTotal>" << rMsg.m_ActivitiesLeft << "</ActivityTotal>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Activity " << rMsg.m_ActivityLabel << " removed from active command target " << rMsg.m_TargetLabel;
	os << " (" << rMsg.m_ActivitiesLeft << " activities left)";

	// If no more activities are left, log the removal of the command target
	// from the active targets container

	if(rMsg.m_ActivitiesLeft > 0)
	{
		os << zEndl;
	}
	else
	{
		os << " " << rMsg.m_TargetLabel << " removed from active command targets";
	}

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogctRemoveCommandTargetActivity::CLogctRemoveCommandTargetActivity(long time, const zString target,
																	 const zString activity,
																	 long activitiesLeft) : CLogConstraintMessage(time), 
																					     m_TargetLabel(target),
																					     m_ActivityLabel(activity),
																						 m_ActivitiesLeft(activitiesLeft)
{

}

CLogctRemoveCommandTargetActivity::~CLogctRemoveCommandTargetActivity()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctRemoveCommandTargetActivity::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << (*this);
}

