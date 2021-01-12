/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// acCreateBondBindsForwardConeToPolymerHeadEventSource.cpp: implementation of the acCreateBondBindsForwardConeToPolymerHeadEventSource class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "acCreateBondBindsForwardConeToPolymerHeadEventSource.h"
#include "ISimCmd.h"
#include "ISimBox.h"
#include "InputData.h"

#include "aeActiveSimBox.h"
#include "IModifyActiveCellNetwork.h"
#include "CommandTargetACNFreePolymers.h"
#include "LogacACNCreateEventSource.h"
#include "LogacNetworkNotModifiable.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString acCreateBondBindsForwardConeToPolymerHeadEventSource::m_Type = "CreateBondBindsForwardConeToPolymerHeadEventSource";

const zString acCreateBondBindsForwardConeToPolymerHeadEventSource::GetType()
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
	xxCommand* Create(long executionTime) {return new acCreateBondBindsForwardConeToPolymerHeadEventSource(executionTime);}

	const zString id = acCreateBondBindsForwardConeToPolymerHeadEventSource::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

acCreateBondBindsForwardConeToPolymerHeadEventSource::acCreateBondBindsForwardConeToPolymerHeadEventSource(long executionTime) : acACNCreateEventSource(executionTime)
{
}

acCreateBondBindsForwardConeToPolymerHeadEventSource::acCreateBondBindsForwardConeToPolymerHeadEventSource(const acCreateBondBindsForwardConeToPolymerHeadEventSource& oldCommand) : acACNCreateEventSource(oldCommand)

{
}


acCreateBondBindsForwardConeToPolymerHeadEventSource::~acCreateBondBindsForwardConeToPolymerHeadEventSource()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//

zOutStream& acCreateBondBindsForwardConeToPolymerHeadEventSource::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	acACNCreateEventSource::put(os);
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	acACNCreateEventSource::put(os);
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& acCreateBondBindsForwardConeToPolymerHeadEventSource::get(zInStream& is)
{
	// Only the base class reads data, the type of this command defines
    // the event source decorator to be created

	return acACNCreateEventSource::get(is);
}

// Non-static function to return the type of the command

const zString acCreateBondBindsForwardConeToPolymerHeadEventSource::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* acCreateBondBindsForwardConeToPolymerHeadEventSource::GetCommand() const
{
	return new acCreateBondBindsForwardConeToPolymerHeadEventSource(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 
// Even though we expect the event source decorators to be used only for analysis, 
// and so not to modify the evolution of the ACN, we still allow this for future
// expansion. If an ACN is marked as not modifiable, the event source cannot be 
// created.
//
// The sequence of steps to create the event source is as follows:
// 1 Get the command target instance that wraps the specified ACN. If it does not
//   exist we issue an error message.
// 2 Create an event source decorator that wraps the command target, and is 
//   updated by the ACN when its events succeed.
// 3 
//

bool acCreateBondBindsForwardConeToPolymerHeadEventSource::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		IActiveSimBox* pIASB = pISimCmd->GetISimBox()->IIActiveSimBox();

        const CSimBox* const pSimBox = pISimCmd->GetISimBox()->GetSimBox();

        // Get a pointer to the target's contained ACN. We know that the target has
        // the type CCommandTargetFreePolymers and use its GetACN() function.


        const zString acnTargetId = GetACNTargetId();
        CCommandTargetACNFreePolymers* pACNTarget = dynamic_cast<CCommandTargetACNFreePolymers*>(pSimBox->GetCommandTarget(acnTargetId));
        const zString acnType = pACNTarget->GetACNType();

		IModifyActiveCellNetwork* const pMACN = pIASB->GetIModifyActiveCellNetworkFromType(acnType);

		if(pMACN)
		{
            // Call the SimBox's implementation of this command to create a
            // taBondBindsToPolymer instance. 
            // Execution of the command is logged by the command's implementation
            // class.

            pISimCmd->CreateBondBindsForwardConeToPolymerHeadEventSource(this);
		}
		else
		{
			 new CLogacNetworkNotModifiable(GetExecutionTime(), acnType);
		}

		return true;
	}
	else
		return false;
}

// Function to check that the data defining the command is valid.
//
// Only the base class reads any data, so we just return its validation state.

bool acCreateBondBindsForwardConeToPolymerHeadEventSource::IsDataValid(const CInputData& riData) const
{
    return acACNCreateEventSource::IsDataValid(riData);
}
