/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeBondTriangle.cpp: implementation of the aeBondTriangle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "aeBondTriangle.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor for use when the internal and linker bonds have the same parameters.

aeBondTriangle::aeBondTriangle(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel,
						   double k2, double l0) : aeBondConnection(pHead, pTail, pFuel),
						    m_SpringConstantInternal(k2), m_UnstretchedLengthInternal(l0),
						    m_SpringConstantLinker(k2),   m_UnstretchedLengthLinker(l0),
							m_UnstretchedLengthCrossLinker(0.0),
							m_BendingModulusLinker(0.0), m_Phi0Linker(0.0),
							m_pInternalHTBond(new CBond(-1,0.0,0.0)),
							m_pInternalHMBond(new CBond(-1,0.0,0.0)),
							m_pInternalMTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHHBond(new CBond(-1,0.0,0.0)),
							m_pLinkerMMBond(new CBond(-1,0.0,0.0)),
							m_pLinkerTTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHMBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerTMBond(new CBond(-1,0.0,0.0)),
							m_pLinkerTHBond(new CBond(-1,0.0,0.0)),
							m_pLinkerMTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerMHBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHHBondPair(new CBondPair(-1,0.0,0.0)),
							m_pLinkerMMBondPair(new CBondPair(-1,0.0,0.0)),
							m_pLinkerTTBondPair(new CBondPair(-1,0.0,0.0))

{
	// Store the head, middle and tail beads of the active bond's monomer in the 
	// internal bond, and its head bead in the tail position of the linker bond. 
	// Note that we assume the H,M,T beads are linearly connected in that order.
	// If this is not the case, this constructor may need to be redesigned.
	//
	// AddHeadMonomer() is used by the aeActivePolymer class to supply the beads
	// needed to complete the specification.

	m_pInternalHTBond->SetBeads(pTail->GetHead(), m_pTail->GetTail());
	m_pInternalHMBond->SetBeads(pTail->GetHead(), m_pTail->GetHeadAdjacentBead());
	m_pInternalMTBond->SetBeads(pTail->GetHeadAdjacentBead(), m_pTail->GetTail());

	m_pLinkerHHBond->SetTailBead(pTail->GetHead());
	m_pLinkerMMBond->SetTailBead(pTail->GetHeadAdjacentBead());
	m_pLinkerTTBond->SetTailBead(pTail->GetTail());

	// Set the parameters for the Hookean springs and stiff bonds: these are all
	// the same for all bonds.

	m_pInternalHTBond->SetSpringConstant(m_SpringConstantInternal);
	m_pInternalHTBond->SetUnStretchedLength(m_UnstretchedLengthInternal);
	m_pInternalHMBond->SetSpringConstant(m_SpringConstantInternal);
	m_pInternalHMBond->SetUnStretchedLength(m_UnstretchedLengthInternal);
	m_pInternalMTBond->SetSpringConstant(m_SpringConstantInternal);
	m_pInternalMTBond->SetUnStretchedLength(m_UnstretchedLengthInternal);

	m_pLinkerHHBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerHHBond->SetUnStretchedLength(m_UnstretchedLengthLinker);
	m_pLinkerMMBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerMMBond->SetUnStretchedLength(m_UnstretchedLengthLinker);
	m_pLinkerTTBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerTTBond->SetUnStretchedLength(m_UnstretchedLengthLinker);
}

// Constructor for use when the internal and linker bonds have different parameters.

aeBondTriangle::aeBondTriangle(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel,
						   double k2internal, double l0internal, double k2linker, double l0linker) : aeBondConnection(pHead, pTail, pFuel),
						    m_SpringConstantInternal(k2internal), m_UnstretchedLengthInternal(l0internal),
						    m_SpringConstantLinker(k2linker), m_UnstretchedLengthLinker(l0linker),
							m_UnstretchedLengthCrossLinker(0.0),
							m_BendingModulusLinker(0.0), m_Phi0Linker(0.0),
							m_pInternalHTBond(new CBond(-1,0.0,0.0)),
							m_pInternalHMBond(new CBond(-1,0.0,0.0)),
							m_pInternalMTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHHBond(new CBond(-1,0.0,0.0)),
							m_pLinkerMMBond(new CBond(-1,0.0,0.0)),
							m_pLinkerTTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHMBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerTMBond(new CBond(-1,0.0,0.0)),
							m_pLinkerTHBond(new CBond(-1,0.0,0.0)),
							m_pLinkerMTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerMHBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHHBondPair(new CBondPair(-1,0.0,0.0)),
							m_pLinkerMMBondPair(new CBondPair(-1,0.0,0.0)),
							m_pLinkerTTBondPair(new CBondPair(-1,0.0,0.0))

