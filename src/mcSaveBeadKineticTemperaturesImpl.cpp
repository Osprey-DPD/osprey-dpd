/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveBeadKineticTemperaturesImpl.cpp: implementation of the mcSaveBeadKineticTemperaturesImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveBeadKineticTemperaturesImpl.h"
#include "mcSaveBeadKineticTemperatures.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "prBeadKineticTemperatures.h"
#include "LogSaveBeadKineticTemperatures.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveBeadKineticTemperaturesImpl::mcSaveBeadKineticTemperaturesImpl()
{
}

mcSaveBeadKineticTemperaturesImpl::~mcSaveBeadKineticTemperaturesImpl()
{

}

// Command handler function to average the kinetic temperatures of all bead types
// separately and write them to file.

void mcSaveBeadKineticTemperaturesImpl::SaveBeadKineticTemperatures(const xxCommand* const pCommand)
{
	const mcSaveBeadKineticTemperatures* const pCmd = dynamic_cast<const mcSaveBeadKineticTemperatures*>(pCommand);

    const long analysisPeriods = pCmd->GetAnalysisPeriods();
    const long dataPoints      = pCmd->GetTotalDataPoints();

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	const zString runId       = pMon->GetISimBox()->GetRunId();
	const long currentTime    = pMon->GetISimBox()->GetCurrentTime();
    const long analysisPeriod = pMon->GetISimBox()->GetAnalysisPeriod();
    const long samplePeriod   = pMon->GetISimBox()->GetSamplePeriod();
    const long totalTime      = pMon->GetISimBox()->GetTotalTime();
    

    long duration = analysisPeriods*analysisPeriod;
    long samples  = duration/samplePeriod;

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

 //   const zString beadName = pMon->GetISimBox()->GetBeadNameFromType(beadType) + "_" + pMon->GetISimBox()->ToString(beadType);

	// Check that the current time is still before the temporal range for analysis.

	if(samplePeriod > 0 && dataPoints > 0 && end <= totalTime && 
       duration%samplePeriod == 0 && dataPoints > 0 && samples%dataPoints == 0)
	{
        long samplesPerPoint = samples/dataPoints;

		// Create the process object.

        long beadTypeTotal = pMon->GetISimBox()->GetBeadTypeTotal(); // this is a kludge for now!

//        xxProcess* pProcess = acfProcessFactory::Instance()->Create(prBeadKineticTemperatures::GetType());

        prBeadKineticTemperatures* const pProcess = new prBeadKineticTemperatures(pMon->m_pSimState, analysisPeriods, dataPoints);
		pProcess->InternalValidateData(pMon->GetISimBox()->IISimState());
		pMon->GetISimBox()->AddProcess(pProcess);

		new CLogSaveBeadKineticTemperatures(pMon->GetCurrentTime(), analysisPeriods, dataPoints, 
                                                start, end, samplePeriod, samplesPerPoint, beadTypeTotal);
	}
	else
	{
		 new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}

}

