/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// evVesicleBindsWall.cpp: implementation of the evVesicleBindsWall class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "evVesicleBindsWall.h"

#include "IEventVesicleBindsWall.h"
#include "LogVesicleBindsWall.h"
#include "ISimEvent.h"
#include "InputData.h"
#include "aaRegionToType.h"
#include "CellProfileSet.h"
#include "CellProfile.h"
#include "Builder.h"		// Needed for the global pi


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString evVesicleBindsWall::m_Type = "VesicleBindsWall";

const zString evVesicleBindsWall::GetType()
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
	xxEvent* Create() {return new evVesicleBindsWall();}

	const zString id = evVesicleBindsWall::GetType();

	const bool bRegistered = acfEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

evVesicleBindsWall::evVesicleBindsWall() : m_pIEvent(NULL),
										   m_AggregateName(""), m_AggregateId(0),
										   m_WallXN(0),
										   m_WallYN(0),
										   m_WallZN(0),
										   m_WallThickness(0),
										   m_MaxCMSeparation(0.0),
										   m_MaxEdgeSeparation(0.0),
										   m_CMDistance(0.0),
										   m_EdgeDistance(0.0)
													   
{

}

// The same aggregate object is passed to the copied event.
//

evVesicleBindsWall::evVesicleBindsWall(const evVesicleBindsWall& oldEvent) : xxEvent(oldEvent),
										m_pIEvent(oldEvent.m_pIEvent),
										m_AggregateName(oldEvent.m_AggregateName),
										m_AggregateId(oldEvent.m_AggregateId),
										m_WallXN(oldEvent.m_WallXN),
										m_WallYN(oldEvent.m_WallYN),
										m_WallZN(oldEvent.m_WallZN),
										m_WallThickness(oldEvent.m_WallThickness),
										m_MaxCMSeparation(oldEvent.m_MaxCMSeparation),
										m_MaxEdgeSeparation(oldEvent.m_MaxEdgeSeparation),
										m_CMDistance(oldEvent.m_CMDistance),
										m_EdgeDistance(oldEvent.m_EdgeDistance)
{
}

evVesicleBindsWall::~evVesicleBindsWall()
{
}



// Member functions to write/read the data specific to the command.

zOutStream& evVesicleBindsWall::put(zOutStream& os) const
{
	// Write base class data first

	xxEvent::put(os);
	
	os << "    Aggregate			" << m_AggregateName << "  " << m_AggregateId << zEndl;
	os << "    WallNormal			" << m_WallXN << "  " << m_WallYN << "  " << m_WallZN << zEndl;
	os << "    WallThickness		" << m_WallThickness		<< zEndl;
	os << "    MaxCMSeparation		" << m_MaxCMSeparation		<< zEndl;
	os << "    MaxEdgeSeparation	" << m_MaxEdgeSeparation	<< zEndl;

	return os;
}

zInStream& evVesicleBindsWall::get(zInStream& is)
{
	// Read base class data first

	xxEvent::get(is);
	
	zString token;
	zString aggregate;
	long id;
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
		is >> aggregate >> id;
		if(!is.good() || aggregate.empty() || id < 1)
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
	m_AggregateId		= id;
	m_WallXN			= xn;
	m_WallYN			= yn;
	m_WallZN			= zn;
	m_WallThickness		= thickness;
	m_MaxCMSeparation	= maxCMSeparation;
	m_MaxEdgeSeparation	= maxEdgeSeparation;


	return is;
}


// Non-static function to return the type of the event

const zString evVesicleBindsWall::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

xxEvent* evVesicleBindsWall::GetEvent() const
{
	return new evVesicleBindsWall(*this);
}

// Convert the CAnalysis pointer to its base class IEventVesicleBindsWall
// interface pointer so that we can access the data needed for the event.

bool evVesicleBindsWall::SetAggregate(CAnalysis* const pAggregate)
{
	m_pIEvent = dynamic_cast<IEventVesicleBindsWall*>(pAggregate);

	return m_pIEvent->VesicleBindsWallSupported();
}

