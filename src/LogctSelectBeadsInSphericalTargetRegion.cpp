/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// LogctSelectBeadsInSphericalTargetRegion.cpp: implementation of the CLogctSelectBeadsInSphericalTargetRegion class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "LogctSelectBeadsInSphericalTargetRegion.h"

//////////////////////////////////////////////////////////////////////
// Global function for serialization
//////////////////////////////////////////////////////////////////////

zOutStream& operator<<(zOutStream& os, const CLogctSelectBeadsInSphericalTargetRegion& rMsg)
{
	// We assume the angles are in radians and convert them to degrees 

	const double theta1 = rMsg.m_Theta1*180.0/xxBase::m_globalPI;
	const double theta2 = rMsg.m_Theta2*180.0/xxBase::m_globalPI;
	const double phi1   = rMsg.m_Phi1*180.0/xxBase::m_globalPI;
	const double phi2   = rMsg.m_Phi2*180.0/xxBase::m_globalPI;

#if EnableXMLCommands == SimXMLEnabled

	// XML output
	os << "<Body>" << zEndl;
	os << "<Name>SelectBeadsInSphericalTargetRegion</Name>" << zEndl;
	os << "<Text>" << zEndl;
	os << "Command target " << rMsg.m_BeadTarget << " holds " << rMsg.m_BeadTotal << " beads " 
	   << rMsg.m_BeadName << " (type " << rMsg.m_BeadType << ") selected from target " << rMsg.m_CmdTarget
       << " in spherical polar region defined by radius/polar/azimuthal coordinates " 
       << rMsg.m_Radius1 << " " << rMsg.m_Radius2 << " "
       << theta1 << " " << theta2  << " " << phi1   << " " << phi2;
	os << "</Text>" << zEndl;
	os << "</Body>" << zEndl;

#elif EnableXMLCommands == SimXMLDisabled

	// ASCII output 
	os << "Command target " << rMsg.m_BeadTarget << " holds " << rMsg.m_BeadTotal << " beads " 
	   << rMsg.m_BeadName << " (type " << rMsg.m_BeadType << ") selected from target " << rMsg.m_CmdTarget
       << " in spherical polar region defined by radius/polar/azimuthal coordinates " 
       << rMsg.m_Radius1 << " " << rMsg.m_Radius2 << " "
       << theta1 << " " << theta2  << " " << phi1   << " " << phi2;

#endif

	return os;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogctSelectBeadsInSphericalTargetRegion::CLogctSelectBeadsInSphericalTargetRegion(long time, 
                               const zString cmdTarget, const zString beadTarget, 
							   const zString beadName, long beadType, long beadTotal,
                               double radius1, double radius2, double theta1, double theta2,
                               double phi1, double phi2) : CLogConstraintMessage(time), 
													   m_CmdTarget(cmdTarget),
													   m_BeadTarget(beadTarget),
													   m_BeadName(beadName),
													   m_BeadType(beadType),
													   m_BeadTotal(beadTotal),
                                                       m_Radius1(radius1), m_Radius2(radius2), 
                                                       m_Theta1(theta1),   m_Theta2(theta2), 
                                                       m_Phi1(phi1), m_Phi2(phi2) 
{

}

CLogctSelectBeadsInSphericalTargetRegion::~CLogctSelectBeadsInSphericalTargetRegion()
{

}

// Pure virtual function to allow the xxMessage-derived object to 
// write its data to file when invoked through an xxMessage pointer. 

void CLogctSelectBeadsInSphericalTargetRegion::Serialize(zOutStream& os) const
{
	CLogConstraintMessage::Serialize(os);

	os << (*this);
}

