/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccCreateActiveSimBoxImpl.cpp: implementation of the ccCreateActiveSimBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccCreateActiveSimBoxImpl.h"
#include "ccCreateActiveSimBox.h"
#include "SimBox.h"
#include "aeActiveSimBox.h"
#include "LogActiveSimBoxCreated.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccCreateActiveSimBoxImpl::ccCreateActiveSimBoxImpl()
{
}

ccCreateActiveSimBoxImpl::~ccCreateActiveSimBoxImpl()
{

}

// Command handler to create the shadow SimBox that holds instances of 
// active networks. Only one shadow SimBox can be created, but it may hold 
// any number of active networks to keep track of active polymer species.

void ccCreateActiveSimBoxImpl::CreateActiveSimBox(const xxCommand* const pCommand)
{
#if EnableACNCommand == SimCommandEnabled

	const ccCreateActiveSimBox* const pCmd = dynamic_cast<const ccCreateActiveSimBox*>(pCommand);

	CSimBox* const pSimBox = dynamic_cast<CSimBox*>(this);

	if(!pSimBox->m_pShadow)
	{
		pSimBox->m_pShadow = aeActiveSimBox::Instance(pSimBox->GetISimBox());

		 new CLogActiveSimBoxCreated(pSimBox->GetCurrentTime());
	}
	else
	{
		 new CLogCommandFailed(pSimBox->GetCurrentTime(), pCmd);
	}
#endif
}
