/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgcSetArgumentsTo2dTriangularLatticeVector.cpp: implementation of the cgcSetArgumentsTo2dTriangularLatticeVector class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgcSetArgumentsTo2dTriangularLatticeVector.h"
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

const zString cgcSetArgumentsTo2dTriangularLatticeVector::m_Type = "SetArgumentsTo2dTriangularLatticeVector";

const zString cgcSetArgumentsTo2dTriangularLatticeVector::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long cgcSetArgumentsTo2dTriangularLatticeVector::m_ArgumentTotal = 11;

long cgcSetArgumentsTo2dTriangularLatticeVector::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new cgcSetArgumentsTo2dTriangularLatticeVector(executionTime);}

    const zString id = cgcSetArgumentsTo2dTriangularLatticeVector::GetType();
    const long  argTotal = cgcSetArgumentsTo2dTriangularLatticeVector::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgcSetArgumentsTo2dTriangularLatticeVector::cgcSetArgumentsTo2dTriangularLatticeVector(long executionTime) : cgcSetArgumentsTo2dRealLatticeVector(executionTime)
{
}

// Constructor for use when creating the command internally.

cgcSetArgumentsTo2dTriangularLatticeVector::cgcSetArgumentsTo2dTriangularLatticeVector(long executionTime, const zString groupName, 
                                             long cmdIndexX, long cmdIndexY, 
                                             const zString argNameX ,const zString argNameY,
                                             long xmax, long ymax, double xorigin, double yorigin, 
                                             double lx, double ly) : cgcSetArgumentsTo2dRealLatticeVector(executionTime, groupName,
                                             cmdIndexX, cmdIndexY, argNameX, argNameY, xmax, ymax,
                                             xorigin, yorigin, lx, ly)
{
}

// Copy constructor
cgcSetArgumentsTo2dTriangularLatticeVector::cgcSetArgumentsTo2dTriangularLatticeVector(const cgcSetArgumentsTo2dTriangularLatticeVector& oldCommand) : cgcSetArgumentsTo2dRealLatticeVector(oldCommand)
{
}

cgcSetArgumentsTo2dTriangularLatticeVector::~cgcSetArgumentsTo2dTriangularLatticeVector()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
// No further data

zOutStream& cgcSetArgumentsTo2dTriangularLatticeVector::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	cgcSetArgumentsTo2dRealLatticeVector::put(os);

    putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	cgcSetArgumentsTo2dRealLatticeVector::put(os);
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& cgcSetArgumentsTo2dTriangularLatticeVector::get(zInStream& is)
{
	// Read the base class data first

	cgcSetArgumentsTo2dRealLatticeVector::get(is);

	return is;
}

// Non-static function to return the type of the command

const zString cgcSetArgumentsTo2dTriangularLatticeVector::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* cgcSetArgumentsTo2dTriangularLatticeVector::GetCommand() const
{
	return new cgcSetArgumentsTo2dTriangularLatticeVector(*this);
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool cgcSetArgumentsTo2dTriangularLatticeVector::Execute(long simTime, ISimCmd* const pISimCmd) const
{
// We only allow execution of the command if the feature is enabled. Note that we
// prefix the handler function with "Internal" to distinguish it from the SimBox's
// implementation function that returns a bool.

	if(simTime == GetExecutionTime())
	{
#if EnableCommandGroups == SimCommandEnabled
		pISimCmd->SetArgumentsTo2dTriangularLatticeVector(this);
#endif
		return true;
	}
	else
		return false;
}

// Function to check that the names of the command group and its contained command
// are valid. We also check that the command group exists. The test on the
// names of the argument and its new value are performed in the implementation class.

bool cgcSetArgumentsTo2dTriangularLatticeVector::IsDataValid(const CInputData& riData) const
{
    return cgcSetArgumentsTo2dRealLatticeVector::IsDataValid(riData);
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
// This command expects the following types (in addition to its base class types):
//
// No further data
//

bool cgcSetArgumentsTo2dTriangularLatticeVector::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        return cgcSetArgumentsTo2dRealLatticeVector::Pack(vArguments);
    }
    else
    {
        return false;
    }

#else
    return false;
#endif
}

