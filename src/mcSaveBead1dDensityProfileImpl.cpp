/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSaveBead1dDensityProfileImpl.cpp: implementation of the mcSaveBead1dDensityProfileImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSaveBead1dDensityProfileImpl.h"
#include "mcSaveBead1dDensityProfile.h"
#include "Monitor.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Bead1dProfile.h"
#include "LogSaveBead1dDensityProfile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSaveBead1dDensityProfileImpl::mcSaveBead1dDensityProfileImpl()
{
}

mcSaveBead1dDensityProfileImpl::~mcSaveBead1dDensityProfileImpl()
{

}

// Command handler function to save a bead's 1d density profile to file.
// The data is sampled at a specified frequency during the specified time period,
// and the density profiles averaged over all samples. Only one profile is
// output by this command. For a sequence of profiles, the target command
// ctSaveBead1dDensityProfile should be used.

void mcSaveBead1dDensityProfileImpl::SaveBead1dDensityProfile(const xxCommand* const pCommand)
{
	const mcSaveBead1dDensityProfile* const pCmd = dynamic_cast<const mcSaveBead1dDensityProfile*>(pCommand);

	const long beadType         = pCmd->GetBeadType();
	const long start            = pCmd->GetStartTime();
	const long end              = pCmd->GetEndTime();
	const long sample           = pCmd->GetSamplePeriod();
	const long nx               = pCmd->GetXNormal();
	const long ny               = pCmd->GetYNormal();
	const long nz               = pCmd->GetZNormal();
	const long sliceTotal       = pCmd->GetSliceTotal();
	const double lowerFraction  = pCmd->GetLowerFraction();
	const double upperFraction  = pCmd->GetUpperFraction();

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	const zString runId    = pMon->GetISimBox()->GetRunId();
	const long    time     = pMon->GetCurrentTime();
    
	// Append the axis to the bead name to distinguish it from other analyses using the same bead

        const zString normal   = pMon->GetISimBox()->ToString(nx) + pMon->GetISimBox()->ToString(ny) + pMon->GetISimBox()->ToString(nz);
        const zString beadName = pMon->GetISimBox()->GetBeadNameFromType(beadType) + "_" + pMon->GetISimBox()->ToString(beadType) + "_" + normal;

	// Convert the fractional coordinate range into absolute values

	double lowerCoord = 0.0;
	double upperCoord = 0.0;

	if(nx == 1)
	{
		lowerCoord = lowerFraction*pMon->GetSimBoxXLength();
		upperCoord = upperFraction*pMon->GetSimBoxXLength();
	}
	else if( ny == 1)
	{
		lowerCoord = lowerFraction*pMon->GetSimBoxYLength();
		upperCoord = upperFraction*pMon->GetSimBoxYLength();
	}
	else
	{
		lowerCoord = lowerFraction*pMon->GetSimBoxZLength();
		upperCoord = upperFraction*pMon->GetSimBoxZLength();
	}

	// Check that the current time is still before the temporal range for analysis.
	// Note that because we have ensured that end >= start, we don't have to check
	// if the current time is less than the end time.
	// We don't check the sample and analysis periods for now.

	if(time <= start)
	{
		// Collect the beads to include in the profile

		BeadList lBeads;

		AbstractBeadVector vAllBeads = pMon->GetISimBox()->GetBeads();

		for(AbstractBeadVectorIterator iterBead = vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == beadType)
				lBeads.push_back(*iterBead);
		}

		// Create the profile object and pass it to the Monitor for sampling during
		// its active period. Note that the Monitor is responsible for storing
		// and destroying the profiles created as a result of this command.

		CBead1dProfile* pProfile = new CBead1dProfile(runId, time, beadName, lBeads, start, end, sample,
												nx, ny, nz, sliceTotal, pMon->GetSimBoxXLength(), pMon->GetSimBoxYLength(), pMon->GetSimBoxZLength(), 
												lowerCoord, upperCoord);

		pMon->m_DensityFields.push_back(pProfile);

		new CLogSaveBead1dDensityProfile(pMon->GetCurrentTime(), beadName, beadType, start, end, sample, 
                                                 nx, ny, nz, sliceTotal);
	}
	else
	{
		 new CLogCommandFailed(pCmd->GetExecutionTime(), pCmd);
	}

}

