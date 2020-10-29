/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InclusionBead.cpp: implementation of the CInclusionBead class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "InclusionBead.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInclusionBead::CInclusionBead()
{

}

CInclusionBead::~CInclusionBead()
{

}

bool CInclusionBead::SetMovable()
{
	m_bIsMovable = false;

	return m_bIsMovable;
}

bool CInclusionBead::SetNotMovable()
{
	m_bIsMovable = false;

	return m_bIsMovable;
}


bool CInclusionBead::SetFrozen()
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

bool CInclusionBead::SetNotFrozen()
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

