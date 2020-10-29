/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ISimState.cpp: implementation of the ISimState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "ISimState.h"
#include "acfCommandFactory.h"
#include "acfParallelMessageFactory.h"
#include "mpmCommandWrapper.h"
#include "pmInteger.h"
#include "pmString.h"

#include "SimState.h"
#include "IModifySimStateBeadTypes.h"
#include "IModifySimStateBondTypes.h"
#include "IModifySimStateIntegration.h"
#include "IModifySimStatePolymers.h"

// SimBox headers

#include "xxCommand.h"          // Needed for command class definition
#include "xxEvent.h"			// Include in case no events are defined


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ISimState::ISimState(CSimState& simState, const ISimBox* const pISimBox) : ISimEvent(pISimBox),
																		m_rSimState(simState),
																		m_pIBeadTypes(new IModifySimStateBeadTypes(simState)),
																		m_pIBondTypes(new IModifySimStateBondTypes(simState)),
																		m_pIIntegration(new IModifySimStateIntegration(simState)),
																		m_pIPolymers(new IModifySimStatePolymers(simState)),
																		m_Events(simState.GetEvents()),
																		m_Commands(simState.GetCommands()),
																		m_SimTime(0),
																		m_TotalTime(simState.GetTotalTime()),
																		m_TotalMCTime(simState.GetTotalMCTime()),
																		m_MaxMCStep(simState.GetMCStepSize()),
																		m_RenormalisationPeriod(1)
{

}

ISimState::~ISimState()
{
	// Delete the interface classes to the CSimState: these are initialised
	// in the constructor and have to exist so we don't check for them.

	delete m_pIBeadTypes;
	delete m_pIBondTypes;
	delete m_pIIntegration;
	delete m_pIPolymers;

	// We don't delete the m_Events and m_Commands here as they are properly
	// destroyed in CAnalysisState.

}

// **********************************************************************
// Forwarding functions to the CSimState to give access to the simulation
// and experiment objects during a run.

ISimulation* ISimState::GetISimulation() const
{
	return m_rSimState.GetISimulation();
}

// **********************************************************************
// Forwarding functions to the CSimState interface classes. These allow
// commands (and other sources) to modify the simulation state in a 
// controlled manner using only functions that are provided for the purpose.

IModifySimStateBeadTypes*  ISimState::IModifyBeadTypes() const
{
	return m_pIBeadTypes;
}

IModifySimStateBondTypes*  ISimState::IModifyBondTypes() const
{
	return m_pIBondTypes;
}

IModifySimStateIntegration*  ISimState::IModifyIntegration() const
{
	return m_pIIntegration;
}

IModifySimStatePolymers*  ISimState::IModifyPolymers() const
{
	return m_pIPolymers;
}

// **********************************************************************
// Protected local functions used by the CSimBox to access ISimState functions.

// Function to cycle through current events to see if any should be executed.

void ISimState::ExecuteEvents()
{
	for(EventIterator iterEvent=m_Events.begin(); iterEvent!=m_Events.end(); iterEvent++)
	{
		if((*iterEvent)->IsActive())
			(*iterEvent)->Execute(m_SimTime, this);
	}
}

// Function to cycle through all current commands to see if any should be executed.
// Because we have to do significantly more work for a paralle simulation, we use
// conditional compilation to select which code block executes. The serial code
// just iterates over all stored commands executing those whose execution time is
// the current simulation time. The parallel code first finds the number of commands
// that are due to execute at the current time, and then sends this number to PN.
// It then sends each command's wrapper message with its arguments to PN that unpacks
// the data into an instance of the appropriate commands.

