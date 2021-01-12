/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctConstantSpecificAreaGhost.cpp: implementation of the ctConstantSpecificAreaGhost class.
//
//////////////////////////////////////////////////////////////////////
  
#include "StdAfx.h"
#include "SimDefs.h"
#include "ctConstantSpecificAreaGhost.h"
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

const zString ctConstantSpecificAreaGhost::m_Type = "ConstantSpecificAreaGhost";

const zString ctConstantSpecificAreaGhost::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctConstantSpecificAreaGhost::m_ArgumentTotal = 6;

long ctConstantSpecificAreaGhost::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctConstantSpecificAreaGhost(executionTime);}

    const zString id = ctConstantSpecificAreaGhost::GetType();
    const long  argTotal = ctConstantSpecificAreaGhost::GetArgumentTotal();

    const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctConstantSpecificAreaGhost::ctConstantSpecificAreaGhost(long executionTime) : xxCommand(executionTime),
                                                                               m_MembraneTargetName(""), m_ReservoirTargetName(""), 
                                                                               m_DecLabel(""),
                                                                               m_BeadName(""),
                                                                               m_SampleRate(0), m_TargetAN(0.0)
{
}

// Constructor for use when creating the command internally. We have to perform
// validation on the data explicitly. If an illegal value for a data item
// is found, the command has its valid flag set false in the base class.
// It is up to the calling routine to check that the command is valid.

ctConstantSpecificAreaGhost::ctConstantSpecificAreaGhost(long executionTime, const zString membraneName, const zString reservoirName, 
	                            const zString decLabel, const zString beadName, long sampleRate, double targetAN) : xxCommand(executionTime),
                                                                               m_MembraneTargetName(membraneName), 
																			   m_ReservoirTargetName(reservoirName), 
                                                                               m_DecLabel(decLabel),
                                                                               m_BeadName(beadName),
                                                                               m_SampleRate(sampleRate), m_TargetAN(targetAN) 
{
}

// Copy constructor

ctConstantSpecificAreaGhost::ctConstantSpecificAreaGhost(const ctConstantSpecificAreaGhost& oldCommand) : xxCommand(oldCommand),
                                     m_MembraneTargetName(oldCommand.m_MembraneTargetName),
									 m_ReservoirTargetName(oldCommand.m_ReservoirTargetName),
									 m_DecLabel(oldCommand.m_DecLabel),
									 m_BeadName(oldCommand.m_BeadName),
									 m_SampleRate(oldCommand.m_SampleRate),
 									 m_TargetAN(oldCommand.m_TargetAN)
{
}

ctConstantSpecificAreaGhost::~ctConstantSpecificAreaGhost()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//


zOutStream& ctConstantSpecificAreaGhost::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctConstantSpecificAreaGhost::get(zInStream& is)
{
	// We perform some simple checks on the data here, but more complicated
	// validation is done in IsDataValid() below as it has access to CInputData.

	is >> m_MembraneTargetName >> m_ReservoirTargetName >> m_DecLabel >> m_BeadName;

	if(!is.good())
	   SetCommandValid(false);

	is >> m_SampleRate >> m_TargetAN;

	// Note that the semi-major axis must be the largest dimension followed
	// by the first semi-minor axis and then the second semi-minor axis

	if(!is.good() || m_SampleRate < 1 || m_TargetAN < 0.01)
	{
	   SetCommandValid(false);
	}

	return is;
}

// Non-static function to return the type of the command

const zString ctConstantSpecificAreaGhost::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctConstantSpecificAreaGhost::GetCommand() const
{
	return new ctConstantSpecificAreaGhost(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctConstantSpecificAreaGhost::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->ConstantSpecificArea(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data is valid. We ensure that the polymers specified have been created,
// and that the target names specified do not already exist.

bool ctConstantSpecificAreaGhost::IsDataValid(const CInputData& riData) const
{
//	CInputData& rincData = const_cast<CInputData&>(riData);

	if(!riData.IsExternalNameValid(m_MembraneTargetName))
		return ErrorTrace("Invalid membrane command target name");
	else if(!riData.IsCommandTargetPresent(m_MembraneTargetName))
		return ErrorTrace("Membrane command target specified does not exist");
	else if(!riData.IsExternalNameValid(m_ReservoirTargetName))
		return ErrorTrace("Invalid reservoir command target name");
	else if(!riData.IsCommandTargetPresent(m_ReservoirTargetName))
		return ErrorTrace("Reservoir command target specified does not exist");
	else if(!riData.IsExternalNameValid(m_DecLabel))
		return ErrorTrace("Constant area ghost decorator name invalid");
	else if(riData.IsActiveCommandTargetPresent(m_DecLabel))
		return ErrorTrace("Constant area ghost decorator already exists");
    else if(!riData.IsExternalNameValid(m_BeadName))
		return ErrorTrace("Invalid bead name");

	return true;
}

// VF that allows an empty command instance to fill up its parameters after creation.
// It is typically used by the command groups to allow the command's arguments to
// be set when each of the group's commands is instantiated. The function returns
// true if all the command's parameters are correctly assigned values.
//
// The argument contains instances of types derived from tguArgumentBase that must
// be in the order expected by the command as it tries to cast them to the types
// it expects.
//
// This command expects the following types:
//
// m_MembraneTargetName     - zString
// m_ReservoirTargetName    - zString
// m_DecLabel               - zString
// m_BeadName               - zString
// m_SampleRate             - long
// m_TargetAN               - double

bool ctConstantSpecificAreaGhost::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled
    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(0)->GetValue(&m_MembraneTargetName);
        vArguments.at(1)->GetValue(&m_ReservoirTargetName);
        vArguments.at(2)->GetValue(&m_DecLabel);
        vArguments.at(3)->GetValue(&m_BeadName);
        vArguments.at(4)->GetValue(&m_SampleRate);
        vArguments.at(5)->GetValue(&m_TargetAN);

        // Validate the arguments: see the get() and IsDataValid() functions for details

        if(!CInputData::GlobalIsExternalNameValid(m_MembraneTargetName) || !CInputData::GlobalIsExternalNameValid(m_ReservoirTargetName) ||
		   !CInputData::GlobalIsExternalNameValid(m_DecLabel) || !CInputData::GlobalIsExternalNameValid(m_BeadName) ||
		   m_SampleRate < 1 || m_TargetAN < 0.01)
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
#else
    return false;
#endif
}
