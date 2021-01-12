/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateFreeLamella.cpp: implementation of the CInitialStateFreeLamella class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "InitialStateFreeLamella.h"
#include "FreeLamellaBuilder.h"
#include "InputData.h"
#include "InitialState.h"
#include "Polymer.h"
#include "PolymerCrossLink.h"


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

const zString CInitialStateFreeLamella::m_Type = "freelamella";

const zString CInitialStateFreeLamella::GetType()
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
	CInitialStateData* Create() {return new CInitialStateFreeLamella();}

	const zString id = CInitialStateFreeLamella::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Note that there is no way to set default data for a lamella as we 
// don't have access to the polymers that are defined in the control data file.

CInitialStateFreeLamella::CInitialStateFreeLamella() : m_X(0), m_Y(0), m_Z(0), m_Thickness(0.0), m_Length(0.0), m_Width(0.0), 
                                                       m_bLinearise(false), m_bPolymerise(false),
				                        m_BilayerArea(0.0),
					                m_Bilayer1Length(0.0),
						         m_Bilayer2Length(0.0),
				                        m_Bilayer1Centre(0.0),
					                m_Bilayer2Centre(0.0),
			                                m_Bilayer3Centre(0.0),
					                m_UpperHead(0.0),
						         m_LowerHead(0.0)
						         
{
	m_Centre[0]   = 0.0;
	m_Centre[1]   = 0.0;
	m_Centre[2]   = 0.0;
	m_bPatches[0] = false;
	m_bPatches[1] = false;

	m_Polymers.clear();
	m_PolymerTypes.clear();
	m_UpperFractions.clear();
	m_CrossLinks.clear();
}

CInitialStateFreeLamella::~CInitialStateFreeLamella()
{

}

CInitialStateFreeLamella& CInitialStateFreeLamella::operator=(const CInitialStateFreeLamella& oldISL)
{
	if(this != &oldISL)
	{
		m_Polymers			= oldISL.m_Polymers;
		m_X					= oldISL.m_X;
		m_Y					= oldISL.m_Y;
		m_Z					= oldISL.m_Z;
		m_Centre[0]			= oldISL.m_Centre[0];
		m_Centre[1]			= oldISL.m_Centre[1];
		m_Centre[2]			= oldISL.m_Centre[2];
		m_Thickness			= oldISL.m_Thickness;
		m_Length			= oldISL.m_Length;
		m_Width				= oldISL.m_Width;
		m_bLinearise		= oldISL.m_bLinearise;
		m_UpperFractions	= oldISL.m_UpperFractions;
		m_bPatches[0]		= oldISL.m_bPatches[0];
		m_bPatches[1]		= oldISL.m_bPatches[1];
		m_bPolymerise		= oldISL.m_bPolymerise;
		m_CrossLinks		= oldISL.m_CrossLinks;

		m_PolymerTypes		= oldISL.m_PolymerTypes;
	    m_BilayerArea		= oldISL.m_BilayerArea;
	    m_Bilayer1Length	= oldISL.m_Bilayer1Length;
	    m_Bilayer2Length	= oldISL.m_Bilayer2Length;
	    m_Bilayer1Centre	= oldISL.m_Bilayer1Centre;
	    m_Bilayer2Centre	= oldISL.m_Bilayer2Centre;
	    m_Bilayer3Centre	= oldISL.m_Bilayer3Centre;
	    m_UpperHead			= oldISL.m_UpperHead;
	    m_LowerHead			= oldISL.m_LowerHead;
	}

	return *this;
}

zString CInitialStateFreeLamella::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateFreeLamella::put(zOutStream& os) const
{
	os << "State	" << "freelamella"	<< zEndl;

	os << "			" << "Polymers		  ";

	for(cStringSequenceIterator iterName=m_Polymers.begin(); iterName!=m_Polymers.end(); iterName++)
	{
		os << " " << (*iterName);
	}

	os << zEndl;

	os << "			" << "Normal		"	<< m_X << "  "		<< m_Y << "  " << m_Z << zEndl;
	os << "			" << "Centre		"	<< m_Centre[0] << "  " << m_Centre[1] << "  " << m_Centre[2] << zEndl;
	os << "			" << "Thickness		"	<< m_Thickness		<< zEndl;
	os << "			" << "Length		"	<< m_Length			<< zEndl;
	os << "			" << "Width			"	<< m_Width			<< zEndl;
	os << "			" << "Linearise 	"	<< m_bLinearise		<< zEndl;

	os << "	        " << "UpperFraction	  ";

	for(long unsigned int i=0; i<m_Polymers.size(); i++)
	{
		os << "	" << m_UpperFractions.at(i);
	}

	os << zEndl;

	os << "			" << "Patches 		"	<< m_bPatches[0] << "  " << m_bPatches[1] << zEndl;

	return os;
}

