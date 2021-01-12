/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctPolymerisePolymerTripleInTarget.cpp: implementation of the ctPolymerisePolymerTripleInTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctPolymerisePolymerTripleInTarget.h"
#include "ISimCmd.h"
#include "InputData.h"
#include "Polymer.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString ctPolymerisePolymerTripleInTarget::m_Type = "PolymerisePolymerTripleInTarget";

const zString ctPolymerisePolymerTripleInTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.
// Note that this class needs a variable number of arguments that 
// depends on the value of the m_BondTotal member variable. Here, we specify
// only the fixed number of arguments.

long ctPolymerisePolymerTripleInTarget::m_ArgumentTotal = 5;

long ctPolymerisePolymerTripleInTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctPolymerisePolymerTripleInTarget(executionTime);}

	const zString id = ctPolymerisePolymerTripleInTarget::GetType();
    const long  argTotal = ctPolymerisePolymerTripleInTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctPolymerisePolymerTripleInTarget::ctPolymerisePolymerTripleInTarget(long executionTime) : ctApplyCommand(executionTime),
							m_PolymerName(""), m_BondTotal(0), m_SpringConstant(0.0), m_UnstretchedLength(0.0)
{
	m_HeadPositions.clear();
	m_TailPositions.clear();
}

ctPolymerisePolymerTripleInTarget::ctPolymerisePolymerTripleInTarget(const ctPolymerisePolymerTripleInTarget& oldCommand) : ctApplyCommand(oldCommand),
							m_PolymerName(oldCommand.m_PolymerName), 
							m_BondTotal(oldCommand.m_BondTotal), 
							m_HeadPositions(oldCommand.m_HeadPositions),
							m_TailPositions(oldCommand.m_TailPositions),
							m_SpringConstant(oldCommand.m_SpringConstant), 
							m_UnstretchedLength(oldCommand.m_UnstretchedLength)

{
}

// Constructor for use when creating the command internally.

ctPolymerisePolymerTripleInTarget::ctPolymerisePolymerTripleInTarget(long executionTime, const zString target,
									 const zString polyName, long bondTotal, zLongVector vHeads, zLongVector vTails, 
									 double spring, double length) : ctApplyCommand(executionTime, target),
									m_PolymerName(polyName), 
									m_BondTotal(bondTotal), 
									m_HeadPositions(vHeads),
									m_TailPositions(vTails),
									m_SpringConstant(spring), 
									m_UnstretchedLength(length)
{
}

ctPolymerisePolymerTripleInTarget::~ctPolymerisePolymerTripleInTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_PolymerName			name of the polymers to polymerise
//	m_BondTotal				number of polymerised bonds to create
//  m_HeadPositions			positions within polymer shape string for bond heads
//  m_TailPositions			positions within polymer shape string for bond tails
//  m_SpringConstant		spring constant of the Hookean bonds
//  m_UnstretchedLength		unstretched length of the Hookean bonds

