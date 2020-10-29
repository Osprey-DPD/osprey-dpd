/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateVesicle.cpp: implementation of the CInitialStateVesicle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "InitialStateVesicle.h"
#include "mpmInitialState.h"
#include "pmISVesicle.h"
#include "VesicleBuilder.h"
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

const zString CInitialStateVesicle::m_Type = "vesicle";

const zString CInitialStateVesicle::GetType()
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
	CInitialStateData* Create() {return new CInitialStateVesicle();}

	const zString id = CInitialStateVesicle::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitialStateVesicle::CInitialStateVesicle() : m_XC(0.0), m_YC(0.0), m_ZC(0.0), 
											   m_OuterRadius(0.0), 
											   m_Thickness(0.0), 
											   m_InnerRadius(0.0),
											   m_bPolymerise(false)
											   
{
	m_bPatches[0] = false;
	m_bPatches[1] = false;

	m_Polymers.clear();
	m_PolymerTypes.clear();
	m_InteriorPolymers.clear();
	m_InteriorPolymerTypes.clear();
	m_OuterFractions.clear();
	m_CrossLinks.clear();

}

CInitialStateVesicle::~CInitialStateVesicle()
{

}

CInitialStateVesicle& CInitialStateVesicle::operator=(const CInitialStateVesicle& oldISV)
{
	if(this != &oldISV)
	{
		m_Polymers				= oldISV.m_Polymers;
		m_InteriorPolymers		= oldISV.m_InteriorPolymers;
		m_XC					= oldISV.m_XC;
		m_YC					= oldISV.m_YC;
		m_ZC					= oldISV.m_ZC;
		m_OuterRadius			= oldISV.m_OuterRadius;
		m_Thickness				= oldISV.m_Thickness;
		m_OuterFractions		= oldISV.m_OuterFractions;
		m_bPatches[0]			= oldISV.m_bPatches[0];
		m_bPatches[1]			= oldISV.m_bPatches[1];
		m_bPolymerise			= oldISV.m_bPolymerise;
		m_CrossLinks			= oldISV.m_CrossLinks;
		m_InnerRadius			= oldISV.m_InnerRadius;
		m_PolymerTypes			= oldISV.m_PolymerTypes;
		m_InteriorPolymerTypes	= oldISV.m_InteriorPolymerTypes;
	}

	return *this;
}

zString CInitialStateVesicle::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateVesicle::put(zOutStream& os) const
{
	os << "State	" << "vesicle"			<< zEndl;

	os << "			" << "Polymers		  ";

	for(cStringSequenceIterator iterName1=m_Polymers.begin(); iterName1!=m_Polymers.end(); iterName1++)
	{
		os << " " << (*iterName1);
	}

	os << zEndl;

	os << "			" << "Interior		  ";

	for(cStringSequenceIterator iterName2=m_InteriorPolymers.begin(); iterName2!=m_InteriorPolymers.end(); iterName2++)
	{
		os << " " << (*iterName2);
	}

	os << zEndl;

	os << "			" << "Centre		"	<< m_XC << "  " << m_YC << "  " << m_ZC << zEndl;
	os << "			" << "OuterRadius	"	<< m_OuterRadius	<< zEndl;
	os << "			" << "Thickness		"	<< m_Thickness		<< zEndl;
	os << "	        " << "OuterFraction	  ";

	for(short i=0; i<m_Polymers.size(); i++)
	{
		os << "	" << m_OuterFractions.at(i);
	}
	os << zEndl;

	os << "			" << "Patches 		"	<< m_bPatches[0] << "  " << m_bPatches[1] << zEndl;


	
	
	return os;
}

