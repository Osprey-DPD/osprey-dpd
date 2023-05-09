/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Builder.cpp: implementation of the CBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimStdlibFlags.h"
#include "Builder.h"
#include "InitialState.h"
#include "Bead.h"
#include "Bond.h"
#include "Polymer.h"
#include "pmInteger.h"          // Needed to normalise initial bead momenta for parallel runs
#include "pmDouble.h"
#include "pmDouble3d.h"

#include "CNTCell.h"			// needed for random number generator
#include "LogState.h"
#include "LogBuilderError.h"
#include "LogVelDistMessage.h"	// needed to output the initial velocity distribution

#include <random>


// STL using declarations



// Uncomment the next flag to recreate the previous (wrong) normalisation for the
// initial random velocities.

// #define PreVelocity3dNormalisation 1



//////////////////////////////////////////////////////////////////////
// Static variable definitions
//////////////////////////////////////////////////////////////////////

// This is a wrapper to allow compilation on C++17 where std::random_shuffle has been removed.
// The shuffle produced will be different to random_shuffle, but that order is implementation
// defined and may not even be stable between runs.

// This is shared. If the Builder code becomes multi-threaded then there
// is a potential issue.
static std::mt19937_64 g_ShuffleRng;

template<class T>
static void random_shuffle_wrapper(T begin, T end)
{
	std::shuffle(begin, end, g_ShuffleRng);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Each builder object must be constructed with the data needed for it
// to know how to create its initial state. A random state has no such
// data but a restart state needs the run id and state id from the previous
// run and other builders may need extra data. Given this data, the builders
// assemble the beads, bonds and polymers defined in the CInitialState object
// into the configuration appropriate to their kind.
//
// The m_CoordErrorLimit is used to ensure that wall beads are positioned
// within the wall volume and do not lie exactly on the SimBox boundaries.

CBuilder::CBuilder() : m_CoordErrorLimit(0.0001), m_MaxwellPointNo(10000000)
{
}

CBuilder::~CBuilder()
{
}

// Function to calculate a set of velocities taken from a Maxwell distribution
// with unit mean and variance. It takes the number of points as arguments and
// returns a vector holding the velocities. Note that this gives only the 
// magnitude of the velocities not their directions and that there may be
// a significant CM motion in the set.
//
// The algorithm generates points from the distribution but the last point
// will be infinite because "factor" takes the value of "dmaxPoints". Hence
// we set the value of the n-1 element equal to that of the n-2. We do not
// expect this to be significant when selecting points randomly from the set.
//
// We cannot return a const reference because the vector is local to this routine.

zDoubleVector CBuilder::MaxwellDist(long n) const
{
	long maxPoints;

	if(n > m_MaxwellPointNo)
		maxPoints = n;
	else
		maxPoints = m_MaxwellPointNo;

	zDoubleVector velDist(maxPoints, 0.0);

	double vel;
	double factor = 1.0;
	double dmaxPoints = static_cast<double>(maxPoints);

	for(long j=0; j<maxPoints-1; j++)
	{
		vel = sqrt(2.0*log(dmaxPoints*factor/(dmaxPoints-factor)));
		factor = exp(0.5*vel*vel);
		velDist.at(j) = vel;
	}

	velDist.at(maxPoints-1) = velDist.at(maxPoints-2);

	return velDist;
}

// Function to position the wall beads randomly within the wall volume. Because
// the wall bead is always the head of a polymer we can access it using the 
// CPolymer's GetHead() member function.
//
// Note that the wall width parameters serve two purposes: if they are zero they
// indicate that there is no wall in that direction, while if they are non-zero
// they limit the bead coordinate in that dimension to lie in the wall width.
// This means that a wall thickness of zero does not limit the bead coordinates
// in the dimension in which it is zero. Hence, it also means that if the wall
// thickness is zero in all three dimensions the wall will fill the SimBox. But
// this condition is explicitly excluded here.
//
// We use the constant m_CoordErrorLimit to ensure that the wall bead coordinates
// lie within the volume of the wall and are not exactly on the boundaries. When 
// reading a restart state numbers like 19.999968 round to 20 and cause an error.
// The wall bead coordinates are assigned using the scheme:

void CBuilder::AssignWallBeadCoords(CInitialState& riState) const
{    
    const long lx = riState.GetCNTXCellNo();
    const long ly = riState.GetCNTYCellNo();
    const long lz = riState.GetCNTZCellNo();

    const long dx = riState.GetWallXCellNo();
    const long dy = riState.GetWallYCellNo();
    const long dz = riState.GetWallZCellNo();

    // Only assign wall bead coordinates if there is at least one wall;
    // this allows us to assume that not all of dx,dy,dz can be zero
    // when collecting the indices of wall cells.

    if(riState.IsWallPresent())
    {
        zLongVector vWallCells;  // 1d vector of indices pointing to wall cells
        vWallCells.clear();

        long index; // counter used here and below

	    for(long k=0; k<lz; k++)
	    {
		    for(long j=0; j<ly; j++)
		    {
			    for(long i=0; i<lx; i++)
			    {
				    index = lx*(ly*k+j) + i;

                    // If this cell lies within the wall volume store its index

                    if((dx > 0 && (i < dx || (lx - dx) <= i)) ||
                       (dy > 0 && (j < dy || (ly - dy) <= j)) ||
                       (dz > 0 && (k < dz || (lz - dz) <= k)))
                    {
                        vWallCells.push_back(index);
                    }
			    }
		    }
	    }

        // Now we have the set of CNT cells in which wall beads must be placed, 
        // we iterate over the wall polymers and assign each one coordinates that
        // are randomly chosen within the wall cells. If the new coordinates are
        // within a distance m_CoordErrorLimit of the SimBox boundaries we move
        // them inwards slightly. This prevents a restart state from rounding
        // the coordinates to values outside the SimBox range.

        const double slx = riState.GetSimBoxXLength() - m_CoordErrorLimit;
        const double sly = riState.GetSimBoxYLength() - m_CoordErrorLimit;
        const double slz = riState.GetSimBoxZLength() - m_CoordErrorLimit;

        const double cx = riState.GetCNTXCellWidth();
        const double cy = riState.GetCNTYCellWidth();
        const double cz = riState.GetCNTZCellWidth();

        long x, y, z;      // Indices of a CNT cell within the wall volume
        double xp[3];      // Bead coordinates

        unsigned long counter = 0;

        for(cPolymerVectorIterator iterPolymer=riState.GetWallPolymers().begin(); iterPolymer!=riState.GetWallPolymers().end(); iterPolymer++)
        {
            if(counter == vWallCells.size())
                counter = 0;

            index = vWallCells.at(counter++);

            x = index % lx;
            y = ((index - x)/lx) % ly;
            z = ((index - x)/lx - y)/ly;

            xp[0] = cx*(static_cast<double>(x) + CCNTCell::GetRandomNo());
            xp[1] = cy*(static_cast<double>(y) + CCNTCell::GetRandomNo());
            xp[2] = cz*(static_cast<double>(z) + CCNTCell::GetRandomNo());

            // Ensure that the coordinates are not within m_CoordErrorLimit of the 
            // SimBox boundaries

            if(xp[0] < m_CoordErrorLimit) {
                xp[0] = m_CoordErrorLimit; }
            else if(xp[0] > slx) {
                xp[0] = slx; }

            if(xp[1] < m_CoordErrorLimit) {
                xp[1] = m_CoordErrorLimit; }
            else if(xp[1] > sly) {
                xp[1] = sly; }

            if(xp[2] < m_CoordErrorLimit) {
                xp[2] = m_CoordErrorLimit; }
            else if(xp[2] > slz) {
                xp[2] = slz; }

	        (*iterPolymer)->GetHead()->SetXPos(xp[0]);
	        (*iterPolymer)->GetHead()->SetYPos(xp[1]);
	        (*iterPolymer)->GetHead()->SetZPos(xp[2]);

    // Copy initial coordinates into un-PBC coordinates array

            (*iterPolymer)->GetHead()->SetunPBCXPos(xp[0]);
            (*iterPolymer)->GetHead()->SetunPBCYPos(xp[1]);
            (*iterPolymer)->GetHead()->SetunPBCZPos(xp[2]);

    // and the initial position array

	        (*iterPolymer)->GetHead()->SetInitialXPos(xp[0]);
	        (*iterPolymer)->GetHead()->SetInitialYPos(xp[1]);
	        (*iterPolymer)->GetHead()->SetInitialZPos(xp[2]);

    // Set the initial momenta to zero: these must remain zero throughout the simulation

		    (*iterPolymer)->GetHead()->SetXMom(0.0);
		    (*iterPolymer)->GetHead()->SetYMom(0.0);
		    (*iterPolymer)->GetHead()->SetZMom(0.0);
        }
    }
}

// **********************************************************************
// Member functions for enclosed class: isRandom

// This object arranges the specified polymers randomly throughout planar regions
// of the SimBox. A series of excluded regions can be specified to take account of
// the walls and any bilayers that have been created. It only handles planar
// regions whose normals lie along the Cartesian axes.
//
// This routine does not check that the bilayer does not overlap any other structure
// including the walls. It assumes that this has been done by the calling routine.

CBuilder::isRandom::isRandom(const zLongVector polymerTypes, const zDoubleVector exRegions,
							 long Xn, long Yn, long Zn) : m_PolymerTypes(polymerTypes),
														  m_ExcludedRegions(exRegions),
														  m_ExcludedRegionTotal(exRegions.size()/2),
														  m_FreeRegionTotal(m_ExcludedRegionTotal-1),
													      m_X(Xn), m_Y(Yn), m_Z(Zn),
														  m_PolymerTotal(0)
{
	m_vRandom.clear();
}

CBuilder::isRandom::~isRandom()
{
}

bool CBuilder::isRandom::Assemble(CInitialState& riState)
{
	// Copy the polymers in the CInitialState into a local vector for ease of access.
	// We loop over all types of polymer adding them to a local vector if
	// their type occurs in the data for a random region.

	PolymerVector randomPolymers = riState.GetPolymers();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		lastPolymer += riState.GetPolymerTotalForType(ip);

		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) != m_PolymerTypes.end())
		{

			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(randomPolymers.begin()+firstPolymer, randomPolymers.begin()+lastPolymer, back_inserter(m_vRandom));
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	m_PolymerTotal = m_vRandom.size();	// Total of all polymer types not in bilayer

	// ****************************************
	// Now fill the spaces between the membrane and the walls randomly
	// with the remaining polymers. For each polymer not of the type of those
	// in the bilayer we loop over its beads and place them one side or the 
	// other of the membrane. We also check that bonded beads are not so far
	// apart that they generate large bond forces. But we only do this for 
	// the non-bilayer polymers. The user specifies how stretched the bilayer
	// polymers are using the m_Thickness parameter and if they set a value 
	// that causes the bonds to be stretched that is a choice we allow.

	// Notice that we assign beads to the free regions according to the regions'
	// length in the direction of their normal. This distributes beads with 
	// an appoximately uniform density, instead of simply assigning them to
	// a randomly chosen region which results in too many beads in thin regions.
	// To do this, we find the total length of the free regions and then store
	// the ratio of each region's length to the total length. This defines the
	// probability for a bead to be added to the region. We then compile a
	// cumulative probability distribution for the regions and use a random
	// number uniformly distributed between zero and one to select each region
	// according to its relative length.

	double xp[3];				// Coordinates to assign to a bead
	xp[0]        = 0.0;
	xp[1]        = 0.0;
	xp[2]        = 0.0;
	unsigned long index   = 0;	// counter used several times below
	double width = 0.0;			// region width used several times below
	double freeLength = 0.0;	// total width of all free regions

	// Get the total length of the free regions. Note that the size of the
	// m_ExcludedRegions container must be even.

	for(index=1; index<m_ExcludedRegions.size()/2; index++)
	{
		width = m_ExcludedRegions.at(2*index) - m_ExcludedRegions.at(2*index-1);
		freeLength += width;
	}

	zDoubleVector freeRegionRatio;	// Cumulative occupation prob for each region

	double cumulativeProb = 0.0;

	for(index=1; index<m_ExcludedRegions.size()/2; index++)
	{
		width = m_ExcludedRegions.at(2*index) - m_ExcludedRegions.at(2*index-1);

		const double prob = width/freeLength;

		cumulativeProb += prob;

		freeRegionRatio.push_back(cumulativeProb);
	}

	// Assign the beads to the free regions in proportion to the widths of 
	// the regions. This results in an approximately uniform density.
	// All beads in a given polymer must be in the same free region and not
	// straddle the membrane. I moved the selection of the actual free region into
    // the outer polymer loop not the inner bead loop as the latter only works for
    // polymers containing a single bead.

	double rindex     = 0.0;
	bool bNotAssigned = true;

	for(cPolymerVectorIterator iterPoly=riState.GetPolymers().begin(); iterPoly!=riState.GetPolymers().end(); iterPoly++)
	{
		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), (*iterPoly)->GetType()) != m_PolymerTypes.end())
		{
			// The old index just used the number of free regions to allocate
			// the beads. The new algorithm uses the relative widths of 
			// the regions to assign an index to each bead.

			//	index = ((*iterBead)->GetId()%m_FreeRegionTotal) + 1;

			rindex = CCNTCell::GetRandomNo();
			bNotAssigned = true;

			for(unsigned long nindex=0; nindex<freeRegionRatio.size(); nindex++)
			{
				if(bNotAssigned && rindex <= freeRegionRatio.at(nindex))
				{
					bNotAssigned = false;
					index = nindex;
				}
			}

			if(bNotAssigned)
				index = 0;

			width = m_ExcludedRegions.at(2*index+2) - m_ExcludedRegions.at(2*index+1);

			for(cBeadVectorIterator iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
			{
				if(m_X == 1)
				{
					xp[0] = m_ExcludedRegions.at(2*index+1) + width*CCNTCell::GetRandomNo();

					xp[1] = 0.95*riState.GetSimBoxYLength()*CCNTCell::GetRandomNo();
					xp[2] = 0.95*riState.GetSimBoxZLength()*CCNTCell::GetRandomNo();
				}
				else if(m_Y == 1)
				{
					xp[0] = 0.95*riState.GetSimBoxXLength()*CCNTCell::GetRandomNo();

					xp[1] = m_ExcludedRegions.at(2*index+1) + width*CCNTCell::GetRandomNo();

					xp[2] = 0.95*riState.GetSimBoxZLength()*CCNTCell::GetRandomNo();
				}
				else if(m_Z == 1)
				{
					xp[0] = 0.95*riState.GetSimBoxXLength()*CCNTCell::GetRandomNo();
					xp[1] = 0.95*riState.GetSimBoxYLength()*CCNTCell::GetRandomNo();
						
					xp[2] = m_ExcludedRegions.at(2*index+1) + width*CCNTCell::GetRandomNo();
				}

				(*iterBead)->SetXPos(xp[0]);
				(*iterBead)->SetYPos(xp[1]);
				(*iterBead)->SetZPos(xp[2]);
			}

			// Check that pairs of beads that are bonded together are not too far apart 
			// that they generate huge bond forces. This arises because their positions 
			// are just randomly selected from the free space and for boxes of 
			// 20 x 20 x 20 or more this can lead to large separations. If they are 
			// further apart than "rbond", move them along the line joining their centres 
			// so that they are only unit distance apart.

			const double rbond = 1.0;
			double rmag;
			double rdiff[3], runit[3];

			for(cBondVectorIterator iterBond=(*iterPoly)->GetBonds().begin(); iterBond!= (*iterPoly)->GetBonds().end(); iterBond++)
			{
				rdiff[0] = (*iterBond)->GetTail()->GetXPos() - (*iterBond)->GetHead()->GetXPos();
				rdiff[1] = (*iterBond)->GetTail()->GetYPos() - (*iterBond)->GetHead()->GetYPos();
				rdiff[2] = (*iterBond)->GetTail()->GetZPos() - (*iterBond)->GetHead()->GetZPos();

				rmag = sqrt(rdiff[0]*rdiff[0] + rdiff[1]*rdiff[1] + rdiff[2]*rdiff[2]);

				runit[0] = rdiff[0]/rmag;
				runit[1] = rdiff[1]/rmag;
				runit[2] = rdiff[2]/rmag;

				if( fabs(rdiff[0]) > rbond )
				{
					(*iterBond)->GetTail()->SetXPos((*iterBond)->GetHead()->GetXPos()+runit[0]);
				}

				if( fabs(rdiff[1]) > rbond )
				{
					(*iterBond)->GetTail()->SetYPos((*iterBond)->GetHead()->GetYPos()+runit[1]);
				}

				if( fabs(rdiff[2]) > rbond )
				{
					(*iterBond)->GetTail()->SetZPos((*iterBond)->GetHead()->GetZPos()+runit[2]);
				}
			}
		}
	}

	return true;
}

// **********************************************************************
// Member functions for enclosed class: isRandomPolymerSlabs

// This object arranges a single polymer type randomly throughout parallel
// planar regions of the SimBox. A series of excluded regions is specified 
// to take account of the walls and other slabs that have been created. 
// It only handles planar regions whose normals lie along the Cartesian axes. 
// Note that each individual polymer is assmebled wholly within a single slab.

CBuilder::isRandomPolymerSlabs::isRandomPolymerSlabs(long polymerType, const zDoubleVector exRegions,
							 long Xn, long Yn, long Zn) : m_PolymerType(polymerType),
														  m_ExcludedRegions(exRegions),
														  m_ExcludedRegionTotal(exRegions.size()/2),
														  m_FreeRegionTotal(m_ExcludedRegionTotal-1),
													      m_X(Xn), m_Y(Yn), m_Z(Zn),
                                                          m_PolymerTotal(0)
{
    m_vRandom.clear();
}

CBuilder::isRandomPolymerSlabs::~isRandomPolymerSlabs()
{
}

