/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prLightField.cpp: implementation of the prLightField class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "prLightField.h"
#include "SimState.h"
#include "ISimBox.h"
#include "TimeSeriesData.h"
#include "InputData.h"


				
//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.

const zString prLightField::m_Type = "LightField";

const zString prLightField::GetType()
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

prLightField::prLightField() : prProbeFieldProcess(true), m_bPolarised(false), 
							   m_Wavelength(400.0), m_Spread(1.0)
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

prLightField::~prLightField()
{
}

// Member functions to write/read the data specific to the process.

zOutStream& prLightField::put(zOutStream& os) const
{
	// Write base class data first

	xxProcess::put(os);
	

	return os;
}

// Function to read the data needed by the process.
//

zInStream& prLightField::get(zInStream& is)
{
	// Read base class data first

	xxProcess::get(is);


	return is;
}


// Non-static function to return the type of the process

const zString prLightField::GetProcessType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current process.

xxProcess* prLightField::GetProcess() const
{
	return new prLightField(*this);
}

// Function to update the state of the process. 

void prLightField::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{




}

// Function to check that the user-specified data is valid and, if so, 
// to find the aggregates and events that are used by the process.
//
// An xxProcessState is created to hold the data that the process wants
// to write to file. Data is passed as a series of CTimeSeriesData objects
// as used for CHistoryState.

bool prLightField::ValidateData(const CInputData &riData)
{
	// All data has been validated so create and store an xxProcessState object
	// to allow the xxProcess to write its data to a process state file.

	SetState(new xxProcessState(xxBase::GetPSPrefix() + GetProcessType() + ToString(GetId()) + "." + riData.GetRunId(), GetStartTime(), GetEndTime()));

	return true;
}

