/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctApplyCommand.cpp: implementation of the ctApplyCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctApplyCommand.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Note that there is no static member "GetArgumentTotal()" for this class,
// as its function is provided by the derived classes. But this class'
// single argument must be included in the total returned by any derived class.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctApplyCommand::ctApplyCommand(long executionTime) : xxCommand(executionTime),
													 m_TargetLabel("")
{

}

ctApplyCommand::ctApplyCommand(const ctApplyCommand& oldCommand) : xxCommand(oldCommand),
								m_TargetLabel(oldCommand.m_TargetLabel)
{
}

ctApplyCommand::ctApplyCommand(long executionTime, const zString target) : xxCommand(executionTime),
																		   m_TargetLabel(target)
{

}


ctApplyCommand::~ctApplyCommand()
{

}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_TargetLabel	Identifier specified in the creation of a 
//					ctApplyCommand-derived class

zOutStream& ctApplyCommand::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output - it is the derived class' responsibility to add
	// the start and end tags and to call this function in between.

	os << "<Targetlabel>" << m_TargetLabel << "</Targetlabel>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << m_TargetLabel << " ";

#endif

	return os;
}

zInStream& ctApplyCommand::get(zInStream& is)
{
	// Read data common to all command target commands.

	is >> m_TargetLabel;

	if(!is.good())
		SetCommandValid(false);

	return is;
}

// Function to check that the command target data is valid. This only consists
// in checking that the target has a valid name. We don't check that the target
// actually exists as restart runs may refer to targets created in the original
// run that have not bee instantiated yet. The implementation classes for all 
// commands that manipulate targets are responsible for verifying that the targets
// exist.

bool ctApplyCommand::IsDataValid(const CInputData &riData) const
{
	if(!riData.IsExternalNameValid(m_TargetLabel))
		return ErrorTrace("Invalid command target name");
//	else if( !riData.IsCommandTargetPresent(m_TargetLabel) )
//		return ErrorTrace("Command target does not exist");

	return true;
}

// VF to pack the arguments that are common to all classes derived from this class.
//
// This command expects the following type:
//
// m_TargetLabel - zString


bool ctApplyCommand::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(!vArguments.empty())
    {
        vArguments.at(0)->GetValue(&m_TargetLabel);

        if(!CInputData::GlobalIsExternalNameValid(m_TargetLabel))
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }

#else
    return false;
#endif
}
