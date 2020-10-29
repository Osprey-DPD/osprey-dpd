/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Bead.cpp: implementation of the CBead class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "Bead.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

bool operator<(const CBead& a, const CBead& b)
{
	return a.GetId() < b.GetId();
}

bool operator==(const CBead& a, const CBead& b)
{
	return a.GetId() == b.GetId();
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Default constructor

CBead::CBead() : CAbstractBead()
{
}

// General constructor for a bead used in the simulation. Note that the bead is
// movable by default and its interaction radius is set in the CAbstractBead 
// base class to a default value.

CBead::CBead(long id, long type, bool movable, double x0[3], double v0[3]) : CAbstractBead(id, type, movable, x0, v0)
{
}

// New constructor for a bead with an interaction radius that depends on its type.

CBead::CBead(long id, long type, bool movable, double radius, double x0[3], double v0[3]) : CAbstractBead(id, type, movable, radius, x0, v0)
{
}

// Constructor used to create a template bead used for copy-constructing the 
// actual beads used in the simulation.
//
// Note that id defaults to -1 to show that this is a template bead not used in the 
// simulation.

CBead::CBead(long type) : CAbstractBead(type)
{
}

CBead::~CBead()
{
}

// Copy constructor for a CBead object.

CBead::CBead(const CBead &oldBead) : CAbstractBead(oldBead)
{
//	m_id			= oldBead.m_id;
//	m_Type			= oldBead.m_Type;
//	m_bIsMovable	= oldBead.m_bIsMovable;
//	m_bIsFrozen		= oldBead.m_bIsFrozen;
//	m_Radius		= oldBead.m_Radius;

//	for(short int i=0; i<3; i++)
//	{
//		m_Pos[i]		= oldBead.m_Pos[i];			// CAbstractBead members
//		m_Mom[i]		= oldBead.m_Mom[i];
//		m_Force[i]		= oldBead.m_Force[i];
//		m_AngMom[i]		= oldBead.m_AngMom[i];
//		m_oldPos[i]		= oldBead.m_oldPos[i];
//		m_oldMom[i]		= oldBead.m_oldMom[i];
//		m_oldForce[i]	= oldBead.m_oldForce[i];
//		m_InitialPos[i]	= oldBead.m_InitialPos[i];
//		m_unPBCPos[i]	= oldBead.m_unPBCPos[i];
//		m_Stress[3*i]	= oldBead.m_Stress[3*i];
//		m_Stress[3*i+1]	= oldBead.m_Stress[3*i+1];
//		m_Stress[3*i+2]	= oldBead.m_Stress[3*i+2];
//	}
}

// Functions used in the integration loop to indicate if a bead has already had
// its position updated. This prevents CCNTCell::UpdatePos() from moving a bead 
// twice if it crosses a CNT cell boundary and is added to a new cell's bead list.
// They return the current state of the bead's m_bIsMovable flag so that a calling
// routine can alter the state and see the new value with a single function call.
//
// The implementations of these function are particular to each class derived from
// CAbstractBead to allow special bead types, such as CWallBead, to prevent their
// instances from moving at all. Beads that have been frozen are prevented from
// being made movable because the m_bIsFrozen flag overrides the m_bIsMovable flag. 
//

bool CBead::SetMovable()
{
	m_bIsMovable = !m_bIsFrozen;

	return m_bIsMovable;
}

bool CBead::SetNotMovable()
{
	m_bIsMovable = false;

	return m_bIsMovable;
}

// Function used to freeze beads in place. This sets the bead's m_bIsMovable flag 
// to false so that it cannot be reset within the integration loop until a call 
// to SetNotFrozen() to unfreeze the bead.  Also note that the function returns a 
// redundant value to allow it to be used in the mem_fun() algorithm.
//
// To ensure that the bead's internal state is consistent we zero its
// momentum and angular momentum, and their prior values. But the force
// and prior value of force we leave as the beads still contribute to the 
// calculation of forces on movable beads, and we may want to sum the forces
// on frozen beads in the future. Similarly, once a bead has been frozen its
// prior position is set to the current (frozen) position, so that if it is
// unfrozen in the future it effectively starts from rest.
//

bool CBead::SetFrozen()
{
	m_bIsMovable	= false;
	m_bIsFrozen		= true;

	for(short int i=0; i<3; i++)
	{
		m_Mom[i]		= 0.0;
		m_AngMom[i]		= 0.0;
		m_oldPos[i]		= m_Pos[i];
		m_oldMom[i]		= 0.0;
	}

	return true;
}

// Inverse function to SetFrozen() that allows the bead to move again. 
// We zero the bead's state data so that it starts from rest.

bool CBead::SetNotFrozen()
{
	m_bIsMovable	= true;
	m_bIsFrozen		= false;

	for(short int i=0; i<3; i++)
	{
		m_Mom[i]		= 0.0;
		m_AngMom[i]		= 0.0;
		m_oldMom[i]		= 0.0;
	}

	return true;
}

