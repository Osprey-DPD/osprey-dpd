/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acACNCreateEventSource.cpp: implementation of the acACNCreateEventSource class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acACNCreateEventSource.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "aeActiveSimBox.h"
#include "IModifyActiveCellNetwork.h"
#include "LogacACNCreateEventSource.h"
#include "LogacNetworkNotModifiable.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acACNCreateEventSource::acACNCreateEventSource(long executionTime) : xxCommand(executionTime),
														m_ACNTargetId(""),
														m_SourceId("")
{
}

acACNCreateEventSource::acACNCreateEventSource(const acACNCreateEventSource& oldCommand) : xxCommand(oldCommand),
								   m_ACNTargetId(oldCommand.m_ACNTargetId),
								   m_SourceId(oldCommand.m_SourceId)

{
}


acACNCreateEventSource::~acACNCreateEventSource()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_SourceId	   - Name of the event source decorator instance that wraps the ACN target
//  m_ACNTargetId  - Name of the command target that wraps an ACN
//

zOutStream& acACNCreateEventSource::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<EventSourceName>"   << m_SourceId     << "</EventSourceName>"	<< zEndl;
	os << "<CommandTargetName>" << m_ACNTargetId  << "</CommandTargetName>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_SourceId << " " << m_ACNTargetId;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acACNCreateEventSource::get(zInStream& is)
{
	// We check that the event source name and ACN name re not null, and perform 
    // further checks in the IsDataValid() function below.

	is >> m_SourceId >> m_ACNTargetId;

	if(!is.good() || m_SourceId.empty() || m_ACNTargetId.empty())
	   SetCommandValid(false);

	return is;
}

// Implementation of the command that is sent by the SimBox to each xxCommand

// Function to check that the data defining the command is valid.
//
// We check that the command target that wraps an ACN exists, and that the 
// alphanumeric identifier that labels the event source decorator is unique, and
// does not already exist. If these conditions are satisfied, we add the new
// event source decorator to the CInputData's container,  casting away the reference's
// const-ness first.

bool acACNCreateEventSource::IsDataValid(const CInputData& riData) const
{
	CInputData& rincData = const_cast<CInputData&>(riData);

	if(!riData.IsExternalNameValid(m_SourceId))
		return ErrorTrace("Invalid event source decorator name");
	else if(riData.IsActiveCommandTargetPresent(m_SourceId))
		return ErrorTrace("Event source decorator with this name already exists");
	else if(!riData.IsExternalNameValid(m_ACNTargetId))
		return ErrorTrace("Invalid ACN command target name");
	else if(!riData.IsCommandTargetPresent(m_ACNTargetId))
		return ErrorTrace("ACN Command target does not exist");

	// New event source label is unique so add it to the container in the CInputData.
    // Note that it is added to the set of "active" targets so that it is continually
    // updated.

	rincData.AddActiveCommandTargetName(m_SourceId);

	return true;
}
