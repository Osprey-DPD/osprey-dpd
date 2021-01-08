/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Vesicle.cpp: implementation of the CVesicle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimMiscellaneousFlags.h"
#include "Vesicle.h"
#include "VesicleData.h"
#include "SimState.h"
#include "ISimBox.h"

#include "Bead.h"
#include "StressGridCell.h"
#include "aaRegionToType.h"

#include "aaScalarSingle.h"				// Types of data that can be sent as observables
#include "aaVectorSingle.h"
#include "aaTensorSingle.h"
#include "aaScalarProfile.h"

#include "ScalarObservable.h"			// Types of observables
#include "VectorObservable.h"
#include "TensorObservable.h"
#include "ScalarProfileObservable.h"

#include "DiagonaliseMatrix.h"			// Needed for the inertia tensor diagonalisation
#include "TimeSeriesData.h"
#include "AnalysisTool.h"				// Need this if no tools are defined

// STL using declarations


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVesicle::CVesicle(const zString runId, long sample, 
				   const CVesicleData* const pAD) : CAnalysis(runId, sample, pAD),
												    m_Polymer(pAD->m_Polymer),
												    m_Solvent(pAD->m_Solvent),
				                                    m_bStress(pAD->m_bStress),
													m_ShellTotal(pAD->m_ShellTotal),
													m_ShellMin(pAD->m_ShellMin),
													m_ShellMax(pAD->m_ShellMax),
													m_LipidType(0),
													m_LipidHeadType(0),
													m_LipidTailType(0),
													m_SolventType(0),
													m_SolventHeadType(0),
													m_MaxVesicleSize(0),
													m_Size(0),
													m_InitialTime(0),
													m_dSize(0.0),
													m_Radius(0.0),
													m_Rg2(0.0),
													m_SurfaceArea(0.0),
													m_SurfaceTensionTrap(0.0),
													m_SurfaceTensionSimpson(0.0),
													m_DiffCoeff(0.0),
													m_InitialCMXPos(0.0),
													m_InitialCMYPos(0.0),
													m_InitialCMZPos(0.0),
													m_CMXPos(0.0),
													m_CMYPos(0.0),
													m_CMZPos(0.0),
													m_CMXVel(0.0),
													m_CMYVel(0.0),
													m_CMZVel(0.0),
													m_Eccentricity(0.0),
													m_StressCellRatio(0.0),
													m_StressCellVolume(0.0),
													m_pSrr(0),
													m_pStt(0),
													m_pStressProfile(0)
{
	// Store a CDiagonaliseMatrix object to perform the diagonalisation of
	// the vesicle inertia tensor. We use a pointer rather than an enclosed
	// object because we need to pass in the order of the matrix to the constructor.

#if EnableMiscClasses == SimMiscEnabled
	m_pJacobi = new CDiagonaliseMatrix(3);
#endif

	m_InertiaTensor.resize(9);
	m_InertiaEigenvalues.resize(3);
	m_vStressCounter.resize(m_ShellTotal); 
	m_vRadialStress.resize(m_ShellTotal);
	m_vTransverseStress.resize(m_ShellTotal);
	m_vStressProfile.resize(m_ShellTotal);
	m_vTensionProfile.resize(m_ShellTotal+1);   // We need an extra location for Simpson's rule when calculating the surface tension

	m_lLipids.clear();
	m_lCurrentLipids.clear();

	m_mPolymerFromHeadId.clear();
	m_mPolymerFromTailId.clear();
	
	m_StressOrigin[0] = 0.0;
	m_StressOrigin[1] = 0.0;
	m_StressOrigin[2] = 0.0;

}

CVesicle::~CVesicle() 
{
	if(m_pJacobi)
		delete m_pJacobi;

	if(m_pSrr)
	{
		delete m_pSrr;
		m_pSrr = 0;
	}

	if(m_pStt)
	{
		delete m_pStt;
		m_pStt = 0;
	}
	
	if(m_pStressProfile)
	{
		delete m_pStressProfile;
		m_pStressProfile = 0;
	}

}

// **********************************************************************
// Function to store the integer ids for beads, bonds and polymers given 
// the strings that were entered by the user in the control data file.
//
// **********************************************************************

void CVesicle::ConvertNames(const CAnalysisState &raState)
{
	m_LipidType			= raState.GetPolymerTypeFromName(m_Polymer);
	m_LipidHeadType		= raState.GetPolymerHeadType(m_LipidType);
	m_LipidTailType		= raState.GetPolymerTailType(m_LipidType);
	m_SolventType		= raState.GetPolymerTypeFromName(m_Solvent);
	m_SolventHeadType	= raState.GetPolymerHeadType(m_SolventType);

}


