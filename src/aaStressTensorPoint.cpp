/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aaStressTensorPoint.cpp: implementation of the aaStressTensorPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aaStressTensorPoint.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Pass in the number of bead, bond and bondpair types in the simulation, 
// calculate how many bead pairs there are, and zero the stress tensor
// arrays for bead-bead, bond and bondpair interactions.

aaStressTensorPoint::aaStressTensorPoint(long beadTypes, long bondTypes, 
										 long bondPairTypes) :	m_BeadTypeTotal(beadTypes),
																m_BondTypeTotal(bondTypes),
																m_BondPairTypeTotal(bondPairTypes),
																m_BeadPairTotal(beadTypes*beadTypes)
{
	// Initialize the bead-bead stress components to zero

	m_vBeadCon11.resize(m_BeadPairTotal);
	m_vBeadCon12.resize(m_BeadPairTotal);
	m_vBeadCon13.resize(m_BeadPairTotal);
	m_vBeadCon21.resize(m_BeadPairTotal);
	m_vBeadCon22.resize(m_BeadPairTotal);
	m_vBeadCon23.resize(m_BeadPairTotal);
	m_vBeadCon31.resize(m_BeadPairTotal);
	m_vBeadCon32.resize(m_BeadPairTotal);
	m_vBeadCon33.resize(m_BeadPairTotal);

	for(long i=0; i<m_BeadPairTotal; i++)
	{
		m_vBeadCon11.at(i) = 0.0;
		m_vBeadCon12.at(i) = 0.0;
		m_vBeadCon13.at(i) = 0.0;
		m_vBeadCon21.at(i) = 0.0;
		m_vBeadCon22.at(i) = 0.0;
		m_vBeadCon23.at(i) = 0.0;
		m_vBeadCon31.at(i) = 0.0;
		m_vBeadCon32.at(i) = 0.0;
		m_vBeadCon33.at(i) = 0.0;
	}

	// Now the bond stress components

	m_vBondCon11.resize(m_BondTypeTotal);
	m_vBondCon12.resize(m_BondTypeTotal);
	m_vBondCon13.resize(m_BondTypeTotal);
	m_vBondCon21.resize(m_BondTypeTotal);
	m_vBondCon22.resize(m_BondTypeTotal);
	m_vBondCon23.resize(m_BondTypeTotal);
	m_vBondCon31.resize(m_BondTypeTotal);
	m_vBondCon32.resize(m_BondTypeTotal);
	m_vBondCon33.resize(m_BondTypeTotal);

	for(long j=0; j<m_BondTypeTotal; j++)
	{
		m_vBondCon11.at(j) = 0.0;
		m_vBondCon12.at(j) = 0.0;
		m_vBondCon13.at(j) = 0.0;
		m_vBondCon21.at(j) = 0.0;
		m_vBondCon22.at(j) = 0.0;
		m_vBondCon23.at(j) = 0.0;
		m_vBondCon31.at(j) = 0.0;
		m_vBondCon32.at(j) = 0.0;
		m_vBondCon33.at(j) = 0.0;
	}

	// Finally, the bondpair stress contributions

	m_vBondPairCon11.resize(m_BondPairTypeTotal);
	m_vBondPairCon12.resize(m_BondPairTypeTotal);
	m_vBondPairCon13.resize(m_BondPairTypeTotal);
	m_vBondPairCon21.resize(m_BondPairTypeTotal);
	m_vBondPairCon22.resize(m_BondPairTypeTotal);
	m_vBondPairCon23.resize(m_BondPairTypeTotal);
	m_vBondPairCon31.resize(m_BondPairTypeTotal);
	m_vBondPairCon32.resize(m_BondPairTypeTotal);
	m_vBondPairCon33.resize(m_BondPairTypeTotal);

	for(long k=0; k<m_BondPairTypeTotal; k++)
	{
		m_vBondPairCon11.at(k) = 0.0;
		m_vBondPairCon12.at(k) = 0.0;
		m_vBondPairCon13.at(k) = 0.0;
		m_vBondPairCon21.at(k) = 0.0;
		m_vBondPairCon22.at(k) = 0.0;
		m_vBondPairCon23.at(k) = 0.0;
		m_vBondPairCon31.at(k) = 0.0;
		m_vBondPairCon32.at(k) = 0.0;
		m_vBondPairCon33.at(k) = 0.0;
	}

	// Now the combined stress tensor for all bead-pairs

	for(short ii=0; ii<9; ii++)
	{
		m_Config[ii] = 0.0;
	}
}

