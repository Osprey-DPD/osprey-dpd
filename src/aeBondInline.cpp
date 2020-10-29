/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeBondInline.cpp: implementation of the aeBondInline class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aeBondInline.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aeBondInline::aeBondInline(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel,
						   double k2, double l0, double k3, double phi0) : aeBondConnection(pHead, pTail, pFuel),
						    m_SpringConstant(k2), m_UnstretchedLength(l0),
							m_BendingModulus(k3), m_Phi0(phi0),
							m_pInternalBond(new CBond(-1,0.0,0.0)),
							m_pLinkerBond(new CBond(-1,0.0,0.0)),
							m_pHeadInternalBond(new CBond(-1,0.0,0.0)),
							m_pInternalBondPair(new CBondPair(-1,0.0,0.0)),
							m_pLinkerBondPair(new CBondPair(-1,0.0,0.0))


{
	// Store the head and tail beads of the active bond's monomer in the internal
	// bond, and its head bead in the tail position of the linker bond. This still
	// requires that the head monomer's tail bead be added to the linker bond as
	// its head bead, and the head and tail beads of the next active bond's internal
	// bond to complete the bond specification for the active bond.
	//
	// AddHeadMonomer() is used by the aeActivePolymer class to supply the beads
	// needed to complete the specification.

	m_pInternalBond->SetBeads(pTail->GetHead(), m_pTail->GetTail());
	m_pLinkerBond->SetTailBead(pTail->GetHead());

	m_pInternalBondPair->SetBonds(m_pInternalBond, m_pLinkerBond);
	m_pLinkerBondPair->SetBonds(m_pLinkerBond, m_pHeadInternalBond);

	// Set the parameters for the Hookean springs and stiff bonds

	m_pInternalBond->SetSpringConstant(k2);
	m_pInternalBond->SetUnStretchedLength(l0);

	m_pLinkerBond->SetSpringConstant(k2);
	m_pLinkerBond->SetUnStretchedLength(l0);

	m_pInternalBondPair->SetModulus(k3);
	m_pInternalBondPair->SetPhi0(phi0);

	m_pLinkerBondPair->SetModulus(k3);
	m_pLinkerBondPair->SetPhi0(phi0);


}

aeBondInline::~aeBondInline()
{
	// Delete the internal bond, linker bond, head internal bond and the linker 
	// bond pair if they exist

	if(m_pInternalBond)
		delete m_pInternalBond;

	if(m_pLinkerBond)
		delete m_pLinkerBond;

	if(m_pHeadInternalBond)
		delete m_pHeadInternalBond;

	if(m_pInternalBondPair)
		delete m_pInternalBondPair;

	if(m_pLinkerBondPair)
		delete m_pLinkerBondPair;
}

// Function to calculate the force between two active bonds when they are connected
// in a straight line by a Hookean spring and a bond stiffness potential. This type
// of bond is the simplest we use, and requires only that the tail CPolymer for
// the bond have a distinct Head and Tail bead defined.
//
// The force is only added if two active bonds are linked together. The last bond 
// in an active polymer has only a tail monomer: it contributes to the bending 
// stiffness of the bondpair linking it to its predecessor active bond but not to 
// the bonds linking beads.
//
// The force is a Hookean function of the monomer separation that constrains the 
// active bond length, and a bending force that constrains the
// active bonds to lie parallel to each other. The length is of the active bond is
// calculated using the head bead of the tail bond and tail bead of the head bond.
// The bending force is calculated from the positions of the THT beads, viz, the 
// tail and head beads of the tail bond and the tail bead of the head bond.

void aeBondInline::AddForce()
{
	if(m_pHead)
	{
		// Calculate the bond force between the active bond's monomer and the
		// head monomer. This consists of a Hookean spring force between the
		// head and tail beads in each monomer, and the bond connecting the
		// two monomers; and the bondpair forces for the two stiff bonds per
		// active bond. The CBond and CBondPair instances handle passing the force
		// data to the beads.

		// We have to calculate the length of the internal bonds separately as we do
		// not want to calculate the force between these beads because they are
		// assumed to be an internal (and hence invisible) part of the active bond.
		// If the monomer head and tail beads belong to a CBond instance in the
		// monomer itself, their bond force is calculated in the general equations
		// of motion for the simulation. We use the CBond::AddPotentialEnergy() 
		// function and ignore the return value because this calculates the
		// bond's length using the un-PBC coordinates, and returns the energy. 
		// The CBond::CalculateLength() uses the PBC coordinates and is not 
		// suitable for the length calculation.

		// If the monomer's head and tail beads are not connected by a bond,
		// we have to calculate the bond force here as well. For m-Actin we
		// expect the monomer to have the architecture H-T so the beads will
		// be connected.

		m_pInternalBond->AddPotentialEnergy();	    // Only calculates bond length
		m_pHeadInternalBond->AddPotentialEnergy();	// Only calculates bond length

		// The beads are labelled in the following order:
		//
		//	1	Tail monomer's tail bead
		//	2	Tail monomer's head bead
		//	3	Head monomer's tail bead
		//	4	Head monomer's head bead
		//
		// This corresponds to the order in which the CBondPair class calculates
		// the force on a triple of beads. 

		m_pLinkerBond->AddPBCForce();	// Calculates bond length and force
		m_pInternalBondPair->AddForce();
		m_pLinkerBondPair->AddForce();
	}
}

// Function to update the Hookean spring and stiff bond that comprise this
// type of bond connector when a new monomer is added as the head of the 
// active bond. If the bond-on condition for the enclosing active bond type is true,
// this function is called. Note that we do not update the actual monomer pointer
// here, only the bonds that depend on the monomer. The monomer is owned by
// the aeActiveBond base class.
//
// Because monomers are represented by CPolymers, we can use the CPolymer's 
// head and tail beads to define the location of the monomers in an active bond.

bool aeBondInline::AddHeadMonomer(CPolymer* const pHead)
{
	// Set the head bead of the linker bond from the head monomer's tail bead.

	m_pHead	= pHead;

	m_pLinkerBond->SetHeadBead(pHead->GetTail());

	// Set the beads in the copy of the head's internal bond used to calculate
	// the linker bondpair forces

	m_pHeadInternalBond->SetBeads(pHead->GetHead(), pHead->GetTail()); 

	return true;
}

// Function to store the tail monomer of the active bond being added as the current
// active bond's head monomer. Because this bond connection instance does not need
// access to adjacent bonds, we just call the overloaded function that takes a pointer
// to a CPolymer.

bool aeBondInline::AddHeadMonomer(aeActiveBond* const pHeadBond)
{
	return AddHeadMonomer(pHeadBond->GetTailMonomer());	
}

// Function to disconnect the containing bond from its head-adjacent neighbour.
// This connection type does not need to modify the adjacent bond.

bool aeBondInline::RemoveHeadMonomer(aeActiveBond* const pHeadBond)
{
	m_pHead	= 0;

	m_pLinkerBond->SetHeadBead(0);
	m_pHeadInternalBond->SetHeadBead(0);
	m_pHeadInternalBond->SetTailBead(0);

	return true;
}

