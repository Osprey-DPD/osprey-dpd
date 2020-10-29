/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aefActinNetwork.cpp: implementation of the aefActinNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "aefActinNetwork.h"
#include "aeCNTCell.h"
#include "aefActinBond.h"
#include "aeActivePolymer.h"
#include "Polymer.h"
#include "IfActinAccessControl.h"
#include "prfActin.h"
#include "LogTextMessage.h"

#include "aevPolymerForms.h"
#include "aevPolymerDissolves.h"
#include "aevBondBindsABSequentiallyToPolymerHead.h"
#include "aevBondBindsABSequentiallyToPolymerTail.h"
#include "aevBondBindsForwardConeToPolymerHead.h"
#include "aevBondBindsForwardConeToPolymerTail.h"
#include "aevBondUnbindsFromPolymerHead.h"
#include "aevBondUnbindsFromPolymerTail.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.

const zString aefActinNetwork::m_Type = "fActin";

const zString aefActinNetwork::GetType()
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
	aeActiveCellNetwork* Create(IActiveSimBox* const pShadow, 
								 long xn, long yn, long zn,
								 double xw, double yw, double zw) {return new aefActinNetwork(pShadow, xn, yn, zn, xw, yw, zw);}

	const zString id = aefActinNetwork::GetType();

	const bool bRegistered = acfACNFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aefActinNetwork::aefActinNetwork(IActiveSimBox* const pShadow, 
								 long xn, long yn, long zn,
								 double xw, double yw, double zw) : IModifyfActinNetwork(pShadow, xn, yn, zn, xw, yw, zw),
                                 m_bRefreshDisplayStatus(false),
                                 m_ATPDisplayId(-1), m_ADPPiDisplayId(-1), m_ADPDisplayId(-1)
{

	// This ACN implements access control to its internal state via the 
	// IACNAccessControl protected member of the aeActiveCellNetwork base class.

	m_pIACNAccess = new IfActinAccessControl(this);

	// Define the default events for this ACN and set their default parameters

	aevPolymerForms* pPolymerForms = new aevPolymerForms();
	pPolymerForms->SetContainingACN(this);
	pPolymerForms->SetDuration(100);
	pPolymerForms->SetRange(2.0);
	pPolymerForms->SetSpringConstant(128.0);
	pPolymerForms->SetLength(1.0);
	SetInternalPolymerFormsEvent(pPolymerForms);

	aevPolymerDissolves* pPolymerDissolves = new aevPolymerDissolves();
	pPolymerDissolves->SetContainingACN(this);
	pPolymerDissolves->SetDuration(100);
	pPolymerDissolves->SetSpringConstant(128.0);
	pPolymerDissolves->SetLength(1.5);
	SetInternalPolymerDissolvesEvent(pPolymerDissolves);

	aevBondBindsForwardConeToPolymerHead* pBondBindsToHead = new aevBondBindsForwardConeToPolymerHead();
	pBondBindsToHead->SetContainingACN(this);
	pBondBindsToHead->SetDuration(100);
	pBondBindsToHead->SetRange(4.0);
	pBondBindsToHead->SetConeAngle(45.0*xxBase::m_globalPI/180.0);
	pBondBindsToHead->SetSpringConstant(32.0);
	pBondBindsToHead->SetLength(1.0);
	SetInternalBondBindsToPolymerHeadEvent(pBondBindsToHead);

	aevBondBindsForwardConeToPolymerTail* pBondBindsToTail = new aevBondBindsForwardConeToPolymerTail();
	pBondBindsToTail->SetContainingACN(this);
	pBondBindsToTail->SetDuration(100);
	pBondBindsToTail->SetRange(4.0);
	pBondBindsToTail->SetConeAngle(45.0*xxBase::m_globalPI/180.0);
	pBondBindsToTail->SetSpringConstant(32.0);
	pBondBindsToTail->SetLength(1.0);
	SetInternalBondBindsToPolymerTailEvent(pBondBindsToTail);

	aevBondUnbindsFromPolymerHead* pBondUnbindsFromHead = new aevBondUnbindsFromPolymerHead();
	pBondUnbindsFromHead->SetContainingACN(this);
	pBondUnbindsFromHead->SetDuration(100);
	pBondUnbindsFromHead->SetSpringConstant(32.0);
	pBondUnbindsFromHead->SetLength(4.0);
	SetInternalBondUnbindsFromPolymerHeadEvent(pBondUnbindsFromHead);

	aevBondUnbindsFromPolymerTail* pBondUnbindsFromTail = new aevBondUnbindsFromPolymerTail();
	pBondUnbindsFromTail->SetContainingACN(this);
	pBondUnbindsFromTail->SetDuration(100);
	pBondUnbindsFromTail->SetSpringConstant(32.0);
	pBondUnbindsFromTail->SetLength(4.0);
	SetInternalBondUnbindsFromPolymerTailEvent(pBondUnbindsFromTail);

}

