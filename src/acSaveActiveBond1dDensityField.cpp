/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acSaveActiveBond1dDensityField.cpp: implementation of the acSaveActiveBond1dDensityField class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acSaveActiveBond1dDensityField.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "aeActiveSimBox.h"
#include "IModifyActiveCellNetwork.h"
#include "LogacSaveActiveBond1dDensityField.h"
#include "LogacEmptyBondACN.h"
#include "LogacNetworkNotModifiable.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString acSaveActiveBond1dDensityField::m_Type = "SaveActiveBond1dDensityField";

const zString acSaveActiveBond1dDensityField::GetType()
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
	xxCommand* Create(long executionTime) {return new acSaveActiveBond1dDensityField(executionTime);}

	const zString id = acSaveActiveBond1dDensityField::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acSaveActiveBond1dDensityField::acSaveActiveBond1dDensityField(long executionTime) : xxCommand(executionTime),
														m_ACNType(""),
														m_Start(0), m_End(0),
														m_SamplePeriod(0), m_AnalysisPeriod(0),
														m_NX(0), m_NY(0), m_NZ(0),
														m_SliceTotal(0),
														m_LowerFraction(0.0), m_UpperFraction(0.0)
{

}

acSaveActiveBond1dDensityField::acSaveActiveBond1dDensityField(const acSaveActiveBond1dDensityField& oldCommand) : xxCommand(oldCommand),
														m_ACNType(oldCommand.m_ACNType),
														m_Start(oldCommand.m_Start), 
														m_End(oldCommand.m_End),
														m_SamplePeriod(oldCommand.m_SamplePeriod), 
														m_AnalysisPeriod(oldCommand.m_AnalysisPeriod),
														m_NX(oldCommand.m_NX), 
														m_NY(oldCommand.m_NY), 
														m_NZ(oldCommand.m_NZ),
														m_SliceTotal(oldCommand.m_SliceTotal),
														m_LowerFraction(oldCommand.m_LowerFraction), 
														m_UpperFraction(oldCommand.m_UpperFraction)

{
}


acSaveActiveBond1dDensityField::~acSaveActiveBond1dDensityField()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_ACNType			- Name of the active cell network to modify (not the bond name!)
//	m_Start				- Simulation time at which density field analysis starts
//	m_End				- Simulation time at which density field analysis ends
//	m_SamplePeriod		- No of time steps between sampling the data
//	m_AnalysisPeriod	- No of time steps between averaging the data
//  m_NX, m_NY, m_NZ	- Direction of density profile through SimBox
//	m_SliceTotal		- No of slices in the profile
//	m_LowerFraction		- Minimum fractional value of the coordinate in the profile direction
//	m_UpperFraction		- Maximum fractional value of the coordinate in the profile direction


zOutStream& acSaveActiveBond1dDensityField::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<ActiveCellNetworkName>" << m_ACNType       << "</ActiveCellNetworkName>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_ACNType;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acSaveActiveBond1dDensityField::get(zInStream& is)
{
	// Check that the ACN name is not null and that the specified 
	// separation is positive.

	is >> m_ACNType;

	if(!is.good() || m_ACNType.empty())
	   SetCommandValid(false);

	// Check that the sample and analysis periods are commensurate with the
	// start and end times, and the normal is non-zero, and the range of 
	// coordinates is well-defined. We perform more complicated checks in the 
	// IsDataValid() function below.

	is >> m_Start >> m_End >> m_SamplePeriod >> m_AnalysisPeriod;

	if(!is.good() || m_Start < 0 || m_End < m_Start || 
		      m_SamplePeriod < 1 || m_AnalysisPeriod < m_SamplePeriod)
	   SetCommandValid(false);

	is >> m_NX >> m_NY >> m_NZ;

	if(!is.good() || (m_NX == 0 && m_NY == 0 && m_NZ == 0))
	   SetCommandValid(false);

	is >> m_SliceTotal >> m_LowerFraction >> m_UpperFraction;

	if(!is.good() || m_SliceTotal < 1 || 
		m_LowerFraction < 0.0 || m_UpperFraction < m_LowerFraction || m_UpperFraction > 1.0)
	   SetCommandValid(false);

	return is;
}

// Non-static function to return the type of the command

const zString acSaveActiveBond1dDensityField::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* acSaveActiveBond1dDensityField::GetCommand() const
{
	return new acSaveActiveBond1dDensityField(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool acSaveActiveBond1dDensityField::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		IActiveSimBox* pIASB = pISimCmd->GetISimBox()->IIActiveSimBox();

		IModifyActiveCellNetwork* const pMACN = pIASB->GetIModifyActiveCellNetworkFromType(m_ACNType);

		if(pMACN)
		{
			// Issue a warning message and ignore the command if the ACN is empty:
			// we still issue the log message for the command so that we can see
			// which command triggered the empty ACN message

			 new CLogacSaveActiveBond1dDensityField(GetExecutionTime(), m_ACNType);

			if(pMACN->GetFreeBondTotal() != 0)
			{
				pMACN->Save1dDensityField(this);
			}
			else
			{
				 new CLogacEmptyBondACN(GetExecutionTime(), m_ACNType);
			}
		}
		else
		{
			 new CLogacNetworkNotModifiable(GetExecutionTime(), m_ACNType);
		}

		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid.
//
// We ensure that the sample and analysis periods result in an integer number
// of analysis periods within the specified analysis time. We then check that the
// profile's normal direction and the coordinate ranges are well-defined.
// Note that we allow the start and end times of the analysis to be equal; this
// allows a single snapshot of the data to be taken.

bool acSaveActiveBond1dDensityField::IsDataValid(const CInputData& riData) const
{
	if( (m_End - m_Start) % m_AnalysisPeriod != 0)
		return ErrorTrace("ACN 1d density profile temporal range incommensurate");
	else if( m_AnalysisPeriod % m_SamplePeriod != 0)
		return ErrorTrace("ACN 1d density profile periods incommensurate");
	else if( !((m_NX == 1 && m_NY == 0 && m_NZ == 0) ||
			   (m_NX == 0 && m_NY == 1 && m_NZ == 0) ||
			   (m_NX == 0 && m_NY == 0 && m_NZ == 1)) )
		return ErrorTrace("ACN 1d density profile normal vector invalid");
			   
	return true;
}
