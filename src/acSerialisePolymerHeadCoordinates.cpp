/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acSerialisePolymerHeadCoordinates.cpp: implementation of the acSerialisePolymerHeadCoordinates class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acSerialisePolymerHeadCoordinates.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString acSerialisePolymerHeadCoordinates::m_Type = "SerialisePolymerHeadCoordinates";

const zString acSerialisePolymerHeadCoordinates::GetType()
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
	xxCommand* Create(long executionTime) {return new acSerialisePolymerHeadCoordinates(executionTime);}

	const zString id = acSerialisePolymerHeadCoordinates::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acSerialisePolymerHeadCoordinates::acSerialisePolymerHeadCoordinates(long executionTime) : acACNCreateEventAnalysis(executionTime),
                                                        m_Period(0)
{
}

acSerialisePolymerHeadCoordinates::acSerialisePolymerHeadCoordinates(const acSerialisePolymerHeadCoordinates& oldCommand) : acACNCreateEventAnalysis(oldCommand),
                                                        m_Period(oldCommand.m_Period)

{
}


acSerialisePolymerHeadCoordinates::~acSerialisePolymerHeadCoordinates()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
// m_Period  Number of time steps between data points
//

zOutStream& acSerialisePolymerHeadCoordinates::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	acACNCreateEventAnalysis::put(os);
	os << "<Period>"    << m_Period << "</Period>"    << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	acACNCreateEventAnalysis::put(os);
    os << " " << m_Period;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acSerialisePolymerHeadCoordinates::get(zInStream& is)
{
	// Read the base class first

    acACNCreateEventAnalysis::get(is);

    is >> m_Period;

	if(!is.good() || m_Period < 1)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString acSerialisePolymerHeadCoordinates::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* acSerialisePolymerHeadCoordinates::GetCommand() const
{
	return new acSerialisePolymerHeadCoordinates(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool acSerialisePolymerHeadCoordinates::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
        // Call the SimBox's implementation of this command to create a
        // taBinSuccessIntervalsEventAnalysis instance. 
        // Execution of the command is logged by the command's implementation
        // class.

        pISimCmd->SerialisePolymerHeadCoordinates(this);

		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid.
//
// Only the base class reads any data, so we just return its validation state.

bool acSerialisePolymerHeadCoordinates::IsDataValid(const CInputData& riData) const
{
    return acACNCreateEventAnalysis::IsDataValid(riData);
}
