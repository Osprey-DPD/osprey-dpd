/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// CommandFile.cpp: implementation of the CCommandFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimXMLFlags.h"
#include "CommandFile.h"
#include "ISimBox.h"
#include "xxCommand.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// The name of the file is made by concatenating "prefix", the runId and
// an extension that is either ".xml" or ".dat" depending on the value
// of the EnableXMLCommands flag.

CCommandFile::CCommandFile(const ISimBox* const pISimBox, const zString protocol, 
							 const zString runId, const zString extension,
                             CommandSequence commands) : xxFile(xxBase::GetCHPrefix() + runId + extension, true), 
													m_pISimBox(pISimBox),
													m_Protocol(protocol),
													m_RunId(runId),
													m_Extension(extension),
													m_Commands(commands)
{
}

CCommandFile::~CCommandFile()
{
}

// Function to write out the commands from the current simulation.

bool CCommandFile::Serialize()
{


// ****************************************
// Only write the command file XML header if xml is enabled

#if EnableXMLCommands == SimXMLEnabled

	// First write the xml and stylesheet PIs: note that the version and
	// stylesheet name are hardwired here.

	m_outStream << "<?xml version=\"1.0\" ?>" << zEndl;
	m_outStream << "<?xml-stylesheet type=\"text/css\" href=\"command.css\" ?>" << zEndl;
	m_outStream << "<Commands>" << zEndl;

#endif

	// Iterate over all commands calling their put() function to serialise them.
	// We cannot use for_each here as we have to pass in an output stream, and
	// the bind2nd(mem_fun1()) does not seem to allow it.

	for(CommandIterator iterCom=m_Commands.begin(); iterCom!=m_Commands.end(); iterCom++)
	{
		(*iterCom)->put(m_outStream);
	}


#if EnableXMLCommands == SimXMLEnabled
	m_outStream << "</Commands>" << zEndl;
#endif

	// If there is an error closing the file, the calling routine will be informed

	return Close();
}
