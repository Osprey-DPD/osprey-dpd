/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctListAllCommandTargetActivities.cpp: implementation of the CLogctListAllCommandTargetActivities class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctListAllCommandTargetActivities.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////
// Current target properties are:
//
// id - numeric identifier (converted to a string)
// type - class name of the target's type
// decLabel - label of a wrapping decorator instance

zOutStream& operator<<(zOutStream& os, const CLogctListAllCommandTargetActivities& rMsg)
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>ListAllCommandTargetActivities</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<TargetTotal>" << rMsg.m_TargetTotal << "</TargetTotal>" << zEndl;

    if(rMsg.m_TargetTotal > 0)
    {
        for(long i=0; i<rMsg.m_TargetTotal; i++)
        {
            cStringSequenceIterator citerDec=rMsg.m_DecList.at(i).begin();

            os << "<TargetId>"   << *citerDec++ << "</TargetId>";
            os << "<TargetType>" << *citerDec++ << "</TargetType>" << zEndl;

            while(citerDec!=rMsg.m_DecList.at(i).end())
            {
                os << "<DecoratorLabel>" << *citerDec << "</DecoratorLabel>" << zEndl;
                citerDec++;
            }
        }
    }
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
    if(rMsg.m_TargetTotal > 0)
    {
	    os << "The following command targets and their activities exist:" << zEndl;
        for(long i=0; i<rMsg.m_TargetTotal; i++)
        {
            for(cStringSequenceIterator citerDec=rMsg.m_DecList.at(i).begin(); citerDec!=rMsg.m_DecList.at(i).end(); citerDec++)
            {
                os << *citerDec << " ";
            }
            os << zEndl;
        }
    }
    else
    {
	    os << "No command targets exist" << zEndl;
    }

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store the set of target decorators that are to be written out as 
// a set of strings. If other properties are added later, the serialisation
// functions above must be modified to format the data appropriately.
// The number of targets is used to determine the number of rows in the 2d
// string container. Each target occupies one row and its data are written to
// a single line.  

// Note that we manually copy the strings in the container as it is passed in 
// by reference, and the original container will be out of scope when this
// message's operator<< function is called.

CLogctListAllCommandTargetActivities::CLogctListAllCommandTargetActivities(long time,  const zArray2dString& decList) : CLogAnalysisMessage(time),
                                      m_TargetTotal(decList.size())
{
    m_DecList.clear();

    if(m_TargetTotal > 0)
    {
        StringSequence targetList;

        for(long i=0; i<m_TargetTotal; i++)
        {
            targetList.clear();
            for(cStringSequenceIterator citerDec=decList.at(i).begin(); citerDec!=decList.at(i).end(); citerDec++)
            {
                targetList.push_back(*citerDec);
            }
            m_DecList.push_back(targetList);
        }
    }
}

CLogctListAllCommandTargetActivities::~CLogctListAllCommandTargetActivities()
{
}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctListAllCommandTargetActivities::Serialize(zOutStream& os) const
{
	CLogAnalysisMessage::Serialize(os);

	os << (*this);
}