aaStressTensorPoint::~aaStressTensorPoint()
{
}

// Function to add the contribution from a given bead-bead pair to an 
// existing stress tensor. The index into the vectors uses the two bead types,
// which lie between 0 and N-1, to create an integer in the range [0, N*N-1].
// The stress is also added to the total configurational stress tensor.

void aaStressTensorPoint::AddBeadStress(long index, const double con[9], const double kin[9])
{
	m_vBeadCon11.at(index)	+= con[0];
	m_vBeadCon12.at(index)	+= con[1];
	m_vBeadCon13.at(index)	+= con[2];
	m_vBeadCon21.at(index)	+= con[3];
	m_vBeadCon22.at(index)	+= con[4];
	m_vBeadCon23.at(index)	+= con[5];
	m_vBeadCon31.at(index)	+= con[6];
	m_vBeadCon32.at(index)	+= con[7];
	m_vBeadCon33.at(index)	+= con[8];

	// Now add to the total stress tensor

	for(short i=0; i<9; i++)
	{
		m_Config[i]  += con[i];
	}
}

// Function to add the contribution from a given bond to an 
// existing stress tensor. The index into the vectors uses the bond type.
// The stress is also added to the total configurational stress tensor.

void aaStressTensorPoint::AddBondStress(long index, const double con[9], const double kin[9])
{
	m_vBondCon11.at(index)	+= con[0];
	m_vBondCon12.at(index)	+= con[1];
	m_vBondCon13.at(index)	+= con[2];
	m_vBondCon21.at(index)	+= con[3];
	m_vBondCon22.at(index)	+= con[4];
	m_vBondCon23.at(index)	+= con[5];
	m_vBondCon31.at(index)	+= con[6];
	m_vBondCon32.at(index)	+= con[7];
	m_vBondCon33.at(index)	+= con[8];

	// Now add to the total stress tensor

	for(short i=0; i<9; i++)
	{
		m_Config[i]  += con[i];
	}
}

// Function to add the contribution from a given bead-bead pair to an 
// existing stress tensor. The index into the vectors uses the bondpair type.
// The stress is also added to the total configurational stress tensor.

void aaStressTensorPoint::AddBondPairStress(long index, const double con[9], const double kin[9])
{
	m_vBondPairCon11.at(index)	+= con[0];
	m_vBondPairCon12.at(index)	+= con[1];
	m_vBondPairCon13.at(index)	+= con[2];
	m_vBondPairCon21.at(index)	+= con[3];
	m_vBondPairCon22.at(index)	+= con[4];
	m_vBondPairCon23.at(index)	+= con[5];
	m_vBondPairCon31.at(index)	+= con[6];
	m_vBondPairCon32.at(index)	+= con[7];
	m_vBondPairCon33.at(index)	+= con[8];

	// Now add to the total stress tensor

	for(short i=0; i<9; i++)
	{
		m_Config[i]  += con[i];
	}
}

// Function to allow the configurational stress tensor components
// to be multiplied by arbitrary constants.

void aaStressTensorPoint::MultiplyStressTensor(double cx, double kx)
{
	long index;

	for(index=0; index<m_BeadPairTotal; index++)
	{
		m_vBeadCon11.at(index)	*= cx;
		m_vBeadCon12.at(index)	*= cx;
		m_vBeadCon13.at(index)	*= cx;
		m_vBeadCon21.at(index)	*= cx;
		m_vBeadCon22.at(index)	*= cx;
		m_vBeadCon23.at(index)	*= cx;
		m_vBeadCon31.at(index)	*= cx;
		m_vBeadCon32.at(index)	*= cx;
		m_vBeadCon33.at(index)	*= cx;
	}

	for(index=0; index<m_BondTypeTotal; index++)
	{
		m_vBondCon11.at(index)	*= cx;
		m_vBondCon12.at(index)	*= cx;
		m_vBondCon13.at(index)	*= cx;
		m_vBondCon21.at(index)	*= cx;
		m_vBondCon22.at(index)	*= cx;
		m_vBondCon23.at(index)	*= cx;
		m_vBondCon31.at(index)	*= cx;
		m_vBondCon32.at(index)	*= cx;
		m_vBondCon33.at(index)	*= cx;
	}

	for(index=0; index<m_BondPairTypeTotal; index++)
	{
		m_vBondPairCon11.at(index)	*= cx;
		m_vBondPairCon12.at(index)	*= cx;
		m_vBondPairCon13.at(index)	*= cx;
		m_vBondPairCon21.at(index)	*= cx;
		m_vBondPairCon22.at(index)	*= cx;
		m_vBondPairCon23.at(index)	*= cx;
		m_vBondPairCon31.at(index)	*= cx;
		m_vBondPairCon32.at(index)	*= cx;
		m_vBondPairCon33.at(index)	*= cx;
	}

	// Now multiply the total stress tensor components

	for(short i=0; i<9; i++)
	{
		m_Config[i]  *= cx;
	}

}

