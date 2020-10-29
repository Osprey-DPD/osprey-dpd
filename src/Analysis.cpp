/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Analysis.cpp: implementation of the CAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "Analysis.h"
#include "AnalysisData.h"
#include "AnalysisTool.h"
#include "Observable.h"
#include "aaStressTensor1d.h"
#include "aaStressTensor2d.h"

//////////////////////////////////////////////////////////////////////
// Static member variable and function definitions
//////////////////////////////////////////////////////////////////////

long CAnalysis::m_AggregateTotal = 0;

long CAnalysis::GetAggregateTotal()
{
	return m_AggregateTotal;
}

// Static member function to reset the number of aggregates to zero.

void CAnalysis::ZeroAggregateTotal()
{
	m_AggregateTotal = 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Set the data in this class from the CAnalysisData-derived 
// object created from the control data. Note that although we treat it 
// as a pointer to a CAnalysisData object it is  actually a pointer to 
// one of the  derived classes.
//
// Each aggregate is given a unique id that depends on the order in which
// they are created. This is used to identify the output files and allow
// xxProcesses to find their associated aggregates.

CAnalysis::CAnalysis(const zString runId, long sample, 
					 const CAnalysisData* const pAD) : m_id(++m_AggregateTotal),
													   m_Type(pAD->GetAnalysisType()),
													   m_Start(pAD->GetStartTime()),
													   m_End(pAD->GetEndTime()),
													   m_SamplePeriod(sample)
{
	// All derived class objects inherit the ability to store their observables
	// in a vector in this base class. The derived classes create the observables
	// but they are stored and destroyed here.

	m_vObservables.clear();

	// CAnalysisTools available to aggregates are stored here

	m_vTools.clear();
	m_vActiveTools.clear();
	m_vActiveToolsElement.clear();

	// Create a aggregate state object to hold time series data for the 
	// newly-created aggregate. The name of the file attached to the object 
	// is constructed from the aggregate's type and an identifier counting 
	// the number of aggregates created.
	// We pass in the start and end times of the analysis period so that the
	// vector holding the CTimeSeries data objects can have space reserved.

	m_pState = new xxAggregateState(runId, m_AggregateTotal, pAD, sample);

}

CAnalysis::~CAnalysis()
{
	// Delete the observables if any were created

	if(!m_vObservables.empty())
	{
		for(ObservableIterator iterOb=m_vObservables.begin(); iterOb!=m_vObservables.end(); iterOb++)
		{
			delete *iterOb;
		}
		m_vObservables.clear();
	}

	// Delete the analysis tools if any were created

	if(!m_vTools.empty())
	{
		for(ToolIterator iterTool=m_vTools.begin(); iterTool!=m_vTools.end(); iterTool++)
		{
			delete *iterTool;
		}
		m_vTools.clear();
	}

	// delete the aggregate state object created in the constructor. Note that
	// it has to exist if a derived class of the pure virtual CAnalysis class 
	// has been instantiated, so we don't bother to check for a non-null pointer.

	delete m_pState;

}

// Public function to allow the monitor to know if the analysis object's UpdateState()
// function should be called. Each analysis object defines a range within which it
// collects data on the properties of its eponymous aggregate.

bool CAnalysis::TimeToSample(long simTime) const
{
	if(simTime > m_Start && simTime <= m_End)
		return true;
	else
		return false;
}

// Public function to allow the monitor to know if the analysis object should be
// serialized to file. The condition is that a complete sampling period should have 
// elapsed not just that the current time is within the aggregate's sampling time.
// Note that each aggregate has its own range for sampling data but they all use
// the same sampling and analysis periods.

bool CAnalysis::TimeToAnalyse(long simTime, long analysisPeriod) const
{
	if(simTime >= (m_Start + analysisPeriod) && simTime <= m_End)
		return true;
	else
		return false;
}

// Forwarding unction to the enclosed aggregate state object.

bool CAnalysis::Serialize() const
{
	return m_pState->Serialize();
}

// Overloaded function to create a one-dimensional stress tensor profile 
// for a derived class to use in calculating the stress tensor averaged 
// over a set of slices through the SimBox. 
// It is up to the derived class to delete the profile.

aaStressTensor* CAnalysis::StressTensorProfile(const ISimBox* const pISimBox,
												 CSliceProfile* const pProfile, 
												 long xn, long yn, long zn)
{
	aaStressTensor1d* pStress1d = new aaStressTensor1d(pISimBox, pProfile, xn, yn, zn);

	pStress1d->CalculateProfile();

	return pStress1d;
}
// Overloaded function to create a one-dimensional stress tensor profile 
// for a derived class to use in calculating the stress tensor averaged 
// over a set of slices through the SimBox. 
// It is up to the derived class to delete the profile.

aaStressTensor* CAnalysis::StressTensorProfile(const ISimBox* const pISimBox,
												 CSlice* const pSlice, 
												 long xn, long yn, long zn)
{
	aaStressTensor2d* pStress2d = new aaStressTensor2d(pISimBox, pSlice, xn, yn, zn);

	pStress2d->CalculateProfile();

	return pStress2d;
}
