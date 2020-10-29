/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aevPolymerFormsBondTriple.cpp: implementation of the aevPolymerFormsBondTriple class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "aevPolymerFormsBondTriple.h"
#include "aeActiveBond.h"
#include "aeActivePolymer.h"
#include "Bond.h"

#include "IActiveSimBox.h"
#include "IActivePolymerFormsBondTriple.h"
#include "aeCNTCell.h"
#include "aeActiveCellNetwork.h"



//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString aevPolymerFormsBondTriple::m_Type = "PolymerFormsBondTriple";

const zString aevPolymerFormsBondTriple::GetType()
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
	aevActiveEvent* Create() {return new aevPolymerFormsBondTriple();}

	const zString id = aevPolymerFormsBondTriple::GetType();

	const bool bRegistered = acfActiveEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aevPolymerFormsBondTriple::aevPolymerFormsBondTriple() : m_Duration(0), 
									 m_Range(0.0),
									 m_SpringConstant(0.0),
									 m_Length(0.0),
									 m_pIEvent(0), 
									 m_pTail(0), m_pFirstHead(0), m_pSecondHead(0), m_pPolymer(0),
									 m_pInternalBond(new CBond(-1,0.0,0.0)),
									 m_Counter(0)

								 
{

}

// The same aggregate object is passed to the copied event. Note that we create
// a new CBond here as the old one will be destroyed when the oldEvent instance is
// destroyed.
//

aevPolymerFormsBondTriple::aevPolymerFormsBondTriple(const aevPolymerFormsBondTriple& oldEvent) : aevActiveEvent(oldEvent),
																	m_Duration(oldEvent.m_Duration),
																	m_Range(oldEvent.m_Range),
																	m_SpringConstant(oldEvent.m_SpringConstant),
																	m_Length(oldEvent.m_Length),                                                                   
																	m_iterTail(oldEvent.m_iterTail),
																	m_pTail(oldEvent.m_pTail),
																	m_pFirstHead(oldEvent.m_pFirstHead),
																	m_pSecondHead(oldEvent.m_pSecondHead),
																	m_pPolymer(oldEvent.m_pPolymer),
																	m_pInternalBond(new CBond(-1,0.0,0.0)),
																	m_Counter(oldEvent.m_Counter)
{
}

aevPolymerFormsBondTriple::~aevPolymerFormsBondTriple()
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

	// Active events do not destroy their contained monomer/polymer instances as 
	// they pass them over to their owning instance once their actions have been 
	// carried out. The active polymers created by this class of event are owned 
	// by the calling ACN.
}

// Member functions to write/read the data specific to the command.
// The name of bead type to be removed from display is the only data needed.

zOutStream& aevPolymerFormsBondTriple::put(zOutStream& os) const
{
	// Write base class data first

	aevActiveEvent::put(os);
	
	return os;
}

zInStream& aevPolymerFormsBondTriple::get(zInStream& is)
{
	// Read base class data first

	aevActiveEvent::get(is);
	
	SetDataValid(true);

	return is;
}

// Non-static function to return the type of the event

const zString aevPolymerFormsBondTriple::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

aevActiveEvent* aevPolymerFormsBondTriple::GetEvent() const
{
	return new aevPolymerFormsBondTriple(*this);
}

// **********************************************************************
// Functions to set the parameters for the event

void aevPolymerFormsBondTriple::SetDuration(long time)
{
	if(time > 0)
	{
		m_Duration = time;
	}
}

void aevPolymerFormsBondTriple::SetRange(double range)
{
	if(range > 0.0 && range < 0.5*GetACN()->GetSimBoxXLength() &&
		               range < 0.5*GetACN()->GetSimBoxYLength() &&
		               range < 0.5*GetACN()->GetSimBoxZLength())
	{
		m_Range = range;
	}
}

void aevPolymerFormsBondTriple::SetSpringConstant(double spring)
{
	if(spring > 0.0)
	{
		m_SpringConstant = spring;
	}
}

