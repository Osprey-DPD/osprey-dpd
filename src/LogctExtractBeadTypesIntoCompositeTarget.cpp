/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctExtractBeadTypesIntoCompositeTarget.cpp: implementation of the CLogctExtractBeadTypesIntoCompositeTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctExtractBeadTypesIntoCompositeTarget.h"
#include "CommandTargetNode.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctExtractBeadTypesIntoCompositeTarget& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>ExtractBeadTypesIntoCompositeTarget</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<SourceTargetName>" << rMsg.m_TargetLabel   << "</SourceTargetName>" << zEndl;
	os << "<DestTargetName>"   << rMsg.m_DestLabel     << "</DestTargetName>" << zEndl;
	os << "<CompTargetName>"   << rMsg.m_CompLabel     << "</CompTargetName>" << zEndl;
	os << "<BeadTotal>"        << rMsg.m_CompBeadTotal << "</BeadTotal>" << zEndl;

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
	os << " into composite target with root name " << rMsg.m_DestLabel << zEndl;
	os << "Composite target " << rMsg.m_CompLabel << " holds " << rMsg.m_CompBeadTotal << " beads";

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

CLogctExtractBeadTypesIntoCompositeTarget::CLogctExtractBeadTypesIntoCompositeTarget(long time, 
									const zString targetLabel,const zString destLabel, 
									const zString compLabel, long compBeadTotal, 
									LongTargetMap mTargets) : CLogConstraintMessage(time),
									m_TargetLabel(targetLabel),
									m_DestLabel(destLabel),
									m_CompLabel(compLabel),
									m_CompBeadTotal(compBeadTotal),
									m_mTargets(mTargets)
{

}

CLogctExtractBeadTypesIntoCompositeTarget::~CLogctExtractBeadTypesIntoCompositeTarget()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctExtractBeadTypesIntoCompositeTarget::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << (*this);
}

