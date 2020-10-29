/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogPolymerDisplay.cpp: implementation of the CLogPolymerDisplay class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogPolymerDisplay.h"


//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogPolymerDisplay& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>TogglePolymerDisplay</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "<PolymerName>" << rMsg.m_PolymerName  << "</PolymerName>" << zEndl;
	os << "<PolymerType>" << rMsg.m_PolymerType  << "</PolymerType>" << zEndl;
	if(rMsg.m_bDisplay)
	{
		os << "<Visible>true</Visible>" << zEndl;
	}
	else
	{
		os << "<Visible>false</Visible>" << zEndl;
	}
	os << "<Modified>" << rMsg.m_bModified  << "</Modified>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;


#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	if(rMsg.m_bDisplay)
	{
		os << "Polymers  " << rMsg.m_PolymerName << " (type " << rMsg.m_PolymerType << ") visible (command succeeded " << rMsg.m_bModified << ")";
	}
	else
	{
		os << "Polymers  " << rMsg.m_PolymerName << " (type " << rMsg.m_PolymerType << ") hidden (command succeeded " << rMsg.m_bModified << ")";
	}

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogPolymerDisplay::CLogPolymerDisplay(long time, const zString name, long type, 
										 bool bDisplay, bool bModified) : CLogInfoMessage(time), 
														 m_PolymerName(name), 
														 m_PolymerType(type), 
                                                         m_bDisplay(bDisplay),
                                                         m_bModified(bModified)
{

}

CLogPolymerDisplay::~CLogPolymerDisplay()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogPolymerDisplay::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << (*this);
}

