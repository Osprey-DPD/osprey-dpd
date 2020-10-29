/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// cgcSetArgumentsTo3dRealLatticeVector.cpp: implementation of the cgcSetArgumentsTo3dRealLatticeVector class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimCommandFlags.h"
#include "cgcSetArgumentsTo3dRealLatticeVector.h"
#include "ISimCmd.h"
#include "InputData.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cgcSetArgumentsTo3dRealLatticeVector::cgcSetArgumentsTo3dRealLatticeVector(long executionTime) : xxCommand(executionTime), 
                                            m_GroupName(""), 
                                            m_XCommandIndex(0), m_YCommandIndex(0), m_ZCommandIndex(0),
                                            m_XArgName(""), m_YArgName(""), m_ZArgName(""), 
                                            m_XMax(0), m_YMax(0), m_ZMax(0),
                                            m_XOrigin(0.0), m_YOrigin(0.0), m_ZOrigin(0.0),
                                            m_LX(0.0), m_LY(0.0), m_LZ(0.0)
{
}

// Constructor for use when creating the command internally.

cgcSetArgumentsTo3dRealLatticeVector::cgcSetArgumentsTo3dRealLatticeVector(long executionTime, const zString groupName, 
                         long cmdIndexX, long cmdIndexY, long cmdIndexZ, 
                         const zString argNameX, const zString argNameY, const zString argNameZ,
                         long xmax, long ymax, long zmax,
                         double xorigin, double yorigin, double zorigin, 
                         double lx, double ly, double lz) : xxCommand(executionTime), m_GroupName(groupName), 
                         m_XCommandIndex(cmdIndexX), m_YCommandIndex(cmdIndexY), m_ZCommandIndex(cmdIndexZ),
						 m_XArgName(argNameX), m_YArgName(argNameY), m_ZArgName(argNameZ), 
                         m_XMax(xmax), m_YMax(ymax), m_ZMax(zmax), 
                         m_XOrigin(xorigin), m_YOrigin(yorigin), m_ZOrigin(zorigin),
                         m_LX(lx), m_LY(ly), m_LZ(lz)
{
}

// Copy constructor
cgcSetArgumentsTo3dRealLatticeVector::cgcSetArgumentsTo3dRealLatticeVector(const cgcSetArgumentsTo3dRealLatticeVector& oldCommand) : xxCommand(oldCommand),
										   m_GroupName(oldCommand.m_GroupName),
										   m_XCommandIndex(oldCommand.m_XCommandIndex),
										   m_YCommandIndex(oldCommand.m_YCommandIndex),
										   m_ZCommandIndex(oldCommand.m_ZCommandIndex),
										   m_XArgName(oldCommand.m_XArgName),
										   m_YArgName(oldCommand.m_YArgName),
										   m_ZArgName(oldCommand.m_ZArgName),
										   m_XMax(oldCommand.m_XMax),
										   m_YMax(oldCommand.m_YMax),
										   m_ZMax(oldCommand.m_ZMax),
										   m_XOrigin(oldCommand.m_XOrigin),
										   m_YOrigin(oldCommand.m_YOrigin),
										   m_ZOrigin(oldCommand.m_ZOrigin),
										   m_LX(oldCommand.m_LX),
										   m_LY(oldCommand.m_LY),
										   m_LZ(oldCommand.m_LZ)
{
}

cgcSetArgumentsTo3dRealLatticeVector::~cgcSetArgumentsTo3dRealLatticeVector()
{
}

// Member functions to read/write the data specific to the command.
//
// Arguments
// *********
//
//	GroupName		            Command group being modified
//  XCommandIndex               Index of the command whose argument is assigned the X component of a vector
//  YCommandIndex               Index of the command whose argument is assigned the Y component of a vector
//  ZCommandIndex               Index of the command whose argument is assigned the Z component of a vector
//  XArgName                    Placeholder name of the argument being assigned the X value
//  YArgName                    Placeholder name of the argument being assigned the Y value
//  ZArgName                    Placeholder name of the argument being assigned the Z value
//  XMax,YMax,ZMax              Number of points in lattice X,Y,Y dimensions
//  XOrigin,YOrigin, ZOrigin    Real vector pointing to lattice origin
//  LX, LY, LZ                  Length of lattice vector components in X,Y,Y

