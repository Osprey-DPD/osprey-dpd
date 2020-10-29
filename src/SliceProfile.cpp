/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// SliceProfile.cpp: implementation of the CSliceProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SliceProfile.h"
#include "ISimBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Default constructor used to create a profile to which pre-existing slices 
// may be added from other profiles.

CSliceProfile::CSliceProfile()
{
}

// Constructor to cover the whole SimBox with a profile of slices.			 			 
// The slice profile creates the number of slices specified with normals in the
// given direction. Each slice object is then added to the CProfile<> base class
// to be stored in the set of regions for that profile object. A region may be
// either a CSlice, CRow or CCell (or any later additional volumes).
// Note that each slice is responsible for collecting its own beads from the
// ISimBox interface.

CSliceProfile::CSliceProfile(long sliceTotal, long xNormal, long yNormal, long zNormal, 
							 const ISimBox* const pISimBox)
{
	// Start at the bottom of the SimBox with the CNT and slice coordinates
	// equal to 0.0. Set the width of the grid slices from the SimBox size in the 
	// direction of the slice normal and the total number of slices. Then store
	// the grid slice width and the CNT cell width so that we can call the
	// CSlice constructor independently of the normal direction.
	// Create the slice object and add it to the profile via its
	// CRegion base class. The slices are deleted by the base class.

	double sliceWidth	 = 0.0;
	double CNTSliceWidth = 0.0;
	long   CNTSliceTotal = 0;

	if(xNormal == 1)
	{
		sliceWidth		= pISimBox->GetSimBoxXLength()/static_cast<double>(sliceTotal);
		CNTSliceWidth	= pISimBox->GetCNTXCellWidth();
		CNTSliceTotal	= pISimBox->GetCNTXCellNo();
	}
	else if(yNormal == 1)
	{
		sliceWidth		= pISimBox->GetSimBoxYLength()/static_cast<double>(sliceTotal);
		CNTSliceWidth	= pISimBox->GetCNTYCellWidth();
		CNTSliceTotal	= pISimBox->GetCNTYCellNo();
	}
	else
	{
		sliceWidth		= pISimBox->GetSimBoxZLength()/static_cast<double>(sliceTotal);
		CNTSliceWidth	= pISimBox->GetCNTZCellWidth();
		CNTSliceTotal	= pISimBox->GetCNTZCellNo();
	}

	// Call the appropriate CSlice constructor to handle the creation of a slice 
	// depending on whether its width is the same, less than, or greater than 
	// the CNT cells in the appropriate normal direction. We pass in the 
	// coordinates of the lower and upper boundaries of the grid slice and the 
	// indices pointing to the CNT cells that contain these boundaries.

	const double invCNTSliceWidth = 1.0/CNTSliceWidth;

	double lowerSliceCoord = 0.0;
	double upperSliceCoord = 0.0;

	CSlice* pSlice = NULL;

	for(long sliceIndex=0; sliceIndex<sliceTotal; sliceIndex++)
	{
		// Calculate upperSliceCoord by multiplying the width by the number of slices. 
		// We don't use a manual increment at the end of the loop because multiple 
		// additions leads to loss of accuracy.
		// For example, a slice width of 0.6 when added 10 times leads to a
		// coordinate value of 6.0 to 15 dp but when this is cast to a long integer
		// it rounds down to 5. The difference 6.0-upperSliceCoord is 8.8e-16.
		// But this is sufficient to change the cast.

		upperSliceCoord = static_cast<double>(sliceIndex+1)*sliceWidth;

		long lowerCNTSliceIndex = static_cast<long>(lowerSliceCoord*invCNTSliceWidth);
		long upperCNTSliceIndex = static_cast<long>(upperSliceCoord*invCNTSliceWidth);
		
		// If the upper CNT slice index points to the edge of the SimBox, reduce
		// it by one because CNT cells are numbered (0, N-1).

		if(upperCNTSliceIndex == CNTSliceTotal)
			upperCNTSliceIndex--;

		if(sliceWidth < CNTSliceWidth)
		{
			pSlice = new CSlice(sliceIndex, lowerCNTSliceIndex, upperCNTSliceIndex, 
								lowerSliceCoord, upperSliceCoord, xNormal, yNormal, zNormal, pISimBox);
		}
		else if(sliceWidth > CNTSliceWidth)
		{
			pSlice = new CSlice(sliceIndex, lowerCNTSliceIndex, upperCNTSliceIndex,
								lowerSliceCoord, upperSliceCoord, xNormal, yNormal, zNormal, pISimBox);
		}
		else
		{
			pSlice = new CSlice(sliceIndex, xNormal, yNormal, zNormal, pISimBox);
		}

		AddRegion(pSlice);

		lowerSliceCoord = upperSliceCoord;
	}

}

CSliceProfile::~CSliceProfile()
{
}

