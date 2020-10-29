/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aevBondBindsForwardConeToPolymerTail.cpp: implementation of the aevBondBindsForwardConeToPolymerTail class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "aevBondBindsForwardConeToPolymerTail.h"
#include "aevBondHydrolysesATP.h"
#include "aeActiveBond.h"
#include "aeActivePolymer.h"
#include "Bond.h"
#include "aeCNTCell.h"

#include "IActiveSimBox.h"
#include "IActiveBondBindsToPolymer.h"
#include "aeActiveCellNetwork.h"



//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString aevBondBindsForwardConeToPolymerTail::m_Type = "BondBindsForwardConeToPolymerTail";

const zString aevBondBindsForwardConeToPolymerTail::GetType()
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
	aevActiveEvent* Create() {return new aevBondBindsForwardConeToPolymerTail();}

	const zString id = aevBondBindsForwardConeToPolymerTail::GetType();

	const bool bRegistered = acfActiveEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aevBondBindsForwardConeToPolymerTail::aevBondBindsForwardConeToPolymerTail() : m_Duration(0), 
									 m_Range(0.0),  m_ConeAngle(0.0),
									 m_SpringConstant(0.0),
									 m_Length(0.0),
									 m_pIEvent(0),
									 m_pPolymer(0),
									 m_pNewTailBond(0),
									 m_pInternalABond(new CBond(-1,0.0,0.0)),
									 m_pInternalBBond(new CBond(-1,0.0,0.0)),
									 m_Counter(0),
									 m_CosCA(0.0)

								 
{

}

// The same aggregate object is passed to the copied event. Note that we create
// a new CBond here as the old one will be destroyed when the oldEvent instance is
// destroyed.
//

aevBondBindsForwardConeToPolymerTail::aevBondBindsForwardConeToPolymerTail(const aevBondBindsForwardConeToPolymerTail& oldEvent) : aevActiveEvent(oldEvent),
																	m_Duration(oldEvent.m_Duration),
																	m_Range(oldEvent.m_Range),
																	m_ConeAngle(oldEvent.m_ConeAngle),
																	m_SpringConstant(oldEvent.m_SpringConstant),
																	m_Length(oldEvent.m_Length),                                                                   
																	m_pPolymer(oldEvent.m_pPolymer),
																	m_pNewTailBond(oldEvent.m_pNewTailBond),
																	m_pInternalABond(new CBond(-1,0.0,0.0)),
																	m_pInternalBBond(new CBond(-1,0.0,0.0)),
																	m_Counter(oldEvent.m_Counter),
																	m_CosCA(oldEvent.m_CosCA)
{
}

aevBondBindsForwardConeToPolymerTail::~aevBondBindsForwardConeToPolymerTail()
{
	// Destroy the contained active event interface instance if it has been created
	
	if(m_pIEvent)
	{
		delete m_pIEvent;
		m_pIEvent = 0;
	}

	// Delete the transient bond between monomers if it exists

	if(m_pInternalABond)
		delete m_pInternalABond;
	if(m_pInternalBBond)
		delete m_pInternalBBond;
}

// Member functions to write/read the data specific to the command.
// The name of bead type to be removed from display is the only data needed.

zOutStream& aevBondBindsForwardConeToPolymerTail::put(zOutStream& os) const
{
	// Write base class data first

	aevActiveEvent::put(os);
	
	return os;
}

zInStream& aevBondBindsForwardConeToPolymerTail::get(zInStream& is)
{
	// Read base class data first

	aevActiveEvent::get(is);
	
	SetDataValid(true);

	return is;
}

// Non-static function to return the type of the event

const zString aevBondBindsForwardConeToPolymerTail::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

aevActiveEvent* aevBondBindsForwardConeToPolymerTail::GetEvent() const
{
	return new aevBondBindsForwardConeToPolymerTail(*this);
}

// **********************************************************************
// Functions to set the parameters for the event

// Function to store the active polymer that is being monitored by this event.
// If the pointer passed in is null there is not much we can do.

void aevBondBindsForwardConeToPolymerTail::SetPolymer(aeActivePolymer *const pPolymer)
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

void aevBondBindsForwardConeToPolymerTail::SetDuration(long time)
{
	if(time > 0)
	{
		m_Duration = time;
	}
}

void aevBondBindsForwardConeToPolymerTail::SetRange(double range)
{
	if(range > 0.0)
	{
		m_Range = range;
	}
}