bool CBuilder::isRandomPolymerSlabs::Assemble(CInitialState& riState)
{
	// Copy the polymers in the CInitialState into a local vector for ease of access.
	// We loop over all types of polymer adding the specified type to a local vector.

	PolymerVector randomPolymers = riState.GetPolymers();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		lastPolymer += riState.GetPolymerTotalForType(ip);

		if(ip == m_PolymerType)
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(randomPolymers.begin()+firstPolymer, randomPolymers.begin()+lastPolymer, back_inserter(m_vRandom));
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	m_PolymerTotal = m_vRandom.size();	// Total of all polymer types in this slab

	// ****************************************
	// Now fill the non-excluded slabs randomly with the specified polymers.
    // For each polymer, we loop over its beads and place all into one slab. 
    // We also check that bonded beads are not so far apart that they generate 
    // large bond forces. 

	// Notice that we assign beads to the free regions according to the regions'
	// length in the direction of their normal. This distributes beads with 
	// an appoximately uniform density, instead of simply assigning them to
	// a randomly chosen region which results in too many beads in thin regions.
	// To do this, we find the total length of the free regions and then store
	// the ratio of each region's length to the total length. This defines the
	// probability for a bead to be added to the region. We then compile a
	// cumulative probability distribution for the regions and use a random
	// number uniformly distributed between zero and one to select each region
	// according to its relative length.

	double xp[3];				// Coordinates to assign to a bead
	xp[0]        = 0.0;
	xp[1]        = 0.0;
	xp[2]        = 0.0;
	unsigned long index   = 0;	// counter used several times below
	double width = 0.0;			// region width used several times below
	double freeLength = 0.0;	// total width of all free regions

	// Get the total length of the free regions. Note that the size of the
	// m_ExcludedRegions container must be even.

	for(index=1; index<m_ExcludedRegions.size()/2; index++)
	{
		width = m_ExcludedRegions.at(2*index) - m_ExcludedRegions.at(2*index-1);
		freeLength += width;
	}

	zDoubleVector freeRegionRatio;	// Cumulative occupation prob for each region

	double cumulativeProb = 0.0;

	for(index=1; index<m_ExcludedRegions.size()/2; index++)
	{
		width = m_ExcludedRegions.at(2*index) - m_ExcludedRegions.at(2*index-1);

		const double prob = width/freeLength;

		cumulativeProb += prob;

		freeRegionRatio.push_back(cumulativeProb);
	}

	// Assign the beads to the free regions in proportion to the widths of 
	// the regions. This results in an approximately uniform density.
	// All beads in a given polymer must be in the same free region and not
	// straddle any excluded region.

	double rindex     = 0.0;
	bool bNotAssigned = true;

	for(cPolymerVectorIterator iterPoly=m_vRandom.begin(); iterPoly!=m_vRandom.end(); iterPoly++)
	{
		// The old index just used the number of free regions to allocate
		// the beads. The new algorithm uses the relative widths of 
		// the regions to assign an index to each bead.

		//	index = ((*iterBead)->GetId()%m_FreeRegionTotal) + 1;

		rindex = CCNTCell::GetRandomNo();
		bNotAssigned = true;

		for(unsigned long nindex=0; nindex<freeRegionRatio.size(); nindex++)
		{
			if(bNotAssigned && rindex <= freeRegionRatio.at(nindex))
			{
				bNotAssigned = false;
				index = nindex;
			}
		}

		if(bNotAssigned)
		{
		    std::cout << "Polymer " << (*iterPoly)->GetId() << " not assigned" << zEndl;
		    index = 0;
		}

		width = m_ExcludedRegions.at(2*index+2) - m_ExcludedRegions.at(2*index+1);

		for(cBeadVectorIterator iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
		{
			if(m_X == 1)
			{
				xp[0] = m_ExcludedRegions.at(2*index+1) + width*CCNTCell::GetRandomNo();
				xp[1] = 0.98*riState.GetSimBoxYLength()*CCNTCell::GetRandomNo();
				xp[2] = 0.98*riState.GetSimBoxZLength()*CCNTCell::GetRandomNo();
			}
			else if(m_Y == 1)
			{
				xp[0] = 0.98*riState.GetSimBoxXLength()*CCNTCell::GetRandomNo();
				xp[1] = m_ExcludedRegions.at(2*index+1) + width*CCNTCell::GetRandomNo();
				xp[2] = 0.98*riState.GetSimBoxZLength()*CCNTCell::GetRandomNo();
			}
			else if(m_Z == 1)
			{
				xp[0] = 0.98*riState.GetSimBoxXLength()*CCNTCell::GetRandomNo();
				xp[1] = 0.98*riState.GetSimBoxYLength()*CCNTCell::GetRandomNo();
					
				xp[2] = m_ExcludedRegions.at(2*index+1) + width*CCNTCell::GetRandomNo();
			}

			(*iterBead)->SetXPos(xp[0]);
			(*iterBead)->SetYPos(xp[1]);
			(*iterBead)->SetZPos(xp[2]);
		}

		// Check that pairs of beads that are bonded together are not too far apart 
		// that they generate huge bond forces. This arises because their positions 
		// are just randomly selected from the free space and for boxes of 
		// 20 x 20 x 20 or more this can lead to large separations. If they are 
		// further apart than "rbond", move them along the line joining their centres 
		// so that they are only unit distance apart.

		const double rbond = 1.0;
		double rmag;
		double rdiff[3], runit[3];

		for(cBondVectorIterator iterBond=(*iterPoly)->GetBonds().begin(); iterBond!= (*iterPoly)->GetBonds().end(); iterBond++)
		{
			rdiff[0] = (*iterBond)->GetTail()->GetXPos() - (*iterBond)->GetHead()->GetXPos();
			rdiff[1] = (*iterBond)->GetTail()->GetYPos() - (*iterBond)->GetHead()->GetYPos();
			rdiff[2] = (*iterBond)->GetTail()->GetZPos() - (*iterBond)->GetHead()->GetZPos();

			rmag = sqrt(rdiff[0]*rdiff[0] + rdiff[1]*rdiff[1] + rdiff[2]*rdiff[2]);

			runit[0] = rdiff[0]/rmag;
			runit[1] = rdiff[1]/rmag;
			runit[2] = rdiff[2]/rmag;

			if( fabs(rdiff[0]) > rbond )
			{
				(*iterBond)->GetTail()->SetXPos((*iterBond)->GetHead()->GetXPos()+runit[0]);
			}

			if( fabs(rdiff[1]) > rbond )
			{
				(*iterBond)->GetTail()->SetYPos((*iterBond)->GetHead()->GetYPos()+runit[1]);
			}

			if( fabs(rdiff[2]) > rbond )
			{
				(*iterBond)->GetTail()->SetZPos((*iterBond)->GetHead()->GetZPos()+runit[2]);
			}
		}
	}

	return true;
}

// **********************************************************************
// Member functions for enclosed class: isBilayer.

// This object assembles the selected polymers into a bilayer. The area of 
// the simulation box, orientation, centre and thickness of the bilayer are 
// arguments and the routine creates a hexagonal lattice with two polymers 
// per lattice site, one in each monolayer. If multiple polymer types are
// being used they are assigned coordinates sequentially. This results in
// different parts of the bilayer being occupied by each type of polymer.
// If a randomised distribution is required we can change the routine later.
//
// This routine does not check that the bilayer does not overlap any other structure
// including the walls. It assumes that this has been done by the calling routine.

CBuilder::isBilayer::isBilayer(const zLongVector& polymerTypes, const zDoubleVector& upperFractions,
                               long Xn, long Yn, long Zn,
		                double centre, double thickness, 
			        double area, double length1, double length2,
				double upperHead, double lowerHead, bool bLinear) : m_PolymerTypes(polymerTypes), 
					                                           m_UpperFractions(upperFractions),
										     m_X(Xn), m_Y(Yn), m_Z(Zn),
										     m_BilayerCentre(centre),
									             m_Thickness(thickness),
										     m_BilayerArea(area),
										     m_Bilayer1Length(length1),
										     m_Bilayer2Length(length2),
										     m_UpperHead(upperHead),
										     m_LowerHead(lowerHead),
										     m_UpperTotal(0),
										     m_LowerTotal(0),
										     m_bMonolayer(false),
									             m_bLinearise(bLinear),
									             m_PolymerMaxSize(0)
{
	m_bPatches[0] = false;
	m_bPatches[1] = false;

	m_vBilayer.clear();
	m_PolymerTotal.clear();
	m_PolymerUpperTotal.clear();
	m_PolymerSize.clear();
	m_RandomUpperIndex.clear();
	m_RandomLowerIndex.clear();
	m_UpperXCoord.clear();
	m_UpperYCoord.clear();
	m_LowerXCoord.clear();
	m_LowerYCoord.clear();

	m_MonolayerSign[0]	= 1.0;		// Sign need to assign correct coordinates to
	m_MonolayerSign[1]	= -1.0;		// beads in polymers in the upper/lower monolayers

	for(short i=0; i<2; i++)
	{
		m_MonolayerHead[i]	= 0.0;
		m_MonolayerTotal[i]	= 0;
		m_LatticeSize[i]	= 0;		
		m_HexSpace[i]		= 0.0;		
		m_HexCAxisSpace[i]	= 0.0;		
		m_Hexe1X[i]			= 0.0;
		m_Hexe1Y[i]			= 0.0;
		m_Hexe2X[i]			= 0.0;	
		m_Hexe2Y[i]			= 0.0;
	}

}

// Constructor for use when it is known whether a bilayer or monolayer is being
// created.

CBuilder::isBilayer::isBilayer(const zLongVector& polymerTypes, const zDoubleVector& upperFractions,
							   long Xn, long Yn, long Zn, double centre, double thickness, 
							   double area, double length1, double length2,
							   double upperHead, double lowerHead, long upperTotal, long lowerTotal,
							   bool bMonolayer, bool bLinear,
							   const bool bPatches[2]) : m_PolymerTypes(polymerTypes),
													   m_UpperFractions(upperFractions),
													   m_X(Xn), m_Y(Yn), m_Z(Zn),
													   m_BilayerCentre(centre),
													   m_Thickness(thickness),
													   m_BilayerArea(area),
													   m_Bilayer1Length(length1),
													   m_Bilayer2Length(length2),
													   m_UpperHead(upperHead),
													   m_LowerHead(lowerHead),
													   m_UpperTotal(upperTotal),
													   m_LowerTotal(lowerTotal),
													   m_bMonolayer(bMonolayer),
													   m_bLinearise(bLinear),
													   m_PolymerMaxSize(0)
{
	m_bPatches[0] = bPatches[0];
	m_bPatches[1] = bPatches[1];

	m_vBilayer.clear();
	m_PolymerTotal.clear();
	m_PolymerUpperTotal.clear();
	m_PolymerSize.clear();
	m_RandomUpperIndex.clear();
	m_RandomLowerIndex.clear();
	m_UpperXCoord.clear();
	m_UpperYCoord.clear();
	m_LowerXCoord.clear();
	m_LowerYCoord.clear();

	m_MonolayerSign[0]	= 1.0;		// Sign need to assign correct coordinates to
	m_MonolayerSign[1]	= -1.0;		// beads in polymers in the upper/lower monolayers

	if(m_UpperTotal == 0)
	{
		m_MonolayerTotal[0]	= 0;
		m_MonolayerTotal[1]	= m_LowerTotal;
		m_LatticeSize[0]	= 0;
		m_LatticeSize[1]	= m_LowerTotal;
		m_MonolayerHead[0]	= 0.0;
		m_MonolayerHead[1]	= m_LowerHead;
	}
	else if(m_LowerTotal == 0)
	{
		m_MonolayerTotal[0]	= m_UpperTotal;
		m_MonolayerTotal[1]	= 0;
		m_LatticeSize[0]	= m_UpperTotal;
		m_LatticeSize[1]	= 0;
		m_MonolayerHead[0]	= m_UpperHead;
		m_MonolayerHead[1]	= 0.0;
	}
	else
	{
		m_MonolayerTotal[0]	= m_UpperTotal;
		m_MonolayerTotal[1]	= m_LowerTotal;
		m_LatticeSize[0]	= m_UpperTotal;
		m_LatticeSize[1]	= m_LowerTotal;
		m_MonolayerHead[0]	= m_UpperHead;
		m_MonolayerHead[1]	= m_LowerHead;
	}

	for(short i=0; i<2; i++)
	{
		m_HexSpace[i]		= 0.0;		
		m_HexCAxisSpace[i]	= 0.0;		
		m_Hexe1X[i]			= 0.0;
		m_Hexe1Y[i]			= 0.0;
		m_Hexe2X[i]			= 0.0;	
		m_Hexe2Y[i]			= 0.0;
	}
}

CBuilder::isBilayer::~isBilayer()
{
}

bool CBuilder::isBilayer::Assemble(CInitialState& riState)
{
	// Copy the polymers in the CInitialState into a local vector for ease of access.
	// We loop over all types of polymer adding them to the vector if
	// their type occurs in the data for a composite bilayer. We also
	// store the numbers of each type of polymer in the upper monolayer, 
	// and the number of beads in the polymer types in the bilayer. 

	PolymerVector bilayerPolymers = riState.GetPolymers();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		long polyNo  = riState.GetPolymerTotalForType(ip);

		lastPolymer += polyNo;

		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) != m_PolymerTypes.end())
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(bilayerPolymers.begin()+firstPolymer, bilayerPolymers.begin()+lastPolymer, back_inserter(m_vBilayer));

			// Store the number of this type of polymer and the number in the
			// upper monolayer

			m_PolymerTotal.push_back(polyNo);


			for(unsigned long polyIndex=0; polyIndex<m_PolymerTypes.size(); polyIndex++)
			{
				if(m_PolymerTypes.at(polyIndex) == ip)
				{
					m_PolymerUpperTotal.push_back(static_cast<long>(m_UpperFractions.at(polyIndex)*polyNo));
				}
			}

			// For linear and branched polymers we count the number of beads between 
			// the head and tail (inclusive) and use this to define the spacing in 
			// the C direction. The spacing is defined as the monolayer thickness
			// divided by m_PolymerSize-1. 
			// We don't know whether the head or tail will be encountered first in the 
			// polymer string so we set a flag, bCount, to indicate when we should count
			// beads. It is reset to null when both head and tail have been counted.
			// The other beads in the polymer are arranged randomly around the head-tail
			// axis.
			//
			// NOTE. This algorithm won't work if the tail bead is found first in the
			// polymer shape string. It will arrange the beads as if the head were first,
			// resulting in the beads between the head and tail having their order
			// reversed.

			const BeadVector&  vBeads = m_vBilayer.back()->GetBeads();
			CBead*		pHead = dynamic_cast<CBead*>(m_vBilayer.back()->GetHead());
			CBead*		pTail = dynamic_cast<CBead*>(m_vBilayer.back()->GetTail());

			CBead* pEnd		= NULL;
			long counter	= 0;
			bool bCount		= false;

			long dummyCount = 0;

			for(cBeadVectorIterator pBead=vBeads.begin();pBead!=vBeads.end(); pBead++)
			{
				dummyCount++;

				if(*pBead == pHead && !bCount)
				{
					bCount = true;
					pEnd   = pTail;
					counter++;
				}
				else if( *pBead == pTail && !bCount)
				{
					bCount = true;
					pEnd   = pHead;
					counter++;
				}
				else if(*pBead == pEnd)
				{
					bCount = false;
					counter++;
				}
				else if(bCount)
				{
					counter++;
				}
			}
			m_PolymerSize.push_back(counter);
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	m_PolymerMaxSize = *max_element(m_PolymerSize.begin(), m_PolymerSize.end());

	// Set the C axis spacing for the beads in the polymers. We use
	// the size of the largest polymer (between its head and tail) to set the
	// spacing for all polymers. This allows shorter ones to be easily 
	// arranged between longer ones without being stretched too far.

	m_HexCAxisSpace[0] = 0.5*m_Thickness/static_cast<double>(m_PolymerMaxSize-1);
	m_HexCAxisSpace[1] = m_HexCAxisSpace[0];

	// ****************************************
	// Calculate the lattice size for the hexagonal lattices that organise
	// the polymers in the two monolayer. Note that either monolayer may be
	// empty. We have to take into account orientation of the bilayer and the 
	// number of polymers in each  monolayer to calculate the spacing.
	// We shrink the lattice vectors by 0.999 and offset the first sites by 0.001 to
	// prevent any sites going outside the SimBox. This occurred with a box size
	// of 20**3 in the test run 11024, changelist 462.

	// Get unit vectors in e1 and e2 directions (NB. 1.1547 = 2/sqrt(3)) and the
	// number of vertices in the lattice.

	if(m_bMonolayer)
	{
		if(m_UpperTotal == 0)	// All polymers in lower monolayer
		{
			m_HexSpace[0] = 0.0;
			m_HexSpace[1] = 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[1]));
		}
		else
		{
			m_HexSpace[0] = 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[0]));
			m_HexSpace[1] = 0.0;
		}
	}
	else
	{
			m_HexSpace[0] = 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[0]));
			m_HexSpace[1] = 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[1]));
	}

	m_Hexe1X[0]	= m_HexSpace[0];			// e1 lattice vector
	m_Hexe2X[0]	= 0.5*m_HexSpace[0];		// e2 lattice vector
	m_Hexe2Y[0]	= 0.866025*m_HexSpace[0];

	m_Hexe1X[1]	= m_HexSpace[1];			// e1 lattice vector
	m_Hexe2X[1]	= 0.5*m_HexSpace[1];		// e2 lattice vector
	m_Hexe2Y[1]	= 0.866025*m_HexSpace[1];

	// ****************************************
	// Create the vectors holding indices to the coordinates of free lattice 
	// sites at which polymers may be placed. As polymers are positioned,
	// the corresponding sites are removed from the vectors allowing a 
	// simple random choice from the remaining elements. We calculate
	// both the upper and lower monolayer coordinates even if all the polymers
	// are in one monolayer.

	for(short int index = 0; index<2; index++)
	{
		double hexp[2];

		hexp[0] = 0.001*m_Hexe1X[index];	// We use an offset to avoid positions coinciding with boundaries
		hexp[1] = 0.001*m_Hexe2Y[index];

		bool bEvenRow = true;

		for(long icu=0; icu<m_LatticeSize[index]; icu++)
		{
			if(index == 0)
			{
				m_RandomUpperIndex.push_back(icu);
				m_UpperXCoord.push_back(hexp[0]);
				m_UpperYCoord.push_back(hexp[1]);
			}
			else
			{
				m_RandomLowerIndex.push_back(icu);
				m_LowerXCoord.push_back(hexp[0]);
				m_LowerYCoord.push_back(hexp[1]);
			}

			hexp[0] += m_Hexe1X[index];

			if(hexp[0] > m_Bilayer1Length)
			{
				if(bEvenRow)
					hexp[0] = m_Hexe2X[index];
				else
					hexp[0] = 0.0;

				hexp[1] += m_Hexe2Y[index];

				// If we have run out of area it is an error

				if(hexp[1] >= m_Bilayer2Length)
				{
					new CLogBuilderError(0, "lamella lattice outside SimBox");
					return false;
				}

				bEvenRow = !bEvenRow;
			}
		}
	}

	// Patches of polymers are specified for each monolayer independently

	if(!m_bPatches[0])
		random_shuffle_wrapper(m_RandomUpperIndex.begin(), m_RandomUpperIndex.end());
	
	if(!m_bPatches[1])
		random_shuffle_wrapper(m_RandomLowerIndex.begin(), m_RandomLowerIndex.end());

	// ****************************************
	// Loop over the number of vertices in the lattices. We have to take care 
	// with the orientation of the 
	// polymers in each monolayer and use the m_MonolayerSign[] variable to
	// adjust the coordinate in the direction of the bilayer normal when setting
	// bead coordinates. Polymer head coordinates are positioned
	// directly above (or below) the tails using the m_Thickness parameter.
	//
	// Note that the xp[] coordinates are without regard to the bilayer 
	// orientation, that is, they are in-plane coordinates. We convert them 
	// to X,Y,Z coordinates later. Also, we set the copies of the bead coordinates
	// using SetunPBCXPos(), SetInitialXPos() later.

	double xp[3];
	double xp1[3];

	xp[0] = 0.0;
	xp[1] = 0.0;
	xp[2] = 0.0;

	xp1[0] = 0.0;
	xp1[1] = 0.0;
	xp1[2] = 0.0;

	short monolayerIndex	= 0;	// Pointer to monolayer
	long species			= 0;	// Counter for polymer types
	long polyNo				= 0;	// Counter for polymers within a type
	long polyUpperCounter	= 0;	// Counter for polymers in upper monolayer
	long polyLowerCounter	= 0;	// Counter for polymers in lower monolayer
	long site				= 0;	// Randomly-chosen site index

	for(unsigned long iv=0; iv<m_vBilayer.size(); iv++)
	{
		if(polyNo == m_PolymerTotal.at(species))
		{
			species++;		// Move to the next polymer type
			polyNo = 0;		// Reset the polymer counter for the next type
		}

		// We still use the variable upperPoly to avoid having to change its name

		CPolymer* upperPoly = m_vBilayer.at(iv);

		if(polyNo < m_PolymerUpperTotal.at(species))
		{
			monolayerIndex = 0;		// Note which monolayer the polymer is in

			// Pick a site in the upper monolayer for the polymer using the 
			// number of polymers in the upper monolayer as an index into a 
			// shuffled vector of site indices.

			site = m_RandomUpperIndex.at(polyUpperCounter++);

			xp[0] = m_UpperXCoord.at(site);
			xp[1] = m_UpperYCoord.at(site);
			xp[2] = 0.0;
		}
		else
		{
			monolayerIndex = 1;

			site = m_RandomLowerIndex.at(polyLowerCounter++);

			xp[0] = m_LowerXCoord.at(site);
			xp[1] = m_LowerYCoord.at(site);
			xp[2] = 0.0;
		}

		// Position the beads in the polymers around the head and tail axis.
		// We have to take account of the orientation of the bilayer now.

		if(m_X == 1)
		{
			upperPoly->GetTail()->SetXPos(m_BilayerCentre + m_MonolayerSign[monolayerIndex]*0.2);
			upperPoly->GetTail()->SetYPos(xp[0]);	// shift tails slightly to avoid
			upperPoly->GetTail()->SetZPos(xp[1]);	// creating large forces

			upperPoly->GetHead()->SetXPos(m_MonolayerHead[monolayerIndex]);
			upperPoly->GetHead()->SetYPos(xp[0]);
			upperPoly->GetHead()->SetZPos(xp[1]);

			// Assign coordinates to other beads in the polymers wrapping them around
			// the head-tail axis. If the user has selected the linearise option then 
			// the beads are given coordinates on a straight line joining the polymer
			// head and tail, otherwise they are positioned somwhat randomly about the
			// head-tail axis. If the polymers are in addition linear, the initial 
			// configuration will consist of exactly straight polymers, but if the 
			// polymers are branched only the beads between the head and tail will
			// be straightened.  We check that the random
			// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
			// We don't have to check for their being less than 0 because we only
			// add a random component, and don't subtract it.

			double beadCounter = 1.0;

			for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
			{
				if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
				{
					if(m_bLinearise)
					{
						xp1[0] = upperPoly->GetHead()->GetXPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
						xp1[1] = xp[0];
						xp1[2] = xp[1];
						beadCounter += 1.0;
					}
					else
					{
						xp1[0] = m_BilayerCentre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();
						xp1[1] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[2] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();

						if(xp1[1] > riState.GetSimBoxYLength())
							xp1[1] -= 0.5*m_Hexe1X[monolayerIndex];

						if(xp1[2] > riState.GetSimBoxZLength())
							xp1[2] -= 0.5*m_Hexe2Y[monolayerIndex];
					}

					(*iterBead2)->SetXPos(xp1[0]);
					(*iterBead2)->SetYPos(xp1[1]);
					(*iterBead2)->SetZPos(xp1[2]);
				}
			}
		}
		else if(m_Y == 1)
		{
			upperPoly->GetTail()->SetXPos(xp[1]);	// shift tails slightly to avoid
			upperPoly->GetTail()->SetYPos(m_BilayerCentre + m_MonolayerSign[monolayerIndex]*0.2);
			upperPoly->GetTail()->SetZPos(xp[0]);	// creating large forces

			upperPoly->GetHead()->SetXPos(xp[1]);
			upperPoly->GetHead()->SetYPos(m_MonolayerHead[monolayerIndex]);
			upperPoly->GetHead()->SetZPos(xp[0]);

			// Assign coordinates to other beads in the polymers wrapping them around
			// the head-tail axis. If the user has selected the linearise option then 
			// the beads are given coordinates on a straight line joining the polymer
			// head and tail, otherwise they are positioned somwhat randomly about the
			// head-tail axis. If the polymers are in addition linear, the initial 
			// configuration will consist of exactly straight polymers, but if the 
			// polymers are branched only the beads between the head and tail will
			// be straightened.  We check that the random
			// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
			// We don't have to check for their being less than 0 because we only
			// add a random component, and don't subtract it.

			double beadCounter = 1.0;

			for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
			{
				if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
				{
					if(m_bLinearise)
					{
						xp1[0] = xp[1];
						xp1[1] = upperPoly->GetHead()->GetYPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
						xp1[2] = xp[0];
						beadCounter += 1.0;
					}
					else
					{
						xp1[0] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[1] = m_BilayerCentre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();
						xp1[2] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();

						if(xp1[0] > riState.GetSimBoxXLength())
							xp1[0] -= 0.5*m_Hexe2Y[monolayerIndex];

						if(xp1[2] > riState.GetSimBoxZLength())
							xp1[2] -= 0.5*m_Hexe1X[monolayerIndex];
					}

					(*iterBead2)->SetXPos(xp1[0]);
					(*iterBead2)->SetYPos(xp1[1]);
					(*iterBead2)->SetZPos(xp1[2]);
				}
			}
		}
		else if(m_Z == 1)
		{
			upperPoly->GetTail()->SetXPos(xp[0]);	// shift tails slightly to avoid
			upperPoly->GetTail()->SetYPos(xp[1]);	// creating large forces
			upperPoly->GetTail()->SetZPos(m_BilayerCentre + m_MonolayerSign[monolayerIndex]*0.2);

			upperPoly->GetHead()->SetXPos(xp[0]);
			upperPoly->GetHead()->SetYPos(xp[1]);
			upperPoly->GetHead()->SetZPos(m_MonolayerHead[monolayerIndex]);

			// Assign coordinates to other beads in the polymers wrapping them around
			// the head-tail axis. If the user has selected the linearise option then 
			// the beads are given coordinates on a straight line joining the polymer
			// head and tail, otherwise they are positioned somwhat randomly about the
			// head-tail axis. If the polymers are in addition linear, the initial 
			// configuration will consist of exactly straight polymers, but if the 
			// polymers are branched only the beads between the head and tail will
			// be straightened.  We check that the random
			// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
			// We don't have to check for their being less than 0 because we only
			// add a random component, and don't subtract it.

			double beadCounter = 1.0;

			for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
			{
				if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
				{
					if(m_bLinearise)
					{
						xp1[0] = xp[0];
						xp1[1] = xp[1];
						xp1[2] = upperPoly->GetHead()->GetZPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
						beadCounter += 1.0;
					}
					else
					{
						xp1[0] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[1] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[2] = m_BilayerCentre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();

						if(xp1[0] > riState.GetSimBoxXLength())
							xp1[0] -= 0.5*m_Hexe1X[monolayerIndex];

						if(xp1[1] > riState.GetSimBoxYLength())
							xp1[1] -= 0.5*m_Hexe2Y[monolayerIndex];
					}

					(*iterBead2)->SetXPos(xp1[0]);
					(*iterBead2)->SetYPos(xp1[1]);
					(*iterBead2)->SetZPos(xp1[2]);
				}
			}
		}

		polyNo++;
	}	// End of loop over all polymers in bilayer

	return true;
}

