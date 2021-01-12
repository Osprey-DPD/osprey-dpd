/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccCreateNanoparticleSphere.cpp: implementation of the ccCreateNanoparticleSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMPSFlags.h"
#include "SimMathFlags.h"
#include "ccCreateNanoparticleSphereImpl.h"
#include "ccCreateNanoparticleSphere.h"
#include "Nanoparticle.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "mpsSimBox.h"
#include "IModifySimStateBeadTypes.h"
#include "IModifySimStateBondTypes.h"
#include "Monitor.h"
#include "CurrentState.h"
#include "Bead.h"
#include "Bond.h"
#include "Polymer.h"
//#include "LogCreateNanoparticle.h"
#include "LogctChangeNamedBeadType.h"
#include "LogctPolymerisePolymersInTarget.h"
#include "LogTextMessage.h"
#include "pmInteger.h"
#include "pmString.h"


#include "CNTCell.h"	// Needed for the RNG

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccCreateNanoparticleSphereImpl::ccCreateNanoparticleSphereImpl()
{

}

ccCreateNanoparticleSphereImpl::~ccCreateNanoparticleSphereImpl()
{

}

// Function to implement the ccCreateNanoparticleSphere command that collects 
// polymers of the (single) specified type whose coordinates lie within a
// sphere, or spherical shell if the inner radius is non-zero, and polymerises them
// into a quasi-rigid nanoparticle.
//
// We assume that the command data has been validated prior to this routine. But note also that 
// the parallel message class pmCreateNanoparticleSphere is responsible for routing the command to the processor
// designated by the procId parameter. So we do not repeat the check here.
//
// Only a single processor is allowed to execute this command: multiple nanoparticles requires the command to be 
// issued multiple times.

