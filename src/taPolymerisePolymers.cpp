/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// taPolymerisePolymers.cpp: implementation of the taPolymerisePolymers class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "taPolymerisePolymers.h"
#include "xxDecoratorState.h"
#include "CommandTargetPolymer.h"
#include "CNTCell.h"
#include "Bead.h"
#include "Bond.h"
#include "Polymer.h"
#include "SimBox.h"
#include "TimeSeriesData.h"
#include "IModifySimStateBondTypes.h"
#include "IGlobalSimBox.h"
#include "LogctNonexistentDecorator.h"
#include "LogTextMessage.h"
#include "LogTextWarningMessage.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////
// Static member variable containing the identifier for this target. 
// The static member function GetType() can be used to identify the target
// represented by objects of this class.

const zString taPolymerisePolymers::m_Type = "PolymerisePolymers";

const zString taPolymerisePolymers::GetType()
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
// Are there any problems in passing a this pointer from an object's constructor???

taPolymerisePolymers::taPolymerisePolymers(const zString label, CCommandTargetNode* const pDec,
                                           CSimBox* const pSimBox, CBond* pPolyBondTemplate, 
                                           long polyType, zLongVector vBeadTypes, long maxBonds, double maxDistance,
                                           double springConst, double unstrLength,
                                           double bondPrefactor, double bondBarrier) : taActionLabel(label, pDec),
                                           m_pSimBox(pSimBox), m_pPolymerTarget(0),
                                           m_pPolyBondTemplate(pPolyBondTemplate),
                                           m_PolymerType(polyType), m_vBeadTypes(vBeadTypes),
                                           m_MaxBonds(maxBonds), m_MaxDistance(maxDistance),
                                           m_sprConst(springConst), m_unStrLength(unstrLength),
                                           m_BondPrefactor(bondPrefactor), m_BondBarrier(bondBarrier),
                                           m_MaxDistSquared(maxDistance*maxDistance),
                                           m_ArrheniusExp(1.0/exp(bondBarrier/IGlobalSimBox::Instance()->GetTemperature())),
                                           m_PolyBondCounter(0), m_PolyBeadTotal(0), m_StickySiteTotal(0),
                                           m_ClusterCounter(0)
{

    std::cout << "Entered taPolymerisePolymers decorator constructor: " << m_vPolymers.size() << zEndl;

    pDec->SetOuterDecorator(this);

    // Store a pointer to the actual polymer target for later use

    m_pPolymerTarget = dynamic_cast<CCommandTargetPolymer*>(GetActiveCommandTarget());

    m_vPolymers.clear();
    m_vPolymers = m_pPolymerTarget->GetPolymers();

     std::cout << "After calling GetPolymers: " << m_vPolymers.size() << zEndl;

   // Get vector of polymerisable beads from all polymers 

    m_mOrderedBeadIds.clear();
    m_mClusterSize.clear();
    m_vSSBondTotals.clear();
    m_vSSToCluster.clear();
    m_vStickyBeads.clear();
    m_vBeadInStickySite.clear();

    long counter = 0;
    bool wasPreviousSticky = false;
    long stickySiteCounter = -1;
    
    // Iteration over each polymer collecting all sticky beads of the specified types and
    // storing them in the m_vStickyBeads vector; creating a map from bead index to sequential bead index; creating a vector linking a bead to its sticky site
        
    for(auto iterPoly = m_vPolymers.begin(); iterPoly != m_vPolymers.end(); ++iterPoly)
    {
       wasPreviousSticky = false; // reset for each polymer

       for(auto iterBead = ((*iterPoly)->GetBeads()).begin(); iterBead != ((*iterPoly)->GetBeads()).end(); ++iterBead)
        {
        	CAbstractBead* pBead = *iterBead;
        	long beadType = pBead->GetType();
            bool isSticky = std::find(m_vBeadTypes.begin(), m_vBeadTypes.end(), beadType) != m_vBeadTypes.end();

        	if(isSticky)
        	{
                if(!wasPreviousSticky)
                {
                    stickySiteCounter++;
                }

        		m_vStickyBeads.push_back(pBead);

        		m_mOrderedBeadIds.insert(zPairLL(pBead->GetId(), counter));
        		
          		m_vBeadInStickySite.push_back(stickySiteCounter);
       		    
                counter++;
                wasPreviousSticky = true;
        	}
              else
            {
                wasPreviousSticky = false;
            }
    	}
    }
    m_PolyBeadTotal = m_vStickyBeads.size();
    m_StickySiteTotal = stickySiteCounter+1; //its +1 because we start at sticky site 0
    
    // Initialise the vector of existing bonds per sticky site and the vector of clusters indexed by the sticky site involved, to zero

    for(long site = 0; site < m_StickySiteTotal; ++site)
    {
        m_vSSBondTotals.push_back(0);
        m_vSSToCluster.push_back(0);
    }
    
    const zString polyBondName = pSimBox->GetBondNameFromType(pPolyBondTemplate->GetType());
    
    std::cout << "taPolymerisePolymers decorator constructor: " << m_vPolymers.size() << zEndl;
    std::cout << " Polymerised bead total: "                    << m_PolyBeadTotal    << zEndl;
    std::cout << " Sticky site total: "                         << m_StickySiteTotal  << zEndl;
    std::cout << "simTime, endTime, label " << m_pSimBox->GetCurrentTime() << " " << m_pSimBox->GetTotalTime() << " " << polyBondName << " " << pPolyBondTemplate->GetType() << " " << polyType << " " << counter << " " << m_PolyBeadTotal << zEndl;


// Create the time sreies instance to record dynamic data in its own file.

#if EnableXMLDecorators == SimXMLEnabled


#elif EnableXMLDecorators == SimXMLDisabled

    SetState(new xxDecoratorState(xxBase::GetTADSPrefix() + GetLabel() + "." + IGlobalSimBox::Instance()->GetRunId(), 
                                  m_pSimBox->GetCurrentTime(), m_pSimBox->GetTotalTime(), IGlobalSimBox::Instance()->GetRunId(), "PolymerisePolymers"));
    zOutStream& os = m_pState->putASCIIStartTags();
    os << "    Target	    " << GetActiveCommandTarget()->GetLabel() << zEndl;
    os << "    Decorator   " << GetLabel() << zEndl;
    
#endif

}
    
