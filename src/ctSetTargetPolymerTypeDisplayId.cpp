/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctSetTargetPolymerTypeDisplayId.cpp: implementation of the ctSetTargetPolymerTypeDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctSetTargetPolymerTypeDisplayId.h"
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

const zString ctSetTargetPolymerTypeDisplayId::m_Type = "SetTargetPolymerTypeDisplayId";

const zString ctSetTargetPolymerTypeDisplayId::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctSetTargetPolymerTypeDisplayId::m_ArgumentTotal = 3;

long ctSetTargetPolymerTypeDisplayId::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctSetTargetPolymerTypeDisplayId(executionTime);}

	const zString id = ctSetTargetPolymerTypeDisplayId::GetType();
    const long  argTotal = ctSetTargetPolymerTypeDisplayId::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctSetTargetPolymerTypeDisplayId::ctSetTargetPolymerTypeDisplayId(long executionTime) : ctApplyCommand(executionTime),
                                                            m_PolymerType(-1), m_DisplayId(-1)
{
}

ctSetTargetPolymerTypeDisplayId::ctSetTargetPolymerTypeDisplayId(const ctSetTargetPolymerTypeDisplayId& oldCommand) : ctApplyCommand(oldCommand),
                                           m_PolymerType(oldCommand.m_PolymerType),
                                           m_DisplayId(oldCommand.m_DisplayId)
{
}

// Constructor for use when creating the command internally.

ctSetTargetPolymerTypeDisplayId::ctSetTargetPolymerTypeDisplayId(long executionTime, const zString name,
										   long polymerType, long displayId) : ctApplyCommand(executionTime, name),
										                     m_PolymerType(polymerType),
										                     m_DisplayId(displayId)
{
}

ctSetTargetPolymerTypeDisplayId::~ctSetTargetPolymerTypeDisplayId()
{
}

// Member functions to read/write the data specific to the command.
// Note that the xxCommand base class does not provide an implementation of
// the put() and get() functions, but it does provide helper functions to
// write the start and end tags for the XML command output. The flag showing
// if the output is to be written as ASCII or XML is set in SimXMLFlags.h that is
// #included in the xxCommand.h header file, so  it is visible to all command classes.
//
// Arguments
// *********
//
//  m_PolymerType -  Numeric identifier for polymers of the type to be modified
//  m_DisplayId   -  Numeric id to assign to beads in the specified polymer type 
//                   in the target; this determines the colour used to display beads 
//                   in the specified polymers in the target in current state snapshots.
//                   All bead types in the polymers have the same id and colour.

zOutStream& ctSetTargetPolymerTypeDisplayId::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	ctApplyCommand::put(os);
	os << "<PolymerType>" << m_PolymerType << "</PolymerType>" << zEndl;
	os << "<DisplayId>"   << m_DisplayId << "</DisplayId>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
    os << m_PolymerType << "  " << m_DisplayId;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctSetTargetPolymerTypeDisplayId::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	is >> m_PolymerType >> m_DisplayId;

	if(!is.good() || m_PolymerType < 0 || m_DisplayId < -1)
		SetCommandValid(false);

	return is;
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 
//
// Note that even though this command is destined for the IMonitor interface, we
// have to pass it to the ISimCmd interface first because it will be checked for
// execution in the CSimBox's command loop and then passed on to the CMonitor.

bool ctSetTargetPolymerTypeDisplayId::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetTargetPolymerTypeDisplayId(this);
		return true;
	}
	else
		return false;
}

// Non-static function to return the type of the command

const zString ctSetTargetPolymerTypeDisplayId::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctSetTargetPolymerTypeDisplayId::GetCommand() const
{
	return new ctSetTargetPolymerTypeDisplayId(*this);
}

// Function to check that the command data are valid. The only data are the name
// of a command target, which is validated in the base class; and a numeric bead
// identifier and a numeric id, both of which must be non-negative integers and this
// is checked above.

bool ctSetTargetPolymerTypeDisplayId::IsDataValid(const CInputData &riData) const
{
	return ctApplyCommand::IsDataValid(riData);
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
// m_TargetLabel  - zString
// m_PolymerType  - long
// m_DisplayId    - long

bool ctSetTargetPolymerTypeDisplayId::Pack(const tguArgumentSequence& vArguments)
{
    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        ctApplyCommand::Pack(vArguments);
        vArguments.at(1)->GetValue(&m_PolymerType);
        vArguments.at(2)->GetValue(&m_DisplayId);

        return true;
    }
    else
    {
        return false;
    }
}