aefActinNetwork::~aefActinNetwork()
{

}

// Non-static function to return the type of the ACN.

const zString aefActinNetwork::GetACNType() const
{
	return m_Type;
}

// Function to attach the associated process to the ACN. We do it here as the
// ACN is the only object that knows the type of its associated process.

void aefActinNetwork::AttachProcess(xxProcess* pProcess)
{
	prfActin* pActinProcess = dynamic_cast<prfActin*>(pProcess);

	pActinProcess->SetACN(this);		
}

// Function that updates the state of all free active bonds, or monomers, and polymers
// that represent fActin filaments. Free monomers exert forces on each other when they 
// are within range, and may assemble into polymers. When a monomer is part of a polymer
// it is said to be "activated". Once an active bond has been activated, it is removed 
// from the free active bond container. The active polymer it is a part
// of is then added to the free active polymer container unless it was already there.
// Hence, any bond that is still in the m_FreeActiveBonds container is by definition
// not part of an active polymer and can exert forces on other bonds.
//
// Each active bond is created by wrapping it around an existing polymer that becomes
// the tail monomer of the bond. Activation occurs when a free active bond comes within
// a specified range of another free active bond, or the head or tail monomers of an
// active polymer, and certain bond type-specific conditions are satisfied (see the
// aeBondOnCondition-derived clases). The two bonds are then joined via their head and
// tail monomers. In the case of two free bonds joining a new active polymer is 
// created, passing the newly joined bonds to it. When a bond joins an existing
// polymer it is just added to the polymer's bond list and the polymer's head or tail
// updated to take account of which end the bond joined.
//
// Each activated bond exerts a force on its contained monomers (via their head and tail
// beads). Only bonds that have been activated exert the force, so we first see if 
// any new bonds can be activated and then calculate the forces exerted by the activated
// bonds.

void aefActinNetwork::Evolve()
{
	// Sweep over all active bonds updating their positions; and call other
	// ACN-level functions that are common to all ACN's including updating all
	// events.

	UpdateActiveBondsInCells();

	// Attempt to nucleate new active polymers using the active event aevPolymerForms.
	// If any bonds satisfy the conditions for the event, a new event
	// instance is created and stored in the aeActiveCellNetwork base class 
	// where it is updated each time step to see if the active polymer can be 
	// formed. Subsequent evolution of the polymer is controlled by the
	// event and further events it creates.

	Nucleate();

    // Refresh the bonds' display status if required

    if(m_bRefreshDisplayStatus)
    {
        UpdateBondDisplayStatus();
    }

	// Generate density profiles of the active bonds and filaments if required

	if(IsDensityFieldAnalysisOn())
	{
		CalculateDensityFields();
	}


	// Now do some simple analysis of the filaments

	long filamentTotal = GetFilamentTotal();

	if(filamentTotal > 0)
	{
		CalculateFilamentProperties();
		CalculateFilamentInCylindricalPotentialProperties();
        CalculateEventCounters();
        CheckMonomers();
/*
		long freeMonoTotal   = GetFreeBondTotal();
		long boundMonoTotal  = GetBoundBondTotal();
		long monoTotal		 = GetMonomerTotal();

		double freeFraction  = GetFreeBondFraction();
		double boundFraction = GetBoundBondFraction();
		double meanSize		 = GetMeanFilamentSize();
		double meanSqSize	 = GetMeanSqFilamentSize();
		double sdevSize		 = GetSDevFilamentSize();
		double meanLength	 = GetMeanFilamentLength();
		double meanSqLength	 = GetMeanSqFilamentLength();
		double sdevLength	 = GetSDevFilamentLength();

		std::cout << "Actin monos = " << freeMonoTotal << "  " << boundMonoTotal << "  " << monoTotal << "  ";
		std::cout << freeFraction << "  " << boundFraction << zEndl;
		std::cout << "Actin filaments = " << filamentTotal << "  " << GetMeanFilamentSize()  << "  " << GetMeanFilamentLength() << zEndl;
 */
	}
}

