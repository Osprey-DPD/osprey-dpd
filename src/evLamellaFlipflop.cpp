/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// evLamellaFlipflop.cpp: implementation of the evLamellaFlipflop class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "evLamellaFlipflop.h"

#include "ISimEvent.h"
#include "InputData.h"
#include "aaRegionToType.h"
#include "CellProfileSet.h"
#include "CellProfile.h"
#include "Builder.h"		// Needed for the global pi
#include "Bilayer.h"


//////////////////////////////////////////////////////////////////////
// Global members
//////////////////////////////////////////////////////////////////////

// Static member variable containing the identifier for this command. 
// The static member function GetType() is invoked by the xxCommandObject 
// to compare the type read from the control data file with each
// xxCommand-derived class so that it can create the appropriate object 
// to hold the command data.

const zString evLamellaFlipflop::m_Type = "LamellaFlipflop";

const zString evLamellaFlipflop::GetType()
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
	xxEvent* Create() {return new evLamellaFlipflop();}

	const zString id = evLamellaFlipflop::GetType();

	const bool bRegistered = acfEventFactory::Instance()->Register(id, Create);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

evLamellaFlipflop::evLamellaFlipflop() : m_AggregateName(""),
										 m_AggregateId(0),
										 m_MinDistance(0.0),
										 m_pBilayer(NULL),
										 m_X(0), m_Y(0), m_Z(0),
										 m_SliceTotal(0),
										 m_RowTotal(0),
										 m_CellTotal(0)
{
	m_vBilayerProfiles.clear();

	m_Polymers.clear();
	m_PolymerTypes.clear();
	m_HeadBeadTypes.clear();

	m_mBilayer.clear();
	m_mUpper.clear();
	m_mLower.clear();

	m_UtoLCounter.clear();
	m_LtoUCounter.clear();
	m_DiffCounter.clear();

}

// The same aggregate object is passed to the copied event.
//

evLamellaFlipflop::evLamellaFlipflop(const evLamellaFlipflop& oldEvent) : xxEvent(oldEvent),
																		  m_AggregateName(oldEvent.m_AggregateName),
																		  m_AggregateId(oldEvent.m_AggregateId),
																		  m_MinDistance(oldEvent.m_MinDistance),
																		  m_pBilayer(oldEvent.m_pBilayer)
{
	m_X				= oldEvent.m_X;
	m_Y				= oldEvent.m_Y;
	m_Z				= oldEvent.m_Z;
	m_SliceTotal	= oldEvent.m_SliceTotal;
	m_RowTotal		= oldEvent.m_RowTotal;
	m_CellTotal		= oldEvent.m_CellTotal;
	m_Polymers		= oldEvent.m_Polymers;
	m_PolymerTypes	= oldEvent.m_PolymerTypes;
	m_HeadBeadTypes	= oldEvent.m_HeadBeadTypes;
	m_mBilayer		= oldEvent.m_mBilayer;
	m_mUpper		= oldEvent.m_mUpper;
	m_mLower		= oldEvent.m_mLower;
	m_UtoLCounter	= oldEvent.m_UtoLCounter;
	m_LtoUCounter	= oldEvent.m_LtoUCounter;
	m_DiffCounter	= oldEvent.m_DiffCounter;

}

evLamellaFlipflop::~evLamellaFlipflop()
{

}



// Member functions to write/read the data specific to the command.
// The name of bead type to be removed from display is the only data needed.

zOutStream& evLamellaFlipflop::put(zOutStream& os) const
{
	// Write base class data first

	xxEvent::put(os);
	
	os << "    Aggregate	" << m_AggregateName << "  " << m_AggregateId << zEndl;
	os << "    Polymers		";

	for(cStringSequenceIterator iterName=m_Polymers.begin(); iterName!=m_Polymers.end(); iterName++)
	{
		os << " " << (*iterName);
	}

	os << zEndl;
	os << "    MinDistance  " << m_MinDistance	<< zEndl;

	return os;
}

zInStream& evLamellaFlipflop::get(zInStream& is)
{
	// Read base class data first

	xxEvent::get(is);
	
	zString token;
	zString aggregate;
	long id;
	zString polymer;
	zString sName;
	double mindist;


	is >> token;
	if(!is.good() || token != "Aggregate")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> aggregate >> id;
		if(!is.good() || aggregate.empty() || id < 1)
		{
			SetDataValid(false);
			return is;
		}
	}

	is >> token;
	if(!is.good() || token != "Polymers")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> sName;

		if(!is.good() || sName == "MinDistance" || sName.empty())
		{
			SetDataValid(false);
			return is;
		}
		else
		{
			while(sName != "MinDistance")
			{
				if(is.good() && !sName.empty())
					m_Polymers.push_back(sName);
				else
				{
					SetDataValid(false);
					return is;
				}

				is >> sName;
			}
		}
	}

	if(!is.good() || sName != "MinDistance")
	{
		SetDataValid(false);
		return is;
	}
	else
	{
		is >> mindist;
		if(!is.good() || mindist < 0.0)
		{
			SetDataValid(false);
			return is;
		}
	}


	// Data has been read successfully so store it in the member variables

	SetDataValid(true);

	m_AggregateName	= aggregate;
	m_AggregateId	= id;
	m_MinDistance	= mindist;


	return is;
}

