/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Frap.cpp: implementation of the CFrap class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimNumericFlags.h"
#include "Frap.h"
#include "aaRegionToType.h"
#include "CNTCell.h"


// STL using declarations

	using std::accumulate;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFrap::CFrap(long beadType) : m_BeadType(beadType)
{

}

CFrap::~CFrap()
{

}

// Function to update the state of the FRAP analysis tool.
// On the first entry, it counts the number of beads of the specified 
// type in the CNT cell profile. These form the region that the bleached 
// polymers occupy in a FRAP experiment. On subsequent calls,
// it counts the number of such beads that remain in the marked cells.
// This represents the observable that is followed to estimate the diffusion
// of fluorescent beads back in to the bleached region.
//
// The cell totals and the current total are only updated in this routine. So
// we use inline functions to return their values for analysis.

void CFrap::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	// On the first call, find the CNT cells that contain beads of the 
	// specified type; on subsequent calls count the number of such beads 
	// left in the cells.

	if(m_CNTCellProfile.Size() == 0)
	{			
		for(cCNTCellIterator cIterCell=pISimBox->GetCNTCells().begin(); cIterCell!=pISimBox->GetCNTCells().end(); cIterCell++)
		{
			const BeadList lBeads = (*cIterCell)->GetBeads();

			cBeadListIterator iterBead = find_if(lBeads.begin(), lBeads.end(), aaBeadType(m_BeadType));

			if(iterBead!=lBeads.end())	// CNT cell contains specified beads
			{
				m_CNTCellProfile.AddCell(*cIterCell);
			}
		}

		// Resize the vector holding bead numbers for each cell

		m_vCellTotal.resize(m_CNTCellProfile.Size());
	}

	// Perform the analysis

	transform(m_CNTCellProfile.Begin(), m_CNTCellProfile.End(), m_vCellTotal.begin(),  aaCNTCellNumber(m_BeadType));

	m_CurrentTotal = accumulate(m_vCellTotal.begin(), m_vCellTotal.end(), 0);

}
