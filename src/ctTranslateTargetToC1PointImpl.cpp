/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctTranslateTargetToC1PointImpl.cpp: implementation of the ctTranslateTargetToC1PointImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctTranslateTargetToC1PointImpl.h"
#include "ctTranslateTargetToC1Point.h"
#include "SimBox.h"
#include "CNTCell.h"
#include "Bead.h"
#include "Polymer.h"
#include "CommandTarget.h"
#include "CommandTargetPolymer.h"
#include "CommandTargetComposite.h"
#include "LogctTranslateTargetToC1Point.h"
#include "LogctEmptyTarget.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctTranslateTargetToC1PointImpl::ctTranslateTargetToC1PointImpl()
{
}

ctTranslateTargetToC1PointImpl::~ctTranslateTargetToC1PointImpl()
{

}

// Command handler function to translate all beads in a command target to a given point with C1 symmetry.
// We check that we have a polymer target because if not we cannot identify which beads belong to  which polymer so that we 
// can rigidly translate them. If we don't have a polymer target, we simply move all beads to the fixed point.
//
// We translate the head bead of each polymer to a position with user-supplied coordinates,
// which are entered as a fraction of the SimBox Z dimension, and then rigidly translate the remaining beads in the polymer. 
// Then we remove each bead from its CNT cell and add it to the new cell. Note that this can lead to overlap of beads, but in DPD 
// this is not a problem as all forces are finite. But it may cause transient oscillations in bead density, so a small 
// time-step is advisable.

