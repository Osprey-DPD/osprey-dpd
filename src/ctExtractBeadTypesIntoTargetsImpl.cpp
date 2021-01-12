/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctExtractBeadTypesIntoTargetsImpl.cpp: implementation of the ctExtractBeadTypesIntoTargetsImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimAlgorithmFlags.h"
#include "aaRegionToType.h"
#include "ctExtractBeadTypesIntoTargetsImpl.h"
#include "ctExtractBeadTypesIntoTargets.h"
#include "SimBox.h"
#include "CommandTarget.h"
#include "CommandTargetComposite.h"
#include "Bead.h"
#include "LogctEmptyTarget.h"
#include "LogctExtractBeadTypesIntoTargets.h"
#include "LogTextMessage.h"


	using std::sort;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctExtractBeadTypesIntoTargetsImpl::ctExtractBeadTypesIntoTargetsImpl()
{
}

ctExtractBeadTypesIntoTargetsImpl::~ctExtractBeadTypesIntoTargetsImpl()
{

}

// Command handler to extract all the distinct bead types in a target into 
// simple bead targets. The names of the new targets are created
// from the destination label supplied by the user followed by the bead type name
// (or "Unnamed" if it has been modified) plus its numeric type identifier. 
// Note that we have already checked that the destination label is
// a valid name by itself, and does not duplicate an existing target. But that 
// once the bead types have been suffixed to the base label, a duplication may
// occur. We do not check for this at the moment.

void ctExtractBeadTypesIntoTargetsImpl::ExtractBeadTypesIntoTargets(const xxCommand* const pCommand)
{
	const ctExtractBeadTypesIntoTargets* const pCmd = dynamic_cast<const ctExtractBeadTypesIntoTargets*>(pCommand);

	const zString	sourceLabel	= pCmd->GetTargetLabel();
	const zString   destLabel   = pCmd->GetDestinationLabel();

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
						CCommandTargetNode* pTarget = pSimBox->GetCommandTarget(targetLabel);

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
				CCommandTargetNode* pTarget = pSimBox->GetCommandTarget(targetLabel);

				mTargets.insert(zPairLongTarget(currentType, pTarget));
			}
			else
			{
				new CLogTextMessage(pSimBox->GetCurrentTime(), "Unable to create destination target");
				new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
			}

			// Log execution of the command passing the target map so that we can 
			// access the target names, bead types and sizes

			new CLogctExtractBeadTypesIntoTargets(pSimBox->GetCurrentTime(), sourceLabel, destLabel, mTargets);
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
