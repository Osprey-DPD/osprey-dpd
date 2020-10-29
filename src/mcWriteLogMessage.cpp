/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcWriteLogMessage.cpp: implementation of the mcWriteLogMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcWriteLogMessage.h"
#include "ISimCmd.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString mcWriteLogMessage::m_Type = "WriteLogMessage";

const zString mcWriteLogMessage::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// command class but a global function hidden in the namespace.

namespace
{
	xxCommand* Create(long executionTime) {return new mcWriteLogMessage(executionTime);}

	const zString id = mcWriteLogMessage::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// We use the same string as the start and end tokens for now.

mcWriteLogMessage::mcWriteLogMessage(long executionTime) : xxCommand(executionTime),
                                    m_StartToken("//"), m_EndToken("//"),
									m_Message("")
{
}

// Constructor for use when issuing the command internally

mcWriteLogMessage::mcWriteLogMessage(long executionTime, const zString message) : xxCommand(executionTime),
                                    m_StartToken("//"), m_EndToken("//"),
									m_Message(message)
{
}

// Constructor for use when the start and end tokens are to be specifically set.

mcWriteLogMessage::mcWriteLogMessage(long executionTime, const zString start, const zString end) : xxCommand(executionTime),
                                    m_StartToken(start), m_EndToken(end),
									m_Message("")
{
}

// Copy constructor

mcWriteLogMessage::mcWriteLogMessage(const mcWriteLogMessage& oldCommand) : xxCommand(oldCommand),
                                    m_StartToken(oldCommand.m_StartToken), 
                                    m_EndToken(oldCommand.m_EndToken),
									m_Message(oldCommand.m_Message)
{
}

mcWriteLogMessage::~mcWriteLogMessage()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_Message		Text string to write to the log file
//

zOutStream& mcWriteLogMessage::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<Message>" << m_Message << "</Message>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_StartToken << "  " << m_Message << "  " << m_EndToken;
	putASCIIEndTags(os);

#endif

	return os;
}

// In order to allow the user to enter an arbitrary text string as the message,
// we need to define start and stop tokens. For the moment, we use the C++ comment
// token "//". These must be the first and last tokens in the string, and must have
// whitespace on both sides. If only the start token is found, the rest of the 
// input file will be treated as part of the message, all subsequent commands are 
// ignored, and the simulation proceeds normally.  Note that whitespace in the message 
// is normalised to a single space character between tokens.
//
// To retain the structure of the command sequence, we insert newline characters
// whenever the token is the "Command" keyword. This will be fooled if the 
// message itself contains this word.

zInStream& mcWriteLogMessage::get(zInStream& is)
{
    zString startToken;
    zString nextToken;

    is >> startToken;

	if(!is.good() || startToken != m_StartToken)
	   SetCommandValid(false);

    is >> nextToken;
    while(nextToken != m_EndToken && !is.eof())
    {
        if(nextToken == "Command")
        {
            m_Message += "\n" + nextToken;
        }
        else
        {
            m_Message += " " + nextToken;
        }
	    is >> nextToken;
    }


	if(!is.eof() && is.fail())
    {
	   SetCommandValid(false);
    }
    else
    {
       // Reset the stream state so that the calling function does not register
       // an error condition because we reached the end of the file

       is.clear();
    }

	return is;
}

// Non-static function to return the type of the command

const zString mcWriteLogMessage::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcWriteLogMessage::GetCommand() const
{
	return new mcWriteLogMessage(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool mcWriteLogMessage::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->WriteLogMessage(this);
		return true;
	}
	else
		return false;
}

// Function to check that the specified message is recognised and that the 
// time is within the simulation time. Note that we cannot check that the
// user has not specified a time that is in the past. But if they do we just
// ignore the command.

bool mcWriteLogMessage::IsDataValid(const CInputData& riData) const
{

	return true;
}
