/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taRotationalMSDPolymer.cpp: implementation of the taRotationalMSDPolymer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taRotationalMSDPolymer.h"
#include "xxDecoratorState.h"
#include "CommandTargetPolymer.h"
#include "Polymer.h"
#include "TimeSeriesData.h"
#include "IGlobalSimBox.h"
#include "LogctRotationalMSDPolymerTarget.h"
#include "LogctNonexistentDecorator.h"
#include "LogTextWarningMessage.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taRotationalMSDPolymer::m_Type = "RotationalMSDPolymer";

const zString taRotationalMSDPolymer::GetType()
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

taRotationalMSDPolymer::taRotationalMSDPolymer(const zString label, CCommandTargetNode* const pDec,
							    long start, long end) : taCumulateDecorator(label, pDec, "", start, end),
                                m_pPolymerTarget(0)
{
	pDec->SetOuterDecorator(this);

    // Store a pointer to the actual polymer target for later use

    m_pPolymerTarget = dynamic_cast<CCommandTargetPolymer*>(GetActiveCommandTarget());

    m_vPolymers.clear();
	m_vPolymers = m_pPolymerTarget->GetPolymers();

    m_PolymerTotal = m_vPolymers.size();

    // Ensure the containers are empty

    m_vXInitial.clear();
    m_vYInitial.clear();
    m_vZInitial.clear();

#if EnableXMLDecorators == SimXMLEnabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId() + ".xml", GetStartTime(), GetEndTime(), IGlobalSimBox::Instance()->GetRunId(), "MSDPolymerCM"));

	// Note that the opening <Header> tag is written in the xxDecoratorState class
	// but the closing tag is written here. This is a hack.

	zOutStream& os = m_pState->putXMLStartTags();
	os << "<TargetLabel>"		 << GetActiveCommandTarget()->GetLabel() << "</TargetLabel>";
    os << "<DecoratorLabel>"     << GetLabel() << "</DecoratorLabel>";
	os << "</Header>" << zEndl;

#elif EnableXMLDecorators == SimXMLDisabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId(), GetStartTime(), GetEndTime(), IGlobalSimBox::Instance()->GetRunId(), "MSDPolymerCM"));

	zOutStream& os = m_pState->putASCIIStartTags();
	os << "    Target	    " << GetActiveCommandTarget()->GetLabel() << zEndl;
    os << "    Decorator   " << GetLabel() << zEndl;

#endif


}

taRotationalMSDPolymer::~taRotationalMSDPolymer()
{

}

// Non-static member function to return the target's type.

const zString taRotationalMSDPolymer::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to calculate and serialise the MSD of all polymers
// contained in a polymer target using their CM coordinates. This is in contrast
// to the bead coordinate MSD that is calculated in the CMonitor per bead type.
// We use the polymer class' GetCM() function on each polymer contained in the
// target and average their coordinates at each time-step. Then we serialise the
// time series data to file when the sampling period ends.
//
// Note that the number, type and order of polymers in the target must NOT change
// while this decorator is calculating, otherwise the results will be wrong.
// The initial coordinates of the polymers' CM are stored using their order
// of storage in the target to index them.
//
// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
// The innermost command target object just holds the beads, so the chain stops when 
// its do-nothing Execute() function is invoked.
//

void taRotationalMSDPolymer::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

	const zString targetLabel = GetActiveCommandTarget()->GetLabel();
	const zString decLabel    = GetLabel();

    // If the target is not a polymer target or is empty, we log a warning message 
    // ang ignore the decorator.

	if(m_pPolymerTarget && m_PolymerTotal > 0)
	{
	    if(simTime > m_Start && simTime <= m_End)
	    {
            double dx[3];
			double dcm = 0.0;

            long index = 0;
			for(PolymerVectorIterator iterPoly = m_vPolymers.begin(); iterPoly != m_vPolymers.end(); iterPoly++)
			{
                aaVector vcm = (*iterPoly)->GetCM();

				dx[0] = (vcm.GetX() - m_vXInitial.at(index));
				dx[1] = (vcm.GetY() - m_vYInitial.at(index));

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
				dx[2] = (vcm.GetZ() - m_vZInitial.at(index));

		        if( dx[2] > IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
			        dx[2] = dx[2] - IGlobalSimBox::Instance()->GetSimBoxZLength();
		        else if( dx[2] < -IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
			        dx[2] = dx[2] + IGlobalSimBox::Instance()->GetSimBoxZLength();
#else
                dx[2] = 0.0;
#endif

                dcm += dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2];

                index++;
			}

            // Now normalise the data by dividing by 6Nt where N is the number of
            // polymers in the target and t is the elapsed time since the start
            // of the calculation (NOT the total simulation time to now). This 
            // assumes that we are calculating normal 3d diffusion. The 
            // time counter cannot be zero or we would not be in this branch.
            // We guard against an integer overflow in the product by casting
            // all intergers to doubles first.

#if SimDimension == 3
           const double denom = 6.0*static_cast<double>(m_PolymerTotal)*IGlobalSimBox::Instance()->GetStepSize()*static_cast<double>(simTime - m_Start);
#else
           const double denom = 4.0*static_cast<double>(m_PolymerTotal)*IGlobalSimBox::Instance()->GetStepSize()*static_cast<double>(simTime - m_Start);
#endif
            dcm /= denom;

            m_vMSD.push_back(dcm);

            // Write the CM data to the decorator state file

	        long dataTotal = 2;

	        CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);

	        pTSD->SetValue(0, simTime, "Time");
	        pTSD->SetValue(1, dcm,     "CM");

	        m_pState->AddTimeSeriesData(pTSD);

            // ********************
            // Postconditions

	        if(simTime == m_End)
	        {
                // Write the time series data to file or log a warning message 
                // if it fails

                if(SaveState())
                {
		             new CLogctRotationalMSDPolymerTarget(simTime, targetLabel, decLabel, m_vMSD.back());
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

            // Store the initial CM coordinates of each polymer in the target 
            // so that we can calculate their displacement over time. Note that
            // we use the PBC coordinates and correct them for possible
            // boundary crossings within a polymer.

		    for(PolymerVectorIterator iterPoly = m_vPolymers.begin(); iterPoly != m_vPolymers.end(); iterPoly++)
		    {
                aaVector vcm = (*iterPoly)->GetCM();

                m_vXInitial.push_back(vcm.GetX());
                m_vYInitial.push_back(vcm.GetY());
                m_vZInitial.push_back(vcm.GetZ());
            }

		    // Record the start of the time series sampling. Note that the same message
            // is used to log the start of the calculation and its end, but with
            // different arguments.

		     new CLogctRotationalMSDPolymerTarget(simTime, targetLabel, decLabel,  
												    m_Start, m_End, m_PolymerTotal);
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

zInStream& taRotationalMSDPolymer::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taRotationalMSDPolymer::Write(zOutStream& os) const
{
    
    return os;
}

