/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctWorkDoneOnBeadTypeInTarget.cpp: implementation of the ctWorkDoneOnBeadTypeInTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctWorkDoneOnBeadTypeInTarget.h"
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

const zString ctWorkDoneOnBeadTypeInTarget::m_Type = "WorkDoneOnBeadTypeInTarget";

const zString ctWorkDoneOnBeadTypeInTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctWorkDoneOnBeadTypeInTarget::m_ArgumentTotal = 2;

long ctWorkDoneOnBeadTypeInTarget::GetArgumentTotal()
{
	return m_ArgumentTotal;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// command class but a global function hidden in the namespace.

namespace
{
	xxCommand* Create(long executionTime) {return new ctWorkDoneOnBeadTypeInTarget(executionTime);}

	const zString id = ctWorkDoneOnBeadTypeInTarget::GetType();
    const long  argTotal = ctWorkDoneOnBeadTypeInTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctWorkDoneOnBeadTypeInTarget::ctWorkDoneOnBeadTypeInTarget(long executionTime) : ctApplyCommand(executionTime),
															m_BeadType(-1)
{
}

ctWorkDoneOnBeadTypeInTarget::ctWorkDoneOnBeadTypeInTarget(const ctWorkDoneOnBeadTypeInTarget& oldCommand) : ctApplyCommand(oldCommand),
															m_BeadType(oldCommand.m_BeadType)
{
	
}

// Constructor for use when creating the command internally.

ctWorkDoneOnBeadTypeInTarget::ctWorkDoneOnBeadTypeInTarget(long executionTime, const zString target) : ctApplyCommand(executionTime, target),
															m_BeadType(-1)
{
}

ctWorkDoneOnBeadTypeInTarget::~ctWorkDoneOnBeadTypeInTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_BeadType		Type identifier of beads whose work is to be calculated
//

zOutStream& ctWorkDoneOnBeadTypeInTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	ctApplyCommand::put(os);
	os << "<BeadType>" << m_BeadType << "</BeadType>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_BeadType;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctWorkDoneOnBeadTypeInTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	is >> m_BeadType;

	if(!is.good() || m_BeadType < 0)
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctWorkDoneOnBeadTypeInTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctWorkDoneOnBeadTypeInTarget::GetCommand() const
{
	return new ctWorkDoneOnBeadTypeInTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctWorkDoneOnBeadTypeInTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->WorkDoneOnBeadTypeInTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. Because the only datum
// is a bead type identifier and this may not occur in the CInputData's map
// if the type is dynamically created we do not check for it here.

bool ctWorkDoneOnBeadTypeInTarget::IsDataValid(const CInputData& riData) const
{
	return ctApplyCommand::IsDataValid(riData);
}
