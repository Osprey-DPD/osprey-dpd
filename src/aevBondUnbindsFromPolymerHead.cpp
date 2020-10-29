/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aevBondUnbindsFromPolymerHead.cpp: implementation of the aevBondUnbindsFromPolymerHead class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "aevBondUnbindsFromPolymerHead.h"
#include "aeActiveBond.h"
#include "aeActivePolymer.h"
#include "Bond.h"

#include "IActiveSimBox.h"
#include "IActiveBondUnbindsFromPolymer.h"
#include "aeActiveCellNetwork.h"
#include "aefActinBond.h"



//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString aevBondUnbindsFromPolymerHead::m_Type = "BondUnbindsFromPolymerHead";

const zString aevBondUnbindsFromPolymerHead::GetType()
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
	aevActiveEvent* Create() {return new aevBondUnbindsFromPolymerHead();}

	const zString id = aevBondUnbindsFromPolymerHead::GetType();

	const bool bRegistered = acfActiveEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aevBondUnbindsFromPolymerHead::aevBondUnbindsFromPolymerHead() : m_Duration(0), 
									 m_SpringConstant(0.0),
									 m_Length(0.0),
									 m_pIEvent(0),
									 m_pPolymer(0), 
                                     m_pOldHeadBond(0),
									 m_pInternalBond(new CBond(-1,0.0,0.0)),
									 m_Counter(0)								 
{

}

// The same aggregate object is passed to the copied event. Note that we create
// a new CBond here as the old one will be destroyed when the oldEvent instance is
// destroyed.
//

aevBondUnbindsFromPolymerHead::aevBondUnbindsFromPolymerHead(const aevBondUnbindsFromPolymerHead& oldEvent) : aevActiveEvent(oldEvent),
																	m_Duration(oldEvent.m_Duration),
																	m_SpringConstant(oldEvent.m_SpringConstant),
																	m_Length(oldEvent.m_Length),                                                                   
																	m_pPolymer(oldEvent.m_pPolymer),
                                                                    m_pOldHeadBond(oldEvent.m_pOldHeadBond),
																	m_pInternalBond(new CBond(-1,0.0,0.0)),
																	m_Counter(oldEvent.m_Counter)
{
}

aevBondUnbindsFromPolymerHead::~aevBondUnbindsFromPolymerHead()
{
	// Destroy the contained active event interface instance if it has been created
	
	if(m_pIEvent)
	{
		delete m_pIEvent;
		m_pIEvent = 0;
	}

	// Delete the transient bond between monomers if it exists

	if(m_pInternalBond)
		delete m_pInternalBond;
}

// Member functions to write/read the data specific to the command.
// The name of bead type to be removed from display is the only data needed.

zOutStream& aevBondUnbindsFromPolymerHead::put(zOutStream& os) const
{
	// Write base class data first

	aevActiveEvent::put(os);
	
	return os;
}

zInStream& aevBondUnbindsFromPolymerHead::get(zInStream& is)
{
	// Read base class data first

	aevActiveEvent::get(is);
	
	SetDataValid(true);

	return is;
}

// Non-static function to return the type of the event

const zString aevBondUnbindsFromPolymerHead::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

aevActiveEvent* aevBondUnbindsFromPolymerHead::GetEvent() const
{
	return new aevBondUnbindsFromPolymerHead(*this);
}

// **********************************************************************
// Functions to set the parameters for the event

// Function to store the active polymer that is being monitored by this event.
// If the pointer passed in is null there is not much we can do.

void aevBondUnbindsFromPolymerHead::SetPolymer(aeActivePolymer *const pPolymer)
{
	if(pPolymer)
	{
		m_pPolymer = pPolymer;
	}
	else
	{
		m_pPolymer = 0;
	}
}

void aevBondUnbindsFromPolymerHead::SetDuration(long time)
{
	if(time > 0)
	{
		m_Duration = time;
	}
}

void aevBondUnbindsFromPolymerHead::SetSpringConstant(double spring)
{
	if(spring > 0.0)
	{
		m_SpringConstant = spring;
	}
}

void aevBondUnbindsFromPolymerHead::SetLength(double length)
{
	if(length > 0.0 && length < 0.5*GetACN()->GetSimBoxXLength() &&
		                length < 0.5*GetACN()->GetSimBoxYLength() &&
		                length < 0.5*GetACN()->GetSimBoxZLength())
	{
		m_Length = length;
	}
}


// **********************************************************************
// Function to validate the data when an event is created on the fly. It
// should be implemented by all event classes that may be created during a simulation.

bool aevBondUnbindsFromPolymerHead::InternalValidateData()
{
	return true;
}

// **********************************************************************
// Check of the event's preconditions to see if it should proceed with its
// actions. This event is automatically created when a new polymer is formed.

bool aevBondUnbindsFromPolymerHead::CheckPreconditions()
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
// This event wraps an active polymer instance and checks to see if its 
// head bond should detach. The conditions for the detachment are determined
// by the sub-class of the aeBondOffCondition specified in the specific
// active bond's constructor. The default is aeBondProximityOff that allows
// a bond to detach if thermal fluctuations take it beyond a fixed distance
// from its adjacent monomer.
// We attach a spring to the bond to be detached and place the other end
// of the spring at twice its unstretched length. Then the spring constant
// is linearly ramped up using a counter until the bond is moved beyond 
// the spring's unstretched length. At this point it is detached and the
// event completes. Finally, the released bond is added back into the
// free active bond pool.

