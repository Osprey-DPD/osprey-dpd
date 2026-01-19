/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Polymer.cpp: implementation of the CPolymer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"
#include "WallBead.h"
#include "CurrentState.h"
#include "ISimBoxBase.h"
#include "mpuExtendedPolymer.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

bool operator<(const CPolymer& a, const CPolymer& b)
{
	return a.GetId() < b.GetId();
}

bool operator==(const CPolymer& a, const CPolymer& b)
{
	return a.GetId() == b.GetId();
}

// Static member variable holding the parameters of the Morse potentials.

// --- Helix “H-bond-like” Morse interactions (Vishnyakov et al., 2012; Doi et al., 2018) ---
// The Morse potential is used as a dissociable bond to mimic hydrogen-bond effects in DPD,
// parameterized by: KM (well depth), alpha (width), re (equilibrium distance), and rM (cutoff).
// Vishnyakov explicitly introduces 1–3 and 1–5 Morse bonds for helix formation, with
// alpha = 8, re(1–3)=0.9 Rc, re(1–5)=0.6 Rc, and cutoff rM = 2.5 Rc. 

// 1–3 Morse: between helix beads separated by two harmonic bonds (i ↔ i+2).
// Role (per Vishnyakov): reduces accessible local angles / torsional freedom so the backbone can
// attain a helical configuration without being rigidly locked in coil/globule states. 
double CPolymer::m_Morse13Depth          = 36.0;   // KM: Morse well depth (paper baseline: K1–3 = 12 kBT)
double CPolymer::m_Morse13Width          = 8.0;    // alpha: Morse width parameter (paper baseline: alpha = 8)
double CPolymer::m_Morse13EqDistance     = 0.9;    // re: equilibrium distance for 1–3 Morse (paper baseline: re = 0.9 Rc)
double CPolymer::m_Morse13CutoffDistance = 10000.0;// rM: cutoff distance beyond which Morse interaction is disabled - we set very large to disable (paper baseline: rM = 2.5 Rc)
double CPolymer::m_Morse13Proximity      = 1.0;    // Optional “activation” distance threshold (not used in the shown AddHelixForces(); if used, it typically gates when the Morse term is evaluated)

// 1–5 Morse: helix-forming “vertical” contact along helix axis (i ↔ i+4).
// Role (per Vishnyakov): models intrachain hydrogen-bonding pattern along the helix axis while
// respecting steric constraints of the coarse-grained chain.
double CPolymer::m_Morse15Depth          = 36.0;   // KM: Morse well depth for 1–5 / i↔i+4 (paper varies K1–5 from 0 to 12 kBT)
double CPolymer::m_Morse15Width          = 8.0;    // alpha: Morse width parameter (paper baseline: alpha = 8)
double CPolymer::m_Morse15EqDistance     = 0.6;    // re: equilibrium distance for 1–5 / i↔i+4 (paper baseline: re = 0.6 Rc)
double CPolymer::m_Morse15CutoffDistance = 10000.0;// rM: cutoff distance (paper baseline: rM = 2.5 Rc; very large ≈ “no cutoff”)
double CPolymer::m_Morse15Proximity      = 1.0;    // Optional “activation” distance threshold (same note as above)

// --- Extra harmonic constraint used in the helix model ---
// In the Doi/Vishnyakov helix setup, the 1–3 interaction includes a harmonic component with
// C = 80 and re = 1.2 (in reduced distance units), in addition to the 1–3 Morse term.
double CPolymer::m_Harmonic13_constant        = 80.0;    // C: harmonic force constant for the 1–3 harmonic constraint (paper baseline: C = 80)
double CPolymer::m_Harmonic13_EqDistance      = 1.2;     // re: equilibrium distance for the 1–3 harmonic constraint (paper baseline: re = 1.2)
double CPolymer::m_Harmonic13_cutoffdistance  = 10000.0; // Optional cutoff for applying the harmonic constraint (not part of the paper’s parameter list; large value ≈ “no cutoff”)



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Default constructor used to make empty polymer instances.

CPolymer::CPolymer() : m_id(0), m_Type(0), m_pHead(0), m_pTail(0),
m_HelixPolymerType(1), m_HelixBeadType(1), m_BeadSep13(2), m_BeadSep15(4)
{
    
#if EnableParallelSimBox == SimMPSEnabled
    m_pExtPolymer = 0;
#endif

    m_vBeads.clear();
    m_vBonds.clear();
	m_vBondPairs.clear();
}


// Constructor that makes a polymer type: this is an example of the actual
// polymers used in the simulation but with id = -1 to show that it is not
// actually used in the simulations but only for creating the polymers that are.
// The vectors of beads and bonds passed in also contain types whose ids are -1,
// to show that they too are not used in the actual simulation.
//
// The polymer just holds its component beads and bonds in vectors. Certain 
// specially designated beads (such as the head and tail) are identified
// for use in measuring observables related to the polymer configuration.
//
// Flags are defined showing whether the polymer is linear or is bound to the wall,
// or whether it contains 3-body forces modelled by CBondPair objects.
// These flags have been disabled while building the parallel code, but may be reinstated
// later if needed.
// A separate constructor is used to initialise a polymer that contains stiff bonds.


