/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpuExtendedNPBond.cpp: implementation of the mpuExtendedNPBond class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimMPSFlags.h"
#include "SimMathFlags.h"
#include "mpuExtendedNPBond.h"
#include "Bead.h"
#include "Bond.h"
#include "xxParallelBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Default constructor

mpuExtendedNPBond::mpuExtendedNPBond() : m_pBond(0), m_pHead(0), m_pTail(0), 
                                     m_id(-1), m_Type(-1), m_HeadIndex(-1), m_TailIndex(-1),
                                     m_SpringConstant(0.0), m_UnStretchedLength(0.0),
				                     m_dx(0.0), m_dy(0.0), m_dz(0.0), m_Length(0.0),
				                     m_fx(0.0), m_fy(0.0), m_fz(0.0)
{
    std::cout << "********** Default constructor for mpuExtendedNPBond called...********** " << zEndl;

}


#if EnableParallelSimBox == SimMPSEnabled

// Type conversion constructor to wrap a concrete CBond instance in an mpuExtendedNPBond instance before either of its beads has left the owning
// processor's space. This is used by the mpuExtendedNanoparticle constructor to wrap an NP's bonds in extended bonds that can have their force 
// calculated even if they cross a processor's boundaries. 

mpuExtendedNPBond::mpuExtendedNPBond(CBond* const pBond) : m_pBond(pBond), 
                                 m_pHead(new mpuGhostBead(pBond->GetHead())), m_pTail(new mpuGhostBead(pBond->GetTail())), 
								 m_id(pBond->GetId()), m_Type(pBond->GetType()), 
								 m_HeadIndex(0), m_TailIndex(0),
								 m_SpringConstant(pBond->GetSprConst()), 
								 m_UnStretchedLength(pBond->GetUnStrLength()),
								 m_dx(0.0), m_dy(0.0), m_dz(0.0), m_Length(0.0),
								 m_fx(0.0), m_fy(0.0), m_fz(0.0) 
{  
}

// Overloaded constructor that represents a CBond but does not yet contain an instance. The passed-in bond is just an exemplar used to obtain the 
// head and tail indices into the polymer's beads.  This is used by the mpuExtendedPolymer constructor that is called when the first bead of a polymer 
// enters a processor's Space. As only a single bead is passed in, we cannot get the ids of the bonds
// that the polymer should contain directly, so we pass in the id of the first bond.

mpuExtendedNPBond::mpuExtendedNPBond(CBond* const pBond, long firstBondId, mpuGhostBead* const pHeadGhost, mpuGhostBead* const pTailGhost) : m_pBond(0), 
                                 m_pHead(pHeadGhost), m_pTail(pTailGhost), 
								 m_id(firstBondId), m_Type(pBond->GetType()), 
								 m_HeadIndex(pBond->GetHeadIndex()), m_TailIndex(pBond->GetTailIndex()),
								 m_SpringConstant(pBond->GetSprConst()), 
								 m_UnStretchedLength(pBond->GetUnStrLength()),
								 m_dx(0.0), m_dy(0.0), m_dz(0.0), m_Length(0.0),
								 m_fx(0.0), m_fy(0.0), m_fz(0.0) 
{
    if( firstBondId == 0)
	{
        std::cout << "Creating empty extended bond with id/type " << firstBondId << " " << pBond->GetType() << " and beads " << pHeadGhost->GetId() << " " << pTailGhost->GetId() << zEndl;
	}
}

#endif

// The bond instance wrapped by this object belongs to the CInitialState or the mpsSimBox objects, so we do not delete it here.

mpuExtendedNPBond::~mpuExtendedNPBond()
{

}

// Function to add the force due to the concrete bond wrapped by this extended bond when both of its concrete bead instances are within the
// owning processor's Space. Because the bead coordinates have been translated into the processor's Space we do not have to apply the PBCs,
// and can just calculate the bond force directly. However, we cannot use the concrete bead instaces in the wrapped bond as they may have
// moved to another processor, instead we use the coordinates of the ghost beads contained in the extended bond.

