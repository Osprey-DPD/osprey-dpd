/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeForminNetwork.cpp: implementation of the aeForminNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "aeForminNetwork.h"
#include "aeCNTCell.h"
#include "aeForminBond.h"
#include "aefActinBond.h"
#include "aeActivePolymer.h"
#include "aefActinNetwork.h"
#include "IfActinAccessControl.h"
#include "prFormin.h"

#include "CNTCell.h"	// needed for random number generator


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.

const zString aeForminNetwork::m_Type = "Formin";

const zString aeForminNetwork::GetType()
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
								 double xw, double yw, double zw) {return new aeForminNetwork(pShadow, xn, yn, zn, xw, yw, zw);}

	const zString id = aeForminNetwork::GetType();

	const bool bRegistered = acfACNFactory::Instance()->Register(id, Create);
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aeForminNetwork::aeForminNetwork(IActiveSimBox* const pShadow, 
								 long xn, long yn, long zn,
								 double xw, double yw, double zw) : IModifyForminNetwork(pShadow, xn, yn, zn, xw, yw, zw),
																	m_bNucleationActive(true)
{

}

aeForminNetwork::~aeForminNetwork()
{

}

// Non-static function to return the type of the ACN.

const zString aeForminNetwork::GetACNType() const
{
	return m_Type;
}

// Function to attach the associated process to the ACN. We do it here as the
// ACN is the only object that knows the type of its associated process.

void aeForminNetwork::AttachProcess(xxProcess *pProcess)
{
	prFormin* pForminProcess = dynamic_cast<prFormin*>(pProcess);

	pForminProcess->SetACN(this);		
}

// Function that updates the state of all free active bonds, or monomers, and polymers
// that represent Formin molecules. Free monomers exert forces on each other when they 
// are within range, and may assemble into polymers. When a monomer is part of a polymer
// it is said to be "activated". Once an active bond has been activated, it is removed 
// from the free active bond container. The active polymer it is a part
// of is then added to the free active polymer container unless it was already there.
// Hence, any bond that is still in the m_FreeActiveBonds container is by definition
// not part of an active polymer and can exert forces on other bonds.
//
// Each formin monomer can bind at most two mActin monomers. The Head and Tail of the
// formin cooperate to transfer mActin monomers from the Tail to the Head where they
// are attached to the end of a growing fActin filament. The formin Tail searches
// for a nearby mActin monomer that is within its binding range and, when it finds
// one, forms an active polymer with it. The formin instance is then removed from 
// its active bonds container and added to the active polymers container. It can then
// proceed to the next step in assembling an fActin filament.
//
//

void aeForminNetwork::Evolve()
{
	// Sweep over all active bonds updating their positions

	UpdateActiveBondsInCells();

	// If allowed, attempt to nucleate new active polymers by finding an mActin 
	// monomer located close to the formin Tail that can be bound

	if(m_bNucleationActive)
	{
		Nucleate();
    }
	
	// Allow existing active polymers to grow and shrink by gain or addition 
	// of monomers at each end

//	Transfer();

	// Now do some simple analysis of the filaments

	if(GetFilamentTotal() > 0)
	{
		CalculateFilamentProperties();
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


		std::cout << "Formin monos = " << freeMonoTotal << "  " << boundMonoTotal << "  " << monoTotal << "  ";
		std::cout << freeFraction << "  " << boundFraction << zEndl;
		std::cout << "Formin filaments = " << GetFilamentTotal() << "  " << GetMeanFilamentSize()  << "  " << GetMeanFilamentLength() << zEndl;
*/
	}

}

// Function that attempts to construct active polymers out of two active bonds that
// satisfy certain conditions. The specific conditions for polymer nucleation
// depend on the active bond type and are specified in the virtual function
// aeActiveBond::Activate().
//
// The formin active bonds bind monomers of mActin and assemble them into an
// fActin filament. Formin has two binding sites, its Head and Tail, and each
// site searches for and binds a nearby mActin monomer and then transfers one 
// of them to the other binding site for attachment to the growing fActin filament. 
//
// This function is used to bind the first and second mActin monomers to a 
// formin molecule. When the first site is full, the active bond is removed from the
// free bonds container and transferred to the active polymers container.
// The Transfer() routine is then used to bind subsequent mActin monomers and 
// transfer them to the growing filament.

