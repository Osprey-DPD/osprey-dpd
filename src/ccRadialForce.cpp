/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccRadialForce.cpp: implementation of the ccRadialForce class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccRadialForce.h"
#include "ISimCmd.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString ccRadialForce::m_Type = "RadialForce";

const zString ccRadialForce::GetType()
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
	xxCommand* Create(long executionTime) {return new ccRadialForce(executionTime);}

	const zString id = ccRadialForce::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccRadialForce::ccRadialForce(long executionTime) : ccApplyForce(executionTime), 
												   m_CX(0.0), m_CY(0.0), m_CZ(0.0),
												   m_Magnitude(0.0)
{

}

ccRadialForce::ccRadialForce(const ccRadialForce& oldCommand) : ccApplyForce(oldCommand),
															    m_CX(oldCommand.m_CX),
															    m_CY(oldCommand.m_CY),
															    m_CZ(oldCommand.m_CZ),
															    m_Magnitude(oldCommand.m_Magnitude)
{
}


ccRadialForce::~ccRadialForce()
{

}

// Member functions to read/write the data specific to the command.
// Note that leading spaces are written here not in the base classes. 
// Also note that we pass the command to the ccApplyForce base class 
// so that its data can be read/written before we get the data for this class.
//
// Arguments
// *********
//
//	m_CX
//	m_CY			Coordinates of the origin of the force (units of SimBox lengths)
//	m_CZ
//	m_Magnitude		Magnitude of the (constant) force

zOutStream& ccRadialForce::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	ccApplyForce::put(os);
	os << "<XC>" << m_CX << "</XC>" << zEndl;
	os << "<YC>" << m_CY << "</YC>" << zEndl;
	os << "<ZC>" << m_CZ << "</ZC>" << zEndl;
	os << "<Magnitude>" << m_Magnitude << "</Magnitude>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ccApplyForce::put(os);
	os << m_CX << " " << m_CY << " " << m_CZ << " " << m_Magnitude;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccRadialForce::get(zInStream& is)
{
	// Read the base class data first

	ccApplyForce::get(is);

	// Now read the local data. Note that the origin of the force can be
	// (0,0,0) but no negative coordinates are allowed, neither can the
	// force magnitude be negative. We check that the origin lies within 
	// SimBox in the IsDataValid() function.

	is >> m_CX >> m_CY >> m_CZ >> m_Magnitude;

	if(!is.good() || m_CX <  0.0 || m_CY <  0.0 || m_CZ <  0.0
		          || m_CX >= 1.0 || m_CY >= 1.0 || m_CZ >= 1.0 || m_Magnitude < 0.0)
		SetCommandValid(false);

	return is;
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands.
//
// This command zeroes the total bead momentum (CM momentum).

bool ccRadialForce::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->RadialForce(this);
		return true;
	}
	else
		return false;
}

const zString ccRadialForce::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccRadialForce::GetCommand() const
{
	return new ccRadialForce(*this);
}

// Function to check the data for the command. We have checked that the
// force origin lies within the SimBox boundaries so we just check the 
// base class data.

bool ccRadialForce::IsDataValid(const CInputData &riData) const
{
	return ccApplyForce::IsDataValid(riData);
}
