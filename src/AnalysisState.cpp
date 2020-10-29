/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// AnalysisState.cpp: implementation of the CAnalysisState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "AnalysisState.h"
#include "InputData.h"
#include "xxProcess.h"
#include "IModifyProcess.h"
#include "Analysis.h"
#include "xxEvent.h"
#include "xxCommand.h"
#include "Observable.h"

#include "Polymer.h"
#include "AbstractBead.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnalysisState::CAnalysisState(const CInputData& rData) : xxState(xxBase::GetASPrefix() + rData.GetRunId(), true, 0, rData.GetRunId()),
														m_Processes(rData.GetProcesses()),
														m_Analysis(rData.GetAggregates()),
														m_Events(rData.GetEvents()),
														m_Commands(rData.GetCommands()),
														m_pvObservables(NULL),
														m_GridXCellNo(rData.GetGridXCellNo()),
														m_GridYCellNo(rData.GetGridYCellNo()),
														m_GridZCellNo(rData.GetGridZCellNo()),
														m_TotalMCTime(rData.GetTotalMCTime()),
														m_TotalTime(rData.GetTotalTime()),
														m_SamplePeriod(rData.GetSamplePeriod()),
														m_AnalysisPeriod(rData.GetAnalysisPeriod()),
														m_DensityPeriod(rData.GetDensityPeriod()),
														m_DisplayPeriod(rData.GetDisplayPeriod()),
														m_RestartPeriod(rData.GetRestartPeriod()),
														m_Lambda(rData.GetLambda()),
														m_RCutOff(rData.GetCutOffRadius()),
														m_MCStepSize(rData.GetMCStepSize()),
														m_StepSize(rData.GetStepSize())
{
	// Store maps allowing bead names and types to be obtained given either
	// as a key.

	cStringLongIterator cIterName;

	for(cIterName = rData.GetBeadNamesMap().begin();
		cIterName!= rData.GetBeadNamesMap().end(); cIterName++)
	{
		zString name = (*cIterName).first;
		long type    = (*cIterName).second;

		m_mBeadNameFromType.insert(zPairLS(type,name));
		m_mBeadTypeFromName.insert(zPairSL(name,type));
	}

	// Store maps allowing bond names and types to be obtained given either
	// as a key.

	for(cIterName = rData.GetBondNamesMap().begin();
		cIterName!= rData.GetBondNamesMap().end(); cIterName++)
	{
		zString name = (*cIterName).first;
		long type    = (*cIterName).second;

		m_mBondNameFromType.insert(zPairLS(type,name));
		m_mBondTypeFromName.insert(zPairSL(name,type));
	}

	// Copy the maps allowing the bonds' head and tail bead types to be obtained
	// from the bond type

	m_mBondHeadTypes = rData.GetBondHeadTypesMap();
	m_mBondTailTypes = rData.GetBondTailTypesMap();
/*
	LongLongIterator iterHeadType;

	for(iterHeadType = m_mBondHeadTypes.begin();
		iterHeadType!= m_mBondHeadTypes.end(); iterHeadType++)
	{
		long bondType = (*iterHeadType).first;
		long headType = (*iterHeadType).second;
	}

	for(iterHeadType = m_mBondTailTypes.begin();
		iterHeadType!= m_mBondTailTypes.end(); iterHeadType++)
	{
		long bondType = (*iterHeadType).first;
		long tailType = (*iterHeadType).second;
	}
*/
	// Store maps allowing bondpair names and types to be obtained given either
	// as a key.

	for(cIterName = rData.GetBondPairNamesMap().begin();
		cIterName!= rData.GetBondPairNamesMap().end(); cIterName++)
	{
		zString name = (*cIterName).first;
		long type    = (*cIterName).second;

		m_mBondPairNameFromType.insert(zPairLS(type,name));
		m_mBondPairTypeFromName.insert(zPairSL(name,type));
	}

	// Copy the maps allowing the bondpairs' head, middle and tail bead types 
	// to be obtained from the bondpair type

	m_mBondPairHeadTypes	= rData.GetBondPairHeadTypesMap();
	m_mBondPairMiddleTypes	= rData.GetBondPairMiddleTypesMap();
	m_mBondPairTailTypes	= rData.GetBondPairTailTypesMap();

	// Store maps allowing polymer names and types to be obtained given either
	// as a key. Also  create a map allowing the head and tail bead types to
	// be obtained from the polymer type

	for(cIterName = rData.GetPolymerNamesMap().begin();
		cIterName!= rData.GetPolymerNamesMap().end(); cIterName++)
	{
		zString name = (*cIterName).first;
		long type	 = (*cIterName).second;

		m_mPolymerNameFromType.insert(zPairLS(type,name));
		m_mPolymerTypeFromName.insert(zPairSL(name,type));

		long BeadType = rData.GetPolymerTypes().at(type)->GetHead()->GetType();
		m_mPolymerHeadTypes.insert(zPairLL(type, BeadType));

		BeadType = rData.GetPolymerTypes().at(type)->GetTail()->GetType();
		m_mPolymerTailTypes.insert(zPairLL(type, BeadType));
	}

	// Create a map allowing polymer shape strings to be obtained given their type

	m_mPolymerShapes.clear();

	cLongStringIterator cIterShape;

	for(cIterShape = rData.GetPolymerShapesMap().begin();
		cIterShape!= rData.GetPolymerShapesMap().end(); cIterShape++)
	{
		long type	  = (*cIterShape).first;
		zString shape = (*cIterShape).second;

		m_mPolymerShapes.insert(zPairLS(type,shape));
	}

	// Convert the bead, bond and polymer names that were used in the 
	// control data file for use in aggregate analysis into integers. 
	// We use the maps just created.

	for(AggregateIterator iterAn=m_Analysis.begin(); iterAn!=m_Analysis.end(); iterAn++)
	{
		(*iterAn)->ConvertNames(*this);
	}

	// Store the processes that want to allow command modification in a map 
	// using the pair (pid, IModifyProcess*) so that process commands 
	// (pcXXX) can find their targets using the unique process id, pid, as the key.
	// All proceses that inherit from IModifyProcess are modifiable.

	for(ProcessIterator iterProc=m_Processes.begin(); iterProc!=m_Processes.end(); iterProc++)
	{
		IModifyProcess* pIMP = dynamic_cast<IModifyProcess*>(*iterProc);

		if(pIMP)
		{
			m_mModifiableProcesses.insert(zPairSMP(pIMP->GetPid(), pIMP));
		}
	}

    // If we are running a parallel simulation, we need to store the names of
    // scheduled commands on PN.

#if SimMPS == SimulationEnabled

    if(!IsProcessZero())
    {
        m_CommandNames.clear();

        StringSequence names = rData.GetCommandNames();
        for(cStringSequenceIterator is=names.begin(); is!=names.end(); is++)
        {
            m_CommandNames.push_back(*is);
        }
    }
#endif
}