void ctTranslateTargetToC1PointImpl::TranslateTargetToC1Point(const xxCommand* const pCommand)
{
	const ctTranslateTargetToC1Point* const pCmd = dynamic_cast<const ctTranslateTargetToC1Point*>(pCommand);

	const zString targetLabel = pCmd->GetTargetLabel();

	const double xc = pCmd->GetXCentre();
	const double yc = pCmd->GetYCentre();
	const double zc = pCmd->GetZCentre();

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Convert relative coordinates to absolute ones and find the integer CNT cell index for the Z component

	double fxc = xc*pSimBox->GetSimBoxXLength();
	double fyc = yc*pSimBox->GetSimBoxYLength();
	double fzc = zc*pSimBox->GetSimBoxZLength();

	long  ifxc = static_cast<long>(fxc/pSimBox->GetSimBoxXCellWidth());
	long  ifyc = static_cast<long>(fyc/pSimBox->GetSimBoxYCellWidth());
	long  ifzc = static_cast<long>(fzc/pSimBox->GetSimBoxZCellWidth());

	// Get the command target from the target list and check if it is a polymer target

	CCommandTargetNode* pCmdTarget = pSimBox->GetCommandTarget(targetLabel);
    
    CCommandTargetPolymer* pPolymerTarget = dynamic_cast<CCommandTargetPolymer*>(pCmdTarget);
	
    if(pPolymerTarget)
    {
	    std::cout << "Processor " << xxParallelBase::GlobalGetRank() << " Found polymer target " << targetLabel << " containing " << pCmdTarget->GetPolymerTotal() << " polymers, and translating them to C1   point = " << fxc << " " << fyc << " " << fzc << " " << ifxc << " " << ifyc << " " << ifzc << zEndl;
	}
    else if(pCmdTarget)
    {        
	    std::cout << "Found bead target " << targetLabel << " containing " << pCmdTarget->GetBeadTotal() << " beads, and translating them to C1   point = " << fxc << " " << fyc << " " << fzc << " " << ifxc << " " << ifyc << " " << ifzc << zEndl;
    }

	// Now we have the target, iterate over all of its beads and replace their coordinates
	// with the specified value. Note that because we use a Composite pattern for the
	// command targets, we don't need to know whether it is composite or
	// simple as the GetBeads() function just returns all beads in the target.
	// However, the data on bead type, bead name and original bead type
	// is not applicable to composite targets because they may contain
	// a multitude of bead types.

#if EnableParallelCommands == SimMPSDisabled
    long oldIndex = 0;
    long newIndex = 0;
    long ix, iy, iz;
    bool bFirstBead = true;  // Flag used to indicate whether we are placing the first bead or subsequent beads

    if(pPolymerTarget)  // We have a polymer target
    {
        PolymerVector vTargetPolymers = pPolymerTarget->GetPolymers();
        
		if(!vTargetPolymers.empty())
		{			
			// Note that in the parallel code there may be polymers in a target that moved into the owning processor's space but are incomplete 
			// at the time the target is created! We need to handle these extended polymers properly.
			
            for(PolymerVectorIterator iterPoly=vTargetPolymers.begin(); iterPoly!=vTargetPolymers.end(); iterPoly++)
            {
                bFirstBead = true;
                
                // For each polymer we iterate over its beads, placing the first one at the user-specified point and rigidly 
                // placing the remaining beads around it.
                				
                BeadVector vBeads = (*iterPoly)->GetBeads();

                const double x0 = vBeads.front()->GetXPos();
                const double y0 = vBeads.front()->GetYPos();
                const double z0 = vBeads.front()->GetZPos();
                
			    for(BeadVectorIterator iterBead=vBeads.begin(); iterBead!=vBeads.end(); iterBead++)
			    {
                    if(bFirstBead)
                    {
                        bFirstBead = false;

                        ix = static_cast<long>(x0/pSimBox->GetSimBoxXCellWidth());
                        iy = static_cast<long>(y0/pSimBox->GetSimBoxYCellWidth());
                        iz = static_cast<long>(z0/pSimBox->GetSimBoxZCellWidth());

                        newIndex = pSimBox->m_CNTXCellNo*(pSimBox->m_CNTYCellNo*ifzc+ifyc) + ifxc;  // first bead is moved to the fixed point

                        (*iterBead)->SetXPos(fxc);  // new x coordinate
                        (*iterBead)->SetYPos(fyc);  // new y coordinate
                        (*iterBead)->SetZPos(fzc);  // new z coordinate
                    }
                    else
                    {
                        // Find the displacement from the polymer's head to the current bead and the CNT cell index containing the current bead
                        
                        double dx = (*iterBead)->GetXPos() - x0;
                        double dy = (*iterBead)->GetYPos() - y0;
                        double dz = (*iterBead)->GetZPos() - z0;
						                        
                        ix = static_cast<long>((*iterBead)->GetXPos()/pSimBox->GetSimBoxXCellWidth());
                        iy = static_cast<long>((*iterBead)->GetYPos()/pSimBox->GetSimBoxYCellWidth());
                        iz = static_cast<long>((*iterBead)->GetZPos()/pSimBox->GetSimBoxZCellWidth());
                        
                        // Now set the bead's new position to the fixed point plus its displacement from the first bead
                        
                        double x1 = fxc + dx;
                        double y1 = fyc + dy;
                        double z1 = fzc + dz;
                        
                        long ix1 = static_cast<long>(x1/pSimBox->GetSimBoxXCellWidth());
                        long iy1 = static_cast<long>(y1/pSimBox->GetSimBoxYCellWidth());
                        long iz1 = static_cast<long>(z1/pSimBox->GetSimBoxZCellWidth());
                        
                        newIndex = pSimBox->m_CNTXCellNo*(pSimBox->m_CNTYCellNo*iz1+iy1) + ix1;  // subsequent beads moved to fixed point + displacement

                        (*iterBead)->SetXPos(x1);  // new x coordinate
                        (*iterBead)->SetYPos(y1);  // new y coordinate
                        (*iterBead)->SetZPos(z1);  // new z coordinate
                    }
                    
                    // Remove the bead from its original CNT cell and add it to the new one
                    
                    oldIndex = pSimBox->GetSimBoxXCellNo()*(pSimBox->GetSimBoxYCellNo()*iz+iy) + ix;
                
                    if(oldIndex < pSimBox->GetCNTCellTotal())
                    {
                        pSimBox->m_vCNTCells[oldIndex]->RemoveBeadFromCell(*iterBead);
                    }
                
                    // What should I do if the index is wrong???
                 
                    if(newIndex < pSimBox->m_CNTCellTotal)
                    {
                        pSimBox->m_vCNTCells[newIndex]->AddBeadtoCell(*iterBead);
                    }
			    }
            
			}
			
	            new CLogctTranslateTargetToC1Point(pSimBox->GetCurrentTime(), targetLabel, xc, yc, zc);
		}
		else
		{
			new CLogctEmptyTarget(pSimBox->GetCurrentTime(), targetLabel);
		}
    }
	else if(pCmdTarget) // we have a bead target
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
				 
                (*iterBead)->SetXPos(fxc);  // new x coordinate
                (*iterBead)->SetYPos(fyc);  // new y coordinate
                (*iterBead)->SetZPos(fzc);  // new z coordinate
				 		
				 newIndex = pSimBox->m_CNTXCellNo*(pSimBox->m_CNTYCellNo*ifzc+ifyc) + ifxc;
				 
				 if(newIndex < pSimBox->m_CNTCellTotal)
                 {
		             pSimBox->m_vCNTCells[newIndex]->AddBeadtoCell(*iterBead);
                 }
			}
			
	            new CLogctTranslateTargetToC1Point(pSimBox->GetCurrentTime(), targetLabel, xc, yc, zc);
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
	
#endif
}
