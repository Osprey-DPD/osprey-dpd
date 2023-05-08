/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prSAXS.cpp: implementation of the prSAXS class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimXMLFlags.h"
#include "prSAXS.h"
#include "IGlobalSimBox.h"
#include "SimState.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Polymer.h"
#include "TimeSeriesData.h"
#include "InputData.h"

#include <unistd.h>
#include <algorithm>
#include <numeric>
#include <thread>


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.
//

static double now()
{
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec*1e-9;
}
    
const zString prSAXS::m_Type = "SAXS";

const zString prSAXS::GetType()
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
	xxProcess* Create() {return new prSAXS();}

	const zString id = prSAXS::GetType();

	const bool bRegistered = acfProcessFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Default constructor

prSAXS::prSAXS() : m_AnalysisPeriods(0), m_QPoints(0), m_QMin(0.0), m_QMax(0.0), m_dQ(0.0), m_SamplePeriod(0), m_SampleTotal(0), m_SamplesTaken(0)
{
    m_mBeadTypes.clear();
	m_vBeads.clear();
    m_vIQ.clear();
    m_vIQSq.clear();
    m_mElectronNo.clear();
}

// Constructor for use when the process is created by command and the user does not specify qmin and qmax.
// The default values using the box size and bead diameter are used. Note that the SimBox must be cubic
// We do NOT check that the analysis performed by this process can be completed during the run, nor that
// the number of bead types is constant during the analysis period. If the context
// within which this process operates becomes invalid, the process terminates and 
// logs an error message to the log file.

