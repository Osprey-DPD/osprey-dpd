/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aevPolymerForms.cpp: implementation of the aevPolymerForms class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "aevPolymerForms.h"
#include "aeActiveBond.h"
#include "aeActivePolymer.h"
#include "Bond.h"

#include "IActiveSimBox.h"
#include "IActivePolymerForms.h"
#include "aeCNTCell.h"
#include "aeActiveCellNetwork.h"

// Other events created by this event

#include "aevPolymerDissolves.h"
#include "aevBondBindsABSequentiallyToPolymerHead.h"
#include "aevBondBindsABSequentiallyToPolymerTail.h"
#include "aevBondBindsForwardConeToPolymerHead.h"
#include "aevBondBindsForwardConeToPolymerTail.h"
#include "aevBondUnbindsFromPolymerHead.h"
#include "aevBondUnbindsFromPolymerTail.h"
#include "aevBondHydrolysesATP.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString aevPolymerForms::m_Type = "PolymerForms";

const zString aevPolymerForms::GetType()
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
	aevActiveEvent* Create() {return new aevPolymerForms();}

	const zString id = aevPolymerForms::GetType();

	const bool bRegistered = acfActiveEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aevPolymerForms::aevPolymerForms() : m_Duration(0), m_Range(0.0),
									 m_SpringConstant(0.0),
									 m_Length(0.0),
									 m_pIEvent(0), 
									 m_pTail(0), m_pHead(0), m_pPolymer(0),
									 m_pInternalBond(new CBond(-1,0.0,0.0)),
									 m_Counter(0)

								 
{

}

// The same aggregate object is passed to the copied event. Note that we create
// a new CBond here as the old one will be destroyed when the oldEvent instance is
// destroyed.
//

aevPolymerForms::aevPolymerForms(const aevPolymerForms& oldEvent) : aevActiveEvent(oldEvent),
																	m_Duration(oldEvent.m_Duration),
																	m_Range(oldEvent.m_Range),
																	m_SpringConstant(oldEvent.m_SpringConstant),
																	m_Length(oldEvent.m_Length),                                                                   
																	m_iterTail(oldEvent.m_iterTail),
																	m_pTail(oldEvent.m_pTail),
																	m_pHead(oldEvent.m_pHead),
																	m_pPolymer(oldEvent.m_pPolymer),
																	m_pInternalBond(new CBond(-1,0.0,0.0)),
																	m_Counter(oldEvent.m_Counter)
{
}

aevPolymerForms::~aevPolymerForms()
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

	// This active event does not destroy its contained monomer/polymer instances
	// as they are passed over to their owning instance once its actions have been 
	// carried out. The active polymers created by this class of event are owned 
	// by the calling ACN.

	// Only decrement the event counter if this event was valid

	if(IsValid())
	{
		GetACN()->DecrementPolymerFormsEventTotal();
	}
}

// Member functions to write/read the data specific to the command.
// The name of bead type to be removed from display is the only data needed.

zOutStream& aevPolymerForms::put(zOutStream& os) const
{
	// Write base class data first

	aevActiveEvent::put(os);
	
	return os;
}

zInStream& aevPolymerForms::get(zInStream& is)
{
	// Read base class data first

	aevActiveEvent::get(is);
	
	SetDataValid(true);

	return is;
}

// Non-static function to return the type of the event

const zString aevPolymerForms::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

aevActiveEvent* aevPolymerForms::GetEvent() const
{
	return new aevPolymerForms(*this);
}

// **********************************************************************
// Functions to set the parameters for the event

void aevPolymerForms::SetDuration(long time)
{
	if(time > 0)
	{
		m_Duration = time;
	}
}

void aevPolymerForms::SetRange(double range)
{
	if(range > 0.0 && range <= 0.5*GetACN()->GetSimBoxXLength() &&
		              range <= 0.5*GetACN()->GetSimBoxYLength() &&
		              range <= 0.5*GetACN()->GetSimBoxZLength())
	{
		m_Range = range;
	}
}

void aevPolymerForms::SetSpringConstant(double spring)
{
	if(spring > 0.0)
	{
		m_SpringConstant = spring;
	}
}

void aevPolymerForms::SetLength(double length)
{
	if(length > 0.0)
	{
		m_Length = length;
	}
}

// **********************************************************************
// Function to validate the data when an event is created on the fly. It 
// should be implemented by all event classes that may be created during a simulation.

bool aevPolymerForms::InternalValidateData()
{
	return true;
}

// **********************************************************************
// Check of the event's preconditions to see if it should proceed with its
// actions.

