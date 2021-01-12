/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prVesicleShear.cpp: implementation of the prVesicleShear class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "prVesicleShear.h"
#include "IVesicleShear.h"
#include "Vesicle.h"
#include "LogCreateEventFailed.h"
#include "SimState.h"
#include "ISimBox.h"
#include "TimeSeriesData.h"
#include "InputData.h"
#include "aaRegionToType.h"

// Events monitored by the process

#include "evVesicleBindsWall.h"
#include "evVesicleUnbindsWall.h"


// STL using declarations


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.

const zString prVesicleShear::m_Type = "VesicleShear";

const zString prVesicleShear::GetType()
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
	xxProcess* Create() {return new prVesicleShear();}

	const zString id = prVesicleShear::GetType();

	const bool bRegistered = acfProcessFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

prVesicleShear::prVesicleShear() : xxProcess(true),
								m_AggregateName(""),
								m_AggregateId(0),
								m_EventName(""),
								m_EventId(0),
								m_WallXN(0),
								m_WallYN(0),
								m_WallZN(0),
								m_WallThickness(0),
								m_MaxCMSeparation(0.0),
								m_MaxEdgeSeparation(0.0),
								m_pVesicle(NULL),
								m_pVesicleBinds(NULL),
								m_pVesicleUnbinds(NULL),
								m_bVesicleWallMaxDistance(false),
								m_bVesicleBound(false),
								m_FirstBoundTime(0),
								m_VesicleCMWallDist(0.0),
								m_VesicleEdgeWallDist(0.0)

{

}

prVesicleShear::~prVesicleShear()
{
	if(m_pVesicle)
	{
		delete m_pVesicle;
		m_pVesicle = NULL;
	}
}

// Member functions to write/read the data specific to the process.

zOutStream& prVesicleShear::put(zOutStream& os) const
{
	// Write base class data first

	xxProcess::put(os);
	
	os << "    Aggregate			" << m_AggregateName	 << "  " << m_AggregateId << zEndl;
	os << "    Event    			" << m_EventName		 << "  " << m_EventId     << zEndl;
	os << "    WallNormal			" << m_WallXN			 << "  " << m_WallYN	  << "  " << m_WallZN << zEndl;
	os << "    WallThickness		" << m_WallThickness		<< zEndl;
	os << "    MaxCMSeparation		" << m_MaxCMSeparation		<< zEndl;
	os << "    MaxEdgeSeparation	" << m_MaxEdgeSeparation	<< zEndl;

	return os;
}

zInStream& prVesicleShear::get(zInStream& is)
{
	// Read base class data first

	xxProcess::get(is);

	zString token;
	zString aggregate, event;
	long aggId, eventId;
	long xn, yn, zn, thickness;
	double maxCMSeparation, maxEdgeSeparation;


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
	if(!is.good() || token != "WallNormal")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> xn >> yn >> zn;
		if(!is.good() || (xn == 0 && yn == 0 && zn == 0) )
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "WallThickness")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> thickness;
		if(!is.good() || thickness < 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "MaxCMSeparation")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> maxCMSeparation;
		if(!is.good() || maxCMSeparation < 0.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "MaxEdgeSeparation")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> maxEdgeSeparation;
		if(!is.good() || maxEdgeSeparation < 0.0 || maxEdgeSeparation > maxCMSeparation)
		{
			SetDataValid(false);
			return is;
		}
	}


	// Data has been read successfully so store it in the member variables

	SetDataValid(true);

	m_AggregateName		= aggregate;
	m_AggregateId		= aggId;
	m_EventName			= event;
	m_EventId			= eventId;
	m_WallXN			= xn;
	m_WallYN			= yn;
	m_WallZN			= zn;
	m_WallThickness		= thickness;
	m_MaxCMSeparation	= maxCMSeparation;
	m_MaxEdgeSeparation	= maxEdgeSeparation;

	return is;
}


// Non-static function to return the type of the process

const zString prVesicleShear::GetProcessType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current process.

xxProcess* prVesicleShear::GetProcess() const
{
	return new prVesicleShear(*this);
}

