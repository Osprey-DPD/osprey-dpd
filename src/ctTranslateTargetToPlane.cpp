/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctTranslateTargetToPlane.cpp: implementation of the ctTranslateTargetToPlane class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctTranslateTargetToPlane.h"
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

const zString ctTranslateTargetToPlane::m_Type = "TranslateTargetToPlane";

const zString ctTranslateTargetToPlane::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctTranslateTargetToPlane::m_ArgumentTotal = 9;

long ctTranslateTargetToPlane::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctTranslateTargetToPlane(executionTime);}

	const zString id = ctTranslateTargetToPlane::GetType();
    const long  argTotal = ctTranslateTargetToPlane::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctTranslateTargetToPlane::ctTranslateTargetToPlane(long executionTime) : ctApplyCommand(executionTime),
											m_DecLabel(""), 
											m_XN(0),   m_YN(0),   m_ZN(0), 
											m_XC(0.0), m_YC(0.0), m_ZC(0.0)
{
}

ctTranslateTargetToPlane::ctTranslateTargetToPlane(const ctTranslateTargetToPlane& oldCommand) : ctApplyCommand(oldCommand),
												m_DecLabel(oldCommand.m_DecLabel),
												m_XN(oldCommand.m_XN),
												m_YN(oldCommand.m_YN),
												m_ZN(oldCommand.m_ZN),
												m_XC(oldCommand.m_XC),
												m_YC(oldCommand.m_YC),
												m_ZC(oldCommand.m_ZC)
{
}

// Constructor for use when creating the command internally.

ctTranslateTargetToPlane::ctTranslateTargetToPlane(long executionTime, const zString targetLabel, const zString decLabel, 
											long xn, long yn, long zn, 
											double xc, double yc, double zc) : ctApplyCommand(executionTime, targetLabel),
											m_DecLabel(decLabel), m_XN(xn), m_YN(yn), m_ZN(zn), 
											m_XC(xc), m_YC(yc), m_ZC(zc)

{
}

ctTranslateTargetToPlane::~ctTranslateTargetToPlane()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_DecLabel      label for this decorator object so that it can be deleted later
//  m_XN             
//  m_YN			Normal vector to the plane 
//  m_ZN 
//	m_XC
//	m_YC			Arbitrary point on the plane
//	m_ZC
//

zOutStream& ctTranslateTargetToPlane::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	os << "<DecoratorLabel>" << m_DecLabel << "</DecoratorLabel>" << zEndl;
	os << "<XN>" << m_XN << "</XN>" << "<YN>" << m_YN << "</YN>" << "<ZN>" << m_ZN << "</ZN>" << zEndl;
	os << "<XC>" << m_XC << "</XC>" << "<YC>" << m_YC << "</YC>" << "<ZC>" << m_ZC << "</ZC>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_DecLabel << "  " << m_XN << " " << m_YN << " " << m_ZN;
	os << "  " << m_XC << " " << m_YC << " " << m_ZC;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctTranslateTargetToPlane::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Now read the local data.

	is >> m_DecLabel >> m_XN >> m_YN >> m_ZN >> m_XC >> m_YC >> m_ZC;
	
	if(!is.good())
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctTranslateTargetToPlane::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctTranslateTargetToPlane::GetCommand() const
{
	return new ctTranslateTargetToPlane(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctTranslateTargetToPlane::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
//		pISimCmd->TranslateTargetToPlane(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. 
// We check that the plane normal is not zero, and that the centre coordinates lie within the 
// SimBox, i.e., (0,1).

bool ctTranslateTargetToPlane::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_DecLabel))
	{
		return ErrorTrace("Translate target to plane active command target name invalid");
	}
	else if(riData.IsActiveCommandTargetPresent(m_DecLabel))
	{
		return ErrorTrace("Translate target to plane command target already exists");
	}
	else if(m_XN == 0 && m_YN == 0 && m_ZN == 0)
	{
		return ErrorTrace("Translate target to plane active command target invalid normal");
	}
	else if(m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
	        m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
	{
		return ErrorTrace("Translate target to plane active command target invalid axis origin");
	}
	else
	{
		// New target is unique so add it to the container in the CInputData

		CInputData& rincData = const_cast<CInputData&>(riData);

		rincData.AddActiveCommandTargetName(m_DecLabel);

		return ctApplyCommand::IsDataValid(riData);
	}
}
