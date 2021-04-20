/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taBinCylinderSpaceCoordinates.cpp: implementation of the taBinCylinderSpaceCoordinates class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taBinCylinderSpaceCoordinates.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taBinCylinderSpaceCoordinates::m_Type = "BinCylinderSpaceCoordinates";

const zString taBinCylinderSpaceCoordinates::GetType()
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class. We select the fixed bin width version of the base class by passing false
// as the last but one argument.

taBinCylinderSpaceCoordinates::taBinCylinderSpaceCoordinates(const zString label, CCommandTargetNode* const pDec,
    long binTotal, double binWidth) : taHistogramDecorator(label, pDec)
{
    m_XWidth = binWidth;
}

taBinCylinderSpaceCoordinates::~taBinCylinderSpaceCoordinates()
{

}

// Non-static member function to return the target's type.

const zString taBinCylinderSpaceCoordinates::GetTargetType() const
{
    return m_Type;
}

// Function to add a new data point. We don't sort the data until  it has all been added,
// but just store the values in a container. We adjust the upper and lower bounds according to the data.
// Note that this routine differs from taBinTimeIntervals because it stores each data value independently
// instead of calculating the intervals between successive data points.

void taBinCylinderSpaceCoordinates::AddDataPoint(double x)
{
    m_Data.push_back(x);

    if(x > m_XMax)
        m_XMax = x;
    else if(x < m_XMin)
        m_XMin = x;
}

// Function to construct a histogram out of the data entered to the time at which this function is called.
// We also normalise it by the area of each cylindrical shell and the number of times the function has
// been called. Note that because using static_cast<long> truncates floats instead of rounding them, we add
// half a bin width before truncating.

void taBinCylinderSpaceCoordinates::Normalise(double norm)
{
    long j; // counter user below

    // Sort the data into the histogram. If there is no data we create a single
    // bin with contents of zero and a width as specified by the user.
                    
    if(GetSampleTotal() > 0)
    {
        const double x = 2.0*m_XWidth/static_cast<double>(m_XMax - m_XMin);

        long binTotal = static_cast<long>((1.0 + sqrt(1.0 + x))/x);
        m_Bins.resize(binTotal, 0);
        m_BinFractions.resize(binTotal, 0.0);

        for(j=0; j<GetSampleTotal(); ++j)
        {
            long pointer  = static_cast<long>((m_Data.at(j)/m_XWidth) + 0.5);
                
 //           std::cout << "sample no " << j << " " << m_Data.at(j) << " " << pointer << zEndl;

            if(pointer >= 0 && pointer < GetBinTotal())
            {
                 m_Bins.at(pointer) += 1;
            }
            else if(pointer == GetBinTotal())
            {
                pointer--;
                m_Bins.at(pointer) += 1;
            }
        }
        
        // Now we have the integer number of points that lie in each cylindrical shell we normalise them
        // according to the number of calls, the area of each shell, and the total number of data points.
        
        if(norm > 0.0)
        {
            const double fixedNorm = norm*xxBase::m_globalPI*m_XWidth*m_XWidth*static_cast<double>(GetSampleTotal());
            
 //           std::cout << "Normalising space data with fixedNorm = " << fixedNorm << " and bins " << GetBinTotal() << zEndl;
            
            for(j=0; j<GetBinTotal(); ++j)
            {
                const double newValue = static_cast<double>(GetBinNumber(j))/(static_cast<double>(2*j + 1)*fixedNorm);
                SetBinFraction(j, newValue);
                
//                std::cout << "Bin, value, newValue = " << j << " " << GetBinNumber(j) << " " << GetBinFraction(j) << zEndl;
                               
            }
        }
    }
    else
    {
        m_Bins.resize(1, 0);
    }
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taBinCylinderSpaceCoordinates::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taBinCylinderSpaceCoordinates::Write(zOutStream& os) const
{
    
    return os;
}

