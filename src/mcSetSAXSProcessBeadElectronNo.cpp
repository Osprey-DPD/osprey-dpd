/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mcSetSAXSProcessBeadElectronNo.cpp: implementation of the mcSetSAXSProcessBeadElectronNo class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mcSetSAXSProcessBeadElectronNo.h"
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

const zString mcSetSAXSProcessBeadElectronNo::m_Type = "SetSAXSProcessBeadElectronNo";

const zString mcSetSAXSProcessBeadElectronNo::GetType()
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
	xxCommand* Create(long executionTime) {return new mcSetSAXSProcessBeadElectronNo(executionTime);}

	const zString id = mcSetSAXSProcessBeadElectronNo::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mcSetSAXSProcessBeadElectronNo::mcSetSAXSProcessBeadElectronNo(long executionTime) : xxCommand(executionTime),
															   m_Pid(-1), m_Name(""), m_Eno(0.0)
{
}

mcSetSAXSProcessBeadElectronNo::mcSetSAXSProcessBeadElectronNo(const mcSetSAXSProcessBeadElectronNo& oldCommand) : xxCommand(oldCommand),
											m_Pid(oldCommand.m_Pid),
                                            m_Name(oldCommand.m_Name),
											m_Eno(oldCommand.m_Eno)
{
}

// Constructor for use when creating the command internally.

mcSetSAXSProcessBeadElectronNo::mcSetSAXSProcessBeadElectronNo(long executionTime, long pid, const zString name, double eno) : xxCommand(executionTime), m_Pid(pid), m_Name(name), m_Eno(eno)
{
}

mcSetSAXSProcessBeadElectronNo::~mcSetSAXSProcessBeadElectronNo()
{
}

// Member functions to read/write the data specific to the command.
// Note that the xxCommand base class does not provide an implementation of
// the put() and get() functions, but it does provide helper functions to
// write the start and end tags for the XML command output. The flag showing
// if the output is to be written as ASCII or XML is set in SimXMLFlags.h that is
// #included in the xxCommand.h header file, so  it is visible to all command classes.
//
// Arguments
// *********
//
//  m_Pid   - Unique integer process id, which is just the number reflecting the order of creation of the process
//  m_Name  - Name of bead type whose electron density is being set
//  m_Eno   - Real number of electrons in the bead type. All assumed to be exactly at the centre of the bead, and we allow fractions.

zOutStream& mcSetSAXSProcessBeadElectronNo::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
    os << "<ProcessId>"  << m_Pid  << "</ProcessId>" << zEndl;
    os << "BeadName>"  << m_Name  << "</BeadName>" << zEndl;
	os << "<Number>" << m_DisplayId << "</Number>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_Pid << " " << m_Name << " " << m_Eno;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& mcSetSAXSProcessBeadElectronNo::get(zInStream& is)
{
	is >> m_Pid >> m_Name >> m_Eno;

	if(!is.good() || m_Pid < 1 || m_Name.empty() || m_Eno < 0.0)
		SetCommandValid(false);

	return is;
}

// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 
//
// Note that even though this command is destined for the IMonitor interface, we
// have to pass it to the ISimCmd interface first because it will be checked for
// execution in the CSimBox's command loop and then passed on to the CMonitor.

bool mcSetSAXSProcessBeadElectronNo::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SetSAXSProcessBeadElectronNo(this);
		return true;
	}
	else
		return false;
}

// Non-static function to return the type of the command

const zString mcSetSAXSProcessBeadElectronNo::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* mcSetSAXSProcessBeadElectronNo::GetCommand() const
{
	return new mcSetSAXSProcessBeadElectronNo(*this);
}

// Function to check that the bead name specified in the command actually
// occurs in the beads names map created in CInputData.

bool mcSetSAXSProcessBeadElectronNo::IsDataValid(const CInputData &riData) const
{
	if(!riData.IsExternalNameValid(m_Name))
		return ErrorTrace("Invalid bead name");

	return true;
}
