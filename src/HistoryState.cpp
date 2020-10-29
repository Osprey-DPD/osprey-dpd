/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// HistoryState.cpp: implementation of the CHistoryState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "HistoryState.h"
#include "InputData.h"
#include "TimeSeriesData.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor to open a stream attached the CHistoryState object to write
// its data to file. We pass the filename and the flag "true" to the xxFile 
// base object to indicate that we only write data to the file. 

// The history state file's name depends on whether it is XML-enabled or not,
// so we use two different constructor signatures to allow for this. If it
// is XML-enabled, we write out the standard processing instructions and
// wrapper tags <HistoryState></HistoryState> as the first and last elements 
// in the file. Because the history state can write data until the simulation ends,
// we only write the end tag in the destructor.

#if EnableXMLProcesses == SimXMLEnabled

CHistoryState::CHistoryState(const CInputData& rData) : xxState(xxBase::GetHSPrefix() + rData.GetRunId() + ".xml", true, 0, rData.GetRunId()),
														m_SamplePeriod(rData.GetSamplePeriod()),
														m_LastPeriodSerialized(0)													
{

	// First write the xml and stylesheet PIs: note that the version and
	// stylesheet name are hardwired here.

	m_outStream << "<?xml version=\"1.0\" ?>" << zEndl;
	m_outStream << "<?xml-stylesheet type=\"text/css\" href=\"historystate.css\" ?>" << zEndl;

	// now the top-level element with its runId attribute

	m_outStream << "<HistoryState runId = \"" << GetRunId() << "\">" << zEndl;

#elif EnableXMLProcesses == SimXMLDisabled

CHistoryState::CHistoryState(const CInputData& rData) : xxState(xxBase::GetHSPrefix() + rData.GetRunId(), true, 0, rData.GetRunId()),
														m_SamplePeriod(rData.GetSamplePeriod()),
														m_LastPeriodSerialized(0)
{														

#endif

	// Set the size of the vectors to hold the amount of data generated during
	// the simulation. This is based on the total simulation time and the
	// sampling period. When the sampling period is not 1 the simulation time 
	// does not equal the index into the arrays.

	long totalTime = rData.GetTotalTime()/rData.GetSamplePeriod();

	m_vTimeSeries.reserve(totalTime);

	m_ProbeBeadXPos.reserve(totalTime);
	m_ProbeBeadYPos.reserve(totalTime);
	m_ProbeBeadZPos.reserve(totalTime);

}

CHistoryState::~CHistoryState()
{
	// Write out the end tag for the XML-enabled output file.

#if EnableXMLProcesses == SimXMLEnabled
	m_outStream << "</HistoryState>" << zEndl;
#endif

	// Delete the vector of time series data objects

	for(TimeSeriesIterator iterTSD=m_vTimeSeries.begin(); iterTSD!=m_vTimeSeries.end(); iterTSD++)
	{
		delete *iterTSD;
	}

	m_vTimeSeries.clear();

	// delete the probe bead's data

	m_ProbeBeadXPos.clear();
	m_ProbeBeadYPos.clear();
	m_ProbeBeadZPos.clear();

}

// We serialize the time series data in chunks from the last analysis period
// to the current time. Note that when the sampling period is not 1 we cannot
// use the simulation time as the index into the arrays.

bool CHistoryState::Serialize()
{
	if(m_IOFlag)	
	{
		for(long time=m_LastPeriodSerialized; time<((m_vTimeSeries.back())->GetTime())/m_SamplePeriod; time++)
		{
			m_outStream << *m_vTimeSeries.at(time);
		}
		m_LastPeriodSerialized = (m_vTimeSeries.back())->GetTime()/m_SamplePeriod;
	}

	return true;
}

// Function to add the probe bead trajectory data for the current timestep.

void CHistoryState::AddProbeBeadData(const double x, const double y, const double z)
{
	m_ProbeBeadXPos.push_back(x);
	m_ProbeBeadYPos.push_back(y);
	m_ProbeBeadZPos.push_back(z);

}

// Function to store the new CTimeSeries object preparatory to writing it
// to file. This is done so that we can add analysis of the whole time series at
// the end of the simulation if required.

void CHistoryState::AddTimeSeriesData(CTimeSeriesData *pTSD)
{
	m_vTimeSeries.push_back(pTSD);
}
