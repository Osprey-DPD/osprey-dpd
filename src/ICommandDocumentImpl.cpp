/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ICommandDocumentImpl.cpp: implementation of the ICommandDocumentImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimProcessFlags.h"
#include "ICommandDocumentImpl.h"
#include "Experiment.h"
#include "ISetProcessData.h"

#if EnableCommandDocumentProcess == SimProcessEnabled
  #include "prCommandContainer.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ICommandDocumentImpl::ICommandDocumentImpl(CExperiment& rExpt) : m_rExperiment(rExpt),
																 m_DocumentName("")
{
	m_Commands.clear();
}

ICommandDocumentImpl::~ICommandDocumentImpl()
{

}

// Function to return a pointer to the containing class' main IExperiment interface.

IExperiment* ICommandDocumentImpl::IIExperiment() const
{
	return &m_rExperiment;
}

// Function to parse the command document and wrap its commands inside a
// command container process. The process is then added to the simulation's
// set of running process so that the commands can be imported into the
// simulation when the process state is next updated.

bool ICommandDocumentImpl::ReadCommandDocument(const zString docName)
{
#if EnableCommandDocumentProcess == SimProcessEnabled

	// Reset the data before importing the new document

	m_DocumentName = docName;
	m_Commands.clear();

	std::cout << "About to read file " << m_DocumentName << zEndl;

	// Read the document and convert the <Command> elements into xxCommand instances.
	//
	// Not implemented yet!
	//

	// Create a container process and add commands to it. Note that its internal
	// validate function is called within the CSimulation::AddProcess function.
	// This allows the process to be filled with data by the creating routine
	// but the validation is provided in a single, common location.

	prCommandContainer* const pCC = new prCommandContainer();

	for(CommandIterator iterCom=m_Commands.begin(); iterCom!=m_Commands.end(); iterCom++)
	{
		const xxCommand* const pCommand = *iterCom;

		if(pCommand)
			pCC->AddCommand(pCommand);
	}

	// Pass the process back into the simulation checking the return type for 
	// the process' validity status

	return m_rExperiment.GetISetProcessData()->AddProcess(pCC);

#else
    return false;
#endif
}
