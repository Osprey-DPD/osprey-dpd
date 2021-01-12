/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taSerialiseBondCoordinatesEventAnalysis.cpp: implementation of the taSerialiseBondCoordinatesEventAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taSerialiseBondCoordinatesEventAnalysis.h"
#include "taTimeSeriesBondCoordinates.h"
#include "taSerializeTimeSeries.h"
#include "IEADSerialiseBondCoordinates.h"
#include "IGlobalSimBox.h"
#include "LogTextWarningMessage.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taSerialiseBondCoordinatesEventAnalysis::m_Type = "SerialiseBondCoordinatesEventAnalysis";

const zString taSerialiseBondCoordinatesEventAnalysis::GetType()
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that passes on the decorator label, and a pointer to the 
// command target being wrapped by the decorator, to the CCommandTargetNode
// base class. Note that the outer decorator label for the wrapped object is
// set in the taEventAnalysisDecorator class' constructor.
// We pass the "period" parameter to the base class to store in its "samples"
// membrane variable. This is the number of time steps between writing the data
// to file.

taSerialiseBondCoordinatesEventAnalysis::taSerialiseBondCoordinatesEventAnalysis(const zString label, CCommandTargetNode* const pDec,
                                                                       long period) : taEventAnalysisDecorator(label, pDec, period),
                                                                        m_PreviousSampleTotal(0),
                                                                        m_pFile(0)
{

    m_pTimeSeries = new taTimeSeriesBondCoordinates(label+"_timeseries", GetOutermostDecorator());

    // Store a taFileLabel object to serialize the time series data to file.

    const zString fileName = xxBase::GetEADPrefix() + label + "." + IGlobalSimBox::Instance()->GetRunId();

    m_pFile = new taSerializeTimeSeries(fileName, label+"_timeseries_output", GetOutermostDecorator());
    m_pFile->SetTimeSeries(m_pTimeSeries);
}

taSerialiseBondCoordinatesEventAnalysis::~taSerialiseBondCoordinatesEventAnalysis()
{
    // We don't destroy the m_pTimeSeries and m_pFile instances here as they are
    // derived from CCommandTargetNode and are owned by the CSimBox as  
    // command target decorators.
}

// Non-static member function to return the target's type.

const zString taSerialiseBondCoordinatesEventAnalysis::GetTargetType() const
{
    return m_Type;
}

// Function called by the SimBox to update the decorator state. All classes
// derived from taEventSourceAnalysis keep track of how many samples
// they have taken and normalise their data according to the value defined in
// the base class function GetSamplesPerAnalysis()
// Even though a time series does not require "normalisation" we use the counter
// to determine how often the data are written to file. Hence, the normalise
// function just has to write out a single data point.
// We also keep a count of the number of data points in the time series so that
// we only write out the data when a new point is added. 

void taSerialiseBondCoordinatesEventAnalysis::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

    IncrementCounter();

    if(IsTimeToNormalise() && m_pTimeSeries->GetSampleTotal() > m_PreviousSampleTotal)
    {
       m_PreviousSampleTotal = m_pTimeSeries->GetSampleTotal();
       Normalise();
    }

}

// Function used to add new event data from an ESD to the analyse performed
// by this EAD. We get a pointer to the ESD interface (IESDBinSuccessIntervals)
// that gives us access data needed by this EAD. We query the ESD and ask it for 
// a pointer to the desirved interface using its 
// short name (BinSuccessIntervals) as a key. The correspondence between the key
// and the interface class name is in the prefix IESDXXX.

void taSerialiseBondCoordinatesEventAnalysis::AnalyseEventState()
{
    IEADSerialiseBondCoordinates* const pIEAD = dynamic_cast<IEADSerialiseBondCoordinates*>(m_pIEAD);

    if(pIEAD)
    {
        m_pTimeSeries->AddDataPoint(pIEAD->GetXPos(), pIEAD->GetYPos(), pIEAD->GetZPos());
    }
}


// PVF to create and store a pointer to an interface class that wraps the
// EAD's source ESD instance. If an instance already exists, we return it using
// a base class pointer. Because the interface class needs to store an instance of
// a class derived from the taEventSourceDecorator class, we perform the cast here.

IEAD* taSerialiseBondCoordinatesEventAnalysis::ConnectESDInterface(taEventSourceDecorator* const pESD)
{
    if(!m_pIEAD)
    {
        IEADSerialiseBondCoordinates*  pIEAD = new IEADSerialiseBondCoordinates(pESD);
        m_pIEAD = pIEAD;
    }

    return m_pIEAD;
}

// Function called by the wrapped ESD to get the EAD to normalise its data and
// write the current data to a file. Although the time series data does not 
// need to be "normalised", we use this PVF simply to serialize the data to file.

void taSerialiseBondCoordinatesEventAnalysis::Normalise()
{
    if(!m_pFile->Serialize())
    {
         new CLogTextWarningMessage(IGlobalSimBox::Instance()->GetCurrentTime(), "Error serialising time series data in decorator " + GetLabel() + " wrapping target " + GetActiveCommandTarget()->GetLabel());
    }
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taSerialiseBondCoordinatesEventAnalysis::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taSerialiseBondCoordinatesEventAnalysis::Write(zOutStream& os) const
{
    
    return os;
}

