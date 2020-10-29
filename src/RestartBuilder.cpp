/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// RestartBuilder.cpp: implementation of the CRestartBuilder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "RestartBuilder.h"
#include "InitialState.h"
#include "InclusiveRestartState.h"

#include "SimMathFlags.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// A restart builder only needs the id of the previous run and the time
// identifying the state to use as the initial configuration. The file name
// of the restart data file is constructed from these. Note that the stateId
// must be a long not a string because it is used in xxState to store the
// time at which the restart state was saved.

CRestartBuilder::CRestartBuilder(zString runId, long stateId) : m_RunId(runId),
																m_StateId(stateId)
{
}

CRestartBuilder::~CRestartBuilder()
{
}


// Function to read the data for a restart run from a previously-saved simulation.
// It uses the information to modify the default CInputData data.
//
//    Variable			Modifiable?		Other Data modifiable
// **********************************************************
// Title					Yes
// Date						Yes
// Comment					Yes
// StepSize					Yes
// TotalTime				Yes
// RNGSeed					Yes
// SamplePeriod				Yes
// AnalysisPeriod			Yes
// DensityPeriod			Yes
// DisplayPeriod			Yes
// RestartPeriod			Yes
// Grid						Yes
//
// and the commands and analysis objects.

bool CRestartBuilder::Assemble(CInitialState& riState)
{
	// The CInitialState object has created the beads, bonds and polymers so
	// here we assign them coordinates from their equivalent values in the 
	// restart file. We rely on the numbering 
	// of the polymers, beads and bonds to identify each new polymer with its 
	// equivalent in the old run. 
    
    // There are two ways to read the restart state: the original method is to
    // create a CRestartState object that only reads/writes the bead coordinates.
    // The new method is to use a CInclusiveRestartState that allows data
    // created dynamically during a run to be stored in a restart state file.
    // This method collects data from several simulation objects and collates
    // it before writing it to the file. When reading an inclusive restart state
    // file, the data are all stored in the initial state object as this builder
    // has no access to the other simulation state objects. The initial state
    // instance is then responsible for propagating the data to the rest of the
    // simulation.

	// If the original control data file does not exist, or an error occurs
	// opening the file or while reading the data we cancel the restart run.

	CInclusiveRestartState oldState(m_StateId, m_RunId, riState);

	if(oldState.IsFileStateOk() && oldState.Serialize())
		return true;

	return false;
}
