/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// IModifyActiveCellNetwork.cpp: implementation of the IModifyActiveCellNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "IModifyActiveCellNetwork.h"
#include "acSaveActiveBond1dDensityField.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IModifyActiveCellNetwork::IModifyActiveCellNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn,
												   double xw, double yw, double zw) : aeActiveCellNetwork(pShadow, xn, yn, zn, xw, yw, zw)
{
}

IModifyActiveCellNetwork::~IModifyActiveCellNetwork()
{
}

// Command handler function to cancel checking whether free active bonds should be
// displayed in current state snapshots or not. Note that the display status is
// left unchanged.

void IModifyActiveCellNetwork::CancelFreeActiveBondDisplayUpdate()
{
	SetFreeBondDisplayUpdateStatus(false);
}

// Command handler function to cancel checking whether free active polymers should be
// displayed in current state snapshots or not.


void IModifyActiveCellNetwork::CancelFreeActivePolymerDisplayUpdate()
{
	SetFreePolymerDisplayUpdateStatus(false);
}

// Command handler functions to set all free active bonds of the specified ACN
// visible/invisible. They are implemented here because they are common to all ACNs,

void IModifyActiveCellNetwork::SetFreeActiveBondsInvisible()
{
	// Mark free bonds invisible 

	SetFreeBondDisplayStatus(false);

	// and tell the ACN to update their display status regularly

	SetFreeBondDisplayUpdateStatus(true);
}

void IModifyActiveCellNetwork::SetFreeActiveBondsVisible()
{
	// Mark free bonds visible 

	SetFreeBondDisplayStatus(true);

	// and tell the ACN to update their display status regularly

	SetFreeBondDisplayUpdateStatus(true);
}

// Command handler function to set all free active polymers of the specified ACN
// invisible. It is implemented here because it is common to all ACNs,
//
// When this function is executed it sets a flag in the ACN so that it automatically
// updates the polymers' visibility state each time the ACN state is updated.
//

void IModifyActiveCellNetwork::SetFreeActivePolymersInvisible()
{
	// Mark free polymers invisible

	SetFreePolymerDisplayStatus(false);

	// and tell the ACN to update their display status regularly

	SetFreePolymerDisplayUpdateStatus(true);
}

void IModifyActiveCellNetwork::SetFreeActivePolymersVisible()
{
	// Mark free polymers visible

	SetFreePolymerDisplayStatus(true);

	// and tell the ACN to update their display status regularly

	SetFreePolymerDisplayUpdateStatus(true);
}

// Command handler function to make the visibility of free active polymers belonging
// to a specified ACN depend on their length, ie. number of monomers.
// If this test is activated, its effect is AND-ed with the SetFreeActivePolymersVisible 
// and SetFreeActivePolymersInvisible functions as well as any other polymer 
// visibility tests.

void IModifyActiveCellNetwork::SetFreeActivePolymersVisibleByMinLength(long minSize)
{
	SetFreePolymerDisplayByMinLength(minSize);
}

// Command handler function to allow an ACN to save a 1d density profile of its
// active bonds to file. The command is passed to the base class of all ACNs
// that implements the profile calculation.

void IModifyActiveCellNetwork::Save1dDensityField(const xxCommand* const pCommand)
{
	const acSaveActiveBond1dDensityField* const pCmd = dynamic_cast<const acSaveActiveBond1dDensityField*>(pCommand);

	const long start		= pCmd->GetStartTime();
	const long end			= pCmd->GetEndTime();
	const long sample		= pCmd->GetSamplePeriod();
	const long analysis		= pCmd->GetAnalysisPeriod();
	const long nx			= pCmd->GetXNormal();
	const long ny			= pCmd->GetYNormal();
	const long nz			= pCmd->GetZNormal();
	const long sliceTotal	= pCmd->GetSliceTotal();
	const double lower		= pCmd->GetLowerFraction();
	const double upper		= pCmd->GetUpperFraction();

	// Convert the fractional coordinate range into absolute values

	double lowerCoord = 0.0;
	double upperCoord = 0.0;

	if(nx == 1)
	{
		lowerCoord = lower*GetSimBoxXLength();
		upperCoord = upper*GetSimBoxXLength();
	}
	else if( ny == 1)
	{
		lowerCoord = lower*GetSimBoxYLength();
		upperCoord = upper*GetSimBoxYLength();
	}
	else
	{
		lowerCoord = lower*GetSimBoxZLength();
		upperCoord = upper*GetSimBoxZLength();
	}

	Add1dDensityField(start, end, sample, analysis, nx, ny, nz, sliceTotal, lowerCoord, upperCoord);

}

