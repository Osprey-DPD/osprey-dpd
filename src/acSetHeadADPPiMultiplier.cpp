/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acSetHeadADPPiMultiplier.cpp: implementation of the acSetHeadADPPiMultiplier class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acSetHeadADPPiMultiplier.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "aeActiveSimBox.h"
#include "IModifyActiveCellNetwork.h"
#include "IModifyfActinNetwork.h"
#include "LogacSetHeadADPPiMultiplier.h"
#include "LogacNetworkNotModifiable.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString acSetHeadADPPiMultiplier::m_Type = "SetHeadADPPiMultiplier";

const zString acSetHeadADPPiMultiplier::GetType()
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
	xxCommand* Create(long executionTime) {return new acSetHeadADPPiMultiplier(executionTime);}

	const zString id = acSetHeadADPPiMultiplier::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acSetHeadADPPiMultiplier::acSetHeadADPPiMultiplier(long executionTime) : xxCommand(executionTime),
														m_ACNType(""),
														m_Multiplier(0.0)
{
}

acSetHeadADPPiMultiplier::acSetHeadADPPiMultiplier(const acSetHeadADPPiMultiplier& oldCommand) : xxCommand(oldCommand),
								   m_ACNType(oldCommand.m_ACNType),
								   m_Multiplier(oldCommand.m_Multiplier)

{
}


acSetHeadADPPiMultiplier::~acSetHeadADPPiMultiplier()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_ACNType     - Name of the active cell network to modify (not the bond name!)
//	m_Multiplier  -  Multiplier of the bond's basal transition rate
//

zOutStream& acSetHeadADPPiMultiplier::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<ActiveCellNetworkName>" << m_ACNType       << "</ActiveCellNetworkName>" << zEndl;
	os << "<Multiplier>"         << m_Multiplier << "</Multiplier>"		 << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_ACNType << " " << m_Multiplier;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acSetHeadADPPiMultiplier::get(zInStream& is)
{
	// We check that the ACN name is not null and that the specified 
	// multiplier is non-negative.

	is >> m_ACNType >> m_Multiplier;

	if(!is.good() || m_ACNType.empty() || m_Multiplier < 0.0)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString acSetHeadADPPiMultiplier::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* acSetHeadADPPiMultiplier::GetCommand() const
{
	return new acSetHeadADPPiMultiplier(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool acSetHeadADPPiMultiplier::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		IActiveSimBox* pIASB = pISimCmd->GetISimBox()->IIActiveSimBox();

		IModifyActiveCellNetwork* const pMACN = pIASB->GetIModifyActiveCellNetworkFromType(m_ACNType);

        // Currently only the fActin bonds have ATP

        IModifyfActinNetwork* const pMActin = dynamic_cast<IModifyfActinNetwork*>(pMACN);

		if(pMActin)
		{
			pMActin->SetHeadADPPiMultiplier(m_Multiplier);

			CLogacSetHeadADPPiMultiplier* pMsg = new CLogacSetHeadADPPiMultiplier(GetExecutionTime(), m_ACNType, m_Multiplier);
		}
		else
		{
			CLogacNetworkNotModifiable* pMsg = new CLogacNetworkNotModifiable(GetExecutionTime(), m_ACNType);
		}

		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid.
//
// There are no further checks to make for this command.

bool acSetHeadADPPiMultiplier::IsDataValid(const CInputData& riData) const
{
	return true;
}