// Function to update the state of the process using data from the associated
// aggregate (CVesicle) and events (evVesiclePore,...).
//
// This process monitors a vesicle in shear flow in the presence of a wall.
// It checks for the vesicle binding to the wall, unbinding and for potential
// fusion via the pbcs (in which the vesicle membrane binds to itself).
//
//	1	Vesicle CM separation from wall    <  m_MaxCMSeparation
//	2	Outer leaflet separation from wall <  m_MaxEdgeSeparation
//
// Each stage is signalled by issuing a CLogInfoMessage, and the appearance of
// pores in the two vesicle membranes is monitored by the evVesiclePore events.
// External events are not defined for the other stages of fusion because they
// involve more than one aggregate.

void prVesicleShear::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	// Check if the vesicle has bound to the wall and, if so, set the m_bVesicleBound
	// flag, calculate the properties of the bound state and create a new event
	// to watch for unbinding, evVesicleUnbindsWall. The new event must be manually
	// added to the CAnalysisState's event sequence because it is only here that we
	// have access to the CSimState.

	if(m_bVesicleBound)
	{
		if(m_pVesicleUnbinds->IsActive())	// Vesicle is still bound
		{
			// Calculate bound state properties and pass them to the
			// xxProcessState object for serialisation via a CTimeSeriesData object.

			if(m_WallXN == 1)
			{
				m_VesicleCMWallDist		= m_pVesicle->GetCMXPos() - m_WallThickness;
				m_VesicleEdgeWallDist	= m_VesicleCMWallDist - m_pVesicle->GetRadius();
			}
			else if(m_WallYN == 1)
			{
				m_VesicleCMWallDist		= m_pVesicle->GetCMYPos() - m_WallThickness;
				m_VesicleEdgeWallDist	= m_VesicleCMWallDist - m_pVesicle->GetRadius();
			}
			else
			{
				m_VesicleCMWallDist		= m_pVesicle->GetCMZPos() - m_WallThickness;
				m_VesicleEdgeWallDist	= m_VesicleCMWallDist - m_pVesicle->GetRadius();
			}

			double vcm[3];
			double vcmDistance;

			vcm[0]	= m_pVesicle->GetCMXPos();
			vcm[1]	= m_pVesicle->GetCMYPos();
			vcm[2]	= m_pVesicle->GetCMZPos();

			vcmDistance = sqrt(vcm[0]*vcm[0] + vcm[1]*vcm[1] + vcm[2]*vcm[2]);

			long dataTotal = 4;

			CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);

			pTSD->SetValue(0, pISimBox->GetCurrentTime(),	"Time");
			pTSD->SetValue(1, m_VesicleCMWallDist,			"CMDistance");
			pTSD->SetValue(2, m_VesicleEdgeWallDist,		"EdgeDistance");
			pTSD->SetValue(3, vcmDistance,					"CMPosition");

			m_pState->AddTimeSeriesData(pTSD);
		}
		else	// Vesicle has just unbound from wall
		{
			m_bVesicleBound = false;

			// Activate the evVesicleBindsWall event to start watching
			// for the vesicle to reattach to the wall. Note that the
			// evVesicleUnbindsWall event is inactivated inside its own
			// Execute() function.

			ActivateEvent(m_pVesicleBinds->GetId());
		}
	}
	else if(!m_pVesicleBinds->IsActive())		// Vesicle has just bound
	{
		m_bVesicleBound = true;

		// If this is the first time the vesicle has bound to the wall,
		// store the current time and create an event to watch for it unbinding, 
		// this is the evVesicleUnbindsWall event. If not the first time,
		// activate the current event. The event is added
		// to the processes local event sequence, and also to the 
		// CAnalysisState global event sequence. The CAnalysisState
		// is used to pass it to the CSimBox via the RefreshEvents() 
		// function.

		// The data to set are:

		// Id					(set on creation of the event)
		// Start and end times	(set in xxProcess::AddEvent)
		// Aggregate name
		// Aggregate id

		// note that the valid, active, and initialise flags are true by default.

		if(!m_pVesicleUnbinds)
		{
			m_FirstBoundTime = pISimBox->GetCurrentTime();

			m_pVesicleUnbinds = new evVesicleUnbindsWall();

			// Set xxEvent base class data

			AddEvent(m_pVesicleUnbinds, pISimBox->GetCurrentTime(), GetEndTime());

			// Set event-specific data, issuing an error message and deleting
			// the event if validation fails.

			if(m_pVesicleUnbinds->InternalValidateData(rSimState, m_pVesicleBinds))
			{
				// Now add the event to the CAnalysisState's global event sequence, 
				// and copy the CSimState's events to the CSimBox for speed of access

				rSimState.GetAnalysisState().AddEvent(m_pVesicleUnbinds);
				pISimBox->IISimEvent()->RefreshEvents();
			}
			else
			{
				new CLogCreateEventFailed(pISimBox->GetCurrentTime(), m_pVesicleUnbinds);

				delete m_pVesicleUnbinds;
			}
		}
		else
		{
			ActivateEvent(m_pVesicleUnbinds->GetId());
		}

			// Calculate bound state properties and pass them to the
			// xxProcessState object for serialisation via a CTimeSeriesData object.

			if(m_WallXN == 1)
			{
				m_VesicleCMWallDist		= m_pVesicle->GetCMXPos() - m_WallThickness;
				m_VesicleEdgeWallDist	= m_VesicleCMWallDist - m_pVesicle->GetRadius();
			}
			else if(m_WallYN == 1)
			{
				m_VesicleCMWallDist		= m_pVesicle->GetCMYPos() - m_WallThickness;
				m_VesicleEdgeWallDist	= m_VesicleCMWallDist - m_pVesicle->GetRadius();
			}
			else
			{
				m_VesicleCMWallDist		= m_pVesicle->GetCMZPos() - m_WallThickness;
				m_VesicleEdgeWallDist	= m_VesicleCMWallDist - m_pVesicle->GetRadius();
			}

			double vcm[3];
			double vcmDistance;

			vcm[0]	= m_pVesicle->GetCMXPos();
			vcm[1]	= m_pVesicle->GetCMYPos();
			vcm[2]	= m_pVesicle->GetCMZPos();

			vcmDistance = sqrt(vcm[0]*vcm[0] + vcm[1]*vcm[1] + vcm[2]*vcm[2]);

			long dataTotal = 4;

			CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);

			pTSD->SetValue(0, pISimBox->GetCurrentTime(),	"Time");
			pTSD->SetValue(1, m_VesicleCMWallDist,			"CMDistance");
			pTSD->SetValue(2, m_VesicleEdgeWallDist,		"EdgeDistance");
			pTSD->SetValue(3, vcmDistance,					"CMPosition");

			m_pState->AddTimeSeriesData(pTSD);
	}
}

