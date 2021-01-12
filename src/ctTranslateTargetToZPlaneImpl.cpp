/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctTranslateTargetToZPlaneImpl.cpp: implementation of the ctTranslateTargetToZPlaneImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctTranslateTargetToZPlaneImpl.h"
#include "ctTranslateTargetToZPlane.h"
#include "SimBox.h"
#include "CNTCell.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "taPlanarAnchorForce.h"
#include "LogctTranslateTargetToZPlane.h"
#include "LogctEmptyTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctTranslateTargetToZPlaneImpl::ctTranslateTargetToZPlaneImpl()
{
}

ctTranslateTargetToZPlaneImpl::~ctTranslateTargetToZPlaneImpl()
{

}

// Command handler function to translate all beads in a command target so that they lie within a specified plane 
// whose normal is in the Z direction. We just replace all the Z coordinates of the beads by the user-supplied Z coordinate,
// which is entered as a fraction of the SimBox Z dimension. Then we remove each bead from its CNT cell and add it to the new 
// cell. Note that this can lead to overlap of beads, but in DPD 
// this is not a problem as all forces are finite. But it may cause transient oscillations in bead density, so a small 
// time-step is advisable.

void ctTranslateTargetToZPlaneImpl::TranslateTargetToZPlane(const xxCommand* const pCommand)
{
	const ctTranslateTargetToZPlane* const pCmd = dynamic_cast<const ctTranslateTargetToZPlane*>(pCommand);

	const zString targetLabel = pCmd->GetTargetLabel();

	const double zc = pCmd->GetZCentre();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Convert relative coordinates to absolute ones and find the integer CNT cell index for the Z component

	double fzc = zc*pSimBox->GetSimBoxZLength();
	long  ifzc = static_cast<long>(fzc/pSimBox->GetSimBoxZCellWidth());

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(targetLabel);
	
	std::cout << "Found target " << targetLabel << " containing " << pCmdTarget->GetBeadTotal() << " beads, and translating them to Z = " << fzc << " " << ifzc << zEndl;
	

	// Now we have the target, iterate over all of its beads and replace their z coordinates
	// with the specified value. Note that because we use a Composite pattern for the
	// command targets, we don't need to know whether it is composite or
	// simple as the GetBeads() function just returns all beads in the target.
	// However, the data on bead type, bead name and original bead type
	// is not applicable to composite targets because they may contain
	// a multitude of bead types.

	long oldIndex = 0;
	long newIndex = 0;
	long ix, iy, iz;

	if(pCmdTarget)
	{
		BeadVector vTargetBeads = pCmdTarget->GetBeads();

		if(!vTargetBeads.empty())
		{
			for(BeadVectorIterator iterBead=vTargetBeads.begin(); iterBead!=vTargetBeads.end(); iterBead++)
			{
                               ix = static_cast<long>((*iterBead)->GetXPos()/pSimBox->GetSimBoxXCellWidth());
                               iy = static_cast<long>((*iterBead)->GetYPos()/pSimBox->GetSimBoxYCellWidth());
                               iz = static_cast<long>((*iterBead)->GetZPos()/pSimBox->GetSimBoxZCellWidth());
				 		
				 oldIndex = pSimBox->GetSimBoxXCellNo()*(pSimBox->GetSimBoxYCellNo()*iz+iy) + ix;
				 
				 if(oldIndex < pSimBox->GetCNTCellTotal())
                 {
		             pSimBox->m_vCNTCells[oldIndex]->RemoveBeadFromCell(*iterBead);
                 }
				 
				 // What should I do if the index is wrong???
				 
                 (*iterBead)->SetZPos(fzc);  // new z coordinate
				 		
				 newIndex = pSimBox->m_CNTXCellNo*(pSimBox->m_CNTYCellNo*ifzc+iy) + ix;
				 
				 if(newIndex < pSimBox->m_CNTCellTotal)
                 {
		             pSimBox->m_vCNTCells[newIndex]->AddBeadtoCell(*iterBead);
                 }
			}
			
	            new CLogctTranslateTargetToZPlane(pSimBox->GetCurrentTime(), targetLabel, zc);
		}
		else
		{
			new CLogctEmptyTarget(pSimBox->GetCurrentTime(), targetLabel);
		}
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
