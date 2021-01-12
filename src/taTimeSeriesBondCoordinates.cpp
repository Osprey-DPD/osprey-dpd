/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taTimeSeriesBondCoordinates.cpp: implementation of the taTimeSeriesBondCoordinates class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taTimeSeriesBondCoordinates.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taTimeSeriesBondCoordinates::m_Type = "TimeSeriesBondCoordinates";

const zString taTimeSeriesBondCoordinates::GetType()
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class. We use a helper function to initialise the time series containers.

taTimeSeriesBondCoordinates::taTimeSeriesBondCoordinates(const zString label, CCommandTargetNode* const pDec) : taLabelDecorator(label, pDec)
{
    Reset();
}

taTimeSeriesBondCoordinates::~taTimeSeriesBondCoordinates()
{

}

// Non-static member function to return the target's type.

const zString taTimeSeriesBondCoordinates::GetTargetType() const
{
    return m_Type;
}

// Function to reset the time series data storage.

void taTimeSeriesBondCoordinates::Reset()
{
    m_XData.clear();
    m_YData.clear();
    m_ZData.clear();
}

// Function used to add a new set of bond (x,y,z) coordinates.

void taTimeSeriesBondCoordinates::AddDataPoint(double x, double y, double z)
{
    m_XData.push_back(x);
    m_YData.push_back(y);
    m_ZData.push_back(z);
}

// Functions to return the x, y, z coordinates from the time series data
// at any point and the last values stored.

double taTimeSeriesBondCoordinates::GetXPos(long i) const
{
    if(i >= 0 && i < static_cast<long>(m_XData.size()))
    {
        return m_XData.at(i);
    }
    else
    {
        return 0.0;
    }
}

double taTimeSeriesBondCoordinates::GetYPos(long i) const
{
    if(i >= 0 && i < static_cast<long>(m_YData.size()))
    {
        return m_YData.at(i);
    }
    else
    {
        return 0.0;
    }
}

double taTimeSeriesBondCoordinates::GetZPos(long i) const
{
    if(i >= 0 && i < static_cast<long>(m_ZData.size()))
    {
        return m_ZData.at(i);
    }
    else
    {
        return 0.0;
    }
}

double taTimeSeriesBondCoordinates::GetLastXPos() const
{
    return m_XData.at(GetSampleTotal()-1);
}

double taTimeSeriesBondCoordinates::GetLastYPos() const
{
    return m_YData.at(GetSampleTotal()-1);
}

double taTimeSeriesBondCoordinates::GetLastZPos() const
{
    return m_ZData.at(GetSampleTotal()-1);
}

// Unused function at the moment.

void taTimeSeriesBondCoordinates::Normalise()
{
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taTimeSeriesBondCoordinates::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taTimeSeriesBondCoordinates::Write(zOutStream& os) const
{
    
    return os;
}

