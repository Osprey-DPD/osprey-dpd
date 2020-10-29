/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Bond.cpp: implementation of the CBond class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimACNFlags.h"
#include "SimMathFlags.h"
#include "SimMPSFlags.h"
#include "Bead.h"
#include "Bond.h"

#if EnableShadowSimBox == SimACNEnabled
#include "aeActiveSimBox.h" // Needed for access to SimBox side lengths
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBond::CBond() : m_id(0), m_Type(0), m_SprConst(0.0), m_UnStrLen(0.0),
				 m_HeadIndex(0), m_TailIndex(0), m_pHead(0), m_pTail(0),
				 m_dx(0.0), m_dy(0.0), m_dz(0.0), m_Length(0.0),
				 m_fx(0.0), m_fy(0.0), m_fz(0.0)
{
}

// Constructor used to set up template CBonds for use in CInputData. Note that 
// their id is set to -1 to show that they are not used in the actual simulation.
// Also note that we cannot set the indices or pointers to the beads at each end
// of the bond yet.

CBond::CBond( long type, double spring, double length0 ) :  m_id(-1), m_Type(type),
															m_SprConst(spring),
															m_UnStrLen(length0),
															m_HeadIndex(0), m_TailIndex(0),
															m_pHead(0), m_pTail(0),
															m_dx(0.0), m_dy(0.0), m_dz(0.0), 
															m_Length(0.0),
															m_fx(0.0), m_fy(0.0), m_fz(0.0) 
{
}

// Copy constructor for CBond objects. 
//
// Note that we cannot set the pointers to the actual beads at the bond's ends
// because they will be different for every bond. But the indices into the 
// polymer bead array will be the same for the equivalent bonds in polymers of the 
// same type.

CBond::CBond(const CBond &oldBond)
{
	m_id		= oldBond.m_id;
	m_Type		= oldBond.m_Type;
	m_SprConst	= oldBond.m_SprConst;
	m_UnStrLen	= oldBond.m_UnStrLen;
	m_HeadIndex	= oldBond.m_HeadIndex;
	m_TailIndex	= oldBond.m_TailIndex;
	m_pHead		= 0;
	m_pTail		= 0;
	m_dx		= oldBond.m_dx;
	m_dy		= oldBond.m_dy;
	m_dz		= oldBond.m_dz;
	m_Length	= oldBond.m_Length;
	m_fx		= oldBond.m_fx;
	m_fy		= oldBond.m_fy;
	m_fz		= oldBond.m_fz;
}

CBond::~CBond()
{

}

// Function to calculate the force exerted by a bond on the two beads at
// its ends. This routine adds its force to a running total which must 
// have been zeroed prior to its being called.
//
// The force law is Hookean:	F(length) = -m_SprConst*(m_Length - m_UnStrLen)
//
// and because we calculate force not energy we don't have to worry about
// a factor of 1/2 multiplying the spring constant. Note that the force on 
// the head is directed towards the tail (if the spring length is greater 
// than its unstretched value) and that the total force is zero. 
//
// We calculate the bond length using bead coordinates that ignore the
// pbcs. These coordinates are updated in UpdatePos() as for the usual pbc 
// coordinates but are not wrapped around when a bead crosses the pbcs. Instead
// they represent the absolute position of the bead with respect to the origin.
// In these coordinates, the bond length is just the difference of the head and 
// tail bead positions.
//
// I added the contribution of the bond to the stress tensor's diagonal components.
// I stored the bond vector components and length in member variables to allow the
// CBondPair objects access to the bond lengths without having to calculate them
// again.

