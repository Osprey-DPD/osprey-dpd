/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogSaveSAXS.cpp: implementation of the CLogSaveSAXS class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogSaveSAXS.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogSaveSAXS& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>SaveSAXS</Name>"  << zEndl;
    for(czBoolVectorIterator citer=rMsg.m_vIncludedBeads.begin(); citer!=rMsg.m_vIncludedBeads.end(); citer++)
    {
        os << "<Include>" << *citer << "<Include>" << zEndl;
    }
	os << "<Text>" << zEndl;
    os << "<StartTime>"            << rMsg.m_Start                 << "</StartTime>" << "<EndTime>" << rMsg.m_End << "</EndTime>" << zEndl;
    os << "<SamplePeriod>"         << rMsg.m_SamplePeriod          << "</SamplePeriod>"    << zEndl;
    os << "<TotalAnalysisPeriods>" << rMsg.m_TotalAnalysisPeriods  << "</TotalAnalysisPeriods>" << zEndl;
    os << "<N>"                    << rMsg.m_TotalDataPoints   << "</N>" << zEndl;
    os << "<QMin>"                 << rMsg.m_QMin   << "</QMin>" << zEndl;
    os << "<QMax>"                 << rMsg.m_QMax   << "</QMax>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
    os << " SAXS scattering function including beads: " << zEndl;
    for(czBoolVectorIterator citer=rMsg.m_vIncludedBeads.begin(); citer!=rMsg.m_vIncludedBeads.end(); citer++)
    {
        os << *citer << " ";
    }
    os << zEndl;
    os << " sampled during " << rMsg.m_Start << " " << rMsg.m_End << " with sample periods " << rMsg.m_SamplePeriod << zEndl;
    os << " (equivalent to " << rMsg.m_TotalAnalysisPeriods << " analysis periods)";
    os << " using " << rMsg.m_TotalDataPoints << " Q values in the range " << rMsg.m_QMin << " " << rMsg.m_QMax << zEndl;


#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogSaveSAXS::CLogSaveSAXS(long time, long analysisPeriods, long dataPoints, double qMin, double qMax,
											   long start, long end, long samplePeriod, 
											   zBoolVector vIncludedBeads) : CLogInfoMessage(time),
												m_TotalAnalysisPeriods(analysisPeriods),
												m_TotalDataPoints(dataPoints),
                                                m_QMin(qMin), m_QMax(qMax),
												m_Start(start), m_End(end), m_SamplePeriod(samplePeriod), 
												m_vIncludedBeads(vIncludedBeads)
{

}

CLogSaveSAXS::~CLogSaveSAXS()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogSaveSAXS::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << (*this);
}

