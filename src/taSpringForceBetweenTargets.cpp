/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taSpringForceBetweenTargets.cpp: implementation of the taSpringForceBetweenTargets class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "taSpringForceBetweenTargets.h"
#include "xxDecoratorState.h"
#include "TimeSeriesData.h"
#include "IGlobalSimBox.h"
#include "LogctNonexistentDecorator.h"
#include "LogTextWarningMessage.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taSpringForceBetweenTargets::m_Type = "SpringForceBetweenTargets";

const zString taSpringForceBetweenTargets::GetType()
{
	return m_Type;
}

// We use an anonymous namespace to wrap the call to the factory object
// so that it is not accessible from outside this file. The identifying
// string is stored in the m_Type static member variable.
//
// Note that the Create() function is not a member function but a global 
// function hidden in the namespace.

namespace
{
	CCommandTargetNode* Create(const zString label) {return new taSpringForceBetweenTargets(label);}

	const zString id = taSpringForceBetweenTargets::GetType();

	const bool bRegistered = acfTargetFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

taSpringForceBetweenTargets::taSpringForceBetweenTargets(const zString label, CCommandTargetNode* const pDec,
                             CCommandTargetNode* const pTarget2, double mag, double deltax, long period) : taForceDecorator(label, pDec),
                               m_pTarget2(pTarget2),
							   m_keff(mag), m_DeltaX(deltax), m_Period(period),
                               m_pTarget1Bead(0), m_pTarget2Bead(0), 
                               m_Counter(0), m_Separation(0.0), m_TargetSeparation(0.0), 
                               m_FX(0.0), m_FY(0.0), m_FZ(0.0),
							   m_RelOX(0.0), m_RelOY(0.0), m_RelOZ(0.0)
{
	pDec->SetOuterDecorator(this);
        
    // Find the beads closest to the CM of each target

    BeadVector vBeads1 = pDec->GetBeads();
    BeadVector vBeads2 = pTarget2->GetBeads();
        
    m_pTarget1Bead = FindCMBead(vBeads1);
    m_pTarget2Bead = FindCMBead(vBeads2);
        
    // and their initial separation so that we can calculate how far they move as the force is applied over time
    
    const double dx = m_pTarget1Bead->GetXPos() - m_pTarget2Bead->GetXPos();
    const double dy = m_pTarget1Bead->GetYPos() - m_pTarget2Bead->GetYPos();
    const double dz = m_pTarget1Bead->GetZPos() - m_pTarget2Bead->GetZPos();
    
    m_TargetSeparation = sqrt( dx*dx + dy*dy + dz*dz );
    
#if EnableXMLDecorators == SimXMLEnabled
    
    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId() + ".xml", IGlobalSimBox::Instance()->GetCurrentTime(), IGlobalSimBox::Instance()->GetTotalTime(), IGlobalSimBox::Instance()->GetRunId(), "SpringForceBetweenTargets"));
    
	// Note that the opening <Header> tag is written in the xxDecoratorState class
	// but the closing tag is written here. This is a hack.
    
	zOutStream& os = m_pState->putXMLStartTags();
	os << "<TargetLabel>"		 << GetActiveCommandTarget()->GetLabel() << "</TargetLabel>";
    os << "<DecoratorLabel>"     << GetLabel() << "</DecoratorLabel>";
	os << "</Header>" << zEndl;
    
#elif EnableXMLDecorators == SimXMLDisabled
    
    // We don't define a start and end time but use the whole duration of the decorator instance's lifetime.
    
    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId(), IGlobalSimBox::Instance()->GetCurrentTime(), IGlobalSimBox::Instance()->GetTotalTime(), IGlobalSimBox::Instance()->GetRunId(), "SpringForceBetweenTargets"));
    
	zOutStream& os = m_pState->putASCIIStartTags();
	os << "    Target	    " << GetActiveCommandTarget()->GetLabel() << zEndl;
    os << "    Decorator   " << GetLabel() << zEndl;
    
#endif

}

// Constructor for use when creating the instance by reading its state in from
// a file. We only define its label here and all remaining member variables
// must be set in the Read() function.

