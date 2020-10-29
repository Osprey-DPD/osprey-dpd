/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// IInclusiveRestartState.cpp: implementation of the IInclusiveRestartState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "IInclusiveRestartState.h"
#include "SimState.h"
#include "AnalysisState.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store a pointer to the CSimState instance so that we can update
// the bead, bond, bondpair and polymer types when loading an inclusive
// restart state. We try to re-use as much of the existing functionality 
// as possible.

IInclusiveRestartState::IInclusiveRestartState(CSimState* pSimState) : m_pSimState(pSimState)
{

}

IInclusiveRestartState::~IInclusiveRestartState()
{

}

// Function to add a new bead type to the initial state. Note that we
// cannot use some of the CSimState's functions for adding bead types as they
// modify the CCNTCell bead-bead interaction data that has not yet been created. 
// It is created in the CNTCell's constructor only once to allow multiple
// runs to receive new values of the data. Instead, we duplicate the portions
// that we need.

bool IInclusiveRestartState::AddBeadType(long type, const zString name)
{
    const long oldType = m_pSimState->GetBeadTypeTotal()-1; // Note zero-based indexing

	// Store the bead's name,type association in the CAnalysisState only
	// if the bead name does not already exist

	if(m_pSimState->GetAnalysisState().AddNamedBeadTypeToMap(type, name))
	{
		// Tell the CInitialState to create a new bead type from beads
		// with the previous type "oldType".

		m_pSimState->GetInitialState().AddBeadType(oldType);

		return true;
	}
	else
		return false;
}

bool IInclusiveRestartState::AddBondType()
{
    return true;
}

bool IInclusiveRestartState::AddBondPairType()
{
    return true;
}

bool IInclusiveRestartState::AddPolymerType()
{
    return true;
}

// Access functions to the CAnalysisState's data

const zString IInclusiveRestartState::GetBeadNameFromType(long type) const
{
    return m_pSimState->GetAnalysisState().GetBeadNameFromType(type);
}

const zString IInclusiveRestartState::GetBondNameFromType(long type) const
{
    return m_pSimState->GetAnalysisState().GetBondNameFromType(type);
}

const zString IInclusiveRestartState::GetBondPairNameFromType(long type) const
{
    return m_pSimState->GetAnalysisState().GetBondPairNameFromType(type);
}

const zString IInclusiveRestartState::GetPolymerNameFromType(long type) const
{
    return m_pSimState->GetAnalysisState().GetPolymerNameFromType(type);
}

// Function to pass pointers to the arrays of BD bead-bead interaction parameters
// to the CCNTCell class for local storage.

void IInclusiveRestartState::SetBDBeadIntArrays()
{

}

// Function to pass pointers to the arrays of DPD bead-bead interaction parameters
// to the CCNTCell class for local storage.

void IInclusiveRestartState::SetDPDBeadIntArrays(const zArray2dDouble* pvvConsInt, const zArray2dDouble* pvvDissInt)
{
    m_pSimState->SetDPDBeadIntArrays(pvvConsInt, pvvDissInt);
}

// Function to pass pointers to the arrays of MD bead-bead interaction parameters
// to the CCNTCell class for local storage.

void IInclusiveRestartState::SetMDBeadIntArrays()
{

}
