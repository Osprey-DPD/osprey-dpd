/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// aeReceptorNetwork.cpp: implementation of the aeReceptorNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "aeReceptorNetwork.h"
#include "aeCNTCell.h"
#include "aeReceptorBond.h"
#include "aefActinBond.h"
#include "aeActivePolymer.h"
#include "aefActinNetwork.h"
#include "IfActinAccessControl.h"
#include "prReceptor.h"

#include "CNTCell.h"	// needed for random number generator


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.

const zString aeReceptorNetwork::m_Type = "Receptor";

const zString aeReceptorNetwork::GetType()
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
								 double xw, double yw, double zw) {return new aeReceptorNetwork(pShadow, xn, yn, zn, xw, yw, zw);}

	const zString id = aeReceptorNetwork::GetType();

	const bool bRegistered = acfACNFactory::Instance()->Register(id, Create);
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

aeReceptorNetwork::aeReceptorNetwork(IActiveSimBox* const pShadow, 
								 long xn, long yn, long zn,
								 double xw, double yw, double zw) : IModifyReceptorNetwork(pShadow, xn, yn, zn, xw, yw, zw),
																	m_bNucleationActive(true)
{

}

aeReceptorNetwork::~aeReceptorNetwork()
{

}

// Non-static function to return the type of the ACN.

const zString aeReceptorNetwork::GetACNType() const
{
	return m_Type;
}

// Function to attach the associated process to the ACN. We do it here as the
// ACN is the only object that knows the type of its associated process.

void aeReceptorNetwork::AttachProcess(xxProcess *pProcess)
{
	prReceptor* pReceptorProcess = dynamic_cast<prReceptor*>(pProcess);

	pReceptorProcess->SetACN(this);		
}

// Function that updates the state of all free active bonds, or monomers, and polymers
// that represent Receptor molecules. Free monomers exert forces on each other when they 
// are within range, and may assemble into polymers. When a monomer is part of a polymer
// it is said to be "activated". Once an active bond has been activated, it is removed 
// from the free active bond container. The active polymer it is a part
// of is then added to the free active polymer container unless it was already there.
// Hence, any bond that is still in the m_FreeActiveBonds container is by definition
// not part of an active polymer and can exert forces on other bonds.
//
// Each receptor monomer can bind at most one mActin monomer to its head. Currently, receptor-ligand bonds
// cannot be broken, i.e., the off-rate is zero.
//
//

void aeReceptorNetwork::Evolve()
{
	// Sweep over all active bonds updating their positions

	UpdateActiveBondsInCells();

	// If allowed, attempt to nucleate new active polymers by finding an mActin 
	// monomer located close to the receptor Tail that can be bound

	if(m_bNucleationActive)
		Nucleate();
		
	BindNext();

	// Now do some simple analysis of the filaments

	long filamentTotal = GetFilamentTotal();

	if(filamentTotal > 0)
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


		std::cout << "Receptor monos = " << freeMonoTotal << "  " << boundMonoTotal << "  " << monoTotal << "  ";
		std::cout << freeFraction << "  " << boundFraction << zEndl;
		std::cout << "filaments = " << filamentTotal << "  " << GetMeanFilamentSize()  << "  " << GetMeanFilamentLength() << zEndl;
		*/
	}
}

// Function that attempts to construct active polymers out of two active bonds that
// satisfy certain conditions. The specific conditions for polymer nucleation
// depend on the active bond type and are specified in the virtual function
// aeActiveBond::Activate().
//
// The receptor active bonds bind monomers of mActin and assemble them into an
// fActin filament. Receptor has two binding sites, its Head and Tail, and each
// site searches for and binds a nearby mActin monomer and then transfers one 
// of them to the other binding site for attachment to the growing fActin filament. 
//
// This function is used to bind the first and second mActin monomers to a 
// receptor molecule. When the first site is full, the active bond is removed from the
// free bonds container and transferred to the active polymers container.
// The Transfer() routine is then used to bind subsequent mActin monomers and 
// transfer them to the growing filament.

