/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Experiment.cpp: implementation of the CExperiment class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "SimProcessFlags.h"
#include "ExperimentDefs.h"

#include "Experiment.h"
#include "Simulation.h"
#include "xxFile.h"
#include "xxProtocol.h"
#include "xxSimulationTool.h"
#include "xxSubstrateTool.h"
#include "xxSculptureTool.h"
#include "xxProbeFieldTool.h"
#include "xxExternalFieldTool.h"
#include "xxProcessTool.h"

// Include headers for protocols known to the Experiment object

#include "acfProtocolFactory.h"

#include "epStd.h"
#include "epRandom.h"
#include "epLamella.h"

//////////////////////////////////////////////////////////////////////
// Static member variable holding a pointer to the single instance of CExperiment
// via its IExperiment interface.

// Note that it does not get assigned a value until the user creates the
// instance.

IExperiment* CExperiment::m_pInstance = 0;

// Public member function to create a single instance of the CExperiment class.
// We call different constructors depending on whether the experiment wraps
// the simulation or the simulation is stand-alone. We also set MPI constants 
// in a base class if the experiment is running on multiple processors.

IExperiment* CExperiment::Instance(const zString protocolName, const zString runId, bool bNormalRun)
{
	if(!m_pInstance)
	{
        // Set static member variables in the base class that hold the 
        // number of processors for the job and the rank of the current instance.
        // Note that ranks are zero-indexed but the minimum number of processors is 1.

#if SimMPS == SimulationEnabled
        // We have to use int in the next two declarations as mpiCC gives an error
        // otherwise.
        int experimentWorld;  
        int experimentRank;

        MPI_Comm_size(MPI_COMM_WORLD, &experimentWorld);
        MPI_Comm_rank(MPI_COMM_WORLD, &experimentRank);

        xxParallelBase::m_World = experimentWorld;
        xxParallelBase::m_Rank  = experimentRank;
#else
        xxParallelBase::m_World = 1;
        xxParallelBase::m_Rank  = 0;
#endif
        
        if(bNormalRun)
        {
#if EnableXMLProtocol == SimXMLEnabled
		    m_pInstance = new CExperiment(protocolName, runId);
#elif EnableXMLProtocol == SimXMLDisabled
		    m_pInstance = new CExperiment(runId);
        }
        else
        {
		    m_pInstance = new CExperiment(runId);
        }
#endif
	}

	return m_pInstance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// The experiment object is the top-level object in the code. It
// creates protocol and simulation objects to carry out the 
// experiment defined by a user-supplied protocol document.
//
// The protocol name is used as the key to an abstract class factory that
// creates an instance of the appropriate protocol.

CExperiment::CExperiment(const zString protocolName, 
						 const zString runId) : m_bProtocolValid(false),
												m_pProtocol(0), 
												m_pISimulation(0),
												m_pSimTool(0),
												m_pSubstrateTool(0),
												m_pSculptureTool(0),
												m_pProbeTool(0),
												m_pExternalTool(0),
												m_pProcessTool(0),
												m_ISecureExpt(*this),
												m_IDocHandler(*this),
												m_ICommandDoc(*this),
												m_IProtocolDoc(*this),
												m_IConnectSim(*this)
{
#if EnableXMLProtocol == SimXMLEnabled

	m_pProtocol = acfProtocolFactory::Instance()->Create(protocolName, runId);

	if(m_pProtocol)
	{
		m_bProtocolValid = true;
		std::cout << "Experiment beginning..." << zEndl;
		std::cout << "Reading protocol document... " << m_pProtocol->GetProtocolDocumentName() << zEndl;

		// Create the singleton simulation instance and store it using
		// its ISimulation interface

		m_pISimulation = CSimulation::Instance(&m_IConnectSim, runId);

	}
	else
	{
		m_bProtocolValid = false;
		std::cout << "Experiment aborted due to unrecognised protocol" << zEndl;
	}

#endif
}

CExperiment::CExperiment(const zString runId) : m_bProtocolValid(false),
												m_pProtocol(0), 
												m_pISimulation(0),
												m_pSimTool(0),
												m_pSubstrateTool(0),
												m_pSculptureTool(0),
												m_pProbeTool(0),
												m_pExternalTool(0),
												m_pProcessTool(0),
												m_ISecureExpt(*this),
												m_IDocHandler(*this),
												m_ICommandDoc(*this),
												m_IProtocolDoc(*this),
												m_IConnectSim(*this)
{
#if EnableXMLProtocol == SimXMLDisabled

	std::cout << "Stand-alone simulation beginning..." << zEndl;

	// Create the singleton stand-alone simulation instance and store it using
	// its ISimulation interface

	m_pISimulation = CSimulation::Instance(0, runId);

#endif
}

CExperiment::~CExperiment()
{
	// Zero the global singleton member

	m_pInstance = 0;

	// Delete the simulation instance that is always created regardless of
	// whether the protocol document or control data file is used

	if(m_pISimulation)
		delete m_pISimulation;

	// If the protocol was was enabled and valid we delete it and check its
	// tools for deletion

	if(m_bProtocolValid)
	{
		delete m_pProtocol;

		if(m_pSimTool)
		{
			delete m_pSimTool;
			m_pSimTool = 0;
		}

		if(m_pSubstrateTool)
		{
			delete m_pSubstrateTool;
			m_pSubstrateTool = 0;
		}

		if(m_pSculptureTool)
		{
			delete m_pSculptureTool;
			m_pSculptureTool = 0;
		}

		if(m_pProbeTool)
		{
			delete m_pProbeTool;
			m_pProbeTool = 0;
		}

		if(m_pExternalTool)
		{
			delete m_pExternalTool;
			m_pExternalTool = 0;
		}
	}

}

// ****************************************
// Helper functions for the experiment

bool CExperiment::IsProbeFieldActive() const
{
	if(m_pProtocol)
		return m_pProtocol->IsProbeFieldActive();
	else
		return false;
}

bool CExperiment::IsExternalFieldActive() const
{
	if(m_pProtocol)
		return m_pProtocol->IsExternalFieldActive();
	else
		return false;
}

// ****************************************
// Implementation of the IExperiment interface

// Function read the experiment's protocol document from file and validate it.
// We have already checked that the document's name is a recognised
// protocol, we check here that the document exists, and then read it.

bool CExperiment::ReadProtocolDocument()
{
#if EnableXMLProtocol == SimXMLEnabled

	IProtocolDocument* pProDoc = m_IDocHandler.GetIProtocolDocument();
	
	const zString docName = m_pProtocol->GetProtocolDocumentName();

	if(xxFile::DoesFileExist(docName) && pProDoc->ReadProtocolDocument(docName))
	{
		return true;
	}
	else
	{
		return false;
	}

#else
    return false;
#endif
}

// Alternative run function to generate a single current state snapshot without
// integrating any time steps. This provides a snapshot of the initial configuration
// of the simulated system for display to the user. Note that this method of 
// executing a simulation ignores any protocol document and assumes that 
// the runId has the system-defined value "previs".

bool CExperiment::RunPrevis(StringSequence excludedPolymers)
{
    return m_pISimulation->RunPrevis(excludedPolymers);
}

// Main function to execute the simulation defined by the experiment's protocol.
// We check that the protocol document was recognised before attempting to run
// the simulation.

bool CExperiment::Run()
{
#if EnableXMLProtocol == SimXMLEnabled

	if(ReadProtocolDocument())
	{
		return m_pISimulation->Run();
	}
	else
		return false;

#elif EnableXMLProtocol == SimXMLDisabled
		return m_pISimulation->Run();
#else
		return false;
#endif
}

// ****************************************
// Functions to return pointers to the interfaces implemented by this class

ISecureExperiment* CExperiment::GetISecureExperiment()
{
	return &m_ISecureExpt;
}

IDocumentHandler* CExperiment::GetIDocumentHandler()
{
	return &m_IDocHandler;
}

ICommandDocument* CExperiment::GetICommandDocument()
{
	return &m_ICommandDoc;
}

IProtocolDocument* CExperiment::GetIProtocolDocument()
{
	return &m_IProtocolDoc;
}

// ****************************************
// Functions to return pointers to the interfaces implemented by the simulation

ISetProcessData* CExperiment::GetISetProcessData() const
{
	return m_pISimulation->GetISetProcessData();
}