taSpringForceBetweenTargets::taSpringForceBetweenTargets(const zString label) : taForceDecorator(label),
                               m_pTarget2(0),
							   m_keff(0.0), m_DeltaX(0.0), m_Period(0),
                               m_pTarget1Bead(0), m_pTarget2Bead(0), 
                               m_Counter(0), m_Separation(0.0), m_TargetSeparation(0.0), 
                               m_FX(0.0), m_FY(0.0), m_FZ(0.0),
							   m_RelOX(0.0), m_RelOY(0.0), m_RelOZ(0.0)
{

}

// We save and close the decorator's state file when the program ends or when the decorator is destroyed is this is sooner.

taSpringForceBetweenTargets::~taSpringForceBetweenTargets()
{
    if(SaveState())
    {
//        CLogctPolymerTargetMSD* pMsg = new CLogctPolymerTargetMSD(simTime, targetLabel, decLabel, m_vMSD.back());
    }
    else
    {
         new CLogTextWarningMessage(IGlobalSimBox::Instance()->GetCurrentTime(), "Error serialising data from decorator " + GetLabel() + " around target " + GetActiveCommandTarget()->GetLabel());
    }
}

// Non-static member function to return the target's type.

const zString taSpringForceBetweenTargets::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to apply a Hookean spring force with spherical symmetry
// to the beads held in the wrapped command target and the second target so as to either pull them together or push them apart. 
// We calculate the initial separation of the centres of mass of the targets, and apply a force to all beads in each target directed 
// either away (m_DeltaX > 0) or towards (m_DeltaX < 0) each other. Note that each bead feels the same force. The force 
// is derived from a harmonic potential of the form:
//
//    V(r) = 0.5*k*(r - r0)**2
//
// where r = sqrt(x*x +y*y + z*z);
// and   r0 = (x0,y0,z0) are the cartesian coordinates of the "origin."
//
// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
//
// Because the innermost command target object just holds the beads, the 
// chain stops when its do-nothing Execute() function is invoked.

void taSpringForceBetweenTargets::Execute(long simTime)
{
    GetInnerDecorator()->Execute(simTime);
    
    // debug checks
    const zString label = GetLabel();

    // We find the current separation between the two targets (as defined by the beads that are closest to their CM), and apply a spring 
    // force to move this separation towards the desired value m_TargetSeparation. There are two ways of doing this: 1) we apply a force to 
    // all beads in the target or, 2) apply a force to the single bead closest to the centre of mass of the target. For now, we use the second
    // option.

    m_RelOX = m_pTarget1Bead->GetXPos() - m_pTarget2Bead->GetXPos();
    m_RelOY = m_pTarget1Bead->GetYPos() - m_pTarget2Bead->GetYPos();
    m_RelOZ = m_pTarget1Bead->GetZPos() - m_pTarget2Bead->GetZPos();
    
    m_Separation = sqrt(m_RelOX*m_RelOX + m_RelOY*m_RelOY + m_RelOZ*m_RelOZ);
    
    m_FX = m_keff*(m_TargetSeparation - m_Separation)*m_RelOX/m_Separation;
    m_FY = m_keff*(m_TargetSeparation - m_Separation)*m_RelOY/m_Separation;
    m_FZ = m_keff*(m_TargetSeparation - m_Separation)*m_RelOZ/m_Separation;
    
    // Apply the force to the two beads nearest the targets' CM
    
    AddBeadXForce(m_pTarget1Bead, m_FX);
    AddBeadYForce(m_pTarget1Bead, m_FY);
    AddBeadZForce(m_pTarget1Bead, m_FZ);
    
    AddBeadXForce(m_pTarget2Bead, -m_FX);
    AddBeadYForce(m_pTarget2Bead, -m_FY);
    AddBeadZForce(m_pTarget2Bead, -m_FZ);
    
/*  
    // To apply the force to all beads in the targets, uncoment this piece of code
	BeadVector vBeads = GetBeads();

	for(BeadVectorIterator iterBead=vBeads.begin(); iterBead!=vBeads.end(); iterBead++)
	{
		AddBeadXForce(*iterBead, m_FX);
		AddBeadYForce(*iterBead, m_FY);
		AddBeadZForce(*iterBead, m_FZ);
	}
    
    // Now do the same for the secondary target
    
    BeadVector vBeads2 = m_pTarget2->GetBeads();

	for(BeadVectorIterator iterBead2=vBeads2.begin(); iterBead2!=vBeads2.end(); iterBead2++)
	{
		AddBeadXForce(*iterBead2, -m_FX);
		AddBeadYForce(*iterBead2, -m_FY);
		AddBeadZForce(*iterBead2, -m_FZ);
	}
 */
    
    // Write the CM data to the decorator state file for offline analysis

    const long dataTotal = 4;
    
    CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);
    
    pTSD->SetValue(0, simTime, "Time");
    pTSD->SetValue(1, m_Separation, "Separation");
    pTSD->SetValue(2, m_TargetSeparation, "Target Separation");
    pTSD->SetValue(3, (m_keff*(m_TargetSeparation - m_Separation)), "FMAG");
    
    m_pState->AddTimeSeriesData(pTSD);
	
    // Increment the desired separation after each period
    
    ++m_Counter;
    
    if(m_Counter == m_Period)
    {
        m_Counter = 0;
        
        m_TargetSeparation += m_DeltaX;
    }
}

