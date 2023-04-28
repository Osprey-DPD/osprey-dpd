/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveSAXSImpl.cpp: implementation of the mcSaveSAXSImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveSAXSImpl.h"
#include "mcSaveSAXS.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "prSAXS.h"
#include "LogSaveSAXS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveSAXSImpl::mcSaveSAXSImpl()
{
}

mcSaveSAXSImpl::~mcSaveSAXSImpl()
{

}

// Command handler function to calculate the SAXS scattering function for a selected set of polymer types.
// The prSAXS process implements the Debye formula for a specified range of q scattering wave vector values.
// All data is assumed to have been validated in the calling command. But note that we have to set the default range
// for the scattering vector here if the user hasn't specified it.

void mcSaveSAXSImpl::SaveSAXS(const xxCommand* const pCommand)
{
	const mcSaveSAXS* const pCmd = dynamic_cast<const mcSaveSAXS*>(pCommand);

    const long   analysisPeriods  = pCmd->GetAnalysisPeriods();
    const long   qPoints          = pCmd->GetTotalDataPoints();
          double qMin             = pCmd->GetQMin();
          double qMax             = pCmd->GetQMax();
    const zBoolVector vIncluded   = pCmd->GetIncludedBeads();
    
    // If the user hasn't specified the scattering vector range, we use the default of the inverse box size and bead diameter.
    
    if(qMin == 0.0 && qMax == 0.0)
    {
        qMin = xxBase::m_globalTwoPI/IGlobalSimBox::Instance()->GetSimBoxZLength();
        qMax = xxBase::m_globalTwoPI;
    }
        
	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	const long currentTime    = pMon->GetISimBox()->GetCurrentTime();
    const long analysisPeriod = pMon->GetISimBox()->GetAnalysisPeriod();
    const long samplePeriod   = pMon->GetISimBox()->GetSamplePeriod();
    const long totalTime      = pMon->GetISimBox()->GetTotalTime();
    
    long duration = analysisPeriods*analysisPeriod;

    // The analysis starts at the beginning of the next full analysis period 
    // and continues for an integer number of periods.

    long start = 0;
    long end   = 0;

    if(currentTime%analysisPeriod == 0)
    {
        start = currentTime;
    }
    else
    {
        start = (currentTime/analysisPeriod + 1)*analysisPeriod;
    }

    end = start + duration;
    
 	// Check that the current time is still before the temporal range for analysis.

	if(samplePeriod > 0 && end <= totalTime && duration%samplePeriod == 0 )
	{
		// Create the process object.
        
        // Create a map containing the types of beads to include in the calculation. We use a map because we can
        // check membership rapidly. Note that some of the bead types may have been dynamically created, but they
        // must exist at the time this command is executed.
        
        LongLongMap  mBeadTypes;
        
        long key=0;
        for(czBoolVectorIterator ind=vIncluded.begin(); ind!=vIncluded.end(); ++ind)
        {
            if((*ind))
            {
//                std::cout << "Including bead " << key << " " << pMon->GetISimBox()->GetBeadNameFromType(key) << zEndl;
                mBeadTypes.insert(zPairLL(key, key));
            }
            ++key;
        }
                
        prSAXS* pProcess =  new prSAXS(pMon->m_pSimState, analysisPeriods, qPoints, qMin, qMax, mBeadTypes);
        
        if(pProcess)
        {
		    pProcess->InternalValidateData(pMon->GetISimBox()->IISimState());
		
		    pMon->GetISimBox()->AddProcess(pProcess);

		    new CLogSaveSAXS(pMon->GetCurrentTime(), analysisPeriods, qPoints, qMin, qMax, start, end, samplePeriod, vIncluded);
        }
        else
        {
            new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
        }
	}
	else
	{
		new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}

}

