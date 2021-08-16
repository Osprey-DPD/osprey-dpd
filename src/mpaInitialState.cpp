/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// mpaInitialState.cpp: implementation of the mpaInitialState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "mpaInitialState.h"
#include "InitialState.h"
#include "InitialStateData.h"
#include "pmCreateTypesData.h"
#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"
#include "Polymer.h"


// ****************************************
// Global members 



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// We store a pointer to the containing instance so that we can access data
// needed to create and manage the initial state.

mpaInitialState::mpaInitialState(CInitialState* const pSIS) : m_pSerialIS(pSIS),
                                                              m_BeadTotal(0),
                                                              m_BondTotal(0),
                                                              m_BondPairTotal(0),
                                                              m_PolymerTotal(0),
								m_SimBoxXLength(pSIS->GetSimBoxXLength()),
								m_SimBoxYLength(pSIS->GetSimBoxYLength()),
								m_SimBoxZLength(pSIS->GetSimBoxZLength()),
                                                              m_SimBoxXOrigin(0.0),
                                                              m_SimBoxYOrigin(0.0),
                                                              m_SimBoxZOrigin(0.0),
								m_SimSpaceXLength(0.0),
								m_SimSpaceYLength(0.0),
							        m_SimSpaceZLength(0.0)
{
    m_vPolymerTypeTotal.clear();
    m_vPolymerFractions.clear();
	
    // Initialise the polymer fractions container to contain zeroes if using the parallel codebase
	
#if EnableParallelSimBox == SimMPSEnabled
     m_vPolymerFractions.resize(m_pSerialIS->GetPolymerTypeTotal(), 0);
#endif
		
    // Assign the coordinates of the processor's origin in the whole simulation Space and the dimensions 
	// of the Space. We cannot use the xxParallelBase class static members as they have not been set by
	// mpsSimBox yet.

    const long pi = GetRank() % pSIS->GetProcessorsXNo();
    const long pj = ((GetRank() - pi)/pSIS->GetProcessorsXNo()) % pSIS->GetProcessorsYNo();
    const long pk = (((GetRank() - pi)/pSIS->GetProcessorsXNo()) - pj)/pSIS->GetProcessorsYNo();

    m_SimBoxXOrigin = static_cast<double>(pi)*m_SimBoxXLength;
    m_SimBoxYOrigin = static_cast<double>(pj)*m_SimBoxYLength;
    m_SimBoxZOrigin = static_cast<double>(pk)*m_SimBoxZLength;
	
    m_SimSpaceXLength = static_cast<double>(pSIS->GetProcessorsXNo())*m_SimBoxXLength;
    m_SimSpaceYLength = static_cast<double>(pSIS->GetProcessorsYNo())*m_SimBoxYLength;
    m_SimSpaceZLength = static_cast<double>(pSIS->GetProcessorsZNo())*m_SimBoxZLength;
	
}

mpaInitialState::mpaInitialState(const mpaInitialState& oldIS) : m_pSerialIS(oldIS.m_pSerialIS),
                                                                 m_BeadTotal(oldIS.m_BeadTotal),
                                                                 m_BondTotal(oldIS.m_BondTotal),
                                                                 m_BondPairTotal(oldIS.m_BondPairTotal),
                                                                 m_PolymerTotal(oldIS.m_PolymerTotal),
                                                                 m_SimBoxXLength(oldIS.m_SimBoxXLength),
                                                                 m_SimBoxYLength(oldIS.m_SimBoxYLength),
                                                                 m_SimBoxZLength(oldIS.m_SimBoxZLength),
                                                                 m_SimBoxXOrigin(oldIS.m_SimBoxXOrigin),
                                                                 m_SimBoxYOrigin(oldIS.m_SimBoxYOrigin), 
                                                                 m_SimBoxZOrigin(oldIS.m_SimBoxZOrigin),
                                                                 m_SimSpaceXLength(oldIS.m_SimSpaceXLength),
								   m_SimSpaceYLength(oldIS.m_SimSpaceYLength),
								   m_SimSpaceZLength(oldIS.m_SimSpaceZLength),
								   m_vPolymerTypeTotal(oldIS.m_vPolymerTypeTotal),
								   m_vPolymerFractions(oldIS.m_vPolymerFractions)

{

}

mpaInitialState::~mpaInitialState()
{

}

// Parallel initial state function that replaces the serial version. Because each processor has to create its own polymers,
// beads, bonds, etc, and the numbers of each may not be the same for all processors, P0 handles the routing of ids between 
// processors.
// First P0 calculates the numbers of polymers, bead, bond, etc, types that are required in its own space. Then it iterates over
// all PN sending the current totals, which are used to assign ids, to each processor in turn which then return the new totals.

