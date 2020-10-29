/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// ccCreateNanoparticlePentagon.cpp: implementation of the ccCreateNanoparticlePentagon class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ccCreateNanoparticlePentagon.h"
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

const zString ccCreateNanoparticlePentagon::m_Type = "CreateNanoparticlePentagon";

const zString ccCreateNanoparticlePentagon::GetType()
{
	return m_Type;
}

// Static member variable holding the number of user-defined arguments
// (not including the execution time) needed by this class.

long ccCreateNanoparticlePentagon::m_ArgumentTotal = 19;

long ccCreateNanoparticlePentagon::GetArgumentTotal()
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
	xxCommand* Create(long executionTime) {return new ccCreateNanoparticlePentagon(executionTime);}

	const zString id = ccCreateNanoparticlePentagon::GetType();
    const long  argTotal = ccCreateNanoparticlePentagon::GetArgumentTotal();

	const bool bRegistered = acfCommandFactory::Instance()->Register(id, argTotal, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ccCreateNanoparticlePentagon::ccCreateNanoparticlePentagon(long executionTime) : xxCommand(executionTime),
                                    m_PolymerName(""), m_ProcId(0), m_ColourId(0),
                                    m_XC(0.0), m_YC(0.0), m_ZC(0.0),
                                    m_BoundingRadius(0.0), m_Side(0.0), m_Thickness(0.0),
                                    m_Theta(0.0), m_Phi(0.0),
                                    m_MaxBonds(0), m_Range(0.0), m_Fraction(0.0), 
                                    m_SpringConstant(0.0), m_UnstretchedLength(0.0),m_NumberTypes(0)
{
    m_vBeadTypes.clear();
    m_vConsInt.clear();
}

ccCreateNanoparticlePentagon::ccCreateNanoparticlePentagon(const ccCreateNanoparticlePentagon& oldCommand) : xxCommand(oldCommand),
                                     m_PolymerName(oldCommand.m_PolymerName),
                                     m_ProcId(oldCommand.m_ProcId),
                                     m_ColourId(oldCommand.m_ColourId),
                                     m_XC(oldCommand.m_XC),
                                     m_YC(oldCommand.m_YC),
									 m_ZC(oldCommand.m_ZC),
									 m_BoundingRadius(oldCommand.m_BoundingRadius),
                                     m_Side(oldCommand.m_Side),
                                     m_Thickness(oldCommand.m_Thickness),
                                     m_Theta(oldCommand.m_Theta),
                                     m_Phi(oldCommand.m_Phi),
                                     m_MaxBonds(oldCommand.m_MaxBonds),
                                     m_Range(oldCommand.m_Range),
                                     m_Fraction(oldCommand.m_Fraction),
                                     m_SpringConstant(oldCommand.m_SpringConstant),
                                     m_UnstretchedLength(oldCommand.m_UnstretchedLength),
                                     m_NumberTypes(oldCommand.m_NumberTypes)

{
    m_vBeadTypes=oldCommand.m_vBeadTypes;
    m_vConsInt=oldCommand.m_vConsInt;
}

// Constructor for use when creating the command internally. We have to perform
// validation on the data explicitly. If an illegal value for a data item
// is found, the command has it svalid flag set false in the base class.
// It is up to the calling routine to check that the command is valid.

ccCreateNanoparticlePentagon::ccCreateNanoparticlePentagon(long executionTime, 
							   const zString poly, long procId, long colourId,
                               double xc, double yc, double zc, double br,
                               double side, double thickness,
                               double theta, double phi,
                               long maxBonds, double range, double fraction, double springConstant, double unstretchedLength, long numbertypes, zLongVector vbeadtypes, zDoubleVector vconsint) : xxCommand(executionTime),
                                                                    m_PolymerName(poly), m_ProcId(procId), m_ColourId(colourId),
                                                                    m_XC(xc), m_YC(yc), m_ZC(zc),
                                                                    m_BoundingRadius(br), m_Side(side), m_Thickness(thickness),
                                                                    m_Theta(theta), m_Phi(phi),
                                                                    m_MaxBonds(maxBonds), m_Range(range), m_Fraction(fraction),
                                                                    m_SpringConstant(springConstant),
                                                                    m_UnstretchedLength(unstretchedLength), m_NumberTypes(numbertypes)
{
    m_vBeadTypes = vbeadtypes;
    m_vConsInt = vconsint;
    
	// Validate the nanoparticle's shape parameters
    
    if(m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
	   m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
	{
        SetCommandValid(false);
	}
	else if(m_BoundingRadius < 1.0 || m_Side < 1.0 || m_Thickness < 1.0 || 
		    m_Side > m_BoundingRadius || m_Thickness > m_BoundingRadius)
    {
        SetCommandValid(false);
	}
    else if(m_Theta < 0.0 || m_Phi < 0.0 || 
            m_Theta > xxBase::m_globalPI || m_Phi > xxBase::m_globalTwoPI)
	{
        SetCommandValid(false);
	}
    
	// Validate the polymerisation spring parameters
    
	if(m_MaxBonds < 1
       || m_Range < 0.0
       || m_Fraction < 0.0 || m_Fraction > 1.0 
       || m_SpringConstant < 0.0 || m_UnstretchedLength < 0.0)
    {
        SetCommandValid(false);
    }
    
    // Validate the interaction parameters
    if(m_NumberTypes < 0
       || m_vBeadTypes.size() != m_NumberTypes
       || m_vConsInt.size() != m_NumberTypes)
    {
        SetCommandValid(false);
    }
    for (long i=0;i<m_NumberTypes;i++)
    {
        if (m_vBeadTypes.at(i)<0)
        {
            SetCommandValid(false);
        }
    }
}


ccCreateNanoparticlePentagon::~ccCreateNanoparticlePentagon()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//  m_PolymerName           Polymer out of which nanoparticle will be created
//  m_ProcId                Processor that is to create the nanoparticle
//  m_ColourId              Display id to be given to the nanoparticle's beads
//	m_XC
//	m_YC		            Pentagon's centre as fractions of SimBox side lengths
//  m_ZC
//	m_BoundingRadius	    Radius of bounding sphere that contains the pentagon in units of bead diameter
//  m_Side                  Length of pentagon's side
//  m_Thickness             Thickness (or depth) of the pentagon
//  m_Theta                 Polar angle of pentagon's normal (0 <= m_Theta <= 180)
//  m_Phi                   Azimuthal angle   "              (0 <= m_Phi <= 360)
//	m_MaxBonds				maximum number of bonds per polymer
//	m_Range				    maximum separation of two polymers that can be bound
//	m_Fraction				fraction of neighbours to bind
//  m_SpringConstant		spring constant of the Hookean bonds
//  m_UnstretchedLength		unstretched length of the Hookean bonds

zOutStream& ccCreateNanoparticlePentagon::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output
	putXMLStartTags(os);
	putXMLEndTags(os);

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	putASCIIStartTags(os);
	putASCIIEndTags(os);

#endif

	return os;
}

zInStream& ccCreateNanoparticlePentagon::get(zInStream& is)
{
	// We perform some simple checks on the data here, but more complicated
	// validation is done in IsDataValid() below as it has access to CInputData.
    
    is >> m_PolymerName >> m_ProcId >> m_ColourId;
    
	if(!is.good() || m_ProcId < 0 || m_ColourId < 0)
        SetCommandValid(false);
        
	is >> m_XC >> m_YC >> m_ZC;
        
	if(!is.good() || m_XC < 0.0 || m_YC < 0.0 || m_ZC < 0.0 ||
       m_XC > 1.0 || m_YC > 1.0 || m_ZC > 1.0)
        SetCommandValid(false);
    
	is >> m_BoundingRadius >> m_Side >> m_Thickness;
        
	if(!is.good() || m_BoundingRadius < 1.0 || m_Side < 1.0 || m_Thickness < 1.0 || 
       m_Side > m_BoundingRadius || m_Thickness > m_BoundingRadius)
	{
        SetCommandValid(false);
	}
    
    // Note that Theta and Phi should be entered in degrees in the input file
	is >> m_Theta >> m_Phi;
    
	if(!is.good() || m_Theta < 0.0 || m_Phi < 0.0 || 
       m_Theta > 180.0 || m_Phi > 360.0)
	{
        SetCommandValid(false);
	}
    
	// Convert angles to radians
    
	m_Theta = m_Theta*xxBase::m_globalPI/180.0;
	m_Phi   =   m_Phi*xxBase::m_globalPI/180.0;
    
    is >> m_MaxBonds >> m_Range >> m_Fraction >> m_SpringConstant >> m_UnstretchedLength;
    
	if(!is.good() || m_MaxBonds < 1 || m_Range < 0.0
                  || m_Fraction < 0.0 || m_Fraction > 1.0 
                  || m_SpringConstant < 0.0 || m_UnstretchedLength < 0.0)
        SetCommandValid(false);
    is >> m_NumberTypes;
    if(!is.good() || m_NumberTypes < 0)
                SetCommandValid(false);
    long temBeadType;
    double temConsInt;
    for (long i=0;i<m_NumberTypes;i++)
    {
        is >> temBeadType >> temConsInt;
        
        m_vBeadTypes.push_back(temBeadType);
        m_vConsInt.push_back(temConsInt);
        
        if (!is.good() || m_vBeadTypes.at(i)<0)
        {
            SetCommandValid(false);
        }
    }

	return is;
}

// Non-static function to return the type of the command

const zString ccCreateNanoparticlePentagon::GetCommandType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current command.

const xxCommand* ccCreateNanoparticlePentagon::GetCommand() const
{
	return new ccCreateNanoparticlePentagon(*this);
}


// Implementation of the command that is sent by the SimBox to each xxCommand
// object to see if it is the right time for it to carry out its operation.
// We return a boolean so that the SimBox can see if the command executed or not
// as this may be useful for considering several commands. 

bool ccCreateNanoparticlePentagon::Execute(long simTime, ISimCmd* const pISimCmd) const
{
	if(simTime == GetExecutionTime())
	{
		pISimCmd->CreateNanoparticlePentagon(this);
		return true;
	}
	else
		return false;
}

// Function to check that the data defining the pentagonal region from
// which to select beads is valid. The target and and bead type are 
// validated in the base class.

bool ccCreateNanoparticlePentagon::IsDataValid(const CInputData& riData) const
{
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
// m_TargetLabel        - zString
// m_PolymerName        - zString
// m_XC, m_YC, m_ZC     - double, double, double
// m_InnerRadius        - double
// m_OuterRadius        - double
// m_MaxBonds           - long
// m_Range              - double
// m_Fraction           - double
// m_SpringConstant     - double
// m_UnstretchedLength  - double

// NOT IMPLEMENTED YET 2/1/13

bool ccCreateNanoparticlePentagon::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

	return true;

#else
    return false;
#endif
}
