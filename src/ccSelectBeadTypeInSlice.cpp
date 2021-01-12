/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectBeadTypeInSlice.cpp: implementation of the ccSelectBeadTypeInSlice class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectBeadTypeInSlice.h"
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

const zString ccSelectBeadTypeInSlice::m_Type = "SelectBeadTypeInSlice";

const zString ccSelectBeadTypeInSlice::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ccSelectBeadTypeInSlice::m_ArgumentTotal = 11;

long ccSelectBeadTypeInSlice::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ccSelectBeadTypeInSlice(executionTime);}

	const zString id = ccSelectBeadTypeInSlice::GetType();
    const long  argTotal = ccSelectBeadTypeInSlice::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectBeadTypeInSlice::ccSelectBeadTypeInSlice(long executionTime) : ccSelectBeadTypeInRegion(executionTime),
									m_X(0), m_Y(0), m_Z(0),
									m_XC(0.0), m_YC(0.0), m_ZC(0.0),
									m_XHalfWidth(0.0), m_YHalfWidth(0.0), m_ZHalfWidth(0.0)
{
}

ccSelectBeadTypeInSlice::ccSelectBeadTypeInSlice(const ccSelectBeadTypeInSlice& oldCommand) : ccSelectBeadTypeInRegion(oldCommand),
									 m_X(oldCommand.m_X),
									 m_Y(oldCommand.m_Y),
									 m_Z(oldCommand.m_Z),
									 m_XC(oldCommand.m_XC),
									 m_YC(oldCommand.m_YC),
									 m_ZC(oldCommand.m_ZC),
									 m_XHalfWidth(oldCommand.m_XHalfWidth),
									 m_YHalfWidth(oldCommand.m_YHalfWidth),
									 m_ZHalfWidth(oldCommand.m_ZHalfWidth)
{
}

// Constructor for use when creating the command internally. We have to perform
// validation on the data explicitly. If an illegal value for a data item
// is found, the command has it svalid flag set false in the base class.
// It is up to the calling routine to check that the command is valid.

ccSelectBeadTypeInSlice::ccSelectBeadTypeInSlice(long executionTime, 
							   const zString target, const zString bead, 
							   long xn, long yn, long zn, 
							   double xc, double yc, double zc, double xw,
							   double yw, double zw) : ccSelectBeadTypeInRegion(executionTime, target, bead),
																	m_X(xn), m_Y(yn), m_Z(zn),
																	m_XC(xc), m_YC(yc), m_ZC(zc),
																	m_XHalfWidth(xw), m_YHalfWidth(yw), m_ZHalfWidth(zw)
{
	if((m_X == 0 && m_Y == 0 && m_Z == 0) || m_X < 0  || m_Y < 0  || m_Z < 0 ||
	   (m_X == 1 && (m_Y == 1 || m_Z == 1)) || (m_Y == 1 && m_Z == 1))

	{
	   SetCommandValid(false);
	}

	if(m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
	   m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
	{
	   SetCommandValid(false);
	}

	if(m_XHalfWidth < 0.0 || m_YHalfWidth < 0.0 || m_ZHalfWidth < 0.0 ||
	   m_XHalfWidth > 0.5 || m_YHalfWidth > 0.5 || m_ZHalfWidth > 0.5)
	{
	   SetCommandValid(false);
	}
}


ccSelectBeadTypeInSlice::~ccSelectBeadTypeInSlice()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_X, m_Y, m_Z	Slice normal axis
//	m_XC
//	m_YC		Slice centre as fractions of SimBox side lengths
//  m_ZC
//	m_XHalfWidth
//	m_YHalfWidth	Slice half-widths as fractions of SimBox side lengths
//  m_ZHalfWidth

zOutStream& ccSelectBeadTypeInSlice::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ccSelectBeadTypeInRegion::put(os);

	os << "<XNormal>"    << m_X			 << "</XNormal>" << zEndl;
	os << "<YNormal>"    << m_Y			 << "</YNormal>" << zEndl;
	os << "<ZNormal>"    << m_Z			 << "</ZNormal>" << zEndl;
	os << "<XCentre>"    << m_XC	     << "</XCentre>" << zEndl;
	os << "<YCentre>"    << m_YC	     << "</YCentre>" << zEndl;
	os << "<ZCentre>"    << m_ZC	     << "</ZCentre>" << zEndl;
	os << "<XHalfWidth>" << m_XHalfWidth << "</XHalfWidth>" << zEndl;
	os << "<YHalfWidth>" << m_YHalfWidth << "</YHalfWidth>" << zEndl;
	os << "<ZHalfWidth>" << m_ZHalfWidth << "</ZHalfWidth>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ccSelectBeadTypeInRegion::put(os);
	os << " " << m_X   << " " << m_Y  << " " << m_Z;
	os << " " << m_XC  << " " << m_YC << " " << m_ZC;
	os << " " << m_XHalfWidth << " " << m_YHalfWidth  << " " << m_ZHalfWidth;
	putASCIIEndTags(os);

#endif

	return os;

}

