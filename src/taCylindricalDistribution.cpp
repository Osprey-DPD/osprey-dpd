/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taCylindricalDistribution.cpp: implementation of the taCylindricalDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taCylindricalDistribution.h"
#include "CommandTargetPolymer.h"
#include "CommandTargetComposite.h"
#include "Polymer.h"
#include "taBinCylinderSpaceCoordinates.h"
#include "taSerializeHistogram.h"
#include "IGlobalSimBox.h"
#include "LogctNonexistentDecorator.h"
#include "LogTextWarningMessage.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taCylindricalDistribution::m_Type = "CylindricalDistribution";

const zString taCylindricalDistribution::GetType()
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

taCylindricalDistribution::taCylindricalDistribution(const zString label, CCommandTargetNode* const pDec, long start, long end,
                                           long samples, long xn, long yn, long zn, double xc, double yc, double zc,
                                           double shellWidth, double outerRadius) : taCumulateDecorator(label, pDec, "", start, end),
                                           m_Samples(samples), m_XN(xn), m_YN(yn), m_ZN(zn),
                                           m_XC(xc), m_YC(yc), m_ZC(zc),
                                           m_ShellWidth(shellWidth), m_OuterRadius(outerRadius),
                                           m_BinTotal(0), m_TimesCalled(0), m_ShellArea(0.0),
                                           m_pHistogram(0), m_pFile(0),
                                           m_pPolymerTarget(0), m_pCompositeTarget(0)
                                           
{
    pDec->SetOuterDecorator(this);

    // Clear containers
    
    m_vPolymers.clear();
    m_Targets.clear();

    // Store a pointer to the target depending on whether it is a simple polymer target or a composite target.
    
    m_pPolymerTarget   = dynamic_cast<CCommandTargetPolymer*>(GetActiveCommandTarget());
    m_pCompositeTarget = dynamic_cast<CCommandTargetComposite*>(GetActiveCommandTarget());
    
    if(m_pPolymerTarget)
    {
        m_vPolymers = m_pPolymerTarget->GetPolymers();
    
        std::cout << "Analysing polymer target " <<m_pPolymerTarget->GetLabel() << " containing " << m_vPolymers.size() << " polymers" << zEndl;
    }
    else if(m_pCompositeTarget)
    {
        m_Targets = m_pCompositeTarget->GetTargets();
        
        std::cout << "Analysing composite target " <<m_pCompositeTarget->GetLabel() << " containing " << m_pCompositeTarget->GetTargetTotal() << " simple targets" << zEndl;
    }
    else
    {
        std::cout << "Error in taCylindricalDistribution constructor - unrecognised target container" << zEndl;
    }

    // We don't add the contained taHistogramDecorator instance to the
    // set of active command targets as it has no Execute() function and does
    // not belong in the chain, but rather as a sub-object of this class.
    // The last argument indicates that the number of bins is fixed.
    
    m_BinTotal = static_cast<long>((m_OuterRadius)/m_ShellWidth);
    
    m_pHistogram = new taBinCylinderSpaceCoordinates(label+"_hist", GetOutermostDecorator(), m_BinTotal, m_ShellWidth);

    // Store a taFileLabel object to serialize the histogram data to file.

    const zString fileName = xxBase::GetEADPrefix() + label + "." + IGlobalSimBox::Instance()->GetRunId();

    m_pFile = new taSerializeHistogram(fileName, label+"_hist_output", GetOutermostDecorator());
    m_pFile->SetHistogram(m_pHistogram);
}

taCylindricalDistribution::~taCylindricalDistribution()
{
    // We don't destroy the m_pHistogram and m_pFile instances here as they are
    // derived from CCommandTargetNode and are owned by the CSimBox as
    // command target decorators.
}

// Non-static member function to return the target's type.

const zString taCylindricalDistribution::GetTargetType() const
{
    return m_Type;
}

// Function called by the SimBox to update the decorator state. All classes
// derived from taEventSourceAnalysis keep track of how many samples
// they have taken and normalise their data according to the value defined in
// the base class function GetSamplesPerAnalysis()

void taCylindricalDistribution::Execute(long simTime)
{
    GetInnerDecorator()->Execute(simTime);

    // If the target is not a polymer target or a composite target or is empty, we log a warning message
    // and ignore the decorator.
    
    // NB I don't implement the samples parameter yet, so the histogram is updated every SamplePeriod steps.
     
    if(m_pPolymerTarget && m_pPolymerTarget->GetPolymerTotal() > 0)
    {
        CalculatePolymerCM(simTime);
    }
    else if(m_pCompositeTarget && m_pCompositeTarget->GetTargetTotal() > 0)
    {
        CalculateTargetCM(simTime);
    }
    else if(!m_bWrapFailure)
    {
        m_bWrapFailure = true;
        new CLogctNonexistentDecorator(simTime, GetLabel(), "");
    }
  }

