/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aevPolymerDissolves.cpp: implementation of the aevPolymerDissolves class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "aevPolymerDissolves.h"
#include "aeActiveBond.h"
#include "aeActivePolymer.h"
#include "Bond.h"

#include "IActiveSimBox.h"
#include "IActivePolymerDissolves.h"
#include "aeActiveCellNetwork.h"
#include "aefActinBond.h"
#include "LogActiveEventError.h"



//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString aevPolymerDissolves::m_Type = "PolymerDissolves";

const zString aevPolymerDissolves::GetType()
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
	aevActiveEvent* Create() {return new aevPolymerDissolves();}

	const zString id = aevPolymerDissolves::GetType();

	const bool bRegistered = acfActiveEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aevPolymerDissolves::aevPolymerDissolves() : m_MinLengthTime(0),
									 m_Duration(0), 
									 m_SpringConstant(0.0),
									 m_Length(0.0),
									 m_pIEvent(0),
									 m_pPolymer(0),
									 m_pInternalBond(new CBond(-1,0.0,0.0)),
									 m_Counter(0)

								 
{

}

// The same aggregate object is passed to the copied event. Note that we create
// a new CBond here as the old one will be destroyed when the oldEvent instance is
// destroyed.
//

aevPolymerDissolves::aevPolymerDissolves(const aevPolymerDissolves& oldEvent) : aevActiveEvent(oldEvent),
																	m_MinLengthTime(oldEvent.m_MinLengthTime),
																	m_Duration(oldEvent.m_Duration),
																	m_SpringConstant(oldEvent.m_SpringConstant),
																	m_Length(oldEvent.m_Length),                                                                   
																	m_pPolymer(oldEvent.m_pPolymer),
																	m_pInternalBond(new CBond(-1,0.0,0.0)),
																	m_Counter(oldEvent.m_Counter)
{
}

// Dependent events are destroyed in the aevActiveEvent base class.

aevPolymerDissolves::~aevPolymerDissolves()
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

zOutStream& aevPolymerDissolves::put(zOutStream& os) const
{
	// Write base class data first

	aevActiveEvent::put(os);
	
	return os;
}

zInStream& aevPolymerDissolves::get(zInStream& is)
{
	// Read base class data first

	aevActiveEvent::get(is);
	
	SetDataValid(true);

	return is;
}

// Non-static function to return the type of the event

const zString aevPolymerDissolves::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

aevActiveEvent* aevPolymerDissolves::GetEvent() const
{
	return new aevPolymerDissolves(*this);
}

// **********************************************************************
// Functions to set the parameters for the event

// Function to store the active polymer that is being monitored by this event.
// If the pointer passed in is null there is not much we can do.

void aevPolymerDissolves::SetPolymer(aeActivePolymer *const pPolymer)
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

void aevPolymerDissolves::SetDuration(long time)
{
	if(time > 0)
	{
		m_Duration = time;
	}
}

void aevPolymerDissolves::SetSpringConstant(double spring)
{
	if(spring > 0.0)
	{
		m_SpringConstant = spring;
	}
}

void aevPolymerDissolves::SetLength(double length)
{
	if(length > 0.0)
	{
		m_Length = length;
	}
}


// **********************************************************************
// Function to validate the data when an event is created on the fly. It 
// should be implemented by all event classes that may be created during a simulation.

bool aevPolymerDissolves::InternalValidateData()
{
	return true;
}

// **********************************************************************
// Check of the event's preconditions to see if it should proceed with its
// actions. This event is automatically created when a new polymer is formed.

bool aevPolymerDissolves::CheckPreconditions()
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
// This event wraps a newly-created active polymer instance and checks
// it to see if should be dissolved. The criteria for dissolution are:
//
//  Polymer has the minimal number of bonds, here 2, for greater than a 
//  user-specified time m_MinLengthTime.
//
// Once this happens, the event starts a counter for a user-specified time,
// m_Duration, and at the end of this period the polymer is destroyed
// and its two active bond instances are returned to the containing ACN's
// free bond container.


