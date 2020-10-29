/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// RowProfileSet.cpp: implementation of the CRowProfileSet class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "RowProfileSet.h"
#include "SliceProfile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that takes a slice profile and a number of rows, rowTotal, and 
// creates a set of row profiles, each containing rowTotal rows, from them. 
// The slice profile does not have to cover the whole SimBox, but may consist of 
// only a few slices or even a single one.
//
// Each row profile is stored in the CProfile<> base class object which also 
// handles the deletion of the objects at the end of their lives. 
//
// We also construct the orthogonal row profile set in which the first row 
// in each old profile is combined into a new profile, the second row in each profile 
// forms the second profile, etc. These profile sets are stored locally not in the
// base class so we have to delete their profiles in the destructor.
// We use the CRow copy constructor to create new ones. Because the CRows only contain 
// pointers to beads we don't need to destroy anything within them.


CRowProfileSet::CRowProfileSet(CSliceProfile* const pSP, long rowTotal)
{
	for(SliceVectorIterator iterSlice=pSP->Begin(); iterSlice!=pSP->End(); iterSlice++)
	{
		CRowProfile* pRP = new CRowProfile(*iterSlice, rowTotal);

		AddRegion(pRP);
	}

	// Create the orthogonal row profiles by creating new CRow objects holding the 
	// same data as the old ones but stored in new CRowProfile objects. The number of
	// orthogonal row profiles is set by rowTotal, and the number of rows in each
	// orthogonal row profile is set by the number of slices.

	// We take the jth rows from each of the original profiles and use them to 
	// construct a new profile. But note that the ids of the rows in the orthogonal
	// profiles will be the same within a profile. This is because we use the
	// copy constructor to copy the row and the same rows are taken from each old
	// profile and their ids are just copied. As the ids are not used this is not
	// important.

	for(long row=0; row<rowTotal; row++)
	{
		RowVector vRows;

		for(long profile=0; profile<Size(); profile++)
		{
			CRowProfile* pOldRP = GetRegion(profile);
			CRow* pRow = new CRow(*(pOldRP->GetRegion(row)));
			vRows.push_back(pRow);
		}

		CRowProfile* pRP = new CRowProfile(vRows);

		m_vOrthogonalSet.push_back(pRP);
	}
}

CRowProfileSet::~CRowProfileSet()
{
	for(RowProfileVectorIterator iterRP=m_vOrthogonalSet.begin(); iterRP!=m_vOrthogonalSet.end(); iterRP++)
	{
		delete *iterRP;
	}
	m_vOrthogonalSet.clear();
}

// Function to return a CRowProfile object pointer given its index into the
// set of row profiles. We pass the request to the base class.

CRowProfile* CRowProfileSet::GetProfile(long index)
{
	return GetRegion(index);
}

// Function to return a pointer to a CRowProfile object from the orthogonal
// set of row profiles.

CRowProfile* CRowProfileSet::GetOrthogonalProfile(long index) const
{
	return m_vOrthogonalSet.at(index);
}
