/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogSaveBeadDensityFluctuations.cpp: implementation of the CLogSaveBeadDensityFluctuations class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogSaveBeadDensityFluctuations.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogSaveBeadDensityFluctuations& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>SaveBeadDensityFluctuations</Name>"  << zEndl;
	os << "<Text>" << zEndl;
    os << "<BeadName>"            << rMsg.m_BeadName            << "</BeadName>"  << zEndl;
    os << "<BeadType>"            << rMsg.m_BeadType            << "</BeadType>"  << zEndl;
    os << "<TotalDataPoints>"     << rMsg.m_TotalDataPoints     << "</TotalDataPoints>" << zEndl;
    os << "<TotalDensityPeriods>" << rMsg.m_TotalDensityPeriods << "</TotalDensityPeriods>" << zEndl;
    os << "<DensityPeriod>"       << rMsg.m_DensityPeriod       << "</DensityPeriod>" << zEndl;
    os << "<StartTime>"           << rMsg.m_Start               << "</StartTime>" << "<EndTime>" << rMsg.m_End << "</EndTime>" << zEndl;
    os << "<XCellNo>"             << rMsg.m_XCellNo             << "</XCellNo>";
    os << "<YCellNo>"             << rMsg.m_YCellNo             << "</YCellNo>";
    os << "<ZCellNo>"             << rMsg.m_ZCellNo             << "</ZCellNo>" << zEndl;
    os << "<ConjugateFlag>"       << rMsg.m_bConjugate          << "</ConjugateFlag>" << zEndl;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << " Saving bead density fluctuations for bead " << rMsg.m_BeadName << " (type " << rMsg.m_BeadType << ")" << zEndl;
    os << " from: " << rMsg.m_Start << " to " << rMsg.m_End << zEndl;
    os << " with " << rMsg.m_TotalDataPoints << " data points ";
    os << " sampled over " << rMsg.m_TotalDensityPeriods << " density periods";
    os << " (of " << rMsg.m_DensityPeriod << " time steps each)" << zEndl;
    os << " for grid cell " << rMsg.m_XCellNo << " " << rMsg.m_YCellNo << " " << rMsg.m_ZCellNo;
    os << " and conjugate flag " << rMsg.m_bConjugate << zEndl;

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogSaveBeadDensityFluctuations::CLogSaveBeadDensityFluctuations(long time, const zString beadName, long beadType,
                                            long totalDataPoints, long totalDensityPeriods, 
                                            long densityPeriod, 
                                            long start, long end, 
                                            long xCell, long yCell, long zCell,
                                            bool bConjugate) : CLogInfoMessage(time), 
                                            m_BeadName(beadName), m_BeadType(beadType),
                                            m_TotalDataPoints(totalDataPoints),
                                            m_TotalDensityPeriods(totalDensityPeriods),
                                            m_DensityPeriod(densityPeriod),
                                            m_Start(start), m_End(end), 
                                            m_XCellNo(xCell), m_YCellNo(yCell), m_ZCellNo(zCell),
                                            m_bConjugate(bConjugate)
{

}

CLogSaveBeadDensityFluctuations::~CLogSaveBeadDensityFluctuations()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogSaveBeadDensityFluctuations::Serialize(zOutStream& os) const
{
	CLogInfoMessage::Serialize(os);

	os << (*this);
}

