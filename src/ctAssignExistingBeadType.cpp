/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctAssignExistingBeadType.cpp: implementation of the ctAssignExistingBeadType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctAssignExistingBeadType.h"
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

const zString ctAssignExistingBeadType::m_Type = "AssignExistingBeadType";

const zString ctAssignExistingBeadType::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctAssignExistingBeadType::m_ArgumentTotal = 2;

long ctAssignExistingBeadType::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctAssignExistingBeadType(executionTime);}

	const zString id = ctAssignExistingBeadType::GetType();
    const long  argTotal = ctAssignExistingBeadType::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctAssignExistingBeadType::ctAssignExistingBeadType(long executionTime) : ctApplyCommand(executionTime),
													m_AssignedName("")
{
}

ctAssignExistingBeadType::ctAssignExistingBeadType(const ctAssignExistingBeadType& oldCommand) : ctApplyCommand(oldCommand),
													m_AssignedName(oldCommand.m_AssignedName)
{
}

ctAssignExistingBeadType::~ctAssignExistingBeadType()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_AssignedName		Name of existing type to assign to the beads in a CCommandTarget
//


zOutStream& ctAssignExistingBeadType::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	// now the derived class data
	os << "<AssignedName>" << m_AssignedName << "</AssignedName>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_AssignedName;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctAssignExistingBeadType::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	is >> m_AssignedName;

	if(!is.good())
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctAssignExistingBeadType::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctAssignExistingBeadType::GetCommand() const
{
	return new ctAssignExistingBeadType(*this);
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctAssignExistingBeadType::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->AssignExistingBeadType(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. The only datum required
// is the currently-existing bead type to assign to the command target's beads.

bool ctAssignExistingBeadType::IsDataValid(const CInputData& riData) const
{
	if(ctApplyCommand::IsDataValid(riData))
	{
		if(!riData.IsExternalNameValid(m_AssignedName))
			return ErrorTrace("Invalid AssignBeadType command bead name");
		else if(!riData.IsBeadinMap(m_AssignedName))
			return ErrorTrace("AssignBeadType command bead type does not exist");
	}
	else
		return false;

	return true;
}

// VF to pack the arguments that are common to all classes derived from this class.
//
// This command expects the following type:
//
// m_TargetLabel    - zString
// m_AssignedName   - long

bool ctAssignExistingBeadType::Pack(const tguArgumentSequence& vArguments)
{
    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        ctApplyCommand::Pack(vArguments);
        vArguments.at(1)->GetValue(&m_AssignedName);

        return true;
    }
    else
    {
        return false;
    }
}
