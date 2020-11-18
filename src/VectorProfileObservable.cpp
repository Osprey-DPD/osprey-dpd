/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// VectorProfileObservable.cpp: implementation of the CVectorProfileObservable class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "VectorProfileObservable.h"
#include "aaVectorProfile.h"


//////////////////////////////////////////////////////////////////////
// Global functions for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CVectorProfileObservable& rOb)
{
	os << rOb.GetName() << zEndl;

	for(long i=0; i<rOb.m_Size; i++)
	{

		os << std::setw(8) << std::setprecision(4);	// set field width and precision
		os << rOb.m_MeanX.at(i) << "  ";
		os << std::setw(8) << std::setprecision(4);	
		os << rOb.m_MeanY.at(i) << "  ";
		os << std::setw(8) << std::setprecision(4);	
		os << rOb.m_MeanZ.at(i) << "  ";
		os << std::setw(8) << std::setprecision(4);	
		os << rOb.m_MeanMag.at(i)   << "  ";

		os << std::setw(8) << std::setprecision(4);
		os << rOb.m_SDevX.at(i) << "  ";
		os << std::setw(8) << std::setprecision(4);
		os << rOb.m_SDevY.at(i) << "  ";
		os << std::setw(8) << std::setprecision(4);
		os << rOb.m_SDevZ.at(i) << "  ";
		os << std::setw(8) << std::setprecision(4);
		os << rOb.m_SDevMag.at(i) << zEndl;

	}

	os << zEndl;

	return os;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVectorProfileObservable::CVectorProfileObservable(zString name, long analysisPeriod, long samplePeriod, 
												   long size) : CObservable(name, analysisPeriod, samplePeriod),
												   m_Size(size)
{
	m_SumX.resize(size);
	m_SumY.resize(size);
	m_SumZ.resize(size);
	m_SumMag.resize(size);

	m_SumX2.resize(size);
	m_SumY2.resize(size);
	m_SumZ2.resize(size);
	m_Sum2Mag.resize(size);

	m_MeanX.resize(size);
	m_MeanY.resize(size);
	m_MeanZ.resize(size);
	m_MeanMag.resize(size);

	m_SDevX.resize(size);
	m_SDevY.resize(size);
	m_SDevZ.resize(size);
	m_SDevMag.resize(size);

	// Zero elements of the vectors in case different compilers do not do it

	for(long i=0; i<size; i++)
	{
		m_SumX.at(i)	= 0.0;
		m_SumY.at(i)	= 0.0;
		m_SumZ.at(i)	= 0.0;
		m_SumMag.at(i)	= 0.0;

		m_SumX2.at(i)	= 0.0;
		m_SumY2.at(i)	= 0.0;
		m_SumZ2.at(i)	= 0.0;
		m_Sum2Mag.at(i)	= 0.0;

		m_MeanX.at(i)	= 0.0;
		m_MeanY.at(i)	= 0.0;
		m_MeanZ.at(i)	= 0.0;
		m_MeanMag.at(i)	= 0.0;

		m_SDevX.at(i)	= 0.0;
		m_SDevY.at(i)	= 0.0;
		m_SDevZ.at(i)	= 0.0;
		m_SDevMag.at(i)	= 0.0;
	}

}

CVectorProfileObservable::~CVectorProfileObservable()
{

}

void CVectorProfileObservable::Normalize(long currentTime)
{
	m_CurrentTime = currentTime;

	for(long i=0; i<m_Size; i++)
	{
		m_MeanX.at(i)	= m_SumX.at(i)/m_SampleSize;
		m_MeanY.at(i)	= m_SumY.at(i)/m_SampleSize;
		m_MeanZ.at(i)	= m_SumZ.at(i)/m_SampleSize;
		m_MeanMag.at(i)	= m_SumMag.at(i)/m_SampleSize;

		m_SDevX.at(i)	= m_SumX2.at(i)/m_SampleSize - m_MeanX.at(i)*m_MeanX.at(i);
		m_SDevY.at(i)	= m_SumY2.at(i)/m_SampleSize - m_MeanY.at(i)*m_MeanY.at(i);
		m_SDevZ.at(i)	= m_SumZ2.at(i)/m_SampleSize - m_MeanZ.at(i)*m_MeanZ.at(i);
		m_SDevMag.at(i)	= m_Sum2Mag.at(i)/m_SampleSize - m_MeanMag.at(i)*m_MeanMag.at(i);

		// We remove the case where the observable is constant over time by
		// checking that the mean and mean square differ by a small amount.

		if(m_SDevX.at(i) < 0.0 && fabs(m_SDevX.at(i)) > 0.000001)
		{
			Trace(GetName() + "observable has X variance less than 0.0");
		}
		else
		{
			m_SDevX.at(i) = sqrt(m_SDevX.at(i));
		}
	
		if(m_SDevY.at(i) < 0.0 && fabs(m_SDevY.at(i)) > 0.000001)
		{
			Trace(GetName() + "observable has Y variance less than 0.0");
		}
		else
		{
			m_SDevY.at(i) = sqrt(m_SDevY.at(i));
		}
	
		if(m_SDevZ.at(i) < 0.0 && fabs(m_SDevZ.at(i)) > 0.000001)
		{
			Trace(GetName() + "observable has Z variance less than 0.0");
		}
		else
		{
			m_SDevZ.at(i) = sqrt(m_SDevZ.at(i));
		}
	
		if(m_SDevMag.at(i) < 0.0 && fabs(m_SDevMag.at(i)) > 0.000001)
		{
			Trace(GetName() + "observable has mag variance less than 0.0");
		}
		else
		{
			m_SDevMag.at(i) = sqrt(m_SDevMag.at(i));
		}

		m_SumX.at(i)	= 0.0;		// reset running sums to zero
		m_SumY.at(i)	= 0.0;		
		m_SumZ.at(i)	= 0.0;
		m_SumMag.at(i)	= 0.0;

		m_SumX2.at(i)	= 0.0;
		m_SumY2.at(i)	= 0.0;		
		m_SumZ2.at(i)	= 0.0;		
		m_Sum2Mag.at(i)	= 0.0;
	}
}

// Debug function to write out the current values stored in the profile observable.

void CVectorProfileObservable::DisplayDebug()
{

}

// Pure virtual function to allow the CVectorProfileObservable to write its data
// to file when invoked via a CObservable pointer.

void CVectorProfileObservable::Serialize(zOutStream& os) const
{
	os << *this;
}

// Function to add a set of 3D vector values to the observable's running total. There
// is one vector for each slice in the profile across the SimBox.

void CVectorProfileObservable::AddData(const aaVectorProfile *const pID)
{
	double mag;

	for(long i=0; i<m_Size; i++)
	{
		double xval = pID->GetData(i,0);
		double yval = pID->GetData(i,1);
		double zval = pID->GetData(i,2);

		m_SumX.at(i) += pID->GetData(i,0);
		m_SumY.at(i) += pID->GetData(i,1);
		m_SumZ.at(i) += pID->GetData(i,2);

		m_SumX2.at(i) += pID->GetData2(i,0);
		m_SumY2.at(i) += pID->GetData2(i,1);
		m_SumZ2.at(i) += pID->GetData2(i,2);

		mag = pID->GetData2(i,0) + pID->GetData2(i,1) + pID->GetData2(i,2);

		m_SumMag.at(i)  += sqrt(mag);
		m_Sum2Mag.at(i) += mag;
	}

}


