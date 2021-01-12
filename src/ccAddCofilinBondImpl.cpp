/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccAddCofilinBondImpl.cpp: implementation of the ccAddCofilinBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccAddCofilinBondImpl.h"
#include "ccAddCofilinBond.h"
#include "aeCofilinBond.h"
#include "SimBox.h"
#include "aeActiveSimBox.h"
#include "aeCofilinNetwork.h"
#include "IModifySimStatePolymers.h"
#include "LogActiveBondAdded.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccAddCofilinBondImpl::ccAddCofilinBondImpl()
{
}

ccAddCofilinBondImpl::~ccAddCofilinBondImpl()
{

}

// Command handler to create a new type of active bond to allow cofilin
// monomers to bind to the ends of filamentous actin and speed up the
// diassociation rate.
//
// The polymer that represents the monomers in the active bonds must exist in
// the CSimState's data before executing this command. The ccAddXXXBond command 
// is used to perform this check. The cofilin active bonds must have an 
// architecture of the form A-B, where A (the tail of the active bond) is the
// cofilin monomer and B (the head of the active bond) is the terminal monomer
// of an fActin filament.  The head and tail beads in the monomers may be any 
// two uniquely identifiable beads in the polymers.
//
// This active bond uses a simple Hooke's law for the spring force applied
// to the beads in the head and tail monomers connected by the bond.

void ccAddCofilinBondImpl::AddCofilinBond(const xxCommand* const pCommand)
{
#if EnableACNCommand == SimCommandEnabled
/*
	const ccAddCofilinBond* const pCmd = dynamic_cast<const ccAddCofilinBond*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Base class data on the active cell network

	const long xCellNo				= pCmd->GetXCellNo();
	const long yCellNo				= pCmd->GetYCellNo();
	const long zCellNo				= pCmd->GetZCellNo();

	// now the data on the actin bonds

	const zString headMonomerName	= pCmd->GetHeadMonomerName();
	const zString tailMonomerName	= pCmd->GetTailMonomerName();
	const double k2					= pCmd->GetSpringConstant();
	const double l0					= pCmd->GetUnstretchedLength();
	const double k3					= pCmd->GetBendingModulus();
	const double phi0				= pCmd->GetPhi0();

	const long headMonomerType		= pSimBox->GetPolymerTypeFromName(headMonomerName);
	const long tailMonomerType		= pSimBox->GetPolymerTypeFromName(tailMonomerName);

	// Create a new cofilin bond to wrap each polymer of the type "tailMonomerType",
	// and store the bonds in the FreeActiveBonds container. This holds all active
	// bonds of all types. They are held by pointers to their aeActiveBond 
	// abstract base class. We obtain the polymer pointers from the CSimState
	// and find the first polymer of the required type by jumping over all
	// the polymers using the GetPolymerTotalForType() to obtain the number
	// of each type. This relies on the polymer ids being assigned sequentially
	// by type: if this assumption is invalidated by creating, destroying or
	// changing polymer types this routine must be changed accordingly.

	// Note that it is the tail monomer that defines the active bond, and the
	// head monomer is bound to the bond when it is activated. The two types 
	// may be identical, as in the aeFActin class, but they need not be.

	const long polymerTypeTotal = pSimBox->GetPolymerTypeTotal();

	long firstMonomerId = 0;
	long monomerTotal   = 0;
	bool bFound         = false;

	for(long type=0; type<polymerTypeTotal; type++)
	{
		if(!bFound)
		{
			long typeTotal = pSimBox->GetPolymerTotalForType(type);

			if(type == tailMonomerType)
			{
				bFound = true;
				monomerTotal = typeTotal;
			}
			else
			{
				firstMonomerId += typeTotal;
			}
		}
	}

	// Check that the shadow SimBox exists, and that no fActin active network
	// has yet been created. If either condition is not met, the command fails.
	// Then check that there actually are some polymers of the required type.
	// If this is not the case, the command fails.

	aeActiveSimBox* const pShadowSimBox = pSimBox->GetActiveSimBox();


	if(pShadowSimBox && !pShadowSimBox->IsfActinNetworkPresent() && bFound && monomerTotal > 0)
	{
		// Tell the shadow SimBox to create the fActin network and pass it the
		// number of CNT cells per active cell in each dimension.

		pShadowSimBox->CreatefActinNetwork(xCellNo, yCellNo, zCellNo);

		// Now we have the number of monomers and the id of the first one we can 
		// wrap each of them in a newly-created aeActiveBond instance. No active 
		// polymers are created as a result of this command. They can only be created 
		// when an active bond finds a second monomer to form a bond with. 

		ActiveBondList lFreeActiveBonds;

		lFreeActiveBonds.clear();

		long monomerId = firstMonomerId;

		for(long id=0; id<monomerTotal; id++)
		{
			CPolymer* const pMonomer = pSimBox->IModifyPolymers()->GetPolymers().at(monomerId);

			aefActinBond* pBond = new aefActinBond(pMonomer, k2, l0, k3, phi0);

			lFreeActiveBonds.push_back(pBond);

			monomerId++;
		}

		long mytotal = lFreeActiveBonds.size();

		std::cout << "new fActin active bonds added " << mytotal << zEndl;

		// Pass the container of active bonds to the network so that it can
		// sort the bonds into the appropriate cells

		pShadowSimBox->GetfActinNetwork()->AssignActiveBondsToCells(lFreeActiveBonds);

		// Tell the CSimState that active bonds are present and log the 
		// command's execution. We don't pass the bond force parameters as
		// they are different for each active bond type, and it is complicated
		// to accomodate all possible force laws in the message.

		pSimBox->SetActiveBondsOn(true);

		 new CLogActiveBondAdded(pSimBox->GetCurrentTime(), "Cofilin", headMonomerName, tailMonomerName,
																						headMonomerType, tailMonomerType);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);

	}
	*/
#endif
}
