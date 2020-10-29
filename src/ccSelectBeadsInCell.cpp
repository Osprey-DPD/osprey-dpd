/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectBeadsInCell.cpp: implementation of the ccSelectBeadsInCell class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectBeadsInCell.h"
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

const zString ccSelectBeadsInCell::m_Type = "SelectBeadsInCell";

const zString ccSelectBeadsInCell::GetType()
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
	xxCommand* Create(long executionTime) {return new ccSelectBeadsInCell(executionTime);}

	const zString id = ccSelectBeadsInCell::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

bool operator==(const ccSelectBeadsInCell& a, const ccSelectBeadsInCell& b)
{
	if(dynamic_cast<const ccSelection&>(a) == dynamic_cast<const ccSelection&>(b) &&
		a.m_XCellIndex == b.m_XCellIndex &&
		a.m_YCellIndex == b.m_YCellIndex &&
		a.m_ZCellIndex == b.m_ZCellIndex)
		return true;
	else
		return false;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectBeadsInCell::ccSelectBeadsInCell(long executionTime) : ccSelection(executionTime),
															   m_XCellIndex(0),
															   m_YCellIndex(0),
															   m_ZCellIndex(0)
{
}

ccSelectBeadsInCell::ccSelectBeadsInCell(const ccSelectBeadsInCell& oldCommand) : ccSelection(oldCommand),
																				  m_XCellIndex(oldCommand.m_XCellIndex),
																				  m_YCellIndex(oldCommand.m_YCellIndex),
																				  m_ZCellIndex(oldCommand.m_ZCellIndex)
{
}

ccSelectBeadsInCell::~ccSelectBeadsInCell()
{
}

// Member functions to write/read the data specific to the command.
// Note that spaces are written here not in the ccSelection or xxCommand
// classes. Also note that we have to pass the command to put or get up 
// to the ccSelection base class so that its data can be read/written 
// before we get the data for this class.
//
// Arguments
// *********
//
//	m_XCellIndex	 X coordinate of cell in CNT cell structure
//	m_YCellIndex	 Y coordinate of cell in CNT cell structure
//	m_ZCellIndex	 Z coordinate of cell in CNT cell structure

zOutStream& ccSelectBeadsInCell::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	ccSelection::put(os);
	os << "<XCellIndex>" << m_XCellIndex << "</XCellIndex>" << zEndl;
	os << "<YCellIndex>" << m_YCellIndex << "</YCellIndex>" << zEndl;
	os << "<ZCellIndex>" << m_ZCellIndex << "</ZCellIndex>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ccSelection::put(os);
	os << m_XCellIndex << " " << m_YCellIndex << " " << m_ZCellIndex;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSelectBeadsInCell::get(zInStream& is)
{
	// Read the base class data first

	ccSelection::get(is);

	// The region has already been checked to ensure that its normal is valid
	// so we just check the cell indices are positive. We check that the indices
	// lie within the SimBox in CInputData.

	is >> m_XCellIndex >> m_YCellIndex >> m_ZCellIndex;

	if(!is.good())
		SetCommandValid(false);

	return is;
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands.
//
// This command zeroes the total bead momentum (CM momentum).

bool ccSelectBeadsInCell::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SelectBeadsInCell(this);
		return true;
	}
	else
		return false;
}

const zString ccSelectBeadsInCell::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSelectBeadsInCell::GetCommand() const
{
	return new ccSelectBeadsInCell(*this);
}


// Function to check the data specific to selecting beads in a CNT cell.
// We just check that the cell coordinates lie within the SimBox boundaries.
// We access the SimBox size from the CInputData reference.

bool ccSelectBeadsInCell::IsDataValid(const CInputData &riData) const
{
	if(ccSelection::IsDataValid(riData))
	{
		if( m_XCellIndex < 0 || m_XCellIndex >= riData.GetCNTXCellNo())
			return ErrorTrace("Selection command X Cell coordinates outside SimBox");
		else if( m_YCellIndex < 0 || m_YCellIndex >= riData.GetCNTYCellNo())
			return ErrorTrace("Selection command Y Cell coordinates outside SimBox");
		else if( m_ZCellIndex < 0 || m_ZCellIndex >= riData.GetCNTZCellNo())
			return ErrorTrace("Selection command Z Cell coordinates outside SimBox");
	}
	else
		return false;

	return true;
}
