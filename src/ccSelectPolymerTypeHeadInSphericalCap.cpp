/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccSelectPolymerTypeHeadInSphericalCap.cpp: implementation of the ccSelectPolymerTypeHeadInSphericalCap class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccSelectPolymerTypeHeadInSphericalCap.h"
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

const zString ccSelectPolymerTypeHeadInSphericalCap::m_Type = "SelectPolymerTypeHeadInSphericalCap";

const zString ccSelectPolymerTypeHeadInSphericalCap::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ccSelectPolymerTypeHeadInSphericalCap::m_ArgumentTotal = 10;

long ccSelectPolymerTypeHeadInSphericalCap::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ccSelectPolymerTypeHeadInSphericalCap(executionTime);}

	const zString id = ccSelectPolymerTypeHeadInSphericalCap::GetType();
    const long  argTotal = ccSelectPolymerTypeHeadInSphericalCap::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccSelectPolymerTypeHeadInSphericalCap::ccSelectPolymerTypeHeadInSphericalCap(long executionTime) : ccSelectPolymerTypeInRegion(executionTime),
									m_XC(0.0), m_YC(0.0), m_ZC(0.0), m_InnerRadius(0.0), m_OuterRadius(0.0),
									m_Theta(0.0), m_Phi(0.0), m_Gamma(0.0)									
{
}

// Constructor for use when creating the command internally. We have to perform
// validation on the data explicitly. If an illegal value for a data item
// is found, the command has its valid flag set false in the base class.
// It is up to the calling routine to check that the command is valid.

