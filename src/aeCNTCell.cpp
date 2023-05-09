/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeCNTCell.cpp: implementation of the aeCNTCell class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimMathFlags.h"
#include "aeCNTCell.h"
#include "aeActiveBond.h"

#include <random>


//////////////////////////////////////////////////////////////////////
// Static member variable and function definitions
//////////////////////////////////////////////////////////////////////

long   aeCNTCell::m_CNTXCellNo			= 0;
long   aeCNTCell::m_CNTYCellNo			= 0;
long   aeCNTCell::m_CNTZCellNo			= 0;
double aeCNTCell::m_CNTXCellWidth		= 0.0;
double aeCNTCell::m_CNTYCellWidth		= 0.0;
double aeCNTCell::m_CNTZCellWidth		= 0.0;

// Static function to define the number and sizes of the active network's CNT cells.
//
// Note the default values of 0 until a call to this function is made.

void aeCNTCell::SetSimBoxLengths(const long nx, const long ny, const long nz,
							     const double cntlx, const double cntly, const double cntlz)
{
	aeCNTCell::m_CNTXCellNo			= nx;
	aeCNTCell::m_CNTYCellNo			= ny;
	aeCNTCell::m_CNTZCellNo			= nz;
	aeCNTCell::m_CNTXCellWidth		= cntlx;
	aeCNTCell::m_CNTYCellWidth		= cntly;
	aeCNTCell::m_CNTZCellWidth		= cntlz;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aeCNTCell::aeCNTCell(long index, long coords[3], bool flag)
{
	// Initialise the member variables of CAbstractCell 

	m_id			= index;

	m_BLIndex[0]	= coords[0];
	m_BLIndex[1]	= coords[1];
	m_BLIndex[2]	= coords[2];

	m_BLCoord[0]	= static_cast<double>(coords[0])*m_CNTXCellWidth;
	m_BLCoord[1]	= static_cast<double>(coords[1])*m_CNTYCellWidth;
	m_BLCoord[2]	= static_cast<double>(coords[2])*m_CNTZCellWidth;

	m_TRCoord[0]	= m_BLCoord[0] + m_CNTXCellWidth;
	m_TRCoord[1]	= m_BLCoord[1] + m_CNTYCellWidth;
	m_TRCoord[2]	= m_BLCoord[2] + m_CNTZCellWidth;

	// Now the CCNTCell member variables

	m_bExternal		= flag;

	for(short int i=0; i<27; i++)	// We allow 3d values for 2d simulations to avoid
	{								// having to include SimDefs.h in the .h file
		m_aNNCells[i] = 0;
	}
	for(short int j=0; j<13; j++)
	{
		m_aIntNNCells[j] = 0;
	}
}

aeCNTCell::~aeCNTCell()
{

}

// Function to map the index of a nearest-neighbour cell to a pointer
// to the cell. Notice that the current cell is one of the possible 
// values.

void aeCNTCell::SetNNCellIndex(long index, aeCNTCell *pCell)
{

#if SimDimension == 2
	if( index >=0 && index < 9 )
#elif SimDimension == 3
	if( index >= 0 && index < 27 )
#endif
	{
// map cell index to cell pointer using an array
		m_aNNCells[index] = pCell;
// map cell index to cell pointer using STL
//		m_mNNCells.insert( make_pair(index, pCell) );
	}
	else
	{
		TraceInt("ERROR: Illegal index for NN cell mapping", index );
	}

}

// Function to store a pointer to a nearest-neighbour cell used in 
// calculating the interactions between active bonds. Because the forces are
// all pairwise we only have to loop over pairs once. Hence we store
// pointers to those cells that are to the right of, or above the current cell.

void aeCNTCell::SetIntNNCellIndex(long index, aeCNTCell* pCell)
{

#if SimDimension == 2
	if( index >=0 && index < 4 )
#elif SimDimension == 3
	if( index >= 0 && index < 13 )
#endif
	{
// map cell index to cell pointer using an array
		m_aIntNNCells[index] = pCell;
	}
	else
	{
		TraceInt("ERROR: Illegal index for NN cell mapping", index );
	}

}

// Function to add an active bond to the cell.

void aeCNTCell::AddActiveBondToCell(aeActiveBond *pBond)
{
	m_lBonds.push_front(pBond);
}

// Function to remove an active bond from the cell.

void aeCNTCell::RemoveActiveBondFromCell(aeActiveBond *pBond)
{
	m_lBonds.remove(pBond);
}

// Function to return a container of all active bonds in the cell.

ActiveBondList aeCNTCell::GetBonds()
{
	return m_lBonds;
}

// Function to calculate the total force on each free active bond due to all other 
// free active bonds. This does not include forces between monomers that have
// assembled into an active polymer, nor any interactions between free monomers 
// and polymers.

void aeCNTCell::UpdateForce()
{
	double dx[3];
	double dr, dr2;

	for(ActiveBondListIterator iterBond=m_lBonds.begin(); iterBond!=m_lBonds.end(); iterBond++)
	{
		// First add interactions between active bonds in the current cell. Note that
		// we don't have to check the PBCs here and we perform a reverse loop
		// over the neighbouring bonds until the iterators are equal. Because you can't
		// compare a forward and reverse iterator we compare the ids for
		// the terminating condition.

		for(rActiveBondListIterator riterBond2=m_lBonds.rbegin(); (*riterBond2)->GetId()!=(*iterBond)->GetId(); ++riterBond2 )
		{
			dx[0] = (*iterBond)->GetTailHeadBead()->GetXPos() - (*riterBond2)->GetTailHeadBead()->GetXPos();

			dx[1] = (*iterBond)->GetTailHeadBead()->GetYPos() - (*riterBond2)->GetTailHeadBead()->GetYPos();

#if SimDimension == 2
			dx[2] = 0.0;
#elif SimDimension == 3
			dx[2] = (*iterBond)->GetTailHeadBead()->GetZPos() - (*riterBond2)->GetTailHeadBead()->GetZPos();
#endif

			dr2 = dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

			if(dr2 < 4.0)
			{
				dr = sqrt(dr2);

				const double fx = -32.0*dx[0]/dr;
				const double fy = -32.0*dx[1]/dr;
				const double fz = -32.0*dx[2]/dr;

				(*iterBond)->GetTailHeadBead()->m_Force[0] += fx;
				(*iterBond)->GetTailHeadBead()->m_Force[1] += fy;
				(*iterBond)->GetTailHeadBead()->m_Force[2] += fz;

				(*riterBond2)->GetTailHeadBead()->m_Force[0] -= fx;
				(*riterBond2)->GetTailHeadBead()->m_Force[1] -= fy;
				(*riterBond2)->GetTailHeadBead()->m_Force[2] -= fz;

			}
		}
	} 

}

// Function to search the vicinity of an active bond (that may be free or at the end
// of an active polymer) to find another free active bond that can nucleate 
// a new active polymer.
//
// We check all free active bonds in the current cell and its nearest neighbours,
// applying the bond type-specific nucleation conditions given in the
// aeActiveBond::Activate() function. If activation succeeds, the target bond
// is returned so that it can be removed from the active bond list and
// a new active polymer created. We check that neither of the two bonds that
// are forming the polymer are phantoms: this test works even if the phantom
// bond feature is not turned on. We test the passed-in bond here and the second
// bond, which is found by searching the space around the first one, in the
// first bond's Activate() function.,

aeActiveBond* aeCNTCell::UpdateNucleation(aeActiveBond* const pBond)
{
	// We only check the current cell for now

	ActiveBondListIterator iterBond=m_lBonds.begin();

	aeActiveBond* pTargetBond = 0;

    if(!pBond->IsPolymerised() && !pBond->IsPhantom())
    {
	    while(!pTargetBond && iterBond!=m_lBonds.end())
	    {
		    if(pBond->Activate(*iterBond))
		    {
			    pTargetBond = *iterBond;
		    }

		    iterBond++;
	    }
    }

	return pTargetBond;
}

// Function to search the vicinity of an active bond (that must be the end
// of an active polymer) to find another free active bond that can attach to
// the polymer. Note that this function is not used to create new filaments.
// Each cell contains a number of actual CCNTCell cells, that is determined
// when the ACN is defined. This functions searchs the current cell and its
// nearest neighbours to find a free active bond to test for binding.

aeActiveBond* aeCNTCell::UpdatePolymerisation(aeActiveBond* const pBond)
{
    // Collect all bonds in the current cell and its nearest neighbours into
    // a local container, and iterate over them to find one potentially able
    // to bind to the end of the filament. We assume a 3d simulation so there 
    // are 27 cells including the current one. We use a vector so that we can
    // randomise the order using the STL algorithm random_shuffle().

    ActiveBondSequence vLocalBonds;
    vLocalBonds.clear();

    for(short int i=0; i<27; i++)
    {
        ActiveBondList lNNBonds = m_aNNCells[i]->GetBonds();
		copy(lNNBonds.begin(), lNNBonds.end(), back_inserter(vLocalBonds));
    }

	// Used shuffle, as random_shuffle deprecated in C++17
	// Shuffle is implementation defined, but so was random_shuffle
	std::minstd_rand rng(rand());
	shuffle(vLocalBonds.begin(), vLocalBonds.end(), rng);

    ActiveBondIterator iterBond=vLocalBonds.begin();
	aeActiveBond* pTargetBond = 0;

	while(!pTargetBond && iterBond!=vLocalBonds.end())
	{
		if(pBond->Activate(*iterBond))
		{
			pTargetBond = *iterBond;
		}

		iterBond++;
	}

	return pTargetBond;
}