prSAXS::prSAXS(const CSimState* const pSimState,
									long analysisPeriods,
									long qPoints, LongLongMap mBeadTypes) :
                                    m_AnalysisPeriods(analysisPeriods),
									m_QPoints(qPoints),
                                    m_QMin(xxBase::m_globalTwoPI/IGlobalSimBox::Instance()->GetSimBoxZLength()),
                                    m_QMax(xxBase::m_globalTwoPI),
                                    m_dQ((m_QMax - m_QMin)/static_cast<double>(m_QPoints-1)),
									m_SamplePeriod(0), m_SampleTotal(0), m_SamplesTaken(0),
                                    m_mBeadTypes(mBeadTypes)
{
	m_vBeads.clear();
    m_vIQ.resize(m_QPoints, 0.0);  // This can be overwritten in the sum over bead pairs
    m_vIQSq.resize(m_QPoints, 0.0);  // This can be overwritten in the sum over bead pairs

    // Empty the map of the electron numbers for all bead types. Note that maps only allow a single entry, so
    // we don't try and fill it with zeroes and overwrite.  We would have to remove an element before we added the new one.
    
    m_mElectronNo.clear();
    
    // Set the times at which the process' analysis will be performed. This is
    // defined to be the time from the start of the next full analysis period as
    // defined in the CMonitor until the end of the number of such periods given 
    // by m_AnalysisPeriods.

    long currentTime      = IGlobalSimBox::Instance()->GetCurrentTime();
    long analysisPeriod   = pSimState->GetAnalysisPeriod();
    m_SamplePeriod		  = pSimState->GetSamplePeriod();
    long duration         = m_AnalysisPeriods*analysisPeriod;
    m_SampleTotal         = duration/m_SamplePeriod;

    long start = 0;
    long end   = 0;

    if(currentTime%analysisPeriod == 0)
    {
        start = currentTime;
    }
    else
    {
        start = (currentTime/analysisPeriod + 1)*analysisPeriod;
    }

    end = start + duration;

	SetStartTime(start);
	SetEndTime(end);

	// Store the beads whose types have been specified as contributing to the SAXS pattern
	
	PolymerVector vAllPolymers = pSimState->GetPolymers();
	
	for(PolymerVectorIterator iterPoly=vAllPolymers.begin(); iterPoly != vAllPolymers.end(); ++iterPoly)
	{
        BeadVector vAllBeads = (*iterPoly)->GetBeads();
			
        for(cBeadVectorIterator iterBead = vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
        {
            if(m_mBeadTypes.find((*iterBead)->GetType()) != m_mBeadTypes.end())
            {
                m_vBeads.push_back(*iterBead);
            }
        }
	}
	
	// Debug output of the total beads
	
	std::cout << "prSAXS using " << m_mBeadTypes.size() << " bead types, and found " << m_vBeads.size() << " total beads" << zEndl;
    std::cout << "Using default limits " << m_QMin << " " << m_QMax << zEndl;

}

// Constructor for use when the process is created by command and the user specifies the minimum and maximum q values.
//
// We do NOT check that the analysis performed by this process can be completed during the run, nor that
// the number of bead types is constant during the analysis period. If the context
// within which this process operates becomes invalid, the process terminates and
// logs an error message to the log file.

prSAXS::prSAXS(const CSimState* const pSimState,
                                    long analysisPeriods,
                                    long qPoints,
                                    double qMin, double qMax,
                                    LongLongMap mBeadTypes) :
                                    m_AnalysisPeriods(analysisPeriods),
                                    m_QPoints(qPoints),
                                    m_QMin(qMin), m_QMax(qMax),
                                    m_dQ((m_QMax - m_QMin)/static_cast<double>(m_QPoints-1)),
                                    m_SamplePeriod(0), m_SampleTotal(0), m_SamplesTaken(0),
                                    m_mBeadTypes(mBeadTypes)
{
    m_vBeads.clear();
    m_vIQ.resize(m_QPoints, 0.0);
    m_vIQSq.resize(m_QPoints, 0.0);

    // Empty the map of the electron numbers for all bead types. Note that maps only allow a single entry, so
    // we don't try and fill it with zeroes and overwrite.  We would have to remove an element before we added the new one.
    
    m_mElectronNo.clear();
    
    // Set the times at which the process' analysis will be performed. This is
    // defined to be the time from the start of the next full analysis period as
    // defined in the CMonitor until the end of the number of such periods given
    // by m_AnalysisPeriods.

    long currentTime      = IGlobalSimBox::Instance()->GetCurrentTime();
    long analysisPeriod   = pSimState->GetAnalysisPeriod();
    m_SamplePeriod          = pSimState->GetSamplePeriod();
    long duration         = m_AnalysisPeriods*analysisPeriod;
    m_SampleTotal         = duration/m_SamplePeriod;

    long start = 0;
    long end   = 0;

    if(currentTime%analysisPeriod == 0)
    {
        start = currentTime;
    }
    else
    {
        start = (currentTime/analysisPeriod + 1)*analysisPeriod;
    }

    end = start + duration;

    SetStartTime(start);
    SetEndTime(end);

    // Store the beads of the selected polymer types that occur within the selected polymer type
    
    PolymerVector vAllPolymers = pSimState->GetPolymers();
    
    for(PolymerVectorIterator iterPoly=vAllPolymers.begin(); iterPoly != vAllPolymers.end(); ++iterPoly)
    {
        BeadVector vAllBeads = (*iterPoly)->GetBeads();
            
        for(cBeadVectorIterator iterBead = vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
        {
            if(m_mBeadTypes.find((*iterBead)->GetType()) != m_mBeadTypes.end())
            {
                m_vBeads.push_back(*iterBead);
            }
        }
    }

    // Debug output of the total beads
    
    std::cout << "prSAXS using " << m_mBeadTypes.size() << " bead types, and found " << m_vBeads.size() << " total beads" << zEndl;
    std::cout << "Using user-defined limits " << m_QMin << " " << m_QMax << " step " << m_dQ << zEndl;

}


prSAXS::~prSAXS()
{

}

// Member functions to write/read the data specific to the process.
// The put() function is empty as the base class putASCIIStartTags(), 
// putASCIIEndTags() and their XML equivalents replace its function.

zOutStream& prSAXS::put(zOutStream& os) const
{
	return os;
}

zInStream& prSAXS::get(zInStream& is)
{
	// Read base class data first

	xxProcess::get(is);

	// No data entry required for this process as it is internal

	return is;
}


// Non-static function to return the type of the process

const zString prSAXS::GetProcessType() const
{
	// We return the associated ACN's type so that the ACN and process
	// are guaranteed to return the same string

	return prSAXS::GetType();
}

// Function to return a pointer to a copy of the current process.

xxProcess* prSAXS::GetProcess() const
{
	return new prSAXS(*this);
}

// Function to update the state of the process from the bead data stored in the
// CSimState. It is called every m_SamplePeriod time steps to collect data,
// but defines its own averaging period.
//
// This process computes the scattering function I(q) using the results from the paper by Paissoni et al lJCTC 2020.
// We sum over all pairs of beads (including i = j), and weight them by the Fourier transform of their separation assuming
// spherical symmetry.

void prSAXS::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	if(pISimBox->GetCurrentTime() > GetStartTime() && pISimBox->GetCurrentTime() <= GetEndTime())
	{
		m_SamplesTaken++;

		std::cout << "Sample " <<  m_SamplesTaken << " of I(q) at time " << pISimBox->GetCurrentTime() << " " << m_QMin << " " << m_QMax << " " << m_dQ << " " << m_QPoints << zEndl;

        const bool verify=false;
        if(verify){
            std::vector<double> origIq=m_vIQ;
            std::vector<double> origIqSqr=m_vIQSq;

            double begin=now();		
            UpdateState_InnerLoop_ref(rSimState, pISimBox);
            double end=now();
            std::cout<<"   Ref took "<<(end-begin)<<"secs\n";
            std::vector<double> refIq=m_vIQ;
            std::vector<double> refIqSqr=m_vIQSq;
            

            m_vIQ.assign(origIq.begin(), origIq.end());
            m_vIQSq.assign(origIqSqr.begin(), origIqSqr.end());
            begin=now();		
            UpdateState_InnerLoop_v4_threads(rSimState, pISimBox);
            end=now();
            std::cout<<"   v2 took "<<(end-begin)<<"secs\n";

            for(unsigned i=0; i<refIq.size(); i++){
                std::cout<<"   "<<i<<", "<<refIq[i]<<", "<<m_vIQ[i]<<"\n";
            }
        }else{
            UpdateState_InnerLoop_v4_threads(rSimState, pISimBox);
        }


		if(m_SamplesTaken == m_SampleTotal)
		{
			CTimeSeriesData* const pTSD = new CTimeSeriesData(m_QPoints+1);
			
			pTSD->SetValue(0,  pISimBox->GetCurrentTime(), "Time");
					
			m_pState->AddTimeSeriesData(pTSD);
			            
            // Now write the I(q) to file, after normalising the result by the number of bead pairs and samples taken.

            const double invTotalBeadPairs = 1.0/static_cast<double>(m_vBeads.size()*m_vBeads.size());
            const double invSamplesTaken = 1.0/static_cast<double>(m_SamplesTaken);
            
        
            for(long iq=0; iq < m_QPoints; ++iq)
            {
                m_vIQ.at(iq)   *= invTotalBeadPairs;
                m_vIQ.at(iq)   *= invSamplesTaken;
                
                m_vIQSq.at(iq) *= invTotalBeadPairs;
                m_vIQSq.at(iq) *= invSamplesTaken;

                const double iqSDev = sqrt(m_vIQSq.at(iq) - m_vIQ.at(iq)*m_vIQ.at(iq));
 
                pTSD->SetValue(iq+1, m_vIQ.at(iq), iqSDev, "I(q)");
            }

            std::cout << "Dumping I(q) to file at time " << pISimBox->GetCurrentTime() << " with q points " << m_QPoints << " dq " << m_dQ << " and samples " << m_SamplesTaken << " and total bead pairs " << 1.0/invTotalBeadPairs << zEndl;
		}
     }

}



void prSAXS::UpdateState_InnerLoop_ref(CSimState& rSimState, const ISimBox* const pISimBox)
{
    double dx[3];
        
    // Define the min and max q values from the box size and bead diameter.

    // Loop over all q values, adding the contributions from each bead pair weighted by their electon numbers.
    // To svoid double counting, we start the second loop with the iterator equal to the first.
    
    double qvalue = m_QMin;

    for(long iq = 0; iq < m_QPoints; ++iq)
    {
        if( (iq%10) == 0){
            std::cerr<<iq<<" of "<<m_QPoints<<"\n";
        }

        for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1)
        {
            const double eno1 = m_mElectronNo.find((*iterBead1)->GetType())->second;

                // Include the double counting of off diagonal elements
            
            for(cBeadVectorIterator iterBead2 = m_vBeads.begin(); iterBead2!=m_vBeads.end(); ++iterBead2)
            {
                const double eno2 = m_mElectronNo.find((*iterBead2)->GetType())->second;
    
                dx[0] = (*iterBead1)->GetXPos() - (*iterBead2)->GetXPos();
                dx[1] = (*iterBead1)->GetYPos() - (*iterBead2)->GetYPos();
                dx[2] = (*iterBead1)->GetZPos() - (*iterBead2)->GetZPos();

                // Correct for the PBCs
/*
                if( dx[0] > IGlobalSimBox::Instance()->GetHalfSimBoxXLength() )
                    dx[0] = dx[0] - IGlobalSimBox::Instance()->GetSimBoxXLength();
                else if( dx[0] < -IGlobalSimBox::Instance()->GetHalfSimBoxXLength() )
                    dx[0] = dx[0] + IGlobalSimBox::Instance()->GetSimBoxXLength();

                if( dx[1] > IGlobalSimBox::Instance()->GetHalfSimBoxYLength() )
                    dx[1] = dx[1] - IGlobalSimBox::Instance()->GetSimBoxYLength();
                else if( dx[1] < -IGlobalSimBox::Instance()->GetHalfSimBoxYLength() )
                    dx[1] = dx[1] + IGlobalSimBox::Instance()->GetSimBoxYLength();

                #if SimDimension == 3
                dx[2] = (*iterBead1)->GetZPos() - (*iterBead2)->GetZPos();

                if( dx[2] > IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
                    dx[2] = dx[2] - IGlobalSimBox::Instance()->GetSimBoxZLength();
                else if( dx[2] < -IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
                    dx[2] = dx[2] + IGlobalSimBox::Instance()->GetSimBoxZLength();
                #else
                    dx[2] = 0.0;
                #endif
*/
                    double dr = sqrt(dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2]);
                    
//                    std::cout << "out: iq " << iq << " " << qvalue << " " << eno1 << " " << eno2 << " " << dr << zEndl;

                
                if(dr > 0.000001)
                {
                    const double dvalue = (eno1*eno2*sin(qvalue*dr)/(qvalue*dr));
                    m_vIQ.at(iq) += dvalue;
                    m_vIQSq.at(iq) += dvalue*dvalue;

//                        std::cout << "Diff: " << iq << " " << qvalue << " " << dvalue << zEndl;
                }
                else
                {
                    // i = j here so we have the same bead, and because Sinx/x = 1 when x = 0, the contribution is just F(q)**2.
                    
                    const double eprod =eno1*eno2;
                    m_vIQ.at(iq) += eprod;
                    m_vIQSq.at(iq) += (eprod*eprod);

//                        std::cout << "adding " << eno1 << " " << eno2 << " " << qvalue << " " << dr << zEndl;
                }
                
            }
        }
        
        
//            std::cout << "After: iq " << iq << " " << qvalue << " " << m_vIQ.at(iq) << zEndl;
        
        qvalue += m_dQ;
    }
}

