/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctDynamicPolymerisePolymersInTarget.cpp: implementation of the ctDynamicPolymerisePolymersInTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctDynamicPolymerisePolymersInTarget.h"
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

const zString ctDynamicPolymerisePolymersInTarget::m_Type = "DynamicPolymerisePolymersInTarget";

const zString ctDynamicPolymerisePolymersInTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctDynamicPolymerisePolymersInTarget::m_ArgumentTotal = 9;

long ctDynamicPolymerisePolymersInTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctDynamicPolymerisePolymersInTarget(executionTime);}

	const zString id = ctDynamicPolymerisePolymersInTarget::GetType();
    const long  argTotal = ctDynamicPolymerisePolymersInTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctDynamicPolymerisePolymersInTarget::ctDynamicPolymerisePolymersInTarget(long executionTime) :
                                    ctApplyCommand(executionTime), m_DecLabel(""),
                                    m_PolymerType(-1), m_BeadTypeTotal(-1), m_MaxBonds(0), m_MaxDistance(0.0),
                                    m_sprConst(0.0), m_unStrLength(0.0), m_BondPrefactor(0.0), m_BondBarrier(0.0)
{
    m_vBeadTypes.clear();
}

ctDynamicPolymerisePolymersInTarget::ctDynamicPolymerisePolymersInTarget(const ctDynamicPolymerisePolymersInTarget& oldCommand) : ctApplyCommand(oldCommand),
												m_DecLabel(oldCommand.m_DecLabel),
                                                m_PolymerType(oldCommand.m_PolymerType),
                                                m_BeadTypeTotal(oldCommand.m_BeadTypeTotal),
                                                m_vBeadTypes(oldCommand.m_vBeadTypes),
                                                m_MaxBonds(oldCommand.m_MaxBonds),
                                                m_MaxDistance(oldCommand.m_MaxDistance),
                                                m_sprConst(oldCommand.m_sprConst),
                                                m_unStrLength(oldCommand.m_unStrLength),
                                                m_BondPrefactor(oldCommand.m_BondPrefactor),
                                                m_BondBarrier(oldCommand.m_BondBarrier)
{
}

// Constructor for use when creating the command internally.

ctDynamicPolymerisePolymersInTarget::ctDynamicPolymerisePolymersInTarget(long executionTime, const zString targetLabel, const zString decLabel, long polyType, long beadTypeTotal, zLongVector vBeadTypes, long maxBonds, double maxDist, double sprConst, double unStrLength, double bondPrefactor, double bondBarrier) :  ctApplyCommand(executionTime, targetLabel),
                                     m_DecLabel(decLabel), m_PolymerType(polyType),m_BeadTypeTotal(beadTypeTotal),
                                     m_vBeadTypes(vBeadTypes),
                                     m_MaxBonds(maxBonds), m_MaxDistance(maxDist),
                                     m_sprConst(sprConst), m_unStrLength(unStrLength),
                                     m_BondPrefactor(bondPrefactor), m_BondBarrier(bondBarrier)
{
}

ctDynamicPolymerisePolymersInTarget::~ctDynamicPolymerisePolymersInTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_TargetLabel      label for the target whose polymers are to be polymerised
//  m_DecLabel         label for this decorator object so that it can be deleted later
//  m_PolymerType      Numeric identifier of the polymer type
//  m_BeadTypeTotal    Number of polymerisable bead types in the polymers
//  m_vBeadTypes       Container of the numeric identifiers of the polymerisable bead types in the polymers
//  m_MaxBonds         Maximum number of bonds that can be attached to a polymer
//  m_MaxDistance      Range within which two polymers will be connected by a bond
//  m_sprConst		Spring constant of the Hookean springs
//  m_unStrLength      Unstretched length of the Hookean springs
//  m_BondPrefactor    Bare prefactor for Arrhenius function for bond formation
//  m_BondBarrier      Energy barrier for Arrhenius function for bond formation

