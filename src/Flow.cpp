/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Flow.cpp: implementation of the CFlow class.
//
// This analysis object measures the viscosity of a fluid flowing under the
// action of an external force. The user specifies the direction of the flow,
// and its normal, and the polymer type whose flow is to be monitored. It
// then constructs profiles of the density and velocity of the fluid in 
// the direction of flow and normal to it to enable the viscosity to be
// measured.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "Flow.h"
#include "FlowData.h"
#include "SimState.h"
#include "ISimBox.h"

#include "SliceProfile.h"				// Use a slice profile for interface analysis
#include "aaRegionToType.h"				// Function objects needed to analyse CRegions
#include "aaScalarSingle.h"
#include "aaVectorSingle.h"
#include "aaScalarProfile.h"
#include "aaVectorProfile.h"

#include "ScalarObservable.h"			// We look at the mean density and momentum  
#include "VectorObservable.h"			// of the beads in the interface
#include "ScalarProfileObservable.h"
#include "VectorProfileObservable.h"

#include "TimeSeriesData.h"

// STL using declarations



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Note that we cannot set the types of the polymers and beads in the constructor
// because we don't have access to the maps at the point the CAnalysis-derived
// objects are created. Instead, a map of the head and tail bead types onto 
// polymer types is stored in the CAnalysisState object and a virtual function,
// CAnalysis::ConvertNames() is used to perform the mapping for any CAnalysis 
// objects that need to.

CFlow::CFlow(const zString runId, long sample, 
			 const CFlowData* const pAD) : CAnalysis(runId, sample, pAD),
										   m_PolymerMajor(pAD->m_PolymerMajor),
										   m_X(pAD->m_X),
										   m_Y(pAD->m_Y),
									  	   m_Z(pAD->m_Z),
										   m_FlowX(pAD->m_FlowX),
										   m_FlowY(pAD->m_FlowY),
										   m_FlowZ(pAD->m_FlowZ),
										   m_LaminarTotal(0),
										   m_pTSD(NULL),
										   m_MajorType(0), 
										   m_MajorHeadType(0), 
										   m_MajorTailType(0), 
										   m_pMajorSP(NULL),
										   m_pMajorDensity(NULL),
										   m_pMajorMomentum(NULL),
										   m_SliceTotal(0),
										   m_MajorMaxDensity(0.0),
										   m_MajorMinDensity(0.0),
										   m_MajorMaxId(0),
										   m_MajorMinId(0),
										   m_MajorMaxVelocity(0.0),
										   m_MajorMinVelocity(0.0)
{
}

CFlow::~CFlow()
{
	// Delete the profile objects created that held the regions

	if(m_pMajorSP)
		delete m_pMajorSP;

	if(m_pMajorDensity)
		delete m_pMajorDensity;

	if(m_pMajorMomentum)
		delete m_pMajorMomentum;
}

// Function to store the integer ids for beads, bonds and polymers given 
// the strings that were entered by the user in the control data file.

void CFlow::ConvertNames(const CAnalysisState& raState)
{
	m_MajorType		= raState.GetPolymerTypeFromName(m_PolymerMajor);
    m_MajorHeadType = raState.GetPolymerHeadType(m_MajorType);
    m_MajorTailType = raState.GetPolymerTailType(m_MajorType);
}

// Main function to measure and analyse all the observables relating to laminar flow
// of a polymer through the SimBox. It breaks the SimBox data (stored in CNTCells) 
// into slices and uses function objects to iterate over the collections measuring 
// the data of interest.
//
// The observables currently measured are:
//
//	Polymer density averaged over slices normal to flow
//  Polymer velocity    "            "      "
//
// We use the head bead to identify the location of a polymer. This might be 
// extended later to use head and tail.