void mpuExtendedNPBond::AddForce()
{
    if(!IsEitherEmpty())
	{
	    m_dx = m_pHead->GetXPos() - m_pTail->GetXPos();
	    m_dy = m_pHead->GetYPos() - m_pTail->GetYPos();
	    m_dz = m_pHead->GetZPos() - m_pTail->GetZPos();
		
	    m_Length = sqrt(m_dx*m_dx + m_dy*m_dy + m_dz*m_dz);

	    // The overall minus sign is used to swap the order of m_Length - m_UnStrLen

	    m_fx = m_SpringConstant*(m_UnStretchedLength - m_Length)*m_dx/m_Length;
	    m_fy = m_SpringConstant*(m_UnStretchedLength - m_Length)*m_dy/m_Length;
	    m_fz = m_SpringConstant*(m_UnStretchedLength - m_Length)*m_dz/m_Length;

        m_pHead->AddForce(m_fx, m_fy, m_fz);
        m_pTail->AddForce(-m_fx, -m_fy, -m_fz);
	}		
}

// Function to calculate the force due to the concrete bond wrapped by this externded bond when its beads are in the Spaces of adjacent processors.
// It only works if the beads are also in adjacent cells, ie, a Border region of the owning processor and its external CNT cells, and the
// Border regions are those used to calculate non-bonded bead forces, namely, U, T, R, etc. This ensures that the passed-in bead's coordinates
// are always larger than those of the owning processor's beads.
//
// The passed-in data refers to the bead in the neighbouring processor's Space whose force is being accumulated in an external CNT cell.
// We don't yet know if it is the head or tail of the bond, so we have to work this out first.

void mpuExtendedNPBond::AddSharedForce(long id, double x, double y, double z, double* pfx, double* pfy, double* pfz)
{
    if(IsHeadGhostEmpty())
	{
	    // Passed-in bead is the bond's head so it must add the bond force from the accumulation
		
	    m_dx = x - m_pTail->GetXPos();
	    m_dy = y - m_pTail->GetYPos();
	    m_dz = z - m_pTail->GetZPos();
		
	    m_Length = sqrt(m_dx*m_dx + m_dy*m_dy + m_dz*m_dz);

	    // The overall minus sign is used to swap the order of m_Length - m_UnStrLen

	    m_fx = m_SpringConstant*(m_UnStretchedLength - m_Length)*m_dx/m_Length;
	    m_fy = m_SpringConstant*(m_UnStretchedLength - m_Length)*m_dy/m_Length;
	    m_fz = m_SpringConstant*(m_UnStretchedLength - m_Length)*m_dz/m_Length;
		
//		std::cout << "Calculating extendded bond force for " << GetId() << " with head/tail beads " << id << " " << GetHeadGhost()->GetId() << " " << GetTailGhost()->GetId() << " when head bead is missing " << m_dx << " " << m_dy << " " << m_dz << " " << m_Length << " " << m_fx << " "<< m_fy << " " << m_fz << zEndl;
		

		*pfx += m_fx;
		*pfy += m_fy;
		*pfz += m_fz;
        m_pTail->AddForce(-m_fx, -m_fy, -m_fz);
	}
	else
	{
	    // Passed-in bead is the bond's tail so it must subtract the bond force from the accumulation
		
	    m_dx = m_pHead->GetXPos() - x;
	    m_dy = m_pHead->GetYPos() - y;
	    m_dz = m_pHead->GetZPos() - z;
		
	    m_Length = sqrt(m_dx*m_dx + m_dy*m_dy + m_dz*m_dz);

	    // The overall minus sign is used to swap the order of m_Length - m_UnStrLen

	    m_fx = m_SpringConstant*(m_UnStretchedLength - m_Length)*m_dx/m_Length;
	    m_fy = m_SpringConstant*(m_UnStretchedLength - m_Length)*m_dy/m_Length;
	    m_fz = m_SpringConstant*(m_UnStretchedLength - m_Length)*m_dz/m_Length;

//		std::cout << "Calculating extendded bond force for " << GetId() << " with head/tail beads " << id << " " << GetHeadGhost()->GetId() << " " << GetTailGhost()->GetId() << " when tail bead is missing " << m_dx << " " << m_dy << " " << m_dz << " " << m_Length << " " << m_fx << " "<< m_fy << " " << m_fz << zEndl;

        m_pHead->AddForce(m_fx, m_fy, m_fz);
		*pfx -= m_fx;
		*pfy -= m_fy;
		*pfz -= m_fz;
	}	
		
	if(m_Length > 2.0)
	{
		std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " Extended bond " << GetId() << " with 1 empty head/tail ghost bead " << m_pHead->GetId() << " " << m_pTail->GetId() << " has bond length/force " << m_Length << " " << m_fx << " " << m_fy << " " << m_fz << zEndl;	
	}
}


