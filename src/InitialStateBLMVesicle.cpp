/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateBLMVesicle.cpp: implementation of the CInitialStateBLMVesicle class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "InitialStateBLMVesicle.h"
#include "BLMVesicleBuilder.h"
#include "InputData.h"
#include "InitialState.h"
#include "Polymer.h"
#include "PolymerCrossLink.h"


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

const zString CInitialStateBLMVesicle::m_Type = "blmvesicle";

const zString CInitialStateBLMVesicle::GetType()
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
	CInitialStateData* Create() {return new CInitialStateBLMVesicle();}

	const zString id = CInitialStateBLMVesicle::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Note that there is no way to set default data for a lamella as we 
// don't have access to the polymers that are defined in the control data file.

CInitialStateBLMVesicle::CInitialStateBLMVesicle() : m_BLMX(0), m_BLMY(0), m_BLMZ(0), 
													m_BLMCentre(0.0),
													m_BLMThickness(0.0),
													m_bBLMLinearise(false),
													m_bBLMPolymerise(false),
													m_VXC(0.0), m_VYC(0.0), m_VZC(0.0), 
													m_VOuterRadius(0.0), 
													m_VThickness(0.0), 
													m_bVPolymerise(false),
													m_BilayerArea(0.0),
													m_Bilayer1Length(0.0),
													m_Bilayer2Length(0.0),
													m_BilayerCentre(0.0),
													m_UpperHead(0.0),
													m_LowerHead(0.0),
													m_VInnerRadius(0.0),
													m_VXCentre(0.0), 
													m_VYCentre(0.0), 
													m_VZCentre(0.0)
{
	// BLM data

	m_BLMPolymers.clear();
	m_BLMReservoir.clear();
	m_BLMUpperFractions.clear();
	m_bBLMPatches[0] = false;
	m_bBLMPatches[1] = false;
	m_BLMCrossLinks.clear();

	// Vesicle data

	m_VPolymers.clear();
	m_VInteriorPolymers.clear();
	m_VExteriorPolymers.clear();
	m_VOuterFractions.clear();
	m_bVPatches[0] = false;
	m_bVPatches[1] = false;
	m_VCrossLinks.clear();

	// local data for both aggregates

	m_BLMPolymerTypes.clear();
	m_BLMReservoirTypes.clear();
	m_VPolymerTypes.clear();
	m_VInteriorTypes.clear();
	m_VExteriorTypes.clear();

}

CInitialStateBLMVesicle::~CInitialStateBLMVesicle()
{
}


