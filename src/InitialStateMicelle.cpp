/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateMicelle.cpp: implementation of the CInitialStateMicelle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "InitialStateMicelle.h"
#include "MicelleBuilder.h"
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

const zString CInitialStateMicelle::m_Type = "micelle";

const zString CInitialStateMicelle::GetType()
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
	CInitialStateData* Create() {return new CInitialStateMicelle();}

	const zString id = CInitialStateMicelle::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Note that there is no way to set default data for a micelle as we 
// don't have access to the polymers that are defined in the control data file.

CInitialStateMicelle::CInitialStateMicelle() : m_XC(0.0), m_YC(0.0), m_ZC(0.0), 
											   m_Radius(0.0)
{
	m_Polymers.clear();
	m_PolymerTypes.clear();
}

CInitialStateMicelle::~CInitialStateMicelle()
{

}

CInitialStateMicelle& CInitialStateMicelle::operator=(const CInitialStateMicelle& oldISM)
{
	if(this != &oldISM)
	{
		m_Polymers			= oldISM.m_Polymers;
		m_XC				= oldISM.m_XC;
		m_YC				= oldISM.m_YC;
		m_ZC				= oldISM.m_ZC;
		m_Radius			= oldISM.m_Radius;
		m_PolymerTypes		= oldISM.m_PolymerTypes;
	}

	return *this;
}

zString CInitialStateMicelle::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateMicelle::put(zOutStream& os) const
{
	os << "State	" << "micelle"	<< zEndl;

	os << "			" << "Polymers		  ";

	for(cStringSequenceIterator iterName=m_Polymers.begin(); iterName!=m_Polymers.end(); iterName++)
	{
		os << " " << (*iterName);
	}

	os << zEndl;

	os << "			" << "Centre		"	<< m_XC << "  " << m_YC << "  " << m_ZC << zEndl;
	os << "			" << "Radius		"	<< m_Radius	<< zEndl;

	return os;
}

zInStream& CInitialStateMicelle::get(zInStream& is)
{
	zString token  = "";
	zString sName  = "";

	double xc, yc, zc;
	double radius;

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
	if(!is.good() || token != "Radius")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> radius;

		// Micelle radius must be greater than a bead diameter. We cannot check 
		// here that it is less than the SimBox size but that will be done in CInputData.

		if(!is.good() || radius < 1.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Data has been read successfully so store it in the member variables

	m_XC			= xc;
	m_YC			= yc;
	m_ZC			= zc;
	m_Radius		= radius;

	return is;
}

// Function to use the CMicelleBuilder object to set the coordinates of all the 
// beads in the simulation into a micelle initial state. Note that the builder 
// is local to this CInitialStateMicelle object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// spherical micelle required.

bool CInitialStateMicelle::Assemble(CInitialState& riState) const
{
	CMicelleBuilder micelle(m_PolymerTypes, m_XC, m_YC, m_ZC, m_Radius);

	return micelle.Assemble(riState);
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateMicelle::Copy() const
{
	CInitialStateMicelle* pISL = new CInitialStateMicelle;	// new data object

	*pISL = *this;		// assign new data values to new object

	return pISL;
}

// Virtual function to check that the data supplied by the user to assemble a
// micelle initial state is consistent. We check that the centre of 
// the micelle and its radius cause it to be created within the SimBox and
// not overlapping one of the PBC boundaries, or the wall if it is present.
//
// This function cannot be const because we store data for later use.

bool CInitialStateMicelle::ValidateData(const CInputData& riData)
{
	// Check that the polymer names specified in the micelle actually
	// occur in the map and store their types for later use.

	for(cStringSequenceIterator polymer=m_Polymers.begin(); polymer!=m_Polymers.end(); polymer++)
	{
		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end() &&
           find(m_PolymerTypes.begin(), m_PolymerTypes.end(), 
		   (*riData.GetPolymerNamesMap().find(*polymer)).second) == m_PolymerTypes.end())

		{
			m_PolymerTypes.push_back( (*riData.GetPolymerNamesMap().find(*polymer)).second);
		}
		else
			return false;
	}

	// Calculate the centre and radius of the micelle.
	//
	// If there is no wall the functions just return zero for its thickness.
	//
	// For each of the SimBox axes, we check that the micelle does not overlap
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


	if((centre[0] + m_Radius) > upper[0] || (centre[0] - m_Radius) < lower[0])
		return false;
	else if((centre[1] + m_Radius) > upper[1] || (centre[1] - m_Radius) < lower[1])
		return false;
	else if((centre[2] + m_Radius) > upper[2] || (centre[2] - m_Radius) < lower[2])
		return false;

	// Input data has been checked so allow the initialisation to proceed.

	return true;
}
