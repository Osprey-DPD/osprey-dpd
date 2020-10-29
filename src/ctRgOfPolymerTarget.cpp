/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctRgOfPolymerTarget.cpp: implementation of the ctRgOfPolymerTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctRgOfPolymerTarget.h"
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

const zString ctRgOfPolymerTarget::m_Type = "RgOfPolymerTarget";

const zString ctRgOfPolymerTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctRgOfPolymerTarget::m_ArgumentTotal = 4;

long ctRgOfPolymerTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctRgOfPolymerTarget(executionTime);}

	const zString id = ctRgOfPolymerTarget::GetType();
    const long  argTotal = ctRgOfPolymerTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctRgOfPolymerTarget::ctRgOfPolymerTarget(long executionTime) : ctApplyCommand(executionTime),
												m_DecLabel(""), m_Start(0), m_End(0)
{
}

ctRgOfPolymerTarget::ctRgOfPolymerTarget(const ctRgOfPolymerTarget& oldCommand) : ctApplyCommand(oldCommand),
												m_DecLabel(oldCommand.m_DecLabel),
												m_Start(oldCommand.m_Start),
												m_End(oldCommand.m_End)
{
}

// Constructor for use when creating the command internally.

ctRgOfPolymerTarget::ctRgOfPolymerTarget(long executionTime, const zString targetLabel, 
											   const zString decLabel, 
											   long start, long end) : ctApplyCommand(executionTime, targetLabel),
												m_DecLabel(decLabel),
												m_Start(start),
												m_End(end)
{
}

ctRgOfPolymerTarget::~ctRgOfPolymerTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_DecLabel      label for this decorator object so that it can be deleted later
//	m_Start			simulation time to start calculating the MSD of the target
//	m_End				"		"      end        "

zOutStream& ctRgOfPolymerTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	ctApplyCommand::put(os);
	os << "<DecoratorLabel>" << m_DecLabel   << "</DecoratorLabel>" << zEndl;
	os << "<StartTime>"      << m_Start      << "</StartTime>"      << zEndl;
	os << "<EndTime>"        << m_End        << "</EndTime>"        << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_DecLabel << " " << m_Start << " " << m_End;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctRgOfPolymerTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Now read the local data.

	is >> m_DecLabel >> m_Start >> m_End;
    
	if(!is.good() || m_Start < 0 || m_End < m_Start)
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctRgOfPolymerTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctRgOfPolymerTarget::GetCommand() const
{
	return new ctRgOfPolymerTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctRgOfPolymerTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->RgPolymerTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid.
//
// We check that the decorator name is unique and allowed, then we check the base class data 
// to make sure the specified command target exists and is unique.
// Because we add each target's name to the CInputData container, we have
// to cast away the const-ness to use it.

bool ctRgOfPolymerTarget::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_DecLabel))
	{
		return ErrorTrace("Polymer Rg active command target name invalid");
	}
	else if(riData.IsActiveCommandTargetPresent(m_DecLabel))
	{
		return ErrorTrace("Polymer Rg active command target already exists");
	}
	else
	{
		// New target is unique so add it to the container in the CInputData

		CInputData& rincData = const_cast<CInputData&>(riData);

		rincData.AddActiveCommandTargetName(m_DecLabel);

		return ctApplyCommand::IsDataValid(riData);
	}
}

