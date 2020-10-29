/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcPanCurrentStateCamera.cpp: implementation of the mcPanCurrentStateCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMPSFlags.h"
#include "mcPanCurrentStateCamera.h"
#include "ISimCmd.h"
#include "InputData.h"
#include "xxParallelBase.h"   // Needed to check for processor rank

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString mcPanCurrentStateCamera::m_Type = "PanCurrentStateCamera";

const zString mcPanCurrentStateCamera::GetType()
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
	xxCommand* Create(long executionTime) {return new mcPanCurrentStateCamera(executionTime);}

	const zString id = mcPanCurrentStateCamera::GetType();

#if EnableMonitorCommand == SimCommandEnabled
	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
#endif
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcPanCurrentStateCamera::mcPanCurrentStateCamera(long executionTime) : xxCommand(executionTime),
																		m_bInitial(true),
																		m_bFinal(true),
																		m_Duration(0),
																		m_FrameRate(0),
																		m_StepsPerFrame(0),
																		m_PanAngle(0.0)
{
	for(long i=0; i<3; i++)
	{
		m_Normal[i]	= 0;
		m_Camera[i] = 0.0;
		m_Target[i]	= 0.0;
	}
}

// Copy constructor

mcPanCurrentStateCamera::mcPanCurrentStateCamera(const mcPanCurrentStateCamera& oldCommand) : xxCommand(oldCommand),
																		m_bInitial(oldCommand.m_bInitial),
																		m_bFinal(oldCommand.m_bFinal),
																		m_Duration(oldCommand.m_Duration),
																		m_FrameRate(oldCommand.m_FrameRate),
																		m_StepsPerFrame(oldCommand.m_StepsPerFrame),
																		m_PanAngle(oldCommand.m_PanAngle)
{
	for(long i=0; i<3; i++)
	{
		m_Normal[i]	= oldCommand.m_Normal[i];
		m_Camera[i] = oldCommand.m_Camera[i];
		m_Target[i]	= oldCommand.m_Target[i];
	}
}

// Constructor used when creating the command internally. Note that we assume that the pan angle is already in radians.

mcPanCurrentStateCamera::mcPanCurrentStateCamera(long executionTime, bool bLog, bool bInitial, bool bFinal, long duration, long frameRate, long stepsPerFrame, double panAngle,
	                        long normal[3], double camera[3], double target[3]) : xxCommand(executionTime, bLog),
																		m_bInitial(bInitial),
																		m_bFinal(bFinal),
																		m_Duration(duration),
																		m_FrameRate(frameRate),
																		m_StepsPerFrame(stepsPerFrame),
																		m_PanAngle(panAngle)
{
	for(long i=0; i<3; i++)
	{
		m_Normal[i]	= normal[i];
		m_Camera[i] = camera[i];
		m_Target[i]	= target[i];		
	}							
}

mcPanCurrentStateCamera::~mcPanCurrentStateCamera()
{
}

// Member functions to write/read the data specific to the command.