void aeReceptorNetwork::Nucleate()
{
	// Obtain a pointer to the IfActinAccessControl interface so that the receptor
	// monomers can locate and bind to mActin monomers. Note that we have to check
	// that the IACNAccessControl pointer grants access to the desired network:
	// not all networks can interact with each other.

	IfActinAccessControl* pIACN = dynamic_cast<IfActinAccessControl*>(GetIActiveSimBox()->GetIACNAccessControlFromType(aefActinNetwork::GetType()));

	if(pIACN && pIACN->IsAccessControlEnabled())
	{
		IfActinAccessFromReceptor* const pIfActin = pIACN->IsAccessAllowed(this);

		if(pIfActin)
		{
			// Iterate over each free receptor molecule to see if it can bind 
			// a new mActin monomer: note that a receptor molecule is included as
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
				// receptor tail binding site than a given distance,add it 
				// to the growing filament. Note that the distance check uses
				// the empty receptor binding site at its tail, but the monomer
				// will be inserted between the head binding site and the back
				// of the growing actin filament.

				aeActiveBond* pTargetBond = pIfActin->GetNearbyMonomer(x,y,z);

				if(pTargetBond && pBond->Activate(pTargetBond))
				{
					// If a suitable target mActin monomer is found, we tell the 
					// fActin network to remove it from its free bond container, 
					// replace its normal on condition with an external trigger, 
					// and bind it to the receptor's Tail. The receptor monomer that 
					// has just bound the mActin is then removed from its free bond 
					// container, and the new polymer added to the free polymer container.

					pIfActin->RemoveNearbyMonomer(pTargetBond);
//					pIfActin->SetBondExternalTriggerOn(pTargetBond);

					// Note the argument order: polymer's head bond then its tail bond
					
					aeActivePolymer* pPolymer = new aeActivePolymer(pTargetBond, *iterBond);

					std::cout << "Created new polymer " << pPolymer->GetId() << " out of bonds " << pTargetBond->GetId() << " " << (*iterBond)->GetId() << zEndl;

					// Add the newly-created polymer to the free polymer container,
					// and remove the receptor bond from the free bond container

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

// Function to check if any receptors that have already bound their first ligand can bind a second, third, etc.
// We simply scan for ligands within the capture zone of existing receptors but only allow binding if they already
// have one or more ligands bound up to the limit set by the active bond type.

void aeReceptorNetwork::BindNext()
{
	// Obtain a pointer to the IfActinAccessControl interface so that the receptor
	// monomers can locate and bind to mActin monomers. Note that we have to check
	// that the IACNAccessControl pointer grants access to the desired network:
	// not all networks can interact with each other.

	IfActinAccessControl* pIACN = dynamic_cast<IfActinAccessControl*>(GetIActiveSimBox()->GetIACNAccessControlFromType(aefActinNetwork::GetType()));

	if(pIACN && pIACN->IsAccessControlEnabled())
	{
		IfActinAccessFromReceptor* const pIfActin = pIACN->IsAccessAllowed(this);

		if(pIfActin)
		{
			// Iterate over each bound receptor molecule to see if it can bind 
			// a new mActin monomer: note that a receptor molecule is included as
			// part of the free active bonds list if it has either its Head or Tail
			// available to capture a new mActin monomer.

           ActivePolymerListIterator iterActiveReceptor = m_FreeActivePolymers.begin();
		   
			while(iterActiveReceptor != m_FreeActivePolymers.end())
			{
				aeActiveBond* pBond = (*iterActiveReceptor)->GetTailBond();

				double x = pBond->GetTailHeadBead()->GetXPos();
				double y = pBond->GetTailHeadBead()->GetYPos();
				double z = pBond->GetTailHeadBead()->GetZPos();

				// Find a nearby mActin monomer and, if it is closer to the 
				// receptor tail binding site than a given distance,add it 
				// to the growing filament. Note that the distance check uses
				// the empty receptor binding site at its tail, but the monomer
				// will be inserted between the head binding site and the back
				// of the growing actin filament.

				aeActiveBond* pTargetBond = pIfActin->GetNearbyMonomer(x,y,z);

				if(pTargetBond && pBond->Activate(pTargetBond))
				{
					// If a suitable target mActin monomer is found, we tell the 
					// fActin network to remove it from its free bond container, 
					// replace its normal on condition with an external trigger, 
					// and bind it to the receptor's Tail. The receptor monomer that 
					// has just bound the mActin is then removed from its free bond 
					// container, and the new polymer added to the free polymer container.

					pIfActin->RemoveNearbyMonomer(pTargetBond);
//					pIfActin->SetBondExternalTriggerOn(pTargetBond);
					
					(*iterActiveReceptor)->AddHeadBond(pTargetBond);
					
					std::cout << "Bound extra ligand to existing polymer " << (*iterActiveReceptor)->GetId() << " from bond " << pTargetBond->GetId()  << zEndl;

					iterActiveReceptor++;
				}
				else
				{
					iterActiveReceptor++;
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

// Command handler function to change the maximum separation at which
// two mActin monomer active bonds can associate to form a filament.
// The distance test is implemented in the aeBondProximityOn class.
//
// Note that we have to modify both the free active bonds and the ones
// already assembled into filaments.

void aeReceptorNetwork::SetProximityOnSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeReceptorBond* pBond = dynamic_cast<aeReceptorBond*>(*iterBond);

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
			aeReceptorBond* pReceptor = dynamic_cast<aeReceptorBond*>(pBond);

			pReceptor->SetOnRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeReceptorBond*>(pHeadBond)->SetOnRateSeparation(max);
	}

}

// Command handler function to change the maximum separation at which
// connected mActin monomers dissociate. The distance test is implemented 
// in the aeBondProximityOff class.
//
// Note that we have to modify both the free active bonds and the ones
// already assembled into filaments.

void aeReceptorNetwork::SetProximityOffSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond=m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeReceptorBond* pBond = dynamic_cast<aeReceptorBond*>(*iterBond);

		pBond->SetOffRateSeparation(max);
	}

	// Now the bonds already assembled into filaments

	for(ActivePolymerListIterator iterPoly = m_FreeActivePolymers.begin(); iterPoly != m_FreeActivePolymers.end(); iterPoly++)
	{
		aeActiveBond* pBond     = (*iterPoly)->GetTailBond();
		aeActiveBond* pHeadBond = (*iterPoly)->GetHeadBond();

		while(pBond != pHeadBond)
		{
			aeReceptorBond* pReceptor = dynamic_cast<aeReceptorBond*>(pBond);

			pReceptor->SetOffRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeReceptorBond*>(pHeadBond)->SetOffRateSeparation(max);
	}

}

// Command handler function to change the maximum separation at which
// connected mActin monomers associate via the head of a filament. 
// The distance test is implemented in the aeBondProximityOn class.

void aeReceptorNetwork::SetProximityHeadOnSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeReceptorBond* pBond = dynamic_cast<aeReceptorBond*>(*iterBond);

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
			aeReceptorBond* pReceptor = dynamic_cast<aeReceptorBond*>(pBond);

			pReceptor->SetHeadOnRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeReceptorBond*>(pHeadBond)->SetHeadOnRateSeparation(max);
	}
}

// Command handler function to change the maximum separation at which
// connected mActin monomers dissociate from the head of a filament. 
// The distance test is implemented in the aeBondProximityOff class.

void aeReceptorNetwork::SetProximityHeadOffSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeReceptorBond* pBond = dynamic_cast<aeReceptorBond*>(*iterBond);

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
			aeReceptorBond* pReceptor = dynamic_cast<aeReceptorBond*>(pBond);

			pReceptor->SetHeadOffRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeReceptorBond*>(pHeadBond)->SetHeadOffRateSeparation(max);
	}
}

// Command handler function to change the maximum separation at which
// connected mActin monomers associate via the tail of a filament. 
// The distance test is implemented in the aeBondProximityOn class.

void aeReceptorNetwork::SetProximityTailOnSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeReceptorBond* pBond = dynamic_cast<aeReceptorBond*>(*iterBond);

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
			aeReceptorBond* pReceptor = dynamic_cast<aeReceptorBond*>(pBond);

			pReceptor->SetTailOnRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeReceptorBond*>(pHeadBond)->SetTailOnRateSeparation(max);
	}
}