// **********************************************************************
// Member functions for enclosed class: isFreeBilayer.

// This object assembles the selected polymers into a bilayer that is freely
// floating in the SimBox. The area of the bilayer, its orientation, centre and 
// thickness are arguments and the routine creates a hexagonal lattice with 
// two polymers per lattice site, one in each monolayer. If multiple polymer 
// types are being used they are assigned coordinates sequentially. This 
// results in different parts of the bilayer being occupied by each type of polymer.
// If a randomised distribution is required we can change the routine later.
//
// This routine does not check that the bilayer does not overlap any other structure
// including the walls. It assumes that this has been done by the calling routine.

CBuilder::isFreeBilayer::isFreeBilayer(const zLongVector& polymerTypes, const zDoubleVector& upperFractions,
							   long Xn, long Yn, long Zn, double thickness, 
							   double area, double length1, double length2,
							   double centre1, double centre2, double centre3, 
							   double upperHead, double lowerHead, long upperTotal, long lowerTotal,
							   bool bMonolayer, bool bLinear,
							   const bool bPatches[2]) : m_PolymerTypes(polymerTypes),
													   m_UpperFractions(upperFractions),
													   m_X(Xn), m_Y(Yn), m_Z(Zn),
													   m_Thickness(thickness),
													   m_BilayerArea(area),
													   m_Bilayer1Length(length1),
													   m_Bilayer2Length(length2),
													   m_Bilayer1Centre(centre1),
													   m_Bilayer2Centre(centre2),
													   m_Bilayer3Centre(centre3),
													   m_UpperHead(upperHead),
													   m_LowerHead(lowerHead),
													   m_UpperTotal(upperTotal),
													   m_LowerTotal(lowerTotal),
													   m_bMonolayer(bMonolayer),
													   m_bLinearise(bLinear),
													   m_PolymerMaxSize(0)
{
	m_bPatches[0] = bPatches[0];
	m_bPatches[1] = bPatches[1];

	m_vBilayer.clear();
	m_PolymerTotal.clear();
	m_PolymerUpperTotal.clear();
	m_PolymerSize.clear();
	m_RandomUpperIndex.clear();
	m_RandomLowerIndex.clear();
	m_UpperXCoord.clear();
	m_UpperYCoord.clear();
	m_LowerXCoord.clear();
	m_LowerYCoord.clear();

	m_MonolayerSign[0]	= 1.0;		// Sign need to assign correct coordinates to
	m_MonolayerSign[1]	= -1.0;		// beads in polymers in the upper/lower monolayers

	// m_bMonolayer does not identify which monolayer is full and which is empty. 
	// Here we set up the lattice(s) for the occupied monolayer(s).

	if(m_UpperTotal == 0)
	{
		m_MonolayerTotal[0]	= 0;
		m_MonolayerTotal[1]	= m_LowerTotal;
		m_LatticeSize[0]	= 0;
		m_LatticeSize[1]	= m_LowerTotal;
		m_MonolayerHead[0]	= 0.0;
		m_MonolayerHead[1]	= m_LowerHead;
	}
	else if(m_LowerTotal == 0)
	{
		m_MonolayerTotal[0]	= m_UpperTotal;
		m_MonolayerTotal[1]	= 0;
		m_LatticeSize[0]	= m_UpperTotal;
		m_LatticeSize[1]	= 0;
		m_MonolayerHead[0]	= m_UpperHead;
		m_MonolayerHead[1]	= 0.0;
	}
	else
	{
		m_MonolayerTotal[0]	= m_UpperTotal;
		m_MonolayerTotal[1]	= m_LowerTotal;
		m_LatticeSize[0]	= m_UpperTotal;
		m_LatticeSize[1]	= m_LowerTotal;
		m_MonolayerHead[0]	= m_UpperHead;
		m_MonolayerHead[1]	= m_LowerHead;
	}

	for(short i=0; i<2; i++)
	{
		m_HexSpace[i]		= 0.0;		
		m_HexCAxisSpace[i]	= 0.0;		
		m_Hexe1X[i]			= 0.0;
		m_Hexe1Y[i]			= 0.0;
		m_Hexe2X[i]			= 0.0;	
		m_Hexe2Y[i]			= 0.0;
	}
}

CBuilder::isFreeBilayer::~isFreeBilayer()
{
}

bool CBuilder::isFreeBilayer::Assemble(CInitialState& riState)
{
	// Copy the polymers in the CInitialState into a local vector for ease of access.
	// We loop over all types of polymer adding them to the vector if
	// their type occurs in the data for a composite bilayer. We also
	// store the numbers of each type of polymer in the upper monolayer, 
	// and the number of beads in the polymer types in the bilayer. 

	PolymerVector bilayerPolymers = riState.GetPolymers();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		long polyNo  = riState.GetPolymerTotalForType(ip);

		lastPolymer += polyNo;

		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) != m_PolymerTypes.end())
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(bilayerPolymers.begin()+firstPolymer, bilayerPolymers.begin()+lastPolymer, back_inserter(m_vBilayer));

			// Store the number of this type of polymer and the number in the
			// upper monolayer

			m_PolymerTotal.push_back(polyNo);


			for(unsigned long polyIndex=0; polyIndex<m_PolymerTypes.size(); polyIndex++)
			{
				if(m_PolymerTypes.at(polyIndex) == ip)
				{
					m_PolymerUpperTotal.push_back(static_cast<long>(m_UpperFractions.at(polyIndex)*polyNo));
				}
			}

			// For linear and branched polymers we count the number of beads between 
			// the head and tail (inclusive) and use this to define the spacing in 
			// the C direction. The spacing is defined as the monolayer thickness
			// divided by m_PolymerSize-1. 
			// We don't know whether the head or tail will be encountered first in the 
			// polymer string so we set a flag, bCount, to indicate when we should count
			// beads. It is reset to null when both head and tail have been counted.
			// The other beads in the polymer are arranged randomly around the head-tail
			// axis.
			//
			// NOTE. This algorithm won't work if the tail bead is found first in the
			// polymer shape string. It will arrange the beads as if the head were first,
			// resulting in the beads between the head and tail having their order
			// reversed.

			const BeadVector&  vBeads = m_vBilayer.back()->GetBeads();
			CBead*		pHead = dynamic_cast<CBead*>(m_vBilayer.back()->GetHead());
			CBead*		pTail = dynamic_cast<CBead*>(m_vBilayer.back()->GetTail());

			CBead* pEnd		= NULL;
			long counter	= 0;
			bool bCount		= false;

			long dummyCount = 0;

			for(cBeadVectorIterator pBead=vBeads.begin();pBead!=vBeads.end(); pBead++)
			{
				dummyCount++;

				if(*pBead == pHead && !bCount)
				{
					bCount = true;
					pEnd   = pTail;
					counter++;
				}
				else if( *pBead == pTail && !bCount)
				{
					bCount = true;
					pEnd   = pHead;
					counter++;
				}
				else if(*pBead == pEnd)
				{
					bCount = false;
					counter++;
				}
				else if(bCount)
				{
					counter++;
				}
			}
			m_PolymerSize.push_back(counter);
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	m_PolymerMaxSize = *max_element(m_PolymerSize.begin(), m_PolymerSize.end());

	// Set the C axis spacing for the beads in the polymers. We use
	// the size of the largest polymer (between its head and tail) to set the
	// spacing for all polymers. This allows shorter ones to be easily 
	// arranged between longer ones without being stretched too far.

	m_HexCAxisSpace[0] = 0.5*m_Thickness/static_cast<double>(m_PolymerMaxSize-1);
	m_HexCAxisSpace[1] = m_HexCAxisSpace[0];

	// ****************************************
	// Calculate the lattice size for the hexagonal lattices that organise
	// the polymers in the two monolayer. Note that either monolayer may be
	// empty. We have to take into account orientation of the bilayer and the 
	// number of polymers in each  monolayer to calculate the spacing.
	// We shrink the lattice vectors by 0.999 and offset the first sites by 0.001 to
	// prevent any sites going outside the SimBox. This occurred with a box size
	// of 20**3 in the test run 11024, changelist 462.

	// Get unit vectors in e1 and e2 directions (NB. 1.1547 = 2/sqrt(3)) and the
	// number of vertices in the lattice.

	if(m_bMonolayer)
	{
		if(m_UpperTotal == 0)	// All polymers in lower monolayer
		{
			m_HexSpace[0] = 0.0;
			m_HexSpace[1] = 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[1]));
		}
		else
		{
			m_HexSpace[0] = 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[0]));
			m_HexSpace[1] = 0.0;
		}
	}
	else
	{
			m_HexSpace[0] = 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[0]));
			m_HexSpace[1] = 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[1]));
	}

	m_Hexe1X[0]	= m_HexSpace[0];			// e1 lattice vector
	m_Hexe2X[0]	= 0.5*m_HexSpace[0];		// e2 lattice vector
	m_Hexe2Y[0]	= 0.866025*m_HexSpace[0];

	m_Hexe1X[1]	= m_HexSpace[1];			// e1 lattice vector
	m_Hexe2X[1]	= 0.5*m_HexSpace[1];		// e2 lattice vector
	m_Hexe2Y[1]	= 0.866025*m_HexSpace[1];

	// ****************************************
	// Create the vectors holding indices to the coordinates of free lattice 
	// sites at which polymers may be placed. As polymers are positioned,
	// the corresponding sites are removed from the vectors allowing a 
	// simple random choice from the remaining elements. We calculate
	// both the upper and lower monolayer coordinates even if all the polymers
	// are in one monolayer.

	// The origin of the area in which the polymers are placed is fixed
	// by the parameters m_Bilayer1Centre, m_Bilayer2Centre, m_Bilayer1Length and 
	// m_Bilayer2Length. The m_Bilayer3Centre is along the normal axis to 
	// the bilayer and is used later in positioning the polymers in the
	// two monolayers.

	double origin[2];
	double width[2];

	origin[0] = m_Bilayer1Centre - 0.5*m_Bilayer1Length;
	origin[1] = m_Bilayer2Centre - 0.5*m_Bilayer2Length;

	width[0] = origin[0] + m_Bilayer1Length;
	width[1] = origin[1] + m_Bilayer2Length;

	for(short int index = 0; index<2; index++)
	{
		double hexp[2];

		hexp[0] = origin[0] + 0.001*m_Hexe1X[index];	// We use an offset to avoid positions coinciding with boundaries
		hexp[1] = origin[1] + 0.001*m_Hexe2Y[index];

		bool bEvenRow = true;

		for(long icu=0; icu<m_LatticeSize[index]; icu++)
		{
			if(index == 0)
			{
				m_RandomUpperIndex.push_back(icu);
				m_UpperXCoord.push_back(hexp[0]);
				m_UpperYCoord.push_back(hexp[1]);
			}
			else
			{
				m_RandomLowerIndex.push_back(icu);
				m_LowerXCoord.push_back(hexp[0]);
				m_LowerYCoord.push_back(hexp[1]);
			}

			hexp[0] += m_Hexe1X[index];

			if(hexp[0] > width[0])
			{
				if(bEvenRow)
					hexp[0] = origin[0] + m_Hexe2X[index];
				else
					hexp[0] = origin[0];

				hexp[1] += m_Hexe2Y[index];

				// If we have run out of area it is an error

				if(hexp[1] >= width[1])
				{
					new CLogBuilderError(0, "free lamella lattice outside SimBox");
					return false;
				}

				bEvenRow = !bEvenRow;
			}
		}
	}

	// Patches of polymers are specified for each monolayer independently

	if(!m_bPatches[0])
		random_shuffle_wrapper(m_RandomUpperIndex.begin(), m_RandomUpperIndex.end());
	
	if(!m_bPatches[1])
		random_shuffle_wrapper(m_RandomLowerIndex.begin(), m_RandomLowerIndex.end());

	// ****************************************
	// Loop over the number of vertices in the lattices. We have to take care 
	// with the orientation of the 
	// polymers in each monolayer and use the m_MonolayerSign[] variable to
	// adjust the coordinate in the direction of the bilayer normal when setting
	// bead coordinates. Polymer head coordinates are positioned
	// directly above (or below) the tails using the m_Thickness parameter.
	//
	// Note that the xp[] coordinates are without regard to the bilayer 
	// orientation, that is, they are in-plane coordinates. We convert them 
	// to X,Y,Z coordinates later. Also, we set the copies of the bead coordinates
	// using SetunPBCXPos(), SetInitialXPos() later.

	double xp[3];
	double xp1[3];

	xp[0] = 0.0;
	xp[1] = 0.0;
	xp[2] = 0.0;

	xp1[0] = 0.0;
	xp1[1] = 0.0;
	xp1[2] = 0.0;

	short monolayerIndex	= 0;	// Pointer to monolayer
	long species			= 0;	// Counter for polymer types
	long polyNo				= 0;	// Counter for polymers within a type
	long polyUpperCounter	= 0;	// Counter for polymers in upper monolayer
	long polyLowerCounter	= 0;	// Counter for polymers in lower monolayer
	long site				= 0;	// Randomly-chosen site index

	for(unsigned long iv=0; iv<m_vBilayer.size(); iv++)
	{
		if(polyNo == m_PolymerTotal.at(species))
		{
			species++;		// Move to the next polymer type
			polyNo = 0;		// Reset the polymer counter for the next type
		}

		// We still use the variable upperPoly to avoid having to change its name

		CPolymer* upperPoly = m_vBilayer.at(iv);

		if(polyNo < m_PolymerUpperTotal.at(species))
		{
			monolayerIndex = 0;		// Note which monolayer the polymer is in

			// Pick a site in the upper monolayer for the polymer using the 
			// number of polymers in the upper monolayer as an index into a 
			// shuffled vector of site indices.

			site = m_RandomUpperIndex.at(polyUpperCounter++);

			xp[0] = m_UpperXCoord.at(site);
			xp[1] = m_UpperYCoord.at(site);
			xp[2] = 0.0;
		}
		else
		{
			monolayerIndex = 1;

			site = m_RandomLowerIndex.at(polyLowerCounter++);

			xp[0] = m_LowerXCoord.at(site);
			xp[1] = m_LowerYCoord.at(site);
			xp[2] = 0.0;
		}

		// Position the beads in the polymers around the head and tail axis.
		// We have to take account of the orientation of the bilayer now.
		// But the m_Bilayer3Centre always contains the coordinate of the bilayer
		// centre along its normal axis. The other two coordinates are obtained
		// from the right-hand rule for the x, y, z axes.

		if(m_X == 1)
		{
			upperPoly->GetTail()->SetXPos(m_Bilayer3Centre + m_MonolayerSign[monolayerIndex]*0.2);
			upperPoly->GetTail()->SetYPos(xp[0]);	// shift tails slightly to avoid
			upperPoly->GetTail()->SetZPos(xp[1]);	// creating large forces

			upperPoly->GetHead()->SetXPos(m_MonolayerHead[monolayerIndex]);
			upperPoly->GetHead()->SetYPos(xp[0]);
			upperPoly->GetHead()->SetZPos(xp[1]);

			// Assign coordinates to other beads in the polymers wrapping them around
			// the head-tail axis. If the user has selected the linearise option then 
			// the beads are given coordinates on a straight line joining the polymer
			// head and tail, otherwise they are positioned somwhat randomly about the
			// head-tail axis. If the polymers are in addition linear, the initial 
			// configuration will consist of exactly straight polymers, but if the 
			// polymers are branched only the beads between the head and tail will
			// be straightened.  We check that the random
			// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
			// We don't have to check for their being less than 0 because we only
			// add a random component, and don't subtract it.

			double beadCounter = 1.0;

			for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
			{
				if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
				{
					if(m_bLinearise)
					{
						xp1[0] = upperPoly->GetHead()->GetXPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
						xp1[1] = xp[0];
						xp1[2] = xp[1];
						beadCounter += 1.0;
					}
					else
					{
						xp1[0] = m_Bilayer3Centre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();
						xp1[1] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[2] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();

						if(xp1[1] > riState.GetSimBoxYLength())
							xp1[1] -= 0.5*m_Hexe1X[monolayerIndex];

						if(xp1[2] > riState.GetSimBoxZLength())
							xp1[2] -= 0.5*m_Hexe2Y[monolayerIndex];
					}

					(*iterBead2)->SetXPos(xp1[0]);
					(*iterBead2)->SetYPos(xp1[1]);
					(*iterBead2)->SetZPos(xp1[2]);
				}
			}
		}
		else if(m_Y == 1)
		{
			upperPoly->GetTail()->SetXPos(xp[1]);	// shift tails slightly to avoid
			upperPoly->GetTail()->SetYPos(m_Bilayer3Centre + m_MonolayerSign[monolayerIndex]*0.2);
			upperPoly->GetTail()->SetZPos(xp[0]);	// creating large forces

			upperPoly->GetHead()->SetXPos(xp[1]);
			upperPoly->GetHead()->SetYPos(m_MonolayerHead[monolayerIndex]);
			upperPoly->GetHead()->SetZPos(xp[0]);

			// Assign coordinates to other beads in the polymers wrapping them around
			// the head-tail axis. If the user has selected the linearise option then 
			// the beads are given coordinates on a straight line joining the polymer
			// head and tail, otherwise they are positioned somwhat randomly about the
			// head-tail axis. If the polymers are in addition linear, the initial 
			// configuration will consist of exactly straight polymers, but if the 
			// polymers are branched only the beads between the head and tail will
			// be straightened.  We check that the random
			// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
			// We don't have to check for their being less than 0 because we only
			// add a random component, and don't subtract it.

			double beadCounter = 1.0;

			for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
			{
				if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
				{
					if(m_bLinearise)
					{
						xp1[0] = xp[1];
						xp1[1] = upperPoly->GetHead()->GetYPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
						xp1[2] = xp[0];
						beadCounter += 1.0;
					}
					else
					{
						xp1[0] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[1] = m_Bilayer3Centre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();
						xp1[2] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();

						if(xp1[0] > riState.GetSimBoxXLength())
							xp1[0] -= 0.5*m_Hexe2Y[monolayerIndex];

						if(xp1[2] > riState.GetSimBoxZLength())
							xp1[2] -= 0.5*m_Hexe1X[monolayerIndex];
					}

					(*iterBead2)->SetXPos(xp1[0]);
					(*iterBead2)->SetYPos(xp1[1]);
					(*iterBead2)->SetZPos(xp1[2]);
				}
			}
		}
		else if(m_Z == 1)
		{
			upperPoly->GetTail()->SetXPos(xp[0]);	// shift tails slightly to avoid
			upperPoly->GetTail()->SetYPos(xp[1]);	// creating large forces
			upperPoly->GetTail()->SetZPos(m_Bilayer3Centre + m_MonolayerSign[monolayerIndex]*0.2);

			upperPoly->GetHead()->SetXPos(xp[0]);
			upperPoly->GetHead()->SetYPos(xp[1]);
			upperPoly->GetHead()->SetZPos(m_MonolayerHead[monolayerIndex]);

			// Assign coordinates to other beads in the polymers wrapping them around
			// the head-tail axis. If the user has selected the linearise option then 
			// the beads are given coordinates on a straight line joining the polymer
			// head and tail, otherwise they are positioned somwhat randomly about the
			// head-tail axis. If the polymers are in addition linear, the initial 
			// configuration will consist of exactly straight polymers, but if the 
			// polymers are branched only the beads between the head and tail will
			// be straightened.  We check that the random
			// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
			// We don't have to check for their being less than 0 because we only
			// add a random component, and don't subtract it.

			double beadCounter = 1.0;

			for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
			{
				if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
				{
					if(m_bLinearise)
					{
						xp1[0] = xp[0];
						xp1[1] = xp[1];
						xp1[2] = upperPoly->GetHead()->GetZPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
						beadCounter += 1.0;
					}
					else
					{
						xp1[0] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[1] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[2] = m_Bilayer3Centre + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();

						if(xp1[0] > riState.GetSimBoxXLength())
							xp1[0] -= 0.5*m_Hexe1X[monolayerIndex];

						if(xp1[1] > riState.GetSimBoxYLength())
							xp1[1] -= 0.5*m_Hexe2Y[monolayerIndex];
					}

					(*iterBead2)->SetXPos(xp1[0]);
					(*iterBead2)->SetYPos(xp1[1]);
					(*iterBead2)->SetZPos(xp1[2]);
				}
			}
		}

		polyNo++;
	}	// End of loop over all polymers in bilayer

	return true;
}

