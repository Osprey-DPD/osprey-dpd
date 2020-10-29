/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// WallBead.cpp: implementation of the CWallBead class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "WallBead.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Protected constructor to take a CAbstractBead object and create a 
// CWallBead from it. All this requires is to set the id, type, movable 
// and frozen flags and interaction radius as all other data in the 
// CAbstractBead base are initialised by the CBuilder when assembling the 
// initial state. 
//
// Note that only CInitialState has access to the protected constructor for CWallBead, 
// and that we cannot pass the CAbstractBead as a const reference because the functions
// that return the interaction vectors are not const.

CWallBead::CWallBead(CAbstractBead& oldBead)
{
	m_id			= oldBead.GetId();
	m_Type			= oldBead.GetType();
	m_bIsMovable	= false;			// defining characteristic of a wall bead
	m_bIsFrozen		= true;
	m_Radius		= oldBead.GetRadius();

	for(short int i=0; i<3; i++)
	{
		m_Pos[i]		= 0.0;			// CAbstractBead members
		m_Mom[i]		= 0.0;
		m_Force[i]		= 0.0;
		m_AngMom[i]		= 0.0;
		m_oldPos[i]		= 0.0;
		m_oldMom[i]		= 0.0;
		m_oldForce[i]	= 0.0;
		m_InitialPos[i]	= 0.0;
		m_unPBCPos[i]	= 0.0;
		m_Stress[3*i]	= 0.0;
		m_Stress[3*i+1]	= 0.0;
		m_Stress[3*i+2]	= 0.0;
	}
}

CWallBead::~CWallBead()
{

}

// Function that must be provided by all CAbstractBead-derived classes, but which
// does nothing for wall beads as they are not allowed to move.

bool CWallBead::SetMovable()
{
	return m_bIsMovable;
}

bool CWallBead::SetNotMovable()
{
	return m_bIsMovable;
}

// Function that must be provided by all CAbstractBead-derived classes. Normally,
// this does nothing because wall beads should not move anyway, but in the future
// I may want to allow wall beads to be frozen and unfrozen at will.

bool CWallBead::SetFrozen()
{
	return true;
}

bool CWallBead::SetNotFrozen()
{
	return true;
}
