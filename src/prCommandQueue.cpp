/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prCommandQueue.cpp: implementation of the prCommandQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "prCommandQueue.h"
#include "LogCommandQueueProcessError.h"
#include "SimState.h"
#include "ISimBox.h"
#include "TimeSeriesData.h"
#include "InputData.h"
#include "xxCommandObject.h"


// STL using declarations

	#include <limits>
	using std::numeric_limits;
	using std::streamsize;
	using std::ios_base;
				
//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.

const zString prCommandQueue::m_Type = "CommandQueue";

const zString prCommandQueue::GetType()
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
	xxProcess* Create() {return new prCommandQueue();}

	const zString id = prCommandQueue::GetType();

#if EnableCommandQueueProcess == SimProcessEnabled
	const bool bRegistered = acfProcessFactory::Instance()->Register(id, Create);
#endif
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

prCommandQueue::prCommandQueue() : xxProcess(true),
									m_Multiplier(100),
                                    m_FileName(xxBase::GetCQPrefix()),
									m_Separator("->"),
									m_Comment("//"),
									m_CommandsRead(0)
{
	// If the comment character is made user-definable, a check must be added to
	// ensure it is distinct from the separator character.

	m_CommandObjects.clear();
}

prCommandQueue::~prCommandQueue()
{
	// Delete the xxCommandObjects and note that they delete their contained
	// command. The xxCommands must be copied to the CAnalysisState so that
	// they can be executed.

	if(!m_CommandObjects.empty())
	{
		for(CommandObjectIterator iterCO=m_CommandObjects.begin(); iterCO!=m_CommandObjects.end(); iterCO++)
		{
			delete *iterCO;
		}
		m_CommandObjects.clear();
	}
}

// Member functions to write/read the data specific to the process.

zOutStream& prCommandQueue::put(zOutStream& os) const
{
	// Write base class data first

	xxProcess::put(os);
	
	os << "    Multiplier			" << m_Multiplier << zEndl;
	os << "    FileName			    " << m_FileName   << zEndl;
	os << "    Separator			" << m_Separator  << zEndl;

	return os;
}

// Function to read the data needed by the process.
//
// This process requires three pieces of data. The first datum specifies how
// frequently the command file should be checked for new commands. The second
// is the name of the command file. The third is the string used to separate 
// commands that have been read from those to be read. 

zInStream& prCommandQueue::get(zInStream& is)
{
	// Read base class data first

	xxProcess::get(is);

	zString token;
	long multi;
	zString fileName, separator;

	is >> token;
	if(!is.good() || token != "Multiplier")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> multi;
		if(!is.good() || multi < 1)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "FileName")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> fileName;
		if(!is.good() || fileName.empty() || fileName == "Separator")
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Separator")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> separator;
		if(!is.good() || separator.empty())
		{
			SetDataValid(false);
			return is;
		}
	}

	m_Multiplier	= multi;
	m_FileName		= fileName;
	m_Separator		= separator;

	return is;
}


// Non-static function to return the type of the process

const zString prCommandQueue::GetProcessType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current process.

xxProcess* prCommandQueue::GetProcess() const
{
	return new prCommandQueue(*this);
}

// Function to update the state of the process. This process reads a user-specified
// file for commands that are to be added to the command queue.
//
// The sequence of steps is as follows:
//
//	1	Check to see if the specified command file exists; if not the process returns.
//
//	2	Create a local zFileStream object and open the file for read/write access.
//
//	3	Find the last separator string: if no separator is found, read all commands
//		in the file as this is the first time the file has been read.
//
//	4	Once all commands have been read, add a new separator to the end of the file.
//
//	5	Close the file.
//
// Note that the process must open and close the file each time it reads it. 
// This allows the user to add new commands to the queue without causing an IO error.
// Also note that the file must end with a newline character or else the stream
// enters a failed state on reading.

