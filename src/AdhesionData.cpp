/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// AdhesionData.cpp: implementation of the CAdhesionData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "AdhesionData.h"
#include "Adhesion.h" 

#include "SimMathFlags.h"

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

zString CAdhesionData::m_Type = "adhesion";

const zString CAdhesionData::GetType()
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
	CAnalysisData* Create() {return new CAdhesionData();}

	const zString id = CAdhesionData::GetType();

	const bool bRegistered = acfAnalysisFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAdhesionData::CAdhesionData() : CAnalysisData(false)
{

}

CAdhesionData::~CAdhesionData()
{

}

// Functions to read and write the data needed by a vesicle aggregate analysis.
// We call the base class's function first to get the more general data before
// that specific to each aggregate.

zOutStream& CAdhesionData::put(zOutStream& os) const
{
	// Write base class data first

	CAnalysisData::put(os);

	return os;
}

zInStream& CAdhesionData::get(zInStream& is)
{
	// Read base class data first

	CAnalysisData::get(is);
	
	return is;
}

CAnalysis* CAdhesionData::CreateAnalysis(const zString runId, long samplePeriod) 
{
	return new CAdhesion(runId, samplePeriod, this);
}

// Virtual function to return the type of analysis represented by the class.
// This has to access the static class variable for the type.

const zString CAdhesionData::GetAnalysisType() const
{
	return m_Type;
}
