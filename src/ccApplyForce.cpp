/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccApplyForce.cpp: implementation of the ccApplyForce class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccApplyForce.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccApplyForce::ccApplyForce(long executionTime) : xxCommand(executionTime),
												m_TargetLabel(""),
												m_Duration(0),
												m_X(0), m_Y(0), m_Z(0)
{

}

ccApplyForce::ccApplyForce(const ccApplyForce& oldCommand) : xxCommand(oldCommand),
															 m_TargetLabel(oldCommand.m_TargetLabel),
															 m_Duration(oldCommand.m_Duration),
															 m_X(oldCommand.m_X),
															 m_Y(oldCommand.m_Y),
															 m_Z(oldCommand.m_Z)
{
}


ccApplyForce::~ccApplyForce()
{

}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_TargetLabel	Identifier specified in the creation of a ccSelection-derived class
//	m_Duration		Time during which force acts on the target
//  m_X, m_Y, m_Z   Direction of force

zOutStream& ccApplyForce::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output - start and end tags provided by derived classes
	os << "<TargetLabel>" << m_TargetLabel << "</TargetLabel>" << zEndl;
	os << "<Duration>"	  << m_Duration	   << "</Duration>"	   << zEndl;
	os << "<XN>"		  << m_X		   << "</XN>"		   << zEndl;
	os << "<YN>"		  << m_Y		   << "</YN>"		   << zEndl;
	os << "<ZN>"		  << m_Z		   << "</ZN>"		   << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << m_TargetLabel << " " << m_Duration << " " << m_X << " " << m_Y << " " << m_Z << " ";

#endif

	return os;
}

zInStream& ccApplyForce::get(zInStream& is)
{
	// Read data common to all force commands: we allow a duration of zero but not
	// a direction with all zero components, although negative components are allowed
	// so that the force can point in any direction in the SimBox.

	is >> m_TargetLabel >> m_Duration >> m_X >> m_Y >> m_Z;

	if(!is.good() || m_Duration < 0 || (m_X == 0 && m_Y == 0 && m_Z == 0))
		SetCommandValid(false);

	return is;
}

// Function to check that the command data is valid.  Because the duration and
// direction of the force have been checked in the get() function above, we
// only check that the force target exists.

bool ccApplyForce::IsDataValid(const CInputData &riData) const
{
	// Check that target name is unique and consists only of allowed characters.

	if( !riData.IsExternalNameValid(GetLabel()) )
		return ErrorTrace("Invalid Add force target name");
	else if( !riData.IsForceTargetPresent(GetLabel()) )
		return ErrorTrace("Add force target does not exist");

	return true;
}