zOutStream& cgcSetArgumentsTo3dRealLatticeVector::put(zOutStream& os) const
{
#if EnableXMLCommands == SimXMLEnabled

	// XML output - it is the derived class' responsibility to add
	// the start and end tags and to call this function in between.

    os << "<CommandGroupName>" << m_GroupName     << "</CommandGroupName>" << zEndl;
	os << "<XCommandIndex>"    << m_XCommandIndex << "</XCommandIndex>"    << zEndl;
	os << "<XArgumentName>"    << m_XArgName      << "</XArgumentName>"    << zEndl;
	os << "<YCommandIndex>"    << m_YCommandIndex << "</YCommandIndex>"    << zEndl;
	os << "<YArgumentName>"    << m_YArgName      << "</YArgumentName>"    << zEndl;
	os << "<ZCommandIndex>"    << m_ZCommandIndex << "</ZCommandIndex>"    << zEndl;
	os << "<ZArgumentName>"    << m_ZArgName      << "</ZArgumentName>"    << zEndl;
	os << "<XMax>"             << m_XMax          << "</XMax>"             << zEndl;
	os << "<YMax>"             << m_YMax          << "</YMax>"             << zEndl;
	os << "<ZMax>"             << m_ZMax          << "</ZMax>"             << zEndl;
	os << "<XOrigin>"          << m_XOrigin       << "</XOrigin>"          << zEndl;
	os << "<YOrigin>"          << m_YOrigin       << "</YOrigin>"          << zEndl;
	os << "<ZOrigin>"          << m_ZOrigin       << "</ZOrigin>"          << zEndl;
	os << "<LX>"               << m_LX            << "</LX>"               << zEndl;
	os << "<LY>"               << m_LY            << "</LY>"               << zEndl;
	os << "<LZ>"               << m_LZ            << "</LZ>"               << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << m_GroupName << " " << m_XCommandIndex << " " << m_XArgName << " " << m_YCommandIndex << " " << m_YArgName << " " << m_ZCommandIndex << " " << m_ZArgName << " ";
    os << m_XMax << " " << m_YMax << " " << m_ZMax << " " << m_XOrigin << " " << m_YOrigin << " " << m_ZOrigin << " " << m_LX << " " << m_LY << " " << m_LZ;

#endif

	return os;
}

zInStream& cgcSetArgumentsTo3dRealLatticeVector::get(zInStream& is)
{
	is >> m_GroupName >> m_XCommandIndex >> m_XArgName >> m_YCommandIndex >> m_YArgName >> m_ZCommandIndex >> m_ZArgName;

	if(!is.good())
	   SetCommandValid(false);

    is >> m_XMax >> m_YMax >> m_ZMax >> m_XOrigin >> m_YOrigin >> m_ZOrigin >> m_LX >> m_LY >> m_LZ;

	if(!is.good())
	   SetCommandValid(false);

	return is;
}

// Function to check that the names of the command group and its contained command
// are valid. We also check that the command group exists. The test on the
// names of the argument and its new value are performed in the implementation class.

bool cgcSetArgumentsTo3dRealLatticeVector::IsDataValid(const CInputData& riData) const
{
	if(!riData.IsExternalNameValid(m_GroupName))
		return ErrorTrace("Invalid name for a command group being modified");
    else if(m_XCommandIndex < 1 || m_YCommandIndex < 1 || m_ZCommandIndex < 1)
		return ErrorTrace("Invalid command index for command group being modified");
	else if(!riData.IsExternalNameValid(m_XArgName) || !riData.IsExternalNameValid(m_YArgName) || !riData.IsExternalNameValid(m_ZArgName))
		return ErrorTrace("Invalid placeholder argument name for command group being modified");
    else if(m_XMax < 1 || m_YMax < 1 || m_ZMax < 1)
		return ErrorTrace("Invalid rectangular lattice size for command group being modified");
    else if(m_XOrigin < 0.0 || m_YOrigin < 0.0 || m_ZOrigin < 0.0)
		return ErrorTrace("Invalid rectangular lattice origin for command group being modified");
    else if(m_LX < 0.0 || m_LY < 0.0 || m_LZ < 0.0)
		return ErrorTrace("Invalid rectangular lattice lengths for command group being modified");

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
// This command expects the following types (in addition to its base class types):
//

bool cgcSetArgumentsTo3dRealLatticeVector::Pack(const tguArgumentSequence& vArguments)
{
#if EnableCommandGroups == SimCommandEnabled

    vArguments.at(0)->GetValue(&m_GroupName);
    vArguments.at(1)->GetValue(&m_XCommandIndex);
    vArguments.at(2)->GetValue(&m_XArgName);
    vArguments.at(3)->GetValue(&m_YCommandIndex);
    vArguments.at(4)->GetValue(&m_YArgName);
    vArguments.at(5)->GetValue(&m_ZCommandIndex);
    vArguments.at(6)->GetValue(&m_ZArgName);
    vArguments.at(7)->GetValue(&m_XMax);
    vArguments.at(8)->GetValue(&m_YMax);
    vArguments.at(9)->GetValue(&m_ZMax);
    vArguments.at(10)->GetValue(&m_XOrigin);
    vArguments.at(11)->GetValue(&m_YOrigin);
    vArguments.at(12)->GetValue(&m_ZOrigin);
    vArguments.at(13)->GetValue(&m_LX);
    vArguments.at(14)->GetValue(&m_LY);
    vArguments.at(15)->GetValue(&m_LZ);

    return true;
#else
    return false;
#endif
}