// Forwarding functions to the base class' functions to return event identifier strings
// and event instances.

const zString IModifyActiveCellNetwork::GetPolymerFormsEventType() const
{
	return GetInternalPolymerFormsEventType();
}

const zString IModifyActiveCellNetwork::GetPolymerDissolvesEventType() const
{
	return GetInternalPolymerDissolvesEventType();
}

const zString IModifyActiveCellNetwork::GetBondBindsToPolymerHeadEventType() const
{
	return GetInternalBondBindsToPolymerHeadEventType();
}

const zString IModifyActiveCellNetwork::GetBondBindsToPolymerTailEventType() const
{
	return GetInternalBondBindsToPolymerTailEventType();
}

const zString IModifyActiveCellNetwork::GetBondUnbindsFromPolymerHeadEventType() const
{
	return GetInternalBondUnbindsFromPolymerHeadEventType();
}

const zString IModifyActiveCellNetwork::GetBondUnbindsFromPolymerTailEventType() const
{
	return GetInternalBondUnbindsFromPolymerTailEventType();
}

aevActiveEvent* IModifyActiveCellNetwork::GetPolymerFormsEvent()
{
	return GetInternalPolymerFormsEvent();
}

aevActiveEvent* IModifyActiveCellNetwork::GetPolymerDissolvesEvent()
{
	return GetInternalPolymerDissolvesEvent();
}

aevActiveEvent* IModifyActiveCellNetwork::GetBondBindsToPolymerHeadEvent()
{
	return GetInternalBondBindsToPolymerHeadEvent();
}

aevActiveEvent* IModifyActiveCellNetwork::GetBondBindsToPolymerTailEvent()
{
	return GetInternalBondBindsToPolymerTailEvent();
}

aevActiveEvent* IModifyActiveCellNetwork::GetBondUnbindsFromPolymerHeadEvent()
{
	return GetInternalBondUnbindsFromPolymerHeadEvent();
}

aevActiveEvent* IModifyActiveCellNetwork::GetBondUnbindsFromPolymerTailEvent()
{
	return GetInternalBondUnbindsFromPolymerTailEvent();
}

// Forwarding function to the base class to restrict the number of free
// active polymers for a given ACN.

void IModifyActiveCellNetwork::SetActivePolymerLimit(long max)
{
	SetMaxPolymers(max);
}

// Forwarding function to the base class to keep the concentration of free
// active bonds constant by using phantom bonds.

void IModifyActiveCellNetwork::SetConstantFreeBondConcentration(double fraction, long phantomsPerEvent)
{
    SetConstantFreeBonds(fraction, phantomsPerEvent);
}

// Forwarding function to the base class to cancel the effect of an ACN maintaining
// a constant fraction of free active bonds.

void IModifyActiveCellNetwork::CancelConstantFreeBondConcentration()
{
    CancelConstantFreeBonds();
}

// Command handler function to toggle the collection of event statistics
// between accumulating the total number of successes and failures, and
// a time series of the outcomes.

bool IModifyActiveCellNetwork::ToggleCumulativeEventStatistics()
{
    return ToggleCumulativeEventStatisticsHandler();
}

// Function to set the period with which a given event type will attempt
// to execute.The period must be 0 or greater, and applies to all events of
// the named type in the named ACN. A period of 0 sets the execution to every
// time step.

void IModifyActiveCellNetwork::SetEventExecutionPeriod(const zString eventType, long period)
{
    SetInternalEventExecutionPeriod(eventType, period);
}

// Function to return the execution period for a named event type.

long IModifyActiveCellNetwork::GetEventExecutionPeriodFromName(const zString eventType) const
{
    return GetInternalEventExecutionPeriodFromName(eventType);
}