{
	// Store the head, middle and tail beads of the active bond's monomer in the 
	// internal bond, and its head bead in the tail position of the linker bond. 
	// Note that we assume the H,M,T beads are linearly connected in that order.
	// If this is not the case, this constructor may need to be redesigned.
	//
	// AddHeadMonomer() is used by the aeActivePolymer class to supply the beads
	// needed to complete the specification.

	m_pInternalHTBond->SetBeads(pTail->GetHead(), m_pTail->GetTail());
	m_pInternalHMBond->SetBeads(pTail->GetHead(), m_pTail->GetHeadAdjacentBead());
	m_pInternalMTBond->SetBeads(pTail->GetHeadAdjacentBead(), m_pTail->GetTail());

	m_pLinkerHHBond->SetTailBead(pTail->GetHead());
	m_pLinkerMMBond->SetTailBead(pTail->GetHeadAdjacentBead());
	m_pLinkerTTBond->SetTailBead(pTail->GetTail());

	// Set the parameters for the Hookean springs and stiff bonds: these are all
	// the same for all bonds.

	m_pInternalHTBond->SetSpringConstant(m_SpringConstantInternal);
	m_pInternalHTBond->SetUnStretchedLength(m_UnstretchedLengthInternal);
	m_pInternalHMBond->SetSpringConstant(m_SpringConstantInternal);
	m_pInternalHMBond->SetUnStretchedLength(m_UnstretchedLengthInternal);
	m_pInternalMTBond->SetSpringConstant(m_SpringConstantInternal);
	m_pInternalMTBond->SetUnStretchedLength(m_UnstretchedLengthInternal);

	m_pLinkerHHBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerHHBond->SetUnStretchedLength(m_UnstretchedLengthLinker);
	m_pLinkerMMBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerMMBond->SetUnStretchedLength(m_UnstretchedLengthLinker);
	m_pLinkerTTBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerTTBond->SetUnStretchedLength(m_UnstretchedLengthLinker);
}


// Constructor for use when stiff bonds are used to link adjacent monomer triples.
// Note that the cross linker bonds' unstretched length (m_UnstretchedLengthCrossLinker)
// is the diagonal of a triangle formed by the internal and linker bonds. This
// provides rigidity to the two adjacent triangular monomers.

aeBondTriangle::aeBondTriangle(CPolymer* const pHead, CPolymer* const pTail, CPolymer* const pFuel,
						   double k2internal, double l0internal, double k2linker, double l0linker,
						   double k3linker, double phi0linker) : aeBondConnection(pHead, pTail, pFuel),
						    m_SpringConstantInternal(k2internal), m_UnstretchedLengthInternal(l0internal),
						    m_SpringConstantLinker(k2linker), m_UnstretchedLengthLinker(l0linker),
							m_UnstretchedLengthCrossLinker(sqrt(l0internal*l0internal + l0linker*l0linker)),
							m_BendingModulusLinker(k3linker), m_Phi0Linker(phi0linker),
							m_pInternalHTBond(new CBond(-1,0.0,0.0)),
							m_pInternalHMBond(new CBond(-1,0.0,0.0)),
							m_pInternalMTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHHBond(new CBond(-1,0.0,0.0)),
							m_pLinkerMMBond(new CBond(-1,0.0,0.0)),
							m_pLinkerTTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHMBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerTMBond(new CBond(-1,0.0,0.0)),
							m_pLinkerTHBond(new CBond(-1,0.0,0.0)),
							m_pLinkerMTBond(new CBond(-1,0.0,0.0)),
							m_pLinkerMHBond(new CBond(-1,0.0,0.0)),
							m_pLinkerHHBondPair(new CBondPair(-1,0.0,0.0)),
							m_pLinkerMMBondPair(new CBondPair(-1,0.0,0.0)),
							m_pLinkerTTBondPair(new CBondPair(-1,0.0,0.0))

