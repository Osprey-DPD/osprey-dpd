/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeActiveBond.cpp: implementation of the aeActiveBond class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "aeActiveBond.h"
#include "Bead.h"

#include "aeBondConnection.h"
#include "aeBondOnCondition.h"
#include "aeBondOffCondition.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// An active bond "wraps" a monomer (that is actually a CPolymer) and 
// searches for another monomer to complete the bond. The tail of the bond
// is fixed on creation but the head is only stored once the bond-on condition
// has been satisfied. The head of the bond can be lost if the bond-off
// condition is satisfied, in which case the bond reverts to being a free
// bond with just the tail monomer specified. The id of the active bond is
// taken from the id of its tail polymer.
//
// Note that the beads specified as the Head and Tail of the CPolymer type 
// from which the active bonds are created are used to generate the force 
// applied by the bond. A CBond instance is created to connect these two 
// beads so that we can reuse the calculation in CBond. We create a new bond
// here instead of using one in the original CPolymer in case the head and tail
// beads in the polymer type are not connected to each other.
//
// The CBond instances, and CBondpair instance, do not change throughout the 
// lifetime of the active bond, only the bead that is stored as the tail of 
// the linker bond changes when a new head monomer is added to the active bond.

aeActiveBond::aeActiveBond(CPolymer* const pTail) : m_bPolymerised(false),
													m_pHeadAdjacentBond(0),
													m_pTailAdjacentBond(0),
													m_pConnector(0),
												    m_pOn(0), m_pOff(0),
													m_pOldConnector(0),
													m_pOldOn(0), m_pOldOff(0)
{

}

// Constructor for use when specifying the bond on and off conditions.

aeActiveBond::aeActiveBond(CPolymer* const pTail, aeBondOnCondition* pOn, 
							aeBondOffCondition* pOff) : m_bPolymerised(false),
														m_pHeadAdjacentBond(0),
														m_pTailAdjacentBond(0),
														m_pConnector(0),
														m_pOn(pOn), m_pOff(pOff),
														m_pOldConnector(0),
														m_pOldOn(0), m_pOldOff(0)
{

}

// Constructor for use when specifying the bond on and off conditions and the
// type of bond connection.

aeActiveBond::aeActiveBond(CPolymer* const pTail, aeBondConnection* pConnector, 
						   aeBondOnCondition* pOn, aeBondOffCondition* pOff) : m_bPolymerised(false),
														m_pHeadAdjacentBond(0),
														m_pTailAdjacentBond(0),
														m_pConnector(pConnector),
														m_pOn(pOn), m_pOff(pOff),
														m_pOldConnector(0),
														m_pOldOn(0), m_pOldOff(0)
{

}


aeActiveBond::~aeActiveBond()
{
	// Delete the linker object binding two active bonds, and its previous instance

	if(m_pConnector)
	{
		delete m_pConnector;
	}

	if(m_pOldConnector)
	{
		delete m_pOldConnector;
	}

	// Delete the bond on and off conditions, and their previous values

	if(m_pOn)
	{
		delete m_pOn;
	}

	if(m_pOff)
	{
		delete m_pOff;
	}

	if(m_pOldOn)
	{
		delete m_pOldOn;
	}

	if(m_pOldOff)
	{
		delete m_pOldOff;
	}

}

// Function to return a pointer to the active bond attached to the bond's head.

aeActiveBond* aeActiveBond::GetHeadAdjacentBond() const
{
	return m_pHeadAdjacentBond;
}

// Function to return a pointer to the active bond attached to the bond's tail.

aeActiveBond* aeActiveBond::GetTailAdjacentBond() const
{
	return m_pTailAdjacentBond;
}

// Functions to return the monomers making up this active bond. The tail monomer
// always exists as the active bond is wrapped around it; the head monomer
// is added when the bond joins a filament; and the fuel monomer is included to
// allow for extensions to the binding mechanism that may include energy consumption.

CPolymer* aeActiveBond::GetHeadMonomer() const
{
	return m_pConnector->GetHeadMonomer();
}

CPolymer* aeActiveBond::GetTailMonomer() const
{
	return m_pConnector->GetTailMonomer();
}

CPolymer* aeActiveBond::GetFuelMonomer() const
{
	return m_pConnector->GetFuelMonomer();
}

// Functions to return the head and tail beads of the active bond's head monomer.
// We check if the monomer exists before attempting to reference it. We don't 
// have to do this for the bond's tail monomer because it always exists.