void ISimState::ExecuteCommands()
{
#if EnableParallelSimBox != SimMPSEnabled
	while(!m_Commands.empty() && m_Commands.front()->Execute(m_SimTime, this))
	{
		m_Commands.pop_front();
	}
#else
    if(IsProcessZero())
    {
        long cno = 0;
        cCommandIterator ic = m_Commands.begin();

        while(!m_Commands.empty() && ic != m_Commands.end() && (m_SimTime == (*ic)->GetExecutionTime()))
        {
            ++cno;
            ++ic;
        }

        // Send the number of commands to expect to PN using pmInteger.
        
        pmInteger rCNo;
        rCNo.SetMessageData(cno);
        rCNo.SendAllP();
		
        // Now iterate over the commands, execute each one on P0, and
        // wrap their data in the corresponding pmXXX message and send 
        // them to PN for execution. 
        
	    while(!m_Commands.empty() && m_Commands.front()->IsTimeToExecute(m_SimTime))
	    {
            const xxCommand* const pCommand = m_Commands.front();
            const zString cname = pCommand->GetCommandType();

            // First, send the command name to PN so it knows what to expect

            pmString rcName;
            rcName.SetMessageData(cname);
            if(rcName.Validate())
            {
                rcName.SendAllP();

                // Next, send the command itself with the message payload filled
                // up from the command instance. We should be deleting the message
                // instances somewhere, but how do we know when PN has received it 
                // and unpacked its data?
                 
	            mpmCommandWrapper* const pMessage = dynamic_cast<mpmCommandWrapper* const>(acfParallelMessageFactory::Instance()->Create(cname));
 
                if(pMessage)
                {
                    pMessage->SetMessageData(pCommand);
                    
                    if(pMessage->Validate())
                    {
                        pMessage->SendAllP();
                    }
                    else
                    {
                        // Issue a command wrapper message invalid data log message here
                    }

                    // Delete the message instance now that we have returned
                    // from the Send function.
                    delete pMessage;
                }
                else
                {
                    // Issue a command wrapper message instantiation failed log message here
					
					std::cout << "P0 failed to instantiate parallel message class for command " << cname << zEndl;
                }
            }
            else
            {
                // Issue an unrecognised command log message here
				
				std::cout << "P0 unable to recognise parallel message class for command " << cname << zEndl;
            }

            // Execute the command on P0 now that we have sent it to PN.                     
            pCommand->Execute(m_SimTime, this);
		    m_Commands.pop_front();
        }
   }
   else
   {
        // Get the expected number of commands on PN

       pmInteger rCNo;
       rCNo.Receive();
       const long cno = rCNo.GetValue();

       // Now loop over the expected commands, first unpacking the name of each
       // command and second instantiating the commands themselves. We assume
       // that the command names are valid as they have been checked by P0 before
       // sending them across to PN.

       if(cno > 0)
       {
           pmString rCName;
		   
           for(long ic=0; ic<cno; ic++)
           {
                rCName.Receive();
                const zString cname = rCName.GetValue();

                // Now receive the command message and unpack its data into a new 
                // command instance inside the wrapper message's Receive() function.

	            mpmCommandWrapper* const pMessage = dynamic_cast<mpmCommandWrapper* const>(acfParallelMessageFactory::Instance()->Create(cname));
                pMessage->SetExecutionTime(m_SimTime);

                pMessage->Receive();
				
                xxCommand* const pCommand = pMessage->GetCommand();
                
                // Delete the message instance now that we have extracted its payload command instance

                delete pMessage;

	            if(pCommand)
	            {
                    // Add the command to the CAnalysisState so that it is destroyed
                    // correctly and execute it. Note that some commands that are directed to a single processor (or a subset)
                    // will only instantiate the command on the target processor(s). Hence, failing to instantiate a command is 
                    // not an error.

		            m_rSimState.GetAnalysisState().AddCommand(pCommand);
                    pCommand->Execute(m_SimTime, this);
	            }
           }
       }
   }

#endif
}

// Function to copy the event sequence from the CSimState into local storage
// to speed up searching for events.