zInStream& CInitialStateVesicle::get(zInStream& is)
{
	zString token  = "";
	zString sName  = "";

	double xc, yc, zc;
	double outerRadius, thickness, outerFraction;
	bool   bPatches[2];

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

	// Read names of polymers inside vesicle

	if(!is.good() || sName != "Interior")
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
					m_InteriorPolymers.push_back(sName);
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

	// Read the fractions of the polymers that are in the outer monolayer.
	// Obviously, the remainder of the polymers are in the inner monolayer.
	// We know how many elements to read in from the size of the m_Polymer vector.

	is >> token;
	if(!is.good() || token != "OuterFraction")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		for(short i=0; i<m_Polymers.size(); i++)
		{
			is >> outerFraction;

			// Fraction must lie between 0 and 1

			if(!is.good() || outerFraction < 0.0 || outerFraction > 1.0)
			{
				SetDataValid(false);
				return is;
			}
			else
				m_OuterFractions.push_back(outerFraction);
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

	m_XC			= xc;
	m_YC			= yc;
	m_ZC			= zc;
	m_OuterRadius	= outerRadius;
	m_Thickness		= thickness;
	m_bPatches[0]	= bPatches[0];
	m_bPatches[1]	= bPatches[1];

	return is;
}

// Function to use the CLamellaBuilder object to set the coordinates of all the 
// beads in the simulation into a lamella initial state. Note that the builder 
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStateVesicle::Assemble(CInitialState& riState) const
{
	CVesicleBuilder vesicle(m_PolymerTypes, m_InteriorPolymerTypes, m_OuterFractions, 
							m_CrossLinks, m_XC,  m_YC,  m_ZC,  
							m_OuterRadius, m_InnerRadius,  m_Thickness, 
							m_bPatches, m_bPolymerise);

	return vesicle.Assemble(riState);
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateVesicle::Copy() const
{
	CInitialStateVesicle* pISV = new CInitialStateVesicle;	// new data object

	*pISV = *this;		// assign new data values to new object

	return pISV;
}

// Virtual function to assemble the IS data from the corresponding 
// mpmInitialState message instance. Note that we don't allow the
// parallel vesicle IS to be polymerised so we set the flat to false here.
//
// Note that we only allow a single polymer type in the vesicle membrane and another single polymer type in its interior for now.

void CInitialStateVesicle::SetData(const mpmInitialState *const pMsg)
{
#if EnableParallelSimBox == SimMPSEnabled

    const pmISVesicle* const pVesicleMsg = dynamic_cast<const pmISVesicle* const>(pMsg);
	
	if(pVesicleMsg)
	{
	    m_Polymers.push_back(pVesicleMsg->GetPolymer());
	    m_InteriorPolymers.push_back(pVesicleMsg->GetInteriorPolymer());
	    m_OuterFractions.push_back(pVesicleMsg->GetOuterFraction());
		
	    m_XC			= pVesicleMsg->GetXCentre();
	    m_YC			= pVesicleMsg->GetYCentre();
	    m_ZC			= pVesicleMsg->GetZCentre();
	    m_OuterRadius	= pVesicleMsg->GetOuterRadius();
	    m_Thickness		= pVesicleMsg->GetThickness();
		m_bPatches[0]   = GetOuterPatches();
		m_bPatches[1]   = GetInnerPatches();
	    m_bPolymerise	= false;
		}
		else
		{
		    // Wrong IS message, so we leave all parameters invalid.
		}
#endif
}


// Virtual function to check that the data supplied by the user to assemble a
// vesicle initial state is consistent. 
//
// This function cannot be const because we store data for later use.

bool CInitialStateVesicle::ValidateData(const CInputData& riData)
{
	// Check that the polymer names specified in the vesicle and the vesicle's
	// interior space actually occur in the CInitialState map and store their 
	// types for later use. A polymer name must not occur in both the m_Polymers 
	// and m_InteriorPolymers containers, nor can its type occur more than once in
	// the m_PolymerTypes container. Similarly, polymers in the interior of the
	// vesicle must not be specified more than once.

	for(cStringSequenceIterator polymer1=m_Polymers.begin(); polymer1!=m_Polymers.end(); polymer1++)
	{
		if(riData.GetPolymerNamesMap().find(*polymer1) != riData.GetPolymerNamesMap().end() &&
		   find(m_PolymerTypes.begin(), m_PolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*polymer1)).second) == m_PolymerTypes.end())
		{
			m_PolymerTypes.push_back((*riData.GetPolymerNamesMap().find(*polymer1)).second);
		}
		else
			return false;
	}

	for(cStringSequenceIterator polymer2=m_InteriorPolymers.begin(); polymer2!=m_InteriorPolymers.end(); polymer2++)
	{
		if(riData.GetPolymerNamesMap().find(*polymer2) != riData.GetPolymerNamesMap().end() &&
		   find(m_PolymerTypes.begin(), m_PolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*polymer2)).second) == m_PolymerTypes.end() &&
		   find(m_InteriorPolymerTypes.begin(), m_InteriorPolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*polymer2)).second) == m_InteriorPolymerTypes.end())
		{
			m_InteriorPolymerTypes.push_back((*riData.GetPolymerNamesMap().find(*polymer2)).second);
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