void aeForminNetwork::Nucleate()
{
	// Obtain a pointer to the IfActinAccessControl interface so that the formin
	// monomers can locate and bind to mActin monomers. Note that we have to check
	// that the IACNAccessControl pointer grants access to the desired network:
	// not all networks can interact with each other.

	IfActinAccessControl* pIACN = dynamic_cast<IfActinAccessControl*>(GetIActiveSimBox()->GetIACNAccessControlFromType(aefActinNetwork::GetType()));

	if(pIACN && pIACN->IsAccessControlEnabled())
	{
		IfActinAccessFromFormin* const pIfActin = pIACN->IsAccessAllowed(this);

		if(pIfActin)
		{
			// Iterate over each free formin molecule to see if it can bind 
			// a new mActin monomer: note that a formin molecule is included as
			// part of the free active bonds list if it has either its Head or Tail
			// available to capture a new mActin monomer.

			ActiveBondListIterator iterBond=m_FreeActiveBonds.begin();

			while(iterBond != m_FreeActiveBonds.end())
			{
				aeActiveBond* pBond = *iterBond;

				double x = pBond->GetTailHeadBead()->GetXPos();
				double y = pBond->GetTailHeadBead()->GetYPos();
				double z = pBond->GetTailHeadBead()->GetZPos();
				
				// Find a nearby mActin monomer and, if it is closer to the 
				// formin tail binding site than a given distance,add it 
				// to the growing filament. Note that the distance check uses
				// the empty formin binding site at its tail, but the monomer
				// will be inserted between the head binding site and the back
				// of the growing actin filament.

				aeActiveBond* pTargetBond = pIfActin->GetNearbyMonomer(x,y,z);

				if(pTargetBond && pBond->Activate(pTargetBond))
				{				
					// If a suitable target mActin monomer is found, we tell the 
					// fActin network to remove it from its free bond container, 
					// replace its normal on condition with an external trigger, 
					// and bind it to the formin's Tail. The formin monomer that 
					// has just bound the mActin is then removed from its free bond 
					// container, and the new polymer added to the free polymer container.

					pIfActin->RemoveNearbyMonomer(pTargetBond);
//					pIfActin->SetBondExternalTriggerOn(pTargetBond);

					// Note the argument order: polymer's head bond then its tail bond
					
					aeActivePolymer* pPolymer = new aeActivePolymer(pTargetBond, *iterBond);
					
					std::cout << "Created new polymer " << pPolymer->GetId() << " out of bonds " << pTargetBond->GetId() << " " << (*iterBond)->GetId() << zEndl;

					// Add the newly-created polymer to the free polymer container,
					// and remove the formin bond from the free bond container

					AddFreePolymer(pPolymer);
					
					iterBond = RemoveFreeBondByIterator(iterBond);
				}
				else
				{
					iterBond++;
				}
			} 
		}
		else
		{
			// Target network does not grant this network access
		}
	}
	else
	{
		// Target network unrecognised or it does not implement access control

	}
}

// Function that allows active formin polymers (ie, ones that already have a bound
// mActin monomer) to bind further monomers and assemble them into a growing filament. A formin polymer is "active" if it has at least
// The filament is transferred from the Head to the Tail of the formin and vice
// versa as successive monomers are added.