zOutStream& ctDynamicPolymerisePolymersInTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	os << "<DecoratorLabel>"    << m_DecLabel    << "</DecoratorLabel>" << zEndl;
    os << "<PolymerType>"       << m_PolymerType << "<PolymerType>" << zEndl;
    
    for(czLongVectorIterator iter=m_vBeadTypes.begin(); iter!=m_vBeadTypes.end(); ++iter)
    {
        "<BeadType>" << *iter  << "</BeadType>";
    }
    os << zEndl;
    
    os << "<MaxBonds>"          << m_MaxBonds      << "</MaxBonds>";
       << "<MaxDistance>"       << m_MaxDistance   << "</MaxDistance>"
       << "<SpringConstant>"    << m_sprConst      << "</SpringConstant>"
       << "<UnstretchedLength>" << m_unStrLength   << "</UnstretchedLength>"
       << "<BondPrefactor>"     << m_BondPrefactor << "</BondPrefactor>"
       << "<BondBarrier>"       << m_BondBarrier   << "</BondBarrier>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
    os << m_DecLabel << "  " << m_PolymerType;
    
    for(czLongVectorIterator iter=m_vBeadTypes.begin(); iter!=m_vBeadTypes.end(); ++iter)
    {
        os << " " << *iter;
    }
    os << zEndl;
	os << " " << m_MaxBonds<< "  " << m_MaxDistance << " " << m_sprConst << " " << m_unStrLength
	   << " " << m_BondPrefactor << " " << m_BondBarrier;
	   putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctDynamicPolymerisePolymersInTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Now read the local data. Because there are an arbitrary number of polymerisable bead types we first
    // read in the number of bead types and then the types.
    
    is >> m_DecLabel >> m_PolymerType >> m_BeadTypeTotal;
    
    if(m_BeadTypeTotal > 0)
    {
        m_vBeadTypes.clear();
        long type = 0;
        
        for(long i=0; i<m_BeadTypeTotal; ++i)
        {
            is >> type;
            m_vBeadTypes.push_back(type);
        }
        
        is >> m_MaxBonds >> m_MaxDistance >> m_sprConst >> m_unStrLength >> m_BondPrefactor >> m_BondBarrier;
    }
    else
    {
        SetCommandValid(false);
    }
    
	if(!is.good())
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctDynamicPolymerisePolymersInTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctDynamicPolymerisePolymersInTarget::GetCommand() const
{
	return new ctDynamicPolymerisePolymersInTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctDynamicPolymerisePolymersInTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->DynamicPolymerisePolymersInTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. 
// Note that we  check if the bead types specified exist so they cannot be dynamically created types.

bool ctDynamicPolymerisePolymersInTarget::IsDataValid(const CInputData& riData) const
{
    CInputData& rincData = const_cast<CInputData&>(riData);

	if(!riData.IsExternalNameValid(m_DecLabel))
	{
		return ErrorTrace("PolymerisePolymersInTarget active command target name invalid");
	}
	else if(riData.IsActiveCommandTargetPresent(m_DecLabel))
	{
		return ErrorTrace("PolymerisePolymersInTarget active command target already exists");
	}
	else if(m_PolymerType < 0 || m_BeadTypeTotal < 1)
	{
		return ErrorTrace("PolymerisePolymersInTarget active command target invalid polymer type or bead type total");
	}
	else if(m_MaxBonds < 1 || m_MaxDistance < 0.1)
	{
		return ErrorTrace("PolymerisePolymersInTarget active command target invalid max bonds or max distance");
	}
	else if(m_sprConst < 0.0)
	{
		return ErrorTrace("PolymerisePolymersInTarget active command target invalid spring constant ");
	}
        else if(m_unStrLength < 0.0)
        {
            return ErrorTrace("PolymerisePolymersInTarget active command target invalid unstretched length");
        }
        else if(m_BondPrefactor < 0.0)
        {
            return ErrorTrace("PolymerisePolymersInTarget active command target invalid bond formation Arrhenius prefactor");
        }
        else if(m_BondBarrier < 0.0)
        {
            return ErrorTrace("PolymerisePolymersInTarget active command target negative bond formation Arrhenius barrier");
        }
	else
	{
        // Check that the bead types exist, and note that they are zero-indexed.
        
        for(czLongVectorIterator citer=m_vBeadTypes.begin(); citer!=m_vBeadTypes.end(); ++citer)
        {
            if(*citer < 0 || rincData.GetBeadTypesMap().find(*citer) == rincData.GetBeadTypesMap().end())
            {
                return ErrorTrace("PolymerisePolymersInTarget active command target invalid bead type");
            }
        }
        
        // New target is unique so add it to the container in the CInputData

		rincData.AddActiveCommandTargetName(m_DecLabel);

		return ctApplyCommand::IsDataValid(riData);
	}
}
