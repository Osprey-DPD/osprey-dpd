/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// prBilayerFusion.cpp: implementation of the prBilayerFusion class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "prBilayerFusion.h"
#include "IBilayerFusion.h"
#include "Bilayer.h"
#include "LogCreateEventFailed.h"
#include "SimState.h"
#include "ISimBox.h"
#include "TimeSeriesData.h"
#include "InputData.h"
#include "aaRegionToType.h"

// Events monitored by the process

#include "evLamellaPore.h"

// Messages issued when critical events on the path to fusion occur

#include "LogVesicleApproach.h"			// Use the vesicle messages if they are appropriate
#include "LogLamellaCusp.h"
#include "LogVesicleDistalMix.h"
#include "LogVesicleDistalTouch.h"
#include "LogVesicleFusionPore.h"
#include "LogLamellaPore.h"
#include "LogVesicleProximalMix.h"
#include "LogVesicleStalk.h"

// STL using declarations


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this process. 
// The static member function GetType() is invoked by the xxProcessObject 
// to compare the type read from the control data file with each
// xxProcess-derived class so that it can create the appropriate object 
// to hold the process data.

const zString prBilayerFusion::m_Type = "BilayerFusion";

const zString prBilayerFusion::GetType()
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
	xxProcess* Create() {return new prBilayerFusion();}

	const zString id = prBilayerFusion::GetType();

	const bool bRegistered = acfProcessFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

prBilayerFusion::prBilayerFusion() : xxProcess(true),
								m_AggregateName1(""),
								m_AggregateName2(""),
								m_AggregateId1(0),
								m_AggregateId2(0),
								m_MaxApproach(0.0),
								m_MinCuspCurvature(0.0),
								m_MaxDistalTouch(0.0),
								m_MinPoreSize(0),
								m_MinFusionPoreSize(0),
								m_pBilayer1(NULL),
								m_pBilayer2(NULL),
								m_bBilayerApproach(false),
								m_bBilayerCusp1(false),
								m_bBilayerCusp2(false),
								m_bBilayerDistalMix(false),
								m_bBilayerDistalTouch(false),
								m_bBilayerFusionPore(false),
								m_bBilayerPore1(false),
								m_bBilayerPore2(false),
								m_bBilayerProximalMix(false),
								m_CMSeparation(0.0)

{

}

prBilayerFusion::~prBilayerFusion()
{
	if(m_pBilayer1)
	{
		delete m_pBilayer1;
		m_pBilayer1 = NULL;
	}
	if(m_pBilayer2)
	{
		delete m_pBilayer2;
		m_pBilayer2 = NULL;
	}
}

// Member functions to write/read the data specific to the process.

zOutStream& prBilayerFusion::put(zOutStream& os) const
{
	// Write base class data first

	xxProcess::put(os);
	
	os << "    Aggregate			" << m_AggregateName1	 << "  " << m_AggregateId1 << zEndl;
	os << "    Aggregate			" << m_AggregateName2	 << "  " << m_AggregateId2 << zEndl;
	os << "    MaxApproach			" << m_MaxApproach		 << zEndl;
	os << "    MinCuspCurvature		" << m_MinCuspCurvature	 << zEndl;
	os << "    MaxDistalTouch		" << m_MaxDistalTouch	 << zEndl;
	os << "    MinPoreSize			" << m_MinPoreSize		 << zEndl;
	os << "    MinFusionPoreSize	" << m_MinFusionPoreSize << zEndl;

	return os;
}

zInStream& prBilayerFusion::get(zInStream& is)
{
	// Read base class data first

	xxProcess::get(is);

	zString token;
	zString aggregate1, aggregate2;
	long aggId1, aggId2;
	long minPoreSize, minFusionPoreSize;
	double maxApproach, minCusp, maxDistalTouch;


	is >> token;
	if(!is.good() || token != "Aggregate")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> aggregate1 >> aggId1;
		if(!is.good() || aggregate1.empty() || aggId1 < 1)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Aggregate")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> aggregate2 >> aggId2;
		if(!is.good() || aggregate2.empty() || aggId2 < 1 || aggId2 == aggId1)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "MaxApproach")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> maxApproach;
		if(!is.good() || maxApproach < 0.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "MinCuspCurvature")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> minCusp;
		if(!is.good() || minCusp < 0.0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "MaxDistalTouch")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> maxDistalTouch;
		if(!is.good() || maxDistalTouch > maxApproach)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "MinPoreSize")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> minPoreSize;
		if(!is.good() || minPoreSize < 0)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "MinFusionPoreSize")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> minFusionPoreSize;
		if(!is.good() || minFusionPoreSize < 0)
		{
			SetDataValid(false);
			return is;
		}
	}


	m_AggregateName1	= aggregate1;
	m_AggregateName2	= aggregate2;
	m_AggregateId1		= aggId1;
	m_AggregateId2		= aggId2;
	m_MaxApproach		= maxApproach;
	m_MinCuspCurvature	= minCusp;
	m_MaxDistalTouch	= maxDistalTouch;
	m_MinPoreSize		= minPoreSize;
	m_MinFusionPoreSize	= minFusionPoreSize;

	return is;
}


// Non-static function to return the type of the process

