/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateLamella.cpp: implementation of the CInitialStateLamella class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "InitialStateLamella.h"
#include "mpmInitialState.h"
#include "pmISLamella.h"
#include "LamellaBuilder.h"
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

const zString CInitialStateLamella::m_Type = "lamella";

const zString CInitialStateLamella::GetType()
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
	CInitialStateData* Create() {return new CInitialStateLamella();}

	const zString id = CInitialStateLamella::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Set the default polymer name for the lamella initial state to agree with 
// that defined in the CInputData's WriteDefaultControlDataFile() function.

CInitialStateLamella::CInitialStateLamella() : m_Polymer("Lipid"), 
											   m_X(0), m_Y(0), m_Z(1), 
											   m_Centre(0.5),
											   m_Thickness(5.0),
											   m_bLinearise(true),
											   m_UpperFraction(0.5),
											   m_bPolymerise(false),
											   m_PolymerisedName(""),
											   m_PolymerisedFraction(0.0),
										   	   m_Position(0),
											   m_SpringConstant(0.0),
											   m_UnstretchedLength(0.0),
											   m_PolymerType(0),
											   m_PolymerisedType(0),
											   m_BilayerArea(0.0),
											   m_Bilayer1Length(0.0),
											   m_Bilayer2Length(0.0),
											   m_BilayerCentre(0.0),
											   m_UpperHead(0.0),
											   m_LowerHead(0.0)
{
}

CInitialStateLamella::~CInitialStateLamella()
{
}

CInitialStateLamella& CInitialStateLamella::operator=(const CInitialStateLamella& oldISL)
{
	if(this != &oldISL)
	{
		m_Polymer				= oldISL.m_Polymer;
		m_X						= oldISL.m_X;
		m_Y						= oldISL.m_Y;
		m_Z						= oldISL.m_Z;
		m_Centre				= oldISL.m_Centre;
		m_Thickness				= oldISL.m_Thickness;
		m_bLinearise			= oldISL.m_bLinearise;
		m_UpperFraction			= oldISL.m_UpperFraction;
	    m_bPolymerise			= oldISL.m_bPolymerise;
	    m_PolymerisedName		= oldISL.m_PolymerisedName;
	    m_PolymerisedFraction	= oldISL.m_PolymerisedFraction;
	    m_Position				= oldISL.m_Position;
	    m_SpringConstant		= oldISL.m_SpringConstant;
	    m_UnstretchedLength		= oldISL.m_UnstretchedLength;

		m_PolymerType			= oldISL.m_PolymerType;
		m_PolymerisedType		= oldISL.m_PolymerisedType;
	    m_BilayerArea			= oldISL.m_BilayerArea;
	    m_Bilayer1Length		= oldISL.m_Bilayer1Length;
	    m_Bilayer2Length		= oldISL.m_Bilayer2Length;
	    m_BilayerCentre			= oldISL.m_BilayerCentre;
	    m_UpperHead				= oldISL.m_UpperHead;
	    m_LowerHead				= oldISL.m_LowerHead;
	}

	return *this;
}

zString CInitialStateLamella::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateLamella::put(zOutStream& os) const
{
	os << "State   " << "lamella"			<< zEndl;
	os << "        " << "Polymer          " << m_Polymer				<< zEndl;
	os << "        " << "Normal           "	<< m_X << "  "	<< m_Y << "  " << m_Z << zEndl;
	os << "        " << "Centre           "	<< m_Centre			<< zEndl;
	os << "        " << "Thickness        "	<< m_Thickness		<< zEndl;
	os << "        " << "Linearise        "	<< m_bLinearise		<< zEndl;
	os << "        " << "UpperFraction    "	<< m_UpperFraction	<< zEndl;
	os << "        " << "Polymerise       "	<< m_bPolymerise	<< zEndl;

	if(m_bPolymerise)
	{
		os << "        " << "Polymer			"	<< m_PolymerisedName		<< zEndl;
		os << "        " << "Fraction			"	<< m_PolymerisedFraction	<< zEndl;
		os << "        " << "Position			"	<< m_Position				<< zEndl;
		os << "        " << "SpringConstant	"	<< m_SpringConstant			<< zEndl;
		os << "        " << "UnstretchedLength	"	<< m_UnstretchedLength		<< zEndl;
	}

	os << zEndl;

	return os;
}