zInStream& ccSelectBeadTypeInSlice::get(zInStream& is)
{
	// Read the base class data first

	ccSelectBeadTypeInRegion::get(is);

	// We perform some simple checks on the data here, but more complicated
	// validation is done in IsDataValid() below as it has access to CInputData.

	is >> m_X >> m_Y >> m_Z;

	if(!is.good() || (m_X == 0 && m_Y == 0 && m_Z == 0) ||
		              m_X < 0  || m_Y < 0  || m_Z < 0)
	   SetCommandValid(false);

	is >> m_XC >> m_YC >> m_ZC;

	if(!is.good() || m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
					 m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
	   SetCommandValid(false);

	is >> m_XHalfWidth >> m_YHalfWidth >> m_ZHalfWidth;

	if(!is.good() || m_XHalfWidth < 0.0 || m_YHalfWidth < 0.0 || m_ZHalfWidth < 0.0 ||
					 m_XHalfWidth > 0.5 || m_YHalfWidth > 0.5 || m_ZHalfWidth > 0.5)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ccSelectBeadTypeInSlice::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSelectBeadTypeInSlice::GetCommand() const
{
	return new ccSelectBeadTypeInSlice(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccSelectBeadTypeInSlice::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SelectBeadTypeInSlice(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the cylindrical region from
// which to select beads is valid. The target and and bead type are 
// validated in the base class. Here we check that the extent of the slice
// does not go outside the SimBox boundaries. If it does, the command fails.
// We also check that the normal is only along one of the major axes.

bool ccSelectBeadTypeInSlice::IsDataValid(const CInputData& riData) const
{
	if( (m_XC - m_XHalfWidth < 0.0 || m_XC + m_XHalfWidth > 1.0) ||
		(m_YC - m_YHalfWidth < 0.0 || m_YC + m_YHalfWidth > 1.0) ||
		(m_ZC - m_ZHalfWidth < 0.0 || m_ZC + m_ZHalfWidth > 1.0))
			return false;
	else if( (m_X != 1 && m_Y != 1 && m_Z != 1) ||
		     (m_X*m_X + m_Y*m_Y + m_Z*m_Z) != 1)
		return false;

	return ccSelectBeadTypeInRegion::IsDataValid(riData);
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
// m_TargetLabel     - zString
// m_BeadName        - zString
// m_X, m_Y, m_Z     - long, long, long
// m_XC, m_YC, m_ZC  - double, double, double
// m_XHalfWidth, m_YHalfWidth, m_ZHalfWidth  - double, double, double

bool ccSelectBeadTypeInSlice::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(ccSelectBeadTypeInRegion::Pack(vArguments) && static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(2)->GetValue(&m_X);
        vArguments.at(3)->GetValue(&m_Y);
        vArguments.at(4)->GetValue(&m_Z);
        vArguments.at(5)->GetValue(&m_XC);
        vArguments.at(6)->GetValue(&m_YC);
        vArguments.at(7)->GetValue(&m_ZC);
        vArguments.at(8)->GetValue(&m_XHalfWidth);
        vArguments.at(9)->GetValue(&m_YHalfWidth);
        vArguments.at(10)->GetValue(&m_ZHalfWidth);

	    if((m_X == 0 && m_Y == 0 && m_Z == 0)   ||
		   (m_X < 0  || m_Y < 0  || m_Z < 0)    ||
           (m_X != 1 && m_Y != 1 && m_Z != 1)   ||
           ((m_X*m_X + m_Y*m_Y + m_Z*m_Z) != 1) ||
            m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
            m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0 ||
            m_XHalfWidth < 0.0 || m_YHalfWidth < 0.0 || m_ZHalfWidth < 0.0 ||
		    m_XHalfWidth > 0.5 || m_YHalfWidth > 0.5 || m_ZHalfWidth > 0.5 ||
           (m_XC - m_XHalfWidth < 0.0 || m_XC + m_XHalfWidth > 1.0) ||
           (m_YC - m_YHalfWidth < 0.0 || m_YC + m_YHalfWidth > 1.0) ||
           (m_ZC - m_ZHalfWidth < 0.0 || m_ZC + m_ZHalfWidth > 1.0))
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
