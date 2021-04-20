/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taBinTimeIntervals.cpp: implementation of the taBinTimeIntervals class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taBinTimeIntervals.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taBinTimeIntervals::m_Type = "BinTimeIntervals";

const zString taBinTimeIntervals::GetType()
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class. We use a helper function to set the data to sensible values.

taBinTimeIntervals::taBinTimeIntervals(const zString label, CCommandTargetNode* const pDec, bool bBinTotal) : taHistogramDecorator(label, pDec), m_bFixedBinTotal(bBinTotal)

{
    // The histogram is constructed using either a fixed number of bins or a fixed
    // bin width. The type chosen cannot be changed during the lifeSpace of the instance.

    if(m_bFixedBinTotal)
    {
        ResetFixedBinTotal(10);   // Use default value for the histogram size, i.e., number of bins
    }
    else
    {
        ResetFixedBinWidth(1.0);  // Use default value for the bin widths
    }

}

taBinTimeIntervals::~taBinTimeIntervals()
{

}

// Non-static member function to return the target's type.

const zString taBinTimeIntervals::GetTargetType() const
{
    return m_Type;
}

// Function used to add a new integer data point. We don't sort the data until
// it has all been added, but just store the values in a temporary container.
// We also adjust the upper and lower bounds according to the data.

void taBinTimeIntervals::AddDataPoint(double x)
{
    // On first entry we just store the value ready for the next time.
    // This assumes that -1 is not a valid value!

    if(m_OldValue < 0.0)
    {
        m_OldValue = x;
    }
    else
    {
        double interval = x - m_OldValue;
        m_OldValue = x;

        m_Data.push_back(interval);

        if(interval > m_XMax)
            m_XMax = interval;
        else if(interval < m_XMin)
            m_XMin = interval;
    }
}

// Function to construct a histogram out of the data entered so far. Note that
// one of the sub-class's helper functions ResetFixedBinTotal() or ResetFixedBinWidth() must
// be called before trying to normalise the data as they define the number of bins
// or the bin width that are used in this function. We do resize the histogram here
// and initialise its elements to zero.
//
// We store the bin data as integer and fractional values. We don't empty the bins
// in case we want to add more data later.  The helper reset functions are used to
// reset the histogram.

void taBinTimeIntervals::Normalise(double norm)
{
/*
 long j; // counter user below

 if(m_bFixedBinTotal)
    {
        m_Bins.resize(m_BinTotal, 0.0);
        m_XWidth = static_cast<double>((m_XMax - m_XMin)*(1 + 2*m_BinTotal))/static_cast<double>(2*m_BinTotal*m_BinTotal);

        std::cout << "Fixed bin total: total, min, max values, binWidth " << m_BinTotal << " " << m_XMin << " " << m_XMax << " " << m_XWidth <<  zEndl;
        
        // Sort the data into the histogram. If there is no data all the bins
        // are zero.

        if(GetSampleTotal() > 0)
        {
            for(j=0; j<GetSampleTotal(); j++)
            {
                long interval = m_Data.at(j);
                long pointer  = static_cast<long>(interval/static_cast<long>(m_XWidth));

                if(pointer >= 0 && pointer < m_BinTotal)
                {
                    m_Bins.at(pointer) += 1;
                }
                else if(pointer == m_BinTotal)
                {
                    pointer--;
                    m_Bins.at(pointer) += 1;
                }
            }
        }
    }
    else
    {
        // Sort the data into the histogram. If there is no data we create a single
        // bin with contents of zero and a width as specified by the user.
        
        std::cout << "Fixed bin width: total,  min, max values, binWidth " << m_BinTotal << " " << m_XMin << " " << m_XMax << " " << m_XWidth <<  zEndl;
        
        if(GetSampleTotal() > 0)
        {
            const double x = 2.0*m_XWidth/static_cast<double>(m_XMax - m_XMin);

            m_BinTotal = static_cast<long>((1.0 + sqrt(1.0 + x))/x);
            m_Bins.resize(m_BinTotal, 0.0);
            
            std::cout << "Setting m_BinTotal = " << m_BinTotal <<  " bin width = " << m_XWidth << " with sample total = " << GetSampleTotal() << zEndl;

            for(j=0; j<GetSampleTotal(); j++)
            {
                long pointer  = static_cast<long>(m_Data.at(j)/m_XWidth);
                
                std::cout << "sample no " << j << " " << m_Data.at(j) << " " << pointer << zEndl;


                if(pointer >= 0 && pointer < m_BinTotal)
                {
                    std::cout << "adding point to bin " << pointer << zEndl;
                    m_Bins.at(pointer) += 1;
                }
                else if(pointer == m_BinTotal)
                {
                    std::cout << "Point in final bin" << zEndl;
                    pointer--;
                    m_Bins.at(pointer) += 1;
                }
            }
        }
        else
        {
            m_BinTotal = 1;
            m_Bins.resize(m_BinTotal, 0.0);
        }
    }
    
    */
}

// Function to reset the calculation of the histogram given that the
// number of bins is constant. The only required parameter is the number of bins
// into which to bin the data. If an invalid value is supplied, we use a single bin.
// We set the size of the m_Bins container in the normalisation functions below.
// The m_Data container is initialised to zero size as we use push_back to add the data.

void taBinTimeIntervals::ResetFixedBinTotal(long bins)
{
    if(bins > 0)
    {
        m_BinTotal = bins;
    }
    else
    {
        m_BinTotal = 1;
    }

    m_XMin     = 0.0;
    m_XMax     = 1.0;
    m_OldValue = -1.0;
    m_XWidth = static_cast<double>((m_XMax - m_XMin)*(1 + 2*m_BinTotal))/static_cast<double>(2*m_BinTotal*m_BinTotal);

    m_Data.clear();
    m_Bins.clear();
}

// Function to reset the calculation of the histogram given that the
// width of the bins is constant. The only required parameter is the bin width.
// If an invalid value is supplied, we use a single bin.

void taBinTimeIntervals::ResetFixedBinWidth(double width)
{
    if(width > 0.0)
    {
        m_XWidth = width;
    }
    else
    {
        m_XWidth = 1.0;
    }

    m_XMin     = 0.0;
    m_XMax     = 1.0;
    m_OldValue = -1.0;

    // Define a width such that it gives the same number of bins as the
    // ResetFixedBinTotal() function for the same bin width.

    const double x = 2.0*m_XWidth/static_cast<double>(m_XMax - m_XMin);

    m_BinTotal = static_cast<long>((1.0 + sqrt(1.0 + x))/x);

    m_Data.clear();
    m_Bins.clear();
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taBinTimeIntervals::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taBinTimeIntervals::Write(zOutStream& os) const
{
    
    return os;
}

