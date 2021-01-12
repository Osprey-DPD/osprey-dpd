/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelection.cpp: implementation of the ccSelection class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "ccSelection.h"
#include "InputData.h"

// STL using declarations

	using std::find;

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

bool operator==(const ccSelection& a, const ccSelection& b)
{
	if(a.m_Label		== b.m_Label &&
	   a.m_X			== b.m_X     &&
	   a.m_Y			== b.m_Y     &&
	   a.m_Z			== b.m_Z     &&
	   a.m_Names.size() == b.m_Names.size())
	   {
		// Now check the bead names. We check to see if each name in the
		// command a also occurs in the command b in any position.

		bool bSameNames = true;

		for(long unsigned int i=0; i<a.m_Names.size(); i++)
		{
			zString name = a.m_Names.at(i);

			if(find(b.m_Names.begin(), b.m_Names.end(), name) == b.m_Names.end())
				bSameNames = false;
		}
		return bSameNames;
	}
	else
		return false;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelection::ccSelection(long executionTime) : xxCommand(executionTime),
											   m_Label(""),
											   m_X(0), m_Y(0), m_Z(0)
{
	m_Names.clear();
}

ccSelection::ccSelection(const ccSelection& oldCommand) : xxCommand(oldCommand),
														  m_Label(oldCommand.m_Label),
														  m_X(oldCommand.m_X),
														  m_Y(oldCommand.m_Y),
														  m_Z(oldCommand.m_Z),
														  m_Names(oldCommand.m_Names)
{
}

ccSelection::~ccSelection()
{
}

// Member functions to read/write the data specific to the selection commands.
// Note that the derived classes must add the endline after the write statements.
// This is to allow them to add their data to that written here.
//
// Arguments
// *********
//
//	Label	The identifier for the selection, e.g., Membrane, cisMonolayer,...
//
//  Region	Slice normal: (1,0,0), (0,1,0), (0,0,1)
//
//  Names	A sequence of bead, bond or polymer names enclosed within bounding quotes. 
//			This is to indicate to the reading routine where the names start and 
//			end and prevent it reading the next command's Command string. There 
//			must be at least one space between the " characters and a name.
//          Names must correspond to bead, bond or polymer names defined 
//			in the control data file.

zOutStream& ccSelection::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output - it is the derived class' responsibility to add
	// the start and end tags and to call this function in between.

	os << "<Label>" << m_Label << "</Label>" << zEndl;
	os << "<XN>" << m_X << "</XN>" << zEndl;
	os << "<YN>" << m_Y << "</YN>" << zEndl;
	os << "<ZN>" << m_Z << "</ZN>" << zEndl;

	for(cStringSequenceIterator iterName=m_Names.begin(); iterName!=m_Names.end(); iterName++)
	{
		os << "<Name>" << (*iterName) << "</Name>" << zEndl;
	}

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << " " << m_Label << " " << m_X << " " << m_Y << " " << m_Z;

	os << " \"";
	for(cStringSequenceIterator iterName=m_Names.begin(); iterName!=m_Names.end(); iterName++)
	{
		os << " " << (*iterName);
	}
	os << " \"";

#endif

	return os;
}

zInStream& ccSelection::get(zInStream& is)
{
	// Leave specific data validation to the derived classes

	is >> m_Label >> m_X >> m_Y >> m_Z;

	if( !is.good())
	   SetCommandValid(false);

	// Selectable object (beads, bonds or polymers) names. Because we don't know
	// how many names will be specified in advance, they are enclosed within
	// quotation marks. Hence, a valid sequence looks like:
	//
	//	" A B C "
	//
	// A space is required around the names to prevent the quotes being interpreted
	// as part of a name. An error occurs if the first quote is not found or if
	// the list is empty, i.e., " ".

	zString sQuote;
	zString sName;

	is >> sQuote >> sName;

	if(sQuote != "\"" || sName == "\"")
		SetCommandValid(false);
	else
	{
		while(sName != "\"")
		{
			if(is.good())
				m_Names.push_back(sName);
			else
				SetCommandValid(false);

			is >> sName;
		}
	}

	return is;
}

// Function to check that data common to ccSelection-derived commands is valid.
// 
// We check that the name of the selection is unique. We cast the CInputData object
// to remove its const-ness so that it can store the names of force targets
// as they are created. Each time this function is called it passes the name defined 
// by the command back to the CInputData object unless that name already occurs in 
// which case it returns an error. For the first target defined we must not search
// as the container is empty.
//
// The slice normal data is checked to ensure that only one of the major axis
// directions is specified. The bead names specified must exist in the CInputData 
// bead  names map.
//
// Checks that rely only on the internal consistency of data for a command are 
// performed in the command's class.
//

bool ccSelection::IsDataValid(const CInputData& riData) const
{
	CInputData& rincData = const_cast<CInputData&>(riData);

	// Check target name is unique and consists only of allowed characters.

	if(!riData.IsExternalNameValid(m_Label))
		return ErrorTrace("Invalid Selection command target name");
	else if(riData.IsForceTargetPresent(m_Label))
		return ErrorTrace("Selection command target already exists");

	// Check slice normal is valid

	if(!((GetX() == 1 && GetY() == 0 && GetZ() == 0) || 
		 (GetX() == 0 && GetY() == 1 && GetZ() == 0) || 
		 (GetX() == 0 && GetY() == 0 && GetZ() == 1)) )
		 return ErrorTrace("Selection command normal is invalid");

	// Check that the names specified exist in CInputData's bead or polymer name maps
	// showing that they have been created. We cannot distinguish between bead and
	// polymer names though, so some commands may pass this validation and still
	// be incorrect.

	for(cStringSequenceIterator iName=m_Names.begin(); iName != m_Names.end(); iName++)
	{
		if(!rincData.IsExternalNameValid(*iName))
			return ErrorTrace("Invalid Selection command bead/polymer name");
		else if(!(rincData.IsBeadinMap(*iName) || rincData.IsPolymerinMap(*iName)))
			return ErrorTrace("Selection command bead/polymer type does not exist");
	}

	// All tests succeeded so add the new selection target to the CInputData set

	rincData.AddForceTargetName(m_Label);

	return true;
}