// Non-static function to return the type of the event

const zString evLamellaFlipflop::GetEventType() const
{
	return m_Type;
}

// Function to return a pointer to a copy of the current event.

xxEvent* evLamellaFlipflop::GetEvent() const
{
	return new evLamellaFlipflop(*this);
}

// Set a pointer to the CAnalysis-derived object that the event
// will monitor for activity during the simulation. This cannot be const
// because the event may write data back to the aggregate.

bool evLamellaFlipflop::SetAggregate(CAnalysis* const pAggregate)
{
	m_pBilayer = dynamic_cast<CBilayer*>(pAggregate);

	return true;
}

// **********************************************************************
// Function to validate the data supplied to the event against the 
// aggregate that is the source of its information about the simulation state.
// We check both the aggregate name and its id in case more than one
// aggregate of the specified type has been created. This way we can assign 
// the same aggregate to many events, by repeating the name/id, or assign 
// different aggregates of the same type, e.g., bilayer, using their ids
// to distinguish them.
//
// If more than one aggregate is needed for the event we still designate one
// as the major aggregate and store the others locally to the xxEvent-derived
// class. 
//
// This function cannot be const because we store data for later use.
//
// Event-specific validation follows.
//

bool evLamellaFlipflop::ValidateData(const CInputData &riData)
{
	// Does the specified aggregate exist? If so store a pointer to it

	cAggregateIterator cIterAgg = find_if(riData.GetAggregates().begin(), 
										  riData.GetAggregates().end(), aaGetAggregateId(m_AggregateId));

	if(cIterAgg != riData.GetAggregates().end() && (*cIterAgg)->GetType() == m_AggregateName)
		SetAggregate(*cIterAgg);
	else
		return false;

	// Check that the polymer names specified actually
	// occur in the map and store their types for later use.

	for(cStringSequenceIterator polymer=m_Polymers.begin(); polymer!=m_Polymers.end(); polymer++)
	{
		if(riData.GetPolymerNamesMap().find(*polymer) != riData.GetPolymerNamesMap().end())
		{
			long polymerType = (*riData.GetPolymerNamesMap().find(*polymer)).second;
			const CPolymer* const pPolymer = riData.GetPolymerTypes().at(polymerType);

			m_PolymerTypes.push_back(polymerType);
			m_HeadBeadTypes.push_back(pPolymer->GetHead()->GetType());

		}
		else
			return false;
	}



	return true;
}

// **********************************************************************
// Implementation of the Execute command sent by the SimBox to each xxEvent.
//
// Events have an active period specified by the user during which they monitor
// the SimBox for their particular conditions and take action if they are triggered.
// We return a boolean so that the SimBox can see if the event is active or not
// as this may be useful for considering several events. Note that the active 
// period begins at the start of the m_Start time-step and stops at the beginning 
// of the time-step labelled by its m_End property.
// Events can issue new commands and messages. Commands are executed after all events
// have been executed. This allows all events to monitor the state of the simulation
// before any new commands change it.
//
// On each call we get the cell profile set from the aggregate and store it locally,
// together with other data describing the bilayer state. No flipflops are
// calculated on the first call.
// On subsequent calls we calculate if each lipid has flipped and, if so,
// move it from one monolayer counter to the other.


