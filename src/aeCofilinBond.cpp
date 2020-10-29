/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeCofilinBond.cpp: implementation of the aeCofilinBond class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "aeCofilinBond.h"
#include "aeBondProximityOn.h"
#include "aeBondOffRate.h"


//////////////////////////////////////////////////////////////////////
// Static member variable and function definitions
//////////////////////////////////////////////////////////////////////

// Total number of active bonds that can be connected to form an active polymer. 
// This is stored in a private static member for each classed derived from
// aeActiveBond. This allows different types of active bond to represent, 
// for example, a receptor-ligand pair (only two bonds per polymer) and 
// filamentous actin (arbitrary number of bonds per polymer).

long aeCofilinBond::m_ActiveBondsPerPolymer = 2;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor to wrap a CPolymer that represents a cofilin monomer in an
// active bond. This allows the cofilin to bind to the end monomer of an
// Actin filament and speed up the depolymerisation.
//
// The force law for the cofilin is a simple Hookean spring to bind the cofilin
// to an fActin monomer, and a bending stiffness to set the filament's rigidity.
//
// Note that the preferred bond angle is in radians.

aeCofilinBond::aeCofilinBond(CPolymer* const pTail, double k2, double l0, 
						   double k3, double phi0) : aeActiveBond(pTail, 
						   new aeBondProximityOn(4.0),
						   new aeBondOffRate(4.0)), 
						   m_SpringConstant(k2), m_UnstretchedLength(l0),
						   m_BendingModulus(k3), m_Phi0(phi0)
{

}

aeCofilinBond::~aeCofilinBond()
{
}

// Function to return the maximum number of active bonds of this type
// permitted in each active polymer.

long aeCofilinBond::GetActiveBondsPerPolymer() const
{
	return m_ActiveBondsPerPolymer;
}

// PVF that is called to check if a target bond, or monomer, can bind to the 
// current bond. The conditions under which monomers can link together are
// specific to each aeActiveBond-derived class. If the binding succeeds, 
// the function returns true, otherwise it returns false. 
//
// We explicitly prevent binding of a bond to itself by comparing the id 
// of the target bond with that of the current bond. This test is not
// made in the aeBondProximityOn class so that we can also use it for 
// dissimilar bonds that can be bound into a polymer, e.g., a receptor-ligand pair.
// Next, we check that the target bond is not already part of an active polymer
// and, finally, we apply the bond-specific binding criteria.

bool aeCofilinBond::Activate(aeActiveBond* const pTargetBond)
{
	if(GetId() != pTargetBond->GetId() && !pTargetBond->IsPolymerised())
	{
		if(GetHeadMonomer())	// Current bond is the tail of the active polymer
		{	
			return TailOnCondition(pTargetBond);
		}
		else	// Current bond is the head of the active polymer
		{
			return HeadOnCondition(pTargetBond);
		}
	}

	return false;
}

// PVF that is called to see if the end monomers of an active polymer can
// dissociate. Because this function is called for both ends of a polymer,
// we check whether the current active bond is the tail of the active polymer,
// in which case its head pointer is occupied, or if it is the head. In the
// latter case, we call the function again with the arguments reversed.

bool aeCofilinBond::Deactivate()
{
	return false;
}
