/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// BondPairType.cpp: implementation of the CBondPairType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "BondPairType.h"


//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////
// Boolean function to check that two CBondPair types have the same names.
// We don't compare their bending modulus and relaxed angle because we want
// to be able to alter these parameters when doing a restart. 
// This is only used by CInputData when checking the data from a restart
// control data file.

bool operator==(const CBondPairType& a, const CBondPairType& b)
{
	if( a.m_Name1 == b.m_Name1 && a.m_Name2 == b.m_Name2  && a.m_Name3 == b.m_Name3)
		return true;
	else
		return false;
}


//////////////////////////////////////////////////////////////////////
// Static member definitions
//////////////////////////////////////////////////////////////////////

long CBondPairType::m_BondPairTypeTotal = 0;	// No of bond types created so far

long CBondPairType::GetTotal()
{
	return m_BondPairTypeTotal;
}

// This function is called by CInputDPDFile and CInputMDFile to reset the counter
// for bond types created between simulations and when reading a previous control 
// data file for a restart simulation.

void CBondPairType::ZeroTotal()
{
	m_BondPairTypeTotal = 0;
}

CBondPairType* CBondPairType::AddBondPairType(zString name1, zString name2, zString name3, double Strength, double Phi0)
{
	m_BondPairTypeTotal++;

	return new CBondPairType(name1, name2, name3, Strength, Phi0);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Protected constructor to prevent any creation of CBondPairTypes except using
// the static member function above.

CBondPairType::CBondPairType(zString name1, zString name2, zString name3, 
							 double Strength, double Phi0) : m_Name1(name1),
															 m_Name2(name2),
															 m_Name3(name3),
															 m_BendingModulus(Strength)
															 
{
	// Convert the relaxed bond angle to radians

	m_Phi0 = Phi0*xxBase::m_globalPI/180.0;
}

CBondPairType::~CBondPairType()
{

}

// Functions to set the values of the stiff bond's bending modulus and
// preferred angle. We don't check that the new values are valid because
// these functions are expected to be only called by CInputData which validates
// the input data itself.

void CBondPairType::SetModulus(double newModulus)
{
	m_BendingModulus = newModulus;
}

void CBondPairType::SetPhi0(double newPhi0)
{
	m_Phi0 = newPhi0;
}
