/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// evLamellaPoreClosed.cpp: implementation of the evLamellaPoreClosed class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "evLamellaPoreClosed.h"

#include "IEventLamellaPoreClosed.h"
#include "LogLamellaPoreClosed.h"
#include "ISimEvent.h"
#include "InputData.h"
#include "SimState.h"
#include "aaRegionToType.h"
#include "CellProfileSet.h"
#include "CellProfile.h"
#include "Builder.h"		// Needed for the global pi
#include "evLamellaPore.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString evLamellaPoreClosed::m_Type = "LamellaPoreClosed";

const zString evLamellaPoreClosed::GetType()
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
	xxEvent* Create() {return new evLamellaPoreClosed();}

	const zString id = evLamellaPoreClosed::GetType();

	const bool bRegistered = acfEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

evLamellaPoreClosed::evLamellaPoreClosed() : m_pIEvent(NULL),
											 m_AggregateName(""), m_AggregateId(0),
											 m_MinSize(0)
{

	m_Polymers.clear();
	m_PolymerTypes.clear();
	m_HeadBeadTypes.clear();

}


// The same aggregate object is passed to the copied event.
//

evLamellaPoreClosed::evLamellaPoreClosed(const evLamellaPoreClosed& oldEvent) : xxEvent(oldEvent),
										  m_pIEvent(oldEvent.m_pIEvent),
										  m_AggregateName(oldEvent.m_AggregateName),
										  m_AggregateId(oldEvent.m_AggregateId),
										  m_MinSize(oldEvent.m_MinSize),
										  m_Polymers(oldEvent.m_Polymers),
										  m_PolymerTypes(oldEvent.m_PolymerTypes),
										  m_HeadBeadTypes(oldEvent.m_HeadBeadTypes)
{
}

evLamellaPoreClosed::~evLamellaPoreClosed()
{

}



// Member functions to write/read the data specific to the command.
// The name of bead type to be removed from display is the only data needed.

zOutStream& evLamellaPoreClosed::put(zOutStream& os) const
{
	// Write base class data first

	xxEvent::put(os);
	
	os << "    Aggregate	" << m_AggregateName << "  " << m_AggregateId << zEndl;
	os << "    Polymers		";

	for(cStringSequenceIterator iterName=m_Polymers.begin(); iterName!=m_Polymers.end(); iterName++)
	{
		os << " " << (*iterName);
	}

	os << zEndl;
	os << "    MinSize      " << m_MinSize	<< zEndl;

	return os;
}

zInStream& evLamellaPoreClosed::get(zInStream& is)
{
	// Read base class data first

	xxEvent::get(is);
	
	zString token;
	zString aggregate;
	long id;
	zString polymer;
	zString sName;
	double minsize;


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
	if(!is.good() || token != "Polymers")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;

		if(!is.good() || sName == "MinSize" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "MinSize")
			{
				if(is.good() && !sName.empty())
					m_Polymers.push_back(sName);
				else
				{
					SetDataValid(false);
					return is;
				}

				is >> sName;
			}
		}
	}

	if(!is.good() || sName != "MinSize")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> minsize;
		if(!is.good() || minsize < 0)
		{
			SetDataValid(false);
			return is;
		}
	}


	// Data has been read successfully so store it in the member variables

	SetDataValid(true);

	m_AggregateName	= aggregate;
	m_AggregateId	= id;
	m_MinSize		= minsize;


	return is;
}


// Non-static function to return the type of the event

const zString evLamellaPoreClosed::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

xxEvent* evLamellaPoreClosed::GetEvent() const
{
	return new evLamellaPoreClosed(*this);
}

// Set a pointer to the CAnalysis-derived object that the event
// will monitor for activity during the simulation. This cannot be const
// because the event may write data back to the aggregate.