// **********************************************************************
// Member functions for enclosed class: isMultiBilayer.

// This object assembles the selected polymers into a set of bilayers. 
// The area of the simulation box, orientation, centres and thickness of the 
// bilaye are arguments and the routine creates a hexagonal lattice with 
// two polymers per lattice site, one in each monolayer. The polymers are
// distributed randomly throughout all bilayers, and no patches are allowed.
//
// Constructor for use when it is known whether a bilayer or monolayer is being
// created.

CBuilder::isMultiBilayer::isMultiBilayer(const zLongVector& polymerTypes, const zDoubleVector& upperFractions,
				  long Xn, long Yn, long Zn, zDoubleVector centres, double thickness, 
				  double area, double length1, double length2,
				  zLongVector upperTotals, zLongVector lowerTotals,
				  bool bLinear) : m_PolymerTypes(polymerTypes),
													   m_UpperFractions(upperFractions),
													   m_X(Xn), m_Y(Yn), m_Z(Zn),
													   m_BilayerCentre(centres),
													   m_Thickness(thickness),
													   m_BilayerArea(area),
													   m_Bilayer1Length(length1),
													   m_Bilayer2Length(length2),
													   m_UpperTotals(upperTotals),
													   m_LowerTotals(lowerTotals),
													   m_bLinearise(bLinear),
													   m_LamellaTotal(centres.size()),
													   m_PolymerMaxSize(0)
{
	m_vBilayer.clear();
	m_PolymerTotal.clear();
	m_PolymerUpperTotal.clear();
	m_PolymerSize.clear();
	m_RandomUpperIndex.clear();
	m_RandomLowerIndex.clear();
	m_UpperXCoord.clear();
	m_UpperYCoord.clear();
	m_LowerXCoord.clear();
	m_LowerYCoord.clear();

	m_MonolayerSign[0]	= 1.0;		// Sign needed to assign correct coordinates to
	m_MonolayerSign[1]	= -1.0;		// beads in polymers in the upper/lower monolayers

	for(short i=0; i<2; i++)
	{
		m_LatticeSize[i]	= 0;
		m_HexSpace[i]		= 0.0;		
		m_HexCAxisSpace[i]	= 0.0;		
		m_Hexe1X[i]			= 0.0;
		m_Hexe1Y[i]			= 0.0;
		m_Hexe2X[i]			= 0.0;	
		m_Hexe2Y[i]			= 0.0;
	}
}

CBuilder::isMultiBilayer::~isMultiBilayer()
{
}

bool CBuilder::isMultiBilayer::Assemble(CInitialState& riState)
{
	// Copy the polymers in the CInitialState into a local vector for ease of access.
	// We loop over all types of polymer adding them to the vector if
	// their type occurs in the data for a composite bilayer. We also
	// store the numbers of each type of polymer in the upper monolayer, 
	// and the number of beads in the polymer types in the bilayer. 

	PolymerVector bilayerPolymers = riState.GetPolymers();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		long polyNo  = riState.GetPolymerTotalForType(ip);

		lastPolymer += polyNo;

		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) != m_PolymerTypes.end())
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(bilayerPolymers.begin()+firstPolymer, bilayerPolymers.begin()+lastPolymer, back_inserter(m_vBilayer));

			// Store the number of this type of polymer and the number in the
			// upper monolayer

			m_PolymerTotal.push_back(polyNo);


			for(unsigned long polyIndex=0; polyIndex<m_PolymerTypes.size(); polyIndex++)
			{
				if(m_PolymerTypes.at(polyIndex) == ip)
				{
					m_PolymerUpperTotal.push_back(static_cast<long>(m_UpperFractions.at(polyIndex)*polyNo));
				}
			}

			// For linear and branched polymers we count the number of beads between 
			// the head and tail (inclusive) and use this to define the spacing in 
			// the C direction. The spacing is defined as the monolayer thickness
			// divided by m_PolymerSize-1. 
			// We don't know whether the head or tail will be encountered first in the 
			// polymer string so we set a flag, bCount, to indicate when we should count
			// beads. It is reset to null when both head and tail have been counted.
			// The other beads in the polymer are arranged randomly around the head-tail
			// axis.
			//
			// NOTE. This algorithm won't work if the tail bead is found first in the
			// polymer shape string. It will arrange the beads as if the head were first,
			// resulting in the beads between the head and tail having their order
			// reversed.

			const BeadVector&  vBeads = m_vBilayer.back()->GetBeads();
			CBead*		pHead = dynamic_cast<CBead*>(m_vBilayer.back()->GetHead());
			CBead*		pTail = dynamic_cast<CBead*>(m_vBilayer.back()->GetTail());

			CBead* pEnd		= NULL;
			long counter	= 0;
			bool bCount		= false;

			long dummyCount = 0;

			for(cBeadVectorIterator pBead=vBeads.begin();pBead!=vBeads.end(); pBead++)
			{
				dummyCount++;

				if(*pBead == pHead && !bCount)
				{
					bCount = true;
					pEnd   = pTail;
					counter++;
				}
				else if( *pBead == pTail && !bCount)
				{
					bCount = true;
					pEnd   = pHead;
					counter++;
				}
				else if(*pBead == pEnd)
				{
					bCount = false;
					counter++;
				}
				else if(bCount)
				{
					counter++;
				}
			}
			m_PolymerSize.push_back(counter);
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	m_PolymerMaxSize = *max_element(m_PolymerSize.begin(), m_PolymerSize.end());

	// Set the C axis spacing for the beads in the polymers. We use
	// the size of the largest polymer (between its head and tail) to set the
	// spacing for all polymers. This allows shorter ones to be easily 
	// arranged between longer ones without being stretched too far.

	m_HexCAxisSpace[0] = 0.5*m_Thickness/static_cast<double>(m_PolymerMaxSize-1);
	m_HexCAxisSpace[1] = m_HexCAxisSpace[0];

	// ****************************************
	// Calculate the lattice size for the hexagonal lattices that organise
	// the polymers in the two monolayers. Note that NEITHER monolayer may be
	// empty. We have to take into account the orientation of the bilayer and 
	// the number of polymers in each  monolayer to calculate the spacing.
	// Because all bilayers are equivalent, except possibly the uppermost one
	// that may contain excess polymers, we use the first to define the lattice
	// for all but the last bilayer and then do the last separately.

	long upperLatticeSize = 0;
	long lowerLatticeSize = 0;

	for(long ln=0; ln<m_LamellaTotal; ln++)
	{
		upperLatticeSize += m_UpperTotals.at(ln);
		lowerLatticeSize += m_LowerTotals.at(ln);
	}

	m_LatticeSize[0] = upperLatticeSize/m_LamellaTotal;
	m_LatticeSize[1] = lowerLatticeSize/m_LamellaTotal;

	// Check that the lattice size is large enough that all polymers can be positioned
	// in one of the lamellae. If there are excess polymers, then increase each lamella
	// by sufficient rows to hold the remaining polymers.
	// We shrink the lattice vectors by 0.999 and offset the first sites by 0.001 to
	// prevent any sites going outside the SimBox. This occurred with a box size
	// of 20**3 in the test run 11024, changelist 462.
/*
	long potentialTotal = upperLatticeSize + lowerLatticeSize;
	long actualTotal	= m_vBilayer.size();

	if(actualTotal > potentialTotal)
	{
		long diff = actualTotal - potentialTotal;

	}
*/
	m_HexSpace[0] = 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[0]));
	m_HexSpace[1] = 0.999*sqrt(1.1547*m_BilayerArea/static_cast<double>(m_LatticeSize[1]));

	// Get unit vectors in e1 and e2 directions (NB. 1.1547 = 2/sqrt(3)) and the
	// number of vertices in the lattice.

	m_Hexe1X[0]	= m_HexSpace[0];			// e1 lattice vector
	m_Hexe2X[0]	= 0.5*m_HexSpace[0];		// e2 lattice vector
	m_Hexe2Y[0]	= 0.866025*m_HexSpace[0];

	m_Hexe1X[1]	= m_HexSpace[1];			// e1 lattice vector
	m_Hexe2X[1]	= 0.5*m_HexSpace[1];		// e2 lattice vector
	m_Hexe2Y[1]	= 0.866025*m_HexSpace[1];

	// ****************************************
	// Create the vectors holding indices to the coordinates of free lattice 
	// sites at which polymers may be placed. As polymers are positioned,
	// the corresponding sites are removed from the vectors allowing a 
	// simple random choice from the remaining elements. We calculate
	// both the upper and lower monolayer coordinates even if all the polymers
	// are in one monolayer.

	for(short int index = 0; index<2; index++)
	{
		double hexp[2];

		hexp[0] = 0.001*m_Hexe1X[index];	// We use an offset to avoid positions coinciding with boundaries
		hexp[1] = 0.001*m_Hexe2Y[index];

		bool bEvenRow = true;

		for(long icu=0; icu<m_LatticeSize[index]; icu++)
		{
			if(index == 0)
			{
				m_RandomUpperIndex.push_back(icu);
				m_UpperXCoord.push_back(hexp[0]);
				m_UpperYCoord.push_back(hexp[1]);
			}
			else
			{
				m_RandomLowerIndex.push_back(icu);
				m_LowerXCoord.push_back(hexp[0]);
				m_LowerYCoord.push_back(hexp[1]);
			}

			hexp[0] += m_Hexe1X[index];

			if(hexp[0] > m_Bilayer1Length)
			{
				if(bEvenRow)
					hexp[0] = m_Hexe2X[index];
				else
					hexp[0] = 0.0;

				hexp[1] += m_Hexe2Y[index];

				// If we have run out of area it is an error

				if(hexp[1] >= m_Bilayer2Length)
				{
					new CLogBuilderError(0, "multi lamella lattice outside SimBox");
					return false;
				}

				bEvenRow = !bEvenRow;
			}
		}
	}

	// No patches are allowed

	random_shuffle_wrapper(m_RandomUpperIndex.begin(), m_RandomUpperIndex.end());
	random_shuffle_wrapper(m_RandomLowerIndex.begin(), m_RandomLowerIndex.end());

	// ****************************************
	// Loop over the number of vertices in the lattices. We have to take care 
	// with the orientation of the 
	// polymers in each monolayer and use the m_MonolayerSign[] variable to
	// adjust the coordinate in the direction of the bilayer normal when setting
	// bead coordinates. Polymer head coordinates are positioned
	// directly above (or below) the tails using the m_Thickness parameter.
	//
	// Note that the xp[] coordinates are without regard to the bilayer 
	// orientation, that is, they are in-plane coordinates. We convert them 
	// to X,Y,Z coordinates later. Also, we set the copies of the bead coordinates
	// using SetunPBCXPos(), SetInitialXPos() later.

	double xp[3];
	double xp1[3];

	xp[0] = 0.0;
	xp[1] = 0.0;
	xp[2] = 0.0;

	xp1[0] = 0.0;
	xp1[1] = 0.0;
	xp1[2] = 0.0;

	short monolayerIndex	        = 0;	// Pointer to monolayer
	long species			        = 0;	// Counter for polymer types
	long polyNo				        = 0;	// Counter for polymers within a type
	unsigned long polyUpperCounter	= 0;	// Counter for polymers in upper monolayer
	unsigned long polyLowerCounter	= 0;	// Counter for polymers in lower monolayer
	long site				        = 0;	// Randomly-chosen site index
	long lamIndex			        = -1;	// Lamella index 

	for(unsigned long iv=0; iv<m_vBilayer.size(); iv++)
	{
		lamIndex++;

		if(lamIndex == static_cast<long>(m_BilayerCentre.size()))
			lamIndex = 0;

		m_MonolayerHead[0]	= m_BilayerCentre.at(lamIndex) + 0.5*m_Thickness;
		m_MonolayerHead[1]	= m_BilayerCentre.at(lamIndex) - 0.5*m_Thickness;

		if(polyNo == m_PolymerTotal.at(species))
		{
			species++;		// Move to the next polymer type
			polyNo = 0;		// Reset the polymer counter for the next type
		}

		// We still use the variable upperPoly to avoid having to change its name

		CPolymer* upperPoly = m_vBilayer.at(iv);

		if(polyNo < m_PolymerUpperTotal.at(species))
		{
			monolayerIndex = 0;		// Note which monolayer the polymer is in

			// Pick a site in the upper monolayer for the polymer using the 
			// number of polymers in the upper monolayer as an index into a 
			// shuffled vector of site indices.

			if(polyUpperCounter >= m_RandomUpperIndex.size())
				polyUpperCounter = 0;

			site = m_RandomUpperIndex.at(polyUpperCounter++);

			xp[0] = m_UpperXCoord.at(site);
			xp[1] = m_UpperYCoord.at(site);
			xp[2] = 0.0;
		}
		else
		{
			monolayerIndex = 1;

			if(polyLowerCounter >= m_RandomLowerIndex.size())
				polyLowerCounter = 0;

			site = m_RandomLowerIndex.at(polyLowerCounter++);

			xp[0] = m_LowerXCoord.at(site);
			xp[1] = m_LowerYCoord.at(site);
			xp[2] = 0.0;
		}

		// Place the polymer in a lamella
		// Position the beads in the polymers around the head and tail axis.
		// We have to take account of the orientation of the bilayer now.

		if(m_X == 1)
		{
			upperPoly->GetTail()->SetXPos(m_BilayerCentre.at(lamIndex) + m_MonolayerSign[monolayerIndex]*0.2);
			upperPoly->GetTail()->SetYPos(xp[0]);	// shift tails slightly to avoid
			upperPoly->GetTail()->SetZPos(xp[1]);	// creating large forces

			upperPoly->GetHead()->SetXPos(m_MonolayerHead[monolayerIndex]);
			upperPoly->GetHead()->SetYPos(xp[0]);
			upperPoly->GetHead()->SetZPos(xp[1]);

			// Assign coordinates to other beads in the polymers wrapping them around
			// the head-tail axis. If the user has selected the linearise option then 
			// the beads are given coordinates on a straight line joining the polymer
			// head and tail, otherwise they are positioned somwhat randomly about the
			// head-tail axis. If the polymers are in addition linear, the initial 
			// configuration will consist of exactly straight polymers, but if the 
			// polymers are branched only the beads between the head and tail will
			// be straightened.  We check that the random
			// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
			// We don't have to check for their being less than 0 because we only
			// add a random component, and don't subtract it.

			double beadCounter = 1.0;

			for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
			{
				if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
				{
					if(m_bLinearise)
					{
						xp1[0] = upperPoly->GetHead()->GetXPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
						xp1[1] = xp[0];
						xp1[2] = xp[1];
						beadCounter += 1.0;
					}
					else
					{
						xp1[0] = m_BilayerCentre.at(lamIndex) + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();
						xp1[1] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[2] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();

						if(xp1[1] > riState.GetSimBoxYLength())
							xp1[1] -= 0.5*m_Hexe1X[monolayerIndex];

						if(xp1[2] > riState.GetSimBoxZLength())
							xp1[2] -= 0.5*m_Hexe2Y[monolayerIndex];
					}

					(*iterBead2)->SetXPos(xp1[0]);
					(*iterBead2)->SetYPos(xp1[1]);
					(*iterBead2)->SetZPos(xp1[2]);
				}
			}
		}
		else if(m_Y == 1)
		{
			upperPoly->GetTail()->SetXPos(xp[1]);	// shift tails slightly to avoid
			upperPoly->GetTail()->SetYPos(m_BilayerCentre.at(lamIndex) + m_MonolayerSign[monolayerIndex]*0.2);
			upperPoly->GetTail()->SetZPos(xp[0]);	// creating large forces

			upperPoly->GetHead()->SetXPos(xp[1]);
			upperPoly->GetHead()->SetYPos(m_MonolayerHead[monolayerIndex]);
			upperPoly->GetHead()->SetZPos(xp[0]);

			// Assign coordinates to other beads in the polymers wrapping them around
			// the head-tail axis. If the user has selected the linearise option then 
			// the beads are given coordinates on a straight line joining the polymer
			// head and tail, otherwise they are positioned somwhat randomly about the
			// head-tail axis. If the polymers are in addition linear, the initial 
			// configuration will consist of exactly straight polymers, but if the 
			// polymers are branched only the beads between the head and tail will
			// be straightened.  We check that the random
			// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
			// We don't have to check for their being less than 0 because we only
			// add a random component, and don't subtract it.

			double beadCounter = 1.0;

			for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
			{
				if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
				{
					if(m_bLinearise)
					{
						xp1[0] = xp[1];
						xp1[1] = upperPoly->GetHead()->GetYPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
						xp1[2] = xp[0];
						beadCounter += 1.0;
					}
					else
					{
						xp1[0] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[1] = m_BilayerCentre.at(lamIndex) + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();
						xp1[2] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();

						if(xp1[0] > riState.GetSimBoxXLength())
							xp1[0] -= 0.5*m_Hexe2Y[monolayerIndex];

						if(xp1[2] > riState.GetSimBoxZLength())
							xp1[2] -= 0.5*m_Hexe1X[monolayerIndex];
					}

					(*iterBead2)->SetXPos(xp1[0]);
					(*iterBead2)->SetYPos(xp1[1]);
					(*iterBead2)->SetZPos(xp1[2]);
				}
			}
		}
		else if(m_Z == 1)
		{
			upperPoly->GetTail()->SetXPos(xp[0]);	// shift tails slightly to avoid
			upperPoly->GetTail()->SetYPos(xp[1]);	// creating large forces
			upperPoly->GetTail()->SetZPos(m_BilayerCentre.at(lamIndex) + m_MonolayerSign[monolayerIndex]*0.2);

			upperPoly->GetHead()->SetXPos(xp[0]);
			upperPoly->GetHead()->SetYPos(xp[1]);
			upperPoly->GetHead()->SetZPos(m_MonolayerHead[monolayerIndex]);

			// Assign coordinates to other beads in the polymers wrapping them around
			// the head-tail axis. If the user has selected the linearise option then 
			// the beads are given coordinates on a straight line joining the polymer
			// head and tail, otherwise they are positioned somwhat randomly about the
			// head-tail axis. If the polymers are in addition linear, the initial 
			// configuration will consist of exactly straight polymers, but if the 
			// polymers are branched only the beads between the head and tail will
			// be straightened.  We check that the random
			// coordinates do not lie outside the SimBox boundaries in the Y,Z directions.
			// We don't have to check for their being less than 0 because we only
			// add a random component, and don't subtract it.

			double beadCounter = 1.0;

			for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
			{
				if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
				{
					if(m_bLinearise)
					{
						xp1[0] = xp[0];
						xp1[1] = xp[1];
						xp1[2] = upperPoly->GetHead()->GetZPos() - m_MonolayerSign[monolayerIndex]*beadCounter*m_HexCAxisSpace[monolayerIndex];
						beadCounter += 1.0;
					}
					else
					{
						xp1[0] = xp[0] + 0.5*m_Hexe1X[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[1] = xp[1] + 0.5*m_Hexe2Y[monolayerIndex]*CCNTCell::GetRandomNo();
						xp1[2] = m_BilayerCentre.at(lamIndex) + 0.5*m_MonolayerSign[monolayerIndex]*m_Thickness*CCNTCell::GetRandomNo();

						if(xp1[0] > riState.GetSimBoxXLength())
							xp1[0] -= 0.5*m_Hexe1X[monolayerIndex];

						if(xp1[1] > riState.GetSimBoxYLength())
							xp1[1] -= 0.5*m_Hexe2Y[monolayerIndex];
					}

					(*iterBead2)->SetXPos(xp1[0]);
					(*iterBead2)->SetYPos(xp1[1]);
					(*iterBead2)->SetZPos(xp1[2]);
				}
			}
		}

		polyNo++;
	}	// End of loop over all polymers in bilayer

	return true;
}