void ISimState::RefreshEvents()
{
	long preSize = m_Events.size();

	m_Events = m_rSimState.GetEvents();

	long postSize = m_Events.size();
}

// Function to add a new process to the CAnalysisState's process sequence.
// Such processes can be created as the result of a command 
// (e.g., StartCommandDocumentProcess). We check that the pointer is 
// non-null before adding it, but do not validate the process itself.

void ISimState::AddProcess(xxProcess* const pProcess)
{
	if(pProcess)
	{
		m_rSimState.GetAnalysisState().AddProcess(pProcess);
	}
}

// Function to add a new command to the CSimState's command sequence. 
// Such commands can be generated by other commands (e.g., WallOn(), WallOff()),
// or by processes, or entered by the user interactively. It is visible 
// from the ISimBox so that processes, the experiment and other entities 
// can access it. We check that the command pointer is non-NULL before using it.

void ISimState::AddCommand(const xxCommand *const pCommand)
{
	// Check that the command execution time is still in the future, and
	// find where the command should be added. If the command's execution
	// time has passed, we just add it to the CAnalysisState for destruction
	// but do not schedule it for execution.

	if(pCommand)
	{
		const long targetTime = pCommand->GetExecutionTime();

		if(targetTime > m_SimTime)
		{
			bool bNotInserted = true;
			CommandIterator iterCmd = m_Commands.begin();

			// Find where the new command should be inserted: insert it after 
			// any commands with the same execution time.

			while(bNotInserted && iterCmd!=m_Commands.end())
			{
				if((*iterCmd)->GetExecutionTime() > targetTime)
				{
					bNotInserted = false;
					m_Commands.insert(iterCmd, pCommand);
				}
				else
					iterCmd++;
			}

			// If the command has not yet been inserted add it to the end

			if(bNotInserted)
            {
				m_Commands.insert(m_Commands.end(), pCommand);
            }

			// Add it to the CAnalysisState so that it gets destroyed appropriately

			m_rSimState.GetAnalysisState().AddCommand(pCommand);
		}
		else
		{
			m_rSimState.GetAnalysisState().AddCommand(pCommand);
		}
	}
}

// Function to add a command to the front of the command queue.
// It provides a more efficient access than the above function when the command
// is to be executed immediately. This function must NOT be called by a 
// command implementation class that results in new commands being created,
// or an infinite loop occurs. This is because the initiating command is still
// in the queue when it executes, and new commands will be pushed onto the front
// before it.

void ISimState::AddCommandToFront(const xxCommand *const pCommand)
{
	if(pCommand)
	{
		m_rSimState.GetAnalysisState().AddCommand(pCommand);
		m_Commands.insert(m_Commands.begin(), pCommand);
	}
}

// Function to add a command to the front of the command queue but after all
// commands at the current time. Hence, new commands are added behind any 
// existing commands in the queue that execute at the same time. Note that
// the added command must have an execution time equal to the current time or it
// is ignored. However, we still add the command to the analysis state to ensure
// that it gets deleted properly.

void ISimState::AddCommandAfterFront(const xxCommand *const pCommand)
{
	if(pCommand)
	{
        if(pCommand->GetExecutionTime() == GetCurrentTime())
        {
		    // Insert the command into the queue after all other commands whose 
            // execution time is the current time. 

            bool bNotInserted = true;
            CommandIterator iterCmd = m_Commands.begin();

		    while(bNotInserted && iterCmd!=m_Commands.end())
		    {
			    if((*iterCmd)->GetExecutionTime() > GetCurrentTime())
			    {
				    bNotInserted = false;
				    m_Commands.insert(iterCmd, pCommand);
			    }
			    else
				    iterCmd++;
		    }

            // If the queue is empty, or all commands are scheduled to execute
            // at the current time, we add the command to the back of the queue.
            if(bNotInserted)
            {
				m_Commands.insert(m_Commands.end(), pCommand);
            }

            // We must add the command to the analysis state for proper deletion later

            m_rSimState.GetAnalysisState().AddCommand(pCommand);
        }
        else
        {
            m_rSimState.GetAnalysisState().AddCommand(pCommand);
        }     
    }
}