// Function that attempts to construct active polymers out of two active bonds that
// satisfy certain conditions. The specific conditions for polymer nucleation
// depend on the active bond type and are specified in the virtual function
// aeActiveBond::Activate().
//
// We iterate over all free active bonds, obtain the cell containing each bond and
// check that cell and its nearest neighbours to see if two bonds can nucleate
// a new polymer. If nucleation succeeds, we remove both bonds from the free active
// bond container and store them inside a newly-created aevPolymerForms event.
// This event then takes over the task of orienting the bonds so that they can form
// a new polymer. When the event's task is complete, it passes control of the bonds
// to a newly-created active polymer which is then stored in the fActin network's
// free active polymer container.

void aefActinNetwork::Nucleate()
{
    // Only try and create new polymers if no restriction is set or we are not 
    // yet at the maximum allowed.

    if(!GetRestrictMaxPolymers() || (GetFreePolymerTotal() < GetMaxPolymers()))
    {
	    // Create the first active event to watch for filament formation, and
	    // pass it a pointer to the ACN so that it can access other ACN's;
	    // then cast it to the specific type and do event-specific processing.
	    // We use the default active event held in the base class. This may
	    // be modified by command and/or have its parameters modified by command.

	    const aevPolymerForms* const pDefaultPolymerForms = dynamic_cast<aevPolymerForms*>(GetInternalPolymerFormsEvent());

	    aevPolymerForms* pPolymerForms = dynamic_cast<aevPolymerForms*>(AddEvent(pDefaultPolymerForms->GetEventType()));
	    pPolymerForms->SetContainingACN(this);
	    pPolymerForms->SetDuration(pDefaultPolymerForms->GetDuration());
	    pPolymerForms->SetRange(pDefaultPolymerForms->GetRange());
	    pPolymerForms->SetSpringConstant(pDefaultPolymerForms->GetSpringConstant());
	    pPolymerForms->SetLength(pDefaultPolymerForms->GetLength());

	    ActiveBondListIterator iterBond=m_FreeActiveBonds.begin();

	    while(iterBond != m_FreeActiveBonds.end())
	    {
		    pPolymerForms->AddTailBond(iterBond);

		    // If the event's  pre-conditions succeeds it starts the process of binding 
		    // the bonds into a new filament and returns true. If the pre-conditions fail, 
		    // the event returns false and we increment the primary bond iterator 
		    // and re-use the event with another bond. Bonds that start the
		    // process of binding have their SetPolymerised() functions called so that
		    // they cannot be used by other events.

		    // If the user has turned on the polymer limit, then the number of events 
		    // that can be created is restricted to the maximum number 
		    // of polymers that this ACN allows

		    if((!GetRestrictMaxPolymers() || (GetPolymerFormsEventTotal() + GetFreePolymerTotal()) < GetMaxPolymers()) && pPolymerForms->CheckPreconditions())
		    {
			    m_PolymerFormsTotal++;		// Increment the event counter

                std::cout << "fActin polymer forms event succeeded: new total " << m_PolymerFormsTotal << zEndl;
			    iterBond = RemoveFreeBondByIterator(iterBond);

	            pPolymerForms = dynamic_cast<aevPolymerForms*>(AddEvent(pDefaultPolymerForms->GetEventType()));
	            pPolymerForms->SetContainingACN(this);
			    pPolymerForms->SetDuration(pDefaultPolymerForms->GetDuration());
			    pPolymerForms->SetRange(pDefaultPolymerForms->GetRange());
			    pPolymerForms->SetSpringConstant(pDefaultPolymerForms->GetSpringConstant());
			    pPolymerForms->SetLength(pDefaultPolymerForms->GetLength());

    			std::cout << "# events = " << GetEventTotal() << " # polymers/bonds " << m_FreeActivePolymers.size() << "  " << m_FreeActiveBonds.size() << zEndl;
		    }
		    else  // pre-conditions failed so move on to the next free bond 
		    {
			    iterBond++;
		    }
	    } 

	    // If the last event's pre-conditions failed delete it

	    if(!pPolymerForms->IsValid())
	    {
		    DeleteEvent(pPolymerForms);
	    }
    }
}


