/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeCofilinNetwork.cpp: implementation of the aeCofilinNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "aeCofilinNetwork.h"
#include "aeCNTCell.h"
#include "aeCofilinBond.h"
#include "aeActivePolymer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aeCofilinNetwork::aeCofilinNetwork(IActiveSimBox* const pShadow, long xn, long yn, long zn,
								   double xw, double yw, double zw) : aeActiveCellNetwork(pShadow, xn, yn, zn, xw, yw, zw)
{

}

aeCofilinNetwork::~aeCofilinNetwork()
{

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

void aeCofilinNetwork::Evolve()
{
	// Sweep over all active bonds updating their positions

	UpdateActiveBondsInCells();

	// Iterate over all free active polymers to calculate the forces that bind
	// the monomers into polymers. These forces are internal to the polymers and 
	// do not affect free monomers or other polymers.

	for(ActivePolymerListIterator iterPoly=m_FreeActivePolymers.begin(); iterPoly!=m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActivePolymer* const pPolymer = *iterPoly;

		if(pPolymer->GetSize() > 2)
		{
			std::cout << "id = " << pPolymer->GetId() << " polymer size = " << pPolymer->GetSize() << zEndl;
		}

		(*iterPoly)->AddForce();
	}

	// Attempt to nucleate new active polymers

	Nucleate();

	// Allow existing active polymers to grow and shrink by gain or addition 
	// of monomers at each end

	Polymerise();

}

// Function that attempts to construct active polymers out of two active bonds that
// satisfy certain conditions. The specific conditions for polymer nucleation
// depend on the active bond type and are specified in the virtual function
// aeActiveBond::Activate().
//
// We iterate over all free active bonds, obtain the cell containing the bond and
// check that cell and its nearest neighbours to see if two bonds can nucleate
// a new polymer. If nucleation succeeds, we remove both bonds from the free active
// bond container and add the new polymer to the free active polymer container.

void aeCofilinNetwork::Nucleate()
{
	ActiveBondListIterator iterBond=m_FreeActiveBonds.begin();

	while(iterBond != m_FreeActiveBonds.end())
	{
		aeCNTCell* const pCell = GetCellFromBond(*iterBond);

		aeActiveBond* pTargetBond = pCell->UpdateNucleation(*iterBond);

		if(pTargetBond)
		{
			// Note the order of the arguments: new polymer's head bond then its tail bond

			aeActivePolymer* pPolymer = new aeActivePolymer(pTargetBond, *iterBond);

			// and add the newly-created polymer to the free polymer container,
			// and remove the bond from the free bond container

			m_FreeActivePolymers.push_back(pPolymer);

			// Now remove the target bond from the free bonds container

			RemoveFreeBond(pTargetBond);
			iterBond = RemoveFreeBondByIterator(iterBond);

		}
		else
		{
			iterBond++;
		}
	} 
}

void aeCofilinNetwork::Polymerise()
{
	// Check to see if any active polymer can grow by adding a free active bond
	// at either end.

	for(ActivePolymerListIterator iterAP=m_FreeActivePolymers.begin(); iterAP!=m_FreeActivePolymers.end(); iterAP++)
	{
		aeActivePolymer* const pPolymer  = *iterAP;
		aeActiveBond*    const pHeadBond = pPolymer->GetHeadBond();
		aeActiveBond*    const pTailBond = pPolymer->GetTailBond();

		// We only allow growth at the polymer's tail or head.

		aeCNTCell* const pHeadCell = GetCellFromBond(pHeadBond);

		aeActiveBond* pHeadTargetBond = pHeadCell->UpdateNucleation(pHeadBond);

		// If there is a free monomer nearby the polymer grows, otherwise we
		// see if it can shrink. Note that the bonds are linked together for
		// navigation purposes within the Activate() function.

		if(pHeadTargetBond)
		{
			pPolymer->AddHeadBond(pHeadTargetBond);

			RemoveFreeBond(pHeadTargetBond);
		}

		// Now see if the polymer can grow or shrink at its tail end

		aeCNTCell* const pTailCell = GetCellFromBond(pTailBond);

		aeActiveBond* pTailTargetBond = pTailCell->UpdateNucleation(pTailBond);

		if(pTailTargetBond)
		{
			pPolymer->AddTailBond(pTailTargetBond);

			RemoveFreeBond(pTailTargetBond);
		}

	}
}
