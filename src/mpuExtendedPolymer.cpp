/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpuExtendedPolymer.cpp: implementation of the mpuExtendedPolymer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimMPSFlags.h"
#include "xxParallelBase.h"
#include "mpuExtendedPolymer.h"
#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"
#include "Polymer.h"
#include "mpuGhostBead.h"
#include "mpuExtendedBond.h"
#include "mpuExtendedBondPair.h"
#include "CurrentState.h"
#include "ISimBoxBase.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

bool operator<(const mpuExtendedPolymer& a, const mpuExtendedPolymer& b)
{
	return a.GetId() < b.GetId();
}

bool operator==(const mpuExtendedPolymer& a, const mpuExtendedPolymer& b)
{
	return a.GetId() == b.GetId();
}

// Static member variable holding the number of extended polymers created.

long mpuExtendedPolymer::m_CreatedExtPolymerTotal = 0;

// Static member function to return the current number of extended polymers created.
// This is just the total number created minus the number deleted.

long mpuExtendedPolymer::GetTotal()
{
	return m_CreatedExtPolymerTotal;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Default constructor

mpuExtendedPolymer::mpuExtendedPolymer() : m_pPolymer(0), m_bValid(false), m_HeadId(-1), m_TailId(-1)
{
    ++m_CreatedExtPolymerTotal;
    std::cout << "********** Default constructor for mpuExtendedPolymer called...********** " << zEndl;
    m_mGhostBeads.clear();
	m_lExtBonds.clear();
//	m_lExtBondPairs.clear();
}

// ****************************************
// Type-conversion constructor that wraps an EXISTING polymer instance when the first bead of the polymer leaves its owning processor's Space. 
// After this constructor has finished, the wrapped polymer's concrete bead instances must not be accessed as they will be recycled as the 
// polymer's beads leave the owning processor's Space. Only the ghost beads must be used from this point on.
//
// The polymer instance passed in is complete so we can use its information on beads, bonds and bondpairs to construct the extended polymer.
// We first create ghost beads that wrap the polymer's beads, except for a single empty ghost bead that represents the concrete bead instance 
// that has just left the owning processor's Space.  Next, we create extended bonds to wrap the polymer's own bonds, and connect the 
// newly-created ghost beads according to the polymer's original bonded shape. Finally, we create bondpairs if the original polymer has any.
//
// This class assumes that the ids of the beads in the polymer are CONSECUTIVE and uses this information to create the extended bonds that
// link beads. This is especially critical if the polymer is branched or looped, and this class will not function correctly if the bead ids
// are not consecutive.

mpuExtendedPolymer::mpuExtendedPolymer(long newPid, const CAbstractBead* const pBead, CPolymer* const pPolymer) : m_pPolymer(pPolymer), 
                                       m_bValid(true), m_HeadId(-1), m_TailId(-1)
{
#if EnableParallelSimBox == SimMPSEnabled

    ++m_CreatedExtPolymerTotal;
		
//	std::cout <<  "Proc " << xxParallelBase::GlobalGetRank() << " Creating the " << mpuExtendedPolymer::GetTotal() << " polymer to wrap the existing polymer with id " << GetId() << " " << pPolymer->GetId() << " " << pBead->GetPolymer()->GetId()  << " and its gained bead id " << pBead->GetId() << " in location " << location << zEndl;

    m_mGhostBeads.clear();
	m_lExtBonds.clear();
//	m_lExtBondPairs.clear();
	
	// Store a pointer to this extended polymer in the wrapped polymer instance for use when the polymer spans several processors' Spaces.
	
	m_pPolymer->SetExtendedPolymer(this);
	
	// Iterate over the polymer's shape, creating ghost beads for all its beads, and extended bonds to connect them. Each ghost wraps one bead
	// except for the one that represents the missing bead that simply holds data on the bead. It also stores the pid of the processor 
	// that owns the missing bead. We also set the indices to the polymer's head and tail beads. But this particular algorithm only works
	// if the wrapped polymer is complete, and has actually had its head and tail beads defined. Hence, it will not work for a partially-filled
	// polymer.
		
	mpuGhostBead* pGhost;
	
	for(BeadVectorIterator iterBead=m_pPolymer->GetBeads().begin(); iterBead!=m_pPolymer->GetBeads().end(); iterBead++)
	{
	    CAbstractBead* pNextBead = *iterBead;
		
		// Note that the polymer's head and tail may be the same bead
		if(pNextBead == m_pPolymer->GetHead())
		{
		    m_HeadId = pNextBead->GetId();
		}
		if(pNextBead == m_pPolymer->GetTail())
		{
		    m_TailId = pNextBead->GetId();
		}
		
		if(pNextBead == pBead)
		{
		    // We represent the missing bead instance by an empty ghost bead that stores the pid of its new owning processor
			pGhost = new mpuGhostBead(newPid, pBead);
		}
		else
		{
		    // and create ghost beads to wrap the remaining beads
		    pGhost = new mpuGhostBead(pNextBead);
		}
		
		pair<LongGhostBeadIterator, bool> iterInsert;
		iterInsert = m_mGhostBeads.insert(make_pair(pGhost->GetId(), pGhost));
					
		if(!iterInsert.second)
		{
			// An error occurred so we destroy the newly-created ghost
			std::cout << "Error in mpuExtendedPolymer: unable to insert ghost bead in map " <<  pGhost->GetId() <<  " for polymer " << pPolymer->GetId() << " on processor " << newPid << zEndl;
            delete pGhost;
		}			
	}
	
	// Now create mpuExtendedBonds to wrap the CBond instances in the polymer. We iterate over the bonds in the original polymer,
	// obtain the unique ids of each bond's head and tail beads, then find the ghost beads that wrap these beads. One of these beads 
	// is an empty ghost and the extended bond's constructor handles this case. We store the CBond instance in the extended bond in case
	// the missing bead returns to the owning processor. But it should not be used as its bead pointers may be invalid when their bead instances
	// are re-used.
	
	mpuExtendedBond* pExtBond;
	mpuGhostBead* pHeadGhost = 0;
	mpuGhostBead* pTailGhost = 0;
	
	for(BondVectorIterator iterBond=m_pPolymer->GetBonds().begin(); iterBond!=m_pPolymer->GetBonds().end(); iterBond++)
	{
	    const long headId = (*iterBond)->GetHead()->GetId();
	    const long tailId = (*iterBond)->GetTail()->GetId();
		
	    pHeadGhost = m_mGhostBeads.find(headId)->second;
	    pTailGhost = m_mGhostBeads.find(tailId)->second;
		
		if(pHeadGhost && pTailGhost)
		{
		    pExtBond = new mpuExtendedBond(*iterBond, pHeadGhost, pTailGhost);
			m_lExtBonds.push_back(pExtBond);
		}
		else
		{
		    std::cout << "ERROR in mpuExtendedBond constructor: could not find head or tail ghost bead for bond " << (*iterBond)->GetId() << " " <<  headId<< " " << tailId << zEndl;
		}
		
	}
	
	// Now create mpuExtendedBondPairs - TBD

#endif	
}

// ****************************************
// Overloaded type-conversion constructor that creates an extended polymer containing a SINGLE concrete bead that has just arrived from another processor. 
//
// Note that the polymer instance that is passed in is empty: only its id and type have been set.  We insert its first bead here, but
// add its remaining beads and bonds, etc, in the ReplaceGhostByBead() function as each bead arrives.  This also means that we cannot 
// retrieve the first bond and bondpair ids from the passed-in polymer instance as there are none: instead we use the other arguments  
// to set the ids of the extended bond or bondpair instances.  An exemplar polymer is used to construct the extended polymer's shape
// with its ghost beads connected by extended bonds and bondpairs. Only one ghost bead actually wraps a concrete bead of course, and the 
// extended bonds are also empty.
//
// This overload is used when a processor receives the first bead of a polymer that has just entered its Space.  We create ghost beads 
// for the polymer, and we set their ids from the fact that beads are numbered sequentially in a polymer  and we know the id of the bead 
// that occupies the position in the polymer specified by the "location" parameter.   Subsequent beads are added, together with their 
// bonds and stiff bonds, in the function ReplaceGhostByBead().
//

mpuExtendedPolymer::mpuExtendedPolymer(long oldPid, CAbstractBead* const pBead, CPolymer* const pPolymer, CPolymer* const pExemplar, long polySize, 
                                       long location,  long firstBondId, long firstBondPairId) : m_pPolymer(pPolymer), m_bValid(false), m_HeadId(-1), m_TailId(-1)
{
#if EnableParallelSimBox == SimMPSEnabled

    ++m_CreatedExtPolymerTotal;
		
    m_mGhostBeads.clear();
	m_lExtBonds.clear();
//	m_lExtBondPairs.clear();

	// Store a pointer to this extended polymer in the wrapped polymer instance for use when the polymer spans several processors' Spaces.
	
	m_pPolymer->SetExtendedPolymer(this);
	
// Iterate over the number of beads in the exemplar polymer, creating a ghost bead for each and storing the passed-in bead in the appropriate ghost
// when we reach the location specified in the last argument. We also store the newly-created ghosts in a temporary vector so that we can access
// them to create the extended bonds and bondpairs. We cannot use the exemplar polymer to obtain the head and tail bead ids as all of its beads
// have the illegal value -1. Instead we use the function that returns the location of the head and tail beads in the exemplar polymer's shape 
// to find the equivalent ghost beads in this extended polymer and use their ids.

    GhostBeadVector vGhosts;
	vGhosts.clear();
	
    mpuGhostBead* pGhost;
	
	const long polyId = pPolymer->GetId();
	const long headIndex = pExemplar->GetHeadIndex();
	const long tailIndex = pExemplar->GetTailIndex();
	
	long beadId = pBead->GetId() - location;
	
    for(long ib=0; ib<polySize; ib++)
	{
		if(ib == headIndex)
		{
		    m_HeadId = beadId;
		}
		if(ib == tailIndex)
		{
		    m_TailId = beadId;
		}
		
	    if(ib != location)
		{
		    // We create an empty ghost bead using the  constructor that sets just the bead id and polymer id as we do not know any of the 
			// other values. Subsequent routines must fill in this data as it becomes available.			
			pGhost = new mpuGhostBead(beadId, polyId);
		}
		else
		{
		    // We wrqp the single concrete bead in a ghost bead 
			pGhost = new mpuGhostBead(pBead);
		}
	
	    pair<LongGhostBeadIterator, bool> iterInsert;
	    iterInsert = m_mGhostBeads.insert(make_pair(pGhost->GetId(), pGhost));
		vGhosts.push_back(pGhost);
			
	    if(!iterInsert.second)
	    {
		    std::cout << "ERROR - failed to insert ghost bead with id " << pGhost->GetId() << " into map for polymer " << pPolymer->GetId() << zEndl;
            delete pGhost;
	    }

        ++beadId;
	}
		
    // Now we have the ghost beads, we create extended bonds (and bondpairs) using the exemplar polymer to connect the ghost beads 
	// according to the polymer's architecture.
	// First, we iterate over the exemplar polymer's bonds, and create an empty extended bond for each one whose head and tail index are set
	// from the order of the bonds in the exemplar polymer. Note that this assumes that the bonds are present in the polymer in the order
	// corresponding to the polymer's shape, ie, no rearrangement of bonds has occurred.
		
	mpuExtendedBond* pExtBond;

	for(cBondVectorIterator citerBond=pExemplar->GetBonds().begin(); citerBond!=pExemplar->GetBonds().end(); citerBond++)
	{
	    const long headIndex = (*citerBond)->GetHeadIndex();
	    const long tailIndex = (*citerBond)->GetTailIndex();
		
        mpuGhostBead* const pHeadGhost = vGhosts.at(headIndex);
        mpuGhostBead* const pTailGhost = vGhosts.at(tailIndex);
		
		if(pHeadGhost && pTailGhost)
		{
            pExtBond = new mpuExtendedBond(*citerBond, firstBondId, pHeadGhost, pTailGhost);
			m_lExtBonds.push_back(pExtBond);
			++firstBondId;
		}
		else
		{
		    std::cout << "ERROR in mpuExtendedBond constructor: could not find head or tail ghost bead for bond " << (*citerBond)->GetId() << " " <<  headIndex << " " << tailIndex << zEndl;
		}
	}
	
#endif
}


// Copy constructor for mpuExtendedPolymer objects. Note that we do NOT
// create a new instance of CPolymer here but only copy the pointer from
// the old mpuExtendedPolymer instance into the new one.
// This is a so-called shallow copy.

mpuExtendedPolymer::mpuExtendedPolymer(const mpuExtendedPolymer &oldPolymer) : m_pPolymer(oldPolymer.m_pPolymer), m_bValid(oldPolymer.m_bValid),
                                                                               m_HeadId(oldPolymer.m_HeadId), m_TailId(oldPolymer.m_TailId) 
{
	std::cout << "Copy constructing the " << mpuExtendedPolymer::GetTotal() << "  polymer that wraps polymer with id " << GetId() << " " << m_pPolymer->GetId() << zEndl;

    m_mGhostBeads      = oldPolymer.m_mGhostBeads;
	m_lExtBonds        = oldPolymer.m_lExtBonds;
	m_lExtBondPairs    = oldPolymer.m_lExtBondPairs;
	
	// We don't copy the CPolymer's extended polymer pointer as we are re-using the old polymer instance that already contains it.
}

// Assignment operator for mpuExtendedPolymer objets.
//
// We don't expect this to be used but it is good to define it anyway.

mpuExtendedPolymer& mpuExtendedPolymer::operator =(const mpuExtendedPolymer& oldPolymer)
{
	if( this != &oldPolymer)
	{

	}
	return *this;
}

// The mpsSimBox owns all polymers so we do not delete the wrapped polymer instance here. The empty concrete bonds and bondpairs are also owned by
// their polymers, so we don't delete them either. But we do delete the ghost beads and extended bonds
// (and bondpairs if any) that were created in the constructor.

mpuExtendedPolymer::~mpuExtendedPolymer()
{	
     // Decrement the counter to keep track of how many extended polymers still exist
	
    for(LongGhostBeadIterator iterGhost=m_mGhostBeads.begin(); iterGhost!=m_mGhostBeads.end(); iterGhost++)
	{
	    mpuGhostBead* pGhost = iterGhost->second;
	    delete pGhost;
	}
	m_mGhostBeads.clear();
	
	// Now delete the extended bonds
	for(ExtendedBondListIterator iterBond=m_lExtBonds.begin(); iterBond!=m_lExtBonds.end(); iterBond++)
	{
	    mpuExtendedBond* pBond = *iterBond;
		delete pBond;
	}
	m_lExtBonds.clear();
	
	// and extended bondpairs if any
}

// Protected function to list the contents of the extended polymer for debug.

void mpuExtendedPolymer::ListBeads() const 
{
	std::cout << "Proc " << IGlobalSimBox::GlobalGetRank() << " Extended polymer " << GetId() << " with validity " << GetValid() << " has " << m_mGhostBeads.size() << " ghost beads (concrete/empty):  ";
	
	for(cLongGhostBeadIterator citerGhost=m_mGhostBeads.begin(); citerGhost!=m_mGhostBeads.end(); citerGhost++)
	{
	    std::cout << citerGhost->second->GetId() << " ( " << !(citerGhost->second->IsEmpty()) << " )  ";
	}
	std::cout << zEndl;
	for(cLongGhostBeadIterator citerGhost=m_mGhostBeads.begin(); citerGhost!=m_mGhostBeads.end(); citerGhost++)
	{
	    if(citerGhost->second->GetBead())
		{
	        std::cout << citerGhost->second->GetId() << " (" << citerGhost->second->GetBead()->GetId() << " " << citerGhost->second->GetBead()->GetType() << ") ";
		}
		else
		{
	        std::cout << citerGhost->second->GetId() << " (0) " ;
		}
	}
	std::cout << zEndl;
	std::cout << "with head/tail beads " << m_HeadId << " " << m_TailId << zEndl;
	if(m_lExtBonds.size() > 0)
	{
        std::cout << " and " << m_lExtBonds.size() << " extended bonds: " << zEndl;
		std::cout << " Ext Bond id/add, concrete bond id/add, head index, tail index, head id, tail id, Spring const, Unstretched" << zEndl;
	
	    for(cExtendedBondListIterator citerExtBond=m_lExtBonds.begin(); citerExtBond!=m_lExtBonds.end(); citerExtBond++)
	    {
		    if( (*citerExtBond)->GetBond())
			{
		        std::cout << (*citerExtBond)->GetId() << " (add: " << (*citerExtBond) << ") wraps concrete bond id " << (*citerExtBond)->GetBond()->GetId() << " (add: " << (*citerExtBond)->GetBond() << ") " << (*citerExtBond)->GetHeadIndex() << " " << (*citerExtBond)->GetTailIndex() << " " << (*citerExtBond)->GetHeadGhost()->GetId() << " " << (*citerExtBond)->GetTailGhost()->GetId() << " " << (*citerExtBond)->GetSpringConstant() << " " << (*citerExtBond)->GetUnstretchedLength() << zEndl;
            }
			else
			{
		        std::cout << (*citerExtBond)->GetId() << " wraps empty concrete bond " << " " << (*citerExtBond)->GetHeadIndex() << " " << (*citerExtBond)->GetTailIndex() << " " << (*citerExtBond)->GetHeadGhost()->GetId() << " " << (*citerExtBond)->GetTailGhost()->GetId() << " " << (*citerExtBond)->GetSpringConstant() << " " << (*citerExtBond)->GetUnstretchedLength() << " " << (*citerExtBond)->GetHeadGhost()->GetPid() << " " <<  (*citerExtBond)->GetTailGhost()->GetPid() << zEndl;
			}
	    }
	}
}

// Protected function to list the concrete bead instances wrapped inside the ghost beads for debug.

void mpuExtendedPolymer::ListConcreteBeads() const 
{
#if EnableParallelSimBox == SimMPSEnabled

	std::cout << "Proc " << IGlobalSimBox::GlobalGetRank() << " Extended polymer " << GetId() << " with validity " << GetValid() << " wrapping concrete polymer " << m_pPolymer->GetId() << " (type " << m_pPolymer->GetType() << ") has " << m_pPolymer->GetSize() << " concrete beads:  " << zEndl;
	
	if(m_pPolymer->GetSize() > 0)
	{
	    std::cout << "Head bead is " << m_pPolymer->GetHead()->GetId() << " and tail bead is " << m_pPolymer->GetTail()->GetId() << zEndl;
	    cBeadVectorIterator citerBead;
	    for( citerBead=m_pPolymer->GetBeads().begin(); citerBead!=m_pPolymer->GetBeads().end(); citerBead++)
	    {
	        std::cout << (*citerBead)->GetId() << " ";
	    }
	    std::cout << zEndl;
	    std::cout << "with types" << zEndl;
	    for( citerBead=m_pPolymer->GetBeads().begin(); citerBead!=m_pPolymer->GetBeads().end(); citerBead++)
	    {
	        std::cout << (*citerBead)->GetType() << " ";
	    }
	    std::cout << zEndl;
	    std::cout << "and owning polymers" << zEndl;
	    for( citerBead=m_pPolymer->GetBeads().begin(); citerBead!=m_pPolymer->GetBeads().end(); citerBead++)
	    {
		    if( (*citerBead)->GetPolymer())
			{
			     std::cout << (*citerBead)->GetPolymer()->GetId() << " ";
		    }
			else
			{
			    std::cout << "No Owning polymer" << zEndl;
			}
	    }
		std::cout << zEndl;
	}
	
	if(m_pPolymer->GetBonds().size() > 0)
	{
        std::cout << " and  bonds: " << zEndl;
		std::cout << " Bond id, head index, tail index, head id, tail id, Spring const, Unstretched length" << zEndl;
	
	    for(cBondVectorIterator citerBond=m_pPolymer->GetBonds().begin(); citerBond!=m_pPolymer->GetBonds().end(); citerBond++)
	    {
		    std::cout << (*citerBond)->GetId() << " " << (*citerBond)->GetHeadIndex() << " " << (*citerBond)->GetTailIndex() << " " << (*citerBond)->GetHead()->GetId() << " " << (*citerBond)->GetTail()->GetId() << " " << (*citerBond)->GetSprConst() << " " << (*citerBond)->GetUnStrLength() << zEndl;
	    }
	}
	
#endif
}


// Access functions that wrap the enclosed polymer instance. We always
// check that their really is an enclosed instance before accessing it.
// The calling routine should check that the returned value is valid befire
// using it.

long mpuExtendedPolymer::GetId() const
{
	if(m_pPolymer)
	{
		return m_pPolymer->GetId();
	}
	else
	{
		return -1;  // this is an invalid value
	}
}

long mpuExtendedPolymer::GetType() const
{
	if(m_pPolymer)
	{
		return m_pPolymer->GetType();
	}
	else
	{
		return -1;  // this is an invalid value
	}
}

// We return the number of ghost beads as this corresponds to the number of beads in a complete polymer instance whereas the wrapped 
// polymer may be incomplete.

long mpuExtendedPolymer::GetSize() const
{
    return m_mGhostBeads.size();
}

// We return the bead wrapped by the ghost bead with the id of the original polymer's head. Note that if the wrapped polymer
// is incomplete, this bead may be invald.

CAbstractBead* mpuExtendedPolymer::GetHead() const
{
	if(m_mGhostBeads.find(m_HeadId) != m_mGhostBeads.end())
	{
	    mpuGhostBead* pGhost = m_mGhostBeads.find(m_HeadId)->second;
		return pGhost->GetBead();
	}
	else
	{
		return 0;
	}
}

// This function will fail if the polymer's Tail bead crosses the processor boundaries. I have to replace it with one that
// is safe.

CAbstractBead* mpuExtendedPolymer::GetTail() const
{
	if(m_mGhostBeads.find(m_TailId) != m_mGhostBeads.end())
	{
	    mpuGhostBead* pGhost = m_mGhostBeads.find(m_TailId)->second;
		return pGhost->GetBead();
	}
	else
	{
		return 0;
	}
}

// This function will work as long as there is at least one bead in the polymer.

bool mpuExtendedPolymer::GetVisible() const
{
	if(m_pPolymer)
	{
		return m_pPolymer->GetVisible();
	}
	else
	{
		return false;
	}
}

void mpuExtendedPolymer::SetVisible(bool bVisible)
{
	if(m_pPolymer)
	{
		m_pPolymer->SetVisible(bVisible);
	}
}

void mpuExtendedPolymer::SetVisible()
{
	if(m_pPolymer)
	{
		m_pPolymer->SetVisible();
	}
}

void mpuExtendedPolymer::SetInvisible()
{
	if(m_pPolymer)
	{
		m_pPolymer->SetInvisible();
	}
}

void mpuExtendedPolymer::SetDisplayId(long id)
{
	if(m_pPolymer)
	{
		m_pPolymer->SetDisplayId(id);
	}
}

// Function to return the CM of all beads in the polymer. We pass back 
// an aaVector containing the three components of the vector.
// We have to modify this for extended polymers as their beads are in different
// processor Spaces. First, we check that the wrapped polymer has a non-zero size.

aaVector mpuExtendedPolymer::GetCM() const
{
    double xcm = 0.0;
    double ycm = 0.0;
    double zcm = 0.0;

/*  const double beadTotal  = static_cast<double>(GetSize());

    if(beadTotal > 0)
    {
		for(cBeadVectorIterator citerBead = m_vBeads.begin(); citerBead != m_vBeads.end(); citerBead++)
		{
			const double x = (*citerBead)->GetXPos();
			const double y = (*citerBead)->GetYPos();
			const double z = (*citerBead)->GetZPos();

			xcm += x;
			ycm += y;
			zcm += z;
		}

		xcm /= beadTotal;
		ycm /= beadTotal;
		zcm /= beadTotal;
    }
*/

    aaVector cm(xcm, ycm, zcm);

    return cm;
}

// Function to calculate the forces due to bonds belonging to this extended polymer both of whose beads are in the owning processor's Space.
// The forces dues to extended bonds that span processor boundaries are calculated elswhere as they require inter-processor messaging.
//
// We iterate over the extended bond instances and calculate the force for those extended bonds both of whose ghost beads wrap a concrete bead instance.
// We cannot use the concrete bead instances in the contained concrete polymer because they may have moved to other processors, but we can use the
// concrete beads that are held in the ghost beads.
//
// Note that the coordinates for ghost bonds in an extended polymer must have been updated before calling this routine; as they are translated
// into the owning processor's Space we do not have to apply the PBCs to them when calculating the bond length.

void mpuExtendedPolymer::AddBondForces()
{
    for(ExtendedBondListIterator iterBond=m_lExtBonds.begin(); iterBond!=m_lExtBonds.end(); iterBond++)
	{
	    (*iterBond)->AddForce();
	}
}

// Function to count the number of concrete beads in the extended polymer. The number of ghost beads should be constant, and equal to
// the size of the wrapped polymer, but the number of concrete and empty ghosts varies as beads move across processor boundaries.

long mpuExtendedPolymer::GetConcreteBeadTotal() const
{
    long total = 0;
    for(cLongGhostBeadIterator iterBead=m_mGhostBeads.begin(); iterBead!=m_mGhostBeads.end(); iterBead++)
	{
	    if(iterBead->second->GetBead())
		{
		    ++total;
		}
	}

    return total;
}

// Function to count the number of empty ghost beads in the extended polymer. This should be equal to the size of the polymer minus the
// number of concrete beads. We do a check for this.

long mpuExtendedPolymer::GetEmptyGhostBeadTotal() const
{
	long total = 0;
	
    for(cLongGhostBeadIterator iterBead=m_mGhostBeads.begin(); iterBead!=m_mGhostBeads.end(); iterBead++)
	{
	    if(!(iterBead->second->GetBead()))
		{
		    ++total;
		}
	}
		
	return total;
}

// Function to return the location of the ghost bead specified by the argument in the extended polymer's shape.
// This is used to pass beads between procesor's when the polymer's structure is not complete on the sending processor.
// We know that the wrapping extended polymer contains a complete set of ghost beads corresponding to the polymer's shape.

long mpuExtendedPolymer::GetGhostBeadLocation(long id) const
{
	long index = -1;	
	bool bNotFound = true;
	
    cLongGhostBeadIterator citerGhost=m_mGhostBeads.begin();		
	
	while(bNotFound && citerGhost!=m_mGhostBeads.end())
	{
		if(citerGhost->second->GetId() == id)
		{
			bNotFound = false;
		}
		++index;
		++citerGhost;
	}
		
	return index;
}

// Function to return the first bond id for the wrapped polymer using the extended bonds that wrap the concrete polymer's bonds.
// We have to do it this way because polymers that are assembed when beads move one by one into a processor's Space do not contain
// concrete bond instances until all the polymer's beads have arrived. But the extended polymer already contains valid extended bonds
// and bondpairs.

long mpuExtendedPolymer::GetFirstExtendedBondId() const
{
    if(!m_lExtBonds.empty())
	{
	    return m_lExtBonds.front()->GetId();
	}
	else
	{
	    return 0;
	}
}

// Function to return the first bond id for the wrapped polymer using the extended bonds that wrap the concrete polymer's bonds.
// We have to do it this way because polymers that are assembed when beads move one by one into a processor's Space do not contain
// concrete bond instances until all the polymer's beads have arrived. But the extended polymer already contains valid extended bonds
// and bondpairs.

long mpuExtendedPolymer::GetFirstExtendedBondPairId() const
{
    if(!m_lExtBondPairs.empty())
	{
	    return m_lExtBondPairs.front()->GetId();
	}
	else
	{
	    return 0;
	}
}


// Function showing if all of the extended polymer's ghost beads are empty. This indicates that the wrapped polymer has moved 
// to another processor and this instance can be deleted.

bool mpuExtendedPolymer::IsEmpty() const
{
    bool bIsEmpty = true;
		
	for(cLongGhostBeadIterator citerGhost=m_mGhostBeads.begin(); citerGhost!=m_mGhostBeads.end(); citerGhost++)
	{
		if(citerGhost->second->GetBead())
		{
			bIsEmpty = false;
		}
	}
	
	return bIsEmpty;
}

// Function showing if the polymer contains its complete set of beads.  Note that we exclude the case where all the extended polymer's 
// ghost beads are empty. This indicates that the polymer that is wrapped by this extended polymer has now moved to another processor(s) 
// which will take care of it.
// 
// If all the ghost beads contain a concrete bead, the polymer is complete, otherwise it is extended. 
// Note that we only check if the ghost beads contain a wrapped bead - we cannot use the wrapped polymer as it will always contain pointers
// to concrete beads, but these pointers are invalid once some of its beads have left its original processor's Space.

bool mpuExtendedPolymer::IsComplete() const
{
    bool bIsComplete = true;
		
	for(cLongGhostBeadIterator citerGhost=m_mGhostBeads.begin(); citerGhost!=m_mGhostBeads.end(); citerGhost++)
	{
		if(!citerGhost->second->GetBead())
		{
			bIsComplete = false;
		}
	}
	
	return (!IsEmpty() && bIsComplete);
}

// Function to remove a CAbstractBead instance from the extended polymer and leave behind an empty ghost bead when the bead leaves the owning
// processor's Space. This function must only be called to replace beads OTHER THAN the first.  When the first bead of a polymer leaves 
// the owning processor's Space, we create a new extended polymer.
//
// Note that we cannot assume that the wrapped concrete polymer instance is valid or complete: when a bead moves from a processor that does
// not own its polymer to another processor that does not own its polymer, the polymer instance will be empty. But the extended polymer
// and its contained ghost beads and extended bonds will all be valid.
//
// We know that the constructor has created ghost beads to wrap the polymer's concrete beads, and extended bonds to wrap the polymer's
// concrete bonds. We remove the missing bead from its wrapper ghost bead, but leave it in the concrete polymer even though the bead will 
// be recycled. This allows the molecular shape of the polymer to be retained, and speeds up replacing the lost bead if it quickly moves
// back into the owning processor's Space. Note that even though the bead's pointer will be invalid, we can still find out where the bead was,
// and hence replace it with a valid bead instance, by using its location in the polymer's shape.
//
// We leave the extended bonds and bondpairs alone as they contain ghost beads that are always valid.

bool mpuExtendedPolymer::RemoveBeadFromGhost(long newPid, CAbstractBead* const pBead)
{
    bool bSuccess = true;
		
#if EnableParallelSimBox == SimMPSEnabled
		
	// Find the wrapper ghost instance and remove its concrete bead, storing the pid of its new owning processor for later use.
									
	if(m_mGhostBeads.find(pBead->GetId()) != m_mGhostBeads.end())
	{		
	        mpuGhostBead* pGhost = m_mGhostBeads.find(pBead->GetId())->second;		
			
			if(pGhost)
			{
	            pGhost->RemoveBead(newPid);		
					
			}
			else
			{
			    std::cout << "ERROR in extended polymer - bead " << pBead->GetId() << " in extended polymer " << GetId() << " has invalid ghost " << pGhost << " " << pGhost->GetId() << zEndl;
                ListBeads();
                ListConcreteBeads();
			}
	}
	else
	{
	    std::cout << " ERROR in extended polymer - unable to find ghost bead with id " << pBead->GetId() << "  in extended polymer " << GetId() << zEndl;
	    std::cout << "Extended polymer " << GetId() << " losing concrete bead " << pBead->GetId() << " from polymer " << m_pPolymer->GetId() << " that contains " << m_pPolymer->GetSize() << " beads,  in position " << m_pPolymer->GetBeadLocation(pBead) << " moving to processor " << newPid << zEndl;
        ListBeads();
        ListConcreteBeads();
	}
			
#endif

    return bSuccess;
}

// Function to wrap a concrete bead instance in a ghost bead and add it to the extended polymer's ghost bead container. 
// This occurs when the bead moves into a processor's Space and its owning polymer already exists in that processor's Space.
// Note that we only create concrete bonds and bondpairs when the final bead is added, and we connect them up according to the
// architecture defined by the extended poymer's extended bonds and bondpairs.
// 
// We use the bead id to locate the particular ghost that receives
// the new bead. We also set the newly-arrived bead's owning polymer pointer. Finally, we check if this is the last bead needed 
// to complete the concrete polymer and, if so, we udpate the polymer with the data it needs so that it can be moved into the bulk
// polymer container and the extended polymer destroyed.

bool mpuExtendedPolymer::AddBeadToGhost(long oldPid, CAbstractBead* pBead)
{
    bool bSuccess = true;
		
#if EnableParallelSimBox == SimMPSEnabled

    mpuGhostBead* pGhost;  // pointers used in several places below
	mpuExtendedBond* pExtBond;

	if(m_mGhostBeads.find(pBead->GetId()) != m_mGhostBeads.end())
	{
	    pBead->SetOwningPolymer(m_pPolymer);
		pGhost = m_mGhostBeads.find(pBead->GetId())->second;
		pGhost->AddBead(oldPid, pBead);
	}
	else
	{
	    std::cout << "********** Error in AddBeadToGhost: polymer " << GetId() << " is unable to find ghost bead with id " << pBead->GetId() << " (type " << pBead->GetType() << ") with owning polymer " << pBead->GetPolymer()->GetId() << " arriving from proc " << oldPid << " candidates are:" << zEndl;
		Dump();
		bSuccess = false;
	}
		
    // If all the ghosts now wrap a concrete bead instance, we set the concrete polymer's data. There are two cases: an existing polymer 
	// may have lost some beads across a processor boundary that have now returned - this is indicated by the m_bValid flag being true; 
	// or the polymer may have been assembled out of beads all of which have newly entered the owning processor's Space - indicated by the 
	// flag being false.
	// 
	// In the first case, the wrapped polymer's beads, bonds and bondpairs are contained in the ghost bead, bond and bondpair instances.
	// Because beads get recycled, we just clear the concrete polymer's bead container and insert the ghost beads' concrete instances.
	// The bonds and bondpairs are taken from the wrapped polymer as they do not get recycled.
			
	if(IsComplete())
	{				
		if(GetValid())
		{		
            m_pPolymer->ClearBeads();
		
		    for(LongGhostBeadIterator iterGhost=m_mGhostBeads.begin(); iterGhost!=m_mGhostBeads.end(); iterGhost++)
		    {
		        pGhost = iterGhost->second;
			    CAbstractBead* pBead1 = pGhost->GetBead();				
                m_pPolymer->AddBead(pBead1);
								
				// Set the polymer's head and tail pointers
			    if(pBead1->GetId() == m_HeadId)
				{
				    m_pPolymer->SetHead(pBead1);
				}
			    if(pBead1->GetId() == m_TailId)
				{
				    m_pPolymer->SetTail(pBead1);
				}
		    }
						
			for(ExtendedBondListIterator iterExtBond=m_lExtBonds.begin(); iterExtBond!=m_lExtBonds.end(); iterExtBond++)
			{
			    pExtBond = *iterExtBond;
				CBond* pBond = pExtBond->GetBond();
				
				pBond->SetBeads(pExtBond->GetHeadGhost()->GetBead(), pExtBond->GetTailGhost()->GetBead());
				pBond->SetBeadIndex(pExtBond->GetHeadIndex(), pExtBond->GetTailIndex());
			}
/*
			if(GetId() == 2999)
			{
	            std::cout << "**********" << zEndl;
		        std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " finished assembling returned polymer instance: " << m_pPolymer->GetId() << " " << m_pPolymer->GetType() << " " << m_pPolymer->GetBeads().size() << " " << m_pPolymer->GetBonds().size() << " " << m_pPolymer->GetBondPairs().size() << " " << m_pPolymer->GetHead()->GetId() << " " << m_pPolymer->GetTail()->GetId() << zEndl; 
			    std::cout << "using " << m_mGhostBeads.size() << " ghost bead instances and " << m_lExtBonds.size() << " extended bond instances; dumping beads " <<  zEndl;
		        ListBeads();
//			    ListConcreteBeads();
	            std::cout << "********************" << zEndl;
            }  */
            // Now the bondpairs - TBD
			
		}
		else
		{
		    // In this case, the extended polymer has been assembled out of beads that have all arrived from other processors, so the
			// wrapped polymer's beads, bonds and bondpairs are empty. We insert the ghost beads' wrapped beads into the polymer, and
			// create new bond and bondpair instances.
					
			if(m_pPolymer->GetSize() > 0)
			{
			    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << zEndl;
		        std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " clearing polymer on receiving final bead " << pBead->GetId() << " of extended polymer " << GetId() << " (" << GetPolymer()->GetId() << ") from proc " << oldPid << zEndl;
			    std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << zEndl;
                m_pPolymer->ClearBeads();
                m_pPolymer->ClearBonds();
                m_pPolymer->ClearBondPairs();
			
			}
			
  		    m_pPolymer->SetId(GetId());
			m_pPolymer->SetType(GetType());
			m_pPolymer->SetExtendedPolymer(this);

		    for(LongGhostBeadIterator iterGhost=m_mGhostBeads.begin(); iterGhost!=m_mGhostBeads.end(); iterGhost++)
		    {
		        pGhost = iterGhost->second;
			    CAbstractBead* pBead2 = pGhost->GetBead();				
                m_pPolymer->AddBead(pBead2);
								
				// Set the polymer's head and tail pointers
			    if(pBead2->GetId() == m_HeadId)
				{
				    m_pPolymer->SetHead(pBead2);
				}
			    if(pBead2->GetId() == m_TailId)
				{
				    m_pPolymer->SetTail(pBead2);
				}
		    }
									
			// Now create the concrete bonds needed to connect the beads in the polymer. We copy construct the extended bonds' 
			// concrete instances and set their parameters directly. Note that their new owning polymer will handle destroying
			// the created bonds and bondpairs.
			
			for(ExtendedBondListIterator iterExtBond=m_lExtBonds.begin(); iterExtBond!=m_lExtBonds.end(); iterExtBond++)
			{
			    pExtBond = *iterExtBond;				
				if(pExtBond->GetBond())
				{
				    CBond* const pBond = pExtBond->GetBond();
				    std::cout << "ERROR in AddBeadToGhost - bond " << pBond->GetId() << " should not exist" << zEndl;
				    std::cout << "Checking  bond " << pBond->GetId() << " with head/tail beads " << pBond->GetHead()->GetId() << " " << pBond->GetTail()->GetId() << zEndl;
				    bSuccess = false;
				}
				else
				{
				    CBond* pBond = new CBond(pExtBond->GetType(), pExtBond->GetSpringConstant(), pExtBond->GetUnstretchedLength());
					pBond->SetId(pExtBond->GetId());
					pBond->SetBeads(pExtBond->GetHeadGhost()->GetBead(), pExtBond->GetTailGhost()->GetBead());
					pBond->SetBeadIndex(pExtBond->GetHeadIndex(), pExtBond->GetTailIndex());
										
					pExtBond->SetBond(pBond);
					m_pPolymer->AddBond(pBond);
				}
			}

/*			if(GetId() == 2999)
			{
	            std::cout << "**********" << zEndl;
		        std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " finished assembling concrete polymer instance: " << m_pPolymer->GetId() << " " << m_pPolymer->GetType() << " " << m_pPolymer->GetBeads().size() << " " << m_pPolymer->GetBonds().size() << " " << m_pPolymer->GetBondPairs().size() << " " << m_pPolymer->GetHead()->GetId() << " " << m_pPolymer->GetTail()->GetId() << zEndl; 
			    std::cout << "using " << m_mGhostBeads.size() << " ghost bead instances and " << m_lExtBonds.size() << " extended bond instances; dumping beads " <<  zEndl;
		        ListBeads();
//			    ListConcreteBeads();
	            std::cout << "********************" << zEndl;
            }  */
            // Now the bondpairs - TBD
			
			
		}  
	}

#endif

    return bSuccess;
}

// Function to return the set of processor pids belonging to all ghost beads in this extended polymer. These are used to identify the
// processors that need to receive copies of this extended polymer's concrete bead coordinates so that it can calculate the bonded
// forces on beads in its own Space.
// We ensure that a unique set of pids is returned by only adding new values if they do not already occur in the container.

zLongVector mpuExtendedPolymer::GetGhostBeadPids() const
{
    zLongVector vPids;
	vPids.clear();
	
    for(cLongGhostBeadIterator iterBead=m_mGhostBeads.begin(); iterBead!=m_mGhostBeads.end(); iterBead++)
	{
	    const long pid = iterBead->second->GetPid();
	    if(find(vPids.begin(), vPids.end(), pid) == vPids.end())
		{
		    vPids.push_back(pid);
		}
	}
	
    return vPids;
}

// Function to update the coordinates of a ghost bead with new values. If the update succeeds (ie, the bead is found and is a ghost), 
// we return true, otherwise we return false.

bool mpuExtendedPolymer::UpdateGhostBeadCoords(long id, double x, double y, double z)
{
    bool bNotFound = true;
	
	LongGhostBeadIterator iterBead = m_mGhostBeads.begin();
	
    while(bNotFound && iterBead != m_mGhostBeads.end())
	{
	    if(iterBead->second->GetId() == id)
		{
		   bNotFound = false;
		   (iterBead->second)->SetXPos(x);
		   (iterBead->second)->SetYPos(y);
		   (iterBead->second)->SetZPos(z);
		   
		}
		++iterBead;
	}

	return !bNotFound;
}

// Function to write out the complete information on the extended polymer and its contained polymer, beads, bonds and bondpairs.
// This is used for debug.

void mpuExtendedPolymer::Dump() const
{
    std::cout << "****************************************" << zEndl;
    std::cout << "**** Dumping extended polymer " << GetId() << " data on proc: " << xxParallelBase::GlobalGetRank() << zEndl;
	ListBeads();
	ListConcreteBeads();
    std::cout << "****************************************" << zEndl;

}