zInStream& CInitialStateFreeLamella::get(zInStream& is)
{
	zString token					= "";
	zString sName					= "";
	double cx						= 0.0;
	double cy						= 0.0;
	double cz						= 0.0;
	double	thickness				= 0.0;
	double	length					= 0.0;
	double	width					= 0.0;
	long x							= 0;
	long y							= 0;
	long z							= 0;
	bool bLinear					= true;
	double upperFraction			= 0.0;
	bool   bPatches[2];

	zString polymerisedName			= "";
	double  polymerisedFraction		= 0.0;
	long    polymerisedPosition		= 0;
	double	springConstant			= 0.0;
	double  unstretchedLength		= 0.0;

	// Local vectors to hold polymer data prior to storing it in CPolymerCrossLinks

	StringSequence	polymerNames;
	zDoubleVector	polymerFractions;
	zLongVector		polymerPositions;
	zDoubleVector	polymerSpringConstants;
	zDoubleVector	polymerLengths;


	is >> token;
	if(!is.good() || token != "Polymers")
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
		is >> cx >> cy >> cz;

		// Centre of bilayer must be a fraction between 0 and 1 in each dimension

		if(!is.good() || cx < 0.0 || cx > 1.0
					  || cy < 0.0 || cy > 1.0
					  || cz < 0.0 || cz > 1.0)
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
		// here that it is less than the SimBox size but that will be done in CInputData.

		if(!is.good() || thickness < 1.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Length")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> length;

		// Bilayer length must be greater than a bead diameter. We cannot check 
		// here that it is less than the SimBox size but that will be done in CInputData.

		if(!is.good() || length < 1.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Width")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> width;

		// Bilayer width must be greater than a bead diameter. We cannot check 
		// here that it is less than the SimBox size but that will be done in CInputData.

		if(!is.good() || width < 1.0)
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

	// Next, read a flag showing whether the polymers should be distributed
	// homogeneously throughout the monolayers or arranged in patches
	// according to their type.

	is >> token;
	if(!is.good() || token != "Patches")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> bPatches[0] >> bPatches[1];

		if(!is.good())
		{
			SetDataValid(false);
			return is;
		}
	}

	// If the polymerise flag shows no polymerisation in the initial state then
	// skip the remaining fields. This is done for backwards compatibility with
	// the previous input files. If new input data is defined, we will have to change
	// this.

	is >> token;
	if(!is.good() || token != "Polymerise")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> m_bPolymerise;

		if(!is.good())
		{
			SetDataValid(false);
			return is;
		}
		else if(m_bPolymerise)
		{

			// Read the cross-link data for each polymer in the bilayer.
			// We know how many there can be from the m_Polymers vector, 
			// but not all the polymers specified there need be polymerised.
			// We count how many names occur in the Polymer keyword data and
			// use that to determine the loops over the remaining data. We
			// store the polymerisationinformation in a set of vectors so that 
			// each piece can be read all on one line instead of one polymer 
			// after another.
			// The information required per polymer is:
			//
			// Name				e.g., Lipid		Cholesterol
			// Fraction				  0.5		1.0
			// Position				  0			1
			// SpringConstant		  128.0		16.0
			// UnstretchedLength	  0.5		1.0

			is >> token;
			if(!is.good() || token != "Polymer")
			{
				SetDataValid(false);
				return is;
			}
			else
			{
				is >> polymerisedName;

				while(polymerisedName != "Fraction")
				{
					if(is.good() && !polymerisedName.empty())
						polymerNames.push_back(polymerisedName);
					else
					{
						SetDataValid(false);
						return is;
					}

					is >> polymerisedName;
				}
			}

			if(!is.good() || polymerisedName != "Fraction")
			{
				SetDataValid(false);
				return is;
			}
			else
			{
				for(long unsigned int i=0; i<polymerNames.size(); i++)
				{			
					is >> polymerisedFraction;
					if(!is.good() || polymerisedFraction < 0.0 || polymerisedFraction > 1.0)
					{
						SetDataValid(false);
						return is;
					}
					polymerFractions.push_back(polymerisedFraction);
				}
			}

			// Bead in polymer architecture to attach polymerised bond to.
			// Note that we don't know the length of the polymer yet so we cannot
			// check the upper limit for this parameter.

			is >> token;
			if(!is.good() || token != "Position")
			{
				SetDataValid(false);
				return is;
			}
			else
			{
				for(long unsigned int i=0; i<polymerNames.size(); i++)
				{			
					is >> polymerisedPosition;
					if(!is.good() || polymerisedPosition < 0)
					{
						SetDataValid(false);
						return is;
					}
					polymerPositions.push_back(polymerisedPosition);
				}
			}

			is >> token;
			if(!is.good() || token != "SpringConstant")
			{
				SetDataValid(false);
				return is;
			}
			else
			{
				for(long unsigned int i=0; i<polymerNames.size(); i++)
				{			
					is >> springConstant;
					if(!is.good() || springConstant < 0.0)
					{
						SetDataValid(false);
						return is;
					}
					polymerSpringConstants.push_back(springConstant);
				}
			}

			is >> token;
			if(!is.good() || token != "UnstretchedLength")
			{
				SetDataValid(false);
				return is;
			}
			else
			{
				for(long unsigned int i=0; i<polymerNames.size(); i++)
				{			
					is >> unstretchedLength;
					if(!is.good() || unstretchedLength < 0.0)
					{
						SetDataValid(false);
						return is;
					}
					polymerLengths.push_back(unstretchedLength);
				}
			}

			// Store the data in CPolymerCrossLink objects and add them to
			// the vector of cross-links. Because we do not
			// yet know the types of the polymers in the bilayer we use the
			// constructor that gives a default value to the type.

			for(long unsigned int i=0; i<polymerNames.size(); i++)
			{			
				CPolymerCrossLink* pLink = new CPolymerCrossLink(polymerNames.at(i),
																 polymerFractions.at(i),
																 polymerPositions.at(i),
																 polymerSpringConstants.at(i),
																 polymerLengths.at(i));
				m_CrossLinks.push_back(pLink);
			}
		}
	}

	// Data has been read successfully so store it in the member variables

	m_X				= x;
	m_Y				= y;
	m_Z				= z;
	m_Centre[0]		= cx;
	m_Centre[1]		= cy;
	m_Centre[2]		= cz;
	m_Thickness		= thickness;
	m_Length		= length;
	m_Width			= width;
	m_bLinearise	= bLinear;
	m_bPatches[0]	= bPatches[0];
	m_bPatches[1]	= bPatches[1];

	return is;
}