zInStream& CInitialStateLamella::get(zInStream& is)
{
	zString token;
	zString polymer;
	double centre, thickness;
	long x, y, z;
	bool bLinear;
	double upperFraction;
	bool bPolymerise;
	zString polymerisedName;
	double  polymerisedFraction;
	long    polymerisedPosition;
	double	springConstant;
	double  unstretchedLength;

	is >> token;
	if(!is.good() || token != "Polymer")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> polymer;
		if(!is.good() || polymer.empty())
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Normal")
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

	// Read the fraction of the polymer that is in the upper monolayer.
	// Obviously, the remainder of the polymers are in the lower monolayer.

	is >> token;
	if(!is.good() || token != "UpperFraction")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> upperFraction;

		// Fraction must lie between 0 and 1

		if(!is.good() || upperFraction < 0.0 || upperFraction > 1.0)
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
		is >> bPolymerise;

		if(!is.good())
		{
			SetDataValid(false);
			return is;
		}
		else if(bPolymerise)
		{
			// Name of polymer to polymerise

			is >> token;
			if(!is.good() || token != "Polymer")
			{
				SetDataValid(false);
				return is;
			}
			else
			{
				is >> polymerisedName;
				if(!is.good() || polymerisedName.empty())
				{
					SetDataValid(false);
					return is;
				}
			}

			// Fraction of polymers to polymerise
			is >> token;
			if(!is.good() || token != "Fraction")
			{
				SetDataValid(false);
				return is;
			}
			else
			{
				is >> polymerisedFraction;
				if(!is.good() || polymerisedFraction < 0.0 || polymerisedFraction > 1.0)
				{
					SetDataValid(false);
					return is;
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
				is >> polymerisedPosition;
				if(!is.good() || polymerisedPosition < 0)
				{
					SetDataValid(false);
					return is;
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
				is >> springConstant;
				if(!is.good() || springConstant < 0.0)
				{
					SetDataValid(false);
					return is;
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
				is >> unstretchedLength;
				if(!is.good() || unstretchedLength < 0.0)
				{
					SetDataValid(false);
					return is;
				}
			}
		}
	}




	// Data has been read successfully so store it in the member variables.
	// We only store the polymerise data if the flag indicates it is used.

	m_Polymer		= polymer;
	m_X				= x;
	m_Y				= y;
	m_Z				= z;
	m_Centre		= centre;
	m_Thickness		= thickness;
	m_bLinearise	= bLinear;
	m_UpperFraction	= upperFraction;
	m_bPolymerise	= bPolymerise;

	if(m_bPolymerise)
	{
		m_PolymerisedName		= polymerisedName;
		m_PolymerisedFraction	= polymerisedFraction;
		m_Position				= polymerisedPosition;
		m_SpringConstant		= springConstant;
		m_UnstretchedLength		= unstretchedLength;
	}

	return is;
}

// Function to use the CLamellaBuilder object to set the coordinates of all the 
// beads in the simulation into a lamella initial state. Note that the builder 
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStateLamella::Assemble(CInitialState& riState) const
{
	CLamellaBuilder lamella(m_PolymerType, m_X, m_Y, m_Z, m_Centre, m_Thickness,
							m_bLinearise, m_UpperFraction,
							m_bPolymerise, m_PolymerisedType, m_PolymerisedFraction,
							m_Position, m_SpringConstant, m_UnstretchedLength,
							m_BilayerArea, m_Bilayer1Length, m_Bilayer2Length,
							m_BilayerCentre, m_LowerHead, m_UpperHead);

#if EnableParallelSimBox == SimMPSEnabled
	return lamella.AssembleP(riState);
#else
	return lamella.Assemble(riState);
#endif
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateLamella::Copy() const
{
	CInitialStateLamella* pISL = new CInitialStateLamella;	// new data object

	*pISL = *this;		// assign new data values to new object

	return pISL;
}

// Virtual function to assemble the IS data from the corresponding 
// mpmInitialState message instance. Note that we don't allow the
// parallel lamella IS to be polymerised so we set the flat to false here.

void CInitialStateLamella::SetData(const mpmInitialState *const pMsg)
{
#if EnableParallelSimBox == SimMPSEnabled

    const pmISLamella* const pLamellaMsg = dynamic_cast<const pmISLamella* const>(pMsg);
	
	if(pLamellaMsg)
	{
	    m_Polymer		= pLamellaMsg->GetPolymer();
	    m_X				= pLamellaMsg->GetXNormal();
	    m_Y				= pLamellaMsg->GetYNormal();
	    m_Z				= pLamellaMsg->GetZNormal();
	    m_Centre		= pLamellaMsg->GetCentre();
	    m_Thickness		= pLamellaMsg->GetThickness();
	    m_bLinearise	= pLamellaMsg->GetLinearise();
	    m_UpperFraction	= pLamellaMsg->GetUpperFraction();
	    m_bPolymerise	= false;
		}
		else
		{
		    // Wrong IS message, so we leave all parameters invalid.
		}
#endif
}

// Parallel code function to calculate whether the pre-assembled IS overlaps the owning processor's space. 
// Because space is not necessarily translationally invariant in the 
// IS, each processor has to work out from the specific structures present how many polymers it owns, and therefore must create,
// in order to fill its own space.  The results are returned as the fraction of each polymer type that must be created in the 
// owning processor's SimBox. This is rescaled by the calling routine to account for the whole simulation Space.
//
// Note that the ValidataDataP() function must have already been called before this function is called.
//
// For a planar bilayer IS, some of the processors will contain the bilayer but others may only contain solvent. Because we
// assume a single-component bilayer, all the remaining polymer types are treated as solvent  and randomly positioned in Space.
	
void CInitialStateLamella::CalculatePolymerFractionsP(double lx, double ly, double lz, double xorigin, double yorigin, double zorigin, double xspace, double yspace, double zspace, 
                                                   zDoubleVector& rvPolymerFractions) const
{
    // Define the fraction of the processor's SimBox volume occupied by the bilayer and solvent.
	
	double bilayerFraction = 0.0;
	double solventFraction = 0.0;
	
	if(m_X == 1)
	{		
		// For now, we assume the bilayer is either wholly in or wholly outside the SimBox but does not straddle the boundaries.
		
		if(m_UpperHead < xorigin || m_LowerHead > (xorigin+lx))  // No part of bilayer is in owning processor's SimBox
		{
		    // No part of the bilayer passes through the owning processor's Space, so we set the blayer polymers' fractions to zero,
			// and the solvent polymer fraction to the ratio of the SimBox to the total solvent volume
			
			bilayerFraction = 0.0;
			solventFraction = 1.0;
		}
		else if(m_LowerHead < xorigin && m_UpperHead > (xorigin+lx))  // Bilayer occupies the whole SimBox, ie no solvent
		{
	        bilayerFraction = 1;
			solventFraction = 0.0;
		}
		else if(m_LowerHead > xorigin || m_UpperHead < xorigin+lx)  // Whole bilayer is in SimBox
		{
	        bilayerFraction = 1;
			solventFraction = 1;
		}
		else if(m_LowerHead < xorigin && m_UpperHead < (xorigin+lx)) // Bilayer straddles bottom of SimBox
		{
		    std::cout << "ERROR in InitialStateLamella -bilayer straddles SimBox boundaries this should not be seen" << zEndl;
	        bilayerFraction = lz*ly/(zspace*yspace);
			solventFraction = (lx - m_Thickness)*lz*ly/(zspace*yspace*(xspace - m_Thickness));
		}
		else if(m_LowerHead < (xorigin+lx) && m_UpperHead > (xorigin+lx)) // Bilayer straddles top of SimBox
		{
		    std::cout << "ERROR in InitialStateLamella -bilayer straddles SimBox boundaries this should not be seen" << zEndl;
	        bilayerFraction = lz*ly/(zspace*yspace);
			solventFraction = (lx - m_Thickness)*lz*ly/(zspace*yspace*(xspace - m_Thickness));
		}
		else
		{
		    std::cout << "ERROR in InitialStateLamella - this should not be seen" << zEndl;
		}
	}
	else if(m_Y == 1)
	{
		// For now, we assume the bilayer is either wholly in or wholly outside the SimBox but does not straddle the boundaries.
		
		if(m_UpperHead < yorigin || m_LowerHead > (yorigin+ly))  // No part of bilayer is in owning processor's SimBox
		{
		    // No part of the bilayer passes through the owning processor's Space, so we set the blayer polymers' fractions to zero,
			// and the solvent polymer fraction to the ratio of the SimBox to the total solvent volume
			
			bilayerFraction = 0.0;
			solventFraction = lx*ly*lz/(xspace*zspace*(yspace-m_Thickness));
		}
		else if(m_LowerHead < yorigin && m_UpperHead > (yorigin+ly))  // Bilayer occupies the whole SimBox, ie no solvent
		{
	        bilayerFraction = lx*lz/(xspace*zspace);
			solventFraction = 0.0;
		}
		else if(m_LowerHead > yorigin || m_UpperHead < yorigin+ly)  // Whole bilayer is in SimBox
		{
	        bilayerFraction = lx*lz/(xspace*zspace);
			solventFraction = lx*lz*(ly - m_Thickness)/(xspace*zspace*(yspace - m_Thickness));
		}
		else if(m_LowerHead < yorigin && m_UpperHead < (yorigin+ly)) // Bilayer straddles bottom of SimBox
		{
		    std::cout << "ERROR in InitialStateLamella -bilayer straddles SimBox boundaries this should not be seen" << zEndl;
	        bilayerFraction = lx*lz/(xspace*zspace);
			solventFraction = (ly - m_Thickness)*lx*lz/(xspace*zspace*(yspace - m_Thickness));
		}
		else if(m_LowerHead < (yorigin+ly) && m_UpperHead > (yorigin+ly)) // Bilayer straddles top of SimBox
		{
		    std::cout << "ERROR in InitialStateLamella -bilayer straddles SimBox boundaries this should not be seen" << zEndl;
	        bilayerFraction = lx*lz/(xspace*zspace);
			solventFraction = (ly - m_Thickness)*lx*lz/(xspace*zspace*(yspace - m_Thickness));
		}
		else
		{
		    std::cout << "ERROR in InitialStateLamella - this should not be seen" << zEndl;
		}
	}
	else
	{
	    // Bilayer normal is in Z direction, so we check if any part of the bilayer passes through the current SimBox
	    // Now we know what fraction of the SimBox is occupied by the bilayer and solvent, we convert these to the fraction of 
	    // the total number of polymers in the whole simulation Space that belong in the current processor's Space. For a 
	    // translationally invariant planar bilayer, this is just the ratio of the area of the bilayer in the current SimBox 
	    // to the total area of the bilayer.
		
		// For now, we assume the bilayer is either wholly in or wholly outside the SimBox but does not straddle the boundaries.
				
		if(m_UpperHead < zorigin || m_LowerHead > (zorigin+lz))  // No part of bilayer is in owning processor's SimBox
		{
		    // No part of the bilayer passes through the owning processor's Space, so we set the blayer polymers' fractions to zero,
			// and the solvent polymer fraction to the ratio of the SimBox to the total solvent volume
			
			bilayerFraction = 0.0;
			solventFraction = 1.0;
		}
		else if(m_LowerHead < zorigin && m_UpperHead > (zorigin+lz))  // Bilayer occupies the whole SimBox, ie no solvent
		{
	        bilayerFraction = 1.0;
			solventFraction = 0.0;
		}
		else if(m_LowerHead > zorigin || m_UpperHead < zorigin+lz)  // Whole bilayer is in SimBox
		{
	        bilayerFraction = 1.0;
			solventFraction = 1.0;
		}
		else if(m_LowerHead < zorigin && m_UpperHead < (zorigin+lz)) // Bilayer straddles bottom of SimBox
		{
		    std::cout << "ERROR in InitialStateLamella -bilayer straddles SimBox boundaries this should not be seen" << zEndl;
	        bilayerFraction = lx*ly/(xspace*yspace);
			solventFraction = (lz - m_Thickness)*lx*ly/(xspace*yspace*zspace);
		}
		else if(m_LowerHead < (zorigin+lz) && m_UpperHead > (zorigin+lz)) // Bilayer straddles top of SimBox
		{
		    std::cout << "ERROR in InitialStateLamella -bilayer straddles SimBox boundaries this should not be seen" << zEndl;
	        bilayerFraction = lx*ly/(xspace*yspace);
			solventFraction = (lz - m_Thickness)*lx*ly/(xspace*yspace*(zspace - m_Thickness));
		}
		else
		{
		    std::cout << "ERROR in InitialStateLamella - this should not be seen" << zEndl;
		}
	}
	
	// If the owning processor's SimBox contains part of the bilayer, we leave the polymer fractions unchanged, but if
	// it does not, we rescale all the solvent polymer fractions to fill the box and set the bilayer polymer fraction to zero.
	
	if(bilayerFraction == 0.0)
	{
	    const double scale = 1.0/(1.0 - rvPolymerFractions.at(m_PolymerType));
		
	    for(long type=0; type<rvPolymerFractions.size(); type++)
	    {
		    if(type == m_PolymerType)
		    {
			    rvPolymerFractions.at(type) = 0.0;
		    }
	        else
		    {
			    rvPolymerFractions.at(type) *= scale;
		    }			
	    }
	}
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

bool CInitialStateLamella::ValidateData(const CInputData& riData)
{
	// Check that the polymer name specified in the lamella actually
	// occurs in the map and store its type for later use.

	if(riData.GetPolymerNamesMap().find(m_Polymer) != riData.GetPolymerNamesMap().end())
	{
		m_PolymerType = (*riData.GetPolymerNamesMap().find(m_Polymer)).second;
	}
	else
		return false;

	// If the initial state is polymerised, check that the polymer name 
	// matches that of the lamella polymer and store its type. This is not
	// actually necessary for a single component lamella, but will need to
	// be generalised for composite lamellae. Check also that the polymerisation
	// position is within the polymer size. A negative value is actually
	// forbidden by the get() function above, but we repeat it here for completeness.
	// The bond force constants have already been checked and do not depend on the
	// polymer properties.

	if(m_bPolymerise)
	{
		if(m_PolymerisedName == m_Polymer)
		   m_PolymerisedType = m_PolymerType;
		else
			return false;

		if(m_Position < 0 || m_Position >= (riData.GetPolymerTypes().at(m_PolymerType))->GetSize())
			return false;
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

	if((centre + 0.5*m_Thickness) > upper || (centre - 0.5*m_Thickness) < lower)
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
	m_BilayerCentre		= centre;
	m_LowerHead			= centre - 0.5*m_Thickness;
	m_UpperHead			= centre + 0.5*m_Thickness;

	return true;
}

// Parallel version of the above function that checks that the single lamella
// specified in the initial state can be assembled within the whole simulation Space.
// It is only called on P0, and is essentially just a scaled up version of the 
// original function that uses the dimensions appropriate to the whole 
// simulation Space and not just the domain of a single processor. Note that we don't allow
// the parallel lamella IS to be polymerised and there cannot be a Wall.

bool CInitialStateLamella::ValidateDataP(const CInputData& riData)
{
#if SimMPS == SimulationEnabled

	// Check that the polymer name specified in the lamella actually
	// occurs in the map and store its type for later use.

	if(riData.GetPolymerNamesMap().find(m_Polymer) != riData.GetPolymerNamesMap().end())
	{
		m_PolymerType = (*riData.GetPolymerNamesMap().find(m_Polymer)).second;
	}
	else
		return false;

	// Calculate the centre, width and area of the lamella with respect to the 
    // whole simulation Space.
	//
	//
	// centre	= fraction of SimBox width at centre of bilayer

	double l1, l2, lower, upper, centre;

	if(m_X == 1)
	{
		l1		= riData.GetSimBoxYLength();
		l2		= riData.GetSimBoxZLength();
		lower	= 0.0;
		upper	= riData.GetSimBoxXLength();
		centre	= m_Centre*riData.GetSimSpaceXLength();
	}
	else if(m_Y == 1)
	{
		l1		= riData.GetSimBoxXLength();
		l2		= riData.GetSimBoxZLength();
		lower	= 0.0;
		upper	= riData.GetSimBoxYLength();
		centre	= m_Centre*riData.GetSimSpaceYLength();
	}
	else if(m_Z == 1)
	{
		l1		= riData.GetSimBoxXLength();
		l2		= riData.GetSimBoxYLength();
		lower	= 0.0;
		upper	= riData.GetSimSpaceZLength();
		centre	= m_Centre*riData.GetSimSpaceZLength();
	}
	else
		return false;

	if((centre + 0.5*m_Thickness) > upper || (centre - 0.5*m_Thickness) < lower)
		return false;

	// Input data has been checked so allow the initialisation to proceed.
	// Copy the coordinates of the bilayer midplane and its extremities into
	// local data for passing to the CLamellaBuilder. Note that we have to keep
	// track of the bilayer orientation to use this data correctly, but we
	// can be sure that it is valid and does not overlap the walls or boundaries.
		
	m_BilayerArea		= l1*l2;
	m_Bilayer1Length	= l1;
	m_Bilayer2Length	= l2;
	m_BilayerCentre		= centre;
	m_LowerHead			= centre - 0.5*m_Thickness;
	m_UpperHead			= centre + 0.5*m_Thickness;

#endif

	return true;
}
