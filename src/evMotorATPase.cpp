/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// evMotorATPase.cpp: implementation of the evMotorATPase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "evMotorATPase.h"
#include "ISimEvent.h"
#include "InputData.h"
#include "aaRegionToType.h"
#include "Motor.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString evMotorATPase::m_Type = "MotorATPase";

const zString evMotorATPase::GetType()
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
	xxEvent* Create() {return new evMotorATPase();}

	const zString id = evMotorATPase::GetType();

	const bool bRegistered = acfEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

evMotorATPase::evMotorATPase() : m_AggregateName(""),
								 m_AggregateId(0),
								 m_Motor(""),
								 m_Fuel(""),
								 m_Filament(""),
								 m_FuelPerStroke(0),
								 m_FuelBindingTime(0),
								 m_FilamentBindingTime(0),
								 m_FuelRange(0.0),
								 m_FilamentRange(0.0),
								 m_BoundFuelRange(0.0),
								 m_BoundFilamentRange(0.0),
								 m_BoundFuelSpringConstant(0.0),
								 m_BoundFilamentSpringConstant(0.0),
								 m_DeltaHead(0.0),
								 m_MotorType(-1),
								 m_FuelType(-1),
								 m_FilamentType(-1),
								 m_pMotor(NULL),
								 m_pMotorHead(NULL),
								 m_pCNTCells(NULL),
								 m_MotorTotal(0)

{
	m_vFuelBindingTimes.clear();
}

evMotorATPase::evMotorATPase(const evMotorATPase& oldEvent) : xxEvent(oldEvent),
															  m_AggregateName(oldEvent.m_AggregateName),
															  m_AggregateId(oldEvent.m_AggregateId),
															  m_Motor(oldEvent.m_Motor),
															  m_Fuel(oldEvent.m_Fuel),
															  m_Filament(oldEvent.m_Filament),
															  m_FuelPerStroke(oldEvent.m_FuelPerStroke),
															  m_FuelBindingTime(oldEvent.m_FuelBindingTime),
															  m_FilamentBindingTime(oldEvent.m_FilamentBindingTime),
															  m_FuelRange(oldEvent.m_FuelRange),
															  m_FilamentRange(oldEvent.m_FilamentRange),
															  m_BoundFuelRange(oldEvent.m_BoundFuelRange),
															  m_BoundFilamentRange(oldEvent.m_BoundFilamentRange),
															  m_BoundFuelSpringConstant(oldEvent.m_BoundFuelSpringConstant),
															  m_BoundFilamentSpringConstant(oldEvent.m_BoundFilamentSpringConstant),
															  m_DeltaHead(oldEvent.m_DeltaHead),
															  m_MotorType(oldEvent.m_MotorType),
															  m_FuelType(oldEvent.m_FuelType),
															  m_FilamentType(oldEvent.m_FilamentType),
															  m_pMotor(oldEvent.m_pMotor),
															  m_pMotorHead(oldEvent.m_pMotorHead),
															  m_pCNTCells(oldEvent.m_pCNTCells),
															  m_MotorTotal(oldEvent.m_MotorTotal)
{
	m_vFuelBindingTimes = oldEvent.m_vFuelBindingTimes;
}

evMotorATPase::~evMotorATPase()
{
}

// Member functions to write/read the data specific to the command.
// The name of bead type to be removed from display is the only data needed.

zOutStream& evMotorATPase::put(zOutStream& os) const
{
	// Write base class data first

	xxEvent::put(os);
	
	os << "    Aggregate					" << m_AggregateName  << "  " << m_AggregateId	<< zEndl;
	os << "    Motor						" << m_Motor						<< zEndl;
	os << "    Fuel							" << m_Fuel							<< zEndl;
	os << "    Filament						" << m_Filament						<< zEndl;
	os << "    FuelPerStroke				" << m_FuelPerStroke				<< zEndl;
	os << "    FuelBindingTime				" << m_FuelBindingTime				<< zEndl;
	os << "    FilamentBindingTime			" << m_FilamentBindingTime			<< zEndl;
	os << "    FuelRange					" << m_FuelRange					<< zEndl;
	os << "    FilamentRange				" << m_FilamentRange				<< zEndl;
	os << "    BoundFuelRange				" << m_BoundFuelRange				<< zEndl;
	os << "    BoundFilamentRange			" << m_BoundFilamentRange			<< zEndl;
	os << "    BoundFuelSpringConstant		" << m_BoundFuelSpringConstant		<< zEndl;
	os << "    BoundFilamentSpringConstant  " << m_BoundFilamentSpringConstant	<< zEndl;

	// Convert the angle for the conformational change back into degrees

	os << "    DeltaHead					" << m_DeltaHead*360.0/xxBase::m_globalTwoPI << zEndl;

	return os;
}

