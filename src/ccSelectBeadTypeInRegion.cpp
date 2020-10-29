/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectBeadTypeInRegion.cpp: implementation of the ccSelectBeadTypeInRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectBeadTypeInRegion.h"
#include "ISimCmd.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectBeadTypeInRegion::ccSelectBeadTypeInRegion(long executionTime) : xxCommand(executionTime),
									m_TargetLabel(""), m_BeadName("")
{
}

ccSelectBeadTypeInRegion::ccSelectBeadTypeInRegion(const ccSelectBeadTypeInRegion& oldCommand) : xxCommand(oldCommand),
									 m_TargetLabel(oldCommand.m_TargetLabel),
									 m_BeadName(oldCommand.m_BeadName)
{
}

// Constructor for use when creating the command internally.

ccSelectBeadTypeInRegion::ccSelectBeadTypeInRegion(long executionTime, const zString target, 
												   const zString bead) : xxCommand(executionTime),
									m_TargetLabel(target), m_BeadName(bead)
{
}


ccSelectBeadTypeInRegion::~ccSelectBeadTypeInRegion()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_TargetLabel	Command target name so that it can be referred to in other commands
//  m_Beadname		Name of bead type held in the command target
//

zOutStream& ccSelectBeadTypeInRegion::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output - it is the derived class' responsibility to add
	// the start and end tags and to call this function in between.

	os << "<Targetlabel>" << m_TargetLabel << "</Targetlabel>" << zEndl;
	os << "<BeadName>"    << m_BeadName    << "</BeadName>"    << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << " " << m_TargetLabel << " " << m_BeadName << " ";

#endif

	return os;
}

zInStream& ccSelectBeadTypeInRegion::get(zInStream& is)
{
	// We check that the target name is valid and that the
	// bead type exists in the IsDataValid() function below.

	is >> m_TargetLabel >> m_BeadName;

	if(!is.good())
	   SetCommandValid(false);

	return is;
}

// Function to check that the command target is unique. Because we add each 
// target's name to the CInputData container, we have to cast away the 
// const-ness to use it. We don't check that the bead type exists here
// because dynamic commands may modify the number of types and their names.
// We leave this check to the commands themselves.

bool ccSelectBeadTypeInRegion::IsDataValid(const CInputData& riData) const
{
	CInputData& rincData = const_cast<CInputData&>(riData);

	if(!riData.IsExternalNameValid(m_TargetLabel))
		return ErrorTrace("Invalid command target name");
	else if(riData.IsCommandTargetPresent(m_TargetLabel))
		return ErrorTrace("Command target already exists");
    else if(!riData.IsExternalNameValid(m_BeadName))
		return ErrorTrace("Invalid bead name");

	// New target is unique so add it to the container in the CInputData

	rincData.AddCommandTargetName(m_TargetLabel);

	return true;
}

// VF to pack the arguments that are common to all classes derived from this class.
//
// This command expects the following types:
//
// m_TargetLabel     - zString
// m_BeadName        - zString


bool ccSelectBeadTypeInRegion::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    // Note that we cannot check whether the number of arguments is valid,
    // as each derived class may have a different number of arguments. But 
    // we can check there are at least the two arguments that this class requires.

    if(vArguments.size() >= 2)
    {
        vArguments.at(0)->GetValue(&m_TargetLabel);
        vArguments.at(1)->GetValue(&m_BeadName);

        if(!CInputData::GlobalIsExternalNameValid(m_TargetLabel) || !CInputData::GlobalIsExternalNameValid(m_BeadName))
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;

#else
    return false;
#endif
}