// Functions to return a particular bead-bead contribution to the stress profile.
// Note that because the contributions are stored using an index constructed 
// from the two bead types, e.g., head-water, and the off-diagonal terms have
// not been combined, i.e., head-water contains different bead pairs than 
// water-head, we need to add the two off-diagonal elements. For the diagonal 
// elements this is not necessary.

double aaStressTensorPoint::GetBeadStressXProfile(long index) const
{
	const long index2 = (index%m_BeadTypeTotal)*m_BeadTypeTotal + index/m_BeadTypeTotal;

	if(index2 == index)
		return GetBeadStress11(index) - 0.5*(GetBeadStress22(index) + GetBeadStress33(index));
	else
	{
		return GetBeadStress11(index)  + GetBeadStress11(index2) - 0.5*(GetBeadStress22(index) + GetBeadStress22(index2) + GetBeadStress33(index) + GetBeadStress33(index2));
	}
}

double aaStressTensorPoint::GetBeadStressYProfile(long index) const
{
	const long index2 = (index%m_BeadTypeTotal)*m_BeadTypeTotal + index/m_BeadTypeTotal;

	if(index2 == index)
		return GetBeadStress22(index) - 0.5*(GetBeadStress33(index) + GetBeadStress11(index));
	else
	{
		return GetBeadStress22(index) + GetBeadStress22(index2) - 0.5*(GetBeadStress33(index) + GetBeadStress33(index2) + GetBeadStress11(index) + GetBeadStress11(index2));
	}
}

double aaStressTensorPoint::GetBeadStressZProfile(long index) const
{
	const long index2 = (index%m_BeadTypeTotal)*m_BeadTypeTotal + index/m_BeadTypeTotal;

	if(index2 == index)
		return GetBeadStress33(index) - 0.5*(GetBeadStress11(index) + GetBeadStress22(index));
	else
	{
		return GetBeadStress33(index) + GetBeadStress33(index2) - 0.5*(GetBeadStress11(index) + GetBeadStress11(index2) + GetBeadStress22(index) + GetBeadStress22(index2));
	}
}

// Functions to return a particular bond's contribution to the stress profile.

double aaStressTensorPoint::GetBondStressXProfile(long index) const
{
	return GetBondStress11(index) - 0.5*(GetBondStress22(index) + GetBondStress33(index));
}

double aaStressTensorPoint::GetBondStressYProfile(long index) const
{
	return GetBondStress22(index) - 0.5*(GetBondStress33(index) + GetBondStress11(index));
}

double aaStressTensorPoint::GetBondStressZProfile(long index) const
{
	return GetBondStress33(index) - 0.5*(GetBondStress11(index) + GetBondStress22(index));
}

// Functions to return a particular bondpair's contribution to the stress profile.

double aaStressTensorPoint::GetBondPairStressXProfile(long index) const
{
	return GetBondPairStress11(index) - 0.5*(GetBondPairStress22(index) + GetBondPairStress33(index));
}

double aaStressTensorPoint::GetBondPairStressYProfile(long index) const
{
	return GetBondPairStress22(index) - 0.5*(GetBondPairStress33(index) + GetBondPairStress11(index));
}

double aaStressTensorPoint::GetBondPairStressZProfile(long index) const
{
	return GetBondPairStress33(index) - 0.5*(GetBondPairStress11(index) + GetBondPairStress22(index));
}
