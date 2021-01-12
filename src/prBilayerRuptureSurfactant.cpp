/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prBilayerRuptureSurfactant.cpp: implementation of the prBilayerRuptureSurfactant class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "prBilayerRuptureSurfactant.h"
#include "Bilayer.h"
#include "evLamellaPore.h"
#include "evLamellaPoreClosed.h"
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

const zString prBilayerRuptureSurfactant::m_Type = "BilayerRuptureSurfactant";

const zString prBilayerRuptureSurfactant::GetType()
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
	xxProcess* Create() {return new prBilayerRuptureSurfactant();}

	const zString id = prBilayerRuptureSurfactant::GetType();

	const bool bRegistered = acfProcessFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

prBilayerRuptureSurfactant::prBilayerRuptureSurfactant() : xxProcess(true),
													m_AggregateName(""),
													m_AggregateId(0),
													m_EventName(""),
													m_EventId(0),
													m_pBilayer(0),
													m_pLamellaPore(0),
													m_pLamellaPoreClosed(0),
													m_bPoreOpened(false),
													m_FirstOpenedTime(0),
													m_FlickerNo(0),
													m_PoreSize(0.0),
													m_PoreRg2(0.0),
													m_PoreArea(0.0),
													m_SurfaceTension(0.0)
{
}

prBilayerRuptureSurfactant::~prBilayerRuptureSurfactant()
{

}

// Member functions to write/read the data specific to the process.

zOutStream& prBilayerRuptureSurfactant::put(zOutStream& os) const
{
	// Write base class data first

	xxProcess::put(os);
	
	os << "    Aggregate	" << m_AggregateName << "  " << m_AggregateId << zEndl;
	os << "    Event    	" << m_EventName     << "  " << m_EventId     << zEndl;

	return os;
}

zInStream& prBilayerRuptureSurfactant::get(zInStream& is)
{
	// Read base class data first

	xxProcess::get(is);

	zString token;
	zString aggregate, event;
	long aggId, eventId;


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


	m_AggregateName		= aggregate;
	m_AggregateId		= aggId;
	m_EventName			= event;
	m_EventId			= eventId;


	return is;
}


// Non-static function to return the type of the process

const zString prBilayerRuptureSurfactant::GetProcessType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current process.

xxProcess* prBilayerRuptureSurfactant::GetProcess() const
{
	return new prBilayerRuptureSurfactant(*this);
}

// Function to update the state of the process using data from the associated
// aggregate (CBilayer) and events (evLamellaPore, evLamellaPoreClosed).

void prBilayerRuptureSurfactant::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	// Check for the pore opening by seeing if the evLamellaPore event has become
	// inactive since the last check. If so, set the m_bPoreOpened flag to true,
	// calculate the pore properties and create an evLamellaPoreClosed event
	// to start watching for pore closure. The new event must be manually added 
	// to the CAnalysisState's event sequence because it is only here that we 
	// have access to the CSimState.

	if(m_bPoreOpened)	// Pore was open
	{
		if(m_pLamellaPoreClosed->IsActive())	// Pore is still open
		{
			// Calculate pore properties here and store the data in a TSD object, 
			// passing it to the xxProcessState object for serialisation. We don't 
			// have to delete the CTimeSeriesData objects 
			// created here as the xxProcessState object will do this.

			m_PoreSize = static_cast<double>(m_pBilayer->GetPoreSize());

			long dataTotal = 2;

			CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);

			pTSD->SetValue(0, pISimBox->GetCurrentTime(), "Time");
			pTSD->SetValue(1, m_PoreSize,    "PoreSize");

			m_pState->AddTimeSeriesData(pTSD);
		}
		else									// Pore has just closed
		{
			m_bPoreOpened = false;

			// Activate the evLamellaPore event to start watching for
			// the pore to reopen. Note that the evLamellaPoreClosed event
			// is inactivated inside its own Execute() function.

			ActivateEvent(m_pLamellaPore->GetId());
		}
	}
	else if(!m_pLamellaPore->IsActive())	// Pore has just opened
	{
		m_bPoreOpened = true;

		// If this is the first time the pore has opened create an
		// evLamellaPoreClosed event to watch for pore resealing,
		// otherwise activate the current event. The event is added
		// to the processes local event sequence, and also to the 
		// CAnalysisState global event sequence. The CAnalysisState
		// is used to pass it to the CSimBox via the RefreshEvents() 
		// function.

		// The data to set are:

		// Id					(set on creation of the event)
		// Start and end times	(set in xxProcess::AddEvent)
		// Aggregate name
		// Aggregate id
		// Pore minimum size
		// Polymer name list
		// Polymer type list
		// Head bead type list
		// Aggregate pointer

		// note that the valid, active, and initialise flags are true by default.

		if(!m_pLamellaPoreClosed)
		{
			m_pLamellaPoreClosed = new evLamellaPoreClosed();

			// Set xxEvent base class data

			AddEvent(m_pLamellaPoreClosed, pISimBox->GetCurrentTime(), GetEndTime());

			// Set event-specific data, issuing an error message and deleting
			// the event if validation fails.

			if(m_pLamellaPoreClosed->InternalValidateData(rSimState, m_pLamellaPore))
			{
				// Now add the event to the CAnalysisState's global event sequence, 
				// and copy the CSimState's events to the CSimBox for speed of access

				rSimState.GetAnalysisState().AddEvent(m_pLamellaPoreClosed);
				pISimBox->IISimEvent()->RefreshEvents();
			}
			else
			{
				 new CLogCreateEventFailed(pISimBox->GetCurrentTime(), m_pLamellaPoreClosed);

				delete m_pLamellaPoreClosed;
			}
		}
		else
		{
			ActivateEvent(m_pLamellaPoreClosed->GetId());
		}

		// Calculate pore properties here and store the data in a TSD object, 
		// passing it to the xxProcessState object for serialisation. We don't 
		// have to delete the CTimeSeriesData objects 
		// created here as the xxProcessState object will do this.

		m_PoreSize = static_cast<double>(m_pBilayer->GetPoreSize());

		long dataTotal = 2;

		CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);

		pTSD->SetValue(0, pISimBox->GetCurrentTime(), "Time");
		pTSD->SetValue(1, m_PoreSize,    "PoreSize");

		m_pState->AddTimeSeriesData(pTSD);
	}
}

// Store a pointer to the CAnalysis-derived object that the process
// will monitor for activity during the simulation. This is const
// because we don't allow data to be written back to the aggregate.
// If more than one aggregate is needed we need to add more functions.

void prBilayerRuptureSurfactant::SetAggregate(const CAnalysis* pAggregate)
{
	m_pBilayer = new IBilayerRupture<CBilayer>(reinterpret_cast<const CBilayer*>(pAggregate));
}

// Store a pointer to the xxEvent-derived object that the process
// will use to track changes to the aggregate. 

void prBilayerRuptureSurfactant::SetEvent(xxEvent* pEvent)
{
	m_pLamellaPore = dynamic_cast<evLamellaPore*>(pEvent);

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

bool prBilayerRuptureSurfactant::ValidateData(const CInputData &riData)
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

	// All data has been validated so create and store an xxProcessState object
	// to allow the xxProcess to write its data to a process state file.

    SetState(new xxProcessState(xxBase::GetPSPrefix() + GetProcessType() + ToString(GetId()) + "." + riData.GetRunId(), GetStartTime(), GetEndTime()));

	return true;
}

