/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ctSelectBeadsInSphericalTargetRegion.cpp: implementation of the ctSelectBeadsInSphericalTargetRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ctSelectBeadsInSphericalTargetRegion.h"
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

const zString ctSelectBeadsInSphericalTargetRegion::m_Type = "SelectBeadsInSphericalTargetRegion";

const zString ctSelectBeadsInSphericalTargetRegion::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ctSelectBeadsInSphericalTargetRegion::m_ArgumentTotal = 9;

long ctSelectBeadsInSphericalTargetRegion::GetArgumentTotal()
{
	return m_ArgumentTotal;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string for the command is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function of the
// command class but a global function hidden in the namespace.

namespace
{
	xxCommand* Create(long executionTime) {return new ctSelectBeadsInSphericalTargetRegion(executionTime);}

	const zString id = ctSelectBeadsInSphericalTargetRegion::GetType();
    const long  argTotal = ctSelectBeadsInSphericalTargetRegion::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ctSelectBeadsInSphericalTargetRegion::ctSelectBeadsInSphericalTargetRegion(long executionTime) : xxCommand(executionTime),
									m_SourceLabel(""), m_NewLabel(""), m_BeadName(""),
                                    m_Radius1(0.0), m_Radius2(0.0),
                                    m_Theta1(0.0), m_Theta2(0.0),
                                    m_Phi1(0.0), m_Phi2(0.0)
{
}

// Constructor for use when creating the command internally. Note that
// the polar and azimuthal angles must be specified in radians in contrast
// to the default constructor/get() function where they are input in degrees 
// and converted to radians in the get() function. Also note that phi1 is 
// allowed to be negative to allow regions that are centred on the x axis.

ctSelectBeadsInSphericalTargetRegion::ctSelectBeadsInSphericalTargetRegion(long executionTime, 
                               const zString sourceLabel, const zString newLabel, const zString beadName,
                               double radius1, double radius2, 
                               double theta1, double theta2, 
                               double phi1, double phi2) : xxCommand(executionTime),
							   m_SourceLabel(sourceLabel), m_NewLabel(newLabel), m_BeadName(beadName),
                               m_Radius1(radius1), m_Radius2(radius2),
                               m_Theta1(theta1), m_Theta2(theta2),
                               m_Phi1(phi1), m_Phi2(phi2)
{
    if(m_SourceLabel.empty() || m_NewLabel.empty() || m_SourceLabel == m_NewLabel || 
       m_BeadName.empty())
	{
	   SetCommandValid(false);
	}
	else if(m_Radius1 < 1.0 || m_Radius2 < 1.0 || m_Radius1 > m_Radius2)
    {
	   SetCommandValid(false);
	}
    else if(m_Theta1 < 0.0 || m_Theta2 < 0.0 || m_Theta1 > m_Theta2 ||
		    m_Theta1 > xxBase::m_globalPI    || m_Theta2 > xxBase::m_globalPI )
	{
	   SetCommandValid(false);
	}
    else if(m_Phi2 < 0.0 || m_Phi1 > m_Phi2 ||
		    m_Phi1 > xxBase::m_globalTwoPI  || m_Phi2 > xxBase::m_globalTwoPI)
	{
	   SetCommandValid(false);
	}

}

ctSelectBeadsInSphericalTargetRegion::ctSelectBeadsInSphericalTargetRegion(const ctSelectBeadsInSphericalTargetRegion& oldCommand) : xxCommand(oldCommand),
									 m_SourceLabel(oldCommand.m_SourceLabel),
									 m_NewLabel(oldCommand.m_NewLabel),
                                     m_BeadName(oldCommand.m_BeadName),
									 m_Radius1(oldCommand.m_Radius1),
									 m_Radius2(oldCommand.m_Radius2),
									 m_Theta1(oldCommand.m_Theta1), 
									 m_Theta2(oldCommand.m_Theta2), 
                                     m_Phi1(oldCommand.m_Phi1),
                                     m_Phi2(oldCommand.m_Phi2)
{
}

ctSelectBeadsInSphericalTargetRegion::~ctSelectBeadsInSphericalTargetRegion()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_SourceLabel    Name of the target whose beads are being modified
//  m_NewLabel       Name for the new bead target
//  m_BeadName       String identifier for the selected beads
//  m_Radius1        Inner radius of bead region
//  m_Radius2        Outer radius of bead region
//  m_Theta1         Lower polar angle of region (0 <= m_Theta <= 180)
//  m_Theta2         Upper polar angle of region (0 <= m_Theta <= 180)
//  m_Phi1           Lower azimuthal angle   "   (0 <= m_Phi <= 360)
//  m_Phi2           Upper azimuthal angle   "   (0 <= m_Phi <= 360)


zOutStream& ctSelectBeadsInSphericalTargetRegion::put(zOutStream& os) const
{
	// We assume the angles are in radians and convert them to degrees before writing them

	const double theta1 = m_Theta1*180.0/xxBase::m_globalPI;
	const double theta2 = m_Theta2*180.0/xxBase::m_globalPI;
	const double phi1   =   m_Phi1*180.0/xxBase::m_globalPI;
	const double phi2   =   m_Phi2*180.0/xxBase::m_globalPI;

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	os << "<SourceTargetLabel>"   << m_SourceLabel << "</SourceTargetLabel>" << zEndl;
	os << "<BeadTargetLabel>"     << m_NewLabel    << "</BeadTargetLabel>" << zEndl;
	os << "<BeadName>"            << m_BeadName    << "</BeadName>" << zEndl;
	os << "<InnerRadius>"         << m_Radius1     << "</InnerRadius>" << zEndl;
	os << "<OuterRadius>"         << m_Radius2     << "</OuterRadius>" << zEndl;
	os << "<LowerPolarAngle>"     << theta1        << "</LowerPolarAngle>" << zEndl;
	os << "<UpperPolarAngle>"     << theta2        << "</UpperPolarAngle>" << zEndl;
	os << "<LowerAzimuthalAngle>" << phi1          << "</LowerAzimuthalAngle>" << zEndl;
	os << "<UpperAzimuthalAngle>" << phi2          << "</UpperAzimuthalAngle>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	os << " " << m_SourceLabel << " " << m_NewLabel << " " << m_BeadName;
	os << " " << m_Radius1 << " " << m_Radius2;
	os << " " << theta1 << " " << theta2;
	os << " " << phi1 << " " << phi2;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ctSelectBeadsInSphericalTargetRegion::get(zInStream& is)
{
	// We perform some simple checks on the data here, but more complicated
	// validation is done in IsDataValid() below as it has access to CInputData.

	is >> m_SourceLabel >> m_NewLabel >> m_BeadName;

	if(!is.good() || m_SourceLabel.empty() || m_NewLabel.empty() || m_BeadName.empty())
	   SetCommandValid(false);

	is >> m_Radius1 >> m_Radius2;

	// Note that the inner radius must be less than or equal to the outer radius

	if(!is.good() || m_Radius1 < 1.0 || m_Radius2 < 1.0 || 
        m_Radius1 > m_Radius2)
	{
	   SetCommandValid(false);
	}

	is >> m_Theta1 >> m_Theta2 >> m_Phi1 >> m_Phi2;

    // Note that we allow phi1 to be negative (but not phi2) so that we can define 
    // regions centred on the x axis. The case with phi2 < 0 should be entered with
    // both phi1 and phi2 positive.

	if(!is.good() || m_Theta1 < 0.0   || m_Theta2 < 0.0   || m_Theta1 > m_Theta2 || 
		             m_Theta1 > 180.0 || m_Theta2 > 180.0 || 
		             m_Phi2 < 0.0     || m_Phi1 > m_Phi2  ||
                     m_Phi1 > 360.0   || m_Phi2 > 360.0)
	{
	   SetCommandValid(false);
	}

	// Convert angles to radians

	m_Theta1 = m_Theta1*xxBase::m_globalPI/180.0;
	m_Theta2 = m_Theta2*xxBase::m_globalPI/180.0;
	m_Phi1   =   m_Phi1*xxBase::m_globalPI/180.0;
	m_Phi2   =   m_Phi2*xxBase::m_globalPI/180.0;

	return is;
}

// Non-static function to return the type of the command

const zString ctSelectBeadsInSphericalTargetRegion::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ctSelectBeadsInSphericalTargetRegion::GetCommand() const
{
	return new ctSelectBeadsInSphericalTargetRegion(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ctSelectBeadsInSphericalTargetRegion::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SelectBeadsInSphericalTargetRegion(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the spherical region from
// which to select beads is valid. Most of the data has been validated above,
// here we just check that the target names are valid.

bool ctSelectBeadsInSphericalTargetRegion::IsDataValid(const CInputData& riData) const
{
	CInputData& rincData = const_cast<CInputData&>(riData);

	if(!riData.IsExternalNameValid(m_SourceLabel))
		return ErrorTrace("Invalid source command target name");
    else if(!riData.IsExternalNameValid(m_NewLabel))
		return ErrorTrace("Invalid bead target name");
    else if(!riData.IsExternalNameValid(m_BeadName))
		return ErrorTrace("Invalid bead type name");

	// New bead target is unique so add it to the container in the CInputData

	rincData.AddCommandTargetName(m_NewLabel);

    return true;
}

// VF that allows an empty command instance to fill up its parameters after creation.
// It is typically used by the command groups to allow the command's arguments to
// be set when each of the group's commands is instantiated. The function returns
// true if all the command's parameters are correctly assigned values.
//
// The argument contains instances of types derived from tguArgumentBase that must
// be in the order expected by the command as it tries to cast them to the types
// it expects.
//
// This command expects the following types:
//
// m_SourceLabel        - zString
// m_NewLabel           - zString
// m_BeadName           - zString
// m_Radius1, m_Radius2 - double, double
// m_Theta1, m_Theta2   - double, double (assumed to be in degrees)
// m_Phi1, m_Phi2       - double, double (assumed to be in degrees)

bool ctSelectBeadsInSphericalTargetRegion::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(0)->GetValue(&m_SourceLabel);
        vArguments.at(1)->GetValue(&m_NewLabel);
        vArguments.at(2)->GetValue(&m_BeadName);
        vArguments.at(3)->GetValue(&m_Radius1);
        vArguments.at(4)->GetValue(&m_Radius2);
        vArguments.at(5)->GetValue(&m_Theta1);
        vArguments.at(6)->GetValue(&m_Theta2);
        vArguments.at(7)->GetValue(&m_Phi1);
        vArguments.at(8)->GetValue(&m_Phi2);

        // Validate the arguments: see the get() and IsDataValid() functions for details

        if(m_SourceLabel.empty()        || m_NewLabel.empty()  || 
           m_SourceLabel == m_NewLabel  || m_BeadName.empty()  || 
           m_Radius1 < 1.0              || m_Radius2 < 1.0     || 
           m_Radius1 > m_Radius2        || 
           m_Theta1 < 0.0    || m_Theta2 < 0.0   || m_Theta1 > m_Theta2 ||
		   m_Theta1 > 180.0  || m_Theta2 > 180.0 ||
           m_Phi2 < 0.0      || m_Phi1 > m_Phi2  || 
           m_Phi1 > 360.0    || m_Phi2 > 360.0)
        {
            return false;
        }

        // Convert angles to radians

	    m_Theta1 = m_Theta1*xxBase::m_globalPI/180.0;
	    m_Theta2 = m_Theta2*xxBase::m_globalPI/180.0;
	    m_Phi1   =   m_Phi1*xxBase::m_globalPI/180.0;
	    m_Phi2   =   m_Phi2*xxBase::m_globalPI/180.0;

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
