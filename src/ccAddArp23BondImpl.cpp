/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccAddArp23BondImpl.cpp: implementation of the ccAddArp23BondImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccAddArp23BondImpl.h"
#include "ccAddArp23Bond.h"
#include "aeArp23Bond.h"
#include "SimBox.h"
#include "IModifySimStatePolymers.h"
#include "LogActiveBondAdded.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccAddArp23BondImpl::ccAddArp23BondImpl()
{
}

ccAddArp23BondImpl::~ccAddArp23BondImpl()
{

}

// Command handler to create a new type of active bond that nucleates the formation
// of fActin filaments. The Arp2/3 complex takes two mAcitn monomers and under 
// certain conditions binds them to form a new filament. It can also create new
// daughter branches at the leading end of an existing fActin filament.
//

void ccAddArp23BondImpl::AddArp23Bond(const xxCommand* const pCommand)
{
#if EnableACNCommand == SimCommandEnabled
	const ccAddArp23Bond* const pCmd = dynamic_cast<const ccAddArp23Bond*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Base class data on the active cell network
	
	const zString headMonomerName	= pCmd->GetHeadMonomerName();
	const zString tailMonomerName	= pCmd->GetTailMonomerName();
	const long headMonomerType	= pSimBox->GetPolymerTypeFromName(headMonomerName);
	const long tailMonomerType	= pSimBox->GetPolymerTypeFromName(tailMonomerName);

/*
	const long xCellNo				= pCmd->GetXCellNo();
	const long yCellNo				= pCmd->GetYCellNo();
	const long zCellNo				= pCmd->GetZCellNo();

	// now the data on the arp23 bonds

	const double k2				= pCmd->GetSpringConstant();
	const double l0				= pCmd->GetUnstretchedLength();
	const double k3				= pCmd->GetBendingModulus();
	const double phi0				= pCmd->GetPhi0();

*/

	// Not implemented yet

	if(true)
	{
		 new CLogActiveBondAdded(pSimBox->GetCurrentTime(), "Arp23", headMonomerName, tailMonomerName, headMonomerType, tailMonomerType);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);

	}
#endif
}
