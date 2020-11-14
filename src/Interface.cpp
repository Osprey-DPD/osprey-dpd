/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Interface.cpp: implementation of the CInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "Interface.h"
#include "InterfaceData.h"
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

#include "aaStressTensor1d.h"			// Stress tensor profile object
#include "aaStressTensorPoint.h"		// Actual stress tensor components at a point



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Note that we cannot set the types of the polymers and beads in the constructor
// because we don't have access to the maps at the point the CAnalysis-derived
// objects are created. Instead, a map of the head and tail bead types onto 
// polymer types is stored in the CAnalysisState object and a virtual function,
// CAnalysis::ConvertNames() is used to perform the mapping for any CAnalysis 
// objects that need to.

CInterface::CInterface(const zString runId, long sample, 
					   const CInterfaceData* const pAD) : CAnalysis(runId, sample, pAD),
														  m_PolymerMajor(pAD->m_PolymerMajor),
														  m_PolymerMinor(pAD->m_PolymerMinor),
														  m_LowDensity(pAD->m_LowDensity),
														  m_HighDensity(pAD->m_HighDensity),
														  m_X(pAD->m_X),
														  m_Y(pAD->m_Y),
														  m_Z(pAD->m_Z),
														  m_pTSD(0),
														  m_MajorType(0), 
														  m_MinorType(0), 
														  m_MajorHeadType(0), 
														  m_MinorHeadType(0),
														  m_MajorTailType(0), 
														  m_MinorTailType(0),
														  m_pMajorSP(0),
														  m_pMinorSP(0),
														  m_pMajorDensity(0),
														  m_pMinorDensity(0),
														  m_pMajorMomentum(0),
														  m_pMinorMomentum(0),
														  m_SliceTotal(0),
														  m_MajorMaxDensity(0.0),
														  m_MajorMinDensity(0.0),
														  m_MajorMaxLimit(0.0),
														  m_MajorMinLimit(0.0),
														  m_MajorHighMean(0.0),
														  m_MajorLowMean(0.0),
														  m_MajorMidDensity(0.0),
														  m_MajorHighNo(0),
														  m_MajorLowNo(0),
														  m_MajorMaxId(0),
														  m_MajorMinId(0),
														  m_MajorMidPoint(0),
														  m_MinorMaxDensity(0.0),
														  m_MinorMinDensity(0.0),
														  m_MinorMaxLimit(0.0),
														  m_MinorMinLimit(0.0),
														  m_MinorHighMean(0.0),
														  m_MinorLowMean(0.0),
														  m_MinorMidDensity(0),
														  m_MinorHighNo(0),
														  m_MinorLowNo(0),
														  m_MinorMaxId(0),
														  m_MinorMinId(0),
														  m_MinorMidPoint(0),
														  m_SumMaxDensity(0.0),
														  m_SumMinDensity(0.0),
														  m_SumMaxId(0),
														  m_SumMinId(0),
														  m_StressGridTotal(0),
														  m_SurfaceTension(0.0),
													  	  m_pStress1d(0),
														  m_pTotalStress(0),
														  m_pMajorHHStress(0),
														  m_pMajorMinorHHStress(0),
														  m_pMinorHHStress(0)
{
	m_vBeadStressIndex.clear();		// We don't yet know how many bead types exist
}

CInterface::~CInterface()
{
	// Delete the profile objects created that held the regions

	if(m_pMajorSP)
		delete m_pMajorSP;

	if(m_pMinorSP)
		delete m_pMinorSP;

	if(m_pMajorDensity)
		delete m_pMajorDensity;

	if(m_pMinorDensity)
		delete m_pMinorDensity;

	if(m_pMajorMomentum)
		delete m_pMajorMomentum;

	if(m_pMinorMomentum)
		delete m_pMinorMomentum;

	if(m_pStress1d)
	{
		delete m_pStress1d;
		m_pStress1d = 0;
	}

	if(m_pTotalStress)
	{
		delete m_pTotalStress;
		m_pTotalStress = 0;
	}

	if(m_pMajorHHStress)
	{
		delete m_pMajorHHStress;
		m_pMajorHHStress = 0;
	}

	if(m_pMajorMinorHHStress)
	{
		delete m_pMajorMinorHHStress;
		m_pMajorMinorHHStress = 0;
	}
	
	if(m_pMinorHHStress)
	{
		delete m_pMinorHHStress;
		m_pMinorHHStress = 0;
	}
}