double prSAXS::DistanceMetric(const CAbstractBead *a, const CAbstractBead *b) const
{
    double dx[3];
    dx[0] = a->GetXPos() - b->GetXPos();
    dx[1] = a->GetYPos() - b->GetYPos();
    dx[2] = a->GetZPos() - b->GetZPos();
    return sqrt(dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2]);
}

float prSAXS::DistanceMetric(const float *a, const float *b) const
{
    float dx[3];
    dx[0] = a[0] - b[0];
    dx[1] = a[1] - b[1];
    dx[2] = a[2] - b[2];
    return sqrt(dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2]);
}

void prSAXS::UpdateState_InnerLoop_v1(CSimState& rSimState, const ISimBox* const pISimBox)
{        
    // Define the min and max q values from the box size and bead diameter.

    // Loop over all q values, adding the contributions from each bead pair weighted by their electon numbers.
    // To svoid double counting, we start the second loop with the iterator equal to the first.

    const auto &bead_types=rSimState.GetBeadTypes();
    std::vector<double> enoCache(bead_types.size());
    for(unsigned i=0; i<bead_types.size(); i++){
        const auto pbt=bead_types[i];
        assert(pbt->GetType()==i);
        enoCache[i]=m_mElectronNo.at(i);
    }
    
    for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1)
    {
        const double eno1 = enoCache[(*iterBead1)->GetType()];

        for(cBeadVectorIterator iterBead2 = m_vBeads.begin(); iterBead2!=m_vBeads.end(); ++iterBead2)
        {
            const double eprod = eno1 * enoCache[(*iterBead2)->GetType()];
            const double dr = DistanceMetric(*iterBead1, *iterBead2);
                            
            if(dr > 0.000001)
            {
                for(long iq = 0; iq < m_QPoints; ++iq)
                {
                    const double qvalue = m_QMin + iq*m_dQ;
                    const double dvalue = eprod*sin(qvalue*dr)/(qvalue*dr);
                    m_vIQ[iq] += dvalue;
                    m_vIQSq[iq] += dvalue*dvalue;
                }
            }
            else
            {
                // i = j here so we have the same bead, and because Sinx/x = 1 when x = 0, the contribution is just F(q)**2.                
                for(long iq = 0; iq < m_QPoints; ++iq)
                {
                    m_vIQ[iq] += eprod;
                    m_vIQSq[iq] += (eprod*eprod);
                }
            }
        }
    }
}


