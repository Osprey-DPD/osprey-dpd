/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccStopImpl.cpp: implementation of the ccStopImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccStopImpl.h"
#include "ccStop.h"
#include "SimBox.h"
#include "LogStopSimulation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccStopImpl::ccStopImpl()
{
}

ccStopImpl::~ccStopImpl()
{

}

// Command handler to stop the simulation safely at the next time step.
// It save a snapshot of the simulation and a restart state, and then
// sets the total simulation time to the current time.
//
// Note that the snapshot and restart states will not be the same as those
// that would be generated if the simulation were to save such states in the
// same timestep normally. This is because the normal save functions are
// called after the Evolve() routine whereas the commands are executed before.
// If coincidence is required, execute the stop command the timestep after
// the normal save timestep.
//
// Also, note that the calculation of ensemble averages will not be accurate 
// unless the time step at which the simulation ends is an exact multiple of
// the analysis period.

void ccStopImpl::Stop(const xxCommand* const pCommand)
{
//	const ccStop* const pCmd = dynamic_cast<const ccStop*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	pSimBox->m_TotalTime = pSimBox->GetCurrentTime();

	pSimBox->SaveCurrentState();
	pSimBox->SaveRestartState();

	// This command cannot fail
	
	 new CLogStopSimulation(pSimBox->GetCurrentTime(), pSimBox->m_TotalTime);
}
