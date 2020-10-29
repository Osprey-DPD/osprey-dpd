/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// BDBeadStructure.cpp: implementation of the CBDBeadStructure class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "BDBeadStructure.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBDBeadStructure::CBDBeadStructure(double radius, double transDiff, double rotDiff, 
                                   zDoubleVector consInt, zDoubleVector dissInt) : m_Radius(radius), 
                                   m_TransDiffusion(transDiff),
                                   m_RotDiffusion(rotDiff),
                                   m_vConsInt(consInt), m_vDissInt(dissInt)
{
}

CBDBeadStructure::~CBDBeadStructure()
{
}

// Function to return the interaction radius for the bead type.

double CBDBeadStructure::GetRadius() const
{
	return m_Radius;
}

// Function to return the translational diffusion coefficient for the bead type.

double CBDBeadStructure::GetTransDiff() const
{
    return m_TransDiffusion;
}

// Function to return the rotational diffusion coefficient for the bead type.

double CBDBeadStructure::GetRotDiff() const
{
    return m_RotDiffusion;
}

// Function to return a sub-vector of conservative interaction parameters.
// We return it by constant reference so it does not create a copy nor 
// can it be altered.

const zDoubleVector& CBDBeadStructure::GetConsInt() const
{
	return m_vConsInt;
}

// Function to return a sub-vector of dissipation interaction parameters.
// We return it by constant reference so it does not create a copy nor 
// can it be altered.

const zDoubleVector& CBDBeadStructure::GetDissInt() const
{
	return m_vDissInt;
}


