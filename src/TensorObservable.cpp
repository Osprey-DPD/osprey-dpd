/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// TensorObservable.cpp: implementation of the CTensorObservable class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aaTensorSingle.h"
#include "TensorObservable.h"


//////////////////////////////////////////////////////////////////////
// Global functions for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CTensorObservable& rOb)
{
	os << rOb.GetName() << zEndl;

	for(short int j=0; j<3; j++)
	{

#if Platform == DECALPHA
		os << std::setw(12) << std::setprecision(6) << zLeft;	// set field width and precision
		os << rOb.m_MeanData[3*j]   << " ";
		os << std::setw(12) << std::setprecision(6) << zLeft;
		os << rOb.m_MeanData[3*j+1] << " ";
		os << std::setw(12) << std::setprecision(6) << zLeft;
		os << rOb.m_MeanData[3*j+2] << "  ";
		os << std::setw(12) << std::setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j] << " ";
		os << std::setw(12) << std::setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j+1] << " ";
		os << std::setw(12) << std::setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j+2] << zEndl;
#elif Platform == SGICC
		os << setw(12) << setprecision(6) << zLeft;	// set field width and precision
		os << rOb.m_MeanData[3*j]   << " ";
		os << setw(12) << setprecision(6) << zLeft;
		os << rOb.m_MeanData[3*j+1] << " ";
		os << setw(12) << setprecision(6) << zLeft;
		os << rOb.m_MeanData[3*j+2] << "  ";
		os << setw(12) << setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j] << " ";
		os << setw(12) << setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j+1] << " ";
		os << setw(12) << setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j+2] << zEndl;
#elif Platform == CRAYJ90
		os << setw(12) << setprecision(6) << zLeft;	// set field width and precision
		os << rOb.m_MeanData[3*j]   << " ";
		os << setw(12) << setprecision(6) << zLeft;
		os << rOb.m_MeanData[3*j+1] << " ";
		os << setw(12) << setprecision(6) << zLeft;
		os << rOb.m_MeanData[3*j+2] << "  ";
		os << setw(12) << setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j] << " ";
		os << setw(12) << setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j+1] << " ";
		os << setw(12) << setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j+2] << zEndl;
#else
		os << std::setw(12) << std::setprecision(6) << zLeft;	// set field width and precision
		os << rOb.m_MeanData[3*j]   << " ";
		os << std::setw(12) << std::setprecision(6) << zLeft;
		os << rOb.m_MeanData[3*j+1] << " ";
		os << std::setw(12) << std::setprecision(6) << zLeft;
		os << rOb.m_MeanData[3*j+2] << "  ";
		os << std::setw(12) << std::setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j] << " ";
		os << std::setw(12) << std::setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j+1] << " ";
		os << std::setw(12) << std::setprecision(6) << zLeft;
		os << rOb.m_SDevData[3*j+2] << zEndl;
#endif

	}

	os << zEndl;

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTensorObservable::CTensorObservable(zString name, long analysisPeriod, long samplePeriod) : CObservable(name, analysisPeriod, samplePeriod)
{
	for(short int j=0; j<9; j++)
	{
		m_SumData[j]	= 0.0;
		m_Sum2Data[j]	= 0.0;
		m_MeanData[j]	= 0.0;
		m_Mean2Data[j]	= 0.0;
		m_VarData[j]	= 0.0;
		m_SDevData[j]	= 0.0;
	}
}

CTensorObservable::~CTensorObservable()
{

}

// Function to normalize the data set collected during the last sampling period.

void CTensorObservable::Normalize(long currentTime)
{
	m_CurrentTime = currentTime;

	for(short int j=0; j<9; j++)
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
}

void CTensorObservable::DisplayDebug()
{
	Trace(GetName());
	for(short int i=0; i<3; i++)
	{
		TraceVector("", m_MeanData[3*i], m_MeanData[3*i+1], m_MeanData[3*i+2]);
	}
	TraceEndl();
	for(short int j=0; j<3; j++)
	{
		TraceVector("", m_SDevData[3*j], m_SDevData[3*j+1], m_SDevData[3*j+2]);
	}
}

// Function to pass the serialize message to this class' "operator<<".

void CTensorObservable::Serialize(zOutStream& os) const
{
	os << *this;
}

// Function to add a single tensor value to the observable's running total.

void CTensorObservable::AddData(const aaTensorSingle* const pID)
{
	for(short int j=0; j<9; j++)
	{
		m_SumData[j]  += pID->GetData(j);
		m_Sum2Data[j] += pID->GetData2(j);
	}
}
