/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcToggleSliceEnergyOutputImpl.cpp: implementation of the mcToggleSliceEnergyOutputImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcToggleSliceEnergyOutputImpl.h"
#include "mcToggleSliceEnergyOutput.h"
#include "Monitor.h"

#if EnableMonitorCommand == SimCommandEnabled
  #include "SliceEnergy.h"
#endif

#include "LogToggleSliceEnergyOutput.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcToggleSliceEnergyOutputImpl::mcToggleSliceEnergyOutputImpl()
{
}

mcToggleSliceEnergyOutputImpl::~mcToggleSliceEnergyOutputImpl()
{

}

// Command handler to create an IRegionAnalysis object to calculate
// observables relating to the beads in a specific region of the SimBox.
// If the command's GetToggleId() function returns 0, a new IRegionAnalysis object
// is created and made active; if a non-zero value is returned, the 
// specified IRegionAnalysis object is removed from the active regions 
// container (but not deleted) if it is currently active, or else added back 
// if it is currently inactive.
//
// This command creates a slice through the SimBox and calculates the total
// energy, kinetic and potential, of all beads within the slice.
// Interactions between beads in the slice and those in neighbouring slices
// are included, as are all bond and bondpair energies, but not bead-bead 
// interactions for bead pairs that are both outside the region even if 
// they interact with some beads within the slice.

void mcToggleSliceEnergyOutputImpl::ToggleSliceEnergyOutput(const xxCommand* const pCommand)
{
#if EnableMonitorCommand == SimCommandEnabled
	const mcToggleSliceEnergyOutput* const pCmd = dynamic_cast<const mcToggleSliceEnergyOutput*>(pCommand);

	const long toggleId		= pCmd->GetToggleId();
	const bool bNormalize	= pCmd->NormalizePerBead();
	const long xn			= pCmd->GetXNormal();
	const long yn			= pCmd->GetYNormal();
	const long zn			= pCmd->GetZNormal();
	const double lower		= pCmd->GetLowerFractionCoord();
	const double upper		= pCmd->GetUpperFractionCoord();

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	if(toggleId == 0)
	{
		// Create a new CSliceEnergy object and store the CNT cells that occur within
		// the slice: note that the SimBox may have different lengths in each 
		// dimension. The upper and lower fractional bounds of the slice have 
		// already been checked to ensure they lie within [0,1].



		// Create the IRegionAnalysis-derived object and add it to the container
		// of regions and the active container for sampling.

		CSliceEnergy* pRA = new CSliceEnergy(pMon->GetISimBox(), bNormalize, xn, yn, zn, lower, upper);

		pMon->m_Regions.push_back(pRA);

		pMon->m_ActiveRegions.push_back(pRA);
	}
	else
	{
		// Toggle the specified active region off if it is active or on if it
		// is inactive. We check to see if the region with the specified id 
		// is in the m_ActiveRegions container or not.

//		for(cRegionIterator cIterRegion = pMon->m_Regions.begin(); cIterRegion!= pMon->m_Regions.end(); cIterRegion++)
//		{
//			const IRegionAnalysis* pRegion = *cIterRegion;
//		}
	}
#endif
}
