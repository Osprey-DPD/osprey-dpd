/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prPolymerBeadRDF.cpp: implementation of the prPolymerBeadRDF class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimXMLFlags.h"
#include "prPolymerBeadRDF.h"
#include "IGlobalSimBox.h"
#include "SimState.h"
#include "ISimBox.h"
#include "Bead.h"
#include "Polymer.h"
#include "TimeSeriesData.h"
#include "InputData.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.
//
    
const zString prPolymerBeadRDF::m_Type = "PolymerBeadRDF";

const zString prPolymerBeadRDF::GetType()
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
	xxProcess* Create() {return new prPolymerBeadRDF();}

	const zString id = prPolymerBeadRDF::GetType();

	const bool bRegistered = acfProcessFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Default constructor

prPolymerBeadRDF::prPolymerBeadRDF() : m_AnalysisPeriods(0), m_DataPoints(0),
									   m_PolymerType(-1), m_BeadType(-1), m_RMax(0.0),
									   m_SamplePeriod(0), m_SampleTotal(0), m_SamplesTaken(0), m_dr(0.0)
{
	m_vBeads.clear();
	m_vRDF.clear();
}

// Constructor for use when the process is created by command. Default values for
// all required parameters must be passed to this constructor. We do NOT check that
// the analysis performed by this process can be completed during the run, nor that
// the number of bead types is constant during the analysis period. If the context
// within which this process operates becomes invalid, the process terminates and 
// logs an error message to the log file.