ccSelectPolymerTypeHeadInSphericalCap::ccSelectPolymerTypeHeadInSphericalCap(long executionTime, 
							   const zString target, const zString poly, 
							   double xc, double yc, double zc, double inRadius, double outRadius,
							   double theta, double phi, double gamma) : ccSelectPolymerTypeInRegion(executionTime, target, poly),
								m_XC(xc), m_YC(yc), m_ZC(zc),
								m_InnerRadius(inRadius), m_OuterRadius(outRadius),
								m_Theta(theta), m_Phi(phi), m_Gamma(gamma)
{
    if(m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
	   m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
	{
	   SetCommandValid(false);
	}
	else if(m_OuterRadius < 0.0 || m_InnerRadius < 0.0 || m_InnerRadius > m_OuterRadius)
	{
	   SetCommandValid(false);
	}
    else if(m_Theta < 0.0 || m_Phi < 0.0 || m_Gamma < 0.0 || 
		    m_Theta > xxBase::m_globalPI || m_Phi > xxBase::m_globalTwoPI || m_Gamma > xxBase::m_globalPIByTwo)
	{
	   SetCommandValid(false);
	}
}

ccSelectPolymerTypeHeadInSphericalCap::ccSelectPolymerTypeHeadInSphericalCap(const ccSelectPolymerTypeHeadInSphericalCap& oldCommand) : ccSelectPolymerTypeInRegion(oldCommand),
									 m_XC(oldCommand.m_XC),
									 m_YC(oldCommand.m_YC),
									 m_ZC(oldCommand.m_ZC),
									 m_InnerRadius(oldCommand.m_InnerRadius),
									 m_OuterRadius(oldCommand.m_OuterRadius),
									 m_Theta(oldCommand.m_Theta),
									 m_Phi(oldCommand.m_Phi),
									 m_Gamma(oldCommand.m_Gamma)
{
}


ccSelectPolymerTypeHeadInSphericalCap::~ccSelectPolymerTypeHeadInSphericalCap()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	m_XC
//	m_YC		    Sphere centre as fractions of SimBox side lengths
//  m_ZC
//	m_InnerRadius	Inner radius of sphere in units of bead diameter
//	m_OuterRadius	Outer radius of sphere in units of bead diameter
//  m_Theta         Polar angle that defines the normal direction to the spherical cap (0 <= m_Theta <= 180)
//  m_Phi           Azimuthal angle that defines the normal direction to the spherical cap  (0 <= m_Phi <= 360)
//  m_Gamma         Half angle that defines the size of the spherical cap  (0 <= m_Gamma <= 90)

zOutStream& ccSelectPolymerTypeHeadInSphericalCap::put(zOutStream& os) const
{
	// We assume the angles are in radians and convert them to degrees before writing them

	const double theta  =   m_Theta*180.0/xxBase::m_globalPI;
	const double phi    =     m_Phi*180.0/xxBase::m_globalPI;
	const double gamma  =   m_Gamma*180.0/xxBase::m_globalPI;
	
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	ccSelectPolymerTypeInRegion::put(os);
	os << "<XCentre>"		 << m_XC	 << "</XCentre>"	 << zEndl;
	os << "<YCentre>"		 << m_YC	 << "</YCentre>"	 << zEndl;
	os << "<ZCentre>"		 << m_ZC	 << "</ZCentre>"	 << zEndl;
	os << "<InnerRadius>"	 << m_InnerRadius << "</InnerRadius>" << zEndl;
	os << "<OuterRadius>"	 << m_OuterRadius << "</OuterRadius>" << zEndl;
	os << "<PolarAngle>"     << theta << "</PolarAngle>" << zEndl;
	os << "<AzimuthalAngle>" << phi << "</AzimuthalAngle>" << zEndl;
	os << "<HalfAngle>"      << gamma << "</HalfAngle>" << zEndl;
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	ccSelectPolymerTypeInRegion::put(os);
	os << " " << m_XC  << " " << m_YC  << " " << m_ZC;
	os << " " << m_InnerRadius << " " << m_OuterRadius << " " << theta << " " << phi << " " << gamma;
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccSelectPolymerTypeHeadInSphericalCap::get(zInStream& is)
{
	// Read the base class data first

	ccSelectPolymerTypeInRegion::get(is);

	// We perform some simple checks on the data here, but more complicated
	// validation is done in IsDataValid() below as it has access to CInputData.

	is >> m_XC >> m_YC >> m_ZC;

	if(!is.good() || m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
					 m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
	   SetCommandValid(false);

	is >> m_InnerRadius >> m_OuterRadius;

	if(!is.good() || m_InnerRadius < 0.0 || m_OuterRadius < 0.0 || 
					 m_InnerRadius > m_OuterRadius)
	   SetCommandValid(false);

	is >> m_Theta >> m_Phi >> m_Gamma;

	if(!is.good() || m_Theta < 0.0   || m_Phi < 0.0    || m_Gamma < 0.0 ||
					 m_Theta > 180.0 || m_Phi >  360.0 || m_Gamma > 90.0)
	   SetCommandValid(false);

	// Convert angles to radians

	m_Theta = m_Theta*xxBase::m_globalPI/180.0;
	m_Phi   = m_Phi*xxBase::m_globalPI/180.0;
	m_Gamma = m_Gamma*xxBase::m_globalPI/180.0;

	return is;
}

// Non-static function to return the type of the command

const zString ccSelectPolymerTypeHeadInSphericalCap::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccSelectPolymerTypeHeadInSphericalCap::GetCommand() const
{
	return new ccSelectPolymerTypeHeadInSphericalCap(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccSelectPolymerTypeHeadInSphericalCap::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->SelectPolymerTypeHeadInSphericalCap(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the sphereical region from
// which to select beads is valid. The target and and bead type are 
// validated in the base class and this is the ony check we need.

bool ccSelectPolymerTypeHeadInSphericalCap::IsDataValid(const CInputData& riData) const
{
	return ccSelectPolymerTypeInRegion::IsDataValid(riData);
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
// m_TargetLabel     - zString
// m_PolymerName     - zString
// m_XC, m_YC, m_ZC  - double, double, double
// m_InnerRadius     - double
// m_OuterRadius     - double
// m_Theta, m_Phi    - double (assumed to be in degrees)
// m_Gamma           - double (assumed to be in degrees)

bool ccSelectPolymerTypeHeadInSphericalCap::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    if(ccSelectPolymerTypeInRegion::Pack(vArguments) && static_cast<long>(vArguments.size()) == GetArgumentTotal())
    {
        vArguments.at(2)->GetValue(&m_XC);
        vArguments.at(3)->GetValue(&m_YC);
        vArguments.at(4)->GetValue(&m_ZC);
        vArguments.at(5)->GetValue(&m_InnerRadius);
        vArguments.at(6)->GetValue(&m_OuterRadius);
        vArguments.at(7)->GetValue(&m_Theta);
        vArguments.at(8)->GetValue(&m_Phi);
        vArguments.at(9)->GetValue(&m_Gamma);

        // Validate the arguments: see the get() and IsDataValid() functions for details

        if(m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
		   m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0 || 
	       m_OuterRadius < 0.0 || m_InnerRadius < 0.0 || m_InnerRadius > m_OuterRadius ||
		   m_Theta < 0.0   || m_Phi < 0.0   || m_Gamma < 0.0   || 
		   m_Theta > 180.0 || m_Phi > 360.0 || m_Gamma > 90.0)
        {
            return false;
        }

        // Convert angles to radians

	    m_Theta = m_Theta*xxBase::m_globalPI/180.0;
	    m_Phi   = m_Phi*xxBase::m_globalPI/180.0;
	    m_Gamma = m_Gamma*xxBase::m_globalPI/180.0;

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

