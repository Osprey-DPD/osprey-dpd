/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taCylinderLinearForce.cpp: implementation of the taCylinderLinearForce class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taCylinderLinearForce.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taCylinderLinearForce::m_Type = "CylinderLinearForce";

const zString taCylinderLinearForce::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	CCommandTargetNode* Create(const zString label) {return new taCylinderLinearForce(label);}

	const zString id = taCylinderLinearForce::GetType();

	const bool bRegistered = acfTargetFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that creates a decorator object to wrap a command target, which
// may already be wrapped by other decorator objects. 
//
// The wrapped object is stored in the m_pInnerDecorator member variable of the
// CCommandTargetNode base class by passing the pDec pointer to the base class 
// constructor. The newly-created object is stored in the m_pOuterDecorator of
// the wrapped object using the function SetOuterDecorator().

taCylinderLinearForce::taCylinderLinearForce(const zString label, CCommandTargetNode* const pDec,
							  long xn, long yn, long zn, 
			  				  double xc, double yc, double zc, double mag) : taForceDecorator(label, pDec),
							   m_X(xn), m_Y(yn), m_Z(zn),
							   m_XOrigin(xc), m_YOrigin(yc), m_ZOrigin(zc), 
							   m_keff(mag), m_FX(0.0), m_FY(0.0), m_FZ(0.0),
							   m_XNormal(0.0),m_YNormal(0.0),m_ZNormal(0.0), 
							   m_RelOX(0.0), m_RelOY(0.0), m_RelOZ(0.0), 
							   m_ForceX(0.0), m_ForceY(0.0), m_ForceZ(0.0), m_ForceMag(0.0)
{
	pDec->SetOuterDecorator(this);

	// Store components of cylinder axis

	m_XNormal = static_cast<double>(m_X);
	m_YNormal = static_cast<double>(m_Y);
	m_ZNormal = static_cast<double>(m_Z);

	const double normalMag = sqrt(m_XNormal*m_XNormal + m_YNormal*m_YNormal + m_ZNormal*m_ZNormal);

	m_XNormal /= normalMag;
	m_YNormal /= normalMag;
	m_ZNormal /= normalMag;
}

// Constructor for use when creating the instance by reading its state in from
// a file. We only define its label here and all remaining member variables
// must be set in the Read() function.

taCylinderLinearForce::taCylinderLinearForce(const zString label) : taForceDecorator(label),
							   m_X(0), m_Y(0), m_Z(0),
							   m_XOrigin(0.0), m_YOrigin(0.0), m_ZOrigin(0.0), 
							   m_keff(0.0), m_FX(0.0), m_FY(0.0), m_FZ(0.0),
							   m_XNormal(0.0),m_YNormal(0.0),m_ZNormal(0.0), 
							   m_RelOX(0.0), m_RelOY(0.0), m_RelOZ(0.0), 
							   m_ForceX(0.0), m_ForceY(0.0), m_ForceZ(0.0), m_ForceMag(0.0)
{

}

taCylinderLinearForce::~taCylinderLinearForce()
{

}

// Non-static member function to return the target's type.

const zString taCylinderLinearForce::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to apply a linear force with cylindrical symmetry
// to the beads held in the wrapped command target. The cylinder axis and a point
// along it, designated its origin, are specified. Each bead feels a force 
// proportional to its distance from the axis in a radial direction. The force
// is derived from a harmonic potential of the form:
//
//    V(r) = 0.5*k*(r - raxis)**2
//
// where r = sqrt(x*x +y*y) for a cylinder oriented along the Z axis;
// and   raxis = sqrt(x0*x0 + y0*y0) and (x0,y0) are the cartesian coordinates
// of the cylinder's "origin."
//
// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
//
// Because the innermost command target object just holds the beads, the 
// chain stops when its do-nothing Execute() function is invoked.