CAbstractBead* aeActiveBond::GetHeadHeadBead() const
{
	if(GetHeadMonomer())
	{
		return GetHeadMonomer()->GetHead();
	}
	else
		return 0;
}

CAbstractBead* aeActiveBond::GetHeadTailBead() const
{
	if(GetHeadMonomer())
	{
		return GetHeadMonomer()->GetTail();
	}
	else
		return 0;
}

void aeActiveBond::SetPolymerised()
{
	m_bPolymerised = true;
}

void aeActiveBond::SetDepolymerised()
{
	m_bPolymerised = false;
}

// Function to return the aeBondConnection instance so that other bonds can
// connect to it.

aeBondConnection* aeActiveBond::GetBondConnection() const
{
	return m_pConnector;
}

// ****************************************
// Protected function to calculate the force exerted between two adjacent active bonds
// in an active polymer. The force law is determined by the type of linker instance 
// that connects two bonds. Such instances are derived from aeBondConnection.
// 
// Note that the forces are only added if the active bond connects two monomers.
// The last bond in an active polymer has only a tail monomer: it contributes
// to the bending stiffness of the bondpair linking it to its predecessor active
// bond but not to the bonds linking beads.
//
// All forces except the two-body force calculated in AddForce() are optional. 
// Classes derived from aeBondConnection may implement them if desired but a do-nothing
// implementation is provided in the base class.

void aeActiveBond::AddForce()
{
	if(m_pConnector)
	{
		m_pConnector->AddForce();
		m_pConnector->AddThreeBodyForce();
	}
}

// Functions used by derived classes to see if their bond-on condition
// is satisfied so that they can add a new bond to a polymer. Note that
// the bond passed as pTargetBond is the free bond and the bond receiving 
// this message is the bound one.

bool aeActiveBond::HeadOnCondition(const aeActiveBond* const pTargetBond) const
{
	return m_pOn->ActivateHead(this, pTargetBond);
}

// The second function checks to see if the free target bond can bind to the
// polymerised bond at its tail.

bool aeActiveBond::TailOnCondition(const aeActiveBond* const pTargetBond) const
{
	return m_pOn->ActivateTail(this, pTargetBond);
}

// Functions used by derived classes to see if their bond-off condition
// is satisfied at the head and tail ends of the current bond. Note that
// the bond which receives this message is the end bond of the polymer.
//
// The first function checks to see if the current bond can separate from the
// adjacent bond at its head. This means the current bond is at the "tail" of 
// the polymer.

bool aeActiveBond::HeadOffCondition() const
{
	return m_pOff->ActivateHead(this);
}

// The second function checks to see if the current bond can separate from the
// adjacent bond at its tail. This means the current bond is at the "head" of 
// the polymer.

bool aeActiveBond::TailOffCondition() const
{
	return m_pOff->ActivateTail(this);
}

// Function to add a new bond adjacent to the current bond's head. 
//
// We check that the current bond is not already connected at its head and that
// it has a connection instance; if either test fails, we return false.
// We connect the new bond to the current bond by storing its tail monomer as 
// the "head" monomer of the current bond, and also connect the current bond to 
// the new bond's tail.
//
// Note that the conditions for binding monomers together are bond-specific.

bool aeActiveBond::AddHeadAdjacentBond(aeActiveBond* const pTargetBond)
{
	if(!GetHeadAdjacentBond() && m_pConnector)
	{
		SetPolymerised();
		m_pHeadAdjacentBond = pTargetBond;

		pTargetBond->SetPolymerised();
		pTargetBond->m_pTailAdjacentBond = this;

		// Now tell the connection instance to update its force law. We do this here
		// so that it can chain along the connected monomers that have just been set up.

		return m_pConnector->AddHeadMonomer(pTargetBond);
	}

	return false;
}

// Function to remove the bond attached to the current bond's head. It assumes that
// the head is a single active bond. See the SeparateHeadAdjacentBond() function
// if this is not the case. This function should only be called if the polymer 
// still has at least two bonds after the head is removed.

bool aeActiveBond::RemoveHeadAdjacentBond()
{
	aeActiveBond* const pHAB = GetHeadAdjacentBond();

	// We pass the head-adjacent bond into the connector so that it can update 
	// the adjacent bonds' three-body forces if necessary

	if(pHAB && m_pConnector && m_pConnector->RemoveHeadMonomer(pHAB))
	{
		m_pHeadAdjacentBond       = 0; 
		pHAB->m_pTailAdjacentBond = 0;
		pHAB->SetDepolymerised();
		return true;
	}
	else
		return false;
}

