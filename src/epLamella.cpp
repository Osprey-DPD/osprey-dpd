/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// epLamella.cpp: implementation of the epLamella class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "epLamella.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this protocol. 
// The static member function GetType() is invoked by the CExperiment 
// to compare the type read from the protocol file with each
// xxProtocol-derived class so that it can create the appropriate object 
// to hold the protocol data.

const zString epLamella::m_Type = "eplamella";

const zString epLamella::GetType()
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
	xxProtocol* Create(const zString runId) {return new epLamella(runId);}

	const zString id = epLamella::GetType();

	const bool bRegistered = acfProtocolFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// The user must supply an identifying label for the protocol.
// This is passed to the xxProtocol base class to assemble the complete
// name of the protocol file that holds the data for the experiment.
//
// If the specified protocol file is not found, the default data stored 
// in this class, and in the CSimulation and related classes is used to 
// define the experiment.

epLamella::epLamella(const zString runId) : xxProtocol("eplamella", runId)
{

}

epLamella::~epLamella()
{

}

// Non-static function to return the type of the protocol

const zString epLamella::GetProtocolType() const
{
	return m_Type;
}

// Functions showing whether a probe field and external field are allowed
// or required by this protocol class. These should be replaced at some point
// by policy classes.

bool epLamella::IsExternalFieldAllowed() const
{
	return false;
}

bool epLamella::IsExternalFieldRequired() const
{
	return false;
}

bool epLamella::IsProbeFieldAllowed() const
{
	return true;
}

bool epLamella::IsProbeFieldRequired() const
{
	return false;
}


