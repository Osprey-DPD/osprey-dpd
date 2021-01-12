/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStatePEP.cpp: implementation of the CInitialStatePEP class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "InitialStatePEP.h"
#include "PEPBuilder.h"
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

const zString CInitialStatePEP::m_Type = "pep";

const zString CInitialStatePEP::GetType()
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
	CInitialStateData* Create() {return new CInitialStatePEP();}

	const zString id = CInitialStatePEP::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Note that there is no way to set default data for a lamella as we 
// don't have access to the polymers that are defined in the control data file.

CInitialStatePEP::CInitialStatePEP() : m_bRandomlyOriented(false),
										m_ParticleNo(0),
										m_MinZFraction(0.0),
										m_MaxZFraction(0.0),
										m_SMA(0.0), m_SMB(0.0), m_SMC(0.0),
										m_SpringConstant(0.0), m_UnstretchedLength(0.0),
										m_Theta(0.0), m_Phi(0.0),
										m_bPatches(false),
										m_PatchNo(0),
										m_bSurface(true),
										m_BLMPolymer(""),
										m_BLMX(0), m_BLMY(0), m_BLMZ(0), 
										m_BLMCentre(0.0),
										m_BLMThickness(0.0),
										m_bBLMPolymerise(false),
										m_BLMPolymerType(-1),
										m_BilayerArea(0.0),
										m_Bilayer1Length(0.0),
										m_Bilayer2Length(0.0),
										m_BilayerCentre(0.0),
										m_UpperHead(0.0),
										m_LowerHead(0.0)
{
	// Polymerised particle data

	m_PatchZC.clear();
	m_PatchRadius.clear();
	m_PatchPsi.clear();

	// Surface data

	m_BLMCrossLinks.clear();


}

CInitialStatePEP::~CInitialStatePEP()
{
}


CInitialStatePEP& CInitialStatePEP::operator=(const CInitialStatePEP& oldIS)
{
	if(this != &oldIS)
	{
		// Particle data

		m_bRandomlyOriented			= oldIS.m_bRandomlyOriented;
		m_ParticleNo				= oldIS.m_ParticleNo;
		m_MinZFraction				= oldIS.m_MinZFraction;
		m_MaxZFraction				= oldIS.m_MaxZFraction;
		m_SMA						= oldIS.m_SMA;
		m_SMB						= oldIS.m_SMB;
		m_SMC						= oldIS.m_SMC;
		m_SpringConstant			= oldIS.m_SpringConstant;
		m_UnstretchedLength			= oldIS.m_UnstretchedLength;
		m_Theta						= oldIS.m_Theta;
		m_Phi						= oldIS.m_Phi;
		m_bPatches					= oldIS.m_bPatches;
		m_PatchNo					= oldIS.m_PatchNo;
		m_PatchZC					= oldIS.m_PatchZC;
		m_PatchRadius				= oldIS.m_PatchRadius;
		m_PatchPsi					= oldIS.m_PatchPsi;
		m_bSurface					= oldIS.m_bSurface;
		m_BLMPolymer				= oldIS.m_BLMPolymer;
		m_BLMX						= oldIS.m_BLMX;
		m_BLMY						= oldIS.m_BLMY;
		m_BLMZ						= oldIS.m_BLMZ;
		m_BLMCentre					= oldIS.m_BLMCentre;
		m_BLMThickness				= oldIS.m_BLMThickness;
		m_bBLMPolymerise			= oldIS.m_bBLMPolymerise;
		m_BLMPolymerType			= oldIS.m_BLMPolymerType;
		m_BilayerArea				= oldIS.m_BilayerArea;
		m_Bilayer1Length			= oldIS.m_Bilayer1Length;
		m_Bilayer2Length			= oldIS.m_Bilayer2Length;
		m_BilayerCentre				= oldIS.m_BilayerCentre;
		m_UpperHead					= oldIS.m_UpperHead;
		m_LowerHead					= oldIS.m_LowerHead;

		// Surface data

		m_BLMPolymer					= oldIS.m_BLMPolymer;
		m_BLMX							= oldIS.m_BLMX;
		m_BLMY							= oldIS.m_BLMY;
		m_BLMZ							= oldIS.m_BLMZ;
		m_BLMCentre						= oldIS.m_BLMCentre;
		m_BLMThickness					= oldIS.m_BLMThickness;
		m_bBLMPolymerise				= oldIS.m_bBLMPolymerise;
		m_BLMCrossLinks					= oldIS.m_BLMCrossLinks;

		// now the local data 

		m_BLMPolymerType				= oldIS.m_BLMPolymerType;
	    m_BilayerArea					= oldIS.m_BilayerArea;
	    m_Bilayer1Length				= oldIS.m_Bilayer1Length;
	    m_Bilayer2Length				= oldIS.m_Bilayer2Length;
	    m_BilayerCentre					= oldIS.m_BilayerCentre;
	    m_UpperHead						= oldIS.m_UpperHead;
	    m_LowerHead						= oldIS.m_LowerHead;

	}

	return *this;
}

