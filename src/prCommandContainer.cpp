/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prCommandContainer.cpp: implementation of the prCommandContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "prCommandContainer.h"
#include "SimState.h"
#include "ISimBox.h"
#include "TimeSeriesData.h"
#include "InputData.h"
#include "xxCommand.h"


// STL using declarations

				
//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.

const zString prCommandContainer::m_Type = "CommandContainer";

const zString prCommandContainer::GetType()
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
	xxProcess* Create() {return new prCommandContainer();}

	const zString id = prCommandContainer::GetType();

	// This process should only be created internally

	const bool bRegistered = acfProcessFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

prCommandContainer::prCommandContainer() : xxProcess(true)
{
	m_Commands.clear();
}

prCommandContainer::~prCommandContainer()
{
	// We don't delete the commands here as the CAnalysisState takes care of it

}

// Member functions to write/read the data specific to the process.

// This process can only be created internally

zOutStream& prCommandContainer::put(zOutStream& os) const
{	
	return os;
}

zInStream& prCommandContainer::get(zInStream& is)
{
	return is;
}

// Non-static function to return the type of the process

const zString prCommandContainer::GetProcessType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current process.

xxProcess* prCommandContainer::GetProcess() const
{
	return new prCommandContainer(*this);
}

// Function to update the state of the process. 
//
// This process receives the commands created as a result of calls to the 
// ICommandDocument interface of the experiment. The commands are read from a 
// user-specified XML document using the prCommandDocument process, converted 
// into xxCommand-derived class instances and packaged into this process. They
// are added to the simulation's command execution set via the ISimBox/ISimState 
// interfaces.

void prCommandContainer::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
#if EnableCommandDocumentProcess == SimProcessEnabled

	// Add the commands to the simulation's command queue. Note that they are
	// time-ordered within the ISimState's AddCommand function.

	for(CommandIterator iterCom=m_Commands.begin(); iterCom!=m_Commands.end(); iterCom++)
	{
		const xxCommand* const pCommand = *iterCom;

		if(pCommand)
			pISimBox->AddCommand(pCommand);
	}

	// Now delete the local command pointers and schedule the process for destruction

	m_Commands.clear();

#endif
}

// Function to check that the user-specified data is valid.

// This process has no data to be validated, so we just create and store 
// an xxProcessState object to allow the xxProcess to write its data to a 
// process state file.

bool prCommandContainer::ValidateData(const CInputData &riData)
{
	SetState(new xxProcessState(xxBase::GetPSPrefix() + GetProcessType() + ToString(GetId()) + "." + riData.GetRunId(), GetStartTime(), GetEndTime()));

	return true;
}

// Function to allow the process data to be validated when it is created internally.
// This function should not be called if the ValidateData() function has
// already been called.
//
// As this process requires no data for its creation, and has no state to 
// serialise, we do not create a state object for it.

bool prCommandContainer::InternalValidateData(const ISimState* const pISimState)
{
	return true;
}

// Function to add a command to the container. We check for null pointers, and 
// flag an error if one is found, but do not perform checks on the command's data.

bool prCommandContainer::AddCommand(const xxCommand *const pCommand)
{
#if EnableCommandDocumentProcess == SimProcessEnabled

	if(pCommand)
	{
		m_Commands.push_back(pCommand);
		return true;
	}
	else
		return false;
#else
    return false;
#endif
}
