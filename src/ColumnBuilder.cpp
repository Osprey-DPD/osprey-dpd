/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ColumnBuilder.cpp: implementation of the CColumnBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "ColumnBuilder.h"
#include "InitialState.h"
#include "Polymer.h"
#include "Bead.h"
#include "Bond.h"

#include "CNTCell.h"			// needed for random number generator


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColumnBuilder::CColumnBuilder(long slabTotal, long xn, long yn, long zn,  
                             zDoubleVector vWidths, LongStringMMap mmPolymers, 
                             StringLongMap mPolymerTypes) : m_SlabTotal(slabTotal),
											  		        m_XN(xn), m_YN(yn), m_ZN(zn),
                                                            m_vWidths(vWidths),
                                                            m_mmPolymers(mmPolymers),
                                                            m_mPolymerTypes(mPolymerTypes)
{

    // Debug output

    std::cout << "ColumnBuilder:" << zEndl;
    std::cout << "Slab total = " << m_SlabTotal << zEndl;
    std::cout << "Normal     = " << m_XN << " " << m_YN << " " << m_ZN << zEndl;

    std::cout << "Widths: ";
    for(long i=0; i<m_SlabTotal; i++)
    {
        std::cout << m_vWidths.at(i) << " ";
    }
    std::cout << zEndl;

    // Now the polymers bwlonging to each slab

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

}

CColumnBuilder::~CColumnBuilder()
{

}


// Virtual function to divide the simulation box into a series of rectangular
// columns, and assemble the desired polymer types in each column.
// 
// The CInitialState object is responsible for creating the beads, bonds and
// polymers from the types defined by the CInputData object, via the function
// CreatePolymers(). This builder sets the coordinates of the beads so
// as to create the required slab configuration. The polymers that fill the slabs
// have already been created using a bulk concentration specified by the user. 
//

bool CColumnBuilder::Assemble(CInitialState &riState)
{
	// ****************************************
	// Position the wall beads first because they cannot move and may be 
	// bonded to beads in the bulk whose positions can be arranged so 
	// that their bonds are not too large initially.

	if(riState.IsWallPresent())
	{
		AssignWallBeadCoords(riState);
	}


	// ****************************************
	// Set the in-plane velocities of the beads from a 3d Maxwell distribution 
	// with a mean  value and variance fixed from the temperature. This applies 
	// to all beads not just the ones in the vesicle. We indicate that a 3d
	// distribution is required by passing in a zero normal vector.

	AssignMaxwellVelocities(riState, 0, 0, 0);

	return true;
}
