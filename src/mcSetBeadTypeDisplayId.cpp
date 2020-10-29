/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetBeadTypeDisplayId.cpp: implementation of the mcSetBeadTypeDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetBeadTypeDisplayId.h"
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

const zString mcSetBeadTypeDisplayId::m_Type = "SetBeadTypeDisplayId";

const zString mcSetBeadTypeDisplayId::GetType()
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
	xxCommand* Create(long executionTime) {return new mcSetBeadTypeDisplayId(executionTime);}

	const zString id = mcSetBeadTypeDisplayId::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetBeadTypeDisplayId::mcSetBeadTypeDisplayId(long executionTime) : xxCommand(executionTime),
															   m_BeadType(-1), m_DisplayId(-1)
{
}

mcSetBeadTypeDisplayId::mcSetBeadTypeDisplayId(const mcSetBeadTypeDisplayId& oldCommand) : xxCommand(oldCommand),
											m_BeadType(oldCommand.m_BeadType),
											m_DisplayId(oldCommand.m_DisplayId)
{
}

// Constructor for use when creating the command internally.

mcSetBeadTypeDisplayId::mcSetBeadTypeDisplayId(long executionTime, 
										 long type, long displayId) : xxCommand(executionTime),
										   m_BeadType(type),
										   m_DisplayId(displayId)
{
}

mcSetBeadTypeDisplayId::~mcSetBeadTypeDisplayId()
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
//	m_BeadType		Numeric identifer of bead type whose display id is being changed
//  m_DisplayId     Numberic id to assign to all beads of the given type; this determines
//                  the colour used to display the beads in current state snapshots.

zOutStream& mcSetBeadTypeDisplayId::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<BeadType>"  << m_BeadType  << "</BeadType>" << zEndl;
	os << "<DisplayId>" << m_DisplayId << "</DisplayId>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_BeadType << " " << m_DisplayId;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mcSetBeadTypeDisplayId::get(zInStream& is)
{
	is >> m_BeadType >> m_DisplayId;

	if(!is.good() || m_BeadType < 0 || m_DisplayId < -1)
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

bool mcSetBeadTypeDisplayId::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetBeadTypeDisplayId(this);
		return true;
	}
	else
		return false;
}

// Non-static function to return the type of the command

const zString mcSetBeadTypeDisplayId::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcSetBeadTypeDisplayId::GetCommand() const
{
	return new mcSetBeadTypeDisplayId(*this);
}

// Function to check that the bead type specified in the command refers to
// a defined bead. Not implemented yet as I don't have access to the bead
// types map from CInputData.

bool mcSetBeadTypeDisplayId::IsDataValid(const CInputData &riData) const
{
	return true;
}
