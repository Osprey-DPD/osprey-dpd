/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveBeadKineticTemperatures.cpp: implementation of the mcSaveBeadKineticTemperatures class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveBeadKineticTemperatures.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString mcSaveBeadKineticTemperatures::m_Type = "SaveBeadKineticTemperatures";

const zString mcSaveBeadKineticTemperatures::GetType()
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
	xxCommand* Create(long executionTime) {return new mcSaveBeadKineticTemperatures(executionTime);}

	const zString id = mcSaveBeadKineticTemperatures::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveBeadKineticTemperatures::mcSaveBeadKineticTemperatures(long executionTime) : xxCommand(executionTime),
														m_TotalAnalysisPeriods(0),
														m_TotalDataPoints(0)
{

}

mcSaveBeadKineticTemperatures::mcSaveBeadKineticTemperatures(const mcSaveBeadKineticTemperatures& oldCommand) : xxCommand(oldCommand),
														m_TotalAnalysisPeriods(oldCommand.m_TotalAnalysisPeriods),
														m_TotalDataPoints(oldCommand.m_TotalDataPoints)
{
}


mcSaveBeadKineticTemperatures::~mcSaveBeadKineticTemperatures()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_TotalAnalysisPeriods	- Number of analysis periods to sample over
//  m_TotalDataPoints       - Number of data points to write out


zOutStream& mcSaveBeadKineticTemperatures::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<AnalysisPeriods>" << m_TotalAnalysisPeriods << "</AnalysisPeriods>" << zEndl;
	os << "<DataPoints>"      << m_TotalDataPoints      << "</DataPoints>"      << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_TotalAnalysisPeriods << " " << m_TotalDataPoints;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mcSaveBeadKineticTemperatures::get(zInStream& is)
{
	// Check that the number of analysis periods is positive definite.
    // We check that there is at least one period left in the run
    // in the IsDataValid() function below.

	is >> m_TotalAnalysisPeriods;

	if(!is.good() || m_TotalAnalysisPeriods < 1)
	   SetCommandValid(false);

	// Check that the number of data points is positive definite. 
    // We check that there is at least one sample possible given the current
    // value of the simulation sampling period below.

	is >> m_TotalDataPoints;

	if(!is.good() || m_TotalDataPoints < 1)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString mcSaveBeadKineticTemperatures::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcSaveBeadKineticTemperatures::GetCommand() const
{
	return new mcSaveBeadKineticTemperatures(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool mcSaveBeadKineticTemperatures::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SaveBeadKineticTemperatures(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid. We check that
// the specified parameters fit into the remaining simulation time, and yield
// a positive number of samples. Note that the analysis starts at the beginning
// of the next full analysis period and continues for an integer number of periods.

bool mcSaveBeadKineticTemperatures::IsDataValid(const CInputData& riData) const
{
    long currentTime  = GetExecutionTime();
    long duration     = m_TotalAnalysisPeriods*riData.GetAnalysisPeriod();
    long samples      = duration/riData.GetSamplePeriod();

    // The analysis starts at the beginning of the next full analysis period 
    // and continues for an integer number of periods.

    long start = 0;
    long end   = 0;

    if(currentTime%riData.GetAnalysisPeriod() == 0)
    {
        start = currentTime;
    }
    else
    {
        start = (currentTime/riData.GetAnalysisPeriod() + 1)*riData.GetAnalysisPeriod();
    }

    end = start + duration;

    if( end > riData.GetTotalTime() || duration%riData.GetSamplePeriod() != 0 || 
        m_TotalDataPoints == 0 || samples%m_TotalDataPoints != 0)
    {
		return ErrorTrace("Invalid duration or number of data points for bead kinetic temperature analysis");
    }
    else
	    return true;
}
