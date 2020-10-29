/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// FlowData.cpp: implementation of the CFlowData class.
//
// Each type of aggregate or structure needs its own CAnalysisData-derived object, 
// called "CtypeData" where "type" is a mnemonic for the structure of interest. 
// These objects hold the data needed to initialise the actual analysis objects, 
// called "Ctype" and derived from CAnalysis. We decouple the two so that the 
// implementation of the file handling can be altered without having to change 
// the CAnalysis objects used in the CMonitor and CAnalysisState objects.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "FlowData.h"
#include "Flow.h"



//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for the aggregates that are
// investigated by analysis objects of this class. It is used to select the 
// appropriate analysis object when the Analysis-Type variable is read in the 
// control data file. The static member function GetType() is invoked by the
// CAnalysisObject to compare the type read from the control data file with each
// analysis data class so that it can create the appropriate object to hold the
// analysis data.

zString CFlowData::m_Type = "flow";

const zString CFlowData::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	CAnalysisData* Create() {return new CFlowData();}

	const zString id = CFlowData::GetType();

	const bool bRegistered = acfAnalysisFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We define default data for a flow analysis object in the constructor. 
// Because we cannot specify default polymer names, we set the IsDataValid flag 
// to false in the CAnalysisData base class to indicate that this object is not 
// ready to do analysis.
//
// The type of the object is specified as a const zString so that it can be used
// without fear of typos in several classes.

CFlowData::CFlowData() : CAnalysisData(false),
						 m_PolymerMajor(""), 
						 m_X(0), m_Y(1), m_Z(0),
						 m_FlowX(1), m_FlowY(0), m_FlowZ(0),
						 m_LaminarTotal(0)

{

}

CFlowData::~CFlowData()
{
}


// Virtual member functions to provide IO for this CAnalysisData-derived class.
// The order of the data members is important and they are case-sensitive. note
// also that we don't write this data out yet but if the default initial state
// (defined in CInputData) needs analysis objects in it then this function will 
// be called.

zOutStream& CFlowData::put(zOutStream& os) const
{
#if EnableXMLProcesses == SimXMLEnabled

	putXMLStartTags(os);
	os << "<Polymer>" << m_PolymerMajor << "</Polymer>" << zEndl;
	os << "<Normal><XN>" << m_X << "</XN><YN>" << m_Y << "</YN><ZN>" << m_Z << "</ZN></Normal>" << zEndl;
	os << "<Flow><XN>" << m_FlowX << "</XN><YN>" << m_FlowY << "</YN><ZN>" << m_FlowZ << "</ZN></Flow>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLProcesses == SimXMLDisabled

	putASCIIStartTags(os);
	os << "    Polymer      " << m_PolymerMajor << zEndl;
	os << "    Normal       " << m_X     << "  " << m_Y     << "  " << m_Z << zEndl;
	os << "    Flow         " << m_FlowX << "  " << m_FlowY << "  " << m_FlowZ << zEndl;
	putASCIIEndTags(os);

#endif

	return os;
}

// Function to read the analysis data from file. The keywords "Analysis"
// and "Type" and the value of type have already been read in by the CInputDPDFile 
// object so we only have to read the actual analysis data.

zInStream& CFlowData::get(zInStream& is)
{
	// Read base class data first

	CAnalysisData::get(is);

	zString token;
	zString polyMajor;
	long x, y, z;
	long flowx, flowy, flowz;

	is >> token;
	if(!is.good() || token != "Polymer")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> polyMajor;
		if(!is.good() || polyMajor.length() == 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Normal")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> x >> y >> z;

		// Force the normal to be parallel to an axis. 

		if(!is.good() || ((x+y) != 0 && z==1) || 
						 ((x+z) != 0 && y==1) ||
						 ((y+z) != 0 && x==1) )
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Flow")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> flowx >> flowy >> flowz;

		// Force the flow direction to be parallel to an axis. 

		if(!is.good() || ((flowx+flowy) != 0 && flowz==1) || 
						 ((flowx+flowz) != 0 && flowy==1) ||
						 ((flowy+flowz) != 0 && flowx==1) )
		{
			SetDataValid(false);
			return is;
		}
	}


	// Data has been read successfully so store it in the member variables

	SetDataValid(true);
	m_PolymerMajor			= polyMajor;
	m_X						= x;
	m_Y						= y;
	m_Z						= z;
	m_FlowX					= flowx;
	m_FlowY					= flowy;
	m_FlowZ					= flowz;

	return is;
}


CAnalysis* CFlowData::CreateAnalysis(const zString runId, long sample)
{
	return new CFlow(runId, sample, this);
}

// Virtual function to return the analysis type.

const zString CFlowData::GetAnalysisType() const
{
	return m_Type;
}

// Function to check that the data defining the aggregate analysis is valid.

bool CFlowData::ValidateData(const CInputData& riData) const
{
	return true;
}
