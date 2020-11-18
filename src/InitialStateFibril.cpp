/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateFibril.cpp: implementation of the CInitialStateFibril class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "InitialStateFibril.h"
#include "FibrilBuilder.h"
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

const zString CInitialStateFibril::m_Type = "fibril";

const zString CInitialStateFibril::GetType()
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
	CInitialStateData* Create() {return new CInitialStateFibril();}

	const zString id = CInitialStateFibril::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitialStateFibril::CInitialStateFibril() : m_CoreBead(""), m_LinkerBead(""),
                                             m_XN(0), m_YN(0), m_ZN(1),
                                             m_XC(0.5), m_YC(0.5), m_ZC(0.5),
                                             m_HalfLength(0.5), m_CoreRadius(0.0), m_LinkerRadius(0.0),
                                             m_Pitch(0.0), m_PitchAngle(0.0),
                                             m_CoreBeadType(-1), m_LinkerBeadType(-1)
											   
{
	m_Polymers.clear();
	m_PolymerTypes.clear();
}

CInitialStateFibril::~CInitialStateFibril()
{

}

CInitialStateFibril& CInitialStateFibril::operator=(const CInitialStateFibril& oldISV)
{
	if(this != &oldISV)
	{
		m_Polymers				= oldISV.m_Polymers;
        m_CoreBead              = oldISV.m_CoreBead;
        m_LinkerBead            = oldISV.m_LinkerBead;
		m_XN					= oldISV.m_XN;
		m_YN					= oldISV.m_YN;
		m_ZN					= oldISV.m_ZN;
		m_XC					= oldISV.m_XC;
		m_YC					= oldISV.m_YC;
		m_ZC					= oldISV.m_ZC;
        m_HalfLength            = oldISV.m_HalfLength;
        m_CoreRadius            = oldISV.m_CoreRadius;
        m_LinkerRadius          = oldISV.m_LinkerRadius;
        m_Pitch                 = oldISV.m_Pitch;
        m_PitchAngle            = oldISV.m_PitchAngle;
		m_PolymerTypes			= oldISV.m_PolymerTypes;
        m_CoreBeadType          = oldISV.m_CoreBeadType;
        m_LinkerBeadType        = oldISV.m_LinkerBeadType;
	}

	return *this;
}

zString CInitialStateFibril::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateFibril::put(zOutStream& os) const
{
	os << "State	" << "fibril"	<< zEndl;

	os << "			" << "Polymers		  ";

	for(cStringSequenceIterator iterName1=m_Polymers.begin(); iterName1!=m_Polymers.end(); iterName1++)
	{
		os << " " << (*iterName1);
	}

	os << zEndl;

    os << "         " << "CoreBead      "    << m_CoreBead      << zEndl;
    os << "         " << "LinkerBead    "    << m_LinkerBead    << zEndl;
	os << "			" << "Normal		"	 << m_XN << "  " << m_YN << "  " << m_ZN << zEndl;
	os << "			" << "Centre		"	 << m_XC << "  " << m_YC << "  " << m_ZC << zEndl;
    os << "         " << "HalfLength    "    << m_HalfLength    << zEndl;
    os << "         " << "CoreRadius    "    << m_CoreRadius    << zEndl;
    os << "         " << "LinkerRadius  "    << m_LinkerRadius  << zEndl;
    os << "         " << "Pitch         "    << m_Pitch         << zEndl;
    os << "         " << "PitchAngle    "    << m_PitchAngle    << zEndl;

	return os;
}

zInStream& CInitialStateFibril::get(zInStream& is)
{
	zString token  = "";
	zString sName  = "";
    
	is >> token;
	if(!is.good() || token != "Polymers")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;
 
		if(!is.good() || sName == "CoreBead" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "CoreBead")
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

    if(!is.good() || sName != "CoreBead")
    {
        SetDataValid(false);
        return is;
    }
    else
    {
        is >> m_CoreBead;
    }

    is >> token;
    
    if(!is.good() || token != "LinkerBead")
    {
        SetDataValid(false);
        return is;
    }
    else
    {
        is >> m_LinkerBead;
    }
    
/*
    is >> token;
    if(!is.good() || token != "Normal")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> m_XN >> m_YN >> m_ZN;
		
		// The normal must be along a major axis

		if(!is.good() || !( (m_XN == 1 && m_YN == 0 && m_ZN == 0) ||
			                (m_XN == 0 && m_YN == 1 && m_ZN == 0) ||
					        (m_XN == 0 && m_YN == 0 && m_ZN == 1) ) )
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
		is >> m_XC >> m_YC >> m_ZC;
		
		// Force the centre to lie within the SimBox

		if(!is.good() || m_XC < 0.01 || m_YC < 0.01 || m_ZC < 0.01
					  || m_XC > 0.99 || m_YC > 0.99 || m_ZC > 0.99 )
		{
			SetDataValid(false);
			return is;
		}
	}
*/
    
/*
        is >> token;
        if(!is.good() || token != "HalfLength")
        {
            SetDataValid(false);
            return is;
        }
        else
        {
            is >> m_HalfLength;
            std::cout << "half lrength = " << m_HalfLength << zEndl;

            // Fibril length must be positive and less than half the box length.

            if(!is.good() || m_HalfLength <= 0.0 || m_HalfLength > 0.5)
            {
                SetDataValid(false);
                return is;
            }
        }
*/

	is >> token;
	if(!is.good() || token != "CoreRadius")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> m_CoreRadius;

		// Fibril core radius must be greater than a bead diameter. We cannot check
		// here that it is less than the SimBox size but that will be done in CInputData.

		if(!is.good() || m_CoreRadius < 1.0)
		{
			SetDataValid(false);
			return is;
		}
	}

    is >> token;
    if(!is.good() || token != "LinkerRadius")
    {
        SetDataValid(false);
        return is;
    }
    else
    {
        is >> m_LinkerRadius;
 
        // Fibril linker radius must be greater than a bead diameter. We cannot check
        // here that it is less than the SimBox size but that will be done in CInputData.

        if(!is.good() || m_LinkerRadius < 1.0)
        {
            SetDataValid(false);
            return is;
        }
    }
    
    is >> token;
    if(!is.good() || token != "Pitch")
    {
        SetDataValid(false);
        return is;
    }
    else
    {
        is >> m_Pitch;

        // Pitch must be at least one bead diameter

        if(!is.good() || m_Pitch < 0.1)
        {
            SetDataValid(false);
            return is;
        }
    }

    is >> token;
    if(!is.good() || token != "PitchAngle")
    {
        SetDataValid(false);
        return is;
    }
    else
    {
        is >> m_PitchAngle;

        // Pitch angle must be between 0 and 360 degrees

        if(!is.good() || m_PitchAngle < 0.0 || m_PitchAngle > 360.0)
        {
            SetDataValid(false);
            return is;
        }
    }

	return is;
}

