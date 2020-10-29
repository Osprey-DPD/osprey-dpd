/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Raft.cpp: implementation of the CRaft class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "Raft.h"

#include "aaRegionToType.h"		// includes CNTCell.h

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRaft::CRaft(zLongVector headTypes, zLongVector tailTypes) : m_HeadBeadTypes(headTypes),
															 m_TailBeadTypes(tailTypes),
															 m_PolymerTotal(0)
{
	m_Cells.Clear();
}

CRaft::~CRaft()
{

}

// Function to count the total number of polymers in the raft. We identify raft
// polymers by the types of their head beads, which must therefore be distinct
// from all other polymer head bead types. Note that not all cells will contain
// raft polymers as we also include the empty cells that neighbour raft cells.
// But the count_if() algorithm takes care of this.

long CRaft::GetPolymerTotal()
{
	m_PolymerTotal = 0;

	for(cCNTCellIterator cIterCell=m_Cells.Begin(); cIterCell!=m_Cells.End(); cIterCell++)
	{
		const BeadList lBeads = (*cIterCell)->GetBeads();

		for(czLongVectorIterator cIterBeadType=m_HeadBeadTypes.begin(); cIterBeadType!=m_HeadBeadTypes.end(); cIterBeadType++)
		{
			const long beadType = *cIterBeadType;

			const long beadNo = count_if(lBeads.begin(), lBeads.end(), aaBeadType(*cIterBeadType));

			m_PolymerTotal += beadNo;
		}
	}

	return m_PolymerTotal;
}
