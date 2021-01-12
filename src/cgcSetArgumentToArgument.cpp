/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgcSetArgumentToArgument.cpp: implementation of the cgcSetArgumentToArgument class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgcSetArgumentToArgument.h"
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

const zString cgcSetArgumentToArgument::m_Type = "SetArgumentToArgument";

const zString cgcSetArgumentToArgument::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long cgcSetArgumentToArgument::m_ArgumentTotal = 5;

long cgcSetArgumentToArgument::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new cgcSetArgumentToArgument(executionTime);}

	const zString id = cgcSetArgumentToArgument::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgcSetArgumentToArgument::cgcSetArgumentToArgument(long executionTime) : xxCommand(executionTime), 
                                            m_GroupName(""), 
                                            m_SourceArgName(""), m_DestArgName(""),
                                            m_SourceCmdIndex(0), m_DestCmdIndex(0)
{
}

// Constructor for use when creating the command internally.

cgcSetArgumentToArgument::cgcSetArgumentToArgument(long executionTime,  const zString groupName, 
                             const zString sourceArgName, const zString destArgName, 
                             long sourceCmdIndex, long destCmdIndex) : xxCommand(executionTime),
						     m_GroupName(groupName), 
                             m_SourceArgName(sourceArgName),   m_DestArgName(destArgName),
                             m_SourceCmdIndex(sourceCmdIndex), m_DestCmdIndex(destCmdIndex)
{
}

// Copy constructor
cgcSetArgumentToArgument::cgcSetArgumentToArgument(const cgcSetArgumentToArgument& oldCommand) : xxCommand(oldCommand),
										   m_GroupName(oldCommand.m_GroupName),
										   m_SourceArgName(oldCommand.m_SourceArgName),
										   m_DestArgName(oldCommand.m_DestArgName),
										   m_SourceCmdIndex(oldCommand.m_SourceCmdIndex),
										   m_DestCmdIndex(oldCommand.m_DestCmdIndex)
{
}

cgcSetArgumentToArgument::~cgcSetArgumentToArgument()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_GroupName;	   Command group name
//  m_SourceArgName;   Placeholder name for the source command's argument
//  m_DestArgName;     Placeholder name for destination command's argument
//  m_SourceCmdIndex;  Index of the source command whose argument is being copied
//  m_DestCmdIndex;    Index of the destination command whose argument is being modified


zOutStream& cgcSetArgumentToArgument::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<CommandGroupName>"         << m_GroupName       << "</CommandGroupName>"        << zEndl;
	os << "<DestinationCommandIndex>"  << m_DestCmdIndex    << "</DestinationCommandIndex>" << zEndl;
	os << "<DestArgumentName>"         << m_DestArgName     << "</DestArgumentName>"        << zEndl;
	os << "<SourceCommandIndex>"       << m_SourceCmdIndex  << "</SourceCommandIndex>"      << zEndl;
	os << "<SourceArgumentName>"       << m_SourceArgName   << "</SourceArgumentName>"      << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_GroupName << " " << m_DestCmdIndex << " " << m_DestArgName << " " << m_SourceCmdIndex << " " << m_SourceArgName << zEndl;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& cgcSetArgumentToArgument::get(zInStream& is)
{
	is >> m_GroupName >> m_DestCmdIndex >> m_DestArgName >> m_SourceCmdIndex >> m_SourceArgName;

	if(!is.good())
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString cgcSetArgumentToArgument::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* cgcSetArgumentToArgument::GetCommand() const
{
	return new cgcSetArgumentToArgument(*this);
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool cgcSetArgumentToArgument::Execute(long simTime, ISimCmd* const pISimCmd) const
{
// We only allow execution of the command if the feature is enabled. 

	if(simTime == GetExecutionTime())
	{
#if EnableCommandGroups == SimCommandEnabled
		pISimCmd->SetArgumentToArgument(this);
#endif
		return true;
	}
	else
		return false;
}

// Function to check that the names of the command group and its contained command
// are valid. We also check that the command group exists. The test on the
// names of the argument and its new value are performed in the implementation class.

bool cgcSetArgumentToArgument::IsDataValid(const CInputData& riData) const
{
	// Check group name is unique and consists only of allowed characters.

	if(!riData.IsExternalNameValid(m_GroupName))
		return ErrorTrace("Invalid name for a command group being modified");
	else if(m_SourceCmdIndex < 1)
		return ErrorTrace("Invalid source command index for command group being modified");
	else if(m_DestCmdIndex < 1)
		return ErrorTrace("Invalid destination command index for command group being modified");
	else if(!riData.IsExternalNameValid(m_SourceArgName))
		return ErrorTrace("Invalid source argument name for a command group being modified");
	else if(!riData.IsExternalNameValid(m_DestArgName))
		return ErrorTrace("Invalid destination argument name for a command group being modified");

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
//  m_GroupName        - zString
//  m_SourceCmdIndex   - long
//  m_SourceArgName    - zString
//  m_DestCmdIndex     - long
//  m_DestArgName      - zString

bool cgcSetArgumentToArgument::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(0)->GetValue(&m_GroupName);
        vArguments.at(1)->GetValue(&m_SourceCmdIndex);
        vArguments.at(2)->GetValue(&m_SourceArgName);
        vArguments.at(3)->GetValue(&m_DestCmdIndex);
        vArguments.at(4)->GetValue(&m_DestArgName);

        std::cout << "Command arguments for " << GetType() << " are: " << zEndl;
        std::cout << m_GroupName << " " << m_SourceCmdIndex << " " << m_SourceArgName << " " << m_DestCmdIndex << " " << m_DestArgName << zEndl;

    return true;
    }
    else
    {
        return false;
    }

#endif
}