bool aevPolymerDissolves::Execute(IActiveSimBox* const pShadow)
{
	// If this is the first time the event is executed, store an interface
	// pointer to the active SimBox and containing ACN and initialise the bond
	// formation counter and transient bond.

	if(!m_pIEvent)
	{
		m_pIEvent = pShadow->GetIActivePolymerDissolves();
		m_Counter = GetDuration();
		m_pInternalBond->SetBeads(m_pPolymer->GetTailBond()->GetTailHeadBead(), m_pPolymer->GetHeadBond()->GetTailHeadBead());
		m_pInternalBond->SetUnStretchedLength(GetLength());
	}

	// Only implement the event if the contained polymer has the minimal number
	// of bonds for the required time: if new bonds add to the polymer while
	// this event is active, it is aborted and its parameters reset to their
	// default values.

	if(m_pPolymer->GetSize() > 2)
	{
		m_Counter = GetDuration();
		SetSpringConstant(dynamic_cast<aevPolymerDissolves*>(GetACN()->GetInternalPolymerDissolvesEvent())->GetSpringConstant());

		return true;
	}
	else
	{
		// Get the separation of the two monomers

		const double dr = m_pInternalBond->CalculatePBCLength();

		if(m_Counter > 0)
		{
			// On subsequent invocations, gradually increase the strength of the 
			// enclosed transient bond until it reaches its pre-defined value, 
			// so that the monomers are gradually positioned at the user-defined 
			// separation for forming an active polymer.

			m_pInternalBond->SetSpringConstant(GetSpringConstant()/static_cast<double>(m_Counter));
			m_pInternalBond->AddPBCForce();
			m_Counter--;

			return true;
		}
		else if(dr < GetLength())
		{
			// Bonds are not yet fully separated, so strengthen the spring 
			// and reset the counter

			SetSpringConstant(2.0*GetSpringConstant());
			m_Counter = GetDuration();

			return true;
		}
		else
		{
			// Now that the bonds are in the right configuration, remove the bonds
			// from the polymer, tell the containing ACN to remove it from its 
			// free polymer container, add the released bonds back into the ACN's
			// free bond container and signal that it should destroy this event 
			// by returning false. 

			// The dependent events that control the polymer's growth at 
			// its head and tail end are destroyed in the base class.

			// This event destroys its contained active polymer but it must first 
			// remove the polymer's bonds so that they can be added back to the
			// containing ACN's free bond container and are not destroyed when the 
			// polymer's destructor is called. There should never be more than two
			// bonds at this point unless the dissolves event is modified to allow
			// it. This may also require changes to the aeActive Polymer::RemoveHeadBond()
			// function so that it correctly handles the disintegration of a polymer
			// that contains more than two bonds.

			aeActiveBond* pOldHead = m_pPolymer->GetHeadBond();
			aeActiveBond* pOldTail = m_pPolymer->GetTailBond();

			if(m_pPolymer->RemoveHeadBond())
			{
				new CLogActiveEventError(pShadow->GetCurrentTime(), "PolymerDissolves event error");
			}
			else
			{
				RemoveFreePolymer(m_pPolymer);
				AddFreeBond(pOldHead);
				AddFreeBond(pOldTail);

                // Destroy the two bonds' ATP-ADP-Pi-ADP events if they exist
                // so that the monomers in the bulk are not wrapped in an event.
                // We search the ACN's events for one whose bond id is the 
                // same as the newly-released head bond.

                GetACN()->DeleteATPDependentEventWrappingBond(pOldHead->GetId());
                GetACN()->DeleteATPDependentEventWrappingBond(pOldTail->GetId());


                // Set the newly-freed bond's ATP status back to true as we
                // don't implement events on free monomers yet. This is hard-wired 
                // for fActin bonds at the moment

                dynamic_cast<aefActinBond*>(pOldHead)->Phosphorylate();
                dynamic_cast<aefActinBond*>(pOldTail)->Phosphorylate();

                
                delete m_pPolymer;
				m_pPolymer = 0;
			}

			return false;
		}
	}
}

