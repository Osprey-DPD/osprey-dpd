/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcTogglePolymerDisplayImpl.cpp: implementation of the mcTogglePolymerDisplayImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcTogglePolymerDisplayImpl.h"
#include "mcTogglePolymerDisplay.h"
#include "Monitor.h"
#include "SimState.h"
#include "Polymer.h"
#include "LogPolymerDisplay.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcTogglePolymerDisplayImpl::mcTogglePolymerDisplayImpl()
{
}

mcTogglePolymerDisplayImpl::~mcTogglePolymerDisplayImpl()
{

}

// Command handler to toggle all polymers of the specified type from being written
// to subsequent CCurrentState output files until reversed by a further command. 
// The beads that belong to polymers of the specified type have their visibility 
// status toggled on/off. Hence, executing this command twice in succession leaves
// the polymer display status unchanged.
//
// We check that the specified polymer type is allowed before modifying the
// CMonitor, and issue a command failed message if it is not.

void mcTogglePolymerDisplayImpl::TogglePolymerDisplay(const xxCommand* const pCommand)
{
	const mcTogglePolymerDisplay* const pCmd = dynamic_cast<const mcTogglePolymerDisplay*>(pCommand);

	const zString polymerName = pCmd->GetPolymerName();	

	CMonitor* pMon = dynamic_cast<CMonitor*>(this);

	CSimState* const psState = pMon->m_pSimState;

	const long polymerType = psState->GetAnalysisState().GetPolymerTypeFromName(polymerName);
	
	if(polymerType >= 0 && polymerType < psState->GetPolymerTypeTotal())
	{
       PolymerVector vPolymers = psState->GetPolymers();

       // Find the current visibility status of the polymer type and store it.
       // We have to iterate over all polymers that precede the desired type,
       // as we don't know where they start in the polymer container.

       bool bNotFound = true;
       bool bNewDisplay = 0;

       PolymerVectorIterator iterPoly1=vPolymers.begin(); 
       
        while(bNotFound && iterPoly1!=vPolymers.end())
        {
            if((*iterPoly1)->GetType() == polymerType)
            {
	            bNotFound = false;
                bNewDisplay = !(*iterPoly1)->GetVisible();
            }
            iterPoly1++;
        }

        // Only modify the visibility status if the polymer type was found,
        // otherwise leave it unchanged. We indicate if the status was changed
        // in the log message. This should never fail, but if polymers have their
        // type changed dynamically in the future, it is possible for polymer types
        // to be out of order and perhaps some members of a type might get missed.

        if(!bNotFound)
        {
            for(PolymerVectorIterator iterPoly=vPolymers.begin(); iterPoly!=vPolymers.end(); iterPoly++)
            {
                if((*iterPoly)->GetType() == polymerType)
                {
		            (*iterPoly)->SetVisible(bNewDisplay);
                }
            }
        }

		new CLogPolymerDisplay(pMon->GetCurrentTime(), polymerName, polymerType, bNewDisplay, !bNotFound);
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}
}
