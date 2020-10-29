/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// DensityState.cpp: implementation of the CDensityState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "DensityState.h"
#include "GridObservable.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDensityState::CDensityState(long currentTime, const zString runId, 
                             const CGridObservable* pOb): xxState(xxBase::GetDSPrefix() + runId + "." + pOb->m_FieldName + "." + ToString(currentTime) + ".dens", true, currentTime, runId),
                                                          m_FieldName(pOb->GetFieldName()),
                                                          m_FieldId(pOb->GetFieldId()),
                                                          m_GridXCellNo(pOb->GetGridXCellNo()),
														  m_GridYCellNo(pOb->GetGridYCellNo()),
														  m_GridZCellNo(pOb->GetGridZCellNo()),
														  m_GridXCellWidth(pOb->GetGridXCellWidth()),
														  m_GridYCellWidth(pOb->GetGridYCellWidth()),
														  m_GridZCellWidth(pOb->GetGridZCellWidth()),
														  m_pDensity(&pOb->m_vDensity),
														  m_pSum(&pOb->m_vSum)														  
{
}

CDensityState::~CDensityState()
{
}

bool CDensityState::Serialize()
{
	// Note that the widths of the cells are assumed to be the same in all dimensions

	if(m_IOFlag)	
	{
		m_outStream << "#GRID ";
		m_outStream << m_GridXCellNo << "  "
					<< m_GridYCellNo << "  " 
					<< m_GridZCellNo << "  " << m_GridXCellWidth << zEndl;

		long index;

		for(long iz=0; iz<m_GridZCellNo; iz++)
		{
			for(long iy=0; iy<m_GridYCellNo; iy++)
			{
				for(long ix=0; ix<m_GridXCellNo; ix++)
				{
					index = m_GridXCellNo*(m_GridYCellNo*iz + iy) + ix;
					m_outStream << m_pDensity->at(index) << " ";
				}
				m_outStream << zEndl;
			}
			m_outStream << zEndl;
		}

		if(!m_outStream.good())
			return IOError("Error writing DensityState data to file");
		m_outStream << zFlush;
	}

	return true;
}

// Function to return the normalised density value in the given grid cell.

double CDensityState::GetDensity(long ix, long iy, long iz) const
{
    double density = 0.0;

    if(ix >= 0 && ix < m_GridXCellNo &&
       iy >= 0 && iy < m_GridYCellNo &&
       iz >= 0 && iz < m_GridZCellNo)
    {
        const long index = m_GridXCellNo*(m_GridYCellNo*iz + iy) + ix;
        density = (*m_pDensity).at(index);
    }

    return density;
}

// Function to return the (unnormalised) data value in the given grid cell.

double CDensityState::GetSum(long ix, long iy, long iz) const
{
    double sum = 0.0;

    if(ix >= 0 && ix < m_GridXCellNo &&
       iy >= 0 && iy < m_GridYCellNo &&
       iz >= 0 && iz < m_GridZCellNo)
    {
        const long index = m_GridXCellNo*(m_GridYCellNo*iz + iy) + ix;
        sum = (*m_pSum).at(index);
    }

    return sum;
}

// Function to return a string identifier for the field's grid data;
// this is typically a bead type name.

const zString CDensityState::GetFieldName() const
{
    return m_FieldName;
}

// Function to return a numeric identifier for the field's grid data,
// this is typically a bead's numeric type. Because dynamically-created
// bead types may have the name "Unnamed" we use their unique numeric type
// instead.

long CDensityState::GetFieldId() const
{
    return m_FieldId;
}
