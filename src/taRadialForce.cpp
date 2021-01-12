/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taRadialForce.cpp: implementation of the taRadialForce class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taRadialForce.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taRadialForce::m_Type = "RadialForce";

const zString taRadialForce::GetType()
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
	CCommandTargetNode* Create(const zString label) {return new taRadialForce(label);}

	const zString id = taRadialForce::GetType();

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

taRadialForce::taRadialForce(const zString label, CCommandTargetNode* const pDec,
							  long xn, long yn, long zn, 
			  				  double xc, double yc, double zc, double mag) : taForceDecorator(label, pDec),
							   m_X(xn), m_Y(yn), m_Z(zn),
							   m_XCentre(xc), m_YCentre(yc), m_ZCentre(zc), 
							   m_Magnitude(mag), m_FX(0.0), m_FY(0.0), m_FZ(0.0),
							   m_RelOX(0.0), m_RelOY(0.0), m_RelOZ(0.0), m_RelDotN(0.0),
							   m_ForceX(0.0), m_ForceY(0.0), m_ForceZ(0.0), m_ForceMag(0.0)
{
	pDec->SetOuterDecorator(this);
}

// Constructor for use when creating the instance by reading its state in from
// a file. We only define its label here and all remaining member variables
// must be set in the Read() function.

taRadialForce::taRadialForce(const zString label) : taForceDecorator(label),
							   m_X(0), m_Y(0), m_Z(0),
							   m_XCentre(0.0), m_YCentre(0.0), m_ZCentre(0.0), 
							   m_Magnitude(0.0), 
                               m_FX(0.0), m_FY(0.0), m_FZ(0.0),
							   m_RelOX(0.0), m_RelOY(0.0), m_RelOZ(0.0), m_RelDotN(0.0),
							   m_ForceX(0.0), m_ForceY(0.0), m_ForceZ(0.0), m_ForceMag(0.0)
{

}


taRadialForce::~taRadialForce()
{

}

// Non-static member function to return the target's type.

const zString taRadialForce::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to apply a radial force to the beads held in
// the wrapped command target. The normal to the plane of the force and the
// origin of the force are specified. Each bead feels a force proportional to
// its distance from the origin in a radial direction.
//
// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
//
// Because the innermost command target object just holds the beads, the 
// chain stops when its do-nothing Execute() function is invoked.

void taRadialForce::Execute(long simTime)
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
		m_RelOX = (*iterBead)->GetXPos() - m_XCentre;
		m_RelOY = (*iterBead)->GetYPos() - m_YCentre;
		m_RelOZ = (*iterBead)->GetZPos() - m_ZCentre;

		m_RelDotN = m_RelOX*m_X + m_RelOY*m_Y + m_RelOZ*m_Z;

		m_ForceX = m_RelOX - m_RelDotN*m_X;
		m_ForceY = m_RelOY - m_RelDotN*m_Y;
		m_ForceZ = m_RelOZ - m_RelDotN*m_Z;

		m_ForceMag = sqrt(m_ForceX*m_ForceX + m_ForceY*m_ForceY + m_ForceZ*m_ForceZ);

		m_FX = m_Magnitude*m_ForceX/m_ForceMag;
		m_FY = m_Magnitude*m_ForceY/m_ForceMag;
		m_FZ = m_Magnitude*m_ForceZ/m_ForceMag;

		AddBeadXForce(*iterBead, m_FX);
		AddBeadYForce(*iterBead, m_FY);
		AddBeadZForce(*iterBead, m_FZ);
	}
}

// Calculate the force on the bead stored in the taForceDecorator base class.
// Because the radial force depends on the bead coordinates, we need access
// to the bead's data. To avoid calculating vector magnitudes three times, we
// only do it in the X component calculation and store all three components
// of the foce. Hence, the GetXForce() function MUST be called before the Y, Z
// components or they will return nonsense.

double taRadialForce::GetXForce(long simTime)
{
	m_RelOX = m_pBead->GetXPos() - m_XCentre;
	m_RelOY = m_pBead->GetYPos() - m_YCentre;
	m_RelOZ = m_pBead->GetZPos() - m_ZCentre;

	m_RelDotN = m_RelOX*m_X + m_RelOY*m_Y + m_RelOZ*m_Z;

	m_ForceX = m_RelOX - m_RelDotN*m_X;
	m_ForceY = m_RelOY - m_RelDotN*m_Y;
	m_ForceZ = m_RelOZ - m_RelDotN*m_Z;

	m_ForceMag = sqrt(m_ForceX*m_ForceX + m_ForceY*m_ForceY + m_ForceZ*m_ForceZ);

	m_FX = m_Magnitude*m_ForceX/m_ForceMag;
	m_FY = m_Magnitude*m_ForceY/m_ForceMag;
	m_FZ = m_Magnitude*m_ForceZ/m_ForceMag;

	return m_FX;
}

double taRadialForce::GetYForce(long simTime)
{
	return m_FY;
}

double taRadialForce::GetZForce(long simTime)
{
	return m_FZ;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taRadialForce::Read(zInStream& is)
{
    is >> m_X >> m_Y >> m_Z;

    if(!is.good() || (m_X == 0 && m_Y == 0 && m_Z == 0))
    {
         new CLogRestartStateBuilderError(0, "Error importing radial force decorator (invalid normal vector?)");
        return is;
    }

    is >> m_XCentre >> m_YCentre >> m_ZCentre >> m_Magnitude;

    if(!is.good() || (m_XCentre < 0 || m_YCentre < 0 || m_ZCentre < 0) || m_Magnitude < 0.0)
    {
         new CLogRestartStateBuilderError(0, "Error importing radial force decorator (invalid origin or magnitude?)");
        return is;
    }

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taRadialForce::Write(zOutStream& os) const
{
     os << m_X << " " << m_Y << " " << m_Z << " " << m_XCentre << " " << m_YCentre << " " << m_ZCentre << " " << m_Magnitude << zEndl;
   
    if(GetOuterDecorator())
        return GetOuterDecorator()->Write(os);
    else
        return os;
}

