/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectPolymerTypeInSimBox.cpp: implementation of the ccSelectPolymerTypeInSimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectPolymerTypeInSimBox.h"
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

const zString ccSelectPolymerTypeInSimBox::m_Type = "SelectPolymerTypeInSimBox";

const zString ccSelectPolymerTypeInSimBox::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ccSelectPolymerTypeInSimBox::m_ArgumentTotal = 2;

long ccSelectPolymerTypeInSimBox::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ccSelectPolymerTypeInSimBox(executionTime);}

	const zString id = ccSelectPolymerTypeInSimBox::GetType();
    const long  argTotal = ccSelectPolymerTypeInSimBox::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectPolymerTypeInSimBox::ccSelectPolymerTypeInSimBox(long executionTime) : ccSelectPolymerTypeInRegion(executionTime)
{
}

// Constructor for use when creating the command internally. We have to perform
// validation on the data explicitly. If an illegal value for a data item
// is found, the command has its valid flag set false in the base class.
// It is up to the calling routine to check that the command is valid.

ccSelectPolymerTypeInSimBox::ccSelectPolymerTypeInSimBox(long executionTime, 
							   const zString target, const zString name) : ccSelectPolymerTypeInRegion(executionTime, target, name)
{
}

// Copy constructor

ccSelectPolymerTypeInSimBox::ccSelectPolymerTypeInSimBox(const ccSelectPolymerTypeInSimBox& oldCommand) : ccSelectPolymerTypeInRegion(oldCommand)
{
}


ccSelectPolymerTypeInSimBox::~ccSelectPolymerTypeInSimBox()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
// This command has no specific data

zOutStream& ccSelectPolymerTypeInSimBox::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	ccSelectPolymerTypeInRegion::put(os);
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ccSelectPolymerTypeInRegion::put(os);
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSelectPolymerTypeInSimBox::get(zInStream& is)
{
	// Read the base class data first

	return ccSelectPolymerTypeInRegion::get(is);
}

// Non-static function to return the type of the command

const zString ccSelectPolymerTypeInSimBox::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSelectPolymerTypeInSimBox::GetCommand() const
{
	return new ccSelectPolymerTypeInSimBox(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccSelectPolymerTypeInSimBox::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SelectPolymerTypeInSimBox(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the planar slice region from
// which to select polymers whose heads are within it is valid. The target
// label and bead type are validated in the base class; here we check that 
// the slice region does not extend beyond the SimBox boundaries.

bool ccSelectPolymerTypeInSimBox::IsDataValid(const CInputData& riData) const
{
	return ccSelectPolymerTypeInRegion::IsDataValid(riData);
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
// Base class arguments only

bool ccSelectPolymerTypeInSimBox::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(ccSelectPolymerTypeInRegion::Pack(vArguments) && static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
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

