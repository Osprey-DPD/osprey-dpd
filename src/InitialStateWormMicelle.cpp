/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateWormMicelle.cpp: implementation of the CInitialStateWormMicelle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "InitialStateWormMicelle.h"
#include "WormBuilder.h"
#include "InputData.h"
#include "InitialState.h"
#include "Polymer.h"
#include "PolymerCrossLink.h"

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

const zString CInitialStateWormMicelle::m_Type = "worm";

const zString CInitialStateWormMicelle::GetType()
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
	CInitialStateData* Create() {return new CInitialStateWormMicelle();}

	const zString id = CInitialStateWormMicelle::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitialStateWormMicelle::CInitialStateWormMicelle() : m_XN(0), m_YN(0), m_ZN(0),
											   m_XC(0.0), m_YC(0.0), m_ZC(0.0),  
											   m_Radius(0.0), 
											   m_HalfLength(0.0), 
											   m_bPatches(false),
											   m_bPolymerise(false)
											   
{

	m_Polymers.clear();
	m_PolymerTypes.clear();
	m_CrossLinks.clear();

}

CInitialStateWormMicelle::~CInitialStateWormMicelle()
{

}

CInitialStateWormMicelle& CInitialStateWormMicelle::operator=(const CInitialStateWormMicelle& oldISV)
{
	if(this != &oldISV)
	{
		m_Polymers				= oldISV.m_Polymers;
		m_XN					= oldISV.m_XN;
		m_YN					= oldISV.m_YN;
		m_ZN					= oldISV.m_ZN;
		m_XC					= oldISV.m_XC;
		m_YC					= oldISV.m_YC;
		m_ZC					= oldISV.m_ZC;
		m_Radius				= oldISV.m_Radius;
		m_HalfLength			= oldISV.m_HalfLength;
		m_bPatches				= oldISV.m_bPatches;
		m_bPolymerise			= oldISV.m_bPolymerise;
		m_CrossLinks			= oldISV.m_CrossLinks;
		m_PolymerTypes			= oldISV.m_PolymerTypes;
	}

	return *this;
}

zString CInitialStateWormMicelle::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateWormMicelle::put(zOutStream& os) const
{
	os << "State	" << "worm"	<< zEndl;

	os << "			" << "Polymers		  ";

	for(cStringSequenceIterator iterName1=m_Polymers.begin(); iterName1!=m_Polymers.end(); iterName1++)
	{
		os << " " << (*iterName1);
	}

	os << zEndl;

	os << "			" << "Normal		"	<< m_XN << "  " << m_YN << "  " << m_ZN << zEndl;
	os << "			" << "Centre		"	<< m_XC << "  " << m_YC << "  " << m_ZC << zEndl;
	os << "			" << "Radius	    "	<< m_Radius		<< zEndl;
	os << "			" << "HalfLength    "	<< m_HalfLength	<< zEndl;
	os << "			" << "Patches 		"	<< m_bPatches   << zEndl;

	return os;
}

