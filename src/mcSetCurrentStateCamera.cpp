/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetCurrentStateCamera.cpp: implementation of the mcSetCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetCurrentStateCamera.h"
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

const zString mcSetCurrentStateCamera::m_Type = "SetCurrentStateCamera";

const zString mcSetCurrentStateCamera::GetType()
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
	xxCommand* Create(long executionTime) {return new mcSetCurrentStateCamera(executionTime);}

	const zString id = mcSetCurrentStateCamera::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetCurrentStateCamera::mcSetCurrentStateCamera(long executionTime) : xxCommand(executionTime)
{
	for(short int i=0; i<3; i++)
	{
		m_Camera[i]	= 0.0;
		m_Target[i]	= 0.0;
	}
}

mcSetCurrentStateCamera::mcSetCurrentStateCamera(const mcSetCurrentStateCamera& oldCommand) : xxCommand(oldCommand)
{
	for(short int i=0; i<3; i++)
	{
		m_Camera[i]	= oldCommand.m_Camera[i];
		m_Target[i]	= oldCommand.m_Target[i];
	}
}

// Constructor for use when executing the command internally. We use a flag 
// to indicate if the command's execution should be logged or not.

mcSetCurrentStateCamera::mcSetCurrentStateCamera(long executionTime, bool bLog,
												 double camera[3], double target[3]) : xxCommand(executionTime, bLog)
{
	for(short int i=0; i<3; i++)
	{
		m_Camera[i]	= camera[i];
		m_Target[i]	= target[i];
	}
}


mcSetCurrentStateCamera::~mcSetCurrentStateCamera()
{
}

// Member functions to write/read the data specific to the command.

zOutStream& mcSetCurrentStateCamera::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<CameraX>" << m_Camera[0] << "</CameraX>" << zEndl;
	os << "<CameraY>" << m_Camera[1] << "</CameraY>" << zEndl;
	os << "<CameraZ>" << m_Camera[2] << "</CameraZ>" << zEndl;
	os << "<TargetX>" << m_Target[0] << "</TargetX>" << zEndl;
	os << "<TargetY>" << m_Target[1] << "</TargetY>" << zEndl;
	os << "<TargetZ>" << m_Target[2] << "</TargetZ>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_Camera[0] << " " << m_Camera[1] << " " << m_Camera[2] << " " 
	   << m_Target[0] << " " << m_Target[1] << " " << m_Target[2];
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mcSetCurrentStateCamera::get(zInStream& is)
{
	double blxc, blyc, blzc, trxc, tryc, trzc;

	is >> blxc >> blyc >> blzc >> trxc >> tryc >> trzc;

	if(!is.good() || ((blxc == trxc) && (blyc == tryc) && (blzc == trzc)))
		SetCommandValid(false);

	m_Camera[0] = blxc;
	m_Camera[1] = blyc;
	m_Camera[2] = blzc;

	m_Target[0] = trxc;
	m_Target[1] = tryc;
	m_Target[2] = trzc;

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

bool mcSetCurrentStateCamera::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetCurrentStateCamera(this);
		return true;
	}
	else
		return false;
}

// Non-static function to return the type of the command

const zString mcSetCurrentStateCamera::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcSetCurrentStateCamera::GetCommand() const
{
	return new mcSetCurrentStateCamera(*this);
}

// Function to check that data for the command is valid. The only constraint
// on the bounding box data is that its coordinates should be non-zero and the
// upper-right corner coordinates be greater than the lower-left coordinates:
// this has been checked in the get() function above.

bool mcSetCurrentStateCamera::IsDataValid(const CInputData &riData) const
{
	return true;
}
