/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeActivePolymer.cpp: implementation of the aeActivePolymer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimMathFlags.h"
#include "aeActivePolymer.h"

#include "aeActiveBond.h"



// **********************************************************************
// Global Functions.
//
// Static member variable holding the number of active polymers created.

long aeActivePolymer::m_PolymerTotal = 0;

// Static member function to obtain the number of active polymers.

long aeActivePolymer::GetPolymerTotal()
{
	return m_PolymerTotal;
}

// Static member function to reset the number of active polymers to zero.

void aeActivePolymer::ZeroPolymerTotal()
{
	m_PolymerTotal = 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor for combining two free active bonds into an active polymer.
// Each free active bond contains only a tail monomer. 
//
// Note that because of the way lists are constructed, the first bond in the list
// is actually the tail of the polymer, and the last bond represents the head.
// Normally, polymers grow mainly at their head, so we use push_back to add
// new bonds to the polymers. The active bond functions AddXXXAdjacentBond()
// take care of connecting the monomers and beads within the active bonds.

aeActivePolymer::aeActivePolymer(aeActiveBond* const pHead, 
								 aeActiveBond* const pTail) : m_id(++aeActivePolymer::m_PolymerTotal),
															  m_pHead(pHead), m_pTail(pTail)
{
	m_Bonds.clear();

	// Store the bonds making up this active polymer

	m_Bonds.push_back(pTail);
	m_Bonds.push_back(pHead);

	// Bind the bonds to each other within a single function called on the tail bond

	pTail->AddHeadAdjacentBond(pHead);
}

// Constructor for wrapping a sequence of linked bonds in a new polymer instance.
// This constructor is used, for example, when formin polymers are broken up
// as a result of the SeverForminFilaments() command. The remnant of the polymer,
// which consists of a set of linked actin monomers, is used to start a new
// actin polymer.
//
// Note that because of the way lists are constructed, the first bond in the list
// is actually the tail of the polymer, and the last bond represents the head.
// Normally, polymers grow mainly at their head, so we use push_back to add
// new bonds to the polymers. The active bond functions AddXXXAdjacentBond()
// take care of connecting the monomers and beads within the active bonds.

aeActivePolymer::aeActivePolymer(aeActiveBond* const pTail) : m_id(++aeActivePolymer::m_PolymerTotal),
															  m_pHead(0), m_pTail(0)
{
	m_Bonds.clear();

	// Bonds are already connected to each other so we just have to add them
	// to the polymer's m_Bonds container and set the head and tail pointers.
	
	aeActiveBond* pBond = pTail;

	while(pBond)
	{
		m_Bonds.push_back(pBond);

		pBond = pBond->GetHeadAdjacentBond();
	}

	m_pHead = GetHeadBond();
	m_pTail = GetTailBond();
}


aeActivePolymer::~aeActivePolymer()
{
	// Note that we do not decrement the polymer ids as polymers may be destroyed
	// out of sequence, so the largest id is not necessarily free after a polymer
	// has been destroyed.

	// We destroy any contained bonds and empty the bond container. Note that there
	// are three situations in which polymers may be destroyed:
	//
	// 1) Program ends - everything is destroyed anyway
	// 2) Active Network is destroyed - polymers must destroy their contained bonds
	// 3) Polymer disintegrates into its component bonds - polymer must not destroy
	//    contained bonds as they have already been added back to the free bond
	//    container. This case must be dealt with in the ACN and its events to prevent
	//    bonds being both destroyed and added to the ACN's free bond container.

	if(!m_Bonds.empty())
	{
		for(ActiveBondListIterator iterBond=m_Bonds.begin(); iterBond!=m_Bonds.end(); iterBond++)
		{
			delete *iterBond;
		} 
		m_Bonds.clear();
	}

	m_pHead = 0;
	m_pTail = 0;
}

// Function to retrieve the head-end active bond in the polymer, i.e., the one
// at the back of the polymer's bond container.

aeActiveBond* aeActivePolymer::GetHeadBond() const
{
	if(!m_Bonds.empty())
		return m_Bonds.back();
	else
		return 0;
}

// Function to retrieve the tail-end active bond in the polymer, i.e., the one
// at the front of the polymer.

aeActiveBond* aeActivePolymer::GetTailBond() const
{
	if(!m_Bonds.empty())
		return m_Bonds.front();
	else
		return 0;
}


// Function to add an active bond to the head of the polymer. 
// The bond is added to the polymer's bond container, and connected to the
// current head bond, and finally the polymer's head bond pointer is updated.
// Note that the aeActiveBond::AddXXXAdjacentBond() function connects the two
// bonds both ways round.

void aeActivePolymer::AddHeadBond(aeActiveBond* const pBond)
{
	m_pHead->AddHeadAdjacentBond(pBond);
	m_pHead = pBond;

	m_Bonds.push_back(pBond);
}

// Function to add an active bond to the tail of the polymer. 
// The bond is added to the polymer's bond container and its head monomer
// updated to point to the tail monomer of the previous tail bond; and
// the polymer's tail bond pointer is updated to the new tail bond.

void aeActivePolymer::AddTailBond(aeActiveBond* const pBond)
{
	m_pTail->AddTailAdjacentBond(pBond);
	m_pTail = pBond;

	m_Bonds.push_front(pBond);
}

// Function to insert a bond somewhere along a polymer's length except as the first
// or last bond. Those positions are modified by AddHeadBond() and AddTailBond()
// respectively. The bond is inserted at the head of the bond specified by the first
// argument. We check that the insertion point is within the polymer and that the
// new bond is not, and return true if insertion is successful. Note that this function
// does not alter the head and tail bonds of the polymer.

bool aeActivePolymer::InsertBond(aeActiveBond* const pPreviousBond, aeActiveBond* const pBond)
{
	ActiveBondListIterator iterPrevious = find(m_Bonds.begin(), m_Bonds.end(), pPreviousBond);

	if(pPreviousBond != m_pHead && find(m_Bonds.begin(), m_Bonds.end(), pBond) == m_Bonds.end())
	{
		aeActiveBond* const pNextBond = (*iterPrevious)->GetHeadAdjacentBond();

		// Disconnect the pPreviousBond from pNextBond, add the new bond
		// in and then reconnect the pNextBond

		if(pPreviousBond->SeparateHeadAdjacentBond() && 
		   pPreviousBond->AddHeadAdjacentBond(pBond) &&
		   pBond->AddHeadAdjacentBond(pNextBond))
		{
			// Add the new bond to the polymer's bonds container

			m_Bonds.insert(++iterPrevious, pBond);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

// Function to remove the head bond from a polymer. If it has at least
// two bonds afterwards the function returns true, otherwise it returns false
// so that the active network can destroy the dissolved polymer and add its
// two bonds back to the free active bond list. Note that we clear the
// polymer's bond container without destroying the contained bonds, so that 
// polymers that disintegrate via this function do not destroy their contained 
// bonds. By contrast, polymers that are destroyed by their containing ACN
// just have their destructor called and it deletes all contained bonds before
// deleting the polymer.

bool aeActivePolymer::RemoveHeadBond()
{
	if(GetSize() > 2)
	{
		ActiveBondListIterator iterBond = --m_Bonds.end();

		aeActiveBond* const pNewHead = *(--iterBond);

		if(pNewHead->RemoveHeadAdjacentBond())
		{
			m_pHead = pNewHead;
			m_Bonds.pop_back();
		}

		return true;
	}
	else
	{
		m_pHead->DissolveFromHead();
		m_Bonds.clear();
		m_pHead = 0;
		m_pTail = 0;
		return false;
	}
}

// Function to remove the tail bond from a polymer. If it has at least
// two bonds afterwards the function returns true, otherwise it returns false
// so that the active network can destroy the dissolved polymer and add its
// two bonds back to the free active bond list. Note that we clear the
// polymer's bond container without destroying the contained bonds, so that 
// polymers that disintegrate via this function do not destroy their contained 
// bonds. By contrast, polymers that are destroyed by their containing ACN
// just have their destructor called and it deletes all contained bonds before
// deleting the polymer.


bool aeActivePolymer::RemoveTailBond()
{
	if(GetSize() > 2)
	{
		ActiveBondListIterator iterBond = m_Bonds.begin();

		aeActiveBond* const pNewTail = *(++iterBond);

		if(pNewTail->RemoveTailAdjacentBond())
		{
			m_pTail = pNewTail;
			m_Bonds.pop_front();
		}

		return true;
	}
	else
	{
		m_pHead->DissolveFromHead();
		m_Bonds.clear();
		m_pHead = 0;
		m_pTail = 0;
		return false;
	}

}

// Function to iterate over the active bonds in the polymer and add the forces
// due to them to the beads that make up the monomers. We include the first and
// last bonds in the loop even though they do not connect two monomers: a check
// is performed in the aeActiveBond::AddForce() function so that it only calculates
// the force when both monomers are present. 
//
// The forces generated by each bond type (derived from the aeActiveBond class)
// are defined in the class itself. Here, we just call a generic function to
// tell the bond to calculate its force(s).

void aeActivePolymer::AddForce()
{
	// Because of the way the bonds are stored in an STL container, the first
	// element in the container is the tail bond of the active polymer. We iterate
	// through the container towards the active polymer's head bond.

	for(ActiveBondListIterator iterBond=m_Bonds.begin(); iterBond!=m_Bonds.end(); iterBond++)
	{
		(*iterBond)->AddForce();
	}
}

// Function to return the maximum, number of active bonds in polymers of
// the given type. Note that as a polymer must have at least two bonds
// in it, we don't check for a null pointer.

long aeActivePolymer::GetMaxSize() const
{
	return m_pHead->GetActiveBondsPerPolymer();
}

// Functions to make the polymers visible/invisible in current state snapshots.
// The message is passed to all active bonds in the polymers and they pass it
// down to their contained CPolymers.

void aeActivePolymer::SetVisible(bool bVisible)
{
	for(ActiveBondListIterator iterBond=m_Bonds.begin(); iterBond!=m_Bonds.end(); iterBond++)
	{
		(*iterBond)->SetVisible(bVisible);
	}
}

void aeActivePolymer::SetVisible()
{
	for(ActiveBondListIterator iterBond=m_Bonds.begin(); iterBond!=m_Bonds.end(); iterBond++)
	{
		(*iterBond)->SetVisible(true);
	}
}

void aeActivePolymer::SetInvisible()
{
	for(ActiveBondListIterator iterBond=m_Bonds.begin(); iterBond!=m_Bonds.end(); iterBond++)
	{
		(*iterBond)->SetVisible(false);
	}
}
