/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccAddArp23Bond.cpp: implementation of the ccAddArp23Bond class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccAddArp23Bond.h"
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

const zString ccAddArp23Bond::m_Type = "AddArp23Bond";

const zString ccAddArp23Bond::GetType()
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
	xxCommand* Create(long executionTime) {return new ccAddArp23Bond(executionTime);}

	const zString id = ccAddArp23Bond::GetType();

#if EnableACNCommand == SimCommandEnabled
    const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
#endif
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccAddArp23Bond::ccAddArp23Bond(long executionTime) : ccAddActiveCellNetwork(executionTime),
													   m_HeadMonomerName(""),
													   m_TailMonomerName(""),
													   m_SpringConstant(0.0),
													   m_UnstretchedLength(0.0),
													   m_BendingModulus(0.0),
													   m_Phi0(0.0)
{
}

ccAddArp23Bond::ccAddArp23Bond(const ccAddArp23Bond& oldCommand) : ccAddActiveCellNetwork(oldCommand),
								   m_HeadMonomerName(oldCommand.m_HeadMonomerName),
								   m_TailMonomerName(oldCommand.m_TailMonomerName),
								   m_SpringConstant(oldCommand.m_SpringConstant),
								   m_UnstretchedLength(oldCommand.m_UnstretchedLength),
								   m_BendingModulus(oldCommand.m_BendingModulus),
								   m_Phi0(oldCommand.m_Phi0)

{
}

ccAddArp23Bond::~ccAddArp23Bond()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_HeadMonomerName	- Name of polymer type that is bound to this active bond
//	m_TailMonomerName	- Name of polymer type that is wrapped by this active bond
//	m_SpringConstant	- k2 in Hooke's law 
//	m_UnstretchedLength	- l0 in Hooke's law
//	m_BendingModulus	- k3 in the stiff bond force law
//  m_Phi0				- Preferred angle in the stiff bond force law (degrees)

zOutStream& ccAddArp23Bond::put(zOutStream& os) const
{
	// We assume the preferred angle, m_Phi0, is in radians and convert it 
	// to degrees before writing it out

	const double phi = m_Phi0*180.0/xxBase::m_globalPI;

#if EnableACNCommand == SimCommandEnabled
#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	ccAddActiveCellNetwork::put(os);

	os << "<HeadMonomerName>"   << m_HeadMonomerName   << "</HeadMonomerName>"   << zEndl;
	os << "<TailMonomerName>"   << m_TailMonomerName   << "</TailMonomerName>"   << zEndl;
	os << "<SpringConstant>"    << m_SpringConstant    << "</SpringConstant>"    << zEndl;
	os << "<UnstretchedLength>" << m_UnstretchedLength << "</UnstretchedLength>" << zEndl;
	os << "<BendingModulus>"	<< m_BendingModulus	   << "</BendingModulus>"    << zEndl;
	os << "<PreferredAngle>"	<< phi				   << "</PreferredAngle>"    << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ccAddActiveCellNetwork::put(os);
	os << " " << m_HeadMonomerName << " " << m_TailMonomerName;
	os << " " << m_SpringConstant  << " " << m_UnstretchedLength;
	os << " " << m_BendingModulus  << " " << phi;
	putASCIIEndTags(os);

#endif
#endif
	return os;
}

zInStream& ccAddArp23Bond::get(zInStream& is)
{
#if EnableACNCommand == SimCommandEnabled
	// Read the base class data first

	ccAddActiveCellNetwork::get(is);

	// We check that the monomer name corresponds to a valid CPolymer type in 
	// IsDataValid() as the CInputData object has to create the map holding 
	// the names of valid CPolymers first.

	is >> m_HeadMonomerName >> m_TailMonomerName >> m_SpringConstant >> m_UnstretchedLength >> m_BendingModulus >> m_Phi0;

	if(!is.good() || m_SpringConstant < 0.0 || m_UnstretchedLength < 0.0
		          || m_BendingModulus < 0.0 || m_Phi0 < 0.0 || m_Phi0 > 180.0)
	   SetCommandValid(false);

	// Convert the preferred angle into radians

	m_Phi0 = m_Phi0*xxBase::m_globalPI/180.0;

#endif
	return is;
}


// Non-static function to return the type of the command

const zString ccAddArp23Bond::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccAddArp23Bond::GetCommand() const
{
	return new ccAddArp23Bond(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccAddArp23Bond::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
#if EnableACNCommand == SimCommandEnabled
		pISimCmd->AddArp23Bond(this);
#endif
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the active bond type is valid.
// We first check that the dimensions of the active cell network are valid.

bool ccAddArp23Bond::IsDataValid(const CInputData& riData) const
{
	if(ccAddActiveCellNetwork::IsDataValid(riData))
	{
		if(!riData.IsExternalNameValid(m_HeadMonomerName))
			return ErrorTrace("Invalid active bond head monomer name");
		else if(!riData.IsPolymerinMap(m_HeadMonomerName))
			return ErrorTrace("Active bond head monomer type does not exist");
		else if(!riData.IsExternalNameValid(m_TailMonomerName))
			return ErrorTrace("Invalid active bond tail monomer name");
		else if(!riData.IsPolymerinMap(m_TailMonomerName))
			return ErrorTrace("Active bond tail monomer type does not exist");
		else if(m_HeadMonomerName != m_TailMonomerName)
			return ErrorTrace("Error: Arp23 active bond monomers are not the same");
	}
	else
		return false;

	return true;
}
