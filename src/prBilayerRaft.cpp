/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prBilayerRaft.cpp: implementation of the prBilayerRaft class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "prBilayerRaft.h"
#include "CompositeBilayer.h"
#include "evLamellaMonolayerDomain.h"
#include "evLamellaMonolayerPhaseSeparation.h"
#include "evLamellaBilayerDomain.h"
#include "evLamellaBilayerPhaseSeparation.h"
#include "LogCreateEventFailed.h"
#include "SimState.h"
#include "ISimBox.h"
#include "TimeSeriesData.h"
#include "InputData.h"
#include "aaRegionToType.h"

// STL using declarations


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.

const zString prBilayerRaft::m_Type = "BilayerRaft";

const zString prBilayerRaft::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	xxProcess* Create() {return new prBilayerRaft();}

	const zString id = prBilayerRaft::GetType();

	const bool bRegistered = acfProcessFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

prBilayerRaft::prBilayerRaft() : xxProcess(true),
								m_AggregateName(""),
								m_AggregateId(0),
								m_EventName(""),
								m_EventId(0),
								m_ProbeName(""),
								m_pBilayer(NULL),
								m_pMonolayerDomain(NULL),
								m_pMonolayerPhaseSep(NULL),
								m_pBilayerDomain(NULL),
								m_pBilayerPhaseSep(NULL),
								m_ProbeType(-1),
								m_ProbeHeadBeadType(-1),
								m_bDomains(false),
								m_bPhaseSeparation(false)
{
}

prBilayerRaft::~prBilayerRaft()
{

}

// Member functions to write/read the data specific to the process.

zOutStream& prBilayerRaft::put(zOutStream& os) const
{
	// Write base class data first

	xxProcess::put(os);
	
	os << "    Aggregate	" << m_AggregateName << "  " << m_AggregateId << zEndl;
	os << "    Event    	" << m_EventName     << "  " << m_EventId     << zEndl;
	os << "    Probe    	" << m_ProbeName     << zEndl;

	return os;
}

zInStream& prBilayerRaft::get(zInStream& is)
{
	// Read base class data first

	xxProcess::get(is);

	zString token;
	zString aggregate, event;
	long aggId, eventId;
	zString probe;


	is >> token;
	if(!is.good() || token != "Aggregate")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> aggregate >> aggId;
		if(!is.good() || aggregate.empty() || aggId < 1)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Event")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> event >> eventId;
		if(!is.good() || event.empty() || eventId < 1)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Probe")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> probe;
		if(!is.good() || event.empty())
		{
			SetDataValid(false);
			return is;
		}
	}


	m_AggregateName		= aggregate;
	m_AggregateId		= aggId;
	m_EventName			= event;
	m_EventId			= eventId;
	m_ProbeName			= probe;


	return is;
}


// Non-static function to return the type of the process

const zString prBilayerRaft::GetProcessType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current process.

xxProcess* prBilayerRaft::GetProcess() const
{
	return new prBilayerRaft(*this);
}

// Function to update the state of the process using data from the associated
// aggregate (CCompositeBilayer) and events (evLamellaMonolayerDomain,...).

void prBilayerRaft::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	// Check for raft formation by seeing if the evLamellaMonolayerDomain event 
	// has become inactive since the last check. If so, set the m_bDomains flag to true,
	// calculate the raft properties. Create an evLamellaMonolayerPhaseSeparation
	// event to start watching for complete phase selaration. The new event must 
	// be manually added to the CAnalysisState's event sequence because it is 
	// only here that we have access to the CSimState.


}

// Store a pointer to the CAnalysis-derived object that the process
// will monitor for activity during the simulation. This is const
// because we don't allow data to be written back to the aggregate.
// If more than one aggregate is needed we need to add more functions.

void prBilayerRaft::SetAggregate(const CAnalysis* pAggregate)
{
	m_pBilayer = dynamic_cast<const CCompositeBilayer*>(pAggregate);
}

// Store a pointer to the xxEvent-derived object that the process
// will use to track changes to the aggregate. 

void prBilayerRaft::SetEvent(xxEvent* pEvent)
{
	m_pMonolayerDomain = dynamic_cast<evLamellaMonolayerDomain*>(pEvent);

	// Store the pointer in the xxProcess base class as well so the event
	// can be activated/inactivated as the process requires. But note that
	// events created by the process must be added to the CAnalysisState
	// so that they can be deleted when the run ends: processes do not
	// delete their own aggregates, commands or events.

	AddEvent(pEvent, pEvent->GetStartTime(), pEvent->GetEndTime());
}

// Function to check that the user-specified data is valid and, if so, 
// to find the aggregates and events that are used by the process.
//
// An xxProcessState is created to hold the data that the process wants
// to write to file. Data is passed as a series of CTimeSeriesData objects
// as used for CHistoryState.

bool prBilayerRaft::ValidateData(const CInputData &riData)
{
	// Does the specified aggregate exist? If so store a pointer to it

	cAggregateIterator cIterAgg = find_if(riData.GetAggregates().begin(), 
										  riData.GetAggregates().end(), aaGetAggregateId(m_AggregateId));

	if(cIterAgg != riData.GetAggregates().end() && (*cIterAgg)->GetType() == m_AggregateName)
		SetAggregate(*cIterAgg);
	else
		return false;

	// Does the specified event exist? If so store a pointer to it

	cEventIterator cIterEvent = find_if(riData.GetEvents().begin(), 
										riData.GetEvents().end(), aaGetEventId(m_EventId));

	if(cIterEvent != riData.GetEvents().end() && (*cIterEvent)->GetEventType() == m_EventName)
		SetEvent(*cIterEvent);
	else
		return false;

	// Check the probe polymer exists

	if(riData.GetPolymerNamesMap().find(m_ProbeName) != riData.GetPolymerNamesMap().end())
	{
		m_ProbeType = (*riData.GetPolymerNamesMap().find(m_ProbeName)).second;
		const CPolymer* const pPolymer = riData.GetPolymerTypes().at(m_ProbeType);

		m_ProbeHeadBeadType = pPolymer->GetHead()->GetType();
	}
	else
		return false;


	// All data has been validated so create and store an xxProcessState object
	// to allow the xxProcess to write its data to a process state file.

    SetState(new xxProcessState(xxBase::GetPSPrefix() + GetProcessType() + ToString(GetId()) + "." + riData.GetRunId(), GetStartTime(), GetEndTime()));

	return true;
}

