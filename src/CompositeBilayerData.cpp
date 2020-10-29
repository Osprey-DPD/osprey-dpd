/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CompositeBilayerData.cpp: implementation of the CCompositeBilayerData class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimXMLFlags.h"
#include "CompositeBilayerData.h"
#include "CompositeBilayer.h"
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

zString CCompositeBilayerData::m_Type = "compositebilayer";

const zString CCompositeBilayerData::GetType()
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
	CAnalysisData* Create() {return new CCompositeBilayerData();}

	const zString id = CCompositeBilayerData::GetType();

	const bool bRegistered = acfAnalysisFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompositeBilayerData::CCompositeBilayerData() : CAnalysisData(false),
												m_Polymer(""),
												m_X(0), m_Y(0), m_Z(0),
												m_SliceTotal(0), m_RowTotal(0), m_CellTotal(0),
												m_Solvent(""),
												m_bFRAP(false),
												m_FRAPPolymer(""),
												m_bRafts(false)
{
	m_CoPolymers.clear();
	m_RaftPolymers.clear();

	m_bMonolayers[0] = false;	// Upper monolayer
	m_bMonolayers[1] = false;	// Lower monolayer

}

CCompositeBilayerData::~CCompositeBilayerData()
{

}

// Functions to read and write the data needed by a composite bilayer 
// aggregate analysis. We call the base class's function first to get 
// the more general data before that specific to each aggregate.

