/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateRestart.cpp: implementation of the CInitialStateRestart class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "InitialStateRestart.h"
#include "RestartBuilder.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for the initial state that 
// is represented by an object of this class. It is used to select the 
// appropriate CInitialStateData-derived object when the State variable is read 
// in the  control data file. The static member function GetType() is invoked by the
// CInitialStateObject to compare the type read from the control data file with each
// initial state data class so that it can create the appropriate object to hold the
// analysis data.

zString const CInitialStateRestart::m_Type = "restart";

const zString CInitialStateRestart::GetType()
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
	CInitialStateData* Create() {return new CInitialStateRestart();}

	const zString id = CInitialStateRestart::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitialStateRestart::CInitialStateRestart()
{
}

CInitialStateRestart::~CInitialStateRestart()
{
}

CInitialStateRestart& CInitialStateRestart::operator=(const CInitialStateRestart& oldISR)
{
	if(this != &oldISR)
	{
		m_RunId		= oldISR.m_RunId;
		m_StateId	= oldISR.m_StateId;
	}

	return *this;
}

zString CInitialStateRestart::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateRestart::put(zOutStream& os) const
{
	os << "State	" << "restart"    << zEndl;
	os << "			" << "RunId		" << "000" << zEndl;
	os << "			" << "StateId	" << 0     << zEndl;

	return os;
}

zInStream& CInitialStateRestart::get(zInStream& is)
{
// At this point we have already read in the "State restart" keywords so 
// we only have to read the data required by the restart state.

	zString token;
	zString runId;
	long stateId;

	is >> token;
	if(!is.good() || token != "RunId")
	{
		SetDataValid(false);
		return is;
	}

	is >> runId;
	if(!is.good())
	{
		SetDataValid(false);
		return is;
	}

	is >> token;
	if(!is.good() || token != "StateId")
	{
		SetDataValid(false);
		return is;
	}

	is >> stateId;
	if(!is.good())
	{
		SetDataValid(false);
		return is;
	}

// Read all data ok so update object

	m_RunId	  = runId;
	m_StateId = stateId;

	return is;
}

// Function to use the CRestartBuilder object to set the coordinates of all the 
// beads in the simulation into a random initial state. Note that the builder 
// is local to the CInitialStateRestart object. We pass it the data it needs
// from the input data to find and read the old restart data file.

bool CInitialStateRestart::Assemble(CInitialState& riState) const
{
	CRestartBuilder restart(m_RunId, m_StateId);

	return restart.Assemble(riState);
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateRestart::Copy() const
{
	CInitialStateRestart* pISR = new CInitialStateRestart;	// new data object

	*pISR = *this;		// assign new data values to new object

	return pISR;
}
