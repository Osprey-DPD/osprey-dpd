/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// DPDBeadStructure.cpp: implementation of the CDPDBeadStructure class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "DPDBeadStructure.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDPDBeadStructure::CDPDBeadStructure(zDoubleVector consInt, zDoubleVector dissInt) : m_Radius(0.5), m_LGRadius(0.0), 
                                        m_vConsInt(consInt), m_vDissInt(dissInt)
{
    m_vLGInt.clear();
}

// Overridden constructor to specify the bead interaction radius.

CDPDBeadStructure::CDPDBeadStructure(double radius, zDoubleVector consInt, zDoubleVector dissInt) : m_Radius(radius), m_LGRadius(0.0), 
                                     m_vConsInt(consInt), m_vDissInt(dissInt)
{
    m_vLGInt.clear();
}

// Overridden constructor that allows liquid-gas interaction between beads.

CDPDBeadStructure::CDPDBeadStructure(double radius, double lgRadius, zDoubleVector consInt, 
                                     zDoubleVector dissInt, zDoubleVector lgInt) : m_Radius(radius),m_LGRadius(lgRadius), 
                                     m_vConsInt(consInt), m_vDissInt(dissInt), m_vLGInt(lgInt)
{
}

CDPDBeadStructure::~CDPDBeadStructure()
{
}

// Function to return the interaction radius for the conservative force of a bead type 

double CDPDBeadStructure::GetRadius() const
{
	return m_Radius;
}

// Function to return the interaction radius for the density-dependent force
// for a bead type in a liquid-gas DPD system.

double CDPDBeadStructure::GetLGRadius() const
{
	return m_LGRadius;
}

// Function to return the conservative interaction parameters.
// We return it by constant reference so it does not create a copy nor 
// can it be altered.

const zDoubleVector& CDPDBeadStructure::GetConsInt() const
{
	return m_vConsInt;
}

// Function to return the dissipative interaction parameters.
// We return it by constant reference so it does not create a copy nor 
// can it be altered.

const zDoubleVector& CDPDBeadStructure::GetDissInt() const
{
	return m_vDissInt;
}

// Function to return the density-dependent liquid-gas interaction parameters. 
// We return it by constant reference so it does not create a copy nor can it 
// be altered.

const zDoubleVector& CDPDBeadStructure::GetLGInt() const
{
	return m_vLGInt;
}

