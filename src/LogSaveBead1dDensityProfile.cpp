/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogSaveBead1dDensityProfile.cpp: implementation of the CLogSaveBead1dDensityProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogSaveBead1dDensityProfile.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogSaveBead1dDensityProfile& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>SaveBead1dDensityProfile</Name>"  << zEndl;
	os << "<Text>" << zEndl;
	os << "<BeadName>" << rMsg.m_BeadName    << "</BeadName>" << zEndl;
	os << "<BeadType>" << rMsg.m_BeadType    << "</BeadType>" << zEndl;
	os << "<Start>"          << rMsg.m_Start        << "</Start>"        << zEndl;
	os << "<End>"            << rMsg.m_End          << "</End>"          << zEndl;
	os << "<SamplePeriod>"   << rMsg.m_SamplePeriod << "</SamplePeriod>" << zEndl;
	os << "<NX>" << rMsg.m_NX << "</NX>" << zEndl;
	os << "<NY>" << rMsg.m_NY << "</NY>" << zEndl;
	os << "<NZ>" << rMsg.m_NZ << "</NZ>" << zEndl;
	os << "<SliceTotal>"     << rMsg.m_SliceTotal   << "</SliceTotal>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << rMsg.m_BeadName << " bead (type " << rMsg.m_BeadType << ") density profile calculated with parameters" << zEndl;
    os << " " << rMsg.m_Start << " " << rMsg.m_End << " " << rMsg.m_SamplePeriod;
    os << " " << rMsg.m_NX << " " << rMsg.m_NY << " " << rMsg.m_NZ;
    os << " " << rMsg.m_SliceTotal;


#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogSaveBead1dDensityProfile::CLogSaveBead1dDensityProfile(long time, const zString name, long type,
                                                           long start, long end, long sample,
                                                           long nx, long ny, long nz, 
                                                           long slices) : CLogInfoMessage(time), 
														   m_BeadName(name), m_BeadType(type),
                                                           m_Start(start), m_End(end), m_SamplePeriod(sample),
                                                           m_NX(nx), m_NY(ny), m_NZ(nz), 
                                                           m_SliceTotal(slices)
{

}

CLogSaveBead1dDensityProfile::~CLogSaveBead1dDensityProfile()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogSaveBead1dDensityProfile::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << (*this);
}

