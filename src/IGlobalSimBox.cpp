/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// IGlobalSimBox.cpp: implementation of the IGlobalSimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "IGlobalSimBox.h"
#include "ISimBox.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member function returning a pointer to the singleton 
// ISimBox interface instance allowing global access to ISimBox functions 
// via the public interface of this class. The instance is stored in
// a static member variable as static functions cannot access non-static
// member data without a "this" pointer. Note that we store a pointer
// to the IGlobalSimBox class instead of the derived ISimBox instance
//  to prevent access to functions outside this interface class.

const IGlobalSimBox* IGlobalSimBox::Instance()
{
	return m_pInstance;
}

ISimBox* IGlobalSimBox::m_pInstance = 0;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Default constructor that does not store a pointer, but relies on the ISimBox singleton class to create it.

IGlobalSimBox::IGlobalSimBox()
{
}

// Constructor used by the derived ISimBox class to store a pointer to itself in this class that can be accessed via the
// static member function Instance().

IGlobalSimBox::IGlobalSimBox(ISimBox* pISimBox)
{
    if(!IGlobalSimBox::m_pInstance)
	{
	    IGlobalSimBox::m_pInstance = pISimBox;
	}
}

IGlobalSimBox::~IGlobalSimBox()
{
}

const zString IGlobalSimBox::GetRunId() const
{
	return IGlobalSimBox::m_pInstance->GetRunId();
}

long IGlobalSimBox::GetCurrentTime() const
{
	return IGlobalSimBox::m_pInstance->GetCurrentTime();
}

long IGlobalSimBox::GetTotalTime() const
{
	return IGlobalSimBox::m_pInstance->GetTotalTime();
}

double IGlobalSimBox::GetStepSize() const
{
	return IGlobalSimBox::m_pInstance->GetStepSize();
}

double IGlobalSimBox::GetSimBoxXLength() const
{
	return IGlobalSimBox::m_pInstance->GetSimBoxXLength();
}

double IGlobalSimBox::GetSimBoxYLength() const
{
	return IGlobalSimBox::m_pInstance->GetSimBoxYLength();
}

double IGlobalSimBox::GetSimBoxZLength() const
{
	return IGlobalSimBox::m_pInstance->GetSimBoxZLength();
}

double IGlobalSimBox::GetHalfSimBoxXLength() const
{
	return IGlobalSimBox::m_pInstance->GetHalfSimBoxXLength();
}

double IGlobalSimBox::GetHalfSimBoxYLength() const
{
	return IGlobalSimBox::m_pInstance->GetHalfSimBoxYLength();
}

double IGlobalSimBox::GetHalfSimBoxZLength() const
{
	return IGlobalSimBox::m_pInstance->GetHalfSimBoxZLength();
}

// Function showing if the simulation uses the liquid-gas conservative interactions.
// If this feature is compiled in, it is used if the input file specifies it,
// otherwise just the standard dpd conservative interactions are used.

bool IGlobalSimBox::IsDPDLG() const
{
#if EnableDPDLG == ExperimentEnabled
    return IGlobalSimBox::m_pInstance->IISimState()->IsDPDLG();
#elif EnableDPDLG == ExperimentDisabled
    return false;
#endif
}

// Forwarding functions to the ISimState class, and its nested members,  to get bead, bond, bondpair and polymer data.
// Note that checks on the validity of the "type" argument are performed in the CInitialState class for these
// functions so we do not repeat them here.

long IGlobalSimBox::GetBeadTotalForType(long type) const
{
    return IGlobalSimBox::m_pInstance->GetBeadTotalForType(type);
}

long IGlobalSimBox::GetBondTotalForType(long type) const
{
	return IGlobalSimBox::m_pInstance->GetBondTotalForType(type);
}

long IGlobalSimBox::GetBondPairTotalForType(long type) const
{
	return IGlobalSimBox::m_pInstance->GetBondPairTotalForType(type);
}

long IGlobalSimBox::GetPolymerTotalForType(long type) const
{
	return IGlobalSimBox::m_pInstance->GetPolymerTotalForType(type);
}

// Function to return the number of beads in a polymer of the specified type. If the specified type does not exist,
// we return an invalid value. This must be checked for in the calling routine.
// This is needed in the parallel code because the parallel SimBox class has no access to the original SimBox.

long IGlobalSimBox::GetPolymerSizeFromType(long type) const
{
	return IGlobalSimBox::m_pInstance->GetPolymerSizeFromType(type);
}

// Function to return the number fraction of the specified polymer type. If the specified type does not exist,
// we return zero. This may need checking in the calling routine.
// This is needed in the parallel code because the parallel SimBox class has no access to the original SimBox.

double IGlobalSimBox::GetPolymerFractionFromType(long type) const
{
	return IGlobalSimBox::m_pInstance->GetPolymerFractionFromType(type);
}

// Functions to return the set of types in the simulation.

const BeadVector&  IGlobalSimBox::GetBeadTypes() const
{
	return IGlobalSimBox::m_pInstance->GetBeadTypes();
}

const BondVector&  IGlobalSimBox::GetBondTypes() const
{
	return IGlobalSimBox::m_pInstance->GetBondTypes();
}

const BondPairVector&  IGlobalSimBox::GetBondPairTypes() const
{
	return IGlobalSimBox::m_pInstance->GetBondPairTypes();
}

const PolymerVector&  IGlobalSimBox::GetPolymerTypes() const
{
	return IGlobalSimBox::m_pInstance->GetPolymerTypes();
}



