/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prBilayer.cpp: implementation of the prBilayer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "prBilayer.h"
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

const zString prBilayer::m_Type = "Bilayer";

const zString prBilayer::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	xxProcess* Create() {return new prBilayer();}

	const zString id = prBilayer::GetType();

	const bool bRegistered = acfProcessFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

prBilayer::prBilayer() : xxProcess(true)

{

}

prBilayer::~prBilayer()
{
}

// Member functions to write/read the data specific to the process.

zOutStream& prBilayer::put(zOutStream& os) const
{
	// Write base class data first

	xxProcess::put(os);
	
	return os;
}

zInStream& prBilayer::get(zInStream& is)
{
	// Read base class data first

	xxProcess::get(is);

	return is;
}


// Non-static function to return the type of the process

const zString prBilayer::GetProcessType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current process.

xxProcess* prBilayer::GetProcess() const
{
	return new prBilayer(*this);
}

// Function to update the state of the process using data from the associated
// aggregate (CVesicle) and events (evVesiclePore,...).
//

void prBilayer::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{

}

// Store pointers to the CAnalysis-derived objects that the process
// will monitor for activity during the simulation. These are const
// because we don't allow data to be written back to the aggregate.
// Because all the aggregates are stored as base class, CAnalysis, pointers,
// we have to use reinterpret_cast to get back to the CBilayer objects.
// This means that if the wrong CAnalysis-derived class is passed in 
// the code should fail gracefully.

void prBilayer::SetAggregate(const CAnalysis* pAggregate1)
{
}

// No external events are defined for this process.

void prBilayer::SetEvent()
{
}

// Function to check that the user-specified data is valid and, if so, 
// to find the aggregates and events that are used by the process.
//
// An xxProcessState is created to hold the data that the process wants
// to write to file. Data is passed as a series of CTimeSeriesData objects
// as used for CHistoryState.

bool prBilayer::ValidateData(const CInputData &riData)
{

	// All data has been validated so create and store an xxProcessState object
	// to allow the xxProcess to write its data to a process state file.

    SetState(new xxProcessState(xxBase::GetPSPrefix() + GetProcessType() + ToString(GetId()) + "." + riData.GetRunId(), GetStartTime(), GetEndTime()));

	return true;
}