taPolymerisePolymers::~taPolymerisePolymers()
{

}

// Non-static member function to return the target's type.

const zString taPolymerisePolymers::GetTargetType() const
{
    return m_Type;
}

// Private member function to return the sequential bead index from its unique id.
// We probably need a check for a non-existent bead id passed in.

long taPolymerisePolymers::GetSequentialBeadIndex(long beadId) const
{
    return (m_mOrderedBeadIds.find(beadId)->second);
}

// Private member function to increment the current number of bonds between two beads.
// The indices passed in are (non-sequential) bead ids, and must be converted to bounded, sequential indices
// before storing in the map.

void taPolymerisePolymers::IncrementBondTotal(long beadId1, long beadId2)
{
    const long site1 = GetStickySiteFromBead(beadId1);
    const long site2 = GetStickySiteFromBead(beadId2);
    const long requiredSize = std::max(site1, site2) + 1;

    if(static_cast<long>(m_vSSBondTotals.size()) < requiredSize)
    {
        m_vSSBondTotals.resize(requiredSize, 0);
    }

    m_vSSBondTotals.at(site1)++;
    m_vSSBondTotals.at(site2)++;
}

// Private member function to make an index out of two sticky sites indices.
// Used to create the map m_mStickySiteBonded

long taPolymerisePolymers::MakeStickySiteBondIndex(long site1, long site2) const
{
    if(site1 > site2)
    {
        std::swap(site1, site2);
    }

    return site1 + m_StickySiteTotal * (site2 - 1);
}

// Private member function to check if two sticky sites are bound or not
// in other words if they are stored in the map m_mStickySiteBonded

bool taPolymerisePolymers::AreStickySitesUnbonded(long site1, long site2) const
{
    const long index = MakeStickySiteBondIndex(site1, site2);
    
    return (m_mStickySiteBonded.find(index) == m_mStickySiteBonded.end());
}

// Private member function to return the number of bonds made by a sticky site.
long taPolymerisePolymers::GetSSBonds(long site) const
{
    return m_vSSBondTotals.at(site);
}

// Private member function to return the sticky site associated to a sequential bead index.
long taPolymerisePolymers::GetStickySiteFromBead(long beadId) const
{
    const long seq_bead_id = GetSequentialBeadIndex(beadId);

    return m_vBeadInStickySite.at(seq_bead_id);
}

