/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctListCommandTargetActivities.cpp: implementation of the CLogctListCommandTargetActivities class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctListCommandTargetActivities.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctListCommandTargetActivities& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>ListCommandTargetActivities</Name>" << zEndl;
	os << "<Text>" << zEndl;
    os << "<TargetLabel>" << rMsg.m_TargetLabel << "</TargetLabel>" << zEndl;

    for(cStringSequenceIterator citerString = rMsg.m_DecList.begin(); citerString != rMsg.m_DecList.end(); citerString++)
    {
        os << "<DecoratorLabel>" << *citerString << "</DecoratorLabel>" << zEndl;
    }

	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Command target " << rMsg.m_TargetLabel << " has the following activities: " << zEndl;

    for(cStringSequenceIterator citerString=rMsg.m_DecList.begin(); citerString!=rMsg.m_DecList.end(); citerString++)
    {
        os << *citerString << " ";
    }
    os << zEndl;

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store the labels of all decorators that wrap the specified command target.

// Note that we manually copy the strings in the container as it is passed in 
// by reference, and the original container will be out of scope when this
// message's operator<< function is called.

CLogctListCommandTargetActivities::CLogctListCommandTargetActivities(long time, const zString label, const StringSequence& decList) : CLogAnalysisMessage(time),
                                                                     m_TargetLabel(label)
{
    m_DecList.clear();

    if(decList.size() > 0)
    {
        for(cStringSequenceIterator citerString=decList.begin(); citerString!=decList.end(); citerString++)
        {
            m_DecList.push_back(*citerString);
        }
    }
}

CLogctListCommandTargetActivities::~CLogctListCommandTargetActivities()
{
}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctListCommandTargetActivities::Serialize(zOutStream& os) const
{
	CLogAnalysisMessage::Serialize(os);

	os << (*this);
}

