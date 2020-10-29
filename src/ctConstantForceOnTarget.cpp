/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctConstantForceOnTarget.cpp: implementation of the ctConstantForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctConstantForceOnTarget.h"
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

const zString ctConstantForceOnTarget::m_Type = "ConstantForceOnTarget";

const zString ctConstantForceOnTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctConstantForceOnTarget::m_ArgumentTotal = 6;

long ctConstantForceOnTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctConstantForceOnTarget(executionTime);}

	const zString id = ctConstantForceOnTarget::GetType();
    const long  argTotal = ctConstantForceOnTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctConstantForceOnTarget::ctConstantForceOnTarget(long executionTime) : ctApplyCommand(executionTime),
												m_DecLabel(""), m_XN(0), m_YN(0), m_ZN(1), m_Magnitude(0.0)
{
}

ctConstantForceOnTarget::ctConstantForceOnTarget(const ctConstantForceOnTarget& oldCommand) : ctApplyCommand(oldCommand),
												m_DecLabel(oldCommand.m_DecLabel),
												m_XN(oldCommand.m_XN),
												m_YN(oldCommand.m_YN),
												m_ZN(oldCommand.m_ZN),
												m_Magnitude(oldCommand.m_Magnitude)
{
}

// Constructor for use when creating the command internally.

ctConstantForceOnTarget::ctConstantForceOnTarget(long executionTime, const zString targetLabel, const zString decLabel,
												 long xn, long yn, long zn,
												 double mag) : ctApplyCommand(executionTime, targetLabel),
												m_DecLabel(decLabel),
												m_XN(xn),
												m_YN(yn),
												m_ZN(zn),
												m_Magnitude(mag)
{
}

ctConstantForceOnTarget::~ctConstantForceOnTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_DecLabel      label for this decorator object so that it can be deleted later
//  m_XN             
//  m_YN			direction of force
//  m_ZN 
//	m_Magnitude		magnitude of force applied to the beads in the target
//

zOutStream& ctConstantForceOnTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	// now the derived class data
	os << "<DecoratorLabel>" << m_DecLabel << "</DecoratorLabel>" << zEndl;
	os << "<XN>" << m_XN << "</XN>" << "<YN>" << m_YN << "</YN>" << "<ZN>" << m_ZN << "</ZN>" << zEndl;
	os << "<Magnitude>" << m_Magnitude << "</Magnitude>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_DecLabel << "  " << m_XN << " " << m_YN << " " << m_ZN << "  " << m_Magnitude;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctConstantForceOnTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Now read the local data.

	is >> m_DecLabel >> m_XN >> m_YN >> m_ZN >> m_Magnitude;

	if(!is.good())
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctConstantForceOnTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctConstantForceOnTarget::GetCommand() const
{
	return new ctConstantForceOnTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctConstantForceOnTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->ConstantForceOnTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid.
//
// We check that the direction of the force is well-defined, that the
// decorator name is unique and allowed, then we check the base class data 
// to make sure the specified command target exists and is unique.
// Because we add each target's name to the CInputData container, we have
// to cast away the const-ness to use it.

bool ctConstantForceOnTarget::IsDataValid(const CInputData& riData) const
{
	if(m_XN == 0 && m_YN == 0 && m_ZN == 0)
	{
		return ErrorTrace("Constant force active command target parameters invalid");
	}
	else if(!riData.IsExternalNameValid(m_DecLabel))
	{
		return ErrorTrace("Constant force active command target name invalid");
	}
	else if(riData.IsActiveCommandTargetPresent(m_DecLabel))
	{
		return ErrorTrace("Constant force active command target already exists");
	}
	else
	{
		// New target is unique so add it to the container in the CInputData

		CInputData& rincData = const_cast<CInputData&>(riData);

		rincData.AddActiveCommandTargetName(m_DecLabel);

		return ctApplyCommand::IsDataValid(riData);
	}
}

