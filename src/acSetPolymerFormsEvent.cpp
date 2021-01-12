/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acSetPolymerFormsEvent.cpp: implementation of the acSetPolymerFormsEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acSetPolymerFormsEvent.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "aevPolymerForms.h"

#include "aeActiveSimBox.h"
#include "IModifyActiveCellNetwork.h"
#include "LogacSetPolymerFormsEvent.h"
#include "LogacInvalidEventCommandTarget.h"
#include "LogacNetworkNotModifiable.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString acSetPolymerFormsEvent::m_Type = "SetPolymerFormsEvent";

const zString acSetPolymerFormsEvent::GetType()
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
	xxCommand* Create(long executionTime) {return new acSetPolymerFormsEvent(executionTime);}

	const zString id = acSetPolymerFormsEvent::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acSetPolymerFormsEvent::acSetPolymerFormsEvent(long executionTime) : xxCommand(executionTime),
														m_ACNType(""),
														m_Duration(0),
														m_Range(0.0),
														m_SpringConstant(0.0),
														m_Length(0.0)
{
}

acSetPolymerFormsEvent::acSetPolymerFormsEvent(const acSetPolymerFormsEvent& oldCommand) : xxCommand(oldCommand),
								   m_ACNType(oldCommand.m_ACNType),
									m_Duration(oldCommand.m_Duration),
									m_Range(oldCommand.m_Range),
									m_SpringConstant(oldCommand.m_SpringConstant),
									m_Length(oldCommand.m_Length)
{
}


acSetPolymerFormsEvent::~acSetPolymerFormsEvent()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_ACNType	     - Name of the active cell network to modify (not the bond name!)
//	m_Duration   	 - No of time steps within which to bring bonds together
//  m_Range          - Maximum distance at which a neighbour bond is chosen
//  m_SpringConstant - Final value of the Hookean spring constant for transient bond
//  m_Length         - Final value of the unstretched length of the transient bond
//

zOutStream& acSetPolymerFormsEvent::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<ActiveCellNetworkName>" << m_ACNType     << "</ActiveCellNetworkName>" << zEndl;
	os << "<Duration>"              << m_Duration << "</Duration>"                 << zEndl;
	os << "<Range>"                 << m_Range << "</Range>"                       << zEndl;
	os << "<SpringConstant>"        << m_SpringConstant << "</SpringConstant>"     << zEndl;
	os << "<Length>"                << m_Length << "</Length>"                     << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_ACNType << " " << m_Duration << " " << m_Range << " " << m_SpringConstant << " " << m_Length;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acSetPolymerFormsEvent::get(zInStream& is)
{
	// We check that the ACN name is not null and that the specified 
	// parameter values are positive. We cannot check their upper limits
	// until we have access to the SimBox side lengths.

	is >> m_ACNType >> m_Duration >> m_Range >> m_SpringConstant >> m_Length;

	if(!is.good() || m_ACNType.empty() || m_Duration < 1 ||
		                 m_Range < 0.0 || m_SpringConstant < 0.0 || m_Length < 0.0)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString acSetPolymerFormsEvent::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* acSetPolymerFormsEvent::GetCommand() const
{
	return new acSetPolymerFormsEvent(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool acSetPolymerFormsEvent::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		IActiveSimBox* pIASB = pISimCmd->GetISimBox()->IIActiveSimBox();

		IModifyActiveCellNetwork* const pMACN = pIASB->GetIModifyActiveCellNetworkFromType(m_ACNType);

		if(pMACN)
		{
			// Check that the event used for polymer formation matches the one
			// expected by this command, and issue a warming message if not.
			// If it is correct, set its parameters to the new values.

			if(pMACN->GetPolymerFormsEventType() == aevPolymerForms::GetType())
			{
				aevPolymerForms* const pPolymerForms = dynamic_cast<aevPolymerForms*>(pMACN->GetPolymerFormsEvent());

				pPolymerForms->SetDuration(GetDuration());
				pPolymerForms->SetRange(GetRange());
				pPolymerForms->SetSpringConstant(GetSpringConstant());
				pPolymerForms->SetLength(GetLength());

				 new CLogacSetPolymerFormsEvent(GetExecutionTime(), m_ACNType, 
																				  m_Duration, m_Range, m_SpringConstant, m_Length);
			}
			else
			{
				 new CLogacInvalidEventCommandTarget(GetExecutionTime(), m_ACNType, 
					                                              pMACN->GetPolymerFormsEventType(),
					                                              aevPolymerForms::GetType());
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

bool acSetPolymerFormsEvent::IsDataValid(const CInputData& riData) const
{
	return true;
}
