/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taMSDBeadCM.cpp: implementation of the taMSDBeadCM class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taMSDBeadCM.h"
#include "xxDecoratorState.h"
#include "CommandTarget.h"
#include "Bead.h"
#include "TimeSeriesData.h"
#include "IGlobalSimBox.h"
#include "LogctMSDBeadTarget.h"
#include "LogctNonexistentDecorator.h"
#include "LogTextWarningMessage.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taMSDBeadCM::m_Type = "MSDBeadCM";

const zString taMSDBeadCM::GetType()
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

taMSDBeadCM::taMSDBeadCM(const zString label, CCommandTargetNode* const pDec,
							    long start, long end) : taCumulateDecorator(label, pDec, "", start, end),
                                m_pBeadTarget(0)
{
	pDec->SetOuterDecorator(this);

    // Store a pointer to the actual Bead target for later use

    m_pBeadTarget = dynamic_cast<CCommandTarget*>(GetActiveCommandTarget());

    m_vBeads.clear();
	m_vBeads = m_pBeadTarget->GetBeads();

    m_BeadTotal = m_vBeads.size();

    // Ensure the containers are empty

    m_vXInitial.clear();
    m_vYInitial.clear();
    m_vZInitial.clear();

#if EnableXMLDecorators == SimXMLEnabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId() + ".xml", GetStartTime(), GetEndTime(), IGlobalSimBox::Instance()->GetRunId(), "MSDBeadCM"));

	// Note that the opening <Header> tag is written in the xxDecoratorState class
	// but the closing tag is written here. This is a hack.

	zOutStream& os = m_pState->putXMLStartTags();
	os << "<TargetLabel>"		 << GetActiveCommandTarget()->GetLabel() << "</TargetLabel>";
    os << "<DecoratorLabel>"     << GetLabel() << "</DecoratorLabel>";
	os << "</Header>" << zEndl;

#elif EnableXMLDecorators == SimXMLDisabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId(), GetStartTime(), GetEndTime(), IGlobalSimBox::Instance()->GetRunId(), "MSDBeadCM"));

	zOutStream& os = m_pState->putASCIIStartTags();
	os << "    Target	    " << GetActiveCommandTarget()->GetLabel() << zEndl;
    os << "    Decorator   " << GetLabel() << zEndl;

#endif


}

taMSDBeadCM::~taMSDBeadCM()
{

}

// Non-static member function to return the target's type.

const zString taMSDBeadCM::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to calculate and serialise the MSD of all Beads
// contained in a Bead target using their CM coordinates. This is in contrast
// to the bead coordinate MSD that is calculated in the CMonitor per bead type.
// We use the Bead class' GetCM() function on each Bead contained in the
// target and average their coordinates at each time-step. Then we serialise the
// time series data to file when the sampling period ends.
//
// Note that the number, type and order of Beads in the target must NOT change
// while this decorator is calculating, otherwise the results will be wrong.
// The initial coordinates of the Beads' CM are stored using their order
// of storage in the target to index them.
//
// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
// The innermost command target object just holds the beads, so the chain stops when 
// its do-nothing Execute() function is invoked.
//

