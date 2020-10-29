/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcToggleEnergyOutput.cpp: implementation of the mcToggleEnergyOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcToggleEnergyOutput.h"
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

const zString mcToggleEnergyOutput::m_Type = "ToggleEnergyOutput";

const zString mcToggleEnergyOutput::GetType()
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
	xxCommand* Create(long executionTime) {return new mcToggleEnergyOutput(executionTime);}

	const zString id = mcToggleEnergyOutput::GetType();

#if EnableMonitorCommand == SimCommandEnabled
	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
#endif
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcToggleEnergyOutput::mcToggleEnergyOutput(long executionTime) : xxCommand(executionTime),
																 m_bNormalizePerBead(false)
{
}

mcToggleEnergyOutput::mcToggleEnergyOutput(const mcToggleEnergyOutput& oldCommand) : xxCommand(oldCommand),
											m_bNormalizePerBead(oldCommand.m_bNormalizePerBead)
{
}

mcToggleEnergyOutput::~mcToggleEnergyOutput()
{
}

// Member functions to write/read the data specific to the command.
// The sole datum for this command is a flag showing whether the energy terms
// (KE, PE, total energy) are normalized by the total number of beads or not.

zOutStream& mcToggleEnergyOutput::put(zOutStream& os) const
{
#if EnableMonitorCommand == SimCommandEnabled
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<NormalizePerBead>" << m_bNormalizePerBead << "</NormalizePerBead>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_bNormalizePerBead;
	putASCIIEndTags(os);

#endif
#endif

	return os;
}

zInStream& mcToggleEnergyOutput::get(zInStream& is)
{
#if EnableMonitorCommand == SimCommandEnabled
	is >> m_bNormalizePerBead;

	if(!is.good())
		SetCommandValid(false);
#endif

	return is;
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 
//
// Note that even though this command is destined for the IMonitor interface, we
// have to pass it to the ISimCmd interface (and thence to its ISimBoxBase base class)
// first because it will be checked for execution in the CSimBox's command loop 
// and then passed on to the CMonitor.

bool mcToggleEnergyOutput::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
#if EnableMonitorCommand == SimCommandEnabled
		pISimCmd->ToggleEnergyOutput(this);
#endif
		return true;
	}
	else
		return false;
}

// Non-static function to return the type of the command

const zString mcToggleEnergyOutput::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcToggleEnergyOutput::GetCommand() const
{
	return new mcToggleEnergyOutput(*this);
}

// Function to check the command is valid: because there is only a boolean
// flag, and we check above that it is read correctly, we just return true.

bool mcToggleEnergyOutput::IsDataValid(const CInputData &riData) const
{
	return true;
}

// Function to return the flag indicating whether to normalize the energies or not.

bool mcToggleEnergyOutput::NormalizePerBead() const
{
	return m_bNormalizePerBead;
}