// Calculate the force on the bead stored in the taForceDecorator base class.
// Because the linear force depends on the bead coordinates, we need access
// to the bead's data. To avoid calculating vector magnitudes three times, we
// only do it in the X component calculation and store all three components
// of the foce. Hence, the GetXForce() function MUST be called before the Y, Z
// components or they will return nonsense.

double taSpringForceBetweenTargets::GetXForce(long simTime)
{
	return m_FX;
}

double taSpringForceBetweenTargets::GetYForce(long simTime)
{
	return m_FY;
}

double taSpringForceBetweenTargets::GetZForce(long simTime)
{
	return m_FZ;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taSpringForceBetweenTargets::Read(zInStream& is)
{
    is >> m_keff >> m_DeltaX >> m_Period;

    if(!is.good() || m_keff < 0.0 || m_Period < 1)
    {
         new CLogRestartStateBuilderError(0, "Error importing spring force between targets decorator (non-positive period?)");
        return is;
    }

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taSpringForceBetweenTargets::Write(zOutStream& os) const
{
    CCommandTargetNode::Write(os);  // Write base class data first
   
    os << m_keff << m_DeltaX << m_Period;
    
    if(GetOuterDecorator())
        return GetOuterDecorator()->Write(os);
    else
        return os;
}

// Private helper function to calculate the Centre of Mass of a set of beads that are passed in as an argument.
// The CM is returned via the remaining arguments.

void taSpringForceBetweenTargets::CalculateCM(BeadVector vBeads, double* pXCM, double* pYCM, double* pZCM)
{
    double xcm = 0.0;
    double ycm = 0.0;
    double zcm = 0.0;
    
	for(BeadVectorIterator iterBead=vBeads.begin(); iterBead!=vBeads.end(); iterBead++)
	{
		xcm += (*iterBead)->GetXPos();
		ycm += (*iterBead)->GetYPos();
		zcm += (*iterBead)->GetZPos();
	}
    
    xcm /= static_cast<double>(vBeads.size());
    ycm /= static_cast<double>(vBeads.size());
    zcm /= static_cast<double>(vBeads.size());
                               
    *pXCM = xcm;
    *pYCM = ycm;
    *pZCM = zcm;
}

// Private helper function to find the bead in a target that is closest to the target's CM.

CBead* taSpringForceBetweenTargets::FindCMBead(BeadVector vBeads)
{
    double xcm, ycm, zcm;
    xcm = 0.0;
    ycm = 0.0;
    zcm = 0.0;
    
    CalculateCM(vBeads, &xcm, &ycm, &zcm);
                                    
    // Iterate over all beads and find the one that is closest to the CM
    
    CBead* pCMBead = 0;
    double squareDistance = 0.0;
    double oldSquareDistance = 1000.0;
    
    for(BeadVectorIterator iterBead=vBeads.begin(); iterBead!=vBeads.end(); ++iterBead)
    {
        double x = (*iterBead)->GetXPos();
        double y = (*iterBead)->GetYPos();
        double z = (*iterBead)->GetZPos();
        
        squareDistance = (x - xcm)*(x - xcm) + (y - ycm)*(y - ycm) + (z - zcm)*(z - zcm);
        
        if(squareDistance < oldSquareDistance)
        {
            oldSquareDistance = squareDistance;
            pCMBead = *iterBead;
        }
    }
                                    
    return pCMBead;
}

                                
                                
                                
                                
                                
