/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taDistanceMoved.cpp: implementation of the taDistanceMoved class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taDistanceMoved.h"
#include "taForceDecorator.h"
#include "LogctDistanceMovedByTarget.h"
#include "LogctNonexistentDecorator.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taDistanceMoved::m_Type = "DistanceMoved";

const zString taDistanceMoved::GetType()
{
	return m_Type;
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

taDistanceMoved::taDistanceMoved(const zString label, CCommandTargetNode* const pDec,
							    taForceDecorator* const pForceDec,
							    long start, long end) : taCumulateDecorator(label, pDec, pForceDec->GetLabel(), start, end),
								m_pForceDec(pForceDec), m_Distance(0.0), m_DistancePerBead(0.0)
{
	pDec->SetOuterDecorator(this);
}

taDistanceMoved::~taDistanceMoved()
{

}

// Non-static member function to return the target's type.

const zString taDistanceMoved::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to sum up the net distance moved by the beads 
// in a specified command target force decorator. This is the sum over all the 
// beads of the distances dx, and gives the net distance moved over the time 
// interval, not the sum of the absolute values of the displacement increments. 
// This allows direct comparison of the results with the taExternalWork decorator.
//
// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
// The innermost command target object just holds the beads, so the chain stops when 
// its do-nothing Execute() function is invoked.
//
// Because the user can issue a command that deletes the activity that applies
// a force to a target, we have to check that it still exists before we try
// and execute this activity.

void taDistanceMoved::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

	const zString targetLabel = GetActiveCommandTarget()->GetLabel();
	const zString decLabel    = GetLabel();

	// Calculate the distance moved as the sum of dx over all beads in 
	// the target during the time interval specified. This relies on the 
	// distance member variables being zeroed in the constructor.

	if(simTime >= m_Start && simTime < m_End)
	{
		// Check that the force activity still exists before accessing it; if it
		// does not then we issue a warning log message and set a flag in the
		// taCumulateDecorator base class to prevent the message being repeatedly issued.

		if(GetOutermostDecorator()->GetDecorator(m_WrappedLabel))
		{
			// We use a temporary vector to hold the target's beads because 
			// applying begin() and end() to the GetBeads() function directly 
			// yields null pointers.

			BeadVector vBeads = GetBeads();

			double distMoved  = 0.0;

			for(BeadVectorIterator iterBead = vBeads.begin(); iterBead != vBeads.end(); iterBead++)
			{
				const double dx = (*iterBead)->GetdXPos();
				const double dy = (*iterBead)->GetdYPos();
				const double dz = (*iterBead)->GetdZPos();

				distMoved += sqrt(dx*dx + dy*dy + dz*dz);
			}

			m_Distance += distMoved;
			m_DistancePerBead  = m_Distance/static_cast<double>(GetBeadTotal());
		}
		else if(!m_bWrapFailure)
		{
			m_bWrapFailure = true;
			 new CLogctNonexistentDecorator(simTime, decLabel, m_WrappedLabel);
		}
	}
	else if(simTime == m_End)
	{
		// Note that the same message is used to log the start of the calculation 
		// and the end of the calculation (last two parameters = accumulated distance
		// moved and average distance moved)

		 new CLogctDistanceMovedByTarget(simTime, targetLabel, m_WrappedLabel, decLabel,  
												m_Distance, m_DistancePerBead);
	}
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taDistanceMoved::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taDistanceMoved::Write(zOutStream& os) const
{
    
    return os;
}