void aevBondBindsForwardConeToPolymerTail::SetConeAngle(double angle)
{
	if(angle >= 0.0 && angle <= 0.5*xxBase::m_globalPI)
	{
		// Angle must be specified in radians. Note that a half-angle of zero means 
		// a zero-thickness cone and a half-angle of 90 degrees means a cone
		// that occupies the half-space ahead of the filament. We also store the 
        // cosine of the angle to speed up comparisons for potential new bonds

		m_ConeAngle = angle;
		m_CosCA     = cos(m_ConeAngle);
	}
}

void aevBondBindsForwardConeToPolymerTail::SetSpringConstant(double spring)
{
	if(spring > 0.0)
	{
		m_SpringConstant = spring;
	}
}

void aevBondBindsForwardConeToPolymerTail::SetLength(double length)
{
	if(length > 0.0)
	{
		m_Length = length;
	}
}


// **********************************************************************
// Function to validate the data when an event is created on the fly. It 
// should be implemented by all event classes that may be created during a simulation.

bool aevBondBindsForwardConeToPolymerTail::InternalValidateData()
{
	return true;
}

// **********************************************************************
// Check of the event's preconditions to see if it should proceed with its
// actions. This event is automatically created when a new polymer is formed.

bool aevBondBindsForwardConeToPolymerTail::CheckPreconditions()
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
// to see if an active bond can attach to its head end. The conditions for
// a bond to attach differ slightly from those of the generic aevBondBindsToPolymerHead.
// Here, a monomer is selected that is within a specified distance of the
// polymer's head bond, and it is then attached by two of its corners only.
// After a period of time to allow its orientation to settle down, the third
// corner is attached. I expect that this prevents attachment of the monomer
// in sub-optimal orientations. This procedure requires the duration of the
// monomer attachment process to be divided into two. We just use equal
// durations for now.
//

