/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Observable.cpp: implementation of the CObservable class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "Observable.h"


//////////////////////////////////////////////////////////////////////
// Global functions for serialization
//////////////////////////////////////////////////////////////////////

// Operator<< is used to pass the Serialize message down to the derived classes
// via the virtual Serialize() function. This then passes the message onto
// the derived class's operator<<.
//
// We leave each derived class to serialize its own data.

zOutStream& operator<<(zOutStream& os, const CObservable& rOb)
{
	rOb.Serialize(os);

	return os;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We assume analysisPeriod is an exact multiple of samplePeriod so that
// the analysis occurs after an integer number of samples.  We make 
// m_SampleSize double because it is used to normalize the data and 
// we thus avoid having to cast it.

CObservable::CObservable(zString name, long analysisPeriod, long samplePeriod) : m_AnalysisPeriod(analysisPeriod),
																					 m_SamplePeriod(samplePeriod),
																					 m_SampleSize(static_cast<double>(analysisPeriod/samplePeriod)),
																					 m_CurrentTime(0),
																					 m_Name(name)
{
}

CObservable::~CObservable()
{
}

// Default function that is overridden by any derived class that wants to display
// its data in debug mode.

void CObservable::DisplayDebug()
{
}