zOutStream& CCompositeBilayerData::put(zOutStream& os) const
{

#if EnableXMLProcesses == SimXMLEnabled

	putXMLStartTags(os);
	os << "<Polymer>" << m_Polymer << "</Polymer>" << zEndl;

	os << "<CoPolymers>" << zEndl;
	for(cStringSequenceIterator iterName=m_CoPolymers.begin(); iterName!=m_CoPolymers.end(); iterName++)
	{
		os << "<PolymerName>" << (*iterName) << "</PolymerName>" << zEndl;
	}
	os << "</CoPolymers>" << zEndl;
	os << "<Normal><XN>" << m_X << "</XN><YN>" << m_Y << "</YN><ZN>" << m_Z << "</ZN></Normal>" << zEndl;
	os << "<Slice>" << m_SliceTotal << "</Slice>" << zEndl;
	os << "<Grid><RowTotal>" << m_RowTotal << "</RowTotal><CellTotal>" << m_CellTotal << "</CellTotal></Grid>" << zEndl;
	os << "<Solvent>" << m_Solvent << "</Solvent>" << zEndl;

	os << "<IsFRAPOn>" << m_bFRAP << "</IsFRAPOn>" << zEndl;

	if(m_bFRAP)
	{
		os << "<FRAPPolymer>" << m_FRAPPolymer << "</FRAPPolymer>" << zEndl;
	}

	os << "<IsRaftsOn>" << m_bRafts << "</IsRaftsOn>" << zEndl;

	if(m_bRafts)
	{
		os << "<RaftPolymers>" << zEndl;

		for(cStringSequenceIterator iterName2=m_RaftPolymers.begin(); iterName2!=m_RaftPolymers.end(); iterName2++)
		{
			os << "<PolymerName>" << (*iterName2) << "</PolymerName>" << zEndl;
		}

		os << "</RaftPolymers>" << zEndl;
	}

	os << "<Monolayers><Upper>" << m_bMonolayers[0] << "</Upper><Lower>" << m_bMonolayers[1] << "</Lower></Monolayers>" << zEndl;

	putXMLEndTags(os);

#elif EnableXMLProcesses == SimXMLDisabled

	putASCIIStartTags(os);
	os << "    Polymer      " << m_Polymer << zEndl;

	os << "    CoPolymers   ";

	for(cStringSequenceIterator iterName=m_CoPolymers.begin(); iterName!=m_CoPolymers.end(); iterName++)
	{
		os << " " << (*iterName);
	}

	os << zEndl;

	os << "    Normal       " << m_X << "  "   << m_Y << "  " << m_Z << zEndl;
	os << "    Slice		" << m_SliceTotal  << zEndl;
	os << "    Grid         " << m_RowTotal    << "  " << m_CellTotal << zEndl;
	os << "    Solvent      " << m_Solvent     << zEndl;
	
	// Only output the analysis tool data if a tool is active

	// ********** FRAP tool
	os << "    FRAPOn		" << m_bFRAP << zEndl;

	if(m_bFRAP)
	{
		os << "    FRAPPolymer  " << m_FRAPPolymer << zEndl;
	}

	// ********** Rafts tool
	os << "    RaftsOn		" << m_bRafts << zEndl; 

	if(m_bRafts)
	{
		os << "    RaftPolymers ";

		for(cStringSequenceIterator iterName2=m_RaftPolymers.begin(); iterName2!=m_RaftPolymers.end(); iterName2++)
		{
			os << " " << (*iterName2);
		}

		os << zEndl;

		os << "    Monolayers	" << m_bMonolayers[0] << "  " << m_bMonolayers[1] << zEndl;
	}

	os << zEndl;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& CCompositeBilayerData::get(zInStream& is)
{
	// Read base class data first

	CAnalysisData::get(is);
	
	zString token   = "";
	zString polymer = "";
	zString solvent = "";
	zString sName   = "";

	long x, y, z;
	long slices, rows, cells;

	// CAnalysisTool data

	bool bFrapOn = false;
	zString frapName = "";		// CFrap tool data

	bool bRaftsOn = false;
	bool bMonolayers[2];		// CDomain2d data
	bMonolayers[0] = false;
	bMonolayers[1] = false;

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
	if(!is.good() || token != "CoPolymers")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;

		if(!is.good() || sName == "Normal" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "Normal")
			{
				if(is.good() && !sName.empty())
					m_CoPolymers.push_back(sName);
				else
				{
					SetDataValid(false);
					return is;
				}

				is >> sName;
			}
		}
	}

	if(!is.good() || sName != "Normal")
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

	// Now the tools available for the composite bilayer aggregate

	// ****************************************
	is >> token >> bFrapOn;			//	CFRAP

	if(!is.good() || token != "FRAPOn")
	{
		SetDataValid(false);
		return is;
	}
	else if(bFrapOn)		// Only read next line as data if the tool is on
	{
		is >> token;
		if(!is.good() || token != "FRAPPolymer")
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			is >> frapName;
			if(!is.good() || frapName.length() == 0)
			{
				SetDataValid(false);
				return is;
			}
		}
	}

	// ****************************************
	is >> token >> bRaftsOn;		// CDomain2d

	if(!is.good() || token != "RaftsOn")
	{
		SetDataValid(false);
		return is;
	}
	else if(bRaftsOn)		// Only read next line as data if the tool is on
	{
		is >> token;
		if(!is.good() || token != "RaftPolymers")
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			is >> sName;

			if(!is.good() || sName == "Monolayers" || sName.empty())
			{
				SetDataValid(false);
				return is;
			}
			else
			{
				while(sName != "Monolayers")
				{
					if(is.good() && !sName.empty())
						m_RaftPolymers.push_back(sName);
					else
					{
						SetDataValid(false);
						return is;
					}

					is >> sName;
				}
			}
		}

		is >> bMonolayers[0] >> bMonolayers[1];

		if(!is.good())
		{
			SetDataValid(false);
			return is;
		}
	}

	// Data has been read successfully so store it in the member variables

	SetDataValid(true);
	m_Polymer		 = polymer;
	m_X				 = x;
	m_Y				 = y;
	m_Z				 = z;
	m_SliceTotal	 = slices;
	m_RowTotal		 = rows;
	m_CellTotal		 = cells;
	m_Solvent		 = solvent;

	m_bFRAP			 = bFrapOn;
	m_FRAPPolymer	 = frapName;

	m_bRafts		 = bRaftsOn;
	m_bMonolayers[0] = bMonolayers[0];
	m_bMonolayers[1] = bMonolayers[1];

	return is;
}