void prSAXS::UpdateState_InnerLoop_v2(CSimState& rSimState, const ISimBox* const pISimBox)
{        
    // Define the min and max q values from the box size and bead diameter.

    // Loop over all q values, adding the contributions from each bead pair weighted by their electon numbers.
    // To svoid double counting, we start the second loop with the iterator equal to the first.

    const auto &bead_types=rSimState.GetBeadTypes();
    std::vector<float> enoCache(bead_types.size());
    for(unsigned i=0; i<bead_types.size(); i++){
        const auto pbt=bead_types[i];
        assert(pbt->GetType()==i);
        auto it= m_mElectronNo.find(i);
        if(it!=m_mElectronNo.end()){
            enoCache[i]=it->second;
        }
    }

    struct bead_info_t
    {
        float x[3];
        float eno;
    };

    std::vector<bead_info_t> pos_cache;
    pos_cache.reserve(m_vBeads.size());
    for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1){
        const auto &b=**iterBead1;
        bead_info_t bi{
            { (float)b.GetXPos(), (float)b.GetYPos(), (float)b.GetZPos()},
            enoCache[b.GetType()]
        };
        pos_cache.push_back(bi);
    }
    
    float QMin = m_QMin;
    float dQ = m_dQ;

    for(unsigned i1=0; i1<pos_cache.size(); i1++){
        const auto &b1=pos_cache[i1];

        const float esquare=b1.eno*b1.eno;
        for(long iq = 0; iq < m_QPoints; ++iq)
        {
            m_vIQ[iq] += esquare;
            m_vIQSq[iq] += (esquare*esquare);
        }

        for(unsigned i2=i1+1; i2<pos_cache.size(); i2++){
            const auto &b2=pos_cache[i2];
    
            // We multiply by 2 here as this only does (i1<i2), so we need to double it to cover (i2<i1)
            // (i1==i2) is covered by the earlier special-case loop
            const float eprod = 2 * b1.eno * b2.eno;
            const float dr = DistanceMetric(b1.x, b2.x);
              
            if(dr > 0.000001)
            {
                for(long iq = 0; iq < m_QPoints; iq++)
                {
                    const float qvalue = QMin + iq*dQ;
                    // TODO: we are evaluating sin(ß (QMin+iq*dQ)*dr )
                    //                         sin( QMin*dr + iq*(dQ*dr) )
                    // So could be done with angle addition formula
                    const float dvalue = eprod*sinf(qvalue*dr)/(qvalue*dr);
                    // Accumulation is still double-precision
                    m_vIQ[iq] += dvalue;
                    m_vIQSq[iq] += dvalue*dvalue;
                }
            }
            else
            {
                // i = j here so we have the same bead, and because Sinx/x = 1 when x = 0, the contribution is just F(q)**2.                
                for(long iq = 0; iq < m_QPoints; ++iq)
                {
                    m_vIQ[iq] += eprod;
                    m_vIQSq[iq] += (eprod*eprod);
                }
            }
        }
    }
}



struct sin_enum_angle_direct
{
    float x;
    float dx;

    sin_enum_angle_direct(float _x, float _dx)
    {
        x=_x;
        dx=_dx;
    }

    float operator()()
    {
        float res=sinf(x);
        x += dx;
        return res;
    }
};
struct sin_enum_angle_recurrence
{
    float cos_dx;
    float sin_dx;
    float sin_x;
    float cos_x;

    sin_enum_angle_recurrence(float x, float dx)
    {
        cos_x=cosf(x);
        sin_x=sinf(x);
        cos_dx=cosf(dx);
        sin_dx=sinf(dx);
    }

    float operator()()
    {
        float res=sin_x;
        float sin_x_next=sin_x * cos_dx + cos_x * sin_dx;
        float cos_x_next=cos_x * cos_dx - sin_x * sin_dx;
        sin_x = sin_x_next;
        cos_x = cos_x_next;
        return res;
    }
};

