/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeActiveBond1dProfile.cpp: implementation of the aeActiveBond1dProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"

#include "aeActiveBond1dProfile.h"
#include "aaRegionToType.h"
#include "LogTextMessage.h"

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

aeActiveBond1dProfile::aeActiveBond1dProfile(const zString runId, long currentTime, const zString acnType, 
								 ActiveBondList lBonds, long start, long end,long sample,
								long nx, long ny, long nz, long sliceTotal, double lx, double ly, double lz, 
								double lowerCoord, double upperCoord) : CDensityField1d(runId, currentTime, acnType, 
								start, end, sample, nx, ny, nz, sliceTotal, lx, ly, lz, lowerCoord, upperCoord),
								m_FreeBonds(lBonds)
{

}

aeActiveBond1dProfile::~aeActiveBond1dProfile()
{

}

// Function to add sample the density field being measured and add the data to 
// the running total.
//
// The data is stored in an ActiveBondList and a function object is used to 
// extract the appropriate coordinate for use in the density profile calculation.

void aeActiveBond1dProfile::Sample()
{
	// Note that we have to size the receiving container so that it has enough
	// space to hold the data

	zDoubleVector bondCoords(m_FreeBonds.size());

	// The profiles are restricted to the major coordinate axes for now

	if(GetXNormal() == 1)
	{
		transform(m_FreeBonds.begin(), m_FreeBonds.end(), bondCoords.begin(), aaActiveBondXPos());
	}
	else if(GetYNormal() == 1)
	{
		transform(m_FreeBonds.begin(), m_FreeBonds.end(), bondCoords.begin(), aaActiveBondYPos());
	}
	else
	{
		transform(m_FreeBonds.begin(), m_FreeBonds.end(), bondCoords.begin(), aaActiveBondZPos());
	}

	// Now we have the coordinates, bin them and create the profile. We first reset
	// the base class m_Data container to hold the new data

	m_Data.clear();
	m_Data.resize(GetSliceTotal());

	double sampleSize = 0.0;

	for(zDoubleVectorIterator id=bondCoords.begin(); id!=bondCoords.end(); id++)
	{
		long index = static_cast<long>((*id)/GetSliceWidth());

		if(index == GetSliceTotal())
			index--;

		m_Data.at(index) += 1.0;
		sampleSize += 1.0;
	}

	if(sampleSize != m_FreeBonds.size())
	{
		const zString msg = "Incorrect total in 1d profile for ACN " + GetDataType();
		new CLogTextMessage(m_CurrentTime, msg);

	}
	// Normalize the binned data by the number of active bonds sampled

	for(long id1 = 0; id1 < GetSliceTotal(); id1++)
	{
		m_Data.at(id1) /= GetSliceVolume();
	}



}

// Function to normalize the averages of the density field.

void aeActiveBond1dProfile::Normalize()
{

}