bool aevPolymerForms::CheckPreconditions()
{
	// Find another bond to form a polymer

	aeCNTCell* const pCell = GetCellFromBond(*m_iterTail);

	aeActiveBond* pTargetBond = pCell->UpdateNucleation(*m_iterTail);

	// If the two bonds are within a given distance, we allow them to form a bond.
	// We use the PBC coordinates for beads and shift them if they are
	// more than half a box length apart because the unPBC coordinates can be
	// arbitrarily far apart unless beads have always been connected by a bond.
	//
	// We connect the two bonds using the aeActiveBond::AddHeadAdjacentBond() so that
	// their polymerised flags are set, thereby preventing them being chosen by later 
	// events. We know that if the event's preconditions are satisfied, the bonds
	// cannot break apart before they have formed a polymer, so there is no
	// danger in tying them together now rather than when the event's Execute() function
	// finally creates the active polymer instance.

	if(*m_iterTail && pTargetBond  && !(*m_iterTail)->IsPhantom() && !pTargetBond->IsPhantom())
	{
		m_pTail = *m_iterTail;
		m_pHead = pTargetBond;

		double dx = m_pTail->GetTailHeadBead()->GetXPos() - m_pHead->GetTailHeadBead()->GetXPos();
		double dy = m_pTail->GetTailHeadBead()->GetYPos() - m_pHead->GetTailHeadBead()->GetYPos();
		double dz = m_pTail->GetTailHeadBead()->GetZPos() - m_pHead->GetTailHeadBead()->GetZPos();

		if( dx > GetACN()->GetIActiveSimBox()->GetHalfSimBoxXLength() )
			dx = dx - GetACN()->GetIActiveSimBox()->GetSimBoxXLength();
		else if( dx < -GetACN()->GetIActiveSimBox()->GetHalfSimBoxXLength() )
			dx = dx + GetACN()->GetIActiveSimBox()->GetSimBoxXLength();

		if( dy > GetACN()->GetIActiveSimBox()->GetHalfSimBoxYLength() )
			dy = dy - GetACN()->GetIActiveSimBox()->GetSimBoxYLength();
		else if( dy < -GetACN()->GetIActiveSimBox()->GetHalfSimBoxYLength() )
			dy = dy + GetACN()->GetIActiveSimBox()->GetSimBoxYLength();

#if SimDimension == 3
		if( dz > GetACN()->GetIActiveSimBox()->GetHalfSimBoxZLength() )
			dz = dz - GetACN()->GetIActiveSimBox()->GetSimBoxZLength();
		else if( dz < -GetACN()->GetIActiveSimBox()->GetHalfSimBoxZLength() )
			dz = dz + GetACN()->GetIActiveSimBox()->GetSimBoxZLength();
#else
		dz = 0.0;
#endif

		const double dr = sqrt(dx*dx + dy*dy + dz*dz);

		if(dr < GetRange())
		{
			m_pTail->AddHeadAdjacentBond(m_pHead);

			RemoveFreeBond(m_pHead);

			SetDataValid(true);

			return true;
		}
	}

	// If the preconditions failed, return false so that the event can be re-used

	SetDataValid(false);

	return false;
}

// **********************************************************************
// Implementation of the Execute command sent by the ActiveSimBox to aevActiveEvents.
//
// The event is active for its lifetime and indicates that it should be 
// destroyed by returning false from its Execute() function.
//
// This event binds two monomers together with a transient bond and gradually 
// increases its spring constant (m_SpringConstant) until their separation 
// is close to that specified for the bond's unstretched length (m_Length). 
// If this is not achieved in the time specified (m_Duration), the final value
// of the spring constant is doubled and the counter starts again. This sequence 
// continues until the monomers satisfy the separation constraint.

