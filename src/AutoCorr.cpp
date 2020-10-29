/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// AutoCorr.cpp: implementation of the CAutoCorr class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "AutoCorr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoCorr::CAutoCorr(long n, long m) : m_N(n), m_M(m), m_bNormalized(false), m_SampleIndex(0)
{
	m_Values.resize(m_N);
	m_SampleTotals.resize(m_M);
	m_ProductSum.resize(m_M);

}

CAutoCorr::~CAutoCorr()
{

}

// Function to add a new sample from the observable stream to the calculation.
// Note that when a total of m_N values have been added further values are ignored.

void CAutoCorr::AddSample(double x)
{
	if(m_SampleIndex < m_N)
	{
		m_Values.at(m_SampleIndex) = x;

		for(long m = 0; m < m_M; m++)
		{
			long lagIndex = m_SampleIndex - m;

			if(lagIndex >= 0)
			{
				m_SampleTotals.at(m)++;
				m_ProductSum.at(m) += m_Values.at(lagIndex)*x;
			}
		}

		m_SampleIndex++;
	}
}

// Function to return a vector containing the numbers of contributing products
// x(n).x(n+m) to each time lag m.

zLongVector CAutoCorr::GetSampleTotals()
{
	return m_SampleTotals;
}

// Function to return a vector containing the sum of all products contributing
// to the autocorrelation at a given time lag.

zDoubleVector CAutoCorr::GetAutoCorr()
{
	if(!m_bNormalized)
		Normalize();

	return m_ProductSum;
}

// Function to return the value of the autocorrelation function at a given time lag.

double CAutoCorr::GetAutoCorrValue(long index)
{
	if(!m_bNormalized)
		Normalize();

	if(index >= 0 && index < m_M)
		return m_ProductSum.at(index);
	else
		return 0.0;
}

// Private helper function to calculate the autocorrelation function. We ensure that
// it is only called once by setting a flag.

void CAutoCorr::Normalize()
{
	m_bNormalized = true;

	// Check that the number of factors contributing to the autocorrelation function
	// agrees with that expected for each time lag

	for(long m=0; m<m_M; m++)
	{
		if(m_SampleTotals.at(m) > 0)
			m_ProductSum.at(m) /= static_cast<double>(m_SampleTotals.at(m));
	}
}
