/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CNTCellSlice.cpp: implementation of the CCNTCellSlice class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "CNTCellSlice.h"
#include "ISimBox.h"
#include "CNTCell.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCNTCellSlice::CCNTCellSlice(long sliceIndex, long xNormal, long yNormal, long zNormal, 
							 const ISimBox* const pISimBox) : m_SliceIndex(sliceIndex),
															  m_XN(xNormal), m_YN(yNormal), m_ZN(zNormal),
															  m_XCellNo(pISimBox->GetCNTXCellNo()), 
															  m_YCellNo(pISimBox->GetCNTYCellNo()), 
															  m_ZCellNo(pISimBox->GetCNTZCellNo()), 
															  m_CellsPerSlice(0)
{
	// Copy the CNT cells into a local vector, calculate the number of cells
	// per slice and the index of the first cell in the selected slice.
	// The spacing between cells depends on the slice normal, and is simply
	// 1 for slices with Z normals.
	//
	// Note that all indices are zero-based.

	const CNTCellVector& rCells = pISimBox->GetCNTCells();

	if(m_XN == 1)
	{
		m_CellsPerSlice = m_YCellNo*m_ZCellNo;

		// Now loop over z slices  and y rows adding in the cell in each row
		// specified by m_SliceIndex

		for(long zIndex = 0; zIndex < m_ZCellNo; zIndex++)
		{
			for(long yIndex = 0; yIndex < m_YCellNo; yIndex++)
			{
				long cellIndex = zIndex*m_XCellNo*m_YCellNo + yIndex*m_XCellNo + m_SliceIndex;

				CCNTCell* pCell = rCells.at(cellIndex);
				m_Cells.AddCell(pCell);
			}
		}
	}
	else if(m_YN == 1)
	{
		m_CellsPerSlice = m_XCellNo*m_ZCellNo;

		// Now loop over z slices adding in all of the cells in the row specified 
		// by m_SliceIndex

		for(long zIndex = 0; zIndex < m_ZCellNo; zIndex++)
		{
			for(long xIndex = 0; xIndex < m_XCellNo; xIndex++)
			{
				long cellIndex = zIndex*m_XCellNo*m_YCellNo + m_SliceIndex*m_XCellNo + xIndex;

				CCNTCell* pCell = rCells.at(cellIndex);
				m_Cells.AddCell(pCell);
			}
		}
	}
	else
	{
		m_CellsPerSlice = m_XCellNo*m_YCellNo;


		// Loop over all CNT Cells adding them to the contained CCNTCellProfile object.
		// For Z normal slices, the counter is just increment by 1 as all cells are within
		// a single slice.

		for(long cellIndex=m_SliceIndex*m_CellsPerSlice; cellIndex < (m_SliceIndex+1)*m_CellsPerSlice; cellIndex++)
		{
			CCNTCell* pCell = rCells.at(cellIndex);
			m_Cells.AddCell(pCell);
		}
	}

}

CCNTCellSlice::~CCNTCellSlice()
{

}

// Fowarding functions to the enclosed CCNTCellProfile object. These are not inline
// to decouple the object from the CCNTCellSlice header.

long CCNTCellSlice::GetSize() const
{
	return m_Cells.Size();
}

const CCNTCell* CCNTCellSlice::GetCell(long index)
{
	return m_Cells.GetCell(index);
}

