/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeBondLigand.cpp: implementation of the aeBondLigand class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "aeBondLigand.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor for use in the general case in which a ligand is bound to a receptor by a single Hookean spring.

aeBondLigand::aeBondLigand(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel,
						   double k2, double l0) : aeBondConnection(pHead, pTail, pFuel),
						    m_SpringConstantLinker(k2), m_UnstretchedLengthLinker(l0),
							m_pLinkerHHBond(new CBond(-1,0.0,0.0))
{
	// Store the head bead in the tail position of the linker bond. 

	m_pLinkerHHBond->SetTailBead(pTail->GetHead());
	
	m_pLinkerHHBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerHHBond->SetUnStretchedLength(m_UnstretchedLengthLinker);
}


aeBondLigand::~aeBondLigand()
{
	if(m_pLinkerHHBond)
		delete m_pLinkerHHBond;
}

// Function to calculate the force between two connected active bonds. We have to use the PBC calculation 
// as dynamically-created bonds may link beads that have moved across the periodic boundaries of the 
// simulation box many times and so are not in the same box any more.
//
// The force is only added if two active bonds are linked together. The last bond 
// in an active polymer has only a tail monomer. As there are no stiff bonds in this
// aeBondConnection, it contributes no force.
//
// The force is a sum of Hookean functions of the monomer separation that constrain the 
// active bond length. The length of the active bond is calculated using the 
// head bead of the tail bond and head bead of the head bond: this is different than for the
// other types of bond.

void aeBondLigand::AddForce()
{
	if(m_pHead)
	{
		m_pLinkerHHBond->AddPBCForce();	// Calculates bond length and force
	}
}

// Function to update the Hookean spring that comprises this type of bond connector 
// when a new monomer is added as the head of the active bond. If the bond-on condition 
// for the enclosing active bond type is true, this function is called. Note that we do 
// not update the actual monomer pointer here, only the bonds that depend on the monomer. 
// The monomer is owned by the aeActiveBond base class.
//
// Because monomers are represented by CPolymers, we can use the CPolymer's 
// head bead to define the location of the single bond used in this kind of linear connector.

bool aeBondLigand::AddHeadMonomer(CPolymer *const pHead)
{
	// Set the head bead of the linker bond from the tail monomer to the head monomer

	m_pHead	= pHead;
	m_pLinkerHHBond->SetHeadBead(pHead->GetHead());

	return true;
}

// Function to store the tail monomer of the active bond being added as the current
// active bond's head monomer. Because this bond connection instance does not need
// access to adjacent bonds, we just call the overloaded function that takes a pointer
// to a CPolymer.

bool aeBondLigand::AddHeadMonomer(aeActiveBond *const pHeadBond)
{
	return AddHeadMonomer(pHeadBond->GetTailMonomer());
}

// Function to update the connection instance when the containing bond is disconnected from its head-adjacent neighbour. 

bool aeBondLigand::RemoveHeadMonomer(aeActiveBond* const pHeadBond)
{
	m_pHead	= 0;
	m_pLinkerHHBond->SetHeadBead(0);

	return true;
}

