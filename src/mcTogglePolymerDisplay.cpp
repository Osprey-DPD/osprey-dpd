/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcTogglePolymerDisplay.cpp: implementation of the mcTogglePolymerDisplay class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcTogglePolymerDisplay.h"
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

const zString mcTogglePolymerDisplay::m_Type = "TogglePolymerDisplay";

const zString mcTogglePolymerDisplay::GetType()
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
	xxCommand* Create(long executionTime) {return new mcTogglePolymerDisplay(executionTime);}

	const zString id = mcTogglePolymerDisplay::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcTogglePolymerDisplay::mcTogglePolymerDisplay(long executionTime) : xxCommand(executionTime),
															   m_PolymerName("")
{
}

mcTogglePolymerDisplay::mcTogglePolymerDisplay(const mcTogglePolymerDisplay& oldCommand) : xxCommand(oldCommand),
											m_PolymerName(oldCommand.m_PolymerName)
{
}

// Constructor for use when creating the command internally.

mcTogglePolymerDisplay::mcTogglePolymerDisplay(long executionTime, 
										 const zString name) : xxCommand(executionTime),
															   m_PolymerName(name)
{
}

mcTogglePolymerDisplay::~mcTogglePolymerDisplay()
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
//	m_PolymerName		Name of polymer type to be toggled in display
//

zOutStream& mcTogglePolymerDisplay::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<PolymerName>" << m_PolymerName << "</PolymerName>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_PolymerName;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mcTogglePolymerDisplay::get(zInStream& is)
{
	is >> m_PolymerName;

	if(!is.good() || m_PolymerName.empty())
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

bool mcTogglePolymerDisplay::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->TogglePolymerDisplay(this);
		return true;
	}
	else
		return false;
}

// Non-static function to return the type of the command

const zString mcTogglePolymerDisplay::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcTogglePolymerDisplay::GetCommand() const
{
	return new mcTogglePolymerDisplay(*this);
}

// Function to check that the bead name specified in the command actually
// occurs in the bead names map created in CInputData.

bool mcTogglePolymerDisplay::IsDataValid(const CInputData &riData) const
{
	if(!riData.IsExternalNameValid(m_PolymerName))
		return ErrorTrace("Invalid polymer name");
	else if(!riData.IsPolymerinMap(m_PolymerName))
		return ErrorTrace("Polymer not found in map");

	return true;
}