CAnalysisState::~CAnalysisState()
{
	// Delete the xxProcess-derived objects if any exist

	for(ProcessIterator iterProc=m_Processes.begin(); iterProc!=m_Processes.end(); iterProc++)
	{
		delete *iterProc;
	}
	m_Processes.clear();

	// Delete the CAnalysis-derived objects if any exist

	for(AggregateIterator iterAn=m_Analysis.begin(); iterAn!=m_Analysis.end(); iterAn++)
	{
		delete *iterAn;
	}
	m_Analysis.clear();

	// Delete the xxEvents

	for(EventIterator iterEvent=m_Events.begin(); iterEvent!=m_Events.end(); iterEvent++)
	{
		delete *iterEvent;
	}
	m_Events.clear();

	// Delete the xxCommand-derived objects if any exist

	for(CommandIterator iterCom=m_Commands.begin(); iterCom!=m_Commands.end(); iterCom++)
	{
		delete *iterCom;
	}
	m_Commands.clear();

	// Zero the static counters of aggregates and commands in the CAnalysis and
	// xxCommand classes so that subsequent simulations start from scratch.

	xxProcess::ZeroProcessTotal();
	CAnalysis::ZeroAggregateTotal();
	  xxEvent::ZeroEventTotal();
	xxCommand::ZeroCommandTotal();

}

// Function to store a pointer to the vector of CObservables obtained from
// the CMonitor so that it can serialize them as necessary.

void CAnalysisState::AddGlobalData(ObservableSequencePointer pvObservables)
{
	m_pvObservables = pvObservables;
}

// Function to serialize the current data from the CObservable objects 
// created by the CMonitor and those generated by each CAnalysis-derived object.
// The current time must have been updated prior to the CAnalysisState receiving
// the Serialize() message.

