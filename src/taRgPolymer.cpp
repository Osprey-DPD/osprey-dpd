/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taRgPolymer.cpp: implementation of the taRgPolymer class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taRgPolymer.h"
#include "xxDecoratorState.h"
#include "CommandTargetPolymer.h"
#include "Polymer.h"
#include "TimeSeriesData.h"
#include "IGlobalSimBox.h"
#include "LogctRgPolymerTarget.h"
#include "LogctNonexistentDecorator.h"
#include "LogTextWarningMessage.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taRgPolymer::m_Type = "RgPolymer";

const zString taRgPolymer::GetType()
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

taRgPolymer::taRgPolymer(const zString label, CCommandTargetNode* const pDec,
							    long start, long end) : taCumulateDecorator(label, pDec, "", start, end),
                                m_pPolymerTarget(0)
{
	pDec->SetOuterDecorator(this);

    // Store a pointer to the actual polymer target for later use

    m_pPolymerTarget = dynamic_cast<CCommandTargetPolymer*>(GetActiveCommandTarget());

    m_vPolymers.clear();
	m_vPolymers = m_pPolymerTarget->GetPolymers();

    m_PolymerTotal = m_vPolymers.size();

    // Set the observables to zero

    m_CM[0] = 0.0;
    m_CM[1] = 0.0;
    m_CM[2] = 0.0;
    m_Rg    = 0.0;
    
#if EnableXMLDecorators == SimXMLEnabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId() + ".xml", GetStartTime(), GetEndTime(), IGlobalSimBox::Instance()->GetRunId(), "RgPolymer"));

	// Note that the opening <Header> tag is written in the xxDecoratorState class
	// but the closing tag is written here. This is a hack.

	zOutStream& os = m_pState->putXMLStartTags();
	os << "<TargetLabel>"		 << GetActiveCommandTarget()->GetLabel() << "</TargetLabel>";
    os << "<DecoratorLabel>"     << GetLabel() << "</DecoratorLabel>";
	os << "</Header>" << zEndl;

#elif EnableXMLDecorators == SimXMLDisabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId(), GetStartTime(), GetEndTime(), IGlobalSimBox::Instance()->GetRunId(), "RgPolymer"));

	zOutStream& os = m_pState->putASCIIStartTags();
	os << "    Target	    " << GetActiveCommandTarget()->GetLabel() << zEndl;
    os << "    Decorator   " << GetLabel() << zEndl;

#endif


}

taRgPolymer::~taRgPolymer()
{

}

// Non-static member function to return the target's type.

const zString taRgPolymer::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to calculate and serialise the radius of gyration of all polymers
// contained in a polymer target using their CM coordinates. We first calculate the instantaneous CM
// of the polymers, and then find the scalar Rg for all polymers using their CM.
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

void taRgPolymer::Execute(long simTime)
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
            m_CM[0] = 0.0;
            m_CM[1] = 0.0;
            m_CM[2] = 0.0;

			for(PolymerVectorIterator iterPoly = m_vPolymers.begin(); iterPoly != m_vPolymers.end(); iterPoly++)
			{
                aaVector vcm = (*iterPoly)->GetCM();
                
                m_CM[0] += vcm.GetX();
                m_CM[1] += vcm.GetY();
                m_CM[2] += vcm.GetZ();
			}
            
            m_CM[0] /= static_cast<double>(m_PolymerTotal);
            m_CM[1] /= static_cast<double>(m_PolymerTotal);
            m_CM[2] /= static_cast<double>(m_PolymerTotal);
            
            // Now loop over the polymers again and calculate the radius of gyration using their individual CM coordinates.
            // Note that if there is only 1 polymer, the Rg will be zero if we use the CPolymer's GetCM function.
            
            m_Rg = 0.0;
            
            for(PolymerVectorIterator iterPoly = m_vPolymers.begin(); iterPoly != m_vPolymers.end(); iterPoly++)
            {
                aaVector vcm = (*iterPoly)->GetCM();
                
                m_Rg += (vcm.GetX() - m_CM[0])*(vcm.GetX() - m_CM[0]);
                m_Rg += (vcm.GetY() - m_CM[1])*(vcm.GetY() - m_CM[1]);
                m_Rg += (vcm.GetZ() - m_CM[2])*(vcm.GetZ() - m_CM[2]);
            }
            
            m_Rg = sqrt(m_Rg/static_cast<double>(m_PolymerTotal));
                        
            // Write the CM data to the decorator state file

	        long dataTotal = 5;

	        CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);

	        pTSD->SetValue(0, simTime,  "Time");
	        pTSD->SetValue(1, m_CM[0],  "XCM");
	        pTSD->SetValue(2, m_CM[1],  "YCM");
	        pTSD->SetValue(3, m_CM[2],  "ZCM");
	        pTSD->SetValue(4, m_Rg,     "Rg");

	        m_pState->AddTimeSeriesData(pTSD);

            // ********************
            // Postconditions

	        if(simTime == m_End)
	        {
                // Write the time series data to file or log a warning message 
                // if it fails

                if(SaveState())
                {
		            new CLogctRgPolymerTarget(simTime, targetLabel, decLabel, m_CM[0], m_CM[1], m_CM[2], m_Rg);
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
            // Preconditions - None

		    // Record the start of the time series sampling. Note that the same message
            // is used to log the start of the calculation and its end, but with
            // different arguments.

		    new CLogctRgPolymerTarget(simTime, targetLabel, decLabel,
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

zInStream& taRgPolymer::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taRgPolymer::Write(zOutStream& os) const
{
    
    return os;
}

