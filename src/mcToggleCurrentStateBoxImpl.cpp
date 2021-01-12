/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcToggleCurrentStateBoxImpl.cpp: implementation of the mcToggleCurrentStateBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcToggleCurrentStateBoxImpl.h"
#include "mcToggleCurrentStateBox.h"
#include "Monitor.h"
#include "LogCurrentStateBox.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcToggleCurrentStateBoxImpl::mcToggleCurrentStateBoxImpl()
{
}

mcToggleCurrentStateBoxImpl::~mcToggleCurrentStateBoxImpl()
{

}

// Command handler to turn on/off the bounding box used in the CCurrentState
// snapshot output file. We just store a flag showing whether the box should be
// displayed.
//
// The default is to show the box.

void mcToggleCurrentStateBoxImpl::ToggleCurrentStateBox(const xxCommand* const pCommand)
{
//	const mcToggleCurrentStateBox* const pCmd = dynamic_cast<const mcToggleCurrentStateBox*>(pCommand);

	CMonitor* const pMon = dynamic_cast<CMonitor*>(this);

	pMon->m_bDisplayBox = !pMon->m_bDisplayBox;

	// This command cannot fail

	new CLogCurrentStateBox(pMon->GetCurrentTime(), pMon->m_bDisplayBox);
}
