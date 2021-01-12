/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// evLamellaMonolayerPhaseSeparation.cpp: implementation of the evLamellaMonolayerPhaseSeparation class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "evLamellaMonolayerPhaseSeparation.h"

#include "IEventLamellaMonolayerPhaseSeparation.h"
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

const zString evLamellaMonolayerPhaseSeparation::m_Type = "LamellaMonolayerPhaseSeparation";

const zString evLamellaMonolayerPhaseSeparation::GetType()
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
	xxEvent* Create() {return new evLamellaMonolayerPhaseSeparation();}

	const zString id = evLamellaMonolayerPhaseSeparation::GetType();

	const bool bRegistered = acfEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

evLamellaMonolayerPhaseSeparation::evLamellaMonolayerPhaseSeparation() : m_pIEvent(NULL), 
									m_AggregateName(""), m_AggregateId(0),
									m_bOuterMonolayer(true), m_MaxMissing(0.0)
{
	m_DomainPolymers.clear();
	m_DomainPolymerTypes.clear();
	m_DomainHeadBeadTypes.clear();
	m_SeaPolymers.clear();
	m_SeaPolymerTypes.clear();
	m_SeaHeadBeadTypes.clear();
}

// The same aggregate object is passed to the copied event.
//

evLamellaMonolayerPhaseSeparation::evLamellaMonolayerPhaseSeparation(const evLamellaMonolayerPhaseSeparation& oldEvent) : xxEvent(oldEvent),
															  m_pIEvent(oldEvent.m_pIEvent),
															  m_AggregateName(oldEvent.m_AggregateName),
															  m_AggregateId(oldEvent.m_AggregateId),
															  m_bOuterMonolayer(oldEvent.m_bOuterMonolayer),
															  m_MaxMissing(oldEvent.m_MaxMissing),
															  m_DomainPolymers(oldEvent.m_DomainPolymers),
															  m_SeaPolymers(oldEvent.m_SeaPolymers),
															  m_DomainPolymerTypes(oldEvent.m_DomainPolymerTypes),
															  m_SeaPolymerTypes(oldEvent.m_SeaPolymerTypes),
															  m_DomainHeadBeadTypes(oldEvent.m_DomainHeadBeadTypes),
															  m_SeaHeadBeadTypes(oldEvent.m_SeaHeadBeadTypes)
{
}

evLamellaMonolayerPhaseSeparation::~evLamellaMonolayerPhaseSeparation()
{
}



// Member functions to write/read the data specific to the command.
// The name of bead type to be removed from display is the only data needed.

zOutStream& evLamellaMonolayerPhaseSeparation::put(zOutStream& os) const
{
	// Write base class data first

	xxEvent::put(os);
	
	os << "    Aggregate		" << m_AggregateName << "  " << m_AggregateId << zEndl;
	os << "    DomainPolymers	";

	for(cStringSequenceIterator iterName=m_DomainPolymers.begin(); iterName!=m_DomainPolymers.end(); iterName++)
	{
		os << " " << (*iterName);
	}

	os << zEndl;

	os << "    SeaPolymers		";

	for(cStringSequenceIterator seaIterName=m_SeaPolymers.begin(); seaIterName!=m_SeaPolymers.end(); seaIterName++)
	{
		os << " " << (*seaIterName);
	}

	os << zEndl;

	os << "    OuterMonolayer	" << m_bOuterMonolayer	<< zEndl;
	os << "    MaxMissing		" << m_MaxMissing		<< zEndl;

	return os;
}

zInStream& evLamellaMonolayerPhaseSeparation::get(zInStream& is)
{
	// Read base class data first

	xxEvent::get(is);
	
	zString token;
	zString aggregate;
	long id;
	zString polymer;
	zString sName;
	bool bOuter;
	double maxMissing;	// Percentage not an integer

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
	if(!is.good() || token != "DomainPolymers")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;

		if(!is.good() || sName == "SeaPolymers" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "SeaPolymers")
			{
				if(is.good() && !sName.empty() && (find(m_DomainPolymers.begin(), 
												        m_DomainPolymers.end(), sName) == m_DomainPolymers.end()))
					m_DomainPolymers.push_back(sName);
				else
				{
					SetDataValid(false);
					return is;
				}

				is >> sName;
			}
		}
	}

	if(!is.good() || sName != "SeaPolymers")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;

		if(!is.good() || sName == "OuterMonolayer" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "OuterMonolayer")
			{
				if(is.good() && !sName.empty() && (find(m_SeaPolymers.begin(), 
												        m_SeaPolymers.end(), sName) == m_SeaPolymers.end()))
					m_SeaPolymers.push_back(sName);
				else
				{
					SetDataValid(false);
					return is;
				}

				is >> sName;
			}
		}
	}

	if(!is.good() || sName != "OuterMonolayer")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> bOuter;
		if(!is.good())
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "MaxMissing")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> maxMissing;
		if(!is.good() || maxMissing < 0.0 || maxMissing > 1.0)
		{
			SetDataValid(false);
			return is;
		}
	}


	// Data has been read successfully so store it in the member variables

	SetDataValid(true);

	m_AggregateName		= aggregate;
	m_AggregateId		= id;
	m_bOuterMonolayer	= bOuter;
	m_MaxMissing		= maxMissing;

	return is;
}


