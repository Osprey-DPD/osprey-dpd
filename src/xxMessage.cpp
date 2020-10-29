/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxMessage.cpp: implementation of the xxMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "xxMessage.h"

// **********************************************************************
// Global Functions.
//
// Function to write out the message data to the log file. 
// Operator<< is used to pass the Serialize message down to the derived classes
// via the virtual Serialize() function. This then passes the message onto
// the derived class's operator<<.
//
// We leave each derived class to serialize its own data including the start and
// end tags for XMl or ASCII output. This is different from the xxCommand implementation.
// We set a flag showing that the message has been written to file so that it is 
// not written repeatedly.
//
// Note that the operator<< cannot use a const reference here because we want to
// set the m_bIsSerialized flag, but all derived classes do use a const operator<<.
// However, the pure virtual Serialize() function is const in all derived classes.

zOutStream& operator<<(zOutStream& os, xxMessage& rMsg)
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	rMsg.putXMLStartTags(os);
	rMsg.Serialize(os);
	rMsg.putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	rMsg.putASCIIStartTags(os);
	rMsg.Serialize(os);
	rMsg.putASCIIEndTags(os);

#endif

	rMsg.m_bIsSerialized = true;

	return os;
}

// Static member variable holding the number of messages created.

long   xxMessage::m_MessageTotal = 0;

// Static member function to return the total number of messages created.
// We have to name it differently so that it does not clash with the 
// class member function GetMessageTotal().

long xxMessage::StaticGetMessageTotal()
{
	return m_MessageTotal;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// Increment the message counter for each message created. When a message is
// destroyed, the counter is decremented.

xxMessage::xxMessage(long time) : m_id(++m_MessageTotal), m_Time(time),
								  m_bIsSerialized(false)

{

}

xxMessage::~xxMessage()
{
	m_MessageTotal--;
}

// Protected functions to write out the <Message></Message> tags that are common 
// to all message classes. They are called by derived classes' put()
// function if the XML option is selected for output. The first function writes
// the message's name, id and execution time and starts the <Body> element; the second
// function provides the closing tags.

zOutStream& xxMessage::putXMLStartTags(zOutStream &os) const
{
	os << "<Message>" << zEndl;
	os << "  <Id>"    << GetId()   << "</Id>"   << zEndl;
	os << "  <Time>"  << GetTime() << "</Time>" << zEndl;


	return os;
}

zOutStream& xxMessage::putXMLEndTags(zOutStream &os) const
{
	// The <Status> start tag is defined in the intermediate base classes 
	// derived from xxMessage that define the types of message that can be output.
	// The CLogMessage-derived classes define the following types of log message:
	//
	// info, analysis, event, constraint, warning and error
	//
	// The colour of each message is defined by the "severity" attribute. of its
	// <Status> tag.

	os << "</Status>" << zEndl;
	os << "</Message>" << zEndl;

	return os;
}

// Protected functions to write out the message id and time using 
// ASCII text to save every derived class having to do so. Note that unlike
// the xxCommand's version, we add a zEndl after this so that the derived class' 
// data appears on the next line. 
//
// The second function just provides an endline character but may be customised later.

zOutStream& xxMessage::putASCIIStartTags(zOutStream &os) const
{
	os << zEndl << "Message " << m_id << " at time " << m_Time  << zEndl;

	return os;
}

zOutStream& xxMessage::putASCIIEndTags(zOutStream &os) const
{
	os << zEndl;

	return os;
}