// **********************************************************************
// Member functions for enclosed class: isMicelle.

// This object assembles the selected polymers into a micelle. The area of 
// the simulation box, centre and radius of the micelle are arguments.
// If multiple polymer types are being used they are assigned coordinates 
// sequentially. This results in different parts of the micelle being 
// occupied by each type of polymer. If a randomised distribution is required 
// we can change the routine later.
//
// This routine does not check that the micelle does not overlap any other 
// structure including the walls. It assumes that this has been done by 
// the calling routine.

CBuilder::isMicelle::isMicelle(const zLongVector polymerTypes, double xc, double yc, double zc,
								double radius) : m_PolymerTypes(polymerTypes),
												 m_XC(xc), m_YC(yc), m_ZC(zc),
												 m_Radius(radius),
												 m_XCentre(0.0),
												 m_YCentre(0.0),
												 m_ZCentre(0.0),
												 m_InnerRadius(0.0),
												 m_OuterRadius(0.0),
												 m_DTheta(0.0),
												 m_DPhi(0.0),
												 m_PolymerTotal(0)
{
	m_vMicelle.clear();
}

CBuilder::isMicelle::~isMicelle()
{
}

bool CBuilder::isMicelle::Assemble(CInitialState& riState)
{
	// Copy the polymers in the CInitialState into a local vector 
	// for ease of access. Also store the number of beads in polymer 
	// types in the micelle. 

	PolymerVector micellePolymers = riState.GetPolymers();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		lastPolymer += riState.GetPolymerTotalForType(ip);

		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) != m_PolymerTypes.end())
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(micellePolymers.begin()+firstPolymer, micellePolymers.begin()+lastPolymer, back_inserter(m_vMicelle));
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	m_PolymerTotal = m_vMicelle.size();	// Total of all polymer types in micelle

	// ****************************************
	// Arrange the polymers with their heads on the surface of a sphere
	// and their tails on the surface of a smaller concentric sphere. This
	// avoids the tail beads all being located at a single point.
	// Because the spheres are concentric we only have to calculate the
	// angular coordinates of the lipids once and then adjust the radius
	// for the inner and outer spheres. We use spherical polar coordinates 
	// to organise the lipids in a roughly hexagonal arrangement on the 
	// sphere's surface. 
	//
	// The theta coordinate determines how many polymer heads are positioned
	// in the circle of latitude specified by it, and the phi coordinate
	// varies with theta as 1/sin(theta).
	//
	// The micelle's centre is located at a position set by the values 
	// m_XC, m_YC, m_ZC as fractions of the SimBox side lengths. Note that
	// we use the whole SimBox size including any wall thicknesses. This places
	// the micelle centre in the actual centre of the box. The calling routine
	// must ensure the micelle does not overlap any walls present.

	m_XCentre		= m_XC*riState.GetSimBoxXLength();
	m_YCentre		= m_YC*riState.GetSimBoxYLength();
	m_ZCentre		= m_ZC*riState.GetSimBoxZLength();
	m_InnerRadius	= exp(log(static_cast<double>(m_PolymerTotal)/8.0)/3.0);
	m_OuterRadius	= m_InnerRadius + m_Radius;
	m_DTheta		= sqrt(4.0*xxBase::m_globalPI/static_cast<double>(m_PolymerTotal));
	m_DPhi			= m_DTheta; // not including the sin(theta) term

	// ****************************************

	// Loop over the number of lipids in the micelle and assign coordinates to 
	// the polymers with their tails positioned at the coordinates in the 
	// inner sphere, and their head coordinates positioned directly above 
	// the tails using the m_Radius parameter.
	//
	// Also, set the copies of the bead coordinates using SetunPBCXPos(), 
	// SetInitialXPos().

	double xt[3];
	double xp1[3];

	// Divide up the available radial distance into segments whose length is
	// set by the number of beads in the polymer. We reduce it by one to
	// allow the head and tail to be positioned exactly on the inner and outer
	// radii, and the others in between.

	const double polySize = static_cast<double>(m_vMicelle.at(0)->GetSize());
	const double deltaR = (m_OuterRadius - m_InnerRadius)/(polySize-1.0);

	double theta = m_DTheta;
	double phi   = 0.0;

	for(long iv=0; iv<m_PolymerTotal; iv++)
	{
		CPolymer* upperPoly = m_vMicelle.at(iv);

		xt[0] = sin(theta)*cos(phi);
		xt[1] = sin(theta)*sin(phi);
		xt[2] = cos(theta);

		// Position the head and tail beads first in the outer and inner spheres

		upperPoly->GetHead()->SetXPos(m_XCentre + m_OuterRadius*xt[0]);
		upperPoly->GetHead()->SetYPos(m_YCentre + m_OuterRadius*xt[1]);
		upperPoly->GetHead()->SetZPos(m_ZCentre + m_OuterRadius*xt[2]);

		upperPoly->GetTail()->SetXPos(m_XCentre + m_InnerRadius*xt[0]);
		upperPoly->GetTail()->SetYPos(m_YCentre + m_InnerRadius*xt[1]);
		upperPoly->GetTail()->SetZPos(m_ZCentre + m_InnerRadius*xt[2]);

		// Assign coordinates to other beads in the polymers wrapping them around
		// the head-tail axis somewhat randomly. We check that these random
		// coordinates do not lie outside the SimBox boundaries in the X,Y directions.
		// We don't have to check for their being less than 0 because we only
		// add a random component, and don't subtract it.

		double counter = 0.0;
		double dist = 0.0;

		for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
		{
			if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
			{
				counter += 1.0;
				dist = m_InnerRadius + counter*deltaR*(1.0 - CCNTCell::GetRandomNo());

				xp1[0] = dist*xt[0];
				xp1[1] = dist*xt[1];
				xp1[2] = dist*xt[2];

				(*iterBead2)->SetXPos(m_XCentre + xp1[0]);
				(*iterBead2)->SetYPos(m_YCentre + xp1[1]);
				(*iterBead2)->SetZPos(m_ZCentre + xp1[2]);
			}
		}

		phi += m_DPhi/sin(theta);

		if(phi > xxBase::m_globalTwoPI)
		{
			theta += m_DTheta;
			phi = 0.0;
		}
	}

	return true;
}

// **********************************************************************
// Member functions for enclosed class: isWormMicelle.

// This object assembles the selected polymers into a worm-like micelle. 
// The area of the simulation box, centre and radius of the worm are arguments.
// If multiple polymer types are being used they are assigned coordinates 
// sequentially. This results in different parts of the micelle being 
// occupied by each type of polymer. If a randomised distribution is required 
// we can change the routine later.
//
// This routine does not check that the worm does not overlap any other 
// structure including the walls. It assumes that this has been done by 
// the calling routine.

CBuilder::isWormMicelle::isWormMicelle(const zLongVector polymerTypes, long xn, long yn, long zn,
					  double xc, double yc, double zc, double radius, double halfLength, 
					  bool bPatches, bool bPolymerise) : m_PolymerTypes(polymerTypes),
										 m_XN(xn), m_YN(yn), m_ZN(zn),
										 m_XC(xc), m_YC(yc), m_ZC(zc),
										 m_Radius(radius),
										 m_HalfLength(halfLength),
										 m_bPatches(bPatches),
										 m_bPolymerise(bPolymerise),
										 m_XCentre(0.0),
										 m_YCentre(0.0),
										 m_ZCentre(0.0),
										 m_LeftEnd(0.0),
										 m_RightEnd(0.0),
										 m_Length(0.0),
										 m_Circumference(0.0),
										 m_InnerRadius(0.0),
										 m_OuterRadius(0.0),
										 m_Area(0.0),
										 m_DZ(0.0),
										 m_DPhi(0.0),
										 m_PolymerTotal(0),
										 m_LatticeSize(0),
										 m_HexSpace(0.0),
										 m_Hexe1X(0.0),
										 m_Hexe1Y(0.0),
										 m_Hexe2X(0.0),
										 m_Hexe2Y(0.0)
{
	m_vMicelle.clear();
	m_RandomIndex.clear();

	m_Axis[0] = 0.0;
	m_Axis[1] = 0.0;
	m_Axis[2] = 0.0;

}

CBuilder::isWormMicelle::~isWormMicelle()
{
}

bool CBuilder::isWormMicelle::Assemble(CInitialState& riState)
{
	// Copy the polymers in the CInitialState into a local vector 
	// for ease of access. Also store the number of beads in polymer 
	// types in the micelle. 

	PolymerVector micellePolymers = riState.GetPolymers();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		lastPolymer += riState.GetPolymerTotalForType(ip);

		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) != m_PolymerTypes.end())
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(micellePolymers.begin()+firstPolymer, micellePolymers.begin()+lastPolymer, back_inserter(m_vMicelle));
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	m_PolymerTotal = m_vMicelle.size();	// Total of all polymer types in micelle

	// ****************************************
	// Arrange the polymers with their heads on the surface of a cylinder
	// and their tails on surface of a smaller cylinder near the axis. This
	// avoids the tail beads all being located at a single point.
	// Because the cylinders are concentric we only have to calculate the
	// angular coordinates of the lipids once and then adjust the radius
	// for the inner and outer cylinders. We use cylindrical polar coordinates 
	// to organise the lipids in a roughly hexagonal arrangement on the 
	// cylinders' surfaces. 
	//
	// The cylinder's centre is located at a position set by the values 
	// m_XC, m_YC, m_ZC as fractions of the SimBox side lengths.

	m_XCentre		= m_XC*riState.GetSimBoxXLength();
	m_YCentre		= m_YC*riState.GetSimBoxYLength();
	m_ZCentre		= m_ZC*riState.GetSimBoxZLength();

	// The end points of the cylinder depend on its orientation: note that we
	// assume the orientation is only along one of the major axes. This must have
	// been checked before calling this function. We set the absolute coordinates of
	// the first lattice site here as they depend on the orientation of the cylinder.

	if(m_XN == 1)
	{
		m_LeftEnd  = m_XCentre - m_HalfLength*riState.GetSimBoxXLength();
		m_RightEnd = m_XCentre + m_HalfLength*riState.GetSimBoxXLength();

		// Check we don't go outside the SimBox boundaries
		if(m_LeftEnd < 0.001)
			m_LeftEnd = 0.001;
		if(m_RightEnd > riState.GetSimBoxXLength())
			m_RightEnd = 0.999*riState.GetSimBoxXLength();

		// Store the coordinates of the first circle on the axis
		m_Axis[0] = m_LeftEnd;
		m_Axis[1] = m_YCentre;
		m_Axis[2] = m_ZCentre;
	}
	else if(m_YN == 1)
	{
		m_LeftEnd  = m_YCentre - m_HalfLength*riState.GetSimBoxYLength();
		m_RightEnd = m_YCentre + m_HalfLength*riState.GetSimBoxYLength();

		if(m_LeftEnd < 0.001)
			m_LeftEnd = 0.001;
		if(m_RightEnd > riState.GetSimBoxYLength())
			m_RightEnd = 0.999*riState.GetSimBoxYLength();

		m_Axis[0] = m_XCentre;
		m_Axis[1] = m_LeftEnd;
		m_Axis[2] = m_ZCentre;
	}
	else
	{
		m_LeftEnd  = m_ZCentre - m_HalfLength*riState.GetSimBoxZLength();
		m_RightEnd = m_ZCentre + m_HalfLength*riState.GetSimBoxZLength();

		if(m_LeftEnd < 0.001)
			m_LeftEnd = 0.001;
		if(m_RightEnd > riState.GetSimBoxZLength())
			m_RightEnd = 0.999*riState.GetSimBoxZLength();

		m_Axis[0] = m_XCentre;
		m_Axis[1] = m_YCentre;
		m_Axis[2] = m_LeftEnd;
	}

	// Fix the inner diameter to unity so that the terminal tail beads do not
	// all overlap. This assumes that the bead radii are not too different from
	// unity. Note that we don't modify that specified cylinder radius even though
	// the tail beads are slightly off axis.

	m_InnerRadius	= 0.5;
	m_OuterRadius	= m_Radius;
	m_Length		= m_RightEnd - m_LeftEnd;
	m_Circumference = xxBase::m_globalTwoPI*m_Radius;
	m_Area			= m_Circumference*m_Length;

	// Now calculate the spacing for a hexagonal lattice of polymers on the
	// surface of the cylinder

	m_LatticeSize = m_PolymerTotal;

	m_HexSpace = 0.999*sqrt(1.1547*m_Area/static_cast<double>(m_LatticeSize));

	m_Hexe1X	= m_HexSpace;			// e1 lattice vector
	m_Hexe2X	= 0.5*m_HexSpace;		// e2 lattice vector
	m_Hexe2Y	= 0.866025*m_HexSpace;

	long noPerRow = static_cast<long>(m_Circumference/m_Hexe2Y);
	long rowTotal = static_cast<long>(m_LatticeSize/noPerRow);

	// Add an extra row at the end to prevent the last row of beads crossing
	// the SimBox boundary due to discretization error in calculating the bead
	// coordinates. We used to do this only if extra lattice sites were needed
	// but now we add one more row anyway.

//	if(noPerRow*rowTotal < m_LatticeSize)
		rowTotal++;

	// Calculate the z spacing shift and angular spacing around each row in radians.
	// I changed the routine from using the m_Hexe1X parameter as the spacing
	// to calculating the spacing from the length of the worm and the number
	// of rows.

	m_DZ = m_Length/static_cast<double>(rowTotal);
	m_DPhi = xxBase::m_globalTwoPI/static_cast<double>(noPerRow);

	// ****************************************
	// Create the vectors holding indices to the coordinates of free lattice 
	// sites at which polymers may be placed. As polymers are positioned,
	// the corresponding sites are removed from the vectors allowing a 
	// simple random choice from the remaining elements. 

	// Note that these vectors are relative: we need to adjust them for the actual
	// location of the cylinder in the SimBox. However, we only use the m_RandomIndex
	// to shuffle the coordinates, and do not make use of the hexp[] coordinates
	// except to check if we run out of coordinates in the lattice.

	double hexp[2];

	hexp[0] = 0.001*m_Hexe1X;	// We use an offset to avoid positions coinciding with boundaries
	hexp[1] = 0.001*m_Hexe2Y;

	bool bEvenRow = true;

	for(long icu=0; icu<m_LatticeSize; icu++)
	{
		m_RandomIndex.push_back(icu);
		hexp[0] += m_Hexe1X;

		if(hexp[0] > m_Circumference)
		{
			if(bEvenRow)
				hexp[0] = m_Hexe2X;
			else
				hexp[0] = 0.0;

			hexp[1] += m_Hexe2Y;

			// If we have run out of area it is an error

			if(hexp[1] >= m_Length)
			{
				new CLogBuilderError(0, "worm micelle lattice outside SimBox");
				return false;
			}

			bEvenRow = !bEvenRow;
		}
	}

	// Randomise the polymers if required, otherwise they occur in patches
	// in the order they are defined

	if(!m_bPatches)
		random_shuffle_wrapper(m_RandomIndex.begin(), m_RandomIndex.end());

	// Loop over the number of lipids in the micelle and assign coordinates to 
	// the polymers with their tails positioned at the coordinates in the 
	// inner cylinder, and their head coordinates positioned directly above 
	// the tails using the m_Radius parameter.
	//
	// Also, set the copies of the bead coordinates using SetunPBCXPos(), 
	// SetInitialXPos().

	double xt[3];
	double xp1[3];

	// Divide up the available radial distance into segments whose length is
	// set by the number of beads in the polymer. We reduce it by one to
	// allow the head and tail to be positioned exactly on the inner and outer
	// radii, and the others in between.

	const double polySize = static_cast<double>(m_vMicelle.at(0)->GetSize());
	const double deltaR = (m_OuterRadius - m_InnerRadius)/(polySize-1.0);

	double phi    = 0.0;
	long polyCounter = 0;

	for(long iv=0; iv<m_PolymerTotal; iv++)
	{
		CPolymer* upperPoly = m_vMicelle.at(m_RandomIndex.at(polyCounter++));

		if(m_XN == 1)
		{
			xt[0] = 0.0; 
			xt[1] = cos(phi);
			xt[2] = sin(phi);
		}
		else if(m_YN == 1)
		{
			xt[0] = sin(phi); 
			xt[1] = 0.0;
			xt[2] = cos(phi);
		}
		else
		{
			xt[0] = cos(phi);
			xt[1] = sin(phi);
			xt[2] = 0.0;
		}

		// Position the head and tail beads first

		upperPoly->GetHead()->SetXPos(m_Axis[0] + m_OuterRadius*xt[0]);
		upperPoly->GetHead()->SetYPos(m_Axis[1] + m_OuterRadius*xt[1]);
		upperPoly->GetHead()->SetZPos(m_Axis[2] + m_OuterRadius*xt[2]);

		upperPoly->GetTail()->SetXPos(m_Axis[0] + m_InnerRadius*xt[0]);
		upperPoly->GetTail()->SetYPos(m_Axis[1] + m_InnerRadius*xt[1]);
		upperPoly->GetTail()->SetZPos(m_Axis[2] + m_InnerRadius*xt[2]);

		// Assign coordinates to other beads in the polymers wrapping them around
		// the head-tail axis somewhat randomly. We check that these random
		// coordinates do not lie outside the SimBox boundaries in the X,Y directions.
		// We don't have to check for their being less than 0 because we only
		// add a random component, and don't subtract it.

		double counter = 0.0;
		double dist = 0.0;

		for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
		{
			if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
			{
				counter += 1.0;
				dist = m_InnerRadius + counter*deltaR*(1.0 - CCNTCell::GetRandomNo());

				xp1[0] = dist*xt[0];
				xp1[1] = dist*xt[1];
				xp1[2] = dist*xt[2];

				(*iterBead2)->SetXPos(m_Axis[0] + xp1[0]);
				(*iterBead2)->SetYPos(m_Axis[1] + xp1[1]);
				(*iterBead2)->SetZPos(m_Axis[2] + xp1[2]);
			}
		}

		phi += m_DPhi;

		if(phi > xxBase::m_globalTwoPI)
		{
			phi = 0.0;
			if(m_XN == 1)
			{
				m_Axis[0] += m_DZ;
			}
			else if( m_YN == 1)
			{
				m_Axis[1] += m_DZ;
			}
			else
			{
				m_Axis[2] += m_DZ;
			}
		}
	}

	return true;
}