// Private member function to update clusters and cluster sizes
void taPolymerisePolymers::UpdateStickySiteClusters(long site1, long site2) 
{
    const long cluster1 = m_vSSToCluster.at(site1);
    const long cluster2 = m_vSSToCluster.at(site2);
    // Case 1: create a new cluster
    if(cluster1 == 0 && cluster2 == 0)
    {         
        const long newCluster = ++m_ClusterCounter;
        m_vSSToCluster.at(site1) = newCluster;
        m_vSSToCluster.at(site2) = newCluster;
        m_mClusterSize[newCluster] = 2;
    }
    // Case 2: add the free sticky site to the existing cluster 
    else if(cluster1 == 0 || cluster2 == 0)
    {
    const long existingCluster = (cluster1 != 0) ? cluster1 : cluster2;
    const long freeSite = (cluster1 == 0) ? site1 : site2;

    m_vSSToCluster.at(freeSite) = existingCluster;
    ++m_mClusterSize.at(existingCluster);
    }
    // Case 3: same cluster already -> nothing to change
    else if(cluster1 == cluster2)
    {
        // nothing
    }
    // Case 4: merge two clusters
    else if(cluster1 != 0 && cluster2 != 0)
    {
        const long keepCluster   = (cluster1 < cluster2) ? cluster1 : cluster2;
        const long removeCluster = (cluster1 < cluster2) ? cluster2 : cluster1;
        for(long i = 0; i < static_cast<long>(m_vSSToCluster.size()); ++i)
        {
            if(m_vSSToCluster.at(i) == removeCluster)
            {
                m_vSSToCluster.at(i) = keepCluster;
            }
        }
        auto iterKeepCluster = m_mClusterSize.find(keepCluster);
        auto iterRemoveCluster = m_mClusterSize.find(removeCluster);
        if(iterKeepCluster != m_mClusterSize.end() && iterRemoveCluster != m_mClusterSize.end())
        {
            iterKeepCluster->second += iterRemoveCluster->second;
            m_mClusterSize.erase(iterRemoveCluster);
        }
    }
}

// Private member function that checks if the cluster exceeds the maximum size (MaxBonds)

bool taPolymerisePolymers::IsClusterSizeAllowed(long site1, long site2) const
{
    const long cluster1 = m_vSSToCluster.at(site1);
    const long cluster2 = m_vSSToCluster.at(site2);

    long newSize = 0;

    if(cluster1 == 0 && cluster2 == 0)
        newSize = 2;
    else if(cluster1 != 0 && cluster2 == 0)
        newSize = m_mClusterSize.at(cluster1) + 1;
    else if(cluster1 == 0 && cluster2 != 0)
        newSize = m_mClusterSize.at(cluster2) + 1;
    else if(cluster1 == cluster2)
        newSize = m_mClusterSize.at(cluster1);
    else
        newSize = m_mClusterSize.at(cluster1) + m_mClusterSize.at(cluster2);

    return newSize <= m_MaxBonds;
}

// Private member function to apply an Arrhenius function to the bond formation event  to see if it succeeds. The prefactor and energy
// barrier are user-defined parameters. We generate a random number between (0,1) and compare it to the Arrhenius factor, allowing the bond
// if the random number is smaller than the factor.

// Note that the function is constant at the moment, but it may later depend on the polymers' states.


bool taPolymerisePolymers::ArrheniusFactorTest() const
{
    const double rno = CCNTCell::GetRandomNo();
    const double prob = m_BondPrefactor*m_ArrheniusExp;
    
//    std::cout << "Arrhenius: " << rno << "  " << prob << zEndl;

    return (rno < prob);
}


// Function used by the CSimBox to dynamically polymerise polymers in a command target by a specified bead
// type in them.
//
// Because there may be many decorator objects around a target, we chain their 
// execution from the innermost decorator to the outermost. This ensures that 
// the same order of execution is used, and the last added decorator is executed last.
// The innermost command target object just holds the beads, so the chain stops when 
// its do-nothing Execute() function is invoked.