// **********************************************************************
// Function to validate the data supplied to the event against the 
// aggregate that is the source of its information about the simulation state.
// We check both the aggregate name and its id in case more than one
// aggregate of the specified type has been created. This way we can assign 
// the same aggregate to many events, by repeating the name/id, or assign 
// different aggregates of the same type, e.g., bilayer, using their ids
// to distinguish them.
//
// If more than one aggregate is needed for the event we still designate one
// as the major aggregate and store the others locally to the xxEvent-derived
// class. 
//
// This function cannot be const because we store data for later use.
//
// Event-specific validation follows.
//

bool evVesicleBindsWall::ValidateData(const CInputData &riData)
{
	// Does the specified aggregate exist? 
	// Search by the unique id that identifies aggregates of all types.
	// If the aggregate exists, check that it supports the IEventXXX interface

	cAggregateIterator cIterAgg = find_if(riData.GetAggregates().begin(), 
										  riData.GetAggregates().end(), aaGetAggregateId(m_AggregateId));

	if(cIterAgg == riData.GetAggregates().end() ||
	  (*cIterAgg)->GetType() != m_AggregateName || !SetAggregate(*cIterAgg))
		return false;


	return true;
}

// **********************************************************************
// Function to validate an internally-created evVesicleBindsWall event. 
// It is equivalent to ValidateData above that is used for events read 
// from the control data file.
//
// Events can only be created by xxProcesses within their UpdateState function.
// This allows the new event to be validated using data from the CSimState object.
// xxEvent base class data should be set using the xxProcess::AddEvent function.

bool evVesicleBindsWall::InternalValidateData(const CSimState& rSimState, xxEvent* const pEvent)
{
	return true;
}

// **********************************************************************
// Implementation of the Execute command sent by the SimBox to each xxEvent.
//
// Events have an active period specified by the user during which they monitor
// the SimBox for their particular conditions and take action if they are triggered.
// We return a boolean so that the SimBox can see if the event is active or not
// as this may be useful for considering several events. Note that the active 
// period begins at the start of the m_Start time-step and stops at the beginning 
// of the time-step labelled by its m_End property.
// Events can issue new commands and messages. Commands are executed after all events
// have been executed. This allows all events to monitor the state of the simulation
// before any new commands change it.
//
// This event uses data provided by the CVesicle class to monitor the
// appearance of a pore in the vesicle bilayer.

bool evVesicleBindsWall::Execute(long simTime, ISimEvent* const pISimEvent)
{
	if(IsActive() && simTime >= GetStartTime() && simTime < GetEndTime())
	{

		// **********************************************************************
		// Perform initialisation steps prior to calculations.

		if(InitialiseEvent())
		{
			UnInitialiseEvent();	// Call base class member to set flag false
		}
		else
		{
		// **********************************************************************
		// Check if vesicle is bound to wall

			if(m_WallXN == 1)
			{
				m_CMDistance	= m_pIEvent->GetCMXPos() - m_WallThickness;
				m_EdgeDistance	= m_CMDistance - m_pIEvent->GetRadius();
			}
			else if(m_WallYN == 1)
			{
				m_CMDistance	= m_pIEvent->GetCMYPos() - m_WallThickness;
				m_EdgeDistance	= m_CMDistance - m_pIEvent->GetRadius();
			}
			else
			{
				m_CMDistance	= m_pIEvent->GetCMZPos() - m_WallThickness;
				m_EdgeDistance	= m_CMDistance - m_pIEvent->GetRadius();
			}
		}

		// Output a message if the vesicle has adhered to the wall, save a 
		// snapshot of the current simulation state, and call the base class 
		// Inactivate() function so that the message is not sent again unless 
		// the pore reseals and then reappears. Notice that the CLogVesicleBindsWall
		// message can also be issued by other events.

		if(m_CMDistance < m_MaxCMSeparation)
		{
			pISimEvent->SaveCurrentState();

			 new CLogVesicleBindsWall(simTime, m_AggregateId, m_CMDistance, m_EdgeDistance);

			InActivateEvent();		
		}

		return true;
	}
	else
		return false;
}

