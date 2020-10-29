/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateMultiVesicle.cpp: implementation of the CInitialStateMultiVesicle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "InitialStateMultiVesicle.h"
#include "MultiVesicleBuilder.h"
#include "InputData.h"
#include "InitialState.h"
#include "Polymer.h"


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

const zString CInitialStateMultiVesicle::m_Type = "multivesicle";

const zString CInitialStateMultiVesicle::GetType()
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
	CInitialStateData* Create() {return new CInitialStateMultiVesicle();}

	const zString id = CInitialStateMultiVesicle::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitialStateMultiVesicle::CInitialStateMultiVesicle() : m_XC(0.0), m_YC(0.0), m_ZC(0.0), 
													   m_OuterRadius(0.0), 
													   m_Thickness(0.0), 
													   m_InnerRadius(0.0)
{
	m_Polymers.clear();
	m_PolymerTypes.clear();
}

CInitialStateMultiVesicle::~CInitialStateMultiVesicle()
{

}

CInitialStateMultiVesicle& CInitialStateMultiVesicle::operator=(const CInitialStateMultiVesicle& oldISV)
{
	if(this != &oldISV)
	{
		m_Polymers			= oldISV.m_Polymers;
		m_XC				= oldISV.m_XC;
		m_YC				= oldISV.m_YC;
		m_ZC				= oldISV.m_ZC;
		m_OuterRadius		= oldISV.m_OuterRadius;
		m_Thickness			= oldISV.m_Thickness;
		m_InnerRadius		= oldISV.m_InnerRadius;
		m_PolymerTypes		= oldISV.m_PolymerTypes;
	}

	return *this;
}

zString CInitialStateMultiVesicle::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateMultiVesicle::put(zOutStream& os) const
{
	os << "State	" << "multivesicle"			<< zEndl;

	os << "			" << "Polymers		  ";

	for(cStringSequenceIterator iterName=m_Polymers.begin(); iterName!=m_Polymers.end(); iterName++)
	{
		os << " " << (*iterName);
	}

	os << zEndl;

	os << "			" << "Centre		"	<< m_XC << "  " << m_YC << "  " << m_ZC << zEndl;
	os << "			" << "OuterRadius	"	<< m_OuterRadius	<< zEndl;
	os << "			" << "Thickness		"	<< m_Thickness		<< zEndl;

	return os;
}

zInStream& CInitialStateMultiVesicle::get(zInStream& is)
{
	zString token  = "";
	zString sName  = "";

	double xc, yc, zc;
	double outerRadius, thickness;

	is >> token;
	if(!is.good() || token != "Polymers")
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
					m_Polymers.push_back(sName);
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
		is >> xc >> yc >> zc;
		
		// Force the centre to lie within the SimBox

		if(!is.good() || xc < 0.01 || yc < 0.01 || zc < 0.01 
					  || xc > 0.99 || yc > 0.99 || zc > 0.99 )
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
		is >> outerRadius;

		// Vesicle radius must be greater than a bead diameter. We cannot check 
		// here that it is less than the SimBox size but that will be done in CInputData.

		if(!is.good() || outerRadius < 1.0)
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
		is >> thickness;

		// Vesicle bilayer thickness must be greater than a bead diameter and
		// less than the outer radius: this is equivalent to ensuring that the
		// inner radius is greater than zero. We assume the bead 
		// diameter sets the length-scale here, but this might not be true if 
		// beads have different radii.
		// We cannot check here that it is thick enough to hold all the beads
		// in the specified polymer, but that will be done in ValidateData() below.

		if(!is.good() || thickness < 1.0 || thickness > outerRadius )
		{
			SetDataValid(false);
			return is;
		}
	}

	// Data has been read successfully so store it in the member variables

	m_XC			= xc;
	m_YC			= yc;
	m_ZC			= zc;
	m_OuterRadius	= outerRadius;
	m_Thickness		= thickness;

	return is;
}

// Function to use the CLamellaBuilder object to set the coordinates of all the 
// beads in the simulation into a lamella initial state. Note that the builder 
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStateMultiVesicle::Assemble(CInitialState& riState) const
{
//	CMultiVesicleBuilder vesicle(m_PolymerTypes, m_XC,  m_YC,  m_ZC,  
//							m_OuterRadius, m_InnerRadius,  m_Thickness);

//	return vesicle.Assemble(riState);
	return false;
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateMultiVesicle::Copy() const
{
	CInitialStateMultiVesicle* pISV = new CInitialStateMultiVesicle;	// new data object

	*pISV = *this;		// assign new data values to new object

	return pISV;
}

// Virtual function to check that the data supplied by the user to assemble a
// vesicle initial state is consistent. 
//
// This function cannot be const because we store data for later use.

bool CInitialStateMultiVesicle::ValidateData(const CInputData& riData)
{
	// Check that the polymer names specified in the vesicle actually
	// occur in the map and store their types for later use.

	for(cStringSequenceIterator polymer=m_Polymers.begin(); polymer!=m_Polymers.end(); polymer++)
	{
		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end())
		{
			m_PolymerTypes.push_back( (*riData.GetPolymerNamesMap().find(*polymer)).second);
		}
		else
			return false;
	}

	// Calculate the centre and radius of the vesicle.
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

	centre[0]	= m_XC*riData.GetSimBoxXLength();
	centre[1]	= m_YC*riData.GetSimBoxYLength();
	centre[2]	= m_ZC*riData.GetSimBoxZLength();

	lower[0]	= riData.GetCNTXCellWidth()*static_cast<double>(riData.GetWallXWidth());
	upper[0]	= riData.GetSimBoxXLength()-lower[0];

	lower[1]	= riData.GetCNTYCellWidth()*static_cast<double>(riData.GetWallYWidth());
	upper[1]	= riData.GetSimBoxYLength()-lower[1];

	lower[2]	= riData.GetCNTZCellWidth()*static_cast<double>(riData.GetWallZWidth());
	upper[2]	= riData.GetSimBoxZLength()-lower[2];


	if((centre[0] + m_OuterRadius) > upper[0] || (centre[0] - m_OuterRadius) < lower[0])
		return false;
	else if((centre[1] + m_OuterRadius) > upper[1] || (centre[1] - m_OuterRadius) < lower[1])
		return false;
	else if((centre[2] + m_OuterRadius) > upper[2] || (centre[2] - m_OuterRadius) < lower[2])
		return false;

	// Define supplementary variables for the vesicle. Note that the thickness
	// has been checked to ensure it is less than half the outer radius above.

	m_InnerRadius = m_OuterRadius - m_Thickness;

	// Input data has been checked so allow the initialisation to proceed.

	return true;
}

