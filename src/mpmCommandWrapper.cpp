/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpmCommandWrapper.cpp: implementation of the mpmCommandWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mpmCommandWrapper.h"
#include "xxCommand.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// This class serves only to hold a pointer to a concrete command instance
// for its derived classes. We do not delete the command instance here as
// it will be owned by the CAnalysisState object.

mpmCommandWrapper::mpmCommandWrapper() : mpmMessage(), m_SimTime(0), m_pCommand(0), m_bDeleteCommand(true)
{
}

mpmCommandWrapper::mpmCommandWrapper(const mpmCommandWrapper& oldMessage) : mpmMessage(oldMessage),
                                         m_SimTime(oldMessage.m_SimTime),
                                         m_pCommand(oldMessage.m_pCommand),
                                         m_bDeleteCommand(oldMessage.m_bDeleteCommand)
{
}

mpmCommandWrapper::~mpmCommandWrapper()
{
    // We delete the command instance here if the flag shows that it has not been
    // extracted; if it has it is owned by CAnalysisState.

    if(m_bDeleteCommand && m_pCommand)
    {
        delete m_pCommand;
        m_pCommand = 0;
    }
}

// Function used by derived classes to set the simulation time at which
// the command whose data are wrapped in this message is to be executed.
// It is set by the ISimState instance before the message has its Receive()
// function called.

void mpmCommandWrapper::SetExecutionTime(long simTime)
{
    m_SimTime = simTime;
}

// Function used by derived classes to return the concrete command instance
// that has been instantiated from the data in the wrapper message class.
// When this function is called, we set a flag showing that this class no longer
// owns the command instance and should not therefore attempt to delete it.
// If the function is not called, we delete the command instance in this class' destructor.

xxCommand* mpmCommandWrapper::GetCommand()
{
    m_bDeleteCommand = false;
    return m_pCommand;
}
