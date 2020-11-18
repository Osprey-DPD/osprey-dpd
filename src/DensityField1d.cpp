/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// DensityField1d.cpp: implementation of the CDensityField1d class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"

#include "DensityField1d.h"
#include "aaRegionToType.h"


	using std::transform;
	using std::slice;
	using std::gslice;
	using std::find;
	using std::find_if;


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDensityField1d::CDensityField1d(const zString runId, long currentTime, const zString acnType, 
								long start, long end,long sample,
								long nx, long ny, long nz, long sliceTotal, double lx, double ly, double lz, 
								double lowerCoord, double upperCoord) : CDensityFieldState(runId, currentTime, acnType, "1d", start, end, sample),
								m_NX(nx), m_NY(ny), m_NZ(nz), m_SliceTotal(sliceTotal), 
								m_LX(lx), m_LY(ly), m_LZ(lz), m_LowerCoord(lowerCoord), m_UpperCoord(upperCoord),
								m_SliceWidth(0.0), m_SliceVolume(0.0)
{

	// Ensure that the slice width is never zero, and calculate the slice volume.

	if(m_UpperCoord > m_LowerCoord && m_SliceTotal > 0)
	{
		m_SliceWidth = (m_UpperCoord - m_LowerCoord)/static_cast<double>(m_SliceTotal);
	
		if(m_NX == 1)
		{
			m_SliceVolume  = m_SliceWidth*m_LY*m_LZ;
		}
		else if(m_NY == 1)
		{
			m_SliceVolume  = m_LX*m_SliceWidth*m_LZ;
		}
		else
		{
			m_SliceVolume  = m_LX*m_LY*m_SliceWidth;
		}
	}
	else
	{
		m_SliceTotal = 1;
		m_SliceVolume = m_LX*m_LY*m_LZ;

		if(m_NX == 1)
		{
			m_SliceWidth  = m_LX;
		}
		else if(m_NY == 1)
		{
			m_SliceWidth  = m_LY;
		}
		else
		{
			m_SliceWidth  = m_LZ;
		}
	}

    // Clear the STL container and set its size to hold the data 
    // so that the derived classes can use it straight away. Note that 
    // this means that only a single time-average can be performed during 
    // the sampling period.

	m_Data.clear();
	m_Data.resize(GetSliceTotal());

	// Write data describing the profile to the output file

	m_outStream << GetStartTime()    << "  " << GetEndTime()    << "  " << GetSamplePeriod() << zEndl;
	m_outStream << GetXNormal()      << "  " << GetYNormal()    << "  " << GetZNormal()      << zEndl;
	m_outStream << GetSliceTotal()   << "  " << GetSliceWidth() << "  "  << GetSliceVolume() << zEndl;
	m_outStream << GetLowerCoord()   << "  " << GetUpperCoord() << zEndl << zEndl;
}

CDensityField1d::~CDensityField1d()
{

}


// PVF to write out the 1d density profile data to file.

bool CDensityField1d::Serialize()
{
	// We check the ioflag as the name of the stream is different for read and write

	if(m_IOFlag)	
	{
		for(zDoubleVectorIterator id=m_Data.begin(); id!=m_Data.end(); id++)
		{
			m_outStream << *id << zEndl;
		}

		if(!m_outStream.good())
			return IOError("Error writing DensityFieldState data to file");

		m_outStream << zEndl;
		m_outStream << zFlush;
	}

	return true;
}
