/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctPolymerisePolymersInTargetImpl.cpp: implementation of the ctPolymerisePolymersInTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ctPolymerisePolymersInTargetImpl.h"
#include "ctPolymerisePolymersInTarget.h"
#include "SimBox.h"
#include "IModifySimStateBondTypes.h"
#include "Bead.h"
#include "Bond.h"
#include "Polymer.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "LogctPolymerisePolymersInTarget.h"
#include "LogTextMessage.h"

#include "CNTCell.h"	// Needed for the RNG

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctPolymerisePolymersInTargetImpl::ctPolymerisePolymersInTargetImpl()
{
}

ctPolymerisePolymersInTargetImpl::~ctPolymerisePolymersInTargetImpl()
{

}

// Command handler to create dynamic bonds between all polymers in a command target
// so that they move as one quasi-rigid object. Such dynamic bonds are always
// created in real space, i.e., the principal image of the SimBox, and never
// across the boundaries of the SimBox.
//
// The required parameters define the maximum number of bonds between any two
// polymers, the maximum separation for which two polymer will be bound,
// the fraction of bonds that are to be made between polymers and the Hookean 
// spring parameters. However, currently, the unstretched length of the bonds 
// is defined to be the instantaneous separation of the beads at the time each
// bond is created. This ensures that the rigid object stays in approximately
// the same shape as when it was polymerised (modulo the strength of the bonds).
//
// Important Note
// **************
// If this command is applied to polymers that consist of only a single bead each,
// its application is straightforward: the beads have their unPBC coordinates
// translated into the principal image of the SimBox and a new bond is created
// between them passing through real space, i.e., not necessarily the shortest
// distance is this would mean crossing the boundaries of the SimBox. This is done
// because CBond calculates the forces between bonded beads in a polymer using the
// unPBC coordinates for speed.
// If this command is applied to polymers with more than one bead in them, the 
// same translation is applied to ALL beads in the polymer even though only 
// the Head and Tail have dynamic bonds created between them. All the beads have 
// to be translated so that CBond's force algorithm works correctly.

