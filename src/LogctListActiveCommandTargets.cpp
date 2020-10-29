/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctListActiveCommandTargets.cpp: implementation of the CLogctListActiveCommandTargets class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctListActiveCommandTargets.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////
// Current target properties are:
//
// id - numeric identifier (converted to a string)
// type - class name of the target's type
// label - unique alphanumeric label of the target decorator
// innerLabel - unique alphanumeric label of the wrapped target

zOutStream& operator<<(zOutStream& os, const CLogctListActiveCommandTargets& rMsg)
{
    cStringSequenceIterator citerString = rMsg.m_TargetList.begin();

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>ListActiveCommandTargets</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<TargetTotal>" << rMsg.m_TargetTotal << "</TargetTotal>" << zEndl;


    for(long i=0; i<rMsg.m_TargetTotal; i++)
    {
        os << "<TargetId>"    << *citerString++ << "</TargetId>"    << " ";
        os << "<TargetType>"  << *citerString++ << "</TargetType>"  << " ";
        os << "<TargetLabel>" << *citerString++ << "</TargetLabel>" << zEndl;
        os << "<ActiveTargetLabel>" << *citerString++ << "</ActiveTargetLabel>" << zEndl;
    }

	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "The following active command targets exist: " << zEndl;
    for(long i=0; i<rMsg.m_TargetTotal; i++)
    {
        for(long j=0; j<rMsg.m_PerLine; j++)
        {
	        os << *citerString++ << " ";
        }
        os << zEndl;
    }

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store the set of target properties that are to be written out as 
// a set of strings. If new properties are added later, the serialisation
// functions above must be modified to format the data appropriately.
// The number of targets is used to determine how many strings should be
// output to each line. We assume that the number of elements in the sequence
// is an exact multiple of the number of targets.

// Note that we manually copy the strings in the container as it is passed in 
// by reference, and the original container will be out of scope when this
// message's operator<< function is called.

CLogctListActiveCommandTargets::CLogctListActiveCommandTargets(long time, long total, const StringSequence& targetList) : CLogAnalysisMessage(time),
                                                                            m_TargetTotal(total)
{
    m_TargetList.clear();

    if(m_TargetTotal > 0)
    {
        for(cStringSequenceIterator citerString=targetList.begin(); citerString!=targetList.end(); citerString++)
        {
            m_TargetList.push_back(*citerString);
        }

        m_PerLine = m_TargetList.size()/m_TargetTotal;
    }
    else
    {
        m_PerLine = 0;
    }
}

CLogctListActiveCommandTargets::~CLogctListActiveCommandTargets()
{
}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctListActiveCommandTargets::Serialize(zOutStream& os) const
{
	CLogAnalysisMessage::Serialize(os);

	os << (*this);
}

