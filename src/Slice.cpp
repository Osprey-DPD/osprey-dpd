/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Slice.cpp: implementation of the CSlice class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "Slice.h"
#include "ISimBox.h"
#include "CNTCell.h"
#include "AbstractBead.h"
#include "aaRegionToType.h"	// Needed for aaBeadXPos() etc function objects


// STL using declarations

	using std::remove_if;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor used when the grid slices are smaller or larger than the CNT cells 
// in the normal direction: we pass in the coordinates of the grid slice and the
// indices of the two CNT cell slices that contain, or bound, the grid slice.

CSlice::CSlice(long sliceIndex, long lowerCNTSliceIndex, long upperCNTSliceIndex, 
			   double lowerSliceCoord, double upperSliceCoord, 
			   long xNormal, long yNormal, long zNormal, 
		       const ISimBox* const pISimBox) : CRegion(sliceIndex), m_X(xNormal), m_Y(yNormal), m_Z(zNormal),
												m_CNTRowsInSlice(0)
{
	// The slices store the set of beads that occur in the SimBox whose coordinates fall
	// within their boundaries. We access the bead lists stored in the CNTCells via
	// their CAbstractCell base class. But note that the bead lists are not const so 
	// we have to take care not to change any data in the beads.
	//
	// It is distasteful that we have to repeat the code for calculating the bead lists,
	// but the cell sizes may vary between dimensions so we cannot factor it out.

	long cellIndex, i, j, k;

	if(xNormal == 1)
	{
		m_CNTRowsInSlice = pISimBox->GetCNTZCellNo();

		// Loop over the CNT cells adding their beads to the slice's bead container
		// if their x coordinate lies within the grid slice range specified by
		// (lowerSliceCoord, upperSliceCoord). We copy the whole CNT cell's bead list
		// to a local list and then remove those elements outside the range.
		// Note that the CNTSliceIndex loop only is only executed once or twice
		// depending on whether the grid slice overlaps a CNT slice boundary or not.

		for(long int CNTSliceIndex=lowerCNTSliceIndex; CNTSliceIndex<=upperCNTSliceIndex; CNTSliceIndex++)
		{
			for(j=0; j<pISimBox->GetCNTYCellNo(); j++)
			{
				for(k=0; k<pISimBox->GetCNTZCellNo(); k++)
				{
					cellIndex = pISimBox->GetCNTXCellNo()*(pISimBox->GetCNTYCellNo()*k + j) + CNTSliceIndex;

					BeadList rBeadList = pISimBox->GetCNTCells()[cellIndex]->GetBeads();

					// Remove beads whose x coordinates are outside the grid slice's 
					// boundaries. We use the remove_if() algorithm that requires
					// a predicate to determine if an element should be removed from the
					// container. We define function objects that take a CAbstractBead*
					// argument and compare the bead's X, Y or Z coordinate with a 
					// specified value, returning true if the coordinate is less than
					// (aaBeadZPosLess) or greater than (aaBeadZPosGreater) the value.
					// We check for equality because if the slice width is commensurate 
					// with the box size some beads may have coordinates exactly on the 
					// upper CNT cell boundary and the lower boundary is included in the current cell.
					// Note that remove_if returns an iterator to the last valid
					// element but the container retains its original size. The elements
					// following the returned iterator position are undefined.

					BeadListIterator lastBead1 = remove_if(rBeadList.begin(), rBeadList.end(), aaBeadXPosLess(lowerSliceCoord) );
					BeadListIterator lastBead2 = remove_if(rBeadList.begin(), lastBead1,	   aaBeadXPosGreaterEqual(upperSliceCoord) );

					copy(rBeadList.begin(), lastBead2, back_inserter(m_lBeads));
				}
			}
		}

		m_Width	= pISimBox->GetSimBoxYLength();
		m_Depth	= pISimBox->GetSimBoxZLength();
	}
	else if(yNormal == 1)
	{
		m_CNTRowsInSlice = pISimBox->GetCNTXCellNo();

		// Loop over the CNT cells adding their beads to the slice's bead container
		// if their y coordinate lies within the grid slice range specified by
		// (lowerSliceCoord, upperSliceCoord). We copy the whole CNT cell's bead list
		// to a local list and then remove those elements outside the range.
		// Note that the CNTSliceIndex loop only is only executed once or twice
		// depending on whether the grid slice overlaps a CNT slice boundary or not.

		for(long int CNTSliceIndex=lowerCNTSliceIndex; CNTSliceIndex<=upperCNTSliceIndex; CNTSliceIndex++)
		{
			for(i=0; i<pISimBox->GetCNTXCellNo(); i++)
			{
				for(k=0; k<pISimBox->GetCNTZCellNo(); k++)
				{
					cellIndex = pISimBox->GetCNTXCellNo()*(pISimBox->GetCNTYCellNo()*k + CNTSliceIndex) + i;

					BeadList rBeadList = pISimBox->GetCNTCells()[cellIndex]->GetBeads();

					// Remove beads whose y coordinates are outside the grid slice's 
					// boundaries. We use the remove_if() algorithm that requires
					// a predicate to determine if an element should be removed from the
					// container. We define function objects that take a CAbstractBead*
					// argument and compare the bead's X, Y or Z coordinate with a 
					// specified value, returning true if the coordinate is less than
					// (aaBeadZPosLess) or greater than (aaBeadZPosGreater) the value.
					// We check for equality because if the slice width is commensurate 
					// with the box size some beads may have coordinates exactly on the 
					// upper CNT cell boundary and the lower boundary is included in the current cell.
					// Note that remove_if returns an iterator to the last valid
					// element but the container retains its original size. The elements
					// following the returned iterator position are undefined.

					BeadListIterator lastBead1 = remove_if(rBeadList.begin(), rBeadList.end(), aaBeadYPosLess(lowerSliceCoord) );
					BeadListIterator lastBead2 = remove_if(rBeadList.begin(), lastBead1,	   aaBeadYPosGreaterEqual(upperSliceCoord) );

					copy(rBeadList.begin(), lastBead2, back_inserter(m_lBeads));
				}
			}
		}

		m_Width	= pISimBox->GetSimBoxZLength();
		m_Depth	= pISimBox->GetSimBoxXLength();
	}
	else if(zNormal == 1)
	{
		m_CNTRowsInSlice = pISimBox->GetCNTYCellNo();

		// Loop over the CNT cells adding their beads to the slice's bead container
		// if their z coordinate lies within the grid slice range specified by
		// (lowerSliceCoord, upperSliceCoord). We copy the whole CNT cell's bead list
		// to a local list and then remove those elements outside the range.
		// Note that the CNTSliceIndex loop only is only executed once or twice
		// depending on whether the grid slice overlaps a CNT slice boundary or not.

		for(long int CNTSliceIndex=lowerCNTSliceIndex; CNTSliceIndex<=upperCNTSliceIndex; CNTSliceIndex++)
		{
			for(i=0; i<pISimBox->GetCNTXCellNo(); i++)
			{
				for(j=0; j<pISimBox->GetCNTYCellNo(); j++)
				{
					cellIndex = pISimBox->GetCNTXCellNo()*(pISimBox->GetCNTYCellNo()*CNTSliceIndex + j) + i;

					BeadList rBeadList = pISimBox->GetCNTCells()[cellIndex]->GetBeads();

					// Remove beads whose z coordinates are outside the grid slice's 
					// boundaries. We use the remove_if() algorithm that requires
					// a predicate to determine if an element should be removed from the
					// container. We define function objects that take a CAbstractBead*
					// argument and compare the bead's X, Y or Z coordinate with a 
					// specified value, returning true if the coordinate is less than
					// (aaBeadZPosLess) or greater than (aaBeadZPosGreater) the value.
					// We check for equality because if the slice width is commensurate 
					// with the box size some beads may have coordinates exactly on the 
					// upper CNT cell boundary and the lower boundary is included in the current cell.
					// Note that remove_if returns an iterator to the last valid
					// element but the container retains its original size. The elements
					// following the returned iterator position are undefined.

					BeadListIterator lastBead1 = remove_if(rBeadList.begin(), rBeadList.end(), aaBeadZPosLess(lowerSliceCoord) );
					BeadListIterator lastBead2 = remove_if(rBeadList.begin(), lastBead1,	   aaBeadZPosGreaterEqual(upperSliceCoord) );

					copy(rBeadList.begin(), lastBead2, back_inserter(m_lBeads));
				}
			}
		}

		m_Width	= pISimBox->GetSimBoxXLength();
		m_Depth	= pISimBox->GetSimBoxYLength();
	}

// Output the bead z coordinates to check their distribution. We use the transform
// algorithm and the function object aaBeadZPos() to transform the list of
// CAbstractBeads into a vector of doubles.

//	std::vector<double> zCoordinates;
//	zCoordinates.resize(m_lBeads.size());

//	std::transform(m_lBeads.begin(), m_lBeads.end(), zCoordinates.begin(), aaBeadZPos());

//	for(zDoubleVectorIterator id=zCoordinates.begin(); id!=zCoordinates.end(); id++)
//	{
//		double zvalue = *id;
//	}

	// Set base class members that depend on the above calculations. It would be nice
	// if they could set automatically in the functions that call them, but that would
	// be inefficient when they are called many times.

	m_Height	= upperSliceCoord - lowerSliceCoord;
	m_Volume    = m_Width*m_Depth*m_Height;
}


