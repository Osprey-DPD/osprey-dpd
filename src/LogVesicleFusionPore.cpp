/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogVesicleFusionPore.cpp: implementation of the CLogVesicleFusionPore class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogVesicleFusionPore.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogVesicleFusionPore& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>VesicleFusionPore</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "Aggregates " << rMsg.m_Name1 << " (id " << rMsg.m_Type1 << ")";
	os << " and "       << rMsg.m_Name2 << " (id " << rMsg.m_Type2 << ")";
	os << " have a fusion pore of " << rMsg.m_PoreSize << " grid points";
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Aggregates " << rMsg.m_Name1 << " (id " << rMsg.m_Type1 << ")";
	os << " and "       << rMsg.m_Name2 << " (id " << rMsg.m_Type2 << ")";
	os << " have a fusion pore of " << rMsg.m_PoreSize << " grid points";

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogVesicleFusionPore::CLogVesicleFusionPore(long time, const zString name1, long type1,
											 const zString name2, long type2, 
											 long poreSize) : CLogEventMessage(time),
															  m_Name1(name1), m_Type1(type1),
															  m_Name2(name2), m_Type2(type2),
															  m_PoreSize(poreSize)
{

}

CLogVesicleFusionPore::~CLogVesicleFusionPore()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogVesicleFusionPore::Serialize(zOutStream& os) const
{
	CLogEventMessage::Serialize(os);

	os << *this;
}