// Store pointers to the CAnalysis-derived objects that the process
// will monitor for activity during the simulation. These are const
// because we don't allow data to be written back to the aggregate.
// Because all the aggregates are stored as base class, CAnalysis, pointers,
// we have to use reinterpret_cast to get back to the CVesicle object.
// This means that if the wrong CAnalysis-derived class is passed in 
// the code should fail gracefully.

void prVesicleShear::SetAggregate(const CAnalysis* pAggregate)
{
	m_pVesicle = new IVesicleShear<CVesicle>(reinterpret_cast<const CVesicle*>(pAggregate));
}

// This process watches for the vesicle binding to th wall, so it needs an
// evVesicleBindsWall event. This assumes that the vesicle is initially 
// unbound.

void prVesicleShear::SetEvent(xxEvent* pEvent)
{
	m_pVesicleBinds = dynamic_cast<evVesicleBindsWall*>(pEvent);

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

bool prVesicleShear::ValidateData(const CInputData &riData)
{
	// Do the specified aggregates exist and are they CVesicle? If so, store
	// pointers to them

	cAggregateIterator cIterAgg1 = find_if(riData.GetAggregates().begin(), 
										   riData.GetAggregates().end(), aaGetAggregateId(m_AggregateId));

	if(cIterAgg1 != riData.GetAggregates().end() && (*cIterAgg1)->GetType() == m_AggregateName)
		SetAggregate(*cIterAgg1);
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
