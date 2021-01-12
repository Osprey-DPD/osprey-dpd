/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgcSetArgumentsTo3dRectangularLatticeVector.cpp: implementation of the cgcSetArgumentsTo3dRectangularLatticeVector class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgcSetArgumentsTo3dRectangularLatticeVector.h"
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

const zString cgcSetArgumentsTo3dRectangularLatticeVector::m_Type = "SetArgumentsTo3dRectangularLatticeVector";

const zString cgcSetArgumentsTo3dRectangularLatticeVector::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long cgcSetArgumentsTo3dRectangularLatticeVector::m_ArgumentTotal = 16;

long cgcSetArgumentsTo3dRectangularLatticeVector::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new cgcSetArgumentsTo3dRectangularLatticeVector(executionTime);}

    const zString id = cgcSetArgumentsTo3dRectangularLatticeVector::GetType();
    const long  argTotal = cgcSetArgumentsTo3dRectangularLatticeVector::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgcSetArgumentsTo3dRectangularLatticeVector::cgcSetArgumentsTo3dRectangularLatticeVector(long executionTime) : cgcSetArgumentsTo3dRealLatticeVector(executionTime)
{
}

// Constructor for use when creating the command internally.

cgcSetArgumentsTo3dRectangularLatticeVector::cgcSetArgumentsTo3dRectangularLatticeVector(long executionTime, const zString groupName, 
                                             long cmdIndexX, long cmdIndexY, long cmdIndexZ,
                                             const zString argNameX, const zString argNameY, const zString argNameZ,
                                             long xmax, long ymax, long zmax, 
                                             double xorigin, double yorigin, double zorigin, 
                                             double lx, double ly, double lz) : cgcSetArgumentsTo3dRealLatticeVector(executionTime, groupName,
                                             cmdIndexX, cmdIndexY, cmdIndexZ, argNameX, argNameY,argNameZ, 
                                             xmax, ymax, zmax, xorigin, yorigin, zorigin, lx, ly, lz)
{
}

// Copy constructor
cgcSetArgumentsTo3dRectangularLatticeVector::cgcSetArgumentsTo3dRectangularLatticeVector(const cgcSetArgumentsTo3dRectangularLatticeVector& oldCommand) : cgcSetArgumentsTo3dRealLatticeVector(oldCommand)
{
}

cgcSetArgumentsTo3dRectangularLatticeVector::~cgcSetArgumentsTo3dRectangularLatticeVector()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
// No further data

zOutStream& cgcSetArgumentsTo3dRectangularLatticeVector::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	cgcSetArgumentsTo3dRealLatticeVector::put(os);

    putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	cgcSetArgumentsTo3dRealLatticeVector::put(os);
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& cgcSetArgumentsTo3dRectangularLatticeVector::get(zInStream& is)
{
	// Read the base class data first

	cgcSetArgumentsTo3dRealLatticeVector::get(is);

	return is;
}

// Non-static function to return the type of the command

const zString cgcSetArgumentsTo3dRectangularLatticeVector::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* cgcSetArgumentsTo3dRectangularLatticeVector::GetCommand() const
{
	return new cgcSetArgumentsTo3dRectangularLatticeVector(*this);
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool cgcSetArgumentsTo3dRectangularLatticeVector::Execute(long simTime, ISimCmd* const pISimCmd) const
{
// We only allow execution of the command if the feature is enabled. Note that we
// prefix the handler function with "Internal" to distinguish it from the SimBox's
// implementation function that returns a bool.

	if(simTime == GetExecutionTime())
	{
#if EnableCommandGroups == SimCommandEnabled
		pISimCmd->SetArgumentsTo3dRectangularLatticeVector(this);
#endif
		return true;
	}
	else
		return false;
}

// Function to check that the names of the command group and its contained command
// are valid. We also check that the command group exists. The test on the
// names of the argument and its new value are performed in the implementation class.

bool cgcSetArgumentsTo3dRectangularLatticeVector::IsDataValid(const CInputData& riData) const
{
    return cgcSetArgumentsTo3dRealLatticeVector::IsDataValid(riData);
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

bool cgcSetArgumentsTo3dRectangularLatticeVector::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        return cgcSetArgumentsTo3dRealLatticeVector::Pack(vArguments);
    }
    else
    {
        return false;
    }

#else
    return false;
#endif
}