// Command handler function to change the maximum separation at which
// two mActin monomer active bonds can associate to form a filament.
// The distance test is implemented in the aeBondProximityOn class.
//
// Note that we have to modify both the free active bonds and the ones
// already assembled into filaments.

void aefActinNetwork::SetProximityOnSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aefActinBond* pBond = dynamic_cast<aefActinBond*>(*iterBond);

		pBond->SetOnRateSeparation(max);
	}

	// Now the bonds already assembled into filaments. Note that the polymers
	// know nothing about the internal workings of their bonds, so they cannot
	// be asked to modify the bond on/off conditions. Instead, we get access
	// to the bonds and modify them here as the network is assumed to know
	// how the bonds behave.

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActiveBond* pBond     = (*iterPoly)->GetTailBond();
		aeActiveBond* pHeadBond = (*iterPoly)->GetHeadBond();

		while(pBond != pHeadBond)
		{
			aefActinBond* pfActin = dynamic_cast<aefActinBond*>(pBond);

			pfActin->SetOnRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aefActinBond*>(pHeadBond)->SetOnRateSeparation(max);
	}

}

// Command handler function to change the maximum separation at which
// connected mActin monomers dissociate. The distance test is implemented 
// in the aeBondProximityOff class.
//
// Note that we have to modify both the free active bonds and the ones
// already assembled into filaments.

void aefActinNetwork::SetProximityOffSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond=m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aefActinBond* pBond = dynamic_cast<aefActinBond*>(*iterBond);

		pBond->SetOffRateSeparation(max);
	}

	// Now the bonds already assembled into filaments

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActiveBond* pBond     = (*iterPoly)->GetTailBond();
		aeActiveBond* pHeadBond = (*iterPoly)->GetHeadBond();

		while(pBond != pHeadBond)
		{
			aefActinBond* pfActin = dynamic_cast<aefActinBond*>(pBond);

			pfActin->SetOffRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aefActinBond*>(pHeadBond)->SetOffRateSeparation(max);
	}

}

// Command handler function to change the maximum separation at which
// connected mActin monomers associate via their heads. Note that this is
// not the head of the filament, but the head end of the bonds. Given that 
// bonds attach to the filament's tail via their heads, this actually affects
// the filament's TAIL attachment rate. 
// The distance test is implemented in the aeBondProximityOn class.

void aefActinNetwork::SetProximityHeadOnSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aefActinBond* pBond = dynamic_cast<aefActinBond*>(*iterBond);

		pBond->SetHeadOnRateSeparation(max);
	}

	// Now the bonds already assembled into filaments. Note that the polymers
	// know nothing about the internal workings of their bonds, so they cannot
	// be asked to modify the bond on/off conditions. Instead, we get access
	// to the bonds and modify them here as the network is assumed to know
	// how the bonds behave.

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActiveBond* pBond     = (*iterPoly)->GetTailBond();
		aeActiveBond* pHeadBond = (*iterPoly)->GetHeadBond();

		while(pBond != pHeadBond)
		{
			aefActinBond* pfActin = dynamic_cast<aefActinBond*>(pBond);

			pfActin->SetHeadOnRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aefActinBond*>(pHeadBond)->SetHeadOnRateSeparation(max);
	}
}

