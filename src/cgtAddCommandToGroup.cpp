/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgtAddCommandToGroup.cpp: implementation of the cgtAddCommandToGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimCommandFlags.h"
#include "cgtAddCommandToGroup.h"
#include "acfCommandFactory.h"
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

const zString cgtAddCommandToGroup::m_Type = "AddCommandToGroup";

const zString cgtAddCommandToGroup::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.
// Because the number of arguments for this command class depends on the type
// of command being added to the group, we can only store here the constant
// number defined by this class. The user has to still enter the number of arguments
// needed by the specific command being added.

long cgtAddCommandToGroup::m_ArgumentTotal = 2;

long cgtAddCommandToGroup::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new cgtAddCommandToGroup(executionTime);}

	const zString id = cgtAddCommandToGroup::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgtAddCommandToGroup::cgtAddCommandToGroup(long executionTime) : xxCommand(executionTime), 
                                            m_GroupName(""), m_CommandName("")
{
    m_ArgNames.clear();
}

// Constructor for use when creating the command internally.

cgtAddCommandToGroup::cgtAddCommandToGroup(long executionTime, const zString groupName,
                                           const zString cmdName, StringSequence argNames) : xxCommand(executionTime),
											m_GroupName(groupName), m_CommandName(cmdName)
{
    m_ArgNames.clear();
    for(StringSequenceIterator iterArg=argNames.begin(); iterArg!=argNames.end(); iterArg++)
    {
        m_ArgNames.push_back(*iterArg);
    }
}

// Copy constructor
cgtAddCommandToGroup::cgtAddCommandToGroup(const cgtAddCommandToGroup& oldCommand) : xxCommand(oldCommand),
										   m_GroupName(oldCommand.m_GroupName),
										   m_CommandName(oldCommand.m_CommandName)

{
    StringSequence oldArgNames = oldCommand.m_ArgNames;

    m_ArgNames.clear();
    for(StringSequenceIterator iterArg=oldArgNames.begin(); iterArg!=oldArgNames.end(); iterArg++)
    {
        m_ArgNames.push_back(*iterArg);
    }
}

cgtAddCommandToGroup::~cgtAddCommandToGroup()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	GroupName		Command group's name (alphanumeric beginning with a letter)
//  CommandName     Name of command to add
//  ArgNames        Placeholder names of arguments to commands

zOutStream& cgtAddCommandToGroup::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<CommandGroupName>" << m_GroupName   << "</CommandGroupName>" << zEndl;
	os << "<CommandName>"      << m_CommandName << "</CommandName>" << zEndl;
    for(cStringSequenceIterator citerArg=m_ArgNames.begin(); citerArg!=m_ArgNames.end(); citerArg++)
    {
	os << "<ArgumentName>"     << *citerArg << "</ArgumentName>" << " ";
    }
    os << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_GroupName << " " << m_CommandName << zEndl;

    for(cStringSequenceIterator citerArg=m_ArgNames.begin(); citerArg!=m_ArgNames.end(); citerArg++)
    {
        os << *citerArg << " ";
    }
    os << zEndl;
	putASCIIEndTags(os);

#endif

	return os;
}

// Note that because we don't know how many arguments a given command needs,
// we retrieve its argument total from the command ACF before attempting 
// to read in the placeholder names. If an illegal value is returned, this
// indicates that the command cannot be added to a group, and we abort the command.

zInStream& cgtAddCommandToGroup::get(zInStream& is)
{
	is >> m_GroupName >> m_CommandName;

	if(!is.good())
	   SetCommandValid(false);

    // Retrieve the command class' argument total from the command ACF;
    // if the command has not registered this total, an illegal value is returned
    // and we abort reading the command. This means the command cannot be used
    // in a command group.

    const long argTotal = acfCommandFactory::Instance()->GetArgumentTotal(m_CommandName);

    if(argTotal >= 0)
    {
        zString sName;

        for(long i=0; i<argTotal; i++)
        {
		    is >> sName;

		    if(is.good() && sName != "Command")
			    m_ArgNames.push_back(sName);
		    else
		    {
	            SetCommandValid(false);
			    return is;
		    }
        }
    }
    else
    {
        SetCommandValid(false);
    }

	return is;
}

// Non-static function to return the type of the command