{
	// Store the head, middle and tail beads of the active bond's monomer in the 
	// internal bond, and its head bead in the tail position of the linker bond. 
	// Note that we assume the H,M,T beads are linearly connected in that order.
	// If this is not the case, this constructor may need to be redesigned.
	//
	// AddHeadMonomer() is used by the aeActivePolymer class to supply the beads
	// needed to complete the specification.

	m_pInternalHTBond->SetBeads(pTail->GetHead(), m_pTail->GetTail());
	m_pInternalHMBond->SetBeads(pTail->GetHead(), m_pTail->GetHeadAdjacentBead());
	m_pInternalMTBond->SetBeads(pTail->GetHeadAdjacentBead(), m_pTail->GetTail());

	m_pLinkerHHBond->SetTailBead(pTail->GetHead());
	m_pLinkerMMBond->SetTailBead(pTail->GetHeadAdjacentBead());
	m_pLinkerTTBond->SetTailBead(pTail->GetTail());
	m_pLinkerHMBond->SetTailBead(pTail->GetHead());
	m_pLinkerHTBond->SetTailBead(pTail->GetHead());
	m_pLinkerTMBond->SetTailBead(pTail->GetTail());
	m_pLinkerTHBond->SetTailBead(pTail->GetTail());
	m_pLinkerMTBond->SetTailBead(pTail->GetHeadAdjacentBead());
	m_pLinkerMHBond->SetTailBead(pTail->GetHeadAdjacentBead());


	// Set the parameters for the Hookean springs and stiff bonds: all internal
	// bonds have the same parameters, linker bonds have their own parameters,
	// and the cross-linker bonds have an unstretched length given by the
	// diagonal of the triangle formed by the other two bond types. The strength
	// of the cross-linker bonds is the same as that of the linker bonds.

	m_pInternalHTBond->SetSpringConstant(m_SpringConstantInternal);
	m_pInternalHTBond->SetUnStretchedLength(m_UnstretchedLengthInternal);
	m_pInternalHMBond->SetSpringConstant(m_SpringConstantInternal);
	m_pInternalHMBond->SetUnStretchedLength(m_UnstretchedLengthInternal);
	m_pInternalMTBond->SetSpringConstant(m_SpringConstantInternal);
	m_pInternalMTBond->SetUnStretchedLength(m_UnstretchedLengthInternal);

	m_pLinkerHHBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerHHBond->SetUnStretchedLength(m_UnstretchedLengthLinker);
	m_pLinkerMMBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerMMBond->SetUnStretchedLength(m_UnstretchedLengthLinker);
	m_pLinkerTTBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerTTBond->SetUnStretchedLength(m_UnstretchedLengthLinker);

	m_pLinkerHMBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerHMBond->SetUnStretchedLength(m_UnstretchedLengthCrossLinker);
	m_pLinkerHTBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerHTBond->SetUnStretchedLength(m_UnstretchedLengthCrossLinker);
	m_pLinkerTMBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerTMBond->SetUnStretchedLength(m_UnstretchedLengthCrossLinker);
	m_pLinkerTHBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerTHBond->SetUnStretchedLength(m_UnstretchedLengthCrossLinker);
	m_pLinkerMTBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerMTBond->SetUnStretchedLength(m_UnstretchedLengthCrossLinker);
	m_pLinkerMHBond->SetSpringConstant(m_SpringConstantLinker);
	m_pLinkerMHBond->SetUnStretchedLength(m_UnstretchedLengthCrossLinker);

	// Note that we cannot define the bonds contained in the bondpairs until monomers
	// are known to be linker together. These tests are done in the aeActiveBond::AddHeadAdjacentBond()
	// and related functions.

	m_pLinkerHHBondPair->SetModulus(m_BendingModulusLinker);
	m_pLinkerHHBondPair->SetPhi0(m_Phi0Linker);
	m_pLinkerMMBondPair->SetModulus(m_BendingModulusLinker);
	m_pLinkerMMBondPair->SetPhi0(m_Phi0Linker);
	m_pLinkerTTBondPair->SetModulus(m_BendingModulusLinker);
	m_pLinkerTTBondPair->SetPhi0(m_Phi0Linker);

}

