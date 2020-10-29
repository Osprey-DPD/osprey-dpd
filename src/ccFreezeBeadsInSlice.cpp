/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccFreezeBeadsInSlice.cpp: implementation of the ccFreezeBeadsInSlice class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccFreezeBeadsInSlice.h"
#include "ISimCmd.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString ccFreezeBeadsInSlice::m_Type = "FreezeBeadsInSlice";

const zString ccFreezeBeadsInSlice::GetType()
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
	xxCommand* Create(long executionTime) {return new ccFreezeBeadsInSlice(executionTime);}

	const zString id = ccFreezeBeadsInSlice::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccFreezeBeadsInSlice::ccFreezeBeadsInSlice(long executionTime) : xxCommand(executionTime),
										    m_XN(0), m_YN(0), m_ZN(0),
											m_LowerBound(0.0), m_Thickness(0.0)
{
}

ccFreezeBeadsInSlice::ccFreezeBeadsInSlice(const ccFreezeBeadsInSlice& oldCommand) : xxCommand(oldCommand),
										    m_XN(oldCommand.m_XN), 
											m_YN(oldCommand.m_YN), 
											m_ZN(oldCommand.m_ZN),
											m_LowerBound(oldCommand.m_LowerBound),
											m_Thickness(oldCommand.m_Thickness)

{
}

ccFreezeBeadsInSlice::~ccFreezeBeadsInSlice()
{

}

// Member functions to write/read the data specific to the command.
// Note that spaces are written here not in the xxCommand function.
//
// Arguments
// *********
//
//  m_XN, m_YN, m_ZN	Normal to slice
//	m_LowerBound		Lower edge of slice (in units of bead diameter)
//	m_Thickness			Thickness of slice (in units of bead diameter)


zOutStream& ccFreezeBeadsInSlice::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<XN>" << m_XN << "</XN>" << zEndl;
	os << "<YN>" << m_YN << "</YN>" << zEndl;
	os << "<ZN>" << m_ZN << "</ZN>" << zEndl;
	os << "<LowerBound>" << m_LowerBound << "</LowerBound>" << zEndl;
	os << "<Thickness>"  << m_Thickness  << "</Thickness>"  << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << m_XN << " " << m_YN << " " << m_ZN << " " << m_LowerBound << " " << m_Thickness;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccFreezeBeadsInSlice::get(zInStream& is)
{
	is >> m_XN >> m_YN >> m_ZN >> m_LowerBound >> m_Thickness;

	if(!is.good())
		SetCommandValid(false);

	return is;
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands.
//
// This command defines a planar slice of CNT cells and freezes all beads within 
// the cells.

bool ccFreezeBeadsInSlice::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->FreezeBeadsInSlice(this);
		return true;
	}
	else
		return false;
}

const zString ccFreezeBeadsInSlice::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccFreezeBeadsInSlice::GetCommand() const
{
	return new ccFreezeBeadsInSlice(*this);
}


// Function to check that the data required to define a planar slice of 
// CNT cells is valid. If the slice thickness is not an integer multiple of
// the CNT cells width (in the normal direction), the slice will be extended
// to include all beads in all CNT cells intersected by the slice volume.
//
// The slice thickness and lower bound must be positive semi-definite, and the
// slice edges must lie on or within the SimBox boundaries. The normal to the
// slice must lie along one of the major SimBox axes.


bool ccFreezeBeadsInSlice::IsDataValid(const CInputData &riData) const
{
	if( !(m_XN == 1 && m_YN == 0 && m_ZN == 0) &&
		!(m_XN == 0 && m_YN == 1 && m_ZN == 0) &&
		!(m_XN == 0 && m_YN == 0 && m_ZN == 1) )
		return ErrorTrace("Slice normal invalid");

	if( m_LowerBound < 0.0 || m_Thickness < 0.0 )
		return ErrorTrace("Slice thickness or lower bound negative");

	return true;
}