void prSAXS::UpdateState_InnerLoop_v3(CSimState& rSimState, const ISimBox* const pISimBox)
{        
    // Define the min and max q values from the box size and bead diameter.

    // Loop over all q values, adding the contributions from each bead pair weighted by their electon numbers.
    // To svoid double counting, we start the second loop with the iterator equal to the first.

    const auto &bead_types=rSimState.GetBeadTypes();
    std::vector<float> enoCache(bead_types.size());
    for(unsigned i=0; i<bead_types.size(); i++){
        const auto pbt=bead_types[i];
        assert(pbt->GetType()==i);
        auto it= m_mElectronNo.find(i);
        if(it!=m_mElectronNo.end()){
            enoCache[i]=it->second;
        }
    }

    struct bead_info_t
    {
        float x[3];
        float eno;
    };

    std::vector<bead_info_t> pos_cache;
    pos_cache.reserve(m_vBeads.size());
    for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1){
        const auto &b=**iterBead1;
        bead_info_t bi{
            { (float)b.GetXPos(), (float)b.GetYPos(), (float)b.GetZPos()},
            enoCache[b.GetType()]
        };
        pos_cache.push_back(bi);
    }
    
    float QMin = m_QMin;
    float dQ = m_dQ;

    for(unsigned i1=0; i1<pos_cache.size(); i1++){
        const auto &b1=pos_cache[i1];

        const float esquare=b1.eno*b1.eno;
        for(long iq = 0; iq < m_QPoints; ++iq)
        {
            m_vIQ[iq] += esquare;
            m_vIQSq[iq] += (esquare*esquare);
        }

        for(unsigned i2=i1+1; i2<pos_cache.size(); i2++){
            const auto &b2=pos_cache[i2];
    
            // We multiply by 2 here as this only does (i1<i2), so we need to double it to cover (i2<i1)
            // (i1==i2) is covered by the earlier special-case loop
            const float eprod = 2 * b1.eno * b2.eno;
            const float dr = DistanceMetric(b1.x, b2.x);
              
            if(dr > 0.000001)
            {
                // We are evaluating sin(ß (QMin+iq*dQ)*dr )
                //                         sin( QMin*dr + iq*(dQ*dr) )
                sin_enum_angle_recurrence sin_enum(QMin*dr, dQ*dr);    
                float qvalue=QMin;
                float scale=eprod / dr;
                for(long iq = 0; iq < m_QPoints; iq++)
                {
                    const float dvalue = scale*sin_enum() / qvalue;
                    // Accumulation is still double-precision
                    m_vIQ[iq] += dvalue;
                    m_vIQSq[iq] += dvalue*dvalue;
                    qvalue += dQ;
                }
            }
            else
            {
                // i = j here so we have the same bead, and because Sinx/x = 1 when x = 0, the contribution is just F(q)**2.                
                for(long iq = 0; iq < m_QPoints; ++iq)
                {
                    m_vIQ[iq] += eprod;
                    m_vIQSq[iq] += (eprod*eprod);
                }
            }
        }
    }
}

void prSAXS::UpdateState_InnerLoop_v4(CSimState& rSimState, const ISimBox* const pISimBox)
{        
    // Define the min and max q values from the box size and bead diameter.

    // Loop over all q values, adding the contributions from each bead pair weighted by their electon numbers.
    // To svoid double counting, we start the second loop with the iterator equal to the first.

    const auto &bead_types=rSimState.GetBeadTypes();
    std::vector<float> enoCache(bead_types.size());
    for(unsigned i=0; i<bead_types.size(); i++){
        const auto pbt=bead_types[i];
        assert(pbt->GetType()==i);
        auto it= m_mElectronNo.find(i);
        if(it!=m_mElectronNo.end()){
            enoCache[i]=it->second;
        }
    }

    struct bead_info_t
    {
        float x[3];
        float eno;
    };

    std::vector<bead_info_t> pos_cache;
    pos_cache.reserve(m_vBeads.size());
    for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1){
        const auto &b=**iterBead1;
        bead_info_t bi{
            { (float)b.GetXPos(), (float)b.GetYPos(), (float)b.GetZPos()},
            enoCache[b.GetType()]
        };
        pos_cache.push_back(bi);
    }
    
    float QMin = m_QMin;
    float dQ = m_dQ;

    std::vector<float> vIQLocal(m_QPoints, 0.0f);
    std::vector<float> vIQSqLocal(m_QPoints, 0.0f);

    for(unsigned i1=0; i1<pos_cache.size(); i1++){
        const auto &b1=pos_cache[i1];

        const float esquare=b1.eno*b1.eno;
        for(long iq = 0; iq < m_QPoints; ++iq)
        {
            vIQLocal[iq] += esquare;
            vIQSqLocal[iq] += (esquare*esquare);
        }

        for(unsigned i2=i1+1; i2<pos_cache.size(); i2++){
            const auto &b2=pos_cache[i2];
    
            // We multiply by 2 here as this only does (i1<i2), so we need to double it to cover (i2<i1)
            // (i1==i2) is covered by the earlier special-case loop
            const float eprod = 2 * b1.eno * b2.eno;
            const float dr = DistanceMetric(b1.x, b2.x);
              
            if(dr > 0.000001)
            {
                // We are evaluating sin(ß (QMin+iq*dQ)*dr )
                //                         sin( QMin*dr + iq*(dQ*dr) )
                sin_enum_angle_recurrence sin_enum(QMin*dr, dQ*dr);    
                float qvalue=QMin;
                float scale=eprod / dr;
                for(long iq = 0; iq < m_QPoints; iq++)
                {
                    const float dvalue = scale*sin_enum() / qvalue;
                    // Accumulation is still double-precision
                    vIQLocal[iq] += dvalue;
                    vIQSqLocal[iq] += dvalue*dvalue;
                    qvalue += dQ;
                }
            }
            else
            {
                // i = j here so we have the same bead, and because Sinx/x = 1 when x = 0, the contribution is just F(q)**2.                
                for(long iq = 0; iq < m_QPoints; ++iq)
                {
                    vIQLocal[iq] += eprod;
                    vIQSqLocal[iq] += (eprod*eprod);
                }
            }
        }

        for(unsigned i=0; i<m_QPoints; i++){
            m_vIQ[i] += vIQLocal[i];
            m_vIQSq[i] += vIQSqLocal[i];
            vIQLocal[i] = 0.0f;
            vIQSqLocal[i] = 0.0f;
        }
    }
}

