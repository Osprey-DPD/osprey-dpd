/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aaStressTensor.cpp: implementation of the aaStressTensor class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aaStressTensor.h"
#include "aaStressTensorPoint.h"
#include "ISimBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Note that the number of steps per CNT cell is defined in the derived
// classes because we don't have access to the CProfile<> object here.

aaStressTensor::aaStressTensor(const ISimBox* const pISimBox, 
							   long xn, long yn, long zn) : m_pISimBox(pISimBox),
															m_BeadTypeTotal(pISimBox->GetBeadTypeTotal()),
															m_BondTypeTotal(pISimBox->GetBondTypeTotal()),
															m_BondPairTypeTotal(pISimBox->GetBondPairTypeTotal()),
															m_BeadPairTypeTotal(m_BeadTypeTotal*m_BeadTypeTotal),
															m_NormalIndex(0),
															m_StressGridSize(0),
															m_Width(0.0),
															m_Volume(0.0)
{
	m_Normal[0]	= xn;
	m_Normal[1]	= yn;
	m_Normal[2]	= zn;

	m_vProfile.clear();  // Vector of aaStressTensorPoints
}

// Delete the stored aaStressTensorPoint objects if they have been created.

aaStressTensor::~aaStressTensor()
{
	if(!m_vProfile.empty())
	{
		for(StressVectorIterator iterS=m_vProfile.begin(); iterS!=m_vProfile.end(); iterS++)
		{
			delete *iterS;
		}
		m_vProfile.clear();
	}
}

// Function to add an aaStressTensorPoint to the stress profile. The stress tensor
// averaged over a specific region (slice, row or cell) is calculated from the
// CMonitor data by the derived classes but stored here. We pass in the number 
// of bead types so that it can create enough storage for all bead-bead pairs.
// The stress is broken down into the contributions from several bead-bead pairs.

void aaStressTensor::AddStressTensorPoint()
{
	m_vProfile.push_back(new aaStressTensorPoint(m_BeadTypeTotal, m_BondTypeTotal, m_BondPairTypeTotal));
}

void aaStressTensor::AddBeadStress(long gridIndex, long beadIndex, const double con[9], const double kin[9]) const
{
	m_vProfile.at(gridIndex)->AddBeadStress(beadIndex, con, kin);
}

void aaStressTensor::AddBondStress(long gridIndex, long bondIndex, const double con[9], const double kin[9]) const
{
	m_vProfile.at(gridIndex)->AddBondStress(bondIndex, con, kin);
}

void aaStressTensor::AddBondPairStress(long gridIndex, long bondPairIndex, const double con[9], const double kin[9]) const
{
	m_vProfile.at(gridIndex)->AddBondPairStress(bondPairIndex, con, kin);
}

// Multiply in the constant factors to the configurational and kinetic contributions
// to the stress tensor. These arise from the volume of the grid regions and other
// terms.

void aaStressTensor::MultiplyStressTensor(double conFactor, double kinFactor) const
{
	for(cStressVectorIterator iterGrid=m_vProfile.begin(); iterGrid!=m_vProfile.end(); iterGrid++)
	{
		(*iterGrid)->MultiplyStressTensor(conFactor, kinFactor);
	}
}

// Forwarding functions to access the stress tensor components stored in the
// aaStressTensorPoint profile.

double aaStressTensor::GetBeadStress11(long slice, long beadPair) const
{
	return m_vProfile.at(slice)->GetBeadStress11(beadPair);
}

double aaStressTensor::GetBeadStress12(long slice, long beadPair) const
{
	return m_vProfile.at(slice)->GetBeadStress12(beadPair);
}

double aaStressTensor::GetBeadStress13(long slice, long beadPair) const
{
	return m_vProfile.at(slice)->GetBeadStress13(beadPair);
}

double aaStressTensor::GetBeadStress21(long slice, long beadPair) const
{
	return m_vProfile.at(slice)->GetBeadStress21(beadPair);
}

double aaStressTensor::GetBeadStress22(long slice, long beadPair) const
{
	return m_vProfile.at(slice)->GetBeadStress22(beadPair);
}

double aaStressTensor::GetBeadStress23(long slice, long beadPair) const
{
	return m_vProfile.at(slice)->GetBeadStress23(beadPair);
}

double aaStressTensor::GetBeadStress31(long slice, long beadPair) const
{
	return m_vProfile.at(slice)->GetBeadStress31(beadPair);
}

double aaStressTensor::GetBeadStress32(long slice, long beadPair) const
{
	return m_vProfile.at(slice)->GetBeadStress32(beadPair);
}

double aaStressTensor::GetBeadStress33(long slice, long beadPair) const
{
	return m_vProfile.at(slice)->GetBeadStress33(beadPair);
}

// Function to return a specified component of the stress tensor for a 
// specified region in the stress tensor profile.

double aaStressTensor::GetComponent(long index, long component) const
{
	return m_vProfile.at(index)->GetComponent(component);
}