bool aevPolymerForms::Execute(IActiveSimBox* const pShadow)
{
	// If this is the first time the event is executed, store an interface
	// pointer to the active SimBox and containing ACN and initialise the bond
	// formation counter and transient bond.

	if(!m_pIEvent)
	{
		m_pIEvent = pShadow->GetIActivePolymerForms();
		m_Counter = GetDuration();
		m_pInternalBond->SetBeads(m_pTail->GetTailHeadBead(), m_pHead->GetTailHeadBead());
		m_pInternalBond->SetUnStretchedLength(GetLength());
	}

	// Get the initial separation of the two monomers

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
	else if(dr > GetLength())
	{
		// Bonds are not yet in place so separate the two bonds and return them
		// to the free monomer container. Note that the DissolveFromHead() function
		// must be called on the head bond of the pair, and it handles the
		// calls to set the bond's polymerise flags to false.
		// We return false so that this event is destroyed.

		m_pHead->DissolveFromHead();
		AddFreeBond(m_pHead);
		AddFreeBond(m_pTail);

		return false;
	}
	else
	{
		// Now that the bonds are in the right configuration, create a new 
		// active polymer, and signal the containing ACN to destroy this event
		// by returning false. Note the order of the arguments: new polymer's 
		// head bond then its tail bond. Finally, add the new polymer to 
		// the ACN's free polymer container.
		//
		// Also create events to control the growth and dissolution of the polymer
		// and store them in the containing ACN's event container. Note that the
		// containing ACN is responsible for actually instantiating and storing
		// the events. Here, we just copy the ACN's default events and use their
		// default parameters. These may be modified by commands sent to the ACN.

		aeActivePolymer* pPolymer = new aeActivePolymer(m_pHead, m_pTail);
		AddFreePolymer(pPolymer);
			
//		std::cout << "forming polymer with id #  " << pPolymer->GetId() << " at range " << dr << zEndl;

		// ****************************************
		// Events for managing attachment/detachment of a bond at the 
		// polymer's head end

		const aevBondBindsForwardConeToPolymerHead* const pDefaultBondBindsToHead = dynamic_cast<aevBondBindsForwardConeToPolymerHead*>(GetACN()->GetInternalBondBindsToPolymerHeadEvent());

		aevBondBindsForwardConeToPolymerHead* pBondBindsToHead = dynamic_cast<aevBondBindsForwardConeToPolymerHead*>(AddEvent(pDefaultBondBindsToHead->GetEventType()));
		pBondBindsToHead->SetContainingACN(GetACN());
		pBondBindsToHead->SetPolymer(pPolymer);
		pBondBindsToHead->SetDuration(pDefaultBondBindsToHead->GetDuration());
		pBondBindsToHead->SetRange(pDefaultBondBindsToHead->GetRange());
		pBondBindsToHead->SetConeAngle(pDefaultBondBindsToHead->GetConeAngle());
		pBondBindsToHead->SetSpringConstant(pDefaultBondBindsToHead->GetSpringConstant());
		pBondBindsToHead->SetLength(pDefaultBondBindsToHead->GetLength());
        pBondBindsToHead->SetExecutionPeriod(GetACN()->GetInternalEventExecutionPeriodFromName(pBondBindsToHead->GetEventType()));
		pBondBindsToHead->CheckPreconditions();

		const aevBondUnbindsFromPolymerHead* const pDefaultBondUnbindsFromHead = dynamic_cast<aevBondUnbindsFromPolymerHead*>(GetACN()->GetInternalBondUnbindsFromPolymerHeadEvent());

		aevBondUnbindsFromPolymerHead* pBondUnbindsFromHead = dynamic_cast<aevBondUnbindsFromPolymerHead*>(AddEvent(pDefaultBondUnbindsFromHead->GetEventType()));
		pBondUnbindsFromHead->SetContainingACN(GetACN());
		pBondUnbindsFromHead->SetPolymer(pPolymer);
		pBondUnbindsFromHead->SetDuration(pDefaultBondUnbindsFromHead->GetDuration());
		pBondUnbindsFromHead->SetSpringConstant(pDefaultBondUnbindsFromHead->GetSpringConstant());
		pBondUnbindsFromHead->SetLength(pDefaultBondUnbindsFromHead->GetLength());
        pBondUnbindsFromHead->SetExecutionPeriod(GetACN()->GetInternalEventExecutionPeriodFromName(pBondUnbindsFromHead->GetEventType()));
		pBondUnbindsFromHead->CheckPreconditions(); 
        
        // Link the on and off events to each other so that they can check
        // the status of their contra event when they execute

		pBondBindsToHead->AddContraEvent(pBondUnbindsFromHead);
		pBondUnbindsFromHead->AddContraEvent(pBondBindsToHead);

		// ****************************************
		// Events for managing attachment/detachment of a bond at the 
		// polymer's tail end

		const aevBondBindsForwardConeToPolymerTail* const pDefaultBondBindsToTail = dynamic_cast<aevBondBindsForwardConeToPolymerTail*>(GetACN()->GetInternalBondBindsToPolymerTailEvent());

		aevBondBindsForwardConeToPolymerTail* pBondBindsToTail = dynamic_cast<aevBondBindsForwardConeToPolymerTail*>(AddEvent(pDefaultBondBindsToTail->GetEventType()));
		pBondBindsToTail->SetContainingACN(GetACN());
		pBondBindsToTail->SetPolymer(pPolymer);
		pBondBindsToTail->SetDuration(pDefaultBondBindsToTail->GetDuration());
		pBondBindsToTail->SetRange(pDefaultBondBindsToTail->GetRange());
		pBondBindsToTail->SetConeAngle(pDefaultBondBindsToTail->GetConeAngle());
		pBondBindsToTail->SetSpringConstant(pDefaultBondBindsToTail->GetSpringConstant());
		pBondBindsToTail->SetLength(pDefaultBondBindsToTail->GetLength());
        pBondBindsToTail->SetExecutionPeriod(GetACN()->GetInternalEventExecutionPeriodFromName(pBondBindsToTail->GetEventType()));
		pBondBindsToTail->CheckPreconditions();

		const aevBondUnbindsFromPolymerTail* const pDefaultBondUnbindsFromTail = dynamic_cast<aevBondUnbindsFromPolymerTail*>(GetACN()->GetInternalBondUnbindsFromPolymerTailEvent());

		aevBondUnbindsFromPolymerTail* pBondUnbindsFromTail = dynamic_cast<aevBondUnbindsFromPolymerTail*>(AddEvent(pDefaultBondUnbindsFromTail->GetEventType()));
		pBondUnbindsFromTail->SetContainingACN(GetACN());
		pBondUnbindsFromTail->SetPolymer(pPolymer);
		pBondUnbindsFromTail->SetDuration(pDefaultBondUnbindsFromTail->GetDuration());
		pBondUnbindsFromTail->SetSpringConstant(pDefaultBondUnbindsFromTail->GetSpringConstant());
		pBondUnbindsFromTail->SetLength(pDefaultBondUnbindsFromTail->GetLength());
        pBondUnbindsFromTail->SetExecutionPeriod(GetACN()->GetInternalEventExecutionPeriodFromName(pBondUnbindsFromTail->GetEventType()));
		pBondUnbindsFromTail->CheckPreconditions();

        // Link the on and off events to each other so that they can check
        // the status of their contra event when they execute

		pBondBindsToTail->AddContraEvent(pBondUnbindsFromTail);
		pBondUnbindsFromTail->AddContraEvent(pBondBindsToTail);

        // ****************************************
		// Events for managing the ATP state of the monomers in the new filament.

        // Create new events to represent the ATP hydrolysis of the
        // two monomers, and store them in the containing ACN. Note that 
        // we assume that the free monomers are NOT associated with such 
        // an event. We set the execution period in case the user has modified
        // it by command.

        aevBondHydrolysesATP* pNextEvent = dynamic_cast<aevBondHydrolysesATP*>(AddEvent(aevBondHydrolysesATP::GetType()));

        pNextEvent->SetContainingACN(GetACN());
        pNextEvent->SetBond(m_pHead);
        pNextEvent->SetExecutionPeriod(GetACN()->GetInternalEventExecutionPeriodFromName(pNextEvent->GetEventType()));
		pNextEvent->CheckPreconditions();


        pNextEvent = dynamic_cast<aevBondHydrolysesATP*>(AddEvent(aevBondHydrolysesATP::GetType()));

        pNextEvent->SetContainingACN(GetACN());
        pNextEvent->SetBond(m_pTail);
        pNextEvent->SetExecutionPeriod(GetACN()->GetInternalEventExecutionPeriodFromName(pNextEvent->GetEventType()));
		pNextEvent->CheckPreconditions();

		// ****************************************
		// Event for managing the polymer's dissolution
/*
		const aevPolymerDissolves* const pDefaultPolymerDissolves = dynamic_cast<aevPolymerDissolves*>(GetACN()->GetInternalPolymerDissolvesEvent());

		aevPolymerDissolves* pPolymerDissolves = dynamic_cast<aevPolymerDissolves*>(AddEvent(pDefaultPolymerDissolves->GetEventType()));
		pPolymerDissolves->SetContainingACN(GetACN());
		pPolymerDissolves->SetPolymer(pPolymer);
		pPolymerDissolves->SetDuration(pDefaultPolymerDissolves->GetDuration());
		pPolymerDissolves->SetSpringConstant(pDefaultPolymerDissolves->GetSpringConstant());
		pPolymerDissolves->SetLength(pDefaultPolymerDissolves->GetLength());

		// We ignore the return value as this event ensures the pre-conditions are satisfied

		pPolymerDissolves->CheckPreconditions();

		pPolymerDissolves->AddContraEvent(pBondBindsToHead);
		pPolymerDissolves->AddContraEvent(pBondBindsToTail);
*/

		// Add the growth events to the dissolution event as dependent events
		// so that they get destroyed when their containing event is destroyed.
		// The AddDependentEvent() function is responsible for removing such
		// events from the containing ACN's event container to prevent them
		// being destroyed more than once.

//		pPolymerDissolves->AddDependentEvent(pBondBindsToHead);
//		pPolymerDissolves->AddDependentEvent(pBondBindsToTail);
//		pPolymerDissolves->AddDependentEvent(pBondUnbindsFromHead);
//		pPolymerDissolves->AddDependentEvent(pBondUnbindsFromTail);

		return false;
	}
}



