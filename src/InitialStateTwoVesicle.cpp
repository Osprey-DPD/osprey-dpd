/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateTwoVesicle.cpp: implementation of the CInitialStateTwoVesicle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "InitialStateTwoVesicle.h"
#include "TwoVesicleBuilder.h"
#include "InputData.h"
#include "InitialState.h"
#include "Polymer.h"

// STL using declarations

	using std::find;


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

const zString CInitialStateTwoVesicle::m_Type = "twovesicle";

const zString CInitialStateTwoVesicle::GetType()
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
	CInitialStateData* Create() {return new CInitialStateTwoVesicle();}

	const zString id = CInitialStateTwoVesicle::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitialStateTwoVesicle::CInitialStateTwoVesicle()
{
	for(short int i=0; i<2; i++)
	{
		m_XC[i]				= 0.0;
		m_YC[i]				= 0.0;
		m_ZC[i]				= 0.0;
		m_OuterRadius[i]	= 0.0;
		m_Thickness[i]		= 0.0;
		m_InnerRadius[i]	= 0.0;
		m_bPatches1[i]		= false;
		m_bPatches2[i]		= false;
	}

	m_Polymers1.clear();
	m_Polymers2.clear();
	m_Interior1.clear();
	m_Interior2.clear();

	m_PolymerTypes1.clear();
	m_PolymerTypes2.clear();
	m_InteriorTypes1.clear();
	m_InteriorTypes2.clear();

	m_OuterFractions1.clear();
	m_OuterFractions2.clear();
}

CInitialStateTwoVesicle::~CInitialStateTwoVesicle()
{

}

CInitialStateTwoVesicle& CInitialStateTwoVesicle::operator=(const CInitialStateTwoVesicle& oldISV)
{
	if(this != &oldISV)
	{
		m_Polymers1			= oldISV.m_Polymers1;
		m_Polymers2			= oldISV.m_Polymers2;
		m_Interior1			= oldISV.m_Interior1;
		m_Interior2			= oldISV.m_Interior2;
		m_XC[0]				= oldISV.m_XC[0];
		m_YC[0]				= oldISV.m_YC[0];
		m_ZC[0]				= oldISV.m_ZC[0];
		m_XC[1]				= oldISV.m_XC[1];
		m_YC[1]				= oldISV.m_YC[1];
		m_ZC[1]				= oldISV.m_ZC[1];
		m_OuterRadius[0]	= oldISV.m_OuterRadius[0];
		m_OuterRadius[1]	= oldISV.m_OuterRadius[1];
		m_Thickness[0]		= oldISV.m_Thickness[0];
		m_Thickness[1]		= oldISV.m_Thickness[1];
		m_OuterFractions1	= oldISV.m_OuterFractions1;
		m_OuterFractions2	= oldISV.m_OuterFractions2;
		m_bPatches1[0]		= oldISV.m_bPatches1[0];
		m_bPatches1[1]		= oldISV.m_bPatches1[1];
		m_bPatches2[0]		= oldISV.m_bPatches2[0];
		m_bPatches2[1]		= oldISV.m_bPatches2[1];
		m_InnerRadius[0]	= oldISV.m_InnerRadius[0];
		m_InnerRadius[1]	= oldISV.m_InnerRadius[1];
		m_PolymerTypes1		= oldISV.m_PolymerTypes1;
		m_PolymerTypes2		= oldISV.m_PolymerTypes2;
		m_InteriorTypes1	= oldISV.m_InteriorTypes1;
		m_InteriorTypes2	= oldISV.m_InteriorTypes2;
	}

	return *this;
}