zString CInitialStatePEP::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStatePEP::put(zOutStream& os) const
{
	os << "State	" << "pep"	<< zEndl;

	// Polymerised particle data first

	os << "         " << "RandomlyOriented  " << m_bRandomlyOriented << zEndl;
	os << "         " << "ParticleNo        " << m_ParticleNo        << zEndl;
	os << "			" << "MinZFraction		" << m_MinZFraction		 << zEndl;
	os << "			" << "MaxZFraction		" << m_MaxZFraction		 << zEndl;
	os << "			" << "SMA		        " << m_SMA << "  "		 << m_SMB << "  " << m_SMC << zEndl;
	os << "			" << "SpringConstant    "	<< m_SpringConstant	 << zEndl;
	os << "			" << "UnstretchedLength	"	<< m_UnstretchedLength	<< zEndl;
	os << "			" << "Theta		        "	<< m_Theta		      << zEndl;
	os << "			" << "Phi		        "	<< m_Phi		      << zEndl;

	// Patch data

	os << "         " << "Patches           " << m_bPatches          << zEndl;

	if(m_bPatches)
	{
		os << "         " << "PatchNo           " << m_PatchNo           << zEndl;

		for(long unsigned int i=0; i<m_PatchZC.size(); i++)
		{
			os << "	" << m_PatchZC.at(i) << "  " << m_PatchRadius.at(i) << "  " << m_PatchPsi.at(i) << zEndl;
		}
	}

	// Surface data

	os << "			" << "SurfacePolymer  " << m_BLMPolymer << zEndl;
	os << "			" << "Normal		  "	<< m_BLMX << "  "		<< m_BLMY << "  " << m_BLMZ << zEndl;
	os << "			" << "Centre		  "	<< m_BLMCentre			<< zEndl;
	os << "			" << "Thickness		  "	<< m_BLMThickness		<< zEndl;

	// Polymerise keyword is missing here


	return os;
}

zInStream& CInitialStatePEP::get(zInStream& is)
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

	is >> token;
	if(!is.good() || token != "SurfacePolymer")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> m_BLMPolymer;

		if(!is.good() || m_BLMPolymer == "Normal" || m_BLMPolymer.empty())
		{
			SetDataValid(false);
			return is;
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

	return is;
}

// Function to use the CPEPBuilder object to set the coordinates of all the 
// beads in the simulation into a PEP initial state. Note that the builder 
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStatePEP::Assemble(CInitialState& riState) const
{

//	CPEPBuilder blm(m_BLMPolymerTypes,	m_BLMReservoirTypes, 
//						   m_BLMUpperFractions, m_BLMCrossLinks,
//					       m_BLMX, m_BLMY, m_BLMZ,
//						   m_BLMCentre, m_BLMThickness,
//						   m_bBLMLinearise, m_bBLMPatches, m_bBLMPolymerise,
//						   m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length, 
//						   m_BilayerCentre, m_UpperHead, m_LowerHead,
//						   m_VPolymerTypes, m_VInteriorTypes, m_VExteriorTypes,
//						   m_VOuterFractions, m_VCrossLinks,
//						   m_VXC, m_VYC, m_VZC,
//						   m_VXCentre, m_VYCentre, m_VZCentre,
//						   m_VOuterRadius, m_VInnerRadius, m_VThickness,
//						   m_bVPatches, m_bVPolymerise);

//	return blm.Assemble(riState);

	return false;
}


// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStatePEP::Copy() const
{
	CInitialStatePEP* pISBLMV = new CInitialStatePEP;	// new data object

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

bool CInitialStatePEP::ValidateData(const CInputData& riData)
{
	/*
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


	// All data has been validated so allow the initialisation to proceed

  */
	return true;
}




