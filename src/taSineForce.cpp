/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taSineForce.cpp: implementation of the taSineForce class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taSineForce.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taSineForce::m_Type = "SineForce";

const zString taSineForce::GetType()
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
	CCommandTargetNode* Create(const zString label) {return new taSineForce(label);}

	const zString id = taSineForce::GetType();

	const bool bRegistered = acfTargetFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

taSineForce::taSineForce(const zString label, CCommandTargetNode* const pDec,
						 long start, long xn, long yn, long zn, 
						 double amp, double period) : taForceDecorator(label, pDec),
													  m_StartTime(start),
													  m_X(xn), m_Y(yn), m_Z(zn),
												      m_Amplitude(amp),
												      m_Period(period),
												      m_Frequency(0.0),
													  m_AmpX(0.0), m_AmpY(0.0), m_AmpZ(0.0),
                                                      m_FX(0.0), m_FY(0.0), m_FZ(0.0)
{
	pDec->SetOuterDecorator(this);

    // Define the angular frequency for speed of use later

	m_Frequency = xxBase::m_globalTwoPI/m_Period;

	// Store the components of the force in each dimension to avoid
	// having to take a square-root at every time step

	const double fmag = sqrt(static_cast<double>(m_X*m_X + m_Y*m_Y + m_Z*m_Z));

	m_AmpX = m_Amplitude*static_cast<double>(m_X)/fmag;
	m_AmpY = m_Amplitude*static_cast<double>(m_Y)/fmag;
	m_AmpZ = m_Amplitude*static_cast<double>(m_Z)/fmag;

}

// Constructor for use when creating the instance by reading its state in from
// a file. We only define its label here and all remaining member variables
// must be set in the Read() function.

taSineForce::taSineForce(const zString label) : taForceDecorator(label),
												  m_StartTime(0),
												  m_X(0), m_Y(0), m_Z(0),
												  m_Amplitude(0.0),
												  m_Period(0.0),
												  m_Frequency(0.0),
												  m_AmpX(0.0), m_AmpY(0.0), m_AmpZ(0.0),
                                                  m_FX(0.0), m_FY(0.0), m_FZ(0.0)
{

}

taSineForce::~taSineForce()
{

}

// Non-static member function to return the target's type.

const zString taSineForce::GetTargetType() const
{
    return m_Type;
}

void taSineForce::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

	m_FX = m_AmpX*sin(m_Frequency*static_cast<double>(simTime - m_StartTime));
	m_FY = m_AmpY*sin(m_Frequency*static_cast<double>(simTime - m_StartTime));
	m_FZ = m_AmpZ*sin(m_Frequency*static_cast<double>(simTime - m_StartTime));

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

double taSineForce::GetXForce(long simTime)
{
	return m_FX;
}

double taSineForce::GetYForce(long simTime)
{
	return m_FY;
}

double taSineForce::GetZForce(long simTime)
{
	return m_FZ;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taSineForce::Read(zInStream& is)
{
    is >> m_StartTime >> m_X >> m_Y >> m_Z;

    if(!is.good() || m_StartTime < 0 || (m_X == 0 && m_Y == 0 && m_Z == 0))
    {
         new CLogRestartStateBuilderError(0, "Error importing sinusoidal force decorator (invalid start time or normal vector?)");
        return is;
    }

    is >> m_Amplitude >> m_Period;

    if(!is.good() || m_Amplitude < 0.0 || m_Period < 1.0)
    {
         new CLogRestartStateBuilderError(0, "Error importing sinusoidal force decorator (invalid amplitude or period?)");
        return is;
    }

    // Define the angular frequency for speed of use later

	m_Frequency = xxBase::m_globalTwoPI/m_Period;

	// Store the components of the force in each dimension to avoid
	// having to take a square-root at every time step

	const double fmag = sqrt(static_cast<double>(m_X*m_X + m_Y*m_Y + m_Z*m_Z));

	m_AmpX = m_Amplitude*static_cast<double>(m_X)/fmag;
	m_AmpY = m_Amplitude*static_cast<double>(m_Y)/fmag;
	m_AmpZ = m_Amplitude*static_cast<double>(m_Z)/fmag;

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taSineForce::Write(zOutStream& os) const
{
    CCommandTargetNode::Write(os);  // Write base class data first

    os << m_StartTime << " " << m_X << " " << m_Y << " " << m_Z << " " << m_Amplitude << " " << m_Period << zEndl;
    
    if(GetOuterDecorator())
        return GetOuterDecorator()->Write(os);
    else
        return os;
}