void ctPolymerisePolymersInTargetImpl::PolymerisePolymersInTarget(const xxCommand* const pCommand)
{
	const ctPolymerisePolymersInTarget* const pCmd = dynamic_cast<const ctPolymerisePolymersInTarget*>(pCommand);

	const zString	targetLabel	= pCmd->GetTargetLabel();

	const long   maxBondsPerPolymer	= pCmd->GetMaxBondsPerPolymer();
	const double range			    = pCmd->GetRange();
	const double fraction			= pCmd->GetBondFraction();
	const double springConstant		= pCmd->GetSpringConstant();
	const double unStretchedLength  = pCmd->GetUnstretchedLength();

	// Derived data

	const double rangeSq = range*range; // used to avoid taking square roots


	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list and the force decorator whose
	// work is to be measured

	CCommandTargetNode*   pCmdTarget = pSimBox->GetCommandTarget(targetLabel);

	// Now we have the command target, we check that it has more than one polymer
	// in it and the bond fraction is not zero; then we collect all the head 
	// beads of the contained polymers and all the tail beads preparatory 
	// to binding them to each other (head-head and tail-tail).

	if(pCmdTarget && pCmdTarget->GetPolymerTotal() > 1 && fraction > 0.0)
	{
		const long polymerTotal = pCmdTarget->GetPolymerTotal();

		PolymerVector vPolymers = pCmdTarget->GetPolymers();

		const long polymerSize = vPolymers.at(0)->GetSize();
		const long maxBonds = polymerTotal*(polymerTotal - 1)/2;

		// Calculate the maximum number of pairwise bonds possible, remove all
		// those that violate the maximum separation or the max number of bonds
		// per polymer, and reduce the remainder
		// to the user-supplied fraction. Given the set of allowed polymer pairs,
		// create a new bond for each polymer head pair and use the instantaneous
		// separation for the unstretched length. Then repeat the process for the
		// polymer tail beads. Note that the separation is stored separately for 
		// the polymer head and tail beads.
		
		// Create a map of the polymers in the target accessed by their id, 
		// a map that holds the number of bonded neighbours, a map 
		// that links a pair of bonded polymers using a unique counter  
		// as the key and the second one as the value, and two
		// maps that hold the head and tail separations accessed by the first
		// polymer's id.

		LongPolymerMap  mPolymers;
		LongLongMap		mNeighbours;	// (polymer id, neighbours)

		zLongVector		vFirstPolymerInPair;	
		zLongVector		vSecondPolymerInPair;	
		zDoubleVector   vHeadSeparation;
		zDoubleVector   vTailSeparation;
		
        mPolymers.clear();
        mNeighbours.clear();
        vFirstPolymerInPair.clear();
        vSecondPolymerInPair.clear();
        vHeadSeparation.clear();
        vTailSeparation.clear();

		for(PolymerVectorIterator iterPoly=vPolymers.begin(); iterPoly!=vPolymers.end(); iterPoly++)
		{
			const long id1 = (*iterPoly)->GetId();

			mPolymers.insert(zPairLongPolymer(id1, *iterPoly));

			if(mNeighbours.find(id1) == mNeighbours.end()) // first polymer has no neighbours yet
			{
				mNeighbours.insert(zPairLL(id1,0));
			}

			PolymerVectorIterator nextPoly1 = iterPoly;

			for(PolymerVectorIterator iterPoly2=++nextPoly1; iterPoly2!=vPolymers.end(); iterPoly2++)
			{
				const long id2 = (*iterPoly2)->GetId();

				if(mNeighbours.find(id2) == mNeighbours.end())
				{
					mNeighbours.insert(zPairLL(id2,0));
				}

				// Check the separation of the polymer head beads. Note that
                // we use the PBC coordinates because the polymers may be in
                // different images of the SimBox and hence have an arbitrarily
                // large separation; if we use the unPBC coordinates to construct
                // the new bond it could generate huge forces. As we are only
                // using the magnitude of the bead separations, we don't have
                // to worry about the sign of the relative position vector.

				const double hdx = (*iterPoly)->GetHead()->GetXPos() - (*iterPoly2)->GetHead()->GetXPos();
				const double hdy = (*iterPoly)->GetHead()->GetYPos() - (*iterPoly2)->GetHead()->GetYPos();
				const double hdz = (*iterPoly)->GetHead()->GetZPos() - (*iterPoly2)->GetHead()->GetZPos();

				const double tdx = (*iterPoly)->GetTail()->GetXPos() - (*iterPoly2)->GetTail()->GetXPos();
				const double tdy = (*iterPoly)->GetTail()->GetYPos() - (*iterPoly2)->GetTail()->GetYPos();
				const double tdz = (*iterPoly)->GetTail()->GetZPos() - (*iterPoly2)->GetTail()->GetZPos();

				const double hrsq = hdx*hdx + hdy*hdy + hdz*hdz;
				const double trsq = tdx*tdx + tdy*tdy + tdz*tdz;

				// Ensure that both head and tail lie with the range and apply
				// the test for the bond fraction and max bonds per polymer

				long n1 = mNeighbours.find(id1)->second;
				long n2 = mNeighbours.find(id2)->second;

				if(hrsq < rangeSq && trsq < rangeSq 
					&& CCNTCell::GetRandomNo() < fraction 
				    && n1 < maxBondsPerPolymer
				    && n2 < maxBondsPerPolymer)
				{
					// Increment the number of neighbours for both polymers:
					// because we are using maps we have to first remove the
					// pairs and then reinsert them

					mNeighbours.erase(id1);
					mNeighbours.erase(id2);
					mNeighbours.insert(zPairLL(id1,++n1));
					mNeighbours.insert(zPairLL(id2,++n2));

					// Store the ids and separations for the pair of polymers
					// in this bond. The separation is the distance measured
                    // in the principal image of the SimBox, so we know that it
                    // cannot be larger than the width of the SimBox.
                    
					vFirstPolymerInPair.push_back(id1);
					vSecondPolymerInPair.push_back(id2);
					vHeadSeparation.push_back(sqrt(hrsq));
					vTailSeparation.push_back(sqrt(trsq));
				}
			}
		}

		// Write some log messages for debug purposes

		zString logText = targetLabel + " target contains " + pSimBox->ToString(polymerTotal) + " polymers";
		CLogTextMessage* pText1  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);
			
		logText = "polymers map contains  " + pSimBox->ToString(static_cast<long>(mPolymers.size())) + " elements";
		CLogTextMessage* pText2  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);

		logText = "neighbour map contains "         + pSimBox->ToString(static_cast<long>(mNeighbours.size())) + " elements";
		CLogTextMessage* pText3  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);

		logText = "first polymer vector contains "  + pSimBox->ToString(static_cast<long>(vFirstPolymerInPair.size())) + " pairs";
		CLogTextMessage* pText4  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);

		logText = "second polymer vector contains " + pSimBox->ToString(static_cast<long>(vSecondPolymerInPair.size())) + " pairs";
		CLogTextMessage* pText5  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);

		logText = "head sepn vector contains "      + pSimBox->ToString(static_cast<long>(vHeadSeparation.size())) + " pairs";
		CLogTextMessage* pText6  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);

		logText = "tail sepn vector contains "      + pSimBox->ToString(static_cast<long>(vTailSeparation.size())) + " pairs";
		CLogTextMessage* pText7  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);

		// Create a template bond for use in creating all polymerised bonds.
		// We get the current number of bond types from the CInitialState
		// object and create a new type to be used as a template for all
		// polymerised bonds. The template is stored in the CInitialState
		// just as the normal bond types using the interface class
		// IModifySimStateBondTypes.

		long polyBondCounter    = pSimBox->GetSimBoxBonds().size();		// used to assign bond ids
		const long polyBondType = pSimBox->GetBondTypeTotal();	// used to create new bond type

		CBond* pPolyBondTemplate = 0;

