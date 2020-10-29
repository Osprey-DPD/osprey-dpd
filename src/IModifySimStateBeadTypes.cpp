/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// IModifySimStateBeadTypes.cpp: implementation of the IModifySimStateBeadTypes class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "IModifySimStateBeadTypes.h" 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Constructor for use in the parallel code

// Standard constructor

IModifySimStateBeadTypes::IModifySimStateBeadTypes(CSimState& simState) : IModifySimState(simState)
{

}

IModifySimStateBeadTypes::~IModifySimStateBeadTypes()
{

}

// Function to tell the CInitialState to recalculate the numbers of each
// bead type. 

void IModifySimStateBeadTypes::CalculateTypeTotals()
{
	m_rSimState.GetInitialState().CalculateBeadTypeTotals();
}

// Function to add a new template bead type to the CSimState. 

void IModifySimStateBeadTypes::AddNewType(long oldType)
{
	m_rSimState.AddBeadType(oldType);
}

// Function to add a new named template bead type to the CSimState.

bool IModifySimStateBeadTypes::AddNewNamedType(long oldType, long newType, const zString name)
{
	if(m_rSimState.AddBeadType(oldType, newType, name))
    {
       CalculateTypeTotals();
       return true;
    }
    else
    {
        return false;
    }
}

// Function to change the value of the conservative interaction between
// two DPD bead types. 

void IModifySimStateBeadTypes::SetDPDConsInt(long firstType, long secondType, double newValue)
{
	m_rSimState.SetDPDConsInt(firstType, secondType, newValue);
}

// Function to change the value of the dissipative interaction between
// two DPD bead types. It changes one element in the matrix stored in the 

void IModifySimStateBeadTypes::SetDPDDissInt(long firstType, long secondType, double newValue)
{
	m_rSimState.SetDPDDissInt(firstType, secondType, newValue);
}

