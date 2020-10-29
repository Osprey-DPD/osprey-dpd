/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcToggleSliceEnergyOutput.cpp: implementation of the mcToggleSliceEnergyOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcToggleSliceEnergyOutput.h"
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

const zString mcToggleSliceEnergyOutput::m_Type = "ToggleSliceEnergyOutput";

const zString mcToggleSliceEnergyOutput::GetType()
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
	xxCommand* Create(long executionTime) {return new mcToggleSliceEnergyOutput(executionTime);}

	const zString id = mcToggleSliceEnergyOutput::GetType();

#if EnableMonitorCommand == SimCommandEnabled
	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
#endif
}

long mcToggleSliceEnergyOutput::m_CommandCounter = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Each mcToggleSliceEnergyOutput command created is given a unique counter,
// stored in the static member variable m_Command Counter. This is used to
// identify the recipients of "toggle off" commands. Because this is an
// implementation detail we do not provide public access functions to the counter.

mcToggleSliceEnergyOutput::mcToggleSliceEnergyOutput(long executionTime) : xxCommand(executionTime),
													 m_bNormalizePerBead(false), 
													 m_CurrentId(++m_CommandCounter),
													 m_ToggleId(0),
													 m_X(0), m_Y(0), m_Z(1),
													 m_LowerFractionCoord(0.0),
													 m_UpperFractionCoord(1.0)
{
}

mcToggleSliceEnergyOutput::mcToggleSliceEnergyOutput(const mcToggleSliceEnergyOutput& oldCommand) : xxCommand(oldCommand),
													 m_bNormalizePerBead(oldCommand.m_bNormalizePerBead),
													 m_CurrentId(oldCommand.m_CurrentId),
													 m_ToggleId(oldCommand.m_ToggleId),
													 m_X(oldCommand.m_X),
													 m_Y(oldCommand.m_Y),
													 m_Z(oldCommand.m_Z),
													 m_LowerFractionCoord(oldCommand.m_LowerFractionCoord),
													 m_UpperFractionCoord(oldCommand.m_UpperFractionCoord)
{
}

mcToggleSliceEnergyOutput::~mcToggleSliceEnergyOutput()
{
}

// Member functions to write/read the data specific to the command.
// The data for this command define the slice normal, its lower and upper edges,
// a flag showing whether the energy terms (KE, PE, total energy) are normalized 
// by the total number of beads or not, and an id identifying the slice
// to toggle on or off. Note that the latter parameter is usually be zero for all
// commands that are written to file as this specifies that a new command will be
// created when the data is read. If it is non-zero, a prior command will be 
// toggled off.

zOutStream& mcToggleSliceEnergyOutput::put(zOutStream& os) const
{
#if EnableMonitorCommand == SimCommandEnabled
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<NX>" << m_X << "</NX> " << "<NY>" << m_Y << "</NY> " << "<NZ>" << m_Z << " </NZ>" << zEndl;
	os << "<LowerFractionCoord>" << m_LowerFractionCoord << "</LowerFractionCoord>"   << zEndl;
	os << "<UpperFractionCoord>" << m_UpperFractionCoord << "</UpperFractionCoord>"   << zEndl;
	os << "<NormalizePerBead>"   << m_bNormalizePerBead  << "</NormalizePerBead>"     << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_X << " " << m_Y << " " << m_Z << "  ";
	os << m_LowerFractionCoord << "  " << m_UpperFractionCoord << "  ";
	os << m_bNormalizePerBead << m_ToggleId;
	putASCIIEndTags(os);

#endif
#endif
	return os;
}

zInStream& mcToggleSliceEnergyOutput::get(zInStream& is)
{
#if EnableMonitorCommand == SimCommandEnabled
	is >> m_X >> m_Y >> m_Z >> m_LowerFractionCoord >> m_UpperFractionCoord;
	is >> m_bNormalizePerBead >> m_ToggleId;


	if(!is.good() || (m_X == 0 && m_Y == 0 && m_Z == 0) || 
		m_LowerFractionCoord < 0.0 || m_UpperFractionCoord < 0.0 ||
		m_LowerFractionCoord > 1.0 || m_UpperFractionCoord > 1.0 ||
		m_LowerFractionCoord > m_UpperFractionCoord ||
		m_ToggleId < 0)
		SetCommandValid(false);
#endif

	return is;
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 
//
// Note that even though this command is destined for the IMonitor interface, we
// have to pass it to the ISimCmd interface (and thence to its ISimBoxBase base class)
// first because it will be checked for execution in the CSimBox's command loop 
// and then passed on to the CMonitor.

bool mcToggleSliceEnergyOutput::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
#if EnableMonitorCommand == SimCommandEnabled
		pISimCmd->ToggleSliceEnergyOutput(this);
#endif
		return true;
	}
	else
		return false;
}

// Non-static function to return the type of the command

const zString mcToggleSliceEnergyOutput::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcToggleSliceEnergyOutput::GetCommand() const
{
	return new mcToggleSliceEnergyOutput(*this);
}

// Function to check the command is valid. We check that the id is either zero,
// in which case a new slice energy command is created, or specifies a command
// that already exists. The m_ToggleId must be less than the m_CurrentId of the
// newly-created command or else the command is rejected.

bool mcToggleSliceEnergyOutput::IsDataValid(const CInputData &riData) const
{
	if(m_ToggleId >= m_CurrentId)
		return false;

	return true;
}

// Function to return the flag indicating whether to normalize the energies or not.

bool mcToggleSliceEnergyOutput::NormalizePerBead() const
{
	return m_bNormalizePerBead;
}

// Functions to return the slice normal vector components

long mcToggleSliceEnergyOutput::GetXNormal() const
{
	return m_X;
}

long mcToggleSliceEnergyOutput::GetYNormal() const
{
	return m_Y;
}

long mcToggleSliceEnergyOutput::GetZNormal() const
{
	return m_Z;
}

// Functions to return the lower and upper edge coordinates of the slice
// as a fraction of the SimBox length in the slice normal direction.
// Note that this assumes the slice normal can only point in one of the
// three major axes.

double mcToggleSliceEnergyOutput::GetLowerFractionCoord() const
{
	return m_LowerFractionCoord;
}

double mcToggleSliceEnergyOutput::GetUpperFractionCoord() const
{
	return m_UpperFractionCoord;
}

long mcToggleSliceEnergyOutput::GetToggleId() const
{
	return m_ToggleId;
}
