/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccUnchargeBeadByTypeImpl.cpp: implementation of the ccUnchargeBeadByTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimMiscellaneousFlags.h"
#include "ccUnchargeBeadByTypeImpl.h"

#if EnableMiscClasses == SimMiscEnabled
  #include "ccUnchargeBeadByType.h"
#endif

#include "SimBox.h"
#include "Bead.h"
#include "BeadChargeWrapper.h"
#include "LogChargeBeadType.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccUnchargeBeadByTypeImpl::ccUnchargeBeadByTypeImpl()
{
}

ccUnchargeBeadByTypeImpl::~ccUnchargeBeadByTypeImpl()
{

}

// Command handler to uncharge a specified bead type. This version uses the
// bead's numeric identifier to determine which beads lose their charge. The 
// other version, ccChargeBeadType, uses the bead's string identifier or name.

void ccUnchargeBeadByTypeImpl::UnchargeBeadByType(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccUnchargeBeadByType* const pCmd = dynamic_cast<const ccUnchargeBeadByType*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Unpack the data identifying the particular bead type that is being uncharged.

	const long type		= pCmd->GetBeadType();

	const zString name  = pSimBox->GetBeadNameFromType(type);

	// Only delete the bead type if it has already been charged

	if(find(pSimBox->m_vChargedBeadTypes.begin(), pSimBox->m_vChargedBeadTypes.end(), type) != pSimBox->m_vChargedBeadTypes.end())
	{
		pSimBox->m_vChargedBeadTypes.erase(find(pSimBox->m_vChargedBeadTypes.begin(), pSimBox->m_vChargedBeadTypes.end(), type));
	
		// Loop over the charged bead container removing those whose type matches 
		// that in the command. First we delete each CBeadChargeWrapper object with
		// the specified bead type, and then we erase the section of the container
		// that used to contain those beads. Because the bead pointers are stored 
		// in a list, we can delete a contiguous range of elements without invalidating
		// the remainder of the container.


		// Note the absence of the increment step because we use erase()  
		// to remove beads of the specified type, and increment it manually when
		// the type is different

		for(ChargedBeadListIterator iterBead=pSimBox->m_lAllChargedBeads.begin(); iterBead!=pSimBox->m_lAllChargedBeads.end(); )
		{
			if((*iterBead)->GetType() == type)
			{
				delete *iterBead;

				iterBead = pSimBox->m_lAllChargedBeads.erase(iterBead);
			}
			else
				iterBead++;
		}

		// If there are no longer any charged beads, toggle the SimState flag off

		if(pSimBox->m_lAllChargedBeads.empty())
			pSimBox->SetBeadChargeOn(false);
		
		// Log a message indicating that the charged bead type has been removed.
		// Note the zero values for the strength and range. These are not contained
		// in the ccUnchargeBeadType command.

		 new CLogChargeBeadType(pSimBox->GetCurrentTime(), name, type, 0.0, 0.0, false);
	}	
#endif
}
