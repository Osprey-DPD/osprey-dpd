/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctExtractBeadTypesIntoTargets.cpp: implementation of the CLogctExtractBeadTypesIntoTargets class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctExtractBeadTypesIntoTargets.h"
#include "CommandTargetNode.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctExtractBeadTypesIntoTargets& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>ExtractBeadTypesIntoTargets</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<SourceTargetName>" << rMsg.m_TargetLabel << "</SourceTargetName>" << zEndl;
	os << "<DestTargetName>"   << rMsg.m_DestLabel   << "</DestTargetName>" << zEndl;

	for(cLongTargetIterator iterTarget=rMsg.m_mTargets.begin(); iterTarget!=rMsg.m_mTargets.end(); iterTarget++)
	{
		os << "<TargetName>" << (iterTarget->second)->GetLabel() << "</TargetName>";
		os << "<BeadType>"   << (iterTarget->second)->GetCurrentBeadType() << "</BeadType>" << zEndl;
		os << "<BeadTotal>"  << (iterTarget->second)->GetBeadTotal() << "</BeadTotal>" << zEndl;
	}

	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Beads extracted from source target " << rMsg.m_TargetLabel;
	os << " into simple targets with root name " << rMsg.m_DestLabel;

	// Note that by construction all the m_Targets are newly-created simple bead 
	// targets, so there is no ambiguity aboug using GetCurrentBeadType().

	for(cLongTargetIterator iterTarget=rMsg.m_mTargets.begin(); iterTarget!=rMsg.m_mTargets.end(); iterTarget++)
	{
		os << zEndl << "Target "<< (iterTarget->second)->GetLabel() << " holds " << (iterTarget->second)->GetBeadTotal() << " beads ";
		os << "of type " << (iterTarget->second)->GetCurrentBeadType();
	}

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogctExtractBeadTypesIntoTargets::CLogctExtractBeadTypesIntoTargets(long time, 
									const zString targetlabel,const zString destlabel,
									LongTargetMap mTargets) : CLogConstraintMessage(time),
									m_TargetLabel(targetlabel),
									m_DestLabel(destlabel),
									m_mTargets(mTargets)
{

}

CLogctExtractBeadTypesIntoTargets::~CLogctExtractBeadTypesIntoTargets()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctExtractBeadTypesIntoTargets::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << (*this);
}

