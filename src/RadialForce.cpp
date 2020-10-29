/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// RadialForce.cpp: implementation of the CRadialForce class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "RadialForce.h"
#include "Bead.h"

#include "SimMathFlags.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor for a CForceLaw that treats the target as  a single object
// and applies a force to it that is directed radially outward in a plane
// defined by a normal vector and an origin. Note that the normal vector is
// stored in the CForceLaw base class using the member variables used to
// hold the force direction for the simpler force laws such as CConstantForce.

CRadialForce::CRadialForce(long start, long end, long x, long y, long z, 
						   double cx, double cy, double cz, double magnitude, 
						   BeadVector beads) : CForceLaw(start, end, x, y, z),
										      m_CentreX(cx),
										      m_CentreY(cy),
										      m_CentreZ(cz),
										      m_Magnitude(magnitude),
										      m_RelOX(0.0),
										      m_RelOY(0.0),
										      m_RelOZ(0.0),
											  m_RelDotN(0.0),
										      m_ForceX(0.0),
										      m_ForceY(0.0),
										      m_ForceZ(0.0),
											  m_ForceMag(0.0),
											  m_TargetSize(static_cast<double>(beads.size())),
											  m_Beads(beads)
{

}

CRadialForce::~CRadialForce()
{
}


// Functions to return the components of the radial force. Note that we don't
// check whether the force should be applied, that is done by the SimBox, we
// only pass out the magnitude of the force at the current time. When the 
// force command is first executed the magnitude of the force is zero.
//
// For this CForceLaw object the force depends on the coordinates of the target
// beads and the origin of the force. The force rotates in a plane defined by
// a normal vector. Each bead experiences a force, of constant magnitude, that
// is directed along a vector from the force origin to the bead's position.
// Hence, we cannot use the usual PVFs that calculate the force, but instead
// calculate the force for all the beads in one call.

double CRadialForce::GetXForce(long time)
{
	return 0.0;
}

double CRadialForce::GetYForce(long time)
{
	return 0.0;
}

double CRadialForce::GetZForce(long time)
{
	return 0.0;
}

void CRadialForce::CalculateVariableForces(const CAbstractBead* const pBead, double* fx, double* fy, double* fz)
{
	m_RelOX = pBead->GetXPos() - m_CentreX;
	m_RelOY = pBead->GetYPos() - m_CentreY;
	m_RelOZ = pBead->GetZPos() - m_CentreZ;

	m_RelDotN = m_RelOX*m_X + m_RelOY*m_Y + m_RelOZ*m_Z;

	m_ForceX = m_RelOX - m_RelDotN*m_X;
	m_ForceY = m_RelOY - m_RelDotN*m_Y;
	m_ForceZ = m_RelOZ - m_RelDotN*m_Z;

	m_ForceMag = sqrt(m_ForceX*m_ForceX + m_ForceY*m_ForceY + m_ForceZ*m_ForceZ);

	*fx = m_Magnitude*m_ForceX/m_ForceMag;
	*fy = m_Magnitude*m_ForceY/m_ForceMag;
	*fz = m_Magnitude*m_ForceZ/m_ForceMag;
}
