/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveBeadDensityFluctuationsImpl.cpp: implementation of the mcSaveBeadDensityFluctuationsImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveBeadDensityFluctuationsImpl.h"
#include "mcSaveBeadDensityFluctuations.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "prBeadDensityFluctuations.h"
#include "LogSaveBeadDensityFluctuations.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveBeadDensityFluctuationsImpl::mcSaveBeadDensityFluctuationsImpl()
{
}

mcSaveBeadDensityFluctuationsImpl::~mcSaveBeadDensityFluctuationsImpl()
{

}

// Command handler function to calculate the fluctuations in the number of beads
// of a single, specified type within a single density grid cell. If desired, it
// also compares the fluctuations with those in the remainder of the simulation box 
// (the so-called conjugate volume). This command only works if the bead density
// grid analysis has been turned on by command and the specified bead type exists.
// Note that because this command may be applied to dynamically-created bead types,
// the command class cannot validate the bead type. We do it here instead.

void mcSaveBeadDensityFluctuationsImpl::SaveBeadDensityFluctuations(const xxCommand* const pCommand)
{
    const mcSaveBeadDensityFluctuations* const pCmd = dynamic_cast<const mcSaveBeadDensityFluctuations*>(pCommand);

    const zString beadName         = pCmd->GetBeadName();

    // We allow totalDataPoints to be modified in case the duration exceeds the
    // remaining simulation time
    long totalDataPoints           = pCmd->GetTotalDataPoints();

    const long totalDensityPeriods = pCmd->GetTotalDensityPeriods();
    const long xCell               = pCmd->GetXCellNo();
    const long yCell               = pCmd->GetYCellNo();
    const long zCell               = pCmd->GetZCellNo();
    const bool bConjugate          = pCmd->GetConjugateFlag();

    CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

    const long beadType       = pMon->GetISimBox()->GetBeadTypeFromName(beadName);
    const long currentTime    = pMon->GetISimBox()->GetCurrentTime();
    const long densityPeriod  = pMon->GetISimBox()->GetDensityPeriod();
//    const long samplePeriod   = pMon->GetISimBox()->GetSamplePeriod();
    const long totalTime      = pMon->GetISimBox()->GetTotalTime();

    long duration = totalDataPoints*totalDensityPeriods*densityPeriod;

    // The analysis starts at the beginning of the next full density grid period 
    // and continues for an integer number of periods or to the end of the run.
    // Note that because the user can only specify an integer number of density
    // periods, the final sample is guaranteed to be at an integer multiple of 
    // the density period.

    long start = 0;
    long end   = 0;

    if(currentTime%densityPeriod == 0)
    {
        start = currentTime;
    }
    else
    {
        start = (currentTime/densityPeriod + 1)*densityPeriod;
    }

    end = start + duration;

    // Check that the simulation is long enough to allow this many samples and,
    // if not, reduce the number of samples so that a whole number fit into 
    // the remaining simulation time. Note that the commands that initiate this
    // process currently abort before letting through a duration that exceeds
    // the remaining simulation time; we allow it here for future expansion of
    // the commands.

    if(end > totalTime)
    {
        duration = totalTime - start;
        totalDataPoints = duration/(totalDensityPeriods*densityPeriod);
        duration = totalDataPoints*totalDensityPeriods*densityPeriod;
        end = start + duration;
    }

	// Check that the current time is still before the temporal range for analysis.

	if(pMon->IsDensityGridAnalysisOn() && beadType >= 0 && beadType < pMon->GetISimBox()->GetBeadTypeTotal() && currentTime <= start &&  end > start && end <= totalTime)
	{
		// Create the process object.

//        xxProcess* pProcess = acfProcessFactory::Instance()->Create(prBeadDensityFluctuations::GetType());

        prBeadDensityFluctuations* const pProcess = new prBeadDensityFluctuations(pMon->m_pSimState, beadType, totalDataPoints, totalDensityPeriods, xCell, yCell, zCell, bConjugate);
		pProcess->InternalValidateData(pMon->GetISimBox()->IISimState());
		pMon->GetISimBox()->AddProcess(pProcess);

		 new CLogSaveBeadDensityFluctuations(pMon->GetCurrentTime(), beadName, beadType, totalDataPoints, totalDensityPeriods, densityPeriod, 
                                                start, end, xCell, yCell, zCell, bConjugate);
	}
	else
	{
		 new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}

}