// Command handler function to change the maximum separation at which
// connected mActin monomers dissociate from the bond connected to their head.
// Given that the terminal bond at a filament's tail is attached by its head,
// this command modified the rate at which bonds leave a filament's TAIL.
// The distance test is implemented in the aeBondProximityOff class.

void aefActinNetwork::SetProximityHeadOffSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aefActinBond* pBond = dynamic_cast<aefActinBond*>(*iterBond);

		pBond->SetHeadOffRateSeparation(max);
	}

	// Now the bonds already assembled into filaments. Note that the polymers
	// know nothing about the internal workings of their bonds, so they cannot
	// be asked to modify the bond on/off conditions. Instead, we get access
	// to the bonds and modify them here as the network is assumed to know
	// how the bonds behave.

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActiveBond* pBond     = (*iterPoly)->GetTailBond();
		aeActiveBond* pHeadBond = (*iterPoly)->GetHeadBond();

		while(pBond != pHeadBond)
		{
			aefActinBond* pfActin = dynamic_cast<aefActinBond*>(pBond);

			pfActin->SetHeadOffRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aefActinBond*>(pHeadBond)->SetHeadOffRateSeparation(max);
	}
}

// Command handler function to change the maximum separation at which
// connected mActin monomers associate via their tails. Note that this is
// not the tail of the filament, but the tail end of the bonds. Given that 
// bonds attach to the filament's head via their tails, this actually affects
// the filament's HEAD attachment rate. 
// The distance test is implemented in the aeBondProximityOn class.

void aefActinNetwork::SetProximityTailOnSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aefActinBond* pBond = dynamic_cast<aefActinBond*>(*iterBond);

		pBond->SetTailOnRateSeparation(max);
	}

	// Now the bonds already assembled into filaments. Note that the polymers
	// know nothing about the internal workings of their bonds, so they cannot
	// be asked to modify the bond on/off conditions. Instead, we get access
	// to the bonds and modify them here as the network is assumed to know
	// how the bonds behave.

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActiveBond* pBond     = (*iterPoly)->GetTailBond();
		aeActiveBond* pHeadBond = (*iterPoly)->GetHeadBond();

		while(pBond != pHeadBond)
		{
			aefActinBond* pfActin = dynamic_cast<aefActinBond*>(pBond);

			pfActin->SetTailOnRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aefActinBond*>(pHeadBond)->SetTailOnRateSeparation(max);
	}
}

// Command handler function to change the maximum separation at which
// connected mActin monomers dissociate from the bond connected to their tail.
// Given that the terminal bond at a filament's head is attached by its tail,
// this command modified the rate at which bonds leave a filament's HEAD.
// The distance test is implemented in the aeBondProximityOff class.

void aefActinNetwork::SetProximityTailOffSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aefActinBond* pBond = dynamic_cast<aefActinBond*>(*iterBond);

		pBond->SetTailOffRateSeparation(max);
	}

	// Now the bonds already assembled into filaments. Note that the polymers
	// know nothing about the internal workings of their bonds, so they cannot
	// be asked to modify the bond on/off conditions. Instead, we get access
	// to the bonds and modify them here as the network is assumed to know
	// how the bonds behave.

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActiveBond* pBond     = (*iterPoly)->GetTailBond();
		aeActiveBond* pHeadBond = (*iterPoly)->GetHeadBond();

		while(pBond != pHeadBond)
		{
			aefActinBond* pfActin = dynamic_cast<aefActinBond*>(pBond);

			pfActin->SetTailOffRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aefActinBond*>(pHeadBond)->SetTailOffRateSeparation(max);
	}
}

