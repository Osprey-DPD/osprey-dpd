/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgtToggleAllCommandExecutionInGroup.cpp: implementation of the cgtToggleAllCommandExecutionInGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgtToggleAllCommandExecutionInGroup.h"
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

const zString cgtToggleAllCommandExecutionInGroup::m_Type = "ToggleAllCommandExecutionInGroup";

const zString cgtToggleAllCommandExecutionInGroup::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long cgtToggleAllCommandExecutionInGroup::m_ArgumentTotal = 1;

long cgtToggleAllCommandExecutionInGroup::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new cgtToggleAllCommandExecutionInGroup(executionTime);}

	const zString id = cgtToggleAllCommandExecutionInGroup::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgtToggleAllCommandExecutionInGroup::cgtToggleAllCommandExecutionInGroup(long executionTime) : xxCommand(executionTime), 
                                            m_GroupName("")
{
}

// Constructor for use when creating the command internally.

cgtToggleAllCommandExecutionInGroup::cgtToggleAllCommandExecutionInGroup(long executionTime, 
                                            const zString groupName) : xxCommand(executionTime),
					                        m_GroupName(groupName)
{
}

// Copy constructor
cgtToggleAllCommandExecutionInGroup::cgtToggleAllCommandExecutionInGroup(const cgtToggleAllCommandExecutionInGroup& oldCommand) : xxCommand(oldCommand),
										   m_GroupName(oldCommand.m_GroupName)
{
}

cgtToggleAllCommandExecutionInGroup::~cgtToggleAllCommandExecutionInGroup()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_GroupName		Command group's name (alphanumeric beginning with a letter)

zOutStream& cgtToggleAllCommandExecutionInGroup::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<CommandGroupName>" << m_GroupName    << "</CommandGroupName>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_GroupName << zEndl;
	putASCIIEndTags(os);

#endif

	return os;
}

// Note that because we don't know how many arguments a given command needs,
// we have to get the user to enter the number. 


zInStream& cgtToggleAllCommandExecutionInGroup::get(zInStream& is)
{
	is >> m_GroupName;

	if(!is.good())
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString cgtToggleAllCommandExecutionInGroup::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* cgtToggleAllCommandExecutionInGroup::GetCommand() const
{
	return new cgtToggleAllCommandExecutionInGroup(*this);
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool cgtToggleAllCommandExecutionInGroup::Execute(long simTime, ISimCmd* const pISimCmd) const
{
// We only allow execution of the command if the feature is enabled. Note that we
// prefix the handler function with "Internal" to distinguish it from the SimBox's
// implementation function that returns a bool.

	if(simTime == GetExecutionTime())
	{
#if EnableCommandGroups == SimCommandEnabled
		pISimCmd->ToggleAllCommandExecutionInGroup(this);
#endif
		return true;
	}
	else
		return false;
}

// Function to check that the name of the command group is valid. It must
// be alphanumeric beginning with a letter and unique amongst all command groups.
// Note that even if a group is deleted its name remains in the map, and cannot
// be used again. We do not check that the group exists, as it may have been
// created dynamically as the result of a command.

bool cgtToggleAllCommandExecutionInGroup::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_GroupName))
		return ErrorTrace("Invalid command group name");

	return true;
}

// VF that allows an empty command instance to fill up its parameters after creation.
// It is typically used by the command groups to allow the command's arguments to
// be set when each of the group's commands is instantiated. The function returns
// true if all the command's parameters are correctly assigned values.
//
// The argument contains instances of types derived from tguArgumentBase that must
// be in the order expected by the command as it tries to cast them to the types
// it expects.
//
// This command expects the following types (in addition to its base class types):
//
// m_GroupName     zString

bool cgtToggleAllCommandExecutionInGroup::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(vArguments.size() == GetArgumentTotal())
    {
        vArguments.at(0)->GetValue(&m_GroupName);

        return true;
    }
    else
    {
        return false;
    }

#endif
}


