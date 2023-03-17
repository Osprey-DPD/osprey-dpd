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


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.
//
    
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

prSAXS::prSAXS() : m_AnalysisPeriods(0), m_QPoints(0), m_SamplePeriod(0), m_SampleTotal(0), m_SamplesTaken(0)
{
    m_mPolyTypes.clear();
	m_vBeads.clear();
    m_vIQ.clear();
    m_mElectronNo.clear();
}

// Constructor for use when the process is created by command. Default values for
// all required parameters must be passed to this constructor. We do NOT check that
// the analysis performed by this process can be completed during the run, nor that
// the number of bead types is constant during the analysis period. If the context
// within which this process operates becomes invalid, the process terminates and 
// logs an error message to the log file.

prSAXS::prSAXS(const CSimState* const pSimState,
									long analysisPeriods,
									long qPoints, LongLongMap mPolyTypes) :
                                    m_AnalysisPeriods(analysisPeriods),
									m_QPoints(qPoints),
									m_SamplePeriod(0), m_SampleTotal(0), m_SamplesTaken(0),
                                    m_mPolyTypes(mPolyTypes)
{
	m_vBeads.clear();
    m_vIQ.resize(m_QPoints, 0.0);  // This can be overwritten in the TrapezoidalRule function
    
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

	// Store the beads of the selected polymer types that occur within the selected polymer type
	
	PolymerVector vAllPolymers = pSimState->GetPolymers();
	
	for(PolymerVectorIterator iterPoly=vAllPolymers.begin(); iterPoly != vAllPolymers.end(); ++iterPoly)
	{
        if(m_mPolyTypes.find((*iterPoly)->GetType()) != m_mPolyTypes.end())
		{
			BeadVector vAllBeads = (*iterPoly)->GetBeads();
			
			for(cBeadVectorIterator iterBead = vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
			{
                m_vBeads.push_back(*iterBead);
            }
		}
	}
	
	// Debug output of the total beads
	
	std::cout << "prSAXS using " << m_mPolyTypes.size() << " polymer types, and found " << m_vBeads.size() << " total beads" << zEndl;

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

		std::cout << "Sample " <<  m_SamplesTaken << " of I(q) at time " << pISimBox->GetCurrentTime() << zEndl;
						
		double dx[3];
        
        // Define the min and max q values from the box size and bead diameter.
    
        const double qmin = xxBase::m_globalTwoPI/IGlobalSimBox::Instance()->GetSimBoxZLength();  // We assume the box is cubic and use LZ
        const double qmax = xxBase::m_globalTwoPI;  // d0 = 1
        const double dq = (qmax - qmin)/static_cast<double>(m_QPoints);
            
        // Loop over all q values, adding the contributions from each bead pair weighted by their electon numbers.
        
        double qvalue = qmin;
    
        for(long iq = 0; iq < m_QPoints; ++iq)
        {
			for(cBeadVectorIterator iterBead1 = m_vBeads.begin(); iterBead1!=m_vBeads.end(); ++iterBead1)
			{
				for(cBeadVectorIterator iterBead2 = m_vBeads.begin(); iterBead2!=m_vBeads.end(); ++iterBead2)
				{
                    const double eno1 = m_mElectronNo.find((*iterBead1)->GetType())->second;
                    const double eno2 = m_mElectronNo.find((*iterBead2)->GetType())->second;

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
                        
 
                        m_vIQ.at(iq) += eno1*eno2*sin(qvalue*dr)/(qvalue*dr);
                    //                      std::cout << "adding " << eno1 << " " << eno2 << " " << qvalue << " " << dr << zEndl;
                    }
                    else
                    {
                        // i = j here so we have the same bead, as Sinx/x = 1, the contribution is just F(q)**2.
                        
                        m_vIQ.at(iq) += eno1*eno2;
                    }
				}
			}
            
            qvalue += dq;
		}
				
		if(m_SamplesTaken == m_SampleTotal)
		{
			CTimeSeriesData* const pTSD = new CTimeSeriesData(m_QPoints+1);
			
			pTSD->SetValue(0,  pISimBox->GetCurrentTime(), "Time");
					
			m_pState->AddTimeSeriesData(pTSD);
			            
            // Now write the I(q) to file, after normalising the result by the number of samples taken.

            for(long iq=0; iq < m_QPoints; ++iq)
            {
                m_vIQ.at(iq) = m_vIQ.at(iq)/static_cast<double>(m_SamplesTaken);
                
                pTSD->SetValue(iq+1, m_vIQ.at(iq), "I(q)");
            }

            std::cout << "Dumping I(q) to file at time " << pISimBox->GetCurrentTime() << " with q points " << " " << m_QPoints << " points and samples " << m_SamplesTaken << zEndl;
		}
     }

}

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
