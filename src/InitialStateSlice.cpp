/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialStateSlice.cpp: implementation of the CInitialStateSlice class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "InitialStateSlice.h"
#include "SliceBuilder.h"
#include "InputData.h"

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
	using std::find;
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

const zString CInitialStateSlice::m_Type = "slice";

const zString CInitialStateSlice::GetType()
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
	CInitialStateData* Create() {return new CInitialStateSlice();}

	const zString id = CInitialStateSlice::GetType();

	const bool bRegistered = acfInitialStateFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitialStateSlice::CInitialStateSlice() : m_SlabTotal(0),
                                           m_XN(0), m_YN(0), m_ZN(0)											   
{
    m_vWidths.clear();
    m_mmPolymers.clear();
    m_mPolymerTypes.clear();
}

CInitialStateSlice::~CInitialStateSlice()
{

}

CInitialStateSlice& CInitialStateSlice::operator=(const CInitialStateSlice& oldIS)
{
	if(this != &oldIS)
	{
		m_SlabTotal	    = oldIS.m_SlabTotal;
		m_XN		    = oldIS.m_XN;
		m_YN		    = oldIS.m_YN;
		m_ZN		    = oldIS.m_ZN;
        m_vWidths       = oldIS.m_vWidths;
        m_mmPolymers    = oldIS.m_mmPolymers;
        m_mPolymerTypes = oldIS.m_mPolymerTypes;
	}

	return *this;
}

zString CInitialStateSlice::GetInitialStateType() const
{
	return m_Type;
}

// Virtual member functions to provide IO for this CInitialStateData-derived class

zOutStream& CInitialStateSlice::put(zOutStream& os) const
{
	os << "State	" << GetInitialStateType()	<< zEndl;
	os << "			" << "SliceTotal "	<< m_SlabTotal	<< zEndl;
	os << "			" << "Widths ";

	for(czDoubleVectorIterator iv=m_vWidths.begin(); iv!=m_vWidths.end(); iv++)
	{
		os << " " << (*iv);
	}
	os << zEndl;

    // Now output the polymer name belonging to each slab

    for(long slab = 0; slab < m_SlabTotal; slab++)
    {
	    os << "			" << "Polymers";

        cLongStringMMIterator imm=m_mmPolymers.find(slab);

        if(imm != m_mmPolymers.end())
        {
            do 
            {
                os << imm->first << " " << imm->second << " ";
                imm++;
            }
            while (imm != m_mmPolymers.upper_bound(slab));
        }
        else
        {
            os << zEndl;  // This means the slab is empty
        }   
    }

	os << "			" << "Normal		"	<< m_XN << "  " << m_YN << "  " << m_ZN << zEndl;

	return os;
}

zInStream& CInitialStateSlice::get(zInStream& is)
{
	zString token  = "";
	zString sName  = "";

    double width;

	is >> token;
	if(!is.good() || token != "SliceTotal")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> m_SlabTotal;
		
		if(!is.good() || m_SlabTotal < 1)
		{
			SetDataValid(false);
			return is;
		}
	}

    // Now read in slabTotal-1 values for the widths: we don't read in the final 
    // one as it is calculated from 1.0 - sum of other widths. This prevents the
    // user from making silly errors in summing the fractional numbers.

	is >> token;
	if(!is.good() || token != "Widths")
	{
		SetDataValid(false);
		return is;
	}
	else
    {
        double runningWidth = 0.0;

        if(m_SlabTotal > 1)
        {
	        for(long iw = 0; iw < m_SlabTotal-1; iw++)
	        {
                is >> width;

                runningWidth += width;

                // The widths are in units of the SimBox size

	            if(!is.good() || width < 0.0 || width > 1.0 || runningWidth >= 1.0)
	            {
		            SetDataValid(false);
		            return is;
	            }
	            else
	            {
                    m_vWidths.push_back(width);
                }
            }

            m_vWidths.push_back(1.0 - runningWidth);
        }
        else
        {
            is >> width;
	        if(!is.good() || width != 1.0)
	        {
		        SetDataValid(false);
		        return is;
	        }
	        else
	        {
                m_vWidths.push_back(1.0);
            }
        }
	}

    // Now read in the polymers that are to be assembled in each slab.
    // Note that the same polymer may occur in more than one slab.
    // Each slab has one line of data beginining with the keyword "Polymer".
    // The number of slabs is known, but the number of polymers in each slab
    // is not. The polymer names are stored in a multimap using their slab number
    // as the key.

    is >> sName;

    for(long iw = 0; iw < m_SlabTotal; iw++)
    {
	    if(!is.good() || sName != "Polymers")
	    {
		    SetDataValid(false);
		    return is;
	    }
	    else
        {
            is >> sName;

            if(!is.good() || sName == "Polymers" || sName == "Normal" || sName.empty())
            {
				SetDataValid(false);
				return is;
			}
            else
            {
			    while(sName != "Polymers" && sName != "Normal")
			    {
				    if(is.good() && !sName.empty())
                    {
			            m_mmPolymers.insert(zPairLS(iw, sName));
                    }
				    else
				    {
					    SetDataValid(false);
					    return is;
				    }

				    is >> sName;
			    }
            }
        }
    }

    // We have to read the normal vector here so that we know when we have finished
    // reading in the (arbitrary) polymers in each slab. Note that the token "Normal"
    // has already been read in as the ending condition of the loop above.

	if(!is.good() || sName != "Normal")
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


	return is;
}