bool aevBondBindsForwardConeToPolymerTail::Execute(IActiveSimBox* const pShadow)
{
	// If this is the first time the event is executed, store an interface
	// pointer to the active SimBox.

	if(!m_pIEvent)
	{
		m_pIEvent = pShadow->GetIActiveBondBindsToPolymer();
	}

	// Only search for a new bond to attach to the polymer if 
    // no contra events are active. Then check that one is not
	// already being worked on and  We select a potential bond 
    // from the cell containing the current one, and then apply 
    // further tests if desired.

	if(!IsAnyContraEventActive())
    {
	    if(!m_pNewTailBond)
	    {
            if(!GetACN()->GetPhantomBondsAllowed() || GetACN()->GetPhantomBondTotal() > 0)
            {
		        aeCNTCell* const pCell = GetCellFromBond(m_pPolymer->GetTailBond());
				        
		        aeActiveBond* pTargetBond = pCell->UpdatePolymerisation(m_pPolymer->GetTailBond());

		        if(pTargetBond)
		        {
			        // Only allow the target bond to bind to the filament if it
			        // lies within the allowed conical region behind the filament's tail.
			        // To define the location of each active bond we use the Head bead 
			        // of the active bond's tail CPolymer. 

			        double dx1, dy1, dz1, dr1;
			        double dx2, dy2, dz2, dr2;
			        double cosTarget;
    /*
			        CAbstractBead* pTargetBondBead = pTargetBond->GetTailMonomer()->GetHead();
			        CAbstractBead* pFilamentBead = m_pPolymer->GetTailBond()->GetTailMonomer()->GetHead();

			        // Unit vector in direction of filament: we use the filament's
			        // tail monomer and its head adjacent monomer to define the local
			        // axis.

			        aeActiveBond* pAdjBond      = m_pPolymer->GetTailBond()->GetHeadAdjacentBond();
			        CAbstractBead* pAdjHeadBead = pAdjBond->GetTailMonomer()->GetHead();

			        dx1 = pFilamentBead->GetXPos() - pAdjHeadBead->GetXPos();
			        dy1 = pFilamentBead->GetYPos() - pAdjHeadBead->GetYPos();
			        dz1 = pFilamentBead->GetZPos() - pAdjHeadBead->GetZPos();
    */
                    aaVector targetCM          = pTargetBond->GetCM();
                    aaVector filamentTailCM    = m_pPolymer->GetTailBond()->GetCM();
                    aaVector filamentAdjTailCM = m_pPolymer->GetTailBond()->GetHeadAdjacentBond()->GetCM();

                    dx1 = filamentTailCM.GetX() - filamentAdjTailCM.GetX();
                    dy1 = filamentTailCM.GetY() - filamentAdjTailCM.GetY();
                    dz1 = filamentTailCM.GetZ() - filamentAdjTailCM.GetZ();

                    if( dx1 > GetACN()->GetIActiveSimBox()->GetHalfSimBoxXLength() )
				        dx1 = dx1 - GetACN()->GetIActiveSimBox()->GetSimBoxXLength();
			        else if( dx1 < -GetACN()->GetIActiveSimBox()->GetHalfSimBoxXLength() )
				        dx1 = dx1 + GetACN()->GetIActiveSimBox()->GetSimBoxXLength();

			        if( dy1 > GetACN()->GetIActiveSimBox()->GetHalfSimBoxYLength() )
				        dy1 = dy1 - GetACN()->GetIActiveSimBox()->GetSimBoxYLength();
			        else if( dy1 < -GetACN()->GetIActiveSimBox()->GetHalfSimBoxYLength() )
				        dy1 = dy1 + GetACN()->GetIActiveSimBox()->GetSimBoxYLength();

        #if SimDimension == 3
			        if( dz1 > GetACN()->GetIActiveSimBox()->GetHalfSimBoxZLength() )
				        dz1 = dz1 - GetACN()->GetIActiveSimBox()->GetSimBoxZLength();
			        else if( dz1 < -GetACN()->GetIActiveSimBox()->GetHalfSimBoxZLength() )
				        dz1 = dz1 + GetACN()->GetIActiveSimBox()->GetSimBoxZLength();
        #else
			        dz1 = 0.0;
        #endif

			        dr1 = sqrt(dx1*dx1 + dy1*dy1 + dz1*dz1);

    //			    dx2 = pTargetBondBead->GetXPos() - pFilamentBead->GetXPos();
    //			    dy2 = pTargetBondBead->GetYPos() - pFilamentBead->GetYPos();
    //			    dz2 = pTargetBondBead->GetZPos() - pFilamentBead->GetZPos();

                    dx2 = targetCM.GetX() - filamentTailCM.GetX();
                    dy2 = targetCM.GetY() - filamentTailCM.GetY();
                    dz2 = targetCM.GetZ() - filamentTailCM.GetZ();

			        if( dx2 > GetACN()->GetIActiveSimBox()->GetHalfSimBoxXLength() )
				        dx2 = dx2 - GetACN()->GetIActiveSimBox()->GetSimBoxXLength();
			        else if( dx2 < -GetACN()->GetIActiveSimBox()->GetHalfSimBoxXLength() )
				        dx2 = dx2 + GetACN()->GetIActiveSimBox()->GetSimBoxXLength();

			        if( dy2 > GetACN()->GetIActiveSimBox()->GetHalfSimBoxYLength() )
				        dy2 = dy2 - GetACN()->GetIActiveSimBox()->GetSimBoxYLength();
			        else if( dy2 < -GetACN()->GetIActiveSimBox()->GetHalfSimBoxYLength() )
				        dy2 = dy2 + GetACN()->GetIActiveSimBox()->GetSimBoxYLength();

        #if SimDimension == 3
			        if( dz2 > GetACN()->GetIActiveSimBox()->GetHalfSimBoxZLength() )
				        dz2 = dz2 - GetACN()->GetIActiveSimBox()->GetSimBoxZLength();
			        else if( dz2 < -GetACN()->GetIActiveSimBox()->GetHalfSimBoxZLength() )
				        dz2 = dz2 + GetACN()->GetIActiveSimBox()->GetSimBoxZLength();
        #else
			        dz2 = 0.0;
        #endif

			        dr2 = sqrt(dx2*dx2 + dy2*dy2 + dz2*dz2);

			        // Calculate the cosine of the angle between the filament axis and the
			        // unit vector from the filament's head to the target bond, and 
			        // only accept the target if this angle is less than the cone angle.

			        cosTarget = (dx1*dx2 + dy1*dy2 + dz1*dz2)/(dr1*dr2);

			        if(cosTarget > m_CosCA)
			        {
				        m_pNewTailBond = pTargetBond;

				        // If there is a free bond within the specified distance of the polymer's
				        // tail, we attempt to attach it to the polymer; we simultaneously
				        // remove it from the ACN's free bond container and set its polymerised 
				        // flag to true so that it is not made the target of a different event.
				        
                        // Indicate to all contra events that this event is active
                        // until the new bond has been attached

                        SetActive();
				        m_Counter = GetDuration();
				        m_pNewTailBond->SetPolymerised();
				        RemoveFreeBond(m_pNewTailBond);

				        // Create the AA and BB internal bonds to bring the new tail bond 
				        // into the correct orientation for attachment to the filament.

				        m_pInternalABond->SetBeads(m_pPolymer->GetTailBond()->GetTailHeadBead(), m_pNewTailBond->GetTailHeadBead());
				        m_pInternalABond->SetUnStretchedLength(GetLength());

				        m_pInternalBBond->SetBeads(m_pPolymer->GetTailBond()->GetTailTailBead(), m_pNewTailBond->GetTailTailBead());
				        m_pInternalBBond->SetUnStretchedLength(GetLength());
			        }
		        }
            }
	    }
	    else
	    {
		    // Get the initial separation of the two monomers: we use the A bond
		    // as our estimate

		    const double dr = m_pInternalABond->CalculatePBCLength();

		    if(dr < GetLength())
		    {
                SetInactive();
                IncrementSuccesses();  // Increment the event's success counter
                BroadcastState();      // Attempt to broadcast the state change

                // Now that the bond is in the right configuration make it the
			    // tail of the polymer. We call the SetPolymer() function simply
			    // to reset the head pointer to the new bond.

			    m_pPolymer->AddTailBond(m_pNewTailBond);
			    SetPolymer(m_pPolymer);
			    m_pNewTailBond = 0;
                m_pInternalABond->SetBeads(0,0);
                m_pInternalBBond->SetBeads(0,0);

                // Toggle the equivalent number of phantom bonds in the bulk  
                // to their active state to maintain the constant number of free
                // bonds in the bulk.

                GetACN()->ConvertPhantomBondsToFree();

                // Create a new event to represent the ATP hydrolysis of the
                // tail monomer, and store it in the containing ACN. Note that 
                // we assume that the free monomer is NOT associated with such 
                // an event.

                aevBondHydrolysesATP* pNextEvent = dynamic_cast<aevBondHydrolysesATP*>(AddEvent(aevBondHydrolysesATP::GetType()));

                pNextEvent->SetContainingACN(GetACN());
                pNextEvent->SetBond(m_pPolymer->GetTailBond());
                pNextEvent->SetExecutionPeriod(GetACN()->GetInternalEventExecutionPeriodFromName(pNextEvent->GetEventType()));
		        pNextEvent->CheckPreconditions();

			    // Note that the internal bonds are stored using their aeActiveEvent
			    // base class so we do not have to overload the ACN's 
			    // GetInternalBondBindsToPolymerHeadEvent() function.

			    SetSpringConstant(dynamic_cast<aevBondBindsForwardConeToPolymerTail*>(GetACN()->GetInternalBondBindsToPolymerTailEvent())->GetSpringConstant());
		    }
		    else if(m_Counter > 0)
		    {
			    // On subsequent invocations, gradually increase the strength of the 
			    // enclosed transient bond until it reaches its pre-defined value, 
			    // so that the bond are gradually positioned at the user-defined 
			    // separation for attaching to the polymer's head.

			    m_pInternalABond->SetSpringConstant(GetSpringConstant()*(1.0 - static_cast<double>(m_Counter)/static_cast<double>(GetDuration())));
			    m_pInternalABond->AddPBCForce();

			    m_pInternalBBond->SetSpringConstant(GetSpringConstant()*(1.0 - static_cast<double>(m_Counter)/static_cast<double>(GetDuration())));
			    m_pInternalBBond->AddPBCForce();
			    m_Counter--;
		    }
		    else
		    {
                SetInactive();
                IncrementFailures();  // Increment the event's failure counter

                // Bond is not yet close enough to attach to the polymer, so release it
			    // back into the free monomer pool, reset its polymerised flag, reset
                // this event to inactive and wait for a new one to attach.

			    m_pNewTailBond->SetDepolymerised();
			    AddFreeBond(m_pNewTailBond);
			    m_pNewTailBond = 0;
                m_pInternalABond->SetBeads(0,0);
                m_pInternalBBond->SetBeads(0,0);
		    }
	    }
    }

	// This event always returns true as it continually monitors
	// the polymer for growth. It is destroyed by the event
	// that monitors the polymer's dissolution.

	return true;
}

// VF that allows this event class to broadcast information about its state
// to its containing instance for analysis by event source/analysis decorators.
// Currently, this event just tells its containing ACN that it has successfully
// executed and a new bond has been attached to the active polymer.

void aevBondBindsForwardConeToPolymerTail::BroadcastState()
{
    if(GetBroadcastState())
    {
        GetACN()->BroadcastEventState(this);
    }
}

// Function to return a pointer to the head bond of the polymer. This can be used
// by Event Source Decorators to analyse the growth of the active polymer.

const aeActiveBond* aevBondBindsForwardConeToPolymerTail::GetTailBond() const
{
    return m_pPolymer->GetTailBond();
}