CInitialStateBLMVesicle& CInitialStateBLMVesicle::operator=(const CInitialStateBLMVesicle& oldISBLMV)
{
	if(this != &oldISBLMV)
	{
		// BLM data first

		m_BLMPolymers					= oldISBLMV.m_BLMPolymers;
		m_BLMReservoir					= oldISBLMV.m_BLMReservoir;
		m_BLMX							= oldISBLMV.m_BLMX;
		m_BLMY							= oldISBLMV.m_BLMY;
		m_BLMZ							= oldISBLMV.m_BLMZ;
		m_BLMCentre						= oldISBLMV.m_BLMCentre;
		m_BLMThickness					= oldISBLMV.m_BLMThickness;
		m_bBLMLinearise					= oldISBLMV.m_bBLMLinearise;
		m_BLMUpperFractions				= oldISBLMV.m_BLMUpperFractions;
		m_bBLMPatches[0]				= oldISBLMV.m_bBLMPatches[0];
		m_bBLMPatches[1]				= oldISBLMV.m_bBLMPatches[1];
		m_bBLMPolymerise				= oldISBLMV.m_bBLMPolymerise;
		m_BLMCrossLinks					= oldISBLMV.m_BLMCrossLinks;

		// now the vesicle data

		m_VPolymers						= oldISBLMV.m_VPolymers;
		m_VInteriorPolymers				= oldISBLMV.m_VInteriorPolymers;
		m_VExteriorPolymers				= oldISBLMV.m_VExteriorPolymers;
		m_VXC							= oldISBLMV.m_VXC;
		m_VYC							= oldISBLMV.m_VYC;
		m_VZC							= oldISBLMV.m_VZC;
		m_VOuterRadius					= oldISBLMV.m_VOuterRadius;
		m_VThickness					= oldISBLMV.m_VThickness;
		m_VOuterFractions				= oldISBLMV.m_VOuterFractions;
		m_bVPatches[0]					= oldISBLMV.m_bVPatches[0];
		m_bVPatches[1]					= oldISBLMV.m_bVPatches[1];
		m_bVPolymerise					= oldISBLMV.m_bVPolymerise;
		m_VCrossLinks					= oldISBLMV.m_VCrossLinks;

		// now the local data for both aggregates

		m_BLMPolymerTypes				= oldISBLMV.m_BLMPolymerTypes;
		m_BLMReservoirTypes				= oldISBLMV.m_BLMReservoirTypes;
	    m_BilayerArea					= oldISBLMV.m_BilayerArea;
	    m_Bilayer1Length				= oldISBLMV.m_Bilayer1Length;
	    m_Bilayer2Length				= oldISBLMV.m_Bilayer2Length;
	    m_BilayerCentre					= oldISBLMV.m_BilayerCentre;
	    m_UpperHead						= oldISBLMV.m_UpperHead;
	    m_LowerHead						= oldISBLMV.m_LowerHead;

		m_VPolymerTypes					= oldISBLMV.m_VPolymerTypes;
		m_VInteriorTypes				= oldISBLMV.m_VInteriorTypes;
		m_VExteriorTypes				= oldISBLMV.m_VExteriorTypes;
		m_VInnerRadius					= oldISBLMV.m_VInnerRadius;
		m_VXCentre						= oldISBLMV.m_VXCentre;
		m_VYCentre						= oldISBLMV.m_VYCentre;
		m_VZCentre						= oldISBLMV.m_VZCentre;
	}

	return *this;
}

zString CInitialStateBLMVesicle::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateBLMVesicle::put(zOutStream& os) const
{
	os << "State	" << "blmvesicle"	<< zEndl;

	// BLM data first

	os << "			" << "BLMPolymers	";

	for(cStringSequenceIterator iterName=m_BLMPolymers.begin(); iterName!=m_BLMPolymers.end(); iterName++)
	{
		os << " " << (*iterName);
	}

	os << zEndl;

	os << "			" << "Reservoir		";

	for(cStringSequenceIterator iterName1=m_BLMReservoir.begin(); iterName1!=m_BLMReservoir.end(); iterName1++)
	{
		os << " " << (*iterName1);
	}

	os << zEndl;

	os << "			" << "Normal		"	<< m_BLMX << "  "		<< m_BLMY << "  " << m_BLMZ << zEndl;
	os << "			" << "Centre		"	<< m_BLMCentre			<< zEndl;
	os << "			" << "Thickness		"	<< m_BLMThickness		<< zEndl;
	os << "			" << "Linearise 	"	<< m_bBLMLinearise		<< zEndl;

	os << "	        " << "UpperFraction	  ";

	for(long unsigned int i=0; i<m_BLMPolymers.size(); i++)
	{
		os << "	" << m_BLMUpperFractions.at(i);
	}

	os << zEndl;

	os << "			" << "Patches 		"	<< m_bBLMPatches[0] << "  " << m_bBLMPatches[1] << zEndl;

	// Polymerise keyword is missing here


	// Vesicle data

	os << "			" << "VPolymers		";

	for(cStringSequenceIterator iterName2=m_VPolymers.begin(); iterName2!=m_VPolymers.end(); iterName2++)
	{
		os << " " << (*iterName2);
	}

	os << zEndl;

	os << "			" << "Interior		";

	for(cStringSequenceIterator iterName3=m_VInteriorPolymers.begin(); iterName3!=m_VInteriorPolymers.end(); iterName3++)
	{
		os << " " << (*iterName3);
	}

	os << zEndl;

	os << "			" << "Exterior		";

	for(cStringSequenceIterator iterName4=m_VExteriorPolymers.begin(); iterName4!=m_VExteriorPolymers.end(); iterName4++)
	{
		os << " " << (*iterName4);
	}

	os << zEndl;

	os << "			" << "Centre		"	<< m_VXC << "  " << m_VYC << "  " << m_VZC << zEndl;
	os << "			" << "OuterRadius	"	<< m_VOuterRadius	<< zEndl;
	os << "			" << "Thickness		"	<< m_VThickness		<< zEndl;
	os << "	        " << "OuterFraction	  ";

	for(long unsigned int i=0; i<m_VPolymers.size(); i++)
	{
		os << "	" << m_VOuterFractions.at(i);
	}
	os << zEndl;

	os << "			" << "Patches 		"	<< m_bVPatches[0] << "  " << m_bVPatches[1] << zEndl;


	// Polymerise keyword is missing here


	return os;
}

