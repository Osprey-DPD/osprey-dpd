/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// AnalysisData.cpp: implementation of the CAnalysisData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimXMLFlags.h"
#include "AnalysisData.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// Function to write out the analysis data to file. We pass the
// stream output operator to the contained CAnalysisData-derived object using
// its put() function. This is because the << and >> operators cannot be
// treated polymporphically. The derived class is then responsible for calling
// the base class put and get functions.
//

zOutStream& operator<<(zOutStream& os, const CAnalysisData& AnData)
{
	return AnData.put(os);
}

// Function to read the analysis data from file. 

zInStream& operator>>(zInStream& is, CAnalysisData& AnData)
{
	return AnData.get(is);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnalysisData::CAnalysisData(bool bValid) : m_bDataValid(bValid), m_Start(0), m_End(0)
{

}

CAnalysisData::~CAnalysisData()
{

}

// Because the aggregate analysis classes can write their output as XML documents, 
// and they use the CAnalysisData::put() function to write identifying data at the
// front of the output file, the put() function is replaced by specialised functions 
// putASCIIStartTags(), putASCIIEndTags() and putXMLStartTags(), putXMLEndTags(). 
// Note that the get() function is NOT XML-enabled as it is only used for reading 
// the control data file, and not for protocol-driven simulations.

zOutStream& CAnalysisData::put(zOutStream& os) const
{
	return os;
}

// The "Analysis" and "Type" keywords and their values have already been 
// read in CInputDPDFile as they determine which CAnalysisData-derived object 
// to create, we fill in the start and end times for analysis here.

zInStream& CAnalysisData::get(zInStream& is)
{
// Read the analysis start and end times

	zString token;
	long start, end;

	is >> token;
	if(!is.good() || token != "Times")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> start >> end;
		if(!is.good() || start < 0 || end < start)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Data was read successfully so copy it into member variables

	SetStartTime(start);
	SetEndTime(end);

	return is;
}

// Public function to check that the stored data is valid.

bool CAnalysisData::IsDataValid() const
{
	return m_bDataValid;
}

// Protected function to allow sub-classes to set their own valid data flags
// as they read their data. An error on read results in this flag being set so
// that the read class can take action.

void CAnalysisData::SetDataValid(bool bValid)
{
	m_bDataValid = bValid;
}

// Functions to return the times at which analysis will begin and end during the simulation.

long CAnalysisData::GetStartTime() const
{
	return m_Start;
}

long CAnalysisData::GetEndTime() const
{
	return m_End;
}

void CAnalysisData::SetStartTime(long start)
{
	m_Start = start;
}

void CAnalysisData::SetEndTime(long end)
{
	m_End = end;
}

// Virtual function that should be overridden by any derived class that wants
// to validate its own data. Required input data are accessed via the reference
// to the CInitialState object. We don't make it const because derived classes
// need to store some data.

bool CAnalysisData::ValidateData(const CInputData& riData) const
{
	return true;
}


// Protected functions used to write the data common to all derived classes 
// to the aggregate analysis file in ASCII format.

zOutStream&  CAnalysisData::putASCIIStartTags(zOutStream &os) const
{
	os << "Analysis" << zEndl;
	os << "    Type         " << GetAnalysisType() << zEndl;
	os << "    Times        " << GetStartTime() << "  " << GetEndTime() << zEndl;

	return os;

}

// There is currently no end tag required for ASCII output.

zOutStream&  CAnalysisData::putASCIIEndTags(zOutStream &os) const
{
	return os;
}

// Protected functions to write out the XML start and end tags for data 
// that is common to all derived classes. Each derived class then adds 
// its own particular data within these tags.

zOutStream&  CAnalysisData::putXMLStartTags(zOutStream &os) const
{
	os << "<Analysis>" << zEndl;
	os << "<Type>" << GetAnalysisType() << "</Type>" << zEndl;
	os << "<Times><Start>" << GetStartTime() << "</Start><End>" << GetEndTime() << "</End></Times>" << zEndl;

	return os;

}

zOutStream&  CAnalysisData::putXMLEndTags(zOutStream &os) const
{
	os << "</Analysis>" << zEndl;

	return os;
}


