/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// AnalysisObject.cpp: implementation of the CAnalysisObject class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AnalysisObject.h"
#include "AnalysisData.h"
#include "acfAnalysisFactory.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// Function to write out the analysis data to file. We pass the
// stream output operator to the contained CAnalysisData-derived object
// if the IsDataValid function shows that we have an object ready to
// hold the data.

zOutStream& operator<<(zOutStream& os, const CAnalysisObject& AnOb)
{
	if(AnOb.IsDataValid())
		os << *(AnOb.m_pAD);

	return os;
}

// Function to read the analysis data from file. We pass the
// stream input operator to its contained CAnalysisData-derived object
// if the internal pointer shows that we have an object. We also check that
// the data is read correctly and set the local flag appropriately.
//
// The CAnalysisObject::m_bpADValid flag performs two functions: on creation,
// it shows whether a valid CAnalysisData-derived object has been created, that is,
// whether the type identifier was recognised, and after having read its data, 
// it shows whether the data is valid.

zInStream& operator>>(zInStream& is, CAnalysisObject& AnOb)
{
	if(AnOb.IsDataValid())
	{
		is >> *(AnOb.m_pAD);

		AnOb.SetDataValid(AnOb.m_pAD->IsDataValid());
	}

	return is;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// The CAnalysisObject uses the "type" argument to determine which CAnalysisData
// object to create. The CAnalysisData-derived classes each have a static member 
// variable that corresponds to the entry in the control data file used to 
// identify the aggregate that will be analysed. This entry is accessed using 
// the static function GetType(). The acfAnalysisFactory object uses this type 
// identifier to call a function implemented by each CAnalysisData-derived class 
// to create an instance of the class.
//
// If the analysis type is not recognised a flag is set showing that data cannot be read.

CAnalysisObject::CAnalysisObject(const zString type) : m_bpADValid(true)
{
	m_pAD = acfAnalysisFactory::Instance()->Create(type);

	if(!m_pAD)
	{
		m_bpADValid = false;
	}	
}

CAnalysisObject::~CAnalysisObject()
{
	// Only delete the contained analysis data object if it exists

	if(m_pAD)
		delete m_pAD;
}

bool CAnalysisObject::IsDataValid() const
{
	return m_bpADValid;
}

// Function to set a flag showing whether the analysis data is valid.

void CAnalysisObject::SetDataValid(bool bValid)
{
	m_bpADValid = bValid;
}

// Pass the message to create a CAnalysis-derived object to the contained 
// CAnalysisData-derived object.

CAnalysis* CAnalysisObject::CreateAnalysis(const zString runId, long sample) const
{
	return m_pAD->CreateAnalysis(runId, sample);
}

// Functions to pass a request for the times to do analysis to the contained 
// CAnalysisData object

long CAnalysisObject::GetStartTime() const
{
	if(m_pAD)
		return m_pAD->GetStartTime();
	else
		return 0;
}

long CAnalysisObject::GetEndTime() const
{
	if(m_pAD)
		return m_pAD->GetEndTime();
	else 
		return 0;

}

// Function to pass the ValidateData message to the enclosed CAnalysisData object.

bool CAnalysisObject::ValidateData(const CInputData &riData) const
{
	return m_pAD->ValidateData(riData);
}
