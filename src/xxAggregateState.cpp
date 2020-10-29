/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxAggregateState.cpp: implementation of the xxAggregateState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "xxAggregateState.h"
#include "AnalysisData.h"
#include "TimeSeriesData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor to open a stream attached the xxAggregateState object to write
// its data to file. We pass the filename and the flag "true" to the xxFile 
// base object to indicate that we only write data to the file. The unique
// integer, id, and aggregate type are obtained from CAnalysis and label the
// number and type of the aggregates so far created.

// The aggregate state file's name depends on whether it is XML-enabled or not,
// so we use two different constructor signatures to allow for this. If it
// is XML-enabled, we write out the standard processing instructions and
// wrapper tags <AggregateState></AggregateState> as the first and last elements 
// in the file. Because the aggregate state can write data until the simulation ends,
// we only write the end tag in the destructor.

#if EnableXMLProcesses == SimXMLEnabled

xxAggregateState::xxAggregateState(const zString runId, long id, const CAnalysisData* pAD, 
								 long sample) : xxState(xxBase::GetAAPrefix() + pAD->GetAnalysisType()+ToString(id)+"."+runId+".xml", true, pAD->GetStartTime(), runId),
												m_SamplePeriod(sample),
											    m_SamplesSerialized(0)
{

	// First write the xml and stylesheet PIs: note that the version and
	// stylesheet name are hardwired here. The stylesheet name incorporates the
	// aggregate type as found in the control data file to allow different
	// aggregate files to use different stylesheets

	const zString styleSheetName = pAD->GetAnalysisType() + "state.css";

	m_outStream << "<?xml version=\"1.0\" ?>" << zEndl;
	m_outStream << "<?xml-stylesheet type=\"text/css\" href=\"" << styleSheetName << "\" ?>" << zEndl;

	// now the top-level element with its runId attribute

	m_outStream << "<AggregateState runId = \"" << GetRunId() << "\">" << zEndl;

	// Write out the CAnalysisData so that the aggregate can be identified

	m_outStream << *pAD;

#elif EnableXMLProcesses == SimXMLDisabled

xxAggregateState::xxAggregateState(const zString runId, long id, const CAnalysisData* pAD, 
								 long sample) : xxState(xxBase::GetAAPrefix() + pAD->GetAnalysisType() + ToString(id) + "." + runId, true, pAD->GetStartTime(), runId),
												m_SamplePeriod(sample),
											    m_SamplesSerialized(0)
{

	m_outStream << *pAD;

#endif
     


	// Set the size of the vectors to hold the amount of data generated during
	// the simulation. This is based on the period during which the aggregate
	// will have its state updated. We assume a sampling period of 1 as this
	// allocates the maximum size. When the sampling period is not 1 the 
	// simulation time does not equal the index into the arrays.

	long totalSamples = (pAD->GetEndTime() - pAD->GetStartTime())/sample;

	m_vTimeSeries.reserve(totalSamples);

}

xxAggregateState::~xxAggregateState()
{
	// Write out the end tag for the XML-enabled output file.

#if EnableXMLProcesses == SimXMLEnabled
	m_outStream << "</AggregateState>" << zEndl;
#endif

	// Delete the vector of time series data objects

	for(TimeSeriesIterator iterTSD=m_vTimeSeries.begin(); iterTSD!=m_vTimeSeries.end(); iterTSD++)
	{
		delete *iterTSD;
	}

	m_vTimeSeries.clear();

}

// We serialize the time series data in chunks from the last time it was written
// out to the latest sample. Data is written only if there are CTimeSeries objects
// to be serialized. The number of samples already serialized is initialised to
// zero and then set to the total number of samples currently taken after each
// serialization period.

bool xxAggregateState::Serialize()
{
	if(m_IOFlag && m_vTimeSeries.size() > 0)	
	{
		for(unsigned long no=m_SamplesSerialized; no<m_vTimeSeries.size(); no++)
		{
			m_outStream << *m_vTimeSeries.at(no);
		}
		m_SamplesSerialized = m_vTimeSeries.size();
	}

	return true;
}

// Function to store the new CTimeSeries object preparatory to writing it
// to file. This is done so that we can add analysis of the whole time series at
// the end of the simulation if required.

void xxAggregateState::AddTimeSeriesData(CTimeSeriesData *pTSD)
{
	m_vTimeSeries.push_back(pTSD);
}