void taMSDBeadCM::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

	const zString targetLabel = GetActiveCommandTarget()->GetLabel();
	const zString decLabel    = GetLabel();

    // If the target is not a Bead target or is empty, we log a warning message
    // ang ignore the decorator.

	if(m_pBeadTarget && m_BeadTotal > 0)
	{
	    if(simTime > m_Start && simTime <= m_End)
	    {
            double dx[3];
			double dcm = 0.0;
            double CM[3];
            CM[0] = 0.0;
            CM[1] = 0.0;
            CM[2] = 0.0;

            long index = 0;
			for(BeadVectorIterator iterBead = m_vBeads.begin(); iterBead != m_vBeads.end(); iterBead++)
			{
				dx[0] = ((*iterBead)->GetXPos() - m_vXInitial.at(index));
				dx[1] = ((*iterBead)->GetYPos() - m_vYInitial.at(index));

                // Correct for the PBCs

                if( dx[0] > IGlobalSimBox::Instance()->GetHalfSimBoxXLength() )
                    dx[0] = dx[0] - IGlobalSimBox::Instance()->GetSimBoxXLength();
                else if( dx[0] < -IGlobalSimBox::Instance()->GetHalfSimBoxXLength() )
                    dx[0] = dx[0] + IGlobalSimBox::Instance()->GetSimBoxXLength();

		        if( dx[1] > IGlobalSimBox::Instance()->GetHalfSimBoxYLength() )
			        dx[1] = dx[1] - IGlobalSimBox::Instance()->GetSimBoxYLength();
		        else if( dx[1] < -IGlobalSimBox::Instance()->GetHalfSimBoxYLength() )
			        dx[1] = dx[1] + IGlobalSimBox::Instance()->GetSimBoxYLength();

#if SimDimension == 3
				dx[2] = ((*iterBead)->GetZPos() - m_vZInitial.at(index));

		        if( dx[2] > IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
			        dx[2] = dx[2] - IGlobalSimBox::Instance()->GetSimBoxZLength();
		        else if( dx[2] < -IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
			        dx[2] = dx[2] + IGlobalSimBox::Instance()->GetSimBoxZLength();
#else
                dx[2] = 0.0;
#endif

                dcm += dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];
                
                CM[0] += (*iterBead)->GetXPos();
                CM[1] += (*iterBead)->GetYPos();
                CM[2] += (*iterBead)->GetZPos();

                index++;
			}

            // Now normalise the data by dividing by 6Nt where N is the number of
            // Beads in the target and t is the elapsed time since the start
            // of the calculation (NOT the total simulation time to now). This 
            // assumes that we are calculating normal 3d diffusion. The 
            // time counter cannot be zero or we would not be in this branch.
            // We guard against an integer overflow in the product by casting
            // all intergers to doubles first.

#if SimDimension == 3
           const double denom = 6.0*static_cast<double>(m_BeadTotal)*IGlobalSimBox::Instance()->GetStepSize()*static_cast<double>(simTime - m_Start);
#else
           const double denom = 4.0*static_cast<double>(m_BeadTotal)*IGlobalSimBox::Instance()->GetStepSize()*static_cast<double>(simTime - m_Start);
#endif
            dcm /= denom;

            m_vMSD.push_back(dcm);
            
            CM[0] /= static_cast<double>(m_BeadTotal);
            CM[1] /= static_cast<double>(m_BeadTotal);
            CM[2] /= static_cast<double>(m_BeadTotal);

            // Write the CM data to the decorator state file

	        long dataTotal = 5;

	        CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);

	        pTSD->SetValue(0, simTime, "Time");
	        pTSD->SetValue(1, CM[0],   "XCM");
	        pTSD->SetValue(2, CM[1],   "YCM");
	        pTSD->SetValue(3, CM[2],   "ZCM");
	        pTSD->SetValue(4, dcm,     "MSD");

	        m_pState->AddTimeSeriesData(pTSD);

            // ********************
            // Postconditions

	        if(simTime == m_End)
	        {
                // Write the time series data to file or log a warning message 
                // if it fails

                if(SaveState())
                {
		            new CLogctMSDBeadTarget(simTime, targetLabel, decLabel, m_vMSD.back());
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
            // Ensure the containers are empty

            m_vMSD.clear();
            m_vXInitial.clear();
            m_vYInitial.clear();
            m_vZInitial.clear();

            // Store the initial coordinates of each bead in the target
            // so that we can calculate their displacement over time.
            
		    for(BeadVectorIterator iterBead = m_vBeads.begin(); iterBead != m_vBeads.end(); iterBead++)
		    {
                m_vXInitial.push_back((*iterBead)->GetXPos());
                m_vYInitial.push_back((*iterBead)->GetYPos());
                m_vZInitial.push_back((*iterBead)->GetZPos());
            }

		    // Record the start of the time series sampling. Note that the same message
            // is used to log the start of the calculation and its end, but with
            // different arguments.

		     new CLogctMSDBeadTarget(simTime, targetLabel, decLabel, m_Start, m_End, m_BeadTotal);
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

zInStream& taMSDBeadCM::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taMSDBeadCM::Write(zOutStream& os) const
{
    
    return os;
}

