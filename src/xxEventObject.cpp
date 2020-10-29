/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxEventObject.cpp: implementation of the xxEventObject class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "xxEventObject.h"
#include "xxEvent.h"
#include "acfEventFactory.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// Function to write out the event data to file. We pass the
// stream output operator to the contained xxEvent-derived object.

zOutStream& operator<<(zOutStream& os, const xxEventObject& EventOb)
{
	if(EventOb.IsEventValid())
		os << *(EventOb.m_pEvent);

	return os;
}

// Function to read the event data from file. We pass the stream input 
// operator to the contained xxEvent-derived object. After reading its 
// data we copy the xxEvent's validity flag back into the xxEventObject
// to indicate that the event has read its data correctly.
//

zInStream& operator>>(zInStream& is, xxEventObject& EventOb)
{
	if(EventOb.IsEventValid())
	{
		is >> *(EventOb.m_pEvent);

		EventOb.SetEventValid(EventOb.m_pEvent->IsValid());
	}

	return is;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// The constructor uses the "type" argument to determine which xxEvent
// object to create and to read the data into. The xxEvent-derived classes
// each have a static member variable that corresponds to the entry in the control
// data file used to identify the command. This entry is accessed using the static
// function GetType(). The acfEventFactory object uses this type identifier to
// call a function implemented by each xxEvent-derived class to create an
// instance of the class.
//
// If the command type is not recognised a flag is set showing that data cannot be read.

xxEventObject::xxEventObject(const zString type) : m_bEventValid(true)
{
	m_pEvent = acfEventFactory::Instance()->Create(type);

	if(!m_pEvent)
	{
		m_bEventValid = false;
	}
}

xxEventObject::~xxEventObject()
{
	// Only delete the contained event if it exists

	if(m_pEvent)
		delete m_pEvent;
}

// Functions to pass a request for the times to check for an event to the contained 
// xxEvent object

long xxEventObject::GetStartTime() const
{
	if(m_pEvent)
		return m_pEvent->GetStartTime();
	else
		return 0;
}

long xxEventObject::GetEndTime() const
{
	if(m_pEvent)
		return m_pEvent->GetEndTime();
	else 
		return 0;
}

// Function to pass a request for an event object pointer to the appropriate derived
// class so that a copy of the event can be created and returned.

xxEvent* xxEventObject::GetEvent() const
{
	if(m_pEvent)
		return m_pEvent->GetEvent();
	else
		return NULL;
}

bool xxEventObject::IsEventValid() const
{
	return m_bEventValid;
}

// Private function to set a flag showing whether the event data is valid.

void xxEventObject::SetEventValid(bool bValid)
{
	m_bEventValid = bValid;
}

