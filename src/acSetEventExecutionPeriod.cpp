/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acSetEventExecutionPeriod.cpp: implementation of the acSetEventExecutionPeriod class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acSetEventExecutionPeriod.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "aeActiveSimBox.h"
#include "aevActiveEvent.h"
#include "IModifyActiveCellNetwork.h"
#include "LogacSetEventExecutionPeriod.h"
#include "LogacNetworkNotAccessible.h"
#include "LogacNetworkNotModifiable.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString acSetEventExecutionPeriod::m_Type = "SetEventExecutionPeriod";

const zString acSetEventExecutionPeriod::GetType()
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
	xxCommand* Create(long executionTime) {return new acSetEventExecutionPeriod(executionTime);}

	const zString id = acSetEventExecutionPeriod::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acSetEventExecutionPeriod::acSetEventExecutionPeriod(long executionTime) : xxCommand(executionTime),
														m_ACNType(""),
														m_EventType(""),
														m_Period(0)
{
}

acSetEventExecutionPeriod::acSetEventExecutionPeriod(const acSetEventExecutionPeriod& oldCommand) : xxCommand(oldCommand),
								   m_ACNType(oldCommand.m_ACNType),
								   m_EventType(oldCommand.m_EventType),
								   m_Period(oldCommand.m_Period)

{
}


acSetEventExecutionPeriod::~acSetEventExecutionPeriod()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_ACNType   - Name of the active cell network whose event is to be modified
//  m_EventType - Name of the event to modify
//	m_Period	- Period with which to check the event's execution
//

zOutStream& acSetEventExecutionPeriod::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<ActiveCellNetwork>" << m_ACNType    << "</ActiveCellNetwork>" << zEndl;
	os << "<ActiveEvent>"       << m_EventType  << "</ActiveEvent>" << zEndl;
	os << "<Period>"            << m_Period     << "</Period>"		 << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_ACNType << " " << m_EventType << " " << m_Period;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acSetEventExecutionPeriod::get(zInStream& is)
{
	// We check that the ACN and event names are not null and that the specified 
	// period is a non-negative integer.

	is >> m_ACNType >> m_EventType >> m_Period;

	if(!is.good() || m_ACNType.empty()  || m_EventType.empty() || m_Period < 0)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString acSetEventExecutionPeriod::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* acSetEventExecutionPeriod::GetCommand() const
{
	return new acSetEventExecutionPeriod(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool acSetEventExecutionPeriod::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		IActiveSimBox* pIASB = pISimCmd->GetISimBox()->IIActiveSimBox();

		IModifyActiveCellNetwork* const pMACN = pIASB->GetIModifyActiveCellNetworkFromType(m_ACNType);

		if(pMACN)
		{
            pMACN->SetEventExecutionPeriod(m_EventType, m_Period);

			 new CLogacSetEventExecutionPeriod(GetExecutionTime(), m_ACNType, 
																			  m_EventType, m_Period);
		}
		else
		{
			 new CLogacNetworkNotModifiable(GetExecutionTime(), m_ACNType);
		}

		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid.
//
// There are no further checks to make for this command.

bool acSetEventExecutionPeriod::IsDataValid(const CInputData& riData) const
{
	return true;
}
