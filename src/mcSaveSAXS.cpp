/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveSAXS.cpp: implementation of the mcSaveSAXS class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveSAXS.h"
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

const zString mcSaveSAXS::m_Type = "SaveSAXS";

const zString mcSaveSAXS::GetType()
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
	xxCommand* Create(long executionTime) {return new mcSaveSAXS(executionTime);}

	const zString id = mcSaveSAXS::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveSAXS::mcSaveSAXS(long executionTime) : xxCommand(executionTime),
														m_TotalAnalysisPeriods(0),
														m_TotalDataPoints(0), m_QMin(0.0), m_QMax(0.0)
{
    m_vIncludedBeads.clear();
}

mcSaveSAXS::mcSaveSAXS(const mcSaveSAXS& oldCommand) : xxCommand(oldCommand),
                            m_TotalAnalysisPeriods(oldCommand.m_TotalAnalysisPeriods),
                            m_TotalDataPoints(oldCommand.m_TotalDataPoints),
                            m_QMin(oldCommand.m_QMin),
                            m_QMax(oldCommand.m_QMax),
                            m_vIncludedBeads(oldCommand.m_vIncludedBeads)
{
}


mcSaveSAXS::~mcSaveSAXS()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_TotalAnalysisPeriods	- Number of analysis periods to sample over
//  m_TotalDataPoints       - Number of Q values in the I(q) curve
//  m_QMin                  - Minimum q value in range
//  m_QMax                  - Maximum q value in range
//  m_vIncludedBeads        - Boolean vector showing which bead types to include, allowing for dynamically-created types too


zOutStream& mcSaveSAXS::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<AnalysisPeriods>" << m_TotalAnalysisPeriods << "</AnalysisPeriods>" << zEndl;
    os << "<DataPoints>"      << m_TotalDataPoints      << "</DataPoints>"      << zEndl;
    os << "<QMin>"            << m_QMin                 << "</QMin>"      << zEndl;
    os << "<QMax>"            << m_QMax                 << "</QMax>"      << zEndl;

    for(czBoolVectorIterator citer=m_vIncludedBeads.begin(); citer!=m_vIncludedBeads.end(); citer++)
    {
        os << "<Included>" << *citer << "<Include>" << zEndl;
    }
    putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
    os << " " << m_TotalAnalysisPeriods << " " << m_TotalDataPoints << " " << m_QMin << " " << m_QMax << " ";
    
    for(czBoolVectorIterator citer=m_vIncludedBeads.begin(); citer!=m_vIncludedBeads.end(); citer++)
    {
        os << *citer << " ";
    }
    os << zEndl;

	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mcSaveSAXS::get(zInStream& is)
{
	// Check that the number of analysis periods is positive definite.
    // We check that there is at least one period left in the run
    // in the IsDataValid() function below.
    
  	is >> m_TotalAnalysisPeriods;

	if(!is.good())
	   SetCommandValid(false);

    // Check that the number of data points is positive definite.
    // We check that there is at least one sample possible given the current
    // value of the simulation sampling period below.

    is >> m_TotalDataPoints;

    if(!is.good())
       SetCommandValid(false);

    // Get the minimum and maximum q values in the range. They must not be negative and qmin must be greater
    // than zero unless both are zero, which indicates that the code should use the default range.
    
    is >> m_QMin;

    if(!is.good())
       SetCommandValid(false);
       
    is >> m_QMax;

    if(!is.good())
       SetCommandValid(false);
       
	   
	// Store which bead types should be included/excluded from the calculation. We don't check that the correct number
    // is entered here because we allow for bead types that will be created by command during the simulation. The set of entries
    // is terminated by -1.
	
    long value = 0;
    
    while(value != -1)
    {
        is >> value;
        if(is.good())
        {
            if(value == 1)
            {
                m_vIncludedBeads.push_back(true);
            }
            else if(value == 0)
            {
                m_vIncludedBeads.push_back(false);
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

const zString mcSaveSAXS::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcSaveSAXS::GetCommand() const
{
	return new mcSaveSAXS(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool mcSaveSAXS::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SaveSAXS(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid. Note that the analysis starts at the beginning
// of the next full analysis period and continues for an integer number of periods. We also ensure the
// maximum range is less than the box size and that the polymer and bead names are valid. We check that the polymer
// type exists but not the bead type so that types created dynamically by command can also have their SAXS pattern calculated.
// Note that polymers cannot be created or renamed by command.

bool mcSaveSAXS::IsDataValid(const CInputData& riData) const
{
    // Check the range of the q values, and the numbers of data points.
    
    if(m_QMin < 0.0 || m_QMax < 0.0)
    {
        return ErrorTrace("Invalid scattering vector range (negative endpoint)");
    }
    else if(m_QMin > 0.0 && m_QMax == m_QMin)
    {
        return ErrorTrace("Invalid scattering vector range (equal endpoints)");
    }
    else if( m_QMax < m_QMin)
    {
        return ErrorTrace("Invalid scattering vector range (upper endpoint smaller than lower)");
    }
    else if (m_TotalDataPoints < 1)
    {
        return ErrorTrace("No q values specified");
    }
    else if (m_TotalAnalysisPeriods < 1)
    {
        return ErrorTrace("At least one analysis period must be specified");
    }
           
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


    if( end > riData.GetTotalTime())
        return ErrorTrace("Invalid duration for SAXS analysis, total time="+std::to_string(riData.GetTotalTime())+", end="+std::to_string(end));
    else if (duration%riData.GetSamplePeriod() != 0)
		return ErrorTrace("Duration is not a multiple of SamplePeriod for SAXS analysis");
    else
	    return true;
}
