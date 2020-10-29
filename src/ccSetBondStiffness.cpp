/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSetBondStiffness.cpp: implementation of the ccSetBondStiffness class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSetBondStiffness.h"
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

const zString ccSetBondStiffness::m_Type = "SetBondStiffness";

const zString ccSetBondStiffness::GetType()
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
	xxCommand* Create(long executionTime) {return new ccSetBondStiffness(executionTime);}

	const zString id = ccSetBondStiffness::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSetBondStiffness::ccSetBondStiffness(long executionTime) : xxCommand(executionTime),
											   m_Name(""),
											   m_BendingModulus(0.0), m_Phi0(0.0)
{
}

ccSetBondStiffness::ccSetBondStiffness(const ccSetBondStiffness& oldCommand) : xxCommand(oldCommand),
														  m_Name(oldCommand.m_Name),
														  m_BendingModulus(oldCommand.m_BendingModulus),
														  m_Phi0(oldCommand.m_Phi0)
{
}

// Constructor for use when creating the command internally.

ccSetBondStiffness::ccSetBondStiffness(long executionTime, const zString name,
									   double modulus, double phi0) : xxCommand(executionTime),
																	   m_Name(name),
																	   m_BendingModulus(modulus), m_Phi0(phi0)
{
}

ccSetBondStiffness::~ccSetBondStiffness()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	Name				Identifier for the CBondPair made up of three bead names concatenated
//	Bending Modulus		New value for the bond stiffness
//	Phi0				New value for the preferred angle for the bond pair (degrees)

zOutStream& ccSetBondStiffness::put(zOutStream& os) const
{
	// We assume the preferred angle is in radians and convert it 
	// to degrees before writing it out

	double phi = m_Phi0*180.0/xxBase::m_globalPI;

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<BondName>"		 << m_Name			 << "</BondName>"		<< zEndl;
	os << "<BendingModulus>" << m_BendingModulus << "</BendingModulus>" << zEndl;
	os << "<PreferredAngle>" << phi				 << "</PreferredAngle>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_Name << " " << m_BendingModulus << " " << phi;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSetBondStiffness::get(zInStream& is)
{
	// Check that the bending stiffness is positive and that the
	// preferred angle lies within (0, 180). We check that the name
	// corresponds to a valid CBondPair object in IsDataValid() as 
	// the CInputData object has to create the map holding the names
	// of valid CBondPairs first.

	is >> m_Name >> m_BendingModulus >> m_Phi0;

	if(!is.good() || m_BendingModulus < 0.0 || m_Phi0 < 0.0 || m_Phi0 > 180.0)
	   SetCommandValid(false);

	// Convert the preferred angle into radians

	m_Phi0 = m_Phi0*xxBase::m_globalPI/180.0;

	return is;
}

// Non-static function to return the type of the command

const zString ccSetBondStiffness::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSetBondStiffness::GetCommand() const
{
	return new ccSetBondStiffness(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccSetBondStiffness::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetBondStiffness(this);
		return true;
	}
	else
		return false;
}

// Function to check that the name of the CBondPair object is valid. Note that 
// we have already validated the bending modulus and preferred angle above.

bool ccSetBondStiffness::IsDataValid(const CInputData& riData) const
{
	// Check target name is unique and consists only of allowed characters.

	if(!riData.IsExternalNameValid(m_Name))
		return ErrorTrace("Invalid BondPair name");
	else if(!riData.IsBondPairinMap(m_Name))
		return ErrorTrace("BondPair not found in map");

	return true;
}