zInStream& CInitialStateBLMVesicle::get(zInStream& is)
{
	// Variables used several times below

	zString token					= "";
	zString sName					= "";

	// Read all the data relating to the bilayer first

	double  centre					= 0.0;
	double  thickness				= 0.0;
	long	blmx					= 0;
	long	blmy					= 0;
	long	blmz					= 0;
	bool	bLinear					= true;
	double  upperFraction			= 0.0;
	bool    bBLMPatches[2];

	is >> token;
	if(!is.good() || token != "BLMPolymers")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;

		if(!is.good() || sName == "Reservoir" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "Reservoir")
			{
				if(is.good() && !sName.empty())
					m_BLMPolymers.push_back(sName);
				else
				{
					SetDataValid(false);
					return is;
				}

				is >> sName;
			}
		}
	}

	if(!is.good() || sName != "Reservoir")
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
					m_BLMReservoir.push_back(sName);
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
		is >> blmx >> blmy >> blmz;
		
		// Force the normal to be parallel to an axis

		if(!is.good() || ((blmx+blmy) != 0 && blmz==1) || 
						 ((blmx+blmz) != 0 && blmy==1) ||
						 ((blmy+blmz) != 0 && blmx==1) )
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
		// here that it is less than the SimBox size but that will be done in CInputData.

		if(!is.good() || thickness < 1.0)
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
		for(long unsigned int i=0; i<m_BLMPolymers.size(); i++)
		{
			is >> upperFraction;

			// Fraction must lie between 0 and 1

			if(!is.good() || upperFraction < 0.0 || upperFraction > 1.0)
			{
				SetDataValid(false);
				return is;
			}
			else
				m_BLMUpperFractions.push_back(upperFraction);
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
		is >> bBLMPatches[0] >> bBLMPatches[1];

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
		is >> m_bBLMPolymerise;

		if(!is.good())
		{
			SetDataValid(false);
			return is;
		}
		else if(m_bBLMPolymerise)
		{

			// Read the cross-link data for each polymer in the bilayer.
			// We know how many there can be from the m_BLMPolymers vector, 
			// but not all the polymers specified there need be polymerised.
			// We count how many names occur in the Polymer keyword data and
			// use that to determine the loops over the remaining data. We
			// store the polymerisation information in a set of vectors so that 
			// each piece can be read all on one line instead of one polymer 
			// after another.
			//
			// The information required per polymer is:
			//
			// Name				e.g., Lipid		Cholesterol
			// Fraction				  0.5		1.0
			// Position				  0			1
			// SpringConstant		  128.0		16.0
			// UnstretchedLength	  0.5		1.0

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
				m_BLMCrossLinks.push_back(pLink);
			}
		}
	}

	// Bilayer data has been read successfully so store it in the member variables
	// before reading the vesicle data

	m_BLMX				= blmx;
	m_BLMY				= blmy;
	m_BLMZ				= blmz;
	m_BLMCentre			= centre;
	m_BLMThickness		= thickness;
	m_bBLMLinearise		= bLinear;
	m_bBLMPatches[0]	= bBLMPatches[0];
	m_bBLMPatches[1]	= bBLMPatches[1];


	double xc				= 0.0;
	double yc				= 0.0;
	double zc				= 0.0;
	double outerRadius		= 0.0;
	double outerFraction	= 0.0;
	bool   bVPatches[2];

	is >> token;
	if(!is.good() || token != "VPolymers")
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
					m_VPolymers.push_back(sName);
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

		if(!is.good() || sName == "Exterior" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "Exterior")
			{
				if(is.good() && !sName.empty())
					m_VInteriorPolymers.push_back(sName);
				else
				{
					SetDataValid(false);
					return is;
				}

				is >> sName;
			}
		}
	}

	// Read names of polymers outside vesicle

	if(!is.good() || sName != "Exterior")
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
					m_VExteriorPolymers.push_back(sName);
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
		for(long unsigned int i=0; i<m_VPolymers.size(); i++)
		{
			is >> outerFraction;

			// Fraction must lie between 0 and 1

			if(!is.good() || outerFraction < 0.0 || outerFraction > 1.0)
			{
				SetDataValid(false);
				return is;
			}
			else
				m_VOuterFractions.push_back(outerFraction);
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
		is >> bVPatches[0] >> bVPatches[1];

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
		is >> m_bVPolymerise;

		if(!is.good())
		{
			SetDataValid(false);
			return is;
		}
		else if(m_bVPolymerise)
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
				m_VCrossLinks.push_back(pLink);
			}
		}
	}

	// Vesicle data has been read successfully so copy it into member variables

	m_VXC			= xc;
	m_VYC			= yc;
	m_VZC			= zc;
	m_VOuterRadius	= outerRadius;
	m_VThickness	= thickness;
	m_bVPatches[0]	= bVPatches[0];
	m_bVPatches[1]	= bVPatches[1];


	return is;
}