zString CInitialStateTwoVesicle::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateTwoVesicle::put(zOutStream& os) const
{
	os << "State	" << "twovesicle"  << zEndl;

	os << "			" << "Polymers		  ";
	for(cStringSequenceIterator iterName1=m_Polymers1.begin(); iterName1!=m_Polymers1.end(); iterName1++)
	{
		os << " " << (*iterName1);
	}
	os << zEndl;

	os << "			" << "Interior		  ";
	for(cStringSequenceIterator iterInt1=m_Interior1.begin(); iterInt1!=m_Interior1.end(); iterInt1++)
	{
		os << " " << (*iterInt1);
	}
	os << zEndl;

	os << "			" << "Polymers		  ";
	for(cStringSequenceIterator iterName2=m_Polymers2.begin(); iterName2!=m_Polymers2.end(); iterName2++)
	{
		os << " " << (*iterName2);
	}
	os << zEndl;

	os << "			" << "Interior		  ";
	for(cStringSequenceIterator iterInt2=m_Interior2.begin(); iterInt2!=m_Interior2.end(); iterInt2++)
	{
		os << " " << (*iterInt2);
	}
	os << zEndl;

	os << "			" << "Centre		"	<< m_XC[0] << "  "	<< m_YC[0] << "  "  << m_ZC[0] << "  "
											<< m_XC[1] << "  "	<< m_YC[1] << "  "  << m_ZC[1] << zEndl;
	os << "			" << "OuterRadius	"	<< m_OuterRadius[0]	<< m_OuterRadius[1]	<< zEndl;
	os << "			" << "Thickness		"	<< m_Thickness[0]	<< m_Thickness[1]	<< zEndl;
	os << "	        " << "OuterFraction	  ";

	for(long unsigned int i=0; i<m_Polymers1.size(); i++)
	{
		os << "	" << m_OuterFractions1.at(i);
	}
	os << zEndl;

	os << "	        " << "OuterFraction	  ";
	for(long unsigned int i=0; i<m_Polymers2.size(); i++)
	{
		os << "	" << m_OuterFractions2.at(i);
	}
	os << zEndl;

	os << "			" << "Patches 		"	<< m_bPatches1[0] << "  " << m_bPatches1[1] << zEndl;

	os << "			" << "Patches 		"	<< m_bPatches2[0] << "  " << m_bPatches2[1] << zEndl;

	return os;
}

zInStream& CInitialStateTwoVesicle::get(zInStream& is)
{
	zString token  = "";
	zString sName  = "";

	double xc[2], yc[2], zc[2];
	double outerRadius[2], thickness[2];
	double outerFraction;
	bool bPatches1[2];
	bool bPatches2[2];

	is >> token;
	if(!is.good() || token != "Polymers")	// read first vesicle polymers
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;

		if(!is.good() || sName == "Interior" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "Interior")
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

	if(!is.good() || sName != "Interior")	// read polymers inside first vesicle 
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
					m_Interior1.push_back(sName);
				else
				{
					SetDataValid(false);
					return is;
				}

				is >> sName;
			}
		}
	}

	if(!is.good() || sName != "Polymers")	// read second vesicle polymers
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;

		if(!is.good() || sName == "Interior" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "Interior")
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

	if(!is.good() || sName != "Interior")	// read polymers inside second vesicle 
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;

		if(!is.good() || sName == "Centre" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "Centre")
			{
				if(is.good() && !sName.empty())
					m_Interior2.push_back(sName);
				else
				{
					SetDataValid(false);
					return is;
				}

				is >> sName;
			}
		}
	}

	if(!is.good() || sName != "Centre")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> xc[0] >> yc[0] >> zc[0] >> xc[1] >> yc[1] >> zc[1] ;
		
		// Force the centre to lie within the SimBox

		if(!is.good() || xc[0] < 0.01 || yc[0] < 0.01 || zc[0] < 0.01 
					  || xc[0] > 0.99 || yc[0] > 0.99 || zc[0] > 0.99
					  || xc[1] < 0.01 || yc[1] < 0.01 || zc[1] < 0.01 
					  || xc[1] > 0.99 || yc[1] > 0.99 || zc[1] > 0.99 )
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "OuterRadius")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> outerRadius[0] >> outerRadius[1];

		// Vesicle radii must be greater than a bead diameter. We cannot check here 
		// that they are less than the SimBox size but that will be done in CInputData.

		if(!is.good() || outerRadius[0] < 1.0 || outerRadius[1] < 1.0)
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

		// Vesicle bilayer thickness must be greater than a bead diameter and
		// less than the outer radius: this is equivalent to ensuring that the
		// inner radius is greater than zero. We assume the bead 
		// diameter sets the length-scale here, but this might not be true if 
		// beads have different radii.
		// We cannot check here that it is thick enough to hold all the beads
		// in the specified polymer, but that will be done in ValidateData() below.

		if(!is.good() || thickness[0] < 1.0 || thickness[0] > outerRadius[0]
					  || thickness[1] < 1.0 || thickness[1] > outerRadius[1])
		{
			SetDataValid(false);
			return is;
		}
	}

	// Read the fractions of the polymers that are in the outer monolayers of
	// the two vesicles.  We know how many elements to read in from the sizes
	// of the m_Polymers1, m_Polymers2 vectors.

	is >> token;
	if(!is.good() || token != "OuterFraction")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		for(long unsigned int i=0; i<m_Polymers1.size(); i++)
		{
			is >> outerFraction;

			// Fraction must lie between 0 and 1

			if(!is.good() || outerFraction < 0.0 || outerFraction > 1.0)
			{
				SetDataValid(false);
				return is;
			}
			else
				m_OuterFractions1.push_back(outerFraction);
		}
	}

	is >> token;
	if(!is.good() || token != "OuterFraction")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		for(long unsigned int i=0; i<m_Polymers2.size(); i++)
		{
			is >> outerFraction;

			// Fraction must lie between 0 and 1

			if(!is.good() || outerFraction < 0.0 || outerFraction > 1.0)
			{
				SetDataValid(false);
				return is;
			}
			else
				m_OuterFractions2.push_back(outerFraction);
		}
	}

	// Next, read a flag showing whether the polymers should be distributed
	// homogeneously throughout the monolayers or arranged in patches
	// according to their type. This is possible for each vesicle independently.

	is >> token;
	if(!is.good() || token != "Patches")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> bPatches1[0] >> bPatches1[1];

		if(!is.good())
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Patches")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> bPatches2[0] >> bPatches2[1];

		if(!is.good())
		{
			SetDataValid(false);
			return is;
		}
	}


	// Data has been read successfully so store it in the member variables

	for(short int i=0; i<2; i++)
	{
		m_XC[i]				= xc[i];
		m_YC[i]				= yc[i];
		m_ZC[i]				= zc[i];
		m_OuterRadius[i]	= outerRadius[i];
		m_Thickness[i]		= thickness[i];
		m_bPatches1[i]		= bPatches1[i];
		m_bPatches2[i]		= bPatches2[i];
	}

	return is;
}