CPolymer::CPolymer(long type, bool blinear, double fraction, 
				   CBead* pHead, CBead* pTail, BeadVector& vBeads, 
				   BondVector& vBonds) : m_id(-1),  m_Type(type), 
													m_pHead(pHead),
													m_pTail(pTail),
													m_vBeads(vBeads),
													m_vBonds(vBonds),
                                                    m_HelixPolymerType(1), m_HelixBeadType(1),
                                                    m_BeadSep13(2), m_BeadSep15(4)
{

#if EnableParallelSimBox == SimMPSEnabled
    m_pExtPolymer = 0;
#endif

	m_vBondPairs.clear();
}

CPolymer::CPolymer(long type, bool blinear, double fraction,
				   CBead* pHead, CBead* pTail, BeadVector& vBeads, 
				   BondVector& vBonds, BondPairVector& vBondPairs) : m_id(-1), 
													m_Type(type), 
													m_pHead(pHead),
													m_pTail(pTail),
													m_vBeads(vBeads),
													m_vBonds(vBonds),
													m_vBondPairs(vBondPairs),
                                                    m_HelixPolymerType(1), m_HelixBeadType(1),
                                                    m_BeadSep13(2), m_BeadSep15(4)
{

#if EnableParallelSimBox == SimMPSEnabled
    m_pExtPolymer = 0;
#endif

}

// The CPolymer destructor is responsible for destroying its own CBeads, CBonds and
// CBondPair objects.