bool mpaInitialState::CreatePolymers()
{
#if EnableParallelSimBox == SimMPSEnabled

    pmCreateTypesData* pMsg = dynamic_cast<pmCreateTypesData*>(acfParallelMessageFactory::Instance()->Create(pmCreateTypesData::GetType()));
	
	// Copy the polymer fractions into a local array for modification according to the concrete pre-assembled IS
	
	m_vPolymerFractions.clear();
	for(long type=0; type<m_pSerialIS->GetPolymerTypeTotal(); type++)
	{
	    m_vPolymerFractions.push_back(m_pSerialIS->GetPolymerFractionForType(type));
	}
	
	// Now modify them
	
	m_pSerialIS->m_pISD->CalculatePolymerFractionsP(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength, m_SimBoxXOrigin, m_SimBoxYOrigin, m_SimBoxZOrigin, 
	                                             m_SimSpaceXLength, m_SimSpaceYLength, m_SimSpaceZLength, m_vPolymerFractions);													 
	
    if(IsProcessZero())
    {
	    // Initialise the entity totals to zero before P0 creates its own. We cannot do this in CreatePolymersP0() as it is also called by PN
		// that uses the totals to set its own entity ids.
		
	     m_BeadTotal	 = 0; 
	     m_BondTotal	 = 0;
	     m_BondPairTotal = 0;
	     m_PolymerTotal	 = 0;

        CreatePolymersP0();
				
        // and pass the data to each PN in turn so that they can uniquely assign ids to the beads, bonds, etc. Note that we have to wait until
		// we receive back the entetiy totals from each processor before we send them to the next one.
		
		for(long procId=1; procId<GetWorld(); procId++)
		{			
		    pMsg->SetMessageData(m_BeadTotal, m_BondTotal, m_BondPairTotal, m_PolymerTotal, m_pSerialIS->GetPolymerTypeTotals());
			
            if(pMsg->Validate())
            {				
		        pMsg->SetTag(0);
				pMsg->Send(procId);
				
				// Now receive the data back from the same processor after it has created its own entities
				

			    pMsg->SetTag(procId);           // set the tag to the procId to uniquely identify each message
                pMsg->Receive();

                m_BeadTotal          = pMsg->GetBeadTotal();
                m_BondTotal          = pMsg->GetBondTotal();
				m_BondPairTotal      = pMsg->GetBondPairTotal();
                m_PolymerTotal       = pMsg->GetPolymerTotal();
                m_vPolymerTypeTotal  = pMsg->GetPolymerTypeTotals();
  
//		        std::cout << "Proc " << GetRank() << " has received from P " << pMsg->GetSenderRank() << " the following entity totals: " << m_BeadTotal << " " << m_BondTotal << " " << m_BondPairTotal << " " << m_PolymerTotal << " " << m_vPolymerTypeTotal.size() << zEndl;
//                std::cout << " and has the following numbers of each polymer type: " << zEndl;
//				for(long jj=0; jj<m_vPolymerTypeTotal.size(); jj++)
//				{
//				    std::cout << " with polymer type " << jj << " having total " << m_vPolymerTypeTotal.at(jj) << zEndl;
//				}
			}
			else
			{
                // Handle error condition here
                std::cout << "mpaInitialState assembly failed on processor " << GetRank() << " P0 unable to validate data to send to P " << procId << zEndl;
		        std::cout << "P0 last had the following entity totals: " << m_BeadTotal << " " << m_BondTotal << " " << m_BondPairTotal << " " << m_PolymerTotal << zEndl;
                //		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
		        return false;
			}			
        }
    }
    else
    {
        // Receive the data defining how many beads, bonds, polymers have already been created and assign ids to the polymers
		// created here

        pMsg->Receive();

        m_BeadTotal          = pMsg->GetBeadTotal();
        m_BondTotal          = pMsg->GetBondTotal();
        m_BondPairTotal      = pMsg->GetBondPairTotal();
        m_PolymerTotal       = pMsg->GetPolymerTotal();
        m_vPolymerTypeTotal  = pMsg->GetPolymerTypeTotals();

//		std::cout << "Proc " << GetRank() << " has received from " << pMsg->GetSenderRank() << " the following entity totals: " << m_BeadTotal << " " << m_BondTotal << " " << m_BondPairTotal << " " << m_PolymerTotal << " " << m_vPolymerTypeTotal.size() << zEndl;

        CreatePolymersP0();
		
//		std::cout << "Proc " << GetRank() << " and has updated this to the following entity totals: " << m_BeadTotal << " " << m_BondTotal << " " << m_BondPairTotal << " " << m_PolymerTotal  << " " << m_vPolymerTypeTotal.size()<< zEndl;
//		for(long pt=0; pt<m_pSerialIS->m_vPolymerTypeTotal.size(); pt++)
//		{
//		    std::cout << "Proc " << GetRank() << " has " << m_pSerialIS->m_vPolymerTypeTotal.at(pt) << " copies of polymer type " << pt << zEndl;
//		}
		// Send the new totals back to P0
		
		pMsg->SetMessageData(m_BeadTotal, m_BondTotal, m_BondPairTotal, m_PolymerTotal, m_pSerialIS->m_vPolymerTypeTotal);

		if(pMsg->Validate())
		{
			    pMsg->SetTag(GetRank());           // set the tag to the rank to uniquely identify each message
				pMsg->SendP0();
	    }
		else
		{
			// Handle error condition here
			std::cout << "mpaInitialState assembly failed on processor " << GetRank() << " P " << GetRank() << " unable to validate data to send to P0" << zEndl;
			std::cout << "Proc " << GetRank() << " last had the following entity totals: " << m_BeadTotal << " " << m_BondTotal << " " << m_BondPairTotal << " " << m_PolymerTotal << zEndl;
			//		   CLogSimParallelInputDataError* pMsg = new CLogSimParallelInputDataError(0);
			return false;
		}
    }

	delete pMsg;
	
	// Ensure we synchronize the start of the run

	MPI_Barrier(MPI_COMM_WORLD);  // Keep all processors synchronised each time-step
	
	return true;
	
#else
    return false;
#endif
}