// Command handler function to change the maximum separation at which
// connected mActin monomers dissociate from the tail of a filament. 
// The distance test is implemented in the aeBondProximityOff class.

void aeReceptorNetwork::SetProximityTailOffSeparation(double max)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeReceptorBond* pBond = dynamic_cast<aeReceptorBond*>(*iterBond);

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
			aeReceptorBond* pReceptor = dynamic_cast<aeReceptorBond*>(pBond);

			pReceptor->SetTailOffRateSeparation(max);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the head that is not included in the above loop

		dynamic_cast<aeReceptorBond*>(pHeadBond)->SetTailOffRateSeparation(max);
	}
}

// Command handler functions to activate/inactivate the formation of new 
// filaments for all free receptor molecules. This does not stop existing 
// filaments from growing, but prevents new ones being nucleated.

void aeReceptorNetwork::ActivateReceptorNucleation()
{
	m_bNucleationActive = true;
}

void aeReceptorNetwork::InactivateReceptorNucleation()
{
	m_bNucleationActive = false;
}

// Function to completely disassemble all receptor-nucleated filaments. This
// is in contrast to the SeverReceptorFilaments() function that just removes the
// receptor molecules from their attached filaments.

void aeReceptorNetwork::DisassembleReceptorFilaments()
{
	// Obtain a pointer to the IfActinAccessControl interface so that the receptor
	// monomers can interact with mActin monomers. Note that we have to check
	// that the IACNAccessControl pointer grants access to the desired network:
	// not all networks can interact with each other.

	IfActinAccessControl* pIACN = dynamic_cast<IfActinAccessControl*>(GetIActiveSimBox()->GetIACNAccessControlFromType(aefActinNetwork::GetType()));

	if(pIACN && pIACN->IsAccessControlEnabled())
	{
		IfActinAccessFromReceptor* const pIfActin = pIACN->IsAccessAllowed(this);

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
				
				// Note that the tail of the polymer is always the receptor monomer,
				// and the head is always an actin monomer

				aeActiveBond* const pReceptorBond = pPoly->GetTailBond();
				aeActiveBond* pActinBond        = pPoly->GetHeadBond();

				while(pPoly->RemoveHeadBond())
				{
					// Released head bond is a single actin monomer so pass it 
					// to the actin network's free bond container.

					pIfActin->AddFreeBond(pActinBond);

					pActinBond = pPoly->GetHeadBond();
				}

				// Polymer has been disassembled, so return the unattached
				// receptor to the free bond container and destroy the empty 
				// receptor polymer instance

				AddFreeBond(pReceptorBond);

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
// assembled by receptor molecules. This only affects existing filaments, but does
// not prevent new ones from being nucleated.
//
// We iterate over all receptors, separate the bonds from 
// their fActin filament segments and add them to the free bond container. 
// The separated fActin filaments are added to the fActin network's free polymers 
// unless the filament is only a single actin monomer, in which case the monomer 
// is added to the fActin network's free bond container.

void aeReceptorNetwork::SeverReceptorFilaments()
{
	// Obtain a pointer to the IfActinAccessControl interface so that the receptors 
	// can interact with mActin monomers. Note that we have to check
	// that the IACNAccessControl pointer grants access to the desired network:
	// not all networks can interact with each other.

	IfActinAccessControl* pIACN = dynamic_cast<IfActinAccessControl*>(GetIActiveSimBox()->GetIACNAccessControlFromType(aefActinNetwork::GetType()));

	if(pIACN && pIACN->IsAccessControlEnabled())
	{
		IfActinAccessFromReceptor* const pIfActin = pIACN->IsAccessAllowed(this);

		if(pIfActin)
		{
			ActivePolymerListIterator iterAP = m_FreeActivePolymers.begin();

			while(iterAP != m_FreeActivePolymers.end())
			{
				aeActivePolymer* pPoly = (*iterAP);

				// Separate the tail bond from the polymer using RemoveTailBond(); 
				// this function returns true if the polymer still has at least 
				// 2 bonds, otherwise it returns false.
				
				// Note that the tail of the polymer is always the receptor monomer,
				// and the head is always an actin monomer

				aeActiveBond* const pReceptorBond = pPoly->GetTailBond();
				aeActiveBond* pActinBond        = pReceptorBond->GetHeadAdjacentBond();
		
				std::cout << "size of receptor polymer " << pPoly->GetSize() << zEndl;

				if(pPoly->RemoveTailBond())
				{
					// Remainder of polymer is an actin filament with at least 2
					// bonds in it, so we create an fActin polymer, using the 
					// constructor that takes a single, connected tail bond as 
					// its argument, and pass it to the fActin network to add to 
					// its free polymer container

					std::cout << "removing receptor from actin filament" << zEndl;
					std::cout << "size of remaining receptor polymer " << pPoly->GetSize() << zEndl;
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

				// Released tail bond is always a single receptor monomer so add
				// it to the receptor free polymer container, and destroy the 
				// receptor polymer instance

				AddFreeBond(pReceptorBond);
				delete pPoly;

				iterAP = m_FreeActivePolymers.erase(iterAP);
			}

			// Clear the free polymer container

			std::cout << "before dissolution receptor bond/polymer size = " << m_FreeActiveBonds.size()  << "  " << m_FreeActivePolymers.size() << zEndl;

			 m_FreeActivePolymers.clear();

			std::cout << "after dissolution receptor bond/polymer size = " << m_FreeActiveBonds.size()  << "  " << m_FreeActivePolymers.size() << zEndl;
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

void aeReceptorNetwork::SetFixedHeadOffRate(double rate)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeReceptorBond* pBond = dynamic_cast<aeReceptorBond*>(*iterBond);

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
			aeReceptorBond* pReceptor = dynamic_cast<aeReceptorBond*>(pBond);

	        pReceptor->SetFixedHeadOffRate(rate);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the bond that is not included in the above loop

		dynamic_cast<aeReceptorBond*>(pHeadBond)->SetFixedHeadOffRate(rate);
	}
}

// Command handler function to set a fixed probability for a monomer to detach
// from its neighbour via its tail connection. See the SetFixedHeadOffRate()
// function above for important details.

void aeReceptorNetwork::SetFixedTailOffRate(double rate)
{
	// Change free bonds first

	for(ActiveBondListIterator iterBond = m_FreeActiveBonds.begin(); iterBond != m_FreeActiveBonds.end(); iterBond++)
	{
		aeReceptorBond* pBond = dynamic_cast<aeReceptorBond*>(*iterBond);

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
			aeReceptorBond* pReceptor = dynamic_cast<aeReceptorBond*>(pBond);

	        pReceptor->SetFixedTailOffRate(rate);

			pBond = pBond->GetHeadAdjacentBond();
		}

		// Now modifiy the bond that is not included in the above loop

		dynamic_cast<aeReceptorBond*>(pHeadBond)->SetFixedTailOffRate(rate);
	}
}
