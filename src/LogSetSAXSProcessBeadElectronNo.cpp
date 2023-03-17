/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogBeadDisplayId.cpp: implementation of the CLogSetSAXSProcessBeadElectronNo class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogSetSAXSProcessBeadElectronNo.h"


//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogSetSAXSProcessBeadElectronNo& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>SetSAXSProcessBeadElectronNo</Name>" << zEndl;
	os << "<Text>" << zEndl;
    os << "<ProcessName>" << rMsg.m_ProcName   << "</ProcessName>" << zEndl;
    os << "<BeadName>"  << rMsg.m_BeadName  << "</BeadName>" << zEndl;
    os << "<BeadType>"  << rMsg.m_Type  << "</BeadType>" << zEndl;
    os << "<BeadTotal>"  << rMsg.m_Total  << "</BeadTotal>" << zEndl;
	os << "<Number>"    << rMsg.m_Eno   << "</Number>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;


#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "SAXS Process " << rMsg.m_ProcName << " has " << rMsg.m_Total << " beads " << rMsg.m_BeadName << " (type " << rMsg.m_Type << ") with electron number " << rMsg.m_Eno;

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogSetSAXSProcessBeadElectronNo::CLogSetSAXSProcessBeadElectronNo(long time, const zString procName, const zString beadName, long type, long total, double eno) : CLogInfoMessage(time),
                                                         m_ProcName(procName),
                                                         m_BeadName(beadName),
                                                         m_Type(type),
                                                         m_Total(total),
														 m_Eno(eno)
{

}

CLogSetSAXSProcessBeadElectronNo::~CLogSetSAXSProcessBeadElectronNo()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogSetSAXSProcessBeadElectronNo::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << (*this);
}