// Function to use the CSliceBuilder object to set the coordinates of all the 
// beads in the simulation into a worm micelle initial state. Note that the builder 
// is local to this CInitialStateLamella object. We pass it the data it needs
// from the input data to position the beads, bonds and polymers into the
// bilayers required.

bool CInitialStateSlice::Assemble(CInitialState& riState) const
{
	CSliceBuilder slice(m_SlabTotal, m_XN,  m_YN,  m_ZN, m_vWidths, m_mmPolymers, m_mPolymerTypes);

	return slice.Assemble(riState);
}

// Pure virtual function to allow CInitialStateData-derived objects to be
// copy-constructed correctly.

CInitialStateData* CInitialStateSlice::Copy() const
{
	CInitialStateSlice* pISV = new CInitialStateSlice;	// new data object

	*pISV = *this;		// assign new data values to new object

	return pISV;
}

// Virtual function to check that the data supplied by the user to assemble a
// slice initial state is consistent. We have checked the normal vector and
// slab widths above, so we only have to check that the specified polymer names
// exist and that the same polymer is not specified more than once in a slab.
//
// This function cannot be const because we store data for later use.

bool CInitialStateSlice::ValidateData(const CInputData& riData)
{
	// Check that the polymer names specified in the slabs actually occur 
	// in the CInitialState map and store their types for later use. 
    // We also ensure that each polymer appears at most once in the map.

    StringSequence slabPolymerNames;

    for(long sn = 0; sn < m_SlabTotal; sn++)
    {
        slabPolymerNames.clear();
        cLongStringMMIterator imm=m_mmPolymers.find(sn);

        if(imm != m_mmPolymers.end())
        {
            do 
            {
                long slab = imm->first;
                const zString polymerName = imm->second;

                if(find(slabPolymerNames.begin(), slabPolymerNames.end(), polymerName) == slabPolymerNames.end())
                {
                    slabPolymerNames.push_back(polymerName);
                }
                else
                {
                    return false;
                }

		        long polymerType = (*riData.GetPolymerNamesMap().find(polymerName)).second;

		        if(riData.GetPolymerNamesMap().find(polymerName) != riData.GetPolymerNamesMap().end())
                {
                    // Only add the polymer type if it is not already there

                    if(m_mPolymerTypes.find(polymerName) == m_mPolymerTypes.end())
                    {
			            m_mPolymerTypes.insert(zPairSL(polymerName, polymerType));
                    }
		        }
		        else
			        return false;

                imm++;
            }
            while (imm != m_mmPolymers.upper_bound(sn));
        }
    }

//    for(cStringLongIterator in = m_mPolymerTypes.begin(); in != m_mPolymerTypes.end(); in++)
//    {
//        const zString polymerName =  in->first;
//        long polymerType =  in->second;
//        std::cout << "Polymer types map member " << polymerName << "  " << polymerType << zEndl;
//    }
	// Input data has been checked so allow the initialisation to proceed.

	return true;
}

