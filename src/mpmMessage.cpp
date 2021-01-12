/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpmMessage.cpp: implementation of the mpmMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mpmMessage.h"

// **********************************************************************
// Global Functions.
//
// Function to write out the message data to file. We pass the
// stream output operator to the contained mpmMessage-derived object using
// its put() function. This is because the << and >> operators cannot be
// treated polymporphically.
//
// We write the "Message" keyword and the type of message involved. Particular data 
// for messages are serialized in their own put() and get() functions.
// The endline must only be written in the derived classes not here: even if
// there is no data needed.

zOutStream& operator<<(zOutStream& os, const mpmMessage& message)
{
	os << "Message  " << message.GetMessageType();
	return message.put(os);
}

// Function to read the message's data from file. Because the name and 
// time of execution have already been read we only need to call the 
// derived class' get() function to fill in its specific data.

zInStream& operator>>(zInStream& is, mpmMessage& message)
{
	return message.get(is);
}

// Static member variable holding the number of messages created.

long mpmMessage::m_MessageTotal = 0;

long mpmMessage::GetMessageTotal()
{
	return m_MessageTotal;
}

// Static member function to reset the number of messages to zero.

void mpmMessage::ZeroMessageTotal()
{
	m_MessageTotal = 0;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
//
// 

mpmMessage::mpmMessage() : m_Tag(0), m_SenderPid(0), m_ErrorCode(0), m_id(++mpmMessage::m_MessageTotal)
{
}

// Copying a message does not change its id.

mpmMessage::mpmMessage(const mpmMessage& oldMessage) : IGlobalSimBox(oldMessage), 
                                                       m_Tag(oldMessage.m_Tag),
                                                       m_SenderPid(oldMessage.m_SenderPid),
                                                       m_ErrorCode(oldMessage.m_ErrorCode),
                                                       m_id(oldMessage.m_id)
{
}

mpmMessage::~mpmMessage()
{
    // Do I need to destroy the created type?
}

// Protected functions to write out the <Message> tags that are common to all 
// command classes. They are called by derived classes' put()
// function if the XML option is selected for output. The first function writes
// the command's name and execution time and starts the <Data> element; the second
// function provides the closing tags.

zOutStream& mpmMessage::putXMLStartTags(zOutStream &os) const
{
	os << "<Message>" << zEndl;
	os << "  <Name>" << GetMessageType() << "</Name>" << zEndl;

	return os;
}

zOutStream& mpmMessage::putXMLEndTags(zOutStream &os) const
{
	os << "</Message>" << zEndl;

	return os;
}

// Public function to allow the message tag to be set to zero or
// any positive integer. If this function is not called, all uses 
// of a message instance use the tag set in the constructor.

void mpmMessage::SetTag(long tag)
{
    if(tag >= 0)
	{
		m_Tag = tag;
	}
	else
	{
		m_Tag = 0;
	}
}

// Public function to allow the message's sending processor pid to be set to zero or
// any positive integer. 

void mpmMessage::SetSenderPid(long pid)
{
    if(pid >= 0)
	{
		m_SenderPid = pid;
	}
	else
	{
		m_SenderPid = 0;
	}
}

// Protected functions to write out the message name using 
// ASCII text to save every derived class having to do so. Note that we add a 
// space character but no zEndl after this so that the derived class' data appears 
// on the same line. The second function just provides an endline character but
// may be customised later.

zOutStream& mpmMessage::putASCIIStartTags(zOutStream &os) const
{
	os << "Message " << GetMessageType() << " ";

	return os;
}

// Function to complete the writing of the message using ASCII text.

zOutStream& mpmMessage::putASCIIEndTags(zOutStream &os) const
{
	os << zEndl;

	return os;
}

// Do-nothing implementations that are overridden by derived classes.

// Wrapper for MPI_Send() to the specified processor procId.

void mpmMessage::Send(long procId)
{

}

// Wrapper for MPI_Send() to P0.

void mpmMessage::SendP0()
{

}

// Wrapper for MPI_Send() sent to all processors including the sender

void mpmMessage::SendAll()
{

}

// Wrapper for MPI_Send() sent to all processors except the sender

void mpmMessage::SendAllP()
{

}

// Wrapper for MPI_Send() sent to the sender

void mpmMessage::SendSelf()
{

}

// Wrapper for MPI_Bcast()

void mpmMessage::Broadcast()
{

}

// Wrappers for MPI_Recv(message, count, MPI_CHAR, m_SenderRank, tag, MPI_COMM_WORLD)
// that allow message to specify a sending processor id or not.

void mpmMessage::Receive()
{

}

void mpmMessage::Receive(long procId)
{

}

// ****************************************
// Non-blocking MPI messages

// Wrapper for the non-blocking MPI_Isend() to the specified processor procId.

void mpmMessage::ISend(long procId, MsgRequestPtr pRequest)
{

}

// Wrapper for the non-blocking function MPI_Irecv(message, count, MPI_CHAR, m_SenderRank, tag, MPI_COMM_WORLD);

void mpmMessage::IReceive(long procId, MsgRequestPtr pRequest)
{

}

// Function to complete the non-blocking ISend and IReceive functions.
// We match the request instance passed in to know which call to complete.

bool mpmMessage::Wait(MsgRequestPtr pRequest)
{
    return false;
}

// Function to check if a non-blocking ISend and IReceive function has completed yet and ignore it if not.
// We match the request instance passed in to know which call to complete.

bool mpmMessage::Test(MsgRequestPtr pRequest)
{
    return false;
}