CAnalysis* CCompositeBilayerData::CreateAnalysis(const zString runId, long samplePeriod) 
{
	return new CCompositeBilayer(runId, samplePeriod, this);
}

// Virtual function to return the type of analysis represented by the class.
// This has to access the static class variable for the type.

const zString CCompositeBilayerData::GetAnalysisType() const
{
	return m_Type;
}

// Virtual function to perform more extensive validation on the user-defined data
// than is possible in the get() function above.

bool CCompositeBilayerData::ValidateData(const CInputData &riData) const
{
	// Check that the specified polymer, copolymers and solvent names actually 
	// occur in the CInitialState map. The m_Polymer name must not occur in 
	// the m_CoPolymers container nor can the solvent.

	if(riData.GetPolymerNamesMap().find(m_Polymer)    == riData.GetPolymerNamesMap().end() ||
	   riData.GetPolymerNamesMap().find(m_Solvent )   == riData.GetPolymerNamesMap().end() ||
	   find(m_CoPolymers.begin(), m_CoPolymers.end(), m_Polymer)     != m_CoPolymers.end() ||
	   find(m_CoPolymers.begin(), m_CoPolymers.end(), m_Solvent)     != m_CoPolymers.end() ||
	   riData.GetPolymerNamesMap().find(m_Polymer) == riData.GetPolymerNamesMap().find(m_Solvent) )
		return false;

	// Check the copolymers are in the polymer names map

	for(cStringSequenceIterator polymer1=m_CoPolymers.begin(); polymer1!=m_CoPolymers.end(); polymer1++)
	{
		if(riData.GetPolymerNamesMap().find(*polymer1) == riData.GetPolymerNamesMap().end()) 
			return false;
	}

	// Check the FRAP tool' data if it is active:
	//
	// The m_FRAPPolymer name must be in the m_CoPolymers container and must not be the
	// same as the main polymer or the solvent.

	if(m_bFRAP)
	{
		if(riData.GetPolymerNamesMap().find(m_FRAPPolymer) == riData.GetPolymerNamesMap().end()			  ||
		   riData.GetPolymerNamesMap().find(m_FRAPPolymer) == riData.GetPolymerNamesMap().find(m_Polymer) ||
		   riData.GetPolymerNamesMap().find(m_FRAPPolymer) == riData.GetPolymerNamesMap().find(m_Solvent) ||
	       find(m_CoPolymers.begin(), m_CoPolymers.end(), m_FRAPPolymer) == m_CoPolymers.end() )
		return false;
	}

	// Check the Rafts tool data if it is active:
	//
	// The raft polymers must be in the m_CoPolymers container and not be the same as the
	// main polymer or the solvent. Note that the raft polymers do not have to
	// include all of the copolymers. 
	// At least one monolayer must be specified for the rafts

	if(m_bRafts)
	{
		for(cStringSequenceIterator polymer1=m_RaftPolymers.begin(); polymer1!=m_RaftPolymers.end(); polymer1++)
		{
			if(riData.GetPolymerNamesMap().find(*polymer1) == riData.GetPolymerNamesMap().end()			  ||
			   riData.GetPolymerNamesMap().find(*polymer1) == riData.GetPolymerNamesMap().find(m_Polymer) ||
		       riData.GetPolymerNamesMap().find(*polymer1) == riData.GetPolymerNamesMap().find(m_Solvent) ||
	           find(m_CoPolymers.begin(), m_CoPolymers.end(), *polymer1) == m_CoPolymers.end() )
				return false;
		}

		if(!m_bMonolayers[0] && !m_bMonolayers[1])
			return false;
	}

	return true;
}