// Constructor used when the grid slices have exactly the same width as the
// CNT cells in the normal direction. The beads contained in all CNT cells 
// in the specified slice (whose normal is passed in by argument) are just 
// copied to the grid slice's local store.

CSlice::CSlice(long sliceIndex, long xNormal, long yNormal, long zNormal, 
		       const ISimBox* const pISimBox) : CRegion(sliceIndex), m_X(xNormal), m_Y(yNormal), m_Z(zNormal),
												m_CNTRowsInSlice(0)
{
	// The slices store the set of beads that occur in the SimBox whose coordinates fall
	// within their boundaries. We access the bead lists stored in the CNTCells via
	// their CAbstractCell base class. But note that the bead lists are not const so 
	// we have to take care not to change any data in the beads.
	//
	// It is distasteful that we have to repeat the code for calculating the bead lists,
	// but the cell sizes may vary between dimensions so we cannot factor it out.

	long cellIndex, i, j, k;

	if(xNormal == 1)
	{
		m_CNTRowsInSlice = pISimBox->GetCNTZCellNo();

		// Loop over the CNT cells adding their beads to the slice's beads

		for(j=0; j<pISimBox->GetCNTYCellNo(); j++)
		{
			for(k=0; k<pISimBox->GetCNTZCellNo(); k++)
			{
				cellIndex = pISimBox->GetCNTXCellNo()*(pISimBox->GetCNTYCellNo()*k + j) + sliceIndex;

				// I need a local copy of the bead container here because the back_inserter()
				// call seems to fail if I pass in the pISimBox pointer. It might
				// be too many nested function calls.

				BeadList rBeadList = pISimBox->GetCNTCells()[cellIndex]->GetBeads();

				copy(rBeadList.begin(), rBeadList.end(), back_inserter(m_lBeads));
			}
		}

		m_Width		= pISimBox->GetSimBoxYLength();
		m_Depth		= pISimBox->GetSimBoxZLength();
		m_Height	= pISimBox->GetCNTXCellWidth();
	}
	else if(yNormal == 1)
	{
		m_CNTRowsInSlice = pISimBox->GetCNTXCellNo();

		// Loop over the CNT cells adding their beads to the slice's bead container

		for(i=0; i<pISimBox->GetCNTXCellNo(); i++)
		{
			for(k=0; k<pISimBox->GetCNTZCellNo(); k++)
			{
				cellIndex = pISimBox->GetCNTXCellNo()*(pISimBox->GetCNTYCellNo()*k + sliceIndex) + i;

				BeadList rBeadList = pISimBox->GetCNTCells()[cellIndex]->GetBeads();

				copy(rBeadList.begin(), rBeadList.end(), back_inserter(m_lBeads));
			}
		}

		m_Width		= pISimBox->GetSimBoxZLength();
		m_Depth		= pISimBox->GetSimBoxXLength();
		m_Height	= pISimBox->GetCNTYCellWidth();
	}
	else if(zNormal == 1)
	{
		m_CNTRowsInSlice = pISimBox->GetCNTYCellNo();

		// Loop over the CNT cells adding their beads to the slice's bead container

		for(i=0; i<pISimBox->GetCNTXCellNo(); i++)
		{
			for(j=0; j<pISimBox->GetCNTYCellNo(); j++)
			{
				cellIndex = pISimBox->GetCNTXCellNo()*(pISimBox->GetCNTYCellNo()*sliceIndex + j) + i;

				BeadList rBeadList = pISimBox->GetCNTCells()[cellIndex]->GetBeads();

				copy(rBeadList.begin(), rBeadList.end(), back_inserter(m_lBeads));
			}
		}

		m_Width		= pISimBox->GetSimBoxXLength();
		m_Depth		= pISimBox->GetSimBoxYLength();
		m_Height	= pISimBox->GetCNTZCellWidth();
	}

	// Set base class members that depend on the above calculations. It would be nice
	// if they could set automatically in the functions that call them, but that would
	// be inefficient when they are called many times.

	m_Volume   = m_Width*m_Depth*m_Height;
}