void CVesicle::UpdateState(CSimState &rSimState, const ISimBox *const pISimBox)
{
	// Counters and iterator used many times below. I have to declare them because
	// different machines may or may not allow them to be redefined in different loops
	// in the same scope.

	cPolymerListIterator iterLipid;
	cPolymerListIterator iterLipid2;

	short int i = 0;

	// **********************************************************************
	// On the first call, count the number of vesicle-forming polymers in the
	// SimBox so that we can allocate memory to the containers and store the 
	// polymers locally for analysis. We also create maps allowing polymers 
	// to be accessed given the ids of their head or tail beads as keys.
	// Note that this requires that ConvertNames() be called before the first
	// call to UpdateState() otherwise the "types" of the polymers and beads
	// will not be available.

	if(m_MaxVesicleSize == 0)
	{
		// Store first sample time for diffusion coefficient calculation

		m_InitialTime = pISimBox->GetCurrentTime();

		// Count the number of polymers able to form a vesicle

		m_MaxVesicleSize = count_if(rSimState.GetPolymers().begin(), rSimState.GetPolymers().end(), aaPolymerType(m_LipidType));

		// Create maps allowing polymer pointers to be obtained given head and tail bead
		// ids as keys. 

		for(cPolymerVectorIterator iterPoly=rSimState.GetPolymers().begin(); iterPoly!=rSimState.GetPolymers().end(); iterPoly++)
		{
			if((*iterPoly)->GetType() == m_LipidType)
			{
				m_lLipids.push_back(*iterPoly);

				m_mPolymerFromHeadId.insert(zPairLongPolymer((*iterPoly)->GetHead()->GetId(), *iterPoly));
				m_mPolymerFromTailId.insert(zPairLongPolymer((*iterPoly)->GetTail()->GetId(), *iterPoly));
			}
		}

		m_Size = m_MaxVesicleSize;
		m_dSize = static_cast<double>(m_Size);


		// Calculate the initial vesicle CM coordinates for use in normalising the
		// data during a run. We only count the head beads for now. 

		m_InitialCMXPos = 0.0;
		m_InitialCMYPos = 0.0;
		m_InitialCMZPos = 0.0;

		for(iterLipid=m_lLipids.begin(); iterLipid!=m_lLipids.end(); iterLipid++)
		{
			m_InitialCMXPos += (*iterLipid)->GetHead()->GetXPos();
			m_InitialCMYPos += (*iterLipid)->GetHead()->GetYPos();
			m_InitialCMZPos += (*iterLipid)->GetHead()->GetZPos();
		}

		m_InitialCMXPos /= m_dSize;
		m_InitialCMYPos /= m_dSize;
		m_InitialCMZPos /= m_dSize;

#if EnableStressTensorSphere == SimMiscEnabled
        // Store the number of stress cells per CNT cell and the inverse volume of the stress cells for normalisation purposes.
		
		m_StressCellRatio  = static_cast<double>(pISimBox->GetStressCellRatio());
		m_StressCellVolume = pISimBox->GetVolume()/static_cast<double>(pISimBox->GetStressCellTotal());
		
        // The size of the  stress profiles is set by the number of shells into which the space is divided
		
		m_pSrr           = new aaScalarProfile(m_ShellTotal);
		m_pStt           = new aaScalarProfile(m_ShellTotal);
		m_pStressProfile = new aaScalarProfile(m_ShellTotal);
#endif		

		// Create the observables needed to hold the vesicle data.

		// Id	Name
		// *********
		//
		//  First define observables derived from the lipid bead lists
		//
		//	0	Current number of polymers in vesicle
		//	1	Radius of vesicle
		//  2   Radius of gyration
		//	3	Eccentricity ( = largest eigenvalue/smallest eigenvalue of inertia tensor)
		//	4	Surface area of vesicle
		//  5   Surface tension of vesicle integrated using Trapezoidal rule
		//  6   Surface tension of vesicle integrated using Simpson's rule
		//	7	CM diffusion coefficient of vesicle
		//	8	Centre of mass position of vesicle
		//	9	Centre of mass velocity of vesicle
		//	XX  Inertia tensor for vesicle - not implemented yet
		// 
		//  Now the stress tensor components.
		//
		//  10  Radial stress tensor component     - rr
		//  11  Transverse stress tensor component - 0.5*(theta-theta + phi-phi)
		//  12  Lateral stress profile = Srr - Stt
		

		CScalarObservable* pSize	= new CScalarObservable(m_Polymer + " total",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pRadius	= new CScalarObservable(m_Polymer + " Vesicle Radius",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pRg2		= new CScalarObservable(m_Polymer + " Vesicle Radius of Gyration",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pEcc		= new CScalarObservable(m_Polymer + " Vesicle Eccentricity",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pArea    = new CScalarObservable(m_Polymer + " Vesicle Surface Area",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pTensionTrap    = new CScalarObservable(m_Polymer + " Vesicle Trapezoidal Surface Tension",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pTensionSimpson = new CScalarObservable(m_Polymer + " Vesicle Simpson Surface Tension",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CScalarObservable* pDiff    = new CScalarObservable(m_Polymer + " Vesicle Diffusion Constant",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CVectorObservable* pCMPos   = new CVectorObservable(m_Polymer + "Vesicle CM Position",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
		CVectorObservable* pCMVel   = new CVectorObservable(m_Polymer + " Vesicle CM Velocity",
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());
//		CTensorObservable* pInertia	= new CTensorObservable(m_Polymer + " vesicle inertia tensor", 
//												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod());

		m_vObservables.push_back(pSize);
		m_vObservables.push_back(pRadius);
		m_vObservables.push_back(pRg2);
		m_vObservables.push_back(pEcc);
		m_vObservables.push_back(pArea);
		m_vObservables.push_back(pTensionTrap);
		m_vObservables.push_back(pTensionSimpson);
		m_vObservables.push_back(pDiff);
		m_vObservables.push_back(pCMPos);
		m_vObservables.push_back(pCMVel);
//		m_vObservables.push_back(pInertia);
		
#if EnableStressTensorSphere == SimMiscEnabled
		// Now the diagonal elements of the stress tensor

	    if(m_bStress)
	    {
	        CScalarProfileObservable* prrStressPro = new CScalarProfileObservable(" Radial Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_ShellTotal);

	        CScalarProfileObservable* pttStressPro = new CScalarProfileObservable(" Transverse Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_ShellTotal);

	        CScalarProfileObservable* pStressPro = new CScalarProfileObservable(" Stress Profile",  
												rSimState.GetAnalysisPeriod(), rSimState.GetSamplePeriod(), m_ShellTotal);


		    m_vObservables.push_back(prrStressPro);
		    m_vObservables.push_back(pttStressPro);
		    m_vObservables.push_back(pStressPro);
	    }
#endif
	}
	else	// If not the first time
	{
	}

	// **********************************************************************
	// Now do the general analysis for the vesicle. Initially we assume that all 
	// polymers are in the vesicle and find the average radius. Then we see if
	// any polymers are actually outside the vesicle and remove them from the
	// set being analysed. Next, we calculate the rest of the vesicle properties,
	// surface area, CM position and momentum, inertia tensor.

	// Store double versions of the number of polymers in the vesicle:
	// note that these numbers can change if polymers migrate out of, or into,
	// the vesicle during a run

	m_CMXPos = 0.0;
	m_CMYPos = 0.0;
	m_CMZPos = 0.0;
	m_CMXVel = 0.0;
	m_CMYVel = 0.0;
	m_CMZVel = 0.0;

	for(iterLipid=m_lLipids.begin(); iterLipid!=m_lLipids.end(); iterLipid++)
	{
		m_CMXPos += (*iterLipid)->GetHead()->GetXPos();
		m_CMYPos += (*iterLipid)->GetHead()->GetYPos();
		m_CMZPos += (*iterLipid)->GetHead()->GetZPos();
		m_CMXVel += (*iterLipid)->GetHead()->GetXMom();
		m_CMYVel += (*iterLipid)->GetHead()->GetYMom();
		m_CMZVel += (*iterLipid)->GetHead()->GetZMom();
	}

	m_CMXPos /= m_dSize;
	m_CMYPos /= m_dSize;
	m_CMZPos /= m_dSize;

	m_CMXVel /= m_dSize;
	m_CMYVel /= m_dSize;
	m_CMZVel /= m_dSize;
	
	// now find the average radius of the vesicle from the location of the 
	// head beads only, its surface area and inertia tensor.

	m_Radius		= 0.0;
	m_Rg2			= 0.0;
	m_Eccentricity	= 0.0;
	m_SurfaceArea	= 0.0;

	for(i=0; i<3; i++)
	{
		m_InertiaTensor.at(3*i)   = 0.0;
		m_InertiaTensor.at(3*i+1) = 0.0;
		m_InertiaTensor.at(3*i+2) = 0.0;
	}

	for(iterLipid=m_lLipids.begin(); iterLipid!=m_lLipids.end(); iterLipid++)
	{
		double hx = (*iterLipid)->GetHead()->GetunPBCXPos();
		double hy = (*iterLipid)->GetHead()->GetunPBCYPos();
		double hz = (*iterLipid)->GetHead()->GetunPBCZPos();

		m_Radius += (hx - m_CMXPos)*(hx - m_CMXPos);
		m_Radius += (hy - m_CMYPos)*(hy - m_CMYPos);
		m_Radius += (hz - m_CMZPos)*(hz - m_CMZPos);

		// now the inertia tensor. We only store the difference in the coordinates
		// of the head particles temporarily as we don't need the actual coordinates.
		// We use the symmetry of the inertia tensor to avoid having to calculate
		// the lower diagonal elements.

		double hxp1 = (*iterLipid)->GetHead()->GetXPos();
		double hyp1 = (*iterLipid)->GetHead()->GetYPos();
		double hzp1 = (*iterLipid)->GetHead()->GetZPos();

			for(iterLipid2=m_lLipids.begin(); iterLipid2!=m_lLipids.end(); iterLipid2++)
			{
				double dhpx = hxp1 - (*iterLipid2)->GetHead()->GetXPos();
				double dhpy = hyp1 - (*iterLipid2)->GetHead()->GetYPos();
				double dhpz = hzp1 - (*iterLipid2)->GetHead()->GetZPos();

				m_InertiaTensor.at(0) += dhpx*dhpx;
				m_InertiaTensor.at(1) += dhpx*dhpy;
				m_InertiaTensor.at(2) += dhpx*dhpz;
				m_InertiaTensor.at(4) += dhpy*dhpy;
				m_InertiaTensor.at(5) += dhpy*dhpz;
				m_InertiaTensor.at(8) += dhpz*dhpz;
			}
	}

	m_Radius                = sqrt(m_Radius/m_dSize);
	m_SurfaceArea           = 4.0*xxBase::m_globalPI*m_Radius*m_Radius;    // Approximate the surface area for now as that of the equivalent sphere
	m_SurfaceTensionTrap    = 0.0;  // This is only calculated if the code is compiled in
	m_SurfaceTensionSimpson = 0.0;  // This is only calculated if the code is compiled in

	// Normalize the inertia tensor

	m_InertiaTensor.at(0) /= (2.0*m_dSize*m_dSize);
	m_InertiaTensor.at(1) /= (2.0*m_dSize*m_dSize);
	m_InertiaTensor.at(2) /= (2.0*m_dSize*m_dSize);
	m_InertiaTensor.at(4) /= (2.0*m_dSize*m_dSize);
	m_InertiaTensor.at(5) /= (2.0*m_dSize*m_dSize);
	m_InertiaTensor.at(8) /= (2.0*m_dSize*m_dSize);

	// and copy the upper diagonal elements to the lower diagonal ones

	m_InertiaTensor.at(3) = m_InertiaTensor.at(1);
	m_InertiaTensor.at(6) = m_InertiaTensor.at(2);
	m_InertiaTensor.at(7) = m_InertiaTensor.at(5);

	// Diagonalise the inertia tensor using the CDiagonaliseMatrix class.
	// The order of the symmetric matrix is passed in to the constructor.
 
    // 07/05/20  The DiaganoliseMatrix class is not implemented yet.

#if EnableMiscClasses == SimMiscEnabled
/*
	long nrot = m_pJacobi->Diagonalise(m_InertiaTensor);

	m_InertiaEigenvalues.at(0) = m_pJacobi->GetEigenvalue(0);
	m_InertiaEigenvalues.at(1) = m_pJacobi->GetEigenvalue(1);
	m_InertiaEigenvalues.at(2) = m_pJacobi->GetEigenvalue(2);

	m_Rg2 = m_InertiaEigenvalues.at(0) + m_InertiaEigenvalues.at(1) + m_InertiaEigenvalues.at(2);

	// Sort the eigenvalues: a > b > c, etc

	double l1 = m_InertiaEigenvalues.at(0);	// debug output
	double l2 = m_InertiaEigenvalues.at(1);
	double l3 = m_InertiaEigenvalues.at(2);

	if(m_InertiaEigenvalues.at(0) > m_InertiaEigenvalues.at(1))
	{
		if(m_InertiaEigenvalues.at(1) > m_InertiaEigenvalues.at(2))
		{
			m_Eccentricity = m_InertiaEigenvalues.at(0)/m_InertiaEigenvalues.at(2);
		}
		else if(m_InertiaEigenvalues.at(0) > m_InertiaEigenvalues.at(2))
		{
			m_Eccentricity = m_InertiaEigenvalues.at(0)/m_InertiaEigenvalues.at(1);
		}
		else
		{
			m_Eccentricity = m_InertiaEigenvalues.at(2)/m_InertiaEigenvalues.at(1);
		}
	}
	else if(m_InertiaEigenvalues.at(0) > m_InertiaEigenvalues.at(2))
	{
	   m_Eccentricity = m_InertiaEigenvalues.at(1)/m_InertiaEigenvalues.at(2);
	}
	else if(m_InertiaEigenvalues.at(1) > m_InertiaEigenvalues.at(2))
	{
	   m_Eccentricity = m_InertiaEigenvalues.at(1)/m_InertiaEigenvalues.at(0);
	}
	else
	{
	   m_Eccentricity = m_InertiaEigenvalues.at(2)/m_InertiaEigenvalues.at(0);
	}
 */
#endif

	// Find the diffusion coefficient from the distance moved by the vesicle CM
	// from its initial position: ignore the calculation for the first sample
	// as the denominator is zero. The definition is
	//
	//	D = Lim (r(t) - r(0))**2/(6*t)
	//
	// where the limit is as t goes to infinity. Note that because the first
	// call of this routine may not be at SimTime = 0, we actually use the 
	// time, and vesicle coordinates, from the first call to calculate
	// the distance moved.

	double dt = pISimBox->GetCurrentTime() - m_InitialTime;

	if(dt > 0.0)
	{
		double dx[3];

		dx[0] = m_CMXPos - m_InitialCMXPos;
		dx[1] = m_CMYPos - m_InitialCMYPos;
		dx[2] = m_CMZPos - m_InitialCMZPos;

		m_DiffCoeff = (dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2])/(6.0*dt);
	}
	
	
#if EnableStressTensorSphere == SimMiscEnabled
	// Calculate the stress profile across the vesicle membrane if required: if it either not compiled in or not selected, we write out zero
	
	if(m_bStress)
	{
	    m_StressOrigin[0] = m_CMXPos;
	    m_StressOrigin[1] = m_CMYPos;
	    m_StressOrigin[2] = m_CMZPos;
	
	    // Send the CM of the vesicle to the SimBox for use in transforming the stress tensor in each CNT cell into spherical polar coordinates,
		// and then integrate the profile across the vesicle membrane to get the surface tension.
		
	    pISimBox->SetStressSphereOrigin(m_StressOrigin);
	    StressProfile(pISimBox);
	}
#endif

	// Copy the data into the vesicle's observables using the encapsulated 
	// data types aaScalarSingle, aaVectorSingle, etc, and call the 
	// AddData() function on each so that it can pass itself
	// to the appropriate CObservable-derived object

	aaScalarSingle size(m_Size);
	aaScalarSingle radius(m_Radius);
	aaScalarSingle rg2(m_Rg2);
	aaScalarSingle ecc(m_Eccentricity);
	aaScalarSingle area(m_SurfaceArea);
	aaScalarSingle tension1(m_SurfaceTensionTrap);
	aaScalarSingle tension2(m_SurfaceTensionSimpson);
	aaScalarSingle diff(m_DiffCoeff);
	aaVectorSingle cmpos(m_CMXPos, m_CMYPos, m_CMZPos);
	aaVectorSingle cmvel(m_CMXVel, m_CMYVel, m_CMZVel);
//	aaTensorSingle intens(m_InertiaTensor);

	size.AddData(m_vObservables.at(0));
	radius.AddData(m_vObservables.at(1));
	rg2.AddData(m_vObservables.at(2));
	ecc.AddData(m_vObservables.at(3));
	area.AddData(m_vObservables.at(4));
	tension1.AddData(m_vObservables.at(5));
	tension2.AddData(m_vObservables.at(6));
	diff.AddData(m_vObservables.at(7));
	cmpos.AddData(m_vObservables.at(8));
	cmvel.AddData(m_vObservables.at(9));
//	intens.AddData(m_vObservables.at(10));

#if EnableStressTensorSphere == SimMiscEnabled
	// Now the diagonal elements of the total stress
	
	if(m_bStress)
	{
	    m_pSrr->AddData(m_vObservables.at(10));
	    m_pStt->AddData(m_vObservables.at(11));
	    m_pStressProfile->AddData(m_vObservables.at(12));
	
	    m_pSrr->ClearData();
	    m_pStt->ClearData();
	    m_pStressProfile->ClearData();
		
		// Tell the SimBox to clear the stress grid cells ready for the next sample.
		
		
	}
#endif

	// Store the data in a TSD object and pass it to the aggregate state object.

	long dataTotal = 8;

	CTimeSeriesData* m_pTSD = new CTimeSeriesData(dataTotal);

	m_pTSD->SetValue(0, pISimBox->GetCurrentTime(), "Time");
	m_pTSD->SetValue(1, m_Size,						"Size");
	m_pTSD->SetValue(2, m_Radius,					"Radius");
	m_pTSD->SetValue(3, m_Rg2,						"Rg2");
	m_pTSD->SetValue(4, m_Eccentricity,				"Eccentricity");
	m_pTSD->SetValue(5, m_DiffCoeff,				"Diffusion");
	m_pTSD->SetValue(6, m_SurfaceTensionTrap,		"TrapezoidalTension");
	m_pTSD->SetValue(7, m_SurfaceTensionSimpson,	"SimpsonTension");

	m_pState->AddTimeSeriesData(m_pTSD);
}

// Private helper function to calculate the stress profile across the vesicle membrane using the results of the CMonitor/CSimBox/CCNTCell calculation
// that is initiated in the CMonitor::CalculateStressSphere() function. It is conditionally compiled using a flag in SimMiscellaneousFlags.h. The function
// assumes that the stress tensir stored locally in the CNT cells has already been transformed into a spherical polar coordinate system.
//
// The number of spherical shells in the profile is determined by an input parameter, but the radial distance out to which the profile is calculated is
// just taken as the smallest side-length of the simulation box.

void CVesicle::StressProfile(const ISimBox* const pISimBox)
{
#if EnableStressTensorSphere == SimMiscEnabled

	// Iterate over the stress cells, retrieve the local spherical stress tensor for each one and average the stress over concentric spheres centred 
	// on the centre of the vesicle.
	
	double maxRadius = 0.0;
	
	if(pISimBox->GetSimSpaceZLength() <= pISimBox->GetSimSpaceXLength())
	{
	    if(pISimBox->GetSimSpaceZLength() <= pISimBox->GetSimSpaceYLength())
		{
            maxRadius = 0.5*pISimBox->GetSimSpaceZLength();  
		}
		else
		{
            maxRadius = 0.5*pISimBox->GetSimSpaceYLength();  
		}
	}
	else if(pISimBox->GetSimSpaceXLength() <= pISimBox->GetSimSpaceYLength())
	{
		maxRadius = 0.5*pISimBox->GetSimSpaceXLength();  
	}
	else
	{
		maxRadius = 0.5*pISimBox->GetSimSpaceYLength();  	
	}
	
	const double deltaRadius  = maxRadius/static_cast<double>(m_ShellTotal);  // Width of each concentric spherical shell
		
	for(long in=0; in<m_ShellTotal; in++)
	{
	    m_vStressCounter.at(in)    = 0.0;
	    m_vRadialStress.at(in)     = 0.0;
		m_vTransverseStress.at(in) = 0.0;
		m_vStressProfile.at(in)    = 0.0;
	    m_vTensionProfile.at(in)   = 0.0;
	}
	
	m_vTensionProfile.at(m_ShellTotal) = 0.0;  // extra location for Simpson's rule
	
	double cellXPos   = 0.0;
	double cellYPos   = 0.0;
	double cellZPos   = 0.0;
	double cellRadius = 0.0;
	
	long index        = 0;
	long cellIndex    = 0;
		
	StressCellVector vCells = pISimBox->GetStressCells();
	for(StressCellIterator iterCell=vCells.begin(); iterCell!=vCells.end(); iterCell++)
	{
		cellXPos = 0.5*((*iterCell)->GetBLXCoord() + (*iterCell)->GetTRXCoord()) - m_CMXPos;
		cellYPos = 0.5*((*iterCell)->GetBLYCoord() + (*iterCell)->GetTRYCoord()) - m_CMYPos;
		cellZPos = 0.5*((*iterCell)->GetBLZCoord() + (*iterCell)->GetTRZCoord()) - m_CMZPos;
		
		cellRadius = sqrt(cellXPos*cellXPos + cellYPos*cellYPos + cellZPos*cellZPos);
		
		index = static_cast<long>(cellRadius/deltaRadius);
		
		// Discard any cells that are outside the maximum radius or have negative index
		if(index < m_ShellTotal)
		{
		    m_vStressCounter.at(index)    += 1.0;
		    m_vRadialStress.at(index)     += (*iterCell)->GetStress11();
		    m_vTransverseStress.at(index) += 0.5*( (*iterCell)->GetStress22() + (*iterCell)->GetStress33() );
		}
		
		(*iterCell)->Clear();
	}
	
	// We average the stress tensor over each concentric shell using the midpoint of each shell as the independent coordinate for summing in the 
	// Trapezoidal or Simpson's rules. The volume of each shell is taken as the volume of all of the stress grid cells that belong it.
	// We also divide by the sampling period as the stress grid cells have accumulated the stress since the last sampling period.
	// Note that we only integrate over the range defined by m_ShellMin to m_ShellMax to avoid large fluctuations that occur at small and
	// large values of the radial coordinate.
	
	
	double r = 0.5*deltaRadius;
	
	xxBasevectorIterator iterStress1 = m_pSrr->Begin();
	xxBasevectorIterator iterStress2 = m_pStt->Begin();
	xxBasevectorIterator iterStress3 = m_pStressProfile->Begin();
	
	for(long in=0; in<m_ShellTotal; in++)
	{
	    if(m_vStressCounter[in] > 0.0)
		{
		    m_vStressCounter[in]        /= m_StressCellRatio;
		    m_vRadialStress.at(in)     /= (static_cast<double>(m_SamplePeriod)*m_vStressCounter[in]);
		    m_vTransverseStress.at(in) /= (static_cast<double>(m_SamplePeriod)*m_vStressCounter[in]);
			m_vStressProfile.at(in)    = m_vRadialStress.at(in) - m_vTransverseStress.at(in);
			
			if(in >= m_ShellMin && in < m_ShellTotal)
			{
			    m_vTensionProfile.at(in) = r*r*m_vStressProfile.at(in);
			}
			else
			{
			    m_vTensionProfile.at(in) = 0.0;
			}
						
//			std::cout << r << "  "  << m_vStressProfile.at(in) << zEndl;
		}
		
		// Add the stress profiles to their observables for averaging
		
		*iterStress1 = new aaScalar(m_vRadialStress.at(in));
		*iterStress2 = new aaScalar(m_vTransverseStress.at(in));
		*iterStress3 = new aaScalar(m_vStressProfile.at(in));
		++iterStress1;
		++iterStress2;
		++iterStress3;

//		m_vTensionProfile.at(in) = r*r;  // Debug test function: loop must use m_ShellTotal+1 for correct integral
	    r += deltaRadius;		
	}


	// We approximate the surface tension integral using the Trapezoidal rule and Simpson's rule to see which is better.
		
	m_SurfaceTensionTrap    = TrapezoidalRule(deltaRadius)/(m_Radius*m_Radius);
	m_SurfaceTensionSimpson = SimpsonsRule(deltaRadius)/(m_Radius*m_Radius);
		
	// Write out the results for debug
	
	std::cout << "Vesicle with Radius/deltaRadius " << m_Radius << " " << deltaRadius << " centred on: " << m_CMXPos << " " << m_CMYPos << " " << m_CMZPos <<  " has surface tension " << m_SurfaceTensionTrap << " " << m_SurfaceTensionSimpson  << zEndl;

#endif
}
// Private helper function to calculate the stress profile across the whole SimBox in slices with Z normal. It is used to compare the spherical stress tensor 
// calculation with the bilayer calculation to see if they agree for debug purposes.

void CVesicle::PlanarStressProfile(const ISimBox* const pISimBox)
{
#if EnableStressTensorSphere == SimMiscEnabled


#endif
}


// Trapezoidal approximation to the surface tension integral. We use the stored values of the stress profile calculated above. The infinitesimal width
// of the bins is passed in as the argument.

double CVesicle::TrapezoidalRule(const double h)
{
	// As a debug test, we can calculate the integralof r*r over the interval 0 to LZ. Note that we need to use m_ShellTotal+1 points to make sure
	// that we include the end-points required in the Trapezoidal rule correctly. Also, the x coordinate (r above) should start at 0.0 for the test
	// otherwise we are integrating from h/2 to LZ+h/2.
	//
	// If we take m_ShellTotal = 100, we have points from 0 to 100, ie 101 points, so we need to add the final point in the vector separately
    // in = 1 to 99, and we add in the first and last points
	
	double tension = 0.0;
	
#if EnableStressTensorSphere == SimMiscEnabled

	for(long in=1; in<m_ShellTotal; in++)
	{
		tension += m_vTensionProfile.at(in);
	}
	
	tension += 0.5*(m_vTensionProfile.at(0) + m_vTensionProfile.at(m_ShellTotal));
	tension *= h;
	
#endif

    return tension;
}

// Simpson's Rule approximation to the surface tension integral. We use the stored values of the stress profile calculated above. But note
// that the integrand must be defined at an even number of points. If it is not, we ignore the final point.
// Note that even though Simpson's rule appears to need 2n points, it actually needs 2n+1 as the zeroth point is included in the sum.

double CVesicle::SimpsonsRule(const double h)
{
	double tension = 0.0;
	
#if EnableStressTensorSphere == SimMiscEnabled

    bool bEven = true;	
	long total = m_ShellTotal/2;
	
    if(m_ShellTotal%2 != 0)
	{
	    bEven = false;
		total = (m_ShellTotal-1)/2;
	}
	
	long oddIndex = 0;
	long evenIndex = 0;
	
	double oddSum  = 0.0;
	double evenSum = 0.0;
	
	// As a debug test, we can calculate the integralof r*r over the interval 0 to LZ. Note that we need to use m_ShellTotal+1 points to make sure
	// that we include the end-points required in the Trapezoidal rule correctly. Also, the x coordinate (r above) should start at 0.0 for the test
	// otherwise we are integrating from h/2 to LZ+h/2.
	// If we take m_ShellTotal = 100, so total = 50, we have the following two sums:
	//
	// We expect the oddIndex = 1 up to 99, and evenIndex = 2 up to 98 then we add the 0 and 100th elements separately
	//
	// in = 0 gives oddIndex = 1, evenIndex = 2
	// in = 49 gives oddIndex = 99, evenIndex = 100
	//
	// Elements 0 and 100 are added separately. The 100th element must be subtracted to compensate for the extra term in the even sum.

	for(long in=0; in<total; in++)
	{
	    oddIndex  = 2*in+1;
	    evenIndex = 2*in+2;
		
        oddSum  += m_vTensionProfile.at(oddIndex);
		evenSum += m_vTensionProfile.at(evenIndex);
	}
	

	if(bEven)
	{
	    tension = m_vTensionProfile.at(0) - m_vTensionProfile.at(m_ShellTotal) + 4.0*oddSum + 2.0*evenSum;
	}
	else
	{
	    tension = m_vTensionProfile.at(0) - m_vTensionProfile.at(m_ShellTotal-1) + 4.0*oddSum + 2.0*evenSum;	
	}
	
	tension *= (0.33333333*h);
	
#endif

    return tension;
}

// **********************************************************************
// IEventXXX interface functions for all supported events.

// IEventVesicleBindsWall and IEventVesicleUnbindsWall
// ***************************************************
//

bool CVesicle::VesicleBindsWallSupported() const
{
	return true;
}

bool CVesicle::VesicleUnbindsWallSupported() const
{
	return true;
}


// **********************************************************************
// IVesicleXXX interface functions for all supported processes.
//
// IVesicleFusion
// **************
//

// Functions to return the CM coordinates of the vesicle. Currently, these
// are calculated from the positions of the lipid head beads only.

double CVesicle::GetInitialCMXPos() const
{
	return m_InitialCMXPos;
}

double CVesicle::GetInitialCMYPos() const
{
	return m_InitialCMYPos;
}

double CVesicle::GetInitialCMZPos() const
{
	return m_InitialCMZPos;
}

double CVesicle::GetCMXPos() const
{
	return m_CMXPos;
}

double CVesicle::GetCMYPos() const
{
	return m_CMYPos;
}

double CVesicle::GetCMZPos() const
{
	return m_CMZPos;
}

double CVesicle::GetRadius() const
{
	return m_Radius;
}


long CVesicle::GetMajorLipidType() const
{
	return m_LipidType;
}

long CVesicle::GetMajorLipidHeadType() const
{
	return m_LipidHeadType;
}

long CVesicle::GetMajorLipidTailType() const
{
	return m_LipidTailType;
}

// **********************************************************************
// Access functions to vesicle analysis quantities
//
//


