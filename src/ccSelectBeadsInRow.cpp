/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectBeadsInRow.cpp: implementation of the ccSelectBeadsInRow class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectBeadsInRow.h"
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

const zString ccSelectBeadsInRow::m_Type = "SelectBeadsInRow";

const zString ccSelectBeadsInRow::GetType()
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
	xxCommand* Create(long executionTime) {return new ccSelectBeadsInRow(executionTime);}

	const zString id = ccSelectBeadsInRow::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

bool operator==(const ccSelectBeadsInRow& a, const ccSelectBeadsInRow& b)
{
	if(dynamic_cast<const ccSelection&>(a) == dynamic_cast<const ccSelection&>(b) &&
		a.m_SliceIndex == b.m_SliceIndex &&
		a.m_RowIndex   == b.m_RowIndex)
		return true;
	else
		return false;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectBeadsInRow::ccSelectBeadsInRow(long executionTime) : ccSelection(executionTime),
															 m_SliceIndex(0),
															 m_RowIndex(0)
{

}

ccSelectBeadsInRow::ccSelectBeadsInRow(const ccSelectBeadsInRow& oldCommand) : ccSelection(oldCommand),
																			   m_SliceIndex(oldCommand.m_SliceIndex),
																			   m_RowIndex(oldCommand.m_RowIndex)
{
}


ccSelectBeadsInRow::~ccSelectBeadsInRow()
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
//	m_SliceIndex	Identifier showing the slice
//	m_RowIndex		Identifier showing the location of a row in the slice
//

zOutStream& ccSelectBeadsInRow::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	ccSelection::put(os);
	os << "<SliceIndex>" << m_SliceIndex << "</SliceIndex>" << zEndl;
	os << "<RowIndex>"   << m_RowIndex   << "</RowIndex>"   << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ccSelection::put(os);
	os << m_SliceIndex << " " << m_RowIndex;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSelectBeadsInRow::get(zInStream& is)
{
	// Read the base class data first

	ccSelection::get(is);

	// Now read the location of the slice and the row within the slice. 
	// Note that the CNT cell structure, and hence the data read here, 
	// is zero-based so the slice and row indices can both be zero.
	// The normal read in by ccSelection cannot though.

	is >> m_SliceIndex >> m_RowIndex;

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

bool ccSelectBeadsInRow::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SelectBeadsInRow(this);
		return true;
	}
	else
		return false;
}

const zString ccSelectBeadsInRow::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSelectBeadsInRow::GetCommand() const
{
	return new ccSelectBeadsInRow(*this);
}

// Function to check that the data required by the SelectBeadsInRow command
// is valid. We first check the generic data by a call to the base class's
// IsDataValid() function and then perform the local checks.
// 
// Check that the slice and row indices specified lie within the SimBox 
// in the directions specified by the slice normal and the conventional
// direction of rows within a slice. See Note p187 for the definitions.
//

bool ccSelectBeadsInRow::IsDataValid(const CInputData &riData) const
{
	if(ccSelection::IsDataValid(riData))
	{
		if(GetX() == 1 && (m_SliceIndex < 0 || m_SliceIndex >= riData.GetCNTXCellNo() ||
			               m_RowIndex   < 0 || m_RowIndex   >= riData.GetCNTZCellNo()) )
		{
			return ErrorTrace("Selection command Z Row outside SimBox");
		}
		if(GetY() == 1 && (m_SliceIndex < 0 || m_SliceIndex >= riData.GetCNTYCellNo() ||
			               m_RowIndex   < 0 || m_RowIndex   >= riData.GetCNTXCellNo()) )
		{
			return ErrorTrace("Selection command X Row outside SimBox");
		}
		if(GetZ() == 1 && (m_SliceIndex < 0 || m_SliceIndex >= riData.GetCNTZCellNo() ||
			               m_RowIndex   < 0 || m_RowIndex   >= riData.GetCNTYCellNo()) )
		{
			return ErrorTrace("Selection command Y Row outside SimBox");
		}
	}
	else
		return false;

	return true;
}
