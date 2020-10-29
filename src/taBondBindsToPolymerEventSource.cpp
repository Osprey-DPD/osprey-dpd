/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taBondBindsToPolymerEventSource.cpp: implementation of the taBondBindsToPolymerEventSource class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taBondBindsToPolymerEventSource.h"
#include "taEventAnalysisDecorator.h"

#if EnableShadowSimBox == SimACNEnabled
    #include "aeActiveBond.h"
    #include "aevActiveEvent.h"
    #include "aevBondBindsForwardConeToPolymerHead.h"
    #include "aevBondBindsForwardConeToPolymerTail.h"
#endif

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taBondBindsToPolymerEventSource::m_Type = "BondBindsToPolymerEventSource";

const zString taBondBindsToPolymerEventSource::GetType()
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class. Note that the outer decorator label for the wrapped object is
// set in the taEventSourceDecorator class' constructor.

taBondBindsToPolymerEventSource::taBondBindsToPolymerEventSource(const zString label, CCommandTargetNode* const pDec) : taEventSourceDecorator(label, pDec)

{

}

taBondBindsToPolymerEventSource::~taBondBindsToPolymerEventSource()
{

}

// Non-static member function to return the target's type.

const zString taBondBindsToPolymerEventSource::GetTargetType() const
{
    return m_Type;
}

// Function called by the SimBox to update the decorator state. Here we use it
// to check if it is time to normalize the data and write it to file.

void taBondBindsToPolymerEventSource::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);
}

// Function used by an ACN to pass data from an event for analysis.
// The Event Source Decorator provides a uniform interface for all Event Analysis Decorators
// to access data common to all event classes. 
//
// This class analyses events that monitor the binding of a free active bond to the 
// head or tail of a growing active polymer. Currently, only the forward cone binding
// events are included, new ones can be added as needed.
//

void taBondBindsToPolymerEventSource::BroadcastEventState(const aevActiveEvent* const pEvent)
{
#if EnableShadowSimBox == SimACNEnabled

    const zString type = pEvent->GetEventType();
    const long time    = pEvent->GetCurrentTime();

    // Store the event's data so that it is accessible by any analysis decorators 
    // that have registered with this ESD. This class monitors the time at which
    // the event successfully executes.

    SetSuccessTime(pEvent->GetCurrentTime());

    // Event-specific data: if the event is of the wrong type we do not pass on its 
    // data to attached EADs.

    std::cout << "ESD " << GetLabel() << " broadcasting the state for event type " << type << "  which fired at time " << time << zEndl;

    if(dynamic_cast<const aevBondBindsForwardConeToPolymerHead*>(pEvent))
    {
        const aevBondBindsForwardConeToPolymerHead* const pSpecificEvent = dynamic_cast<const aevBondBindsForwardConeToPolymerHead*>(pEvent);
 
        std::cout << "Polymer head event has range/length " << pSpecificEvent->GetRange() << "  " << pSpecificEvent->GetLength() << zEndl;
    
    SetBond(pSpecificEvent->GetHeadBond());
    }
    else if(dynamic_cast<const aevBondBindsForwardConeToPolymerTail*>(pEvent))
    {
        const aevBondBindsForwardConeToPolymerTail* const pSpecificEvent = dynamic_cast<const aevBondBindsForwardConeToPolymerTail*>(pEvent);

        std::cout << "Polymer tail event has range/length " << pSpecificEvent->GetRange() << "  " << pSpecificEvent->GetLength() << zEndl;

        SetBond(pSpecificEvent->GetTailBond());
    }
    else
    {
        SetBond(0);
    }

    if(m_pBond)
    {
        // Now broadcast the new data to any connected EADs.

        StringEventAnalysisIterator iterEAD = m_mEventAnalysisFromType.find(GetLabel());

        if(iterEAD != m_mEventAnalysisFromType.end())
        {
            while(iterEAD != m_mEventAnalysisFromType.upper_bound(GetLabel()))
            {
                iterEAD->second->AnalyseEventState();
                iterEAD++;
            }
        }
    }

#endif
}

// Function to store a pointer to the active bond at the head of an active polymer.
// This allows data relating to the growth of the polymer at its head to be
// available for analysis by EADs.

void taBondBindsToPolymerEventSource::SetBond(const aeActiveBond* const pBond)
{
    m_pBond = pBond;
}

// Functions to return the x, y, z coordinates of an active bond. We use its tail
// coordinates as these are guaranteed to exist even if the bond is not
// part of a filament.

double taBondBindsToPolymerEventSource::GetXCoordinate() const
{
#if EnableShadowSimBox == SimACNEnabled

    if(m_pBond)
    {
        return m_pBond->GetTailHeadBead()->GetXPos();
    }
    else
    {
        return 0.0;
    }
#else
    return 0.0;
#endif
}

double taBondBindsToPolymerEventSource::GetYCoordinate() const
{
#if EnableShadowSimBox == SimACNEnabled

    if(m_pBond)
    {
        return m_pBond->GetTailHeadBead()->GetYPos();
    }
    else
    {
        return 0.0;
    }
#else
    return 0.0;
#endif
}

double taBondBindsToPolymerEventSource::GetZCoordinate() const
{
#if EnableShadowSimBox == SimACNEnabled

    if(m_pBond)
    {
        return m_pBond->GetTailHeadBead()->GetZPos();
    }
    else
    {
        return 0.0;
    }
#else
    return 0.0;
#endif
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taBondBindsToPolymerEventSource::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taBondBindsToPolymerEventSource::Write(zOutStream& os) const
{
    
    return os;
}