aeBondTriangle::~aeBondTriangle()
{
	// Delete the internal bonds, linker bonds and linker bondpairs if they exist

	if(m_pInternalHTBond)
		delete m_pInternalHTBond;

	if(m_pInternalHMBond)
		delete m_pInternalHMBond;

	if(m_pInternalMTBond)
		delete m_pInternalMTBond;

	if(m_pLinkerHHBond)
		delete m_pLinkerHHBond;

	if(m_pLinkerMMBond)
		delete m_pLinkerMMBond;

	if(m_pLinkerTTBond)
		delete m_pLinkerTTBond;

	if(m_pLinkerHMBond)
		delete m_pLinkerHMBond;

	if(m_pLinkerHTBond)
		delete m_pLinkerHTBond;

	if(m_pLinkerTMBond)
		delete m_pLinkerTMBond;

	if(m_pLinkerTHBond)
		delete m_pLinkerTHBond;

	if(m_pLinkerMTBond)
		delete m_pLinkerMTBond;

	if(m_pLinkerMHBond)
		delete m_pLinkerMHBond;

	if(m_pLinkerHHBondPair)
		delete m_pLinkerHHBondPair;

	if(m_pLinkerMMBondPair)
		delete m_pLinkerMMBondPair;

	if(m_pLinkerTTBondPair)
		delete m_pLinkerTTBondPair;

}

// Function to calculate the force between two active bonds when they are connected
// by three Hookean spring in a triangle. This type of bond is an attempt to increase
// the rigidity between monomers in an active polymer. It requires that each monomer
// define a head, tail and middle bead.
//
// The force is only added if two active bonds are linked together. The last bond 
// in an active polymer has only a tail monomer. As there are no stiff bonds in this
// aeBondConnection, it contributes no force.
//
// The force is a sum of Hookean functions of the monomer separation that constrain the 
// active bond length. The length is of the active bond is calculated using the 
// head bead of the tail bond and tail bead of the head bond.

void aeBondTriangle::AddForce()
{
	if(m_pHead)
	{
		// Calculate the bond force between the active bond's monomer and the
		// head monomer. This consists of a Hookean spring force between the
		// head and tail beads in each monomer, and the bond connecting the
		// two monomers.

		// IMPORTANT NOTE 
		// The following applies mainly to mActin

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

		// Because we don't know whethere the monomer's head, middle and tail beads 
		// are connected by bonds in the monomer architecture (ie CPolymer's architecture),
		// we calculate the bond forces here as well. 
		
		// 08/11/11 The following functions use the unPBC coordinates of the beads, and for dynamically-created bonds
		// this may lead to the wrong force calculation. I should check whether using PBC calculation is correct.

		m_pInternalHMBond->AddForce();	    // Calculates bond length and force
		m_pInternalHTBond->AddForce();	    // Calculates bond length and force
		m_pInternalMTBond->AddForce();	    // Calculates bond length and force

		m_pLinkerHHBond->AddForce();	// Calculates bond length and force
		m_pLinkerMMBond->AddForce();	// Calculates bond length and force
		m_pLinkerTTBond->AddForce();	// Calculates bond length and force
		m_pLinkerHMBond->AddForce();	// Calculates bond length and force
		m_pLinkerHTBond->AddForce();	// Calculates bond length and force
		m_pLinkerTMBond->AddForce();	// Calculates bond length and force
		m_pLinkerTHBond->AddForce();	// Calculates bond length and force
		m_pLinkerMTBond->AddForce();	// Calculates bond length and force
		m_pLinkerMHBond->AddForce();	// Calculates bond length and force

	}
}

// Function to add the bending stiffness due to bondpairs that link adjacent monomer 
// triples in an active polymer. We only add this force if the current monomer has a 
// tail-adjacent monomer and that monomer also has a tail-adjacent monomer.

void aeBondTriangle::AddThreeBodyForce()
{
	// Check if the current bond implements the 3-body force, and add it to the beads if so

	if(GetThreeBodyForce())
	{
		m_pLinkerHHBondPair->AddForce();
		m_pLinkerMMBondPair->AddForce();
		m_pLinkerTTBondPair->AddForce();
	}
}

// Function to update the Hookean springs that comprise this type of bond connector 
// when a new monomer is added as the head of the active bond. If the bond-on condition 
// for the enclosing active bond type is true, this function is called. Note that we do 
// not update the actual monomer pointer here, only the bonds that depend on the monomer. 
// The monomer is owned by the aeActiveBond base class.
//
// Because monomers are represented by CPolymers, we can use the CPolymer's 
// head, head adjacent and tail beads to define the location of the bonds in the
// bond triangle.