// **********************************************************************
// Member functions for enclosed class: isFibril.

// This object assembles the selected polymers into a rigid, linear fibril with its long axis
// along the Z axis leaving no space at its ends. It is intended to be used to analyse the
// interactions of mobile elements with the surface of the fibril. The first polymer type specified
// must be the core polymer that composes the fibril, and the second one must be the GFP linker polymer.
// There should only be one linker polymer per monomer in the fibril. But this can be changed by the user.
// It may be interesting to have two GFP-linkers per monomer.
//
// It arranges the polymers in the fibril along the Z axis at a fixed separation and,
// if specified, a fixed pitch angle. This places them in a helical arrangement. The portions of
// the polymers within the fibril core are frozen but the remainder of the polymer shape is allowed to
// fluctuate in the solvent around the fibril.
//
// This routine does not check that the fibril does not overlap any other
// structure including the walls. It assumes that this has been done by
// the calling routine.

CBuilder::isFibril::isFibril(const zLongVector polymerTypes, long coreBeadType, long linkerBeadType,
                             long xn, long yn, long zn, double xc, double yc, double zc, double halfLength,
                             double coreRadius, double linkerRadius, double pitch, double pitchAngle) :
                            m_PolymerTypes(polymerTypes), m_CoreBeadType(coreBeadType), m_LinkerBeadType(linkerBeadType),
                            m_XN(0), m_YN(0), m_ZN(1),
                            m_XC(0.5), m_YC(0.5), m_ZC(0.5),
                            m_HalfLength(0.5), m_CoreRadius(coreRadius), m_LinkerRadius(linkerRadius),
                            m_Pitch(pitch), m_PitchAngle(pitchAngle), 
                            m_XCentre(0.0),
                            m_YCentre(0.0),
                            m_ZCentre(0.0),
                            m_LeftEnd(0.0),
                            m_RightEnd(0.0),
                            m_Length(0.0),
                            m_Circumference(0.0),
                            m_InnerRadius(0.0),
                            m_OuterRadius(0.0),
                            m_Area(0.0),
                            m_DZ(0.0),
                            m_DPhi(0.0),
                            m_CoreTotal(0),
                            m_LinkerTotal(0),
                            m_LatticeSize(0),
                            m_HexSpace(0.0),
                            m_Hexe1X(0.0),
                            m_Hexe1Y(0.0),
                            m_Hexe2X(0.0),
                            m_Hexe2Y(0.0)
{
    m_vFibril.clear();
    m_vLinker.clear();
    m_RandomIndex.clear();

    m_Axis[0] = 0.0;
    m_Axis[1] = 0.0;
    m_Axis[2] = 0.0;
}

CBuilder::isFibril::~isFibril()
{
}

bool CBuilder::isFibril::Assemble(CInitialState& riState)
{
    // Copy the polymers in the CInitialState into a local vector
    // for ease of access. Also store the number of beads in polymer
    // types in the fibril. We separate the core polymers from the linker polymers. The core polymers consist
    // only of one bead type and form the bulk of the fibril. The linker polymers have the same core bead type
    // but also a linker region attached with possibly a mobile GFP at the end.

    PolymerVector allFibrilPolymers = riState.GetPolymers();

    long firstPolymer = 0;
    long lastPolymer  = 0;
    
    for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
    {
        lastPolymer += riState.GetPolymerTotalForType(ip);
        
        if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) != m_PolymerTypes.end())
        {
            // Copy the polymer pointers for the required polymers into local storage.
            // We use a back_inserter to avoid having to manually resize the container.
            // The first type is assumed to be the core polymers and the second type is the linker polymer.
            
            if(ip == m_PolymerTypes.at(0))
            {
                copy(allFibrilPolymers.begin()+firstPolymer, allFibrilPolymers.begin()+lastPolymer, back_inserter(m_vFibril));
            }
            else if(ip == m_PolymerTypes.at(1))
            {
                copy(allFibrilPolymers.begin()+firstPolymer, allFibrilPolymers.begin()+lastPolymer, back_inserter(m_vLinker));
            }
        }

        firstPolymer += riState.GetPolymerTotalForType(ip);
    }

    m_CoreTotal   = m_vFibril.size();
    m_LinkerTotal = m_vLinker.size();

    // Find the number of core and linker beads in each polymer.
    
    BeadVector vFibrilBeads = m_vFibril.at(0)->GetBeads();
    
    long coreTotal, linkerTotal;
    coreTotal = 0;
    linkerTotal = 0;
    
    for(BeadVectorIterator iterBead=vFibrilBeads.begin(); iterBead!=vFibrilBeads.end(); ++iterBead)
    {
        if((*iterBead)->GetType() == m_CoreBeadType)
            ++coreTotal;
        else if ((*iterBead)->GetType() == m_LinkerBeadType)
            ++linkerTotal;
    }
    
    std::cout << "Building fibril out of " << m_CoreTotal << " core polymers, and " << m_LinkerTotal << " linker polymers" << zEndl;
    std::cout << "Fibril normal               = " << m_XN << " " << m_YN << " " << m_ZN << zEndl;
    std::cout << "Fibril centre               = " << m_XC << " " << m_YC << " " << m_ZC << zEndl;
    std::cout << "Fibril halflength           = " << m_HalfLength << zEndl;
    std::cout << "Core and linker radii       = " << m_CoreRadius << " " << m_LinkerRadius << zEndl;
    std::cout << "Pitch and angle             = " << m_Pitch << " " << m_PitchAngle << zEndl;

    // ****************************************
    // Arrange the polymers with their first core beads at the centre of the fibril
    // and their last core bead on the surface of the cylinder with radius coreRadius.
    // We don't mind if the inner beads overlap as they are going to be frozen inside the core.
    // Because the cylinders are concentric we only have to calculate the
    // angular coordinates of the polymers once and then adjust the radius
    // for the inner and outer cylinders. We use cylindrical polar coordinates
    // to organise them in a roughly hexagonal arrangement on the
    // cylinders' surfaces.
    //
    // The cylinder's centre is located at a position set by the values
    // m_XC, m_YC, m_ZC as fractions of the SimBox side lengths.

    m_XCentre        = m_XC*riState.GetSimBoxXLength();
    m_YCentre        = m_YC*riState.GetSimBoxYLength();
    m_ZCentre        = m_ZC*riState.GetSimBoxZLength();

    // The end points of the cylinder depend on its orientation: note that we
    // assume the orientation is only along the Z axis.

    m_LeftEnd  = m_ZCentre - m_HalfLength*riState.GetSimBoxZLength();
    m_RightEnd = m_ZCentre + m_HalfLength*riState.GetSimBoxZLength();

    if(m_LeftEnd < 0.001)
        m_LeftEnd = 0.001;
    if(m_RightEnd > riState.GetSimBoxZLength())
        m_RightEnd = 0.999*riState.GetSimBoxZLength();

    m_Axis[0] = m_XCentre;
    m_Axis[1] = m_YCentre;
    m_Axis[2] = m_LeftEnd;
    
    // Fix the inner diameter to a fraction of the core radius so the beads are not identically overlapping

    m_InnerRadius    = 0.5*m_CoreRadius;
    m_Length        = m_RightEnd - m_LeftEnd;
    m_Circumference = xxBase::m_globalTwoPI*m_CoreRadius;
    m_Area            = m_Circumference*m_Length;

    std::cout << "Creating fibril with axis coords: " << m_Axis[0] << " " << m_Axis[1] << " " << m_Axis[2] << zEndl;
    std::cout << "Length, pitch and pitch angle: "    << m_Length << " " << m_Pitch << " " << m_PitchAngle << zEndl;
    std::cout << " and left and right ends: " << m_LeftEnd << " " << m_RightEnd << zEndl;
    std::cout << " and inner and outer radii: " << m_InnerRadius << " " << m_CoreRadius << zEndl;
    
    // Fix the number of rows by the box size and pitch, and then set the number of polymers per row by spacing
    // them equally around the circumference of the rows. Note that the number of polymers per row should be chosen
    
    // so that the last core beads of the polymers are fairly close to each other. Otherwise, the fibril's surface will
    // have gaps between the polymers.
    
    m_DZ = m_Pitch;
    
    long rowTotal = static_cast<long>((m_Length+0.5)/m_DZ);
    long polyPerRow = static_cast<long>(0.5 + static_cast<double>(m_CoreTotal + m_LinkerTotal)/static_cast<double>(rowTotal));
    
    std::cout << "Estimated row total and polymers per row = " << rowTotal << " " << polyPerRow << zEndl;
    
    // Adjust the row total upwards if there are polymers left over once the estimated number of rows are filled.
    // We ignore too few polymers as it is not a problem.
    
    const long excessPoly = m_CoreTotal + m_LinkerTotal - rowTotal*polyPerRow;
    
    if(excessPoly > 0)
    {
        rowTotal += (1 + excessPoly/polyPerRow);
    }
    m_DPhi =xxBase::m_globalTwoPI/static_cast<double>(polyPerRow);
    
    std::cout << "Actual row total and polymers per row  " << rowTotal << " " << polyPerRow << " " << m_DZ << " " << m_DPhi << zEndl;


    // Loop over the number of polymers in the fibril and assign coordinates to
    // the polymers with their tails positioned at the coordinates in the
    // inner cylinder, and their head coordinates positioned directly above
    // the tails using the m_CoreRadius parameter.
    //
    // Also, set the copies of the bead coordinates using SetunPBCXPos(),
    // SetInitialXPos().

    double xt[3];
    double xp1[3];

    // Divide up the available radial distance into segments whose length is
    // set by the number of core beads in the polymer. We reduce it by one to
    // allow the first core bead and first linker bead to be positioned exactly
    // on the inner and outer radii, and the others in between.

    const double deltaR = (m_CoreRadius - m_InnerRadius)/static_cast<double>(coreTotal);
    
    double phi         = 0.0;
    long coreCounter   = 0;
    long linkerCounter = 0;
   
    for(long irow=0; irow < rowTotal; ++irow)
    {
        // Place the linker polymer first
        
        if(linkerCounter < m_LinkerTotal)
        {
            CPolymer* linkerPoly = m_vLinker.at(linkerCounter++);

//            std::cout << "Placing row " << irow << " linker polymer " << linkerCounter << " out of total " << m_vLinker.size() << zEndl;

            double clCounter = 0.0;
            double lCounter  = 0.0;
            double mCounter  = 0.0;
            double lDist     = 0.0;
                  
            for(cBeadVectorIterator iterBead3=(linkerPoly->GetBeads().begin()); iterBead3!=linkerPoly->GetBeads().end(); iterBead3++)
            {
                if((*iterBead3)->GetType() == m_CoreBeadType)
                {
                    clCounter += 1.0;
                    lDist = m_InnerRadius + clCounter*deltaR*(1.0 + 0.05*(1.0 - CCNTCell::GetRandomNo()));
                    
                    xp1[0] = lDist*cos(phi);
                    xp1[1] = lDist*sin(phi);
        
                    (*iterBead3)->SetXPos(m_Axis[0] + xp1[0]);
                    (*iterBead3)->SetYPos(m_Axis[1] + xp1[1]);
                    (*iterBead3)->SetZPos(m_Axis[2]);
                           
                    (*iterBead3)->SetFrozen();
                }
                else if((*iterBead3)->GetType() == m_LinkerBeadType)
                {
                    lCounter += 1.0;
                    lDist = m_CoreRadius + lCounter*deltaR*(1.0 + 0.05*(1.0 - CCNTCell::GetRandomNo()));

                    xp1[0] = lDist*cos(phi);
                    xp1[1] = lDist*sin(phi);
                              
                    (*iterBead3)->SetXPos(m_Axis[0] + xp1[0]);
                    (*iterBead3)->SetYPos(m_Axis[1] + xp1[1]);
                    (*iterBead3)->SetZPos(m_Axis[2]);
                }
                else
                {
                    // All other beads in the linker polymer are randomly positioned in the bulk solvent.
                    // If this segment is large, e.g., the GFP domain, placing them all in a straight line
                    // can move them outside the SimBox. So we try and place them in an isotropic distribution
                    // around the last linker bead. We restrict the Z coordinate adjustment as the final row of
                    // polymers may be very close to the the SimBox boundary, and we don't want to push them
                    // outside the box.
                          
                    mCounter += 1.0;
                    lDist = m_CoreRadius + m_LinkerRadius + deltaR;

                    xp1[0] = lDist*cos(phi);
                    xp1[1] = lDist*sin(phi);
                    
                    double newXPos = m_Axis[0] + xp1[0] + 5.0*deltaR*(1.0 - CCNTCell::GetRandomNo());
                    double newYPos = m_Axis[1] + xp1[1] + 5.0*deltaR*(1.0 - CCNTCell::GetRandomNo());
                    double newZPos = m_Axis[2] + 0.05*deltaR*(1.0 - CCNTCell::GetRandomNo());

                    const long coordAdjustmentLimit = 20;
                    long coordCounter = 0;
                    
                    while( newXPos > (riState.GetSimBoxXLength() - deltaR) && coordCounter++ < coordAdjustmentLimit)
                    {
                        newXPos -= deltaR*(1.0 - 2.0*CCNTCell::GetRandomNo());
                    }
                    
                    if(coordCounter == coordAdjustmentLimit && newXPos > riState.GetSimBoxXLength())
                    {
                        std::cout << "Linker X coordinate outside box" << zEndl;
                    }
                    
                    coordCounter = 0;
                    while( newYPos > (riState.GetSimBoxYLength() - deltaR) && coordCounter++ < coordAdjustmentLimit)
                    {
                        newYPos -= deltaR*(1.0 - 2.0*CCNTCell::GetRandomNo());
                    }
                    
                    if(coordCounter == coordAdjustmentLimit && newYPos > riState.GetSimBoxYLength())
                    {
                        std::cout << "Linker Y coordinate outside box" << zEndl;
                    }

                    coordCounter = 0;
                    while( newZPos > (riState.GetSimBoxZLength() - deltaR) && coordCounter++ < coordAdjustmentLimit)
                    {
                        newZPos -= deltaR*(1.0 - 2.0*CCNTCell::GetRandomNo());
                    }
                    
                    if(coordCounter == coordAdjustmentLimit && newZPos > riState.GetSimBoxZLength())
                    {
                        std::cout << "Linker Z coordinate outside box" << zEndl;
                    }

                    (*iterBead3)->SetXPos(newXPos);
                    (*iterBead3)->SetYPos(newYPos);
                    (*iterBead3)->SetZPos(newZPos);
                }
            }
            
            phi += m_DPhi;
        }
        else
        {
            std::cout << "no more linker polymers" << zEndl;
        }

        for(long iphi=0; iphi < polyPerRow-1; ++iphi)
        {
            if(coreCounter < m_CoreTotal)
            {
 //               std::cout << "Placing row " << irow << " core polymer " << coreCounter << " out of total " << m_vFibril.size() << zEndl;
            
                CPolymer* corePoly = m_vFibril.at(coreCounter++);
                
                xt[0] = cos(phi);
                xt[1] = sin(phi);
                xt[2] = 0.0;   // This should be adjusted for the fibril pitch

                // Assign coordinates to the beads in the core polymers wrapping them around
                // the head-tail axis somewhat randomly. We check that these random
                // coordinates do not lie outside the SimBox boundaries in the X,Y directions.
                // We don't have to check for their being less than 0 because we only
                // add a random component, and don't subtract it.

                double counter = 0.0;
                double dist = 0.0;

                for(cBeadVectorIterator iterBead2=(corePoly->GetBeads().begin()); iterBead2!=corePoly->GetBeads().end(); iterBead2++)
                {
                    // We position the core beads along the fibril but freeze only the core beads; core polymers should not have
                    // any other bead types.
            
                    if((*iterBead2)->GetType() == m_CoreBeadType)
                    {
                        counter += 1.0;
                        dist = m_InnerRadius + counter*deltaR*(1.0 + 0.05*(1.0 - CCNTCell::GetRandomNo()));

                        xp1[0] = dist*xt[0];
                        xp1[1] = dist*xt[1];
                        xp1[2] = dist*xt[2];
                        
                        (*iterBead2)->SetXPos(m_Axis[0] + xp1[0]);
                        (*iterBead2)->SetYPos(m_Axis[1] + xp1[1]);
                        (*iterBead2)->SetZPos(m_Axis[2]);            // Only increment Z coordinate when starting new row
                
                        (*iterBead2)->SetFrozen();
                    }
                    else
                    {
                        std::cout << "This should not be seen as the core polymers have only core beads" << zEndl;
                    }
                }
            }
                
            phi += m_DPhi;
        }
        
        // Before we increment the Z axis along the fibril, we increment the pitch angle
        
        phi += m_PitchAngle;
        phi -= xxBase::m_globalTwoPI;
        
        m_Axis[2] += m_DZ;
        
        if(m_Axis[2] > m_RightEnd - 0.5)
            m_Axis[2] =m_RightEnd - 0.5;
    }
    
    return true;
}



// **********************************************************************
// Member functions for enclosed class: isVesicle.

// This object assembles the selected polymers into a bilayer vesicle. The 
// vesicle's centre and outer radius are arguments.
// If multiple polymer types are being used they are assigned coordinates 
// sequentially. This results in different parts of the vesicle being occupied 
// by each type of polymer. If a randomised distribution is required we can 
// change the routine later.
//
// This routine does not check that the vesicle does not overlap any other
// structure including the walls. It assumes that this has been done by the 
// calling routine.

CBuilder::isVesicle::isVesicle(const zLongVector polymerTypes, const zDoubleVector outerFractions,
							   double xc, double yc, double zc, double outerRadius, 
							   double innerRadius, double thickness, 
							   bool bPatches[2]) : m_PolymerTypes(polymerTypes),
													m_OuterFractions(outerFractions),
													m_XC(xc), m_YC(yc), m_ZC(zc),
													m_OuterRadius(outerRadius),
													m_InnerRadius(innerRadius),
													m_Thickness(thickness),
													m_AllPolymerTotal(0),
													m_PolymerMaxSize(0),
													m_XCentre(0.0),
													m_YCentre(0.0),
													m_ZCentre(0.0)
{
	m_bPatches[0] = bPatches[0];
	m_bPatches[1] = bPatches[1];

	// Store data used in loops over the two monolayers. Notice that we
	// increase the outer radius and decrease the inner radius slightly
	// to avoid polymer tail beads overlapping at the bilayer midplane.
	// The m_MonolayerSign[] array is used to position beads between the
	// polymer head and tail in the appropriate direction for each monolayer.
	// The mean number of polymers in each monolayer is calculated from the
	// vesicle radius and bilayer thickness assuming equal area-per-polymer
	// for both. The actual number of polymers is determined from the user-defined
	// number fractions and inner/outer fractions, and then used to calculate 
	// the actual area-per-polymers. If these do not agree with the relaxed values,
	// the vesicle will be under strain.

	m_vVesicle.clear();
	m_PolymerTotal.clear();
	m_PolymerOuterTotal.clear();
	m_PolymerSize.clear();
	m_RandomOuterIndex.clear();
	m_RandomInnerIndex.clear();
	m_OuterTheCoord.clear();
	m_OuterPhiCoord.clear();
	m_InnerTheCoord.clear();
	m_InnerPhiCoord.clear();

	m_MeanMonolayerTotal[0]	= 0;	// Relaxed number of polymers in outer monolayer
	m_MeanMonolayerTotal[1]	= 0;
	m_MonolayerTotal[0]		= 0;	// Actual polymers in outer monolayer
	m_MonolayerTotal[1]		= 0;
	m_MonolayerSign[0]		= -1.0;
	m_MonolayerSign[1]		= 1.0;
	m_MonolayerRadius[0]	= m_OuterRadius;
	m_MonolayerRadius[1]	= m_InnerRadius;
	m_BilayerRadius[0]		= 1.01*(m_OuterRadius - 0.5*m_Thickness);
	m_BilayerRadius[1]		= 0.99*(m_InnerRadius + 0.5*m_Thickness);
	m_HexCAxisSpace[0]		= 0.0;
	m_HexCAxisSpace[1]		= 0.0;
    m_DTheta[0]				= 0.0;
    m_DTheta[1]				= 0.0;
    m_DPhi[0]				= 0.0;
    m_DPhi[1]				= 0.0;

}

