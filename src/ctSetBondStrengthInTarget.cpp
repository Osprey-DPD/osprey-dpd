/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctSetBondStrengthInTarget.cpp: implementation of the ctSetBondStrengthInTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctSetBondStrengthInTarget.h"
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

const zString ctSetBondStrengthInTarget::m_Type = "SetBondStrengthInTarget";

const zString ctSetBondStrengthInTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctSetBondStrengthInTarget::m_ArgumentTotal = 4;

long ctSetBondStrengthInTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctSetBondStrengthInTarget(executionTime);}

	const zString id = ctSetBondStrengthInTarget::GetType();
    const long  argTotal = ctSetBondStrengthInTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctSetBondStrengthInTarget::ctSetBondStrengthInTarget(long executionTime) : ctApplyCommand(executionTime),
							m_Name(""), m_SpringConstant(0.0), m_UnstretchedLength(0.0)
{
}

ctSetBondStrengthInTarget::ctSetBondStrengthInTarget(const ctSetBondStrengthInTarget& oldCommand) : ctApplyCommand(oldCommand),
							m_Name(oldCommand.m_Name), 
							m_SpringConstant(oldCommand.m_SpringConstant), 
							m_UnstretchedLength(oldCommand.m_UnstretchedLength)

{
}

// Constructor for use when creating the command internally.

ctSetBondStrengthInTarget::ctSetBondStrengthInTarget(long executionTime, const zString target,
									 const zString name, double spring, 
									 double length) : ctApplyCommand(executionTime, target),
									m_Name(name), m_SpringConstant(spring), 
									m_UnstretchedLength(length)
{
}

ctSetBondStrengthInTarget::~ctSetBondStrengthInTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_Name					name of the target bond type
//  m_SpringConstant		spring constant for the target Hookean spring
//  m_UnstretchedLength		unstretched length of target Hooekan spring

zOutStream& ctSetBondStrengthInTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	os << "<BondName>"			<< m_Name			   << "</BondName>" << zEndl;
	os << "<SpringConstant>"	<< m_SpringConstant    << "</SpringConstant>" << zEndl;
	os << "<UnStretchedLength>" << m_UnstretchedLength << "</UnStretchedLength>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_Name << " " << m_SpringConstant << " " << m_UnstretchedLength;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctSetBondStrengthInTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Check that the spring constant and unstretched length are positive.
	// We check that the name corresponds to a valid CBond object in 
	// IsDataValid() as the CInputData object has to create the map holding 
	// the names of valid CBonds first.

	is >> m_Name >> m_SpringConstant >> m_UnstretchedLength;

	if(!is.good() || m_SpringConstant < 0.0 || m_UnstretchedLength < 0.0)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctSetBondStrengthInTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctSetBondStrengthInTarget::GetCommand() const
{
	return new ctSetBondStrengthInTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctSetBondStrengthInTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetBondStrengthInTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. 
// We check that the specified bond name is valid, that the bond type 
// exists and let the base class check the target name. The actual
// bond parameters have been checked above.

bool ctSetBondStrengthInTarget::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_Name))
		return ErrorTrace("Invalid Bond name");
	else if(!riData.IsBondinMap(m_Name))
		return ErrorTrace("Bond not found in map");
	else
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
// m_Name               - string
// m_SpringConstant     - double
// m_UnstretchedLength  - double

bool ctSetBondStrengthInTarget::Pack(const tguArgumentSequence& vArguments)
{
    if(ctApplyCommand::Pack(vArguments) && static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(1)->GetValue(&m_Name);
        vArguments.at(2)->GetValue(&m_SpringConstant);
        vArguments.at(3)->GetValue(&m_UnstretchedLength);

        if(m_SpringConstant < 0.0 || m_UnstretchedLength < 0.0)
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