bool CAnalysisState::Serialize()
{
	if(m_IOFlag)	
	{
		// Write the current time first to identify the output

		m_outStream << "Time = " << m_CurrentTime << zEndl;

		// Normalize and serialize the global observables created by the CMonitor

		for(ObservableIterator iterOb = (*m_pvObservables).begin(); iterOb != (*m_pvObservables).end(); iterOb++)
		{
			(*iterOb)->Normalize(m_CurrentTime);
#ifdef TraceOn
			(*iterOb)->DisplayDebug();
#endif
			m_outStream << *(*iterOb);
		}

		// Now write out the data for the aggregate analysis using
		// the vectors of CObservables for each CAnalysis-derived object.
		// Data is only written out at the end of a complete analysis period.
		// Because the aggregate analysis does not necessarily start at the 
		// beginning of the simulation, and may occur over different ranges for 
		// different aggregates, we check that the data has been sampled
		// for a complete analysis period before writing the data.

		for(cAggregateIterator iterAn=m_Analysis.begin(); iterAn!=m_Analysis.end(); iterAn++)
		{
			if((*iterAn)->TimeToAnalyse(m_CurrentTime, GetAnalysisPeriod()))
			{
				m_outStream << (*iterAn)->GetType() << " Aggregate" << zEndl;

				for(ObservableIterator iterOb=(*iterAn)->GetObservables().begin(); iterOb!=(*iterAn)->GetObservables().end(); iterOb++)
				{
					(*iterOb)->Normalize(m_CurrentTime);
#ifdef TraceOn
					(*iterOb)->DisplayDebug();
#endif
					m_outStream << *(*iterOb);
				}
			}
		}

	}

	return true;
}

long CAnalysisState::GetBeadTypeFromName(const zString& name) const
{
	if(m_mBeadTypeFromName.find(name) != m_mBeadTypeFromName.end())
		return (*m_mBeadTypeFromName.find(name)).second;
	else
		return -1;
}

zString CAnalysisState::GetBeadNameFromType(long type) const
{
	if(m_mBeadNameFromType.find(type) != m_mBeadNameFromType.end())
		return (*m_mBeadNameFromType.find(type)).second;
	else
		return "Unnamed";
}

long CAnalysisState::GetBondTypeFromName(const zString& name) const
{
	if(m_mBondTypeFromName.find(name) != m_mBondTypeFromName.end())
		return (*m_mBondTypeFromName.find(name)).second;
	else
		return -1;
}

zString CAnalysisState::GetBondNameFromType(long type) const
{
	if(m_mBondNameFromType.find(type) != m_mBondNameFromType.end())
		return (*m_mBondNameFromType.find(type)).second;
	else
		return "Unnamed";
}

long CAnalysisState::GetBondHeadType(long bondType) const
{
	if(m_mBondHeadTypes.find(bondType) != m_mBondHeadTypes.end())
		return (*m_mBondHeadTypes.find(bondType)).second;
	else
		return -1;
}

long CAnalysisState::GetBondTailType(long bondType) const
{
	if(m_mBondTailTypes.find(bondType) != m_mBondTailTypes.end())
		return (*m_mBondTailTypes.find(bondType)).second;
	else
		return -1;
}

long CAnalysisState::GetBondPairTypeFromName(const zString& name) const
{
	if(m_mBondPairTypeFromName.find(name) != m_mBondPairTypeFromName.end())
		return (*m_mBondPairTypeFromName.find(name)).second;
	else
		return -1;
}

zString CAnalysisState::GetBondPairNameFromType(long type) const
{
	if(m_mBondPairNameFromType.find(type) != m_mBondPairNameFromType.end())
		return (*m_mBondPairNameFromType.find(type)).second;
	else
		return "Unnamed";
}

long CAnalysisState::GetBondPairHeadType(long bondPairType) const
{
	if(m_mBondPairHeadTypes.find(bondPairType) != m_mBondPairHeadTypes.end())
		return (*m_mBondPairHeadTypes.find(bondPairType)).second;
	else
		return -1;
}

long CAnalysisState::GetBondPairMiddleType(long bondPairType) const
{
	if(m_mBondPairMiddleTypes.find(bondPairType) != m_mBondPairMiddleTypes.end())
		return (*m_mBondPairMiddleTypes.find(bondPairType)).second;
	else
		return -1;
}

long CAnalysisState::GetBondPairTailType(long bondPairType) const
{
	if(m_mBondPairTailTypes.find(bondPairType) != m_mBondPairTailTypes.end())
		return (*m_mBondPairTailTypes.find(bondPairType)).second;
	else
		return -1;
}

