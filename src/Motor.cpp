/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Motor.cpp: implementation of the CMotor class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "Motor.h"		
#include "MotorData.h"
#include "SimState.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Bond.h"
#include "aaRegionToType.h"

#include "SliceProfile.h"
#include "RowProfileSet.h"
#include "CellProfileSet.h"
#include "CNTCellProfile.h"

#include "aaScalarSingle.h"				// Types of data that can be sent as observables
#include "aaVectorSingle.h"
#include "aaScalarProfile.h"
#include "aaVectorProfile.h"

#include "ScalarObservable.h"			// Types of observables
#include "VectorObservable.h"
#include "TensorObservable.h"
#include "ScalarProfileObservable.h"

#include "TimeSeriesData.h"

#include "Builder.h"					// Needed to access pi




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We pass the CAnalysisData-derived object to the CAnalysis base class for 
// initialisation of all general analysis data. The pointer is passed as
// const as we do not expect it to be altered.

CMotor::CMotor(const zString runId, long samplePeriod, 
			   const CMotorData* const pAD) : CAnalysis(runId, samplePeriod, pAD),
												    m_Polymer(pAD->m_Polymer),
												    m_Fuel(pAD->m_Fuel),
												    m_Solvent(pAD->m_Solvent),
													m_pTSD(NULL),
													m_MotorTotal(0),
													m_MotorType(0),
													m_MotorHeadType(0),
													m_MotorNeckType(0),
													m_MotorHingeType(0),
													m_MotorTailType(0),
													m_FuelType(0),
													m_FuelHeadType(0),
													m_SolventType(0),
													m_pFuelSP(NULL),
													m_pFuelDensity(NULL)

{
	m_lMotors.clear();
	m_lMotorHead.clear();
	m_lMotorNeck.clear();
	m_lMotorHinge.clear();

	m_mPolymerFromHeadId.clear();
	m_mPolymerFromTailId.clear();
}

CMotor::~CMotor()
{
	if(m_pFuelSP)
	{
		delete m_pFuelSP;
		m_pFuelSP = NULL;
	}

	if(m_pFuelDensity)
	{
		delete m_pFuelDensity;
		m_pFuelDensity = NULL;
	}


}

// **********************************************************************
// Function to analyse the properties of a motor made up of a single 
// polymer.
//
// **********************************************************************

