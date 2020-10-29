/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctDistanceMovedByTarget.cpp: implementation of the CLogctDistanceMovedByTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctDistanceMovedByTarget.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctDistanceMovedByTarget& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>DistanceMovedByTarget</Name>" << zEndl;
	os << "<Text>"  << zEndl;
	if(rMsg.m_bStart)
	{
		os << "Distance moved activity " << rMsg.m_DecLabel << " applied to force activity " << rMsg.m_ForceLabel;
		os << " (command target " << rMsg.m_TargetLabel << ")";
		os << " holding " << rMsg.m_BeadTotal << " beads " << rMsg.m_BeadName << " (type " << rMsg.m_BeadType << ")";
		os << " during " << rMsg.m_StartTime << " " << rMsg.m_EndTime;
	}
	else
	{
		os << "Distance moved activity " << rMsg.m_DecLabel << " applied to force activity " << rMsg.m_ForceLabel;
		os << " (command target " << rMsg.m_TargetLabel << ")";
		os << std::setw(12) << std::setprecision(6);
		os << " calculates distance moved " << rMsg.m_Distance << " and average distance " << rMsg.m_DistancePerBead;
	}
	os << "</Text>" << zEndl;	
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	if(rMsg.m_bStart)
	{
		os << "Distance moved activity " << rMsg.m_DecLabel << " applied to force activity " << rMsg.m_ForceLabel;
		os << " (command target " << rMsg.m_TargetLabel << ")";
		os << " holding " << rMsg.m_BeadTotal << " beads " << rMsg.m_BeadName << " (type " << rMsg.m_BeadType << ")";
		os << " during " << rMsg.m_StartTime << " " << rMsg.m_EndTime;
	}
	else
	{
		os << "Distance moved activity " << rMsg.m_DecLabel << " applied to force activity " << rMsg.m_ForceLabel;
		os << " (command target " << rMsg.m_TargetLabel << ")";
		os << std::setw(12) << std::setprecision(6);
		os << " calculates distance moved " << rMsg.m_Distance << " and average distance " << rMsg.m_DistancePerBead;
	}

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogctDistanceMovedByTarget::CLogctDistanceMovedByTarget(long time, const zString targetlabel, 
									const zString forceLabel, const zString decLabel,
									long startTime, long endTime, const zString name, 
									long type, long total) : CLogAnalysisMessage(time), m_bStart(true), 
									m_TargetLabel(targetlabel), m_ForceLabel(forceLabel), 
									m_DecLabel(decLabel),
									m_StartTime(startTime), m_EndTime(endTime),
									m_BeadName(name), m_BeadType(type), m_BeadTotal(total),
									m_Distance(0.0), m_DistancePerBead(0.0)
{

}

CLogctDistanceMovedByTarget::CLogctDistanceMovedByTarget(long time, const zString targetlabel, 
									const zString forceLabel, const zString decLabel, 
									double distance, double distancePerBead) : CLogAnalysisMessage(time), m_bStart(false), 
									m_TargetLabel(targetlabel), m_ForceLabel(forceLabel), 
									m_DecLabel(decLabel), m_StartTime(0), m_EndTime(0),
									m_BeadName(""), m_BeadType(-1), m_BeadTotal(0),
									m_Distance(distance), m_DistancePerBead(distancePerBead)
{

}

CLogctDistanceMovedByTarget::~CLogctDistanceMovedByTarget()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctDistanceMovedByTarget::Serialize(zOutStream& os) const
{
	CLogAnalysisMessage::Serialize(os);

	os << (*this);
}