void taCylinderLinearForce::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

	// debug checks
	const zString label = GetLabel();

	// We use a temporary vector to hold the target's beads because 
	// applying begin() and end() to the GetBeads() function directly 
	// yields null pointers.

	BeadVector vBeads = GetBeads();

	for(BeadVectorIterator iterBead=vBeads.begin(); iterBead!=vBeads.end(); iterBead++)
	{
		// Find the point on the cylinder closest to the bead's coordinates

		m_RelOX = (*iterBead)->GetXPos() - m_XOrigin;
		m_RelOY = (*iterBead)->GetYPos() - m_YOrigin;
		m_RelOZ = (*iterBead)->GetZPos() - m_ZOrigin;

		m_RelDotN = m_RelOX*m_XNormal + m_RelOY*m_YNormal + m_RelOZ*m_ZNormal;

		m_ForceX = ((*iterBead)->GetXPos() - m_XOrigin) - m_RelDotN*m_XNormal;
		m_ForceY = ((*iterBead)->GetYPos() - m_YOrigin) - m_RelDotN*m_YNormal;
		m_ForceZ = ((*iterBead)->GetZPos() - m_ZOrigin) - m_RelDotN*m_ZNormal;

		m_ForceMag = sqrt(m_ForceX*m_ForceX + m_ForceY*m_ForceY + m_ForceZ*m_ForceZ);

		m_FX = -m_keff*m_ForceX/m_ForceMag;
		m_FY = -m_keff*m_ForceY/m_ForceMag;
		m_FZ = -m_keff*m_ForceZ/m_ForceMag;

		AddBeadXForce(*iterBead, m_FX);
		AddBeadYForce(*iterBead, m_FY);
		AddBeadZForce(*iterBead, m_FZ);
	}
}

// Calculate the force on the bead stored in the taForceDecorator base class.
// Because the linear force depends on the bead coordinates, we need access
// to the bead's data. To avoid calculating vector magnitudes three times, we
// only do it in the X component calculation and store all three components
// of the foce. Hence, the GetXForce() function MUST be called before the Y, Z
// components or they will return nonsense.

double taCylinderLinearForce::GetXForce(long simTime)
{
	// Find the point on the cylinder closest to the bead's coordinates

	m_RelOX = m_pBead->GetXPos() - m_XOrigin;
	m_RelOY = m_pBead->GetYPos() - m_YOrigin;
	m_RelOZ = m_pBead->GetZPos() - m_ZOrigin;

	m_RelDotN = m_RelOX*m_XNormal + m_RelOY*m_YNormal + m_RelOZ*m_ZNormal;

	m_ForceX = (m_pBead->GetXPos() - m_XOrigin) - m_RelDotN*m_XNormal;
	m_ForceY = (m_pBead->GetYPos() - m_YOrigin) - m_RelDotN*m_YNormal;
	m_ForceZ = (m_pBead->GetZPos() - m_ZOrigin) - m_RelDotN*m_ZNormal;

	m_ForceMag = sqrt(m_ForceX*m_ForceX + m_ForceY*m_ForceY + m_ForceZ*m_ForceZ);

	m_FX = -m_keff*m_ForceX/m_ForceMag;
	m_FY = -m_keff*m_ForceY/m_ForceMag;
	m_FZ = -m_keff*m_ForceZ/m_ForceMag;

	return m_FX;
}

double taCylinderLinearForce::GetYForce(long simTime)
{
	return m_FY;
}

double taCylinderLinearForce::GetZForce(long simTime)
{
	return m_FZ;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. See taConstantForce for details.

zInStream& taCylinderLinearForce::Read(zInStream& is)
{
    is >> m_X >> m_Y >> m_Z;

    if(!is.good() || (m_X == 0 && m_Y == 0 && m_Z == 0))
    {
         new CLogRestartStateBuilderError(0, "Error importing cylinder linear force decorator (invalid normal vector?)");
        return is;
    }

    is >> m_XOrigin >> m_YOrigin >> m_ZOrigin >> m_keff;

    if(!is.good() || m_XOrigin < 0 || m_YOrigin < 0 || m_ZOrigin < 0 || m_keff < 0.0)
    {
         new CLogRestartStateBuilderError(0, "Error importing cylinder linear force decorator (invalid origin?)");
        return is;
    }

	// Store components of cylinder axis

	m_XNormal = static_cast<double>(m_X);
	m_YNormal = static_cast<double>(m_Y);
	m_ZNormal = static_cast<double>(m_Z);

	const double normalMag = sqrt(m_XNormal*m_XNormal + m_YNormal*m_YNormal + m_ZNormal*m_ZNormal);

	m_XNormal /= normalMag;
	m_YNormal /= normalMag;
	m_ZNormal /= normalMag;

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taCylinderLinearForce::Write(zOutStream& os) const
{
    CCommandTargetNode::Write(os);  // Write base class data first
   
    os << m_X << m_Y << m_Z;

    os << m_XOrigin << m_YOrigin << m_ZOrigin << m_keff;

    if(GetOuterDecorator())
        return GetOuterDecorator()->Write(os);
    else
        return os;
}

