/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccAddBeadsInCell.cpp: implementation of the ccAddBeadsInCell class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "ccAddBeadsInCell.h"
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

const zString ccAddBeadsInCell::m_Type = "AddBeadsInCell";

const zString ccAddBeadsInCell::GetType()
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
	xxCommand* Create(long executionTime) {return new ccAddBeadsInCell(executionTime);}

	const zString id = ccAddBeadsInCell::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}


//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// Compare the base, ccSelectBeadsInCell, part of each command object for identity.

bool operator==(const ccAddBeadsInCell& a, const ccAddBeadsInCell& b)
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

ccAddBeadsInCell::ccAddBeadsInCell(long executionTime) : ccSelection(executionTime),
															m_XCellIndex(0),
															m_YCellIndex(0),
															m_ZCellIndex(0)
{
}

ccAddBeadsInCell::ccAddBeadsInCell(const ccAddBeadsInCell& oldCommand) : ccSelection(oldCommand),
																		  m_XCellIndex(oldCommand.m_XCellIndex),
																		  m_YCellIndex(oldCommand.m_YCellIndex),
																		  m_ZCellIndex(oldCommand.m_ZCellIndex)
{
}


ccAddBeadsInCell::~ccAddBeadsInCell()
{

}

// Member functions to write/read the data specific to the command.
// Note that spaces are written here not in the ccSelection or xxCommand
// classes. 

zOutStream& ccAddBeadsInCell::put(zOutStream& os) const
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

zInStream& ccAddBeadsInCell::get(zInStream& is)
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

bool ccAddBeadsInCell::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->AddBeadsInCell(this);
		return true;
	}
	else
		return false;
}

const zString ccAddBeadsInCell::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccAddBeadsInCell::GetCommand() const
{
	return new ccAddBeadsInCell(*this);
}


// Function to check the data specific to selecting beads in a CNT cell.
// We just check that the cell coordinates lie within the SimBox boundaries.
// We access the SimBox size from the CInputData reference.

bool ccAddBeadsInCell::IsDataValid(const CInputData &riData) const
{
	// Check target name exists and consists only of allowed characters.

	if( !riData.IsExternalNameValid(GetLabel()) )
		return ErrorTrace("Invalid Add force target name");
	else if( !riData.IsForceTargetPresent(GetLabel()) )
		return ErrorTrace("Add force target does not exist");

	// Check that the bead names exist in the CInputData's bead names map

	const StringSequence targetBeadNames = GetNames();

	for(cStringSequenceIterator iBeadName=targetBeadNames.begin(); iBeadName != targetBeadNames.end(); iBeadName++)
	{
		if(!riData.IsExternalNameValid(*iBeadName))
			return ErrorTrace("Invalid Add command bead name");
		else if(!riData.IsBeadinMap(*iBeadName))
			return ErrorTrace("Add command bead does not exist");
	}
	// Check the cell indices lie within the SimBox boundaries

	if( GetXCellIndex() < 0 || GetXCellIndex() >= riData.GetCNTXCellNo())
		return ErrorTrace("X Cell coordinates outside SimBox");
	else if( GetYCellIndex() < 0 || GetYCellIndex()>= riData.GetCNTYCellNo())
		return ErrorTrace("Y Cell coordinates outside SimBox");
	else if( GetZCellIndex() < 0 || GetZCellIndex() >= riData.GetCNTZCellNo())
		return ErrorTrace("Z Cell coordinates outside SimBox");

	// The following tests see if the command is a duplicate of a previous one.
	// Because there are several classes all inheriting from ccSelection this
	// is a non-trivial exercise.
	//
	// Find the Select command's target corresponding to this Add command. We only 
	// check commands that precede the current one as the Selectxxx command must
	// occur before any Addxxx command. We use the command id's to search.

	cCommandIterator iCmd = riData.GetCommands().begin();

	while((*iCmd)->GetId() < GetId())
	{
		// Find the Select/Add command corresponding to the current Add command.
		// We check both that the current command has a matching Select command
		// and that it is not a duplicate of another Add command.

		const ccSelectBeadsInCell* const pTarget = dynamic_cast<const ccSelectBeadsInCell*>(*iCmd);

		if(pTarget && pTarget->GetLabel() == GetLabel())
		{
			// Now we have the target check equality between the ccSelection 
			// part of the current Addxxx and the Selectxxx or Addxxx commands

			const ccSelection* const pS1 = dynamic_cast<const ccSelection*>(*iCmd);
			const ccSelection* const pS2 = dynamic_cast<const ccSelection*>(this);

			if(*pS1 == *pS2)
			{
				// Found the target now check the cell coordinates

				if(pTarget->GetXCellIndex() == GetXCellIndex() &&
				   pTarget->GetYCellIndex() == GetYCellIndex() &&
				   pTarget->GetZCellIndex() == GetZCellIndex())
				{
					return ErrorTrace("Error: Add command duplicates Select/Add command");
				}
			}
			else if(pS1->GetX() == pS2->GetX() && pS1->GetY() == pS2->GetY() && 
				    pS1->GetZ() == pS2->GetZ() && 
					pTarget->GetXCellIndex() == GetXCellIndex() &&
				    pTarget->GetYCellIndex() == GetYCellIndex() &&
				    pTarget->GetZCellIndex() == GetZCellIndex())
			{
				// If the normals and cell coordinates are the same check that the 
				// bead lists do not overlap. This means that no entry in the current 
				// Addxxx command can occur in the iCmd list.

				bool bDistinctNames = true;

				for(long unsigned int i=0; i<GetNames().size(); i++)
				{
					zString name = GetNames().at(i);

					const StringSequence targetBeads = pTarget->GetNames();

					if(std::find(targetBeads.begin(), targetBeads.end(), name) != targetBeads.end())
						bDistinctNames = false;
				}
				if(!bDistinctNames)
					return ErrorTrace("Error: Beads overlap with previous Select/Add command");;
			}
		}
		iCmd++;
	}

	return true;
}
