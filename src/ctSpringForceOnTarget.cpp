/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctSpringForceOnTarget.cpp: implementation of the ctSpringForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctSpringForceOnTarget.h"
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

const zString ctSpringForceOnTarget::m_Type = "SpringForceOnTarget";

const zString ctSpringForceOnTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctSpringForceOnTarget::m_ArgumentTotal = 6;

long ctSpringForceOnTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctSpringForceOnTarget(executionTime);}

	const zString id = ctSpringForceOnTarget::GetType();
    const long  argTotal = ctSpringForceOnTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctSpringForceOnTarget::ctSpringForceOnTarget(long executionTime) : ctApplyCommand(executionTime),
											m_DecLabel(""), m_XC(0.0), m_YC(0.0), m_ZC(0.0), m_keff(0.0)
{
}

ctSpringForceOnTarget::ctSpringForceOnTarget(const ctSpringForceOnTarget& oldCommand) : ctApplyCommand(oldCommand),
												m_DecLabel(oldCommand.m_DecLabel),
												m_XC(oldCommand.m_XC),
												m_YC(oldCommand.m_YC),
												m_ZC(oldCommand.m_ZC),
												m_keff(oldCommand.m_keff)
{
}

// Constructor for use when creating the command internally.

ctSpringForceOnTarget::ctSpringForceOnTarget(long executionTime, const zString targetLabel, const zString decLabel, 
											double xc, double yc, double zc, double magnitude) : ctApplyCommand(executionTime, targetLabel),
											m_DecLabel(decLabel), m_XC(xc), m_YC(yc), m_ZC(zc), m_keff(magnitude)
{
}

ctSpringForceOnTarget::~ctSpringForceOnTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_DecLabel      label for this decorator object so that it can be deleted later
//	m_XC
//	m_YC			Point in simulation box to which polymers in the target will be restrained 
//	m_ZC
//	m_keff			magnitude of spring force applied to the beads in the target
//

zOutStream& ctSpringForceOnTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	os << "<DecoratorLabel>" << m_DecLabel << "</DecoratorLabel>" << zEndl;
	os << "<XC>" << m_XC << "</XC>" << "<YC>" << m_YC << "</YC>" << "<ZC>" << m_ZC << "</ZC>" << zEndl;
	os << "<Keff>" << m_keff << "</Keff>" << zEndl;

	// now the derived class data - except there isn't any for this command
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_DecLabel << "  " << m_XC << " " << m_YC << " " << m_ZC << "  " << m_keff;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctSpringForceOnTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);
	
	// Now read the local data.

	is >> m_DecLabel >> m_XC >> m_YC >> m_ZC >> m_keff;
	
	if(!is.good())
		SetCommandValid(false);
		
	return is;
}

// Non-static function to return the type of the command

const zString ctSpringForceOnTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctSpringForceOnTarget::GetCommand() const
{
	return new ctSpringForceOnTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctSpringForceOnTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SpringForceOnTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. Because no data 
// is required by this command except a valid command target, and the
// check for validity is performed in the base class, it just returns 
// the base class result.

bool ctSpringForceOnTarget::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_DecLabel))
	{
		return ErrorTrace("Spring force active command target name invalid");
	}
	else if(riData.IsActiveCommandTargetPresent(m_DecLabel))
	{
		return ErrorTrace("Spring force active command target already exists");
	}
	else if(m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
	        m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
	{
		return ErrorTrace("Spring force active command target invalid origin");
	}
	else if(m_keff < 0.0)
	{
		return ErrorTrace("Spring force active command target invalid magnitude");
	}
	else
	{
		// New target is unique so add it to the container in the CInputData

		CInputData& rincData = const_cast<CInputData&>(riData);

		rincData.AddActiveCommandTargetName(m_DecLabel);

		return ctApplyCommand::IsDataValid(riData);
	}
}