// Function to use the CLamellaBuilder object to set the coordinates of all the 
// beads in the simulation into a lamella initial state. Note that the builder 
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStateBLMVesicle::Assemble(CInitialState& riState) const
{
//	CCompositeLamellaBuilder lamella(m_PolymerTypes, m_UpperFractions, m_CrossLinks,
//									 m_X, m_Y, m_Z, m_Centre, m_Thickness, m_bPatches, 
//									 m_bPolymerise,
//									 m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
//									 m_BilayerCentre, m_UpperHead, m_LowerHead, m_bLinearise);

//	CVesicleBuilder vesicle(m_PolymerTypes, m_InteriorPolymerTypes, m_OuterFractions, 
//							m_CrossLinks, m_XC,  m_YC,  m_ZC,  
//							m_OuterRadius, m_InnerRadius,  m_Thickness, 
//							m_bPatches, m_bPolymerise);

	CBLMVesicleBuilder blm(m_BLMPolymerTypes,	m_BLMReservoirTypes, 
						   m_BLMUpperFractions, m_BLMCrossLinks,
					       m_BLMX, m_BLMY, m_BLMZ,
						   m_BLMCentre, m_BLMThickness,
						   m_bBLMLinearise, m_bBLMPatches, m_bBLMPolymerise,
						   m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length, 
						   m_BilayerCentre, m_UpperHead, m_LowerHead,
						   m_VPolymerTypes, m_VInteriorTypes, m_VExteriorTypes,
						   m_VOuterFractions, m_VCrossLinks,
						   m_VXC, m_VYC, m_VZC,
						   m_VXCentre, m_VYCentre, m_VZCentre,
						   m_VOuterRadius, m_VInnerRadius, m_VThickness,
						   m_bVPatches, m_bVPolymerise);

	return blm.Assemble(riState);
}


// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateBLMVesicle::Copy() const
{
	CInitialStateBLMVesicle* pISBLMV = new CInitialStateBLMVesicle;	// new data object

	*pISBLMV = *this;		// assign new data values to new object

	return pISBLMV;
}


// Virtual function to check that the data supplied by the user to assemble a
// BLMVesicle initial state is consistent. We check that the centre of 
// the bilayer and its thickness cause it to be created within the SimBox and
// not overlapping one of the PBC boundaries, or the wall if it is present;
// we also calculate the area of each monolayer for later use.
//
// When the bilayer is polymerised we have to check that the data specified
// for the polymers, cross-link position, etc is all valid. In particular, 
// the names of the polymers must be valid.
//
// Note there should not be a wall around the SimBox other than one with the
// same orientation as the bilayer. After checking the bilayer we verify that 
// the vesicle does not overlap the wall (if present) or SimBox boundary, but
// we allow it to overlap the bilayer as they be a useful initial configuration.
//
// The polymers composing the bilayer, the vesicle and the reservoir must be
// disjoint. But those in the interior of the vesicle, the exterior and the
// reservoir may have common elements. This allows the same solvent to occupy
// all the internal and external spaces if desired.
//
// This function cannot be const because we store data for later use.

