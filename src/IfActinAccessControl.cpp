/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// IfActinAccessControl.cpp: implementation of the IfActinAccessControl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "IfActinAccessControl.h"
#include "aefActinNetwork.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IfActinAccessControl::IfActinAccessControl(aefActinNetwork* const pNetwork) : m_pIAccess(new IfActinAccess(pNetwork))
{

}

IfActinAccessControl::~IfActinAccessControl()
{

}

// Function to indicate that this ACN implements access control. Other ACNs use
// the related IsAccessAllowed() functions to get a pointer to an interface that
// allows them to interact with this ACN.

bool IfActinAccessControl::IsAccessControlEnabled() const
{
	return true;
}

// Function to return a pointer to the interface used by the Arp23 ACN. 

IfActinAccessFromArp23* IfActinAccessControl::IsAccessAllowed(const aeArp23Network* const pACN)
{
	return dynamic_cast<IfActinAccessFromArp23*>(m_pIAccess);
}

// Function to return a pointer to the interface used by the Formin ACN. 

IfActinAccessFromFormin* IfActinAccessControl::IsAccessAllowed(const aeForminNetwork* const pACN)
{
	return dynamic_cast<IfActinAccessFromFormin*>(m_pIAccess);
}

// Function to return a pointer to the interface used by the Receptor ACN. 

IfActinAccessFromReceptor* IfActinAccessControl::IsAccessAllowed(const aeReceptorNetwork* const pACN)
{
	return dynamic_cast<IfActinAccessFromReceptor*>(m_pIAccess);
}