long CAnalysisState::GetPolymerTypeFromName(const zString& name) const
{
	if(m_mPolymerTypeFromName.find(name) != m_mPolymerTypeFromName.end())
		return (*m_mPolymerTypeFromName.find(name)).second;
	else
		return -1;
}

zString CAnalysisState::GetPolymerNameFromType(long type) const
{
	if(m_mPolymerNameFromType.find(type) != m_mPolymerNameFromType.end())
		return (*m_mPolymerNameFromType.find(type)).second;
	else
		return "Unknown";
}

long CAnalysisState::GetPolymerHeadType(long polyType) const
{
	if(m_mPolymerHeadTypes.find(polyType) != m_mPolymerHeadTypes.end())
		return (*m_mPolymerHeadTypes.find(polyType)).second;
	else
		return -1;
}

long CAnalysisState::GetPolymerTailType(long polyType) const
{
	if(m_mPolymerTailTypes.find(polyType) != m_mPolymerTailTypes.end())
		return (*m_mPolymerTailTypes.find(polyType)).second;
	else
		return -1;
}

// Function to return a polymer's shape string given its type identifier.
// If the polymer type is invalid we return an empty string.

const zString CAnalysisState::GetPolymerShapeFromType(long polyType) const
{
	if(m_mPolymerShapes.find(polyType) != m_mPolymerShapes.end())
		return (*m_mPolymerShapes.find(polyType)).second;
	else
		return "";
}

// Function to return a polymer's shape string with terminal spaces. It is similar
// to the above function except it guarantees that the shape string has a single
// space at the front and back of the string. Note that if the polymer type is invalid
// the above function returns an empty string and this is checked in
// xxFile::RemoveTerminalSpaces so that AddTerminalSpaces results in a string 
// containing a single space being returned. Hence, all strings returned by this
// function contain a single space at the start and end of the string, although 
// for an empty string these are the same space.

const zString CAnalysisState::GetSpaceWrappedPolymerShapeFromType(long polyType) const
{
	return AddTerminalSpaces(RemoveTerminalSpaces(GetPolymerShapeFromType(polyType)));
}

// Function to allow a process created during a run to be added to the set of 
// running processes. Because processes are not sequential we just add it
// to the end of the sequence. We check if the process is modifiable and
// add it to the modifiable processes container if so.
//

void CAnalysisState::AddProcess(xxProcess* pProcess)
{
	m_Processes.insert(m_Processes.end(), pProcess);

	IModifyProcess* pIMP = dynamic_cast<IModifyProcess*>(pProcess);

	if(pIMP)
	{
		m_mModifiableProcesses.insert(zPairSMP(pIMP->GetPid(), pIMP));
	}

}

// Function to allow a command created during a run to be added to the set of commands
// created from the input control data file so that they can be safely deleted at the
// end of the run. 
//
// The new command is added to the sequence using its execution time to find the 
// appropriate location. Note that we add the command after all commands with the same
// execution time (if any). If the target time is larger than the execution times of 
// all commands in the vector we add the new command to the end. This way the command
// will always be added to the vector whatever its creation time and no errors can occur.

void CAnalysisState::AddCommand(const xxCommand * const pCmd)
{
	long targetTime = pCmd->GetExecutionTime();

	bool bInsertedFlag = false;

	CommandIterator iterCmd = m_Commands.begin();

	// Find where the new command should be inserted: insert it after any commands with 
	// the same execution time.

	while(iterCmd!=m_Commands.end() && !bInsertedFlag)
	{
		if((*iterCmd)->GetExecutionTime() > targetTime)
		{
			bInsertedFlag = true;
			m_Commands.insert(iterCmd, pCmd);
		}
		else
			iterCmd++;
	}

	// If the command has not yet been inserted add it to the end

	if(!bInsertedFlag)
		m_Commands.insert(m_Commands.end(), pCmd);

}

// Function to allow Events to be added to the event sequence during a simulation.
// This allows new events to be monitored when the simulation detects that certain
// conditions have been fulfilled. Because events are not sequential we just add it
// to the end of the sequence.

void CAnalysisState::AddEvent(xxEvent *pEvent)
{
	m_Events.insert(m_Events.end(), pEvent);
}

// Function to return a pointer to the IModifyProcess interface of an xxProcess
// if it is implemented by the named process. Note that this function should
// only return singleton processes, as more than one instance of a named process
// will create an ambiguity as to which one to return.

