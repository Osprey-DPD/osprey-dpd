/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acColourBondsByATPState.cpp: implementation of the acColourBondsByATPState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acColourBondsByATPState.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "aeActiveSimBox.h"
#include "IModifyfActinNetwork.h"
#include "LogacColourBondsByATPState.h"
#include "LogacNetworkNotModifiable.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString acColourBondsByATPState::m_Type = "ColourBondsByATPState";

const zString acColourBondsByATPState::GetType()
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
	xxCommand* Create(long executionTime) {return new acColourBondsByATPState(executionTime);}

	const zString id = acColourBondsByATPState::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acColourBondsByATPState::acColourBondsByATPState(long executionTime) : xxCommand(executionTime),
														m_ACNType(""),
														m_ATPDisplayId(-1),
														m_ADPPiDisplayId(-1),
														m_ADPDisplayId(-1)
{
}

acColourBondsByATPState::acColourBondsByATPState(const acColourBondsByATPState& oldCommand) : xxCommand(oldCommand),
								   m_ACNType(oldCommand.m_ACNType),
								   m_ATPDisplayId(oldCommand.m_ATPDisplayId),
								   m_ADPPiDisplayId(oldCommand.m_ADPPiDisplayId),
								   m_ADPDisplayId(oldCommand.m_ADPDisplayId)
{
}


acColourBondsByATPState::~acColourBondsByATPState()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_ACNType	      - Name of the active cell network to modify (not the bond name!)
//	m_ATPDisplayId	  - Display Id for a bond with bound ATP
//	m_ADPPiDisplayId  - Display Id for a bond with bound ADP-Pi
//	m_ADPDisplayId	  - Display Id for a bond with bound ADP
//

zOutStream& acColourBondsByATPState::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<ActiveCellNetworkName>" << m_ACNType     << "</ActiveCellNetworkName>" << zEndl;
	os << "<DisplayId>"           << m_ATPDisplayId << "</DisplayId>"         << zEndl;
	os << "<DisplayId>"           << m_ADPPiDisplayId << "</DisplayId>"         << zEndl;
	os << "<DisplayId>"           << m_ADPDisplayId << "</DisplayId>"         << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_ACNType << " " << m_ATPDisplayId << " " << m_ADPPiDisplayId << " " << m_ADPDisplayId;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acColourBondsByATPState::get(zInStream& is)
{
	// We check that the ACN name is not null and that the display Ids are integers
    // greater than or equal to -1. The value -1 is used to reset a bond's display
    // id to that of its component beads.

	is >> m_ACNType >> m_ATPDisplayId >> m_ADPPiDisplayId >> m_ADPDisplayId;

	if(!is.good() || m_ACNType.empty() || m_ATPDisplayId < -1 || 
                  m_ADPPiDisplayId < -1 || m_ADPDisplayId < -1)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString acColourBondsByATPState::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* acColourBondsByATPState::GetCommand() const
{
	return new acColourBondsByATPState(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool acColourBondsByATPState::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		IActiveSimBox* pIASB = pISimCmd->GetISimBox()->IIActiveSimBox();

		IModifyActiveCellNetwork* const pMACN = pIASB->GetIModifyActiveCellNetworkFromType(m_ACNType);

        // Currently only the fActin bonds have ATP

        IModifyfActinNetwork* const pMActin = dynamic_cast<IModifyfActinNetwork*>(pMACN);

		if(pMActin)
		{
			pMActin->ColourBondsByATPState(m_ATPDisplayId, m_ADPPiDisplayId, m_ADPDisplayId);

			 new CLogacColourBondsByATPState(GetExecutionTime(), m_ACNType, m_ATPDisplayId, m_ADPPiDisplayId, m_ADPDisplayId);
		}
		else
		{
			 new CLogacNetworkNotModifiable(GetExecutionTime(), m_ACNType);
		}

		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid.
//
// There are no further checks to make for this command.

bool acColourBondsByATPState::IsDataValid(const CInputData& riData) const
{
	return true;
}