void ccCreateNanoparticleSphereImpl::CreateNanoparticleSphere(const xxCommand* const pCommand)
{    
    const ccCreateNanoparticleSphere* const pCmd = dynamic_cast<const ccCreateNanoparticleSphere*>(pCommand);
        
    const zString polymerName   = pCmd->GetPolymerName();		// Name of polymer type
    const long    procId        = pCmd->GetProcessorId();       // Processor that will create the nanoparticle
    const long    displayId     = pCmd->GetColourId();          // New colour of nanoparticle beads
    const double xCentre		= pCmd->GetXCentre();
    const double yCentre		= pCmd->GetYCentre();		    // Sphere's centre (fraction)
    const double zCentre		= pCmd->GetZCentre();
    const double innerRadius	= pCmd->GetInnerRadius();	    // Inner radius of shell
    const double outerRadius	= pCmd->GetOuterRadius();	    // Inner radius of shell
    
	const long   maxBondsPerPolymer	= pCmd->GetMaxBondsPerPolymer();
	const double range			    = pCmd->GetRange();
	const double fraction			= pCmd->GetBondFraction();
	const double springConstant		= pCmd->GetSpringConstant();
	const double unStretchedLength  = pCmd->GetUnstretchedLength();
//	const double numberType         = pCmd->GetNumberTypes();
	const zLongVector vBeadTypes    = pCmd->GetBeadTypes();
	const zDoubleVector vConsInt    = pCmd->GetConsInt();
            
	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);
        
	// Find the integer type of the specified polymers and their head bead type
    
	const long polyType   = pSimBox->GetPolymerTypeFromName(polymerName);
	const long oldType    = pSimBox->GetPolymerHeadType(polyType);
    const zString oldName = pSimBox->GetBeadNameFromType(oldType);
    
    // We need the following variables so that the owning processor can create the new bead type 
    // and all other processors can receive messages containing the data and update their own state.
    
    long newType = 0;
    zString newName = "";
    
	PolymerVector vPolymers;
	vPolymers.clear();
    
	BeadVector vHeadBeads;
	vHeadBeads.clear();
    
    BeadVector vBeads;
    vBeads.clear();
    
    long polymerTotal = 0;  // We have to define this here to ensure it is in scope after the processor-dependent if statement next
    
    // We only execute the command on the target processor.
    
    if(xxParallelBase::GlobalGetRank() == procId)
    {
//        std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " ";
//        std::cout << "Creating nanoparticle with parameters " << polymerName << " " << procId << " " << displayId << " " << xCentre << " " << yCentre << " " << zCentre << " " << innerRadius << " " << outerRadius << " " << maxBondsPerPolymer << " " << range << " " << fraction << " " << springConstant << " " << unStretchedLength << " " << polyType << " " << oldType << zEndl;
 
        // Get the polymers to be turned into the nanoparticle
        
	    const double xc		  = xCentre*pSimBox->GetSimBoxXLength();
	    const double yc		  = yCentre*pSimBox->GetSimBoxYLength();
	    const double zc		  = zCentre*pSimBox->GetSimBoxZLength();
        const double rinsq    = innerRadius*innerRadius;
        const double routsq   = outerRadius*outerRadius;
        
//        std::cout << "xc, etc = " << xc << " " << yc << " " << zc << " " << rinsq << " " << routsq << zEndl;

	    // Collect the polymers whose type matches that specified and whose head
	    // beads lie within the spherical region specified
        //
        // In the serial code we can retrieve all the polymers in the SimBox, but in the parallel code some of these polymer pointers 
        // will be invalid if the polymers have moved to other processors' spaces. So we get only those polymers in a region around 
        // the location of the nanoparticle creation site. Note that we cannot use this function in the serial code as the bead 
        // instances do not contain a pointer to their owning polymers in that case.
    
#if EnableParallelSimBox == SimMPSEnabled
        PolymerVector vAllPolymers = pSimBox->GetPolymersInRectangularRegion(xc, yc, zc, outerRadius, outerRadius, outerRadius);
#else
        PolymerVector vAllPolymers = pSimBox->GetAllPolymers();
#endif
            
	    PolymerVector vSourcePolymers;
        vSourcePolymers.clear();
    
	    PolymerVectorIterator iterPoly;
    
	    for(iterPoly=vAllPolymers.begin(); iterPoly!=vAllPolymers.end(); iterPoly++)
	    {
            if((*iterPoly)->GetType() == polyType)
            {
                vSourcePolymers.push_back(*iterPoly);
            }
	    }
    
        CBead* pBead = 0;
        
	    for(iterPoly=vSourcePolymers.begin(); iterPoly!=vSourcePolymers.end(); iterPoly++)
	    {
		    CPolymer* const pPolymer = *iterPoly;
        
            if(pPolymer)
            {
                pBead = dynamic_cast<CBead*>(pPolymer->GetHead());
        
	  	        if(pBead)
		        {
			        double xBead = pBead->GetXPos();
			        double yBead = pBead->GetYPos();
			        double zBead = pBead->GetZPos();
            
			        double rDistSq =  (xBead - xc)*(xBead - xc) + (yBead - yc)*(yBead - yc) + (zBead - zc)*(zBead - zc);
            
			        if(rDistSq > rinsq && rDistSq < routsq)
			        {
				        vPolymers.push_back(pPolymer);
				        vHeadBeads.push_back(pBead);
                        BeadVector vPolymerBeads = pPolymer->GetBeads();
                
                        copy(vPolymerBeads.begin(), vPolymerBeads.end(), back_inserter(vBeads));
			        }
		        }
                else
                {
                    std::cout << "ERROR - this should not be seen" << zEndl;
                }
            }
            else
            {
                std::cout << "ERROR - this should not be seen" << zEndl;
            }
	    }
    
        polymerTotal = vPolymers.size();
            
        // Change the type of the beads in the nanoparticle so that we can modify their interactions
    
        newType = pSimBox->GetBeadTypeTotal();
        newName = CCNTCell::GetRandomString(oldName, "-");
    
        // Change the numeric type and display id (ie colour) of the beads

        if(!vBeads.empty())
        {
            for(BeadVectorIterator iterBead = vBeads.begin(); iterBead != vBeads.end(); iterBead++)
            {
                (*iterBead)->SetType(newType);
                (*iterBead)->SetVisible();
                CCurrentState::SetBeadDisplayId((*iterBead)->GetId(), displayId);
            }
        
            // Note that we pass the oldType parameter to the IModifyBeadTypes()
            // interface object because the new type is just one greater than the
            // total number of bead types but we want to copy the interaction
            // parameters for the old type into the new type.
        
            pSimBox->IModifyBeadTypes()->AddNewNamedType(oldType, newType, newName);
                
            // and propagate the changes to the CMonitor
        
            pSimBox->UpdateBeadTypes();
        
            // Set the conservative interactions of the nanoparticle's bead type
        
            long j = 0;
            for (czLongVectorIterator iter2Type=vBeadTypes.begin(); iter2Type != vBeadTypes.end(); iter2Type++)
            {   
                if(*iter2Type < pSimBox->GetBeadTypeTotal())
                {
//                    std::cout << "Proc " << xxParallelBase::GlobalGetRank() << " adding ConsInt value " << newType << " " << *iter2Type << " " << vConsInt.at(j) << zEndl;
                    pSimBox->IModifyBeadTypes()->SetDPDConsInt(newType, *iter2Type, vConsInt.at(j++));
                }
            }
        }
        
        // Parallel log messages not implemented yet except on P0.
        
        if(procId == 0)
        {
             new CLogctChangeNamedBeadType(pSimBox->GetCurrentTime(), "Nanoparticle", oldName, newName, oldType, newType);
        }
    }
    