IModifyProcess* CAnalysisState::GetModifiableProcessFromName(const zString name) const
{
	// Iterate through the map and compare the names of each process with that 
	// specified

	IModifyProcess* pTargetProcess = 0;

	for(cStringModifiableProcessIterator citerProc=m_mModifiableProcesses.begin(); citerProc!=m_mModifiableProcesses.end(); citerProc++)
	{
		IModifyProcess* const pProcess = citerProc->second;
		const zString targetName = pProcess->GetProcessType();

		if(name == targetName)
		{
			pTargetProcess = pProcess;
		}
	}

	return pTargetProcess;
}

// Function to return a pointer to the IModifyProcess interface of an xxProcess
// if it is implemented by the process whose id is specified.

IModifyProcess* CAnalysisState::GetModifiableProcessFromPid(const zString pid) const
{
	if(m_mModifiableProcesses.find(pid) != m_mModifiableProcesses.end())
		return (*m_mModifiableProcesses.find(pid)).second;
	else
		return 0;
}

// Function to return a container holding modifiable processes. Because they
// are stored in a map we copy the process pointers into a sequential 
// container to return them.

ProcessSequence CAnalysisState::GetModifiableProcesses() const
{
	ProcessSequence modProcesses;

	modProcesses.clear();

	for(cStringModifiableProcessIterator citerProc=m_mModifiableProcesses.begin(); citerProc!=m_mModifiableProcesses.end(); citerProc++)
	{
		xxProcess* const pProcess = citerProc->second;

		modProcesses.push_back(pProcess);
	}

	return modProcesses;
}

// Function to associate a bead type's name and its numeric
// type identifier in two maps. Note that this does not create a new type, 
// nor modify any existing beads: it simply stores the name,type association
// in the maps for later access. We check that the bead name is not empty, 
// its type is not negative, and that it does not already occur in the map, 
// if these conditions are not met, the function returns false.

bool CAnalysisState::AddNamedBeadTypeToMap(long type, const zString name)
{
	if(name.empty() || type < 0 || IsBeadTypeInMap(type) || IsBeadNameInMap(name))
    {
		return false;
    }
	else
	{
		m_mBeadNameFromType.insert(zPairLS(type,name));
		m_mBeadTypeFromName.insert(zPairSL(name,type));
	}

	return true;
}

// Function to associate a bond's name and its numeric type identifier in two 
// maps. It is essentially the same as the above function for bead types.

bool CAnalysisState::AddNamedBondTypeToMap(long type, const zString name)
{
	if(name.empty() || type < 0 || IsBondTypeInMap(type) || IsBondNameInMap(name))
    {
		return false;
    }
	else
	{
		m_mBondNameFromType.insert(zPairLS(type,name));
		m_mBondTypeFromName.insert(zPairSL(name,type));
	}

	return true;
}

// Function to associate a bondpair's name and its numeric type identifier in two 
// maps. It is essentially the same as the above function for bead types.


bool CAnalysisState::AddNamedBondPairTypeToMap(long type, const zString name)
{
	if(name.empty() || type < 0 || IsBondPairTypeInMap(type) || IsBondPairNameInMap(name))
    {
		return false;
    }
	else
	{
		m_mBondPairNameFromType.insert(zPairLS(type,name));
		m_mBondPairTypeFromName.insert(zPairSL(name,type));
	}

	return true;
}

// Helper function to return true if a specified bead name already exists
// in the beadnames map. We ignore the actual type associated with the name
// if it exists.

bool CAnalysisState::IsBeadNameInMap(const zString name) const
{
	return (m_mBeadTypeFromName.find(name) != m_mBeadTypeFromName.end());
}

// Helper function to return true if a specified bead type already exists
// in the beadtypes map. We ignore the name associated with the type
// if it exists.

bool CAnalysisState::IsBeadTypeInMap(long type) const
{
	return (m_mBeadNameFromType.find(type) != m_mBeadNameFromType.end());
}

// Helper function to return true if a specified bond name already exists
// in the map. We ignore the actual type associated with the name
// if it exists.

bool CAnalysisState::IsBondNameInMap(const zString name) const
{
	return (m_mBondTypeFromName.find(name) != m_mBondTypeFromName.end());
}

// Helper function to return true if a specified bond type already exists
// in the map. We ignore the name associated with the type
// if it exists.

bool CAnalysisState::IsBondTypeInMap(long type) const
{
	return (m_mBondNameFromType.find(type) != m_mBondNameFromType.end());
}

// Helper function to return true if a specified bondpair name already exists
// in the map. We ignore the actual type associated with the name
// if it exists.

