/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// PolymerType.cpp: implementation of the CPolymerType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "PolymerType.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////
// Boolean function to test two polymer typesfor equality.
// This is only used by CInputData when checking the data from a restart
// control data file. We check the polymer's name, head and tail bead names,
// shape string and number fraction.
//
// I removed the test for the head and tail beads so that different lengths can
// be measured on the same polymers for subsequent restart runs. If they are 
// required later the following line should be added back.

//		a.m_Head	 == b.m_Head && a.m_Tail	 == b.m_Tail 

bool operator==(const CPolymerType& a, const CPolymerType& b)
{
	if( a.m_Name	 == b.m_Name	 &&
		a.m_Fraction == b.m_Fraction &&
		a.m_Shape    == b.m_Shape    )
		return true;
	else
		return false;
}


//////////////////////////////////////////////////////////////////////
// Static member definitions
//////////////////////////////////////////////////////////////////////

long CPolymerType::m_PolymerTypeTotal = 0;	// No of polymer types created so far

long CPolymerType::GetTotal()
{
	return m_PolymerTypeTotal;
}

// This function is called by CInputDPDFile and CInputMDFile to reset the counter
// for polymer types created between simulations and when reading a previous control 
// data file for a restart simulation.

void CPolymerType::ZeroTotal()
{
	m_PolymerTypeTotal = 0;
}

CPolymerType* CPolymerType::AddPolymerType(zString name, zString shape, double fraction)
{
	m_PolymerTypeTotal++;

	return new CPolymerType(name, shape, fraction);
}

CPolymerType* CPolymerType::AddPolymerType(zString name, zString shape, double fraction,
										   zString head, zString tail)
{
	m_PolymerTypeTotal++;

	return new CPolymerType(name, shape, fraction, head, tail);
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolymerType::CPolymerType(zString name, zString shape, double fraction) : m_Name(name),
																m_Shape(shape),
																m_Fraction(fraction),
																m_Head(""),
																m_Tail("")
{
}

CPolymerType::CPolymerType(zString name, zString shape, double fraction,
						   zString head, zString tail) : m_Name(name),
																m_Shape(shape),
																m_Fraction(fraction),
																m_Head(head),
																m_Tail(tail)
{
}

CPolymerType::~CPolymerType()
{

}

