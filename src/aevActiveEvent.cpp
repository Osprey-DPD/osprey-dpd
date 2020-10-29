/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aevActiveEvent.cpp: implementation of the aevActiveEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aevActiveEvent.h"
#include "aeActiveCellNetwork.h"
#include "aeActiveSimBox.h"

// **********************************************************************
// Global Functions.
//
// Function to write out the event data to file. We pass the
// stream output operator to the contained aevActiveEvent-derived object using
// its put() function. This is because the << and >> operators cannot be
// treated polymporphically.
//

zOutStream& operator<<(zOutStream& os, const aevActiveEvent& event)
{
	return event.put(os);
}

// Function to read the event data from file. Because the name has
// already been read we only need to call the derived class' get() function 
// to fill in its specific data.

zInStream& operator>>(zInStream& is, aevActiveEvent& event)
{
	return event.get(is);
}

// Static member variable holding the number of events created.

long aevActiveEvent::m_EventTotal = 0;

long aevActiveEvent::GetEventTotal()
{
	return m_EventTotal;
}
// Static member function to reset the number of events to zero.

void aevActiveEvent::ZeroEventTotal()
{
	m_EventTotal = 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Active events are created with their validity flag set to false: the 
// derived classes' are responsible for checking their data and setting the
// flag to true to indicate that the event is valid.
// The event's execution period is set to 0 by default, so the event is
// checked every time step.

aevActiveEvent::aevActiveEvent() : m_id(++aevActiveEvent::m_EventTotal), 
					 m_bValid(false), m_bDependent(false), m_bIsActive(false), 
                     m_bBroadcastState(false), m_pACN(0),
                     m_SuccessCounter(0), m_FailureCounter(0), 
                     m_ExecutionPeriod(0), m_Timer(0)
{
	m_DependentEvents.clear();
	m_ContraEvents.clear();
}

// Constructor used to initialise the event's execution period to a chosen value.

aevActiveEvent::aevActiveEvent(long period) : m_id(++aevActiveEvent::m_EventTotal), 
					 m_bValid(false), m_bDependent(false), m_bIsActive(false), 
                     m_bBroadcastState(false), m_pACN(0),
                     m_SuccessCounter(0), m_FailureCounter(0), 
                     m_ExecutionPeriod(period), m_Timer(period)
{
	m_DependentEvents.clear();
	m_ContraEvents.clear();
}

// Copying an event does not change its id nor its contained dependent events.

aevActiveEvent::aevActiveEvent(const aevActiveEvent& oldEvent) : m_id(oldEvent.m_id), 
											m_bValid(oldEvent.m_bValid),
											m_bDependent(oldEvent.m_bDependent),
											m_bIsActive(oldEvent.m_bIsActive),
											m_bBroadcastState(oldEvent.m_bBroadcastState),
											m_pACN(oldEvent.m_pACN),
                                            m_SuccessCounter(oldEvent.m_SuccessCounter),
                                            m_FailureCounter(oldEvent.m_FailureCounter),
                                            m_ExecutionPeriod(oldEvent.m_ExecutionPeriod),
                                            m_Timer(oldEvent.m_Timer)
{
	// Copying of contained events not implemented yet.
}

aevActiveEvent::~aevActiveEvent()
{
	// We do not decrement the event id counter as it may not be the last event
	// that is being destroyed.

	// Destroy all events that depend on this one by calling the
	// containing ACN's DeleteEvent() function.

	if(!m_DependentEvents.empty())
	{
		for(ActiveEventListIterator iterEvent=m_DependentEvents.begin(); iterEvent!=m_DependentEvents.end(); iterEvent++)
		{
			GetACN()->DeleteEvent(*iterEvent);
		} 
		m_DependentEvents.clear();
	}

	// Contra events are not owned by this event, but are only used to obtain 
	// information about their state, hence we do not destroy them here.
}

// Active events are not persisted to file.

zOutStream& aevActiveEvent::put(zOutStream& os) const
{
	return os;
}

// Active events cannot be read in from file.

zInStream& aevActiveEvent::get(zInStream& is)
{
	return is;
}

// Function to validate the data when an event is created on the fly. It 
// should be implemented by all event classes that may be created during a simulation.

bool aevActiveEvent::InternalValidateData()
{
	return false;
}

// Protected function to allow events to store a pointer to their containing
// ACN. This is needed to allow each ACN to update its events using their 
// Execute() function.

void aevActiveEvent::SetContainingACN(aeActiveCellNetwork* const pACN)
{
	// Should we check for a NULL pointer? But what do we do it it is null?

	m_pACN = pACN;
}

// ****************************************
// Forwarding functions to the event's containing ACN

aeCNTCell* aevActiveEvent::GetCellFromBond(const aeActiveBond *const pBond) const
{
	if(GetACN() && pBond)
	{
		return GetACN()->GetCellFromBond(pBond);
	}
	else
		return 0;
}

void aevActiveEvent::AddFreeBond(aeActiveBond* const pBond)
{
	if(GetACN() && pBond)
	{
		GetACN()->AddFreeBond(pBond);
	}
}

void aevActiveEvent::RemoveFreeBond(aeActiveBond* const pBond)
{
	if(GetACN() && pBond)
	{
		GetACN()->RemoveFreeBond(pBond);
	}
}

ActiveBondListIterator aevActiveEvent::RemoveFreeBondByIterator(ActiveBondListIterator iterBond)
{
	if(GetACN() && *iterBond)
	{
		return GetACN()->RemoveFreeBondByIterator(iterBond);
	}
	else
    {
		return iterBond;
    }
}


void aevActiveEvent::AddFreePolymer(aeActivePolymer* pPolymer)
{
	if(GetACN() && pPolymer)
	{
		GetACN()->AddFreePolymer(pPolymer);
	}
}

void aevActiveEvent::RemoveFreePolymer(aeActivePolymer* const pPolymer)
{
	if(GetACN() && pPolymer)
	{
		GetACN()->RemoveFreePolymer(pPolymer);
	}
}

// Function to allow active events to create new events and pass them to their
// containing ACN.

aevActiveEvent*  aevActiveEvent::AddEvent(const zString type)
{
	if(GetACN())
	{
		return GetACN()->AddEvent(type);
	}
	else
	{
		return 0;
	}
}


// Function to store an event locally that depends on this one and should be 
// destroyed when this event is destroyed. It sets the  "dependent" flag so that
// it is not destroyed twice, once in this class and once by the ACN.

void aevActiveEvent::AddDependentEvent(aevActiveEvent* const pEvent)
{
	pEvent->SetDependent(true);

	m_DependentEvents.push_back(pEvent);
}

void aevActiveEvent::AddContraEvent(aevActiveEvent * const pEvent)
{
	m_ContraEvents.push_back(pEvent);
}

// **********************************************************************
// Functions to set a flag showing whether an event is currently executing or not.
// Each event class is responsible for defining when it is active.

void aevActiveEvent::SetActive()
{
	m_bIsActive = true;
}

void aevActiveEvent::SetInactive()
{
	m_bIsActive = false;
}

// Function that returns true if any of the contained contra events are active.
// If there are no contra events it returns false.

bool aevActiveEvent::IsAnyContraEventActive() const
{
	if(!m_ContraEvents.empty())
	{
		for(cActiveEventListIterator citerEvent=m_ContraEvents.begin(); citerEvent!=m_ContraEvents.end(); citerEvent++)
		{
			if((*citerEvent)->IsActive())
			{
				return true;
			}
		} 
	}

	return false;
}

// Function that returns true if all of the contained contra events are active.
// If there are no contra events, it returns false.

bool aevActiveEvent::AreAllContraEventsActive() const
{
	bool bActive = false;

	if(!m_ContraEvents.empty())
	{
		bActive = true;

		for(cActiveEventListIterator citerEvent=m_ContraEvents.begin(); citerEvent!=m_ContraEvents.end(); citerEvent++)
		{
			if(!(*citerEvent)->IsActive())
			{
				bActive = false;
			}
		} 

	}

	return bActive;
}

// ****************************************
// Functions to manipulate the event activation counters.

void aevActiveEvent::IncrementSuccesses()
{
    m_SuccessCounter++;
}

void aevActiveEvent::IncrementFailures()
{
    m_FailureCounter++;
}

long aevActiveEvent::GetSuccessCounter() const
{
    return m_SuccessCounter;
}

long aevActiveEvent::GetFailureCounter() const
{
    return m_FailureCounter;
}

void aevActiveEvent::ResetAllCounters()
{
    ResetSuccessCounter();
    ResetFailureCounter();
}

void aevActiveEvent::ResetSuccessCounter()
{
    m_SuccessCounter = 0;
}

void aevActiveEvent::ResetFailureCounter()
{
    m_FailureCounter = 0;
}

// ****************************************
// Set of functions relating to the timer that controls how frequently
// an event is allowed to checked its execution conditions.
// 
// Function to set the period between checking the event's Execute conditions and
// to reset the timer to the new value. This is typically used by events
// when creating new events internally to ensure that they start life with
// a valid timer.
// 
// To avoid comparing random numbers that are smaller than 10**-06 or so on every
// time step, we check the event's conditions only one per m_ExecutionTime steps and 
// increase the probability of the event's occurring accordingly.
// Note that a period of 0 will cause the event to be always checked.

void aevActiveEvent::SetExecutionPeriod(long time)
{
    ResetExecutionPeriod(time);
    ResetTimer();
}

// Function to set the event's execution period to a new value but not
// modify the current value of the timer. This is used by commands that
// change an event's execution period during a run. Note that if the new
// value of the period is less than the current value of the timer, the
// event will take longer than expected to execute again after the command
// has taken effect.

void aevActiveEvent::ResetExecutionPeriod(long time)
{
    if(time >= 0)
    {
        m_ExecutionPeriod = time;
    }
}

// Function to reset the event's execution timer to m_ExecutionPeriod. 

void aevActiveEvent::ResetTimer()
{
    m_Timer = m_ExecutionPeriod;
}

// Function that returns true if the event's timer is zero indicating that
// the event's Execution function can be checked.

bool aevActiveEvent::IsTimerZero() const
{
    if(m_Timer == 0)
    {
        return true;
    }
    else
        return false;
}

// Function to reduce the event's execution timer. If the execution period
// is zero, we return true so that the event is always checked; otherwise
// we decrement the timer and see if it is then zero and return true if so;
// if both of these fail, we return false.

bool aevActiveEvent::DecrementTimer()
{
    if(m_ExecutionPeriod == 0)
    {
        return true;
    }
    else if(--m_Timer == 0)
    {
        m_Timer = m_ExecutionPeriod;
        return true;
    }
    else
    {
        return false;
    }

}

// VF that is over-ridden by any derived class that wants to broadcast its state
// to its containing instance for analysis by event source/analysis decorators.
// The base class implementation does nothing so that all events can be treated
// uniformly regardless of whether they implement this feature or not.

void aevActiveEvent::BroadcastState()
{
}

// Function to return the current time from the event. This allows event source 
// decorators to get the time at which an event succeeds for analysis.

long aevActiveEvent::GetCurrentTime() const
{
    return aeActiveSimBox::GetIActiveSimBox()->GetCurrentTime();
}
