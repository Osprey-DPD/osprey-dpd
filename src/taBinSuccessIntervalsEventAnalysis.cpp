/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taBinSuccessIntervalsEventAnalysis.cpp: implementation of the taBinSuccessIntervalsEventAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taBinSuccessIntervalsEventAnalysis.h"
#include "IEADBinSuccessIntervals.h"
#include "taBinTimeIntervals.h"
#include "taSerializeHistogram.h"
#include "IGlobalSimBox.h"
#include "LogTextWarningMessage.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taBinSuccessIntervalsEventAnalysis::m_Type = "BinSuccessIntervalsEventAnalysis";

const zString taBinSuccessIntervalsEventAnalysis::GetType()
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
// We check that only one of the bin total or the bin width is non-zero, otherwise
// we assume the bin total is the value to use.

taBinSuccessIntervalsEventAnalysis::taBinSuccessIntervalsEventAnalysis(const zString label, CCommandTargetNode* const pDec,
                                                                       long bins, double binWidth, long samples) : taEventAnalysisDecorator(label, pDec, samples),
                                                                        m_BinTotal(bins),
                                                                        m_BinWidth(binWidth),
                                                                        m_pHistogram(0),
                                                                        m_pFile(0)
{
    // We don't add the contained taBinTimeIntervals decorator to the
    // set of active command targets as it has no Execute() function and does
    // not belong in the chain, but rather as a sub-object of this class.
    // The last argument indicates that the number of bins is fixed.

    if(m_BinTotal != 0)
    {
//        std::cout << "Fixed bins " << m_BinTotal << "  " << m_BinWidth << zEndl;
        m_pHistogram = new taBinTimeIntervals(label+"_hist", GetOutermostDecorator(), true);

        // Reset the histogram object to use the required number of bins. Note that
        // we don't have to supply the number of data points as it calculates the
        // results once all data has been entered. 

        m_pHistogram->ResetFixedBinTotal(m_BinTotal);
    }
    else
    {
//        std::cout << "Fixed width " << m_BinTotal << "  " << m_BinWidth << zEndl;
        m_pHistogram = new taBinTimeIntervals(label+"_hist", GetOutermostDecorator(), false);
        m_pHistogram->ResetFixedBinWidth(m_BinWidth);
    }

    // Store a taFileLabel object to serialize the histogram data to file.

    const zString fileName = xxBase::GetEADPrefix() + label + "." + IGlobalSimBox::Instance()->GetRunId();

    m_pFile = new taSerializeHistogram(fileName, label+"_hist_output", GetOutermostDecorator());
    m_pFile->SetHistogram(m_pHistogram);
}

taBinSuccessIntervalsEventAnalysis::~taBinSuccessIntervalsEventAnalysis()
{
    // We don't destroy the m_pHistogram and m_pFile instances here as they are
    // derived from CCommandTargetNode and re owned by the CSimBox as  
    // command target decorators.
}

// Non-static member function to return the target's type.

const zString taBinSuccessIntervalsEventAnalysis::GetTargetType() const
{
    return m_Type;
}

// Function called by the SimBox to update the decorator state. All classes
// derived from taEventSourceAnalysis keep track of how many samples
// they have taken and normalise their data according to the value defined in
// the base class function GetSamplesPerAnalysis()

void taBinSuccessIntervalsEventAnalysis::Execute(long simTime)
{
	GetInnerDecorator()->Execute(simTime);

    IncrementCounter();

    if(IsTimeToNormalise())
    {
//       std::cout << "normalising histogram at time " << simTime << zEndl;
       Normalise();
    }

}

// Function used to add new event data from an ESD to the analyse performed
// by this EAD. We get a pointer to the ESD interface (IESDBinSuccessIntervals)
// that gives us access data needed by this EAD. We query the ESD and ask it for 
// a pointer to the desirved interface using its 
// short name (BinSuccessIntervals) as a key. The correspondence between the key
// and the interface class name is in the prefix IESDXXX.

void taBinSuccessIntervalsEventAnalysis::AnalyseEventState()
{
    IEADBinSuccessIntervals* const pIEAD = dynamic_cast<IEADBinSuccessIntervals*>(m_pIEAD);

    if(pIEAD)
    {
        long time = pIEAD->GetTime();

        m_pHistogram->AddDataPoint(time);
    }
}


// PVF to create and store a pointer to an interface class that wraps the
// EAD's source ESD instance. If an instance already exists, we return it using
// a base class pointer.

IEAD* taBinSuccessIntervalsEventAnalysis::ConnectESDInterface(taEventSourceDecorator* const pESD)
{
    if(!m_pIEAD)
    {
        IEADBinSuccessIntervals*  pIEAD = new IEADBinSuccessIntervals(pESD);
        m_pIEAD = pIEAD;
    }

    return m_pIEAD;
}

// Function called by the wrapped ESD to get the EAD to normalise its data and
// write the current histogram to a file. We then reset the histogram ready for
// the next data set.

void taBinSuccessIntervalsEventAnalysis::Normalise()
{
    m_pHistogram->Normalise(0);

    if(!m_pFile->Serialize())
    {
         new CLogTextWarningMessage(IGlobalSimBox::Instance()->GetCurrentTime(), "Error serialising data from histogram calculation in decorator " + GetLabel() + " wrapping target " + GetActiveCommandTarget()->GetLabel());
    }

    if(m_BinTotal > 0)
    {
        m_pHistogram->ResetFixedBinTotal(m_BinTotal);
    }
    else
    {
        m_pHistogram->ResetFixedBinWidth(m_BinWidth);
    }
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taBinSuccessIntervalsEventAnalysis::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taBinSuccessIntervalsEventAnalysis::Write(zOutStream& os) const
{
    
    return os;
}

