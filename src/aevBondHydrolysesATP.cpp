/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aevBondHydrolysesATP.cpp: implementation of the aevBondHydrolysesATP class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "aevBondHydrolysesATP.h"
#include "aevBondReleasesPi.h"
#include "aeActiveBond.h"
#include "aefActinBond.h"

#include "IActiveSimBox.h"
#include "IActiveBondHydrolysesATP.h"
#include "aeActiveCellNetwork.h"



//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString aevBondHydrolysesATP::m_Type = "BondHydrolysesATP";

const zString aevBondHydrolysesATP::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	aevActiveEvent* Create() {return new aevBondHydrolysesATP();}

	const zString id = aevBondHydrolysesATP::GetType();

	const bool bRegistered = acfActiveEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aevBondHydrolysesATP::aevBondHydrolysesATP() : m_pIEvent(0), m_pBond(0)
{

}

// The same aggregate object is passed to the copied event. 

aevBondHydrolysesATP::aevBondHydrolysesATP(const aevBondHydrolysesATP& oldEvent) : aevActiveEvent(oldEvent),
                                           m_pBond(oldEvent.m_pBond)
{
}

aevBondHydrolysesATP::~aevBondHydrolysesATP()
{
	// Destroy the contained active event interface instance if it has been created
	
	if(m_pIEvent)
	{
		delete m_pIEvent;
		m_pIEvent = 0;
	}
}

// Member functions to write/read the data specific to the command.
// The name of bead type to be removed from display is the only data needed.

zOutStream& aevBondHydrolysesATP::put(zOutStream& os) const
{
	// Write base class data first

	aevActiveEvent::put(os);
	
	return os;
}

zInStream& aevBondHydrolysesATP::get(zInStream& is)
{
	// Read base class data first

	aevActiveEvent::get(is);
	
	SetDataValid(true);

	return is;
}

// Non-static function to return the type of the event

const zString aevBondHydrolysesATP::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

aevActiveEvent* aevBondHydrolysesATP::GetEvent() const
{
	return new aevBondHydrolysesATP(*this);
}

// **********************************************************************
// Functions to set the parameters for the event

// Function to store the active polymer that is being monitored by this event.
// If the pointer passed in is null there is not much we can do.

void aevBondHydrolysesATP::SetBond(aeActiveBond *const pBond)
{
    // We hardwire the bond to fActin for now until we decide where
    // to store the member variables needed to handle bound ATP.

	if(dynamic_cast<aefActinBond*>(pBond))
	{
		m_pBond = dynamic_cast<aefActinBond*>(pBond);
	}
	else
	{
		m_pBond = 0;
	}
}


// **********************************************************************
// Function to validate the data when an event is created on the fly. It
// should be implemented by all event classes that may be created during a simulation.

bool aevBondHydrolysesATP::InternalValidateData()
{
	return true;
}

// **********************************************************************
// Check of the event's preconditions to see if it should proceed with its
// actions. This event currently has no preconditions.

bool aevBondHydrolysesATP::CheckPreconditions()
{
	SetDataValid(true);
	return true;
}

// **********************************************************************
// Implementation of the Execute command sent by the ActiveSimBox to aevActiveEvents.
//
// The event is active for its lifetime and indicates that it should be 
// destroyed by returning false from its Execute() function.
//

bool aevBondHydrolysesATP::Execute(IActiveSimBox* const pShadow)
{
	// If this is the first time the event is executed, store an interface
	// pointer derived from IActiveEventSource to allow it to access other
    // ACNs via the active SimBox.

	if(!m_pIEvent)
	{
		m_pIEvent = pShadow->GetIActiveBondHydrolysesATP();
	}

	// Only implement the event if none of its contra events are active,
    // if the execution period is not set or the timer is zero (indicating
    // that the event should execute), and if the bond's hydrolysis condition 
    // is satisfied. Note that this event is never marked as active as it
    // takes place instantaneously. The default state in the aevActiveEvent
    // base class is to be inactive. When the event has executed, it returns
    // false indicating that it should be destroyed.

    if(!IsAnyContraEventActive())
    {
	    if(DecrementTimer() && m_pBond->Hydrolyse())
	    {
            // Create next event in the chain and store it in the containing bond.
            // Note that we know the identity of the next event so we don't need
            // to get the default instance type from the ACN, we just create 
            // a new instance of the desired type, viz, aevBondReleasesPi.

            aevBondReleasesPi* pNextEvent = dynamic_cast<aevBondReleasesPi*>(AddEvent(aevBondReleasesPi::GetType()));

            pNextEvent->SetContainingACN(GetACN());
            pNextEvent->SetBond(m_pBond);
            pNextEvent->SetExecutionPeriod(GetACN()->GetInternalEventExecutionPeriodFromName(pNextEvent->GetEventType()));
		    pNextEvent->CheckPreconditions();

 //           std::cout << "ATP Hydrolysed " << m_pBond->GetId() << zEndl;

            return false; // Signals event should be destroyed
        }  
    }
    return true;
}

// Function to return the id of the wrapped bond.

long aevBondHydrolysesATP::GetBondId() const
{
    if(m_pBond)
    {
        return m_pBond->GetId();
    }
    else
    {
        return -1; // Flag an empty event
    }
}