void aeForminNetwork::Transfer()
{	
	// Obtain a pointer to the IfActinAccessControl interface so that the formin
	// monomers can locate and bind to mActin monomers. Note that we have to check
	// that the IACNAccessControl pointer grants access to the desired network:
	// not all networks can interact with each other.

	IfActinAccessControl* pIACN = dynamic_cast<IfActinAccessControl*>(GetIActiveSimBox()->GetIACNAccessControlFromType(aefActinNetwork::GetType()));

	if(pIACN && pIACN->IsAccessControlEnabled())
	{
		IfActinAccessFromFormin* const pIfActin = pIACN->IsAccessAllowed(this);

		if(pIfActin)
		{
	        std::cout << "Formin transfer" << zEndl;		
			// Iterate over each activated formin molecule to see if it can bind 
			// a new mActin monomer: note that a formin molecule is activated once
			// it has bound its first mActin monomer.

			ActivePolymerListIterator iterAP=m_FreeActivePolymers.begin();

			while(iterAP != m_FreeActivePolymers.end())
			{
				aeActivePolymer*  pPolymer = *iterAP;

				// An actin filament grows from the formin active polymer's
				// head, and a new mActin, if found, is initially attached 
				// to its tail and transferred to its head.

//				aeActiveBond*  pHeadBond = pPolymer->GetHeadBond();
				aeActiveBond*  pTailBond = pPolymer->GetTailBond();

//				aeActiveBond*  pHeadHeadAdjacentBond = pHeadBond->GetHeadAdjacentBond();
//				aeActiveBond*  pHeadTailAdjacentBond = pHeadBond->GetTailAdjacentBond();
//				aeActiveBond*  pTailHeadAdjacentBond = pTailBond->GetHeadAdjacentBond();
//				aeActiveBond*  pTailTailAdjacentBond = pTailBond->GetTailAdjacentBond();


				double x = pTailBond->GetTailHeadBead()->GetXPos();
				double y = pTailBond->GetTailHeadBead()->GetYPos();
				double z = pTailBond->GetTailHeadBead()->GetZPos();

				// Find a nearby mActin monomer and, if it is closer to the 
				// formin tail binding site than a given distance,add it 
				// to the growing filament. Note that the distance check uses
				// the empty formin binding site at its tail, but the monomer
				// will be inserted between the head binding site and the back
				// of the growing actin filament.

				aeActiveBond* pTargetBond = pIfActin->GetNearbyMonomer(x,y,z);

//				double testNo = CCNTCell::GetRandomNo();

				if(pTargetBond && pTailBond->Activate(pTargetBond))
				{
					// If a target mActin monomer is found we tell the 
					// fActin network to remove it from its free bond container
					// and replace its normal on condition with an external trigger. 

					pIfActin->RemoveNearbyMonomer(pTargetBond);
					pIfActin->SetBondExternalTriggerOn(pTargetBond);

					// Adding an mActin bond to the tail of the formin does not
					// result in the formin being added to the mActin's head as it
					// does not have the correcct bond type and the aeActiveBond::AddHeadMonomer()
					// function checks that mActin can only bond with mActin.
					// We ignore this for now.

//					pPolymer->AddTailBond(pTargetBond);

					// Now transfer the new mActin to the growing filament.
					// Note that we may have to allow some time
					// for this process to occur: here we just connect it immediately
					// and let a spring pull it into position.
					// There are three steps:
					//
					// 1 Get a pointer to the formin bond that is always at the polymer's tail
					// 2 Insert the new mActin monomer after the formin
					
					// Store a pointer to the last mActin in the filament

//					aeActiveBond* const pActinTail = pPolymer->GetTailBond()->GetHeadAdjacentBond();
						
					// Insert the new mActin monomer at the tail of the growing actin 
					// filament

					pPolymer->InsertBond(pTailBond, pTargetBond);

//					pHeadBond = pPolymer->GetHeadBond();
//					pTailBond = pPolymer->GetTailBond();

//					pHeadHeadAdjacentBond = pHeadBond->GetHeadAdjacentBond();
//					pHeadTailAdjacentBond = pHeadBond->GetTailAdjacentBond();
//					pTailHeadAdjacentBond = pTailBond->GetHeadAdjacentBond();
//					pTailTailAdjacentBond = pTailBond->GetTailAdjacentBond();

				}
					
				iterAP++;
			}
		}
		else
		{
			// Target network does not grant this network access
		}
	}
	else
	{
		// Target network unrecognised or it does not implement access control

	}

}

// Command handler function to change the maximum separation at which
// two mActin monomer active bonds can associate to form a filament.
// The distance test is implemented in the aeBondProximityOn class.
//
// Note that we have to modify both the free active bonds and the ones
// already assembled into filaments.

void aeForminNetwork::SetProximityOnSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeForminBond* pBond = dynamic_cast<aeForminBond*>(*iterBond);

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
			aeForminBond* pFormin = dynamic_cast<aeForminBond*>(pBond);

			pFormin->SetOnRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeForminBond*>(pHeadBond)->SetOnRateSeparation(max);
	}

}

// Command handler function to change the maximum separation at which
// connected mActin monomers dissociate. The distance test is implemented 
// in the aeBondProximityOff class.
//
// Note that we have to modify both the free active bonds and the ones
// already assembled into filaments.

void aeForminNetwork::SetProximityOffSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond=m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeForminBond* pBond = dynamic_cast<aeForminBond*>(*iterBond);

		pBond->SetOffRateSeparation(max);
	}

	// Now the bonds already assembled into filaments

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActiveBond* pBond     = (*iterPoly)->GetTailBond();
		aeActiveBond* pHeadBond = (*iterPoly)->GetHeadBond();

		while(pBond != pHeadBond)
		{
			aeForminBond* pFormin = dynamic_cast<aeForminBond*>(pBond);

			pFormin->SetOffRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeForminBond*>(pHeadBond)->SetOffRateSeparation(max);
	}

}

// Command handler function to change the maximum separation at which
// connected mActin monomers associate via the head of a filament. 
// The distance test is implemented in the aeBondProximityOn class.

