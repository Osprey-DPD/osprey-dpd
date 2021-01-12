/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ISerialiseInclusiveRestartState.cpp: implementation of the ISerialiseInclusiveRestartState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ISerialiseInclusiveRestartState.h"
#include "LogRestartStateBuilderError.h"
#include "LogRestartStateBuilderWarning.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ISerialiseInclusiveRestartState::ISerialiseInclusiveRestartState() : m_bNoRestartWarnings(true),
                                                                     m_bValidRestartData(true)
{

}

ISerialiseInclusiveRestartState::~ISerialiseInclusiveRestartState()
{

}

// Function to indicate that the restart state is invalid, and to issue a log
// message describing the fault. The error text is passed in from the calling routine.

void ISerialiseInclusiveRestartState::SetRestartStateInvalid(const zString msg)
{
    m_bValidRestartData = false;
    new CLogRestartStateBuilderError(0, msg);
}

// Function to log a warning message if the restart state is inconsistent but not
// actually in error. The warning message should indicate that the simulation can
// proceed, but the entities defined in it may not all be created or valid.

void ISerialiseInclusiveRestartState::IssueRestartStateWarningMessage(const zString msg)
{
    m_bNoRestartWarnings = false;
    new CLogRestartStateBuilderWarning(0, msg);
}

