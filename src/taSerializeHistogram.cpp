/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taSerializeHistogram.cpp: implementation of the taSerializeHistogram class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taSerializeHistogram.h"
#include "taBinTimeIntervals.h"
#include "IGlobalSimBox.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taSerializeHistogram::m_Type = "SerializeHistogram";

const zString taSerializeHistogram::GetType()
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that creates a label decorator to write out a histogram
// to a file.

taSerializeHistogram::taSerializeHistogram(const zString fileName, const zString label, 
                         CCommandTargetNode* const pDec) : taFileLabel(fileName, label, pDec)
{

}

taSerializeHistogram::~taSerializeHistogram()
{
    Close();
}

// Non-static member function to return the target's type.

const zString taSerializeHistogram::GetTargetType() const
{
    return m_Type;
}

// PVF that writes the data to file. It must be provided by all derived
// classes of taFileLabel that want to be instantiated. The calling object
// is responsible for manipulating the data (normalisation, resetting the histogram)
// this class simply writes out a generic set of histogram data.

bool taSerializeHistogram::Serialize()
{
    if(m_pHistogram)
    {
        m_outStream << zEndl;
        m_outStream << "Time         = " << IGlobalSimBox::Instance()->GetCurrentTime() << zEndl;
        m_outStream << "Bin total    = " << m_pHistogram->GetBinTotal() << zEndl;
        m_outStream << "Bin width    = " << m_pHistogram->GetBinWidth() << zEndl;
        m_outStream << "Lower bound  = " << m_pHistogram->GetLowerBound() << zEndl;
        m_outStream << "Upper bound  = " << m_pHistogram->GetUpperBound() << zEndl;
        m_outStream << "Data points  = " << m_pHistogram->GetSampleTotal() << zEndl << zEndl;
 
        for(long i=0; i<m_pHistogram->GetBinTotal(); i++)
        {
            long number = m_pHistogram->GetBinNumber(i);
            double value = m_pHistogram->GetBinFraction(i);
            m_outStream << i << "  " << number << "  " << value << zEndl;
        }
        return true;
    }
    else
    {
        return false;
    }
}

// Function to store a pointer to a taBinTimeIntervals instance so that its
// histogram data can be written to a file. Note that this class does not 
// own the histogram object, but just accesses its data.

void taSerializeHistogram::SetHistogram(taBinTimeIntervals* const pHistogram)
{
    if(pHistogram)
    {
        m_pHistogram = pHistogram;
    }
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taSerializeHistogram::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taSerializeHistogram::Write(zOutStream& os) const
{
    
    return os;
}

