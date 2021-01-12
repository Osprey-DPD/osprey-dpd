/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taTargetCM.cpp: implementation of the taTargetCM class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taTargetCM.h"
#include "xxDecoratorState.h"
#include "TimeSeriesData.h"
#include "Bead.h"
#include "IGlobalSimBox.h"
#include "LogctTargetCM.h"
#include "LogctNonexistentDecorator.h"
#include "LogTextWarningMessage.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taTargetCM::m_Type = "TargetCM";

const zString taTargetCM::GetType()
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
// Note that pDec may be a pointer to a decorator instance or an actual target instance.

taTargetCM::taTargetCM(const zString label, CCommandTargetNode* const pDec,
							    long start, long end) : taCumulateDecorator(label, pDec, pDec->GetLabel(), start, end),
								m_BeadTotal(0.0), m_XCM(0.0), m_YCM(0.0), m_ZCM(0.0)
{
	pDec->SetOuterDecorator(this);

    BeadVector vBeads = pDec->GetBeads();
    m_BeadTotal = static_cast<double>(vBeads.size());

#if EnableXMLDecorators == SimXMLEnabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId() + ".xml", GetStartTime(), GetEndTime(), IGlobalSimBox::Instance()->GetRunId(), "TargetCM"));

	// Note that the opening <Header> tag is written in the xxDecoratorState class
	// but the closing tag is written here. This is a hack.

	zOutStream& os = m_pState->putXMLStartTags();
	os << "<TargetLabel>"		 << GetActiveCommandTarget()->GetLabel() << "</TargetLabel>";
    os << "<DecoratorLabel>"     << GetLabel() << "</DecoratorLabel>";
	os << "</Header>" << zEndl;

#elif EnableXMLDecorators == SimXMLDisabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId(), GetStartTime(), GetEndTime(), IGlobalSimBox::Instance()->GetRunId(), "TargetCM"));

	zOutStream& os = m_pState->putASCIIStartTags();
	os << "    Target	    " << GetActiveCommandTarget()->GetLabel() << zEndl;
    os << "    Decorator   " << GetLabel() << zEndl;

#endif

}

taTargetCM::~taTargetCM()
{

}

// Non-static member function to return the target's type.

const zString taTargetCM::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to output the components of the CM position of 
// a target as a function of time.
//
// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
// The innermost command target object just holds the beads, so the chain stops when 
// its do-nothing Execute() function is invoked.
//
// Because the user can issue a command that deletes the decorator that is 
// wrapped by this one (or, indeed, even the target itself), we check that it 
// still exists before we try and execute this activity.

void taTargetCM::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

	const zString targetLabel = GetActiveCommandTarget()->GetLabel();
	const zString decLabel    = GetLabel();

    // If the target does not implement GetBeads() or is empty, we log a warning message 
    // ang ignore the decorator.

	if(m_BeadTotal > 0.0)
	{
	    if(simTime > m_Start && simTime <= m_End)
	    {
            double x[3];
            double dx[3];
			double xcm[3];
			xcm[0] = 0.0;
			xcm[1] = 0.0;
			xcm[2] = 0.0;

            long index = 0;
			for(BeadVectorIterator iterBead = m_vBeads.begin(); iterBead != m_vBeads.end(); iterBead++)
			{
                x[0] = (*iterBead)->GetXPos();
                x[1] = (*iterBead)->GetYPos();
                x[2] = (*iterBead)->GetZPos();

                dx[0] = x[0] - xcm[0];       
                dx[1] = x[1] - xcm[1];       
                dx[2] = x[2] - xcm[2];       

                // Correct for the PBCs if a bead is more than half the simbox 
                // size away from the current CM in any component.
                // What do we do if the target is larger than half the simbox?

                if( dx[0] > IGlobalSimBox::Instance()->GetHalfSimBoxXLength() )
                    dx[0] = dx[0] - IGlobalSimBox::Instance()->GetSimBoxXLength();
                else if( dx[0] < -IGlobalSimBox::Instance()->GetHalfSimBoxXLength() )
                    dx[0] = dx[0] + IGlobalSimBox::Instance()->GetSimBoxXLength();

		        if( dx[1] > IGlobalSimBox::Instance()->GetHalfSimBoxYLength() )
			        dx[1] = dx[1] - IGlobalSimBox::Instance()->GetSimBoxYLength();
		        else if( dx[1] < -IGlobalSimBox::Instance()->GetHalfSimBoxYLength() )
			        dx[1] = dx[1] + IGlobalSimBox::Instance()->GetSimBoxYLength();

#if SimDimension == 3
		        if( dx[2] > IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
			        dx[2] = dx[2] - IGlobalSimBox::Instance()->GetSimBoxZLength();
		        else if( dx[2] < -IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
			        dx[2] = dx[2] + IGlobalSimBox::Instance()->GetSimBoxZLength();
#else
                dx[2] = 0.0;
#endif

                xcm[0] += x[0];
                xcm[1] += x[1];
                xcm[2] += x[2];

                index++;
			}

            // Now normalise the data by dividing by the number of beads in the target.

            xcm[0] /= m_BeadTotal;
            xcm[1] /= m_BeadTotal;
            xcm[2] /= m_BeadTotal;

            // Write the CM data to the decorator state file

	        long dataTotal = 4;

	        CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);

	        pTSD->SetValue(0, simTime, "Time");
	        pTSD->SetValue(1, xcm[0],  "XCM");
	        pTSD->SetValue(2, xcm[1],  "YCM");
	        pTSD->SetValue(3, xcm[2],  "ZCM");

	        m_pState->AddTimeSeriesData(pTSD);

            // ********************
            // Postconditions

	        if(simTime == m_End)
	        {
                // Write the time series data to file or log a warning message 
                // if it fails

                if(SaveState())
                {
		             new CLogctTargetCM(simTime, targetLabel, decLabel);
                }
                else
                {
                     new CLogTextWarningMessage(simTime, "Error serialising data from decorator " + decLabel + " around target " + targetLabel);
                }
            }
            // ********************
	    }
	    else if(simTime == m_Start)
	    {
            // ********************
            // Preconditions

            m_XCM = 0.0;
            m_YCM = 0.0;
            m_ZCM = 0.0;

		    // Record the start of the time series sampling. Note that the same message
            // is used to log the start of the calculation and its end, but with
            // different arguments.

		     new CLogctTargetCM(simTime, targetLabel, decLabel,  
									                  m_Start, m_End, static_cast<long>(m_BeadTotal));
	    }
    }
	else if(!m_bWrapFailure)
	{
		m_bWrapFailure = true;
		 new CLogctNonexistentDecorator(simTime, decLabel, "");
	}
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taTargetCM::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taTargetCM::Write(zOutStream& os) const
{
    
    return os;
}

