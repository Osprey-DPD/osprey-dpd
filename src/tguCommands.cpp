/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// tguCommands.cpp: implementation of the tguCommands class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "tguCommands.h"
#include "ISimState.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Note that this class does not own the commands it stores, it simply acts 
// as a wrapper so that they can be passed to the ISimState for execution. They
// are owned by the CAnalysisState instance as are all executable commands.

tguCommands::tguCommands()
{
    m_Commands.clear();
}

tguCommands::~tguCommands()
{

}

// Function to add a command to the local store. Commands are added to the back
// of the container, so that are in the same order in the container as the user
// enters them in the CDF.

void tguCommands::AddCommand(const xxCommand* const pCommand)
{
    if(pCommand)
    {
        m_Commands.push_back(pCommand);
    }
}

// Function to pass all the stored commands to the ISimState for execution after
// all commands already in the ISimState's queue that have the same execution time.
// Note that because of the way the AddCommandAfterFront() function works, we iterate
// over the commands from the front and each command is inserted after all the 
// previous ones. This is different frmo the ExecuteCommandsImmediately() function.

void tguCommands::ExecuteCommands(ISimState* const pISimState)
{
    for(CommandIterator iterCmd=m_Commands.begin(); iterCmd!=m_Commands.end(); iterCmd++)
    {
        pISimState->AddCommandAfterFront(*iterCmd);
    }
}

// Function to pass all the stored commands to the ISimState for immediate execution.
// This function must not be used directly from a currently-executing command as it
// will result in an infinite loop. It should be used only when scheduling commands
// for execution from an active command group.

void tguCommands::ExecuteCommandsImmediately(ISimState* const pISimState)
{
    for(rCommandIterator riterCmd=m_Commands.rbegin(); riterCmd!=m_Commands.rend(); ++riterCmd)
    {
        pISimState->AddCommandToFront(*riterCmd);
    }
}
