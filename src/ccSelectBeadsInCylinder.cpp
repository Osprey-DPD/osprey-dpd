/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectBeadsInCylinder.cpp: implementation of the ccSelectBeadsInCylinder class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectBeadsInCylinder.h"
#include "ISimCmd.h"
#include "InputData.h"

#include "SimMathFlags.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString ccSelectBeadsInCylinder::m_Type = "SelectBeadsInCylinder";

const zString ccSelectBeadsInCylinder::GetType()
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
	xxCommand* Create(long executionTime) {return new ccSelectBeadsInCylinder(executionTime);}

	const zString id = ccSelectBeadsInCylinder::GetType();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Global functions
//////////////////////////////////////////////////////////////////////

// We can only use an explicit equality check on integers, so we check the
// radii using a small maximum allowed error.

bool operator==(const ccSelectBeadsInCylinder& a, const ccSelectBeadsInCylinder& b)
{
	if(dynamic_cast<const ccSelection&>(a) == dynamic_cast<const ccSelection&>(b) &&
		 a.m_LowerEdge == b.m_LowerEdge &&
		 a.m_UpperEdge == b.m_UpperEdge &&
		fabs(a.m_Centre1 - b.m_Centre1) < 0.0001 &&
		fabs(a.m_Centre2 - b.m_Centre2) < 0.0001 &&
		fabs(a.m_InnerRadius - b.m_InnerRadius) < 0.0001 &&
		fabs(a.m_OuterRadius - b.m_OuterRadius) < 0.0001)
		return true;
	else
		return false;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectBeadsInCylinder::ccSelectBeadsInCylinder(long executionTime) : ccSelection(executionTime),
												m_LowerEdge(0), m_UpperEdge(0),
												m_Centre1(0.5), m_Centre2(0.5),
												m_InnerRadius(0.0), m_OuterRadius(0.0)
{

}

ccSelectBeadsInCylinder::ccSelectBeadsInCylinder(const ccSelectBeadsInCylinder& oldCommand) : ccSelection(oldCommand),
																					   m_LowerEdge(oldCommand.m_LowerEdge),
																					   m_UpperEdge(oldCommand.m_UpperEdge),
																					     m_Centre1(oldCommand.m_Centre1),
																					     m_Centre2(oldCommand.m_Centre2),
																					 m_InnerRadius(oldCommand.m_InnerRadius),
																					 m_OuterRadius(oldCommand.m_OuterRadius)
{
}

ccSelectBeadsInCylinder::~ccSelectBeadsInCylinder()
{

}

// Member functions to write/read the data specific to the command.
// Note that spaces are written here not in the ccSelection or xxCommand
// classes. Also note that we have to pass the command to put or get up 
// to the ccSelection base class so that its data can be read/written 
// before we get the data for this class.
//
// Arguments
// *********
//
//	m_LowerEdge		Cylinder bottom as a CNTCell slice index
//	m_UpperEdge		Cylinder top as a CNTCell slice index inclusive of top slice
//	m_Centre1		Cylinder central axis as a fraction of SimBox size
//	m_Centre2		Cylinder central axis         "
//	m_InnerRadius	Cylinder inner radius in units of bead diameter
//	m_OuterRadius	Cylinder outer radius in units of bead diameter
//

zOutStream& ccSelectBeadsInCylinder::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output - write the start tags first then write the base class
	// data before writing data in this class

	putXMLStartTags(os);
	ccSelection::put(os);
	os << "<LowerEdge>"   << m_LowerEdge   << "</LowerEdge>"   << zEndl;
	os << "<UpperEdge>"   << m_UpperEdge   << "</UpperEdge>"   << zEndl;
	os << "<Centre1>"     << m_Centre1     << "</Centre1>"     << zEndl;
	os << "<Centre2>"     << m_Centre2     << "</Centre2>"     << zEndl;
	os << "<InnerRadius>" << m_InnerRadius << "</InnerRadius>" << zEndl;
	os << "<OuterRadius>" << m_OuterRadius << "</OuterRadius>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ccSelection::put(os);
	os << " " << m_LowerEdge   << " " << m_UpperEdge;
	os << " " << m_Centre1     << " " << m_Centre2;
	os << " " << m_InnerRadius << " " << m_OuterRadius;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSelectBeadsInCylinder::get(zInStream& is)
{
	// Read the base class data first

	ccSelection::get(is);

	is >> m_LowerEdge >> m_UpperEdge >> m_Centre1 >> m_Centre2 >> m_InnerRadius >> m_OuterRadius;

	if(!is.good() || m_LowerEdge < 0 || m_UpperEdge < m_LowerEdge ||
					 m_Centre1 < 0.0 || m_Centre1 > 1.0 ||
					 m_Centre2 < 0.0 || m_Centre2 > 1.0 ||
			     m_InnerRadius < 0.0 || m_OuterRadius < m_InnerRadius)
	   SetCommandValid(false);


	return is;
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands.


bool ccSelectBeadsInCylinder::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SelectBeadsInCylinder(this);
		return true;
	}
	else
		return false;
}

