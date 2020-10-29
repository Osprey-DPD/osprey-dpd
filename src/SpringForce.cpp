/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// SpringForce.cpp: implementation of the CSpringForce class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SpringForce.h"
#include "Bead.h"

#include "SimMathFlags.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor for a CForceLaw that treats the target as  a single object
// and applies a force to it that models a transducer attached by a Hookean
// spring moving away with a constant velocity. The initial position of the
// target is the CM of the beads composing it. When the force is calculated
// we use the new CM of the target beads to define the point of application 
// of the force.

CSpringForce::CSpringForce(long start, long end, long x, long y, long z, 
						   double springConst, double springLength, 
						   double velocity, BeadVector beads) : CForceLaw(start, end, x, y, z),
										      m_SpringConst(springConst),
										      m_SpringLength(springLength),
										      m_Velocity(velocity),
											  m_Length(0.0),
											  m_TransXPos(0.0),
											  m_TransYPos(0.0),
											  m_TransZPos(0.0),
											  m_TransXPos0(0.0),
											  m_TransYPos0(0.0),
											  m_TransZPos0(0.0),
											  m_TargetXPos(0.0),
											  m_TargetYPos(0.0),
											  m_TargetZPos(0.0),
											  m_TargetSize(static_cast<double>(beads.size())),
											  m_Beads(beads)
{
	// Calculate the initial coordinates of the transducer, taken as the
	// CM of the target plus the unstretched spring length in the direction of 
	// motion of the transducer.

	for(BeadVectorIterator iterBead=m_Beads.begin(); iterBead!=m_Beads.end(); iterBead++)
	{
		m_TargetXPos += (*iterBead)->GetXPos();
		m_TargetYPos += (*iterBead)->GetYPos();
		m_TargetZPos += (*iterBead)->GetZPos();
	}

	m_TargetXPos /= m_TargetSize;
	m_TargetYPos /= m_TargetSize;
	m_TargetZPos /= m_TargetSize;

	// Set the transducer's initial position to the target plus the unstretched
	// spring length in the direction of the transducer's movement. We assume
	// the unit vector in the direction of motion is actually a unit vector.
	// This has been normalized in the CForceLaw base class.

	m_TransXPos0 = m_TargetXPos + m_SpringLength*m_X;
	m_TransYPos0 = m_TargetYPos + m_SpringLength*m_Y;
	m_TransZPos0 = m_TargetZPos + m_SpringLength*m_Z;

}

CSpringForce::~CSpringForce()
{
}


// Functions to return the components of the spring force. Note that we don't
// check whether the force should be applied, that is done by the SimBox, we
// only pass out the magnitude of the force at the current time. When the 
// force command is first executed the magnitude of the force is zero.
//
// For this CForceLaw object the force depends on the coordinates of the target
// beads and the transducer. It also depends on the initial coordinates of the
// target, so we have to store them in a vector the first time the functions to
// get the forces are called. We calculate all components in the GetXForce as the
// GetYForce and GetZForce functions should be called after it.

double CSpringForce::GetXForce(long simTime)
{
	// Calculate the target CM position. 

	m_TargetXPos = 0.0;
	m_TargetYPos = 0.0;
	m_TargetZPos = 0.0;

	for(BeadVectorIterator iterBead=m_Beads.begin(); iterBead!=m_Beads.end(); iterBead++)
	{
		m_TargetXPos += (*iterBead)->GetXPos();
		m_TargetYPos += (*iterBead)->GetYPos();
		m_TargetZPos += (*iterBead)->GetZPos();
	}

	m_TargetXPos /= m_TargetSize;
	m_TargetYPos /= m_TargetSize;
	m_TargetZPos /= m_TargetSize;

	// Calculate the instantaneous spring length from the current position of the
	// transducer and target. Note that even though the transducer is pulled in 
	// a given direction, the force on the target may be offset due to their
	// relative positions. The direction of the transducer motion is converted
	// into a unit vector in the CApplyForce constructor, so we don't have to 
	// check its components here.

	m_TransXPos = m_TransXPos0 + m_Velocity*m_X*static_cast<double>(simTime - m_Start) - m_TargetXPos;
	m_TransYPos = m_TransYPos0 + m_Velocity*m_Y*static_cast<double>(simTime - m_Start) - m_TargetYPos;
	m_TransZPos = m_TransZPos0 + m_Velocity*m_Z*static_cast<double>(simTime - m_Start) - m_TargetZPos;

	m_Length = sqrt(m_TransXPos*m_TransXPos + m_TransYPos*m_TransYPos + m_TransZPos*m_TransZPos);

	return m_SpringConst*(m_Length - m_SpringLength)*m_TransXPos/m_Length;
}

double CSpringForce::GetYForce(long simTime)
{
	return m_SpringConst*(m_Length - m_SpringLength)*m_TransYPos/m_Length;
}

double CSpringForce::GetZForce(long simTime)
{
	return m_SpringConst*(m_Length - m_SpringLength)*m_TransZPos/m_Length;
}
