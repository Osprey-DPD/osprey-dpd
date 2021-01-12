/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taConstantForce.cpp: implementation of the taConstantForce class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taConstantForce.h"

#include "SimMathFlags.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taConstantForce::m_Type = "ConstantForce";

const zString taConstantForce::GetType()
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
	CCommandTargetNode* Create(const zString label) {return new taConstantForce(label);}

	const zString id = taConstantForce::GetType();

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
//
// Are there any problems in passing a this pointer from an object's constructor???

taConstantForce::taConstantForce(const zString label, CCommandTargetNode* const pDec, 
								 long xn, long yn, long zn,
								 double mag) : taForceDecorator(label, pDec),
											   m_X(xn), m_Y(yn), m_Z(zn),
											   m_Magnitude(mag)
{
	pDec->SetOuterDecorator(this);

	// Store the (constant) components of the force for efficicency

	const double fmag = sqrt(static_cast<double>(m_X*m_X + m_Y*m_Y + m_Z*m_Z));

	m_FX = m_Magnitude*static_cast<double>(m_X)/fmag;
	m_FY = m_Magnitude*static_cast<double>(m_Y)/fmag;
	m_FZ = m_Magnitude*static_cast<double>(m_Z)/fmag;

}

// Constructor for use when creating the instance by reading its state in from
// a file. We only define its label here and all remaining member variables
// must be set in the Read() function.

taConstantForce::taConstantForce(const zString label) : taForceDecorator(label),
											   m_X(0), m_Y(0), m_Z(0),
											   m_Magnitude(0)
{
}



taConstantForce::~taConstantForce()
{

}

// Non-static member function to return the target's type.

const zString taConstantForce::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to apply a constant force to the beads held in
// the wrapped command target. Because there may be many decorator objects 
// around a target, we chain their execution from the innermost decorator 
// to the outermost. This ensures that the same order of execution is used, 
// and the last added decorator is executed last.
//
// Because the innermost command target object just holds the beads, the 
// chain stops when its do-nothing Execute() function is invoked.

void taConstantForce::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

	// We use a temporary vector to hold the target's beads because 
	// applying begin() and end() to the GetBeads() function directly 
	// yields null pointers.

	BeadVector vBeads = GetBeads();

	for(BeadVectorIterator iterBead=vBeads.begin(); iterBead!=vBeads.end(); iterBead++)
	{
		AddBeadXForce(*iterBead, m_FX);
		AddBeadYForce(*iterBead, m_FY);
		AddBeadZForce(*iterBead, m_FZ);
	}
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

// Function to read an instance's data from a file and set all of its member variables
// that are normally set in the constructor. It is crucial that the wrapped target,
// which is pDec in the normal constructor, be set correctly. The CCommandTargetNode
// base class should have already read in the target's inner and outer decorator labels,
// so that we can use this information in the calling routine to wrap the decorator 
// around its target or inner decorator appropriately. Here, we just set the local data
// that defines the decorator's action.
//

zInStream& taConstantForce::Read(zInStream& is)
{
    is >> m_X >> m_Y >> m_Z >> m_Magnitude;

    if(!is.good() || (m_X == 0 && m_Y == 0 && m_Z == 0))
    {
         new CLogRestartStateBuilderError(0, "Error importing constant force decorator (invalid normal vector?)");
        return is;
    }

	// Store the (constant) components of the force for efficicency

	const double fmag = sqrt(static_cast<double>(m_X*m_X + m_Y*m_Y + m_Z*m_Z));

	m_FX = m_Magnitude*static_cast<double>(m_X)/fmag;
	m_FY = m_Magnitude*static_cast<double>(m_Y)/fmag;
	m_FZ = m_Magnitude*static_cast<double>(m_Z)/fmag;

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taConstantForce::Write(zOutStream& os) const
{
    CCommandTargetNode::Write(os);  // Write base class data first

    os << m_X << " " << m_Y << " " << m_Z << " " << m_Magnitude << zEndl;

    if(GetOuterDecorator())
        return GetOuterDecorator()->Write(os);
    else
        return os;
}