zInStream& CInitialStateWormMicelle::get(zInStream& is)
{
	zString token  = "";
	zString sName  = "";

	long xn, yn, zn;
	double xc, yc, zc;
	double radius, halfLength;
	bool   bPatches;

	// Data relating to polymerisation of the vesicle surface 

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
		is >> xn >> yn >> zn;
		
		// The normal must be along a major axis

		if(!is.good() || !( (xn == 1 && yn == 0 && zn == 0) ||
			                (xn == 0 && yn == 1 && zn == 0) ||
					        (xn == 0 && yn == 0 && zn == 1) ) )
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

		// Worm radius must be greater than a bead diameter. We cannot check 
		// here that it is less than the SimBox size but that will be done in CInputData.

		if(!is.good() || radius < 1.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "HalfLength")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> halfLength;

		// Worm length must be positive and less than half the box length.

		if(!is.good() || halfLength <= 0.0 || halfLength > 0.5)
		{
			SetDataValid(false);
			return is;
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
		is >> bPatches;

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

			short i;	// Counter used below

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
				for(i=0; i<polymerNames.size(); i++)
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
				for(i=0; i<polymerNames.size(); i++)
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
				for(i=0; i<polymerNames.size(); i++)
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
				for(i=0; i<polymerNames.size(); i++)
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

			for(i=0; i<polymerNames.size(); i++)
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

	m_XN			= xn;
	m_YN			= yn;
	m_ZN			= zn;
	m_XC			= xc;
	m_YC			= yc;
	m_ZC			= zc;
	m_Radius		= radius;
	m_HalfLength	= halfLength;
	m_bPatches		= bPatches;

	return is;
}

// Function to use the CWormBuilderBuilder object to set the coordinates of all the 
// beads in the simulation into a worm micelle initial state. Note that the builder 
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStateWormMicelle::Assemble(CInitialState& riState) const
{
	CWormBuilder worm(m_PolymerTypes, m_CrossLinks, m_XN,  m_YN,  m_ZN,  
							m_XC,  m_YC,  m_ZC, m_Radius, m_HalfLength, 
							m_bPatches, m_bPolymerise);

	return worm.Assemble(riState);
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateWormMicelle::Copy() const
{
	CInitialStateWormMicelle* pISV = new CInitialStateWormMicelle;	// new data object

	*pISV = *this;		// assign new data values to new object

	return pISV;
}

// Virtual function to check that the data supplied by the user to assemble a
// worm micelle initial state is consistent. 
//
// This function cannot be const because we store data for later use.

bool CInitialStateWormMicelle::ValidateData(const CInputData& riData)
{
	// Check that the polymer names specified in the worm actually occur 
	// in the CInitialState map and store their types for later use. 
	// A polymer name must not occur more than once in the m_PolymerTypes container. 

	for(cStringSequenceIterator polymer1=m_Polymers.begin(); polymer1!=m_Polymers.end(); polymer1++)
	{
		const long polymerType = (*riData.GetPolymerNamesMap().find(*polymer1)).second;

		if(riData.GetPolymerNamesMap().find(*polymer1) != riData.GetPolymerNamesMap().end() &&
		   find(m_PolymerTypes.begin(), m_PolymerTypes.end(), polymerType) == m_PolymerTypes.end())
		{
			m_PolymerTypes.push_back(polymerType);
		}
		else
			return false;
	}

	// Calculate the centre and radius of the worm.
	//
	// If there is no wall the functions just return zero for its thickness.
	//
	// For each of the SimBox axes, we check that the worm does not overlap
	// the wall, if it exists, in that dimension:
	//
	// lower	= thickness of wall on bottom of SimBox.
	// upper	= distance from bottom of SimBox to start of wall on top face
	// centre	= fraction of SimBox width at centre of worm

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

	// Calculate the length of the worm and check it does not exceed the SimBox length
	// in the direction of its axis taking a wall into account If the normal is 
	// invalid we exit with an error.

	double actualHalfLength, leftEnd, rightEnd;

	if(m_XN == 1)
	{
	    if((centre[1] + m_Radius) > upper[1] || (centre[1] - m_Radius) < lower[1])
		    return false;
	    else if((centre[2] + m_Radius) > upper[2] || (centre[2] - m_Radius) < lower[2])
		    return false;

		actualHalfLength = m_HalfLength*riData.GetSimBoxXLength();
		leftEnd  = centre[0] - actualHalfLength;
		rightEnd = centre[0] + actualHalfLength;

		if(leftEnd < lower[0] || rightEnd > upper[0])
			return false;
	}
	else if(m_YN == 1)
	{
	    if((centre[0] + m_Radius) > upper[0] || (centre[0] - m_Radius) < lower[0])
		    return false;
	    else if((centre[2] + m_Radius) > upper[2] || (centre[2] - m_Radius) < lower[2])
		    return false;

		actualHalfLength = m_HalfLength*riData.GetSimBoxYLength();
		leftEnd  = centre[1] - actualHalfLength;
		rightEnd = centre[1] + actualHalfLength;

		if(leftEnd < lower[1] || rightEnd > upper[1])
			return false;
	}
	else if(m_ZN == 1)
	{
	    if((centre[0] + m_Radius) > upper[0] || (centre[0] - m_Radius) < lower[0])
		    return false;
	    else if((centre[1] + m_Radius) > upper[1] || (centre[1] - m_Radius) < lower[1])
		    return false;

		actualHalfLength = m_HalfLength*riData.GetSimBoxZLength();
		leftEnd  = centre[2] - actualHalfLength;
		rightEnd = centre[2] + actualHalfLength;

		if(leftEnd < lower[2] || rightEnd > upper[2])
			return false;
	}
	else
	{
		return false;
	}

	// If the initial state is polymerised, check that the polymer names 
	// match those in the vesicle specification above and store their types. 
	// We add the type to the CPolymerCrossLink objects.  We don't insist that 
	// the order of the polymers be the same as that specified above. In fact, 
	// not all the polymers in the vesicle need to be polymerised. But obviously 
	// the data for each polymer that is polymerised must be in the order of their 
	// names. If a polymer name is not found in the CInputData's map or it is not
	// in the first polymer names list we also return false, and if a polymer name
	// occurs more than once we return false.

	// Check also that the polymerisation positions are within the polymers' lengths. 
	// A negative value is actually forbidden by the get() function above, but we 
	// repeat it here for completeness. The bond force constants have already been 
	// checked and do not depend on the polymer properties.

	zLongVector crosslinkPolymerTypes;

	if(m_bPolymerise)
	{
		for(PolymerCrossLinkIterator iterLink=m_CrossLinks.begin(); iterLink!=m_CrossLinks.end(); iterLink++)
		{
			const zString name	= (*iterLink)->GetName();

			if(find(m_Polymers.begin(), m_Polymers.end(), name) != m_Polymers.end() &&
			   find(crosslinkPolymerTypes.begin(), crosslinkPolymerTypes.end(), 
			   (*riData.GetPolymerNamesMap().find(name)).second) == crosslinkPolymerTypes.end())
			{
				crosslinkPolymerTypes.push_back((*riData.GetPolymerNamesMap().find(name)).second);
				(*iterLink)->SetType((*riData.GetPolymerNamesMap().find(name)).second);
			}
			else
				return false;

			long position	= (*iterLink)->GetPosition();
			long type		= (*iterLink)->GetType();

			if(position < 0 || position >= (riData.GetPolymerTypes().at(type))->GetSize())
				return false;
		}
	}

	// Input data has been checked so allow the initialisation to proceed.

	return true;
}