zOutStream& ctPolymerisePolymerTripleInTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	// now the derived class data
	os << "<PolymerName>" << m_PolymerName << "</PolymerName>" << zEndl;
	os << "<BondTotal>"	  << m_BondTotal   << "</BondTotal>"   << zEndl;

	for(long ib=0; ib<m_BondTotal; ib++)
	{
		os << "<HeadPosition>"	<< m_HeadPositions.at(ib) << "</HeadPosition>" << zEndl;
		os << "<TailPosition>"	<< m_TailPositions.at(ib) << "</TailPosition>" << zEndl;
	}
	
	os << "<SpringConstant>"	<< m_SpringConstant		<< "</SpringConstant>"		<< zEndl;
	os << "<UnStretchedLength>"	<< m_UnstretchedLength	<< "</UnStretchedLength>"	<< zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_PolymerName << " " << m_BondTotal << " " ;

	for(long ib=0; ib<m_BondTotal; ib++)
	{
		os << m_HeadPositions.at(ib) << " " << m_TailPositions.at(ib) << " ";
	}
		
	os << m_SpringConstant << " " << m_UnstretchedLength;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctPolymerisePolymerTripleInTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Check that the spring constant and unstretched length are positive.
	// We check that the name corresponds to a valid CBond object in 
	// IsDataValid() as the CInputData object has to create the map holding 
	// the names of valid CBonds first.

	is >> m_PolymerName >> m_BondTotal;
	
	if(!is.good() || m_PolymerName.empty() || m_BondTotal < 1)
	   SetCommandValid(false);
	
	long headPos;
	long tailPos;

	m_HeadPositions.clear();
	m_TailPositions.clear();

	for(long ib=0; ib<m_BondTotal; ib++)
	{
		is >> headPos >> tailPos;

		if(!is.good())
			SetCommandValid(false);
		else
		{
			m_HeadPositions.push_back(headPos);
			m_TailPositions.push_back(tailPos);
		}
	}
	
	is >> m_SpringConstant >> m_UnstretchedLength;

	if(!is.good() || m_SpringConstant < 0.0 || m_UnstretchedLength < 0.0)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctPolymerisePolymerTripleInTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctPolymerisePolymerTripleInTarget::GetCommand() const
{
	return new ctPolymerisePolymerTripleInTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctPolymerisePolymerTripleInTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->PolymerisePolymerTripleInTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. 
// We check that the specified polymer name is valid and exists,
// ensure that the polymerised bond positions are valid and let 
// the base class check the target name. The actual Hookean
// bond parameters have been checked above.

bool ctPolymerisePolymerTripleInTarget::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_PolymerName))
		return ErrorTrace("Invalid Polymer name");
	else if(!riData.IsPolymerinMap(m_PolymerName))
		return ErrorTrace("Polymer not found in map");

	// Check that the bond positions lie within the size of the polymer.

	for(long iv=0; iv<m_BondTotal; iv++)
	{
		if(m_HeadPositions.at(iv) < 0)
			return ErrorTrace("Polymerised bond head position negative");
		if(m_TailPositions.at(iv) < 0)
			return ErrorTrace("Polymerised bond tail position negative");
		else if(m_HeadPositions.at(iv) >= riData.GetPolymerTypeSize(m_PolymerName))
			return ErrorTrace("Polymerised bond head position greater than polymer size");
		else if(m_TailPositions.at(iv) >= riData.GetPolymerTypeSize(m_PolymerName))
			return ErrorTrace("Polymerised bond tail position greater than polymer size");
	}

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
// m_PolymerName        - zString
// m_BondTotal          - long
// m_HeadPositions[]    - set of "m_BondTotal" longs
// m_TailPositions[]    - set of "m_BondTotal" longs
// m_SpringConstant     - double
// m_UnstretchedLength  - double
//
// Because the number of arguments to this command is variable, we have to 
// perform the test in two parts: first, we get the number of bonds required,
// and then we check the total number of arguments.

bool ctPolymerisePolymerTripleInTarget::Pack(const tguArgumentSequence& vArguments)
{
    // First check that at least the number of constant arguments is present,
    // and this should include the m_BondTotal parameter

    if(static_cast<long>(vArguments.size()) > GetArgumentTotal())
    {
        ctApplyCommand::Pack(vArguments);

        vArguments.at(1)->GetValue(&m_PolymerName);
        vArguments.at(2)->GetValue(&m_BondTotal);

        std::cout << "Command arguments for " << GetType() << " are: " << zEndl;
        std::cout << GetTargetLabel() << " " << GetPolymerName() << " "<< GetBondTotal() << " ";

        // Now check that there are exactly m_ArgumentTotal + m_BondTotal arguments
    
        if(static_cast<long>(vArguments.size()) == m_BondTotal + GetArgumentTotal())
        {
            long headIndex = 0;
            long tailIndex = 0;
            for(long ib=0; ib<m_BondTotal; ib++)
	        {
                vArguments.at(3+ib)->GetValue(&headIndex);
                vArguments.at(3+ib)->GetValue(&tailIndex);
                m_HeadPositions.push_back(headIndex);
                m_TailPositions.push_back(tailIndex);

                std::cout << m_HeadPositions.at(ib) << " "<< m_TailPositions.at(ib) << " ";
	        }

            vArguments.at(3+m_BondTotal)->GetValue(&m_SpringConstant);
            vArguments.at(4+m_BondTotal)->GetValue(&m_UnstretchedLength);

            std::cout << GetSpringConstant() << " " << GetUnstretchedLength() << zEndl;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