void prCommandQueue::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
#if EnableCommandQueueProcess == SimProcessEnabled

	if(xxFile::DoesFileExist(m_FileName) && 
	  (pISimBox->GetCurrentTime()%(m_Multiplier*pISimBox->GetSamplePeriod()) == 0))
	{
		// Check to see if the simulation has been extended and, if so, 
		// update the end time of the process

		if(pISimBox->GetTotalTime() > GetEndTime())
		{
			SetEndTime(pISimBox->GetTotalTime());
		}

		zFileStream ioStream;	// We need a read/write stream object

		zFileStream::pos_type readPos = 0;	// Needed to hold separator position

		ioStream.open(m_FileName.c_str(), ios_base::in|ios_base::out);

		// If the file exists but could not be opened, or is empty,
		// flag an error to the log file but continue the simulation

		if(!ioStream.fail())
		{
			// Find the last occurence of the separator; if found, read all 
			// subsequent commands, if it is not found, rewind to the start 
			// of the file and read all commands.
			// Write the separator to the end of the file in both cases.

			zString token = "";
			bool bFound = false;
			bool bData  = false;

			while(!ioStream.eof())
			{
				ioStream >> token;

				if(token == m_Separator)
				{
					bFound = true;
					readPos = ioStream.tellg();
				}
			}

			ioStream.clear();
			ioStream.seekg(readPos);

			while(!ioStream.eof())
			{
				ioStream >> token;

				if(!token.empty())
					bData = true;
			}

			// If there are commands in the file and the separator was not found 
			// then rewind to the start of the file to read all commands. If the 
			// separator was found then read only the commands that occur after it.
			// Note that the flush() operation does not move the stream
			// position. We reset the stream's eof flag by calling clear().

			ioStream.clear();

			if(bData)
			{
				if(!bFound)
				{
					ioStream.seekg(0, ios_base::beg);
				}
				else
				{
					ioStream.seekg(readPos);
				}

				if(ReadCommands(ioStream))	// Read all commands, but count only new ones
				{
					if(m_CommandsRead > 0)
					{
						// Now store the commands in the CSimState's queue for later execution.
						// Each xxCommandObject instance holds an xxCommand pointer. 
						// The xxCommand::GetCommand() member function returns a new
						// xxCommand instance so subsequent use of the command instance
						// is decoupled from the instance stored in the xxCommandObject.

						for(CommandObjectIterator iterCO=m_CommandObjects.begin(); iterCO!=m_CommandObjects.end(); iterCO++)
						{
							const xxCommand* const pCommand = (*iterCO)->GetCommand();

							if(pCommand)
								pISimBox->AddCommand(pCommand);
						}

						// Stream is already at eof so we can just write to it

						ioStream.clear();
						ioStream << zEndl << m_Separator << zEndl;
					}
					else
					{
						// If no commands were read, ie, all were commented out,
						// just clear the stream from the eof state prior to closing it

						ioStream.clear();
					}

				}
				else // Error trying to read commands from queue file
				{
					 new CLogCommandQueueProcessError(pISimBox->GetCurrentTime(), "Error reading commands from queue file: " + m_FileName);

					// The stream may have stopped reading before the eof, so we
					// have to position it at the end before trying to write to it

					ioStream.clear();
					ioStream.seekg(0, ios_base::end);
					ioStream << zEndl << m_Separator << zEndl;

				}
			}

//			if(ioStream.rdstate() & ios_base::eofbit)
//			{
//				std::cout << "1 eof bit is set" << zEndl;
//				ioStream.clear(ios_base::eofbit);
//			}

			ioStream.close();

			if(ioStream.fail()) // Error closing queue file
			{
				 new CLogCommandQueueProcessError(pISimBox->GetCurrentTime(), "Error closing command queue file: " + m_FileName);
			}
		}
		else // Error opening queue file
		{
			 new CLogCommandQueueProcessError(pISimBox->GetCurrentTime(), "Error opening command queue file: " + m_FileName);
		}
	}
#endif
}

// Function to check that the user-specified data is valid and, if so, 
// to find the aggregates and events that are used by the process.

// This process checks a file to see if new commands have been entered that
// are to be executed during an executing simulation. If the file does not
// exist no action is taken. This is not an error as the user may not want
// commands to be added during a given run even though the process is specified,
// or they may create the file later in the run. Once the file name has been
// specified and the separator string set, they may not be altered during the
// run.
//
// An xxProcessState is created to hold the data that the process wants
// to write to file. Data is passed as a series of CTimeSeriesData objects
// as used for CHistoryState.

bool prCommandQueue::ValidateData(const CInputData &riData)
{
	// All data has been validated so create and store an xxProcessState object
	// to allow the xxProcess to write its data to a process state file.

    SetState(new xxProcessState(xxBase::GetPSPrefix() + GetProcessType() + ToString(GetId()) + "." + riData.GetRunId(), GetStartTime(), GetEndTime()));

	return true;
}

// Private member function to read a sequence of commands from the queue file.
//
// The format for the commands is the same as in the control data file:
//
//	Command WallOff	10000	data1 data2 ...
//
// If the first non-whitespace character string on a line is equal to the 
// m_Comment string, the rest of that line is ignored. This does not affect 
// the remainder of the command lines. 

// NB. The comment string may be made user-definable later, in which case we have
// to ensure that such a string cannot be part of the first token, viz, Command. 
// This requires that the user must be prevented from specifying such a string.
//
// This function assumes that each command occupies only one line, and that the 
// first token read is the keyword "Command". If these conditions are not met,
// the routine flags an error. The command queue file should contain only commands
// interspersed with single lines containing the separator string. The separator
// string must be read before calling this routine. It also assumes that once it 
// is called only commands occur subsequently. The number of commands is returned
// in the m_CommandsRead member variable.

bool prCommandQueue::ReadCommands(zFileStream& ioStream)
{
	bool bSuccess = true;

#if EnableCommandQueueProcess == SimProcessEnabled

	m_CommandsRead = 0;
    long lastExecutionTime = 1;

	zString token;
	ioStream >> token;

	while(ioStream.good() && bSuccess)
	{
		const long unsigned int commentPos = token.find(m_Comment);

		if(commentPos == 0)
		{
			// If the line starts with the m_Comment string ignore the command.
			// Note that we have to use the \n character here not zEndl
			// as the latter gives an error.

			ioStream.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else if(commentPos < token.size() || token != "Command")
		{
			// If the comment string appears but is not the first non-whitespace
			// characters, flag an error

			bSuccess = false;
		}
		else
		{
			// Otherwise read the command

			zString commandName("");
			long executionTime = 0;

			ioStream >> commandName >> executionTime;

			if(ioStream.good() && executionTime >= lastExecutionTime)
			{
				lastExecutionTime = executionTime;

				xxCommandObject* pCO = new xxCommandObject(commandName, executionTime);
				ioStream >> *pCO;

				if(!ioStream.good() || !pCO->IsCommandValid())
					bSuccess = false;

				m_CommandObjects.push_back(pCO);
				m_CommandsRead++;
			}
			else
			{
				bSuccess = false;
			}
		}


		token = "";
		ioStream >> token;
	}

	if(!ioStream.eof())
	{
		bSuccess = false;
	}
#endif

	return bSuccess;
}
