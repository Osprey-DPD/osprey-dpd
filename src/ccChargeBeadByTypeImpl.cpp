/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccChargeBeadByTypeImpl.cpp: implementation of the ccChargeBeadByTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimMiscellaneousFlags.h"
#include "ccChargeBeadByTypeImpl.h"

#if EnableMiscClasses == SimMiscEnabled
  #include "ccChargeBeadByType.h"
#endif

#include "SimBox.h"
#include "Bead.h"
#include "BeadChargeWrapper.h"
#include "LogChargeBeadType.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccChargeBeadByTypeImpl::ccChargeBeadByTypeImpl()
{
}

ccChargeBeadByTypeImpl::~ccChargeBeadByTypeImpl()
{

}

// Command handler to charge up a specified bead type. This version uses the
// bead's numeric identifier to determine which beads acquire a charge. The 
// other version, ccChargeBeadType, uses the bead's string identifier or name.

void ccChargeBeadByTypeImpl::ChargeBeadByType(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccChargeBeadByType* const pCmd = dynamic_cast<const ccChargeBeadByType*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Unpack the data identifying the particular bead that is charged by the command.
	// If the simulation type is MD we normalize the force constant by the
	// energy scale taken from the first species' LJ potential depth, and 
	// the range by the first LJ potential range. Hence, these parameters cannot
	// be const as is usual for command data.

	const long type		= pCmd->GetBeadType();
	double strength		= pCmd->GetStrength();
	double range		= pCmd->GetRange();
	double kappa		= 0.0;

	const zString name  = pSimBox->GetBeadNameFromType(type);
	const long total    = pSimBox->GetBeadTotalForType(type);

	// Only create charged bead wrappers if the specified bead type has not yet 
	// been charged, and some beads of that type exist. We use the find algorithm 
	// to see if the bead type occurs in the current set of charged bead types.

	if(total > 0 && find(pSimBox->m_vChargedBeadTypes.begin(), pSimBox->m_vChargedBeadTypes.end(), type) == pSimBox->m_vChargedBeadTypes.end())
	{
		pSimBox->m_vChargedBeadTypes.push_back(type);

		// How do I normalize the MD parameters???

#if SimIdentifier == MD
		strength *= (GetLengthScale()*GetLengthScale()/GetEnergyScale());
		range    /= GetLengthScale();
#endif

		// If the user has set the range to less than 10**-08, we treat this as 
		// having a strength of zero.

		if(range < 0.00000001)
		{
			strength = 0.0;
			kappa	 = 0.0;
		}
		else
		{
			kappa = 1.0/range;
		}

		// Loop over all beads adding those of the specified type to the 
		// m_lAllChargedBeads container. We create a wrapper object from the 
		// CBeadChargeWrapper class, store the original CAbstractBead within it, 
		// and use forwarding functions to access the properties of the inner bead.

		long beadTotal = 0;

        AbstractBeadVector vAllBeads = pSimBox->GetAllBeadsInCNTCells();
	    for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == type)
			{
				beadTotal++;

				CBeadChargeWrapper* pChargedBead = new CBeadChargeWrapper(*iterBead, strength, range, kappa);

				pSimBox->m_lAllChargedBeads.push_back(pChargedBead);
			}
		}

		pSimBox->m_vChargedBeadTotals.push_back(beadTotal);	// Store the number of newly-charged beads

		// Tell the SimState that charged beads are being used if this is the first time

		if(!pSimBox->IsBeadChargeOn())
			pSimBox->SetBeadChargeOn(true);

		// Log a message indicating that the bead type has been charged.

		CLogChargeBeadType* pMsg = new CLogChargeBeadType(pSimBox->GetCurrentTime(), name, type, strength, range, true);
	}
#endif
}
