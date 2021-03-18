// taExternalWork.cpp: implementation of the taExternalWork class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taExternalWork.h"
#include "taForceDecorator.h"
#include "IGlobalSimBox.h"
#include "LogctExternalWorkOnTarget.h"
#include "LogctNonexistentDecorator.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taExternalWork::m_Type = "ExternalWork";

const zString taExternalWork::GetType()
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that creates a decorator instance to wrap a taForceDecorator
// instance whose data is to be accumulated. Both decorators must wrap the same
// command target instance.
//
// The wrapped object is stored in the m_pInnerDecorator member variable of the
// CCommandTargetNode base class by passing the pDec pointer to the base class 
// constructor. The newly-created object is stored in the m_pOuterDecorator of
// the wrapped object using the function SetOuterDecorator().
//
// This decorator calculates the work done by a force on the beads in a command
// target and stores the particular taForceDecorator instance in a local
// member variable. In order to do this, it has to receive a pointer to the 
// tacForceDecorator instance via the pForceDec pointer, but it also has to store 
// the outermost decorator wrapping its enclosed command target in 
// its base class via the pDec pointer. The label of the wrapped force decorator
// is stored so that we can check it still exists before trying to access it. It may
// happen that the user mistakenly deletes a force decorator before its data
// accumulation is complete.

taExternalWork::taExternalWork(const zString label, CCommandTargetNode* const pDec, 
							   taForceDecorator* const pForceDec,
							   long start, long end) : taCumulateDecorator(label, pDec, pForceDec->GetLabel(), start, end),
							   xxFile(label + "." + IGlobalSimBox::Instance()->GetRunId(), true), m_pForceDec(pForceDec), m_Work(0.0)
{
	pDec->SetOuterDecorator(this);
}

taExternalWork::~taExternalWork()
{
    Close();
}

// Non-static member function to return the target's type.

const zString taExternalWork::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to sum up the work done by an external force
// on the beads in a command target.

// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
//
// Because the innermost command target object just holds the beads, the 
// chain stops when its do-nothing Execute() function is invoked.
//
// Because the user can issue a command that deletes the activity that applies
// a force to a target, we have to check that it still exists before we try
// and execute this activity.

void taExternalWork::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

	const zString targetLabel = GetActiveCommandTarget()->GetLabel();
	const zString decLabel    = GetLabel();

	// Calculate the work done as the sum over all beads in the target of F.dx
	// during the time interval specified. Because the radial force depends on 
	// the bead coordinates we have to pass a bead pointer in to the taForceDecorator
	// base class so that it can be accessed by the taRadialForce via the base class
	// pointer.

	if(simTime >= m_Start && simTime < m_End)
	{
		// Check that the force activity still exists before accessing it; if it
		// does not we issue a warning log message and set a flag in the
		// taCumulateDecorator base class to prevent the message being repeatedly issued.

		if(GetOutermostDecorator()->GetDecorator(m_WrappedLabel))
		{
			// We use a temporary vector to hold the target's beads because 
			// applying begin() and end() to the GetBeads() function directly 
			// yields null pointers.

			BeadVector vBeads = GetBeads();

			double workDone  = 0.0;

			for(BeadVectorIterator iterBead = vBeads.begin(); iterBead != vBeads.end(); iterBead++)
			{
				m_pForceDec->SetBeadPointer(*iterBead);

				const double fx = m_pForceDec->GetXForce(simTime);
				const double fy = m_pForceDec->GetYForce(simTime);
				const double fz = m_pForceDec->GetZForce(simTime);

				const double fdx = fx*(*iterBead)->GetdXPos();
				const double fdy = fy*(*iterBead)->GetdYPos();
				const double fdz = fz*(*iterBead)->GetdZPos();

				workDone += (fdx + fdy + fdz);
			}

			m_Work += workDone;
			
			m_outStream << simTime << " " << workDone << " " << m_Work << zEndl;

		}
		else if(!m_bWrapFailure)
		{
			m_bWrapFailure = true;
			new CLogctNonexistentDecorator(simTime, decLabel, m_WrappedLabel);
		}
	}
	else if(simTime == m_End)
	{
		// Note that the same message is used to log the start of the calculation,
		// when details about the bead type in the target are recorded, and the
		// end of the calculation, when only the work done is written out.

		new CLogctExternalWorkOnTarget(simTime, targetLabel, m_WrappedLabel, decLabel, m_Work);
	}
}

bool taExternalWork::Serialize()
{
    return true;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taExternalWork::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taExternalWork::Write(zOutStream& os) const
{
    
    return os;
}