void CBond::AddForce()
{
#if EnableParallelSimBox == SimMPSEnabled
    // Parallel code uses the bead coordinates in the principal image of the SimBox as the beads that move between processors have their coordinates udpated
	// automatically, so we don't need to check for PBCs.
	
	m_dx = m_pHead->GetXPos() - m_pTail->GetXPos();
	m_dy = m_pHead->GetYPos() - m_pTail->GetYPos();
	m_dz = m_pHead->GetZPos() - m_pTail->GetZPos();

//    std::cout << "Bond force for bond id " << GetId() << " with length " << m_Length << " and components " << m_dx << " " << m_dy << " " << m_dz << zEndl;
#else
    // Serial code uses the unPBC coordinates to ensure bonds that span the SimBox boundaries have the correct length
	
	m_dx = m_pHead->GetunPBCXPos() - m_pTail->GetunPBCXPos();
	m_dy = m_pHead->GetunPBCYPos() - m_pTail->GetunPBCYPos();
	m_dz = m_pHead->GetunPBCZPos() - m_pTail->GetunPBCZPos();

#endif

	m_Length = sqrt(m_dx*m_dx + m_dy*m_dy + m_dz*m_dz);
	
	// The overall minus sign is used to swap the order of m_Length - m_UnStrLen

	m_fx = m_SprConst*(m_UnStrLen - m_Length)*m_dx/m_Length;
	m_fy = m_SprConst*(m_UnStrLen - m_Length)*m_dy/m_Length;
	m_fz = m_SprConst*(m_UnStrLen - m_Length)*m_dz/m_Length;

	m_pHead->m_Force[0]+= m_fx;
	m_pHead->m_Force[1]+= m_fy;
	m_pHead->m_Force[2]+= m_fz;

	m_pTail->m_Force[0]-= m_fx;
	m_pTail->m_Force[1]-= m_fy;
	m_pTail->m_Force[2]-= m_fz;

	// Stress tensor contributions from bond force. 
	// Removed the stress from the tail bead to avoid double counting its 
	// contribution to the pressure. This does not affect the stress profile
	// calculation in CMonitor.
	// Changed the sign of the stress after checking it in notes, p42ff.

	m_pHead->m_Stress[0] += m_dx*m_fx;
	m_pHead->m_Stress[1] += m_dy*m_fx;
	m_pHead->m_Stress[2] += m_dz*m_fx;
	m_pHead->m_Stress[3] += m_dx*m_fy;
	m_pHead->m_Stress[4] += m_dy*m_fy;
	m_pHead->m_Stress[5] += m_dz*m_fy;
	m_pHead->m_Stress[6] += m_dx*m_fz;
	m_pHead->m_Stress[7] += m_dy*m_fz;
	m_pHead->m_Stress[8] += m_dz*m_fz;
}

// Function to calculate the bond force using bead coordinates that are 
// restricted to the primary simulation box image. If any component of the 
// bond length is greater than one half of the simulation box size in 
// that dimension, it is shifted to within the interval (0, L) 
// where L = Lx, Ly or Lz.
//
// Note that we need access to the simulation box side lengths, and we get these
// from the aeActiveSimBox's global member functions. If the active SimBox has
// not been created, this will fail so we add a check for its existence. No force
// is generated if the active SimBox does not exist.

void CBond::AddPBCForce()
{
#if EnableShadowSimBox == SimACNEnabled

	if(aeActiveSimBox::GetIActiveSimBox())
	{
		m_dx = m_pHead->GetXPos() - m_pTail->GetXPos();
		m_dy = m_pHead->GetYPos() - m_pTail->GetYPos();
		m_dz = m_pHead->GetZPos() - m_pTail->GetZPos();

		if( m_dx > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxXLength() )
			m_dx = m_dx - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxXLength();
		else if( m_dx < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxXLength() )
			m_dx = m_dx + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxXLength();

		if( m_dy > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxYLength() )
			m_dy = m_dy - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxYLength();
		else if( m_dy < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxYLength() )
			m_dy = m_dy + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxYLength();

	#if SimDimension == 3
		if( m_dz > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxZLength() )
			m_dz = m_dz - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxZLength();
		else if( m_dz < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxZLength() )
			m_dz = m_dz + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxZLength();
	#else
		m_dz = 0.0;
	#endif

		m_Length = sqrt(m_dx*m_dx + m_dy*m_dy + m_dz*m_dz);
		
		// The overall minus sign is used to swap the order of m_Length - m_UnStrLen

		m_fx = m_SprConst*(m_UnStrLen - m_Length)*m_dx/m_Length;
		m_fy = m_SprConst*(m_UnStrLen - m_Length)*m_dy/m_Length;
		m_fz = m_SprConst*(m_UnStrLen - m_Length)*m_dz/m_Length;

		m_pHead->m_Force[0]+= m_fx;
		m_pHead->m_Force[1]+= m_fy;
		m_pHead->m_Force[2]+= m_fz;

		m_pTail->m_Force[0]-= m_fx;
		m_pTail->m_Force[1]-= m_fy;
		m_pTail->m_Force[2]-= m_fz;

		// Stress tensor contributions from bond force. 
		// Removed the stress from the tail bead to avoid double counting its 
		// contribution to the pressure. This does not affect the stress profile
		// calculation in CMonitor.
		// Changed the sign of the stress after checking it in notes, p42ff.

		m_pHead->m_Stress[0] += m_dx*m_fx;
		m_pHead->m_Stress[1] += m_dy*m_fx;
		m_pHead->m_Stress[2] += m_dz*m_fx;
		m_pHead->m_Stress[3] += m_dx*m_fy;
		m_pHead->m_Stress[4] += m_dy*m_fy;
		m_pHead->m_Stress[5] += m_dz*m_fy;
		m_pHead->m_Stress[6] += m_dx*m_fz;
		m_pHead->m_Stress[7] += m_dy*m_fz;
		m_pHead->m_Stress[8] += m_dz*m_fz;
	}
#endif

}

