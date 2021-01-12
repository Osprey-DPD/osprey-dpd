/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateMultiLamella.cpp: implementation of the CInitialStateMultiLamella class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "InitialStateMultiLamella.h"
#include "MultiLamellaBuilder.h"
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

const zString CInitialStateMultiLamella::m_Type = "multilamella";

const zString CInitialStateMultiLamella::GetType()
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
	CInitialStateData* Create() {return new CInitialStateMultiLamella();}

	const zString id = CInitialStateMultiLamella::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Note that there is no way to set default data for a lamella as we 
// don't have access to the polymers that are defined in the control data file.

CInitialStateMultiLamella::CInitialStateMultiLamella() : m_X(0), m_Y(0), m_Z(0), 
														 m_Centre(0.0),
													     m_Thickness(0.0),
													     m_SolventGap(0.0),
														 m_bLinearise(false),
													     m_LamellaTotal(0),
														 m_BilayerArea(0.0),
														 m_Bilayer1Length(0.0),
														 m_Bilayer2Length(0.0),
														 m_DeltaCentre(0.0)
{
	m_Polymers.clear();
	m_UpperFractions.clear();

	m_PolymerTypes.clear();
	m_BilayerCentres.clear();
}

CInitialStateMultiLamella::~CInitialStateMultiLamella()
{

}

CInitialStateMultiLamella& CInitialStateMultiLamella::operator=(const CInitialStateMultiLamella& oldISL)
{
	if(this != &oldISL)
	{
		m_Polymers			= oldISL.m_Polymers;
		m_X					= oldISL.m_X;
		m_Y					= oldISL.m_Y;
		m_Z					= oldISL.m_Z;
		m_Centre			= oldISL.m_Centre;
		m_Thickness			= oldISL.m_Thickness;
		m_SolventGap		= oldISL.m_SolventGap;
		m_bLinearise		= oldISL.m_bLinearise;
		m_UpperFractions	= oldISL.m_UpperFractions;
		m_PolymerTypes		= oldISL.m_PolymerTypes;
		m_LamellaTotal		= oldISL.m_LamellaTotal;
	    m_BilayerArea		= oldISL.m_BilayerArea;
	    m_Bilayer1Length	= oldISL.m_Bilayer1Length;
	    m_Bilayer2Length	= oldISL.m_Bilayer2Length;
	    m_DeltaCentre		= oldISL.m_DeltaCentre;
		m_BilayerCentres	= oldISL.m_BilayerCentres;
	}

	return *this;
}

zString CInitialStateMultiLamella::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateMultiLamella::put(zOutStream& os) const
{
	os << "State	" << "multilamella"		<< zEndl;
	os << "			" << "Polymer		  ";

	for(cStringSequenceIterator iterName=m_Polymers.begin(); iterName!=m_Polymers.end(); iterName++)
	{
		os << " " << (*iterName);
	}

	os << zEndl;
	os << "			" << "Normal		"	<< m_X << "  " << m_Y << "  " << m_Z << zEndl;
	os << "			" << "Centre		"	<< m_Centre			<< zEndl;
	os << "			" << "Thickness		"	<< m_Thickness		<< zEndl;
	os << "			" << "SolventGap	"	<< m_SolventGap		<< zEndl;
	os << "			" << "Linearise		"	<< m_bLinearise		<< zEndl;

	os << "	        " << "UpperFraction	  ";

	for(long unsigned int i=0; i<m_Polymers.size(); i++)
	{
		os << "	" << m_UpperFractions.at(i);
	}
	os << zEndl;

	return os;
}

zInStream& CInitialStateMultiLamella::get(zInStream& is)
{
	zString token					= "";
	zString sName					= "";
	zString polymer;
	double centre, thickness, gap;
	long x, y, z;
	bool bLinear;
	double upperFraction			= 0.0;

	is >> token;
	if(!is.good() || token != "Polymer")
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
		is >> centre;

		// Centre of bilayer must be a fraction between 0 and 1

		if(!is.good() || centre < 0.0 || centre > 1.0)
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

		// Bilayer thickness must be greater than a bead diameter. We cannot check 
		// here that it is less than the SimBox size but that will be done in ValidateData().

		if(!is.good() || thickness < 1.0)
		{
			SetDataValid(false);
			return is;
		}
	}
	is >> token;
	if(!is.good() || token != "SolventGap")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> gap;

		// Solvent gap thickness must be greater than a bead diameter. 
		// We cannot check here that it is less than the SimBox size but 
		// that will be done in ValidateData().

		if(!is.good() || gap < 1.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Next, read a flag showing whether the polymers in the bilayer should
	// be given linear coordinates in the direction of the lamella normal
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

	// Read the fractions of the polymers that are in the upper monolayer.
	// Obviously, the remainder of the polymers are in the lower monolayer.
	// We know how many elements to read in from the size of the m_Polymer vector.

	is >> token;
	if(!is.good() || token != "UpperFraction")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		for(long unsigned int i=0; i<m_Polymers.size(); i++)
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
	m_Centre		= centre;
	m_Thickness		= thickness;
	m_SolventGap	= gap;
	m_bLinearise	= bLinear;

	return is;
}

