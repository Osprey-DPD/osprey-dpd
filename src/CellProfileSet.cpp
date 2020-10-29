/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CellProfileSet.cpp: implementation of the CCellProfileSet class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "CellProfileSet.h"
#include "RowProfile.h"

#include "AbstractBead.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor taking a CRowProfile and a number of cells, cellTotal, and creating 
// a CCellProfile object containing the specified number of cells from each row.
// It also creates the orthogonal set of cell profiles constructed using the cells
// indexed by their location in the row profiles.

CCellProfileSet::CCellProfileSet(CRowProfile* const pRP, long cellTotal)
{	
	for(RowVectorIterator iterRow=pRP->Begin(); iterRow!=pRP->End(); iterRow++)
	{
		CCellProfile* pCP = new CCellProfile(*iterRow, cellTotal);
		AddRegion(pCP);
	}

	// Create the orthogonal cell profiles by creating new CCell objects holding the 
	// same data as the old ones but stored in new CCellProfile objects. Note that
	// as for the orthogonal row profiles, all the cells in an orthogonal cell profile
	// will have the same id as they are taken from the same location in the original
	// cell profiles. This is not important yet as they are not used for anything.

	for(long index=0; index<cellTotal; index++)
	{
		CellVector vCells;

		for(CellProfileVectorIterator iterCP=Begin(); iterCP!=End(); iterCP++)
		{
			CCellProfile* pOldCP = *iterCP;

			CCell* pCell = new CCell(*(pOldCP->GetRegion(index)));
			vCells.push_back(pCell);
		}	

		CCellProfile* pCP = new CCellProfile(vCells);
		m_vOrthogonalSet.push_back(pCP);
	}

	// Debug for orthogonal cell profile

//	for(long cellProfileIndex=0; cellProfileIndex<GetOrthogonalSize(); cellProfileIndex++)
//	{
//		CCellProfile* const pCP = GetOrthogonalProfile(cellProfileIndex);
//		long myCellSize = pCP->Size();
//		for(long cellIndex=0; cellIndex<pCP->Size(); cellIndex++)
//		{
//			CCell* const pCell = pCP->GetRegion(cellIndex);
//			long myId       = pCell->GetId();
//			double myWidth  = pCell->GetWidth();
//			double myDepth  = pCell->GetDepth();
//			double myHeight = pCell->GetHeight();
//			long myBeadTotal = pCell->GetBeadTotal();
//			BeadList mylBeads = pCell->GetBeads();
//			for(BeadListIterator itBead=mylBeads.begin(); itBead!=mylBeads.end(); itBead++)
//			{
//				long beadId = (*itBead)->GetId();
//				double myX  = (*itBead)->GetXPos();
//				double myY  = (*itBead)->GetYPos();
//				double myZ  = (*itBead)->GetZPos();
//			}
//		}
//	}


}

CCellProfileSet::~CCellProfileSet()
{
	for(CellProfileVectorIterator iterCP=m_vOrthogonalSet.begin(); iterCP!=m_vOrthogonalSet.end(); iterCP++)
	{
		delete *iterCP;
	}
	m_vOrthogonalSet.clear();
}

// Function to return a CCellProfile pointer given its index in the cell profile set.
// We pass the request onto the base class.

CCellProfile* CCellProfileSet::GetProfile(long index)
{
	return GetRegion(index);
}

// Function to return a pointer to a CCellProfile object stored in the orthogonal
// cell profile set.

CCellProfile* CCellProfileSet::GetOrthogonalProfile(long index) const
{
	return m_vOrthogonalSet.at(index);
}
