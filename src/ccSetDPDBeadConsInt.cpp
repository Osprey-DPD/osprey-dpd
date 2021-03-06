/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSetDPDBeadConsInt.cpp: implementation of the ccSetDPDBeadConsInt class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSetDPDBeadConsInt.h"
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

const zString ccSetDPDBeadConsInt::m_Type = "SetDPDBeadConsInt";

const zString ccSetDPDBeadConsInt::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ccSetDPDBeadConsInt::m_ArgumentTotal = 3;

long ccSetDPDBeadConsInt::GetArgumentTotal()
{
	return m_ArgumentTotal;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// command class but a global function hidden in the namespace.

namespace
{
	xxCommand* Create(long executionTime) {return new ccSetDPDBeadConsInt(executionTime);}

	const zString id = ccSetDPDBeadConsInt::GetType();
    const long  argTotal = ccSetDPDBeadConsInt::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSetDPDBeadConsInt::ccSetDPDBeadConsInt(long executionTime) : xxCommand(executionTime),
									m_FirstName(""), m_SecondName(""), m_ConsInt(0.0)
{
}

ccSetDPDBeadConsInt::ccSetDPDBeadConsInt(const ccSetDPDBeadConsInt& oldCommand) : xxCommand(oldCommand),
									 m_FirstName(oldCommand.m_FirstName),
									 m_SecondName(oldCommand.m_SecondName),
									 m_ConsInt(oldCommand.m_ConsInt)
{
}

ccSetDPDBeadConsInt::~ccSetDPDBeadConsInt()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	FirstName			Identifier for the primary CBead type whose interaction is being changed
//  SecondName			Identifier for the secondary CBead type
//  ConsInt				New value of conservative interaction for specified bead types

zOutStream& ccSetDPDBeadConsInt::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<FirstBeadName>"  << m_FirstName  << "</FirstBeadName>"  << zEndl;
	os << "<SecondBeadName>" << m_SecondName << "</SecondBeadName>" << zEndl;
	os << "<ConsInt>"		 << m_ConsInt	 << "</ConsInt>"		<< zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_FirstName << " " << m_SecondName << " " << m_ConsInt;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSetDPDBeadConsInt::get(zInStream& is)
{
	// We allow the conservative interaction paramter to be negative as this
	// represents an attractive force.

	// We check that the names correspond to valid CBead types in 
	// IsDataValid() as the CInputData object has to create the map holding 
	// the names of valid CBeads first.

	is >> m_FirstName >> m_SecondName >> m_ConsInt;

	if(!is.good())
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ccSetDPDBeadConsInt::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSetDPDBeadConsInt::GetCommand() const
{
	return new ccSetDPDBeadConsInt(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccSetDPDBeadConsInt::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetDPDBeadConsInt(this);
		return true;
	}
	else
		return false;
}

// Function to check that the names of the two CBead objects are valid.
// We allow any real value for the conservative interaction (including negative).

bool ccSetDPDBeadConsInt::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_FirstName))
		return ErrorTrace("Invalid primary Bead name");
	else if(!riData.IsExternalNameValid(m_SecondName))
		return ErrorTrace("Invalid secondary Bead name");

	return true;
}

// VF that allows an empty command instance to fill up its parameters after creation.
// It is typically used by the command groups to allow the command's arguments to
// be set when each of the group's commands is instantiated. The function returns
// true if all the command's parameters are correctly assigned values.
//
// The argument contains instances of types derived from tguArgumentBase that must
// be in the order expected by the command as it tries to cast them to the types
// it expects.
//
// This command expects the following types:
//
// m_FirstName    zString
// m_SecondName   zString
// m_ConsInt      double


bool ccSetDPDBeadConsInt::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(0)->GetValue(&m_FirstName);
        vArguments.at(1)->GetValue(&m_SecondName);
        vArguments.at(2)->GetValue(&m_ConsInt);

        if(!CInputData::GlobalIsExternalNameValid(m_FirstName) || !CInputData::GlobalIsExternalNameValid(m_SecondName))
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }

#else
    return false;
#endif
}