#if OSPREY_DPD_HAVE_OPENMP
void prSAXS::UpdateState_InnerLoop_v4_omp(CSimState& rSimState, const ISimBox* const pISimBox)
{        
    // Define the min and max q values from the box size and bead diameter.

    // Loop over all q values, adding the contributions from each bead pair weighted by their electon numbers.
    // To svoid double counting, we start the second loop with the iterator equal to the first.

    const auto &bead_types=rSimState.GetBeadTypes();
    std::vector<float> enoCache(bead_types.size());
    for(unsigned i=0; i<bead_types.size(); i++){
        const auto pbt=bead_types[i];
        assert(pbt->GetType()==i);
        auto it= m_mElectronNo.find(i);
        if(it!=m_mElectronNo.end()){
            enoCache[i]=it->second;
        }
    }

    struct bead_info_t
    {
        float x[3];
        float eno;
    };

    std::vector<bead_info_t> pos_cache;
    pos_cache.reserve(m_vBeads.size());
    for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1){
        const auto &b=**iterBead1;
        bead_info_t bi{
            { (float)b.GetXPos(), (float)b.GetYPos(), (float)b.GetZPos()},
            enoCache[b.GetType()]
        };
        pos_cache.push_back(bi);
    }
    
    float QMin = m_QMin;
    float dQ = m_dQ;

    std::vector<float> vIQLocal(m_QPoints, 0.0f);
    std::vector<float> vIQSqLocal(m_QPoints, 0.0f);

    #pragma omp parallel for firstprivate(vIQLocal) firstprivate(vIQSqLocal)
    for(unsigned i1=0; i1<pos_cache.size(); i1++){
        const auto &b1=pos_cache[i1];

        const float esquare=b1.eno*b1.eno;
        for(long iq = 0; iq < m_QPoints; ++iq)
        {
            vIQLocal[iq] += esquare;
            vIQSqLocal[iq] += (esquare*esquare);
        }

        for(unsigned i2=i1+1; i2<pos_cache.size(); i2++){
            const auto &b2=pos_cache[i2];
    
            // We multiply by 2 here as this only does (i1<i2), so we need to double it to cover (i2<i1)
            // (i1==i2) is covered by the earlier special-case loop
            const float eprod = 2 * b1.eno * b2.eno;
            const float dr = DistanceMetric(b1.x, b2.x);
              
            if(dr > 0.000001)
            {
                // We are evaluating sin(ß (QMin+iq*dQ)*dr )
                //                         sin( QMin*dr + iq*(dQ*dr) )
                sin_enum_angle_recurrence sin_enum(QMin*dr, dQ*dr);    
                float qvalue=QMin;
                float scale=eprod / dr;
                for(long iq = 0; iq < m_QPoints; iq++)
                {
                    const float dvalue = scale*sin_enum() / qvalue;
                    // Accumulation is still double-precision
                    vIQLocal[iq] += dvalue;
                    vIQSqLocal[iq] += dvalue*dvalue;
                    qvalue += dQ;
                }
            }
            else
            {
                // i = j here so we have the same bead, and because Sinx/x = 1 when x = 0, the contribution is just F(q)**2.                
                for(long iq = 0; iq < m_QPoints; ++iq)
                {
                    vIQLocal[iq] += eprod;
                    vIQSqLocal[iq] += (eprod*eprod);
                }
            }
        }

        #pragma omp critical
        {
            // This is not too bad as long is m_QPoints is small
            for(unsigned i=0; i<m_QPoints; i++){
                m_vIQ[i] += vIQLocal[i];
                m_vIQSq[i] += vIQSqLocal[i];
                vIQLocal[i] = 0.0f;
                vIQSqLocal[i] = 0.0f;
            }
        }
    }
}
#endif

#if OSPREY_DPD_HAVE_STL_PARALLEL
void prSAXS::UpdateState_InnerLoop_v4_stl_parallel(CSimState& rSimState, const ISimBox* const pISimBox)
{        
    // Define the min and max q values from the box size and bead diameter.

    // Loop over all q values, adding the contributions from each bead pair weighted by their electon numbers.
    // To svoid double counting, we start the second loop with the iterator equal to the first.

    const auto &bead_types=rSimState.GetBeadTypes();
    std::vector<float> enoCache(bead_types.size());
    for(unsigned i=0; i<bead_types.size(); i++){
        const auto pbt=bead_types[i];
        assert(pbt->GetType()==i);
        auto it= m_mElectronNo.find(i);
        if(it!=m_mElectronNo.end()){
            enoCache[i]=it->second;
        }
    }

    struct bead_info_t
    {
        float x[3];
        float eno;
    };

    std::vector<bead_info_t> pos_cache;
    pos_cache.reserve(m_vBeads.size());
    for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1){
        const auto &b=**iterBead1;
        bead_info_t bi{
            { (float)b.GetXPos(), (float)b.GetYPos(), (float)b.GetZPos()},
            enoCache[b.GetType()]
        };
        pos_cache.push_back(bi);
    }
    
    float QMin = m_QMin;
    float dQ = m_dQ;

    using histogram_pair_t = std::pair<std::vector<float>,std::vector<float>>;

    histogram_pair_t init{
        std::vector<float>(m_QPoints, 0),
        std::vector<float>(m_QPoints, 0)
    };

    auto transform=[&](const bead_info_t &b1 ) -> histogram_pair_t
    {
        unsigned i1 = &b1 - &pos_cache[0];

        // Malloc is not technically illegal in C++ parallel algorithms I think.
        // Not sure how else to do it though.
        std::vector<float> vIQLocal(m_QPoints, 0.0f);
        std::vector<float> vIQSqLocal(m_QPoints, 0.0f);

        const float esquare=b1.eno*b1.eno;
        for(long iq = 0; iq < m_QPoints; ++iq)
        {
            vIQLocal[iq] += esquare;
            vIQSqLocal[iq] += (esquare*esquare);
        }

        for(unsigned i2=i1+1; i2<pos_cache.size(); i2++){
            const auto &b2=pos_cache[i2];
    
            // We multiply by 2 here as this only does (i1<i2), so we need to double it to cover (i2<i1)
            // (i1==i2) is covered by the earlier special-case loop
            const float eprod = 2 * b1.eno * b2.eno;
            const float dr = DistanceMetric(b1.x, b2.x);
              
            if(dr > 0.000001)
            {
                // We are evaluating sin(ß (QMin+iq*dQ)*dr )
                //                         sin( QMin*dr + iq*(dQ*dr) )
                sin_enum_angle_recurrence sin_enum(QMin*dr, dQ*dr);    
                float qvalue=QMin;
                float scale=eprod / dr;
                for(long iq = 0; iq < m_QPoints; iq++)
                {
                    const float dvalue = scale*sin_enum() / qvalue;
                    // Accumulation is still double-precision
                    vIQLocal[iq] += dvalue;
                    vIQSqLocal[iq] += dvalue*dvalue;
                    qvalue += dQ;
                }
            }
            else
            {
                // i = j here so we have the same bead, and because Sinx/x = 1 when x = 0, the contribution is just F(q)**2.                
                for(long iq = 0; iq < m_QPoints; ++iq)
                {
                    vIQLocal[iq] += eprod;
                    vIQSqLocal[iq] += (eprod*eprod);
                }
            }
        }

        return {vIQLocal, vIQSqLocal};
    };

    auto reduce = [&](const histogram_pair_t &a, const histogram_pair_t &b) -> histogram_pair_t
    {
        histogram_pair_t res=a;
        for(unsigned i=0; i<m_QPoints; i++){
            res.first[i] += b.first[i];
            res.second[i] += b.second[i];
        }
        return res;
    };

    histogram_pair_t res=std::transform_reduce(
        //std::execution::par,
        pos_cache.begin(), pos_cache.end(), 
        init,
        reduce,
        transform        
    );

    for(unsigned i=0; i<m_QPoints; i++){
        m_vIQ[i] += res.first[i];
        m_vIQSq[i] += res.second[i];
    }
}
#endif


