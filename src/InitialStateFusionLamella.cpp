/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateFusionLamella.cpp: implementation of the CInitialStateFusionLamella class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "InitialStateFusionLamella.h"
#include "FusionLamellaBuilder.h"
#include "InputData.h"
#include "InitialState.h"
#include "Polymer.h"

// STL using declarations

#if Platform == DECALPHA
	using std::find;
#elif Platform == SGICC
#elif Platform == CRAYJ90
#elif Platform == BORLAND6
	using std::find;
#elif Platform == I7XEON
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
	using std::find;
#elif Platform == XCMAC
	using std::find;
#elif Platform == NEWPLATFORM1
#elif Platform == NEWPLATFORM2
#elif Platform == NEWPLATFORM3
#elif Platform == NEWPLATFORM4
#elif Platform == NEWPLATFORM5
#elif Platform == NEWPLATFORM6
#elif Platform == NEWPLATFORM7
#elif Platform == NEWPLATFORM8
#elif Platform == NEWPLATFORM9
#elif Platform == NEWPLATFORM10
#else
	using std::find;
#endif					


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for the initial state that 
// is represented by an object of this class. It is used to select the 
// appropriate CInitialStateData-derived object when the State variable is read 
// in the  control data file. The static member function GetType() is invoked by the
// CInitialStateObject to compare the type read from the control data file with each
// initial state data class so that it can create the appropriate object to hold the
// analysis data.

const zString CInitialStateFusionLamella::m_Type = "fusionlamella";

const zString CInitialStateFusionLamella::GetType()
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
	CInitialStateData* Create() {return new CInitialStateFusionLamella();}

	const zString id = CInitialStateFusionLamella::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Note that there is no way to set default data for the two lamellae as we 
// don't have access to the polymers that are defined in the control data file.

CInitialStateFusionLamella::CInitialStateFusionLamella() : m_X(0), m_Y(0), m_Z(0), 
											   m_BilayerArea(0.0),
											   m_Bilayer1Length(0.0),
											   m_Bilayer2Length(0.0),
											   m_bLinearise(false)
{
	m_Polymers1.clear();
	m_Polymers2.clear();
	m_PolymerTypes1.clear();
	m_PolymerTypes2.clear();
	m_UpperFractions.clear();

	for(short int i=0; i<2; i++)
	{
		m_Centre[i]			= 0.0;
		m_Thickness[i]		= 0.0;
		m_BilayerCentre[i]	= 0.0;
		m_UpperHead[i]		= 0.0;
		m_LowerHead[i]		= 0.0;
	}


}

CInitialStateFusionLamella::~CInitialStateFusionLamella()
{

}

CInitialStateFusionLamella& CInitialStateFusionLamella::operator=(const CInitialStateFusionLamella& oldISL)
{
	if(this != &oldISL)
	{
		m_Polymers1			= oldISL.m_Polymers1;
		m_Polymers2			= oldISL.m_Polymers2;
		m_X					= oldISL.m_X;
		m_Y					= oldISL.m_Y;
		m_Z					= oldISL.m_Z;
		m_Centre[0]			= oldISL.m_Centre[0];
		m_Centre[1]			= oldISL.m_Centre[1];
		m_Thickness[0]		= oldISL.m_Thickness[0];
		m_Thickness[1]		= oldISL.m_Thickness[1];
		m_bLinearise		= oldISL.m_bLinearise;
		m_UpperFractions	= oldISL.m_UpperFractions;
		m_PolymerTypes1		= oldISL.m_PolymerTypes1;
		m_PolymerTypes2		= oldISL.m_PolymerTypes2;
	    m_BilayerArea		= oldISL.m_BilayerArea;
	    m_Bilayer1Length	= oldISL.m_Bilayer1Length;
	    m_Bilayer2Length	= oldISL.m_Bilayer2Length;
	    m_BilayerCentre[0]	= oldISL.m_BilayerCentre[0];
	    m_BilayerCentre[1]	= oldISL.m_BilayerCentre[1];
	    m_UpperHead[0]		= oldISL.m_UpperHead[0];
	    m_UpperHead[1]		= oldISL.m_UpperHead[1];
	    m_LowerHead[0]		= oldISL.m_LowerHead[0];
	    m_LowerHead[1]		= oldISL.m_LowerHead[1];
	}

	return *this;
}