const zString ccSelectBeadsInCylinder::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSelectBeadsInCylinder::GetCommand() const
{
	return new ccSelectBeadsInCylinder(*this);
}

// Function to check that the data required by the command is valid. 
// We first check the generic data by a call to the base class's
// IsDataValid() function and then perform the local checks.
// 
// We have already checked that the cylinder top and bottom are greater 
// than zero and that the inner and outer radii have valid values. 
// The bead names have been checked in the ccSelection base class.
//
// Here we check that the cylinder top lies within the SimBox and that the
// specification of the central axis and the cylinder outer radius does
// not lead to the cylinder overlapping the SimBox boundaries.
// Note that we allow the cylinder to overlap the walls (if present) so that
// beads of the specified types can be collected even if they have penetrated
// the (soft) walls somewhat.
// We rely on the fact that the cylinder normal is one of the major axes,
// so we can just compare the number of CNT slices in the normal direction
// with the cylinder upper edge index. The cylinder includes the upper
// edge slice so that the user can select a cylinder spanning the whole
// SimBox if desired.  Note that this specification of the cylinder normal
// is not generalisable to arbitrary directions.

bool ccSelectBeadsInCylinder::IsDataValid(const CInputData &riData) const
{

	if(ccSelection::IsDataValid(riData))
	{
		if(GetX() == 1)
		{
			if(m_UpperEdge >= riData.GetCNTXCellNo() )
			{
				return ErrorTrace("Selection command X cylinder top edge outside SimBox");
			}

			const double centre1 = m_Centre1*riData.GetSimBoxYLength();
			const double centre2 = m_Centre2*riData.GetSimBoxZLength();

			if( (centre1 + m_OuterRadius) > riData.GetSimBoxYLength() ||
				(centre1 - m_OuterRadius) < 0.0 ||
				(centre2 + m_OuterRadius) > riData.GetSimBoxZLength() ||
				(centre2 - m_OuterRadius) < 0.0 )
				return ErrorTrace("Selection command X cylinder outside SimBox");
		}
		else if(GetY() == 1)
		{
			if(m_UpperEdge >= riData.GetCNTYCellNo() )
			{
				return ErrorTrace("Selection command Y cylinder top edge outside SimBox");
			}

			const double centre1 = m_Centre1*riData.GetSimBoxZLength();
			const double centre2 = m_Centre2*riData.GetSimBoxXLength();

			if( (centre1 + m_OuterRadius) > riData.GetSimBoxZLength() ||
				(centre1 - m_OuterRadius) < 0.0 ||
				(centre2 + m_OuterRadius) > riData.GetSimBoxXLength() ||
				(centre2 - m_OuterRadius) < 0.0 )
				return ErrorTrace("Selection command Y cylinder outside SimBox");
		}
		else if(GetZ() == 1)
		{
			if(m_UpperEdge >= riData.GetCNTZCellNo() )
			{
				return ErrorTrace("Selection command Z cylinder top edge outside SimBox");
			}

			const double centre1 = m_Centre1*riData.GetSimBoxXLength();
			const double centre2 = m_Centre2*riData.GetSimBoxYLength();

			if( (centre1 + m_OuterRadius) > riData.GetSimBoxXLength() ||
				(centre1 - m_OuterRadius) < 0.0 ||
				(centre2 + m_OuterRadius) > riData.GetSimBoxYLength() ||
				(centre2 - m_OuterRadius) < 0.0 )
				return ErrorTrace("Selection command Z cylinder outside SimBox");
		}
	}
	else
		return false;

	return true;
}
