/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctSerialisePolymerHeadCoordinatesImpl.cpp: implementation of the ctSerialisePolymerHeadCoordinatesImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctSerialisePolymerHeadCoordinatesImpl.h"
#include "acSerialisePolymerHeadCoordinates.h"
#include "taSerialiseBondCoordinatesEventAnalysis.h"
#include "taEventSourceDecorator.h"
#include "SimBox.h"
#include "LogacACNCreateEventAnalysis.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctSerialisePolymerHeadCoordinatesImpl::ctSerialisePolymerHeadCoordinatesImpl()
{
}

ctSerialisePolymerHeadCoordinatesImpl::~ctSerialisePolymerHeadCoordinatesImpl()
{

}

// Command handler function to wrap an Event Source Decorator (ESD) target with 
// an Event Analysis Decorator (EAD) so that events controlled by the ACN and 
// watched by the ESD can have their data sent to all registered EADs for analysis.
// Note that information flows in the following direction:
//
// Event --> ACN --> Event Source Decorator --> Event Analysis Decorator
//
// so the decorators do not need to pass information back to the ACN, and are
// prevented from doing so.  There is a 1-1 relationship between event types and
// ESDs, although many event types may be managed by a single ACN, and each one
// is mapped to a unique ESD. There is a 1-many relationship between each ESD and
// the EADs, as there may be many forms of analysis that are to be performed on the
// data coming from one event type.

// The logic for linking an EAD to an existing ESD is as follows. The user supplies 
// the unique id of an existing ESD, an id for the newly-created EAD, an
// alphanumeric key and any further arguments required for the EAD's analysis.
// Currently the key will default to the ESD's id, but in future it will allow
// an ESD to select which of several possible interfaces exposed by an EAD it calls.
// The default analysis is to pass the simulation time at which an event succeeds.

void ctSerialisePolymerHeadCoordinatesImpl::SerialisePolymerHeadCoordinates(const xxCommand* const pCommand)
{
	const acSerialisePolymerHeadCoordinates* const pCmd = dynamic_cast<const acSerialisePolymerHeadCoordinates*>(pCommand);

	const zString sourceLabel = pCmd->GetEventSourceId();
	const zString decLabel    = pCmd->GetEventAnalysisId();
	const zString key         = pCmd->GetKey();
        const long    period      = pCmd->GetPeriod();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the ESD command target from the target list: note that we don't know
    // the name of the target that wraps the ACN, only the ESD decorator so we have
    // to use the SimBox's GetCommandTargetActivity() function. As we do know its
    // type, we can cast it to the appropriate decorator type. Once we have the
    // decorator instance, we can obtain the target it wraps from the function
    // CCommandTargetNode::GetActiveCommandTarget().

	CCommandTargetNode* pSourceDec = pSimBox->GetCommandTargetActivity(sourceLabel);
    taEventSourceDecorator* const pESD = dynamic_cast<taEventSourceDecorator*>(pSourceDec);
    CCommandTargetNode* pCmdTarget = pSourceDec->GetActiveCommandTarget();

	// Check that the ESD has the correct type, i.e., that the cast above worked, 
    // and create a taBinEventSuccessIntervalsEventAnalysis instance and wrap the 
    // target with it, and also store a pointer to the decorator in the ACN so that it 
    // can forward event broadcast information to the event source.

	if(pCmdTarget && pESD)
	{
		// If the target is already active, remove it before adding it back
		// using the new decorator: we just check if the label of the
		// outermost decorator instance differs from the target's label.
		// Note that because we now know that pCmdTarget exists, we don't 
		// have to check that pSourceTarget is non-NULL as it must either be
		// a pointer to a decorator instance or equal to pCmdTarget.

		CCommandTargetNode* pActiveTarget = pCmdTarget->GetOutermostDecorator();

        if(pActiveTarget->GetLabel() != pCmdTarget->GetLabel())
		{
			pSimBox->m_ActiveCommandTargets.remove(pActiveTarget);
		}
     
        // Wrap the active target in a new event analysis instance
		// and add it to the active command targets container. We use a different 
        // constructor if the user has specified the number of bins or their width.
        // Both of these parameters must not be non-zero.

		taSerialiseBondCoordinatesEventAnalysis* const pDec = new taSerialiseBondCoordinatesEventAnalysis(decLabel, pActiveTarget, period);

		pSimBox->m_ActiveCommandTargets.push_back(pDec);

        // Pass the newly-created EAD to the ESD so that it can receive event
        // data as it is broadcasted.

        pESD->AddEventAnalysis(sourceLabel, pDec);

		// Log sucessful execution of the command

//		new CLogacACNCreateEventAnalysis(pSimBox->GetCurrentTime(), sourceLabel, decLabel, binTotal, binWidth, sampleTotal);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
