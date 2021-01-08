/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// SliceEnergy.cpp: implementation of the CSliceEnergy class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SliceEnergy.h"
#include "ISimBox.h"
#include "TimeSeriesData.h"
#include "CNTCellSlice.h"
#include "CNTCell.h"

#include "SingleSliceEnergy.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSliceEnergy::CSliceEnergy(const ISimBox* pISimBox, bool bNormalize, 
						   long xn, long yn, long zn, double lowerFraction, 
						   double upperFraction) : IRegionAnalysis(pISimBox),
													 m_bNormalize(bNormalize),
													 m_Xn(xn), m_Yn(yn), m_Zn(zn),
													 m_LowerFraction(lowerFraction),
													 m_UpperFraction(upperFraction),
													 m_BeadTotal(0),
													 m_KineticEnergy(0.0),
													 m_PotentialEnergy(0.0),
													 m_TotalEnergy(0.0),
													 m_LowerCoord(0.0), 
													 m_UpperCoord(0.0)
{
	// Collect all the CNT cells contained within the specified slice region and
	// store them for analysis. If the lower or upper coordinates of the region
	// lie outside the SimBox we adjust them to the SimBox size; and if the
	// lower fraction is less than the upper we adjust them to select the whole SimBox.

	if(m_LowerFraction < 0.0) {
		m_LowerFraction = 0.0; }
	
	if(m_UpperFraction > 1.0) {
		m_UpperFraction = 1.0; }

	if(m_LowerFraction > m_UpperFraction)
	{
		m_LowerFraction = 0.0;
		m_UpperFraction = 1.0;
	}

	if(xn == 1)
	{
		m_LowerCoord = m_LowerFraction*pISimBox->GetSimBoxXLength();
		m_UpperCoord = m_UpperFraction*pISimBox->GetSimBoxXLength();
	}
	else if(yn == 1)
	{
		m_LowerCoord = m_LowerFraction*pISimBox->GetSimBoxYLength();
		m_UpperCoord = m_UpperFraction*pISimBox->GetSimBoxYLength();
	}
	else
	{
		m_LowerCoord = m_LowerFraction*pISimBox->GetSimBoxZLength();
		m_UpperCoord = m_UpperFraction*pISimBox->GetSimBoxZLength();
	}

	// Collect the CNT cells that occur in the slice region and pass them to 
	// the CSliceEnergy region analysis object. Note that by using CNT cells 
	// as the basis for the analysis we allow for beads that enter and leave 
	// the specified region. This avoids having to update the set of beads to
	// analyse on every sample.

	// Note that the lastSliceIndex is one greater than the actual last slice,
	// but the loop below takes care of this.

	long firstSliceIndex = 0;
	long lastSliceIndex  = 0;

	if( xn == 1 )
	{
		firstSliceIndex = static_cast<long>(m_LowerCoord/pISimBox->GetCNTXCellWidth());
		lastSliceIndex  = static_cast<long>(m_UpperCoord/pISimBox->GetCNTXCellWidth());
	}
	else if( yn == 1 )
	{
		firstSliceIndex = static_cast<long>(m_LowerCoord/pISimBox->GetCNTYCellWidth());
		lastSliceIndex  = static_cast<long>(m_UpperCoord/pISimBox->GetCNTYCellWidth());
	}
	else
	{
		firstSliceIndex = static_cast<long>(m_LowerCoord/pISimBox->GetCNTZCellWidth());
		lastSliceIndex  = static_cast<long>(m_UpperCoord/pISimBox->GetCNTZCellWidth());
	}

	// Iterate over all the slices of CNT cells that lie within the specified region,
	// create a CCNTCellSlice object to hold the cells in each slice and add the cells
	// to a local container. Also count the number of beads initially in the region.

	m_Cells.Clear();
	m_BeadTotal = 0;

	for(long index = firstSliceIndex; index < lastSliceIndex; index++)
	{
		CCNTCellSlice slice1(index, xn, yn, zn, pISimBox);

		for(long cellIndex=0; cellIndex<slice1.GetSize(); cellIndex++)
		{
			CCNTCell* pCell = const_cast<CCNTCell*>(slice1.GetCell(cellIndex));

			m_Cells.AddCell(pCell);
			m_BeadTotal += pCell->CellBeadTotal();
		}
	}
}

// Do NOT delete any CNT cells here as we have only copied pointers from one container
// to a local container.

CSliceEnergy::~CSliceEnergy()
{
}

// Function to perform the analysis on the specified region.
//
// This class calculates the total energy of all beads contained within the
// specified slice of the SimBox. All CNT cells that are between, or intersected
// by, the m_LowerFractionCoord and m_UpperFractionCoord are included in the
// analysis.
// 
// Because the CCNTCell::UpdateTotalEnergy() function returns the kinetic and potential
// energies of all beads in the cell to the CMonitor::AddBeadEnergy() function, which
// adds them to running totals for all cells, we need to zero the totals in the monitor
// before updatig the slice energy and then retrieve the total at the end.
//
void CSliceEnergy::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	// Update the total number of beads in the region: although the number of cells
	// is constant, beads can move in and out of the region

	m_BeadTotal	   = 0;
	m_KineticEnergy   = 0.0;
	m_PotentialEnergy = 0.0;
	m_TotalEnergy	  = 0.0;

	double kinetic   = 0.0;
	double potential = 0.0;

	for(long cellIndex = 0; cellIndex < m_Cells.Size(); cellIndex++)
	{
		const CCNTCell* const pCell = m_Cells.GetCell(cellIndex);

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
	m_pTSD->SetValue(1, m_KineticEnergy,		"KE");
	m_pTSD->SetValue(2, m_PotentialEnergy,	"PE");
	m_pTSD->SetValue(3, m_TotalEnergy,		"totalE");

	AddTimeSeriesData(m_pTSD);

}