// ****************************************
// Functions to turn on/off constraints in the CSimState

void ISimState::SetActiveBondsOn(bool bActive)
{
	m_rSimState.SetActiveBondsOn(bActive);
}

void ISimState::SetBeadChargeOn(bool bCharge)
{
	m_rSimState.SetBeadChargeOn(bCharge);
}

void ISimState::SetBeadStressContributionOn(bool bStress)
{
	m_rSimState.SetBeadStressContributionOn(bStress);
}

void ISimState::SetBondStressContributionOn(bool bStress)
{
	m_rSimState.SetBondStressContributionOn(bStress);
}

void ISimState::SetBondPairStressContributionOn(bool bStress)
{
	m_rSimState.SetBondPairStressContributionOn(bStress);
}

void ISimState::SetGravityOn(bool bGravity)
{
	m_rSimState.SetGravityOn(bGravity);
}

void ISimState::SetRenormaliseMomenta(bool bRenormalise)
{
	m_rSimState.SetRenormaliseMomenta(bRenormalise);
}

void ISimState::SetWallOn(bool bWall)
{
	m_rSimState.SetWallOn(bWall);
}

// ****************************************
// Access functions to the CSimState and its enclosed objects. 
// These return information about the simulation state but do not 
// allow it to be modified.

// Function that identifies the simulation

const zString ISimState::GetRunId() const
{
	return m_rSimState.GetRunId();
}

// Functions that return boolean information about the CSimState

bool ISimState::TimeToSample() const
{
	return (m_SimTime%m_rSimState.GetSamplePeriod() == 0);
}

bool ISimState::TimeToDisplay() const
{
	return (m_SimTime%m_rSimState.GetDisplayPeriod() == 0);
}

bool ISimState::TimeToRestart() const
{
	return (m_SimTime%m_rSimState.GetRestartPeriod() == 0);
}

bool ISimState::IsActiveBondsOn() const
{
	return m_rSimState.IsActiveBondsOn();
}

bool ISimState::IsBeadChargeOn() const
{
	return m_rSimState.IsBeadChargeOn();
}

bool ISimState::IsBeadStressAdded() const
{
	return m_rSimState.IsBeadStressAdded();
}

bool ISimState::IsBondStressAdded() const
{
	return m_rSimState.IsBondStressAdded();
}

bool ISimState::IsBondPairStressAdded() const
{
	return m_rSimState.IsBondPairStressAdded();
}

bool ISimState::IsEnergyOutputOn() const
{
	return m_rSimState.IsEnergyOutputOn();
}

bool ISimState::IsGravityOn() const
{
	return m_rSimState.IsGravityOn();
}

bool ISimState::IsRenormalisationOn() const
{
	return m_rSimState.IsRenormalisationOn();
}

bool ISimState::IsWallOn() const
{
	return m_rSimState.IsWallOn();
}

bool ISimState::IsWallPolymerFlexible() const
{
	return m_rSimState.IsWallPolymerFlexible();
}

// ****************************************
// Functions that return information about the physical state
// of the simulation

double ISimState::GetCNTXCellWidth() const
{
	return m_rSimState.GetCNTXCellWidth();
}

double ISimState::GetCNTYCellWidth() const
{
	return m_rSimState.GetCNTYCellWidth();
}

double ISimState::GetCNTZCellWidth() const
{
	return m_rSimState.GetCNTZCellWidth();
}

double ISimState::GetSimBoxXLength() const
{
	return m_rSimState.GetSimBoxXLength();
}

double ISimState::GetSimBoxYLength() const
{
	return m_rSimState.GetSimBoxYLength();
}

double ISimState::GetSimBoxZLength() const
{
	return m_rSimState.GetSimBoxZLength();
}