// Function called by the wrapped ESD to get the EAD to normalise its data and
// write the current histogram to a file. We then reset the histogram ready for
// the next data set.

void taCylindricalDistribution::Normalise()
{
    //  Normalise the data according to the type of histogram, in this case it uses the area of the cylindrical shells.
    // We don't need to pass in the number of times the function has been called because we sum over all samples over all times.
         
    m_pHistogram->Normalise(1.0);

    if(!m_pFile->Serialize())
    {
        new CLogTextWarningMessage(IGlobalSimBox::Instance()->GetCurrentTime(), "Error serialising data from histogram calculation in decorator " + GetLabel() + " wrapping target " + GetActiveCommandTarget()->GetLabel());
    }

}

// Helper function called by Execute() to do the calculation for a simple polymer target. It assumes the
// targat has been validated and contains a non-zero number of polymers.

void taCylindricalDistribution::CalculatePolymerCM(long simTime)
{
    if(simTime > m_Start && simTime <= m_End)
    {
        m_TimesCalled++;
    
        // Loop over all target polymers and add them to the histogram according to the
        // normal distance of their CM from the axis.

        // HARDWIRED for the axis along Z for now!
               
        double dx[3];     // Vector of polymer CM from defined axial point
        double dcm;       // Shortest distance of polymer CM to axis

        for(PolymerVectorIterator iterPoly = m_vPolymers.begin(); iterPoly != m_vPolymers.end(); ++iterPoly)
        {
            aaVector vcm = (*iterPoly)->GetCM();

            dx[0] = vcm.GetX() - m_XC;
            dx[1] = vcm.GetY() - m_YC;

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
            dx[2] = vcm.GetZ() - m_ZC;

            if( dx[2] > IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
                dx[2] = dx[2] - IGlobalSimBox::Instance()->GetSimBoxZLength();
            else if( dx[2] < -IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
                dx[2] = dx[2] + IGlobalSimBox::Instance()->GetSimBoxZLength();
        #else
            dx[2] = 0.0;
        #endif
               
            dcm = sqrt(dx[0]*dx[0] + dx[1]*dx[1]);

            m_pHistogram->AddDataPoint(dcm);
        }

        // ********************
        // Postconditions

        if(simTime == m_End)
        {
            Normalise();
        }
        // ********************
    }
    else if(simTime == m_Start)
    {
        // ********************
        // Preconditions - None

    }
}

// Helper function called by Execute() to do the calculation for a composite target that holds a set
// of simple bead or polymer targets. Note that it does NOT recurse into the contained targets, and they
// must be simple targets not composite.

void taCylindricalDistribution::CalculateTargetCM(long simTime)
{
 if(simTime > m_Start && simTime <= m_End)
 {
     m_TimesCalled++;
 
     // Loop over all simple targets in the composite target and add them to the histogram according to the
     // normal distance of their CM from the axis.

     // HARDWIRED for the axis along Z for now!
            
     double dx[3];     // Vector of target CM from defined axial point
     double dcm;       // Shortest distance of target CM to axis

     for(CommandTargetIterator iterT = m_Targets.begin(); iterT != m_Targets.end(); ++iterT)
     {
         aaVector vcm = (*iterT)->GetCM();

         dx[0] = vcm.GetX() - m_XC;
         dx[1] = vcm.GetY() - m_YC;
         
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
         dx[2] = vcm.GetZ() - m_ZC;

         if( dx[2] > IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
             dx[2] = dx[2] - IGlobalSimBox::Instance()->GetSimBoxZLength();
         else if( dx[2] < -IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
             dx[2] = dx[2] + IGlobalSimBox::Instance()->GetSimBoxZLength();
     #else
         dx[2] = 0.0;
     #endif
 
         dcm = sqrt(dx[0]*dx[0] + dx[1]*dx[1]);
         
         m_pHistogram->AddDataPoint(dcm);
     }

     // ********************
     // Postconditions

     if(simTime == m_End)
     {
         Normalise();
     }
     // ********************
 }
 else if(simTime == m_Start)
 {
     // ********************
     // Preconditions - None

 }

 
 
}



// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taCylindricalDistribution::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taCylindricalDistribution::Write(zOutStream& os) const
{
    
    return os;
}

