/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctSineForceOnTarget.cpp: implementation of the ctSineForceOnTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctSineForceOnTarget.h"
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

const zString ctSineForceOnTarget::m_Type = "SineForceOnTarget";

const zString ctSineForceOnTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctSineForceOnTarget::m_ArgumentTotal = 7;

long ctSineForceOnTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctSineForceOnTarget(executionTime);}

	const zString id = ctSineForceOnTarget::GetType();
    const long  argTotal = ctSineForceOnTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctSineForceOnTarget::ctSineForceOnTarget(long executionTime) : ctApplyCommand(executionTime),
										m_DecLabel(""),
										m_XN(0), m_YN(0), m_ZN(0),
										m_Amplitude(0.0), m_Period(0.0)
{
}

ctSineForceOnTarget::ctSineForceOnTarget(const ctSineForceOnTarget& oldCommand) : ctApplyCommand(oldCommand),
										m_DecLabel(oldCommand.m_DecLabel),
										m_XN(oldCommand.m_XN),
										m_YN(oldCommand.m_YN),
										m_ZN(oldCommand.m_ZN),
										m_Amplitude(oldCommand.m_Amplitude),
										m_Period(oldCommand.m_Period)
{
}

// Constructor for use when creating the command internally.

ctSineForceOnTarget::ctSineForceOnTarget(long executionTime, const zString targetLabel, const zString decLabel, 
										long xn, long yn, long zn,
										double amp, double period) : ctApplyCommand(executionTime, targetLabel),
										m_DecLabel(decLabel),
										m_XN(xn),
										m_YN(yn),
										m_ZN(zn),
										m_Amplitude(amp),
										m_Period(period)
{
}

ctSineForceOnTarget::~ctSineForceOnTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_DecLabel;		// Label for the decorator object
//	m_XN;
//	m_YN;			// Direction of force
//	m_ZN;
//	m_Amplitude;	// Amplitude of force
//	m_Period;		// Period of force in numbers of timesteps
//

zOutStream& ctSineForceOnTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	// now the derived class data
	os << "<DecoratorLabel>" << m_DecLabel << "</DecoratorLabel>" << zEndl;
	os << "<XN>" << m_XN << "</XN>" << "<YN>" << m_YN << "</YN>" << "<ZN>" << m_ZN << "</ZN>" << zEndl;
	os << "<Amplitude>" << m_Amplitude << "</Amplitude>" << zEndl;
	os << "<Period>" << m_Period << "</Period>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_DecLabel << " " << m_XN << " " << m_YN << " " << m_ZN << " " << m_Amplitude << " " << m_Period;
	putASCIIEndTags(os);
	putXMLEndTags(os);


#endif

	return os;
}

zInStream& ctSineForceOnTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Now read the local data.

	is >> m_DecLabel >> m_XN >> m_YN >> m_ZN >> m_Amplitude >> m_Period;

	if(!is.good())
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctSineForceOnTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctSineForceOnTarget::GetCommand() const
{
	return new ctSineForceOnTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctSineForceOnTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SineForceOnTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. We require that the
// amplitude of the force be non-negative, the period be at least 1.0 timesteps,
// and the direction of the force be well-defined (not all components zero). 
// Finally, we check the base class data.

bool ctSineForceOnTarget::IsDataValid(const CInputData& riData) const
{
	if((m_XN == 0 && m_YN == 0 && m_ZN == 0))
	{
		return ErrorTrace("Sine force direction cannot be (0, 0, 0)");
	}
	else if(m_Amplitude < 0.0)
	{
		return ErrorTrace("Sine force amplitude must be greater than 0.0");
	}
	else if(m_Period < 1.0)
	{
		return ErrorTrace("Sine force period must be greater than 1.0 timesteps");
	}
	else if(!riData.IsExternalNameValid(m_DecLabel))
	{
		return ErrorTrace("Sine force active command target name invalid");
	}
	else if(riData.IsActiveCommandTargetPresent(m_DecLabel))
	{
		return ErrorTrace("Sine force active command target already exists");
	}
	else
	{
		// New target is unique so add it to the container in the CInputData

		CInputData& rincData = const_cast<CInputData&>(riData);

		rincData.AddActiveCommandTargetName(m_DecLabel);

		return ctApplyCommand::IsDataValid(riData);
	}
}