void aevPolymerFormsBondTriple::SetLength(double length)
{
	if(length >= 0.0 && length < 0.5*GetACN()->GetSimBoxXLength() &&
		                length < 0.5*GetACN()->GetSimBoxYLength() &&
		                length < 0.5*GetACN()->GetSimBoxZLength())
	{
		m_Length = length;
	}
}

// **********************************************************************
// Function to validate the data when an event is created on the fly. It 
// should be implemented by all event classes that may be created during a simulation.

bool aevPolymerFormsBondTriple::InternalValidateData()
{
	return true;
}

// **********************************************************************
// Check of the event's preconditions to see if it should proceed with its
// actions.

bool aevPolymerFormsBondTriple::CheckPreconditions()
{
	// Find another bond to form a polymer

	aeCNTCell* const pCell = GetCellFromBond(*m_iterTail);

	aeActiveBond* pTargetBond = pCell->UpdateNucleation(*m_iterTail);

	// If the two bonds are within a given distance, we allow them to form a bond.
	// We connect the two bonds using the aeActiveBond::AddHeadAdjacentBond() so that
	// their polymerised flags are set, thereby preventing them being chosen by later 
	// events. We know that if the event's preconditions are satisfied, the bonds
	// cannot break apart before they have formed a polymer, so there is no
	// danger in tying them together now rather than when the event's Execute() function
	// finally creates the active polymer instance.

	if(*m_iterTail && pTargetBond)
	{
		m_pTail = *m_iterTail;
		m_pFirstHead = pTargetBond;

		double dx = m_pTail->GetTailHeadBead()->GetXPos() - m_pFirstHead->GetTailHeadBead()->GetXPos();
		double dy = m_pTail->GetTailHeadBead()->GetYPos() - m_pFirstHead->GetTailHeadBead()->GetYPos();
		double dz = m_pTail->GetTailHeadBead()->GetZPos() - m_pFirstHead->GetTailHeadBead()->GetZPos();

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
			m_pTail->AddHeadAdjacentBond(m_pFirstHead);

			RemoveFreeBond(m_pFirstHead);

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

bool aevPolymerFormsBondTriple::Execute(IActiveSimBox* const pShadow)
{
	// If this is the first time the event is executed, store an interface
	// pointer to the active SimBox and containing ACN and initialise the bond
	// formation counter and transient bond.

	if(!m_pIEvent)
	{
		m_pIEvent = pShadow->GetIActivePolymerFormsBondTriple();
		m_Counter = GetDuration();
		m_pInternalBond->SetBeads(m_pTail->GetTailHeadBead(), m_pFirstHead->GetTailHeadBead());
		m_pInternalBond->SetUnStretchedLength(GetLength());
	}

	// Get the initial separation of the two monomers

	const double dr = m_pInternalBond->CalculateunPBCLength();

	if(m_Counter > 0)
	{
		// On subsequent invocations, gradually increase the strength of the 
		// enclosed transient bond until it reaches its pre-defined value, 
		// so that the monomers are gradually positioned at the user-defined 
		// separation for forming an active polymer.

		m_pInternalBond->SetSpringConstant(GetSpringConstant()/static_cast<double>(m_Counter));
		m_pInternalBond->AddForce();
		m_Counter--;

		return true;
	}
	else if(dr > GetLength())
	{
		// Bonds are not yet in place, so strengthen the spring for this event
		// and reset the counter

		SetSpringConstant(2.0*GetSpringConstant());
		m_Counter = GetDuration();

		return true;
	}
	else
	{
		// Now that the bonds are in the right configuration, create a new 
		// active polymer, and signal the containing ACN to destroy this event
		// by returning false. Note the order of the arguments: new polymer's 
		// head bond then its tail bond. Finally, add the new polymer to 
		// the ACN's free polymer container.
			
		aeActivePolymer* pPolymer = new aeActivePolymer(m_pFirstHead, m_pTail);
		AddFreePolymer(pPolymer);
	
		return false;
	}
}



