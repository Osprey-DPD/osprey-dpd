/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// IModifySimStateBondTypes.cpp: implementation of the IModifySimStateBondTypes class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "IModifySimStateBondTypes.h" 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Standard constructor

IModifySimStateBondTypes::IModifySimStateBondTypes(CSimState& simState) : IModifySimState(simState)
{

}

IModifySimStateBondTypes::~IModifySimStateBondTypes()
{

}

// Function to tell the CInitialState to recalculate the numbers of each
// bond type. 

void IModifySimStateBondTypes::CalculateTypeTotals()
{
	m_rSimState.GetInitialState().CalculateBondTypeTotals();
}

// Function to add a new template bond type to the simulation state. 

void IModifySimStateBondTypes::AddNewType(CBond* const pBond)
{
	m_rSimState.AddBondType(pBond);
}

// Function to add a new named template bond type to the simulation state.

bool IModifySimStateBondTypes::AddNewNamedType(CBond* const pBond, long newType, const zString name)
{
	if(m_rSimState.AddBondType(pBond, newType, name))
    {
       CalculateTypeTotals();
       return true;
    }
    else
    {
        return false;
    }
}

// Function to add a new polymerised bond to the CSimState.
// Polymerised bonds can connect different polymer types and have to be
// stored separately from the polymers' internal bonds.

void IModifySimStateBondTypes::AddPolymerisedBond(CBond *const pBond)
{
	m_rSimState.AddPolymerisedBond(pBond);
}

