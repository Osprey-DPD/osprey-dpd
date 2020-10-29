/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Micelle.cpp: implementation of the CMicelle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "Micelle.h"			
#include "MicelleData.h"
#include "SimState.h"
#include "ISimBox.h"
#include "Bead.h"
#include "aaRegionToType.h"

#include "aaScalarSingle.h"				// Types of data that can be sent as observables
#include "aaVectorSingle.h"
#include "aaScalarProfile.h"
#include "aaVectorProfile.h"

#include "ScalarObservable.h"			// Types of observables
#include "VectorObservable.h"
#include "TensorObservable.h"
#include "ScalarProfileObservable.h"

#include "TimeSeriesData.h"
#include "AutoCorr.h"

// STL using declarations

#if Platform == DECALPHA 
#elif Platform == SGICC
#elif Platform == CRAYJ90
#elif Platform == BORLAND6
	using std::transform;
	using std::slice;
	using std::gslice;
#elif Platform == I7XEON
	using std::transform;
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
#elif Platform == XCMAC
#elif Platform == NEWPLATFORM1
#elif Platform == NEWPLATFORM2
#elif Platform == NEWPLATFORM3
#elif Platform == NEWPLATFORM4
#elif Platform == NEWPLATFORM5
#elif Platform == NEWPLATFORM6
#elif Platform == NEWPLATFORM7
#elif Platform == NEWPLATFORM8
#elif Platform == NEWPLATFORM9
#elif Platform == NEWPLATFORM10
#else							
	using std::transform;
	using std::slice;
	using std::gslice;
#endif			


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We pass the CAnalysisData-derived object to the CAnalysis base class for 
// initialisation of all general analysis data. The pointer is passed as
// const as we do not expect it to be altered.

CMicelle::CMicelle(const zString runId, long samplePeriod, 
				   const CMicelleData* const pAD) : CAnalysis(runId, samplePeriod, pAD),
												    m_Polymer(pAD->m_Polymer),
												    m_Solvent(pAD->m_Solvent),
													m_VACTimeLag(pAD->m_VACTimeLag),
													m_pTSD(NULL),
													m_LipidType(0),
													m_LipidHeadType(0),
													m_LipidTailType(0),
													m_SolventType(0),
													m_SolventHeadType(0),
													m_MaxMicelleSize(0),
													m_Size(0),
													m_PolymerSize(0),
													m_InitialTime(0),
													m_dSize(0.0),
													m_dBeadSize(0.0),
													m_Radius(0.0),
													m_Rg2(0.0),
													m_SurfaceArea(0.0),
													m_DiffCoeff(0.0),
													m_CMVelMag(0.0),
													m_pVAC(0),
													m_pVACProfile(0)

{
	for(short int i=0; i<3; i++)
	{
		m_CMInitialPos[i]	= 0.0;
		m_CMPos[i]			= 0.0;
		m_CMVel[i]			= 0.0;
		m_Inertia[3*i]		= 0.0;
		m_Inertia[3*i+1]	= 0.0;
		m_Inertia[3*i+2]	= 0.0;
	}

	m_lLipids.clear();
	m_lCurrentLipids.clear();

	m_mPolymerFromHeadId.clear();
	m_mPolymerFromTailId.clear();
}

CMicelle::~CMicelle()
{
	if(m_pVAC)
	{
		delete m_pVAC;
		m_pVAC = 0;
	}

	if(m_pVACProfile)
	{
		delete m_pVACProfile;
		m_pVACProfile = 0;
	}

}

// **********************************************************************
// Function to analyse the properties of a micelle made up of a single 
// major component, called "lipids" for convenience. In the future, minor 
// components may be added to the micelle but there must be a single 
// defining component that forms the bulk of the bilayer. We collect all 
// polymers that are of the user-specified type and store them locally 
// for rapid analysis.
//
// **********************************************************************

