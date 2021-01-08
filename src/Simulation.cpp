/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Simulation.cpp: implementation of the CSimulation class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimStdlibFlags.h"
#include "Simulation.h"
#include "InputData.h"
#include "InitialStateData.h"
#include "pmString.h"
#include "pmInputData.h"
#include "pmBeadData.h"
#include "pmBondData.h"
#include "pmBondPairData.h"
#include "pmPolymerData.h"
#include "pmCommandNames.h"
#include "SimState.h"
#include "ISimBox.h"
#include "xxProcess.h"
#include "IConnectSimulation.h"
#include "LogSimCDError.h"
#include "LogSimISError.h"
#include "LogSimParallelInputDataError.h"

// Set of allowed parallel initial states

#include "pmISRandom.h"
#include "pmISLamella.h"
#include "pmISVesicle.h"


//////////////////////////////////////////////////////////////////////
// Static member variable holding a pointer to the single instance of CSimulation
// via its ISimulation interface.

// Note that it does not get assigned a value until the user creates the
// instance.

ISimulation* CSimulation::m_pInstance = 0;

// Public member function to create a single instance of the CSimulation class.

ISimulation* CSimulation::Instance(IConnectSimulation* pExpt, const zString runId)
{
	if(!m_pInstance)
	{
		if(pExpt)
			m_pInstance = new CSimulation(pExpt, runId);
		else
			m_pInstance = new CSimulation(runId);

	}

	return m_pInstance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// A CSimulation object must start in a valid state. It requires only
// a runId, in order to label all the output files, to execute the default run.
//
// **********************************************************************
//
// Control Data File:	CDFPrefix.runId
// *****************
//
// All the data required to run a (non-restarted) simulation are specified in 
// the "Control Data File" whose name must be of the form: "CDFPrefix.runId", 
// where "CDFPrefix" is a string that is determined by the function GetCDFPrefix()
// xxBase.h, and "runId" is a string used to label all the output files produced by 
// the run. Note that the suffix is a string not a number, although it is common to 
// specify consecutive integers to label a related sequence of runs.
//
// If no control data file with the name "CDFPrefix.runId" is found, the default
// run executes. The default run is initialised using data in the CInputData 
// constructor. The default data in the CInputData object is always loaded 
// first, but if a control data file is found the data contained in it 
// overwrites the  default values. 
//
// If the control data file specifies "restart" as the value of the "State" keyword, 
// together with the appropriate data, a previous simulation will be continued 
// using the data found in the new control data file to replace those from the 
// original run. Only certain parameters can be overwritten and illegal changes 
// will cause the run to abort.
//
//
// Initial State Assembly: CInitialStateData and CBuilder classes
// **********************
//
// The data for each type of initial state is read in using an object
// of a class derived from CInitialStateData, e.g., CInitialStateRestart, 
// CInitialStateLamella, CInitialStateBLMVesicle, etc. Once the data has
// been read CInputData calls the virtual CInitialStateData::ValidateData() 
// member function to check that the data is sufficient and correct for the
// selected initial state. The data is then passed to a CBuilder-derived object 
// to assemble the beads into the appropriate initial configuration.
//
// The default initial configuration is "random". This can be changed by 
// replacing the initial state object in the CInputData constructor by another
// valid initial state and supplying the necessary data. Obviously, 
// CInitialStateRestart cannot be used to define a default simulation.
//
//
// Typedefs and compilation flags
// ******************************
//
// Flags controlling aspects of the simulation are defined in the file "SimDefs.h". 
// Typedefs and other definitions are contained in the base class "xxBase".
// Both of these files must be included in all compilation units that need
// access to the typedefs and/or the flags used to control compile-time options.
//
// Parallel DPD Code
// *****************
//
// The dpd code can be compiled to run on multiple processors using MPI.
// Flags controlling this compilation are in SimDefs.h and SimMPSFlags.h
//  as they are needed in many compilation units. All classes that need to be 
// parallel aware, should inherit from xxParallelBase which itself inherits from xxBase. 
// It provides functions allowing objects to determine their rank, the total 
// number of processors, and if they are process zero.
//
//
// **********************************************************************

#if EnableParallelSimBox == SimMPSEnabled
// ****************************************
// Parallelised construction/destruction

// Constructor for use when the simulation is run as part of an experiment.

CSimulation::CSimulation(IConnectSimulation* pExpt, const zString runId) : m_runId(runId), 
                                                                    m_iFileName(xxBase::GetCDFPrefix() + m_runId),
																	m_pInputData(0),
																	m_pSimState(0),
																	m_pISimBox(0),
																	m_pExpt(pExpt),
																	m_IExptAccess(*this),
																	m_ISecureSim(*this),
																	m_ISetSimulationData(*this),
																	m_ISetSubstrateData(*this),
																	m_ISetSculptureData(*this),
																	m_ISetProbeFieldData(*this),
																	m_ISetExternalFieldData(*this),
																	m_ISetProcessData(*this)
{
	// Create the CInputData object that holds all the data needed to define
	// a simulation prior to its execution. Note that we do not yet read data
	// from the control data file to avoid having a read error trash the 
	// constructor. We read the data later using CInputData::GetInputData().

    // In the parallel code, the input file is only read on P0 but must be created
    // on all processors, the relevant data is sent to the other processors after
    // construction.

	m_pInputData = new CInputData(m_runId);
}

// Constructor for use when the simulation is run stand-alone.

CSimulation::CSimulation(const zString runId) : m_runId(runId), 
												m_iFileName(xxBase::GetCDFPrefix() + m_runId),
												m_pInputData(0),
												m_pSimState(0),
												m_pISimBox(0),
												m_pExpt(0),
												m_IExptAccess(*this),
												m_ISecureSim(*this),
												m_ISetSimulationData(*this),
												m_ISetSubstrateData(*this),
												m_ISetSculptureData(*this),
												m_ISetProbeFieldData(*this),
												m_ISetExternalFieldData(*this),
												m_ISetProcessData(*this)
{
	// Create the CInputData object that holds all the data needed to define
	// a simulation prior to its execution. Note that we do not yet read data
	// from the control data file to avoid having a read error trash the 
	// constructor. We read the data later using CInputData::GetInputData().
    // In the parallel code, the input file is only read on P0, and we copy all 
    // the relevant data to the other processors later.

	m_pInputData = new CInputData(m_runId);
}

CSimulation::~CSimulation()
{
	// Set the global singleton member to 0 to ensure that a new
	// simulation instance is created for each run in a sequence.

	m_pInstance = 0;

	if(m_pInputData)
	{
		delete m_pInputData;
		m_pInputData = 0;
	}

	if(m_pSimState)
	{
		delete m_pSimState;
		m_pSimState = 0;
	}

	if(m_pISimBox)
	{
		delete m_pISimBox;
		m_pISimBox = 0;
	}
}

#elif EnableParallelSimBox == SimMPSDisabled
// ****************************************
// Standard construction/destruction

// Constructor for use when the simulation is run as part of an experiment.

CSimulation::CSimulation(IConnectSimulation* pExpt, const zString runId) : m_runId(runId), 
                                                                    m_iFileName(xxBase::GetCDFPrefix() + m_runId),
																	m_pInputData(0),
																	m_pSimState(0),
																	m_pISimBox(0),
																	m_pExpt(pExpt),
																	m_IExptAccess(*this),
																	m_ISecureSim(*this),
																	m_ISetSimulationData(*this),
																	m_ISetSubstrateData(*this),
																	m_ISetSculptureData(*this),
																	m_ISetProbeFieldData(*this),
																	m_ISetExternalFieldData(*this),
																	m_ISetProcessData(*this)
{
	// Create the CInputData object that holds all the data needed to define
	// a simulation prior to its execution. Note that we do not yet read data
	// from the control data file to avoid having a read error trash the 
	// constructor. We read the data later using CInputData::GetInputData().

	m_pInputData = new CInputData(m_runId);
}

// Constructor for use when the simulation is run stand-alone.

CSimulation::CSimulation(const zString runId) : m_runId(runId), 
												m_iFileName(xxBase::GetCDFPrefix() + m_runId),
												m_pInputData(0),
												m_pSimState(0),
												m_pISimBox(0),
												m_pExpt(0),
												m_IExptAccess(*this),
												m_ISecureSim(*this),
												m_ISetSimulationData(*this),
												m_ISetSubstrateData(*this),
												m_ISetSculptureData(*this),
												m_ISetProbeFieldData(*this),
												m_ISetExternalFieldData(*this),
												m_ISetProcessData(*this)
{
	// Create the CInputData object that holds all the data needed to define
	// a simulation prior to its execution. Note that we do not yet read data
	// from the control data file to avoid having a read error trash the 
	// constructor. We read the data later using CInputData::GetInputData().

	m_pInputData = new CInputData(m_runId);
}

CSimulation::~CSimulation()
{
	// Set the global singleton member to 0 to ensure that a new
	// simulation instance is created for each run in a sequence.

	m_pInstance = 0;

	if(m_pInputData)
	{
		delete m_pInputData;
		m_pInputData = 0;
	}

	if(m_pSimState)
	{
		delete m_pSimState;
		m_pSimState = 0;
	}

	if(m_pISimBox)
	{
		delete m_pISimBox;
		m_pISimBox = 0;
	}
}

#endif

// **********************************************************************
// Function to set up the initial state of a simulation in one of several ways.
// Any error in defining the initial state of the simulation causes this
// function to return false and stop the simulation.
//
// The possible options for a simulation are:
//
//  1	Create a new run from the default configuration
//
//	2	Read a new run from an input data file
//
//  3	Restart a run from a previously-saved configuration
//
// In all cases the user can modify (if only partially in the restart case)
// the initial state before starting the simulation via the data in the
// new control data file.
//
// The CSimulation constructor creates a default CInputData object and
// if the specified control data file does not exist the default
// data is used. If the control data file is found then the default data is 
// overwritten by that in the file. If a restart was selected only some of the
// data may be overwritten by the new control data.
//
// Once the data on bead, bond and polymer types, etc, has been read, it is used
// to construct the CSimState (and its component state objects). The simulation box 
// and monitor objects are then constructed. Each holds a pointer to the other so 
// that data can be monitored and analysed during the simulation. 
//
// **********************************************************************

bool CSimulation::Initialize()
{
	// Create a CSimState object to hold the state of the simulation We pass in
	// a "this" pointer  so that the experiment/simulation objects can be
	// accessed by process and commands while the experiment is running.
	//
	// It creates the xxState objects that are needed to start a valid simulation 
	// (CInitialState, CAnalysisState, CHistoryState and CLogState), and 
	// assembles the beads into the initial configuration specified. The CInputData 
	// object is responsible for creating a default initial state or reading the data 
	// from the control data file or from a restart state. All input data is validated 
	// there so we don't have to check anything here. 
	//
	// If the control data is invalid, the program exits immediately; any error
	// messages are written directly to the screen. To change this behaviour
	// change the function xxBase::Trace(). Because the CLogState is created by 
	// the CSimState, which happens after we have validated the control data file, 
	// we cannot post a message to indicate invalid control data.

#if EnableParallelSimBox == SimMPSEnabled

    // We hide the reading of the input file by P0 and its distribution to 
    // the PN inside private helper functions that manage the set of 
    // messages needed to distribute the data. Once all the data has been
    // distributed to all processors, we validate it and assemble the 
    // SimState object. See the comment in the header of the BroadcastSimulationData() routine 
	// that describes why the ValidateDataP() function is called twice on P0.

    if(BroadcastSimulationData() && BroadcastPolymerData() && 
       BroadcastCommandData() && BroadcastISData() && m_pInputData->IsInitialStateValid())
    {
        m_pSimState = new CSimState(this, *m_pInputData);
    }
    else
    {
		return false;
    }

    // We use the parallel AssembleP function in the next line so that P0 and PN
    // can assemble their pieces of the initial state

	if(!m_pSimState->AssembleP())
	{
		new CLogSimISError(0);
		return false;
	}

    m_pISimBox = ISimBox::Instance(*m_pSimState);

    // Check if the parallel SimBox assembled correctly on all processors,
    // and abort the run if not.

    if(m_pISimBox->GetParallelAssemblyFailed())
    {
        return false;
    }

#elif EnableParallelSimBox == SimMPSDisabled

	if(!m_pInputData->GetInputData(m_iFileName))
	{
        // We cannot create an error message yet
//		new CLogSimCDError(0);
		return false;
	}

	m_pSimState = new CSimState(this, *m_pInputData);

	if(!m_pSimState->Assemble())
	{
		CLogSimISError(0);
		return false;
	}

	// Create the singleton ISimBox object that represents the interface to the
	// CSimBox that actually does the simulation. The interface object also
	// creates the CMonitor object that monitors the progress of the simulation.
	// We use the static ISimBox::Instance member function to access the protected 
	// ISimBox constructor thus ensuring that only one SimBox and Monitor can be 
	// created for this simulation.

	m_pISimBox = ISimBox::Instance(*m_pSimState);

#endif

	return true;
}

// **********************************************************************
// Function to generate a single snapshot of the initial state without
// integrating any timesteps. Note that the format of the snapshot is the
// default format. The names of polymer types that are to be excluded from 
// the snapshot (such as the solvent) are passed in via a string container 
// so that the Monitor can remove them from the snapshot using an internal
// copy of the mcTogglePolymerDisplay command.

bool CSimulation::RunPrevis(StringSequence excludedPolymers)
{
    if(Initialize())
    {
#if EnableParallelSimBox == SimMPSEnabled

        for(StringSequenceIterator iterName=excludedPolymers.begin(); iterName!=excludedPolymers.end(); iterName++)
        {
            m_pISimBox->InternalTogglePolymerDisplay(*iterName);
        }

        m_pISimBox->SavePrevisCurrentState();


#elif EnableParallelSimBox == SimMPSDisabled

        for(StringSequenceIterator iterName=excludedPolymers.begin(); iterName!=excludedPolymers.end(); iterName++)
        {
            m_pISimBox->InternalTogglePolymerDisplay(*iterName);
        }

        m_pISimBox->SavePrevisCurrentState();

#endif
    }

    return true;
}

// **********************************************************************
// Function to run the simulation in the CSimBox if the initialisation 
// was executed successfully. Prior to an MD simulation we perform
// a Monte Carlo relaxation of the bead coordinates to remove the huge forces
// that result from the LJ potentials when beads are placed randomly in the SimBox.
// We don't need to do this for DPD because the potentials are soft.

bool CSimulation::Run()
{
	if(Initialize())
	{
#if EnableParallelSimBox == SimMPSEnabled
        // No parallel MD yet so no MC equilibration

		m_pISimBox->Run();

#elif EnableParallelSimBox == SimMPSDisabled

#if SimIdentifier == MD
		m_pISimBox->RunMCRelaxation();
#endif

		m_pISimBox->Run();
#endif
		return true;
	}
	else
		return false;
}

// ****************************************
// Functions to return pointers to all the interfaces implemented by this class

IExperimentAccessControl* CSimulation::GetIExperimentAccessControl()
{
	return &m_IExptAccess;
}

ISecureSimulation* CSimulation::GetISecureSimulation()
{
	return &m_ISecureSim;
}

ISetSimulationData* CSimulation::GetISetSimulationData()
{
	return &m_ISetSimulationData;
}

ISetSubstrateData* CSimulation::GetISetSubstrateData()
{
	return &m_ISetSubstrateData;
}

ISetSculptureData* CSimulation::GetISetSculptureData()
{
	return &m_ISetSculptureData;
}

ISetProbeFieldData* CSimulation::GetISetProbeFieldData()
{
	return &m_ISetProbeFieldData;
}

ISetExternalFieldData* CSimulation::GetISetExternalFieldData()
{
	return &m_ISetExternalFieldData;
}

ISetProcessData* CSimulation::GetISetProcessData()
{
	return &m_ISetProcessData;
}

// ****************************************
// Implementation of IExperimentAccessControl interface

// Function to indicate if the experiment is accessible during the current run.
// We check whether the simulation was constructed with a pointer to an 
// experiment or not.

bool CSimulation::IsExperimentAccessible() const
{
	if(m_pExpt)
		return true;
	else
		return false;
}

// Function to return a pointer to the experiment's IConnectSimulation interface.
// This provides the main interface between the experiment and the simulation.
// Because a simulation can be run stand-alone, in which case it is not
// connected to an experiment, we return a pointer that is null for the
// stand-alone simulation.

IConnectSimulation* CSimulation::GetIConnectSimulation()
{
	return m_pExpt;
}

// ****************************************
// Implementation of ISetProcessData interface

// The process has already been given its unique pid on creation (currently just 
// its id that is the number of processes created via the static member 
// xxProcess::m_ProcessTotal) and just needs to be validated and added to the 
// CAnalysisState's process container via the ISimBox/ISimState interfaces.
//
// We check that the process pointer is non-null, and call its internal validate
// member to perform any validation of the process' data and allow it to create 
// its process state instance.

bool CSimulation::AddProcess(xxProcess* const pProcess)
{
#if EnableParallelSimBox == SimMPSEnabled
	if(pProcess && pProcess->InternalValidateData(m_pISimBox->IISimState()))
	{
		m_pISimBox->AddProcess(pProcess);
		return true;
	}
	else
    {
		return false;
    }
#elif EnableParallelSimBox == SimMPSDisabled
	if(pProcess && pProcess->InternalValidateData(m_pISimBox->IISimState()))
	{
		m_pISimBox->AddProcess(pProcess);
		return true;
	}
	else
    {
		return false;
    }
#endif
}

// Function to return the size of the X dimension of the simulation Space.

double CSimulation::GetSimSpaceXLength() const
{
    if(m_pSimState)
    {
        return m_pSimState->GetSimSpaceXLength();
    }
    else
    {
        return 0.0;
    }
}

// Function to return the size of the Y dimension of the simulation Space.

double CSimulation::GetSimSpaceYLength() const
{
    if(m_pSimState)
    {
        return m_pSimState->GetSimSpaceYLength();
    }
    else
    {
        return 0.0;
    }
}

// Function to return the size of the Z dimension of the simulation Space.

double CSimulation::GetSimSpaceZLength() const
{
    if(m_pSimState)
    {
        return m_pSimState->GetSimSpaceZLength();
    }
    else
    {
        return 0.0;
    }
}

// Private helper function to broadcast the generic simulation data (e.g., 
// simulation box size, temperature etc) to all PN. This does not include
// the data relating to bead types, etc, which is sent using a different set
// of messages. However, it does include the number of bead types, bond types, 
// etc, as this information is needed by PN to know how many messages containing
// datatypes it should expect to receive.
//
// Note that only P0 performs IO, so only it reads the CDF and then propagates the data
// to the other processors. But it validates the input data using the CInputData::IsInitialStateValid() routine,
// which in turn calls the CInitialStateData::ValidateDataP() routine on the derived type.
// Note that this means that ValidateDataP() will be called TWICE on P0, but only once on the other PN.
// Hence, if it has any side-effects (see InitialStateCompositeLamella.cpp, for example, that uses push_back() to store numeric types for polymers), 
// they must be taken into account to prevent them accumulating. 
 
bool CSimulation::BroadcastSimulationData()
{
#if EnableParallelSimBox == SimMPSEnabled

    // P0 packs the non-polymer data relating to the simulation into pmInputData 
    // and  calls the appropriate MPI message to send it to all the other processors. 

    pmInputData* pInputMsg = dynamic_cast<pmInputData*>(acfParallelMessageFactory::Instance()->Create(pmInputData::GetType()));

    if(IsProcessZero())
    {
	    if(m_pInputData->GetInputData(m_iFileName))
	    {
            pInputMsg->SetMessageData(m_pInputData);
	    }
        else
        {
//		    CLogSimCDError* pMsg = new CLogSimCDError(0);
		    return false;
        }

        // Note that at this point, we have read in all the simulation data and
        // the number of bead, bond, bondpair and polymer datatypes on P0 and 
        // we need to pass the number of the datatypes to PN so that they know 
        // how many datatype messages to receive. Note also that we send the message
        // even if validation fails as the PN need to make their own check of the 
        // data so that they can fail gracefully.

        pInputMsg->SendAllP();

        if(!pInputMsg->Validate())
        {
           // Handle error condition here
           std::cout << "pmInputData assembly failed on processor " << GetRank() << zEndl;
//		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		   return false;
        }
    }
    else
    {
        // Here we receive all the data from P0 and use it to set up our copy
        // of the simulation state. The rank of the sending process is held in 
        // the base class' m_SenderRank member variable.

        pInputMsg->Receive();
        if(pInputMsg->Validate())
        {
           m_pInputData->ReceiveInputData(pInputMsg);
        }
        else
        {
		   return false;
        }
    }

	delete pInputMsg;

#endif

    return true;
}

// Private helper function to broadcast the bead, bond, bondpair and polymer
// type data to all PN. We assume that P0 has already validated the data, and
// that it has created its own data types, and distributed the numbers of 
// bead types, bond types, bondpair types and polymer types, so
// that PN can simply instantiate the types one by one when it receives the messages.
// We loop over the number of types, sending the data in the appropriate messages 
// in turn. This is done to simplify the packing of data relating to each type.

bool CSimulation::BroadcastPolymerData()
{
#if EnableParallelSimBox == SimMPSEnabled

    if(!SendBeadTypeMessage())
    {
        // Bead message failure handling

        return false;
    }
    else if(!SendBondTypeMessage())
    {
        // Bond message failure handling

        return false;
    }
    else if(!SendBondPairTypeMessage())
    {
        // Bondpair message failure handling

        return false;
    }
    else if(!SendPolymerTypeMessage())
    {
        // Polymer message failure handling

        return false;
    }

#endif

    return true;
}

// Private helper function to send the data defining all bead types from P0 to PN.

bool CSimulation::SendBeadTypeMessage()
{
#if EnableParallelSimBox == SimMPSEnabled

    // Loop over all bead types on P0 sending a message for each one that is
    // subsequently unpacked by the PN and used to construct the equivalent types.

    long typeTotal = 0;  // local counter used for each type

    // We have to be careful which bead type total we use!

    if(IsProcessZero())
    {
        typeTotal = m_pInputData->GetBeadTypeTotal();
    }
    else
    {
        typeTotal = m_pInputData->GetExpectedBeadTypeTotal();
    }

    for(long type = 0; type < typeTotal; type++)
    {
        pmBeadData* pTypeMsg = dynamic_cast<pmBeadData*>(acfParallelMessageFactory::Instance()->Create(pmBeadData::GetType()));

        if(IsProcessZero())
        {        
            pTypeMsg->SetMessageData(m_pInputData);
            pTypeMsg->SendAllP();

            if(!pTypeMsg->Validate())
            {
               // Handle error condition here
               std::cout << "pmBeadData assembly failed on processor " << GetRank() << zEndl;
    //		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		       return false;
            }
        }
        else
        {
            pTypeMsg->Receive();
            if(pTypeMsg->Validate())
            {
                m_pInputData->ReceiveBeadTypeData(pTypeMsg);
            }
            else
            {
		       return false;
            }
        }

		delete pTypeMsg;
    }

#endif

    return true;
}

// Private helper function to send the data defining all bond types from P0 to PN.

bool CSimulation::SendBondTypeMessage()
{
#if EnableParallelSimBox == SimMPSEnabled

    long typeTotal = 0;  // local counter used for each type

    if(IsProcessZero())
    {
        typeTotal = m_pInputData->GetBondTypeTotal();
    }
    else
    {
        typeTotal = m_pInputData->GetExpectedBondTypeTotal();
    }

    for(long type = 0; type < typeTotal; type++)
    {
        pmBondData* pTypeMsg = dynamic_cast<pmBondData*>(acfParallelMessageFactory::Instance()->Create(pmBondData::GetType()));

        if(IsProcessZero())
        {        
            pTypeMsg->SetMessageData(m_pInputData);
            pTypeMsg->SendAllP();

            if(!pTypeMsg->Validate())
            {
               // Handle error condition here
               std::cout << "pmBondData assembly failed on processor " << GetRank() << zEndl;
    //		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		       return false;
            }
        }
        else
        {
            pTypeMsg->Receive();
            if(pTypeMsg->Validate())
            {
                m_pInputData->ReceiveBondTypeData(pTypeMsg);
            }
            else
            {
		       return false;
            }
        }

		delete pTypeMsg;
    }

#endif

    return true;
}

// Private helper function to send the data defining all bondpair types from P0 to PN.

bool CSimulation::SendBondPairTypeMessage()
{
#if EnableParallelSimBox == SimMPSEnabled

    long typeTotal = 0;  // local counter used for each type

    if(IsProcessZero())
    {
        typeTotal = m_pInputData->GetBondPairTypeTotal();
    }
    else
    {
        typeTotal = m_pInputData->GetExpectedBondPairTypeTotal();
    }

    for(long type = 0; type < typeTotal; type++)
    {
        pmBondPairData* pTypeMsg = dynamic_cast<pmBondPairData*>(acfParallelMessageFactory::Instance()->Create(pmBondPairData::GetType()));

        if(IsProcessZero())
        {        
            pTypeMsg->SetMessageData(m_pInputData);
            pTypeMsg->SendAllP();

            if(!pTypeMsg->Validate())
            {
               // Handle error condition here
               std::cout << "pmBondPairData assembly failed on processor " << GetRank() << zEndl;
    //		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		       return false;
            }
        }
        else
        {
            pTypeMsg->Receive();
            if(pTypeMsg->Validate())
            {
                m_pInputData->ReceiveBondPairTypeData(pTypeMsg);
            }
            else
            {
		       return false;
            }
        }

		delete pTypeMsg;
    }

#endif

    return true;
}

// Private helper function to send the data defining all polymer types from P0 to PN.

bool CSimulation::SendPolymerTypeMessage()
{
#if EnableParallelSimBox == SimMPSEnabled

    long typeTotal = 0;  // local counter used for each type

    if(IsProcessZero())
    {
        typeTotal = m_pInputData->GetPolymerTypeTotal();
    }
    else
    {
        typeTotal = m_pInputData->GetExpectedPolymerTypeTotal();
    }

    for(long type = 0; type < typeTotal; type++)
    {
        pmPolymerData* pTypeMsg = dynamic_cast<pmPolymerData*>(acfParallelMessageFactory::Instance()->Create(pmPolymerData::GetType()));

        if(IsProcessZero())
        {        
            pTypeMsg->SetMessageData(m_pInputData);
            pTypeMsg->SendAllP();

            if(!pTypeMsg->Validate())
            {
               // Handle error condition here
               std::cout << "pmPolymerData assembly failed on processor " << GetRank() << zEndl;
    //		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		       return false;
            }
        }
        else
        {
            pTypeMsg->Receive();
            if(pTypeMsg->Validate())
            {
                m_pInputData->ReceivePolymerTypeData(pTypeMsg);
            }
            else
            {
		       return false;
            }
        }

		delete pTypeMsg;
    }

#endif

    return true;
}

// Private helper fnction to pass the data required to assemble the initial state 
// to PN as only P0 reads the CDF.

bool CSimulation::BroadcastISData()
{
#if EnableParallelSimBox == SimMPSEnabled

    // P0 has to send the IS string identifier to all PN so that they know which
    // IS message class to instantiate, then they can unpack the data needed for
    // the specified IS.

    pmString* const pISTypeMsg = new pmString();
	
    zString ISName;  // Sting used to hold the IS type so P0 and PN can create the appropriate message instance

    if(IsProcessZero())
    {
        ISName = m_pInputData->GetISD()->GetInitialStateType();
		
        pISTypeMsg->SetMessageData(ISName);
        pISTypeMsg->SendAllP();
		
        if(!pISTypeMsg->Validate())
        {
           // Handle error condition here
           std::cout << "mpmInitialState assembly failed: P0 failed ISType string validation" << zEndl;
//		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		   return false;
        }
    }
    else
    {
        // Here we receive all the data from P0 and use it to set up our copy
        // of the simulation state. The rank of the sending process is held in 
        // the base class' m_SenderRank member variable.

        pISTypeMsg->Receive();
        if(pISTypeMsg->Validate())
        {
            ISName = pISTypeMsg->GetValue();
        }
        else
        {
		   return false;
        }
    }

	delete pISTypeMsg;

    // Now all processors know the IS type identifer, we create the appropriate message to 
    // pass the IS data from P0 to PN. If the IS type is unknown we abort the run.

    mpmInitialState* pMsg = dynamic_cast<mpmInitialState*>(acfParallelMessageFactory::Instance()->Create(ISName));

    if(pMsg)
    {
        if(IsProcessZero())
        {
            const CInitialStateData* const pISD = m_pInputData->GetISD();
			
            pMsg->SetMessageData(pISD);			
            pMsg->SendAllP();
			
            if(!pMsg->Validate())
            {
               // Handle error condition here
               std::cout << "mpmInitialState assembly failed: P0 failed ISD data validation" << zEndl;
    //		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		       return false;
            }
        }
        else
        {
            // Here we receive all the data from P0 and use it to set up our copy
            // of the simulation state. The rank of the sending process is held in 
            // the base class' m_SenderRank member variable.

            pMsg->Receive();
            if(pMsg->Validate())
            {
                m_pInputData->ReceiveISData(pMsg);
            }
            else
            {
		       return false;
            }
        }

		delete pMsg;
    }
    else
    {
               std::cout << "mpmInitialState assembly failed: Unknown initial state type on processor " << GetRank() << zEndl;
    //		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		       return false;
    }

#endif

    return true;
}

// Private helper function to pass the names of commands scheduled for execution 
// to PN. Even if there are no commands, we still send the message but take no
// action on receiving it.

bool CSimulation::BroadcastCommandData()
{
#if EnableParallelSimBox == SimMPSEnabled

    pmCommandNames* pMsg = dynamic_cast<pmCommandNames*>(acfParallelMessageFactory::Instance()->Create(pmCommandNames::GetType()));

    if(IsProcessZero())
    {
        // Note that at this point, we have read in all the simulation data and
        // the bead, bond, bondpair and polymer datatypes on P0 and we need to
        // pass the number of the datatypes to PN so that they know how many
        // datatype messages to receive.

        pMsg->SetMessageData(m_pInputData);
        pMsg->SendAllP();

        if(!pMsg->Validate())
        {
           // Handle error condition here
           std::cout << "pmCommandNames assembly failed on processor " << GetRank() << zEndl;
//		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		   return false;
        }
    }
    else
    {
        // Here we receive all the data from P0 and use it to set up our copy
        // of the simulation state. The rank of the sending process is held in 
        // the base class' m_SenderRank member variable.

        pMsg->Receive();
        if(pMsg->Validate())
        {
            m_pInputData->ReceiveCommandNames(pMsg);
        }
        else
        {
		   return false;
        }
    }

	delete pMsg;

#endif

    return true;
}
