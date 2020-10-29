/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccFreezeBeadsInSphericalShell.cpp: implementation of the ccFreezeBeadsInSphericalShell class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccFreezeBeadsInSphericalShell.h"
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

const zString ccFreezeBeadsInSphericalShell::m_Type = "FreezeBeadsInSphericalShell";

const zString ccFreezeBeadsInSphericalShell::GetType()
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
	xxCommand* Create(long executionTime) {return new ccFreezeBeadsInSphericalShell(executionTime);}

	const zString id = ccFreezeBeadsInSphericalShell::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccFreezeBeadsInSphericalShell::ccFreezeBeadsInSphericalShell(long executionTime) : xxCommand(executionTime),
								m_InnerRadius(0.0), m_Thickness(0.0)
{
}

ccFreezeBeadsInSphericalShell::ccFreezeBeadsInSphericalShell(const ccFreezeBeadsInSphericalShell& oldCommand) : xxCommand(oldCommand),
														m_InnerRadius(oldCommand.m_InnerRadius),
														  m_Thickness(oldCommand.m_Thickness)

{
}

ccFreezeBeadsInSphericalShell::~ccFreezeBeadsInSphericalShell()
{

}

// Member functions to write/read the data specific to the command.
// Note that spaces are written here not in the xxCommand function.
//
// Arguments
// *********
//
//	m_InnerRadius		Radius of spherical shell (in units of bead diameter)
//	m_Thickness			Thickness of spherical shell (in units of bead diameter)


zOutStream& ccFreezeBeadsInSphericalShell::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<InnerRadius>" << m_InnerRadius << "</InnerRadius>" << zEndl;
	os << "<Thickness>"   << m_Thickness   << "</Thickness>"   << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_InnerRadius << " " << m_Thickness;
	putASCIIStartTags(os);

#endif

	return os;
}

zInStream& ccFreezeBeadsInSphericalShell::get(zInStream& is)
{
	is >> m_InnerRadius >> m_Thickness;

	if(!is.good())
		SetCommandValid(false);

	return is;
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands.
//
// This command defines a spherical shell of CNT cells and freezes all beads within 
// the cells.

bool ccFreezeBeadsInSphericalShell::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->FreezeBeadsInSphericalShell(this);
		return true;
	}
	else
		return false;
}

const zString ccFreezeBeadsInSphericalShell::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccFreezeBeadsInSphericalShell::GetCommand() const
{
	return new ccFreezeBeadsInSphericalShell(*this);
}


// Function to check that the data required to define a spherical shell of 
// CNT cells is valid. 
//
// The shell radius and thickness must be positive semi-definite. If the thickness 
// equals the radius, the shell is actually a sphere and all CNT cells within the 
// sphere are excluded. The shell radius + thickness is allowed to be larger than 
// the SimBox so that the corners of the SimBox can be exclusively excluded. If 
// the radius is larger than the SimBox diameter then no cells will be excluded, 
// but this case is still allowed.
// 
//

bool ccFreezeBeadsInSphericalShell::IsDataValid(const CInputData &riData) const
{
	if( m_InnerRadius < 0.0 || m_Thickness < 0.0 )
		return ErrorTrace("Shell thickness or radius negative");


	return true;
}

