/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acCancelFreeActivePolymersVisibleByMinLength.cpp: implementation of the acCancelFreeActivePolymersVisibleByMinLength class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acCancelFreeActivePolymersVisibleByMinLength.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "aeActiveSimBox.h"
#include "IModifyActiveCellNetwork.h"
#include "LogacSetFreeActivePolymersVisibleByMinLength.h"
#include "LogacEmptyPolymerACN.h"
#include "LogacNetworkNotModifiable.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString acCancelFreeActivePolymersVisibleByMinLength::m_Type = "CancelFreeActivePolymersVisibleByMinLength";

const zString acCancelFreeActivePolymersVisibleByMinLength::GetType()
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
	xxCommand* Create(long executionTime) {return new acCancelFreeActivePolymersVisibleByMinLength(executionTime);}

	const zString id = acCancelFreeActivePolymersVisibleByMinLength::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acCancelFreeActivePolymersVisibleByMinLength::acCancelFreeActivePolymersVisibleByMinLength(long executionTime) : xxCommand(executionTime),
														m_ACNType(""), m_MinSize(0)
{
}

acCancelFreeActivePolymersVisibleByMinLength::acCancelFreeActivePolymersVisibleByMinLength(const acCancelFreeActivePolymersVisibleByMinLength& oldCommand) : xxCommand(oldCommand),
														m_ACNType(oldCommand.m_ACNType),
														m_MinSize(oldCommand.m_MinSize)

{
}

acCancelFreeActivePolymersVisibleByMinLength::~acCancelFreeActivePolymersVisibleByMinLength()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_ACNType	    - Name of the active cell network to modify (not the bond name!)
 
zOutStream& acCancelFreeActivePolymersVisibleByMinLength::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<ActiveCellNetworkName>" << m_ACNType << "</ActiveCellNetworkName>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_ACNType;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acCancelFreeActivePolymersVisibleByMinLength::get(zInStream& is)
{
	// We check that the ACN name is not null and that the specified 
	// separation is positive.

	is >> m_ACNType;

	if(!is.good() || m_ACNType.empty())
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString acCancelFreeActivePolymersVisibleByMinLength::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* acCancelFreeActivePolymersVisibleByMinLength::GetCommand() const
{
	return new acCancelFreeActivePolymersVisibleByMinLength(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool acCancelFreeActivePolymersVisibleByMinLength::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		IActiveSimBox* pIASB = pISimCmd->GetISimBox()->IIActiveSimBox();

		IModifyActiveCellNetwork* const pMACN = pIASB->GetIModifyActiveCellNetworkFromType(m_ACNType);

		if(pMACN)
		{
			// Issue a warning message if there are no free polymers, but still
			// set the visible flag as there may be no polymers at the time the
			// command is executed, but we still want to modify the visibility of
			// polymers formed later.

			// This command disables the length check by setting the value to 0

			pMACN->SetFreeActivePolymersVisibleByMinLength(m_MinSize);

			 new CLogacSetFreeActivePolymersVisibleByMinLength(GetExecutionTime(), m_ACNType, m_MinSize);

			if(pMACN->GetFreePolymerTotal() == 0)
			{
				 new CLogacEmptyPolymerACN(GetExecutionTime(), m_ACNType);
			}
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

bool acCancelFreeActivePolymersVisibleByMinLength::IsDataValid(const CInputData& riData) const
{
	return true;
}
