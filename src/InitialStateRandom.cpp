/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateRandom.cpp: implementation of the CInitialStateRandom class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "InitialStateRandom.h"
#include "RandomBuilder.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for the initial state that 
// is represented by an object of this class. It is used to select the 
// appropriate CInitialStateData-derived object when the State variable is read 
// in the  control data file. The static member function GetType() is invoked by the
// CInitialStateObject to compare the type read from the control data file with each
// initial state data class so that it can create the appropriate object to hold the
// analysis data.

const zString CInitialStateRandom::m_Type = "random";

const zString CInitialStateRandom::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	CInitialStateData* Create() {return new CInitialStateRandom();}

	const zString id = CInitialStateRandom::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitialStateRandom::CInitialStateRandom()
{
}

CInitialStateRandom::~CInitialStateRandom()
{
}

CInitialStateRandom& CInitialStateRandom::operator=(const CInitialStateRandom& oldISR)
{
//	if(this != &oldISR)	// no data members so need to check
//	{
//	}

	return *this;
}

zString CInitialStateRandom::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateRandom::put(zOutStream& os) const
{
	os << "State	" << "random" << zEndl << zEndl;

	return os;
}

zInStream& CInitialStateRandom::get(zInStream& is)
{
	// There is no data needed to define an initial state of random beads.

	return is;
}

// Function to use the CRandomBuilder class to set the coordinates of all the 
// beads in the simulation into a random initial state. Note that the builder 
// is local to this CInitialStateRandom object.

bool CInitialStateRandom::Assemble(CInitialState& riState) const
{
	CRandomBuilder random;

#if EnableParallelSimBox == SimMPSEnabled
	return random.AssembleP(riState);
#else
	return random.Assemble(riState);
#endif
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateRandom::Copy() const
{
	CInitialStateRandom* pISR = new CInitialStateRandom;

	*pISR = *this;

	return pISR;
}

// Virtual function to assemble the IS data from the corresponding 
// mpmInitialState message instance.
// This IS has no data except its type.

void CInitialStateRandom::SetData(const mpmInitialState *const pMsg)
{
    // Do nothing!
}

// Parallel code function to calculate whether the pre-assembled IS overlaps the owning processor's space. 
// Because the simulation Space is translationally invariant for a random IS, we just set all polymer fractions to unity.
// This indicates that the whole number fraction for each polymer should be created in the owning processor's Space.
	
void CInitialStateRandom::CalculatePolymerFractionsP(double lx, double ly, double lz, double xorigin, double yorigin, double zorigin, double xspace, double yspace, double zspace, 
                                                   zDoubleVector& rvPolymerFractions) const
{
}