// Function to use the CLamellaBuilder object to set the coordinates of all the 
// beads in the simulation into a lamella initial state. Note that the builder 
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStateTwoVesicle::Assemble(CInitialState& riState) const
{
	CTwoVesicleBuilder vesicle(m_PolymerTypes1, m_PolymerTypes2, 
								m_InteriorTypes1, m_InteriorTypes2,
								m_OuterFractions1, m_OuterFractions2,
								m_XC, m_YC, m_ZC, m_OuterRadius, m_InnerRadius,  
								m_Thickness, m_bPatches1, m_bPatches2);

	return vesicle.Assemble(riState);
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateTwoVesicle::Copy() const
{
	CInitialStateTwoVesicle* pISV = new CInitialStateTwoVesicle;	// new data object

	*pISV = *this;		// assign new data values to new object

	return pISV;
}

// Virtual function to check that the data supplied by the user to assemble a
// vesicle fusion initial state is consistent. 
//
// This function cannot be const because we store data for later use.

bool CInitialStateTwoVesicle::ValidateData(const CInputData& riData)
{
	// Check that the polymer names specified in the vesicles actually
	// occur in the map and store their types for later use. Also check that 
	// the same polymer is not specified twice in the same vesicle, or in both 
	// vesicles or their interiors.

	cStringSequenceIterator polymer;

	for(polymer=m_Polymers1.begin(); polymer!=m_Polymers1.end(); polymer++)
	{
		const long polymerType = (*riData.GetPolymerNamesMap().find(*polymer)).second;

		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end() &&
		   find(m_PolymerTypes1.begin(), m_PolymerTypes1.end(), polymerType) == m_PolymerTypes1.end())
		{
			m_PolymerTypes1.push_back(polymerType);
		}
		else
			return false;
	}

	for(polymer=m_Interior1.begin(); polymer!=m_Interior1.end(); polymer++)
	{
		 const long polymerType = (*riData.GetPolymerNamesMap().find(*polymer)).second;

		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end() &&
		   find(m_PolymerTypes1.begin(), m_PolymerTypes1.end(), polymerType) == m_PolymerTypes1.end() &&
		   find(m_InteriorTypes1.begin(), m_InteriorTypes1.end(), polymerType) == m_InteriorTypes1.end())
		{
			m_InteriorTypes1.push_back(polymerType);
		}
		else
			return false;
	}

	for(polymer=m_Polymers2.begin(); polymer!=m_Polymers2.end(); polymer++)
	{
		const long polymerType = (*riData.GetPolymerNamesMap().find(*polymer)).second;

		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end() &&
		   find(m_PolymerTypes1.begin(), m_PolymerTypes1.end(), polymerType) == m_PolymerTypes1.end() &&
		   find(m_InteriorTypes1.begin(), m_InteriorTypes1.end(), polymerType) == m_InteriorTypes1.end() &&
		   find(m_PolymerTypes2.begin(), m_PolymerTypes2.end(), polymerType) == m_PolymerTypes2.end())
		{
			m_PolymerTypes2.push_back(polymerType);
		}
		else
			return false;
	}

	for(polymer=m_Interior2.begin(); polymer!=m_Interior2.end(); polymer++)
	{
		const long polymerType = (*riData.GetPolymerNamesMap().find(*polymer)).second;

		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end() &&
		   find(m_PolymerTypes1.begin(), m_PolymerTypes1.end(), polymerType) == m_PolymerTypes1.end() &&
		   find(m_InteriorTypes1.begin(), m_InteriorTypes1.end(), polymerType) == m_InteriorTypes1.end() &&
		   find(m_PolymerTypes2.begin(), m_PolymerTypes2.end(), polymerType) == m_PolymerTypes2.end() &&
		   find(m_InteriorTypes2.begin(), m_InteriorTypes2.end(), polymerType) == m_InteriorTypes2.end())
		{
			m_InteriorTypes2.push_back(polymerType);
		}
		else
			return false;
	}

	// Calculate the centre and radius of the vesicles.
	//
	// If there is no wall the functions just return zero for its thickness.
	//
	// For each of the SimBox axes, we check that the vesicle does not overlap
	// the wall, if it exists, in that dimension:
	//
	// lower	= thickness of wall on bottom of SimBox.
	// upper	= distance from bottom of SimBox to start of wall on top face
	// centre	= fraction of SimBox width at centre of bilayer

	double lower[3], upper[3], centre[3];

	for(short int i=0; i<2; i++)
	{
		centre[0]	= m_XC[i]*riData.GetSimBoxXLength();
		centre[1]	= m_YC[i]*riData.GetSimBoxYLength();
		centre[2]	= m_ZC[i]*riData.GetSimBoxZLength();

		lower[0]	= riData.GetCNTXCellWidth()*static_cast<double>(riData.GetWallXWidth());
		upper[0]	= riData.GetSimBoxXLength()-lower[0];

		lower[1]	= riData.GetCNTYCellWidth()*static_cast<double>(riData.GetWallYWidth());
		upper[1]	= riData.GetSimBoxYLength()-lower[1];

		lower[2]	= riData.GetCNTZCellWidth()*static_cast<double>(riData.GetWallZWidth());
		upper[2]	= riData.GetSimBoxZLength()-lower[2];


		if((centre[0] + m_OuterRadius[i]) > upper[0] || (centre[0] - m_OuterRadius[i]) < lower[0])
			return false;
		else if((centre[1] + m_OuterRadius[i]) > upper[1] || (centre[1] - m_OuterRadius[i]) < lower[1])
			return false;
		else if((centre[2] + m_OuterRadius[i]) > upper[2] || (centre[2] - m_OuterRadius[i]) < lower[2])
			return false;

		// Define supplementary variables for the vesicles. Note that the thickness
		// has been checked to ensure it is less than half the outer radius above.

		m_InnerRadius[i] = m_OuterRadius[i] - m_Thickness[i];
	}

	// Input data has been checked so allow the initialisation to proceed.

	return true;
}