zInStream& evMotorATPase::get(zInStream& is)
{
	// Read base class data first

	xxEvent::get(is);
	
	zString token;
	zString aggregate;
	long	id;
	zString motor;
	zString fuel;
	zString filament;
	long fuelPerStroke;
	long fuelBindingTime;
	long filamentBindingTime;
	double fuelRange, filamentRange;
	double boundFuelRange, boundFilamentRange;
	double boundFuelSpringConstant, boundFilamentSpringConstant;
	double deltaHead;

	is >> token;
	if(!is.good() || token != "Aggregate")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> aggregate >> id;
		if(!is.good() || aggregate.empty() || id < 1 || id > CAnalysis::GetAggregateTotal())
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Motor")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> motor;
		if(!is.good() || motor.length() == 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Fuel")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> fuel;
		if(!is.good() || fuel.length() == 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Filament")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> filament;
		if(!is.good() || filament.length() == 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Now the data relating to the fuel consumption and interactions between
	// the motor and its fuel and filament

	is >> token;
	if(!is.good() || token != "FuelPerStroke")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> fuelPerStroke;
		if(!is.good() || fuelPerStroke < 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "FuelBindingTime")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> fuelBindingTime;
		if(!is.good() || fuelBindingTime < 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "FilamentBindingTime")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> filamentBindingTime;
		if(!is.good() || filamentBindingTime < 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "FuelRange")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> fuelRange;
		if(!is.good() || fuelRange < 0.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "FilamentRange")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> filamentRange;
		if(!is.good() || filamentRange < 0.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Bound fuel must be kept closer to the motor than free fuel, and the bound
	// motor must be closer to the filament than a free motor

	is >> token;
	if(!is.good() || token != "BoundFuelRange")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> boundFuelRange;
		if(!is.good() || boundFuelRange < 0.0 || boundFuelRange > fuelRange)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "BoundFilamentRange")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> boundFilamentRange;
		if(!is.good() || boundFilamentRange < 0.0 || boundFilamentRange > filamentRange)
		{
			SetDataValid(false);
			return is;
		}
	}

	// The motor binds to fuel and the filament using Hookean springs, so the
	// spring constants must be greater than zero

	is >> token;
	if(!is.good() || token != "BoundFuelSpringConstant")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> boundFuelSpringConstant;
		if(!is.good() || boundFuelSpringConstant < 0.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "BoundFilamentSpringConstant")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> boundFilamentSpringConstant;
		if(!is.good() || boundFilamentSpringConstant < 0.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "DeltaHead")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> deltaHead;
		if(!is.good() || deltaHead < 0.0 || deltaHead > 180.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Data has been read successfully so store it in the member variables

	SetDataValid(true);

	m_AggregateName					= aggregate;
	m_AggregateId					= id;
	m_Motor							= motor;
	m_Fuel							= fuel;
	m_Filament						= filament;
	m_FuelPerStroke					= fuelPerStroke;
	m_FuelBindingTime				= fuelBindingTime;
	m_FilamentBindingTime			= filamentBindingTime;
	m_FuelRange						= fuelRange;
	m_FilamentRange					= filamentRange;
	m_BoundFuelRange				= boundFuelRange;
	m_BoundFilamentRange			= boundFilamentRange;
	m_BoundFuelSpringConstant		= boundFuelSpringConstant;
	m_BoundFilamentSpringConstant	= boundFilamentSpringConstant;

	// Convert the angle for the conformational change into radians

	m_DeltaHead	= deltaHead*xxBase::m_globalTwoPI/360.0;

	return is;
}


// Non-static function to return the type of the event

const zString evMotorATPase::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

xxEvent* evMotorATPase::GetEvent() const
{
	return new evMotorATPase(*this);
}

// Set a pointer to the (const) CAnalysis-derived object that the event
// will monitor for activity during the simulation. This cannot be const
// because the event may write data back to the aggregate.

bool evMotorATPase::SetAggregate(CAnalysis* const pAggregate)
{
	m_pMotor = dynamic_cast<CMotor*>(pAggregate);

	return true;
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

bool evMotorATPase::ValidateData(const CInputData &riData)
{
	// Does the specified aggregate exist? If so store a pointer to it

	cAggregateIterator cIterAgg = find_if(riData.GetAggregates().begin(), 
										  riData.GetAggregates().end(), aaGetAggregateName(m_AggregateName));

	if(cIterAgg != riData.GetAggregates().end() && (*cIterAgg)->GetId() == m_AggregateId)
		SetAggregate(*cIterAgg);
	else
		return false;

	// Do the specified polymers exist?

	if(riData.GetPolymerNamesMap().find(m_Motor)	== riData.GetPolymerNamesMap().end() ||
	   riData.GetPolymerNamesMap().find(m_Fuel)		== riData.GetPolymerNamesMap().end() ||
	   riData.GetPolymerNamesMap().find(m_Filament) == riData.GetPolymerNamesMap().end() ||
	   riData.GetPolymerNamesMap().find(m_Motor)	== riData.GetPolymerNamesMap().find(m_Fuel) ||
	   riData.GetPolymerNamesMap().find(m_Motor)	== riData.GetPolymerNamesMap().find(m_Filament) ||
	   riData.GetPolymerNamesMap().find(m_Fuel)		== riData.GetPolymerNamesMap().find(m_Filament) )
		return false;

	// Now we know the polymers exist store their types for later use

	m_MotorType		= (*riData.GetPolymerNamesMap().find(m_Motor)).second;
	m_FuelType		= (*riData.GetPolymerNamesMap().find(m_Fuel)).second;
	m_FilamentType	= (*riData.GetPolymerNamesMap().find(m_Filament)).second;

	// Check that the first three beads of the motor polymer, starting at its head,
	// are KHead-Neck-Hinge. If not fail.


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
// The motor ATPase checks for a fuel polymer near it and, if it finds one, it
// binds it buy creating a bond to tie it to the motor polymer's head bead.
// Then it looks for a filament polymer and binds to it if it finds one.
// The duration of binding the fuel and filament are user-specified.
// A timer is set when the fuel binds and the bond bending potential is changed
// so as to generate the power stroke. When the timer has finished the fuel
// is released, after having its ADP-PI bond doubled in length to prevent the
// fuel rebinding to a motor, and the filament bond is slowly released until 
// another fuel polymer is consumed.

bool evMotorATPase::Execute(long simTime, ISimEvent* const pISimEvent)
{
// If this is the first call count the number of motors to monitor. We have
// already checked that at least one aggregate exists in ValidateData. We use this
// count to distinguish between the first and subsequent calls. Create elements
// to hold the fuel-binding counters for each motor polymer.

	if(simTime >= GetStartTime() && simTime < GetEndTime())
	{

		if(m_MotorTotal == 0)
		{
			m_MotorTotal = m_pMotor->GetMotorTotal();
			m_vFuelBindingTimes.resize(m_MotorTotal);

			// Get a pointer to the motor polymer's head bead as this is the
			// active site for binding.





		}
		else
		{



		}

		// The head bead of the motor polymer (KHead) is the active site for 
		// fuel binding. Its name must be KHead. The next beads must be
		// Neck and Hinge respectively. The remainder of the polymer is
		// arbitrary. 
		//
		// Find any fuel polymers (ATP) in the CNT cell containing a motor 
		// polymer's head, and its immediate neighbours, and a filament polymer
		// as well. If both are found then try and bind the fuel and filament
		// and make a conformational change to the motor causing its tail to rotate
		// with respect to its head. We do this by changing the preferred angle for 
		// the motor's bondpair.
		// The first three beads of the motor polymer must be
		//
		// KHead-Neck-Hinge
		//
		// And a bondpair for these beads must exist. Its parameters are changed
		// by adding a specified angle (m_DeltaHead) to the bondpair's preferred angle.



		return true;
	}
	else
		return false;
}