const zString cgtAddCommandToGroup::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* cgtAddCommandToGroup::GetCommand() const
{
	return new cgtAddCommandToGroup(*this);
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool cgtAddCommandToGroup::Execute(long simTime, ISimCmd* const pISimCmd) const
{
// We only allow execution of the command if the feature is enabled. Note that we
// prefix the handler function with "Internal" to distinguish it from the SimBox's
// implementation function that returns a bool.

	if(simTime == GetExecutionTime())
	{
#if EnableCommandGroups == SimCommandEnabled
		pISimCmd->AddCommandToGroup(this);
#endif
		return true;
	}
	else
		return false;
}

// Function to check that the name of the command group is valid. It must
// be alphanumeric beginning with a letter and unique amongst all command groups.
// Note that even if a group is deleted its name remains in the map, and cannot
// be used again. We also ensure that the placeholder names for the command's
// arguments are unique within each command.

bool cgtAddCommandToGroup::IsDataValid(const CInputData& riData) const
{
	// Check group name is unique and consists only of allowed characters.

	if(!riData.IsExternalNameValid(m_GroupName))
		return ErrorTrace("Invalid command group name");
	if(!riData.IsExternalNameValid(m_CommandName))
		return ErrorTrace("Invalid command name being added to group");

    for(cStringSequenceIterator citerArg=m_ArgNames.begin(); citerArg!=m_ArgNames.end(); citerArg++)
    {
	    if(!riData.IsExternalNameValid(*citerArg))
		    return ErrorTrace("Invalid argument to command being added to group");

        // Check that this argument is unique by searching all subsequent names for 
        // duplicates

        cStringSequenceIterator citerNext = citerArg;
        citerNext++;
        cStringSequenceIterator citerDup = find(citerNext, m_ArgNames.end(), *citerArg);

        if(citerDup != m_ArgNames.end())
		    return ErrorTrace("Error: placeholder names cannot be duplicated within a command being added to a group");
    }

	return true;
}

// VF that allows an empty command instance to fill up its parameters after creation.
// It is typically used by the command groups to allow the command's arguments to
// be set when each of the group's commands is instantiated. The function returns
// true if all the command's parameters are correctly assigned values.
//
// The argument contains instances of types derived from tguArgumentBase that must
// be in the order expected by the command as it tries to cast them to the types
// it expects. To avoid the user having to explicitly tell this command the 
// number of arguments to the command being added, we retrieve the number of
// arguments required by the command being instantiated from the command ACF.
// If the command class has not stored this information, it cannot be used
// in a command group, and we abort the packing of the instance's arguments.
//
// This command expects the following types (in addition to its base class types):
//
// m_GroupName     zString
// m_CommandName   zString
// m_ArgNames      StringSequence

bool cgtAddCommandToGroup::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(static_cast<long>(vArguments.size()) >= GetArgumentTotal())
    {
        vArguments.at(0)->GetValue(&m_GroupName);
        vArguments.at(1)->GetValue(&m_CommandName);

        // Now we have the command name, we can get its argument total and check
        // the remaining arguments are present.

        const long argTotal = acfCommandFactory::Instance()->GetArgumentTotal(m_CommandName);

        std::cout << "Command arguments for " << GetType() << " are: " << zEndl;
        std::cout << vArguments.at(0) << " " << vArguments.at(1) << zEndl;

        if(static_cast<long>(vArguments.size()) == argTotal+2)
        {
            // Add their placeholder names of the command's arguments
            // to the local container. This works because all of the names
            // are simple strings.

            zString newArg = "";
            for(long ib=0; ib<argTotal; ib++)
	        {
                vArguments.at(2+ib)->GetValue(&newArg);
                m_ArgNames.push_back(newArg);

                std::cout << m_ArgNames.at(ib) << " ";
	        }
            std::cout << zEndl;

            // Now check that the arguments are not duplicated. We cannot check that
            // the names are valid strings, but this has already been done when the 
            // command was read from the CDF.

            for(StringSequenceIterator iterArg=m_ArgNames.begin(); iterArg!=m_ArgNames.end(); iterArg++)
            {
                StringSequenceIterator iterNext = iterArg;
                iterNext++;
                StringSequenceIterator iterDup = find(iterNext, m_ArgNames.end(), *iterArg);

                if(iterDup != m_ArgNames.end())
		            return false;
            }

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

#endif
}
