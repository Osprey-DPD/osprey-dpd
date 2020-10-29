/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveBead1dDensityProfile.cpp: implementation of the mcSaveBead1dDensityProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveBead1dDensityProfile.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "LogSaveBead1dDensityProfile.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString mcSaveBead1dDensityProfile::m_Type = "SaveBead1dDensityProfile";

const zString mcSaveBead1dDensityProfile::GetType()
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
	xxCommand* Create(long executionTime) {return new mcSaveBead1dDensityProfile(executionTime);}

	const zString id = mcSaveBead1dDensityProfile::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveBead1dDensityProfile::mcSaveBead1dDensityProfile(long executionTime) : xxCommand(executionTime),
														m_BeadType(-1),
														m_Start(0), m_End(0),
														m_SamplePeriod(0), 
														m_NX(0), m_NY(0), m_NZ(0),
														m_SliceTotal(0),
														m_LowerFraction(0.0), m_UpperFraction(1.0)
{

}

mcSaveBead1dDensityProfile::mcSaveBead1dDensityProfile(const mcSaveBead1dDensityProfile& oldCommand) : xxCommand(oldCommand),
														m_BeadType(oldCommand.m_BeadType),
														m_Start(oldCommand.m_Start), 
														m_End(oldCommand.m_End),
														m_SamplePeriod(oldCommand.m_SamplePeriod), 
														m_NX(oldCommand.m_NX), 
														m_NY(oldCommand.m_NY), 
														m_NZ(oldCommand.m_NZ),
														m_SliceTotal(oldCommand.m_SliceTotal),
														m_LowerFraction(oldCommand.m_LowerFraction), 
														m_UpperFraction(oldCommand.m_UpperFraction)

{
}


mcSaveBead1dDensityProfile::~mcSaveBead1dDensityProfile()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_BeadType			- Numeric bead type to modify
//	m_Start				- Simulation time at which density field analysis starts
//	m_End				- Simulation time at which density field analysis ends
//	m_SamplePeriod		- No of time steps between sampling the data
//  m_NX, m_NY, m_NZ	- Direction of density profile through SimBox
//	m_SliceTotal		- No of slices in the profile
//	m_LowerFraction		- Minimum fractional value of the coordinate in the profile direction
//	m_UpperFraction		- Maximum fractional value of the coordinate in the profile direction


zOutStream& mcSaveBead1dDensityProfile::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<BeadType>"       << m_BeadType     << "</BeadType>"     << zEndl;
	os << "<Start>"          << m_Start        << "</Start>"        << zEndl;
	os << "<End>"            << m_End          << "</End>"          << zEndl;
	os << "<SamplePeriod>"   << m_SamplePeriod << "</SamplePeriod>" << zEndl;
	os << "<NX>" << m_NX << "</NX>" << zEndl;
	os << "<NY>" << m_NY << "</NY>" << zEndl;
	os << "<NZ>" << m_NZ << "</NZ>" << zEndl;
	os << "<SliceTotal>"     << m_SliceTotal   << "</SliceTotal>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_BeadType << " " << m_Start << " " << m_End << " " << m_SamplePeriod;
    os << " " << m_NX << " " << m_NY << " " << m_NZ;
    os << " " << m_SliceTotal;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mcSaveBead1dDensityProfile::get(zInStream& is)
{
	// Check that the bead type is non-negative
	is >> m_BeadType;

	if(!is.good() || m_BeadType < 0)
	   SetCommandValid(false);

	// Check that the sample period is a factor of the total sampling time 
	// defined by the start and end times, and the normal is non-zero, and the 
	// range of coordinates is well-defined. We perform more complicated checks 
	// in the IsDataValid() function below.

	is >> m_Start >> m_End >> m_SamplePeriod;

	if(!is.good())
	   SetCommandValid(false);

	is >> m_NX >> m_NY >> m_NZ;

	if(!is.good() ||
       !((m_NX == 1 && m_NY == 0 && m_NZ == 0) ||
	     (m_NX == 0 && m_NY == 1 && m_NZ == 0) ||
         (m_NX == 0 && m_NY == 0 && m_NZ == 1)) )
	   SetCommandValid(false);

	is >> m_SliceTotal;

	if(!is.good() || m_SliceTotal < 1)
	   SetCommandValid(false);

	/*
	// These input parameters are disabled because it is difficult to take
	// an average over the density profile if a different number of beads are counted
	// within the range of the profile. Using the whole SimBox for the profile ensures
	// that the total beads counted is always the same.

	is m_LowerFraction >> m_UpperFraction;

	if(!is.good() || m_LowerFraction < 0.0 || m_UpperFraction < m_LowerFraction || m_UpperFraction > 1.0)
	   SetCommandValid(false);
*/
	return is;
}

// Non-static function to return the type of the command

const zString mcSaveBead1dDensityProfile::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcSaveBead1dDensityProfile::GetCommand() const
{
	return new mcSaveBead1dDensityProfile(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool mcSaveBead1dDensityProfile::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SaveBead1dDensityProfile(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid. Most parameters
// have been validated in the get() function above, here we just check that
// the start and end of the sampling period range are within the total simulation 
// time.

bool mcSaveBead1dDensityProfile::IsDataValid(const CInputData& riData) const
{
    if(m_Start < 1 || m_End < m_Start || m_SamplePeriod < 1 ||
         m_End > riData.GetTotalTime() || (m_End + 1 - m_Start) % m_SamplePeriod != 0)
    {
		return ErrorTrace("Invalid or incommensurate sampling period for 1d bead density profile");
    }
    else
	    return true;
}
