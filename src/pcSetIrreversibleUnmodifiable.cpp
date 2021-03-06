/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// pcSetIrreversibleUnmodifiable.cpp: implementation of the pcSetIrreversibleUnmodifiable class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "pcSetIrreversibleUnmodifiable.h"
#include "IModifyBLMVesicleFusion.h"
#include "LogpcProcessNotAccessible.h"
#include "LogpcProcessNotModifiable.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString pcSetIrreversibleUnmodifiable::m_Type = "SetProcessIrreversiblyUnmodifiable";

const zString pcSetIrreversibleUnmodifiable::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// command class but a global function hidden in the namespace.

namespace
{
	xxCommand* Create(long executionTime) {return new pcSetIrreversibleUnmodifiable(executionTime);}

	const zString id = pcSetIrreversibleUnmodifiable::GetType();

#if EnableProcessCommand == SimCommandEnabled
	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
#endif
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

pcSetIrreversibleUnmodifiable::pcSetIrreversibleUnmodifiable(long executionTime) : xxCommand(executionTime),
								   m_pid("")
{
}

pcSetIrreversibleUnmodifiable::pcSetIrreversibleUnmodifiable(const pcSetIrreversibleUnmodifiable& oldCommand) : xxCommand(oldCommand),
									 m_pid(oldCommand.m_pid)
{
}

pcSetIrreversibleUnmodifiable::~pcSetIrreversibleUnmodifiable()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_pid		Process id string that uniquely identifies each process object

zOutStream& pcSetIrreversibleUnmodifiable::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<ProcessId>"    << m_pid   << "</ProcessId>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_pid;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& pcSetIrreversibleUnmodifiable::get(zInStream& is)
{
	// Check that the process id is not empty

	is >> m_pid;

	if(!is.good() || m_pid == "")
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString pcSetIrreversibleUnmodifiable::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* pcSetIrreversibleUnmodifiable::GetCommand() const
{
	return new pcSetIrreversibleUnmodifiable(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 
//
// First we check that the associated process is stored as modifiable and
// is currently allowing itself to be modified (by calling its IsModifiable() 
// function)) then we check that the process can receive this command, and finally
// we set the stretch delay. If the process is not modifiable at all or not 
// allowing modifications or is the wrong type of process we ignore the command.

bool pcSetIrreversibleUnmodifiable::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
#if EnableProcessCommand == SimCommandEnabled
		IModifyProcess* pIMP = pISimCmd->GetISimBox()->GetModifiableProcessFromPid(m_pid);

		if(pIMP)
		{
			IModifyBLMVesicleFusion* pIProcess = dynamic_cast<IModifyBLMVesicleFusion*>(pIMP);

			if(pIProcess)
			{
				pIProcess->SetIrreversibleUnmodifiable(this);
			}
			else
			{
				 new CLogpcProcessNotAccessible(GetExecutionTime(), m_pid, GetCommandType());
			}
		}
		else
		{
				 new CLogpcProcessNotModifiable(GetExecutionTime(), m_pid);
		}
#endif
		return true;
	}
	else
		return false;
}

// Function to check that the command data is valid: there are no further checks.

bool pcSetIrreversibleUnmodifiable::IsDataValid(const CInputData& riData) const
{
	return true;
}
