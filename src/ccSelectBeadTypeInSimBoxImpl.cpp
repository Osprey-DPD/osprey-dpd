/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectBeadTypeInSimBoxImpl.cpp: implementation of the ccSelectBeadTypeInSimBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "ccSelectBeadTypeInSimBoxImpl.h"
#include "ccSelectBeadTypeInSimBox.h"
#include "SimBox.h"
#include "Bead.h"
#include "CommandTarget.h"
#include "LogSelectBeadTypeInRegion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectBeadTypeInSimBoxImpl::ccSelectBeadTypeInSimBoxImpl()
{

}

ccSelectBeadTypeInSimBoxImpl::~ccSelectBeadTypeInSimBoxImpl()
{

}

// Function to implement a ccSelectBeadTypeInRegion command that collects 
// beads of the (single) specified type whose coordinates lie anywhere in
// SimBox.
//
// The beads are packaged into a CCommandTarget aggregate that can 
// receive subsequent ctApplyCommand commands.
//
// We assume that the data has been validated prior to this routine.

void ccSelectBeadTypeInSimBoxImpl::SelectBeadTypeInSimBox(const xxCommand* const pCommand)
{    
#if EnableTargetCommand == SimCommandEnabled
    
    const ccSelectBeadTypeInSimBox* const pCmd = dynamic_cast<const ccSelectBeadTypeInSimBox*>(pCommand);
    
	const zString label			= pCmd->GetTargetLabel();	// Label of command target
	const zString beadName		= pCmd->GetBeadName();		// Name of bead type
    
	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);
    
	// Find the integer type of the selected beads from their name
    
	const long beadType = pSimBox->GetBeadTypeFromName(beadName);
    
	// Loop over all the beads in the SimBox and store pointers to those whose
	// type matches the specified type
    
	BeadVector vTargetBeads;
	vTargetBeads.clear();
    
    AbstractBeadVector vAllBeads = pSimBox->GetAllBeadsInCNTCells();
	for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		if((*iterBead)->GetType() == beadType)
		{
			vTargetBeads.push_back(dynamic_cast<CBead*>(*iterBead));
		}
	}
    
	const long beadTotal = vTargetBeads.size();
    
	// Create and store a CCommandTarget to hold the beads and receive commands.
	// We pass in the bead type in case the targetBeads container is empty as
	// we still want to know what type of bead was specified.
    
	if(pSimBox->CreateCommandTarget(label, beadType, vTargetBeads))
	{
		 new CLogSelectBeadTypeInRegion(pSimBox->GetCurrentTime(), label, "SimBox", beadName, beadType, beadTotal);
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
    
#endif

}

