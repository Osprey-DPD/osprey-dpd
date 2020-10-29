/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// VectorObservable.cpp: implementation of the CVectorObservable class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "VectorObservable.h"
#include "aaVectorSingle.h"

//////////////////////////////////////////////////////////////////////
// Global functions for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CVectorObservable& rOb)
{
#if Platform == DECALPHA
	os << rOb.GetName() << zEndl;
	for(short int j=0; j<3; j++)
	{
		os << std::setw(14) << std::setprecision(8) << zLeft;
		os << rOb.m_MeanData[j] << " ";
		os << std::setw(14) << std::setprecision(8) << zLeft;
		os << rOb.m_SDevData[j] << zEndl;
	}
	os << std::setw(14) << std::setprecision(8) << zLeft;
	os << rOb.m_MeanMag << " ";
	os << std::setw(14) << std::setprecision(8) << zLeft;
	os << rOb.m_SDevMag << zEndl;
	os << zEndl;

#elif Platform == SGICC
	os << rOb.GetName() << zEndl;
	for(short int j=0; j<3; j++)
	{
		os << setw(14) << setprecision(8) << zLeft;
		os << rOb.m_MeanData[j] << " ";
		os << setw(14) << setprecision(8) << zLeft;
		os << rOb.m_SDevData[j] << zEndl;
	}
	os << setw(14) << setprecision(8) << zLeft;
	os << rOb.m_MeanMag << " ";
	os << setw(14) << setprecision(8) << zLeft;
	os << rOb.m_SDevMag << zEndl;
	os << zEndl;
#elif Platform == CRAYJ90
	os << rOb.GetName() << zEndl;
	for(short int j=0; j<3; j++)
	{
		os << setw(14) << setprecision(8) << zLeft;
		os << rOb.m_MeanData[j] << " ";
		os << setw(14) << setprecision(8) << zLeft;
		os << rOb.m_SDevData[j] << zEndl;
	}
	os << setw(14) << setprecision(8) << zLeft;
	os << rOb.m_MeanMag << " ";
	os << setw(14) << setprecision(8) << zLeft;
	os << rOb.m_SDevMag << zEndl;
	os << zEndl;
#else
	os << rOb.GetName() << zEndl;
	for(short int j=0; j<3; j++)
	{
		os << std::setw(14) << std::setprecision(8) << zLeft;
		os << rOb.m_MeanData[j] << " ";
		os << std::setw(14) << std::setprecision(8) << zLeft;
		os << rOb.m_SDevData[j] << zEndl;
	}
	os << std::setw(14) << std::setprecision(8) << zLeft;
	os << rOb.m_MeanMag << " ";
	os << std::setw(14) << std::setprecision(8) << zLeft;
	os << rOb.m_SDevMag << zEndl;
	os << zEndl;
#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVectorObservable::CVectorObservable(zString name, long analysisPeriod, long samplePeriod) : CObservable(name, analysisPeriod, samplePeriod)
{
	for(short int j=0; j<3; j++)
	{
		m_SumData[j]	= 0.0;
		m_Sum2Data[j]	= 0.0;
		m_MeanData[j]	= 0.0;
		m_Mean2Data[j]	= 0.0;
		m_VarData[j]	= 0.0;
		m_SDevData[j]	= 0.0;
	}

	m_SumMag	= 0.0;
	m_Sum2Mag	= 0.0;
	m_MeanMag	= 0.0;
	m_Mean2Mag	= 0.0;
	m_VarMag	= 0.0;
	m_SDevMag	= 0.0;


}

CVectorObservable::~CVectorObservable()
{

}

// Function to normalize the running average of a vector observable over the
// last sampling period.
//
// It calculates the mean value of each of component and their variances
// and the magnitude. It then sets the running totals of the data to zero
// ready for the next averaging period.
//
// The current time is stored in the CObservable base class for use in writing
// the data to file if the observables have different normalisation times. 

void CVectorObservable::Normalize(long currentTime)
{
	m_CurrentTime = currentTime;

	for(short int j=0; j<3; j++)
	{
		m_MeanData[j]	= m_SumData[j]/m_SampleSize;
		m_Mean2Data[j]	= m_Sum2Data[j]/m_SampleSize;
		m_VarData[j]	= m_Mean2Data[j] - m_MeanData[j]*m_MeanData[j];

		// We remove the case where the observable is constant over time by
		// checking that the mean and mean square differ by a small amount.

		if(m_VarData[j] < 0.0 && fabs(m_VarData[j]) > 0.000001)
		{
			Trace(GetName() + " observable has variance less than 0");
		}
		else
		{
			m_SDevData[j] = sqrt(m_VarData[j]);
		}

		m_SumData[j]	= 0.0;
		m_Sum2Data[j]	= 0.0;

	}

	m_MeanMag	= m_SumMag/m_SampleSize;
	m_Mean2Mag	= m_Sum2Mag/m_SampleSize;
	m_VarMag	= m_Mean2Mag - m_MeanMag*m_MeanMag;

	if(m_VarMag < 0.0)
		Trace(GetName() + " magnitude has variance less than 0");
	else
		m_SDevMag= sqrt(m_VarMag);

	m_SumMag	= 0.0;
	m_Sum2Mag	= 0.0;
}

// Function to write out the current mean and variance of the observable
// for debug purposes.

void CVectorObservable::DisplayDebug()
{
	Trace(GetName());
	TraceDouble2(" 1 = ", m_MeanData[0], m_SDevData[0]);
	TraceDouble2(" 2 = ", m_MeanData[1], m_SDevData[1]);
	TraceDouble2(" 3 = ", m_MeanData[2], m_SDevData[2]);
	TraceDouble2("Magnitude = ", m_MeanMag, m_SDevMag);
	TraceEndl();
}


void CVectorObservable::Serialize(zOutStream& os) const
{
	os << *this;
}

// Function to add a single vector value to the observable's running total.

void CVectorObservable::AddData(const aaVectorSingle* const pID)
{
	double mag = 0.0;

	for(short int j=0; j<3; j++)
	{
		m_SumData[j]  += pID->GetData(j);
		m_Sum2Data[j] += pID->GetData2(j);
		mag           += pID->GetData2(j);
	}

	// Calculate the mean of the vector's magnitude

	m_SumMag	+= sqrt(mag);
	m_Sum2Mag	+= mag;
}
