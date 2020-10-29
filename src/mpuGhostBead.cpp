/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpuGhostBead.cpp: implementation of the mpuGhostBead class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mpuGhostBead.h"
#include "AbstractBead.h"
#include "Polymer.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

bool operator<(const mpuGhostBead& a, const mpuGhostBead& b)
{
	return a.GetId() < b.GetId();
}

bool operator==(const mpuGhostBead& a, const mpuGhostBead& b)
{
	return a.GetId() == b.GetId();
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Default constructor

mpuGhostBead::mpuGhostBead() : m_pBead(0), m_Pid(-1), m_id(-1), m_Type(-1), m_PolymerId(-1),
                               m_bIsVisible(true), m_bIsMovable(true), m_bIsFrozen(false), m_Radius(0.0),
                               m_X(0.0), m_Y(0.0), m_Z(0.0)
{
}

#if EnableParallelSimBox == SimMPSEnabled
// Type conversion constructor that wraps an existing bead instance in the ghost bead. This represents those beads 
// in an extended polymer that are still within the owning processor's Space.  We set the bead's pid to an illegal value 
// to indicate that it is not in a neighbouring processor, but we still copy its data into the ghost for later use.

mpuGhostBead::mpuGhostBead(CAbstractBead* const pBead) : m_pBead(pBead), m_Pid(-1), m_id(pBead->GetId()), m_Type(pBead->GetType()), 
                           m_PolymerId(pBead->GetPolymer()->GetId()),
						   m_bIsVisible(pBead->GetVisible()), m_bIsMovable(pBead->GetMovable()), m_bIsFrozen(pBead->GetFrozen()), 
						   m_Radius(pBead->GetRadius()),
                           m_X(pBead->GetXPos()), m_Y(pBead->GetYPos()), m_Z(pBead->GetZPos())
{
//    std::cout << "Ghost bead constructor has parameters " << pBead->GetId() << " " << m_id << " " << m_Type << " " << m_PolymerId << zEndl;
}
#endif

#if EnableParallelSimBox == SimMPSEnabled
// Type conversion constructor that uses a ghost bead to represent a missing concrete bead instance in the owning polymer 
// when the bead leaves the processor's Space. We store the pid of the missing bead's owning processor so that we can get 
// its coordinates later for bonded force calculations. The bead's coordinates are now outside the owning processor's Space, 
// but we store them anyway.
//
// Note that the bead instance passed in will be recycled so we do not store it here, but just copy its data into the ghost.

mpuGhostBead::mpuGhostBead(long newPid, const CAbstractBead* const pBead) : m_pBead(0), m_Pid(newPid), m_id(pBead->GetId()), m_Type(pBead->GetType()), 
                           m_PolymerId(pBead->GetPolymer()->GetId()),
						   m_bIsVisible(pBead->GetVisible()), m_bIsMovable(pBead->GetMovable()), m_bIsFrozen(pBead->GetFrozen()), 
						   m_Radius(pBead->GetRadius()),
                           m_X(pBead->GetXPos()), m_Y(pBead->GetYPos()), m_Z(pBead->GetZPos())
{

}
#endif

// Constructor for a ghost bead that represents a bead belonging to a different processor. It is used when the first bead of a polymer arrives
// in a processor's Space and we have to create empty ghosts to represent all the other beads of the polymer except the one that has just arrived.
// Because we cannot assume that the polymer's other beads belong to the same processor as that the bead just arrived from (imagine a polymer spanning
// several processors at a corner), we can only store the bead and its owning polymer's ids. All subsequent data must be set later.

mpuGhostBead::mpuGhostBead(long beadId, long polyId) : m_pBead(0), m_Pid(-1), m_id(beadId), m_Type(-1), m_PolymerId(polyId),
						   m_bIsVisible(true), m_bIsMovable(true), m_bIsFrozen(false), m_Radius(0.0),
						   m_X(0.0), m_Y(0.0), m_Z(0.0)
{


}

mpuGhostBead::~mpuGhostBead()
{
	// Note that the ghost bead does not own its wrapped instance.
}

// Copy constructor for a mpuGhostBead object. We do a shallow copy, and 
// just pass the wrapped bead pointer to the new instance.

mpuGhostBead::mpuGhostBead(const mpuGhostBead &oldBead)
{
	m_pBead        = oldBead.m_pBead;
    m_Pid          = oldBead.m_Pid;
	m_id           = oldBead.m_id;
	m_Type         = oldBead.m_Type;
	m_PolymerId    = oldBead.m_PolymerId;
	m_bIsVisible   = oldBead.m_bIsVisible;
	m_bIsMovable   = oldBead.m_bIsMovable;
	m_bIsFrozen    = oldBead.m_bIsFrozen;
	m_Radius       = oldBead.m_Radius;
	m_X            = oldBead.GetXPos();
	m_Y            = oldBead.GetYPos();
	m_Z            = oldBead.GetZPos();
}

// ****************************************
// Forwarding functions to the wrapped bead instance.
// All of these functions check that the wrapped instance exists and returns its data if so, if not, they return the ghost's data.

long mpuGhostBead::GetId() const
{
	if(m_pBead)
	{
		return m_pBead->GetId();
	}
	else
	{
		return m_id;
	}
}

long mpuGhostBead::GetType() const
{
	if(m_pBead)
	{
		return m_pBead->GetType();
	}
	else
	{
		return m_Type;
	}
}

// We conditionally compile this function as the GetOwningPolymer() function
// is only compiled in for the parallel code.

long mpuGhostBead::GetPolymerId() const
{
#if EnableParallelSimBox == SimMPSEnabled
	if(m_pBead && m_pBead->GetPolymer())
	{
		return m_pBead->GetPolymer()->GetId();
	}
	else
	{
		return m_PolymerId;
	}
#else
	return -1;
#endif
}


double mpuGhostBead::GetXPos() const
{
	if(m_pBead)
	{
		return m_pBead->GetXPos();
	}
	else
	{
		return m_X;
	}
}

double mpuGhostBead::GetYPos() const
{
	if(m_pBead)
	{
		return m_pBead->GetYPos();
	}
	else
	{
		return m_Y;
	}
}

double mpuGhostBead::GetZPos() const
{
	if(m_pBead)
	{
		return m_pBead->GetZPos();
	}
	else
	{
		return m_Z;
	}
}

double mpuGhostBead::GetUnPBCXPos() const
{
	if(m_pBead)
	{
		return m_pBead->GetunPBCXPos();
	}
	else
	{
		return 0.0;
	}
}

double mpuGhostBead::GetUnPBCYPos() const
{
	if(m_pBead)
	{
		return m_pBead->GetunPBCYPos();
	}
	else
	{
		return 0.0;
	}
}

double mpuGhostBead::GetUnPBCZPos() const
{
	if(m_pBead)
	{
		return m_pBead->GetunPBCZPos();
	}
	else
	{
		return 0.0;
	}
}

// Functions to update the coordinates of the ghost bond with those received from the processor that owns the corresponding 
// concrete bead instance.  Note that these must have been translated into the owning processor's Space before calling this function
// as we make no check on them here. 

void mpuGhostBead::SetXPos(double x)
{
	m_X = x;
}

void mpuGhostBead::SetYPos(double y)
{
	m_Y = y;
}

void mpuGhostBead::SetZPos(double z)
{
	m_Z = z;
}

// Functions to add forces to the wrapped bead instance, either by component or all at once. 

void mpuGhostBead::AddXForce(double fx)
{
#if EnableParallelSimBox == SimMPSEnabled
	if(m_pBead)
	{
	    m_pBead->m_Force[0]+= fx;
	}
#endif
}

void mpuGhostBead::AddYForce(double fy)
{
#if EnableParallelSimBox == SimMPSEnabled
	if(m_pBead)
	{
	    m_pBead->m_Force[1]+= fy;
	}
#endif
}

void mpuGhostBead::AddZForce(double fz)
{
#if EnableParallelSimBox == SimMPSEnabled
	if(m_pBead)
	{
	    m_pBead->m_Force[2]+= fz;
	}
#endif
}

void mpuGhostBead::AddForce(double fx, double fy, double fz)
{
#if EnableParallelSimBox == SimMPSEnabled
	if(m_pBead)
	{
	    m_pBead->m_Force[0]+= fx;
	    m_pBead->m_Force[1]+= fy;
	    m_pBead->m_Force[2]+= fz;
	}
#endif
}

// Function to insert a concrete bead instance into the ghost bead when the former arrives from a neighbouring processor 
// identified by the first argument.
// We set the ghost's data  from the bead data so that if the bead is later removed the ghost still keeps a record. 
// Note that the bead's owning polymer id must be set before calling this function.

void mpuGhostBead::AddBead(long oldPid, CAbstractBead* const pBead)
{
#if EnableParallelSimBox == SimMPSEnabled
    if(!m_pBead)
	{
	m_pBead      = pBead;
	
    m_Pid        = oldPid;
	m_id         = pBead->GetId();
	m_Type       = pBead->GetType();
	m_PolymerId  = pBead->GetPolymer()->GetId();  // Note the calling routine must have already set the bead's owning polymer
	m_bIsVisible = pBead->GetVisible();
	m_bIsMovable = pBead->GetMovable();
	m_bIsFrozen  = pBead->GetFrozen();
	m_Radius     = pBead->GetRadius();
	m_X          = pBead->GetXPos();
	m_Y          = pBead->GetYPos();
	m_Z          = pBead->GetZPos();
	
	}
	else
	{
	    std::cout << "Ghost bead " << GetId() << " already has bead " << m_pBead->GetId() << "when AddBead is called" << zEndl;
	}
#endif
}


// Function to remove the ghost bead's wrapped bead instance when the latter moves to a new processor, and
// to store the pid that now owns the bead.  We update the ghost's dynamic data with the bead's data and 
// then zero the wrapped bead pointer.  We do not destroy the removed bead as it is recycled.

void mpuGhostBead::RemoveBead(long newPid)
{
#if EnableParallelSimBox == SimMPSEnabled
    m_Pid        = newPid;
	m_bIsVisible = m_pBead->GetVisible();
	m_bIsMovable = m_pBead->GetMovable();
	m_bIsFrozen  = m_pBead->GetFrozen();
	m_X          = m_pBead->GetXPos();
	m_Y          = m_pBead->GetYPos();
	m_Z          = m_pBead->GetZPos();
	
	m_pBead      = 0;
#endif
}