bool aevBondUnbindsFromPolymerHead::Execute(IActiveSimBox* const pShadow)
{
	// If this is the first time the event is executed, store an interface
	// pointer derived from IActiveEventSource to allow it to access other
    // ACNs via the active SimBox.

	if(!m_pIEvent)
	{
		m_pIEvent = pShadow->GetIActiveBondUnbindsFromPolymer();
	}

	// Only implement the event if none of its contra events are active,
    // if the execution period is not set or the timer is zero (indicating
    // that the event should evecute), the contained polymer has more than 
    // the minimal number of bonds, and if the terminal bond's off condition 
    // is activated.

    if(!IsAnyContraEventActive())
    {
	    if(!IsActive() && DecrementTimer() && m_pPolymer->GetSize() > 2 && m_pPolymer->GetHeadBond()->Deactivate())
	    {
            SetActive();

            // Set up the initial conditions for the bond detaching

 		    m_Counter = GetDuration();
			m_pInternalBond->SetBeads(m_pPolymer->GetHeadBond()->GetTailAdjacentBond()->GetTailHeadBead(), m_pPolymer->GetHeadBond()->GetTailHeadBead());
		    m_pInternalBond->SetSpringConstant(GetSpringConstant());
		    m_pInternalBond->SetUnStretchedLength(2.0*GetLength());

            // Store the old terminal bond until it has completed its separation, but
            // remove it from the polymer so that polymers of length 3 do not initiate
            // unbding events at both ends. Indicate to all contra events that 
            // this event is now active. We ignore the return value from 
		    // RemoveHeadBond() as we have checked above that the polymer 
            // has more than its minimal number of bonds.

		    m_pOldHeadBond = m_pPolymer->GetHeadBond();
		    m_pPolymer->RemoveHeadBond();
        }  
        else if(IsActive())
        {
            const double dr = m_pInternalBond->CalculatePBCLength();  // Initial separation of the two monomers

            // Check to see if the bond has detached regardless of the
            // counter value; if it has not and the counter is positive,
            // ramp up the spring constant; otherwise, double the value
            // of the internal bond's spring constant and reset the counter
            // for another round. Once the bond has moved beyond the event's
            // internal range parameter, inactivate this event and reset its
            // its internal spring constant ready for the next activation.

		    if(dr > GetLength())
		    {
                SetInactive();
                IncrementSuccesses();  // Increment the event's success counter
                BroadcastState();      // Attempt to broadcast the state change

			    // Now that the bond has moved away from the polymer,
			    // add it back to the containing ACN's free bond container. 
			    AddFreeBond(m_pOldHeadBond);

                // Destroy the bond's ATP-ADP-Pi-ADP event if it exists
                // so that the monomers in the bulk are not wrapped in an event.
                // We search the ACN's events for one whose bond id is the 
                // same as the newly-released head bond.

                GetACN()->DeleteATPDependentEventWrappingBond(m_pOldHeadBond->GetId());

                // Set the newly-freed bond's ATP status back to true as we
                // don't implement events on free monomers yet. This is hard-wired 
                // for fActin bonds at the moment

                dynamic_cast<aefActinBond*>(m_pOldHeadBond)->Phosphorylate();

                // Toggle the equivalent number of active bonds in the bulk
                // to their phantom state to maintain the constant number of 
                // bulk free active bonds. Note that this must be called after 
                // the newly-released bond has been added to the ACN's free bond 
                // container.

                GetACN()->ConvertFreeBondsToPhantom();

                // Reset event data for the next invocation
			    m_Counter = GetDuration();
			    SetSpringConstant(dynamic_cast<aevBondUnbindsFromPolymerHead*>(GetACN()->GetInternalBondUnbindsFromPolymerHeadEvent())->GetSpringConstant());
		    }
		    else if(m_Counter > 0)
		    {
			    m_Counter--;
			    // On subsequent invocations, gradually increase the strength of the 
			    // enclosed transient bond until it reaches its pre-defined value, 
			    // so that the head monomer is moved away from the remaining polymer
			    // and can be released. We also reduce the strength of the filament's
			    // internal bonds holding the head onto the polymer to make it easier
			    // for the head to be moved away.

			    m_pInternalBond->SetSpringConstant(GetSpringConstant()*(1.0 - static_cast<double>(m_Counter)/static_cast<double>(GetDuration())));
			    m_pInternalBond->AddPBCForce();
	        }
		    else
		    {
			    // Bonds are not yet fully separated so double the event's
                // spring constant and start the counter again. We treat this
                // case as a "failure" in order to count the number of times
                // it happens, but a bond can never actually fail to separate
                // as the transient bond strength is increased until the monomer
                // is moved sufficiently far from the polymer as to detach.

                IncrementFailures();  // Increment the event's failure counter
			    SetSpringConstant(2.0*GetSpringConstant());
			    m_Counter = GetDuration();
		    }
	    }
    }

	// This event always returns true as it continually monitors
	// the polymer for detachment of its head. It is destroyed by the event
	// that monitors the polymer's dissolution.

	return true;

}

// VF that allows this event class to broadcast information about its state
// to its containing instance for analysis by event source/analysis decorators.
// Currently, this event just tells its containing ACN that it has successfully
// executed and a new bond has been attached to the active polymer.

void aevBondUnbindsFromPolymerHead::BroadcastState()
{
    if(GetBroadcastState())
    {
        GetACN()->BroadcastEventState(this);
    }
}

