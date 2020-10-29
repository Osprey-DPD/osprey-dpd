/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccRemoveCNTCellsOutsideRadius.cpp: implementation of the ccRemoveCNTCellsOutsideRadius class.
//
//////////////////////////////////////////////////////////////////////

// ccRemoveCNTCellsInsideRadius.cpp: implementation of the ccRemoveCNTCellsInsideRadius class.
//
//////////////////////////////////////////////////////////////////////

// ccFreezeBeadsInSphericalShell.cpp: implementation of the ccFreezeBeadsInSphericalShell class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccRemoveCNTCellsOutsideRadius.h"
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

const zString ccRemoveCNTCellsOutsideRadius::m_Type = "RemoveCNTCellsOutsideRadius";

const zString ccRemoveCNTCellsOutsideRadius::GetType()
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
	xxCommand* Create(long executionTime) {return new ccRemoveCNTCellsOutsideRadius(executionTime);}

	const zString id = ccRemoveCNTCellsOutsideRadius::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccRemoveCNTCellsOutsideRadius::ccRemoveCNTCellsOutsideRadius(long executionTime) : xxCommand(executionTime),
															 m_Radius(0.0)
{
}

ccRemoveCNTCellsOutsideRadius::ccRemoveCNTCellsOutsideRadius(const ccRemoveCNTCellsOutsideRadius& oldCommand) : xxCommand(oldCommand),
														m_Radius(oldCommand.m_Radius)
{
}

ccRemoveCNTCellsOutsideRadius::~ccRemoveCNTCellsOutsideRadius()
{

}

// Member functions to write/read the data specific to the command.
// Note that spaces are written here not in the xxCommand function.
//
// Arguments
// *********
//
//	m_Radius		Radius of spherical shell (in units of bead diameter)
//

zOutStream& ccRemoveCNTCellsOutsideRadius::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<Radius>" << m_Radius << "</Radius>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_Radius;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccRemoveCNTCellsOutsideRadius::get(zInStream& is)
{
	is >> m_Radius;

	if(!is.good())
		SetCommandValid(false);

	return is;
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands.
//
// This command remove all CNT cells that are outside a specified radius from the
// integration loop. This effectively removes their beads from interacting with 
// any remaining beads, and hence speeds up the simulation.

bool ccRemoveCNTCellsOutsideRadius::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->RemoveCNTCellsOutsideRadius(this);
		return true;
	}
	else
		return false;
}

const zString ccRemoveCNTCellsOutsideRadius::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccRemoveCNTCellsOutsideRadius::GetCommand() const
{
	return new ccRemoveCNTCellsOutsideRadius(*this);
}


// Function to check that the data for the command are valid. 
// The radius of the shell inside which to remove all beads must be non-negative.

bool ccRemoveCNTCellsOutsideRadius::IsDataValid(const CInputData &riData) const
{
	if( m_Radius < 0.0 )
		return ErrorTrace("Shell radius negative");

	return true;
}

