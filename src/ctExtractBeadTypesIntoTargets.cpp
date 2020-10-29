/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctExtractBeadTypesIntoTargets.cpp: implementation of the ctExtractBeadTypesIntoTargets class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctExtractBeadTypesIntoTargets.h"
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

const zString ctExtractBeadTypesIntoTargets::m_Type = "ExtractBeadTypesIntoTargets";

const zString ctExtractBeadTypesIntoTargets::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctExtractBeadTypesIntoTargets::m_ArgumentTotal = 2;

long ctExtractBeadTypesIntoTargets::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctExtractBeadTypesIntoTargets(executionTime);}

	const zString id = ctExtractBeadTypesIntoTargets::GetType();
    const long  argTotal = ctExtractBeadTypesIntoTargets::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctExtractBeadTypesIntoTargets::ctExtractBeadTypesIntoTargets(long executionTime) : ctApplyCommand(executionTime),
															m_DestLabel("")
{
}

// Copy constructor

ctExtractBeadTypesIntoTargets::ctExtractBeadTypesIntoTargets(const ctExtractBeadTypesIntoTargets& oldCommand) : ctApplyCommand(oldCommand),
															m_DestLabel(oldCommand.m_DestLabel)
{
}

// Constructor for use when creating the command internally.

ctExtractBeadTypesIntoTargets::ctExtractBeadTypesIntoTargets(long executionTime, const zString target, const zString destLabel) : ctApplyCommand(executionTime, target),
															m_DestLabel(destLabel)
{
}

ctExtractBeadTypesIntoTargets::~ctExtractBeadTypesIntoTargets()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_DestLabel     root name of the destination target(s)
//

zOutStream& ctExtractBeadTypesIntoTargets::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	ctApplyCommand::put(os);
	os << "<DestinationLabel>" << m_DestLabel   << "</DestinationLabel>"  << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_DestLabel;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctExtractBeadTypesIntoTargets::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Now read the local data.

	is >> m_DestLabel;

	if(!is.good() || m_DestLabel.empty())
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctExtractBeadTypesIntoTargets::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctExtractBeadTypesIntoTargets::GetCommand() const
{
	return new ctExtractBeadTypesIntoTargets(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctExtractBeadTypesIntoTargets::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->ExtractBeadTypesIntoTargets(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. We check that the root name
// of the destination targets is valid, and form the names of the new targets
// by appending the name and numeric identifier of each bead type to it.
// We provide a check against the simple error of using the same label for the 
// source and destination targets, but cannot check the destination target names
// without knowing the bead types that are going to be inserted into the 
// targets. Similarly, we cannot add the names of the destination targets 
// to the target list yet.
// NB. Although we could in principle allow the source and destination names to
// be the same, because the bead name/type will distinguish them, if the user 
// issues the command twice by cut-and-paste and forgets to alter the second
// destination name,  there is the chance of a duplicate target name. Hence, 
// we use the principle of least surprise to exclude this trivial case.

bool ctExtractBeadTypesIntoTargets::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_DestLabel))
	{
		return ErrorTrace("Extract targets destination root name invalid");
	}
	else if(m_DestLabel == GetTargetLabel())
	{
		return ErrorTrace("Extract targets source and destination names identical");
	}

	return ctApplyCommand::IsDataValid(riData);
}
