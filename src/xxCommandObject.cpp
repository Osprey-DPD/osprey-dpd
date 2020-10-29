/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxCommandObject.cpp: implementation of the xxCommandObject class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "xxCommandObject.h"
#include "xxCommand.h"
#include "acfCommandFactory.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// Function to write out the command data to file. We pass the
// stream output operator to the contained xxCommand-derived object.

zOutStream& operator<<(zOutStream& os, const xxCommandObject& ComOb)
{
	if(ComOb.IsCommandValid())
		os << *(ComOb.m_pCommand);

	return os;
}

// Function to read the command data from file. We pass the stream input 
// operator to the contained xxCommand-derived object. We also check that 
// the data is read correctly and set the local flag appropriately.
//

zInStream& operator>>(zInStream& is, xxCommandObject& ComOb)
{
	if(ComOb.IsCommandValid())
	{
		is >> *(ComOb.m_pCommand);

		ComOb.SetCommandValid(ComOb.m_pCommand->IsCommandValid());
	}

	return is;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// The constructor uses the "type" argument to determine which xxCommand
// object to create and to read the data into. The xxCommand-derived classes
// each have a static member variable that corresponds to the entry in the control
// data file used to identify the command. This entry is accessed using the static
// function GetType(). The acfCommandFactory object uses this type identifier to
// call a function implemented by each xxCommand-derived class to create an
// instance of the class.
//
// If the command type is not recognised a flag is set showing that data cannot be read.

xxCommandObject::xxCommandObject(const zString type, long executionTime) : m_bCommandValid(true)
{
	m_pCommand = acfCommandFactory::Instance()->Create(type, executionTime);

	if(!m_pCommand)
	{
		m_bCommandValid = false;
	}
}

xxCommandObject::~xxCommandObject()
{
	// Only delete the contained command if it exists

	if(m_pCommand)
		delete m_pCommand;
}

bool xxCommandObject::IsCommandValid() const
{
	return m_bCommandValid;
}

void xxCommandObject::SetCommandValid(bool bValid)
{
	m_bCommandValid = bValid;
}

long xxCommandObject::GetExecutionTime() const
{
	if(m_pCommand)
		return m_pCommand->GetExecutionTime();
	else
		return 0;
}

// Function to pass a request for command object pointer to the appropriate derived
// class so that a copy of the command can be created and returned.

const xxCommand* xxCommandObject::GetCommand() const
{
	if(m_pCommand)
		return m_pCommand->GetCommand();
	else
		return NULL;
}
