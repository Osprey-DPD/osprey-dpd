/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LightField.cpp: implementation of the CLightField class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LightField.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLightField::CLightField() : m_c(2.9979e+08),
                             m_bPolarised(false),
							 m_Wavelength(400.0),
							 m_Spread(1.0), 
							 m_Frequency(1.0e+09*m_c/m_Wavelength)
{
	// Set the data to sensible initial values. Note that the light
	// wavelength is in nanometres, hence the factor of 10**9 in the frequency.
	// The source coordinates are at the origin.

	m_Source[0] = 0.0;	// In units of bead diameters
	m_Source[1] = 0.0;
	m_Source[2] = 0.0;

	m_PolAxis[0] = 0.0;	// Must be a unit vector
	m_PolAxis[1] = 0.0;
	m_PolAxis[2] = 1.0;


}

CLightField::~CLightField()
{

}

// Handler function to connect the bead density grid calculated in the
// simulation, and accessed via a prProbeFieldProcess object, to the
// IProbeField-derived class. The probe field object calculates the 
// interactions of a particular form of EM radiation with the matter 
// density field in the SimBox.
//
//

void CLightField::RegisterBeadDensityGrid()
{

}

// Handler function to connect the scattered radiation field, discretised
// onto a rectangular grid, with a specific pattern of detectors defined
// in an IDetectorGrid-derived class. The detector grid object uses this 
// function to gain access to the scattered field and so to calculate the 
// integrated output signal.

void CLightField::RegisterScatteredFieldGrid()
{

}

// Function to update the state of the light field given a new state of the
// SimBox. It uses the bead density grid obtained from the SimBox to calculate
// the matter distribution and the effect on the light field.

void CLightField::UpdateState()
{

}