zString CInitialStateFusionLamella::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateFusionLamella::put(zOutStream& os) const
{
	os << "State	" << "fusionlamella"	<< zEndl;
	os << "			" << "Polymers		  ";

	for(cStringSequenceIterator iterName1=m_Polymers1.begin(); iterName1!=m_Polymers1.end(); iterName1++)
	{
		os << " " << (*iterName1);
	}

	os << zEndl;

	os << "			" << "Polymers		  ";

	for(cStringSequenceIterator iterName2=m_Polymers2.begin(); iterName2!=m_Polymers2.end(); iterName2++)
	{
		os << " " << (*iterName2);
	}

	os << zEndl;

	os << "			" << "Normal		"	<< m_X << "  " << m_Y << "  " << m_Z << zEndl;
	os << "			" << "Centre		"	<< m_Centre[0]    << m_Centre[1]    << zEndl;
	os << "			" << "Thickness		"	<< m_Thickness[0] << m_Thickness[1]	<< zEndl;
	os << "			" << "Linearise 	"	<< m_bLinearise		<< zEndl;

	os << "	        " << "UpperFraction	  ";

	for(short i=0; i<m_Polymers1.size(); i++)
	{
		os << "	" << m_UpperFractions.at(i);
	}

	os << zEndl;

	return os;
}

zInStream& CInitialStateFusionLamella::get(zInStream& is)
{
	zString token  = "";
	zString sName  = "";

	double centre[2], thickness[2];
	long x, y, z;
	bool bLinear;
	double upperFraction;

	is >> token;
	if(!is.good() || token != "Polymers")	// read first bilayer polymers
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;

		if(!is.good() || sName == "Polymers" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "Polymers")
			{
				if(is.good() && !sName.empty())
					m_Polymers1.push_back(sName);
				else
				{
					SetDataValid(false);
					return is;
				}

				is >> sName;
			}
		}
	}

	if(!is.good() || sName != "Polymers")	// read second bilayer polymers
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
					m_Polymers2.push_back(sName);
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
	if(!is.good() || token != "Centre")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> centre[0] >> centre[1];

		// Centre of bilayers must be a fraction between 0 and 1

		if(!is.good() || centre[0] < 0.0 || centre[0] > 1.0
				      || centre[1] < 0.0 || centre[1] > 1.0
					  || centre[0] > centre[1])
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Thickness")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> thickness[0] >> thickness[1];

		// Bilayer thicknesses must be greater than a bead diameter. We cannot check 
		// here that it is less than the SimBox size but that will be done in CInputData.

		if(!is.good() || thickness[0] < 1.0 || thickness[1] < 1.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Next, read a flag showing whether the polymers in the bilayers should
	// be given linear coordinates in the direction of the lamellae normal
	// or be randomly arranged around their head-tail axis.

	is >> token;
	if(!is.good() || token != "Linearise")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> bLinear;

		if(!is.good())
		{
			SetDataValid(false);
			return is;
		}
	}

	// Finally, read the fractions of the polymers that are in the upper monolayer.
	// Obviously, the remainder of the polymers are in the lower monolayer.
	// We assume that the two bilayers have exactly the same polymer types, and
	// get the number of types from the size of the m_Polymers1 vector.

	is >> token;
	if(!is.good() || token != "UpperFraction")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		for(short i=0; i<m_Polymers1.size(); i++)
		{
			is >> upperFraction;

			// Fraction must lie between 0 and 1

			if(!is.good() || upperFraction < 0.0 || upperFraction > 1.0)
			{
				SetDataValid(false);
				return is;
			}
			else
				m_UpperFractions.push_back(upperFraction);
		}
	}

	
	// Data has been read successfully so store it in the member variables

	m_X				= x;
	m_Y				= y;
	m_Z				= z;
	m_Centre[0]		= centre[0];
	m_Centre[1]		= centre[1];
	m_Thickness[0]	= thickness[0];
	m_Thickness[1]	= thickness[1];
	m_bLinearise	= bLinear;

	return is;
}

// Function to use the CFusionLamellaBuilder object to set the coordinates of 
// all the beads in the simulation into an initial state with two parallel 
// bilayers. Note that the builder is local to this CInitialStateLamella object. 
// We use the CCompositeLamellaBuilder to construct two composite lamellae and
// position their beads in the simulation box.

