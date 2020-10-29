/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctTranslateTargetToC2Point.cpp: implementation of the ctTranslateTargetToC2Point class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctTranslateTargetToC2Point.h"
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

const zString ctTranslateTargetToC2Point::m_Type = "TranslateTargetToC2Point";

const zString ctTranslateTargetToC2Point::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctTranslateTargetToC2Point::m_ArgumentTotal = 8;

long ctTranslateTargetToC2Point::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctTranslateTargetToC2Point(executionTime);}

	const zString id = ctTranslateTargetToC2Point::GetType();
    const long  argTotal = ctTranslateTargetToC2Point::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctTranslateTargetToC2Point::ctTranslateTargetToC2Point(long executionTime) : ctApplyCommand(executionTime), m_XN(0), m_YN(0), m_ZN(0), 
                                                       m_XC(0.0), m_YC(0.0), m_ZC(0.0), m_Separation(0.0)
{
}

ctTranslateTargetToC2Point::ctTranslateTargetToC2Point(const ctTranslateTargetToC2Point& oldCommand) : ctApplyCommand(oldCommand),
                                                       m_XN(oldCommand.m_XN), m_YN(oldCommand.m_YN), m_ZN(oldCommand.m_ZN),
                                                       m_XC(oldCommand.m_XC), m_YC(oldCommand.m_YC), m_ZC(oldCommand.m_ZC),
                                                       m_Separation(oldCommand.m_Separation)
{
}

// Constructor for use when creating the command internally.

ctTranslateTargetToC2Point::ctTranslateTargetToC2Point(long executionTime, const zString targetLabel, long xn, long yn, long zn, 
                                                       double xc, double yc, double zc, double separation) : ctApplyCommand(executionTime, targetLabel),
                                                       m_XN(xn), m_YN(yn), m_ZN(zn), m_XC(xc), m_YC(yc), m_ZC(zc), m_Separation(separation)

{
}

ctTranslateTargetToC2Point::~ctTranslateTargetToC2Point()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_XN            C2 axis normal vector
//	m_YN            
//	m_ZN            
//	m_XC            Fractional distance of the point in the X dimension 
//	m_YC            Fractional distance of the point in the Y dimension 
//	m_ZC            Fractional distance of the point in the Z dimension 
//  m_Separation    C2 axis separation distance

zOutStream& ctTranslateTargetToC2Point::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	os << "<XN>" << m_XN << "</XN>" << "<YN>" << m_YN << "</YN>" << "<ZN>" << m_ZN << "</ZN>";
	os << "<XC>" << m_XC << "</XC>" << "<YC>" << m_YC << "</YC>" << "<ZC>" << m_ZC << "</ZC>" 
    os << "<Separation>" << m_Separation << "</Separation>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_XN << " " << m_YN << " " << m_ZN << " " << m_XC << " " << m_YC << " " << m_ZC << " " << m_Separation;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctTranslateTargetToC2Point::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Now read the local data.

	is >> m_XN >> m_YN >> m_ZN >> m_XC >> m_YC >> m_ZC >> m_Separation;
	
	if(!is.good())
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctTranslateTargetToC2Point::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctTranslateTargetToC2Point::GetCommand() const
{
	return new ctTranslateTargetToC2Point(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctTranslateTargetToC2Point::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->TranslateTargetToC2Point(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. 
// We check that the C2 axis normal and separation are ok, and the fractional fixed point coordinates lie within the 
// SimBox, i.e., (0,1).

bool ctTranslateTargetToC2Point::IsDataValid(const CInputData& riData) const
{
	if(m_XN < 0 || m_YN < 0 || m_ZN < 0)
    {
		return ErrorTrace("Translate target to C2 point invalid axis normal");        
    }
    else if(m_XC < 0.0 || m_XC > 1.0 || m_YC < 0.0 || m_YC > 1.0 || m_ZC < 0.0 || m_ZC > 1.0)
	{
		return ErrorTrace("Translate target to C2 point invalid origin");
	}
    else if(m_Separation < 0.0)
    {
		return ErrorTrace("Translate target to C2 point invalid axis separation");
    }
	
	return ctApplyCommand::IsDataValid(riData);
}
