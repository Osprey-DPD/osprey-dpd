/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ScalarProfileObservable.cpp: implementation of the CScalarProfileObservable class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ScalarProfileObservable.h"
#include "aaScalarProfile.h"


//////////////////////////////////////////////////////////////////////
// Global functions for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CScalarProfileObservable& rOb)
{
	os << rOb.GetName() << zEndl;

	for(long i=0; i<rOb.m_Size; i++)
	{

#if Platform == DECALPHA
		os << std::setw(12) << std::setprecision(8);	// set field width and precision
		os << rOb.m_MeanData.at(i) << "  ";
		os << std::setw(12) << std::setprecision(8);
		os << rOb.m_SDevData.at(i) << zEndl;
#elif Platform == SGICC
		os << setw(12) << setprecision(8);	// set field width and precision
		os << rOb.m_MeanData.at(i) << "  ";
		os << setw(12) << setprecision(8);
		os << rOb.m_SDevData.at(i) << zEndl;
#elif Platform == CRAYJ90
		os << setw(12) << setprecision(8);	// set field width and precision
		os << rOb.m_MeanData.at(i) << "  ";
		os << setw(12) << setprecision(8);
		os << rOb.m_SDevData.at(i) << zEndl;
#else
		os << std::setw(12) << std::setprecision(8);	// set field width and precision
		os << rOb.m_MeanData.at(i) << "  ";
		os << std::setw(12) << std::setprecision(8);
		os << rOb.m_SDevData.at(i) << zEndl;
#endif

	}

	os << zEndl;

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScalarProfileObservable::CScalarProfileObservable(zString name, long analysisPeriod, 
						  long samplePeriod, long size) : CObservable(name, analysisPeriod, samplePeriod),
						                                  m_Size(size)
{
	m_SumData.resize(size);
	m_Sum2Data.resize(size);
	m_MeanData.resize(size);
	m_SDevData.resize(size);

	// Zero element of the vector in case different compilers do not do it

	for(long i=0; i<size; i++)
	{
		m_SumData.at(i)  = 0.0;
		m_Sum2Data.at(i) = 0.0;
		m_MeanData.at(i) = 0.0;
		m_SDevData.at(i) = 0.0;
	}

}

CScalarProfileObservable::~CScalarProfileObservable()
{
}

void CScalarProfileObservable::Normalize(long currentTime)
{
	m_CurrentTime = currentTime;

	// normalize the scalar data set

	for(long i=0; i<m_Size; i++)
	{
		m_MeanData.at(i)	= m_SumData.at(i)/m_SampleSize;
		m_SDevData.at(i)	= m_Sum2Data.at(i)/m_SampleSize - m_MeanData.at(i)*m_MeanData.at(i);

		// We remove the case where the observable is constant over time by
		// checking that the mean and mean square differ by a small amount.

		if(m_SDevData.at(i) < 0.0 && fabs(m_SDevData.at(i)) > 0.000001)
		{
			Trace(GetName() + " observable has variance less than 0");
		}
		else
		{
			m_SDevData.at(i)	= sqrt(m_SDevData.at(i));
		}


		m_SumData.at(i)	 = 0.0;		//reset running sums to zero ready for next sampling period
		m_Sum2Data.at(i) = 0.0;
	}
}

// Debug function to write out the current value of the observable

void CScalarProfileObservable::DisplayDebug()
{
}


// Pure virtual function to allow the CScalarObservable object to write its data
// to file when invoked through a CObservable pointer. 
//
// We cannot use operator<<  by itself because that only invokes the CObservable 
// version and does not behave polymorphically. Instead, we call CObservable's 
// operator<< which invokes the CScalarObservable::Serialize() function which 
// then calls CScalarObservable's operator<< to write the data the the stream 
// passed in with the operator<<.

void CScalarProfileObservable::Serialize(zOutStream& os) const
{
	os << *this;
}

// Function to add a set of scalar values to the observable's running total.

void CScalarProfileObservable::AddData(const aaScalarProfile* const pID)
{
	for(long i=0; i<m_Size; i++)
	{
		m_SumData.at(i)  += pID->GetData(i);
		m_Sum2Data.at(i) += pID->GetData2(i);
	}
}

