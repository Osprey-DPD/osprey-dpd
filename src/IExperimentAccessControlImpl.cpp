/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// IExperimentAccessControlImpl.cpp: implementation of the IExperimentAccessControlImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "IExperimentAccessControlImpl.h"
#include "Simulation.h"
#include "IConnectSimulation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IExperimentAccessControlImpl::IExperimentAccessControlImpl(CSimulation& rSim) : m_rSimulation(rSim)
{

}

IExperimentAccessControlImpl::~IExperimentAccessControlImpl()
{

}

// Function to return a pointer to the containing class' main ISimulation interface.

ISimulation* IExperimentAccessControlImpl::IISimulation() const
{
	return &m_rSimulation;
}

// Forwarding function to determine if the simulation is part of an experiment
// or is running stand-alone.

bool IExperimentAccessControlImpl::IsExperimentAccessible() const
{
	return m_rSimulation.IsExperimentAccessible();
}

// Forwarding function to return a pointer to the IDocumentHandler interface
// on the experiment. We check that the simulation is not stand-alone first.

IDocumentHandler* IExperimentAccessControlImpl::GetIDocumentHandler()
{
	if(IsExperimentAccessible())
	{
		return m_rSimulation.GetIConnectSimulation()->GetIDocumentHandler();
	}
	else
		return 0;
}
