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

// Command handler function to calculate the RDF of a selected bead type in a selected polymer type and write it to file.

void mcSaveSAXSImpl::SaveSAXS(const xxCommand* const pCommand)
{
	const mcSaveSAXS* const pCmd = dynamic_cast<const mcSaveSAXS*>(pCommand);

    const long analysisPeriods  = pCmd->GetAnalysisPeriods();
    const long qPoints          = pCmd->GetTotalDataPoints();
    const zBoolVector vExcluded = pCmd->GetExcludedPolymers();
    
    std::cout << "Executing SAXS process " << analysisPeriods << " " << qPoints << " " << vExcluded.size() << zEndl;
    
	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	const long currentTime    = pMon->GetISimBox()->GetCurrentTime();
    const long analysisPeriod = pMon->GetISimBox()->GetAnalysisPeriod();
    const long samplePeriod   = pMon->GetISimBox()->GetSamplePeriod();
    const long totalTime      = pMon->GetISimBox()->GetTotalTime();
    
//    const long polymerType    = pMon->GetISimBox()->IISimState()->GetPolymerTypeFromName(polymerName);

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
        
        // Create a map containing the types of polymers to include in the calculation. We use a map because we can
        // check membership rapidly. Note this map is the inverse of the exclusion map that is passed in.
        
        LongLongMap  mPolyTypes;
        
        long key=0;
        for(czBoolVectorIterator ind=vExcluded.begin(); ind!=vExcluded.end(); ++ind)
        {
            if(!(*ind))
            {
                std::cout << "Including polymer " << key << " " << pMon->GetISimBox()->GetPolymerNameFromType(key) << zEndl;
                mPolyTypes.insert(zPairLL(key, key));
            }
            ++key;
        }
                

        prSAXS* const pProcess = new prSAXS(pMon->m_pSimState, analysisPeriods, qPoints, mPolyTypes);
        
		pProcess->InternalValidateData(pMon->GetISimBox()->IISimState());
		
		pMon->GetISimBox()->AddProcess(pProcess);

		new CLogSaveSAXS(pMon->GetCurrentTime(), analysisPeriods, qPoints,
                                                 start, end, samplePeriod, vExcluded);
	}
	else
	{
		new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}

}

