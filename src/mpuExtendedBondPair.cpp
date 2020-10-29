/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpuExtendedBondPair.cpp: implementation of the mpuExtendedBondPair class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "mpuExtendedBondPair.h"
#include "AbstractBead.h"
#include "Bond.h"
#include "BondPair.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Default constructor

mpuExtendedBondPair::mpuExtendedBondPair() : m_pBondPair(0), m_pBond1(0), m_pBond2(0), 
                                             m_id(-1), m_Type(-1),
											 m_Modulus(0.0), m_Phi0(0.0)
{

}

// Type conversion constructor to wrap a CBondPair instance in an mpuExtendedBondPair instance.
//

mpuExtendedBondPair::mpuExtendedBondPair(CBondPair* pBondPair, mpuExtendedBond* pBond1, mpuExtendedBond* pBond2) : m_pBondPair(pBondPair), 
                                         m_pBond1(pBond1), m_pBond2(pBond2), 
                                         m_id(pBondPair->GetId()), m_Type(pBondPair->GetType()),
									     m_Modulus(pBondPair->GetModulus()), m_Phi0(pBondPair->GetPhi0())
{

}

mpuExtendedBondPair::~mpuExtendedBondPair()
{

}

long mpuExtendedBondPair::GetId() const
{
	if(m_pBondPair)
	{
		return m_pBondPair->GetId();
	}
	else
	{
		return m_id; 
	}
}

long mpuExtendedBondPair::GetType() const
{
	if(m_pBondPair)
	{
		return m_pBondPair->GetType();
	}
	else
	{
		return m_Type;
	}
}

double mpuExtendedBondPair::GetModulus() const
{
	if(m_pBondPair)
	{
		return m_pBondPair->GetModulus();
	}
	else
	{
		return m_Modulus;
	}
}

double mpuExtendedBondPair::GetPhi0() const
{
	if(m_pBondPair)
	{
		return m_pBondPair->GetPhi0();
	}
	else
	{
		return m_Phi0;
	}
}