void prSAXS::UpdateState_InnerLoop_v4_threads(CSimState& rSimState, const ISimBox* const pISimBox)
{        
    // Define the min and max q values from the box size and bead diameter.

    // Loop over all q values, adding the contributions from each bead pair weighted by their electon numbers.
    // To svoid double counting, we start the second loop with the iterator equal to the first.

    const auto &bead_types=rSimState.GetBeadTypes();
    std::vector<float> enoCache(bead_types.size());
    for(unsigned i=0; i<bead_types.size(); i++){
        const auto pbt=bead_types[i];
        assert(pbt->GetType()==i);
        auto it= m_mElectronNo.find(i);
        if(it!=m_mElectronNo.end()){
            enoCache[i]=it->second;
        }
    }

    struct bead_info_t
    {
        float x[3];
        float eno;
    };

    std::vector<bead_info_t> pos_cache;
    pos_cache.reserve(m_vBeads.size());
    for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1){
        const auto &b=**iterBead1;
        bead_info_t bi{
            { (float)b.GetXPos(), (float)b.GetYPos(), (float)b.GetZPos()},
            enoCache[b.GetType()]
        };
        pos_cache.push_back(bi);
    }
    
    float QMin = m_QMin;
    float dQ = m_dQ;

    using histogram_pair_t = std::pair<std::vector<float>,std::vector<float>>;

    histogram_pair_t init{
        std::vector<float>(m_QPoints, 0),
        std::vector<float>(m_QPoints, 0)
    };

    std::mutex mutex;
    std::atomic<unsigned> i1_next;
    i1_next = 0;

    auto worker_prov = [&]() -> void
    {
        std::vector<float> vIQLocal(m_QPoints, 0.0f);
        std::vector<float> vIQSqLocal(m_QPoints, 0.0f);

        while(1){
            unsigned i1=i1_next.fetch_add(1);
            if(i1 >= pos_cache.size()){
                return;
            }

            const auto &b1 = pos_cache[i1];

            const float esquare=b1.eno*b1.eno;
            for(long iq = 0; iq < m_QPoints; ++iq)
            {
                vIQLocal[iq] += esquare;
                vIQSqLocal[iq] += (esquare*esquare);
            }

            for(unsigned i2=i1+1; i2<pos_cache.size(); i2++){
                const auto &b2=pos_cache[i2];
        
                // We multiply by 2 here as this only does (i1<i2), so we need to double it to cover (i2<i1)
                // (i1==i2) is covered by the earlier special-case loop
                const float eprod = 2 * b1.eno * b2.eno;
                const float dr = DistanceMetric(b1.x, b2.x);
                
                if(dr > 0.000001)
                {
                    // We are evaluating sin(ß (QMin+iq*dQ)*dr )
                    //                         sin( QMin*dr + iq*(dQ*dr) )
                    sin_enum_angle_recurrence sin_enum(QMin*dr, dQ*dr);    
                    float qvalue=QMin;
                    float scale=eprod / dr;
                    for(long iq = 0; iq < m_QPoints; iq++)
                    {
                        const float dvalue = scale*sin_enum() / qvalue;
                        // Accumulation is still double-precision
                        vIQLocal[iq] += dvalue;
                        vIQSqLocal[iq] += dvalue*dvalue;
                        qvalue += dQ;
                    }
                }
                else
                {
                    // i = j here so we have the same bead, and because Sinx/x = 1 when x = 0, the contribution is just F(q)**2.                
                    for(long iq = 0; iq < m_QPoints; ++iq)
                    {
                        vIQLocal[iq] += eprod;
                        vIQSqLocal[iq] += (eprod*eprod);
                    }
                }
            }

            {
                std::unique_lock<std::mutex> lk(mutex);
                for(unsigned i=0; i<m_QPoints; i++){
                    m_vIQ[i] += vIQLocal[i];
                    m_vIQSq[i] += vIQSqLocal[i];
                    vIQLocal[i] = 0;
                    vIQSqLocal[i] = 0;
                }
            }    
        }
    };



    std::vector<std::thread> workers;
    unsigned numthreads=std::thread::hardware_concurrency();
    numthreads=std::max<unsigned>(1, numthreads-1);
    for(unsigned i=0; i<numthreads; i++){
        workers.push_back(std::thread(worker_prov));
    }

    for(unsigned i=0; i<numthreads; i++){
        workers[i].join();
    }
}