#if EnableParallelSimBox == SimMPSEnabled
        // We have to propagate the changes to the other processors in a parallel run so that 
        // all processors know about the new type when it comes to saving snapshots.
        // We need to send the oldType, newType and newName parameters.

        // Send the new bead type to all PN except the sending processor and get them to unpack the data 
        // and update their own states with the new bead type.
                
        pmInteger newTypeMessage;
        pmString  newNameMessage;
        
        newTypeMessage.SetTag(1);
        newNameMessage.SetTag(2);
                
        if(xxParallelBase::GlobalGetRank() == procId)
        {
            newTypeMessage.SetMessageData(newType);
            newNameMessage.SetMessageData(newName);
            
            newTypeMessage.SendAllP();
            newNameMessage.SendAllP();
        }
        else
        {
            newTypeMessage.Receive();
            newNameMessage.Receive();
                 
            newType = newTypeMessage.GetValue();
            newName = newNameMessage.GetValue();
            
            pSimBox->IModifyBeadTypes()->AddNewNamedType(oldType, newType, newName);
        
            // and propagate the changes to the CMonitor
        
            pSimBox->UpdateBeadTypes();
        }
#endif

        // The owning processor now polymerizes the polymers in the nanoparticle.
        
    if(xxParallelBase::GlobalGetRank() == procId)
    {    
        // Now polymerise all the polymers in the nanoparticle, ie, those in the container vTargetPolymers
    
	    const double rangeSq = range*range; // used to avoid taking square roots
    
	    // Check that we have more than one polymer and the bond fraction is not zero; then we collect all the head 
	    // beads of the contained polymers and all the tail beads preparatory 
	    // to binding them to each other (head-head and tail-tail).
    
	if(polymerTotal > 1 && fraction > 0.0)
	{
		const long polymerSize = vPolymers.at(0)->GetSize();
       
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
		LongLongMap	mNeighbours;	// (polymer id, neighbours)
        
		zLongVector	vFirstPolymerInPair;	
		zLongVector	vSecondPolymerInPair;	
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
        
        // Parallel log messages not implemented yet
        
//		zString logText = "Nanoparticle contains " + pSimBox->ToString(polymerTotal) + " polymers";
//		CLogTextMessage* pText1  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);
        
//		logText = "polymers map contains  " + pSimBox->ToString(static_cast<long>(mPolymers.size())) + " elements";
//		CLogTextMessage* pText2  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);
        
//		logText = "neighbour map contains "         + pSimBox->ToString(static_cast<long>(mNeighbours.size())) + " elements";
//		CLogTextMessage* pText3  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);
        
//		logText = "first polymer vector contains "  + pSimBox->ToString(static_cast<long>(vFirstPolymerInPair.size())) + " pairs";
//		CLogTextMessage* pText4  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);
        
//		logText = "second polymer vector contains " + pSimBox->ToString(static_cast<long>(vSecondPolymerInPair.size())) + " pairs";
//		CLogTextMessage* pText5  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);
        
//		logText = "head sepn vector contains "      + pSimBox->ToString(static_cast<long>(vHeadSeparation.size())) + " pairs";
//		CLogTextMessage* pText6  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);
        
//		logText = "tail sepn vector contains "      + pSimBox->ToString(static_cast<long>(vTailSeparation.size())) + " pairs";
//		CLogTextMessage* pText7  = new CLogTextMessage(pSimBox->GetCurrentTime(), logText);

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
        
        const zString polyBondName = CCNTCell::GetRandomString("nanoparticle", "-");
        
		pSimBox->IModifyBondTypes()->AddNewNamedType(pPolyBondTemplate, polyBondType, polyBondName);
        
        // and propagate the changes to the CMonitor
        
        pSimBox->UpdateBondTypes();
        
		BondVector vPolyBonds;
		vPolyBonds.clear();
        
		// Now we have a set of polymer pairs that can be bonded at their
		// head and tails. Note that these may contain the same polymer several
		// times if it has several neighbours, but we have ensured that it does
		// not have more than maxBondsPerPolymer neighbours. We iterate over all 
		// pairs, create two bonds for their head and tail, set their parameters 
		// and add them to the simulation's bond container. 
        
		double actualSep = 0.0;
        
		for(long unsigned int pairIndex=0; pairIndex<vFirstPolymerInPair.size(); pairIndex++)
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
                
        // Tell the CSimBox to create and store a nanoparticle containing the bonds just created. 
        // Note that the CSimBox is responsible for destroying the nanoparticles.

         pSimBox->CreateNanoparticle(vPolyBonds);
    }
    
    }
 
}

