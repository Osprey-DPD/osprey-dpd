/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectPolymerTypeInSimBoxImpl.cpp: implementation of the ccSelectPolymerTypeInSimBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectPolymerTypeInSimBoxImpl.h"
#include "ccSelectPolymerTypeInSimBox.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Polymer.h"
#include "CommandTargetPolymer.h"
#include "LogSelectPolymerTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectPolymerTypeInSimBoxImpl::ccSelectPolymerTypeInSimBoxImpl()
{
}

ccSelectPolymerTypeInSimBoxImpl::~ccSelectPolymerTypeInSimBoxImpl()
{
}

// Command handler to create a command target out of all polymers of a given type
// anywhere in the SimBox.

void ccSelectPolymerTypeInSimBoxImpl::SelectPolymerTypeInSimBox(const xxCommand* const pCommand)
{
	const ccSelectPolymerTypeInSimBox* const pCmd = dynamic_cast<const ccSelectPolymerTypeInSimBox*>(pCommand);

	const zString label			= pCmd->GetTargetLabel();	 // Label of command target
	const zString polyName		= pCmd->GetPolymerName();	 // Name of polymer type

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Find the integer type of the selected polymers and then collect all 
    // polymers of this type into a command target. Note that we search all
    // polymers instead of trying to get the contiguous set of polymer pointers;
    // this allows there to be a variable number of polymers of each type 
    // in future versions of the code.

	const long polyType = pSimBox->GetPolymerTypeFromName(polyName);

	PolymerVectorIterator iterPoly;	// Iterator used several times below

	PolymerVector vAllPolymers = pSimBox->GetISimBox()->GetPolymers();

	PolymerVector vSourcePolymers;
	vSourcePolymers.clear();

	for(iterPoly=vAllPolymers.begin(); iterPoly!=vAllPolymers.end(); iterPoly++)
	{
		if((*iterPoly)->GetType() == polyType)
			vSourcePolymers.push_back(*iterPoly);
	}

	const long polyTotal = vSourcePolymers.size();

	// Create and store a CCommandTargetPolymer that can receive commands.
	// We pass in the polymer type in case the vTargetPolymers container 
	// is empty as we still want to know what type of polymer was specified.

	if(pSimBox->CreateCommandTargetPolymer(label, polyType, vSourcePolymers))
	{
#if EnableParallelCommands == SimMPSEnabled
        if(xxParallelBase::GlobalGetRank() == 0)
		{
		     new CLogSelectPolymerTypeInRegion(pSimBox->GetCurrentTime(), label, "SimBox", polyName, polyType, polyTotal);
		}
	}
	else
	{
        if(xxParallelBase::GlobalGetRank() == 0)
		{
		     new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
		}
	}
#else

		 new CLogSelectPolymerTypeInRegion(pSimBox->GetCurrentTime(), label, "SimBox", polyName, polyType, polyTotal);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}

#endif
}

