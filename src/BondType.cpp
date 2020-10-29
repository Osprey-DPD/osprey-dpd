/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// BondType.cpp: implementation of the CBondType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "BondType.h"


//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////
// Boolean function to check that two bond types have the same name.
// We don't compare their spring constants and length because we want
// to be able to alter these parameters when doing a restart. 
// This is only used by CInputData when checking the data from a restart
// control data file.

bool operator==(const CBondType& a, const CBondType& b)
{
	if( a.m_headName == b.m_headName &&	a.m_tailName == b.m_tailName )
		return true;
	else
		return false;
}


//////////////////////////////////////////////////////////////////////
// Static member definitions
//////////////////////////////////////////////////////////////////////

long CBondType::m_BondTypeTotal = 0;	// No of bond types created so far

long CBondType::GetTotal()
{
	return m_BondTypeTotal;
}

// This function is called by CInputDPDFile and CInputMDFile to reset the counter
// for bond types created between simulations and when reading a previous control 
// data file for a restart simulation.

void CBondType::ZeroTotal()
{
	m_BondTypeTotal = 0;
}

CBondType* CBondType::AddBondType(zString head, zString tail, double SprConst, double UnStrLen)
{
	m_BondTypeTotal++;

	return new CBondType(head, tail, SprConst, UnStrLen);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// A bond type must receive valid bead types as its head and tail for it
// to be constructed.

CBondType::CBondType(zString head, zString tail, double SprConst, double UnStrLen) : m_headName(head), m_tailName(tail), m_SprConst(SprConst), m_UnStrLen(UnStrLen)
{
}

CBondType::~CBondType()
{

}

// Functions to set the values of the bond parameters: the spring constant and
// unstretched length. We do not check that the values are sensible because these
// functions are only supposed to be used by the CInputData object that has already
// validated the data.

void CBondType::SetSpringConst(double springConst)
{
	m_SprConst = springConst;
}

void CBondType::SetLength(double unstretchedLength)
{
	m_UnStrLen = unstretchedLength;
}
