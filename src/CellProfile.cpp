/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CellProfile.cpp: implementation of the CCellProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "CellProfile.h"
#include "Row.h"
#include "AbstractBead.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCellProfile::CCellProfile(CRow* const pRow, long cellTotal) : m_pRow(pRow)
{
	// A copy of the row's beads is defined here because the for loops 
	// seem to fail when the row's GetBeads() function is used directly. 
	// An iterator is declared here because some machines do not allow
	// a redeclaration of the loop counter in different loops.

	BeadList lBeads = pRow->GetBeads();
	cBeadListIterator iterBead;

	// Create the cells by dividing the row into cellTotal parts. The cell
	// width and depth correspond to the row's length/cellTotal and width.

	double cellWidth  = pRow->GetWidth()/static_cast<double>(cellTotal);
	double cellDepth  = pRow->GetDepth();
	double cellHeight = pRow->GetHeight();

	for(long cellIndex=0; cellIndex<cellTotal; cellIndex++)
	{
		CCell* pCell = new CCell(cellIndex, cellWidth, cellDepth, cellHeight);
		AddRegion(pCell);
	}

	// Sort the beads in the original row into the cells. Because we only allow the
	// three orientations along the major axes, we have to distinguish them before
	// assigning the beads. In the general case, we would have a single loop
	// over the beads, but the calculation of the normals would be complicated.

	if(pRow->GetXNormal() == 1)			// Row normal is X so cell axis is Z
	{
		long zIndex;
		for(iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			zIndex = static_cast<long>((*iterBead)->GetZPos()/cellWidth);
			GetRegion(zIndex)->AddBead(*iterBead);
		}
	}
	else if(pRow->GetYNormal() == 1)	// Row normal is Y so cell axis is X
	{
		long xIndex;
		for(iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			xIndex = static_cast<long>((*iterBead)->GetXPos()/cellWidth);
			GetRegion(xIndex)->AddBead(*iterBead);
		}
	}
	else if(pRow->GetZNormal() == 1)	// Row normal is Z so cell axis is Y
	{
		long yIndex;
		for(iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
		{
			yIndex = static_cast<long>((*iterBead)->GetYPos()/cellWidth);
			GetRegion(yIndex)->AddBead(*iterBead);
		}
	}


//	for(CellVectorIterator iCell=Begin(); iCell!=End(); iCell++)
//	{
//		long id = (*iCell)->GetId();
//		long nBeads = (*iCell)->GetBeadTotal();
//	}

}

// Constructor to create a cell profile from a vector of cells. The cells 
// become part of the CCellProfile and will be deleted when the profile object is
// deleted so they must have been created on the heap.

CCellProfile::CCellProfile(CellVector vCells) : m_pRow(NULL)
{
	for(CellVectorIterator iterCell=vCells.begin(); iterCell!= vCells.end(); iterCell++)
	{
		AddRegion(*iterCell);
	}
}

CCellProfile::~CCellProfile()
{

}
