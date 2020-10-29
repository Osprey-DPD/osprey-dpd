/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// RowProfile.cpp: implementation of the CRowProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "RowProfile.h"
#include "Slice.h"
#include "AbstractBead.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor to divide a slice up into a known number of identically-sized rows.
// Each row has its long axis determined by the cross-product of the row normal
// and the original slice normal. The row normal is assigned according to the 
// comments below. This scheme is used so that the long axis of a row always 
// points along a positive coordinate direction.
//
// The rows are stored in the CProfile<> base class.

CRowProfile::CRowProfile(CSlice* const pSlice, long rowTotal) : m_pSlice(pSlice) 
{	
	// A copy of the slice's BeadList is defined here because the for loops 
	// seem to fail when the slice's GetBeads() function is used directly. 
	// An iterator is declared because some machines do not allow
	// a redeclaration of the loop counter in different loops. 

	BeadList lBeads = pSlice->GetBeads();
	cBeadListIterator iterBead;

	long xNormal = 0;
	long yNormal = 0;
	long zNormal = 0;

	if(pSlice->GetXNormal() == 1)		// Slice has X normal so row normal is Z
	{
		zNormal = 1;
	}
	else if(pSlice->GetYNormal() == 1)	// Slice has Y normal so row normal is X
	{
		xNormal = 1;
	}
	else if(pSlice->GetZNormal() == 1)	// Slice has Z normal so row normal is Y
	{
		yNormal = 1;
	}

	// Create the rows

	double rowWidth   = pSlice->GetWidth();
	double rowDepth   = pSlice->GetDepth()/static_cast<double>(rowTotal);
	double rowHeight  = pSlice->GetHeight();

	for(long rowIndex=0; rowIndex<rowTotal; rowIndex++)
	{
		CRow* pRow = new CRow(rowIndex, xNormal, yNormal, zNormal, rowWidth, rowDepth, rowHeight);
		AddRegion(pRow);
	}

	// Sort the beads in the original slice into the rows. Because we only allow the
	// three orientations along the major axes, we have to distinguish them before
	// calculating the row widths. In the general case, we would have a single loop
	// over the beads, but the calculation of the row normals would be complicated.
	// We distribute the beads om the slice amongst the newly-created rows in the 
	// current profile.

	if(xNormal == 1)	// Row normal is X
	{
		for(iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			long xIndex = static_cast<long>((*iterBead)->GetXPos()/rowDepth);
			GetRegion(xIndex)->AddBead(*iterBead);
		}
	}
	else if(yNormal == 1)	// Row normal is Y
	{
		for(iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			long yIndex = static_cast<long>((*iterBead)->GetYPos()/rowDepth);
			GetRegion(yIndex)->AddBead(*iterBead);
		}
	}
	else if(zNormal == 1)	// Row normal is Z
	{
		for(iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			long zIndex = static_cast<long>((*iterBead)->GetZPos()/rowDepth);
			GetRegion(zIndex)->AddBead(*iterBead);
		}
	}
}

// Constructor to create a row profile from a vector of rows. The rows 
// become part of the CRowProfile and will be deleted when the profile object is
// deleted so they must have been created on the heap.

CRowProfile::CRowProfile(RowVector vRows) : m_pSlice(NULL)
{
	for(RowVectorIterator iterRow=vRows.begin(); iterRow!= vRows.end(); iterRow++)
	{
		AddRegion(*iterRow);
	}
}


CRowProfile::~CRowProfile()
{
}


