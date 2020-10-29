/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acACNCreateEventAnalysis.cpp: implementation of the acACNCreateEventAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acACNCreateEventAnalysis.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "aeActiveSimBox.h"
#include "IModifyActiveCellNetwork.h"
#include "LogacACNCreateEventAnalysis.h"
#include "LogacNetworkNotModifiable.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acACNCreateEventAnalysis::acACNCreateEventAnalysis(long executionTime) : xxCommand(executionTime),
														m_AnalysisId(""), m_SourceId(""),
														m_Key("")
{
}

acACNCreateEventAnalysis::acACNCreateEventAnalysis(const acACNCreateEventAnalysis& oldCommand) : xxCommand(oldCommand),
								   m_AnalysisId(oldCommand.m_AnalysisId),
								   m_SourceId(oldCommand.m_SourceId),
								   m_Key(oldCommand.m_Key)
{
}


acACNCreateEventAnalysis::~acACNCreateEventAnalysis()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_AnalysisId  - Name of the event analysis decorator instance that wraps the ESD
//	m_SourceId	  - Name of the ESD instance that wraps the ACN target
//  m_Key         - Unique alphanumeric key that allows an ESD to select a specific
//                  interface on an EAD when it notifies it of event changes.

zOutStream& acACNCreateEventAnalysis::put(zOutStream& os) const
{

#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	os << "<EventAnalysisName>" << m_AnalysisId  << "</EventAnalysisName>" << zEndl;
	os << "<EventSourceName>"   << m_SourceId    << "</EventSourceName>"   << zEndl;
	os << "<EventKey>"          << m_Key         << "</EventKey>"          << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_AnalysisId << " " << m_SourceId << " " << m_Key;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acACNCreateEventAnalysis::get(zInStream& is)
{
	// We check that the event source and event analysis decorator names
    // are not null, and perform further checks in the IsDataValid() function below.

	is >> m_AnalysisId >> m_SourceId >> m_Key;

	if(!is.good() || m_AnalysisId.empty() || m_SourceId.empty() || m_Key.empty())
	   SetCommandValid(false);

	return is;
}

// Implementation of the command that is sent by the SimBox to each xxCommand

// Function to check that the data defining the command is valid.
//
// We check that the ESD that wraps an ACN target exists, and that the 
// alphanumeric identifier that labels the event source and analysis decorators
// are unique, and that the ESD exists and the new analysis decorator does not
// already exist. If these conditions are satisfied, we add the new
// event analysis decorator to the CInputData's container,  casting away the 
// reference's const-ness first.

bool acACNCreateEventAnalysis::IsDataValid(const CInputData& riData) const
{
	CInputData& rincData = const_cast<CInputData&>(riData);

	if(!riData.IsExternalNameValid(m_SourceId))
		return ErrorTrace("Invalid event source decorator name");
	else if(!riData.IsActiveCommandTargetPresent(m_SourceId))
		return ErrorTrace("Specified event source decorator does not exist");
	else if(!riData.IsExternalNameValid(m_AnalysisId))
		return ErrorTrace("Invalid event analysis decorator name");
	else if(riData.IsActiveCommandTargetPresent(m_AnalysisId))
		return ErrorTrace("Event analysis decorator with this name already exists");
	else if(!riData.IsExternalNameValid(m_Key))
		return ErrorTrace("Invalid event analysis key name");

	// New event analysis label is unique so add it to the container in the CInputData.
    // Note that it is added to the set of "active" targets so that it is continually
    // updated.

	rincData.AddActiveCommandTargetName(m_AnalysisId);

	return true;
}