void CMicelle::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	// Counters and iterator used many times below. I have to declare them because
	// different machines may or may not allow them to be redefined in different loops
	// in the same scope.

	cPolymerListIterator iterLipid;
	cBeadVectorIterator iterBead;

	// **********************************************************************
	// On the first call, count the number of micelle-forming polymers in the
	// SimBox so that we can allocate memory to the containers and store the 
	// polymers locally for analysis. We also create maps allowing polymers 
	// to be accessed given the ids of their head or tail beads as keys.
	// Note that this requires that ConvertNames() be called before the first
	// call to UpdateState() otherwise the "types" of the polymers and beads
	// will not be available.

	if(m_MaxMicelleSize == 0)
	{
		// Store first sample time for diffusion coefficient calculation

		m_InitialTime = pISimBox->GetCurrentTime();

		// Count the number of polymers able to form a micelle

		m_MaxMicelleSize = count_if(rSimState.GetPolymers().begin(), rSimState.GetPolymers().end(), aaPolymerType(m_LipidType));

		// Create maps allowing polymer pointers to be obtained given head and tail bead
		// ids as keys. These are used in the bilayer height calculataion.

		for(cPolymerVectorIterator iterPoly=rSimState.GetPolymers().begin(); iterPoly!=rSimState.GetPolymers().end(); iterPoly++)
		{
			if((*iterPoly)->GetType() == m_LipidType)
			{
				m_lLipids.push_back(*iterPoly);

				m_mPolymerFromHeadId.insert(zPairLongPolymer((*iterPoly)->GetHead()->GetId(), *iterPoly));
				m_mPolymerFromTailId.insert(zPairLongPolymer((*iterPoly)->GetTail()->GetId(), *iterPoly));
			}
		}

		// Store the number of beads per micelle polymer and the total number of beads
		// in all micelle-forming polymers

		m_PolymerSize = m_lLipids.front()->GetSize();
		m_dBeadSize = static_cast<double>(m_MaxMicelleSize*m_PolymerSize);

		// Calculate the initial micelle CM coordinates for use in normalising the
		// data during a run

		m_CMInitialPos[0] = 0.0;
		m_CMInitialPos[1] = 0.0;
		m_CMInitialPos[2] = 0.0;

		for(iterLipid=m_lLipids.begin(); iterLipid!=m_lLipids.end(); iterLipid++)
		{
			for(iterBead=(*iterLipid)->GetBeads().begin(); iterBead!=(*iterLipid)->GetBeads().end(); iterBead++)
			{
				double xb = (*iterBead)->GetunPBCXPos();
				double yb = (*iterBead)->GetunPBCYPos();
				double zb = (*iterBead)->GetunPBCZPos();

				m_CMInitialPos[0] += xb;
				m_CMInitialPos[1] += yb;
				m_CMInitialPos[2] += zb;
			}
		}

		m_CMInitialPos[0] /= m_dBeadSize;
		m_CMInitialPos[1] /= m_dBeadSize;
		m_CMInitialPos[2] /= m_dBeadSize;


		// Create the observables needed to hold the micelle data.

		// Id	Name
		// *********
		//
		//  First define observables derived from the lipid bead lists
		//
		//	0	Current number of polymers in micelle
		//	1	Radius of micelle
		//	2	Surface area of micelle
		//	3	CM diffusion coefficient of micelle
		//	4	Centre of mass position of micelle
		//	5	Centre of mass velocity of micelle
		//  6   CM velocity autocorrelation function

		CScalarObservable* pSize	= new CScalarObservable(m_Polymer + " total",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pRadius	= new CScalarObservable(m_Polymer + " micelle radius",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pArea    = new CScalarObservable(m_Polymer + " micelle surface area",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pDiff    = new CScalarObservable(m_Polymer + " micelle diffusion",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CVectorObservable* pCMPos   = new CVectorObservable(m_Polymer + " micelle CM position",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CVectorObservable* pCMVel   = new CVectorObservable(m_Polymer + " micelle CM velocity",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
//		CTensorObservable* pInertia	= new CTensorObservable(m_Polymer + " micelle inertia tensor", 
//												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

		// Create the CAutoCorr object to calculate the micelle's CM velocity 
		// autocorrelation function, and the scalar profile to hold results

		m_pVAC = new CAutoCorr(rSimState.GetAnalysisPeriod()/rSimState.GetSamplePeriod(), m_VACTimeLag);

		m_pVACProfile	= new aaScalarProfile(m_VACTimeLag);

		// Pass the sample period instead of the analysis period to prevent 
		// CScalarProfileObservable normalising the data by analysisPeriod/samplePeriod.
		// This would work if the data were sampled every samplePeriod steps,
		// but we only pass the autocorrelation data once per analysis period.

	    CScalarProfileObservable* pVAutoCorr = new CScalarProfileObservable("VelAutoCorr",  
												rSimState.GetSamplePeriod(), rSimState.GetSamplePeriod(), m_VACTimeLag);


		m_vObservables.push_back(pSize);
		m_vObservables.push_back(pRadius);
		m_vObservables.push_back(pArea);
		m_vObservables.push_back(pDiff);
		m_vObservables.push_back(pCMPos);
		m_vObservables.push_back(pCMVel);
//		m_vObservables.push_back(pInertia);
		m_vObservables.push_back(pVAutoCorr);

	}
	else	// If not the first time
	{
	}

	// **********************************************************************
	// Now do the general analysis for the micelle. Initially we assume that all 
	// polymers are in the micelle and find the average radius. Then we see if
	// any polymers are actually outside the micelle and remove them from the
	// set being analysed. Next, we calculate the rest of the micelle properties,
	// surface area, CM position and momentum, inertia tensor.

	m_Size = m_MaxMicelleSize;

	// Store double versions of the number of polymers and beads in the micelle:
	// note that these numbers can change if polymers migrate out of, or into,
	// the micelle during a run

	m_dSize	    = static_cast<double>(m_Size);
	m_dBeadSize = static_cast<double>(m_Size*m_PolymerSize);

	m_CMPos[0] = 0.0;
	m_CMPos[1] = 0.0;
	m_CMPos[2] = 0.0;
	m_CMVel[0] = 0.0;
	m_CMVel[1] = 0.0;
	m_CMVel[2] = 0.0;

	for(iterLipid=m_lLipids.begin(); iterLipid!=m_lLipids.end(); iterLipid++)
	{
		for(iterBead=(*iterLipid)->GetBeads().begin(); iterBead!=(*iterLipid)->GetBeads().end(); iterBead++)
		{
			double xb = (*iterBead)->GetunPBCXPos();
			double yb = (*iterBead)->GetunPBCYPos();
			double zb = (*iterBead)->GetunPBCZPos();

			m_CMPos[0] += xb;
			m_CMPos[1] += yb;
			m_CMPos[2] += zb;
			m_CMVel[0] += (*iterBead)->GetXMom();
			m_CMVel[1] += (*iterBead)->GetYMom();
			m_CMVel[2] += (*iterBead)->GetZMom();
		}
	}

	m_CMPos[0] /= m_dBeadSize;
	m_CMPos[1] /= m_dBeadSize;
	m_CMPos[2] /= m_dBeadSize;

	m_CMVel[0] /= m_dBeadSize;
	m_CMVel[1] /= m_dBeadSize;
	m_CMVel[2] /= m_dBeadSize;

	m_CMVelMag = sqrt(m_CMVel[0]*m_CMVel[0] + m_CMVel[1]*m_CMVel[1] + m_CMVel[2]*m_CMVel[2]);

	// now find the average radius of the micelle from the location of the 
	// head beads only

	m_Radius		= 0.0;
	m_SurfaceArea	= 0.0;

	for(iterLipid=m_lLipids.begin(); iterLipid!=m_lLipids.end(); iterLipid++)
	{
		double hx = (*iterLipid)->GetHead()->GetunPBCXPos();
		double hy = (*iterLipid)->GetHead()->GetunPBCYPos();
		double hz = (*iterLipid)->GetHead()->GetunPBCZPos();

		m_Radius += (hx - m_CMPos[0])*(hx - m_CMPos[0]);
		m_Radius += (hy - m_CMPos[1])*(hy - m_CMPos[1]);
		m_Radius += (hz - m_CMPos[2])*(hz - m_CMPos[2]);
	}

	m_Radius = sqrt(m_Radius/m_dSize);

	// Fake the surface area for now as that of the equivalent sphere

	m_SurfaceArea = 4.0*xxBase::m_globalPI*m_Radius*m_Radius;

	// Find the diffusion coefficient from the distance moved by the micelle CM
	// from its initial position: ignore the calculation for the first sample
	// as the denominator is zero. The definition is
	//
	//	D = Lim (r(t) - r(0))**2/(6*t)
	//
	// where the limit is as t goes to infinity, and t = N*dt, dt = integration
	// step size and N is the number of time steps at the sample time.
	// Note that because the first call of this routine may not be at SimTime = 0, 
	// we actually use the time, and micelle coordinates, from the first call 
	// to calculate the distance moved.

	double dt = (pISimBox->GetCurrentTime() - m_InitialTime)*pISimBox->GetStepSize();

	if(dt > 0.0)
	{
		double dx[3];

		dx[0] = m_CMPos[0] - m_CMInitialPos[0];
		dx[1] = m_CMPos[1] - m_CMInitialPos[1];
		dx[2] = m_CMPos[2] - m_CMInitialPos[2];

		m_DiffCoeff = (dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2])/(6.0*dt);
	}

	// Check to see if any polymers have their tail beads outside the micelle:
	// this is the criterion for removing a polymer from the current micelle state.
	// This might not work if the micelle has a very asymmetric shape as the tail
	// of polymers that are within the micelle might be further away from the
	// centre of mass than the average radius.

	// Not implemented yet



	// Copy the data into the micelle's observables using the encapsulated 
	// data types aaScalarSingle, aaVectorSingle, etc, and call the 
	// AddData() function on each so that it can pass itself
	// to the appropriate CObservable-derived object

	aaScalarSingle size(m_Size);
	aaScalarSingle radius(m_Radius);
	aaScalarSingle area(m_SurfaceArea);
	aaScalarSingle diff(m_DiffCoeff);
	aaVectorSingle cmpos(m_CMPos[0], m_CMPos[1], m_CMPos[2]);
	aaVectorSingle cmvel(m_CMVel[0], m_CMVel[1], m_CMVel[2]);

	size.AddData(m_vObservables.at(0));
	radius.AddData(m_vObservables.at(1));
	area.AddData(m_vObservables.at(2));
	diff.AddData(m_vObservables.at(3));
	cmpos.AddData(m_vObservables.at(4));
	cmvel.AddData(m_vObservables.at(5));

	// Add the micelle's CM velocity to the autocorrelation function object.
	// One must exist as it was created above in the first call to this function.
	// Ff the current simulation time is a multiple of the analysis period,
	// we pass the data to the observable for output. Because we only store 
	// the function once in the observable, no averaging over time is performed.

	m_pVAC->AddSample(m_CMVelMag);


// Test case of a sine wave
//	double xx = sin(10.0*m_globalTwoPI*static_cast<double>(pISimBox->GetCurrentTime())/1000.0);
//	m_pVAC->AddSample(xx);

	if(pISimBox->GetCurrentTime()%rSimState.GetAnalysisPeriod() == 0)
	{
//		std::cout << "starting normalisation" << zEndl;

		for(long m=0; m<m_VACTimeLag; m++)
		{
			double x = m_pVAC->GetAutoCorrValue(m);

			zLongVector sams = m_pVAC->GetSampleTotals();
			long total = sams.at(m);

//			std::cout << "lag = " << m << " # samples =  " << total << "  C2(m) = " << x << zEndl;
		}

		// Copy the data from the CAutoCorr object to a local profile

		zDoubleVector c2CMVel = m_pVAC->GetAutoCorr();

		transform(c2CMVel.begin(), c2CMVel.end(), m_pVACProfile->Begin(), aaDouble());

		m_pVACProfile->AddData(m_vObservables.at(6));
		m_pVACProfile->ClearData();

		// Create a new CAutoCorr object to hold the next data set

		delete m_pVAC;
		m_pVAC = new CAutoCorr(rSimState.GetAnalysisPeriod()/rSimState.GetSamplePeriod(), m_VACTimeLag);
	}

	// Store the time-dependent data in a TSD object and pass it to the 
	// aggregate state object. 

	long dataTotal = 8;

	m_pTSD = new CTimeSeriesData(dataTotal);

	m_pTSD->SetValue(0, pISimBox->GetCurrentTime(), "Time");
	m_pTSD->SetValue(1, m_Radius,        "Radius");
	m_pTSD->SetValue(2, m_SurfaceArea,   "SurfaceArea");
	m_pTSD->SetValue(3, m_DiffCoeff,     "Diffusion");
	m_pTSD->SetValue(4, m_CMPos[0],      "XCMPos");
	m_pTSD->SetValue(5, m_CMPos[1],      "YCMPos");
	m_pTSD->SetValue(6, m_CMPos[2],      "ZCMPos");
	m_pTSD->SetValue(7, m_CMVelMag,      "CMVelMag");

	m_pState->AddTimeSeriesData(m_pTSD);

}

// **********************************************************************
// Function to store the integer ids for beads, bonds and polymers given 
// the strings that were entered by the user in the control data file.
//
// **********************************************************************

void CMicelle::ConvertNames(const CAnalysisState &raState)
{
	m_LipidType			= raState.GetPolymerTypeFromName(m_Polymer);
	m_LipidHeadType		= raState.GetPolymerHeadType(m_LipidType);
	m_LipidTailType		= raState.GetPolymerTailType(m_LipidType);
	m_SolventType		= raState.GetPolymerTypeFromName(m_Solvent);
	m_SolventHeadType	= raState.GetPolymerHeadType(m_SolventType);
}