// Function to use the CFibrilBuilder object to set the coordinates of all the
// beads in the simulation into a single fibril initial state. Note that the builder
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStateFibril::Assemble(CInitialState& riState) const
{
	CFibrilBuilder fibril(m_PolymerTypes, m_CoreBeadType, m_LinkerBeadType, m_XN,  m_YN,  m_ZN,
							m_XC,  m_YC,  m_ZC, m_HalfLength, m_CoreRadius, m_LinkerRadius, m_Pitch, m_PitchAngle);

	return fibril.Assemble(riState);
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateFibril::Copy() const
{
	CInitialStateFibril* pISV = new CInitialStateFibril;	// new data object

	*pISV = *this;		// assign new data values to new object

	return pISV;
}

// Virtual function to check that the data supplied by the user to assemble a
// fibril initial state is consistent.
//
// This function cannot be const because we store data for later use.

bool CInitialStateFibril::ValidateData(const CInputData& riData)
{
	// Check that the polymer names specified in the fibril actually occur
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
    
    // Check that the bead names are valid: note that we don't check they actually occur in the polymer yet
    
     if(!riData.IsExternalNameValid(m_CoreBead))
        return false;
     else if(!riData.IsBeadinMap(m_CoreBead))
        return false;

    m_CoreBeadType = (*riData.GetBeadNamesMap().find(m_CoreBead)).second;

    if(!riData.IsExternalNameValid(m_LinkerBead))
       return false;
    else if(!riData.IsBeadinMap(m_LinkerBead))
       return false;

     m_LinkerBeadType = (*riData.GetBeadNamesMap().find(m_LinkerBead)).second;
    
    // Convert the pitch angle to radians
    
    m_PitchAngle = m_PitchAngle*xxBase::m_globalPI/180.0;

	// Calculate the centre and radius of the fibril.
	//
	// If there is no wall the functions just return zero for its thickness.
	//
	// For each of the SimBox axes, we check that the fibril does not overlap
	// the wall, if it exists, in that dimension:
	//
	// lower	= thickness of wall on bottom of SimBox.
	// upper	= distance from bottom of SimBox to start of wall on top face
	// centre	= fraction of SimBox width at centre of fibril

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
    
	// Calculate the length of the fibril and check it does not exceed the SimBox length
	// in the direction of its axis taking a wall into account If the normal is 
	// invalid we exit with an error.
    
    // Also check that the fibril width fits within the box. Note that we do not include the GFP moiety
    // at the end of the linker as we don't know its size.

	double actualHalfLength, leftEnd, rightEnd;
    
    const double fibrilWidth = m_CoreRadius + m_LinkerRadius;
    
	if(m_XN == 1)
	{
	    if((centre[1] + fibrilWidth) > upper[1] || (centre[1] - fibrilWidth) < lower[1])
		    return false;
	    else if((centre[2] + fibrilWidth) > upper[2] || (centre[2] - fibrilWidth) < lower[2])
		    return false;

		actualHalfLength = m_HalfLength*riData.GetSimBoxXLength();
		leftEnd  = centre[0] - actualHalfLength;
		rightEnd = centre[0] + actualHalfLength;

		if(leftEnd < lower[0] || rightEnd > upper[0])
			return false;
	}
	else if(m_YN == 1)
	{
	    if((centre[0] + fibrilWidth) > upper[0] || (centre[0] - fibrilWidth) < lower[0])
		    return false;
	    else if((centre[2] + fibrilWidth) > upper[2] || (centre[2] - fibrilWidth) < lower[2])
		    return false;

		actualHalfLength = m_HalfLength*riData.GetSimBoxYLength();
		leftEnd  = centre[1] - actualHalfLength;
		rightEnd = centre[1] + actualHalfLength;

		if(leftEnd < lower[1] || rightEnd > upper[1])
			return false;
	}
	else if(m_ZN == 1)
	{
	    if((centre[0] + fibrilWidth) > upper[0] || (centre[0] - fibrilWidth) < lower[0])
		    return false;
	    else if((centre[1] + fibrilWidth) > upper[1] || (centre[1] - fibrilWidth) < lower[1])
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

	// Input data has been checked so allow the initialisation to proceed.

	return true;
}
