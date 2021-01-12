/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpuExtendedNanoparticle.cpp: implementation of the mpuExtendedNanoparticle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimMPSFlags.h"
#include "xxParallelBase.h"
#include "mpuExtendedNanoparticle.h"
#include "mpuExtendedNPBond.h"
#include "mpuGhostBead.h"
#include "Nanoparticle.h"
#include "Bead.h"
#include "Bond.h"
#include "Polymer.h"
#include "ISimBoxBase.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

bool operator<(const mpuExtendedNanoparticle& a, const mpuExtendedNanoparticle& b)
{
	return a.GetId() < b.GetId();
}

bool operator==(const mpuExtendedNanoparticle& a, const mpuExtendedNanoparticle& b)
{
	return a.GetId() == b.GetId();
}

// Static member variable holding the number of extended nanoparticles created.

long mpuExtendedNanoparticle::m_CreatedExtNanoparticleTotal = 0;

// Static member function to return the current number of extended nanoparticles created.
// This is just the total number created minus the number deleted.

long mpuExtendedNanoparticle::GetTotal()
{
	return m_CreatedExtNanoparticleTotal;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Default constructor: this wraps a nanoparticle that is leaving the owning processor's space.

mpuExtendedNanoparticle::mpuExtendedNanoparticle() : m_bIsLeaving(true), m_pNanoparticle(0)
{
    ++m_CreatedExtNanoparticleTotal;
    std::cout << "********** Default constructor for mpuExtendedNanoparticle called...********** " << zEndl;

}

// Constructor for a nanoparticle that is about to leave its owning processor's space. We assume that no beads have left 
// the owning processor's space yet. All bonds in the NP are wrapped in extended bonds that
// themselves create ghost beads that wrap the concrete beads. 
//
// From this time on, the forces due to the bonds in the NP are calculated using the wrapper extended bonds. For bonds that 
// have both of their beads in the owning processor's space, we just call the normal bond force function. Bonds that are missing
// beads have their forces calculated separately.
//

mpuExtendedNanoparticle::mpuExtendedNanoparticle(CNanoparticle* pNP) : m_bIsLeaving(true), m_pNanoparticle(pNP)
{
#if EnableParallelSimBox == SimMPSEnabled
    ++m_CreatedExtNanoparticleTotal;
    std::cout << "********** Overloaded constructor for leaving mpuExtendedNanoparticle called...********** " << zEndl;
    
    // Iterate over the NP's bonds wrapping each in an extended bond instance. 
    
    m_lConcreteNPBonds.clear();
    m_lExtNPBonds.clear();
    
    BondVector vBonds = m_pNanoparticle->GetBonds();

    for(BondVectorIterator iterBond=vBonds.begin(); iterBond!=vBonds.end(); ++iterBond)
    {
        m_lConcreteNPBonds.push_back(new mpuExtendedNPBond(*iterBond));
    } 
#endif
}


// Constructor for a nanoparticle that is entering a processor's space. This is called when the first bead of an NP
// appears in a neighbouring processor's space. Even if several beads cross the boundary in the same time-step, there 
// is still an ordering to the beads, and the first one that appears in the list is used to create this extended NP.
// Subsequent beads are then added to the NP as they arrive.

mpuExtendedNanoparticle::mpuExtendedNanoparticle(CAbstractBead* pBead) : m_bIsLeaving(false), m_pNanoparticle(0)
{
    ++m_CreatedExtNanoparticleTotal;
    std::cout << "********** Overloaded constructor for arriving mpuExtendedNanoparticle called...********** " << zEndl;

}


// Copy constructor for mpuExtendedNanoparticle objects. We don't copy construct the wrapped CNanoparticle instance but just copy 
// it into the new extended nanoparticle.

mpuExtendedNanoparticle::mpuExtendedNanoparticle(const mpuExtendedNanoparticle &oldNP) : m_bIsLeaving(oldNP.m_bIsLeaving), m_pNanoparticle(oldNP.m_pNanoparticle)
{
	std::cout << "Copy constructing the " << mpuExtendedNanoparticle::GetTotal() << "  polymer that wraps polymer with id " << GetId() << " " << m_pNanoparticle->GetId() << zEndl;

}

// Assignment operator for mpuExtendedNanoparticle objets.
//
// We don't expect this to be used but it is good to define it anyway.

mpuExtendedNanoparticle& mpuExtendedNanoparticle::operator =(const mpuExtendedNanoparticle& oldNP)
{
	if( this != &oldNP)
	{

	}
	return *this;
}

// The CSimBox owns all CNanoparticles so we do not delete the wrapped nanoparticle instance here. 
// But we decrement the counter as Border nanoparticles may return to the Bulk region of a processor's Space 
// and the wrapped nanoparticle is removed leaving the empty extended nanoparticle to be destroyed.
// We also destroy all of the extended bonds and ghost beads but not the wrapped bond and bead instances.

mpuExtendedNanoparticle::~mpuExtendedNanoparticle()
{	
    if(!m_lConcreteNPBonds.empty())
	{
	    std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " nanoparticle " << GetId() << "  destroying its " << m_lConcreteNPBonds.size() << " concrete bonds" << zEndl;
	    for (ExtendedNPBondListIterator iterBond=m_lConcreteNPBonds.begin(); iterBond!= m_lConcreteNPBonds.end(); ++iterBond)
	    {
	        delete *iterBond;
	    }
	    m_lConcreteNPBonds.clear();
	}

    // The extended NP bonds should be empty as the nanoparticle is supposed to have moved back into the Bulk region
    
    if(!m_lExtNPBonds.empty())
	{
	    std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " ERROR: nanoparticle " << GetId() << "  destroying its " << m_lConcreteNPBonds.size() << " extended bonds" << zEndl;
	    for (ExtendedNPBondListIterator iterExtBond=m_lExtNPBonds.begin(); iterExtBond!= m_lExtNPBonds.end(); ++iterExtBond)
	    {
	        delete *iterExtBond;
	    }
	    m_lExtNPBonds.clear();
	}

    --m_CreatedExtNanoparticleTotal;
}

// Access functions that wrap the enclosed nanoparticle instance. We always
// check that their really is an enclosed instance before accessing it.
// The calling routine should check that the returned value is valid before
// using it.

long mpuExtendedNanoparticle::GetId() const
{
	if(m_pNanoparticle)
	{
		return m_pNanoparticle->GetId();
	}
	else
	{
		return -1;  // this is an invalid value
	}
}


long mpuExtendedNanoparticle::GetSize() const
{
    return 0;
}

// Function to calculate the forces due to bonds that are wholly contained within the owning processor's Space.
// We iterate over all the concrete bonds in the extended NP and call and use their normal force calculation.

void mpuExtendedNanoparticle::AddBondForces()
{
    for(ExtendedNPBondListIterator iterBond=m_lExtNPBonds.begin(); iterBond!=m_lExtNPBonds.end(); ++iterBond)
    {
        (*iterBond)->AddForce();
    }
}

// Function to calculate the forces due to bonds that cross a processor's boundaries. Such bonds are wrapped in 
// extended bond instances that handle the messaging required to obtain the missing bead's coordinates and to send the 
// resulting forces to the other processor.

void mpuExtendedNanoparticle::AddExtendedBondForces()
{


}

// Function that returns true if the wrapped nanoparticle contains only CBond instances that themselves contain two CBeads.
// This indicates that the nanoparticle has all of its beads in the owning processor's Space.

bool mpuExtendedNanoparticle::IsComplete() const
{
    return m_pNanoparticle->IsComplete();
}