// Function to add pointers to the beads at each end of the bond.

void CBond::SetBeads(CAbstractBead* pHead, CAbstractBead* pTail)
{
	m_pHead = pHead;
	m_pTail = pTail;
}

// Function to add a pointer to the bead at the head of the bond.

void CBond::SetHeadBead(CAbstractBead* pHead)
{
	m_pHead = pHead;
}

// Function to add a pointer to the beads at the tail of the bond.

void CBond::SetTailBead(CAbstractBead* pTail)
{
	m_pTail = pTail;
}

// Function to add integer indices into the bead array in a polymer showing
// which beads the bond has at its ends. This is used when copying bonds
// from the templates into actual bonds used in the simulation.

void CBond::SetBeadIndex(long lHead, long lTail)
{
	m_HeadIndex = lHead;
	m_TailIndex = lTail;
}

// Function to return the potential energy of the bond. 
//
// We cannot assume that the AddForce() function has been called so we have to
// calculate the bond length again. But we do assume that the spring constant 
// and unstretched length have been appropriate normalized by the length-scale 
// used in the simulation. This should have been done in CInputData::NormalizeMD().

double CBond::AddPotentialEnergy()
{
	m_dx = m_pHead->GetunPBCXPos() - m_pTail->GetunPBCXPos();
	m_dy = m_pHead->GetunPBCYPos() - m_pTail->GetunPBCYPos();
	m_dz = m_pHead->GetunPBCZPos() - m_pTail->GetunPBCZPos();

	m_Length = sqrt(m_dx*m_dx + m_dy*m_dy + m_dz*m_dz);

	return 0.5*m_SprConst*(m_Length - m_UnStrLen)*(m_Length - m_UnStrLen);
}

// Function to calculate a bond's length using the PBC coordinates
// of its head and tail beads without changing the state of the bond.

double CBond::CalculateLength() const
{
	double dx, dy, dz;

	dx = m_pHead->GetXPos() - m_pTail->GetXPos();
	dy = m_pHead->GetYPos() - m_pTail->GetYPos();
	dz = m_pHead->GetZPos() - m_pTail->GetZPos();

	return sqrt(dx*dx + dy*dy + dz*dz);
}

// Function to calculate the length of the bond using the PBC coordinates and
// restricting each component of the bond vector to be less than one half
// of the simulation box side lengths.

double CBond::CalculatePBCLength() const
{
#if EnableShadowSimBox == SimACNEnabled

	double dx, dy, dz;

	dx = 0.0;
	dy = 0.0;
	dz = 0.0;

	if(aeActiveSimBox::GetIActiveSimBox())
	{
		dx = m_pHead->GetXPos() - m_pTail->GetXPos();
		dy = m_pHead->GetYPos() - m_pTail->GetYPos();
		dz = m_pHead->GetZPos() - m_pTail->GetZPos();

		if( dx > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxXLength() )
			dx = dx - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxXLength();
		else if( dx < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxXLength() )
			dx = dx + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxXLength();

		if( dy > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxYLength() )
			dy = dy - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxYLength();
		else if( dy < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxYLength() )
			dy = dy + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxYLength();

	#if SimDimension == 3
		if( dz > aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxZLength() )
			dz = dz - aeActiveSimBox::GetIActiveSimBox()->GetSimBoxZLength();
		else if( dz < -aeActiveSimBox::GetIActiveSimBox()->GetHalfSimBoxZLength() )
			dz = dz + aeActiveSimBox::GetIActiveSimBox()->GetSimBoxZLength();
	#else
		dz = 0.0;
	#endif
	}

	return sqrt(dx*dx + dy*dy + dz*dz);
#else
    return 0.0;
#endif
}

// Function to calculate a bond's length using the non-periodic
// coordinates. 

double CBond::CalculateunPBCLength() const
{
	double dx, dy, dz;

	dx = m_pHead->GetunPBCXPos() - m_pTail->GetunPBCXPos();
	dy = m_pHead->GetunPBCYPos() - m_pTail->GetunPBCYPos();
	dz = m_pHead->GetunPBCZPos() - m_pTail->GetunPBCZPos();

	return sqrt(dx*dx + dy*dy + dz*dz);
}