bool aeBondTriangle::AddHeadMonomer(CPolymer *const pHead)
{
	// Set the head beads of the linker bonds from the tail monomer to the head monomer

	m_pHead	= pHead;

	m_pLinkerHHBond->SetHeadBead(pHead->GetHead());
	m_pLinkerMMBond->SetHeadBead(pHead->GetHeadAdjacentBead());
	m_pLinkerTTBond->SetHeadBead(pHead->GetTail());
	m_pLinkerHMBond->SetHeadBead(pHead->GetHeadAdjacentBead());
	m_pLinkerHTBond->SetHeadBead(pHead->GetTail());
	m_pLinkerTMBond->SetHeadBead(pHead->GetHeadAdjacentBead());
	m_pLinkerTHBond->SetHeadBead(pHead->GetHead());
	m_pLinkerMTBond->SetHeadBead(pHead->GetTail());
	m_pLinkerMHBond->SetHeadBead(pHead->GetHead());

	return true;
}

// Function to allow this connection instance access to the active bonds adjacent to it
// so that it can store bondpairs that provide stiffness between adjacent monomer triples.
//
// When a monomer is added to the head of the current active bond, the current bond
// checks to see if it has a tail adjacent monomer by chaining along the connected 
// monomers. This requires that all monomers be connected in the active polymer before
// the bond connection's AddHeadMonomer() function is called. This is done in the
// aeActiveBond::AddHeadAdjacentBond() function.

bool aeBondTriangle::AddHeadMonomer(aeActiveBond *const pHeadBond)
{
	// Get a pointer to the head-adjacent bond's tail monomer (a CPolymer instance)

	CPolymer* const pHeadMonomer = pHeadBond->GetTailMonomer();

	// See if there is a tail-adjacent aeActiveBond instance by chaining through the 
	// active bonds. Note that pHeadBond->GetTailAdjacentBond() is the bond that contains
	// this aeBondTriangle instance.

	aeActiveBond* pTailBond = 0;

	if(pHeadBond->GetTailAdjacentBond() && pHeadBond->GetTailAdjacentBond()->GetTailAdjacentBond())
	{
		pTailBond = pHeadBond->GetTailAdjacentBond()->GetTailAdjacentBond();

		const aeBondTriangle* const pTailConnector = dynamic_cast<const aeBondTriangle*>(pTailBond->GetBondConnection());
			
		if(pTailConnector)
		{
			// Call the overloaded AddHeadMonomer function to connect the linker bonds

			AddHeadMonomer(pHeadMonomer);

			// Indicate that 3-body forces should be added to the total forces for this
			// bond connector

			SetThreeBodyForceOn();

			// Set up the bondpairs using the linker bonds between the three active bonds:

			m_pLinkerHHBondPair->SetBonds(m_pLinkerHHBond, pTailConnector->m_pLinkerHHBond);
			m_pLinkerMMBondPair->SetBonds(m_pLinkerMMBond, pTailConnector->m_pLinkerMMBond);
			m_pLinkerTTBondPair->SetBonds(m_pLinkerTTBond, pTailConnector->m_pLinkerTTBond);
		}
		else
		{
			// The tail-adjacent bond does not use the aeBondTriangle connector, so
			// only use the bond forces not the bondpair forces

			return AddHeadMonomer(pHeadMonomer);
		}
	}
	else
	{
		// There is no tail-adjacent active bond, so only use the bond forces not
		// the bondpair forces

		return AddHeadMonomer(pHeadMonomer);
	}

	return true;
}

// Function to update the connection instance when the containing bond is
// disconnected from its head-adjacent neighbour. This connection type needs 
// to check whether the adjacent bond calculates 3-body forces and, if so, 
// update its connector instance to prevent from being calculated.

bool aeBondTriangle::RemoveHeadMonomer(aeActiveBond* const pHeadBond)
{
	m_pHead	= 0;

	m_pLinkerHHBond->SetHeadBead(0);
	m_pLinkerMMBond->SetHeadBead(0);
	m_pLinkerTTBond->SetHeadBead(0);
	m_pLinkerHMBond->SetHeadBead(0);
	m_pLinkerHTBond->SetHeadBead(0);
	m_pLinkerTMBond->SetHeadBead(0);
	m_pLinkerTHBond->SetHeadBead(0);
	m_pLinkerMTBond->SetHeadBead(0);
	m_pLinkerMHBond->SetHeadBead(0);

	SetThreeBodyForceOff();

	aeBondTriangle* const pHeadConnector = dynamic_cast<aeBondTriangle*>(pHeadBond->GetBondConnection());

	if(pHeadConnector)
	{
		pHeadConnector->SetThreeBodyForceOff();
	}

	return true;
}

