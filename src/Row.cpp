/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Row.cpp: implementation of the CRow class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "Row.h"
#include "Slice.h"
#include "AbstractBead.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRow::CRow(long id, long xNormal, long yNormal, long zNormal,
		   double width, double depth, double height) : CRegion(id, width, depth, height),
														m_X(xNormal), m_Y(yNormal), m_Z(zNormal)
														
{
}

// Alternative constructor that takes a CSlice and collects all the beads in
// a specified row of it for use in the SelectxxxInRow and AddxxxInRow functions.
// We set the CRegion's id to 0 and use its constructor that does not specify 
// values for the size of the region. These are then set in the constructor body.
//
// Note that the row normal is fixed by the orientation of the slice, and the 
// nubmer of rows in the slice by the number of CNT cells in that dimension 
// of the SimBox. We use one index to indicate the position of the row in its 
// bounding slice. This routine does not access the CNT cells directly only via 
// the CSlice that has already sorted beads into itself.
// Hence it is possible to use this constructor even if the slice size is
// independent of the CNT cell structure. However, the number of rows in the
// slice must be set before this routine is called.

CRow::CRow(const CSlice& rSlice, long rowIndex) : CRegion(0), m_X(0), m_Y(0), m_Z(0)														
{
	// Get the size of the row from its bounding slice

	m_Width  = rSlice.GetWidth();
	m_Depth  = rSlice.GetDepth()/static_cast<double>(rSlice.GetCNTRowsInSlice());
	m_Height = rSlice.GetHeight();

	m_Volume = m_Width*m_Depth*m_Height;

	// Sort the beads in the slice into the row. 

	// A copy of the slice's BeadList is defined here because the for loops 
	// seem to fail when the slice's GetBeads() function is used directly. 
	// An iterator is declared because some machines do not allow
	// a redeclaration of the loop counter in different loops. 

	BeadList lBeads = rSlice.GetBeads();
	cBeadListIterator iterBead;

	if(rSlice.GetYNormal() == 1)		// Slice has Y normal so Row normal is X
	{
		m_X = 1;
		for(iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			long xIndex = static_cast<long>((*iterBead)->GetXPos()/m_Depth);
			if(xIndex == rowIndex)
				AddBead(*iterBead);
		}
	}
	else if(rSlice.GetZNormal() == 1)	// Slice has Z normal so Row normal is Y
	{
		m_Y = 1;
		for(iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			long yIndex = static_cast<long>((*iterBead)->GetYPos()/m_Depth);
			if(yIndex == rowIndex)
				AddBead(*iterBead);
		}
	}
	else if(rSlice.GetXNormal() == 1)	// Slice has X normal Row normal is Z
	{
		m_Z = 1;
		for(iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			long zIndex = static_cast<long>((*iterBead)->GetZPos()/m_Depth);
			if(zIndex == rowIndex)
				AddBead(*iterBead);
		}
	}
}

// Copy constructor. This must also result in calling the base class copy constructor
// so we pass the original CRow object to the base class. Because a derived class object
// can always be used in place of a base class object, the CRegion class treats a
// CRow as if it were just a CRegion.

CRow::CRow(const CRow& oldRow) : CRegion(oldRow)
{
	m_X = oldRow.m_X;
	m_Y = oldRow.m_Y;
	m_Z = oldRow.m_Z;
}


CRow::~CRow()
{
}