CBuilder::isVesicle::~isVesicle()
{
}

bool CBuilder::isVesicle::Assemble(CInitialState& riState)
{
	// Copy the polymers in the CInitialState into a local vector for ease of access.
	// We loop over all types of polymer adding them to a local vector if
	// their type occurs in the data for a composite vesicle. Then the number of
	// polymers of each type that are in the outer monolayer, as determined by the
	// user-defined m_OuterFractions vector, are calculated. The number of beads
	// in each type of polymer is also stored.

	PolymerVector vesiclePolymers = riState.GetPolymers();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		long polyNo = riState.GetPolymerTotalForType(ip);

		lastPolymer += polyNo;

		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) != m_PolymerTypes.end())
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(vesiclePolymers.begin()+firstPolymer, vesiclePolymers.begin()+lastPolymer, back_inserter(m_vVesicle));

			// Store the number of this type of polymer and the number in the outer 
			// monolayer. We subtract the upper number from the total number to
			// ensure that no polymers are missed due to rounding of the upper 
			// fraction.
			
			m_PolymerTotal.push_back(polyNo);
		
			for(unsigned long polyIndex=0; polyIndex<m_PolymerTypes.size(); polyIndex++)
			{
				if(m_PolymerTypes.at(polyIndex) == ip)
				{
					long currentPolymerOuterTotal = static_cast<long>(m_OuterFractions.at(polyIndex)*static_cast<double>(polyNo));
					m_PolymerOuterTotal.push_back(currentPolymerOuterTotal);

					m_MonolayerTotal[0] += currentPolymerOuterTotal;
					m_MonolayerTotal[1] += (polyNo - currentPolymerOuterTotal);
				}
			}

			// For linear and branched polymers we count the number of beads between 
			// the head and tail (inclusive) and use this to define the spacing in 
			// the C direction. The spacing is defined as the monolayer thickness
			// divided by m_PolymerSize-1. 
			// We don't know whether the head or tail will be encountered first in the 
			// polymer string so we set a flag, bCount, to indicate when we should count
			// beads. It is reset to null when both head and tail have been counted.
			// The other beads in the polymer are arranged randomly around the head-tail
			// axis.
			//
			// NOTE. This algorithm won't work if the tail bead is found first in the
			// polymer shape string. It will arrange the beads as if the head were first,
			// resulting in the beads between the head and tail having their order
			// reversed.

			const BeadVector&  vBeads = m_vVesicle.back()->GetBeads();
			CBead*		pHead = dynamic_cast<CBead*>(m_vVesicle.back()->GetHead());
			CBead*		pTail = dynamic_cast<CBead*>(m_vVesicle.back()->GetTail());

			CBead* pEnd		= NULL;
			long counter	= 0;
			bool bCount		= false;

			long dummyCount = 0;

			for(cBeadVectorIterator pBead=vBeads.begin();pBead!=vBeads.end(); pBead++)
			{
				dummyCount++;

				if(*pBead == pHead && !bCount)
				{
					bCount = true;
					pEnd   = pTail;
					counter++;
				}
				else if( *pBead == pTail && !bCount)
				{
					bCount = true;
					pEnd   = pHead;
					counter++;
				}
				else if(*pBead == pEnd)
				{
					bCount = false;
					counter++;
				}
				else if(bCount)
				{
					counter++;
				}
			}

			m_PolymerSize.push_back(counter);
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	m_AllPolymerTotal = m_vVesicle.size();	// Total of polymers of all types in vesicle

	// Store the length of the longest polymer in the vesicle so that we can
	// fix the distance between beads.

	m_PolymerMaxSize = *max_element(m_PolymerSize.begin(), m_PolymerSize.end());

	// Set the C axis spacing for the beads in the polymers. We use
	// the size of the largest polymer (between its head and tail) to set the
	// spacing for all polymers. This allows shorter ones to be easily 
	// arranged between longer ones without being stretched too far.

	if(m_PolymerMaxSize > 1)
	{
		m_HexCAxisSpace[0] = 0.5*m_Thickness/static_cast<double>(m_PolymerMaxSize-1);
		m_HexCAxisSpace[1] = m_HexCAxisSpace[0];
	}
	else
	{
		m_HexCAxisSpace[0] = 0.5*m_Thickness;
		m_HexCAxisSpace[1] = m_HexCAxisSpace[0];
	}


	// ****************************************
	// Arrange the polymers with their heads on the surface of two concentric
	// spheres (the inner and outer vesicle radii) and their tails on the 
	// surface of a central concentric sphere. 
	// Because the spheres are concentric we only have to calculate the
	// angular coordinates of the lipids once and then adjust the radius
	// for the inner and outer spheres. We use spherical polar coordinates 
	// to organise the lipids in a roughly hexagonal arrangement on the 
	// spherical surfaces. 
	// The theta coordinate determines how many polymer heads are positioned
	// in the circle of latitude specified by it, and the phi coordinate
	// varies with theta as 1/sin(theta).
	// The vesicle's centre is located at a position set by the values 
	// m_XC, m_YC, m_ZC as fractions of the SimBox side lengths.

	m_XCentre		= m_XC*riState.GetSimBoxBulkXLength();
	m_YCentre		= m_YC*riState.GetSimBoxBulkYLength();
	m_ZCentre		= m_ZC*riState.GetSimBoxBulkZLength();

	// Calculate the number of polymers in the outer and inner monolayers.
	// They should have equal values for their area-per-polymer, so that the 
	// difference in number is determined by the difference in the monolayer 
	// surface areas. But if the user has specified polymer number fractions and
	// inner/outer monolayer fractions that lead to an uneven distribution of
	// polymers the vesicle will be distorted away from this state. The number of
	// polymers of each type is used together with the inner/outer fractions to
	// calculate the number in each monolayer. These values are used to determine
	// the area element for each polymer.

	const double radiusRatio = m_MonolayerRadius[0]/m_MonolayerRadius[1];
	const double saRatio	 = radiusRatio*radiusRatio;

	m_MeanMonolayerTotal[0] = static_cast<long>(static_cast<double>(m_AllPolymerTotal)/(1.0 + 1.0/saRatio));
	m_MeanMonolayerTotal[1] = m_AllPolymerTotal - m_MeanMonolayerTotal[0];

	m_DTheta[0]		= sqrt(4.0*xxBase::m_globalPI/static_cast<double>(m_MonolayerTotal[0]));
	m_DTheta[1]		= sqrt(4.0*xxBase::m_globalPI/static_cast<double>(m_MonolayerTotal[1]));

	m_DPhi[0]		= m_DTheta[0]; // not including the sin(theta) term which occurs below
	m_DPhi[1]		= m_DTheta[1];

	// First store the coordinates of possible polymer head positions for
	// each monolayer, and a vector of sequential indices to access them. 
	// If the initial state specifies an homogeneoux mix of all types of
	// polymer the indices are shuffled, but if patches are desired, the 
	// index shuffling is omitted. Then we loop over all polymers finding
	// a site for each one according to the user-defined fraction of each type
	// that should occur in the inner and outer monolayers.

	short int monolayerIndex;		// Counter used in several loops below

	for(monolayerIndex=0; monolayerIndex<2; monolayerIndex++)
	{
		double theta  = 0.5*m_DTheta[monolayerIndex];
		double phi    = m_DPhi[monolayerIndex];

		for(long icu=0; icu<m_MonolayerTotal[monolayerIndex]; icu++)
		{
			if(monolayerIndex == 0)
			{
				m_RandomOuterIndex.push_back(icu);
				m_OuterTheCoord.push_back(theta);
				m_OuterPhiCoord.push_back(phi);
			}
			else
			{
				m_RandomInnerIndex.push_back(icu);
				m_InnerTheCoord.push_back(theta);
				m_InnerPhiCoord.push_back(phi);
			}

            // Changelist 1551
            // Test solution to the tense vesicle initial bursting problem:
            // we add a small random value to the phi angle with a maximum
            // equal to the phi increment.

 //           phi += 2.0*(1.0 - CCNTCell::GetRandomNo())*(m_DPhi[monolayerIndex]/sin(theta));

  			phi += (m_DPhi[monolayerIndex]/sin(theta));

			if(phi > xxBase::m_globalTwoPI - 0.5*m_DPhi[monolayerIndex])
			{
				theta += m_DTheta[monolayerIndex];
				phi = 0.0;
			}
		}
	}

	// Patches of polymers are specified for each monolayer independently

	if(!m_bPatches[0])
		random_shuffle_wrapper(m_RandomOuterIndex.begin(), m_RandomOuterIndex.end());

	if(!m_bPatches[1])
		random_shuffle_wrapper(m_RandomInnerIndex.begin(), m_RandomInnerIndex.end());

	// ****************************************
	// Loop over each type of polymer in the monolayers and assign coordinates to 
	// them with their tails positioned at the centre of the bilayer and their
	// heads positioned directly above/below the tails using the 
	// m_OuterRadius/m_InnerRadius parameters.
	//
	// Also, set the copies of the bead coordinates using SetunPBCXPos(), 
	// SetInitialXPos().

	// Divide up the available radial distance into segments whose length is
	// set by the number of beads in the polymer. We reduce it by one to
	// allow the head and tail to be positioned exactly on the inner and outer
	// radii, and the others in between. The inner monolayer extends from
	// m_InnerRadius to m_BilayerRadius, and the outer monolayer from
	// m_BilayerRadius to m_OuterRadius, but they both have the same width.
	// However, we reduce the radial distance slightly to avoid tail beads
	// overlapping at the bilayer midplane.
	//
	// Note that we have to reset the theta, phi variables when we start
	// assigning coordinates to polymers in the inner monolayer.

	long species			= 0;
	long polyNo				= 0;
	long polyOuterCounter	= 0;
	long polyInnerCounter	= 0;
	long site				= 0;

	double xt[3];
	double xp1[3];

	double theta	= 0.0;
	double phi		= 0.0;
	double deltaR	= 0.0;

	for(unsigned long iv=0; iv<m_vVesicle.size(); iv++)
	{
		if(polyNo == m_PolymerTotal.at(species))
		{
			species++;			// Move to next polymer type
			polyNo = 0;			// Reset the polymer counter for next type
		}

		if(polyNo < m_PolymerOuterTotal.at(species))
		{	
			monolayerIndex = 0;

			// Pick a site in the outer monolayer for the polymer using the
			// number of polymers in the monolayer as an index into the
			// (possibly) shuffled vector of site indices

			site	= m_RandomOuterIndex.at(polyOuterCounter++);
			theta	= m_OuterTheCoord.at(site);
			phi		= m_OuterPhiCoord.at(site);
		}
		else
		{
			monolayerIndex = 1;

			site	= m_RandomInnerIndex.at(polyInnerCounter++);
			theta	= m_InnerTheCoord.at(site);
			phi		= m_InnerPhiCoord.at(site);
		}

		xt[0] = sin(theta)*cos(phi);
		xt[1] = sin(theta)*sin(phi);
		xt[2] = cos(theta);

		// We still use the variable upperPoly to avoid having to change its 
		// name everywhere

		CPolymer* upperPoly = m_vVesicle.at(iv);

		// Position the head and tail beads first in the outer and inner spheres.
		// We move the tail beads slightly off the exact centre of the bilayer
		// to avoid overlaps.

		upperPoly->GetHead()->SetXPos(m_XCentre + m_MonolayerRadius[monolayerIndex]*xt[0]);
		upperPoly->GetHead()->SetYPos(m_YCentre + m_MonolayerRadius[monolayerIndex]*xt[1]);
		upperPoly->GetHead()->SetZPos(m_ZCentre + m_MonolayerRadius[monolayerIndex]*xt[2]);

		upperPoly->GetTail()->SetXPos(m_XCentre + m_BilayerRadius[monolayerIndex]*xt[0]);
		upperPoly->GetTail()->SetYPos(m_YCentre + m_BilayerRadius[monolayerIndex]*xt[1]);
		upperPoly->GetTail()->SetZPos(m_ZCentre + m_BilayerRadius[monolayerIndex]*xt[2]);

		// Assign coordinates to other beads in the polymers wrapping them around
		// the head-tail axis somewhat randomly. We check that these random
		// coordinates do not lie outside the SimBox boundaries in the X,Y directions.
		// We don't have to check for their being less than 0 because we only
		// add a random component, and don't subtract it.
		//
		// This assumes that the beads are counted starting at the polymer head.

        // Changelist 1551        
        // We add the condition that all beads of the same type as the polymer's 
        // Head bead are placed at the same radial distance from the centre,
        // and all remaining beads in the polymer are displaced by one half
        // the head bead's radius into the bilayer.
        // This is intended to reduce the distance that such beads must move to
        // find their equilibrium position on the outer or inner surfaces of the
        // vesicle.


		deltaR = m_MonolayerSign[monolayerIndex]*m_HexCAxisSpace[monolayerIndex];

		double counter = 0.0;
		double dist = 0.0;

		for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
		{
//            const long polyHeadType = upperPoly->GetHead()->GetType();
//            const double polyHeadRadius = upperPoly->GetHead()->GetRadius();

			if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
			{
  				counter += 1.0;
  				dist = m_MonolayerRadius[monolayerIndex] + counter*deltaR*(1.0 - CCNTCell::GetRandomNo());
 
//                if((*iterBead2)->GetType() == polyHeadType)
//                {
//				    dist = m_MonolayerRadius[monolayerIndex] + deltaR*(1.0 - CCNTCell::GetRandomNo());
//                }
//                else
//                {
//				    counter += 1.0;
//				    dist = m_MonolayerRadius[monolayerIndex] + m_MonolayerSign[monolayerIndex]*polyHeadRadius + counter*deltaR*(1.0 - CCNTCell::GetRandomNo());
//                }

				xp1[0] = dist*xt[0];
				xp1[1] = dist*xt[1];
				xp1[2] = dist*xt[2];

				(*iterBead2)->SetXPos(m_XCentre + xp1[0]);
				(*iterBead2)->SetYPos(m_YCentre + xp1[1]);
				(*iterBead2)->SetZPos(m_ZCentre + xp1[2]);
			}
		}

		polyNo++;
	}	// End of loop over all polymers in vesicle

	return true;
}

// **********************************************************************
// Member functions for enclosed class: isMultiVesicle.

// This object assembles the selected polymers into a set of identical
// bilayer vesicles. The vesicles' outer radius, membrane thickness and the
// types and number of polymers are all equal, and the centres of the vesicles
// are supplied in a vector.
// If multiple polymer types are being used they are assigned coordinates 
// sequentially. This results in different parts of the vesicle being occupied 
// by each type of polymer. If a randomised distribution is required we can 
// change the routine later.
//
// This routine does not check that the vesicles do not overlap any other
// structure including the walls. It assumes that this has been done by the 
// calling routine.

CBuilder::isMultiVesicle::isMultiVesicle(const zLongVector polymerTypes, const zDoubleVector outerFractions,
									   zDoubleVector xc, zDoubleVector yc, zDoubleVector zc, double outerRadius, 
									   double innerRadius, double thickness, 
									   bool bPatches[2]) : m_VesicleTotal(xc.size()),
										m_PolymerTypes(polymerTypes),
										m_OuterFractions(outerFractions),
										m_XC(xc), m_YC(yc), m_ZC(zc),
										m_OuterRadius(outerRadius),
										m_InnerRadius(innerRadius),
										m_Thickness(thickness),
										m_AllPolymerTotal(0),
										m_PolymersPerVesicle(0),
										m_PolymersInLastVesicle(0),
										m_PolymerMaxSize(0),
										m_XCentre(0.0),
										m_YCentre(0.0),
										m_ZCentre(0.0)
{
	m_bPatches[0] = bPatches[0];
	m_bPatches[1] = bPatches[1];

	// Store data used in loops over the two monolayers. Notice that we
	// increase the outer radius and decrease the inner radius slightly
	// to avoid polymer tail beads overlapping at the bilayer midplane.
	// The m_MonolayerSign[] array is used to position beads between the
	// polymer head and tail in the appropriate direction for each monolayer.
	// The mean number of polymers in each monolayer is calculated from the
	// vesicle radius and bilayer thickness assuming equal area-per-polymer
	// for both. The actual number of polymers is determined from the user-defined
	// number fractions and inner/outer fractions, and then used to calculate 
	// the actual area-per-polymers. If these do not agree with the relaxed values,
	// the vesicle will be under strain.

	m_vVesicle.clear();
	m_PolymerTotal.clear();
	m_PolymerOuterTotal.clear();
	m_PolymerSize.clear();
	m_RandomOuterIndex.clear();
	m_RandomInnerIndex.clear();
	m_OuterTheCoord.clear();
	m_OuterPhiCoord.clear();
	m_InnerTheCoord.clear();
	m_InnerPhiCoord.clear();

	m_MeanMonolayerTotal[0]	= 0;	// Relaxed number of polymers in outer monolayer
	m_MeanMonolayerTotal[1]	= 0;
	m_MonolayerTotal[0]		= 0;	// Actual polymers in outer monolayer
	m_MonolayerTotal[1]		= 0;
	m_MonolayerSign[0]		= -1.0;
	m_MonolayerSign[1]		= 1.0;
	m_MonolayerRadius[0]	= m_OuterRadius;
	m_MonolayerRadius[1]	= m_InnerRadius;
	m_BilayerRadius[0]		= 1.01*(m_OuterRadius - 0.5*m_Thickness);
	m_BilayerRadius[1]		= 0.99*(m_InnerRadius + 0.5*m_Thickness);
	m_HexCAxisSpace[0]		= 0.0;
	m_HexCAxisSpace[1]		= 0.0;
    m_DTheta[0]				= 0.0;
    m_DTheta[1]				= 0.0;
    m_DPhi[0]				= 0.0;
    m_DPhi[1]				= 0.0;

}

CBuilder::isMultiVesicle::~isMultiVesicle()
{
}