// Function to use the CLamellaBuilder object to set the coordinates of all the 
// beads in the simulation into a lamella initial state. Note that the builder 
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStateFreeLamella::Assemble(CInitialState& riState) const
{
	CFreeLamellaBuilder lamella(m_PolymerTypes, m_UpperFractions, m_CrossLinks,
									 m_bPatches, m_bPolymerise, m_bLinearise,
									 m_X, m_Y, m_Z, m_Centre, m_Thickness, 
									 m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
									 m_Bilayer1Centre, m_Bilayer2Centre, m_Bilayer3Centre, 
									 m_UpperHead, m_LowerHead);
	return lamella.Assemble(riState);
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateFreeLamella::Copy() const
{
	CInitialStateFreeLamella* pISL = new CInitialStateFreeLamella;	// new data object

	*pISL = *this;		// assign new data values to new object

	return pISL;
}

// Virtual function to check that the data supplied by the user to assemble a
// lamella initial state is consistent. We check that the centre of 
// the bilayer and its thickness, length and width cause it to be created 
// within the SimBox and not overlapping one of the PBC boundaries, or the 
// wall if it is present; we also calculate the area of each monolayer for later use. 
//
// When the bilayer is polymerised we have to check that the data specified
// for the polymers, cross-link position, etc is all valid. In particular, 
// the names of the polymers must be valid.
//
// Note that unlike the initial states containing a bilayer that spans the simulation
// box, here we allow a wall around the SimBox in any dimension.
//
// This function cannot be const because we store data for later use.

bool CInitialStateFreeLamella::ValidateData(const CInputData& riData)
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

	// If the initial state is polymerised, check that the polymer names 
	// match those in the lamella specification above and store their types. We add the
	// type to the CPolymerCrossLink objects.  We don't insist that the order of the
	// polymers be the same as that specified above. In fact, not all the polymers
	// in the lamella need to be polymerised. But obviously the data for each polymer
	// that is polymerised must be in the order of their names. If any polymer name
	// is not found in the CInputData's map we return false, and if any polymer name
	// is not found in the first polymer names list we also return false.

	// Check also that the polymerisation positions are within the polymers' lengths. 
	// A negative value is actually forbidden by the get() function above, but we 
	// repeat it here for completeness. The bond force constants have already been 
	// checked and do not depend on the polymer properties.

	if(m_bPolymerise)
	{
		for(PolymerCrossLinkIterator iterLink=m_CrossLinks.begin(); iterLink!=m_CrossLinks.end(); iterLink++)
		{
			const zString name	= (*iterLink)->GetName();

			if(riData.GetPolymerNamesMap().find(name) != riData.GetPolymerNamesMap().end())
			{
				(*iterLink)->SetType((*riData.GetPolymerNamesMap().find(name)).second);
			}
			else
				return false;

			bool nameFound = false;

			for(cStringSequenceIterator polymer=m_Polymers.begin(); polymer!=m_Polymers.end(); polymer++)
			{
				if((*polymer) == name)
					nameFound = true;
			}

			long position	= (*iterLink)->GetPosition();
			long type		= (*iterLink)->GetType();

			if(!nameFound || position < 0 || position >= (riData.GetPolymerTypes().at(type))->GetSize())
				return false;
		}
	}

	// Calculate the centre, width and area of the lamella.
	//
	// If there is no wall the functions just return zero for its thickness.
	// Note that the lamella is assembled using the normal to define the "top"
	// and "bottom" of the SimBox.
	//
	// lower	= thickness of wall on bottom of SimBox.
	// upper	= distance from bottom of SimBox to start of wall on top face
	// centre	= fraction of SimBox width at centre of bilayer
	//
	// Note that from this point on, m_Bilayer3Centre contains the coordinates,
	// in bead diameters, of the centre of the bilayer in its normal direction.
	// This may be the x, y, or z axis of the SimBox. This allows the
	// CBuilder object to use m_Bilayer3Centre without having to know the actual
	// normal to the bilayer.

	double l1, l2, lower, upper, centre;

	if(m_X == 1)
	{
		l1		= m_Length;
		l2		= m_Width;
		lower	= riData.GetCNTXCellWidth()*static_cast<double>(riData.GetWallXWidth());
		upper	= riData.GetSimBoxXLength()-lower;
		centre	= m_Centre[0]*riData.GetSimBoxXLength();
		m_Bilayer3Centre	= m_Centre[0]*riData.GetSimBoxXLength();
		m_Bilayer1Centre	= m_Centre[1]*riData.GetSimBoxYLength();
		m_Bilayer2Centre	= m_Centre[2]*riData.GetSimBoxZLength();
	}
	else if(m_Y == 1)
	{
		l1		= m_Length;
		l2		= m_Width;
		lower	= riData.GetCNTYCellWidth()*static_cast<double>(riData.GetWallYWidth());
		upper	= riData.GetSimBoxYLength()-lower;
		centre	= m_Centre[1]*riData.GetSimBoxYLength();
		m_Bilayer2Centre	= m_Centre[0]*riData.GetSimBoxXLength();
		m_Bilayer3Centre	= m_Centre[1]*riData.GetSimBoxYLength();
		m_Bilayer1Centre	= m_Centre[2]*riData.GetSimBoxZLength();
	}
	else if(m_Z == 1)
	{
		l1		= m_Length;
		l2		= m_Width;
		lower	= riData.GetCNTZCellWidth()*static_cast<double>(riData.GetWallZWidth());
		upper	= riData.GetSimBoxZLength()-lower;
		centre	= m_Centre[2]*riData.GetSimBoxZLength();
		m_Bilayer1Centre	= m_Centre[0]*riData.GetSimBoxXLength();
		m_Bilayer2Centre	= m_Centre[1]*riData.GetSimBoxYLength();
		m_Bilayer3Centre	= m_Centre[2]*riData.GetSimBoxZLength();
	}
	else
		return false;

	// We allow the wall to be present even if there is a bilayer so that we
	// can simulate the effects of closing the SimBox on a bilayer.

	if((centre + 0.5*m_Thickness) > upper || (centre - 0.5*m_Thickness) < lower)
		return false;
//	else if(m_X == 1 && (riData.GetWallYWidth() > 0 || riData.GetWallZWidth() > 0))
//		return false;
//	else if(m_Y == 1 && (riData.GetWallXWidth() > 0 || riData.GetWallZWidth() > 0))
//		return false;
//	else if(m_Z == 1 && (riData.GetWallXWidth() > 0 || riData.GetWallYWidth() > 0))
//		return false;

	// Input data has been checked so allow the initialisation to proceed.
	// Copy the coordinates of the bilayer midplane and its extremities into
	// local data for passing to the CLamellaBuilder. Note that we have to keep
	// track of the bilayer orientation to use this data correctly, but we
	// can be sure that it is valid and does not overlap the walls or boundaries.

	m_BilayerArea		= l1*l2;
	m_Bilayer1Length	= l1;
	m_Bilayer2Length	= l2;
	m_LowerHead			= centre - 0.5*m_Thickness;
	m_UpperHead			= centre + 0.5*m_Thickness;

	return true;
}