CPolymer::~CPolymer()
{
	// Remove the head bead if it is a CWallBead not included in the vector of
	// CBeads that are deleted next.

	if(dynamic_cast<CWallBead*>(m_pHead))
	{
		delete m_pHead;
	}

	if(!m_vBeads.empty())
	{
		for(BeadVectorIterator iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
		{
			delete *iterBead;
		}
		m_vBeads.clear();
	}

	if(!m_vBonds.empty())
	{
		for(BondVectorIterator iterBond=m_vBonds.begin(); iterBond!=m_vBonds.end(); iterBond++)
		{
			delete *iterBond;
		}
		m_vBonds.clear();
	}

	if(!m_vBondPairs.empty())
	{
		for(BondPairVectorIterator iterBP=m_vBondPairs.begin(); iterBP!=m_vBondPairs.end(); iterBP++)
		{
			delete *iterBP;
		}
		m_vBondPairs.clear();
	}
}

// Copy constructor for CPolymer objects. Note that all the beads are assumed
// to be CBead* not CWallBead*: polymers are copied before the head beads are
// changed into wall beads for those polymers attached to the wall.
//
// Because polymers hold vectors of bead pointers we have to define what
// it means to copy a CPolymer type into usable CPolymer objects.
// In the parallel code, the beads composing a polymer are given a pointer to 
// their owning polymer so that they can navigate upwards when passing beads
// between processors.

CPolymer::CPolymer(const CPolymer &oldPolymer): m_HelixPolymerType(oldPolymer.m_HelixPolymerType),
                                                m_HelixBeadType(oldPolymer.m_HelixBeadType),
                                                m_BeadSep13(oldPolymer.m_BeadSep13),
                                                m_BeadSep15(oldPolymer.m_BeadSep15)
{
	m_id				=	oldPolymer.m_id;
	m_Type				=	oldPolymer.m_Type;

	// Now create new beads and bonds to compose this polymer by copying them
	// from the appropriate template class. We cannot set their ids here as 
	// we don't have access to the total number of beads and bonds. That is
	// done in CInitialState::CreatePolymers().
	// We have to set the Head and Tail pointers here when we are copying the
	// appropriate beads and the indices to the beads to which the bond is attached.
	// Note that the CBond copy constructor can copy the bead indices for bonds
	// correctly (as equivalent bonds in polymers of the same type have the same
	// indices) but the actual pointers to beads must be set independently.

	for(cBeadVectorIterator iterBead=oldPolymer.m_vBeads.begin(); iterBead!=oldPolymer.m_vBeads.end(); iterBead++)
	{
		CBead* pBead = new CBead(**iterBead);

#if EnableParallelSimBox == SimMPSEnabled
		pBead->SetOwningPolymer(this);
		
        m_pExtPolymer = 0;  // We don't copy the wrapping extended polymer as I don't understand what that would imply yet

#endif

		if((*iterBead) == oldPolymer.m_pHead)	// when we are creating the head or tail
			m_pHead = pBead;					// beads set the pointers: note we cannot
		if((*iterBead) == oldPolymer.m_pTail)	// use else if in case the head and tail
			m_pTail = pBead;					// are the same bead (e.g., single bead polymers)
		m_vBeads.push_back(pBead);
	}

	for(cBondVectorIterator iterBond=oldPolymer.m_vBonds.begin(); iterBond!=oldPolymer.m_vBonds.end(); iterBond++)
	{
		CBond* pBond = new CBond(**iterBond);
		pBond->SetBeads(m_vBeads.at(pBond->GetHeadIndex()), m_vBeads.at(pBond->GetTailIndex()));
		m_vBonds.push_back(pBond);
	}

	// Copy the CBondPair objects. Again we have to set the bond pointers outside
	// the CBondPair copy constructor, but their indices can be set correctly
	// because they are the same for all polymers of the same type.

	for(cBondPairVectorIterator iterBP=oldPolymer.m_vBondPairs.begin(); iterBP!=oldPolymer.m_vBondPairs.end(); iterBP++)
	{
		CBondPair* pBondPair = new CBondPair(**iterBP);
		pBondPair->SetBonds( m_vBonds.at((*iterBP)->GetFirstIndex()), 
							 m_vBonds.at((*iterBP)->GetSecondIndex()) );
		m_vBondPairs.push_back(pBondPair);
	}

}

// Assignment operator for CPolymer objets.
//
// We don't expect this to be used but it is good to define it anyway.

CPolymer& CPolymer::operator =(const CPolymer &oldPolymer)
{
	if( this != &oldPolymer)
	{

	}
	return *this;
}

// Function to set the ids of the beads that compose the polymer.
// The current value of the bead id counter is passed in and its new
// value is returned, hence we have to use a reference or a pointer.

long CPolymer::SetBeadIds(long startId)
{
	for(BeadVectorIterator iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
	{
		startId++;
		(*iterBead)->SetId(startId);
	}

	return startId;
}

// Function to set the ids of the bonds that compose the polymer. The current
// value of the id counter (i.e., the number of bonds that have been created)
// is passed in and its new value is returned.

long CPolymer::SetBondIds(long startId)
{
	for(BondVectorIterator iterBond=m_vBonds.begin(); iterBond!=m_vBonds.end(); iterBond++)
	{
		startId++;
		(*iterBond)->SetId(startId);
	}

	return startId;
}

// Function to set the ids of the CBondPairs that compose the polymer. The current
// value of the id counter (i.e., the number of bondpairs that have been created)
// is passed in and its new value is returned.

long CPolymer::SetBondPairIds(long startId)
{
	for(BondPairVectorIterator iterBP=m_vBondPairs.begin(); iterBP!=m_vBondPairs.end(); iterBP++)
	{
		startId++;
		(*iterBP)->SetId(startId);
	}

	return startId;
}

// Function to change the head of a polymer to a CWallBead so that the polymer is
// fixed to the wall. We remove the old head from the bead vector in the polymer
// and make the head pointer (and tail if the polymer is a single bead) point to
// the new CWallBead. The id of the new head is taken from the old bead.
//
// Note that we do not add the new head to the m_vBeads as that can only hold 
// CBead objects.

void CPolymer::ChangeHeadtoWallBead()
{
	CWallBead* pNewHead = new CWallBead(*m_pHead);
	pNewHead->SetId(m_pHead->GetId());				// use the old bead id

	// Find all bonds that contain the old head bead and replace it by the new one.
	// Note that if the polymer consists of a single bead there are no bonds, so we
	// don't have to worry about replacing both the head and tail

	for(BondVectorIterator bondIter=m_vBonds.begin(); bondIter!=m_vBonds.end(); bondIter++)
	{
		if((*bondIter)->GetHead()->GetId() == m_pHead->GetId())
			(*bondIter)->SetHeadBead(pNewHead);
		else if ((*bondIter)->GetTail()->GetId() == m_pHead->GetId())
			(*bondIter)->SetTailBead(pNewHead);
	}

	// now remove the old head bead from m_vBeads

	m_vBeads.erase(std::find(m_vBeads.begin(), m_vBeads.end(), m_pHead));

	// The remove() function appears to leave the length of the vector unchanged
	//	remove(m_vBeads.begin(), m_vBeads.end(), m_pHead);	// remove the old head

	// Delete the old head bead and set the new head and tail pointers but don't add
	// the new head bead into m_vBeads because that only holds CBead* and the head bead
	// is now a CWallBead*

	if(m_pTail == m_pHead)	// update the head and tail pointers and delete old head
	{
		delete m_pHead;
		m_pTail = m_pHead = pNewHead;
	}
	else
	{
		delete m_pHead;
		m_pHead = pNewHead;
	}

}

// Function to calculate the total potential energy of a polymer due to its
// contained bonds and bondpairs. It does not calculate the bead-bead interactions
// as this requires the CNTCell structure to get access to the neighbours of
// the polymer's beads. This function is used by the MC relaxation routine
// CSimBox::RunMCRelaxation() to calculate the change in the energy of the system
// as each bead is moved during the MC moves.

double CPolymer::AddPotentialEnergy()
{
	double bondPE = 0.0;

	// Calculate the bond stretching energies first so that the lengths of the
	// bonds are recalculated before the CBondPair function accesses them.

	for(BondVectorIterator iterBond=m_vBonds.begin(); iterBond!=m_vBonds.end(); iterBond++)
	{
		bondPE += (*iterBond)->AddPotentialEnergy();
	}

	for(BondPairVectorIterator iterBP=m_vBondPairs.begin(); iterBP!=m_vBondPairs.end(); iterBP++)
	{
		bondPE += (*iterBP)->AddPotentialEnergy();
	}

	return bondPE;
}

// Functions to modify all beads in the polymer so that they are visible/invisible 
// in current state snapshots. The first of the functions takes an argument 
// showing the visibility status, while the second and third set them all visible
// or invisible respectively. The function to check the visibility status of 
// the polymer is inline.

bool CPolymer::GetVisible() const
{
    return m_vBeads.front()->GetVisible();
}

void CPolymer::SetVisible(bool bVisible)
{
	for(BeadVectorIterator iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
	{
		(*iterBead)->SetVisible(bVisible);
	}
}

void CPolymer::SetVisible()
{
	for(BeadVectorIterator iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
	{
		(*iterBead)->SetVisible();
	}
}

// Function to set all beads in the polymer invisible in current state snapshots.

void CPolymer::SetInvisible()
{
	for(BeadVectorIterator iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
	{
		(*iterBead)->SetInvisible();
	}
}

// Function to return a pointer to the first bead adjacent to the polymer's designated
// head bead. If there are several beads attached to the head, this routine
// just returns the one next to the head in the polymer's shape. We iterate over the 
// polymer's beads until we find the head bead and return the next one.
//
// In rare cases, which require branches or loops, the next bead may not actually
// be bonded to the polymer's head bead. This should be checked in the calling routine.
//
// Note that there is nothing to stop the user specifying a polymer with only two beads,
// in which case this routine returns the polymer's tail. If the polymer has only a 
// single bead (such as the typical solvent W), that bead is returned.

CAbstractBead* CPolymer::GetHeadAdjacentBead() const
{
	if(GetSize() > 2)
	{
		cBeadVectorIterator iterHead;

		iterHead = find(m_vBeads.begin(), m_vBeads.end(), GetHead());

		CAbstractBead* pNext = *(++iterHead);

		return pNext;
	}
	else if(GetSize() == 2)
	{
		return GetTail();
	}
	else		// Single bead in polymer
	{
		return GetHead();
	}
}

// Functions to return the locations of the head and tail beads of the polymer.  This only works if the head and tail beads have been defined
// and are actually present in the polymer's beads. If the head or tail bead are not found, we return the illegal value of -1.
// We cannot use the GetBeadLocation() function as it uses bead ids to search, and exemplar polymers have no ids. So we have to search using the
// head and tail bead addresses.

long CPolymer::GetHeadIndex() const
{
    long index = -1;
	
    if(m_pHead && find(m_vBeads.begin(), m_vBeads.end(), m_pHead) != m_vBeads.end())
	{
	    ++index;
        for(cBeadVectorIterator citerBead=m_vBeads.begin(); citerBead!=m_vBeads.end(); citerBead++)
		{
		    if((*citerBead) == m_pHead)
			{
			    return index;
			}
			else
			{
			    ++index;
			}
		}	
	}
	
	return index;
}
 
long CPolymer::GetTailIndex() const
{
    long index = -1;
	
    if(m_pTail && find(m_vBeads.begin(), m_vBeads.end(), m_pTail) != m_vBeads.end())
	{
	    ++index;
        for(cBeadVectorIterator citerBead=m_vBeads.begin(); citerBead!=m_vBeads.end(); citerBead++)
		{
		    if((*citerBead) == m_pTail)
			{
			    return index;
			}
			else
			{
			    ++index;
			}
		}	
	}
	
	return index;
}
 
 
// Function to return the zero-indexed location of a bead in the polymer. We start counting with the first bead in the polymer's shape
// which is not necessarily its head bead. Note that to speed up the search we assume the bead DOES appear somewhere in the polyner's shape:
// if this assumption fails, the function returns the size of the polymer. If the polymer contains no beads at all, we return an illegal value.
//
// In the parallel codebase, the polymer may be empty or only partially filled. Because bead instances are reused, the specific bead instances that
// are held in the polymer instance may belong to different polymers. Hence, in this case, we retrieve the bead's location from the wrapping extended 
// polymer.

long CPolymer::GetBeadLocation(const CAbstractBead* const pBead) const
{
    long index = -1;
	
//	if(GetId() == 8412)
//	{
//	    std::cout << "Polymer 8412 searching for location of bead " << pBead->GetId() << " (type: " << pBead->GetType() << " add: " << pBead << " ) " << zEndl;
//		long b=0;
//		for(cBeadVectorIterator iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
//		{
//		    std::cout << "Location " << b++ << " occupied by bead  " << (*iterBead)->GetId() << " (type " << (*iterBead)->GetType() << " add: " << (*iterBead) << " ) " << zEndl;
//		}	
//	}
	
#if EnableParallelSimBox == SimMPSEnabled
	if(m_pExtPolymer)
	{
	    index = m_pExtPolymer->GetGhostBeadLocation(pBead->GetId());
	}	
	else
#endif
	{
	    bool bNotFound = true;
	    cBeadVectorIterator citerNext=m_vBeads.begin();
		
	    while(bNotFound && citerNext!=m_vBeads.end())
	    {
		    if(pBead->GetId()  == (*citerNext)->GetId())
		    {
		        bNotFound = false;
		    }
	        ++index;
		    ++citerNext;
	    }
	}
	  
//	if(GetId() == 8412)
//	{
//	    std::cout << "Polymer 8412 returning location of bead " << pBead->GetId() << " as " << index << zEndl;
//    }
	  
	return index;
}

// Function to return the unique numeric id of the first bond in the polymer. This is used when assembling the polymer's beads that have
// passed to a new processor. There are several cases for which we obtain the index:
//
// 1) If the polymer contains bonds, we use the id of the first bond in the polymer
// 2) If the polymer contains no bonds but it is part of an extended polymer, we obtain the index from the wrapper instance
// 3) Otherwise, we return 0.

long CPolymer::GetFirstBondId() const
{
#if EnableParallelSimBox == SimMPSEnabled
    if(!m_vBonds.empty())
	{
	    return m_vBonds.front()->GetId();
	}
	else if(m_pExtPolymer)
	{
	    return m_pExtPolymer->GetFirstExtendedBondId();
	}
	else
#endif
	{
	    return 0;
	}
}

// Function to return the unique numeric id of the first bondpair in the polymer. This is used when assembling the polymer's beads that have
// all passed to a new processor. If there are not bonds,  we return 0 as there is never a stiff bond 0 in the polymers.

long CPolymer::GetFirstBondPairId() const
{
#if EnableParallelSimBox == SimMPSEnabled
    if(!m_vBondPairs.empty())
	{
	    return m_vBondPairs.front()->GetId();
	}
	else if(m_pExtPolymer)
	{
	    return m_pExtPolymer->GetFirstExtendedBondId();
	}
	else
#endif
	{
	    return 0;
	}
}

// Function to set the display id for all beads in the polymer.
// If the desired id is -1, beads are coloured according to their type, otherwise
// they are coloured according to the scheme in the CCurrentStateFormat class.

void CPolymer::SetDisplayId(long id)
{
	if(id == -1)
    {
        for(BeadVectorIterator iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
		{
			CCurrentState::SetBeadDisplayId((*iterBead)->GetId(), (*iterBead)->GetType());
		}
	}
	else if(id >= 0)
	{
	    for(BeadVectorIterator iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
	    {
		    CCurrentState::SetBeadDisplayId((*iterBead)->GetId(), id);
	    }
    }
}

// Function to return the CM of all beads in the polymer. We pass back 
// an aaVector containing the three components of the vector.

aaVector CPolymer::GetCM() const
{
    const double beadTotal  = static_cast<double>(GetSize());

    double xcm = 0.0;
    double ycm = 0.0;
    double zcm = 0.0;

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

    aaVector cm(xcm, ycm, zcm);

    return cm;
}

// Functions to set the polymer's head/tail beads manually: we need these when polymers 
// are extended across processor boundaries. Note that we don't check for the validity of
// the new beads - this must be done by the calling routine. In particular, we don't 
// insist that the beads already be part of the polymer's body as we may be inserting
// ghost beads.

void CPolymer::SetHead(CAbstractBead* const pHead)
{
	m_pHead = pHead;
}

void CPolymer::SetTail(CAbstractBead* const pTail)
{
	m_pTail = pTail;
}

#if EnableParallelSimBox == SimMPSEnabled
// Function used in the parallel code to store a pointer to a polymer instance's wrapping extended polymer.
// This is needed when a polymer spans the Spaces of three or more processors. When a bead first moves from one processor to another
// the bead's owning polymer is used to pass information relating to the polymer's structure  (viz, bead location, first bond id, etc). 
// But when such a bead moves to a different processor, its owning polymer instance is empty (unless all its beads have moved to the 
// same processor, in which case the polymer now lives in a single processor Space), and hence this data is 
// not available.  The pmSendXXXBeads message classes need access to this information, so we make it accessible via the  polymer's 
// wrappper extended polymer that does have access to the polymer's structure on all processors where the polymer lives.

void CPolymer::SetExtendedPolymer(mpuExtendedPolymer* pExtPolymer)
{
	m_pExtPolymer = pExtPolymer;
}
#endif


// Functions to calculate the forces on a polymer's beads due to its bonds.
// This includes both internal bonds that bind the beads into the polymer itself,
// and external bonds that result from dynamically binding polymers together
// in, for example, polymerised command targets. The first override just 
// calculates the forces while the second also sends the bonds to the Monitor 
// so that it can add the bond stresses into the stress profile.

void CPolymer::AddBondForces()
{
	for(BondVectorIterator iterBond=m_vBonds.begin(); iterBond!=m_vBonds.end(); iterBond++)
	{
		(*iterBond)->AddForce();
	}
}

void CPolymer::AddBondForces(ISimBoxBase* const pISimBoxBase)
{
	for(BondVectorIterator iterBond=m_vBonds.begin(); iterBond!=m_vBonds.end(); iterBond++)
	{
		(*iterBond)->AddForce();

		pISimBoxBase->AddBondStress(*iterBond);
	}
}

// Function to calculate the forces on this polymer's beads due to its bondpairs.
// As above, the two overrides calculate the forces alone and send the forces
// to the Monitor to add to the stress profile.

void CPolymer::AddBondPairForces()
{
	for(BondPairVectorIterator iterBP=m_vBondPairs.begin(); iterBP!=m_vBondPairs.end(); iterBP++)
	{
		(*iterBP)->AddForce();
	}
}

// Function to calculate the forces on this polymer's beads due to its bondpairs.

void CPolymer::AddBondPairForces(ISimBoxBase* const pISimBoxBase)
{
	for(BondPairVectorIterator iterBP=m_vBondPairs.begin(); iterBP!=m_vBondPairs.end(); iterBP++)
	{
		(*iterBP)->AddForce();

		pISimBoxBase->AddBondPairStress(*iterBP);
	}
}

// Function to add helix-forming forces to the S bead type in the Helix polymers.
// They are identified by the unique numeric identifier, not the name.
// All parameters are hard-wired in this class.
// Note: This is quite inefficient, but we can store the Helix polymers locally later to speed it up.

void CPolymer::AddHelixForces()
{
    if( GetType() == m_HelixPolymerType )  // Not really needed as CSimBox already selected Helix polymers.
    {
        // iterate over all beads and copy the helix-forming beads beads into a  local vector.
        // As this doesn't change, we could to it in the constructor to be more efficient.
        
        BeadVector vHelixBeads;
        vHelixBeads.clear();
        for(BeadVectorIterator iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
        {
            if( (*iterBead)->GetType() == m_HelixBeadType )
            {
                vHelixBeads.push_back(*iterBead);
                
                //std::cout << "Polymer/bead ids " << GetId() << " " << (*iterBead)->GetId() << zEndl;
            }
        }

        // Apply the proximity test to the S beads to form the helix, and then the force.
        // Note that as we iterate over each bead, we know that we must look ahead precisely n beads, so we don't
        // need a double loop. But we do need to check that the beads that would be connected are S beads.
        //
        //  m_BeadSep13 and m_BeadSep15 are the separation of the S beads that are to be tied together.
        //
        
        if( !vHelixBeads.empty() )
        {
			int i=1;
            for(BeadVectorIterator iterBead=vHelixBeads.begin(); iterBead!=vHelixBeads.end(); iterBead++)
            {
                CBead* pBead1 = *iterBead; 

				// === 1–3 INTERACTION ===
				BeadVectorIterator iterBead13 = iterBead;

				if( i<vHelixBeads.size()-1) //problem with this condition ! 58 le last
                {

                    std::advance(iterBead13, m_BeadSep13);

                    if( iterBead13 != vHelixBeads.end() )// not necessary 
                    {

                        CBead* pBead2 = *iterBead13;

                        // Both must be S beads by construction of vHelixBeads

                        double dx = pBead1->GetunPBCXPos() - pBead2->GetunPBCXPos(); //Periodic Boundary Conditions are considered here!!
                        double dy = pBead1->GetunPBCYPos() - pBead2->GetunPBCYPos();
                        double dz = pBead1->GetunPBCZPos() - pBead2->GetunPBCZPos();

                        double r2 = dx*dx + dy*dy + dz*dz;

						// just in case of numerical round-off
                        if(r2 > 0.0)
                        {
                            double r = sqrt(r2);
							double invr = 1.0 / r;
						// === 1-3 HARMONIC INTERACTIONS ===

							// 𝐅𝑖𝑗 = 𝐶(𝑟 𝑒− 𝑟𝑖𝑗)𝐧𝑖𝑗 
							double forceMag_harmonic=m_Harmonic13_constant*(m_Harmonic13_EqDistance-r);
							double fx=forceMag_harmonic*dx*invr;
							double fy=forceMag_harmonic*dy*invr;
							double fz=forceMag_harmonic*dz*invr;


						// === 1-3 MORSE INTERACTIONS ===
                            // Proximity / cutoff test
                            if( r < m_Morse13CutoffDistance)
                            {
								// Doi et al. describe/implement a modified Morse-force form (with |r - re| in the exponent) 
                                double dr = fabs(r - m_Morse13EqDistance);
                                double e = exp(-m_Morse13Width * dr);

                                // |F| = 2 * α * K_M e^{-α(r-r_e)} ( e^{-α(r-r_e)} - 1 ) ## il faut jouer avec le alpha ici
                                double forceMag = 2.0 * m_Morse13Width * m_Morse13Depth * e * (e - 1.0); // α multiplied here because of derivative

								// Fx,ij = F_mag (r) * dx / r_ij etc
                                fx += forceMag * dx * invr;
                                fy += forceMag * dy * invr;
                                fz += forceMag * dz * invr;

                            }
						// Apply equal and opposite forces to the bead pair
                        	pBead1->AddHelixForces( fx,  fy,  fz );
                            pBead2->AddHelixForces( -fx, -fy, -fz );
                        }
                    }
                }

                // === 1–5 MORSE INTERACTION ===
				BeadVectorIterator iterBead15 = iterBead;

				if(i<vHelixBeads.size()-3) //problem with this condition !
                {

                    std::advance(iterBead15, m_BeadSep15);

                    if( iterBead15 != vHelixBeads.end() ) // not necessary 
                    {

                        CBead* pBead2 = *iterBead15;

                        // Both must be S beads by construction of vHelixBeads

                        double dx = pBead1->GetunPBCXPos() - pBead2->GetunPBCXPos(); //Periodic Boundary Conditions are considered here!!
                        double dy = pBead1->GetunPBCYPos() - pBead2->GetunPBCYPos();
                        double dz = pBead1->GetunPBCZPos() - pBead2->GetunPBCZPos();

                        double r2 = dx*dx + dy*dy + dz*dz;

						// just in case of numerical round-off..
                        if(r2 > 0.0)
                        {
                            double r = sqrt(r2);

                            // Proximity / cutoff test
                            if( r < m_Morse15CutoffDistance)
                            {
                                // Doi et al. describe/implement a modified Morse-force form (with |r - re| in the exponent) 
                                double dr = fabs(r - m_Morse15EqDistance);
                                double e = exp(-m_Morse15Width * dr);

                                // |F| = 2 * α * K_M e^{-a(r-r_e)} ( e^{-a(r-r_e)} - 1 )
                                double forceMag = 2.0 * m_Morse15Width * m_Morse15Depth * e * (e - 1.0); // α multiplied here because of derivative

								// Fx,ij = F_mag (r) * dx / r_ij etc
                                double invr = 1.0 / r;
                                double fx = forceMag * dx * invr;
                                double fy = forceMag * dy * invr;
                                double fz = forceMag * dz * invr;

                                // Apply equal and opposite forces to the bead pair
                                pBead1->AddHelixForces( fx,  fy,  fz );
                                pBead2->AddHelixForces( -fx, -fy, -fz );
                            }
                        }
                    }
                }
				i+=1;
            }
        }
    }
}




// Function to empty all the bead, bond and bondpair containers in the polymer, and reset its id and type, before it is added
// to the empty polymers containers.  This is needed so that the polymer's beads, etc, are released before being owned by one 
// of the empty bead containers.

void CPolymer::Clear()
{
#if EnableParallelSimBox == SimMPSEnabled
//	m_pExtPolymer = 0;
#endif
    m_id    = -1;
	m_Type  = -1;
	m_pHead = 0;
	m_pTail = 0;
	m_vBeads.clear();
	m_vBonds.clear();
	m_vBondPairs.clear();  
}

// Function to write out the polymer's complete member variables for debug purposes. This is used to check the addresses of the beads, bonds and bondpairs
// and to ensure that the same bead, bond or bondpair instance does not occur in more than one polymer.

void CPolymer::Dump() const
{
    std::cout << "Polymer " << GetId() << " (type " << GetType() << ") has " << m_vBeads.size() << " beads, " << m_vBonds.size() << " bonds, " << m_vBondPairs.size() << " bondpairs" << zEndl;
    if(m_pHead && m_pTail)
	{
        std::cout << "Head/tail beads are:     " << m_pHead          << " " << m_pTail << zEndl;
        std::cout << "Head/tail bead ids are:  " << m_pHead->GetId() << " " << m_pTail->GetId() << zEndl;
		std::cout << "Head/tail indices are:   " << GetHeadIndex()   << " " << GetTailIndex() << zEndl;
	}
	else
	{
	    if(m_pHead)
		{
            std::cout << "Head bead is: " << m_pHead->GetId() << " but tail bead is empty: address is " << m_pTail << zEndl;
		    std::cout << "Head/tail indices are: " << GetHeadIndex() << " " << GetTailIndex() << zEndl;
		}
		else if(m_pTail)
		{
            std::cout << "Tail bead is: " << m_pTail->GetId() << " but head bead is empty: address is " << m_pHead << zEndl;
		    std::cout << "Head/tail indices are: " << GetHeadIndex() << " " << GetTailIndex() << zEndl;
		}
		else
		{
		    std::cout << "Both head and tail pointers are empty: addresses are: " << m_pHead << " " << m_pTail << zEndl;
		    std::cout << "Head/tail indices are: " << GetHeadIndex() << " " << GetTailIndex() << zEndl;
		}
	}
	
		std::cout << "Beads: id (type, address): ";
        for(cBeadVectorIterator cib1=m_vBeads.begin(); cib1!=m_vBeads.end(); cib1++)
	    {
	        std::cout << (*cib1)->GetId() << " (" << (*cib1)->GetType() << ",  " << (*cib1) << "):   ";
	    }
        std::cout << zEndl;
		
	    std::cout << "Bonds: id (type, address): head index, head id, head add, tail index, tail id, tail add"<< zEndl;
	    for(cBondVectorIterator cib2=m_vBonds.begin(); cib2!= m_vBonds.end(); cib2++)
	    {
	        std::cout << (*cib2)->GetId() << " (" << (*cib2)->GetType() << ", " <<  (*cib2) << "):  " << (*cib2)->GetHeadIndex() << "  " << (*cib2)->GetHead()->GetId() << " " <<  (*cib2)->GetHead() << "  " << (*cib2)->GetTailIndex() << " " << (*cib2)->GetTail()->GetId() << " " << (*cib2)->GetTail() << zEndl;
	    }
	    std::cout << "Bondpairs: id (type, address): " << zEndl;
	    for(cBondPairVectorIterator cib3=m_vBondPairs.begin(); cib3!= m_vBondPairs.end(); cib3++)
	    {
	        std::cout << (*cib3)->GetId() << " (" << (*cib3)->GetType() << ", " << (*cib3) << "): "  << zEndl;
	    }

}

// Function to add the lone bead to a single-bead polymer. Note that the polymer must have had its Clear() function called
// before calling this function to ensure that its containers are empty.   Also note that 
// we have to cast the CAbstractBead pointer to the expected CBead type before adding it.

bool CPolymer::AddSingleBead(CAbstractBead* const pBead)
{
#if EnableParallelSimBox == SimMPSEnabled
    if(pBead)
	{
	    pBead->SetOwningPolymer(this);
	    m_vBeads.push_back(dynamic_cast<CBead*>(pBead));
	    m_pHead = pBead;
	    m_pTail = pBead;
		return true;
	}
	else
	{
	    return false;
	}
#else
	return false;
#endif
}

// Functions to add a bead, bond or bondpair to a multi-bead polymer. Note that the polymer must have had its Clear() function called
// prior to using these functions otherwise it will just append beads to the polymer's existing beads.

bool CPolymer::AddBead(CAbstractBead* const pBead)
{
    m_vBeads.push_back(dynamic_cast<CBead*>(pBead));
	return true;
}

bool CPolymer::AddBond(CBond* const pBond)
{
	m_vBonds.push_back(pBond);
	return true;
}

bool CPolymer::AddBondPair(CBondPair* const pBondPair)
{
    m_vBondPairs.push_back(pBondPair);
    return false;
}

// Function to vaidate the polymer's data when it moves to a new processor and is added to the bulk polymer's map.
// The following conditions must be satisfied:
//
// Id  > 0
// Type = 0
// Head Bead exists and is one of the polymer's beads
// Tail Bead exists and is one of the polymer's beads
// Owning polymer is null as the polymer is now wholly contained in one processor's Space
// Polymer's beads are present and have this polymer as their owning polymer
// Polymer's bonds are present and contain the correct beads
// Polymer's bondpairs are present - TBD

bool CPolymer::Validate()
{
    bool bValid = true;

#if EnableParallelSimBox == SimMPSEnabled
    if(GetId() < 1)
	{   
	    bValid = false;
	    std::cout << "Polymer has invalid id " << m_id << zEndl;
	}
	else if(GetType() < 0)
	{
	    bValid = false;
	    std::cout << "Polymer " << GetId() << " has invalid type " << m_Type << zEndl;
	
	}
	else if(!m_pHead || !m_pTail)
	{
		bValid = false;
	    std::cout << "Polymer " << GetId() << " (type " << GetType() << ") has no head " << m_pHead << " or tail " << m_pTail << zEndl;
	}
	else if(m_pExtPolymer)
	{
		bValid = false;
	    std::cout << "Polymer " << GetId() << " (type " << GetType() << ") has invalid wrapper pointer " << m_pExtPolymer->GetId() <<  zEndl;
	}
	else if(m_vBeads.empty())
	{
	    bValid = false;
	    std::cout << "Polymer " << GetId() << " (type " << GetType() << ") has no beads " <<  zEndl;
	}
	else if(m_vBonds.empty())
	{
	    bValid = false;
	    std::cout << "Polymer " << GetId() << " (type " << GetType() << ") has no bonds " <<  zEndl;
	}
	
	// Now check that the beads really belong to this polymer
	
	BeadVectorIterator iterBead;
	
	bool bHeadFound = false;
	bool bTailFound = false;
	
	long counter = 0;
	for(iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
	{
	    CBead* pBead = *iterBead;
		
		if(pBead->GetId() == m_pHead->GetId())
		{
		    bHeadFound = true;
		}
		if(pBead->GetId() == m_pTail->GetId())
		{
		    bTailFound = true;
		}
		
        if(!(pBead->GetPolymer()))
		{
			bValid = false;
			std::cout << "Polymer " << GetId() << " (type " << GetType() << ") bead " << counter << " " <<  pBead->GetId() << " has empty owning polymer " << pBead->GetPolymer() << zEndl;
		}
		else if (pBead->GetPolymer() != this)
		{
	        bValid = false;
	        std::cout << "Polymer " << GetId() << " (type " << GetType() << ") bead " << counter << " " <<  pBead->GetId() << " has wrong owning polymer " << pBead->GetPolymer()->GetId() << zEndl;
		}	
		++counter;
	}

    if(!bHeadFound)
	{
		bValid = false;
		std::cout << "Polymer " << GetId() << " (type " << GetType() << ") head bead " << m_pHead->GetId() <<  " is not in the polymer: candidates are:" << zEndl;
	    for(iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
	    {
		    std::cout << (*iterBead)->GetId() << " (type " << (*iterBead)->GetType() << ")  ";
		}
		std::cout << zEndl;
	}

    if(!bTailFound)
	{
		bValid = false;
		std::cout << "Polymer " << GetId() << " (type " << GetType() << ") tail bead " << m_pTail->GetId() <<  " is not in the polymer: candidates are:" << zEndl;
	    for(iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
	    {
		    std::cout << (*iterBead)->GetId() << " (type " << (*iterBead)->GetType() << ")  ";
		}
		std::cout << zEndl;
	}
	
	// Now check the bonds contain only beads that are in this polymer.
	
	for(BondVectorIterator iterBond=m_vBonds.begin(); iterBond!=m_vBonds.end(); iterBond++)
	{
	    bHeadFound = false;
	    bTailFound = false;
	    CBond* pBond = *iterBond;
		CAbstractBead* pHead = pBond->GetHead();
		CAbstractBead* pTail = pBond->GetTail();
		
     	for(iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
	    {
		    if((*iterBead)->GetId() == pHead->GetId())
		    {
		        bHeadFound = true;
		    }
		    if((*iterBead)->GetId() == pTail->GetId())
		    {
		        bTailFound = true;
		    }
		}
		
        if(!bHeadFound)
	    {
		    bValid = false;
		    std::cout << "Polymer " << GetId() << " (type " << GetType() << ") bond " << pBond->GetId() << " head bead " << pHead->GetId() <<  " is not in the polymer" << zEndl;
		    std::cout << zEndl;
	    }
        if(!bTailFound)
	    {
		    bValid = false;
		    std::cout << "Polymer " << GetId() << " (type " << GetType() << ") bond " << pBond->GetId() << " tail bead " << pTail->GetId() <<  " is not in the polymer" << zEndl;
		    std::cout << zEndl;
	    }
		
		if(!bHeadFound || !bTailFound)
		{
		    std::cout << "Missing bond must have one of the following beads" << zEndl;
	        for(iterBead=m_vBeads.begin(); iterBead!=m_vBeads.end(); iterBead++)
	        {
		        std::cout << (*iterBead)->GetId() << " (type " << (*iterBead)->GetType() << ")  ";
		    }
			std::cout << zEndl;
		}
	}
		
#endif	

    return bValid;
}



