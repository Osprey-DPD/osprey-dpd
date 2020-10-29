/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxCommand.cpp: implementation of the xxCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "xxCommand.h"

// **********************************************************************
// Global Functions.
//
// Function to write out the command data to file. We pass the
// stream output operator to the contained xxCommand-derived object using
// its put() function. This is because the << and >> operators cannot be
// treated polymporphically.
//
// We write the "Command" keyword, the type of command involved and the
// execution time here as they are common to all commands. Particular data 
// for commands are serialized in their own put() and get() functions.
// The endline must only be written in the derived classes not here: even if
// there is no data needed.

zOutStream& operator<<(zOutStream& os, const xxCommand& command)
{
	os << "Command  " << command.GetCommandType() << "  " << command.GetExecutionTime();
	return command.put(os);
}

// Function to read the command data from file. Because the name and 
// time of execution have already been read we only need to call the 
// derived class' get() function to fill in its specific data.

zInStream& operator>>(zInStream& is, xxCommand& command)
{
	return command.get(is);
}

// Static member variable holding the number of commands created.

long xxCommand::m_CommandTotal = 0;

long xxCommand::GetCommandTotal()
{
	return m_CommandTotal;
}

// Static member function to reset the number of commands to zero.

void xxCommand::ZeroCommandTotal()
{
	m_CommandTotal = 0;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// A command object must be given a time at which it is to execute when it is
// created. The SimBox will send it Execute() messages at each time step to
// check if it should perform its operations. It also stores a unique id that
// is the number of commands created. This is a static member variable of 
// the xxCommand class.
//
// Commands are created with their validity set to true: the derived classes' 
// get() routines must set the validity to false if an error occurs while 
// reading a command's data.
// 
// Commands will log their execution to the log state file by default.

xxCommand::xxCommand(long executionTime) : m_id(++xxCommand::m_CommandTotal), 
										   m_Time(executionTime), 
										   m_bCommandValid(true),
										   m_bLogExecution(true)
{
}

// Constructor to allow a command to disable execution logging.

xxCommand::xxCommand(long executionTime, bool bLog) : m_id(++xxCommand::m_CommandTotal), 
										   m_Time(executionTime), 
										   m_bCommandValid(true),
										   m_bLogExecution(bLog)
{
}

// Copying a command does not change its id.

xxCommand::xxCommand(const xxCommand& oldCommand) : xxBase(oldCommand), 
                                                    m_id(oldCommand.m_id), 
													m_Time(oldCommand.m_Time),
												    m_bCommandValid(oldCommand.m_bCommandValid),												
												    m_bLogExecution(oldCommand.m_bLogExecution)													
{
}

xxCommand::~xxCommand()
{
}

void xxCommand::SetCommandValid(bool bValid)
{
	m_bCommandValid = bValid;
}

// Protected functions to write out the <Command> tags that are common to all 
// command classes. They are called by derived classes' put()
// function if the XML option is selected for output. The first function writes
// the command's name and execution time and starts the <Data> element; the second
// function provides the closing tags.

zOutStream& xxCommand::putXMLStartTags(zOutStream &os) const
{
	os << "<Command>" << zEndl;
	os << "  <Name>" << GetCommandType() << "</Name>" << zEndl;
	os << "  <ExecutionTime>" << GetExecutionTime() << "</ExecutionTime>" << zEndl;
	os << "  <Data>" << zEndl;

	return os;
}

zOutStream& xxCommand::putXMLEndTags(zOutStream &os) const
{
	os << "  </Data>" << zEndl;
	os << "</Command>" << zEndl;

	return os;
}

// Protected functions to write out the command name and execution time using 
// ASCII text to save every derived class having to do so. Note that we add a 
// space character but no zEndl after this so that the derived class' data appears 
// on the same line. The second function just provides an endline character but
// may be customised later.

zOutStream& xxCommand::putASCIIStartTags(zOutStream &os) const
{
	os << "Command " << GetCommandType() << "  " << GetExecutionTime() << " ";

	return os;
}

// Function to write out the command name and execution time to save every
// derived class having to do so. Note that we add a space character but no zEndl 
// after this so that the derived class' data appears on the same line.

zOutStream& xxCommand::putASCIIEndTags(zOutStream &os) const
{
	os << zEndl;

	return os;
}

// Do-nothing implementation of a VF function. Derived classes must provide an
// override for this function if they want to pack their arguments into an 
// empty instance. If the packing succeeds, the function should return true, otherwise
// it should return false.  This is used by the command group functionality, and
// only command classes that implement this function can be used inside a command group.

bool xxCommand::Pack(const tguArgumentSequence& vArguments)
{
    return false;
}

// VF that should be implemented by all derived classes that need to return
// the number of their arguments via a non-static function. The derived classes
// define a static member that holds their argument total, and it is returned
// by the implementation of this function.

long xxCommand::GetCommandArgumentTotal() const
{
    return 0;
}