bool CInitialStateBLMVesicle::ValidateData(const CInputData& riData)
{
	// Check that the polymer names specified in the bilayer occur exactly once
	// in the map, and store their types for later use.

	for(cStringSequenceIterator polymer=m_BLMPolymers.begin(); polymer!=m_BLMPolymers.end(); polymer++)
	{
		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end() &&
		   find(m_BLMPolymerTypes.begin(), m_BLMPolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*polymer)).second) == m_BLMPolymerTypes.end())

		{
			m_BLMPolymerTypes.push_back( (*riData.GetPolymerNamesMap().find(*polymer)).second);
		}
		else
			return false;
	}

	// Now check that the reservoir polymers occur in the map only once and do
	// not occur in the bilayer

	for(cStringSequenceIterator rPolymer=m_BLMReservoir.begin(); rPolymer!=m_BLMReservoir.end(); rPolymer++)
	{
		if(riData.GetPolymerNamesMap().find(*rPolymer) != riData.GetPolymerNamesMap().end() &&
		   find(m_BLMReservoirTypes.begin(), m_BLMReservoirTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*rPolymer)).second) == m_BLMReservoirTypes.end() &&
		   find(m_BLMPolymerTypes.begin(), m_BLMPolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*rPolymer)).second) == m_BLMPolymerTypes.end())
		{
			m_BLMReservoirTypes.push_back( (*riData.GetPolymerNamesMap().find(*rPolymer)).second);
		}
		else
			return false;
	}

	// If the bilayer is polymerised, check that the polymer names 
	// match those in the specification above and store their types. 
	// We add the type to the CPolymerCrossLink objects.  We don't insist that 
	// the order of the polymers be the same as that specified above. In fact, 
	// not all the polymers in the bilayer need to be polymerised. But obviously 
	// the data for each polymer that is polymerised must be in the order of their 
	// names. If any polymer name is not found in the CInputData's map we return 
	// false, and if any polymer name is not found in the first polymer names list 
	// we also return false.

	// Check also that the polymerisation positions are within the polymers' lengths. 
	// A negative value is actually forbidden by the get() function above, but we 
	// repeat it here for completeness. The bond force constants have already been 
	// checked and do not depend on the polymer properties.

	if(m_bBLMPolymerise)
	{
		for(PolymerCrossLinkIterator iterLink=m_BLMCrossLinks.begin(); iterLink!=m_BLMCrossLinks.end(); iterLink++)
		{
			const zString name	= (*iterLink)->GetName();

			if(riData.GetPolymerNamesMap().find(name) != riData.GetPolymerNamesMap().end())
			{
				(*iterLink)->SetType((*riData.GetPolymerNamesMap().find(name)).second);
			}
			else
				return false;

			bool nameFound = false;

			for(cStringSequenceIterator polymer=m_BLMPolymers.begin(); polymer!=m_BLMPolymers.end(); polymer++)
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

	// Calculate the centre, width and area of the bilayer.
	//
	// If there is no wall the functions just return zero for its thickness.
	// Note that the lamella is assembled using the normal to define the "top"
	// and "bottom" of the SimBox.
	//
	// lower	= thickness of wall on bottom of SimBox.
	// upper	= distance from bottom of SimBox to start of wall on top face
	// centre	= fraction of SimBox width at centre of bilayer

	double l1, l2, blmLower, blmUpper, blmCentre;

	if(m_BLMX == 1)
	{
		l1			= riData.GetSimBoxYLength();
		l2			= riData.GetSimBoxZLength();
		blmLower	= riData.GetCNTXCellWidth()*static_cast<double>(riData.GetWallXWidth());
		blmUpper	= riData.GetSimBoxXLength()-blmLower;
		blmCentre	= m_BLMCentre*riData.GetSimBoxXLength();
	}
	else if(m_BLMY == 1)
	{
		l1			= riData.GetSimBoxXLength();
		l2			= riData.GetSimBoxZLength();
		blmLower	= riData.GetCNTYCellWidth()*static_cast<double>(riData.GetWallYWidth());
		blmUpper	= riData.GetSimBoxYLength()-blmLower;
		blmCentre	= m_BLMCentre*riData.GetSimBoxYLength();
	}
	else if(m_BLMZ == 1)
	{
		l1			= riData.GetSimBoxXLength();
		l2			= riData.GetSimBoxYLength();
		blmLower	= riData.GetCNTZCellWidth()*static_cast<double>(riData.GetWallZWidth());
		blmUpper	= riData.GetSimBoxZLength()-blmLower;
		blmCentre	= m_BLMCentre*riData.GetSimBoxZLength();
	}
	else
		return false;

	// We allow the wall to be present even if there is a bilayer so that we
	// can simulate the effects of closing the SimBox on a bilayer.

	if((blmCentre + 0.5*m_BLMThickness) > blmUpper || (blmCentre - 0.5*m_BLMThickness) < blmLower)
		return false;
//	else if(m_X == 1 && (riData.GetWallYWidth() > 0 || riData.GetWallZWidth() > 0))
//		return false;
//	else if(m_Y == 1 && (riData.GetWallXWidth() > 0 || riData.GetWallZWidth() > 0))
//		return false;
//	else if(m_Z == 1 && (riData.GetWallXWidth() > 0 || riData.GetWallYWidth() > 0))
//		return false;

	// Bilayer data has been checked so allow the validation to proceed to the vesicle,
	// but first copy the coordinates of the bilayer midplane and its extremities into
	// local data for passing to the CBLMVesicleBuilder. Note that we have to keep
	// track of the bilayer orientation to use this data correctly, but we
	// can be sure that it is valid and does not overlap the walls or boundaries.

	m_BilayerArea		= l1*l2;
	m_Bilayer1Length	= l1;
	m_Bilayer2Length	= l2;
	m_BilayerCentre		= blmCentre;
	m_LowerHead			= blmCentre - 0.5*m_BLMThickness;
	m_UpperHead			= blmCentre + 0.5*m_BLMThickness;

	// Check that the polymer names specified in the vesicle and the vesicle's
	// interior and exterior spaces actually occur in the CInitialState map and 
	// store their types for later use. A polymer name must not occur in more than
	// one of the m_BLMPolymers, m_BLMReservoir, m_VPolymers, m_VInteriorPolymers 
	// or m_VExteriorPolymers containers, nor can its type occur more than once 
	// in the m_PolymerTypes container. Similarly, polymers in the interior of 
	// the vesicle must not be specified more than once.
	// Note that solvent polymers must be distinct for the vesicle interior, exterior
	// and the bilayer reservoir space.

	for(cStringSequenceIterator vPolymer=m_VPolymers.begin(); vPolymer!=m_VPolymers.end(); vPolymer++)
	{
		if(riData.GetPolymerNamesMap().find(*vPolymer) != riData.GetPolymerNamesMap().end() &&
		   find(m_VPolymerTypes.begin(), m_VPolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*vPolymer)).second) == m_VPolymerTypes.end() &&
		   find(m_BLMPolymerTypes.begin(), m_BLMPolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*vPolymer)).second) == m_BLMPolymerTypes.end() &&
		   find(m_BLMReservoirTypes.begin(), m_BLMReservoirTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*vPolymer)).second) == m_BLMReservoirTypes.end())
		{
			m_VPolymerTypes.push_back((*riData.GetPolymerNamesMap().find(*vPolymer)).second);
		}
		else
			return false;
	}

	for(cStringSequenceIterator intPolymer=m_VInteriorPolymers.begin(); intPolymer!=m_VInteriorPolymers.end(); intPolymer++)
	{
		if(riData.GetPolymerNamesMap().find(*intPolymer) != riData.GetPolymerNamesMap().end() &&
		   find(m_VInteriorTypes.begin(), m_VInteriorTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*intPolymer)).second) == m_VInteriorTypes.end() &&
		   find(m_VPolymerTypes.begin(), m_VPolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*intPolymer)).second) == m_VPolymerTypes.end() &&
		   find(m_BLMPolymerTypes.begin(), m_BLMPolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*intPolymer)).second) == m_BLMPolymerTypes.end() &&
		   find(m_BLMReservoirTypes.begin(), m_BLMReservoirTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*intPolymer)).second) == m_BLMReservoirTypes.end())
		{
			m_VInteriorTypes.push_back((*riData.GetPolymerNamesMap().find(*intPolymer)).second);
		}
		else
			return false;
	}

	for(cStringSequenceIterator extPolymer=m_VExteriorPolymers.begin(); extPolymer!=m_VExteriorPolymers.end(); extPolymer++)
	{
		if(riData.GetPolymerNamesMap().find(*extPolymer) != riData.GetPolymerNamesMap().end() &&
		   find(m_VExteriorTypes.begin(), m_VExteriorTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*extPolymer)).second) == m_VExteriorTypes.end() &&
		   find(m_VPolymerTypes.begin(), m_VPolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*extPolymer)).second) == m_VPolymerTypes.end() &&
		   find(m_VInteriorTypes.begin(), m_VInteriorTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*extPolymer)).second) == m_VInteriorTypes.end() &&
		   find(m_BLMPolymerTypes.begin(), m_BLMPolymerTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*extPolymer)).second) == m_BLMPolymerTypes.end() &&
		   find(m_BLMReservoirTypes.begin(), m_BLMReservoirTypes.end(), 
		        (*riData.GetPolymerNamesMap().find(*extPolymer)).second) == m_BLMReservoirTypes.end())
		{
			m_VExteriorTypes.push_back((*riData.GetPolymerNamesMap().find(*extPolymer)).second);
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

	double vLower[3], vUpper[3], vCentre[3];

	vCentre[0]	= m_VXC*riData.GetSimBoxXLength();
	vCentre[1]	= m_VYC*riData.GetSimBoxYLength();
	vCentre[2]	= m_VZC*riData.GetSimBoxZLength();

	vLower[0]	= riData.GetCNTXCellWidth()*static_cast<double>(riData.GetWallXWidth());
	vUpper[0]	= riData.GetSimBoxXLength()-vLower[0];

	vLower[1]	= riData.GetCNTYCellWidth()*static_cast<double>(riData.GetWallYWidth());
	vUpper[1]	= riData.GetSimBoxYLength()-vLower[1];

	vLower[2]	= riData.GetCNTZCellWidth()*static_cast<double>(riData.GetWallZWidth());
	vUpper[2]	= riData.GetSimBoxZLength()-vLower[2];


	if((vCentre[0] + m_VOuterRadius) > vUpper[0] || (vCentre[0] - m_VOuterRadius) < vLower[0])
		return false;
	else if((vCentre[1] + m_VOuterRadius) > vUpper[1] || (vCentre[1] - m_VOuterRadius) < vLower[1])
		return false;
	else if((vCentre[2] + m_VOuterRadius) > vUpper[2] || (vCentre[2] - m_VOuterRadius) < vLower[2])
		return false;

	// Define supplementary variables for the vesicle. Note that the thickness
	// has been checked to ensure it is less than half the outer radius above.

	m_VInnerRadius = m_VOuterRadius - m_VThickness;
	m_VXCentre	   = vCentre[0];
	m_VYCentre	   = vCentre[1];
	m_VZCentre	   = vCentre[2];

	// If the vesicle is polymerised, check that the polymer names 
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

	if(m_bVPolymerise)
	{
		for(PolymerCrossLinkIterator iterLink=m_VCrossLinks.begin(); iterLink!=m_VCrossLinks.end(); iterLink++)
		{
			const zString name	= (*iterLink)->GetName();

			if(find(m_VPolymers.begin(), m_VPolymers.end(), name) != m_VPolymers.end() &&
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

	// All data has been validated so allow the initialisation to proceed

	return true;
}