double ISimState::GetSimBoxBulkXLength() const
{
	return m_rSimState.GetInitialState().GetSimBoxBulkXLength();
}

double ISimState::GetSimBoxBulkYLength() const
{
	return m_rSimState.GetInitialState().GetSimBoxBulkYLength();
}

double ISimState::GetSimBoxBulkZLength() const
{
	return m_rSimState.GetInitialState().GetSimBoxBulkZLength();
}

double ISimState::GetWallXWidth() const
{
	return m_rSimState.GetInitialState().GetWallXWidth();
}

double ISimState::GetWallYWidth() const
{
	return m_rSimState.GetInitialState().GetWallYWidth();
}

double ISimState::GetWallZWidth() const
{
	return m_rSimState.GetInitialState().GetWallZWidth();
}

double ISimState::GetkT() const
{
	return m_rSimState.GetkT();
}

double ISimState::GetLengthScale() const
{
	return m_rSimState.GetLengthScale();
}

double ISimState::GetEnergyScale() const
{
	return m_rSimState.GetEnergyScale();
}

// ****************************************
// Functions that return the number of bead types, the numbers
// of each bead type, etc.

long ISimState::GetBeadTypeTotal() const
{
	return m_rSimState.GetBeadTypeTotal();
}

long ISimState::GetBondTypeTotal() const
{
	return m_rSimState.GetBondTypeTotal();
}

long ISimState::GetBondPairTypeTotal() const
{
	return m_rSimState.GetBondPairTypeTotal();
}

long ISimState::GetPolymerTypeTotal() const
{
	return m_rSimState.GetPolymerTypeTotal();
}

long ISimState::GetBeadTotalForType(long type) const
{
	return m_rSimState.GetInitialState().GetBeadTotalForType(type);
}

long ISimState::GetBondTotalForType(long type) const
{
	return m_rSimState.GetInitialState().GetBondTotalForType(type);
}

long ISimState::GetBondPairTotalForType(long type) const
{
	return m_rSimState.GetInitialState().GetBondPairTotalForType(type);
}

long ISimState::GetPolymerTotalForType(long type) const
{
	return m_rSimState.GetInitialState().GetPolymerTotalForType(type);
}

// Functions to return the set of exemplars of each bead, bond, bondpair and polymer type.

const BeadVector& ISimState::GetBeadTypes() const
{
    return m_rSimState.GetBeadTypes();
}

const BondVector&  ISimState::GetBondTypes()  const
{
    return m_rSimState.GetBondTypes();
}

const BondPairVector& ISimState::GetBondPairTypes() const
{
    return m_rSimState.GetBondPairTypes();
}

const PolymerVector& ISimState::GetPolymerTypes() const
{
    return m_rSimState.GetPolymerTypes();
}

// ****************************************
// Functions that query the polymer structures

long ISimState::GetBeadTypeFromName(const zString name) const
{
	return m_rSimState.GetAnalysisState().GetBeadTypeFromName(name);
}

long ISimState::GetBondTypeFromName(const zString name) const
{
	return m_rSimState.GetAnalysisState().GetBondTypeFromName(name);
}

long ISimState::GetBondPairTypeFromName(const zString name) const
{
	return m_rSimState.GetAnalysisState().GetBondPairTypeFromName(name);
}

long ISimState::GetPolymerTypeFromName(const zString name) const
{
	return m_rSimState.GetAnalysisState().GetPolymerTypeFromName(name);
}

long ISimState::GetPolymerHeadType(long polyType) const
{
	return m_rSimState.GetAnalysisState().GetPolymerHeadType(polyType);
}

long ISimState::GetPolymerTailType(long polyType) const
{
	return m_rSimState.GetAnalysisState().GetPolymerTailType(polyType);
}

const zString ISimState::GetBeadNameFromType(long type) const
{
	return m_rSimState.GetAnalysisState().GetBeadNameFromType(type);
}