prPolymerBeadRDF::prPolymerBeadRDF(const CSimState* const pSimState,
									long analysisPeriods,
									long dataPoints, double rMax,
									long polymerType, long beadType) : m_AnalysisPeriods(analysisPeriods),
									m_DataPoints(dataPoints),
									m_PolymerType(polymerType), m_BeadType(beadType), m_RMax(rMax),
									m_SamplePeriod(0), m_SampleTotal(0), m_SamplesTaken(0), m_dr(m_RMax/static_cast<double>(m_DataPoints))
{
	m_vBeads.clear();
	m_vRDF.resize(m_DataPoints, 0.0);
	
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

	// Store the beads of the selected type that occur within the selected polymer type
	
	PolymerVector vAllPolymers = pSimState->GetPolymers();
	
	for(PolymerVectorIterator iterPoly=vAllPolymers.begin(); iterPoly != vAllPolymers.end(); ++iterPoly)
	{
		if((*iterPoly)->GetType() == m_PolymerType)
		{
			BeadVector vAllBeads = (*iterPoly)->GetBeads();
			
			for(cBeadVectorIterator iterBead = vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
			{
				if((*iterBead)->GetType() == m_BeadType)
				{
					m_vBeads.push_back(*iterBead);
				}
			 }
		}
	}
	
	// Debug output of the total beads
	
	std::cout << "prPolymerBeadRDF, found " << m_vBeads.size() << " of type " << m_BeadType << " in polymers of type " << m_PolymerType << zEndl;

}

prPolymerBeadRDF::~prPolymerBeadRDF()
{

}

// Member functions to write/read the data specific to the process.
// The put() function is empty as the base class putASCIIStartTags(), 
// putASCIIEndTags() and their XML equivalents replace its function.

zOutStream& prPolymerBeadRDF::put(zOutStream& os) const
{
	return os;
}

zInStream& prPolymerBeadRDF::get(zInStream& is)
{
	// Read base class data first

	xxProcess::get(is);

	// No data entry required for this process as it is internal

	return is;
}


// Non-static function to return the type of the process

const zString prPolymerBeadRDF::GetProcessType() const
{
	// We return the associated ACN's type so that the ACN and process
	// are guaranteed to return the same string

	return prPolymerBeadRDF::GetType();
}

// Function to return a pointer to a copy of the current process.

xxProcess* prPolymerBeadRDF::GetProcess() const
{
	return new prPolymerBeadRDF(*this);
}

// Function to update the state of the process from the bead data stored in the
// CSimState. It is called every m_SamplePeriod time steps to collect data,
// but defines its own averaging period.
//
// This process computes a histogram of the number of beads that lie within concentric spherical shells of each other
// out to the specified maximum radius. The normalisation is such that the integral of the RDF over the volume of
// all space is unity. This requires some careful calculations as the beads are moving, so there may not be the same
// number present in histograms evaluated at different times.
//
// The number of shells is constant over time. At each sampling time, we normalise the elements in the histogram
// by the total number of bead pairs found at that time. Then after the final sample, we normalise further by
// the (constant) shell volume and number of samples. This means that integrating the resulting histogram over all space
// gives unity.

void prPolymerBeadRDF::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	if(pISimBox->GetCurrentTime() > GetStartTime() && pISimBox->GetCurrentTime() <= GetEndTime())
	{
		m_SamplesTaken++;

		std::cout << "Sample " <<  m_SamplesTaken << " of RDF at time " << pISimBox->GetCurrentTime() << zEndl;
		
		// Calculate the instantaneous RDF for beads of selected type and add to the running histogram.
		// We iterate over each bead and add the number of beads within each spherical shell to a running
		// total.
		
		long beadPairNorm = 0;
		
		double dx[3];
		
		for(long ir=0; ir < m_DataPoints; ++ir)
		{
			const double rin = ir*m_dr;
			const double rout = rin + m_dr;
			
			for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1)
			{
				for(cBeadVectorIterator iterBead2 = m_vBeads.begin(); iterBead2!=m_vBeads.end(); ++iterBead2)
				{
					if(iterBead1 != iterBead2)
					{
						dx[0] = (*iterBead1)->GetXPos() - (*iterBead2)->GetXPos();
						dx[1] = (*iterBead1)->GetYPos() - (*iterBead2)->GetYPos();
						
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
							dx[2] = (*iterBead1)->GetZPos() - (*iterBead2)->GetZPos();

							if( dx[2] > IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
								dx[2] = dx[2] - IGlobalSimBox::Instance()->GetSimBoxZLength();
							else if( dx[2] < -IGlobalSimBox::Instance()->GetHalfSimBoxZLength() )
								dx[2] = dx[2] + IGlobalSimBox::Instance()->GetSimBoxZLength();
						#else
							dx[2] = 0.0;
						#endif
						
						const double dr = sqrt(dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2]);
						
						if(dr > rin && dr <= rout)
						{
							++beadPairNorm;
							m_vRDF.at(ir) += 1;
						}
					}
				}
			}
		}
		
		
		// Normalise the summed histograms by the number of bead pairs found over all samples and shell volumes,
		// and write the result to file at the end of the sampling period
		
		if(m_SamplesTaken == m_SampleTotal)
		{
			CTimeSeriesData* const pTSD = new CTimeSeriesData(m_DataPoints+1);
			
			pTSD->SetValue(0,  pISimBox->GetCurrentTime(), "Time");
			
			// If no bead pairs are found set the norm to unity as the bins are all empty
			
			if(beadPairNorm == 0)
				beadPairNorm = 1;

			double rdfSum = 0;
					
			for(long ir=0; ir < m_DataPoints; ++ir)
			{
				const double rin = ir*m_dr;
				const double rout = rin + m_dr;
				const double shellVolume = 4.0*xxBase::m_globalPI*(3.0*rin*rout + m_dr*m_dr)*m_dr/3.0;
				
				rdfSum += m_vRDF.at(ir);
				
                // The beadPairNorm in next line should be removed for RDF curves to increase with increasing concentration
				m_vRDF.at(ir) /= (beadPairNorm*shellVolume*m_SamplesTaken);
				
				std::cout << ir << " " << shellVolume << " " << m_vRDF.at(ir) << zEndl;
				
				pTSD->SetValue(ir+1, m_vRDF.at(ir), "Bead RDF");
			}

			m_pState->AddTimeSeriesData(pTSD);
			
			std::cout << "Dumping histogram to file at time " << pISimBox->GetCurrentTime() << " with beadPairNorm " << beadPairNorm << " "<< m_DataPoints << " points and samples " << m_SamplesTaken << " and rdfSum = " << rdfSum << zEndl;

			
		}
     }

}

// Function to check that the user-specified data is valid and. As this process
// is internally generated by the shadow SimBox we do not perform validity checking.

bool prPolymerBeadRDF::ValidateData(const CInputData &riData)
{
	return false;
}

// Function to check that the data for a process that is internally generated
// is valid, and to create the xxProcessState to serialise the data to file.

bool prPolymerBeadRDF::InternalValidateData(const ISimState* const pISimState)
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
	os << "    RMax		 " << m_RMax	 << zEndl;
	os << "    Points       " << m_DataPoints	 << zEndl;
	os << "    Polymer type " << m_PolymerType	 << zEndl;
	os << "    Bead type    " << m_BeadType	 << zEndl;

#endif

	return true;
}

