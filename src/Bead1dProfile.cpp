/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Bead1dProfile.cpp: implementation of the CBead1dProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"

#include "Bead1dProfile.h"
#include "aaRegionToType.h"
#include "LogTextMessage.h"


#if Platform == DECALPHA 
#elif Platform == SGICC
	#define BILAYER_NO_USE_VALARRAY 1
#elif Platform == CRAYJ90
	#define BILAYER_NO_USE_VALARRAY 1
#elif Platform == BORLAND6
	using std::transform;
	using std::slice;
	using std::gslice;
	using std::find;
	using std::find_if;
#elif Platform == I7XEON
	using std::transform;
	using std::slice;
	using std::gslice;
#elif Platform == I7ITANIUM
#elif Platform == GCC
#elif Platform == CW55MAC
	using std::transform;
	using std::slice;
	using std::gslice;
	using std::find;
	using std::find_if;
#elif Platform == XCMAC
	using std::transform;
	using std::slice;
	using std::gslice;
	using std::find;
	using std::find_if;
#elif Platform == NEWPLATFORM1
#elif Platform == NEWPLATFORM2
#elif Platform == NEWPLATFORM3
#elif Platform == NEWPLATFORM4
#elif Platform == NEWPLATFORM5
#elif Platform == NEWPLATFORM6
#elif Platform == NEWPLATFORM7
#elif Platform == NEWPLATFORM8
#elif Platform == NEWPLATFORM9
#elif Platform == NEWPLATFORM10
#else							
	using std::transform;
	using std::slice;
	using std::gslice;
	using std::find;
	using std::find_if;
#endif					


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBead1dProfile::CBead1dProfile(const zString runId, long currentTime, const zString acnType, 
								 BeadList lBeads, long start, long end,long sample,
								long nx, long ny, long nz, long sliceTotal, double lx, double ly, double lz, 
								double lowerCoord, double upperCoord) : CDensityField1d(runId, currentTime, acnType, 
								start, end, sample, nx, ny, nz, sliceTotal, lx, ly, lz, lowerCoord, upperCoord),
								m_Beads(lBeads)
{

}

CBead1dProfile::~CBead1dProfile()
{

}

// Function to sample the density field being measured and add the data to 
// the running total. Samples are only taken during the specified sampling period.
// Note that a given CBead1dProfile instance only performs
// a single average of the data over a specified sampling period. Hence, the 
// running totals are zeroed in the constructor.
//
// The data is stored in an STL container and a function object is used to 
// extract the appropriate coordinate for use in the density profile calculation.

void CBead1dProfile::Sample()
{
	zDoubleVector beadCoords(m_Beads.size());

	// The profiles are restricted to the major coordinate axes for now

	if(GetXNormal() == 1)
	{
		transform(m_Beads.begin(), m_Beads.end(), beadCoords.begin(), aaBeadXPos());
	}
	else if(GetYNormal() == 1)
	{
		transform(m_Beads.begin(), m_Beads.end(), beadCoords.begin(), aaBeadYPos());
	}
	else
	{
		transform(m_Beads.begin(), m_Beads.end(), beadCoords.begin(), aaBeadZPos());
	}

	// Now we have the coordinates, bin them and add to the running total

    m_ActualSamples++;  // Keep track of the number of samples

	double beadSampleSize = 0.0;  // Keep track of the number of beads in the profile

	for(zDoubleVectorIterator id=beadCoords.begin(); id!=beadCoords.end(); id++)
	{
		long index = static_cast<long>((*id)/GetSliceWidth());

		if(index == GetSliceTotal())
			index--;

		m_Data.at(index) += 1.0;
		beadSampleSize += 1.0;
	}

	if(beadSampleSize != m_Beads.size())
	{
		const zString msg = "Incorrect total in 1d profile for bead type " + GetDataType();
		new CLogTextMessage(m_CurrentTime, msg);
	}
}

// Function to normalize the density field over the number of samples taken.

void CBead1dProfile::Normalize()
{
    // Reset the normalization flag first so that it occurs only once

    NormalizationDone();

    if(m_ActualSamples == GetExpectedSamples())
    {
	    // Normalize the binned data by the volume of a slice and the sample total

	    for(long id1 = 0; id1 < GetSliceTotal(); id1++)
	    {
		    m_Data.at(id1) /= (static_cast<double>(m_ActualSamples)*GetSliceVolume());
	    }
    }
    else
    {
        // Set all data in the profile to zero to indicate an error in the sampling

	    for(zDoubleVectorIterator id=m_Data.begin(); id!=m_Data.end(); id++)
	    {
            *id = 0.0;
        }

    }
}