const zString prBilayerFusion::GetProcessType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current process.

xxProcess* prBilayerFusion::GetProcess() const
{
	return new prBilayerFusion(*this);
}

// Function to update the state of the process using data from the associated
// aggregate (CVesicle) and events (evVesiclePore,...).
//
// This process monitors two planar bilayers to see if they fuse. Messages are 
// issued at the following (hyperthetical) stages of fusion
//
//	1	Bilayer CM separation <  m_MaxApproach
//  2	A cusp with a minimum curvature of m_MinCuspCurvature has appeared in either bilayer
//	3	Outer leaflet separation <  m_MaxDistalTouch
//	4	Bilayer outer leaflet lipids mix
//	5	Pores form in either bilayer
//	6	A fusion stalk appears in the bilayers
//	7	A fusion pore joins the opposite sides of both bilayers
//
// Each stage is signalled by issuing a CLogInfoMessage, and the appearance of
// pores in the two bilayer membranes is monitored by the evLamellaPore events.
// External events are not defined for the other stages of fusion because they
// involve more than one aggregate.

void prBilayerFusion::UpdateState(CSimState& rSimState, const ISimBox* const pISimBox)
{
	double vcm[3];
	double vcmDistance;

	vcm[0]	= m_pBilayer1->GetCMXPos() - m_pBilayer2->GetCMXPos();
	vcm[1]	= m_pBilayer1->GetCMYPos() - m_pBilayer2->GetCMYPos();
	vcm[2]	= m_pBilayer1->GetCMZPos() - m_pBilayer2->GetCMZPos();

	vcmDistance = sqrt(vcm[0]*vcm[0] + vcm[1]*vcm[1] + vcm[2]*vcm[2]);

	// ****************************************
	// First fusion stage: are the bilayers closer than a set distance?
	// Issue the CLogVesicleApproach message if true; if the event has already 
	// occurred check to see if the bilayers have moved apart again, in which 
	// case reset the event flag.

//	std::cout << "cm sep = " << vcmDistance << zEndl;

	if(!m_bBilayerApproach && vcmDistance < m_MaxApproach)
	{
		m_bBilayerApproach = true;

		pISimBox->SaveCurrentState();

		 new CLogVesicleApproach(pISimBox->GetCurrentTime(), m_AggregateName1, m_AggregateId1, 
															m_AggregateName2, m_AggregateId2, vcmDistance);

		// ****************************************
		// Fusion stage 2: is there a cusp in either bilayer?

		if(!m_bBilayerCusp1)
		{
			m_bBilayerCusp1 = true;


		}
		else if(!m_bBilayerCusp2)
		{
			m_bBilayerCusp2 = false;
		}
		// Fusion stage 2
		// ****************************************
	}
	else if(m_bBilayerApproach && vcmDistance > m_MaxApproach)
	{
		m_bBilayerApproach = false;
	}
	// Fusion stage 1
	// ****************************************


}

// Store pointers to the CAnalysis-derived objects that the process
// will monitor for activity during the simulation. These are const
// because we don't allow data to be written back to the aggregate.
// Because all the aggregates are stored as base class, CAnalysis, pointers,
// we have to use reinterpret_cast to get back to the CBilayer objects.
// This means that if the wrong CAnalysis-derived class is passed in 
// the code should fail gracefully.

void prBilayerFusion::SetAggregate(const CAnalysis* pAggregate1, const CAnalysis* pAggregate2)
{
	m_pBilayer1 = new IBilayerFusion<CBilayer>(reinterpret_cast<const CBilayer*>(pAggregate1));
	m_pBilayer2 = new IBilayerFusion<CBilayer>(reinterpret_cast<const CBilayer*>(pAggregate2));
}

// No external events are defined for this process.

void prBilayerFusion::SetEvent()
{
}

// Function to check that the user-specified data is valid and, if so, 
// to find the aggregates and events that are used by the process.
//
// An xxProcessState is created to hold the data that the process wants
// to write to file. Data is passed as a series of CTimeSeriesData objects
// as used for CHistoryState.

bool prBilayerFusion::ValidateData(const CInputData &riData)
{
	// Do the specified aggregates exist and are they CVesicle? If so, store
	// pointers to them

	cAggregateIterator cIterAgg1 = find_if(riData.GetAggregates().begin(), 
										   riData.GetAggregates().end(), aaGetAggregateId(m_AggregateId1));

	cAggregateIterator cIterAgg2 = find_if(riData.GetAggregates().begin(), 
										   riData.GetAggregates().end(), aaGetAggregateId(m_AggregateId2));

	if(cIterAgg1 != riData.GetAggregates().end() && (*cIterAgg1)->GetType() == m_AggregateName1 &&
	   cIterAgg2 != riData.GetAggregates().end() && (*cIterAgg2)->GetType() == m_AggregateName2)
		SetAggregate(*cIterAgg1, *cIterAgg2);
	else
		return false;

	// All data has been validated so create and store an xxProcessState object
	// to allow the xxProcess to write its data to a process state file.

    SetState(new xxProcessState(xxBase::GetPSPrefix() + GetProcessType() + ToString(GetId()) + "." + riData.GetRunId(), GetStartTime(), GetEndTime()));

	return true;
}

