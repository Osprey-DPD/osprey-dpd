/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxProcessState.cpp: implementation of the xxProcessState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "xxProcessState.h"
#include "xxProcess.h"
#include "TimeSeriesData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor to open a stream attached the xxProcessState object to write
// its data to file. We pass the filename and the flag "true" to the xxFile 
// base object to indicate that we only write data to the file. The process 
// name and unique id are obtained from xxProcess before creating
// this object, and are used to label the number and type of the processes
//  so far created.

xxProcessState::xxProcessState(const zString fileName, long startTime, 
							   long endTime) : xxState(fileName, true, startTime),
											   m_Type(""),
											   m_Start(startTime), m_End(endTime),
											   m_SamplesSerialized(0)
{
	// Set the size of the vectors to hold the amount of data generated during
	// the simulation. This assumes that the process will have its state updated
	// every time-step but only be written to file every m_SamplePeriod steps.

	const long totalSamples = endTime - startTime;

	m_vTimeSeries.reserve(totalSamples);

}

// Constructor to use when passing the runId to the base class.

xxProcessState::xxProcessState(const zString fileName, long startTime, long endTime, 
							   const zString runId, const zString type) : xxState(fileName, true, startTime, runId), 
							   m_Type(type), m_Start(startTime), m_End(endTime),				  
							   m_SamplesSerialized(0)
{
	// Set the size of the vectors to hold the amount of data generated during
	// the simulation. This assumes that the process will have its state updated
	// every time-step but only be written to file every m_SamplePeriod steps.

	const long totalSamples = endTime - startTime;

	m_vTimeSeries.reserve(totalSamples);

}

xxProcessState::~xxProcessState()
{
	// Write the closing tags to the process file

#if EnableXMLProcesses == SimXMLEnabled
	putXMLEndTags();
#elif EnableXMLProcesses == SimXMLDisabled
	putASCIIEndTags();
#endif

	// delete the vector of time series data objects

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

bool xxProcessState::Serialize()
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

void xxProcessState::AddTimeSeriesData(CTimeSeriesData *pTSD)
{
	m_vTimeSeries.push_back(pTSD);
}

// Protected functions used to write data common to all xxProcess-derived classes 
// to the process analysis file in ASCII format.
// We write the "Process" keyword, the type of process involved and the
// start and end time here as they are common to all processes.
// Particular data for each object is serialized in its own put() and get() 
// functions.

zOutStream&  xxProcessState::putASCIIStartTags()
{
	m_outStream << "Process" << zEndl;
	m_outStream << "    Type         " << m_Type  << zEndl;
	m_outStream << "    Times        " << m_Start << "  " << m_End << zEndl;

	return m_outStream;
}

// There is currently no end tag required for ASCII output.

zOutStream&  xxProcessState::putASCIIEndTags()
{
	return m_outStream;
}

// Protected functions to write out the XML start and end tags for data 
// that is common to all derived classes. Each derived class then adds 
// its own particular data within these tags.

zOutStream&  xxProcessState::putXMLStartTags()
{
	// First write the xml and stylesheet PIs: note that the version and
	// stylesheet name are hardwired here. The stylesheet name incorporates the
	// aggregate type as found in the control data file to allow different
	// aggregate files to use different stylesheets

	const zString styleSheetName = m_Type + "process.css";

	m_outStream << "<?xml version=\"1.0\" ?>" << zEndl;
	m_outStream << "<?xml-stylesheet type=\"text/css\" href=\"" << styleSheetName << "\" ?>" << zEndl;

	// now the top-level element with its runId attribute

	m_outStream << "<Process runId = \"" << GetRunId() << "\">" << zEndl;
	m_outStream << "<Header>" << zEndl;
	m_outStream << "<Type>" << m_Type << "</Type>" << zEndl;
	m_outStream << "<Times><Start>" << m_Start << "</Start><End>" << m_End << "</End></Times>" << zEndl;

	return m_outStream;
}

zOutStream&  xxProcessState::putXMLEndTags()
{
	m_outStream << "</Process>" << zEndl;

	return m_outStream;
}


// ****************************************
// Functions to allow raw data to be written to a process state file.

// Only integers can be written for now; other data formats can be 
// added later by over-riding this function.

void xxProcessState::WriteRawEndline()
{
	m_outStream << zEndl;
}

void xxProcessState::WriteRawData(long x)
{
	m_outStream << x;
}

void xxProcessState::WriteRawData(double x)
{
	m_outStream << x;
}

void xxProcessState::WriteRawData(long x, long y)
{
	m_outStream << x << "  " << y << zEndl;
}

void xxProcessState::WriteRawData(double x, double y)
{
	m_outStream << x << "  " << y << zEndl;
}

