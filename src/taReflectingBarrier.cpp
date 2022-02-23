/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taReflectingBarrier.cpp: implementation of the taReflectingBarrier class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taReflectingBarrier.h"
#include "IGlobalSimBox.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taReflectingBarrier::m_Type = "ReflectingBarrier";

const zString taReflectingBarrier::GetType()
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
	CCommandTargetNode* Create(const zString label) {return new taReflectingBarrier(label);}

	const zString id = taReflectingBarrier::GetType();

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

taReflectingBarrier::taReflectingBarrier(const zString label, CCommandTargetNode* const pDec,
							  long xn, long yn, long zn, double mag) : taForceDecorator(label, pDec),
							   m_X(xn), m_Y(yn), m_Z(zn), m_mag(mag),
							   m_XOrigin(0), m_YOrigin(0), m_ZOrigin(0), 
							   m_FX(0.0), m_FY(0.0), m_FZ(0.0)
{
	pDec->SetOuterDecorator(this);
    
    m_XOrigin = mag*static_cast<double>(m_X)*IGlobalSimBox::Instance()->GetSimBoxXLength();
    m_YOrigin = mag*static_cast<double>(m_Y)*IGlobalSimBox::Instance()->GetSimBoxYLength();
    m_ZOrigin = mag*static_cast<double>(m_Z)*IGlobalSimBox::Instance()->GetSimBoxZLength();

}

// Constructor for use when creating the instance by reading its state in from
// a file. We only define its label here and all remaining member variables
// must be set in the Read() function.

taReflectingBarrier::taReflectingBarrier(const zString label) : taForceDecorator(label),
							   m_X(0), m_Y(0), m_Z(0), m_mag(0.0),
							   m_XOrigin(0.0), m_YOrigin(0.0), m_ZOrigin(0.0), 
							   m_FX(0.0), m_FY(0.0), m_FZ(0.0)
{

}

taReflectingBarrier::~taReflectingBarrier()
{

}

// Non-static member function to return the target's type.

const zString taReflectingBarrier::GetTargetType() const
{
    return m_Type;
}

// Function invoked by command that erects a barrier spanning the SimBox along one of its major axes to reflect all
// beads (of all types) contained within the specified command target away from the barrier and prevent their crossing it. 
//
//
// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
//
// Because the innermost command target object just holds the beads, the 
// chain stops when its do-nothing Execute() function is invoked.

void taReflectingBarrier::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

	// debug checks
	const zString label = GetLabel();

	// We use a temporary vector to hold the target's beads because 
	// applying begin() and end() to the GetBeads() function directly 
	// yields null pointers.

	BeadVector vBeads = GetBeads();
	
//	std::cout << "Reflecting barrier " << GetLabel() << " at " << m_XOrigin << " " << m_YOrigin << " " << m_ZOrigin << " has Total # beads " << vBeads.size() << zEndl;
	

    // We do the check on the axis before the loop to avoid checking it for every bead. We assume that only
    // the major axes have been specified, and it is up to the calling routine to guarantee this.
    // Note that we have to check the normal vector components not the position as the latter could be at the origin.
    
    // We set the distance within which the retarding force is applied to the CNTCell width as we are guaranteed that
    // no bead can move farther than this in one timestep. A smaller value ight be possible, but could let beads through.
    
    const double  dretard = 1.0;
    
    if(m_X)
    {
        for(BeadVectorIterator iterBead=vBeads.begin(); iterBead!=vBeads.end(); ++iterBead)
        {
            if( fabs((*iterBead)->GetXPos() - m_XOrigin) < dretard )   // is bead close to barrier?
            {
                (*iterBead)->SetXForce( -((*iterBead)->GetOldXMom()/IGlobalSimBox::Instance()->GetStepSize()) - 0.5*(*iterBead)->GetOldXForce() );
            }
        }
    }
    else if(m_Y)
    {
        for(BeadVectorIterator iterBead=vBeads.begin(); iterBead!=vBeads.end(); ++iterBead)
        {
            if( fabs((*iterBead)->GetYPos() - m_YOrigin) < dretard )   // is bead close to barrier?
            {
                (*iterBead)->SetYForce( -((*iterBead)->GetOldYMom()/IGlobalSimBox::Instance()->GetStepSize()) - 0.5*(*iterBead)->GetOldYForce() );
            }
        }
    }
    else
    {
        for(BeadVectorIterator iterBead=vBeads.begin(); iterBead!=vBeads.end(); ++iterBead)
        {
            if( fabs((*iterBead)->GetZPos() - m_ZOrigin) < dretard )   // is bead close to barrier?
            {
                (*iterBead)->SetZForce( -((*iterBead)->GetOldZMom()/IGlobalSimBox::Instance()->GetStepSize()) - 0.5*(*iterBead)->GetOldZForce() );
            }
        }
    }
}

// Calculate the force on the bead stored in the taForceDecorator base class.
// Because the linear force depends on the bead coordinates, we need access
// to the bead's data. To avoid calculating vector magnitudes three times, we
// only do it in the X component calculation and store all three components
// of the foce. Hence, the GetXForce() function MUST be called before the Y, Z
// components or they will return nonsense.

double taReflectingBarrier::GetXForce(long simTime)
{
	return m_FX;
}

double taReflectingBarrier::GetYForce(long simTime)
{
	return m_FY;
}

double taReflectingBarrier::GetZForce(long simTime)
{
	return m_FZ;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. See taConstantForce for details.

zInStream& taReflectingBarrier::Read(zInStream& is)
{
    is >> m_X >> m_Y >> m_Z;

    if(!is.good() || (m_X == 0 && m_Y == 0 && m_Z == 0))
    {
         new CLogRestartStateBuilderError(0, "Error importing reflecting barrier decorator (invalid normal vector)");
        return is;
    }

    is >> m_mag;

    if(!is.good() || m_mag < 0.0 || m_mag >= 1.0)
    {
         new CLogRestartStateBuilderError(0, "Error importing reflecting barrier decorator (invalid position)");
        return is;
    }
    
    // Store the location of the barrier
    
    m_XOrigin = m_mag*static_cast<double>(m_X);
    m_YOrigin = m_mag*static_cast<double>(m_Z);
    m_ZOrigin = m_mag*static_cast<double>(m_X);
    
    m_FX = 0.0;
    m_FY = 0.0;
    m_FZ = 0.0;

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taReflectingBarrier::Write(zOutStream& os) const
{
    CCommandTargetNode::Write(os);  // Write base class data first
   
    os << m_X << m_Y << m_Z;

    os << m_mag;

    if(GetOuterDecorator())
        return GetOuterDecorator()->Write(os);
    else
        return os;
}

