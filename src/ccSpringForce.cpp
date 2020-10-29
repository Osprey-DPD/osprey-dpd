/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSpringForce.cpp: implementation of the ccSpringForce class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSpringForce.h"
#include "ISimCmd.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString ccSpringForce::m_Type = "SpringForce";

const zString ccSpringForce::GetType()
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
	xxCommand* Create(long executionTime) {return new ccSpringForce(executionTime);}

	const zString id = ccSpringForce::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSpringForce::ccSpringForce(long executionTime) : ccApplyForce(executionTime), 
												   m_SpringConst(0.0),
												   m_SpringLength(0.0),
												   m_Velocity(0.0)
{

}

ccSpringForce::ccSpringForce(const ccSpringForce& oldCommand) : ccApplyForce(oldCommand),
															    m_SpringConst(oldCommand.m_SpringConst),
															    m_SpringLength(oldCommand.m_SpringLength),
															    m_Velocity(oldCommand.m_Velocity)
{
}


ccSpringForce::~ccSpringForce()
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
//	m_SpringConst	Hookean spring constant
//	m_SpringLength	Unstretched length of spring
//	m_Velocity		Retraction velocity of spring 

zOutStream& ccSpringForce::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	ccApplyForce::put(os);
	os << "<SpringConst>"		<< m_SpringConst  << "</SpringConst>"		<< zEndl;
	os << "<UnstretchedLength>"	<< m_SpringLength << "</UnstretchedLength>"	<< zEndl;
	os << "<Velocity>"			<< m_Velocity	  << "</Velocity>"			<< zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ccApplyForce::put(os);
	os << m_SpringConst << " " << m_SpringLength << " " << m_Velocity;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSpringForce::get(zInStream& is)
{
	// Read the base class data first

	ccApplyForce::get(is);

	// Now read the local data. Note that the velocity can be zero or negative
	// to allow forces to be applied in all directions in the SimBox.

	is >> m_SpringConst >> m_SpringLength >> m_Velocity;

	if(!is.good() || m_SpringConst < 0.0 || m_SpringLength < 0.0)
		SetCommandValid(false);

	return is;
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands.
//
// This command zeroes the total bead momentum (CM momentum).

bool ccSpringForce::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SpringForce(this);
		return true;
	}
	else
		return false;
}

const zString ccSpringForce::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSpringForce::GetCommand() const
{
	return new ccSpringForce(*this);
}

// Function to check the data for the command. The three parameters have 
// already been checked in the get() function above so we just return 
// the base class IsDataValid result.

bool ccSpringForce::IsDataValid(const CInputData &riData) const
{
	return ccApplyForce::IsDataValid(riData);
}