// Non-static function to return the type of the event

const zString evLamellaMonolayerPhaseSeparation::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

xxEvent* evLamellaMonolayerPhaseSeparation::GetEvent() const
{
	return new evLamellaMonolayerPhaseSeparation(*this);
}

// Set a pointer to the CAnalysis-derived object that the event
// will monitor for activity during the simulation. This should be const
// because the event should not write data back to the aggregate.

bool evLamellaMonolayerPhaseSeparation::SetAggregate(CAnalysis* const pAggregate)
{
	m_pIEvent = dynamic_cast<IEventLamellaMonolayerPhaseSeparation*>(pAggregate);

	return m_pIEvent->LamellaMonolayerPhaseSeparationSupported();
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

bool evLamellaMonolayerPhaseSeparation::ValidateData(const CInputData &riData)
{
	// Does the specified aggregate exist? 
	// Search by the unique id that identifies aggregates of all types.
	// If the aggregate exists, check that it supports the IEventXXX interface

	cAggregateIterator cIterAgg = find_if(riData.GetAggregates().begin(), 
										  riData.GetAggregates().end(), aaGetAggregateId(m_AggregateId));

	if(cIterAgg == riData.GetAggregates().end() ||
	  (*cIterAgg)->GetType() != m_AggregateName || !SetAggregate(*cIterAgg))
		return false;

	// Check that the polymer names specified for the domains and the sea actually
	// occur in the map and store their types for later use. Note that the same polymer
	// types are not allowed in the domains and the sea, nor may they share the same
	// head bead type. We simply check for intersection of the two vectors of head bead types.

	for(cStringSequenceIterator polymer=m_DomainPolymers.begin(); polymer!=m_DomainPolymers.end(); polymer++)
	{
		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end())
		{
			long polymerType = (*riData.GetPolymerNamesMap().find(*polymer)).second;
			const CPolymer* const pPolymer = riData.GetPolymerTypes().at(polymerType);

			m_DomainPolymerTypes.push_back(polymerType);
			m_DomainHeadBeadTypes.push_back(pPolymer->GetHead()->GetType());
		}
		else
			return false;
	}

	for(cStringSequenceIterator seaPolymer=m_SeaPolymers.begin(); seaPolymer!=m_SeaPolymers.end(); seaPolymer++)
	{
		if(riData.GetPolymerNamesMap().find(*seaPolymer) != riData.GetPolymerNamesMap().end())
		{
			long polymerType = (*riData.GetPolymerNamesMap().find(*seaPolymer)).second;
			const CPolymer* const pPolymer = riData.GetPolymerTypes().at(polymerType);

			m_SeaPolymerTypes.push_back(polymerType);
			m_SeaHeadBeadTypes.push_back(pPolymer->GetHead()->GetType());
		}
		else
			return false;
	}

	// If the sea and domain polymers share a head bead return false. Because we do not
	// know in advance how many bead types occur in the vectors, we make the intersection
	// vector large enough to hold all of the elements. Note that the vectors must
	// be sorted before doing the intersection check.

	sort(m_SeaHeadBeadTypes.begin(), m_SeaHeadBeadTypes.end());
	sort(m_DomainHeadBeadTypes.begin(), m_DomainHeadBeadTypes.end());

	zLongVector headIntersect;

	if(m_SeaHeadBeadTypes.size() > m_DomainHeadBeadTypes.size())
		headIntersect.resize(m_SeaHeadBeadTypes.size(), -1);
	else
		headIntersect.resize(m_DomainHeadBeadTypes.size(), -1);

	zLongVectorIterator iterHead = headIntersect.begin();

	iterHead = set_intersection(m_SeaHeadBeadTypes.begin(), m_SeaHeadBeadTypes.end(),
								m_DomainHeadBeadTypes.begin(), m_DomainHeadBeadTypes.end(),
								headIntersect.begin());

	// The set_intersection algorithm returns an iterator to the end of the 
	// intersection sequence: if this is not equal to the beginning there 
	// are common elements and we return false.

	if(iterHead != headIntersect.begin())
		return false;

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
// This event uses data provided by the CBilayer class to check if a pore has
// opened up in the bilayer. If a pore opens, a message is sent to the log state
// and a command issued to create an evLamellaPoreClosed event and the current 
// event deactivated. This allows both pore opening and closing to be monitored 
// and used to  calculate pore lifetimes.


bool evLamellaMonolayerPhaseSeparation::Execute(long simTime, ISimEvent* const pISimEvent)
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

		}

		// note that no log message has been defined for this event yet

		if(m_pIEvent->HasPhaseSeparated())
		{
			pISimEvent->SaveCurrentState();

			InActivateEvent();		
		}

		return true;
	}
	else
		return false;
}