void aeForminNetwork::SetProximityHeadOnSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeForminBond* pBond = dynamic_cast<aeForminBond*>(*iterBond);

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
			aeForminBond* pFormin = dynamic_cast<aeForminBond*>(pBond);

			pFormin->SetHeadOnRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeForminBond*>(pHeadBond)->SetHeadOnRateSeparation(max);
	}
}

// Command handler function to change the maximum separation at which
// connected mActin monomers dissociate from the head of a filament. 
// The distance test is implemented in the aeBondProximityOff class.

void aeForminNetwork::SetProximityHeadOffSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeForminBond* pBond = dynamic_cast<aeForminBond*>(*iterBond);

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
			aeForminBond* pFormin = dynamic_cast<aeForminBond*>(pBond);

			pFormin->SetHeadOffRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeForminBond*>(pHeadBond)->SetHeadOffRateSeparation(max);
	}
}

// Command handler function to change the maximum separation at which
// connected mActin monomers associate via the tail of a filament. 
// The distance test is implemented in the aeBondProximityOn class.

void aeForminNetwork::SetProximityTailOnSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeForminBond* pBond = dynamic_cast<aeForminBond*>(*iterBond);

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
			aeForminBond* pFormin = dynamic_cast<aeForminBond*>(pBond);

			pFormin->SetTailOnRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeForminBond*>(pHeadBond)->SetTailOnRateSeparation(max);
	}
}

// Command handler function to change the maximum separation at which
// connected mActin monomers dissociate from the tail of a filament. 
// The distance test is implemented in the aeBondProximityOff class.

void aeForminNetwork::SetProximityTailOffSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeForminBond* pBond = dynamic_cast<aeForminBond*>(*iterBond);

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
			aeForminBond* pFormin = dynamic_cast<aeForminBond*>(pBond);

			pFormin->SetTailOffRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeForminBond*>(pHeadBond)->SetTailOffRateSeparation(max);
	}
}

// Command handler functions to activate/inactivate the formation of new 
// filaments for all free formin molecules. This does not stop existing 
// filaments from growing, but prevents new ones being nucleated.

void aeForminNetwork::ActivateForminNucleation()
{
	m_bNucleationActive = true;
}

void aeForminNetwork::InactivateForminNucleation()
{
	m_bNucleationActive = false;
}

// Function to completely disassemble all formin-nucleated filaments. This
// is in contrast to the SeverForminFilaments() function that just removes the
// formin molecules from their attached filaments.

void aeForminNetwork::DisassembleForminFilaments()
{
	// Obtain a pointer to the IfActinAccessControl interface so that the formin
	// monomers can interact with mActin monomers. Note that we have to check
	// that the IACNAccessControl pointer grants access to the desired network:
	// not all networks can interact with each other.

	IfActinAccessControl* pIACN = dynamic_cast<IfActinAccessControl*>(GetIActiveSimBox()->GetIACNAccessControlFromType(aefActinNetwork::GetType()));

	if(pIACN && pIACN->IsAccessControlEnabled())
	{
		IfActinAccessFromFormin* const pIfActin = pIACN->IsAccessAllowed(this);

		if(pIfActin)
		{
			ActivePolymerListIterator iterAP = m_FreeActivePolymers.begin();

			while(iterAP != m_FreeActivePolymers.end())
			{
				aeActivePolymer* pPoly = (*iterAP);

				// Iterate along the polymer starting at its head, and separate
				// the head bond from the polymer using RemoveHeadBond(); this
				// function returns true if the polymer still has at least 
				// 2 bonds, otherwise it returns false.
				
				// Note that the tail of the polymer is always the formin monomer,
				// and the head is always an actin monomer

				aeActiveBond* const pForminBond = pPoly->GetTailBond();
				aeActiveBond* pActinBond        = pPoly->GetHeadBond();

				while(pPoly->RemoveHeadBond())
				{
					// Released head bond is a single actin monomer so pass it 
					// to the actin network's free bond container.

					pIfActin->AddFreeBond(pActinBond);

					pActinBond = pPoly->GetHeadBond();
				}

				// Polymer has been disassembled, so return the unattached
				// formin to the free bond container and destroy the empty 
				// formin polymer instance

				AddFreeBond(pForminBond);

				delete pPoly;

				iterAP = m_FreeActivePolymers.erase(iterAP);
			}

			// Clear the free polymer container

			 m_FreeActivePolymers.clear();
		}
		else
		{
			// Target network does not grant this network access
		}
	}
	else
	{
		// Target network unrecognised or it does not implement access control

	}
}

// Command handler function to release all filaments that are currently being
// assembled by formin molecules. This only affects existing filaments, but does
// not prevent new ones from being nucleated.
//
// We iterate over all formin polymers, separate the formin bond from 
// their fActin filament segments and add them to the free formin bond container. 
// The separated fActin filaments are added to the fActin network's free polymers 
// unless the filament is only a single actin monomer, in which case the monomer 
// is added to the fActin network's free bond container.

