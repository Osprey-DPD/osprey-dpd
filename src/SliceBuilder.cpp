/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// SliceBuilder.cpp: implementation of the CSliceBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SliceBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"

#include "CNTCell.h"			// needed for random number generator


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSliceBuilder::CSliceBuilder(long slabTotal, long xn, long yn, long zn,  
                             zDoubleVector vWidths, LongStringMMap mmPolymers, 
                             StringLongMap mPolymerTypes) : m_SlabTotal(slabTotal),
											  		        m_XN(xn), m_YN(yn), m_ZN(zn),
                                                            m_vWidths(vWidths),
                                                            m_mmPolymers(mmPolymers),
                                                            m_mPolymerTypes(mPolymerTypes),
                                                            m_SimBoxWidth(0.0),
                                                            m_WallWidth(0.0)
{

    // Debug output

    /*
    std::cout << "SliceBuilder:" << zEndl;
    std::cout << "Slab total = " << m_SlabTotal << zEndl;
    std::cout << "Normal     = " << m_XN << " " << m_YN << " " << m_ZN << zEndl;
    std::cout << "Widths: ";
    for(long i=0; i<m_SlabTotal; i++)
    {
        std::cout << m_vWidths.at(i) << " ";
    }
    std::cout << zEndl;

    // Now the polymers belonging to each slab

    for(long sn = 0; sn < m_SlabTotal; sn++)
    {
        std::cout << "Slab " << sn << zEndl;
        cLongStringMMIterator imm=m_mmPolymers.find(sn);

        if(imm != m_mmPolymers.end())
        {
            do 
            {
                const zString polymerName =  imm->second;
		long polymerType = (*m_mPolymerTypes.find(polymerName)).second;

                std::cout << " " << polymerName << " (type " << polymerType << ")";

                imm++;
            }
            while (imm != m_mmPolymers.upper_bound(sn));
        }
        std::cout << zEndl;
    }
    */

}

CSliceBuilder::~CSliceBuilder()
{

}


// Virtual function to divide the simulation box into a series or rectangular
// slabs, and assemble the desired polymer types in each slab.
// 
// The CInitialState object is responsible for creating the beads, bonds and
// polymers from the types defined by the CInputData object, via the function
// CreatePolymers(). This builder sets the coordinates of the beads so
// as to create the required slab configuration. The polymers that fill the slabs
// have already been created using a bulk concentration specified by the user. 
//
// The slab normal sets the direction of the slabs, and their widths are supplied
// by the user. The polymer types to be distributed within each slab are also
// user-supplied. We first calculate the total number of polymers of all types
// from the polymer number fractions. Then we iterate over the slabs calculating 
// the fraction of each polymer type that is present in the slab from the ratio
// of the slab width to the simulation box side length. As the slab widths add up 
// to unity, the total number of polymers of each type assigned to the slabs matches
// that specified.

bool CSliceBuilder::Assemble(CInitialState &riState)
{
	// ****************************************
	// Position the wall beads first because they cannot move and may be 
	// bonded to beads in the bulk whose positions can be arranged so 
	// that their bonds are not too large initially. 

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}

    // Store the SimBox and wall widths in the direction of the slab normal 
    // for later use in defining excluded regions. If no wall is present, the
    // returned value for its width is zero and the bulk SimBox length is equal
    // to the actual length.

	if(m_XN == 1)
	{
        m_SimBoxWidth = riState.GetSimBoxBulkXLength();
        m_WallWidth   = riState.GetWallXWidth();
	}
	else if(m_YN == 1)
	{
        m_SimBoxWidth = riState.GetSimBoxBulkYLength();
        m_WallWidth   = riState.GetWallYWidth();
	}
	else if(m_ZN == 1)
	{
        m_SimBoxWidth = riState.GetSimBoxBulkZLength();
        m_WallWidth   = riState.GetWallZWidth();
	}

//    std::cout << "SimBox and wall widths = " << m_SimBoxWidth << "  " << m_WallWidth << zEndl;


    // Iterate over each polymer type distributing each polymer among all the allowed
    // slabs for that type. The set of excluded regions for each polymer type is just
    // the set of slabs that do NOT contain that polymer type: and this is usually
    // different for different polymer types.  Note that the CBuilder
    // class takes of ensuring that polymers are approximately uniformly distributed
    // within the available regions.

	zDoubleVector exRegions;

    for(cStringLongIterator in = m_mPolymerTypes.begin(); in != m_mPolymerTypes.end(); in++)
    {
        const zString polymerName =  in->first;
        long polymerType =  in->second;

        exRegions.clear();          // Empty the container for the next polymer
	exRegions.push_back(0.0);   // and exclude the wall if present
        exRegions.push_back(m_WallWidth);

        // Now add excluded regions for all slabs that do NOT have the given 
        // polymer type in them. Note that this does not depend on the normal vector,
        // but the slab widths must be converted from fractions to values in units
        // of the bead diameter.

        double runningSlabWidth = 0.0;
        double slabStart        = 0.0;
        double slabEnd          = 0.0;

        for(long sn = 0; sn < m_SlabTotal; sn++)
        {
            slabStart = runningSlabWidth;
            slabEnd   = slabStart + m_vWidths.at(sn)*m_SimBoxWidth;
            runningSlabWidth = slabEnd;

            cLongStringMMIterator imm=m_mmPolymers.find(sn);

            bool bFound = false;
            if(imm != m_mmPolymers.end())
            {
                do 
                {
                    const zString testName =  imm->second;

                    if(testName == polymerName)
                    {
                        bFound = true;  // polymer exists in this slab so it is not excluded
                    }
                    imm++;
                }
                while (!bFound && imm != m_mmPolymers.upper_bound(sn));

                if(!bFound)
                {
		            exRegions.push_back(slabStart);
		            exRegions.push_back(slabEnd);
                }
            }
            else
            {
                // An empty slab is excluded just as if the polymer had not been found in it.
		        exRegions.push_back(slabStart);
		        exRegions.push_back(slabEnd);
            }
        }

        // End with the opposite wall if it exists

        exRegions.push_back(m_SimBoxWidth - m_WallWidth);
	exRegions.push_back(m_SimBoxWidth);

	// Fill the slabs with randomly-positioned polymers

	isRandomPolymerSlabs random(polymerType, exRegions, m_XN, m_YN, m_ZN);

	random.Assemble(riState);
    }

	// Copy the coordinates of all beads into the other bead arrays used to ensure
	// that analysis of diffusion etc, is calculated from the initial bead positions.

	AssignUnPBCCoords(riState);

	// ****************************************
	// Set the in-plane velocities of the beads from a 3d Maxwell distribution 
	// with a mean  value and variance fixed from the temperature. This applies 
	// to all beads not just the ones in the vesicle. We indicate that a 3d
	// distribution is required by passing in a zero normal vector.

	AssignMaxwellVelocities(riState, 0, 0, 0);

	return true;
}
