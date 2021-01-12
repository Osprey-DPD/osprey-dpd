/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctExtractBeadTypesIntoCompositeTargetImpl.cpp: implementation of the ctExtractBeadTypesIntoCompositeTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "aaRegionToType.h"
#include "ctExtractBeadTypesIntoCompositeTargetImpl.h"
#include "ctExtractBeadTypesIntoCompositeTarget.h"
#include "SimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "Bead.h"
#include "LogctEmptyTarget.h"
#include "LogctExtractBeadTypesIntoCompositeTarget.h"
#include "LogTextMessage.h"


	using std::sort;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctExtractBeadTypesIntoCompositeTargetImpl::ctExtractBeadTypesIntoCompositeTargetImpl()
{
}

ctExtractBeadTypesIntoCompositeTargetImpl::~ctExtractBeadTypesIntoCompositeTargetImpl()
{

}

// Command handler to extract all beads in a command target into a composite
// target that contains one simple target per bead type.

void ctExtractBeadTypesIntoCompositeTargetImpl::ExtractBeadTypesIntoCompositeTarget(const xxCommand* const pCommand)
{
	const ctExtractBeadTypesIntoCompositeTarget* const pCmd = dynamic_cast<const ctExtractBeadTypesIntoCompositeTarget*>(pCommand);

	const zString	sourceLabel	= pCmd->GetTargetLabel();
	const zString   destLabel   = pCmd->GetDestinationLabel();

	const zString destCompositeLabel = destLabel + "Comp";

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	// Get the command target from the target list

	CCommandTargetNode*   pCmdTarget = pSimBox->GetCommandTarget(sourceLabel);

	if(pCmdTarget)
	{
		// Now we have the command target we loop over all beads contained in it 
		// and store their types in a container. Each bead type is aggregated 
		// into a separate target.  Note that if the target is 
		// composite, the GetBeads() function recurses into all its contained 
		// targets. Also note that the source target may be a geometric or 
		// analysis target, not necessarily a bead or polymer target.
		// If the target is empty we issue a warning message but not an error.

		BeadVector vTargetBeads = pCmdTarget->GetBeads();

		if(!vTargetBeads.empty())
		{
			// We sort the container of beads using their types and iterate over 
			// the source target's beads creating a new target to hold each bead
			// type found. If the source target is a simple bead target or we
			// are dealing with the final bead type in the target, we create one
			// last target outside of the loop.

                       CCommandTargetNode* pTarget;
			LongTargetMap mTargets;
			mTargets.clear();

			sort(vTargetBeads.begin(), vTargetBeads.end(), aaBeadTypeCompLess());

			// Now that the beads are in order of their type we can iterate through
			// them and add them to a container before passing the container to
			// the new target's constructor.

			BeadVector vTempBeads;
			vTempBeads.clear();

			long currentType = vTargetBeads.at(0)->GetType();

			for(BeadVectorIterator ib=vTargetBeads.begin(); ib!=vTargetBeads.end(); ib++)
			{
				const long type = (*ib)->GetType();

				if(type == currentType)
				{
					vTempBeads.push_back(*ib);
				}
				else
				{
					const zString beadName = pSimBox->GetBeadNameFromType(currentType);
					const zString targetLabel = destLabel + beadName + pSimBox->ToString(currentType);

					if(pSimBox->CreateCommandTarget(targetLabel, currentType, vTempBeads))
					{
						pTarget = pSimBox->GetCommandTarget(targetLabel);
						mTargets.insert(zPairLongTarget(currentType, pTarget));

						// Now empty the temporary bead container ready for the next target

						vTempBeads.clear();
						vTempBeads.push_back(*ib);

						currentType = type;
					}
					else
					{
						new CLogTextMessage(pSimBox->GetCurrentTime(), "Unable to create destination target");
						 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
					}
				}
			}

			// When we get to here, either all the beads in the source target had
			// the same type or we are creating the final destination target.

			const zString beadName = pSimBox->GetBeadNameFromType(currentType);
			const zString targetLabel = destLabel + beadName + pSimBox->ToString(currentType);

			if(pSimBox->CreateCommandTarget(targetLabel, currentType, vTempBeads))
			{
				pTarget = pSimBox->GetCommandTarget(targetLabel);
				mTargets.insert(zPairLongTarget(currentType, pTarget));

				// Create the composite target and add the simple targets to it manually, 
				// as the normal command to create a composite target does not add
				// targets to it.

				if(pSimBox->CreateCompositeCommandTarget(destCompositeLabel))
				{
					CCommandTargetNode*   pDestinationTarget = pSimBox->GetCommandTarget(destCompositeLabel);

					for(LongTargetIterator it=mTargets.begin(); it!=mTargets.end(); it++)
					{
						pDestinationTarget->AddTarget(it->second);
					}
				}
				else
				{
					// A target with the same name as the composite target exists

					new CLogTextMessage(pSimBox->GetCurrentTime(), "Unable to create composite target");
					 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
				}
			}
			else
			{
				// A target with the same name as one of the destination targets exists

				new CLogTextMessage(pSimBox->GetCurrentTime(), "Unable to create destination target to add to composite target");
				new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
			}

			// Log execution of the command passing the target map so that we can 
			// access the target names, bead types and sizes

			new CLogctExtractBeadTypesIntoCompositeTarget(pSimBox->GetCurrentTime(), sourceLabel, destLabel, 
			destCompositeLabel, pSimBox->GetCommandTarget(destCompositeLabel)->GetBeadTotal(), mTargets);
		}
		else
		{
			new CLogctEmptyTarget(pSimBox->GetCurrentTime(), sourceLabel);
		}
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
}
