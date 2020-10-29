/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// GridObservable.cpp: implementation of the CGridObservable class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "GridObservable.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridObservable::CGridObservable(const zString name, long analysisPeriod, long samplePeriod,
								 long GridXCellNo, long GridYCellNo, long GridZCellNo, 
								 double GridXCellWidth, double GridYCellWidth, double GridZCellWidth, 
								 long ObjectTotal, 
								 const zString fieldName, long fieldId) : CObservable(name, analysisPeriod, samplePeriod),
								 m_GridXCellNo(GridXCellNo),
							 	 m_GridYCellNo(GridYCellNo),
								 m_GridZCellNo(GridZCellNo),
								 m_GridXCellWidth(GridXCellWidth),
							 	 m_GridYCellWidth(GridYCellWidth),
								 m_GridZCellWidth(GridZCellWidth),
								 m_ObjectTotal(ObjectTotal),
								 m_FieldName(fieldName), m_FieldId(fieldId)
{
	// Set the size of the vectors to hold the grid data. We do this
	// because we access them using the at() function instead of push_back()
	// and at() does not automatically increase the size of the vector.

	  m_vField.resize(m_GridXCellNo*m_GridYCellNo*m_GridZCellNo, 0.0);
	    m_vSum.resize(m_GridXCellNo*m_GridYCellNo*m_GridZCellNo, 0.0);
	m_vDensity.resize(m_GridXCellNo*m_GridYCellNo*m_GridZCellNo, 0.0);

	// Store the product of the number of objects being sampled (beads, bonds or 
	// polymers) and the number of timesteps over which sampling takes place.
	// This allows us to verage the data over the total number of beads
	// (or polymers, or whatever) as well as over the number of timesteps.

	m_DensitySampleSize = m_SampleSize*static_cast<double>(m_ObjectTotal);

}

CGridObservable::~CGridObservable()
{

}

void CGridObservable::DisplayDebug()
{
	long index;

	for(long iz=0; iz<m_GridZCellNo; iz++)
	{
		for(long iy=0; iy<m_GridYCellNo; iy++)
		{
			for(long ix=0; ix<m_GridXCellNo; ix++)
			{
				index = m_GridXCellNo*(m_GridYCellNo*iz + iy) + ix;

				TraceDoubleNoEndl(m_vDensity.at(index));
			}
			Trace("");
		}
		Trace("");
	}
	Trace("");

}

// The Grid cell data is normalized over the number of samples and the number
// of objects, i.e., the number of beads, bonds or polymers, used in the
// sampling. 

void CGridObservable::Normalize(long currentTime)
{
	m_CurrentTime = currentTime;

	long index;

	index = 0;

	for(czDoubleVectorIterator iterSum=(m_vField).begin(); iterSum!=(m_vField).end(); iterSum++)
	{
		m_vSum.at(index)		= (*iterSum)/m_SampleSize;
		m_vDensity.at(index)	= (*iterSum)/m_DensitySampleSize;
        m_vField.at(index)      = 0.0;
		index++;
	}
}

// Function needed for compatibility with the other CObservable-derived classes
// but not used for serialization here as it is already provided by the
// CDensityState class.

void CGridObservable::Serialize(zOutStream& os) const
{
}

