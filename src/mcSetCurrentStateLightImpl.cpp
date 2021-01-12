/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetCurrentStateLightImpl.cpp: implementation of the mcSetCurrentStateLightImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetCurrentStateLightImpl.h"
#include "mcSetCurrentStateLight.h"
#include "Monitor.h"
#include "LogCurrentStateLight.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetCurrentStateLightImpl::mcSetCurrentStateLightImpl()
{
}

mcSetCurrentStateLightImpl::~mcSetCurrentStateLightImpl()
{

}

// Command handler to set the coordinates of the light in the CCurrentState snapshot 
// file. The coordinates are specified as multiples of the SimBox size and converted 
// to actual coordinates before storing them in a container to pass to the
// CCurrentState.

void mcSetCurrentStateLightImpl::SetCurrentStateLight(const xxCommand* const pCommand)
{
	const mcSetCurrentStateLight* const pCmd = dynamic_cast<const mcSetCurrentStateLight*>(pCommand);

	const double fx = pCmd->GetLight(0);
	const double fy = pCmd->GetLight(1);
	const double fz = pCmd->GetLight(2);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	double dx = fx*pMon->m_SimBoxXLength;
	double dy = fy*pMon->m_SimBoxYLength;
	double dz = fz*pMon->m_SimBoxZLength;

	pMon->m_vLightX.push_back(dx);
	pMon->m_vLightY.push_back(dy);
	pMon->m_vLightZ.push_back(dz);

	new CLogCurrentStateLight(pMon->GetCurrentTime(), dx, dy, dz);
}

