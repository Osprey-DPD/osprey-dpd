/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveBeadDensityFluctuationsByType.cpp: implementation of the mcSaveBeadDensityFluctuationsByType class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveBeadDensityFluctuationsByType.h"
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

const zString mcSaveBeadDensityFluctuationsByType::m_Type = "SaveBeadDensityFluctuationsByType";

const zString mcSaveBeadDensityFluctuationsByType::GetType()
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
	xxCommand* Create(long executionTime) {return new mcSaveBeadDensityFluctuationsByType(executionTime);}

	const zString id = mcSaveBeadDensityFluctuationsByType::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveBeadDensityFluctuationsByType::mcSaveBeadDensityFluctuationsByType(long executionTime) : xxCommand(executionTime),
														m_BeadType(-1),
                                                        m_TotalDataPoints(0),
														m_TotalDensityPeriods(0),
                                                        m_XCellNo(0), m_YCellNo(0), m_ZCellNo(0),
                                                        m_bConjugate(true)
{

}

mcSaveBeadDensityFluctuationsByType::mcSaveBeadDensityFluctuationsByType(const mcSaveBeadDensityFluctuationsByType& oldCommand) : xxCommand(oldCommand),
														m_BeadType(oldCommand.m_BeadType),
														m_TotalDataPoints(oldCommand.m_TotalDataPoints),
														m_TotalDensityPeriods(oldCommand.m_TotalDensityPeriods),
                                                        m_XCellNo(oldCommand.m_XCellNo), 
                                                        m_YCellNo(oldCommand.m_YCellNo), 
                                                        m_ZCellNo(oldCommand.m_ZCellNo), 
                                                        m_bConjugate(oldCommand.m_bConjugate)

{
}


mcSaveBeadDensityFluctuationsByType::~mcSaveBeadDensityFluctuationsByType()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_BeadType            - Bead's numeric identifier
//  m_TotalDataPoints     - Number of values to output to file
//  m_TotalDensityPeriods - Number of density periods to average over
//  m_XCellNo             - X index of grid cell being analysed
//  m_YCellNo             - Y index of grid cell being analysed
//  m_ZCellNo             - Z index of grid cell being analysed
//  m_bConjugate          - Boolean flag showing if the cell and its complementary volume
//                          being analysed

zOutStream& mcSaveBeadDensityFluctuationsByType::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<BeadType>"               << m_BeadType << "</BeadType>" << zEndl;
	os << "<TotalDataPoints>"        << m_TotalDataPoints     << "</TotalDataPoints>" << zEndl;
	os << "<TotalDensityPeriods>"    << m_TotalDensityPeriods << "</TotalDensityPeriods>" << zEndl;
	os << "<XCellNo>"                << m_XCellNo             << "</XCellNo>"             << zEndl;
	os << "<YCellNo>"                << m_YCellNo             << "</YCellNo>"             << zEndl;
	os << "<ZCellNo>"                << m_ZCellNo             << "</ZCellNo>"             << zEndl;
	os << "<BooleanFlag>"            << m_bConjugate          << "</BooleanFlag>"         << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_BeadType << "  " << m_TotalDataPoints << " " << m_TotalDensityPeriods;
    os << " " << m_XCellNo << " " << m_YCellNo << " " << m_ZCellNo;
    os << " " << m_bConjugate;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mcSaveBeadDensityFluctuationsByType::get(zInStream& is)
{
	// We check that the name corresponds to a valid CBead type in 
	// IsDataValid() as the CInputData object has to create the map holding 
	// the names of valid CBeads first.

	is >> m_BeadType;

	if(!is.good())
	   SetCommandValid(false);

	// Check that the number of density periods is positive definite.
    // We check that there is at least one period left in the run
    // in the IsDataValid() function below.

	is >> m_TotalDataPoints >> m_TotalDensityPeriods;

	if(!is.good() || m_TotalDataPoints < 1 || m_TotalDensityPeriods < 1)
	   SetCommandValid(false);

	// We check that the specified cell exists in IsDataValid() below

	is >> m_XCellNo >> m_YCellNo >> m_ZCellNo;

	if(!is.good())
	   SetCommandValid(false);

    is >> m_bConjugate;
	if(!is.good())
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString mcSaveBeadDensityFluctuationsByType::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcSaveBeadDensityFluctuationsByType::GetCommand() const
{
	return new mcSaveBeadDensityFluctuationsByType(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool mcSaveBeadDensityFluctuationsByType::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SaveBeadDensityFluctuationsByType(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid. We check that
// the specified parameters fit into the remaining simulation time, and yield
// a positive number of samples. Note that the analysis starts at the beginning
// of the next full density period and continues to the end of the simulation
// or whenever the process is destroyed by the user.

bool mcSaveBeadDensityFluctuationsByType::IsDataValid(const CInputData& riData) const
{
    long currentTime    = GetExecutionTime();
    long totalTime      = riData.GetTotalTime();
    long densityPeriod  = riData.GetDensityPeriod();
    long duration       = m_TotalDataPoints*m_TotalDensityPeriods*densityPeriod;
//    long samples        = duration/riData.GetSamplePeriod();

    // The analysis starts at the beginning of the next full density grid analysis period 
    // and continues for an integer number of periods.

    long start = 0;
    long end   = 0;

    if(currentTime%densityPeriod == 0)
    {
        start = currentTime;
    }
    else
    {
        start = (currentTime/densityPeriod + 1)*densityPeriod;
    }

    end = start + duration;

    // Check that the specified bead type is valid, i.e., a non-negative integer. 
    // We don't check that any such bead type is defined as it may only be 
    // created dynamically as the result of a command. In this case, the bead
    // name and type will not be in the maps held by CInputData.
    // Then check that the cell indices are valid: they must lie within the range 
    // of 0 and the number of density grid cells in each dimension.

//	const zString beadName = (*(riData.GetBeadTypesMap().find(m_BeadType))).second;

    if(m_BeadType < 0)
    {
		return ErrorTrace("Invalid bead type specified for bead density fluctuations by type analysis");
    }
    else if(m_XCellNo < 0 || m_XCellNo >= riData.GetGridXCellNo() ||
            m_YCellNo < 0 || m_YCellNo >= riData.GetGridYCellNo() ||
            m_ZCellNo < 0 || m_ZCellNo >= riData.GetGridZCellNo())
    {
		return ErrorTrace("Invalid grid cell indices for bead density fluctuations by type analysis");
    }
    else if(end > totalTime)
    {
		return ErrorTrace("Insufficient time remaining in simulation for SaveBeadDensityFluctuationsByType");
    }
    else
	    return true;
}