bool evLamellaFlipflop::Execute(long simTime, ISimEvent* const pISimEvent)
{
	if(simTime >= GetStartTime() && simTime < GetEndTime())
	{				
		
		std::cout << simTime << zEndl;

		// Store the aggregate's latest cell profile data locally for speed

		m_vBilayerProfiles.clear();

		copy(m_pBilayer->GetCellProfileSet().begin(), m_pBilayer->GetCellProfileSet().end(), back_inserter(m_vBilayerProfiles));

		// **********************************************************************

		if(InitialiseEvent())
		{
			UnInitialiseEvent();		// Call base class member to set flag false

			m_X					= m_pBilayer->GetXNormal();
			m_Y					= m_pBilayer->GetYNormal();
			m_Z					= m_pBilayer->GetZNormal();
			m_SliceTotal		= m_pBilayer->GetSliceTotal();
			m_RowTotal			= m_pBilayer->GetRowTotal();
			m_CellTotal			= m_pBilayer->GetCellTotal();

			// Copy the lipid head bead pointers into local arrays for each monolayer.
			// First we store all beads in the bilayer, then we sort them into
			// the monolayers based on their coordinates compared to the
			// CM coordinates of all beads in the bilayer.
			// We assume the m_lBilayer, m_lUpper and m_lLower maps were zeroed 
			// in the constructor.
			// We use the orthogonal cell profiles that are created in the
			// CBilayer class

			for(long i=m_RowTotal-1; i>=0; i--)
			{
				CCellProfileSet* pBilayer1d = m_vBilayerProfiles.at(i);

				for(long j=0; j<pBilayer1d->GetOrthogonalSize(); j++)
				{
					CCellProfile* pOCP = pBilayer1d->GetOrthogonalProfile(j);

					BeadList lBeads;
					lBeads.clear();

					long beadTotal = 0;
					double beadCM  = 0.0;

					for(long k=0; k<pOCP->Size(); k++)
					{
						CCell* const pCell = pOCP->GetRegion(k);

						BeadList beads = pCell->GetBeads();

						for(cBeadListIterator iterBead=beads.begin(); iterBead!=beads.end(); iterBead++)
						{
							if(find(m_HeadBeadTypes.begin(), m_HeadBeadTypes.end(), (*iterBead)->GetType())!=m_HeadBeadTypes.end())
							{
								beadTotal++;

								lBeads.push_back(*iterBead);

								if(m_X == 1)
								{
									beadCM += (*iterBead)->GetXPos();	
								}
								else if(m_Y == 1)
								{
									beadCM += (*iterBead)->GetYPos();	
								}
								else if(m_Z == 1)
								{
									beadCM += (*iterBead)->GetZPos();	
								}
							}
						}
					}

					if(beadTotal > 0)
						beadCM /= static_cast<double>(beadTotal);

					// Now we have the beads in the bilayer, sort them into monolayers.
					// Compare each beads position with the CM in its orthogonal
					// cell profile and assign it to the upper or lower monolayer
					// depending on whether it is larger or smaller. Some polymers 
					// may be embedded in the hydrophobic region with their heads close
					// to the bilayer midplane: they are still assigned to a monolayer
					// here based on their head bead coordinate, but we impose a minimum
					// distance away from the midplane that they have to move before
					// we say they have flipped from one monolayer to the other. The
					// minimum distance is m_MinDistance and is user-defined.
						
					if(m_X == 1)
					{
						for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
						{
							if((*iterBead)->GetXPos() > beadCM)
							{
								m_mUpper.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
							else
							{
								m_mLower.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
						}
					}
					else if(m_Y == 1)
					{
						for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
						{
							if((*iterBead)->GetYPos() > beadCM)
							{
								m_mUpper.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
							else
							{
								m_mLower.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
						}
					}
					else if(m_Z == 1)
					{
						for(BeadListIterator iterBead=lBeads.begin(); iterBead!=lBeads.end(); iterBead++)
						{
							if((*iterBead)->GetZPos() > beadCM)
							{
								m_mUpper.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
							else
							{
								m_mLower.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
						}
					}
				}
			}
		}
		else
		{
		// **********************************************************************
			// On succeeding calls, compare each polymer's head bead's current 
			// position with the CM of all bilayer head beads in the orthogonal 
			// cell profile and see if any have flipped.
			//
			// Given the beads in the monolayers we check for a flipflop by seeing
			// if the new position of a bead puts it in the same monolayer as before.
			// If not we count the flipflop and move the bead to the other monolayer.
			// This avoids having to sort all the beads all the time.

			// Set the size of the counters to hold all bead types so that
			// we can access them simply using a bead's type as the index.

			m_UtoLCounter.assign(pISimEvent->GetSimBoxBeadTypeTotal(), 0);
			m_LtoUCounter.assign(pISimEvent->GetSimBoxBeadTypeTotal(), 0);
			m_DiffCounter.assign(pISimEvent->GetSimBoxBeadTypeTotal(), 0);

			for(long i=m_RowTotal-1; i>=0; i--)
			{
				CCellProfileSet* pBilayer1d = m_vBilayerProfiles.at(i);

				for(long j=0; j<pBilayer1d->GetOrthogonalSize(); j++)
				{
					CCellProfile* pOCP = pBilayer1d->GetOrthogonalProfile(j);

					BeadList lOCPBeads;		// All lipid head beads in OCP

					long beadTotal		  = 0;
					double beadCM		  = 0.0;
					double beadCMPlusMin  = 0.0;
					double beadCMMinusMin = 0.0;

					// Loop over all cells in the (vertical) orthogonal cell profile,
					// calculate the CM coordinate of all head beads, and then 
					// check to see if any polymers previously in the upper monolayer
					// are now in the lower and vice versa.

					for(long k=0; k<pOCP->Size(); k++)
					{
						const CCell* const pCell = pOCP->GetRegion(k);

						BeadList beads = pCell->GetBeads();

						for(cBeadListIterator iterBead=beads.begin(); iterBead!=beads.end(); iterBead++)
						{
							if(find(m_HeadBeadTypes.begin(), m_HeadBeadTypes.end(), (*iterBead)->GetType())!=m_HeadBeadTypes.end())
							{						
								beadTotal++;

								lOCPBeads.push_back(*iterBead);

								if(m_X == 1)
								{
									beadCM += (*iterBead)->GetXPos();	
								}
								else if(m_Y == 1)
								{
									beadCM += (*iterBead)->GetYPos();	
								}
								else if(m_Z == 1)
								{
									beadCM += (*iterBead)->GetZPos();	
								}
							}
						}
					}

					if(beadTotal > 0)
					{
						beadCM /= static_cast<double>(beadTotal);
						beadCMPlusMin  = beadCM  + m_MinDistance;
						beadCMMinusMin = beadCM  - m_MinDistance;
					}

					if(m_X == 1)
					{
						for(BeadListIterator iterBead=lOCPBeads.begin(); iterBead!=lOCPBeads.end(); iterBead++)
						{
							// If bead was in upper monolayer and now has a position 
							// more than m_MinDistance below the OCP head bead CM, 
							// move it to the lower monolayer.

							if(m_mUpper.find((*iterBead)->GetId()) != m_mUpper.end() && (*iterBead)->GetXPos() < beadCMMinusMin)
							{
								m_UtoLCounter.at((*iterBead)->GetType())++;
								m_mUpper.erase((*iterBead)->GetId());
								m_mLower.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
							else if(m_mLower.find((*iterBead)->GetId()) != m_mLower.end() && (*iterBead)->GetXPos() > beadCMPlusMin)
							{
								m_LtoUCounter.at((*iterBead)->GetType())++;
								m_mLower.erase((*iterBead)->GetId());
								m_mUpper.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
						}

					}
					else if(m_Y == 1)
					{
						for(BeadListIterator iterBead=lOCPBeads.begin(); iterBead!=lOCPBeads.end(); iterBead++)
						{
							// If bead was in upper monolayer and now has a position 
							// more than m_MinDistance below the OCP head bead CM, 
							// move it to the lower monolayer.

							if(m_mUpper.find((*iterBead)->GetId()) != m_mUpper.end() && (*iterBead)->GetYPos() < beadCMMinusMin)
							{
								m_UtoLCounter.at((*iterBead)->GetType())++;
								m_mUpper.erase((*iterBead)->GetId());
								m_mLower.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
							else if(m_mLower.find((*iterBead)->GetId()) != m_mLower.end() && (*iterBead)->GetYPos() > beadCMPlusMin)
							{
								m_LtoUCounter.at((*iterBead)->GetType())++;
								m_mLower.erase((*iterBead)->GetId());
								m_mUpper.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
						}
					}
					else if(m_Z == 1)
					{
						for(BeadListIterator iterBead=lOCPBeads.begin(); iterBead!=lOCPBeads.end(); iterBead++)
						{
							// If bead was in upper monolayer and now has a position 
							// more than m_MinDistance below the OCP head bead CM, 
							// move it to the lower monolayer.

							if(m_mUpper.find((*iterBead)->GetId()) != m_mUpper.end() && (*iterBead)->GetZPos() < beadCMMinusMin)
							{
								m_UtoLCounter.at((*iterBead)->GetType())++;
								m_mUpper.erase((*iterBead)->GetId());
								m_mLower.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
							else if(m_mLower.find((*iterBead)->GetId()) != m_mLower.end() && (*iterBead)->GetZPos() > beadCMPlusMin)
							{
								m_LtoUCounter.at((*iterBead)->GetType())++;
								m_mLower.erase((*iterBead)->GetId());
								m_mUpper.insert(zPairLongBead((*iterBead)->GetId(), *iterBead));
							}
						}
					}
				}
			}

			// Count the net number of flips: a positive value means more
			// polymers left the upper monolayer than entered it

			for(long unsigned int type=0; type<m_UtoLCounter.size(); type++)
			{
				m_DiffCounter.at(type) = m_UtoLCounter.at(type) - m_LtoUCounter.at(type);

				long uptodown = m_UtoLCounter.at(type);
				long downtoup = m_LtoUCounter.at(type);
				long diff	  = m_DiffCounter.at(type);

				std::cout << uptodown << "  " << downtoup << "  " << diff << zEndl;

			}
		}

		return true;
	}
	else
		return false;
}


