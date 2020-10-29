/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectACNFreePolymersAsTarget.cpp: implementation of the ccSelectACNFreePolymersAsTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectACNFreePolymersAsTarget.h"
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

const zString ccSelectACNFreePolymersAsTarget::m_Type = "SelectACNFreePolymersAsTarget";

const zString ccSelectACNFreePolymersAsTarget::GetType()
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
	xxCommand* Create(long executionTime) {return new ccSelectACNFreePolymersAsTarget(executionTime);}

	const zString id = ccSelectACNFreePolymersAsTarget::GetType();

#if EnableACNCommand == SimCommandEnabled
	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
#endif

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectACNFreePolymersAsTarget::ccSelectACNFreePolymersAsTarget(long executionTime) : xxCommand(executionTime),
													   m_TargetLabel(""),
													   m_ACNType("")
{
}

ccSelectACNFreePolymersAsTarget::ccSelectACNFreePolymersAsTarget(const ccSelectACNFreePolymersAsTarget& oldCommand) : xxCommand(oldCommand),
								   m_TargetLabel(oldCommand.m_TargetLabel),
								   m_ACNType(oldCommand.m_ACNType)
{
}


ccSelectACNFreePolymersAsTarget::~ccSelectACNFreePolymersAsTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_TargetLabel	- Name of command target
//  m_ACNType       - Name of ACN to wrap in a command target

zOutStream& ccSelectACNFreePolymersAsTarget::put(zOutStream& os) const
{
#if EnableACNCommand == SimCommandEnabled
#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<TargetLabel>" << m_TargetLabel << "</TargetLabel>" << zEndl;
	os << "<ACNType>"     << m_ACNType     << "</ACNType>"     << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_TargetLabel << " " << m_ACNType;
	putASCIIEndTags(os);

#endif
#endif

	return os;
}

zInStream& ccSelectACNFreePolymersAsTarget::get(zInStream& is)
{
	// We check that the monomer name corresponds to a valid CPolymer type in 
	// IsDataValid() as the CInputData object has to create the map holding 
	// the names of valid CPolymers first.

#if EnableACNCommand == SimCommandEnabled

	is >> m_TargetLabel >> m_ACNType;

	if(!is.good() || m_TargetLabel.empty() || m_ACNType.empty())
	   SetCommandValid(false);
#endif

	return is;
}

// Non-static function to return the type of the command

const zString ccSelectACNFreePolymersAsTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSelectACNFreePolymersAsTarget::GetCommand() const
{
	return new ccSelectACNFreePolymersAsTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccSelectACNFreePolymersAsTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
#if EnableACNCommand == SimCommandEnabled
		pISimCmd->SelectACNFreePolymersAsTarget(this);
#endif
		return true;
	}
	else
		return false;
}

// Function to check that the new command target name is valid and not already used.
// If it satisfies these conditions, we add it to the CInputData's set of valid
// targets.  We cannnot check that the ACN exists as we do not have a list of 
// them in the CInputData instance.

bool ccSelectACNFreePolymersAsTarget::IsDataValid(const CInputData& riData) const
{
	CInputData& rincData = const_cast<CInputData&>(riData);

	if( !riData.IsExternalNameValid(m_TargetLabel) )
		return ErrorTrace("Invalid command target name");
	else if( riData.IsCommandTargetPresent(m_TargetLabel) )
		return ErrorTrace("Command target already exists");


	// New target is unique so add it to the container in the CInputData

	rincData.AddCommandTargetName(m_TargetLabel);

	return true;
}
