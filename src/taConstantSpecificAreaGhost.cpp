/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taConstantSpecificAreaGhost.cpp: implementation of the taConstantSpecificAreaGhost class.
//
//////////////////////////////////////////////////////////////////////
  
#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taConstantSpecificAreaGhost.h"
#include "xxDecoratorState.h"
#include "CommandTargetPolymer.h"
#include "Bead.h"
#include "Polymer.h"
#include "TimeSeriesData.h"
#include "ISimState.h"
#include "CurrentState.h"
//#include "LogctConstantSpecificAreaGhostTimeSeries.h"
#include "LogctConstantSpecificAreaGhostFailed.h"
#include "LogctNonexistentDecorator.h"
#include "LogTextWarningMessage.h"

//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taConstantSpecificAreaGhost::m_Type = "ConstantSpecificAreaGhost";

const zString taConstantSpecificAreaGhost::GetType()
{
	return m_Type;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor that creates a decorator object to wrap a command target, which
// may already be wrapped by other decorator objects. 
//
// The wrapped object is stored in the m_pInnerDecorator member variable of the
// CCommandTargetNode base class by passing the pDec pointer to the base class 
// constructor. The newly-created object is stored in the m_pOuterDecorator of
// the wrapped object using the function SetOuterDecorator().
//

taConstantSpecificAreaGhost::taConstantSpecificAreaGhost(const zString decLabel, CCommandTargetNode* const pReservoirTarget, 
                                                        CCommandTargetNode* const pMembraneTarget, ISimState* pISimState, long beadType, 
                                                        long start, long end,long sampleRate,  double targetAN) : taCumulateDecorator(decLabel, pReservoirTarget, "", start, end),
                                                        m_BeadType(beadType), m_SampleRate(sampleRate), m_TargetAN(targetAN), 
                                                        m_pReservoirTarget(0), m_pMembraneTarget(0), m_pISimState(pISimState), 
                                                        m_DeltaPolymerTotal(1), m_CurrentPolymerTotal(0), m_CurrentArea(0.0),  m_DeltaArea(0.0),
                                                        m_pGhostLipid(0), m_pMembraneLipid(0), m_pGhost(0)
{
    pReservoirTarget->SetOuterDecorator(this);

    // Store pointers to the membrane and reservoir targets for later use

    m_pReservoirTarget  = dynamic_cast<CCommandTargetPolymer*>(GetActiveCommandTarget());    
    m_pMembraneTarget   = dynamic_cast<CCommandTargetPolymer*>(pMembraneTarget);
        
   // Clear the local containers
    
    m_vBulkReservoir.clear();
    m_vMembraneReservoir.clear();
    m_vBeads.clear();
    m_vGhostLipidBeads.clear();
    m_vMembraneLipidBeads.clear();

#if EnableXMLDecorators == SimXMLEnabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId() + ".xml", GetStartTime(), GetEndTime(), IGlobalSimBox::Instance()->GetRunId(), "ConstantSpecificAreaGhost"));

	// Note that the opening <Header> tag is written in the xxDecoratorState class
	// but the closing tag is written here. This is a hack.

	zOutStream& os = m_pState->putXMLStartTags();
	os << "<TargetLabel>"		 << GetActiveCommandTarget()->GetLabel() << "</TargetLabel>";
    os << "<DecoratorLabel>"     << GetLabel() << "</DecoratorLabel>";
	os << "</Header>" << zEndl;

#elif EnableXMLDecorators == SimXMLDisabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId(), GetStartTime(), GetEndTime(), IGlobalSimBox::Instance()->GetRunId(), "ConstantSpecificAreaGhost"));

	zOutStream& os = m_pState->putASCIIStartTags();
	os << "    Target	    " << GetActiveCommandTarget()->GetLabel() << zEndl;
    os << "    Decorator   " << GetLabel() << zEndl;

#endif


}

taConstantSpecificAreaGhost::~taConstantSpecificAreaGhost()
{

}

// Non-static member function to return the target's type.

const zString taConstantSpecificAreaGhost::GetTargetType() const
{
    return m_Type;
}

// Function used by the CSimBox to maintain a constant area per molecule (the specific area) of a membrane by allowing a second molecule type (the ghosts)
// to move between a reservoir in the bulk solvent and the membrane. It calculates the mean area per molecule in the membrane and compares this 
// to the user-specified target A/N and then calculates a number of amphiphiles to move from the bulk into the membrane or vice versa.
// It changes the bead-bead conservative interactions of the ghost molecules to those of the amphiphile when it releases them from the reservoir.
// They then diffuse into the membrane in a time set by their natural diffusion constant. This means that we should wait at least this time between 
// invocations for the membrane to equilibrate so that the amphiphile transfer does not drive the membrane.
// Molecules can also move back from the membrane into the bulk reservoir if the mean A/N is smaller than the target value. This is effected by changing the 
// conservative interactions of some of the ex-ghost molecules back to that of water and letting them diffuse out of the membrane back into the bulk solvent.
//
// Instead of moving several molecules from the bulk reservoir to the membrane we only change one at a time. This prevents the needless transfer of many groups 
// of molecules if earlier ones do not diffuse into the membrane in a short time.
//
// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
// The innermost command target object just holds the beads, so the chain stops when 
// its do-nothing Execute() function is invoked.
//

void taConstantSpecificAreaGhost::Execute(long simTime)
{
    GetInnerDecorator()->Execute(simTime);

    const zString targetLabel = GetActiveCommandTarget()->GetLabel();
    const zString decLabel    = GetLabel();

    // If the target is not a polymer target or the membrane target is invalid, we log a warning message 
    // ang ignore the decorator. But if the targets are empty we issue the same error condition as when we run out of ghosts.
 
    if(m_pReservoirTarget && m_pMembraneTarget)
    {
	    if(simTime >= m_Start && simTime <= m_End)
		{
	        if(simTime == m_Start)
	        {
                // ********************
                // Preconditions
                // Ensure the containers are empty and then copy pointers to all the ghost molecules in the simulation box into a local reservoir container.
                // Note that we assume that all the ghost molecules start out in the bulk solvent.

                m_vBulkReservoir.clear();
                m_vMembraneReservoir.clear();
                m_vBeads.clear();
                m_vGhostLipidBeads.clear();
                m_vMembraneLipidBeads.clear();
            
                PolymerVector  vGhosts = m_pReservoirTarget->GetPolymers();
                PolymerVector  vLipids = m_pMembraneTarget->GetPolymers();
				
                // Store pointers to exemplar membrane and ghost lipids and their beads to use in changing ghost lipids' interactions
            
                m_pGhostLipid         = vGhosts.at(0);
                m_pMembraneLipid      = vLipids.at(0);
                m_vGhostLipidBeads    = m_pGhostLipid->GetBeads();
                m_vMembraneLipidBeads = m_pMembraneLipid->GetBeads();

               // Store pointers to all the beads used to calculate the membrane's area: these are identified by their type m_BeadType
               // and stored in m_vBeads. NB. We only use the membrane lipids' beads for the triangulation.

                for(PolymerVectorIterator iterPoly = vLipids.begin(); iterPoly != vLipids.end(); iterPoly++)
                {
                   BeadVector  vBeads = (*iterPoly)->GetBeads();

                    for(BeadVectorIterator iterBead = vBeads.begin(); iterBead != vBeads.end(); iterBead++)
                    {
                       if((*iterBead)->GetType() == m_BeadType)
                       {
                           m_vBeads.push_back(*iterBead);
                       }
                    }
                }

                // Copy all the ghost polymers into the local container
            
                for(PolymerVectorIterator iterPoly = vGhosts.begin(); iterPoly != vGhosts.end(); iterPoly++)
                {
                    m_vBulkReservoir.push_back(*iterPoly);
                }
             
               // Record the start of the time series sampling. Note that the same message
               // is used to log the start of the calculation and its end, but with
               // different arguments.

//		    CLogctConstantSpecificAreaGhostTimeSeries* pMsg = new CLogctConstantSpecificAreaGhostTimeSeries(simTime, targetLabel, decLabel,  
//												                  m_Start, m_End, m_vPolymers.size());
		    }

            if(simTime % m_SampleRate == 0 || simTime == m_Start)
            {
	         // ********************
	         // Main loop
                // Calculate the mean area in the membrane's current state
            
                CalculateArea();
            
                std::cout << "At time " << simTime << " the delta area of the membrane is " << m_DeltaArea << " and membrane and bulk vectors have " 
                          << m_vMembraneReservoir.size() << " " << m_vBulkReservoir.size() << " elements respectively" << zEndl;
            
                // Compare the current A/N to the target value, m_TargetAN, and transfer a number of molecules 
                // from the bulk reservoir to the membrane or vice versa. A negative value means that polymers leave 
                // the membrane and return to the bulk reservoir. Note that we move a fixed number m_DeltaPolymerTotal = 1 of lipids each invocation.
		// We change the conservative interactions of the lipids before moving them.
				
		if(m_DeltaArea > 0)
                {
                    // move polymers from the bulk reservoir to the membrane but check that there actually some ghosts left
					
			if(m_vBulkReservoir.size() > 0)
			{
//                       for(long i=0; i<m_DeltaPolymerTotal; ++i)
//                       {
			m_pGhost = m_vBulkReservoir.back();
                        m_vMembraneReservoir.push_back(m_pGhost);
                    
                        m_vBulkReservoir.erase(--m_vBulkReservoir.end());
                
                        BeadVector vGhostBeads = m_pGhost->GetBeads();
                    
                        // Copy the membrane polymer's interactions into the newly-arrived ghost polymer
                    
			 double delta = 0.1;
						
                        for(long unsigned int j=0; j<vGhostBeads.size(); ++j)
                        {
                            CBead* pMembraneBead = m_vMembraneLipidBeads.at(j);
                            CBead* pGhostBead    = vGhostBeads.at(j);
			     pGhostBead->SetType(pMembraneBead->GetType());
                            CCurrentState::SetBeadDisplayId(pGhostBead->GetId(), CCurrentState::GetBeadDisplayId(pMembraneBead->GetId()));
							
			     double dx = 16;
                            double dy = 16;
                            double dz = 16;
							
			     m_pISimState->MoveBeadBetweenCNTCells(pGhostBead, dx+delta, dy+delta, dz+delta);
							
                            delta += 0.1;
                        }
 //                       }
                    }
		else
		{
		     new CLogctConstantSpecificAreaGhostFailed(simTime, decLabel, m_pReservoirTarget->GetLabel(), m_pMembraneTarget->GetLabel(), m_vBulkReservoir.size(), m_vMembraneReservoir.size());
					}
                }
                else if(m_DeltaArea < 0)
                {
                    // move polymers from the membrane to the bulk reservoir but check that there are some ghosts in the membrane first
					                
					if(m_vMembraneReservoir.size() > 0)
					{
//                       for(long i=0; i<m_DeltaPolymerTotal; ++i)
//                       {
                        m_pGhost = m_vMembraneReservoir.back();
                        m_vBulkReservoir.push_back(m_pGhost);
                    
                        m_vMembraneReservoir.erase(--m_vMembraneReservoir.end());
                
                        BeadVector vGhostBeads = m_pGhost->GetBeads();
                    
                        // Copy the membrane polymer's interactions into the newly-arrived ghost polymers
                    
                        for(long unsigned int j=0; j<vGhostBeads.size(); ++j)
                        {
                            CBead* pOriginalGhostBead = m_vGhostLipidBeads.at(j);
                            CBead* pGhostBead         = vGhostBeads.at(j);
                            
                            pGhostBead->SetType(pOriginalGhostBead->GetType());
                            CCurrentState::SetBeadDisplayId(pGhostBead->GetId(), CCurrentState::GetBeadDisplayId(pOriginalGhostBead->GetId()));
                        }
//                       }
                    }
			else
			{
		            new CLogctConstantSpecificAreaGhostFailed(simTime, decLabel, m_pReservoirTarget->GetLabel(), m_pMembraneTarget->GetLabel(), m_vBulkReservoir.size(), m_vMembraneReservoir.size());
			}
                }

                // Write the CM data to the decorator state file

	            long dataTotal = 2;

	            CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);

	            pTSD->SetValue(0, simTime, "Time");
	            pTSD->SetValue(1, m_CurrentPolymerTotal,     "Membrane polymers");

	            m_pState->AddTimeSeriesData(pTSD);
		    }

	        if(simTime == m_End)
	        {
                // ********************
                // Postconditions

                // Write the time series data to file or log a warning message if it fails

                if(SaveState())
                {
 //                   new CLogctConstantSpecificAreaGhostTimeSeries(simTime, targetLabel, decLabel, m_Start, m_End, m_vPolymers.size());
                }
                else
                {
                     new CLogTextWarningMessage(simTime, "Error serialising data from decorator " + decLabel + " around target " + targetLabel);
                }
		    }
	    }
    }
    else if(!m_bWrapFailure)
    {
	    m_bWrapFailure = true;
	     new CLogctNonexistentDecorator(simTime, decLabel, "");
    }
}

