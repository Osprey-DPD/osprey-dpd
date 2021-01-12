/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcToggleBeadDisplayImpl.cpp: implementation of the mcToggleBeadDisplayImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcToggleBeadDisplayImpl.h"
#include "mcToggleBeadDisplay.h"
#include "Monitor.h"
#include "SimState.h"
#include "AbstractBead.h"
#include "LogBeadDisplay.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcToggleBeadDisplayImpl::mcToggleBeadDisplayImpl()
{
}

mcToggleBeadDisplayImpl::~mcToggleBeadDisplayImpl()
{

}

// Command handler to remove all beads of the specified type from subsequent
// CCurrentState output files until reversed by a further command. The status of 
// each bead type is stored in a container, but each bead's visibility status is
// stored in the instances. This allows subsequent commands to toggle the visibility
// of beads that are initially invisible.
//
// We check that the specified bead type is allowed before modifying the
// CMonitor, and issue a command failed message if it is not.

void mcToggleBeadDisplayImpl::ToggleBeadDisplay(const xxCommand* const pCommand)
{
	const mcToggleBeadDisplay* const pCmd = dynamic_cast<const mcToggleBeadDisplay*>(pCommand);

	const zString beadName = pCmd->GetBeadName();	

	CMonitor* pMon = dynamic_cast<CMonitor*>(this);

	CSimState* const psState = pMon->m_pSimState;

	const long beadType = psState->GetAnalysisState().GetBeadTypeFromName(beadName);
	
	if(beadType >= 0 && beadType < psState->GetBeadTypeTotal())
	{
        // I removed the use of excluded bead types, and set the visibility flags of 
        // all beads of the specified type individually. I use the excluded bead types
        // container to store the state of each type. I need to do this because specific
        // beads of a given type may have their visibility changed by a number of commands,
        // so I cannot just toggle their visibility depending on the state of a specific
        // bead, e.g., the first in the container.

		pMon->m_ExcludedBeadTypes.at(beadType) = 1 - pMon->m_ExcludedBeadTypes.at(beadType);

		bool bDisplay = true;

		if(pMon->m_ExcludedBeadTypes.at(beadType) == 1)
        {
			bDisplay = false;
			
#if EnableParallelCommands == SimMPSEnabled
            // The parallel code has to modify the beads actually held in the SimBox not those originally in the initial state instance
			// as beads that move between processors are not added to the initial state.

	        AbstractBeadVector vAllBeads = pMon->GetSimBoxBeads();
	        for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	        {
                if((*iterBead)->GetType() == beadType)
                {
                    (*iterBead)->SetInvisible();
                }
            }
#else
            psState->GetInitialState().SetBeadTypeInvisible(beadType);
#endif
        }
        else
        {
			bDisplay = true;
			
#if EnableParallelCommands == SimMPSEnabled
            // The parallel code has to modify the beads actually held in the SimBox not those originally in the initial state instance
			// as beads that move between processors are not added to the initial state.

	        AbstractBeadVector vAllBeads = pMon->GetSimBoxBeads();
	        for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	        {
                if((*iterBead)->GetType() == beadType)
                {
                    (*iterBead)->SetVisible();
                }
            }
#else
            psState->GetInitialState().SetBeadTypeVisible(beadType);
#endif
        }

        // We only perform parallel processing here if the EnableParallelCommands
        // flag is set. 
#if EnableParallelCommands == SimMPSEnabled
        if(xxParallelBase::GlobalGetRank() == 0)
        {
		    new CLogBeadDisplay(pMon->GetCurrentTime(), beadName, beadType, bDisplay);
        }
		else
		{
		    // send a message to P0 recording change of bead visibility status. We ignore this for now as we 
			// assume that all processors implement the command.
		}
	}
	else
	{
        if(xxParallelBase::GlobalGetRank() == 0)
        {
		     new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
        }
        else
        {
            // send a pmParallelCommandFailed error message back to P0 so that it can
            // log a CLogParallelCommandFailed message to the log file.
        }
    }
#else
		new CLogBeadDisplay(pMon->GetCurrentTime(), beadName, beadType, bDisplay);
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}
#endif
}
