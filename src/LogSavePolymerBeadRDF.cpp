/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogSavePolymerBeadRDF.cpp: implementation of the CLogSavePolymerBeadRDF class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogSavePolymerBeadRDF.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogSavePolymerBeadRDF& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>SavePolymerBeadRDF</Name>"  << zEndl;
	os << "<Text>" << zEndl;
    os << "<PolymerName>"          << rMsg.m_PolymerName          << "</PolymerName>"   << zEndl;
    os << "<BeadName>"             << rMsg.m_BeadName             << "</BeadName>"   << zEndl;
    os << "<StartTime>"            << rMsg.m_Start                 << "</StartTime>" << "<EndTime>" << rMsg.m_End << "</EndTime>" << zEndl;
    os << "<SamplePeriod>"         << rMsg.m_SamplePeriod          << "</SamplePeriod>"    << zEndl;
    os << "<TotalAnalysisPeriods>" << rMsg.m_TotalAnalysisPeriods  << "</TotalAnalysisPeriods>" << zEndl;
    os << "<Distance>"             << rMsg.m_RMax                  << "</Distance>" << zEndl;
    os << "<N>"                    << rMsg.m_TotalDataPoints   << "</N>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << " Saving polymer/bead RDF for polymer: " << rMsg.m_PolymerName << " bead: " << rMsg.m_BeadName<< zEndl;
    os << " sampling during " << rMsg.m_Start << " " << rMsg.m_End << " with sample period " << rMsg.m_SamplePeriod << zEndl;
    os << " (equivalent to " << rMsg.m_TotalAnalysisPeriods << " analysis periods)";
    os << " out to distance " << rMsg.m_RMax << " using " << rMsg.m_TotalDataPoints << " data points" << zEndl;


#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogSavePolymerBeadRDF::CLogSavePolymerBeadRDF(long time, long analysisPeriods, long dataPoints, 
											   long start, long end, long samplePeriod, 
											   double rmax, const zString polymerName, const zString beadName) : CLogInfoMessage(time),
												m_TotalAnalysisPeriods(analysisPeriods),
												m_TotalDataPoints(dataPoints),
												m_Start(start), m_End(end), m_SamplePeriod(samplePeriod), 
												m_RMax(rmax),
												m_PolymerName(polymerName), m_BeadName(beadName)
{

}

CLogSavePolymerBeadRDF::~CLogSavePolymerBeadRDF()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogSavePolymerBeadRDF::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << (*this);
}

