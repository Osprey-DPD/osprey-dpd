/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeReceptorBond.cpp: implementation of the aeReceptorBond class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "aeReceptorBond.h"
#include "aeBondLigand.h"
#include "aeBondOffRate.h"
#include "aeBondProximityOn.h"
#include "aeBondProximityOff.h"


//////////////////////////////////////////////////////////////////////
// Static member variable and function definitions
//////////////////////////////////////////////////////////////////////

// Total number of active bonds that can be connected to form an active polymer. 
// This is stored in a private static member for each classed derived from
// aeActiveBond. This allows different types of active bond to represent, 
// for example, a receptor-ligand pair (only one bond per polymer) and 
// filamentous actin (arbitrary number of bonds per polymer).

long aeReceptorBond::m_ActiveBondsPerPolymer = 2;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor to wrap a CPolymer that represents a receptor monomer in an
// active bond. This allows the monomers to be strung together into 
// active polymers that can grow and shrink to represent actin filaments.
//
// The force law for the filaments is a simple Hookean spring model
// for the bonds between adjacent monomers to set the filament's contour length, 
// and a bending stiffness to set the filament's rigidity.
//
// Note that the preferred bond angle is in radians.

aeReceptorBond::aeReceptorBond(CPolymer* const pTail, double k2, double l0, 
						   double k3, double phi0) : aeActiveBond(pTail, 
						   new aeBondLigand(0, pTail, 0, k2, l0),
						   new aeBondProximityOn(2.0, 2.0),
						   new aeBondProximityOff(1.0, 1.0)), 
						   m_SpringConstant(k2), m_UnstretchedLength(l0),
						   m_BendingModulus(k3), m_Phi0(phi0)
{
}

aeReceptorBond::~aeReceptorBond()
{
}

// Function to return the maximum number of active bonds of this type
// permitted in each active polymer.

long aeReceptorBond::GetActiveBondsPerPolymer() const
{
	return m_ActiveBondsPerPolymer;
}

// PVF that is called to check if a target bond, or monomer, can bind to the 
// current bond that is at the end of an active polymer. The conditions under 
// which monomers will successfully bind are specific to each aeActiveBond-derived 
// class, and are specified by the instance of a class derived from aeBondOnCondition
// that is created in the bond class' constructor above. If the binding succeeds, 
// the function returns true, otherwise it returns false. 
//
// We explicitly prevent binding of a bond to itself by comparing the id 
// of the target bond with that of the current bond. This test is not
// made in the aeBondProximityOn class so that we can also use it for 
// dissimilar bonds that can be bound into a polymer, e.g., a receptor-ligand pair.
// Next, we check that the target bond is not already part of an active polymer
// and, finally, we apply the bond-specific binding criteria appropriate for
// a free bond that binds to a polymerised bond at its tail, if the polymerised
// bond is at the polymer's tail, or its head, if the bond is at the polymer's head.
// Note that head and tail refer to the polymerised bond not the free one.

bool aeReceptorBond::Activate(aeActiveBond* const pTargetBond)
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
// we check whether the current bond is the tail of the active polymer,
// in which case its head pointer is occupied, or if it is the head, in which case
// its head pointer is null.

bool aeReceptorBond::Deactivate()
{
	if(GetHeadMonomer())	// Current bond is the tail of the active polymer
	{	
		return HeadOffCondition();
	}
	else	// Current bond is the head of the active polymer
	{
		return TailOffCondition();
	}

	return false;
}

// Function to change the probability of an active bond detaching from its
// neighbour via its head connection. If the bond does not implement the
// appropriate aeBondOffCondition, it is not modified.

void aeReceptorBond::SetFixedHeadOffRate(double rate)
{
    if(dynamic_cast<aeBondOffRate*>(m_pOff))
    {
	    dynamic_cast<aeBondOffRate*>(m_pOff)->SetHeadProbability(rate);
    }
}

// Function to change the probability of an active bond detaching from its
// neighbour via its tail connection. If the bond does not implement the
// appropriate aeBondOffCondition, it is not modified.

void aeReceptorBond::SetFixedTailOffRate(double rate)
{
    if(dynamic_cast<aeBondOffRate*>(m_pOff))
    {
	    dynamic_cast<aeBondOffRate*>(m_pOff)->SetTailProbability(rate);
    }
}

// Function to change the maximum distance at which two fActin monomers can
// associate to form a filament. The test uses a simple proximity check 
// to see if the two active bonds are closer than a maximum separation.

void aeReceptorBond::SetOnRateSeparation(double max)
{
	dynamic_cast<aeBondProximityOn*>(m_pOn)->SetSeparation(max);
}

void aeReceptorBond::SetOffRateSeparation(double max)
{
	dynamic_cast<aeBondProximityOff*>(m_pOff)->SetSeparation(max);
}

void aeReceptorBond::SetHeadOnRateSeparation(double max)
{
	dynamic_cast<aeBondProximityOn*>(m_pOn)->SetHeadSeparation(max);
}

void aeReceptorBond::SetHeadOffRateSeparation(double max)
{
	dynamic_cast<aeBondProximityOff*>(m_pOff)->SetHeadSeparation(max);
}

void aeReceptorBond::SetTailOnRateSeparation(double max)
{
	dynamic_cast<aeBondProximityOn*>(m_pOn)->SetTailSeparation(max);
}

void aeReceptorBond::SetTailOffRateSeparation(double max)
{
	dynamic_cast<aeBondProximityOff*>(m_pOff)->SetTailSeparation(max);
}

