/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// BeadType.cpp: implementation of the CBeadType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "BeadType.h"
#include "BDBeadStructure.h"
#include "DPDBeadStructure.h"
#include "MDBeadStructure.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////
// Boolean function to test two bead types for equality. This is
// only used by CInputData to compare the bead types being read from 
// a restart control data file. It only compares the bead names
// because this is only only attribute that must not be changed 
// between runs.

bool operator==(const CBeadType& a, const CBeadType& b)
{
	return a.m_Name == b.m_Name;
}

//////////////////////////////////////////////////////////////////////
// Static member definitions
//////////////////////////////////////////////////////////////////////

long CBeadType::m_BeadTypeTotal = 0;	// No of bead types created so far

long CBeadType::GetTotal()
{
	return m_BeadTypeTotal;
}

// This function is called by CInputBDFile, CInputDPDFile and CInputMDFile to reset 
// the counter for bead types created between simulations and when reading a previous 
// control data file for a restart simulation.

void CBeadType::ZeroTotal()
{
	m_BeadTypeTotal = 0;
}

// Public function to create a new BD bead type by calling the constructur
// that handles Brownian Dynamics beads. Each bead requires the following parameters:
//
//  radius     - Used in the DPD force laws that are copied to BD particles
//  transDiff  - Translational diffusion coefficient
//  rotDiff    - Rotational diffusion coefficient
//
//  DPD conservative and dissipative force parameters with other beads.
//

CBeadType* CBeadType::AddBDBeadType(zString name, double radius, double transDiff, double rotDiff, 
                                    zDoubleVector consInt, zDoubleVector dissInt)
{
	m_BeadTypeTotal++;	// count types starting at 1 but access vector from 0

	CBDBeadStructure* pStructure = new CBDBeadStructure(radius, transDiff, rotDiff, consInt, dissInt);

	return new CBeadType(name, pStructure);
}


// Public function to create a new DPD bead type by calling the constructor that 
// handles DPD beads. Each bead requires only its vector of conservative and
// dissipative interactions with the other bead types already defined. We count 
// the number of existing bead types using the static counter m_BeadTypeTotal.

CBeadType* CBeadType::AddDPDBeadType(zString name, zDoubleVector consInt, zDoubleVector dissInt)
{
	m_BeadTypeTotal++;	// count types starting at 1 but access vector from 0

	CDPDBeadStructure* pStructure = new CDPDBeadStructure(consInt, dissInt);

	return new CBeadType(name, pStructure);
}

// Overloaded function to create a new DPD bead type by calling the constructor that 
// handles DPD beads. Each bead requires a radius of interaction as well as
// its vector of conservative and dissipative interactions with the other bead types 
// already defined. We count the number of existing bead types using the static 
// counter m_BeadTypeTotal.

CBeadType* CBeadType::AddDPDBeadType(zString name, double radius, zDoubleVector consInt, zDoubleVector dissInt)
{
	m_BeadTypeTotal++;	// count types starting at 1 but access vector from 0

#ifndef UseDPDBeadRadii
	CDPDBeadStructure* pStructure = new CDPDBeadStructure(consInt, dissInt);
#else
	CDPDBeadStructure* pStructure = new CDPDBeadStructure(radius, consInt, dissInt);
#endif

	return new CBeadType(name, pStructure);
}

// Overloaded function to create a new liquid-gas DPD bead type by calling the 
// constructor that handles DPD beads. Each bead requires two radii of interactions,
// one for the standard DPD conservative force and one for the density-dependent
// force, as well as the vectors of conservative and dissipative interactions and
// the density-dependent force parameters. We count the number of existing bead types 
// using the static counter m_BeadTypeTotal.

CBeadType* CBeadType::AddDPDBeadType(zString name, double radius, double lgRadius, 
                                     zDoubleVector consInt, zDoubleVector dissInt, zDoubleVector lgInt)
{
	m_BeadTypeTotal++;	// count types starting at 1 but access vector from 0

	CDPDBeadStructure* pStructure = new CDPDBeadStructure(radius, lgRadius, consInt, dissInt, lgInt);

	return new CBeadType(name, pStructure);
}

// Public function to create a new MD bead type by calling the constructor that 
// handles MD beads.

CBeadType* CBeadType::AddMDBeadType(zString name, zDoubleVector ljDepth, zDoubleVector ljRange,
												  zDoubleVector scDepth, zDoubleVector scRange)
{
	m_BeadTypeTotal++;	// count types starting at 1 but access vector from 0

	CMDBeadStructure* pStructure = new CMDBeadStructure(ljDepth, ljRange, scDepth, scRange);

	return new CBeadType(name, pStructure);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBeadType::CBeadType(zString name, CBeadStructure* const pStructure) : m_Name(name), m_pStructure(pStructure)
{
}

// Destroy the composed CBeadStructure object if it exists

CBeadType::~CBeadType()
{
	if(m_pStructure)
		delete m_pStructure;
}

// Function to return the embedded CBDBeadStructure object. 

const CBDBeadStructure* CBeadType::GetBDStructure() const
{
	return dynamic_cast<CBDBeadStructure*>(m_pStructure);
}

// Function to return the embedded CDPDBeadStructure object. 

const CDPDBeadStructure* CBeadType::GetDPDStructure() const
{
	return dynamic_cast<CDPDBeadStructure*>(m_pStructure);
}

// Function to return the embedded CMDBeadStructure object. 

const CMDBeadStructure* CBeadType::GetMDStructure() const
{
	return dynamic_cast<CMDBeadStructure*>(m_pStructure);
}