// Function to store the integer ids for beads, bonds and polymers given 
// the strings that were entered by the user in the control data file.

void CInterface::ConvertNames(const CAnalysisState& raState)
{
	m_MajorType		= raState.GetPolymerTypeFromName(m_PolymerMajor);
    m_MajorHeadType = raState.GetPolymerHeadType(m_MajorType);
    m_MajorTailType = raState.GetPolymerTailType(m_MajorType);

	m_MinorType		= raState.GetPolymerTypeFromName(m_PolymerMinor);
	m_MinorHeadType = raState.GetPolymerHeadType(m_MinorType);
	m_MinorTailType = raState.GetPolymerTailType(m_MinorType);
}

// Main function to measure and analyse all the observables relating to an interface.
// It breaks the SimBox data (stored in CNTCells) into slices, row or cells and 
// uses function objects to iterate over the collections measuring the data of interest.
//
// The observables currently measured are:
//
//	Major polmyer species density averaged over slices
//  Minor polmyer    "       "       "
//  Location of interface determined by gradient of polymer density
//  Width of interface determined by user-defined cut-offs in fractional density
//
//


void CInterface::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	long sliceNo;		// Counter used in several places below

	// Create CSliceProfiles to hold the data for the major and minor polymer species.
	// We test for the first time they are created by looking at the major profile 
	// pointer: because the major and minor profiles are created together we don't have
	// to check both. The first time we also create profiles and observables to 
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
			m_SliceTotal	  = pISimBox->GetCNTXCellNo();
			m_StressGridTotal = rSimState.GetGridXCellNo();
		}
		else if(m_Y == 1)
		{
			m_SliceTotal	  = pISimBox->GetCNTYCellNo();
			m_StressGridTotal = rSimState.GetGridYCellNo();
		}
		else if(m_Z == 1)
		{
			m_SliceTotal	  = pISimBox->GetCNTZCellNo();
			m_StressGridTotal = rSimState.GetGridZCellNo();
		}

		m_pMajorSP		= new CSliceProfile(m_SliceTotal, m_X, m_Y, m_Z, pISimBox);
		m_pMinorSP		= new CSliceProfile(m_SliceTotal, m_X, m_Y, m_Z, pISimBox);

		// Create the profile objects to hold the scalar, vector or tensor data returned
		// from the transform() algorithm.

		m_pMajorDensity  = new aaScalarProfile(m_SliceTotal);
		m_pMinorDensity  = new aaScalarProfile(m_SliceTotal);
		m_pMajorMomentum = new aaVectorProfile(m_SliceTotal); 
		m_pMinorMomentum = new aaVectorProfile(m_SliceTotal);

		// The size of the stress tensor profiles is set from the size of
		// analysis grid in the appropriate direction.

		m_pTotalStress			= new aaScalarProfile(m_StressGridTotal);
		m_pMajorHHStress		= new aaScalarProfile(m_StressGridTotal);
		m_pMajorMinorHHStress	= new aaScalarProfile(m_StressGridTotal);
		m_pMinorHHStress		= new aaScalarProfile(m_StressGridTotal);

		// Store the bead indexes needed to access the stress tensor components 
		// created for each bead-bead interaction. These are stored in the order
		// in which the contributions are calculated. Assume for now that there
		// are two polymer types: Oil and solvent.
		//
		//	Solvent-Solvent, Oil-Solvent, Oil-Oil
		//
		// But the number of bead types, and the types of each can be in an
		// arbitrary order that depends on how many bead types have been created.
		// Hence the actual indexes stored may not be monotonically ordered. We have to
		// create vectors with enough elements to hold all possible bead type pairs.
		// If further contributions are added the appropriate indexes must be added
		// here. We order them by choosing the larger of the two indices as the first
		// when accessing the off-diagonal elements. This corresponds to the order in 
		// which they are stored in CMonitor::AddBeadStress(). 

		// If the user has set both polymer types in the interface the same only
		// one bead type will exist. To allow the code to still access the data as if
		// there were an interface between two bead types, we fake the data to be all
		// the same as the single bead type.

		if(rSimState.GetBeadTypeTotal() >= 2)
		{
			m_vBeadStressIndex.resize(rSimState.GetBeadTypeTotal()*rSimState.GetBeadTypeTotal());
			m_vBeadStressIndex.at(0) = m_MajorHeadType*rSimState.GetBeadTypeTotal() + m_MajorHeadType;
			m_vBeadStressIndex.at(1) = m_MajorHeadType*rSimState.GetBeadTypeTotal() + m_MinorHeadType;
			m_vBeadStressIndex.at(2) = m_MinorHeadType*rSimState.GetBeadTypeTotal() + m_MinorHeadType;
		}
		else
		{
			m_vBeadStressIndex.resize(3);
			m_vBeadStressIndex.at(0) = m_MajorHeadType*rSimState.GetBeadTypeTotal() + m_MajorHeadType;
			m_vBeadStressIndex.at(1) = m_vBeadStressIndex.at(0);
			m_vBeadStressIndex.at(2) = m_vBeadStressIndex.at(0);
		}

		// Create the observables needed to hold data on the interface.
		// We order them so that the polymer name labels the group of
		// observables for that polymer type.
		//
		// Id	Name
		// *********
		//  0	Major polymer density			
		//  1	Major polymer high mean density			
		//  2	Major polymer low mean density
		//  3   Major polymer interface midpoint
		//  4	Major polymer average velocity
		//
		//  5	Minor polymer density			
		//  6	Minor polymer high mean density			
		//  7	Minor polymer low mean density
		//  8   Minor polymer interface midpoint
		//  9	Minor polymer average velocity
		//
		//  10  Major bead density profiled across SimBox
		//  11  Minor bead density profiled across SimBox
		//  12  Major bead average momentum profiled across SimBox
		//  13  Minor bead average momentum profiled across SimBox
		//
		//  14  Maximum of summed densities
		//  15  Location of summed density maximum
		//  16  Minimum of summed densities
		//  17  Location of summed density minimum

		// Now the stress tensor observables. Note that these have a size set by
		// the number of slices in the whole SimBox. Also note that the user can specify any polymer type as the "major" component, 
		// so we cannot label them by polymer names only by "major" and "minor"
		//
		//  18  Surface tension of interface
		//	19	Total stress profile through interface
		//	20	Major-Major component stress profile through interface
		//	21	Major-Minor component stress profile through interface
		//	22	Minor-Minor component stress profile through interface

		CScalarObservable* pMajorDens	= new CScalarObservable(m_PolymerMajor + " Density",       rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pMajorHigh	= new CScalarObservable(m_PolymerMajor + " High Density",  rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pMajorLow	= new CScalarObservable(m_PolymerMajor + " Low Density",   rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pMajorMid	= new CScalarObservable(m_PolymerMajor + " Midpoint",      rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CVectorObservable* pMajorMom	= new CVectorObservable(m_PolymerMajor + " Velocity",      rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

		CScalarObservable* pMinorDens	= new CScalarObservable(m_PolymerMinor + " Density",       rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pMinorHigh	= new CScalarObservable(m_PolymerMinor + " High Density",  rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pMinorLow	= new CScalarObservable(m_PolymerMinor + " Low Density",   rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pMinorMid	= new CScalarObservable(m_PolymerMinor + " Midpoint",      rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CVectorObservable* pMinorMom	= new CVectorObservable(m_PolymerMinor + " Velocity",      rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

		m_vObservables.push_back(pMajorDens);
		m_vObservables.push_back(pMajorHigh);
		m_vObservables.push_back(pMajorLow);
		m_vObservables.push_back(pMajorMid);
		m_vObservables.push_back(pMajorMom);
		m_vObservables.push_back(pMinorDens);
		m_vObservables.push_back(pMinorHigh);
		m_vObservables.push_back(pMinorLow);
		m_vObservables.push_back(pMinorMid);
		m_vObservables.push_back(pMinorMom);



		CScalarProfileObservable* pMajorDensPro = new CScalarProfileObservable(m_PolymerMajor + " Density Profile",  
													rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_SliceTotal);

		CScalarProfileObservable* pMinorDensPro = new CScalarProfileObservable(m_PolymerMinor + " Density Profile",  
													rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_SliceTotal);
		
		CVectorProfileObservable* pMajorMomPro	= new CVectorProfileObservable(m_PolymerMajor + " Momentum Profile",  
													rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_SliceTotal);
		
		CVectorProfileObservable* pMinorMomPro	= new CVectorProfileObservable(m_PolymerMinor + " Momentum Profile",  
													rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_SliceTotal);
		
		m_vObservables.push_back(pMajorDensPro);
		m_vObservables.push_back(pMinorDensPro);
		m_vObservables.push_back(pMajorMomPro);
		m_vObservables.push_back(pMinorMomPro);

		CScalarObservable* pSumHigh		= new CScalarObservable("Summed High Density",   rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pSumHighLoc	= new CScalarObservable("Summed High Location",  rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pSumLow		= new CScalarObservable("Summed Low Density",    rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pSumLowLoc	= new CScalarObservable("Summed Low Location",   rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

		m_vObservables.push_back(pSumHigh);
		m_vObservables.push_back(pSumHighLoc);
		m_vObservables.push_back(pSumLow);
		m_vObservables.push_back(pSumLowLoc);


		// Stress-related observables

		CScalarObservable* pSurfaceTension	= new CScalarObservable("Interfacial Tension",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		
	    CScalarProfileObservable* pTotalStressPro = new CScalarProfileObservable("Total Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

	    CScalarProfileObservable* pMajorHHStressPro = new CScalarProfileObservable(m_PolymerMajor + "-" + m_PolymerMajor + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);

	    CScalarProfileObservable* pMajorMinorHHStressPro = new CScalarProfileObservable(m_PolymerMajor + "-" + m_PolymerMinor + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);
												
	    CScalarProfileObservable* pMinorHHStressPro = new CScalarProfileObservable(m_PolymerMinor + "-" + m_PolymerMinor + " Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_StressGridTotal);


		m_vObservables.push_back(pSurfaceTension);
		m_vObservables.push_back(pTotalStressPro);
		m_vObservables.push_back(pMajorHHStressPro);
		m_vObservables.push_back(pMajorMinorHHStressPro);
		m_vObservables.push_back(pMinorHHStressPro);
	}
	else	// If not the first time delete the old slice profiles before creating new ones
	{
		delete m_pMajorSP;
		delete m_pMinorSP;

		m_pMajorSP		= new CSliceProfile(m_SliceTotal, m_X, m_Y, m_Z, pISimBox);
		m_pMinorSP		= new CSliceProfile(m_SliceTotal, m_X, m_Y, m_Z, pISimBox);
	}

	// Analyse the slice profiles for the major and minor polymer species and
	// store their densities averaged over each slice in a container. A function 
	// object reads each CSlice and outputs a pointer to an aaScalar object 
	// holding the density for that slice. The average momentum of the beads
	// in the slice is calculated by aaMomentum().

	transform(m_pMajorSP->Begin(), m_pMajorSP->End(),
			  m_pMajorDensity->Begin(), aaDensity(m_MajorHeadType));
	transform(m_pMinorSP->Begin(), m_pMinorSP->End(),
			  m_pMinorDensity->Begin(), aaDensity(m_MinorHeadType));

	// Now do the same for the bead momenta in the slices but use an aaVector object

	transform(m_pMajorSP->Begin(), m_pMajorSP->End(),
			  m_pMajorMomentum->Begin(), aaMomentum(m_MajorHeadType));
	transform(m_pMinorSP->Begin(), m_pMinorSP->End(),
		      m_pMinorMomentum->Begin(), aaMomentum(m_MinorHeadType));

	// Calculate the mean values of the observables averaged over the whole
	// simulation box for now

	double meanMajorDens = 0.0;
	double meanMinorDens = 0.0;

	double meanMajorVel[3];
	double meanMinorVel[3];

	for(short int i=0; i<3; i++)
	{
		meanMajorVel[i]	= 0.0;
		meanMinorVel[i]	= 0.0;
	}

	double sumDens = 0.0;

	// We have the densities of the beads in the major and minor polymer species,
	// so we can calculate the solubility of each in the other. First find the 
	// average densities for the species over the whole simulation box as a check,
	// and find the max and min

	m_MajorMaxDensity	= -1.0;
	m_MajorMinDensity	= 999.0;
	m_MinorMaxDensity	= -1.0;
	m_MinorMinDensity	= 999.0;
	m_SumMaxDensity		= -1.0;
	m_SumMinDensity		= 999.0;

	for(sliceNo=0; sliceNo<m_SliceTotal; sliceNo++)
	{
		meanMajorDens   += m_pMajorDensity->GetData(sliceNo);
		meanMinorDens   += m_pMinorDensity->GetData(sliceNo);

		meanMajorVel[0] += m_pMajorMomentum->GetData(sliceNo,0);
		meanMajorVel[1] += m_pMajorMomentum->GetData(sliceNo,1);
		meanMajorVel[2] += m_pMajorMomentum->GetData(sliceNo,2);

		meanMinorVel[0] += m_pMinorMomentum->GetData(sliceNo,0);
		meanMinorVel[1] += m_pMinorMomentum->GetData(sliceNo,1);
		meanMinorVel[2] += m_pMinorMomentum->GetData(sliceNo,2);

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

		if(m_pMinorDensity->GetData(sliceNo) > m_MinorMaxDensity)
		{
			m_MinorMaxDensity = m_pMinorDensity->GetData(sliceNo);
			m_MinorMaxId      = sliceNo;
		}

		if(m_pMinorDensity->GetData(sliceNo) < m_MinorMinDensity)
		{
			m_MinorMinDensity = m_pMinorDensity->GetData(sliceNo);
			m_MinorMinId      = sliceNo;
		}

		sumDens = m_pMajorDensity->GetData(sliceNo) + m_pMinorDensity->GetData(sliceNo);

		if(sumDens > m_SumMaxDensity)
		{
			m_SumMaxDensity = sumDens;
			m_SumMaxId      = sliceNo;
		}

		if(sumDens < m_SumMinDensity)
		{
			m_SumMinDensity = sumDens;
			m_SumMinId      = sliceNo;
		}

	}

	meanMajorDens  /= static_cast<double>(m_SliceTotal);
	meanMinorDens  /= static_cast<double>(m_SliceTotal);

	meanMajorVel[0] /= static_cast<double>(m_SliceTotal);
	meanMajorVel[1] /= static_cast<double>(m_SliceTotal);
	meanMajorVel[2] /= static_cast<double>(m_SliceTotal);

	meanMinorVel[0] /= static_cast<double>(m_SliceTotal);
	meanMinorVel[1] /= static_cast<double>(m_SliceTotal);
	meanMinorVel[2] /= static_cast<double>(m_SliceTotal);

	m_MajorMidDensity = 0.5*(m_MajorMaxDensity + m_MajorMinDensity);
	m_MinorMidDensity = 0.5*(m_MinorMaxDensity + m_MinorMinDensity);


	// Copy the data to the observables using classes derived from 
	// the template classes aaSingle<> and aaProfile<> that provide
	// containers for holding single and multiple pieces of data 
	// respectively.

	aaScalarSingle  meanDens1(meanMajorDens);
	aaScalarSingle  meanDens2(meanMinorDens);

	aaVectorSingle  meanVel1(meanMajorVel[0], meanMajorVel[1], meanMajorVel[2]);
	aaVectorSingle  meanVel2(meanMinorVel[0], meanMinorVel[1], meanMinorVel[2]);


	// Find the thresholds for high and low density. Note that the prefactors,
	// m_HighDensity and m_LowDensity are dimensionless quantities entered by the
	// user as the thresholds for defining "high" and "low" densities.

	m_MajorMaxLimit = m_HighDensity*(m_MajorMaxDensity-m_MajorMinDensity) + m_MajorMinDensity;
	m_MajorMinLimit = m_LowDensity*(m_MajorMaxDensity-m_MajorMinDensity) + m_MajorMinDensity;

	m_MinorMaxLimit = m_HighDensity*(m_MinorMaxDensity-m_MinorMinDensity) + m_MinorMinDensity;
	m_MinorMinLimit = m_LowDensity*(m_MinorMaxDensity-m_MinorMinDensity) + m_MinorMinDensity;

	m_MajorHighMean   = 0.0;
	m_MajorLowMean    = 0.0;
	m_MajorHighNo     = 0;
	m_MajorLowNo      = 0;
	m_MajorMidPoint   = 0;

	m_MinorHighMean   = 0.0;
	m_MinorLowMean    = 0.0;
	m_MinorHighNo     = 0;
	m_MinorLowNo      = 0;
	m_MinorMidPoint   = 0;

	double currentDens1 = 0.0;
	double currentDens2 = 0.0;

	// Calculate the mean value of the densities above the higher threshold and below
	// the lower threshold and locate the interface

	for(sliceNo=0; sliceNo<m_SliceTotal; sliceNo++)
	{
		currentDens1 = m_pMajorDensity->GetData(sliceNo);
		currentDens2 = m_pMinorDensity->GetData(sliceNo);

		if(currentDens1 > m_MajorMaxLimit)
		{
			m_MajorHighNo++;
			m_MajorHighMean += currentDens1;
		}
		else if(currentDens1 < m_MajorMinLimit)
		{
			m_MajorLowNo++;
			m_MajorLowMean += currentDens1;
		}

		if(currentDens2 > m_MinorMaxLimit)
		{
			m_MinorHighNo++;
			m_MinorHighMean += currentDens2;
		}
		else if(currentDens2 < m_MinorMinLimit)
		{
			m_MinorLowNo++;
			m_MinorLowMean += currentDens2;
		}

		// Find the interface location from the point where the density crosses the
		// midpoint of its range. We have to check whether the maximum density slice
		// occurs before or after the minimum

		if(m_MajorMaxId < m_MajorMinId)
		{
			if( (sliceNo >= m_MajorMaxId && sliceNo < m_MajorMinId) &&
			    (currentDens1 > m_MajorMidDensity) &&
			    (m_pMajorDensity->GetData(sliceNo+1) < m_MajorMidDensity))
			{
				m_MajorMidPoint = sliceNo;
			}
		}
		else
		{
			if( (sliceNo >= m_MajorMinId && sliceNo < m_MajorMaxId) &&
			    (currentDens1 < m_MajorMidDensity) &&
			    (m_pMajorDensity->GetData(sliceNo+1) > m_MajorMidDensity))
			{
				m_MajorMidPoint = sliceNo;
			}
		}

		if(m_MinorMaxId < m_MinorMinId)
		{
			if( (sliceNo >= m_MinorMaxId && sliceNo < m_MinorMinId) &&
			    (currentDens2 > m_MinorMidDensity) &&
			    (m_pMinorDensity->GetData(sliceNo+1) < m_MinorMidDensity))
			{
				m_MinorMidPoint = sliceNo;
			}
		}
		else
		{
			if( (sliceNo >= m_MinorMinId && sliceNo < m_MinorMaxId) &&
			    (currentDens2 < m_MinorMidDensity) &&
			    (m_pMinorDensity->GetData(sliceNo+1) > m_MinorMidDensity))
			{
				m_MinorMidPoint = sliceNo;
			}
		}
	}

	// It is possible for the user to set thresholds of (0,1) so we have to check
	// that there are some slices in the mean value calculations

	if(m_MajorHighNo > 0)
		m_MajorHighMean /= static_cast<double>(m_MajorHighNo);
	if(m_MajorLowNo > 0)
		m_MajorLowMean  /= static_cast<double>(m_MajorLowNo);

	if(m_MinorHighNo > 0)
		m_MinorHighMean /= static_cast<double>(m_MinorHighNo);
	if(m_MinorLowNo > 0)
		m_MinorLowMean  /= static_cast<double>(m_MinorLowNo);

	// Add the mean high and low densities for both species to observables 

	aaScalarSingle  highMeanDens1(m_MajorHighMean);
	aaScalarSingle  lowMeanDens1(m_MajorLowMean);
	aaScalarSingle  midPointDens1(static_cast<double>(m_MajorMidPoint));

	aaScalarSingle  highMeanDens2(m_MinorHighMean);
	aaScalarSingle  lowMeanDens2(m_MinorLowMean);
	aaScalarSingle  midPointDens2(static_cast<double>(m_MinorMidPoint));

	// Write out the overall mean densities for checking

	long noset = m_vObservables.size();

	     meanDens1.AddData(m_vObservables.at(0));
	 highMeanDens1.AddData(m_vObservables.at(1));
	  lowMeanDens1.AddData(m_vObservables.at(2));
	 midPointDens1.AddData(m_vObservables.at(3));
	      meanVel1.AddData(m_vObservables.at(4));
	     meanDens2.AddData(m_vObservables.at(5));
	 highMeanDens2.AddData(m_vObservables.at(6));
	  lowMeanDens2.AddData(m_vObservables.at(7));
	 midPointDens2.AddData(m_vObservables.at(8));
	      meanVel2.AddData(m_vObservables.at(9));

	// Write out the variation of the major bead density across the SimBox.
	// We pass a pointer to the vector of data so that it can be accessed
	// in the CObservable-derived class.

	m_pMajorDensity->AddData(m_vObservables.at(10));
	m_pMinorDensity->AddData(m_vObservables.at(11));
	m_pMajorMomentum->AddData(m_vObservables.at(12));
	m_pMinorMomentum->AddData(m_vObservables.at(13));

	// Summed density observables: just the magnitudes and locations of the
	// highest and lowest summed density

	aaScalarSingle  highSumDens(m_SumMaxDensity);
	aaScalarSingle  highSumLoc(static_cast<double>(m_SumMaxId));
	aaScalarSingle  lowSumDens(m_SumMinDensity);
	aaScalarSingle  lowSumLoc(static_cast<double>(m_SumMinId));

	 highSumDens.AddData(m_vObservables.at(14));
	  highSumLoc.AddData(m_vObservables.at(15));
	  lowSumDens.AddData(m_vObservables.at(16));
	   lowSumLoc.AddData(m_vObservables.at(17));

	// Calculate the surface tension of the interface

	Stress1d(pISimBox);

	SurfaceTension();

	// Output the stress tensor observables

	aaScalarSingle gSurfaceTension(m_SurfaceTension);

	gSurfaceTension.AddData(m_vObservables.at(18));
	m_pTotalStress->AddData(m_vObservables.at(19));
	m_pMajorHHStress->AddData(m_vObservables.at(20));
	m_pMajorMinorHHStress->AddData(m_vObservables.at(21));
	m_pMinorHHStress->AddData(m_vObservables.at(22));

	// Store the data in a TSD object and pass it to the aggregate state object

	long dataTotal = 12;

	m_pTSD = new CTimeSeriesData(dataTotal);

	m_pTSD->SetValue(0,  pISimBox->GetCurrentTime(), "Time");
	m_pTSD->SetValue(1,  m_MajorLowMean,  "MajorLow");
	m_pTSD->SetValue(2,  m_MajorMidPoint, "MajorMid");
	m_pTSD->SetValue(3,  m_MajorHighMean, "MajorHigh");
	m_pTSD->SetValue(4,  m_MinorLowMean,  "MinorLow");
	m_pTSD->SetValue(5,  m_MinorMidPoint, "MinorMid");
	m_pTSD->SetValue(6,  m_MinorHighMean, "MinorHigh");
	m_pTSD->SetValue(7,  m_SumMinDensity, "SumMin");
	m_pTSD->SetValue(8,  m_SumMinId,      "SumMinLoc");
	m_pTSD->SetValue(9,  m_SumMaxDensity, "SumMax");
	m_pTSD->SetValue(10, m_SumMaxId,      "SumMaxLoc");
	m_pTSD->SetValue(11, m_SurfaceTension,"SurfaceTension");

	m_pState->AddTimeSeriesData(m_pTSD);


	// We have to delete the data created by the function objects for the scalar,
	// vector and tensor profiles to avoid a memory leak. This is because the transform()
	// algorithm can only insert data into a vector of a non-zero size. I don't want
	// to keep aaScalar, etc objects around and put new data into them so I have to
	// create and delete the objects as they are used. This means that their construction
	// and destruction should be as simple as possible.

	m_pMajorDensity->ClearData();
	m_pMinorDensity->ClearData();
	m_pMajorMomentum->ClearData();
	m_pMinorMomentum->ClearData();

	m_pTotalStress->ClearData();
	m_pMajorHHStress->ClearData();
	m_pMajorMinorHHStress->ClearData();
	m_pMinorHHStress->ClearData();
}

// Function to calculate the stress tensor averaged over sections through the slices
// in a CSliceProfile through the SimBox. The analysis slices may be thinner than
// the CNT cell slices used to hold the beads. The overloaded CAnalysis base class 
// function, StressTensorProfile(), returns a pointer to an aaStressTensor1d object 
// that is used to calculate and store the average stress tensor. The selection of
// the 1d calculation is based on the type of CProfile<> passed in as the second
// argument.
//
// Note that the stress tensor is calculated anew each time the bilayer analysis
// is done, so we have to delete the previous aaStressTensor1d object before
// storing the new one. The width of the slices need not be the same as the
// CNT cell width, in fact, it should be much smaller to get an accurate picture
// of the stress in the bilayer.
//

void CInterface::Stress1d(const ISimBox *const pISimBox)
{
	if(m_pStress1d)
		delete m_pStress1d;

	// Because the overloaded CAnalysis::StressTensorProfile() functions all 
	// return an aaStressTensor* we have to cast it to the appropriate real type

	m_pStress1d = dynamic_cast<aaStressTensor1d*>(StressTensorProfile(pISimBox, m_pMajorSP, m_X, m_Y, m_Z));
}

// Function to calcuate the surface tension of the interface from the stress tensor
// averaged over thin slices parallel to the interface.
//
// The surface tension across a planar interface is given by the difference of the
// normal and transverse components of the stress tensor integrated across the
// interface.
//
// This routine also outputs the stress tensor for each type of interaction in the
// interface: so far these are just the major and minor polymer head bead types.
// We extract the tensor contributions for each type and store them in
// scalar profiles across the SimBox.

void CInterface::SurfaceTension()
{
	// Get the contributions to the stress tensor from each bead-bead interaction
	// and convert it into a scalar pressure profile using a function object that
	// iterates over a vector of aaStressTensorPoints.
	//
	// Note that the total stress contains contributions from all beads not just the
	// solvent, head and tail beads. Hence it may show non-zero values even when the 
	// selected bead-bead interactions show zero.

	m_SurfaceTension = 0.0;

	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pMajorHHStress->Begin(),      aaBeadStressZProfile(m_vBeadStressIndex[0]));
	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pMajorMinorHHStress->Begin(), aaBeadStressZProfile(m_vBeadStressIndex[1]));
	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pMinorHHStress->Begin(),      aaBeadStressZProfile(m_vBeadStressIndex[2]));
	transform(m_pStress1d->Begin(), m_pStress1d->End(), m_pTotalStress->Begin(),        aaStressZProfile());

	for(long i=0; i<m_pStress1d->GetSize(); i++)
	{
		// display the actual components for debugging purposes

//		double s11 = m_pStress1d->Get11(i,m_vBeadStressIndex[0]);
//		double s12 = m_pStress1d->Get12(i,m_vBeadStressIndex[0]);
//		double s13 = m_pStress1d->Get13(i,m_vBeadStressIndex[0]);
//		double s21 = m_pStress1d->Get21(i,m_vBeadStressIndex[0]);
//		double s22 = m_pStress1d->Get22(i,m_vBeadStressIndex[0]);
//		double s23 = m_pStress1d->Get23(i,m_vBeadStressIndex[0]);
//		double s31 = m_pStress1d->Get31(i,m_vBeadStressIndex[0]);
//		double s32 = m_pStress1d->Get32(i,m_vBeadStressIndex[0]);
//		double s33 = m_pStress1d->Get33(i,m_vBeadStressIndex[0]);

//		double sww = m_pMajorHHStress->GetData(i);
//		double shw = m_pMinorHHStress->GetData(i);
//		double stw = m_pMajorMinorHHStress->GetData(i);

		double stotal = m_pTotalStress->GetData(i);

		m_SurfaceTension += stotal;
	}

	// Multiply in the width of the stress slices to complete the integral approximation

	m_SurfaceTension *= m_pStress1d->GetWidth();

	// Manual check on the surface tension calculation

//	double surfaceTension2 = 0.0;
//	for(cStressVectorIterator pSP=m_pStress1d->Begin(); pSP!=m_pStress1d->End(); pSP++)
//	{
//		surfaceTension2 += (0.5*((*pSP)->GetConfigTotal(0) + (*pSP)->GetConfigTotal(4)) - (*pSP)->GetConfigTotal(8));
//	}
//	surfaceTension2 *= m_pStress1d->GetWidth();
}
