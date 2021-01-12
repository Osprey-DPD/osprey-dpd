/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetDisplayBeadRangeImpl.cpp: implementation of the mcSetDisplayBeadRangeImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetDisplayBeadRangeImpl.h"
#include "mcSetDisplayBeadRange.h"
#include "Monitor.h"
#include "LogSetDisplayBeadRange.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetDisplayBeadRangeImpl::mcSetDisplayBeadRangeImpl()
{
}

mcSetDisplayBeadRangeImpl::~mcSetDisplayBeadRangeImpl()
{

}

// Command handler function to set a range for bead coordinates such that 
// only beads with coordinates in the range are output to the CCurrentState 
// snapshot files. Each such command specifies one axis and a lower and 
// upper limit. The command applies to all bead types. The flag 
// m_bRestrictCurrentStateCoords is set if a restriction applies in any axis.
// It is cleared only if no restrictions apply.

void mcSetDisplayBeadRangeImpl::SetDisplayBeadRange(const xxCommand* const pCommand)
{
	const mcSetDisplayBeadRange* const pCmd = dynamic_cast<const mcSetDisplayBeadRange*>(pCommand);

	CMonitor* pMon = dynamic_cast<CMonitor*>(this);

	const zString axis  = pCmd->GetAxis();
	const double  min   = pCmd->GetMinFraction();
	const double  max   = pCmd->GetMaxFraction();

    if( (axis == "x" || axis == "y" || axis == "z") && !(min < 0.0) && !(max > 1.0) )
	{
	    if(axis == "x")
	    {
		    pMon->m_MinXFraction = min;
		    pMon->m_MaxXFraction = max;
	    }
	    else if(axis == "y")
	    {
		    pMon->m_MinYFraction = min;
		    pMon->m_MaxYFraction = max;
	    }
	    else if(axis == "z")
	    {
		    pMon->m_MinZFraction = min;
		    pMon->m_MaxZFraction = max;
	    }

	    // If there is a restriction on any axis, set the 
	    // m_bRestrictCurrentStateCoords flag to indicate this

	    if(pMon->m_MinXFraction > 0.0 || pMon->m_MaxXFraction < 1.0 ||
	       pMon->m_MinYFraction > 0.0 || pMon->m_MaxYFraction < 1.0 ||
	       pMon->m_MinZFraction > 0.0 || pMon->m_MaxZFraction < 1.0)
	       pMon->m_bRestrictCurrentStateCoords = true;
	    else
	       pMon->m_bRestrictCurrentStateCoords = false;

        // Log the command's execution - but only on P0 if this is a parallel run
#if EnableParallelCommands == SimMPSEnabled
        if(xxParallelBase::GlobalGetRank() == 0)
        {
	        new CLogSetDisplayBeadRange(pMon->GetCurrentTime(), axis, min, max);
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
		new CLogSetDisplayBeadRange(pMon->GetCurrentTime(), axis, min, max);
	}
	else
	{
		 new CLogCommandFailed(pMon->GetCurrentTime(), pCmd);
	}
#endif
}