bool CBuilder::isMultiVesicle::Assemble(CInitialState& riState)
{
	// Copy the polymers in the CInitialState into a local vector for ease of access.
	// We loop over all types of polymer adding them to a local vector if
	// their type occurs in the data for a composite vesicle. Then the number of
	// polymers of each type that are in the outer monolayer, as determined by the
	// user-defined m_OuterFractions vector, are calculated. The number of beads
	// in each type of polymer is also stored.

	PolymerVector vesiclePolymers = riState.GetPolymers();

	long firstPolymer = 0;
	long lastPolymer  = 0;

	for(long ip=0; ip<riState.GetPolymerTypeTotal(); ip++)
	{
		long polyNo = riState.GetPolymerTotalForType(ip);

		lastPolymer += polyNo;

		if(find(m_PolymerTypes.begin(), m_PolymerTypes.end(), ip) != m_PolymerTypes.end())
		{
			// Copy the polymer pointers for the required polymers into local storage.
			// We use a back_inserter to avoid having to manually resize the container.

			copy(vesiclePolymers.begin()+firstPolymer, vesiclePolymers.begin()+lastPolymer, back_inserter(m_vVesicle));

			// Store the number of this type of polymer and the number in the outer 
			// monolayer. We subtract the upper number from the total number to
			// ensure that no polymers are missed due to rounding of the upper 
			// fraction.
			
			m_PolymerTotal.push_back(polyNo);
		
			for(unsigned long polyIndex=0; polyIndex<m_PolymerTypes.size(); polyIndex++)
			{
				if(m_PolymerTypes.at(polyIndex) == ip)
				{
					long currentPolymerOuterTotal = static_cast<long>(m_OuterFractions.at(polyIndex)*static_cast<double>(polyNo));
					m_PolymerOuterTotal.push_back(currentPolymerOuterTotal);

					m_MonolayerTotal[0] += currentPolymerOuterTotal;
					m_MonolayerTotal[1] += (polyNo - currentPolymerOuterTotal);
				}
			}

			// For linear and branched polymers we count the number of beads between 
			// the head and tail (inclusive) and use this to define the spacing in 
			// the C direction. The spacing is defined as the monolayer thickness
			// divided by m_PolymerSize-1. 
			// We don't know whether the head or tail will be encountered first in the 
			// polymer string so we set a flag, bCount, to indicate when we should count
			// beads. It is reset to null when both head and tail have been counted.
			// The other beads in the polymer are arranged randomly around the head-tail
			// axis.
			//
			// NOTE. This algorithm won't work if the tail bead is found first in the
			// polymer shape string. It will arrange the beads as if the head were first,
			// resulting in the beads between the head and tail having their order
			// reversed.

			const BeadVector&  vBeads = m_vVesicle.back()->GetBeads();
			CBead*		pHead = dynamic_cast<CBead*>(m_vVesicle.back()->GetHead());
			CBead*		pTail = dynamic_cast<CBead*>(m_vVesicle.back()->GetTail());

			CBead* pEnd		= NULL;
			long counter	= 0;
			bool bCount		= false;

			long dummyCount = 0;

			for(cBeadVectorIterator pBead=vBeads.begin();pBead!=vBeads.end(); pBead++)
			{
				dummyCount++;

				if(*pBead == pHead && !bCount)
				{
					bCount = true;
					pEnd   = pTail;
					counter++;
				}
				else if( *pBead == pTail && !bCount)
				{
					bCount = true;
					pEnd   = pHead;
					counter++;
				}
				else if(*pBead == pEnd)
				{
					bCount = false;
					counter++;
				}
				else if(bCount)
				{
					counter++;
				}
			}

			m_PolymerSize.push_back(counter);
		}

		firstPolymer += riState.GetPolymerTotalForType(ip);
	}

	m_AllPolymerTotal = m_vVesicle.size();	// Total of polymers of all types in vesicle

	// Calculate how many polymers of all types are in each vesicle. Because the
	// fractions may not result in exactly an integer number of polymers of each
	// type we have to leave the last vesicle with a variable number.

	m_PolymersPerVesicle = m_AllPolymerTotal/m_VesicleTotal;
	m_PolymersInLastVesicle = m_AllPolymerTotal - m_VesicleTotal*m_PolymersPerVesicle;

	// Store the length of the longest polymer in the vesicle so that we can
	// fix the distance between beads.

	m_PolymerMaxSize = *max_element(m_PolymerSize.begin(), m_PolymerSize.end());

	// Set the C axis spacing for the beads in the polymers. We use
	// the size of the largest polymer (between its head and tail) to set the
	// spacing for all polymers. This allows shorter ones to be easily 
	// arranged between longer ones without being stretched too far.

	if(m_PolymerMaxSize > 1)
	{
		m_HexCAxisSpace[0] = 0.5*m_Thickness/static_cast<double>(m_PolymerMaxSize-1);
		m_HexCAxisSpace[1] = m_HexCAxisSpace[0];
	}
	else
	{
		m_HexCAxisSpace[0] = 0.5*m_Thickness;
		m_HexCAxisSpace[1] = m_HexCAxisSpace[0];
	}


	// ****************************************
	// Arrange the polymers with their heads on the surface of two concentric
	// spheres (the inner and outer vesicle radii) and their tails on the 
	// surface of a central concentric sphere. 
	// Because the spheres are concentric we only have to calculate the
	// angular coordinates of the lipids once and then adjust the radius
	// for the inner and outer spheres. We use spherical polar coordinates 
	// to organise the lipids in a roughly hexagonal arrangement on the 
	// spherical surfaces. 
	// The theta coordinate determines how many polymer heads are positioned
	// in the circle of latitude specified by it, and the phi coordinate
	// varies with theta as 1/sin(theta).

	// Calculate the number of polymers in the outer and inner monolayers.
	// They should have equal values for their area-per-polymer, so that the 
	// difference in number is determined by the difference in the monolayer 
	// surface areas. But if the user has specified polymer number fractions and
	// inner/outer monolayer fractions that lead to an uneven distribution of
	// polymers the vesicle will be distorted away from this state. The number of
	// polymers of each type is used together with the inner/outer fractions to
	// calculate the number in each monolayer. These values are used to determine
	// the area element for each polymer.

	const double radiusRatio = m_MonolayerRadius[0]/m_MonolayerRadius[1];
	const double saRatio	 = radiusRatio*radiusRatio;

	m_MeanMonolayerTotal[0] = static_cast<long>(static_cast<double>(m_PolymersPerVesicle)/(1.0 + 1.0/saRatio));
	m_MeanMonolayerTotal[1] = m_PolymersPerVesicle - m_MeanMonolayerTotal[0];

	m_DTheta[0]		= sqrt(4.0*xxBase::m_globalPI/static_cast<double>(m_MonolayerTotal[0]));
	m_DTheta[1]		= sqrt(4.0*xxBase::m_globalPI/static_cast<double>(m_MonolayerTotal[1]));

	m_DPhi[0]		= m_DTheta[0]; // not including the sin(theta) term which occurs below
	m_DPhi[1]		= m_DTheta[1];

	// First store the coordinates of possible polymer head positions for
	// each monolayer, and a vector of sequential indices to access them. 
	// If the initial state specifies an homogeneoux mix of all types of
	// polymer the indices are shuffled, but if patches are desired, the 
	// index shuffling is omitted. Then we loop over all polymers finding
	// a site for each one according to the user-defined fraction of each type
	// that should occur in the inner and outer monolayers.

	short int monolayerIndex;		// Counter used in several loops below

	for(monolayerIndex=0; monolayerIndex<2; monolayerIndex++)
	{
		double theta  = m_DTheta[monolayerIndex];
		double phi    = 0.0;

		for(long icu=0; icu<m_MonolayerTotal[monolayerIndex]; icu++)
		{
			if(monolayerIndex == 0)
			{
				m_RandomOuterIndex.push_back(icu);
				m_OuterTheCoord.push_back(theta);
				m_OuterPhiCoord.push_back(phi);
			}
			else
			{
				m_RandomInnerIndex.push_back(icu);
				m_InnerTheCoord.push_back(theta);
				m_InnerPhiCoord.push_back(phi);
			}

			phi += (m_DPhi[monolayerIndex]/sin(theta));

			if(phi > xxBase::m_globalTwoPI - m_DPhi[monolayerIndex])
			{
				theta += m_DTheta[monolayerIndex];
				phi = 0.0;
			}
		}
	}

	// Patches of polymers are specified for each monolayer independently

	if(!m_bPatches[0])
		random_shuffle_wrapper(m_RandomOuterIndex.begin(), m_RandomOuterIndex.end());

	if(!m_bPatches[1])
		random_shuffle_wrapper(m_RandomInnerIndex.begin(), m_RandomInnerIndex.end());

	// ****************************************
	// Loop over each type of polymer in the monolayers and assign coordinates to 
	// them with their tails positioned at the centre of the bilayer and their
	// heads positioned directly above/below the tails using the 
	// m_OuterRadius/m_InnerRadius parameters.
	//
	// Also, set the copies of the bead coordinates using SetunPBCXPos(), 
	// SetInitialXPos().

	// Divide up the available radial distance into segments whose length is
	// set by the number of beads in the polymer. We reduce it by one to
	// allow the head and tail to be positioned exactly on the inner and outer
	// radii, and the others in between. The inner monolayer extends from
	// m_InnerRadius to m_BilayerRadius, and the outer monolayer from
	// m_BilayerRadius to m_OuterRadius, but they both have the same width.
	// However, we reduce the radial distance slightly to avoid tail beads
	// overlapping at the bilayer midplane.
	//
	// Note that we have to reset the theta, phi variables when we start
	// assigning coordinates to polymers in the inner monolayer.

	long species			= 0;
	long polyNo				= 0;
	long polyOuterCounter	= 0;
	long polyInnerCounter	= 0;
	long site				= 0;

	double xt[3];
	double xp1[3];

	double theta	= 0.0;
	double phi		= 0.0;
	double deltaR	= 0.0;

	// Use a counter to select each vesicle in turn to assign polymers to

	short int vesicleCounter = 0;

	for(unsigned long iv=0; iv<m_vVesicle.size(); iv++)
	{
		if(polyNo == m_PolymerTotal.at(species))
		{
			species++;			// Move to next polymer type
			polyNo = 0;			// Reset the polymer counter for next type
		}

		if(polyNo < m_PolymerOuterTotal.at(species))
		{	
			monolayerIndex = 0;

			// Pick a site in the outer monolayer for the polymer using the
			// number of polymers in the monolayer as an index into the
			// (possibly) shuffled vector of site indices

			site	= m_RandomOuterIndex.at(polyOuterCounter++);
			theta	= m_OuterTheCoord.at(site);
			phi		= m_OuterPhiCoord.at(site);
		}
		else
		{
			monolayerIndex = 1;

			site	= m_RandomInnerIndex.at(polyInnerCounter++);
			theta	= m_InnerTheCoord.at(site);
			phi		= m_InnerPhiCoord.at(site);
		}

		xt[0] = sin(theta)*cos(phi);
		xt[1] = sin(theta)*sin(phi);
		xt[2] = cos(theta);

		// The vesicle's centre is located at a position set by the values 
		// m_XC, m_YC, m_ZC as fractions of the SimBox side lengths.

		m_XCentre		= m_XC.at(vesicleCounter)*riState.GetSimBoxBulkXLength();
		m_YCentre		= m_YC.at(vesicleCounter)*riState.GetSimBoxBulkYLength();
		m_ZCentre		= m_ZC.at(vesicleCounter)*riState.GetSimBoxBulkZLength();

		// We still use the variable upperPoly to avoid having to change its 
		// name everywhere

		CPolymer* upperPoly = m_vVesicle.at(iv);

		// Position the head and tail beads first in the outer and inner spheres.
		// We move the tail beads slightly off the exact centre of the bilayer
		// to avoid overlaps.

		upperPoly->GetHead()->SetXPos(m_XCentre + m_MonolayerRadius[monolayerIndex]*xt[0]);
		upperPoly->GetHead()->SetYPos(m_YCentre + m_MonolayerRadius[monolayerIndex]*xt[1]);
		upperPoly->GetHead()->SetZPos(m_ZCentre + m_MonolayerRadius[monolayerIndex]*xt[2]);

		upperPoly->GetTail()->SetXPos(m_XCentre + m_BilayerRadius[monolayerIndex]*xt[0]);
		upperPoly->GetTail()->SetYPos(m_YCentre + m_BilayerRadius[monolayerIndex]*xt[1]);
		upperPoly->GetTail()->SetZPos(m_ZCentre + m_BilayerRadius[monolayerIndex]*xt[2]);

		// Assign coordinates to other beads in the polymers wrapping them around
		// the head-tail axis somewhat randomly. We check that these random
		// coordinates do not lie outside the SimBox boundaries in the X,Y directions.
		// We don't have to check for their being less than 0 because we only
		// add a random component, and don't subtract it.
		//
		// This assumes that the beads are counted starting at the polymer head.

		// If we have more than one bead in the polymer distribute them in a line,
		// otherwise just leave the single bead in place.

		deltaR = m_MonolayerSign[monolayerIndex]*m_HexCAxisSpace[monolayerIndex];

		double counter = 0.0;
		double dist = 0.0;

		for(cBeadVectorIterator iterBead2=upperPoly->GetBeads().begin(); iterBead2!= upperPoly->GetBeads().end(); iterBead2++)
		{
			if((*iterBead2)->GetId() != upperPoly->GetTail()->GetId() && (*iterBead2)->GetId() != upperPoly->GetHead()->GetId())
			{
				counter += 1.0;
				dist = m_MonolayerRadius[monolayerIndex] + counter*deltaR*(1.0 - CCNTCell::GetRandomNo());

				xp1[0] = dist*xt[0];
				xp1[1] = dist*xt[1];
				xp1[2] = dist*xt[2];

				(*iterBead2)->SetXPos(m_XCentre + xp1[0]);
				(*iterBead2)->SetYPos(m_YCentre + xp1[1]);
				(*iterBead2)->SetZPos(m_ZCentre + xp1[2]);
			}
		}

		vesicleCounter++;	// select next vesicle
		if(vesicleCounter == m_VesicleTotal)
			vesicleCounter = 0;

		polyNo++;
	}	// End of loop over all polymers in vesicle

	return true;
}


// Function to assign velocities to the set of beads in the initial state. 
// The values are taken from a Maxwell distribution with a mean of unity and 
// then scaled by the temperature defined in the control data file. 
// The bead velocities are selected randomly from a much larger vector of 
// velocities drawn from a Maxwell distribution with unit mean and variance. 
// If the number of values needed exceeds m_MaxwellPointNo then the actual 
// number required is used.
//
// This routine works for both 2 and 3 dimensions. For 2 dimensions, the excluded 
// component is indicated by the normal vector (Xn,Yn,Zn), if all components are 
// zero, a 3 distribution is generated. The 2d case is useful, for example, for
// an initial state that contains a lamella in which the velocity normal to the
// lamella plane is zeroed to avoid beads having initial velocities that would 
// push them into the bilayer.

void CBuilder::AssignMaxwellVelocities(CInitialState &riState, long Xn, long Yn, long Zn)
{
	long i;	// Counter used several times below

	const double kT			= riState.GetkT();
	const long   lBeadTotal = riState.GetBeadTotal();
	const double dBeadTotal = static_cast<double>(lBeadTotal);

	const zDoubleVector rvelDist = MaxwellDist(lBeadTotal);

	zDoubleVector VXDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector VYDist(riState.GetBeadTotal(), 0.0);
	zDoubleVector VZDist(riState.GetBeadTotal(), 0.0);

	long index;
	double vmag, vtheta, vphi;
	double vp[3], vcm[3], vmean[3], v2mean[3], var[3];

	for(i=0; i<3; i++)
	{
		vp[i]		= 0.0;
		vcm[i]		= 0.0;
		vmean[i]	= 0.0;
		v2mean[i]	= 0.0;
		var[i]		= 0.0;
	}

	// Assign velocities from a Maxwell distribution and calculate the 
	// CM velocity.
	//
	// In 2d and 3d we choose the azimuthal angle, phi, randomly from
	// the range [0,2*pi], but in 3d we also choose the polar angle, theta,
	// in the range [0,pi]. We do this by picking a value for cos(theta)
	// randomly from the uniform distribution [-1,1] and taking the acos of it.
	// Because the random numbers are uniformly distributed on the interval
	// [0,1], the argument of the acos function lies between 0.0 and pi.

	if(Xn ==0 && Yn == 0 && Zn == 0)		// 3d distribution
	{
		for(i=0; i<lBeadTotal; i++)
		{
			index	= static_cast<long>(rvelDist.size()*CCNTCell::GetRandomNo());
			vmag	= rvelDist.at(index);
			vphi	= xxBase::m_globalTwoPI*CCNTCell::GetRandomNo();
			vtheta	= acos(1.0-2.0*CCNTCell::GetRandomNo());

			vp[0]   = vmag*sin(vtheta)*cos(vphi);
			vp[1]   = vmag*sin(vtheta)*sin(vphi);
			vp[2]   = vmag*cos(vtheta);

			vcm[0] += vp[0];
			vcm[1] += vp[1];
			vcm[2] += vp[2];

			VXDist.at(i) = vp[0];
			VYDist.at(i) = vp[1];
			VZDist.at(i) = vp[2];
		}
	}
	else		// 2d distribution: we ignore the Z component
	{
		for(i=0; i<lBeadTotal; i++)
		{
			index	= static_cast<long>(rvelDist.size()*CCNTCell::GetRandomNo());
			vmag	= rvelDist.at(index);
			vphi	= xxBase::m_globalTwoPI*CCNTCell::GetRandomNo();

			vp[0]   = vmag*cos(vphi);
			vp[1]   = vmag*sin(vphi);

			vcm[0] += vp[0];
			vcm[1] += vp[1];

			VXDist.at(i) = vp[0];
			VYDist.at(i) = vp[1];
		}
	}

	// Now subtract off the CM motion and scale the velocities according to 
	// the specified temperature

	vcm[0] /= dBeadTotal;
	vcm[1] /= dBeadTotal;
	vcm[2] /= dBeadTotal;

	double vtotal = 0.0;

	for(i=0; i<lBeadTotal; i++)
	{
		VXDist.at(i)  -= vcm[0];
		VYDist.at(i)  -= vcm[1];
		VZDist.at(i)  -= vcm[2];

		vtotal = vtotal + VXDist.at(i)*VXDist.at(i) +
						  VYDist.at(i)*VYDist.at(i) +
						  VZDist.at(i)*VZDist.at(i);
	}

	// Normalize the velocities according to the dimension of the distribution
	// and the number of beads. 

	if(Xn ==0 && Yn == 0 && Zn == 0)
	{
		vtotal = sqrt(vtotal/(3.0*dBeadTotal*kT));
	}
	else
	{
		vtotal = sqrt(vtotal/(2.0*dBeadTotal*kT));
	}

	// Normalize the velocities and copy them into the initial state's beads. 
	// If a 2d distribution is required we check which component should be zero.
	// Note that we always use the VXDist and VYDist vectors for 2d distributions.

	for(i=0; i<lBeadTotal; i++)
	{
		VXDist.at(i) = VXDist.at(i)/vtotal;
		VYDist.at(i) = VYDist.at(i)/vtotal;
		VZDist.at(i) = VZDist.at(i)/vtotal;

		if(Xn == 1)
		{
			(riState.GetBeads()).at(i)->SetXMom(0.0);
			(riState.GetBeads()).at(i)->SetYMom(VYDist.at(i));
			(riState.GetBeads()).at(i)->SetZMom(VXDist.at(i));
		}
		else if(Yn == 1)
		{
			(riState.GetBeads()).at(i)->SetXMom(VXDist.at(i));
			(riState.GetBeads()).at(i)->SetYMom(0.0);
			(riState.GetBeads()).at(i)->SetZMom(VYDist.at(i));
		}
		else if(Zn == 1)
		{
			(riState.GetBeads()).at(i)->SetXMom(VXDist.at(i));
			(riState.GetBeads()).at(i)->SetYMom(VYDist.at(i));
			(riState.GetBeads()).at(i)->SetZMom(0.0);
		}
		else
		{
			(riState.GetBeads()).at(i)->SetXMom(VXDist.at(i));
			(riState.GetBeads()).at(i)->SetYMom(VYDist.at(i));
			(riState.GetBeads()).at(i)->SetZMom(VZDist.at(i));
		}

		vmean[0] += VXDist.at(i);
		vmean[1] += VYDist.at(i);
		vmean[2] += VZDist.at(i);

		v2mean[0] += VXDist.at(i)*VXDist.at(i);
		v2mean[1] += VYDist.at(i)*VYDist.at(i);
		v2mean[2] += VZDist.at(i)*VZDist.at(i);
	}

	for(i=0; i<3; i++)
	{
		vmean[i]  =  vmean[i]/dBeadTotal;
		v2mean[i] = v2mean[i]/dBeadTotal;
		var[i]    = v2mean[i] - vmean[i]*vmean[i];
	}

	// Write the initial velocity distribution to the CLogState object using
	// a CLogVelDistMessage object. And only write it to the screen in debug.

	new CLogVelDistMessage(0, kT, vmean, v2mean, var);


#ifdef TraceOn
	Trace("Initial vel distn");
	TraceDouble("Temperature   = ", kT);
	TraceVector("Mean Vel      = ", vmean[0],  vmean[1],  vmean[2]);
	TraceVector("Mean Sq Vel   = ", v2mean[0], v2mean[1], v2mean[2]);
	TraceVector("Var Vel       = ", var[0],    var[1],    var[2]);
	Trace("********************");
#endif

}

// Function to copy the initial bead coordinates of all beads into the arrays
// used to calculate, for example, the diffusion coefficients of beads. This 
// requires coordinates that are not wrapped around by the PBCs. We also store
// the initial coordinates of all beads for time-dependent analysis of bead
// motion.

void CBuilder::AssignUnPBCCoords(CInitialState &riState)
{
	for(long iBead=0; iBead<riState.GetBeadTotal(); iBead++)
	{
		(riState.GetBeads()).at(iBead)->SetunPBCXPos((riState.GetBeads()).at(iBead)->GetXPos());
		(riState.GetBeads()).at(iBead)->SetunPBCYPos((riState.GetBeads()).at(iBead)->GetYPos());
		(riState.GetBeads()).at(iBead)->SetunPBCZPos((riState.GetBeads()).at(iBead)->GetZPos());

		(riState.GetBeads()).at(iBead)->SetInitialXPos((riState.GetBeads()).at(iBead)->GetXPos());
		(riState.GetBeads()).at(iBead)->SetInitialYPos((riState.GetBeads()).at(iBead)->GetYPos());
		(riState.GetBeads()).at(iBead)->SetInitialZPos((riState.GetBeads()).at(iBead)->GetZPos());
	}
}

// Function that checks if a specified point (px,py,pz) lies within a distance (radius)
// of another specified point (cx,cy,cz).

bool CBuilder::IsPointWithinSphere(double cx, double cy, double cz, double radius, 
								   double px, double py, double pz) const
{
	const double dx = (px - cx);
	const double dy = (py - cy);
	const double dz = (pz - cz);

	if( (dx*dx + dy*dy + dz*dz) < radius*radius)
		return true;
	else
		return false;


}

// Do-nothing implementation to allow non-parallelised derived classes to be 
// instantiated.

bool CBuilder::AssembleP(CInitialState& riState)
{
    return false;
}