// Private helper function to calculate the actual area of the membrane not its projected area. We triangulate the surface and sum the areas of all the triangles.
// The result is stored in the member variable m_CurrentArea and the change to this area reqruied to maintain the target A/N is stored in m_DeltaArea.

void taConstantSpecificAreaGhost::CalculateArea()
{
    m_CurrentArea = 0.0;
    m_DeltaArea   = 0.0;
	
	m_CurrentPolymerTotal = m_pMembraneTarget->GetPolymerTotal() + m_vMembraneReservoir.size();
        

    
    // calculate triangulated area here...

    zDoubleVector vXCoords;
    zDoubleVector vYCoords;
    zDoubleVector vZCoords;


    for(BeadVectorIterator iterBead = m_vBeads.begin(); iterBead != m_vBeads.end(); iterBead++)
    {
        vXCoords.push_back((*iterBead)->GetXPos());
        vYCoords.push_back((*iterBead)->GetYPos());
        vZCoords.push_back((*iterBead)->GetZPos());
    }
	
	// Store the change in the current area  needed to maintain the target A/N. This determines whether lipids are moved from the bulk reservoir to the membrane 
	// or vice versa.
	
	m_DeltaArea = (m_CurrentArea - m_CurrentPolymerTotal*m_TargetAN);
	
	m_DeltaArea = 1.0;  // fake it for now as we don't have the triangulation algorithm working
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taConstantSpecificAreaGhost::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taConstantSpecificAreaGhost::Write(zOutStream& os) const
{
    
    return os;
}