const zString ISimState::GetBondNameFromType(long type) const
{
	return m_rSimState.GetAnalysisState().GetBondNameFromType(type);
}

const zString ISimState::GetBondPairNameFromType(long type) const
{
	return m_rSimState.GetAnalysisState().GetBondPairNameFromType(type);
}

const zString ISimState::GetPolymerNameFromType(long type) const
{
	return m_rSimState.GetAnalysisState().GetPolymerNameFromType(type);
}

long ISimState::GetProcessorsXNo() const
{
	return m_rSimState.GetProcessorsXNo();
}

long ISimState::GetProcessorsYNo() const
{
	return m_rSimState.GetProcessorsYNo();
}

long ISimState::GetProcessorsZNo() const
{
	return m_rSimState.GetProcessorsZNo();
}

long ISimState::GetGridXCellNo() const
{
    return m_rSimState.GetGridXCellNo();
}

long ISimState::GetGridYCellNo() const
{
    return m_rSimState.GetGridYCellNo();
}

long ISimState::GetGridZCellNo() const
{
    return m_rSimState.GetGridZCellNo();
}

long ISimState::GetCNTXCellNo() const
{
	return m_rSimState.GetCNTXCellNo();
}

long ISimState::GetCNTYCellNo() const
{
	return m_rSimState.GetCNTYCellNo();
}

long ISimState::GetCNTZCellNo() const
{
	return m_rSimState.GetCNTZCellNo();
}

// Function used by the CSimBox to obtain the latest set of bonds in the simulation.
// This is needed after commands that modify the bond totals.

const BondVector ISimState::GetAllBonds() const
{
	return m_rSimState.GetBonds();
}

const BondVector ISimState::GetAllPolymerisedBonds() const
{
	return m_rSimState.GetPolymerisedBonds();
}

// Function to return the set of command targets imported into the initial state
// during a restart run. The return value of this function must NOT be used directly
// in a loop, but must be copied into a local variable. It seems that using the construct:
//
//  for(CommandTargetIterator iterTarget=GetCommandTargets().begin(); iterTarget!=GetCommandTargets().end(); iterTarget++)
//    {
//        std::cout << "found target " << (*iterTarget)->GetLabel() << " with inner/outer decs " << (*iterTarget)->GetInnerDecorator()->GetLabel() << " " << (*iterTarget)->GetOuterDecorator()->GetLabel() << zEndl;
//    }
//
// yields null pointers. This may me a flaw in the STL container.

CommandTargetSequence ISimState::GetCommandTargets() const
{
    return m_rSimState.GetInitialState().GetCommandTargets();
}

// Function to return a flag showing if the density-depdendent DPD force
// is enabled and actually being used.

bool ISimState::IsDPDLG() const
{
#if EnableDPDLG == ExperimentEnabled
    return m_rSimState.IsDPDLG();
#elif EnableDPDLG == ExperimentDisabled
    return false;
#endif
}

// Functions to return the origin of  the owning processor's SimBox in the whole
// simulation space.

double ISimState::GetSimBoxXOrigin() const
{
    return m_rSimState.GetSimBoxXOrigin();
}

double ISimState::GetSimBoxYOrigin() const
{
    return m_rSimState.GetSimBoxYOrigin();
}

double ISimState::GetSimBoxZOrigin() const
{
    return m_rSimState.GetSimBoxZOrigin();
}

// Functions to return the size of the whole simulation Space. This is just the
// SimBox size for the serial code, but the World size for the parallel code.

double ISimState::GetSimSpaceXLength() const
{
	return m_rSimState.GetSimSpaceXLength();
}

double ISimState::GetSimSpaceYLength() const
{
	return m_rSimState.GetSimSpaceYLength();
}

double ISimState::GetSimSpaceZLength() const
{
	return m_rSimState.GetSimSpaceZLength();
}