zOutStream& mcPanCurrentStateCamera::put(zOutStream& os) const
{
#if EnableMonitorCommand == SimCommandEnabled
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<CameraX>" << m_Camera[0] << "</CameraX>" << zEndl;
	os << "<CameraY>" << m_Camera[1] << "</CameraY>" << zEndl;
	os << "<CameraZ>" << m_Camera[2] << "</CameraZ>" << zEndl;
	os << "<TargetX>" << m_Target[0] << "</TargetX>" << zEndl;
	os << "<TargetY>" << m_Target[1] << "</TargetY>" << zEndl;
	os << "<TargetZ>" << m_Target[2] << "</TargetZ>" << zEndl;
	os << "<NX>" << m_Normal[0] << "</NX>" << zEndl;
	os << "<NY>" << m_Normal[1] << "</NY>" << zEndl;
	os << "<NZ>" << m_Normal[2] << "</NZ>" << zEndl;
	os << "<PanAngle>" << m_PanAngle*180.0/xxBase::m_globalPI << "</PanAngle>" << zEndl;
	os << "<Duration>"			<< m_Duration		<< "</Duration>"		  << zEndl;
	os << "<FrameRate>"			<< m_FrameRate		<< "</FrameRate>"		  << zEndl;
	os << "<StepsPerFrame>"		<< m_StepsPerFrame  << "</StepsPerFrame>"	  << zEndl;
	os << "<JumpToInitial>"	<< m_bInitial		<< "</JumpToInitial>" << zEndl;
	os << "<StayAtFinal>"	<< m_bFinal			<< "</StayAtFinal>"	  << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_Camera[0] << " " << m_Camera[1] << " " << m_Camera[2] << "  ";
	os << m_Target[0] << " " << m_Target[1] << " " << m_Target[2] << "  ";
	os << m_Normal[0] << " " << m_Normal[1] << " " << m_Normal[2] << "  ";
	os << m_PanAngle*180.0/xxBase::m_globalPI << "  ";
	os << m_Duration  << " " << m_FrameRate  << " " << m_StepsPerFrame << "  ";
	os << m_bInitial  << " " << m_bFinal << zEndl;
	putASCIIEndTags(os);

#endif
#endif
	return os;
}

zInStream& mcPanCurrentStateCamera::get(zInStream& is)
{
#if EnableMonitorCommand == SimCommandEnabled
	is >> m_Camera[0] >> m_Camera[1] >> m_Camera[2];

	if(!is.good())
		SetCommandValid(false);

	is >> m_Target[0] >> m_Target[1] >> m_Target[2];

	if(!is.good() || (m_Camera[0] == m_Target[0] &&
					  m_Camera[1] == m_Target[1] &&
					  m_Camera[2] == m_Target[2]))
		SetCommandValid(false);

	is >> m_Normal[0] >> m_Normal[1] >> m_Normal[2];

	if(!is.good() || (m_Normal[0] == 0 && m_Normal[1] == 0 && m_Normal[2] == 0))
		SetCommandValid(false);

	// Note that the pan can be greater than 360 degrees or negative

	is >> m_PanAngle;

	if(!is.good())
		SetCommandValid(false);

	m_PanAngle	= m_PanAngle*xxBase::m_globalPI/180.0;

	is >> m_Duration >> m_FrameRate >> m_StepsPerFrame >> m_bInitial >> m_bFinal;

	if(!is.good() || m_Duration < 1 || m_FrameRate < 1 || m_StepsPerFrame < 1)
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
// have to pass it to the ISimCmd interface first because it will be checked for
// execution in the CSimBox's command loop and then passed on to the CMonitor.

// Because only P0 needs to implement this command in the parallel codebase, we conditionally compile
// the execution statement.

bool mcPanCurrentStateCamera::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
#if EnableMonitorCommand == SimCommandEnabled

#if EnableParallelCommands == SimMPSEnabled		

		    pISimCmd->PanCurrentStateCamera(this);
#else
		pISimCmd->PanCurrentStateCamera(this);
#endif

#endif
		return true;
	}
	else
		return false;
}

// Non-static function to return the type of the command

const zString mcPanCurrentStateCamera::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcPanCurrentStateCamera::GetCommand() const
{
	return new mcPanCurrentStateCamera(*this);
}

// Function to check that data for the command is valid.  Because we have
// checked the simple erros in the get() function above, there is nothing
// to do here.

bool mcPanCurrentStateCamera::IsDataValid(const CInputData &riData) const
{
    if(m_Normal[0] == 0 && m_Normal[1] == 0 && m_Normal[2] == 0)
	{
		return ErrorTrace("Invalid (zero) normal vector for camera pan command");
	}
	else if(m_Camera[0] == m_Target[0] && m_Camera[1] == m_Target[1] && m_Camera[2] == m_Target[2])
	{
		return ErrorTrace("Invalid (identical) camera and target vectors for camera pan command");
	}
	else if(m_Duration < 1 || m_FrameRate < 1 || m_StepsPerFrame < 1)
	{
		return ErrorTrace("Invalid duration, frameRate or stepsPerFrame parameters for camera pan command");
	}
	
	return true;
}