// Command handler function to set a fixed probability for a monomer to detach
// from its neighbour via its head connection. There are two crucial points
// to note:
//
//  1 The rate depends on the time-step size: if the time-step is changed, the
//    rate should also be changed as a simple test is performed at each time-step
//    to see if a bond should detach. Hence, the rate is "per time-step" not
//    per unit time.
//
//  2 The detachment occurs via the active BOND's head connection, not the head
//    of the polymer to which it is attached. This allows more generic detachment
//    events than if we identified the detachment event solely with a monomer
//    leaving the end of a filament.

void aefActinNetwork::SetFixedHeadOffRate(double rate)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aefActinBond* pBond = dynamic_cast<aefActinBond*>(*iterBond);

		pBond->SetFixedHeadOffRate(rate);
	}

	// Now the bonds already assembled into filaments. Note that the polymers
	// know nothing about the internal workings of their bonds, so they cannot
	// be asked to modify the bond on/off conditions. Instead, we get access
	// to the bonds and modify them here as the network is assumed to know
	// how the bonds behave.

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActiveBond* pBond     = (*iterPoly)->GetTailBond();
		aeActiveBond* pHeadBond = (*iterPoly)->GetHeadBond();

		while(pBond != pHeadBond)
		{
			aefActinBond* pfActin = dynamic_cast<aefActinBond*>(pBond);

	        pfActin->SetFixedHeadOffRate(rate);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the bond that is not included in the above loop

		dynamic_cast<aefActinBond*>(pHeadBond)->SetFixedHeadOffRate(rate);
	}
}

// Command handler function to set a fixed probability for a monomer to detach
// from its neighbour via its tail connection. See the SetFixedHeadOffRate()
// function above for important details.

void aefActinNetwork::SetFixedTailOffRate(double rate)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aefActinBond* pBond = dynamic_cast<aefActinBond*>(*iterBond);

		pBond->SetFixedTailOffRate(rate);
	}

	// Now the bonds already assembled into filaments. Note that the polymers
	// know nothing about the internal workings of their bonds, so they cannot
	// be asked to modify the bond on/off conditions. Instead, we get access
	// to the bonds and modify them here as the network is assumed to know
	// how the bonds behave.

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActiveBond* pBond     = (*iterPoly)->GetTailBond();
		aeActiveBond* pHeadBond = (*iterPoly)->GetHeadBond();

		while(pBond != pHeadBond)
		{
			aefActinBond* pfActin = dynamic_cast<aefActinBond*>(pBond);

			pfActin->SetFixedTailOffRate(rate);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the bond that is not included in the above loop

		dynamic_cast<aefActinBond*>(pHeadBond)->SetFixedTailOffRate(rate);
	}
}

// Command handler function to set the fixed probability for an actin
// monomer to undergo the transition ATP --> ADP-Pi. The resulting 
// internal state change can be used to modify the probabilities of its
// behaviour, e.g., detaching from a filament. 
// Note that we check that the new probability is within [0,1] in the aefActinBond function.

void aefActinNetwork::SetATPHydrolysisProbability(double rate)
{
    aefActinBond::SetATPHydrolysisProbability(rate);
}

// Command handler function to set the fixed probability for an actin
// monomer to undergo the transition ADP-Pi -- ADP + pi. The resulting 
// internal state change can be used to modify the probabilities of its
// behaviour, e.g., detaching from a filament.
// Note that we check that the new probability is within [0,1] in the aefActinBond function.

void aefActinNetwork::SetADPReleasePiProbability(double rate)
{
    aefActinBond::SetADPReleasePiProbability(rate);
}

// Command handler function to set the fixed probability for an actin
// monomer to undergo the transition ADP --> ATP. The resulting 
// internal state change can be used to modify the probabilities of its
// behaviour, e.g., detaching from a filament.
// Note that we check that the new probability is within [0,1] in the aefActinBond function.

void aefActinNetwork::SetADPPhosphorylationProbability(double rate)
{
    aefActinBond::SetADPPhosphorylationProbability(rate);
}

// Command handler function to set the multiplier for an ADP-Pi
// bound actin monomer to detach from a filament via its (the bond's) head.
// Note that we check that the new probability is positive semi-definite
// in the aefActinBond function.