// Function to return the pid of the processor that owns the empty ghost bead if one of them is empty. We check that both are not empty first.

long mpuExtendedNPBond::GetPid() const
{
    long pid = -1;
	
    if(!IsEmpty())
	{
	    if(IsHeadGhostEmpty())
		{
		    pid = m_pHead->GetPid();
		}
		else
		{
		    pid =  m_pTail->GetPid();
		}
	}
	
	return pid;
}

// Forwarding functions to the wrapped CBond instance. If this is null, we return the local data members.

long mpuExtendedNPBond::GetId() const
{
	if(m_pBond)
	{
		return m_pBond->GetId();
	}
	else
	{
		return m_id;
	}
}

long mpuExtendedNPBond::GetType() const
{
	if(m_pBond)
	{
		return m_pBond->GetType();
	}
	else
	{
		return m_Type;
	}
}

long mpuExtendedNPBond::GetHeadIndex() const
{
	if(m_pBond)
	{
		return m_pBond->GetHeadIndex();
	}
	else
	{
		return m_HeadIndex;
	}
}

long mpuExtendedNPBond::GetTailIndex() const
{
	if(m_pBond)
	{
		return m_pBond->GetTailIndex();
	}
	else
	{
		return m_TailIndex;
	}
}

CAbstractBead* mpuExtendedNPBond::GetHead() const
{
	if(m_pBond)
	{
		return m_pBond->GetHead();
	}
	else
	{
		return NULL;
	}
}

CAbstractBead* mpuExtendedNPBond::GetTail() const
{
	if(m_pBond)
	{
		return m_pBond->GetTail();
	}
	else
	{
		return NULL;
	}
}

// Functions to set the pointers to the extended bond's head and ghost beads to null. We leave the bonds themselves, and the data stored in the 
// extended bond class unchanged.

void mpuExtendedNPBond::RemoveHeadGhost()
{
    m_pHead = 0;
}

void mpuExtendedNPBond::RemoveTailGhost()
{
    m_pTail = 0;
}

// Debug function to write out the contents of the extended bond instance.

void mpuExtendedNPBond::Dump() const
{
    std::cout << "****************************************" << zEndl;
    std::cout << "**** Dumping extended bond " << GetId() << " data on proc: " << xxParallelBase::GlobalGetRank() << zEndl;
	std::cout << "Extended bond " << GetId() << " type " << GetType() << " with head ghost (empty?) " << GetHeadGhost()->GetId() << " (" << IsHeadGhostEmpty() << ") and tail ghost " << GetTailGhost()->GetId() << " (" << IsTailGhostEmpty()  << ")" << zEndl;

	if(m_pBond)
	{		
		std::cout << "wraps concrete bond " << GetBond()->GetId() << " with head id (type): " << GetHead()->GetId() << " (" << GetHead()->GetType() << ") and tail id (type): " << GetTail()->GetId() << " (" <<  GetTail()->GetType() << ")" << zEndl; 
	}
	else
	{
		std::cout << "wraps empty bond " << zEndl;
	}
}


