/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSavePairCorrelationFunction.cpp: implementation of the mcSavePairCorrelationFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSavePairCorrelationFunction.h"
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

const zString mcSavePairCorrelationFunction::m_Type = "SavePairCorrelationFunction";

const zString mcSavePairCorrelationFunction::GetType()
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
	xxCommand* Create(long executionTime) {return new mcSavePairCorrelationFunction(executionTime);}

	const zString id = mcSavePairCorrelationFunction::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSavePairCorrelationFunction::mcSavePairCorrelationFunction(long executionTime) : xxCommand(executionTime),
														m_TotalAnalysisPeriods(0),
														m_TotalDataPoints(0), m_RMax(1.0)
{
    m_vExcludedPolymers.clear();
}

mcSavePairCorrelationFunction::mcSavePairCorrelationFunction(const mcSavePairCorrelationFunction& oldCommand) : xxCommand(oldCommand),  m_TotalAnalysisPeriods(oldCommand.m_TotalAnalysisPeriods),
                            m_TotalDataPoints(oldCommand.m_TotalDataPoints),
                            m_RMax(oldCommand.m_RMax), m_vExcludedPolymers(oldCommand.m_vExcludedPolymers)
{
}


mcSavePairCorrelationFunction::~mcSavePairCorrelationFunction()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_TotalAnalysisPeriods	- Number of analysis periods to sample over
//  m_TotalDataPoints       - Number of bins in the histogram
//  m_RMax                  - Maximum separation out to which the histogram is calculated
//  m_ExcludedPolymers      - Boolean vector showing which polymers to exclude and include


zOutStream& mcSavePairCorrelationFunction::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<AnalysisPeriods>" << m_TotalAnalysisPeriods << "</AnalysisPeriods>" << zEndl;
	os << "<DataPoints>"      << m_TotalDataPoints      << "</DataPoints>"      << zEndl;
	os << "<RMax>"            << m_RMax                 << "</RMax>"            << zEndl;

    for(czBoolVectorIterator citer=m_vExcludedPolymers.begin(); citer!=m_vExcludedPolymers.end(); citer++)
    {
        os << "<Exclude>" << *citer << "<Exclude>" << zEndl;
    }
    putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
    os << " " << m_TotalAnalysisPeriods << " " << m_TotalDataPoints << " " << m_RMax << " ";
    
    for(czBoolVectorIterator citer=m_vExcludedPolymers.begin(); citer!=m_vExcludedPolymers.end(); citer++)
    {
        os << *citer << " ";
    }
    os << zEndl;

	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mcSavePairCorrelationFunction::get(zInStream& is)
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
	   
	// Get the maximum separation but we leave the check that it is less than the box size to the IsDataValid() function.

	is >> m_RMax;

	if(!is.good() || m_RMax < 0.0)
	   SetCommandValid(false);

	// Store which polymers should be included/excluded from the calculation. We don't check that the correct number
    // is entered here, but do it in the Validate function. We need to know the total number of
    // polymer types.
	
    long value = 0;
    
    while(value != -1)
    {
        is >> value;
        if(is.good())
        {
            if(value == 0)
            {
                m_vExcludedPolymers.push_back(1);
            }
            else if(value == 1)
            {
                m_vExcludedPolymers.push_back(0);
            }
        }
        else
        {
            SetCommandValid(false);
            return is;
        }
    }

	return is;
}

// Non-static function to return the type of the command

const zString mcSavePairCorrelationFunction::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcSavePairCorrelationFunction::GetCommand() const
{
	return new mcSavePairCorrelationFunction(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool mcSavePairCorrelationFunction::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SavePairCorrelationFunction(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid. Note that the analysis starts at the beginning
// of the next full analysis period and continues for an integer number of periods. We also ensure the
// maximum range is less than the box size and that the polymer and bead names are valid. We check that the polymer
// type exists but not the bead type so that types created dynamically by command can also have their RDF calculated.
// Note that polymers cannot be created or renamed by command.

bool mcSavePairCorrelationFunction::IsDataValid(const CInputData& riData) const
{
    long currentTime  = GetExecutionTime();
    long duration     = m_TotalAnalysisPeriods*riData.GetAnalysisPeriod();

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
    
    if( end > riData.GetTotalTime() || duration%riData.GetSamplePeriod() != 0)
		return ErrorTrace("Invalid duration or multiple of SamplePeriod for polymer bead RDF analysis");
	else if(m_RMax > riData.GetSimBoxXLength() || m_RMax > riData.GetSimBoxYLength() || m_RMax > riData.GetSimBoxZLength())
		return ErrorTrace("Max separation larger than box size");
	else if( m_vExcludedPolymers.size() != riData.GetPolymerTypeTotal())
        return ErrorTrace("Excluded polymer vector has wrong size");
    else
	    return true;
}
