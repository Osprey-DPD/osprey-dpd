/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// DensityFieldState.cpp: implementation of the CDensityFieldState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "DensityFieldState.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDensityFieldState::CDensityFieldState(const zString runId, long currentTime, const zString acnType, const zString dim,
                                       long start, long end,long sample): xxState(xxBase::GetDPPrefix() + runId + "." + acnType + "." + dim + "." + ToString(start) + "." + ToString(end) + ".dens", true, currentTime, runId),
										m_ActualSamples(0),
                                        m_DataType(acnType), m_Start(start), m_End(end), 
                                        m_SamplePeriod(sample),
                                        m_ExpectedSamples(0),
                                        m_bNormalize(false)

{
    // We only do the calculation if the current time is before the start time
    // of the sampling period, and the sampling period range is an exact multiple
    // of the sampling frequency.

    if(currentTime <= m_Start)
    {
        if(m_End == m_Start)  // sampling period is irrelevant for this case
        {
            m_ExpectedSamples  = 1;
        }
        else if(m_SamplePeriod > 0 && (m_End + 1 - m_Start)%m_SamplePeriod == 0)
        {
            m_ExpectedSamples  = (m_End + 1 - m_Start)/m_SamplePeriod;
        }
        else
        {
           m_ExpectedSamples  = 0;
        }
    }
    else
    {
       m_ExpectedSamples  = 0;
    }
}

CDensityFieldState::~CDensityFieldState()
{

}

// Boolean function to show if a sample should be taken or not.

bool CDensityFieldState::IsTimeToSample(long currentTime)
{
    if(currentTime >= GetStartTime() && currentTime < GetEndTime())
    {
        return true;
    }
    else if(currentTime == GetEndTime())
    {
        m_bNormalize = true;
        return true;
    }

    return false;
}

