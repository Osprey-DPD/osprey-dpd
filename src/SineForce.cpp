/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// SineForce.cpp: implementation of the CSineForce class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SineForce.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSineForce::CSineForce(long start, long end, long x, long y, long z, 
					   double amp, double period) : CForceLaw(start, end, x, y, z),
												    m_Amplitude(amp),
												    m_Period(period)
{
	// Define the angular frequency for speed of use later

	m_Frequency = 2.0*xxBase::m_globalTwoPI/m_Period;
}

CSineForce::~CSineForce()
{
}

// Functions to return the components of the sinusoidal force. Note that we don't
// check whether the force should be applied, that is done by the SimBox, we
// only pass out the magnitude of the force at the current time. When the 
// sine force command is first executed the magnitude of the force is zero,
// i.e., the phase is zero.

double CSineForce::GetXForce(long simTime)
{
	return m_Amplitude*m_X*sin(m_Frequency*static_cast<double>(simTime - m_Start));
}

double CSineForce::GetYForce(long simTime)
{
	return m_Amplitude*m_Y*sin(m_Frequency*static_cast<double>(simTime - m_Start));
}

double CSineForce::GetZForce(long simTime)
{
	return m_Amplitude*m_Z*sin(m_Frequency*static_cast<double>(simTime - m_Start));
}
