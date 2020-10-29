/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSetCommandTimer.cpp: implementation of the ccSetCommandTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSetCommandTimer.h"
#include "ISimCmd.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString ccSetCommandTimer::m_Type = "SetCommandTimer";

const zString ccSetCommandTimer::GetType()
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
	xxCommand* Create(long executionTime) {return new ccSetCommandTimer(executionTime);}

	const zString id = ccSetCommandTimer::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSetCommandTimer::ccSetCommandTimer(long executionTime) : xxCommand(executionTime),
									m_Delay(0), m_LinesToRead(0)
{
}

ccSetCommandTimer::ccSetCommandTimer(const ccSetCommandTimer& oldCommand) : xxCommand(oldCommand),
									 m_Delay(oldCommand.m_Delay),
									 m_LinesToRead(oldCommand.m_LinesToRead)
{
}

ccSetCommandTimer::~ccSetCommandTimer()
{
	// This command is not responsible for destroying its payload of commands:
	// that is done in the CAnalysisState.
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	delay		No of time steps to wait before executing commands
//  lines		No of commands to read from CDF, each occurs on one line
//  command		A command and its data
//
// Note that we cannot write out the command sequence in general as it is specific
// to each use of this command. And we do not implement the get() function fully
// as it would require the class definitions of all possible commands to be 
// included here, and command instances to be instantiated from their keywords.
// This is not possible until the class factory has been implemented

zOutStream& ccSetCommandTimer::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<Delay>"			<< m_Delay		 << "</Delay>" << zEndl;
	os << "<CommandTotal>"  << m_LinesToRead << "</CommandTotal>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_Delay << "  " << m_LinesToRead;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSetCommandTimer::get(zInStream& is)
{
	// Check that the delay is greater than 0

	is >> m_Delay;

	if(!is.good() || m_Delay <= 0)
	   SetCommandValid(false);

	// Get the number of commands to read

	is >> m_LinesToRead;

	if(!is.good() || m_LinesToRead < 1)
		SetCommandValid(false);


	return is;
}

// Non-static function to return the type of the command

const zString ccSetCommandTimer::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSetCommandTimer::GetCommand() const
{
	return new ccSetCommandTimer(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccSetCommandTimer::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetCommandTimer(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data is valid: we have checked that
// the delay is positive definite, here we check that the commands specified
// exist and have appropriate data. NOT IMPLEMENTED YET.

bool ccSetCommandTimer::IsDataValid(const CInputData& riData) const
{
	return true;
}

// Function to allow a command to be added to the set that will be executed
// when the timer runs out. It is used by processes, and other entities within
// the simulation, when they want to schedule commands for later execution.

void ccSetCommandTimer::AddCommand(const xxCommand *const pCommand)
{
	m_Commands.push_back(pCommand);
}

// Function to set the number of time steps before the timer executes its
// payload of commands. Note that non-positive delays are set to one time step.

void ccSetCommandTimer::SetDelay(long delay)
{
	if(delay > 0)
		m_Delay = delay;
	else
		m_Delay = 1;
}