bool evLamellaPoreClosed::SetAggregate(CAnalysis* const pAggregate)
{
	m_pIEvent = dynamic_cast<IEventLamellaPoreClosed*>(pAggregate);

	return m_pIEvent->LamellaPoreClosedSupported();
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

bool evLamellaPoreClosed::ValidateData(const CInputData &riData)
{
	// Does the specified aggregate exist? If so store a pointer to it

	cAggregateIterator cIterAgg = find_if(riData.GetAggregates().begin(), 
										  riData.GetAggregates().end(), aaGetAggregateId(m_AggregateId));

	if(cIterAgg != riData.GetAggregates().end() && (*cIterAgg)->GetType() == m_AggregateName)
		SetAggregate(*cIterAgg);
	else
		return false;

	// Check that the polymer names specified actually
	// occur in the map and store their types for later use.

	for(cStringSequenceIterator polymer=m_Polymers.begin(); polymer!=m_Polymers.end(); polymer++)
	{
		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end())
		{
			long polymerType = (*riData.GetPolymerNamesMap().find(*polymer)).second;
			const CPolymer* const pPolymer = riData.GetPolymerTypes().at(polymerType);

			m_PolymerTypes.push_back(polymerType);
			m_HeadBeadTypes.push_back(pPolymer->GetHead()->GetType());

		}
		else
			return false;
	}

	return true;
}

// **********************************************************************
// Function to validate an internally-created evLamellaPoreClosed event. 
// It is equivalent to ValidateData above that is used for events read 
// from the control data file.
//
// Events can only be created by xxProcesses within their UpdateState function.
// This allows the new event to be validated using data from the CSimState object.
// xxEvent base class data should be set using the xxProcess::AddEvent function.

bool evLamellaPoreClosed::InternalValidateData(const CSimState& rSimState, xxEvent* const pEvent)
{
	const CAnalysisState& raState = rSimState.cGetAnalysisState();

	const evLamellaPore* const pOldEvent = dynamic_cast<const evLamellaPore*>(pEvent);

	m_AggregateName = pOldEvent->GetAggregateName();
	m_AggregateId   = pOldEvent->GetAggregateId();
	m_MinSize		= pOldEvent->GetMinSize();
	m_Polymers		= pOldEvent->GetPolymerNames();

	// Does the specified aggregate exist? If so store a pointer to it

	cAggregateIterator cIterAgg = find_if(raState.GetAggregates().begin(), 
										  raState.GetAggregates().end(), aaGetAggregateId(m_AggregateId));

	if(cIterAgg != raState.GetAggregates().end() && (*cIterAgg)->GetType() == m_AggregateName)
		SetAggregate(*cIterAgg);
	else
		return false;

	// Check that the polymer names specified occur in the CAnalysisState and store 
	// their types for later use

	for(cStringSequenceIterator polymer=m_Polymers.begin(); polymer!=m_Polymers.end(); polymer++)
	{
		if(raState.GetPolymerTypeFromName(*polymer) != -1)
		{
			long polymerType = raState.GetPolymerTypeFromName(*polymer);
			long headType    = raState.GetPolymerHeadType(polymerType);

			m_PolymerTypes.push_back(polymerType);
			m_HeadBeadTypes.push_back(headType);
		}
		else
			return false;
	}
	
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

bool evLamellaPoreClosed::Execute(long simTime, ISimEvent* const pISimEvent)
{
	if(IsActive() && simTime >= GetStartTime() && simTime < GetEndTime())
	{
		// **********************************************************************
		// Perform initialisation steps prior to calculations

		// Set the m_bPoreOpen flag to agree with the bilayer state, so that
		// the event is triggered only if the pore changes from open to closed.

		if(InitialiseEvent())
		{
			UnInitialiseEvent();		// Call base class member to set flag false
		}
		else
		{
		// **********************************************************************

		}

		// Output a message if the pore has closed and inactivate the event 
		// so that the message is not sent repeatedly

		if(m_pIEvent->GetPoreSize() <= m_MinSize)
		{
			pISimEvent->SaveCurrentState();

			 new CLogLamellaPoreClosed(simTime, m_AggregateName, m_AggregateId);

			InActivateEvent();		
		}

		return true;
	}
	else
		return false;

}