void CMotor::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	// Counters and iterator used many times below. I have to declare them because
	// different machines may or may not allow them to be redefined in different loops
	// in the same scope.


	// **********************************************************************

	if(m_MotorTotal == 0)
	{
		// Count the number of motor polymers that form separate aggregates

		m_MotorTotal = count_if(rSimState.GetPolymers().begin(), rSimState.GetPolymers().end(), aaPolymerType(m_MotorType));

		// Create maps allowing polymer pointers to be obtained given head and tail bead
		// ids as keys.

		for(cPolymerVectorIterator iterPoly=rSimState.GetPolymers().begin(); iterPoly!=rSimState.GetPolymers().end(); iterPoly++)
		{
			if((*iterPoly)->GetType() == m_MotorType)
			{
				m_lMotors.push_back(*iterPoly);

				// Now get pointers to the motors head, neck and hinge beads.
				// Note that this relies on the motor polymer architecture having
				// the correct form, viz, (Head Neck Hinge Tail ...)

				CBond* firstBond  = (*iterPoly)->GetBonds().at(0);
				CBond* secondBond = (*iterPoly)->GetBonds().at(1);
				CBond* thirdBond  = (*iterPoly)->GetBonds().at(2);

				CAbstractBead* pHead	= firstBond->GetHead();
				CAbstractBead* pNeck	= secondBond->GetHead();
				CAbstractBead* pHinge	= thirdBond->GetHead();

				m_MotorNeckType  = pNeck->GetType();
				m_MotorHingeType = pHinge->GetType();

				m_lMotorHead.push_back(pHead);
				m_lMotorNeck.push_back(pNeck);
				m_lMotorHinge.push_back(pHinge);


				m_mPolymerFromHeadId.insert(zPairLongPolymer((*iterPoly)->GetHead()->GetId(), *iterPoly));
				m_mPolymerFromTailId.insert(zPairLongPolymer((*iterPoly)->GetTail()->GetId(), *iterPoly));
			}
		}


		// Create a 1d scalar profile to hold the fuel density in slices

		m_pFuelDensity	= new aaScalarProfile(10);

	}




	// **********************************************************************
	// Find the CNT cell containing the motor polymer head(s) and the 
	// surrounding cells so that we can find out whether fuel and the 
	// filament are nearby.
	//
	// We assume only a single motor for now...

	m_vMotorCellIndices.clear();


	long index, ix, iy, iz;

	const CAbstractBead* pHead = m_lMotorHead.front();

	ix = static_cast<long>(pHead->GetXPos()/pISimBox->GetCNTXCellWidth());
	iy = static_cast<long>(pHead->GetYPos()/pISimBox->GetCNTYCellWidth());
	iz = static_cast<long>(pHead->GetZPos()/pISimBox->GetCNTZCellWidth());		

	index = pISimBox->GetCNTXCellNo()*(pISimBox->GetCNTYCellNo()*iz+iy) + ix;

	m_vMotorCellIndices.push_back(index);

	const CCNTCell* const pCell = pISimBox->GetCNTCells().at(index);

	CCNTCellProfile nnCells;

	for(short int i=0; i<27; i++)
	{
		nnCells.AddCell(pCell->GetNNCell(i));
	}

	// **********************************************************************
	// Get the CNT cells that contain fuel polymers for the motor. We identify
	// the fuel by the type of its head bead. We create a new slice profile
	// for ach call so that we can get the latest coordinates of the polymers.
	// We define a Z oriented profile by convention.

	if(m_pFuelSP)
			delete m_pFuelSP;

	m_pFuelSP	= new CSliceProfile(10, 0, 0, 1, pISimBox);


	// Remove those slices that don't contain any fuel polymers

	CSliceProfile fuelSP;

	for(cSliceVectorIterator iterSlice=m_pFuelSP->Begin(); iterSlice!=m_pFuelSP->End(); iterSlice++)
	{
		BeadList lBeads = (*iterSlice)->GetBeads();

		cBeadListIterator iterBead = find_if(lBeads.begin(), lBeads.end(), aaBeadType(m_FuelHeadType));

		if(iterBead!=lBeads.end())	// Slice contains lipid head/tail beads
		{
			CSlice* pSlice = new CSlice(**iterSlice);
			fuelSP.AddRegion(pSlice);
		}
	}

	// Check the density profile of lipid head and tail beads in the whole SimBox

	transform(m_pFuelSP->Begin(), m_pFuelSP->End(), m_pFuelDensity->Begin(), aaDensity(m_FuelHeadType));

	// Is there any fuel in the neighbouring CNT cells?

	zLongVector nnCellFuelProfile(nnCells.Size());

	transform(nnCells.Begin(), nnCells.End(), nnCellFuelProfile.begin(), aaCNTCellNumber(m_FuelHeadType));


	for(zLongVectorIterator iter=nnCellFuelProfile.begin(); iter!=nnCellFuelProfile.end(); iter++)
	{



	}

	// Delete the data on the fuel density profile ready for the next call of this routine

	m_pFuelDensity->ClearData();


}

// **********************************************************************
// Function to store the integer ids for beads, bonds and polymers given 
// the strings that were entered by the user in the control data file.
//
// **********************************************************************

void CMotor::ConvertNames(const CAnalysisState &raState)
{
	m_MotorType			= raState.GetPolymerTypeFromName(m_Polymer);
	m_MotorHeadType		= raState.GetPolymerHeadType(m_MotorType);
	m_MotorTailType		= raState.GetPolymerTailType(m_MotorType);

	m_FuelType			= raState.GetPolymerTypeFromName(m_Fuel);
	m_FuelHeadType		= raState.GetPolymerHeadType(m_FuelType);

	m_SolventType		= raState.GetPolymerTypeFromName(m_Solvent);


}