void aeForminNetwork::SeverForminFilaments()
{
	// Obtain a pointer to the IfActinAccessControl interface so that the formin
	// monomers can interact with mActin monomers. Note that we have to check
	// that the IACNAccessControl pointer grants access to the desired network:
	// not all networks can interact with each other.

	IfActinAccessControl* pIACN = dynamic_cast<IfActinAccessControl*>(GetIActiveSimBox()->GetIACNAccessControlFromType(aefActinNetwork::GetType()));

	if(pIACN && pIACN->IsAccessControlEnabled())
	{
		IfActinAccessFromFormin* const pIfActin = pIACN->IsAccessAllowed(this);

		if(pIfActin)
		{
			ActivePolymerListIterator iterAP = m_FreeActivePolymers.begin();

			while(iterAP != m_FreeActivePolymers.end())
			{
				aeActivePolymer* pPoly = (*iterAP);

				// Separate the tail bond from the polymer using RemoveTailBond(); 
				// this function returns true if the polymer still has at least 
				// 2 bonds, otherwise it returns false.
				
				// Note that the tail of the polymer is always the formin monomer,
				// and the head is always an actin monomer

				aeActiveBond* const pForminBond = pPoly->GetTailBond();
				aeActiveBond* pActinBond        = pForminBond->GetHeadAdjacentBond();
		
				std::cout << "size of formin polymer " << pPoly->GetSize() << zEndl;

				if(pPoly->RemoveTailBond())
				{
					// Remainder of polymer is an actin filament with at least 2
					// bonds in it, so we create an fActin polymer, using the 
					// constructor that takes a single, connected tail bond as 
					// its argument, and pass it to the fActin network to add to 
					// its free polymer container

					std::cout << "removing formin from actin filament" << zEndl;
					std::cout << "size of remaining formin polymer " << pPoly->GetSize() << zEndl;
					std::cout << "head of actin filament = "<< pActinBond->GetHeadAdjacentBond() << zEndl;
					std::cout << "next head of actin filament = "<< pActinBond->GetHeadAdjacentBond()->GetHeadAdjacentBond() << zEndl;


					aeActivePolymer* pActinPolymer = new aeActivePolymer(pActinBond);

					std::cout << "size of new actin polymer " << pActinPolymer->GetSize() << zEndl;

					pIfActin->AddFreePolymer(pActinPolymer);
				}
				else
				{
					// Remainder of polymer is a single actin monomer, so pass
					// it to the fActin network to add to its free bond container

					pIfActin->AddFreeBond(pActinBond);

				}

				// Released tail bond is always a single formin monomer so add
				// it to the formin free polymer container, and destroy the 
				// formin polymer instance

				AddFreeBond(pForminBond);
				delete pPoly;

				iterAP = m_FreeActivePolymers.erase(iterAP);
			}

			// Clear the free polymer container

			std::cout << "before dissolution formin bond/polymer size = " << m_FreeActiveBonds.size()  << "  " << m_FreeActivePolymers.size() << zEndl;

			 m_FreeActivePolymers.clear();

			std::cout << "after dissolution formin bond/polymer size = " << m_FreeActiveBonds.size()  << "  " << m_FreeActivePolymers.size() << zEndl;
		}
		else
		{
			// Target network does not grant this network access
		}
	}
	else
	{
		// Target network unrecognised or it does not implement access control

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


void aeForminNetwork::SetFixedHeadOffRate(double rate)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeForminBond* pBond = dynamic_cast<aeForminBond*>(*iterBond);

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
			aeForminBond* pFormin = dynamic_cast<aeForminBond*>(pBond);

	        pFormin->SetFixedHeadOffRate(rate);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the bond that is not included in the above loop

		dynamic_cast<aeForminBond*>(pHeadBond)->SetFixedHeadOffRate(rate);
	}
}

// Command handler function to set a fixed probability for a monomer to detach
// from its neighbour via its tail connection. See the SetFixedHeadOffRate()
// function above for important details.

void aeForminNetwork::SetFixedTailOffRate(double rate)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeForminBond* pBond = dynamic_cast<aeForminBond*>(*iterBond);

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
			aeForminBond* pFormin = dynamic_cast<aeForminBond*>(pBond);

	        pFormin->SetFixedTailOffRate(rate);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the bond that is not included in the above loop

		dynamic_cast<aeForminBond*>(pHeadBond)->SetFixedTailOffRate(rate);
	}
}