// Function to disconnect the bond receiving this message from its head adjacent 
// neighbour. This splits the containing active polymer into two parts. The difference
// from the RemoveHeadAdjacentBond() function is that this function does not assume 
// the head adjacent bond is a single bond, so it does not call its SetDepolymerised()
// function. Because this function does not return any information about the bonds
// being split, the calling routine must store the split bonds for potential resue.
//
// This function should only be called for bonds already in an active polymer and
// not at the head of the polymer. It ignores any attempt to separate a bond that 
// has no head adjacent bond  and returns false to indicate a failure.

bool aeActiveBond::SeparateHeadAdjacentBond()
{
	aeActiveBond* const pHAB = GetHeadAdjacentBond();

	if(pHAB && m_pConnector && m_pConnector->RemoveHeadMonomer(pHAB))
	{
		m_pHeadAdjacentBond       = 0; 
		pHAB->m_pTailAdjacentBond = 0;
		return true;
	}
	else
		return false;
}


// Function to add a new bond adjacent to the current bond's tail. 
// We check that the current bond is not already connected and 
// simply call the potential target bond's AddHeadAdjacentBond().

bool aeActiveBond::AddTailAdjacentBond(aeActiveBond* const pTargetBond)
{
	if(!GetTailAdjacentBond() && pTargetBond->AddHeadAdjacentBond(this))
	{
		return true;
	}

	return false;
}

// Function to remove the bond attached to the current bond's tail. Note that
// we cannot just call RemoveHeadAdjacentBond() on the adjacent tail bond, as the
// remainder of the active polymer need not be a single active bond.
// This function should only be called if the polymer still has at least two
// bonds after the tail is removed.

bool aeActiveBond::RemoveTailAdjacentBond()
{
	aeActiveBond* const pTAB = GetTailAdjacentBond();

	if(pTAB && pTAB->m_pConnector && pTAB->m_pConnector->RemoveHeadMonomer(this))
	{
		m_pTailAdjacentBond       = 0;
		pTAB->m_pHeadAdjacentBond = 0;
		pTAB->SetDepolymerised();
		return true;
	}
	else
		return false;
}

// Function called to detach the two bonds contained in an active polymer
// that has only two bonds by removing the tail bond from the current bond.
// The current bond must be the head of the polymer. This results in the 
// polymer's dissolution and both bonds have their Depolymerise() functions called.

void aeActiveBond::DissolveFromHead()
{
	// Disconnect the head bond from the tail bond and vice versa

	GetTailAdjacentBond()->SetDepolymerised();
	GetTailAdjacentBond()->m_pConnector->RemoveHeadMonomer(this);
	GetTailAdjacentBond()->m_pHeadAdjacentBond = 0;

	SetDepolymerised();
	m_pTailAdjacentBond = 0;
}

// Protected helper functions to count the number of bonds between the current bond
// and the head/tail end of the filament containing it. It simply calls the same 
// function on each adjcaent bond until it reaches the end of the filament and 
// then unwinds.

long aeActiveBond::CountHeadAdjacentBonds() const
{
	if(GetHeadAdjacentBond())
	{
		return GetHeadAdjacentBond()->CountHeadAdjacentBonds() + 1;
	}
	else
	{
		return 0;
	}
}

long aeActiveBond::CountTailAdjacentBonds() const
{
	if(GetTailAdjacentBond())
	{
		return GetTailAdjacentBond()->CountTailAdjacentBonds() + 1;
	}
	else
	{
		return 0;
	}
}

// Function to replace the specific connection between active bonds in a filament.
// The previous connection instance is stored so that is can be restored later if 
// required. Note that the calling routine allocates the memory for the connection, 
// but it is dellocated here or in this class' destructor.

void aeActiveBond::ReplaceConnection(aeBondConnection* const pNewConnector)
{
	if(m_pOldConnector)
	{
		delete m_pOldConnector;
	}

	m_pOldConnector = m_pConnector;
	m_pConnector    = pNewConnector;
}

// Function to restore the previously-stored connection object for active bonds
// bound into a filament. If a previous condition exists, we replace the current one
// with the stored value; if it does not exist, we do nothing. Note that no memory
// is allocated or dellocated in these routines.