void aefActinNetwork::SetHeadADPPiMultiplier(double factor)
{
    aefActinBond::SetHeadADPPiMultiplier(factor);
}

// Command handler function to set the multiplier for an ADP-Pi
// bound actin monomer to detach from a filament via its (the bond's) tail.
// Note that we check that the new probability is positive semi-definite
// in the aefActinBond function.

void aefActinNetwork::SetTailADPPiMultiplier(double factor)
{
    aefActinBond::SetTailADPPiMultiplier(factor);
}

// Command handler function to set the multiplier for an ADP
// bound actin monomer to detach from a filament via its (the bond's) head.
// Note that we check that the new probability is positive semi-definite
// in the aefActinBond function.

void aefActinNetwork::SetHeadADPMultiplier(double factor)
{
    aefActinBond::SetHeadADPMultiplier(factor);
}

// Command handler function to set the multiplier for an ADP
// bound actin monomer to detach from a filament via its (the bond's) tail.
// Note that we check that the new probability is positive semi-definite
// in the aefActinBond function.

void aefActinNetwork::SetTailADPMultiplier(double factor)
{
    aefActinBond::SetTailADPMultiplier(factor);
}

// Command handler function to display fActin monomers according to their bound
// ATP status. This applies to both free and bound monomers. We store the new
// values (if they take allowed values) for re-use before calling the refresh 
// function. We then set a flag to indicate if their status should be refreshed
// continually. Only if all display ids are -1 (indicating that beads are 
// all coloured according to their own type) do we disable the update.

void aefActinNetwork::ColourBondsByATPState(long ATPId, long ADPPiId, long ADPId)
{
    if(ATPId >= -1)
    {
        m_ATPDisplayId = ATPId;
    }

    if(ADPPiId >= -1)
    {
        m_ADPPiDisplayId = ADPPiId;
    }

    if(ADPId >= -1)
    {
        m_ADPDisplayId = ADPId;
    }

    UpdateBondDisplayStatus();

    if(m_ATPDisplayId == -1 && m_ADPPiDisplayId == -1 && m_ADPDisplayId == -1)
    {
        m_bRefreshDisplayStatus = false;
    }
    else
    {
        m_bRefreshDisplayStatus = true;
    }
}

// Function to update the display status of all bonds in this network.
// This refreshes the status of bonds that may need to be altered as a 
// result of commands that modify their display status depending on their 
// internal state.

void aefActinNetwork::UpdateBondDisplayStatus()
{
    // Modify bonds that are bound into filaments first: these are the ones we
    // are most interested in colouring to show the distribution of ATP states
    // in filaments.

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
        ActiveBondList lBonds = (*iterPoly)->GetBonds();

        for(ActiveBondListIterator iterBond = lBonds.begin(); iterBond != lBonds.end(); iterBond++)
	    {
		    aefActinBond* pBond = dynamic_cast<aefActinBond*>(*iterBond);

            if(pBond->IsATPBound()) // Bond has bound ATP
            {
                pBond->GetTailMonomer()->SetDisplayId(m_ATPDisplayId);
            }
            else if(pBond->IsPiBound()) // Bond has bound ADP-Pi
            {
                pBond->GetTailMonomer()->SetDisplayId(m_ADPPiDisplayId);
            }
            else  // Bond has bound ADP
            {
                pBond->GetTailMonomer()->SetDisplayId(m_ADPDisplayId);
            }
        }
	}

    // Now modify the free bonds
    
    for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aefActinBond* pBond = dynamic_cast<aefActinBond*>(*iterBond);

        if(pBond->IsATPBound())
        {
            pBond->GetTailMonomer()->SetDisplayId(m_ATPDisplayId);
        }
        else if(pBond->IsPiBound())
        {
            pBond->GetTailMonomer()->SetDisplayId(m_ADPPiDisplayId);
        }
        else
        {
            pBond->GetTailMonomer()->SetDisplayId(m_ADPDisplayId);
        }
	}

}

