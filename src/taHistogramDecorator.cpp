/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taHistogramDecorator.cpp: implementation of the taHistogramDecorator class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taHistogramDecorator.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class. We use a helper function to set the data to sensible values.

taHistogramDecorator::taHistogramDecorator(const zString label, CCommandTargetNode* const pDec) : taLabelDecorator(label, pDec),
            m_XMin(0.0), m_XMax(0.0), m_XWidth(0.0)
{
    m_Data.clear();
    m_Bins.clear();
    m_BinFractions.clear();
}

taHistogramDecorator::~taHistogramDecorator()
{

}

// Function to return the integer number of entries in the ith bin. If i is outside 
// the range of the histogram, we return 0.

long taHistogramDecorator::GetBinNumber(long i) const
{
    long value = 0;

    if(i >= 0 && i < GetBinTotal())
    {
        value = m_Bins.at(i);
    }

    return value;
}

// Function to return the fraction of entries in the ith bin.

double taHistogramDecorator::GetBinFraction(long i) const
{
    double value = 0.0;

    if(i >= 0 && i < GetBinTotal())
    {
        value = m_BinFractions.at(i);
    }

    return value;
}

// Function to allow derived classes to normalise the values stored. We don't do it in this class as different histograms
// may use different types of normalisation, and how the data is normalised is not a property of a histogram.

void taHistogramDecorator::SetBinFraction(long i, double newValue)
{
    if(i >= 0 && i < GetBinTotal())
    {
        m_BinFractions.at(i) = newValue;
    }
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taHistogramDecorator::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taHistogramDecorator::Write(zOutStream& os) const
{
    
    return os;
}

