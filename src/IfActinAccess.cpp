/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// IfActinAccess.cpp: implementation of the IfActinAccess class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "IfActinAccess.h"
#include "aefActinNetwork.h"
#include "aeActiveBond.h"
#include "aeBondExternalTriggerOn.h"
#include "aeBondExternalTriggerOff.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IfActinAccess::IfActinAccess(aefActinNetwork* const pNetwork) : m_pNetwork(pNetwork)
{

}

IfActinAccess::~IfActinAccess()
{

}

// ****************************************
// Functions belonging to the interface used by the Arp23 network.




// ****************************************
// Functions belonging to the interface used by the Formin network.

// Function to return the mActin monomer closest to the point passed in. 
// We use PBC coordinates for the distance check, so the passed-in point 
// must be specified using the same coordinate system. We apply tests to use
// the minimum distance if the two monomers are in different periodic images.
// We cannot use the unPBC coordinates as the monomers may be far apart in
// the unPBC space.
//
// We search all possible mActin monomers for now: it would be more efficient
// just to search the aeCNTCells surrounding the passed in point. If found,
// the returned monomer is NOT removed from the network's free monomer container,
// this is left to the calling routine that should use RemoveNearbyMonomer() to 
// do this.
//
// Note that it is possible that no monomer is found so the calling routine 
// must check for a returned NULL pointer.

aeActiveBond* IfActinAccess::GetNearbyMonomer(double x, double y, double z)
{
	aeActiveBond* pNearbyBond = 0;

	double minSquareSep = -1.0;
	double sqSep        = 0.0;

	const double SimBoxXLength = m_pNetwork->GetIActiveSimBox()->GetSimBoxXLength();
	const double SimBoxYLength = m_pNetwork->GetIActiveSimBox()->GetSimBoxYLength();
	const double SimBoxZLength = m_pNetwork->GetIActiveSimBox()->GetSimBoxZLength();

	const double HalfSimBoxXLength = m_pNetwork->GetIActiveSimBox()->GetHalfSimBoxXLength();
	const double HalfSimBoxYLength = m_pNetwork->GetIActiveSimBox()->GetHalfSimBoxYLength();
	const double HalfSimBoxZLength = m_pNetwork->GetIActiveSimBox()->GetHalfSimBoxZLength();

	ActiveBondListIterator iterBond=m_pNetwork->m_FreeActiveBonds.begin();

	while(iterBond != m_pNetwork->m_FreeActiveBonds.end())
	{
		CAbstractBead* pTail = (*iterBond)->GetTailTailBead();

		double dx = pTail->GetXPos() - x;
		double dy = pTail->GetYPos() - y;
		double dz = pTail->GetZPos() - z;

		if( dx > HalfSimBoxXLength )
			dx = dx - SimBoxXLength;
		else if( dx < -HalfSimBoxXLength )
			dx = dx + SimBoxXLength;

		if( dy > HalfSimBoxYLength )
			dy = dy - SimBoxYLength;
		else if( dy < -HalfSimBoxYLength )
			dy = dy + SimBoxYLength;

#if SimDimension == 3
		if( dz > HalfSimBoxZLength )
			dz = dz - SimBoxZLength;
		else if( dz < -HalfSimBoxZLength )
			dz = dz + SimBoxZLength;
#else
		dz = 0.0;
#endif

		sqSep = dx*dx + dy*dy + dz*dz;

		if(sqSep < minSquareSep || minSquareSep < 0.0)
		{
//		    std::cout << "Found  fActin monomer with id " << (*iterBond)->GetId() << " near " << x << " " << y << " " << z << " with separation " << sqSep << zEndl;
			minSquareSep = sqSep;
			pNearbyBond = *iterBond;
		}

		iterBond++;
	} 

	return pNearbyBond;
}

// Function to allow the calling network to remove a monomer from this network's
// free monomer container. It is used in conjunction with GetNearbyMonomer()
// to allow the calling network to locate and bind monomers in this network.

void IfActinAccess::RemoveNearbyMonomer(aeActiveBond* const pBond)
{
	if(pBond)
	{
		m_pNetwork->RemoveFreeBond(pBond);
	}
}


// Function to replace an aefActin bond's current on condition, which determines 
// when the bond binds to another active bond, with a condition that is controlled by
// an external trigger. This is used by other ACNs to bind together two bonds
// from different networks. The original on condition can be restored using the
// RestorePreviousOnCondition().
//
// This version uses an instance of aeBondExternalTriggerOn to control the binding.

void IfActinAccess::SetBondExternalTriggerOn(aeActiveBond* pBond)
{
	aeBondExternalTriggerOn* const pTriggerOn = new aeBondExternalTriggerOn();

	pBond->ReplaceOnCondition(pTriggerOn);
}

// Function to allow a different network to add a released mActin monomer back 
// into the fActin network's free bond container. The calling function must ensure
// that the bond has been properly disconnected from other bonds in its previous
// filament. Note that we do not check for the actual type of the bond instance:
// this means the calling object must ensure it is passing an fActinBond instance.
// This allows us in the future to mix bond types in a network.

void IfActinAccess::AddFreeBond(aeActiveBond* const pBond)
{
	m_pNetwork->AddFreeBond(pBond);
}

// Function to allow a different network to add a released actin filament back 
// into the fActin network's free polymer container. Note that the calling 
// function is responsible for creating the polymer instance that is passed in 
// here and subsequently owned by the fActin network.

void IfActinAccess::AddFreePolymer(aeActivePolymer* const pPoly)
{
	m_pNetwork->AddFreePolymer(pPoly);
}
