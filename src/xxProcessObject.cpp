/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// xxProcessObject.cpp: implementation of the xxProcessObject class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "xxProcessObject.h"
#include "xxProcess.h"
#include "acfProcessFactory.h"

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// Function to write out the process data to file. We pass the
// stream output operator to the contained xxProcess-derived object.

zOutStream& operator<<(zOutStream& os, const xxProcessObject& ProcessOb)
{
	if(ProcessOb.IsProcessValid())
		os << *(ProcessOb.m_pProcess);

	return os;
}

// Function to read the process data from file. We pass the stream input 
// operator to the contained xxProcess-derived object. After reading its 
// data we copy the xxProcess' validity flag back into the xxProcessObject
// to indicate that the process has read its data correctly.
//

zInStream& operator>>(zInStream& is, xxProcessObject& ProcessOb)
{
	if(ProcessOb.IsProcessValid())
	{
		is >> *(ProcessOb.m_pProcess);

		ProcessOb.SetProcessValid(ProcessOb.m_pProcess->IsProcessValid());
	}

	return is;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// The constructor uses the "type" argument to determine which xxProcess
// object to create and to read the data into. The xxProcess-derived classes
// each have a static member variable that corresponds to the entry in the control
// data file used to identify the command. This entry is accessed using the static
// function GetType(). The acfProcessFactory object uses this type identifier to
// call a function implemented by each xxProcess-derived class to create an
// instance of the class.
//
// If the process type is not recognised a flag is set showing that data cannot be read.

xxProcessObject::xxProcessObject(const zString type) : m_bProcessValid(true)
{
	m_pProcess = acfProcessFactory::Instance()->Create(type);

	if(!m_pProcess)
	{
		m_bProcessValid = false;
	}
}

xxProcessObject::~xxProcessObject()
{
	// Only delete the contained process if it exists

	if(m_pProcess)
		delete m_pProcess;
}

// Functions to pass a request for the times to monitor a process
// to the contained xxProcess object

long xxProcessObject::GetStartTime() const
{
	if(m_pProcess)
		return m_pProcess->GetStartTime();
	else
		return 0;
}

long xxProcessObject::GetEndTime() const
{
	if(m_pProcess)
		return m_pProcess->GetEndTime();
	else 
		return 0;
}

// Function to pass a request for a process pointer to the appropriate derived
// class so that a copy of the process can be created and returned.

xxProcess* xxProcessObject::GetProcess() const
{
	if(m_pProcess)
		return m_pProcess->GetProcess();
	else
		return NULL;
}

bool xxProcessObject::IsProcessValid() const
{
	return m_bProcessValid;
}

// Private function to set a flag showing whether the process data is valid.

void xxProcessObject::SetProcessValid(bool bValid)
{
	m_bProcessValid = bValid;
}

