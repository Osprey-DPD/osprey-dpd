/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateData.cpp: implementation of the CInitialStateData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "InitialStateData.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global virtual function to allow each derived class to read/write its own data.
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CInitialStateData& inState)
{
	return inState.put(os);
}

zInStream& operator>>(zInStream& is, CInitialStateData& inState)
{
	return inState.get(is);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// Each object of a derived class defines its own particular initial
// state data. This base class is used as the interface to the particular
// state objects which can then be handled by the appropriate CBuilder-derived
// object. The valid data flag is set true on creation and will be set false
// by the derived classes when they read their data from the file.

CInitialStateData::CInitialStateData() : m_bDataValid(true)
{
}

// Delete the CBuilder object associated with the initial state data object.

CInitialStateData::~CInitialStateData()
{
}

// Virtual function that must be overridden by all ISD classes that are to be usable in the parallel code.
// It is not a PVF as we don't want to force all serial code ISD classes to implement it unnecessarily.
//
// It calculates the fraction of each polymer type that must be created by the current processor according to the how much of
// the pre-assembled structure lies within the processor's Space. This calculation can only be done within the concrete ISD classes 
// as only they know the parameters defining the initial state configuration. 
//
// Note that the important question is how many polymers of each type belong in the owning processor's Space, and this can only
// be calculated from the total number of polymers of each type, not from their number fractions. Only if the simulation Space 
// is translationally invariant (as in a random IS) are the number fractions the same for all processors. The result of this function
// is the fraction of the total number of polymers of each type that belongs in the owning processor's Space. Note that these
// fractions may not add up to unity as they are not related to each other.
 
void CInitialStateData::CalculatePolymerFractionsP(double lx, double ly, double lz, 
                                                double xorigin, double yorigin, double zorigin, 
                                                double xspace, double yspace, double zspace, 
												zDoubleVector& rvPolymerFractions) const
{
}

bool CInitialStateData::IsDataValid() const
{
	return m_bDataValid;
}

void CInitialStateData::SetDataValid(bool bValid)
{
	m_bDataValid = bValid;
}

// Virtual function that should be overridden by any derived class that wants
// to validate its own data. Required input data are accessed via the reference
// to the CInitialState object. We don't make it const because derived classes
// need to store some data.

bool CInitialStateData::ValidateData(const CInputData& riData)
{
	return true;
}

// Virtual function that should be overridden by any derived class
// that provides a parallel initial state builder. This function is
// only called by P0 in the serial code, but by all PN in the parallel code and TWICE by P0.
// It ensures that the data describe a valid initial state, but also may be used to 
// convert input data into new forms, such as translating polymer names into numeric types.
// Any side-effects must be taken into account to prevent them accumulating.
// Within this function, we refer to the whole simulation space not just one processor's domain.

bool CInitialStateData::ValidateDataP(const CInputData& riData)
{
    return true;
}

// Virtual function to allow each ISD class to set its data from the corresponding
// mpmMessage instance during a parallel simulation.

void CInitialStateData::SetData(const mpmInitialState *const pMsg)
{

}
