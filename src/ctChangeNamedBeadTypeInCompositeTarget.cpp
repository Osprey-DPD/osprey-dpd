/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctChangeNamedBeadTypeInCompositeTarget.cpp: implementation of the ctChangeNamedBeadTypeInCompositeTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctChangeNamedBeadTypeInCompositeTarget.h"
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

const zString ctChangeNamedBeadTypeInCompositeTarget::m_Type = "ChangeNamedBeadTypeInCompositeTarget";

const zString ctChangeNamedBeadTypeInCompositeTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctChangeNamedBeadTypeInCompositeTarget::m_ArgumentTotal = 2;

long ctChangeNamedBeadTypeInCompositeTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctChangeNamedBeadTypeInCompositeTarget(executionTime);}

	const zString id = ctChangeNamedBeadTypeInCompositeTarget::GetType();
    const long  argTotal = ctChangeNamedBeadTypeInCompositeTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctChangeNamedBeadTypeInCompositeTarget::ctChangeNamedBeadTypeInCompositeTarget(long executionTime) : ctApplyCommand(executionTime),
                                             m_NewBeadName("")
{
}

ctChangeNamedBeadTypeInCompositeTarget::ctChangeNamedBeadTypeInCompositeTarget(const ctChangeNamedBeadTypeInCompositeTarget& oldCommand) : ctApplyCommand(oldCommand),
                                             m_NewBeadName(oldCommand.m_NewBeadName)
{
}

// Constructor for use when creating the command internally.

ctChangeNamedBeadTypeInCompositeTarget::ctChangeNamedBeadTypeInCompositeTarget(long executionTime, const zString target,
											 const zString newBeadName) : ctApplyCommand(executionTime, target),
											 m_NewBeadName(newBeadName)
{
}

ctChangeNamedBeadTypeInCompositeTarget::~ctChangeNamedBeadTypeInCompositeTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_NewBeadName  new string identifier for the command's beads
//

zOutStream& ctChangeNamedBeadTypeInCompositeTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	// now the derived class data
	os << "<BeadName>" << m_NewBeadName << "</BeadName>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_NewBeadName << " ";
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctChangeNamedBeadTypeInCompositeTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	is >> m_NewBeadName;

	if(!is.good() || m_NewBeadName.empty())
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctChangeNamedBeadTypeInCompositeTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctChangeNamedBeadTypeInCompositeTarget::GetCommand() const
{
	return new ctChangeNamedBeadTypeInCompositeTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctChangeNamedBeadTypeInCompositeTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->ChangeNamedBeadTypeInCompositeTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. We ensure that the new
// bead name is valid but do not add it to the CInputData's map. We let the
// command implementation check for the uniqueness of the bead names. This
// avoids a conflict between the validation of commands that occur in the 
// CDF and those that are sent to a running simulation.

bool ctChangeNamedBeadTypeInCompositeTarget::IsDataValid(const CInputData& riData) const
{
	if(!ctApplyCommand::IsDataValid(riData) || !riData.IsExternalNameValid(m_NewBeadName))
    {
		return ErrorTrace("Invalid new bead name for composite target beads");
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
// it expects.
//
// This command expects the following types (in addition to the base class data):
//
// m_NewBeadName - zString

bool ctChangeNamedBeadTypeInCompositeTarget::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(ctApplyCommand::Pack(vArguments) && static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(1)->GetValue(&m_NewBeadName);

        if(!CInputData::GlobalIsExternalNameValid(m_NewBeadName))
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }

#else
    return false;
#endif
}
