/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctPolymerisePolymersInTarget.cpp: implementation of the ctPolymerisePolymersInTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctPolymerisePolymersInTarget.h"
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

const zString ctPolymerisePolymersInTarget::m_Type = "PolymerisePolymersInTarget";

const zString ctPolymerisePolymersInTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctPolymerisePolymersInTarget::m_ArgumentTotal = 6;

long ctPolymerisePolymersInTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctPolymerisePolymersInTarget(executionTime);}

	const zString id = ctPolymerisePolymersInTarget::GetType();
    const long  argTotal = ctPolymerisePolymersInTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctPolymerisePolymersInTarget::ctPolymerisePolymersInTarget(long executionTime) : ctApplyCommand(executionTime),
							m_MaxBonds(0), m_Range(0.0), m_Fraction(0.0), 
							m_SpringConstant(0.0), m_UnstretchedLength(0.0)
{
}

ctPolymerisePolymersInTarget::ctPolymerisePolymersInTarget(const ctPolymerisePolymersInTarget& oldCommand) : ctApplyCommand(oldCommand),
							m_MaxBonds(oldCommand.m_MaxBonds), 
							m_Range(oldCommand.m_Range), 
							m_Fraction(oldCommand.m_Fraction), 
							m_SpringConstant(oldCommand.m_SpringConstant), 
							m_UnstretchedLength(oldCommand.m_UnstretchedLength)

{
}

// Constructor for use when creating the command internally.

ctPolymerisePolymersInTarget::ctPolymerisePolymersInTarget(long executionTime, const zString target,
									 long maxBonds, double range, double fraction, 
									 double spring, double length) : ctApplyCommand(executionTime, target),
									m_MaxBonds(maxBonds),
									m_Range(range),
									m_Fraction(fraction),
									m_SpringConstant(spring), 
									m_UnstretchedLength(length)
{
}

ctPolymerisePolymersInTarget::~ctPolymerisePolymersInTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_MaxBonds				maximum number of bonds per polymer
//	m_Range				    maximum separation of two polymers that can be bound
//	m_Fraction				fraction of neighbours to bind
//  m_SpringConstant		spring constant of the Hookean bonds
//  m_UnstretchedLength		unstretched length of the Hookean bonds

zOutStream& ctPolymerisePolymersInTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	// now the derived class data
	
	os << "<MaxBonds>"			<< m_MaxBonds			<< "</MaxBonds>"		<< zEndl;
	os << "<Range>"				<< m_Range				<< "</Range>"			<< zEndl;
	os << "<BondFraction>"	    << m_Fraction		    << "</BondFraction>"		<< zEndl;
	os << "<SpringConstant>"	<< m_SpringConstant		<< "</SpringConstant>"		<< zEndl;
	os << "<UnStretchedLength>"	<< m_UnstretchedLength	<< "</UnStretchedLength>"	<< zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
		
	os << m_MaxBonds << "  " << m_Range << "  " << m_Fraction << "  " << m_SpringConstant << " " << m_UnstretchedLength;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctPolymerisePolymersInTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Check that the spring constant and unstretched length are positive, 
	// and that the bound fraction is contained in [0,1].
	
	is >> m_MaxBonds >> m_Range >> m_Fraction >> m_SpringConstant >> m_UnstretchedLength;

	if(!is.good() || m_MaxBonds < 1
				  || m_Range < 0.0
				  || m_Fraction < 0.0 || m_Fraction > 1.0 
		          || m_SpringConstant < 0.0 || m_UnstretchedLength < 0.0)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctPolymerisePolymersInTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctPolymerisePolymersInTarget::GetCommand() const
{
	return new ctPolymerisePolymersInTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctPolymerisePolymersInTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->PolymerisePolymersInTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. 
// The bond data has been checked above so we just return the base class result.

bool ctPolymerisePolymersInTarget::IsDataValid(const CInputData& riData) const
{
	return ctApplyCommand::IsDataValid(riData);
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
// This command expects the following types (in addition to the base class types):
//
// m_MaxBonds           - long
// m_Range              - double
// m_Fraction           - double
// m_SpringConstant     - double
// m_UnstretchedLength  - double

bool ctPolymerisePolymersInTarget::Pack(const tguArgumentSequence& vArguments)
{
    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        ctApplyCommand::Pack(vArguments);
        vArguments.at(1)->GetValue(&m_MaxBonds);
        vArguments.at(2)->GetValue(&m_Range);
        vArguments.at(3)->GetValue(&m_Fraction);
        vArguments.at(4)->GetValue(&m_SpringConstant);
        vArguments.at(5)->GetValue(&m_UnstretchedLength);

        return true;
    }
    else
    {
        return false;
    }
}