bool CAnalysisState::IsBondPairNameInMap(const zString name) const
{
	return (m_mBondPairTypeFromName.find(name) != m_mBondPairTypeFromName.end());
}

// Helper function to return true if a specified bondpair type already exists
// in the map. We ignore the name associated with the type
// if it exists.

bool CAnalysisState::IsBondPairTypeInMap(long type) const
{
	return (m_mBondPairNameFromType.find(type) != m_mBondPairNameFromType.end());
}

bool CAnalysisState::IsPolymerNameInMap(const zString name) const
{
	return (m_mPolymerTypeFromName.find(name) != m_mPolymerTypeFromName.end());
}

bool CAnalysisState::IsPolymerTypeInMap(long type) const
{
	return (m_mPolymerNameFromType.find(type) != m_mPolymerNameFromType.end());
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. The data includes structures, 
// such as the DPD bead-bead conservative and dissipative interaction parameters, 
// that can be modified by command during  a run, and which therefore may be 
// different from their values in the CDF. The restarted run can thus recreate the
// dynamic state of the old run in its initial state.


// Function to read data from a restart state file and overwrite the values
// of all parameters that were dynamically modified in the old run.

zInStream& CAnalysisState::Read(zInStream& is)
{
    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// Data are typically modified by command (or some other means). To allow 
// for extensibility we write out data that cannot yet be modified, such as 
// the maps of polymer names and type.
//
// The data for all types of simulation are the following:
//
// AnalysisState data
// *****************
//
//  Note that pairs of maps of the form (string name, numeric type), (type, name) 
//  are written out one way round only as the reading routines can reconstruct 
//  the pair of maps given just one of them.
//
//  Map of (bead type/name) pairs
//  Map of (bond type/name) pairs
//  Map of (bondpair type/name) pairs
//  Map of (polymer type/name) pairs
//  Map of (bond type, head/tail bead type)
//  Map of (bondpair type, head/middle/tail bead type)
//  Map of (polymer type, head/tail bead type)
//

zOutStream& CAnalysisState::Write(zOutStream& os) const
{
    cLongStringIterator citerLS;  // iterators used below
    cLongLongIterator   citerLL; 
    cLongLongIterator   citerLL2; 
    cLongLongIterator   citerLL3; 

    os << zEndl; // Start with a new line

    // Bead maps
    for(citerLS=m_mBeadNameFromType.begin(); citerLS!=m_mBeadNameFromType.end(); citerLS++)
    {
        os << citerLS->first << " " << citerLS->second << zEndl;
    }

    // Bond maps
    for(citerLS=m_mBondNameFromType.begin(); citerLS!=m_mBondNameFromType.end(); citerLS++)
    {
        os << citerLS->first << " " << citerLS->second << zEndl;
    }

    // BondPair maps
    for(citerLS=m_mBondPairNameFromType.begin(); citerLS!=m_mBondPairNameFromType.end(); citerLS++)
    {
        os << citerLS->first << " " << citerLS->second << zEndl;
    }

    // Polymer maps
    for(citerLS=m_mPolymerNameFromType.begin(); citerLS!=m_mPolymerNameFromType.end(); citerLS++)
    {
        os << citerLS->first << " " << citerLS->second << zEndl;
    }

    // Bond head/tail maps: we combine the data for the bond head/tail bead types

    citerLL2 = m_mBondTailTypes.begin();

    for(citerLL=m_mBondHeadTypes.begin(); citerLL!=m_mBondHeadTypes.end(); citerLL++)
    {
        os << citerLL->first << " " << citerLL->second << " "  << citerLL2->second << zEndl;
        citerLL2++;
    }

    // BondPair head/middle/tail maps

    citerLL2 = m_mBondPairMiddleTypes.begin();
    citerLL3 = m_mBondPairTailTypes.begin();

    for(citerLL=m_mBondPairHeadTypes.begin(); citerLL!=m_mBondPairHeadTypes.end(); citerLL++)
    {
        os << citerLL->first << " " << citerLL->second << " " << citerLL2->second << " " << citerLL3->second << zEndl;
        citerLL2++;
        citerLL3++;
    }

    // Polymer head/tail maps

    citerLL2=m_mPolymerTailTypes.begin();

    for(citerLL=m_mPolymerHeadTypes.begin(); citerLL!=m_mPolymerHeadTypes.end(); citerLL++)
    {
        os << citerLL->first << " " << citerLL->second << " " << citerLL2->second << zEndl;
        citerLL2++;
    }

    return os;
}




