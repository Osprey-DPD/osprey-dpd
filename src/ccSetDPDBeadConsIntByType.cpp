/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSetDPDBeadConsIntByTypeByType.cpp: implementation of the ccSetDPDBeadConsIntByTypeByType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSetDPDBeadConsIntByType.h"
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

const zString ccSetDPDBeadConsIntByType::m_Type = "SetDPDBeadConsIntByType";

const zString ccSetDPDBeadConsIntByType::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ccSetDPDBeadConsIntByType::m_ArgumentTotal = 3;

long ccSetDPDBeadConsIntByType::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ccSetDPDBeadConsIntByType(executionTime);}

	const zString id = ccSetDPDBeadConsIntByType::GetType();
    const long  argTotal = ccSetDPDBeadConsIntByType::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSetDPDBeadConsIntByType::ccSetDPDBeadConsIntByType(long executionTime) : xxCommand(executionTime),
									m_FirstType(-1), m_SecondType(-1), m_ConsInt(0.0)
{
}

ccSetDPDBeadConsIntByType::ccSetDPDBeadConsIntByType(const ccSetDPDBeadConsIntByType& oldCommand) : xxCommand(oldCommand),
									 m_FirstType(oldCommand.m_FirstType),
									 m_SecondType(oldCommand.m_SecondType),
									 m_ConsInt(oldCommand.m_ConsInt)
{
}

// Constructor for use when creating the command internally. If the bead types are 
// negative, we set the command valid flag to false in the base class. It is up
// to the calling routine to check that the command is validated.

ccSetDPDBeadConsIntByType::ccSetDPDBeadConsIntByType(long executionTime, long firstType, 
													 long secondType, double consInt) : xxCommand(executionTime),
														m_FirstType(firstType), 
														m_SecondType(secondType), 
														m_ConsInt(consInt)
{
	if(m_FirstType < 0 || m_SecondType < 0)
	{
	   SetCommandValid(false);   
	}

}


ccSetDPDBeadConsIntByType::~ccSetDPDBeadConsIntByType()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	FirstType			Identifier for the primary CBead type whose interaction is being changed
//  SecondType			Identifier for the secondary CBead type
//  ConsInt				New value of conservative interaction for specified bead types

zOutStream& ccSetDPDBeadConsIntByType::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<FirstBeadType>"  << m_FirstType  << "</FirstBeadType>"  << zEndl;
	os << "<SecondBeadType>" << m_SecondType << "</SecondBeadType>" << zEndl;
	os << "<ConsInt>"		 << m_ConsInt	 << "</ConsInt>"		<< zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_FirstType << " " << m_SecondType << " " << m_ConsInt;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSetDPDBeadConsIntByType::get(zInStream& is)
{
	// We allow the conservative interaction paramter to be negative as this
	// represents an attractive force.

	is >> m_FirstType >> m_SecondType >> m_ConsInt;

	if(!is.good() || m_FirstType < 0 || m_SecondType < 0)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ccSetDPDBeadConsIntByType::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSetDPDBeadConsIntByType::GetCommand() const
{
	return new ccSetDPDBeadConsIntByType(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccSetDPDBeadConsIntByType::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetDPDBeadConsIntByType(this);
		return true;
	}
	else
		return false;
}

// Function to validate the command's data. Because this command may act
// on beads created dynamically, we cannot use the CInputData to check
// that the bead types exist as they may only be created later in the run.

bool ccSetDPDBeadConsIntByType::IsDataValid(const CInputData& riData) const
{
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
// m_FirstType    long
// m_SecondType   long
// m_ConsInt      double

bool ccSetDPDBeadConsIntByType::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(0)->GetValue(&m_FirstType);
        vArguments.at(1)->GetValue(&m_SecondType);
        vArguments.at(2)->GetValue(&m_ConsInt);

        if(m_FirstType < 0 || m_SecondType < 0)
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