bool CInitialStateFusionLamella::Assemble(CInitialState& riState) const
{
	CFusionLamellaBuilder lamella(m_PolymerTypes1, m_PolymerTypes2,
								  m_X, m_Y, m_Z, m_Centre, m_Thickness,
								  m_bLinearise, m_UpperFractions,
								  m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
								  m_BilayerCentre, m_LowerHead, m_UpperHead);

	return lamella.Assemble(riState);
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateFusionLamella::Copy() const
{
	CInitialStateFusionLamella* pISL = new CInitialStateFusionLamella;	// new data object

	*pISL = *this;		// assign new data values to new object

	return pISL;
}

// Virtual function to check that the data supplied by the user to assemble a
// lamella initial state is consistent. We check that the centre of 
// the bilayer and its thickness cause it to be created within the SimBox and
// not overlapping one of the PBC boundaries, or the wall if it is present;
// we also calculate the area of each monolayer for later use.
//
// Note there should not be a wall around the SimBox other than one with the
// same orientation as the bilayer.
//
// This function cannot be const because we store data for later use.

bool CInitialStateFusionLamella::ValidateData(const CInputData& riData)
{
	// Check that the polymer names specified in the two lamellae actually
	// occur in the map, and that any polymer name in the second list does 
	// not occur in the first list: the same polymer type cannot be in both 
	// lamellae.

	cStringSequenceIterator polymer;

	for(polymer=m_Polymers1.begin(); polymer!=m_Polymers1.end(); polymer++)
	{
		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end())
		{
			m_PolymerTypes1.push_back( (*riData.GetPolymerNamesMap().find(*polymer)).second);
		}
		else
			return false;
	}

	for(polymer=m_Polymers2.begin(); polymer!=m_Polymers2.end(); polymer++)
	{
		long polymerType = (*riData.GetPolymerNamesMap().find(*polymer)).second;

		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end() &&
		   find(m_PolymerTypes1.begin(), m_PolymerTypes1.end(), polymerType) == m_PolymerTypes1.end())
		{
			m_PolymerTypes2.push_back( (*riData.GetPolymerNamesMap().find(*polymer)).second);
		}
		else
			return false;
	}

	// Calculate the centres, widths and area of the lamellae.
	//
	// If there is no wall the functions just return zero for its thickness.
	// Note that the lamellae are assembled using the normal to define the "top"
	// and "bottom" of the SimBox.
	//
	// lower	= thickness of wall on bottom of SimBox.
	// upper	= distance from bottom of SimBox to start of wall on top face
	// centre	= fraction of SimBox width at centre of bilayer

	double l1, l2, lower, upper, centre[2];

	if(m_X == 1)
	{
		l1			= riData.GetSimBoxYLength();
		l2			= riData.GetSimBoxZLength();
		lower		= riData.GetCNTXCellWidth()*static_cast<double>(riData.GetWallXWidth());
		upper		= riData.GetSimBoxXLength()-lower;
		centre[0]	= m_Centre[0]*riData.GetSimBoxXLength();
		centre[1]	= m_Centre[1]*riData.GetSimBoxXLength();
	}
	else if(m_Y == 1)
	{
		l1			= riData.GetSimBoxXLength();
		l2			= riData.GetSimBoxZLength();
		lower		= riData.GetCNTYCellWidth()*static_cast<double>(riData.GetWallYWidth());
		upper		= riData.GetSimBoxYLength()-lower;
		centre[0]	= m_Centre[0]*riData.GetSimBoxYLength();
		centre[1]	= m_Centre[1]*riData.GetSimBoxZLength();
	}
	else if(m_Z == 1)
	{
		l1			= riData.GetSimBoxXLength();
		l2			= riData.GetSimBoxYLength();
		lower		= riData.GetCNTZCellWidth()*static_cast<double>(riData.GetWallZWidth());
		upper		= riData.GetSimBoxZLength()-lower;
		centre[0]	= m_Centre[0]*riData.GetSimBoxZLength();
		centre[1]	= m_Centre[1]*riData.GetSimBoxZLength();
	}
	else
		return false;

	if((centre[0] + 0.5*m_Thickness[0]) > upper || (centre[0] - 0.5*m_Thickness[0]) < lower)
		return false;
	else if((centre[1] + 0.5*m_Thickness[1]) > upper || (centre[1] - 0.5*m_Thickness[1]) < lower)
		return false;
	else if(m_X == 1 && (riData.GetWallYWidth() > 0 || riData.GetWallZWidth() > 0))
		return false;
	else if(m_Y == 1 && (riData.GetWallXWidth() > 0 || riData.GetWallZWidth() > 0))
		return false;
	else if(m_Z == 1 && (riData.GetWallXWidth() > 0 || riData.GetWallYWidth() > 0))
		return false;

	// Input data has been checked so allow the initialisation to proceed.
	// Copy the coordinates of the bilayer midplane and its extremities into
	// local data for passing to the CLamellaBuilder. Note that we have to keep
	// track of the bilayer orientation to use this data correctly, but we
	// can be sure that it is valid and does not overlap the walls or boundaries.

	m_BilayerArea		= l1*l2;
	m_Bilayer1Length	= l1;
	m_Bilayer2Length	= l2;

	m_BilayerCentre[0]	= centre[0];
	m_LowerHead[0]		= centre[0] - 0.5*m_Thickness[0];
	m_UpperHead[0]		= centre[0] + 0.5*m_Thickness[0];

	m_BilayerCentre[1]	= centre[1];
	m_LowerHead[1]		= centre[1] - 0.5*m_Thickness[1];
	m_UpperHead[1]		= centre[1] + 0.5*m_Thickness[1];

	return true;
}
