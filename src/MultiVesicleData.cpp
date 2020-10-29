/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// MultiVesicleData.cpp: implementation of the CMultiVesicleData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "MultiVesicleData.h"
#include "MultiVesicle.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for the aggregates that are
// investigated by analysis objects of this class. It is used to select the 
// appropriate analysis object when the Analysis/Type variable is read in the 
// control data file. The static member function GetType() is invoked by the
// CAnalysisObject to compare the type read from the control data file with each
// analysis data class so that it can create the appropriate object to hold the
// analysis data.

zString CMultiVesicleData::m_Type = "multivesicle";

const zString CMultiVesicleData::GetType()
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
	CAnalysisData* Create() {return new CMultiVesicleData();}

	const zString id = CMultiVesicleData::GetType();

	const bool bRegistered = acfAnalysisFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiVesicleData::CMultiVesicleData() : CAnalysisData(false),
										 m_Polymer(""),
										 m_Solvent("")
{

}

CMultiVesicleData::~CMultiVesicleData()
{

}

// Functions to read and write the data needed by a vesicle aggregate analysis.
// We call the base class's function first to get the more general data before
// that specific to each aggregate.

zOutStream& CMultiVesicleData::put(zOutStream& os) const
{

#if EnableXMLProcesses == SimXMLEnabled

	putXMLStartTags(os);
	os << "<Polymer>" << m_Polymer << "</Polymer>" << zEndl;
	os << "<Solvent>" << m_Solvent << "</Solvent>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLProcesses == SimXMLDisabled

	putASCIIStartTags(os);
	os << "    Polymer      " << m_Polymer << zEndl;
	os << "    Solvent      " << m_Solvent << zEndl;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& CMultiVesicleData::get(zInStream& is)
{
	// Read base class data first

	CAnalysisData::get(is);

	zString token;
	zString polymer;
	zString solvent;

	is >> token;
	if(!is.good() || token != "Polymer")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> polymer;
		if(!is.good() || polymer.length() == 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Solvent")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> solvent;
		if(!is.good() || solvent.length() == 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Data has been read successfully so store it in the member variables

	m_Polymer = polymer;
	m_Solvent = solvent;

	SetDataValid(true);

	return is;
}

CAnalysis* CMultiVesicleData::CreateAnalysis(const zString runId, long samplePeriod) 
{
	return new CMultiVesicle(runId, samplePeriod, this);
}

// Virtual function to return the type of analysis represented by the class.
// This has to access the static class variable for the type.

const zString CMultiVesicleData::GetAnalysisType() const
{
	return m_Type;
}

// Function to check that the data defining the aggregate analysis is valid.

bool CMultiVesicleData::ValidateData(const CInputData& riData) const
{
	return true;
}
