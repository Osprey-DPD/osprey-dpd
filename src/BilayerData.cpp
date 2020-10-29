/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// BilayerData.cpp: implementation of the CBilayerData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "BilayerData.h"
#include "Bilayer.h"
#include "InputData.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for the aggregates that are
// investigated by analysis objects of this class. It is used to select the 
// appropriate analysis object when the Analysis/Type variable is read in the 
// control data file. The static member function GetType() is invoked by the
// CAnalysisObject to compare the type read from the control data file with each
// analysis data class so that it can create the appropriate object to hold the
// analysis data.

zString CBilayerData::m_Type = "bilayer";

const zString CBilayerData::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	CAnalysisData* Create() {return new CBilayerData();}

	const zString id = CBilayerData::GetType();

	const bool bRegistered = acfAnalysisFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// I set the default data to reproduce that in test 2057h so that the 
// default control data file can be used for a regression test.

CBilayerData::CBilayerData() : CAnalysisData(false),
							   m_Polymer("Lipid"),
							   m_X(0), m_Y(0), m_Z(1),
							   m_SliceTotal(128), m_RowTotal(16), m_CellTotal(16),
							   m_Solvent("Water")
{
	// Set the start and end times in the base class

	SetStartTime(0);
	SetEndTime(20);
}

CBilayerData::~CBilayerData()
{

}

// Functions to read and write the data needed by a bilayer aggregate analysis.
// We call the base class's function first to get the more general data before
// that specific to each aggregate.

zOutStream& CBilayerData::put(zOutStream& os) const
{
#if EnableXMLProcesses == SimXMLEnabled

	putXMLStartTags(os);
	os << "<Polymer>" << m_Polymer << "</Polymer>" << zEndl;
	os << "<Normal><XN>" << m_X << "</XN><YN>" << m_Y << "</YN><ZN>" << m_Z << "</ZN></Normal>" << zEndl;
	os << "<Slice>" << m_SliceTotal << "</Slice>" << zEndl;
	os << "<Grid><RowTotal>" << m_RowTotal << "</RowTotal><CellTotal>" << m_CellTotal << "</CellTotal></Grid>" << zEndl;
	os << "<Solvent>" << m_Solvent << "</Solvent>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLProcesses == SimXMLDisabled

	putASCIIStartTags(os);
	os << "    Polymer      " << m_Polymer << zEndl;
	os << "    Normal       " << m_X << "  "  << m_Y << "  " << m_Z << zEndl;
	os << "    Slice        " << m_SliceTotal << zEndl;
	os << "    Grid         " << m_RowTotal << "  " << m_CellTotal << zEndl;
	os << "    Solvent      " << m_Solvent << zEndl;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& CBilayerData::get(zInStream& is)
{
	// Read base class data first

	CAnalysisData::get(is);
	
	zString token;
	zString polymer;
	zString solvent;
	long x, y, z;
	long slices, rows, cells;

	is >> token;
	if(!is.good() || token != "Polymer")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> polymer;
		if(!is.good() || polymer.length() == 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Normal")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> x >> y >> z;
		
		// Force the normal to be parallel to an axis

		if(!is.good() || ((x+y) != 0 && z==1) || 
						 ((x+z) != 0 && y==1) ||
						 ((y+z) != 0 && x==1) )
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Slice")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> slices;

		// There must be at least one slice in the profiles

		if(!is.good() || slices < 1)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Grid")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> rows >> cells;

		// There must be at least one row and cell in the profiles

		if(!is.good() || rows < 1 || cells < 1)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Solvent")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> solvent;
		if(!is.good() || solvent.length() == 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Data has been read successfully so store it in the member variables

	SetDataValid(true);
	m_Polymer		= polymer;
	m_X				= x;
	m_Y				= y;
	m_Z				= z;
	m_SliceTotal	= slices;
	m_RowTotal		= rows;
	m_CellTotal		= cells;
	m_Solvent		= solvent;

	return is;
}

CAnalysis* CBilayerData::CreateAnalysis(const zString runId, long samplePeriod) 
{
	return new CBilayer(runId, samplePeriod, this);
}

// Virtual function to return the type of analysis represented by the class.
// This has to access the static class variable for the type.

const zString CBilayerData::GetAnalysisType() const
{
	return m_Type;
}

// Function to check that the data defining the aggregate analysis is valid.
// We just check that the polymers specified for the bilayer and solvent
// have been defined.

bool CBilayerData::ValidateData(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_Polymer))
		return ErrorTrace("Invalid bilayer analysis polymer name");
	else if(!riData.IsPolymerinMap(m_Polymer))
		return ErrorTrace("Bilayer analysis polymer not found in map");
	else if(!riData.IsExternalNameValid(m_Solvent))
		return ErrorTrace("Invalid bilayer analysis solvent name");
	else if(!riData.IsPolymerinMap(m_Solvent))
		return ErrorTrace("Bilayer analysis solvent not found in map");

	return true;
}
