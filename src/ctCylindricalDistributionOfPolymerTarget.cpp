/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctCylindricalDistributionOfPolymerTarget.cpp: implementation of the ctCylindricalDistributionOfPolymerTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctCylindricalDistributionOfPolymerTarget.h"
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

const zString ctCylindricalDistributionOfPolymerTarget::m_Type = "CylindricalDistributionOfPolymerTarget";

const zString ctCylindricalDistributionOfPolymerTarget::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctCylindricalDistributionOfPolymerTarget::m_ArgumentTotal = 13;

long ctCylindricalDistributionOfPolymerTarget::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ctCylindricalDistributionOfPolymerTarget(executionTime);}

	const zString id = ctCylindricalDistributionOfPolymerTarget::GetType();
    const long  argTotal = ctCylindricalDistributionOfPolymerTarget::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctCylindricalDistributionOfPolymerTarget::ctCylindricalDistributionOfPolymerTarget(long executionTime) : ctApplyCommand(executionTime),
											m_DecLabel(""), m_Start(0), m_End(0), m_Samples(0),
											m_XN(0),   m_YN(0),   m_ZN(0), m_XC(0.0), m_YC(0.0), m_ZC(0.0),
                                            m_ShellWidth(0.0), m_OuterRadius(0.0)
{
}

ctCylindricalDistributionOfPolymerTarget::ctCylindricalDistributionOfPolymerTarget(const ctCylindricalDistributionOfPolymerTarget& oldCommand) : ctApplyCommand(oldCommand), m_DecLabel(oldCommand.m_DecLabel),
                                m_Start(oldCommand.m_Start),
                                m_End(oldCommand.m_End),
                                m_Samples(oldCommand.m_Samples),
                                m_XN(oldCommand.m_XN),
                                m_YN(oldCommand.m_YN),
                                m_ZN(oldCommand.m_ZN),
                                m_XC(oldCommand.m_XC),
                                m_YC(oldCommand.m_YC),
                                m_ZC(oldCommand.m_ZC),
                                m_ShellWidth(oldCommand.m_ShellWidth),
                                m_OuterRadius(oldCommand.m_OuterRadius)
{
}

// Constructor for use when creating the command internally.

ctCylindricalDistributionOfPolymerTarget::ctCylindricalDistributionOfPolymerTarget(long executionTime, const zString targetLabel,
                            const zString decLabel, long start, long end, long samples,
                            long xn, long yn, long zn, double xc, double yc, double zc,
                            double shellWidth, double outerRadius) : ctApplyCommand(executionTime, targetLabel),
                            m_DecLabel(decLabel), m_Start(start), m_End(end), m_Samples(samples),
                            m_XN(xn), m_YN(yn), m_ZN(zn),
                            m_XC(xc), m_YC(yc), m_ZC(zc), m_ShellWidth(shellWidth), m_OuterRadius(outerRadius)
{
}

ctCylindricalDistributionOfPolymerTarget::~ctCylindricalDistributionOfPolymerTarget()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_DecLabel      label for this decorator object so that it can be deleted later
//  m_Start         time at which to start analysis
//  m_End           time at which to end analysis
//  m_Samples       multiple of the SamplePeriod at which to sample the data
//  m_XN             
//  m_YN			Unit vector along cylinder axis
//  m_ZN 
//	m_XC
//	m_YC			Arbitrary point on axis labelled as the "origin" as fractions of SimBox size
//	m_ZC
//	m_ShellWidth	width of the cylindrical shells in which to bin the polymer coordinates
//  m_OuterRadius   outer radius of analysis

zOutStream& ctCylindricalDistributionOfPolymerTarget::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);

	// Put the base class data first
	ctApplyCommand::put(os);

	os << "<DecoratorLabel>" << m_DecLabel << "</DecoratorLabel>" << zEndl;
    os << "<StartTime>" << m_Start << "</StartTime>" << "<EndTime>" << m_End << "</EndTime>" << "<Samples>" << m_Samples << "</Samples>" << zEndl;
    os << "<XN>" << m_XN << "</XN>" << "<YN>" << m_YN << "</YN>" << "<ZN>" << m_ZN << "</ZN>" << zEndl;
	os << "<XC>" << m_XC << "</XC>" << "<YC>" << m_YC << "</YC>" << "<ZC>" << m_ZC << "</ZC>" << zEndl;
    os << "<ShellWidth>" << m_ShellWidth << "</ShellWidth>" << zEndl;
    os << "<OuterRadius>" << m_OuterRadius << "</OuterRadius>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ctApplyCommand::put(os);
	os << m_DecLabel << "  " << m_Start << " " << m_End << " " << m_Samples << " " << m_XN << " " << m_YN << " " << m_ZN;
	os << "  " << m_XC << " " << m_YC << " " << m_ZC << "  " << m_ShellWidth << " " << m_OuterRadius;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctCylindricalDistributionOfPolymerTarget::get(zInStream& is)
{
	// Read the base class data first

	ctApplyCommand::get(is);

	// Now read the local data.

	is >> m_DecLabel >> m_Start >> m_End >> m_Samples >> m_XN >> m_YN >> m_ZN >> m_XC >> m_YC >> m_ZC >> m_ShellWidth >> m_OuterRadius;

	if(!is.good())
		SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString ctCylindricalDistributionOfPolymerTarget::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctCylindricalDistributionOfPolymerTarget::GetCommand() const
{
	return new ctCylindricalDistributionOfPolymerTarget(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctCylindricalDistributionOfPolymerTarget::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->CylindricalDistributionOfPolymerTarget(this);
		return true;
	}
	else
		return false;
}

// Function to check that the command data are valid. 
// We check that the normal is not zero, the centre coordinates lie within the 
// SimBox, i.e., (0,1) and the magnitude is greater or equal to zero.

bool ctCylindricalDistributionOfPolymerTarget::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_DecLabel))
	{
		return ErrorTrace("Cylindrical distribution active command target name invalid");
	}
	else if(riData.IsActiveCommandTargetPresent(m_DecLabel))
	{
		return ErrorTrace("Cylindrical distribution active command target already exists");
	}
    else if(m_Start < 0 || m_End < m_Start)
    {
        return ErrorTrace("Cylindrical distribution illegal start/end times");
    }
    else if(m_Samples < 1)
    {
        return ErrorTrace("Cylindrical distribution illegal SamplePeriod multiplier");
    }
    else if(m_XN == 0 && m_YN == 0 && m_ZN == 0)
    {
        return ErrorTrace("Cylindrical distribution illegal axis normal vector");
    }
	else if(m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
	        m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
	{
		return ErrorTrace("Cylindrical distribution invalid point on axis fractions");
	}
    else if(m_ShellWidth < 0.0)
    {
        return ErrorTrace("Cylindrical distribution invalid shell width");
    }
    else if(m_OuterRadius < 1.0 || m_OuterRadius < m_ShellWidth)
    {
        return ErrorTrace("Cylindrical distribution invalid outer radius");
    }
	else
	{
		// New target is unique so add it to the container in the CInputData

		CInputData& rincData = const_cast<CInputData&>(riData);

		rincData.AddActiveCommandTargetName(m_DecLabel);

		return ctApplyCommand::IsDataValid(riData);
	}
}
