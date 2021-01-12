/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccToggleDPDBeadConservativeForcesImpl.cpp: implementation of the ccToggleDPDBeadConservativeForcesImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccToggleDPDBeadConservativeForcesImpl.h"
#include "ccToggleDPDBeadConservativeForces.h"
#include "SimBox.h"
#include "IModifySimStateIntegration.h"
#include "LogToggleDPDBeadConservativeForces.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccToggleDPDBeadConservativeForcesImpl::ccToggleDPDBeadConservativeForcesImpl()
{
}

ccToggleDPDBeadConservativeForcesImpl::~ccToggleDPDBeadConservativeForcesImpl()
{

}

// Command handler to toggle the DPD conservative bead-bead interactions on and off.
// This removes all distinction between bead types and leaves only the 
// random and dissipative forces between beads. External forces may still be
// directed at specific bead types and their effects are included in the 
// equations of motion.

void ccToggleDPDBeadConservativeForcesImpl::ToggleDPDBeadConservativeForces(const xxCommand* const pCommand)
{
//	const ccToggleDPDBeadConservativeForces* const pCmd = dynamic_cast<const ccToggleDPDBeadConservativeForces*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);
	
	const bool bZeroForce = pSimBox->IModifyIntegration()->ToggleDPDBeadConservativeForces();
		
// When writing out the data don't forget to #include CCNTCell.h above and
// make this class a friend of CCNTCell

/*	for(long row=0; row!=CCNTCell::m_vvConsInt.size(); row++ )
	{
		for(long col=0; col<CCNTCell::m_vvConsInt.size(); col++)
		{
			std::cout << CCNTCell::m_vvConsInt.at(row).at(col) << " ";
		}
		std::cout << zEndl;
	}

	for( row=0; row!=CCNTCell::m_vvConsIntBackup.size(); row++ )
	{
		for(long col=0; col<CCNTCell::m_vvConsIntBackup.size(); col++)
		{
			std::cout << CCNTCell::m_vvConsIntBackup.at(row).at(col) << " ";
		}
		std::cout << zEndl;
	} */

	// This command cannot fail

	 new CLogToggleDPDBeadConservativeForces(pSimBox->GetCurrentTime(), bZeroForce);
}