// Function to use the CLamellaBuilder object to set the coordinates of all the 
// beads in the simulation into a lamella initial state. Note that the builder 
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStateMultiLamella::Assemble(CInitialState& riState) const
{
	CMultiLamellaBuilder lamella(m_PolymerTypes, m_X, m_Y, m_Z,
								 m_BilayerCentres, m_UpperFractions, 
								 m_bLinearise, m_Thickness,
								 m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length);

	return lamella.Assemble(riState);
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateMultiLamella::Copy() const
{
	CInitialStateMultiLamella* pISL = new CInitialStateMultiLamella;	// new data object

	*pISL = *this;		// assign new data values to new object

	return pISL;
}

// Virtual function to check that the data supplied by the user to assemble a
// multilamella initial state is consistent. We check that the centre of 
// the first (lowest) lamella and its thickness cause it to be created within 
// the SimBox and not overlapping the PBC boundary, or the wall if 
// it is present; we also calculate the area of each lamella for later use.
// We have already checked that there is at least one lamella, so we just check
// that the ones above the first lamella fit into the SimBox given the 
// specified thickness, solvent gap and number.
//
// Note there should not be a wall around the SimBox other than one with the
// same orientation as the bilayer.
//
// This function cannot be const because we store data for later use.

bool CInitialStateMultiLamella::ValidateData(const CInputData& riData)
{
	// Check that the polymer names specified in the lamella actually
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

	// Check that neither monolayer is empty.

	double upperFracSum = 0.0;

	for(long unsigned int ip=0; ip<m_UpperFractions.size(); ip++)
	{
		upperFracSum += m_UpperFractions.at(ip);
	}

	if(upperFracSum < 0.000001 || (1.0 - upperFracSum) < 0.000001)
		return false;

	// Calculate the centre, width and area of the first lamella.
	//
	// If there is no wall the functions just return zero for its thickness.
	// Note that the lamella is assembled using the normal to define the "top"
	// and "bottom" of the SimBox.
	//
	// lower	= thickness of wall on bottom of SimBox.
	// upper	= distance from bottom of SimBox to start of wall on top face
	// centre	= fraction of SimBox width at centre of bilayer

	double l1, l2, lower, upper, centre;

	if(m_X == 1)
	{
		l1		= riData.GetSimBoxYLength();
		l2		= riData.GetSimBoxZLength();
		lower	= riData.GetCNTXCellWidth()*static_cast<double>(riData.GetWallXWidth());
		upper	= riData.GetSimBoxXLength()-lower;
		centre	= m_Centre*riData.GetSimBoxXLength();
	}
	else if(m_Y == 1)
	{
		l1		= riData.GetSimBoxXLength();
		l2		= riData.GetSimBoxZLength();
		lower	= riData.GetCNTYCellWidth()*static_cast<double>(riData.GetWallYWidth());
		upper	= riData.GetSimBoxYLength()-lower;
		centre	= m_Centre*riData.GetSimBoxYLength();
	}
	else if(m_Z == 1)
	{
		l1		= riData.GetSimBoxXLength();
		l2		= riData.GetSimBoxYLength();
		lower	= riData.GetCNTZCellWidth()*static_cast<double>(riData.GetWallZWidth());
		upper	= riData.GetSimBoxZLength()-lower;
		centre	= m_Centre*riData.GetSimBoxZLength();
	}
	else
		return false;

	// Get the lamellae repeat distance: centre to centre distance

	m_DeltaCentre	= m_Thickness + m_SolventGap;

	if((centre + 0.5*m_Thickness) > upper || (centre - 0.5*m_Thickness) < lower)
		return false;
	else if(m_X == 1 && (riData.GetWallYWidth() > 0 || riData.GetWallZWidth() > 0))
		return false;
	else if(m_Y == 1 && (riData.GetWallXWidth() > 0 || riData.GetWallZWidth() > 0))
		return false;
	else if(m_Z == 1 && (riData.GetWallXWidth() > 0 || riData.GetWallYWidth() > 0))
		return false;
	else if(m_DeltaCentre > upper)
		return false;

	// Input data has been checked so allow the initialisation to proceed.
	// Calculate the centre, thickness and gap for each succeeding lamella 
	// above the first.

	// Copy the coordinates of the bilayer midplane and its extremities into
	// local data for passing to the CLamellaBuilder. Note that we have to keep
	// track of the bilayer orientation to use this data correctly, but we
	// can be sure that it is valid and does not overlap the walls or boundaries.

	m_BilayerArea		= l1*l2;
	m_Bilayer1Length	= l1;
	m_Bilayer2Length	= l2;

	// Calculate the number of lamella given that they are not allowed to
	// overlap the walls and the repeat distance (the distance from the lamella
	// lower edge to the next lower edge) is specified by the user. Note that this
	// does allow lamellae to overlap each other. If the user wants them to
	// be very close together, to simulate dehydrated headgroups perhaps, we 
	// allow this. 

	double upperDistance = upper - (centre - 0.5*m_Thickness);

	m_LamellaTotal = static_cast<long>(upperDistance/m_DeltaCentre);

	// Add the centres and extent of each lamella to the containers

	for(long lam=0; lam<m_LamellaTotal; lam++)
	{
		m_BilayerCentres.push_back(centre);
		centre += m_DeltaCentre;
	}


	return true;
}