// Parallel version of the CInitialState::CreatePolymers() function that is used only by P0. Because a parallel initial state is 
// not necessarily translationally invarianent, the numbers of polymers created by each processor may be different, so we cannot
// calculate how many of each type are needed without information about the structure of the initial state. Here, we access the
// CInitialStateData object contained in the CInitialState instance to calculate what we must create on each processor. We have
// access to the ISD data as we are a friend class of CInitialState.
//
// The fraction of each polymer type required in the owning processor is given in the m_vPolymerFractions container.
//
// NB The random initial state is special in that the same number of all types of polymer are created on all processors as 
// we assume translational symmetry of the initial state.

void mpaInitialState::CreatePolymersP0()
{
#if EnableParallelSimBox == SimMPSEnabled

	// First calculate the numbers of each polymer required in the whole simulation Space from their number fraction
	// and the total number of beads in the simulation Space.

	double totalBeads = m_pSerialIS->AveBeadDensity*GetSimBoxXLength()*GetSimBoxYLength()*GetSimBoxZLength();

	double nalphasum = 0.0;

	for(long unsigned int type=0; type<m_pSerialIS->vPolymerTypes.size(); type++)
	{
		nalphasum+= static_cast<double>(m_pSerialIS->GetPolymerSizeForType(type))*m_vPolymerFractions.at(type);
	}
	
	for(long unsigned int type1=0; type1<m_pSerialIS->vPolymerTypes.size(); type1++)
	{
		m_pSerialIS->m_vPolymerTypeTotal.push_back(static_cast<long>(m_vPolymerFractions.at(type1)*totalBeads/nalphasum));
	}

	// Now we know the required numbers of polymers of each type we can create 
	// them using the template polymers and let the CPolymer copy constructor
	// create the beads and bonds. Note that we have to set the 
	// bead and bond ids here as the copy constructor does not have access to
	// the counters. Also note that we are passing the total numbers of beads,
	// bonds and polymers by reference so that we guarantee that the variables
	// is valid when the CPolymer routines increment them.

	// We also store pointers to all the beads, bonds and polymers in single 
	// arrays to speed up the loops in the CSimBox.

	for(cPolymerVectorIterator iterPolyType=m_pSerialIS->vPolymerTypes.begin(); iterPolyType!=m_pSerialIS->vPolymerTypes.end(); iterPolyType++)
	{
		for(long pno=0; pno<m_pSerialIS->m_vPolymerTypeTotal.at((*iterPolyType)->GetType()); pno++)
		{
			m_PolymerTotal++;
			CPolymer* pPolymer = new CPolymer(**iterPolyType);
			pPolymer->SetId(m_PolymerTotal);

			m_BeadTotal		= pPolymer->SetBeadIds(m_BeadTotal);
			m_BondTotal		= pPolymer->SetBondIds(m_BondTotal);
			m_BondPairTotal	= pPolymer->SetBondPairIds(m_BondPairTotal);

			m_pSerialIS->vAllPolymers.push_back(pPolymer);

			copy((pPolymer->GetBeads()).begin(),	(pPolymer->GetBeads()).end(),		back_inserter(m_pSerialIS->vAllBeads));
			copy((pPolymer->GetBonds()).begin(),	(pPolymer->GetBonds()).end(),		back_inserter(m_pSerialIS->vAllBonds));
			copy((pPolymer->GetBondPairs()).begin(),(pPolymer->GetBondPairs()).end(),	back_inserter(m_pSerialIS->vAllBondPairs));
		}
	}
	
	m_pSerialIS->CalculateBeadTypeTotals();
	m_pSerialIS->CalculateBondTypeTotals();
	m_pSerialIS->CalculateBondPairTypeTotals();

#endif
}


