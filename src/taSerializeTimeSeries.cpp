/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taSerializeTimeSeries.cpp: implementation of the taSerializeTimeSeries class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taSerializeTimeSeries.h"
#include "taTimeSeriesBondCoordinates.h"
#include "IGlobalSimBox.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taSerializeTimeSeries::m_Type = "SerializeTimeSeries";

const zString taSerializeTimeSeries::GetType()
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that creates a label decorator to write out a set of
// time series data to a file.

taSerializeTimeSeries::taSerializeTimeSeries(const zString fileName, const zString label, 
                         CCommandTargetNode* const pDec) : taFileLabel(fileName, label, pDec)
{

}

taSerializeTimeSeries::~taSerializeTimeSeries()
{
    Close();
}

// Non-static member function to return the target's type.

const zString taSerializeTimeSeries::GetTargetType() const
{
    return m_Type;
}


// PVF that writes the data to file. It must be provided by all derived
// classes of taFileLabel that want to be instantiated. The calling object
// is responsible for manipulating the data (normalisation, resetting the histogram)
// this class simply writes out a generic set of histogram data.

bool taSerializeTimeSeries::Serialize()
{
    if(m_pTimeSeries)
    {
        double x = m_pTimeSeries->GetLastXPos();
        double y = m_pTimeSeries->GetLastYPos();
        double z = m_pTimeSeries->GetLastZPos();

        m_outStream << IGlobalSimBox::Instance()->GetCurrentTime() << "  " << x << "  " << y << "  " << z << zEndl;
         return true;
    }
    else
    {
        return false;
    }
}

// Function to store a pointer to a taTimeSeriesBondCoordinates instance so that its
// time series data can be written to a file. Note that this class does not 
// own the histogram object, but just accesses its data.

void taSerializeTimeSeries::SetTimeSeries(taTimeSeriesBondCoordinates* const pTimeSeries)
{
    if(pTimeSeries)
    {
        m_pTimeSeries = pTimeSeries;
    }
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taSerializeTimeSeries::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taSerializeTimeSeries::Write(zOutStream& os) const
{
    
    return os;
}