// Alternative constructor used by the CSimBox when searching for beads that occur in
// a slice to be made into an aggregate that can received ApplyForce commands.
// The difference from the previous one is that it receives only the three coordinates
// (two of which must be zero) that specify the slice of CNT cells to use.
// Note that the id passed to the CRegion base class is not used for anything.
// Also note that the CNT cell index is 1-based on entry to this routine so that
// the zeroth slice can be distinguished from nothing, but it must be shifted to
// the zero-based indexing used in the CNT cell structure.

CSlice::CSlice(long xCoord, long yCoord, long zCoord, 
		       const ISimBox* const pISimBox) : CRegion(0), m_X(xCoord), m_Y(yCoord), m_Z(zCoord),
												m_CNTRowsInSlice(0)
{
	// The slices store the set of beads that occur in the SimBox whose coordinates fall
	// within their boundaries. We access the bead lists stored in the CNTCells via
	// their CAbstractCell base class. But note that the bead lists are not const so 
	// we have to take care not to change any data in the beads.
	//
	// It is distasteful that we have to repeat the code for calculating the bead lists,
	// but the cell sizes may vary between dimensions so we cannot factor it out.

	long cellIndex, i, j, k;

	if(xCoord > 0)
	{
		xCoord--;
		m_CNTRowsInSlice = pISimBox->GetCNTZCellNo();

		// Loop over the CNT cells adding their beads to the slice's beads

		for(j=0; j<pISimBox->GetCNTYCellNo(); j++)
		{
			for(k=0; k<pISimBox->GetCNTZCellNo(); k++)
			{
				cellIndex = pISimBox->GetCNTXCellNo()*(pISimBox->GetCNTYCellNo()*k + j) + xCoord;

				// I need a local copy of the bead container here because the back_inserter()
				// call seems to fail if I pass in the pISimBox pointer. It might
				// be too many nested function calls.

				BeadList rBeadList = pISimBox->GetCNTCells()[cellIndex]->GetBeads();

				copy(rBeadList.begin(), rBeadList.end(), back_inserter(m_lBeads));
			}
		}
		m_Width		= pISimBox->GetSimBoxYLength();
		m_Depth		= pISimBox->GetSimBoxZLength();
		m_Height	= pISimBox->GetCNTXCellWidth();
	}
	else if(yCoord > 0)
	{
		yCoord--;
		m_CNTRowsInSlice = pISimBox->GetCNTXCellNo();

		// Loop over the CNT cells adding their beads to the slice's bead container

		for(i=0; i<pISimBox->GetCNTXCellNo(); i++)
		{
			for(k=0; k<pISimBox->GetCNTZCellNo(); k++)
			{
				cellIndex = pISimBox->GetCNTXCellNo()*(pISimBox->GetCNTYCellNo()*k + yCoord) + i;

				BeadList rBeadList = pISimBox->GetCNTCells()[cellIndex]->GetBeads();


				copy(rBeadList.begin(), rBeadList.end(), back_inserter(m_lBeads));
			}
		}
		m_Width		= pISimBox->GetSimBoxZLength();
		m_Depth		= pISimBox->GetSimBoxXLength();
		m_Height	= pISimBox->GetCNTYCellWidth();
	}
	else if(zCoord > 0)
	{
		zCoord--;
		m_CNTRowsInSlice = pISimBox->GetCNTYCellNo();

		// Loop over the CNT cells adding their beads to the slice's bead container

		for(i=0; i<pISimBox->GetCNTXCellNo(); i++)
		{
			for(j=0; j<pISimBox->GetCNTYCellNo(); j++)
			{
				cellIndex = pISimBox->GetCNTXCellNo()*(pISimBox->GetCNTYCellNo()*zCoord + j) + i;

				BeadList rBeadList = pISimBox->GetCNTCells()[cellIndex]->GetBeads();

				copy(rBeadList.begin(), rBeadList.end(), back_inserter(m_lBeads));
			}
		}
		m_Width		= pISimBox->GetSimBoxXLength();
		m_Depth		= pISimBox->GetSimBoxYLength();
		m_Height	= pISimBox->GetCNTZCellWidth();
	}
	else
	{
	}

	// Set base class members that depend on the above calculations. It would be nice
	// if they could set automatically in the functions that call them, but that would
	// be inefficient when they are called many times.

	m_Volume    = m_Width*m_Depth*m_Height;
}


// Copy constructor. This must also result in calling the base class copy constructor
// so we pass the original CSlice object to the base class. Because a derived class 
// object can always be used in place of a base class object, the CRegion class treats a
// CSlice as if it were just a CRegion.

CSlice::CSlice(const CSlice& oldSlice) : CRegion(oldSlice)
{
	m_X					= oldSlice.m_X;
	m_Y					= oldSlice.m_Y;
	m_Z					= oldSlice.m_Z;
	m_CNTRowsInSlice	= oldSlice.GetCNTRowsInSlice();
}

CSlice::~CSlice()
{
}