void CFlow::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	long sliceNo;		// Counter used in several places below

	// Create CSliceProfiles to hold the data for the major polymer species.
	// We test for the first time they are created by looking at the major profile 
	// pointer. The first time we also create profiles and observables to 
	// hold the output from the interface analysis, but these are reused on subsequent
	// calls. However, the slice profiles have to be deleted and recreated each time
	// to get the new bead coordinates.

	// The data needed to initialise a slice profile are:
	//
	//	Number of slices covering the selected dimension of the SimBox
	//	Normal to the slices (only x, y, or z for now)

	// The number of slices for the profile is taken for now from the number of CNTCells
	// in the direction of the profile normal. This means that only profiles parallel
	// to the SimBox axes can be created. The data is taken from the ISimBox interface
	// which gives us access to the CNTCell structure in a way which prevents 
	// any changes to the CNTCells during the simulation.

	if(!m_pMajorSP)
	{
		if(m_X == 1)
		{
			m_SliceTotal = pISimBox->GetCNTXCellNo();
		}
		else if(m_Y == 1)
		{
			m_SliceTotal = pISimBox->GetCNTYCellNo();
		}
		else if(m_Z == 1)
		{
			m_SliceTotal = pISimBox->GetCNTZCellNo();
		}

		m_pMajorSP		= new CSliceProfile(m_SliceTotal, m_X, m_Y, m_Z, pISimBox);

		// Create the profile objects to hold the scalar, vector or tensor data returned
		// from the transform() algorithm.

		m_pMajorDensity  = new aaScalarProfile(m_SliceTotal);
		m_pMajorMomentum = new aaVectorProfile(m_SliceTotal); 

		// Create the observables needed to hold data on the interface.
		// We order them so that the polymer name labels the group of
		// observables for that polymer type.
		//
		// Id	Name
		// *********
		//  0  Major polymer average density			
		//  1  Major polymer average velocity
		//  2  Major bead density profiled across SimBox
		//  3  Major bead average momentum profiled across SimBox
		//

		CScalarObservable* pMajorDens	= new CScalarObservable(m_PolymerMajor + " Density",       rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CVectorObservable* pMajorMom	= new CVectorObservable(m_PolymerMajor + " Velocity",      rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

		m_vObservables.push_back(pMajorDens);
		m_vObservables.push_back(pMajorMom);

		CScalarProfileObservable* pMajorDensPro = new CScalarProfileObservable(m_PolymerMajor + " Density Profile",  
													rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_SliceTotal);
		
		CVectorProfileObservable* pMajorMomPro	= new CVectorProfileObservable(m_PolymerMajor + " Momentum Profile",  
													rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_SliceTotal);
				
		m_vObservables.push_back(pMajorDensPro);
		m_vObservables.push_back(pMajorMomPro);

	}
	else	// If not the first time delete the old slice profiles before creating new ones
	{
		delete m_pMajorSP;

		m_pMajorSP	= new CSliceProfile(m_SliceTotal, m_X, m_Y, m_Z, pISimBox);
	}

	// Analyse the slice profiles for the major polymer species and
	// store its density averaged over each slice in a container. A function 
	// object reads each CSlice and outputs a pointer to an aaScalar object 
	// holding the density for that slice. The average momentum of the beads
	// in the slice is calculated by aaMomentum().

	transform(m_pMajorSP->Begin(), m_pMajorSP->End(),
			  m_pMajorDensity->Begin(), aaDensity(m_MajorHeadType));

	// Now do the same for the bead momenta in the slices but use an aaVector object

	transform(m_pMajorSP->Begin(), m_pMajorSP->End(),
			  m_pMajorMomentum->Begin(), aaMomentum(m_MajorHeadType));

	// Calculate the mean values of the observables averaged over the whole
	// simulation box for now

	double meanMajorDens = 0.0;

	double meanMajorVel[3];

	for(short int i=0; i<3; i++)
	{
		meanMajorVel[i]	= 0.0;
	}

	m_MajorMaxDensity	= -1.0;
	m_MajorMinDensity	= 999.0;

	m_MajorMaxVelocity  = -1.0;
	m_MajorMinVelocity  = 999.0;

	for(sliceNo=0; sliceNo<m_SliceTotal; sliceNo++)
	{
		meanMajorDens   += m_pMajorDensity->GetData(sliceNo);

		meanMajorVel[0] += m_pMajorMomentum->GetData(sliceNo,0);
		meanMajorVel[1] += m_pMajorMomentum->GetData(sliceNo,1);
		meanMajorVel[2] += m_pMajorMomentum->GetData(sliceNo,2);

		if(m_pMajorDensity->GetData(sliceNo) > m_MajorMaxDensity)
		{
			m_MajorMaxDensity = m_pMajorDensity->GetData(sliceNo);
			m_MajorMaxId      = sliceNo;
		}

		if(m_pMajorDensity->GetData(sliceNo) < m_MajorMinDensity)
		{
			m_MajorMinDensity = m_pMajorDensity->GetData(sliceNo);
			m_MajorMinId      = sliceNo;
		}


		// Find the maximum and minimum values of the component of velocity
		// in the direction of flow

		if(m_FlowX == 1)
		{
			if(m_pMajorMomentum->GetData(sliceNo,0) > m_MajorMaxVelocity)
				m_MajorMaxVelocity = m_pMajorMomentum->GetData(sliceNo,0);
			else if(m_pMajorMomentum->GetData(sliceNo,0) < m_MajorMinVelocity)
				m_MajorMinVelocity = m_pMajorMomentum->GetData(sliceNo,0);
		}
		else if(m_FlowY == 1)
		{
			if(m_pMajorMomentum->GetData(sliceNo,1) > m_MajorMaxVelocity)
				m_MajorMaxVelocity = m_pMajorMomentum->GetData(sliceNo,1);
			else if(m_pMajorMomentum->GetData(sliceNo,1) < m_MajorMinVelocity)
				m_MajorMinVelocity = m_pMajorMomentum->GetData(sliceNo,1);
		}
		else if(m_FlowZ == 1)
		{
			if(m_pMajorMomentum->GetData(sliceNo,2) > m_MajorMaxVelocity)
				m_MajorMaxVelocity = m_pMajorMomentum->GetData(sliceNo,2);
			else if(m_pMajorMomentum->GetData(sliceNo,2) < m_MajorMinVelocity)
				m_MajorMinVelocity = m_pMajorMomentum->GetData(sliceNo,2);
		}
	}

	meanMajorDens  /= static_cast<double>(m_SliceTotal);

	meanMajorVel[0] /= static_cast<double>(m_SliceTotal);
	meanMajorVel[1] /= static_cast<double>(m_SliceTotal);
	meanMajorVel[2] /= static_cast<double>(m_SliceTotal);

	// Copy the data to the observables using classes derived from 
	// the template classes aaSingle<> and aaProfile<> that provide
	// containers for holding single and multiple pieces of data 
	// respectively.

	aaScalarSingle  meanDens1(meanMajorDens);

	aaVectorSingle  meanVel1(meanMajorVel[0], meanMajorVel[1], meanMajorVel[2]);

	// Write out the overall mean densities for checking

	meanDens1.AddData(m_vObservables.at(0));
	meanVel1.AddData(m_vObservables.at(1));

	// Write out the variation of the major bead density across the SimBox.
	// We pass a pointer to the vector of data so that it can be accessed
	// in the CObservable-derived class.

	m_pMajorDensity->AddData(m_vObservables.at(2));
	m_pMajorMomentum->AddData(m_vObservables.at(3));

	// Store the data in a TSD object and pass it to the aggregate state object

	long dataTotal = 3;

	m_pTSD = new CTimeSeriesData(dataTotal);

	m_pTSD->SetValue(0,  pISimBox->GetCurrentTime(), "Time");
	m_pTSD->SetValue(1,  m_MajorMaxVelocity,         "Max Vel");
	m_pTSD->SetValue(2,  m_MajorMinVelocity,	   "Min Vel");

	m_pState->AddTimeSeriesData(m_pTSD);


	// We have to delete the data created by the function objects for the scalar,
	// vector and tensor profiles to avoid a memory leak. This is because the transform()
	// algorithm can only insert data into a vector of a non-zero size. I don't want
	// to keep aaScalar, etc objects around and put new data into them so I have to
	// create and delete the objects as they are used. This means that their construction
	// and destruction should be as simple as possible.

	m_pMajorDensity->ClearData();
	m_pMajorMomentum->ClearData();
}