/*
void prSAXS::UpdateState_InnerLoop_v3(CSimState& rSimState, const ISimBox* const pISimBox)
{        
    #error "Not complete. WIP"

    // Define the min and max q values from the box size and bead diameter.

    // Loop over all q values, adding the contributions from each bead pair weighted by their electon numbers.
    // To svoid double counting, we start the second loop with the iterator equal to the first.

    const auto &bead_types=rSimState.GetBeadTypes();
    unsigned ntypes=bead_types.size();

    std::vector<float> enoCache(ntypes);
    for(unsigned i=0; i<ntypes; i++){
        const auto pbt=bead_types[i];
        assert(pbt->GetType()==i);
        enoCache[i]=m_mElectronNo.at(i);
    }

    float drMax = sqrt(std::pow(pISimBox->GetSimBoxXLength(),2) + std::pow(pISimBox->GetSimBoxYLength(),2) + std::pow(pISimBox->GetSimBoxZLength(),2)); 
    float drQuant = 0.01;
    float drQuantInv = 1.0/drQuant;
    unsigned drQuantMax = std::ceil( drMax * drQuantInv ) + 1;

    std::vector<std::vector<uint64_t>> histograms;
    for(unsigned i1=0; i1+1<ntypes; i1++){
        for(unsigned i2=i1+1; i2<ntypes; i2++){
            histograms[i1*ntypes+i2].resize( drQuantMax );
        }
    }

    struct bead_info_t
    {
        float x[3];
        uint32_t type;
    };

    std::vector<unsigned> type_counts(ntypes, 0);

    std::vector<bead_info_t> pos_cache;
    pos_cache.reserve(m_vBeads.size());
    for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1){
        const auto &b=**iterBead1;
        bead_info_t bi{
            { (float)b.GetXPos(), (float)b.GetYPos(), (float)b.GetZPos()},
            b.GetType()
        };
        pos_cache.push_back(bi);

        type_counts[bi.type] += 1;
    }
    
    float QMin = m_QMin;
    float dQ = m_dQ;

    for(unsigned i1=0; i1<pos_cache.size(); i1++){
        const auto &b1=pos_cache[i1];

        for(unsigned i2=i1+1; i2<pos_cache.size(); i2++){
            const auto &b2=pos_cache[i2];

            unsigned type_min=std::min(b1.type,b2.type);
            unsigned type_max=std::max(b1.type,b2.type);
            unsigned type_type_index=type_min * ntypes + type_max;

            const float dr = DistanceMetric(b1.x, b2.x);
            const unsigned drq = unsigned( dr * drQuantInv );

            histograms[type_type_index][drq] += 1;
        }
    }

    for(unsigned i1=0; i1+1<ntypes; i1++){
        for(unsigned i2=i1+1; i2<ntypes; i2++){
            

            histograms[i1*ntypes+i2].resize( drQuantMax );
        }
    }
}
*/

// Function to check that the user-specified data is valid and. As this process
// is internally generated by the shadow SimBox we do not perform validity checking.

bool prSAXS::ValidateData(const CInputData &riData)
{
	return false;
}

// Function to check that the data for a process that is internally generated
// is valid, and to create the xxProcessState to serialise the data to file.

bool prSAXS::InternalValidateData(const ISimState* const pISimState)
{
	// An xxProcessState is created to hold the data that the process wants
	// to write to file. Data is passed as a series of CTimeSeriesData objects
	// as used for CHistoryState. Because this function is called after the
	// original xxProcess instance is copied (so that the xxProcessObject instance
	// can be discarded), creating the xxAggregateState here does not result in
	// it being destroyed. Hence, we can write the XML opening tags to the file safely.

	// The process state's file name depends on whether it is XML-enabled or not,
	// so we use two different SetState() signatures to allow for this. If it
	// is XML-enabled, we write out the standard processing instructions and
	// wrapper tags <ProcessState></ProcessState> as the first and last elements 
	// in the file. Because the process state can write data until the simulation 
	// ends, we only write the end tag in the base class's destructor.

#if EnableXMLProcesses == SimXMLEnabled

	SetState(new xxProcessState(xxBase::GetPSPrefix() + GetProcessType() + ToString(GetId()) + "." + pISimState->GetRunId() + ".xml", GetStartTime(), GetEndTime(), pISimState->GetRunId(), GetProcessType()));

	// Note that the opening <Header> tag is written in the xxProcessState class
	// but the closing tag is written here. This is a hack.

	zOutStream& os = m_pState->putXMLStartTags();
//	os << "<MonomerTotal>"		<< m_MonomerTotal		<< "</MonomerTotal>"	<< zEndl;
	os << "</Header>" << zEndl;

#elif EnableXMLProcesses == SimXMLDisabled

	SetState(new xxProcessState(xxBase::GetPSPrefix() + GetProcessType() + ToString(GetId()) + "." + pISimState->GetRunId(), GetStartTime(), GetEndTime(), pISimState->GetRunId(), GetProcessType()));

	zOutStream& os = m_pState->putASCIIStartTags();
	os << "    AnalysisPeriods  " << m_AnalysisPeriods	 << zEndl;
	os << "    QPoints          " << m_QPoints	 << zEndl;

#endif

	return true;
}

// Function to set the number of electrons in a specified bead type. We allow fractional values.

bool prSAXS::SetBeadTypeElectronNo(long beadType, double eno)
{
    bool bValid = false;
    
    m_mElectronNo.insert(zPairLD(beadType, eno));

    // Check the entry succeeded, otherwise return an error flag.
    
    LongDoubleIterator iter = m_mElectronNo.find(beadType);
    
    if(iter != m_mElectronNo.end())
    {
        bValid = true;
    }
    
    return bValid;
}