void aeActiveBond::RestorePreviousConnection()
{
	if(m_pOldConnector)
	{
		aeBondConnection* const pMiddle = m_pConnector;
		m_pConnector    = m_pOldConnector;
		m_pOldConnector = pMiddle;
	}
}

// Functions to replace the conditions that determine when an active bond
// binds/unbinds to another one. The previous conditions are stored so that
// they can be restored later if required. Note that the calling routine allocates
// the memory for the on/off conditions, but it is dellocated here or in this class'
// destructor.

void aeActiveBond::ReplaceOnCondition(aeBondOnCondition* const pNewOn)
{
	if(m_pOldOn)
	{
		delete m_pOldOn;
	}

	m_pOldOn = m_pOn;
	m_pOn    = pNewOn;
}

void aeActiveBond::ReplaceOffCondition(aeBondOffCondition* const pNewOff)
{
	if(m_pOldOff)
	{
		delete m_pOldOff;
	}

	m_pOldOff = m_pOff;
	m_pOff    = pNewOff;
}

// Function to restore the previously-stored conditions for binding/unbinding two
// active bonds. If a previous condition exists, we replace the current one
// with the stored value; if it does not exist, we do nothing. Note that no memory
// is allocated or dellocated in these routines.

void aeActiveBond::RestorePreviousOnCondition()
{
	if(m_pOldOn)
	{
		aeBondOnCondition* const pMiddle = m_pOn;
		m_pOn    = m_pOldOn;
		m_pOldOn = pMiddle;
	}
}

void aeActiveBond::RestorePreviousOffCondition()
{
	if(m_pOldOff)
	{
		aeBondOffCondition* const pMiddle = m_pOff;
		m_pOff    = m_pOldOff;
		m_pOldOff = pMiddle;
	}
}

// Functions to make the bond visible/invisible in current state snapshots.
// We loop over all beads in the bond's head, tail and fuel polymers
// checking that the head and fuel actually exist first. The tail must
// exist for the bond to be valid. The first version takes an argument showing
// the visibility status.

void aeActiveBond::SetVisible(bool bVisible)
{
	GetTailMonomer()->SetVisible(bVisible);

	if(GetHeadMonomer())
	{
		GetHeadMonomer()->SetVisible(bVisible);
	}

	if(GetFuelMonomer())
	{
		GetFuelMonomer()->SetVisible(bVisible);
	}
}

void aeActiveBond::SetVisible()
{
	GetTailMonomer()->SetVisible();

	if(GetHeadMonomer())
	{
		GetHeadMonomer()->SetVisible();
	}

	if(GetFuelMonomer())
	{
		GetFuelMonomer()->SetVisible();
	}
}

void aeActiveBond::SetInvisible()
{
	GetTailMonomer()->SetInvisible();

	if(GetHeadMonomer())
	{
		GetHeadMonomer()->SetInvisible();
	}

	if(GetFuelMonomer())
	{
		GetFuelMonomer()->SetInvisible();
	}
}

// Function to return the CM of all beads composing the active bond.
// Note that we only use the bond's tail monomer here as unpolymerised
// bonds do not have a head monomer.

aaVector aeActiveBond::GetCM() const
{
    CPolymer* const pTail = GetTailMonomer();

    const BeadVector vBeads = pTail->GetBeads();
    const double beadTotal  = static_cast<double>(vBeads.size());

    double xcm = 0.0;
    double ycm = 0.0;
    double zcm = 0.0;

   for(cBeadVectorIterator iterBead = vBeads.begin(); iterBead != vBeads.end(); iterBead++)
    {
        const double x = (*iterBead)->GetXPos();
        const double y = (*iterBead)->GetYPos();
        const double z = (*iterBead)->GetZPos();

        xcm += x;
        ycm += y;
        zcm += z;
    }

    xcm /= beadTotal;
    ycm /= beadTotal;
    zcm /= beadTotal;

    aaVector cm(xcm, ycm, zcm);

    return cm;
}

// Function to toggle the state of the active bond between its active and
// phantom states. We forward the function to the contained aeBondOnCondition
// instance where the state is stored. Note that the aeBondOffCondition does
// not take part in this process as any active bond in a polymer cannot be
// a phantom.

void aeActiveBond::SetPhantom(bool bPhantom)
{
    m_pOn->SetPhantom(bPhantom);
}

// Function to return the active/phantom state of the bond.

bool aeActiveBond::IsPhantom() const
{
    return m_pOn->IsPhantom();
}
