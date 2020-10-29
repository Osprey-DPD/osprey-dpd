/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxEvent.cpp: implementation of the xxEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "xxEvent.h"

// **********************************************************************
// Global Functions.
//
// Function to write out the event data to file. We pass the
// stream output operator to the contained xxEvent-derived object using
// its put() function. This is because the << and >> operators cannot be
// treated polymporphically.
//

zOutStream& operator<<(zOutStream& os, const xxEvent& event)
{
	return event.put(os);
}

// Function to read the event data from file. Because the name has
// already been read we only need to call the derived class' get() function 
// to fill in its specific data.

zInStream& operator>>(zInStream& is, xxEvent& event)
{
	return event.get(is);
}

// Static member variable holding the number of events created.

long xxEvent::m_EventTotal = 0;

long xxEvent::GetEventTotal()
{
	return m_EventTotal;
}
// Static member function to reset the number of events to zero.

void xxEvent::ZeroEventTotal()
{
	m_EventTotal = 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// An event must be given time to start and stop executing when it is created. 
// The SimBox will send it Execute() messages at each time step to check if 
// it should perform its operations. 
// It also stores a unique id that is the number of events created. 
// We also store a static member that holds the total number of events created. 
// Static member functions are provided to query this value and to reset it to 
// zero between simulations.
//
// Events are created with their validity and active flags set to true: the 
// derived classes' get() routines must set the validity to false if an error 
// occurs while reading a event's data. 

xxEvent::xxEvent() : m_id(++xxEvent::m_EventTotal), 
					 m_Start(0), m_End(0), 
					 m_bValid(true),
				 	 m_bActive(true),
					 m_bInitialise(true)
{
}

// Copying an event does not change its id.

xxEvent::xxEvent(const xxEvent& oldEvent) : xxBase(oldEvent), 
                                            m_id(oldEvent.m_id), 
										    m_Start(oldEvent.m_Start),
										    m_End(oldEvent.m_End),
											m_bValid(oldEvent.m_bValid),
											m_bActive(oldEvent.m_bActive),
											m_bInitialise(oldEvent.m_bInitialise)
{
}

xxEvent::~xxEvent()
{
}


// We write the "Event" keyword, the type of event involved and the
// start and end times for event monitoring here as they are common to all events.
// Particular data for each object is serialized in its own put() and get() functions.

zOutStream& xxEvent::put(zOutStream& os) const
{
	os << "Event" << zEndl;
	os << "    Type         " << GetEventType()		<< zEndl;
	os << "    Times        " << GetStartTime()		<< "  " << GetEndTime() << zEndl;

	return os;
}

// The "Event" keyword and the "Type" keyword and its value have already been 
// read in CInputDPDFile as they determine which xxEvent-derived object 
// to create, we fill in the general event data here and call the derived 
// class' get() function to fill in its specific data.
//
// Note that we cannot read in the aggregate's type here because different events
// may need more than one aggregate. We leave it to the derived classes to deal
// with their aggregates.

zInStream& xxEvent::get(zInStream& is)
{
// Read the start and end times

	zString token;
	long start, end;

	is >> token;
	if(!is.good() || token != "Times")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> start >> end;
		if(!is.good() || start < 0 || end < start)
		{
			SetDataValid(false);
			return is;
		}
	}

	// Data was read successfully so copy it into member variables

	SetStartTime(start);
	SetEndTime(end);

	return is;
}

// Function to validate the data when an event is created on the fly. It is
// equivalent to the above ValidateData() function. it should be implemented
// by all event classes that may be created during a simulation.

bool xxEvent::InternalValidateData(const CSimState& rSimState, xxEvent* const pEvent)
{
	return false;
}