void taPolymerisePolymers::Execute(long simTime)
{
    GetInnerDecorator()->Execute(simTime);
    
    CAbstractBead* pBead1 = 0;
    CAbstractBead* pBead2 = 0;
    
    double dx, dy, dz;
    double ds2;       // Temp variable storing separation of the two beads
    
    for(auto iterBead1 = m_vStickyBeads.begin(); iterBead1 != m_vStickyBeads.end(); iterBead1++)
    {
    	 pBead1 = (*iterBead1);

        for(auto iterBead2 = m_vStickyBeads.rbegin(); (*iterBead2)->GetId() != (*iterBead1)->GetId(); iterBead2++)
        {
             pBead2 = (*iterBead2);           

            // Apply range check to head and tail separations of the polymers: we have 4
            // mutually exclusive checks because we don't want to polymerise
            // the same two polymers more than once.
            //
            // head - head, head - tail, tail - head, tail - tail
                    
            dx = pBead1->GetXPos() - pBead2->GetXPos();
            dy = pBead1->GetYPos() - pBead2->GetYPos();
            dz = pBead1->GetZPos() - pBead2->GetZPos();
                
            // Correct for the PBCs in each dimension if the bead are separated by more than
            // half the simbox size.

 /*           if( dx > IGlobalSimBox::Instance()->GetHalfSimBoxXLength() )
                dx = dx - IGlobalSimBox::Instance()->GetSimBoxXLength();
            else if( dx < -IGlobalSimBox::Instance()->GetHalfSimBoxXLength() )
                dx = dx + IGlobalSimBox::Instance()->GetSimBoxXLength();

            if( dy > IGlobalSimBox::Instance()->GetHalfSimBoxYLength() )
                dy = dy - IGlobalSimBox::Instance()->GetSimBoxYLength();
            else if( dy < -IGlobalSimBox::Instance()->GetHalfSimBoxYLength() )
                dy = dy + IGlobalSimBox::Instance()->GetSimBoxYLength();

#if SimDimension == 3
            if( dz > IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
                dz = dz - IGlobalSimBox::Instance()->GetSimBoxZLength();
            else if( dz < -IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
                dz = dz + IGlobalSimBox::Instance()->GetSimBoxZLength();
#else
            dz = 0.0;
#endif

 */
            // 03/06/26 I disabled the PBC check so that bonds can only be created in the real space box, not
            // across the boundaries. I want to see if this prevents all the CNT cell error messages.
            
            ds2 = dx*dx + dy*dy + dz*dz;
                        
            // Now check if the beads are within range and
            // have spare bond capacity to be polymerised

            
            // Here we must add the new constraint that if thwe two beads are in the same sticky site, we forbid the bond
            // and if the two sticky sites containing the two beads have already had a bond made between any pair of their 
            // sticky beads, we forbid the bond.  

            if(ds2 < m_MaxDistSquared && ArrheniusFactorTest()
            && pBead1->GetXPos() > 1.0 && pBead1->GetXPos() < IGlobalSimBox::Instance()->GetSimBoxXLength()-1
            && pBead1->GetYPos() > 1.0 && pBead1->GetYPos() < IGlobalSimBox::Instance()->GetSimBoxYLength()-1
            && pBead1->GetZPos() > 1.0 && pBead1->GetZPos() < IGlobalSimBox::Instance()->GetSimBoxZLength()-1
            && pBead2->GetXPos() > 1.0 && pBead2->GetXPos() < IGlobalSimBox::Instance()->GetSimBoxXLength()-1
            && pBead2->GetYPos() > 1.0 && pBead2->GetYPos() < IGlobalSimBox::Instance()->GetSimBoxYLength()-1
            && pBead2->GetZPos() > 1.0 && pBead2->GetZPos() < IGlobalSimBox::Instance()->GetSimBoxZLength()-1
            && GetSSBonds(GetStickySiteFromBead(pBead1->GetId())) < m_MaxBonds-1
            && GetSSBonds(GetStickySiteFromBead(pBead2->GetId())) < m_MaxBonds-1
            && GetStickySiteFromBead(pBead1->GetId()) != GetStickySiteFromBead(pBead2->GetId())
            && AreStickySitesUnbonded(GetStickySiteFromBead(pBead1->GetId()), GetStickySiteFromBead(pBead2->GetId()))
            && IsClusterSizeAllowed(GetStickySiteFromBead(pBead1->GetId()), GetStickySiteFromBead(pBead2->GetId())))
            {
                // Increment the polymer's polymerised bond totals to prevent
                // excess polymerisation.
                
                ++m_PolyBondCounter;
                
                // Store the bead pair using the index as the key
                
                IncrementBondTotal(pBead1->GetId(), pBead2->GetId());
                
                // Store the sticky-site pair
                
                m_mStickySiteBonded[MakeStickySiteBondIndex(GetStickySiteFromBead(pBead1->GetId()), GetStickySiteFromBead(pBead2->GetId()))] = true;
                
                // Updates the clusters 

                UpdateStickySiteClusters(GetStickySiteFromBead(pBead1->GetId()), GetStickySiteFromBead(pBead2->GetId()));

                // Create a bond between head-head and set its parameters.
                // Translate all newly-bonded beads so that their unPBC coordinates
                // are in the principal image of the SimBox. This ensures that the
                // bond force calculations in CBond do not calculate forces based
                // on beads in different images of the SimBox. This does not break
                // the diffusion constant calculation as the newly-created bonds would
                // have changed the beads' evolution from their unbonded states anyway.
                    
                CBond* pBond = new CBond(*m_pPolyBondTemplate);    // Use copy constructor
                        
                pBond->SetId(m_pSimBox->GetSimBoxBonds().size());
                pBond->SetBeads(pBead1, pBead2);
                pBond->SetBeadIndex(pBead1->GetId(), pBead2->GetId());
                                                          
                // Add the new bond to the ordinary bond container

                m_pSimBox->IModifyBondTypes()->AddPolymerisedBond(pBond);
                
                // Recalculate how many bonds of each type exist and tell the CMonitor
                // to update its local counters. We do this for each case because we don't
                // want to call it if no new bonds were added.

                m_pSimBox->IModifyBondTypes()->CalculateTypeTotals();
                m_pSimBox->UpdateBondTypes();

                }
            }
    	}
    
    if(m_pSimBox->TimeToSample())
    {
        // Calculate current number of clusters and statistics on cluster size
    
        double meanClusterSize   = 0.0;
        double meanSqClusterSize = 0.0;
        double sdevClusterSize   = 0.0;
    
    
    	long clusterTotal = 0;
    	for(auto iterCluster = m_mClusterSize.begin(); iterCluster != m_mClusterSize.end(); ++iterCluster)
        {
            if(iterCluster->second != 0)
            {
               ++clusterTotal;
               meanClusterSize   += iterCluster->second;
               meanSqClusterSize += (iterCluster->second)*(iterCluster->second);
               
//               std::cout << "Cluster id / size " << iterCluster->first << " " << iterCluster->second << zEndl;
            }
        }

        // Normalise the data
        
        if(clusterTotal > 0)
        {
            meanClusterSize   /= static_cast<double>(clusterTotal);
            meanSqClusterSize /= static_cast<double>(clusterTotal);
            
            // Note that if the square mean equals the mean squared, the sdev is zero anyway, so we don't have to calculate it then.
            
            if(meanSqClusterSize > meanClusterSize*meanClusterSize)
            {
                sdevClusterSize = sqrt(meanSqClusterSize - meanClusterSize*meanClusterSize);
            }
        }
        
        // Write the data to the decorator state file
    
        long dataTotal = 5;

        CTimeSeriesData* const pTSD = new CTimeSeriesData(dataTotal);

        pTSD->SetValue(0, simTime,           "Time");
        pTSD->SetValue(1, m_PolyBondCounter, "PolyBonds");
        pTSD->SetValue(2, clusterTotal,      "ClusterTotal");
        pTSD->SetValue(3, meanClusterSize,   "MeanClusterSize");
        pTSD->SetValue(4, sdevClusterSize,   "SdevClusterSize");

        m_pState->AddTimeSeriesData(pTSD);
 
        if(SaveState())
        {
            zString logText = GetActiveCommandTarget()->GetLabel() + " target has " + ToString(m_PolyBondCounter) + " polymerised bonds";
	    new CLogTextMessage(m_pSimBox->GetCurrentTime(), logText);
        }
        else
        {
            new CLogTextWarningMessage(simTime, "Error serialising data from decorator " + GetLabel() + " around target " + GetActiveCommandTarget()->GetLabel());
        }
    } 

    
}



// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. 

zInStream& taPolymerisePolymers::Read(zInStream& is)
{

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// First we write the generic target data from the base class (its label and 
// those of its inner and outer decorators), then we add the target-specific data. 

zOutStream& taPolymerisePolymers::Write(zOutStream& os) const
{
    
    return os;
}
