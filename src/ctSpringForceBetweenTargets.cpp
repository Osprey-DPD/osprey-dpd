/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctSpringForceBetweenTargets.cpp: implementation of the ctSpringForceBetweenTargets class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctSpringForceBetweenTargets.h"
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

const zString ctSpringForceBetweenTargets::m_Type = "SpringForceBetweenTargets";

const zString ctSpringForceBetweenTargets::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctSpringForceBetweenTargets::m_ArgumentTotal = 6;

long ctSpringForceBetweenTargets::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctSpringForceBetweenTargets(executionTime);}

	const zString id = ctSpringForceBetweenTargets::GetType();
    const long  argTotal = ctSpringForceBetweenTargets::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctSpringForceBetweenTargets::ctSpringForceBetweenTargets(long executionTime) : ctApplyCommand(executionTime),
											m_Target2Label(""), m_DecLabel(""), m_Keff(0.0), m_DeltaX(0.0), m_Period(0)
{
}

ctSpringForceBetweenTargets::ctSpringForceBetweenTargets(const ctSpringForceBetweenTargets& oldCommand) : ctApplyCommand(oldCommand),
                                                m_Target2Label(oldCommand.m_Target2Label),
                                                m_DecLabel(oldCommand.m_DecLabel),
                                                m_Keff(oldCommand.m_Keff),
                                                m_DeltaX(oldCommand.m_DeltaX),
                                                m_Period(oldCommand.m_Period)
{
}

// Constructor for use when creating the command internally.

ctSpringForceBetweenTargets::ctSpringForceBetweenTargets(long executionTime, const zString target1Label, const zString target2Label, const zString decLabel, 
											double magnitude, double deltax, long period) : ctApplyCommand(executionTime, target1Label),
											m_Target2Label(target2Label), m_DecLabel(decLabel), m_Keff(magnitude), m_DeltaX(deltax), m_Period(period)
{
}

ctSpringForceBetweenTargets::~ctSpringForceBetweenTargets()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_Target2Label  label for the second command target instance that has the force applied to it
//  m_DecLabel      label for this decorator object so that it can be deleted later
//	m_Keff			magnitude of spring force applied to the beads in the target (> 0)
//  m_DeltaX        amount by which the target separation is increase every "m_Period" time steps (negative and zero allowed)
//  m_Period        numbr of time steps between incrementing the separation (> 0)

zOutStream& ctSpringForceBetweenTargets::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	os << "<TargetLabelLabel>" << m_Target2Label << "</TargetLabelLabel>" << zEndl;
	os << "<DecoratorLabel>" << m_DecLabel << "</DecoratorLabel>" << zEndl;
	os << "<Keff>"   << m_Keff   << "</Keff>"   << zEndl;
	os << "<DeltaX>" << m_DeltaX << "</DeltaX>" << zEndl;
	os << "<Period>" << m_Period << "</Period>" << zEndl;

	// now the derived class data - except there isn't any for this command
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_Target2Label << "  " << m_DecLabel << "  " << "  " << m_Keff << " " << m_DeltaX << " " << m_Period;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctSpringForceBetweenTargets::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);
	
	// Now read the local data.

	is >> m_Target2Label >> m_DecLabel >> m_Keff >> m_DeltaX >> m_Period;
    
	if(!is.good())
		SetCommandValid(false);
		
	return is;
}

// Non-static function to return the type of the command

const zString ctSpringForceBetweenTargets::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctSpringForceBetweenTargets::GetCommand() const
{
	return new ctSpringForceBetweenTargets(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctSpringForceBetweenTargets::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SpringForceBetweenTargets(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. 

bool ctSpringForceBetweenTargets::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_DecLabel))
	{
		return ErrorTrace("Spring force between targets active command target name invalid");
	}
	else if(riData.IsActiveCommandTargetPresent(m_DecLabel))
	{
		return ErrorTrace("Spring force between targets active command target already exists");
	}
	if(!riData.IsExternalNameValid(m_Target2Label))
	{
		return ErrorTrace("Spring force between targets second command target name invalid");
	}
	else if(m_Keff < 0.0)
	{
		return ErrorTrace("Spring force between targets active command target invalid magnitude");
	}
	else if(m_Period < 1)
	{
		return ErrorTrace("Spring force between targets active command target invalid periodn");
	}
	else
	{
		// New target is unique so add it to the container in the CInputData

		CInputData& rincData = const_cast<CInputData&>(riData);

		rincData.AddActiveCommandTargetName(m_DecLabel);

		return ctApplyCommand::IsDataValid(riData);
	}
}
