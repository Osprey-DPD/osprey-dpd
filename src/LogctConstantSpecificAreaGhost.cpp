/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctConstantSpecificAreaGhost.cpp: implementation of the CLogctConstantSpecificAreaGhost class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctConstantSpecificAreaGhost.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctConstantSpecificAreaGhost& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>ConstantSpecificAreaGhost</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<MembraneTargetName>" << rMsg.m_MembraneTargetName   << "</MembraneTargetName>" << zEndl;
	os << "<RservoirTargetName>" << rMsg.m_ReservoirTargetName  << "</ReservoirTargetName>" << zEndl;
	os << "<DecoratorName>"      << rMsg.m_DecLabel             << "</DecoratorName>" << zEndl;
	os << "<BeadName>"           << rMsg.m_BeadName             << "</BeadName>"    << zEndl;
	os << "<SampleRate>"         << rMsg.m_SampleRate           << "</SampleRate>"    << zEndl;
	os << "<TargetAN>"           << rMsg.m_TargetAN             << "</TargetAN>"    << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Constant specific area ghost activity " << rMsg.m_DecLabel << " applied to reservoir command target " << rMsg.m_ReservoirTargetName;
	os << " with parameters " << rMsg.m_MembraneTargetName << " " << rMsg.m_BeadName << " " << rMsg.m_SampleRate << "  " << rMsg.m_TargetAN;

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogctConstantSpecificAreaGhost::CLogctConstantSpecificAreaGhost(long time, const zString membraneTarget, const zString reservoirTarget, 
                                                                 const zString decLabel, const zString beadName, 
                                                                 const long sampleRate, const double targetAN) : CLogConstraintMessage(time), 
                                                                 m_MembraneTargetName(membraneTarget), m_ReservoirTargetName(reservoirTarget), 
                                                                 m_DecLabel(decLabel), m_BeadName(beadName),
                                                                 m_SampleRate(sampleRate), m_TargetAN(targetAN)
{

}

CLogctConstantSpecificAreaGhost::~CLogctConstantSpecificAreaGhost()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctConstantSpecificAreaGhost::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << (*this);
}

