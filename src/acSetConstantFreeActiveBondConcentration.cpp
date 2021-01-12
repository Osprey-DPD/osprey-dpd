/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acSetConstantFreeActiveBondConcentration.cpp: implementation of the acSetConstantFreeActiveBondConcentration class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acSetConstantFreeActiveBondConcentration.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "aeActiveSimBox.h"
#include "IModifyActiveCellNetwork.h"
#include "LogacSetConstantFreeActiveBondConcentration.h"
#include "LogacNetworkNotModifiable.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString acSetConstantFreeActiveBondConcentration::m_Type = "SetConstantFreeActiveBondConcentration";

const zString acSetConstantFreeActiveBondConcentration::GetType()
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
	xxCommand* Create(long executionTime) {return new acSetConstantFreeActiveBondConcentration(executionTime);}

	const zString id = acSetConstantFreeActiveBondConcentration::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We hardwire the number of phantoms per event to 1 for now as this is the most
// common case when an active bond binds to/unbinds from an active polymer.

acSetConstantFreeActiveBondConcentration::acSetConstantFreeActiveBondConcentration(long executionTime) : xxCommand(executionTime),
														m_ACNType(""),
                                                        m_Fraction(0.0),
														m_PhantomsPerEvent(1)
{
}

acSetConstantFreeActiveBondConcentration::acSetConstantFreeActiveBondConcentration(const acSetConstantFreeActiveBondConcentration& oldCommand) : xxCommand(oldCommand),
								   m_ACNType(oldCommand.m_ACNType),
								   m_Fraction(oldCommand.m_Fraction),
								   m_PhantomsPerEvent(oldCommand.m_PhantomsPerEvent)
{
}


acSetConstantFreeActiveBondConcentration::~acSetConstantFreeActiveBondConcentration()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_ACNType	       - Name of the active cell network to modify (not the bond name!)
//	m_Fraction	       - Fraction of all active bonds that is to be maintained constant
//  m_PhantomsPerEvent - Number of phantom bonds toggled on or off when the associated
//                       event occurs

zOutStream& acSetConstantFreeActiveBondConcentration::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<ActiveCellNetworkName>" << m_ACNType          << "</ActiveCellNetworkName>" << zEndl;
	os << "<ConstantFraction>"      << m_Fraction         << "</ConstantFraction>"      << zEndl;
	os << "<PhantomsPerEvent>"      << m_PhantomsPerEvent << "</PhantomsPerEvent>"      << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_ACNType << " " << m_Fraction << " " << m_PhantomsPerEvent;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acSetConstantFreeActiveBondConcentration::get(zInStream& is)
{
	// We check that the ACN name is not null and that the specified 
	// separation is positive.

	is >> m_ACNType >> m_Fraction;

	if(!is.good() || m_ACNType.empty() || m_Fraction < 0.0 || m_Fraction > 1.0 || m_PhantomsPerEvent < 0)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString acSetConstantFreeActiveBondConcentration::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* acSetConstantFreeActiveBondConcentration::GetCommand() const
{
	return new acSetConstantFreeActiveBondConcentration(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool acSetConstantFreeActiveBondConcentration::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		IActiveSimBox* pIASB = pISimCmd->GetISimBox()->IIActiveSimBox();

		IModifyActiveCellNetwork* const pMACN = pIASB->GetIModifyActiveCellNetworkFromType(m_ACNType);

		if(pMACN)
		{
			pMACN->SetConstantFreeBondConcentration(m_Fraction, m_PhantomsPerEvent);

			 new CLogacSetConstantFreeActiveBondConcentration(GetExecutionTime(), m_ACNType, m_Fraction, pMACN->GetConstantFreeBondNumber(), pMACN->GetPhantomBondTotal(), m_PhantomsPerEvent);
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

bool acSetConstantFreeActiveBondConcentration::IsDataValid(const CInputData& riData) const
{
	return true;
}
