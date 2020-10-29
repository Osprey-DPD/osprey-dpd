/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// SingleSliceEnergy.cpp: implementation of the CSingleSliceEnergy class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SingleSliceEnergy.h"
#include "ISimBox.h"
#include "TimeSeriesData.h"
#include "CNTCellSlice.h"
#include "CNTCell.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSingleSliceEnergy::CSingleSliceEnergy(const ISimBox* pISimBox, bool bNormalize, 
						   long xn, long yn, long zn, 
						   long sliceIndex) : IRegionAnalysis(pISimBox),
											 m_bNormalize(bNormalize),
											 m_Xn(xn), m_Yn(yn), m_Zn(zn),
											 m_SliceIndex(sliceIndex),
											 m_BeadTotal(0),
											 m_KineticEnergy(0.0),
											 m_PotentialEnergy(0.0),
											 m_TotalEnergy(0.0),
											 m_LowerCoord(0.0), m_UpperCoord(0.0),
											 m_pSlice(0)
{
	// Collect all the CNT cells contained within the specified slice and
	// store them for analysis. Note that the CNT slices are zero-based
	// and a slice with index n extends from n*cell width to (n+1)*cell width.
	// If the slice index lies outside the SimBox we adjust it to the nearest 
	// valid slice.

	if(xn == 1)
	{
		if(m_SliceIndex < 0)
			m_SliceIndex = 0;
		else if(m_SliceIndex >= pISimBox->GetCNTXCellNo())
			m_SliceIndex = pISimBox->GetCNTXCellNo() - 1;

		m_LowerCoord = m_SliceIndex*pISimBox->GetCNTXCellWidth();
		m_UpperCoord = (m_SliceIndex + 1)*pISimBox->GetCNTXCellWidth();
	}
	else if(yn == 1)
	{
		if(m_SliceIndex < 0)
			m_SliceIndex = 0;
		else if(m_SliceIndex >= pISimBox->GetCNTYCellNo())
			m_SliceIndex = pISimBox->GetCNTYCellNo() - 1;

		m_LowerCoord = m_SliceIndex*pISimBox->GetCNTYCellWidth();
		m_UpperCoord = (m_SliceIndex + 1)*pISimBox->GetCNTYCellWidth();
	}
	else
	{
		if(m_SliceIndex < 0)
			m_SliceIndex = 0;
		else if(m_SliceIndex >= pISimBox->GetCNTZCellNo())
			m_SliceIndex = pISimBox->GetCNTZCellNo() - 1;

		m_LowerCoord = m_SliceIndex*pISimBox->GetCNTZCellWidth();
		m_UpperCoord = (m_SliceIndex + 1)*pISimBox->GetCNTZCellWidth();
	}


	// Create a CCNTCellSlice object to hold the cells in the slice. Also 
	// count the number of beads initially in the region.

	m_BeadTotal = 0;

	m_pSlice = new CCNTCellSlice(m_SliceIndex, m_Xn, m_Yn, m_Zn, pISimBox);

	const long firstTotal  = m_pSlice->GetCellsPerSlice();
	const long secondTotal = m_pSlice->GetSize();

	for(long cellIndex=0; cellIndex<m_pSlice->GetSize(); cellIndex++)
	{
		const CCNTCell* const pCell = m_pSlice->GetCell(cellIndex);

		m_BeadTotal += pCell->CellBeadTotal();
	}


}

// Do NOT delete any CNT cells here as we have only copied pointers from one container
// to a local container.

CSingleSliceEnergy::~CSingleSliceEnergy()
{
	delete m_pSlice;
}

// Function to perform the analysis on the specified region.
//
// This class calculates the total energy of all beads contained within the
// specified slice of the SimBox. All CNT cells that belong to the indexed slice
// are included in the analysis.
// 

void CSingleSliceEnergy::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	// Update the total number of beads in the region: although the number of cells
	// is constant, beads can move in and out of the region

	m_BeadTotal		  = 0;
	m_KineticEnergy   = 0.0;
	m_PotentialEnergy = 0.0;
	m_TotalEnergy	  = 0.0;

	double kinetic   = 0.0;
	double potential = 0.0;

	for(long cellIndex = 0; cellIndex < m_pSlice->GetSize(); cellIndex++)
	{
		const CCNTCell* const pCell = m_pSlice->GetCell(cellIndex);

		pCell->UpdateTotalEnergy(&kinetic, &potential);

		m_BeadTotal += pCell->CellBeadTotal();

		m_KineticEnergy   += kinetic;
		m_PotentialEnergy += potential;
	}

	// Normalize the energies if specified, but check that m_BeadTotal is not zero

	if(m_bNormalize && m_BeadTotal > 0)
	{
		m_KineticEnergy /= static_cast<double>(m_BeadTotal);
		m_PotentialEnergy /= static_cast<double>(m_BeadTotal);
	}

	// Insert factor of 2 into kinetic energy

	m_KineticEnergy /= 2.0;

	m_TotalEnergy = m_KineticEnergy + m_PotentialEnergy;

	// Package the data into a CTimeSeriesData object and store it for serialisation

	const long dataTotal = 4;

	CTimeSeriesData* const m_pTSD = new CTimeSeriesData(dataTotal);

	m_pTSD->SetValue(0, pISimBox->GetCurrentTime(), "Time");
	m_pTSD->SetValue(1, m_KineticEnergy,			"KE");
	m_pTSD->SetValue(2, m_PotentialEnergy,			"PE");
	m_pTSD->SetValue(3, m_TotalEnergy,				"totalE");

	AddTimeSeriesData(m_pTSD);

}
