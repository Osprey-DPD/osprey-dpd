/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctPolymerisePolymerTripleInTargetImpl.cpp: implementation of the ctPolymerisePolymerTripleInTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctPolymerisePolymerTripleInTargetImpl.h"
#include "ctPolymerisePolymerTripleInTarget.h"
#include "SimBox.h"
#include "IModifySimStateBondTypes.h"
#include "Bead.h"
#include "Bond.h"
#include "Polymer.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "LogctPolymerisePolymerTripleInTarget.h"

#include "CNTCell.h"  // Needed for the RNG

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctPolymerisePolymerTripleInTargetImpl::ctPolymerisePolymerTripleInTargetImpl()
{
}

ctPolymerisePolymerTripleInTargetImpl::~ctPolymerisePolymerTripleInTargetImpl()
{

}

// Command handler to create dynamic bonds between all polymers in a command target
// so that they move as one quasi-rigid object.
//

void ctPolymerisePolymerTripleInTargetImpl::PolymerisePolymerTripleInTarget(const xxCommand* const pCommand)
{
	const ctPolymerisePolymerTripleInTarget* const pCmd = dynamic_cast<const ctPolymerisePolymerTripleInTarget*>(pCommand);

	// Get a pointer to the CSimBox so that we can access its bead data

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Unpack the data identifying the bond type that is changed by the command 
	// and the new values of its spring constant and unstretched length.
	// If the simulation type is MD we normalize the spring constant by the
	// energy scale taken from the first species' LJ potential depth, and the 
	// lengths by the first LJ potential range.

	const zString label				= pCmd->GetTargetLabel();
	const zString polyName			= pCmd->GetPolymerName();
	const zLongVector vHeadPos		= pCmd->GetHeadPositions();
	const zLongVector vTailPos		= pCmd->GetTailPositions();
	const double springConstant		= pCmd->GetSpringConstant();
	const double unStretchedLength	= pCmd->GetUnstretchedLength();

	const long polyType	= pSimBox->GetPolymerTypeFromName(polyName);


	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(label);

	// Now we have the target, check it is not composite, get the 
	// polymers contained in it and from them obtain the specified bonds;
	// if no bonds are found issue an error message.

	if(pCmdTarget && !pCmdTarget->IsComposite() && pCmdTarget->GetPolymerTotalForType(polyType) > 3)
	{
		PolymerVector vTargetPolymers = pCmdTarget->GetPolymers();

		// Find a polymer to act as the centre for the polymerisation.
		// We assume the command target has a small enough geometric extent
		// so that the bonds are not stretched too far from their unstretched 
		// value. If the user has specified a large target this may cause problems.

		// Get three random polymers in the target

		long firstIndex  = static_cast<long>(CCNTCell::GetRandomNo()*vTargetPolymers.size());
		long secondIndex = static_cast<long>(CCNTCell::GetRandomNo()*vTargetPolymers.size());
		long thirdIndex  = static_cast<long>(CCNTCell::GetRandomNo()*vTargetPolymers.size());

		// Check we don't get 1 from the RNG. This should be rare enough that
		// can apply a deterministic adjustment.

		if(firstIndex == vTargetPolymers.size())
			firstIndex--;
		if(secondIndex == vTargetPolymers.size())
			secondIndex--;
		if(thirdIndex == vTargetPolymers.size())
			thirdIndex--;

		CPolymer* const firstPoly  = vTargetPolymers.at(firstIndex);
		CPolymer* const secondPoly = vTargetPolymers.at(secondIndex);
		CPolymer* const thirdPoly  = vTargetPolymers.at(thirdIndex);

		std::cout << "Polymerising polymers " << firstPoly->GetId() << " " << secondPoly->GetId() << " " << thirdPoly->GetId() << zEndl;


		// Create a template bond for use in creating all polymerised bonds.
		// We get the current number of bond types from the CInitialState
		// object and create a new type to be used as a template for all
		// polymerised bonds. The template is stored in the CInitialState
		// just as the normal bond types using the interface class
		// IModifySimStateBondTypes.

		long polyBondCounter    = pSimBox->GetSimBoxBonds().size();		// used to assign bond ids
		const long polyBondType = pSimBox->GetBondTypeTotal();	// used to create new bond type

		CBond* pPolyBondTemplate = 0;

//		riState.SetPolymerisedState(true);

		long headSize = vHeadPos.size();
		long tailSize = vTailPos.size();

		std::cout << headSize << "  " << tailSize << zEndl;


#if SimIdentifier == MD
		springConstant		*= (pSimBox->GetLengthScale()*pSimBox->GetLengthScale()/pSimBox->GetEnergyScale());
		unStretchedLength   /= pSimBox->GetLengthScale();
#endif

		pPolyBondTemplate = new CBond(polyBondType, springConstant, unStretchedLength);

		// Store the new template bond for later use

		pSimBox->IModifyBondTypes()->AddNewType(pPolyBondTemplate);

		BondVector vPolyBonds;
		vPolyBonds.clear();

		for(long bp=0; bp<vHeadPos.size(); bp++)
		{
			CBead* pHead1 = firstPoly->GetBeads().at(vHeadPos.at(bp));
			CBead* pTail1 = secondPoly->GetBeads().at(vTailPos.at(bp));

			std::cout << "First bond beads " << pHead1->GetId() << " " << pTail1->GetId() << zEndl;


			CBead* pHead2 = secondPoly->GetBeads().at(vHeadPos.at(bp));
			CBead* pTail2 = thirdPoly->GetBeads().at(vTailPos.at(bp));

			std::cout << "Second bond beads " << pHead2->GetId() << " " << pTail2->GetId() << zEndl;

			CBead* pHead3 = thirdPoly->GetBeads().at(vHeadPos.at(bp));
			CBead* pTail3 = firstPoly->GetBeads().at(vTailPos.at(bp));

			std::cout << "Third bond beads " << pHead3->GetId() << " " << pTail3->GetId() << zEndl;

			
			CBond* pBond1 = new CBond(*pPolyBondTemplate);
			CBond* pBond2 = new CBond(*pPolyBondTemplate);
			CBond* pBond3 = new CBond(*pPolyBondTemplate);

			pBond1->SetId(polyBondCounter++);
			pBond1->SetBeads(pHead1, pTail1);
			pBond1->SetBeadIndex(pHead1->GetId(), pTail1->GetId());

			pBond2->SetId(polyBondCounter++);
			pBond2->SetBeads(pHead2, pTail2);
			pBond2->SetBeadIndex(pHead2->GetId(), pTail2->GetId());

			pBond3->SetId(polyBondCounter++);
			pBond3->SetBeads(pHead3, pTail3);
			pBond3->SetBeadIndex(pHead3->GetId(), pTail3->GetId());

			vPolyBonds.push_back(pBond1);
			vPolyBonds.push_back(pBond2);
			vPolyBonds.push_back(pBond3);
		}

		for(BondVectorIterator iterBond=vPolyBonds.begin(); iterBond!=vPolyBonds.end(); iterBond++)
		{
			pSimBox->IModifyBondTypes()->AddPolymerisedBond(*iterBond);
		}

		// Recalculate how many bonds of each type exist and tell the CMonitor
		// to update its local counters

		pSimBox->IModifyBondTypes()->CalculateTypeTotals();

		pSimBox->UpdateBondTypes();

		std::cout << vPolyBonds.size() << " polymerised bonds created" << zEndl;


		// Create a message indicating that the bond parameters have changed

		CLogctPolymerisePolymerTripleInTarget* pMsg = new CLogctPolymerisePolymerTripleInTarget(pSimBox->GetCurrentTime(), label, polyName, polyType, vHeadPos.size(), springConstant, unStretchedLength);
	}
	else	// Log a command failed message
	{
		CLogCommandFailed* pMsg = new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