#if SimIdentifier == MD
		springConstant		*= (pSimBox->GetLengthScale()*pSimBox->GetLengthScale()/pSimBox->GetEnergyScale());
		unStretchedLength   /= pSimBox->GetLengthScale();
#endif

		pPolyBondTemplate = new CBond(polyBondType, springConstant, unStretchedLength);

		// Create a new unique name for the bond type and store the template bond 
        // for later use. The bond name is constructed using the target's (unique)
        // label, a random string, and a counter. The latter index can be used
        // in the future if many bond types are constructed for the same target.
        // We use a dash as the separator to distinguish the target name
        // and the random string. A sample dynamic bond name is,
        //
        // TargetLabel-12345678-1

        const zString polyBondName = CCNTCell::GetRandomString(targetLabel, "-");

		pSimBox->IModifyBondTypes()->AddNewNamedType(pPolyBondTemplate, polyBondType, polyBondName);

		BondVector vPolyBonds;
		vPolyBonds.clear();

		// Now we have a set of polymer pairs that can be bonded at their
		// head and tails. Note that these may contain the same polymer several
		// times if it has several neighbours, but we have ensured that it does
		// not have more than maxBondsPerPolymer neighbours. We iterate over all 
		// pairs, create two bonds for their head and tail, set their parameters 
		// and add them to the simulation's bond container. 

		double actualSep = 0.0;

		for(long pairIndex=0; pairIndex<vFirstPolymerInPair.size(); pairIndex++)
		{
			const long firstId  = vFirstPolymerInPair.at(pairIndex);
			const long secondId = vSecondPolymerInPair.at(pairIndex);

			CPolymer* const pPoly1 = mPolymers.find(firstId)->second;
			CPolymer* const pPoly2 = mPolymers.find(secondId)->second;
				
			CAbstractBead* pHead1 = pPoly1->GetHead();
			CAbstractBead* pTail1 = pPoly1->GetTail();
			CAbstractBead* pHead2 = pPoly2->GetHead();
			CAbstractBead* pTail2 = pPoly2->GetTail();
		
			// Create a bond and set its parameters: we replace the default
			// unstretched length with the beads' actual separation, and
            // translate all newly-bonded beads so that their unPBC coordinates
            // are in the principal image of the SimBox. This ensures that the
            // bond force calculations in CBond do not calculate forces based
            // on beads in different images of the SimBox. This does not break 
            // the diffusion constant calculation as the newly-created bonds would
            // have changed the beads' evolution from their unbonded states anyway.

            // Note that we cannot rely on the two polymers having the
            // same architecture; but we know they will have a well-defined
            // head and tail bead.

            BeadVector vmodBeads  = pPoly1->GetBeads();
            BeadVector vmodBeads2 = pPoly2->GetBeads();

            for(BeadVectorIterator iterBead=vmodBeads.begin(); iterBead!=vmodBeads.end(); iterBead++)
            {
                CAbstractBead* const pBead = (*iterBead);

                pBead->SetunPBCXPos(pBead->GetXPos());
                pBead->SetunPBCYPos(pBead->GetYPos());
                pBead->SetunPBCZPos(pBead->GetZPos());
                pBead->SetInitialXPos(pBead->GetXPos());
                pBead->SetInitialYPos(pBead->GetYPos());
                pBead->SetInitialZPos(pBead->GetZPos());
            }

            for(BeadVectorIterator iterBead2=vmodBeads2.begin(); iterBead2!=vmodBeads2.end(); iterBead2++)
            {
                CAbstractBead* const pBead = (*iterBead2);

                pBead->SetunPBCXPos(pBead->GetXPos());
                pBead->SetunPBCYPos(pBead->GetYPos());
                pBead->SetunPBCZPos(pBead->GetZPos());
                pBead->SetInitialXPos(pBead->GetXPos());
                pBead->SetInitialYPos(pBead->GetYPos());
                pBead->SetInitialZPos(pBead->GetZPos());
            }


			CBond* pBond1 = new CBond(*pPolyBondTemplate);

			pBond1->SetId(polyBondCounter++);
			pBond1->SetBeads(pHead1, pHead2);
			pBond1->SetBeadIndex(pHead1->GetId(), pHead2->GetId());

			actualSep = vHeadSeparation.at(pairIndex);
			pBond1->SetUnStretchedLength(actualSep);

			vPolyBonds.push_back(pBond1);

			// Add the tail bond if the polymer is not a single bead

			if(polymerSize > 1)
			{
				CBond* pBond2 = new CBond(*pPolyBondTemplate);

				pBond2->SetId(polyBondCounter++);
				pBond2->SetBeads(pTail1, pTail2);
				pBond2->SetBeadIndex(pTail1->GetId(), pTail2->GetId());

				actualSep = vTailSeparation.at(pairIndex);
				pBond2->SetUnStretchedLength(actualSep);

				vPolyBonds.push_back(pBond2);
			}
		}


		// Add the new bonds to the ordinary bond container

		for(BondVectorIterator iterBond=vPolyBonds.begin(); iterBond!=vPolyBonds.end(); iterBond++)
		{
			pSimBox->IModifyBondTypes()->AddPolymerisedBond(*iterBond);
		}

		// Recalculate how many bonds of each type exist and tell the CMonitor
		// to update its local counters

		pSimBox->IModifyBondTypes()->CalculateTypeTotals();

		pSimBox->UpdateBondTypes();

		logText = pSimBox->ToString(static_cast<long>(vPolyBonds.size())) + " polymerised bonds created";
		CLogTextMessage* pText8  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);


		CLogctPolymerisePolymersInTarget* pMsg = new CLogctPolymerisePolymersInTarget(pSimBox->GetCurrentTime(), targetLabel, polyBondName, polyBondType,
													 polymerTotal, vPolyBonds.size(), maxBondsPerPolymer, range, fraction, springConstant, unStretchedLength);
	}
	else
	{
		CLogCommandFailed* pMsg = new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
