/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InitialState.cpp: implementation of the CInitialState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimXMLFlags.h"
#include "IInclusiveRestartState.h"
#include "InitialStateData.h"
#include "InitialState.h"
#include "InputData.h"
#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"
#include "Polymer.h"
#include "acfTargetFactory.h"
#include "CommandTargetNode.h"

#include "xxParallelBase.h"
#include "pmInteger.h"

// STL using declarations

	using std::setw;
	using std::setprecision;



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor to construct an initial state from the CInputData object that 
// is passed in as an argument. A valid initial state specifies the numbers of
// beads, bonds and polymers, their coordinates and connecitivity, the wall and
// gravity options, some physical constants (temperature) and the RNG seed. It is
// intended that a CInitialState object contains just the data necessary such that 
// starting a specific simulation type with it will produce the same subsequent 
// evolution of the simulation. Hence, we have included the RNG seed in the specification
// but not the details of an integration scheme. That is assumed to be part of the 
// simulation type not the system being simulation. Similarly, we have included the
// specification of polymers in terms of beads but not the detailed structure of a
// bead. Interactions between beads are assumed to be part of the simulation type.
// 
// The CInitialState object forms one of the xxState-derived objects that are required 
// to create a valid CSimState which is used in turn to initialise the CSimBox in a 
// valid state prior to running a simulation. Because this object is responsible for
// creating all the beads, bonds and polymers used in the simulation it is also
// responsible for destroying them at the end of the simulation when it receives a 
// message to destroy itself.
//
// The initial state is independent of the type of simulation being done (DPD or MD),
// but does depend on the structure of the polymers and the boundary conditions
// on the simulation box. Polymers are assumed to be made of beads connected by
// bonds, but the structure of the beads and bonds is hidden from this class. This
// allows alternative implementations of the simulation technique to be compatible
// with the CInitialState-CInputData pair.

// The dimension of the simulation box is checked in CSimBox
// and if it is 2 then any Z dimension specified here will be ignored.
// This is to avoid having to check the dimension in many routines. Instead
// only the CSimBox object has to keep track of the dimension.
// Note that the bead, bond and polymer totals are initialised to -1 so
// that after adding the first item they can be used with zero-indexed arrays.

// The initial state file's name depends on whether it is XML-enabled or not,
// so we use two different constructor signatures to allow for this. If it
// is XML-enabled, we write out wrapper tags <InitialState></InitialState> as the first
// and last elements in the file.

#if EnableXMLProtocol == SimXMLEnabled

    CInitialState::CInitialState(CSimState* pSimState, const CInputData& rData) : xxState(xxBase::GetISPrefix() + rData.GetRunId() + ".xml", true, 0, rData.GetRunId()),
                                                        m_pIRS(new IInclusiveRestartState(pSimState)),
                                                        m_bDPDLG(rData.IsDPDLG()),
														RNGSeed(rData.GetRNGSeed()),
														ProcessorsXNo(rData.GetProcessorsXNo()),
														ProcessorsYNo(rData.GetProcessorsYNo()),
														CNTXCellNo(rData.GetCNTXCellNo()),
														CNTYCellNo(rData.GetCNTYCellNo()),
														CNTXCellWidth(rData.GetCNTXCellWidth()),
														CNTYCellWidth(rData.GetCNTYCellWidth()),
														AveBeadDensity(rData.GetAveBeadDensity()),
														kT(rData.GetkT()),
														vBeadTypes(rData.GetBeadTypes()),
														vBondTypes(rData.GetBondTypes()),
														vBondPairTypes(rData.GetBondPairTypes()),
														vPolymerTypes(rData.GetPolymerTypes()),
														m_pISD(rData.GetISD()),
													    m_TempBeadTotal(0),
														m_TempBondTotal(0),  
														m_TempBondPairTotal(0),   
														m_TempPolymerTotal(0) 
{

#elif EnableXMLProtocol == SimXMLDisabled

        CInitialState::CInitialState(CSimState* pSimState, const CInputData& rData) : xxState(xxBase::GetISPrefix() + rData.GetRunId(), true, 0, rData.GetRunId()),
                                                        m_pIRS(new IInclusiveRestartState(pSimState)),
                                                        m_bDPDLG(rData.IsDPDLG()),
                                                        RNGSeed(rData.GetRNGSeed()),
														ProcessorsXNo(rData.GetProcessorsXNo()),
														ProcessorsYNo(rData.GetProcessorsYNo()),
														CNTXCellNo(rData.GetCNTXCellNo()),
														CNTYCellNo(rData.GetCNTYCellNo()),
														CNTXCellWidth(rData.GetCNTXCellWidth()),
														CNTYCellWidth(rData.GetCNTYCellWidth()),
														AveBeadDensity(rData.GetAveBeadDensity()),
														kT(rData.GetkT()),
														vBeadTypes(rData.GetBeadTypes()),
														vBondTypes(rData.GetBondTypes()),
														vBondPairTypes(rData.GetBondPairTypes()),
														vPolymerTypes(rData.GetPolymerTypes()),
														m_pISD(rData.GetISD()),
														m_TempBeadTotal(0),
														m_TempBondTotal(0),  
														m_TempBondPairTotal(0),   
														m_TempPolymerTotal(0) 
{

#endif

    // Clear the temporary locations for entity totals
	
	m_vTempBeadTypeTotal.clear();
	m_vTempBondTypeTotal.clear();
	m_vTempBondPairTypeTotal.clear();
	m_vTempPolymerTypeTotal.clear();
	
	// Store the size and number fractions of the polymer types in maps for later use.

	m_mPolymerSizes.clear();
	m_mPolymerFractions.clear();
	
	for(PolymerVectorIterator iterPoly=vPolymerTypes.begin(); iterPoly!=vPolymerTypes.end(); iterPoly++)
	{
	    m_mPolymerSizes.insert(make_pair((*iterPoly)->GetType(), rData.GetPolymerTypeSize((*iterPoly)->GetType())));
	    m_mPolymerFractions.insert(make_pair((*iterPoly)->GetType(), rData.GetPolymerTypeFraction((*iterPoly)->GetType())));
	}

	// We treat the Z dimension separately to allow for conditional compilation
	// in 2 and 3 dimensions

#if SimDimension == 2
    ProcessorsZNo       = 1;
	CNTZCellNo			= 1;
	CNTZCellWidth		= 0.0;
#elif SimDimension == 3
    ProcessorsZNo       = rData.GetProcessorsZNo(),
	CNTZCellNo			= rData.GetCNTZCellNo();
	CNTZCellWidth		= rData.GetCNTZCellWidth();
#endif

	// **********************************************************************
	// If this is a parallel simulation, we delegate functions to a nested class
    // to create and manage beads, bonds, bondpairs and polymers.

#if EnableParallelSimBox == SimMPSEnabled
    m_pParallelIS = new mpaInitialState(this);
#endif

	// **********************************************************************
	// Set default values for the constraints that may be imposed on the SimBox.

	// Copy the data defining the wall if the user has specified one. 
	// Note that the data has been validated in CInputData (numeric values 
	// non-zero and wall polymer name occurs in the polymer names map).
	// We assume the wall polymer has only a single bead, and hence is
	// not movable, until the CreateWallPolymers() function is called and
	// sets the flag correctly.

	if(rData.IsWallPresent())
	{
		m_bWall					= true;
		m_bWallPolymerFlexible	= false;
		WallPolymerName			= rData.GetWallPolymerName();		
		WallPolymerType			= (*(rData.GetPolymerNamesMap().find(WallPolymerName))).second;
		WallBeadDensity			= rData.GetWallBeadDensity();				
		WallXWidth				= rData.GetWallXWidth();
		WallYWidth				= rData.GetWallYWidth();
		WallZWidth				= rData.GetWallZWidth();
	}
	else
	{
		m_bWall					= false;
		m_bWallPolymerFlexible	= false;
		WallPolymerName			= "";
		WallPolymerType			= 0;
		WallBeadDensity			= 0.0;				
		WallXWidth				= 0;
		WallYWidth				= 0;
		WallZWidth				= 0;
	}

	// Copy the data defining a uniform body force that acts throughout 
	// the SimBox that we call gravity if the user has specified it. 
	// Note that the data has been validated in CInputData (bead names 
	// are valid). Also note that we can only fill up the vGravityBeads vector
	// once the bulk beads have been created (i.e., after CreatePolymers() 
	// has been called). Hence we simply ensure the vector is empty here and 
	// call SetGravityBeads() from within CreatePolymers().

	if(rData.IsGravityPresent())
	{
		m_bGravity			= true;
		vGravityBeadTypes	= rData.GetGravityBeadTypes();	// vector of longs not strings
		vGravityBeads.clear();
		GravityXForce		= rData.GetGravityXForce();
		GravityYForce		= rData.GetGravityYForce();
		GravityZForce		= rData.GetGravityZForce();
	}
	else
	{
		m_bGravity			= false;
		vGravityBeadTypes.clear();
		vGravityBeads.clear();
		GravityXForce		= 0.0;
		GravityYForce		= 0.0;
		GravityZForce		= 0.0;
	}

	// Copy the data defining shear on the simulation box. NOT IMPLEMENTED YET

	if(rData.IsShearPresent())
	{
		m_bShear = true;
	}
	else
	{
		m_bShear = false;
	}

	// Set the initial state as not polymerised by default

	m_bIsPolymerised = false;

	// **********************************************************************

	// Store the bead structure data: bead-bead interaction parameters, etc.
	// This is written to the initial state file file for reference. Note that the
	// CInputData functions return pointers to the data so we copy the data 
	// itself to avoid keeping pointers to the CInputData around. But note that
	// the CNTCell static pointers point to the CInputData structure, although
	// these are then used to copy the data itself.

#if SimIdentifier == BD
	m_vvConsInt = *rData.GetDPDBeadConsIntArray();
	m_vvDissInt = *rData.GetDPDBeadDissIntArray();
#elif SimIdentifier == DPD
	m_vvConsInt = *rData.GetDPDBeadConsIntArray();
	m_vvDissInt = *rData.GetDPDBeadDissIntArray();
	m_vvLGInt   = *rData.GetDPDBeadLGIntArray();
#elif SimIdentifier == MD
	m_vvLJDepth = *rData.GetMDBeadLJDepthArray();
	m_vvLJRange = *rData.GetMDBeadLJRangeArray();
	m_vvSCDepth = *rData.GetMDBeadSCDepthArray();
	m_vvSCRange = *rData.GetMDBeadSCRangeArray();
#endif

    // Ensure the local containers of command targets and decorators are empty. 
    // They are only used when reading a restart state that contains saved targets.

    m_CommandTargets.clear();
    m_Decorators.clear();
}

// Delete the CPolymer objects representing the "types" of polymers in the simulation
// and the CBead, CBond and CBondPair types. There are two kinds of such types in the 
// simulation: the first are zString-based representations of the objects 
// used for reading and writing from file and stored in maps in CInputData
// using a name as the index into the map: these do not need to be deleted as they 
// are not created using new; the second are the CBead, CBond, CBondPair and CPolymer 
// objects used as templates to create the multiple copies needed for the actual 
// simulation. We delete the vectors holding the template objects as they were 
// created using new in CInputData, but are stored here.
//
// The serial code owns all the beads, bonds, bondpairs and polymers, so it can delete them here 
// as it knows they are of constant number. It also deletes all dynamically-created polymerized bonds.
// 
// But for the parallel code, polymers (and their containe beads, bonds and bondpairs) are owned and destroyed by the parallel SimBox,
// that is also responsible for deleting any dynamically-created polymerized bonds.

CInitialState::~CInitialState()
{
    // If this is a parallel simulation, we delete the nested parallel initial state instance that was created in the constructor.

#if EnableParallelSimBox == SimMPSEnabled

    delete m_pParallelIS;
    m_pParallelIS = 0;
	
#else
	// Delete all the polymers that were created to form the initial state object, and
	// let the CPolymers destroy their own beads and bonds. 

	if(!vAllPolymers.empty())
	{
		for(PolymerVectorIterator iterPolymer=vAllPolymers.begin(); iterPolymer!=vAllPolymers.end(); iterPolymer++)
		{
			delete (*iterPolymer);
		} 
		vAllPolymers.clear();
	}

	// Now do the same for the CWallPolymers that compose the walls in the simulation

	if( !vWallPolymers.empty() )
	{
		for(PolymerVectorIterator iterPolymer=vWallPolymers.begin(); iterPolymer!=vWallPolymers.end(); iterPolymer++)
		{
			delete (*iterPolymer);
		} 
		vWallPolymers.clear();
	}

	// Delete the polymerised bonds separately because they are not stored within
	// polymers

	if(!vAllPolymerisedBonds.empty())
	{
		for(BondVectorIterator iterBond=vAllPolymerisedBonds.begin(); iterBond!=vAllPolymerisedBonds.end(); iterBond++)
		{
			delete (*iterBond);
		} 
		vAllPolymerisedBonds.clear();
	}

#endif

	// For both the serial and parallel code, delete the bead, bond, bondpair and polymer templates stored in vectors of "types"

	for(BeadVectorIterator iterBead=vBeadTypes.begin(); iterBead!=vBeadTypes.end(); iterBead++)
	{
		delete *iterBead;
	}

	for(BondVectorIterator iterBond=vBondTypes.begin(); iterBond!=vBondTypes.end(); iterBond++)
	{
		delete *iterBond;
	}

	for(BondPairVectorIterator iterBondPair=vBondPairTypes.begin(); iterBondPair!=vBondPairTypes.end(); iterBondPair++)
	{
		delete *iterBondPair;
	}

	for(PolymerVectorIterator iterPolymer=vPolymerTypes.begin(); iterPolymer!=vPolymerTypes.end(); iterPolymer++)
	{
		delete *iterPolymer;
	}

// Delete the inclusive restart state object if it exists

	if(m_pIRS)
	{
		delete m_pIRS;
	}
}

// **********************************************************************
// Note that the dimension of the simulation is set by conditional compilation
// and not by any arguments to the CInitialState functions.
// **********************************************************************

const zString CInitialState::GetInitialStateType() const
{
	return m_pISD->GetInitialStateType();
}

double CInitialState::GetSimBoxXLength() const
{
	return CNTXCellWidth*static_cast<double>(CNTXCellNo);
}

double CInitialState::GetSimBoxYLength() const
{
	return CNTYCellWidth*static_cast<double>(CNTYCellNo);
}

double CInitialState::GetSimBoxZLength() const
{
	return CNTZCellWidth*static_cast<double>(CNTZCellNo);
}

// Functions to return the size of the bulk simulation box, that is the
// length along each side not including the wall.

double CInitialState::GetSimBoxBulkXLength() const
{
	return CNTXCellWidth*static_cast<double>(CNTXCellNo-2*WallXWidth);
}

double CInitialState::GetSimBoxBulkYLength() const
{
	return CNTYCellWidth*static_cast<double>(CNTYCellNo-2*WallYWidth);
}

double CInitialState::GetSimBoxBulkZLength() const
{
	return CNTZCellWidth*static_cast<double>(CNTZCellNo-2*WallZWidth);
}

// Functions to return the width of the wall.

double CInitialState::GetWallXWidth() const
{
	return CNTXCellWidth*static_cast<double>(WallXWidth);
}

double CInitialState::GetWallYWidth() const
{
	return CNTYCellWidth*static_cast<double>(WallYWidth);
}

double CInitialState::GetWallZWidth() const
{
	return CNTZCellWidth*static_cast<double>(WallZWidth);
}

// Function to create the required number of polymers of each type so as to
// fill the simulation box with the density of beads specified and satisfy the
// polymer number fractions. Note that because the number fraction of each
// polymer type may not lead to a whole number of polymers of that type, given 
// the total bead number constraint, the total number of beads created for all
// the actual polymers might not equal the average bead density multiplied by 
// the simulation box volume.
//
// The inputs to this routine are the vectors of bead, bond, bondpair and 
// polymer types.
//
// The outputs of this routine are vectors of all the beads, bonds and bondpairs
// used in the simulation.
//
// This function should only be used in a serial simulation.

void CInitialState::CreatePolymers()
{
	// First calculate the numbers of each polymer required from their number fraction
	// and the total number of beads in the simulation box.

#if SimDimension == 2
	double totalBeads = AveBeadDensity*GetSimBoxXLength()*GetSimBoxYLength();
#elif SimDimension == 3
	double totalBeads = AveBeadDensity*GetSimBoxXLength()*GetSimBoxYLength()*GetSimBoxZLength();
#endif

	double nalphasum = 0.0;

	for(long unsigned type=0; type<vPolymerTypes.size(); type++)
	{
		nalphasum+= static_cast<double>(GetPolymerSizeForType(type))*GetPolymerFractionForType(type);
	}

	for(long unsigned type1=0; type1<vPolymerTypes.size(); type1++)
	{
		m_vPolymerTypeTotal.push_back(static_cast<long>(GetPolymerFractionForType(type1)*totalBeads/nalphasum));
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
	
	long BeadTotal		= 0; 
	long BondTotal		= 0;
	long BondPairTotal	= 0;
	long PolymerTotal	= 0;

	for(cPolymerVectorIterator iterPolyType=vPolymerTypes.begin(); iterPolyType!=vPolymerTypes.end(); iterPolyType++)
	{
		for(long pno=0; pno<m_vPolymerTypeTotal.at((*iterPolyType)->GetType()); pno++)
		{
			PolymerTotal++;
			CPolymer* pPolymer = new CPolymer(**iterPolyType);
			pPolymer->SetId(PolymerTotal);

			BeadTotal		= pPolymer->SetBeadIds(BeadTotal);
			BondTotal		= pPolymer->SetBondIds(BondTotal);
			BondPairTotal	= pPolymer->SetBondPairIds(BondPairTotal);

			vAllPolymers.push_back(pPolymer);

			copy((pPolymer->GetBeads()).begin(),	(pPolymer->GetBeads()).end(),		back_inserter(vAllBeads));
			copy((pPolymer->GetBonds()).begin(),	(pPolymer->GetBonds()).end(),		back_inserter(vAllBonds));
			copy((pPolymer->GetBondPairs()).begin(),(pPolymer->GetBondPairs()).end(),	back_inserter(vAllBondPairs));
		}
	}

// We have created all beads, bonds and polymers so we now count the numbers of each type,
// noting that the polymers have already been counted above.

	CalculateBeadTypeTotals();
	CalculateBondTypeTotals();
	CalculateBondPairTypeTotals();

// Copy bead pointers into a vector if the beads are affected by the body force. This
// is used to add the body force into the simulation by simply looping over the vector.

	SetGravityBeads();
}

// Function to count the number of beads (CBead objects) of each type.

void CInitialState::CalculateBeadTypeTotals()
{
	// Clear the vector and set its size to the number of bead types 
	// before counting to ensure that multiple calls 
	// to this function do not continually increment the counters

	m_vBeadTypeTotal.clear();
	m_vBeadTypeTotal.resize(GetBeadTypeTotal(), 0);

	for(cBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		m_vBeadTypeTotal.at((*iterBead)->GetType())++;
	}
}

// Function to count the number of bonds (CBond objects) of each type.

void CInitialState::CalculateBondTypeTotals()
{
	// Clear the vector and set its size to the number of bead types 
	// before counting to ensure that multiple calls 
	// to this function do not continually increment the counters

	m_vBondTypeTotal.clear();
	m_vBondTypeTotal.resize(GetBondTypeTotal(), 0);

	for(cBondVectorIterator iterBond=vAllBonds.begin(); iterBond!=vAllBonds.end(); iterBond++)
	{
		m_vBondTypeTotal.at((*iterBond)->GetType())++;
	}
}

// Function to count the number of stiff bonds (CBondPair objects), if any, of each type.

void CInitialState::CalculateBondPairTypeTotals()
{
	// Clear the vector and set its size to the number of bead types 
	// before counting to ensure that multiple calls 
	// to this function do not continually increment the counters

	m_vBondPairTypeTotal.clear();
	m_vBondPairTypeTotal.resize(GetBondPairTypeTotal(), 0);

	for(cBondPairVectorIterator iterBP=vAllBondPairs.begin(); iterBP!=vAllBondPairs.end(); iterBP++)
	{
		m_vBondPairTypeTotal.at((*iterBP)->GetType())++;
	}
}

// Function to create the CWallPolymers that have their head fixed in the wall  
// surrouding the simulation box if this feature is selected.
// We calculate the number of wall polymers required from the wall bead
// density and thickness of the walls. Because the wall polymers are attached
// by their head bead to the wall, there is a 1-1 correspondence between 
// them. Note that we have already validated the wall data in CInputData so the 
// bead density and wall thicknesses are ok.
//
// This function must be called after CreatePolymers() because it uses the number
// of polymers already created to start the wall polymer counter used to assign ids.

void CInitialState::CreateWallPolymers()
{
	double totalArea = GetSimBoxXLength()*GetSimBoxYLength();
	double bulkArea  = GetSimBoxBulkXLength()*GetSimBoxBulkYLength();

#if SimDimension == 2
	long totalWallBeads = static_cast<long>(WallBeadDensity*(totalArea - bulkArea));
#elif SimDimension == 3
	long totalWallBeads = static_cast<long>(WallBeadDensity*
						(totalArea*GetSimBoxZLength() - bulkArea*GetSimBoxBulkZLength()));
#endif

	// Create the wall's CPolymers by copying the archetype in vPolymerTypes corresponding
	// to the wall polymer's type. The WallPolymerTotal is initialized to zero in the
	// constructor and we start the wall polymer id count at the previous value set by
	// the total number of ordinary polymers in the simulation.

	long BeadTotal		  = GetBeadTotal();
	long BondTotal		  = GetBondTotal();
	long BondPairTotal	  = GetBondPairTotal();
	long PolymerTotal	  = GetPolymerTotal();
	long WallPolymerTotal = 0;

	CPolymer* pPolymerType = vPolymerTypes.at(WallPolymerType);

	for(long pno=0; pno<totalWallBeads; pno++)
	{
		WallPolymerTotal++;
		CPolymer* pPolymer = new CPolymer(*pPolymerType);
		pPolymer->SetId(PolymerTotal + WallPolymerTotal);

		BeadTotal		= pPolymer->SetBeadIds(BeadTotal);
		BondTotal		= pPolymer->SetBondIds(BondTotal);
		BondPairTotal	= pPolymer->SetBondPairIds(BondPairTotal);
		pPolymer->ChangeHeadtoWallBead();		// this removes the wall bead
												// but keeps the original bead id
//		TraceStringNoEndl("Wall Polymer no ");
//		TraceIntNoEndl(pPolymer->GetId());
//		TraceIntNoEndl(pPolymer->GetType());
//		TraceIntNoEndl(pPolymer->GetSize());
//		TraceEndl();

		vWallBeads.push_back(pPolymer->GetHead());
		vWallPolymers.push_back(pPolymer);

		copy((pPolymer->GetBeads()).begin(),	(pPolymer->GetBeads()).end(),		back_inserter(vAllBeads));
		copy((pPolymer->GetBonds()).begin(),	(pPolymer->GetBonds()).end(),		back_inserter(vAllBonds));	
		copy((pPolymer->GetBondPairs()).begin(),(pPolymer->GetBondPairs()).end(),	back_inserter(vAllBondPairs));
	}

	// Store a flag showing whether the wall polymers have only a single bead,
	// and hence cannot move, or whether they have a body that can fluctuate.
	// Notice that the head bead has already been removed from the vector of
	// beads, so the test is that if the "size" is empty there are no flexible
	// beads in the polymer.

	if(vWallPolymers.at(0)->GetSize() > 0)
		m_bWallPolymerFlexible = true;
	else
		m_bWallPolymerFlexible = false;

}

// Function to copy the bead pointers from the vAllBeads vector into vGravityBeads
// for those beads whose types are affected by the body force. We use the algorithm
// find() to search the vector of bead types affected by gravity for the type of each
// bead in the vAllBeads vector. If a bead's type is found, a pointer to it is added 
// to the vGravityBeads vector for use by the SimBox in adding the body force to all beads.

void CInitialState::SetGravityBeads()
{
	zLongVectorIterator iBeadType;

	for(cBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		iBeadType = find(vGravityBeadTypes.begin(), vGravityBeadTypes.end(), (*iterBead)->GetType());

		if(iBeadType != vGravityBeadTypes.end())	// bead is affected by gravity so add it to vector
		{
			vGravityBeads.push_back(*iterBead);
		}
	}
}

// Function to assemble the beads, bonds and polymers into the appropriate initial
// state as defined by the CInitialStateData-derived object and its builder. We
// create the required number of objects here, and create the wall beads  if there is
// one, and then call the CInitialStateData object to assign coordinates to the
// beads. We also write out the initial state specifications to file.

bool CInitialState::Assemble()
{
	// Create the beads, bonds and polymers specified in the control data file
	// and passed in via the CInputData object.

	CreatePolymers();
	
	if(IsWallPresent())
		CreateWallPolymers();

	// Now get the appropriate CInitialStateData-derived object to assemble 
	// the beads into its associated initial state using the CBuilder-derived
	// object. If this succeeds, write out the data on beads, bonds and polymers 
    // created for the initial state, and then close the file to force it to be 
    // written immediately so that the user can view it.

    if(m_pISD->Assemble(*this))
    {
	    // Copy the entity totals into local storage that is required by the parallel builder but only used here to avoid having to write two versions of the
		// serialisation function.
		
		m_TempBeadTotal     = GetAllProcessorBeadTotal();
		m_TempBondTotal     = GetAllProcessorBondTotal();
		m_TempBondPairTotal = GetAllProcessorBondPairTotal();
		m_TempPolymerTotal  = GetAllProcessorPolymerTotal();
		
		for(long beadType=0; beadType<GetBeadTypeTotal(); beadType++)
		{
		    m_vTempBeadTypeTotal.push_back(GetAllProcessorBeadTotalForType(beadType));
		}
		for(long bondType=0; bondType<GetBondTypeTotal(); bondType++)
		{
		    m_vTempBondTypeTotal.push_back(GetAllProcessorBondTotalForType(bondType));
		}
		for(long bondPairType=0; bondPairType<GetBondPairTypeTotal(); bondPairType++)
		{
		    m_vTempBondPairTypeTotal.push_back(GetAllProcessorBondPairTotalForType(bondPairType));
		}
		for(long polymerType=0; polymerType<GetPolymerTypeTotal(); polymerType++)
		{
		    m_vTempPolymerTypeTotal.push_back(GetAllProcessorPolymerTotalForType(polymerType));
		}

        if(Serialize())
            return true;
        else
		    return ErrorTrace("Error writing the initial state specification to file");
    }
    else
    {
        return false;
    }
}

// Parallel version of the Assemble() function that manages the creation of polymers for a parallel simulation. 	
// We delegate the creation of the beads, bonds and polymers 
// specified in the control data file to the nested instance to handle both P0 and PN creation.
// Once all polymers have been created, P0 writes out the totals to file. Because the pre-assembled
// initial states are not translationally invariant, we delegate the creation of polymers to the 
// concrete CInitialStateData-derived classes, where they know the required configuration and can 
// work out how many polymers of each type are needed.

bool CInitialState::AssembleP()
{
#if EnableParallelSimBox == SimMPSEnabled
	
    if(m_pParallelIS->CreatePolymers() && m_pISD->Assemble(*this))
    {
	    // Because only P0 calls the Serialize() function we have to store the entity totals for the whole simulation space
		// in temporary locations that are accessible by all processors. Note that this function must only be called once
		// otherwise the totals will accumulate.
		
		m_TempBeadTotal     = GetAllProcessorBeadTotal();
		m_TempBondTotal     = GetAllProcessorBondTotal();
		m_TempBondPairTotal = GetAllProcessorBondPairTotal();
		m_TempPolymerTotal  = GetAllProcessorPolymerTotal();
		
		for(long beadType=0; beadType<GetBeadTypeTotal(); beadType++)
		{
		    m_vTempBeadTypeTotal.push_back(GetAllProcessorBeadTotalForType(beadType));
		}
		for(long bondType=0; bondType<GetBondTypeTotal(); bondType++)
		{
		    m_vTempBondTypeTotal.push_back(GetAllProcessorBondTotalForType(bondType));
		}
		for(long bondPairType=0; bondPairType<GetBondPairTypeTotal(); bondPairType++)
		{
		    m_vTempBondPairTypeTotal.push_back(GetAllProcessorBondPairTotalForType(bondPairType));
		}
		for(long polymerType=0; polymerType<GetPolymerTypeTotal(); polymerType++)
		{
		    m_vTempPolymerTypeTotal.push_back(GetAllProcessorPolymerTotalForType(polymerType));
		}
		
        if(IsProcessZero())
        {
            if(Serialize())
                return true;
            else
		        return ErrorTrace("Error writing the parallel initial state specification to file");
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
#else
    return false;
#endif
}

// Functions to write out data defining the structure of the beads, bonds and polymers.
// These functions depend on the type of simulation being performed and the types of
// interactions between the beads (Lennard-Jones vs effective DPD forces for example).
// Hence, they will change if any details of the bead class changes.

void CInitialState::WriteBeadTypeData() const
{
	Trace("********************");
	for( cBeadVectorIterator iterBead=vBeadTypes.begin(); iterBead!=vBeadTypes.end(); iterBead++ )
	{
		TraceInt("Bead type", (*iterBead)->GetType());

#if SimIdentifier == BD

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			TraceDoubleNoEndl(m_vvConsInt.at((*iterBead)->GetType()).at(j));
		}
		TraceEndl();
		for(j=0; j<vBeadTypes.size(); j++)
		{
			TraceDoubleNoEndl(m_vvDissInt.at((*iterBead)->GetType()).at(j));
		}
		TraceEndl();

#elif SimIdentifier == DPD

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			TraceDoubleNoEndl(m_vvConsInt.at((*iterBead)->GetType()).at(j));
		}
		TraceEndl();
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			TraceDoubleNoEndl(m_vvDissInt.at((*iterBead)->GetType()).at(j));
		}
		TraceEndl();

#elif SimIdentifier == MD

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			TraceDoubleNoEndl(m_vvLJDepth.at((*iterBead)->GetType()).at(j));
		}
		TraceEndl();
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			TraceDoubleNoEndl(m_vvLJRange.at((*iterBead)->GetType()).at(j));
		}
		TraceEndl();

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			TraceDoubleNoEndl(m_vvSCDepth.at((*iterBead)->GetType()).at(j));
		}
		TraceEndl();
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			TraceDoubleNoEndl(m_vvSCRange.at((*iterBead)->GetType()).at(j));
		}
		TraceEndl();

#endif
	}
}

void CInitialState::WriteBondTypeData() const
{
 	Trace("********************");
 	for( cBondVectorIterator iterBond=vBondTypes.begin(); iterBond!=vBondTypes.end(); iterBond++ )
 	{
 		TraceInt("Bond type", (*iterBond)->GetType());
 		TraceDouble2("  Spring Const/Unstr length", (*iterBond)->GetSprConst(), (*iterBond)->GetUnStrLength());
 	}
}

void CInitialState::WriteBondPairTypeData() const
{
 	Trace("********************");
 	for( cBondPairVectorIterator iterBP=vBondPairTypes.begin(); iterBP!=vBondPairTypes.end(); iterBP++ )
 	{
 		TraceInt("Stiff bond type", (*iterBP)->GetType());
 		TraceDouble2("  Modulus/Angle", (*iterBP)->GetModulus(), (*iterBP)->GetPhi0());
 	}
}

void CInitialState::WritePolymerData() const
{
	Trace("********************");
 	for( cPolymerVectorIterator iterPoly=vPolymerTypes.begin(); iterPoly!=vPolymerTypes.end(); iterPoly++ )
	{
		TraceInt("Polymer type ",(*iterPoly)->GetType());
		TraceDouble("  with fraction          ",		GetPolymerFractionForType((*iterPoly)->GetType()));
		TraceInt(   "  containing beads       ",		(*iterPoly)->GetBeads().size());
		TraceInt(   "  and bonds              ",		(*iterPoly)->GetBonds().size());
		TraceInt(   "  and stiff bonds        ",		(*iterPoly)->GetBondPairs().size());

 		Trace("Bead types");
 
		for( cBeadVectorIterator iterBead=((*iterPoly)->GetBeads()).begin(); iterBead!=((*iterPoly)->GetBeads()).end(); iterBead++ )
		{
			TraceIntNoEndl((*iterBead)->GetType());
		}

		TraceEndl();

		Trace("Bond types");
 
		for( cBondVectorIterator iterBond=((*iterPoly)->GetBonds()).begin(); iterBond!=((*iterPoly)->GetBonds()).end(); iterBond++ )
		{
			TraceIntNoEndl((*iterBond)->GetType());
		}
		TraceEndl();

		Trace("Stiff bond types");
 
		for( cBondPairVectorIterator iterBP=((*iterPoly)->GetBondPairs()).begin(); iterBP!=((*iterPoly)->GetBondPairs()).end(); iterBP++ )
		{
			TraceIntNoEndl((*iterBP)->GetType());
		}
		TraceEndl();
 		TraceEndl();
 	}
}

// Function to write out the initial conditions of the simulation to file.
// We only write data that is constant over the whole simulation, such as the
// number of beads, bonds and polymers created,  their structure, i.e., interactions
// and bead radii, and the constraints on the SimBox at the start.

bool CInitialState::Serialize()
{
	if(m_IOFlag && m_bOpenFlag)	
	{
#if EnableXMLProtocol == SimXMLEnabled
		SerializeXMLFile();	
#elif EnableXMLProtocol == SimXMLDisabled
		SerializeAsciiFile();	
#endif
	}
	else
		return false; 

	return Close();
}

// Private helper functions to write the initial state data to an XML or ASCII file.
// We use separate functions to avoid having to use conditionals for each write
// statement. No check is made on whether the files exist or are writable: this must
// be done in the calling Serialize() routine.

void CInitialState::SerializeXMLFile()
{
	// First write the xml and stylesheet PIs: note that the version and
	// stylesheet name are hardwired here.

	m_outStream << "<?xml version=\"1.0\" ?>" << zEndl;
	m_outStream << "<?xml-stylesheet type=\"text/css\" href=\"initialstate.css\" ?>" << zEndl;

	// now the top-level InitialState element with its runId attribute

	m_outStream << "<InitialState runId = \"" << GetRunId() << "\">" << zEndl;

	// Numbers of each bead, bond and polymer type created 

	m_outStream << "<PolymerTotal>"  << m_TempPolymerTotal  << "</PolymerTotal>"  << zEndl;
	m_outStream << "<BeadTotal>"     << m_TempBeadTotal     << "</BeadTotal>"     << zEndl;
	m_outStream << "<BondTotal>"     << m_TempBondTotal     << "</BondTotal>"     << zEndl;
	m_outStream << "<BondPairTotal>" << m_TempBondPairTotal << "</BondPairTotal>" << zEndl;

	for(long type2=0; type2<GetPolymerTypeTotal(); type2++)
	{
		m_outStream << "<PolymerTypeTotal>";
		m_outStream << "<Type>"  << type2 << "</Type>";
		m_outStream << "<Total>" << m_vTempPolymerTypeTotal.at(type2) << "</Total>";
		m_outStream << "</PolymerTypeTotal>" << zEndl;
	}

	if(m_bWall)
	{
		m_outStream << zEndl;
		m_outStream << "<WallBeadTotal>"    << GetWallBeadTotal()    << "</WallBeadTotal>" << zEndl;
		m_outStream << "<WallPolymerTotal>" << GetWallPolymerTotal() << "</WallPolymerTotal>" << zEndl;
	}
	
	// Bead structure data depends on whether it is a BD, DPD or MD simulation, and on
    // whether a DPD bead includes the density-dependent LG interaction.

	m_outStream << "<Beads>" << zEndl;
	for(cBeadVectorIterator iterBead=vBeadTypes.begin(); iterBead!=vBeadTypes.end(); iterBead++)
	{
		m_outStream << "<Bead>" << zEndl;
		m_outStream << "<Type>"  << (*iterBead)->GetType() << "</Type>" << zEndl;

#if SimIdentifier == BD

		m_outStream << "<Radius>"  << (*iterBead)->GetRadius() << "</Radius>" << zEndl;
		m_outStream << "<TranslationalDiffusionCoefficient>"  << (*iterBead)->GetTransDiff() << "</TranslationalDiffusionCoefficient>" << zEndl;
		m_outStream << "<RotationalDiffusionCoefficient>"     << (*iterBead)->GetRotDiff()   << "</RotationalDiffusionCoefficient>" << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "<ConsInt>"  << m_vvConsInt.at((*iterBead)->GetType()).at(j) << "</ConsInt>";
		}
		m_outStream << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "<DissInt>"  << m_vvDissInt.at((*iterBead)->GetType()).at(j) << "</DissInt>";
		}
		m_outStream << zEndl;

#elif SimIdentifier == DPD

#if EnableDPDLG == ExperimentEnabled

        if(IsDPDLG())
        {
		    m_outStream << "<Radius>"    << (*iterBead)->GetRadius() << "</Radius>" << zEndl;
		    m_outStream << "<LGRadius>"  << (*iterBead)->GetLGRadius() << "</LGRadius>" << zEndl;
        }
        else
        {
		    m_outStream << "<Radius>"    << (*iterBead)->GetRadius() << "</Radius>" << zEndl;
        }

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "<ConsInt>"  << m_vvConsInt.at((*iterBead)->GetType()).at(j) << "</ConsInt>";
		}
		m_outStream << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "<DissInt>"  << m_vvDissInt.at((*iterBead)->GetType()).at(j) << "</DissInt>";
		}
		m_outStream << zEndl;

        if(IsDPDLG())
        {
		    for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		    {
			    m_outStream << "<LGInt>"  << m_vvLGInt.at((*iterBead)->GetType()).at(j) << "</LGInt>";
		    }
		    m_outStream << zEndl;
        }
#elif EnableDPDLG == ExperimentDisabled
		m_outStream << "<Radius>"  << (*iterBead)->GetRadius() << "</Radius>" << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "<ConsInt>"  << m_vvConsInt.at((*iterBead)->GetType()).at(j) << "</ConsInt>";
		}
		m_outStream << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "<DissInt>"  << m_vvDissInt.at((*iterBead)->GetType()).at(j) << "</DissInt>";
		}
		m_outStream << zEndl;
#endif
#elif SimIdentifier == MD

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "<LJDepth>"  << m_vvLJDepth.at((*iterBead)->GetType()).at(j) << "</LJDepth>";
		}
		m_outStream << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "<LJRange>"  << m_vvLJRange.at((*iterBead)->GetType()).at(j) << "</LJRange>";
		}
		m_outStream << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "<SCDepth>"  << m_vvSCDepth.at((*iterBead)->GetType()).at(j) << "</SCDepth>";
		}
		m_outStream << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "<SCRange>"  << m_vvSCRange.at((*iterBead)->GetType()).at(j) << "</SCRange>";
		}
		m_outStream << zEndl;

#endif
		m_outStream << "</Bead>" << zEndl;
	}
	m_outStream << "</Beads>" << zEndl;

	// Bond data 
	m_outStream << "<Bonds>" << zEndl;
	for( cBondVectorIterator iterBond=vBondTypes.begin(); iterBond!=vBondTypes.end(); iterBond++ )
 	{
 		m_outStream << "<Bond>" << zEndl;
		m_outStream << "<Type>" << (*iterBond)->GetType() << "</Type>" << zEndl;
		m_outStream << "<SpringConstant>"    << (*iterBond)->GetSprConst()    << "</SpringConstant>" << zEndl;
		m_outStream << "<UnstretchedLength>" << (*iterBond)->GetUnStrLength() << "</UnstretchedLength>" << zEndl;
 		m_outStream << "</Bond>" << zEndl;
	}
	m_outStream << "</Bonds>" << zEndl;

	// Bond pair data
 	m_outStream << "<BondPairs>" << zEndl;
	for( cBondPairVectorIterator iterBP=vBondPairTypes.begin(); iterBP!=vBondPairTypes.end(); iterBP++ )
 	{
 		m_outStream << "<BondPair>" << zEndl;
		m_outStream << "<Type>"           << (*iterBP)->GetType()    << "</Type>" << zEndl;
		m_outStream << "<BendingModulus>" << (*iterBP)->GetModulus() << "</BendingModulus>" << zEndl;
		m_outStream << "<PreferredAngle>" << (*iterBP)->GetPhi0()    << "</PreferredAngle>" << zEndl;
 		m_outStream << "</BondPair>" << zEndl;
 	}
	 m_outStream << "</BondPairs>" << zEndl;

	// Polymer data
 	m_outStream << "<Polymers>" << zEndl;
 	for(cPolymerVectorIterator iterPoly=vPolymerTypes.begin(); iterPoly!=vPolymerTypes.end(); iterPoly++)
	{
 		m_outStream << "<Polymer>"  << zEndl;
		m_outStream << "<Type>"     << (*iterPoly)->GetType()      << "</Type>"     << zEndl;
		m_outStream << "<Fraction>" << setw(12) << setprecision(8) << zLeft << GetPolymerFractionForType((*iterPoly)->GetType()) << "</Fraction>" << zEndl;


 		m_outStream << "<BeadTypes>";
		m_outStream << "<BeadTotal>" << (*iterPoly)->GetBeads().size() << "</BeadTotal>" << zEndl;
		for( cBeadVectorIterator iterBead=((*iterPoly)->GetBeads()).begin(); iterBead!=((*iterPoly)->GetBeads()).end(); iterBead++ )
		{
			m_outStream << "<Type>" << (*iterBead)->GetType() << "</Type>";
		}
 		m_outStream << "</BeadTypes>" << zEndl;

		m_outStream << "<BondTypes>";
		m_outStream << "<BondTotal>" << (*iterPoly)->GetBonds().size() << "</BondTotal>" << zEndl;
		if((*iterPoly)->GetBonds().size() > 0)
		{
			for( cBondVectorIterator iterBond=((*iterPoly)->GetBonds()).begin(); iterBond!=((*iterPoly)->GetBonds()).end(); iterBond++ )
			{
				m_outStream << "<Type>" << (*iterBond)->GetType() << "</Type>";
			}
		}
 		m_outStream << "</BondTypes>" << zEndl;


		m_outStream << "<BondPairTypes>";
		m_outStream << "<BondPairTotal>" << (*iterPoly)->GetBondPairs().size() << "</BondPairTotal>" << zEndl;
		if((*iterPoly)->GetBondPairs().size() > 0)
		{
			for( cBondPairVectorIterator iterBP=((*iterPoly)->GetBondPairs()).begin(); iterBP!=((*iterPoly)->GetBondPairs()).end(); iterBP++ )
			{
				m_outStream << "<Type>" << (*iterBP)->GetType() << "</Type>";
			}
		}
 		m_outStream << "</BondPairTypes>" << zEndl;

		m_outStream << "</Polymer>" << zEndl;
 	}
 	m_outStream << "</Polymers>" << zEndl;

	m_outStream << "</InitialState>" << zEndl;
}

void CInitialState::SerializeAsciiFile()
{
	// Numbers of each bead, bond, bondpair and polymer type created 
	
	m_outStream << "Number of polymers created		" << m_TempPolymerTotal	    << zEndl;
	m_outStream << "Number of beads created			" << m_TempBeadTotal		<< zEndl;
	m_outStream << "Number of bonds created			" << m_TempBondTotal	    << zEndl;
	m_outStream << "Number of bond pairs created	" << m_TempBondPairTotal    << zEndl;
		
	for(long type2=0; type2<GetPolymerTypeTotal(); type2++)
	{
		m_outStream << "  # of type " << type2 << "  " << m_vTempPolymerTypeTotal.at(type2) << zEndl;
	}

	if(m_bWall)
	{
		m_outStream << zEndl;
		m_outStream << "Number of Wall beads created	" << GetWallBeadTotal()    << zEndl;
		m_outStream << "Number of Wall polymers created	" << GetWallPolymerTotal() << zEndl;
	}
	
	// Bead structure data depending on whether it is a BD, DPD or MD simulation


	for(cBeadVectorIterator iterBead=vBeadTypes.begin(); iterBead!=vBeadTypes.end(); iterBead++)
	{
		m_outStream << zEndl;
		m_outStream << "Bead type " << (*iterBead)->GetType() << zEndl;

#if SimIdentifier == BD

		m_outStream << "  Radius: ";
		m_outStream << (*iterBead)->GetRadius() << zEndl;
		m_outStream << "  Translational Diffusion: ";
		m_outStream << (*iterBead)->GetTransDiff() << zEndl;
		m_outStream << "  Rotational Diffusion: ";
		m_outStream << (*iterBead)->GetRotDiff() << zEndl;

		m_outStream << "  Cons Int: ";
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << m_vvConsInt.at((*iterBead)->GetType()).at(j) << " ";
		}
		m_outStream << zEndl;

		m_outStream << "  Diss Int: ";
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << m_vvDissInt.at((*iterBead)->GetType()).at(j) << " ";
		}
		m_outStream << zEndl;

#elif SimIdentifier == DPD

#if EnableDPDLG == ExperimentEnabled
		m_outStream << "  Radius: ";

        if(IsDPDLG())
        {
		    m_outStream << (*iterBead)->GetRadius() << " " << (*iterBead)->GetLGRadius() << zEndl;
        }
        else
        {
		    m_outStream << (*iterBead)->GetRadius() << zEndl;
        }

		m_outStream << "  Cons Int: ";
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << m_vvConsInt.at((*iterBead)->GetType()).at(j) << " ";
		}
		m_outStream << zEndl;

		m_outStream << "  Diss Int: ";
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << m_vvDissInt.at((*iterBead)->GetType()).at(j) << " ";
		}
		m_outStream << zEndl;

        if(IsDPDLG())
        {
		    m_outStream << "  LG Int:   ";
		    for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		    {
			    m_outStream << m_vvLGInt.at((*iterBead)->GetType()).at(j) << " ";
		    }

		    m_outStream << zEndl;
        }
#elif EnableDPDLG == ExperimentDisabled
		m_outStream << "  Radius: ";
		m_outStream << (*iterBead)->GetRadius() << zEndl;

		m_outStream << "  Cons Int: ";
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << m_vvConsInt.at((*iterBead)->GetType()).at(j) << " ";
		}
		m_outStream << zEndl;

		m_outStream << "  Diss Int: ";
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << m_vvDissInt.at((*iterBead)->GetType()).at(j) << " ";
		}
		m_outStream << zEndl;
#endif
#elif SimIdentifier == MD

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "  LJ depth: " << m_vvLJDepth.at((*iterBead)->GetType()).at(j) << " ";
		}
		m_outStream << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "  LJ range: " << m_vvLJRange.at((*iterBead)->GetType()).at(j) << " ";
		}
		m_outStream << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "  SC depth: " << m_vvSCDepth.at((*iterBead)->GetType()).at(j) << " ";
		}
		m_outStream << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			m_outStream << "  SC range: " << m_vvSCRange.at((*iterBead)->GetType()).at(j) << " ";
		}
		m_outStream << zEndl;

#endif
	}

	// Bond data 
 	for( cBondVectorIterator iterBond=vBondTypes.begin(); iterBond!=vBondTypes.end(); iterBond++ )
 	{
 		m_outStream << zEndl;
		m_outStream << "Bond type " << (*iterBond)->GetType() << zEndl;
 		m_outStream << "  Spring Const/Unstr length " << (*iterBond)->GetSprConst() << " " << (*iterBond)->GetUnStrLength() << zEndl;
	}
	m_outStream << zEndl;

	// Bond pair data
 	for( cBondPairVectorIterator iterBP=vBondPairTypes.begin(); iterBP!=vBondPairTypes.end(); iterBP++ )
 	{
 			m_outStream << zEndl;
			m_outStream << "Stiff bond type " << (*iterBP)->GetType() << zEndl;
 			m_outStream << "  Modulus/Angle " << (*iterBP)->GetModulus() << " " << (*iterBP)->GetPhi0() << zEndl;
 	}
	m_outStream << zEndl;		
	
	// Polymer data
 	for(cPolymerVectorIterator iterPoly=vPolymerTypes.begin(); iterPoly!=vPolymerTypes.end(); iterPoly++)
	{
		m_outStream << "Polymer type " << (*iterPoly)->GetType() << zEndl;

		// Note the use of format specifiers for the next output field

		m_outStream << "  with fraction          " << setw(12) << setprecision(8) << zLeft;
		m_outStream << GetPolymerFractionForType((*iterPoly)->GetType())  << zEndl;

		m_outStream << "  containing beads       " << (*iterPoly)->GetBeads().size()	<< zEndl;
		m_outStream << "  and bonds              " << (*iterPoly)->GetBonds().size()	<< zEndl;

 		m_outStream << "  Bead types" << zEndl;

		for( cBeadVectorIterator iterBead=((*iterPoly)->GetBeads()).begin(); iterBead!=((*iterPoly)->GetBeads()).end(); iterBead++ )
		{
			m_outStream << "  " << (*iterBead)->GetType() << " ";
		}

		// Only write bond data if the polymer has more than one bead
		if((*iterPoly)->GetBonds().size() > 0)
		{
			m_outStream << zEndl << "  Bond types" << zEndl;

			for( cBondVectorIterator iterBond=((*iterPoly)->GetBonds()).begin(); iterBond!=((*iterPoly)->GetBonds()).end(); iterBond++ )
			{
				m_outStream << "  " << (*iterBond)->GetType() << " ";
			}

			m_outStream << zEndl << "  Stiff bond types" << zEndl;

			for( cBondPairVectorIterator iterBP=((*iterPoly)->GetBondPairs()).begin(); iterBP!=((*iterPoly)->GetBondPairs()).end(); iterBP++ )
			{
				m_outStream << "  " << (*iterBP)->GetType() << " ";
			}
		}
		m_outStream << zEndl;
		m_outStream << zEndl;		
 	}
}


// Functions to return the DPD bead conservative and dissipative interaction
// parameters. We check first that the types specified are legal.

double CInitialState::GetDPDBeadConsInt(long first, long second) const
{
	if(0 <= first  && first  < GetBeadTypeTotal() && 
	   0 <= second && second < GetBeadTypeTotal())
	{
		return m_vvConsInt.at(first).at(second);
	}
	else
		return 0.0;
}

double CInitialState::GetDPDBeadDissInt(long first, long second) const
{
	if(0 <= first  && first  < GetBeadTypeTotal() && 
	   0 <= second && second < GetBeadTypeTotal())
	{
		return m_vvDissInt.at(first).at(second);
	}
	else
		return 0.0;
}

// Functions to return the DPD bead density-dependent interaction
// parameters. We check first that the types specified are legal.

double CInitialState::GetDPDBeadLGInt(long first, long second) const
{
#if EnableDPDLG == ExperimentEnabled
	if(0 <= first  && first  < GetBeadTypeTotal() && 
	   0 <= second && second < GetBeadTypeTotal())
	{
		return m_vvLGInt.at(first).at(second);
	}
	else
		return 0.0;
#else
    return 0.0;
#endif
}

// Functions to set a new value for the DPD bead-bead conservative and dissipative
// interaction parameters. These are called, for example, in response to a command.

void CInitialState::SetDPDBeadConsInt(long first, long second, double newValue)
{
	if(0 <= first  && first  < GetBeadTypeTotal() && 
	   0 <= second && second < GetBeadTypeTotal())
	{
		m_vvConsInt.at(first).at(second) = newValue;
		m_vvConsInt.at(second).at(first) = newValue;	
	}
}

void CInitialState::SetDPDBeadDissInt(long first, long second, double newValue)
{
	if(0 <= first  && first  < GetBeadTypeTotal() && 
	   0 <= second && second < GetBeadTypeTotal())
	{
		m_vvDissInt.at(first).at(second) = newValue;
		m_vvDissInt.at(second).at(first) = newValue;
	}
}

// Function to set the a new for the DPD density-dependent interaction parameters.

void CInitialState::SetDPDBeadLGInt(long first, long second, double newValue)
{
#if EnableDPDLG == ExperimentEnabled
	if(0 <= first  && first  < GetBeadTypeTotal() && 
	   0 <= second && second < GetBeadTypeTotal())
	{
		m_vvLGInt.at(first).at(second) = newValue;
		m_vvLGInt.at(second).at(first) = newValue;
	}
#endif
}

// Functions to return the energy and length scales in the simulation.
// For an MD simulation these are the first entries in the LJ potential
// vectors, i.e., those values for the first bead species defined in the
// control data file.
// The BD and DPD simulations do not have  physical energy and length scales so
// we just return unity.

double CInitialState::GetEnergyScale() const
{
#if SimIdentifier == BD
    return 1.0;
#elif SimIdentifier == DPD
	return 1.0;
#elif SimIdentifier == MD
	return m_vvLJDepth.at(0).at(0);
#endif
}

double CInitialState::GetLengthScale() const
{
#if SimIdentifier == BD
    return 1.0;
#elif SimIdentifier == DPD
	return 1.0;
#elif SimIdentifier == MD
	return m_vvLJRange.at(0).at(0);
#endif
}

// Function to store bonds between polymerised polymers. It is used for
// bonds created other than by the CInputData such as bonds between polymers
// when an initial state is polymerised. It adds them to the vector holding 
// only polymerised bonds. This allows iteration over all polymerised bonds 
// separately. The bonds are deleted in the CInitialState destructor because 
// they do not belong to specific polymers, and hence are not destroyed when 
// the polymers are destroyed.
//
// The polymerised bonds are also added to the general bond vector, vAllBonds, 
// by the second function. These functions are separated so that polymerised 
// bonds can be pruned away according to various conditions (polymerisation 
// fraction, bonds not being allowed to cross the SimBox boundaries, etc) 
// before being added to the general bond vector.

void CInitialState::AddPolymerisedBond(CBond *pBond)
{
	vAllPolymerisedBonds.push_back(pBond);
}

void CInitialState::AddPolymerisedBondtoAllBonds(CBond *pBond)
{
	vAllBonds.push_back(pBond);
}

// Function to dynamically add a bead type to the simulation. It is 
// usually called as the result of a command such as ccChangeBeadType.
//
// We use the overloaded CBead constructor that takes only an integer 
// type identifier as its argument and creates a template bead with -1 
// as its id to indicate that it is not a bead that is actually used 
// in the CSimBox. The new template bead is destroyed in CInitialState's 
// destructor. After creating a template bead type, it copies the radius and
// interaction parameters associated with the original type to the new type.
// Note that the numeric value of the new bead type is just the
// previous number of bead types.

void CInitialState::AddBeadType(long oldType)
{
	CBead* const pBead = new CBead(vBeadTypes.size());

    pBead->SetRadius(vBeadTypes.at(oldType)->GetRadius());

	vBeadTypes.push_back(pBead);

	// Copy the interactions associated with the original type to the
	// new location. This depends on the simulation method: DPD or MD.

	AddDPDBeadType(oldType);
}

// Function to add the DPD conservative and dissipative interactions for
// a dynamically-created bead type. It is a copy of the CCNTCell function
// of the same name. It simply copies the interaction parameters
// associcated with the type "oldType" to the last column and row
// of the interaction matrices. Hence, the new type automatically has a
// type given by the previous type total.

void CInitialState::AddDPDBeadType(long oldType)
{
	// Add the original bead type's interactions to the end of
	// each row and create a new row at the end of the matrix

	for(long unsigned int row = 0; row < m_vvConsInt.size(); row++)
	{
		double oldConsInt = m_vvConsInt.at(row).at(oldType);
		double oldDissInt = m_vvDissInt.at(row).at(oldType);

		m_vvConsInt.at(row).push_back(oldConsInt);
		m_vvDissInt.at(row).push_back(oldDissInt);
	}

	// Copy the row corresponding to the original type to the
	// bottom of the matrix to create the new type's row

	m_vvConsInt.push_back(m_vvConsInt.at(oldType));
	m_vvDissInt.push_back(m_vvDissInt.at(oldType));

}

// Function to add the MD Lennard-Jones and Soft-Core interactions
// for a dynamically-created bead type.
//

void CInitialState::AddMDBeadType(long oldType)
{
	// NOT IMPLEMENTED YET
}

// Function to dynamically add a bond type to the simulation. It is 
// usually called as the result of a command such as 
// ctPolymerisePolymerTripleInTarget. 
//
// Note that it only adds a template bond to the bond "types": it does 
// not add bonds that are used in the simulation's evolution. These must 
// be created and added separately using the AddPolymerisedBond() and 
// AddPolymerisedBondToAllBonds() functions above.

void CInitialState::AddBondType(CBond * const pBond)
{
	vBondTypes.push_back(pBond);
}

void CInitialState::AddBondPairType(CBondPair * const pBondPair)
{
	vBondPairTypes.push_back(pBondPair);
}

void CInitialState::AddPolymerType(CPolymer * const pPolymer)
{
	vPolymerTypes.push_back(pPolymer);
}

// Functions to return the numbers of beads, bonds, bondpairs and polymers
// of each type. We include range checking for security here.

long CInitialState::GetBeadTotalForType(long type) const
{
	if(type >= 0 && type < static_cast<long>(m_vBeadTypeTotal.size()))
	{
		return m_vBeadTypeTotal.at(type);
	}
	else
		return 0;
}

long CInitialState::GetBondTotalForType(long type) const
{
	if(type >= 0 && type < static_cast<long>(m_vBondTypeTotal.size()))
	{
		return m_vBondTypeTotal.at(type);
	}
	else
		return 0;
}

long CInitialState::GetBondPairTotalForType(long type) const
{
	if(type >= 0 && type < static_cast<long>(m_vBondPairTypeTotal.size()))
	{
		return m_vBondPairTypeTotal.at(type);
	}
	else
		return 0;
}

long CInitialState::GetPolymerTotalForType(long type) const
{
	if(type >= 0 && type < static_cast<long>(m_vPolymerTypeTotal.size()))
	{
		return m_vPolymerTypeTotal.at(type);	
	}
	else
		return 0;
}

// Function to return the size of a specified polymer type.
// If the specified type does not exist, we return 0.

long CInitialState::GetPolymerSizeForType(long type) const
{
	if(type >= 0 && type < static_cast<long>(m_mPolymerSizes.size()))
	{
		cLongLongIterator iterPoly = m_mPolymerSizes.find(type);

		return (*iterPoly).second;
	}
	else
		return 0;
}


// Function to return the number fraction of a specified polymer type.
// If the specified type does not exist, we return 0.

double CInitialState::GetPolymerFractionForType(long type) const
{
	if(type >= 0 && type < static_cast<long>(m_mPolymerFractions.size()))
	{
		cLongDoubleIterator iterPoly = m_mPolymerFractions.find(type);
		return (*iterPoly).second;
	}
	else
		return 0.0;
}

// ****************************************
// Implementation of the ISerialiseInclusiveRestartState interface that
// handles read/write of this class' data. The data includes structures, 
// such as the DPD bead-bead conservative and dissipative interaction parameters, 
// that can be modified by command during  a run, and which therefore may be 
// different from their values in the CDF. The restarted run can thus recreate the
// dynamic state of the old run in its initial state.

// Function to read data from a restart state file and overwrite the default values
// of all parameters from the restarted run. This allows command targets to be used
// to modify bead, bond, bondpair properties.
// The order of data reading is the following:
//
// Bead type and radius
// Cons parameter aij
// Diss parameter gij
// Bond type, spring constant and unstretched length
// Bondpair type, bending constant and preferred angle (radians)
// Dynamically-created polymerised bonds
// Command targets (bead, polymer and composite targets only as at 23/7/07)
//
// We use the IInclusiveRestartState interface to the CSimState to use the 
// existing framework for command handlers to create new types.  
//
//
//

zInStream& CInitialState::Read(zInStream& is)
{
    // Bead data first: note that the bead types and names should NOT be modified 
    // by hand, but the radius and force parameters can be.

    long jb, type, newTypes;
    double radius, cons, diss;
    zString name;  // Used to hold the string labels for beads, bonds, bondpairs,
    //                polymers, and targets

    // Store the number of bead types defined in the CDF so that we can update
    // their properties depending if new types have been created.

    const long oldTypes = vBeadTypes.size();
    is >> newTypes;

    if(!is.good() || newTypes < 0)
    {
        SetRestartStateInvalid("Restart state error: invalid number of bead types");
        return is;
    }
//    std::cout << "Reading data for " << newTypes << " bead types" << zEndl;

    // First update the properties of all bead types that were defined in the CDF.
    // Note that we have to add their bead-bead interaction parameters with the 
    // new types as well as the old as the data occurs on one line.

	for(cBeadVectorIterator iterBead=vBeadTypes.begin(); iterBead!=vBeadTypes.end(); iterBead++)
	{
	    is >> type >> name >> radius;
        if(!is.good() || type < 0 || name.empty() || radius < 0.0)
        {
            SetRestartStateInvalid("Restart state error: invalid existing bead type parameters");
            return is;
        }

//        std::cout << "setting Bead old/new type " << (*iterBead)->GetType() << " " << type << " radius to " << radius << zEndl;
        (*iterBead)->SetRadius(radius);

#if SimIdentifier == BD

//		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
//		{
//			is >> cons;
//          m_vvConsInt.at(type).at(j) = cons;
//		}
//		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
//		{
//			is >> diss;
//          m_vvDissInt.at(type).at(j) = diss;
//		}

#elif SimIdentifier == DPD

        // First, read in the conservative interactions with existing types
		for(long j=0; j<oldTypes; j++)
		{
		  is >> cons;
          if(!is.good())
          {
            SetRestartStateInvalid("Restart state error: invalid bead-bead conservative parameter");
            return is;
          }
          m_vvConsInt.at(type).at(j) = cons;
//          std::cout << "setting Bead type " << type << " cons int with type " << j << " to " << cons << zEndl;
		}

        // Now append the interactions with the new types
		for(long j=oldTypes; j<newTypes; j++)
		{
		  is >> cons;
          if(!is.good())
          {
            SetRestartStateInvalid("Restart state error: invalid bead-bead conservative parameter");
            return is;
          }
          m_vvConsInt.at(type).push_back(cons);
//          std::cout << "adding Bead type " << type << " cons int with type " << j << " to " << cons << zEndl;
		}

        // Now do the same for the dissipative interactions
		for(long j=0; j<oldTypes; j++)
		{
		  is >> diss;
          if(!is.good() || diss < 0.0)
          {
            SetRestartStateInvalid("Restart state error: invalid bead-bead dissipative parameter");
            return is;
          }
          m_vvDissInt.at(type).at(j) = diss;
//          std::cout << "setting Bead type " << type << " diss int with type " << j << " to " << diss << zEndl;
		}

        // Now append the interactions with the new types
		for(long j=oldTypes; j<newTypes; j++)
		{
		  is >> diss;
          if(!is.good() || diss < 0.0)
          {
            SetRestartStateInvalid("Restart state error: invalid bead-bead dissipative parameter");
            return is;
          }
          m_vvDissInt.at(type).push_back(diss);
//          std::cout << "adding Bead type " << type << " diss int with type " << j << " to " << diss << zEndl;
		}

#elif SimIdentifier == MD

//		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
//		{
//			os << m_vvLJDepth.at((*iterBead)->GetType()).at(j) << " ";
//		}
//		os << zEndl;
//		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
//		{
//			os << m_vvLJRange.at((*iterBead)->GetType()).at(j) << " ";
//		}
//		os << zEndl;

//		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
//		{
//			os << m_vvSCDepth.at((*iterBead)->GetType()).at(j) << " ";
//		}
//		os << zEndl;
//		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
//		{
//			os << m_vvSCRange.at((*iterBead)->GetType()).at(j) << " ";
//		}
//		os << zEndl;
#endif
	}

    // Now add new bead types if any are required

    if(newTypes != oldTypes)
    {
        for(jb=oldTypes; jb<newTypes; jb++)
        {
	        is >> type >> name >> radius;
            if(!is.good() || type < 0 || name.empty() || radius < 0.0)
            {
                SetRestartStateInvalid("Restart state error: invalid new bead type parameters");
                return is;
            }
//            std::cout << "Creating new Bead type " << type << " " << name << " with radius " << radius << zEndl;

            m_pIRS->AddBeadType(type, name);
            (vBeadTypes.at(jb))->SetRadius(radius);

    #if SimIdentifier == BD

    //		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
    //		{
    //			is >> cons;
    //          m_vvConsInt.at(type).at(j) = cons;
    //		}
    //		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
    //		{
    //			is >> diss;
    //          m_vvDissInt.at(type).at(j) = diss;
    //		}

    #elif SimIdentifier == DPD

            // Conservative and dissipative interaction parameters have already
            // been added for the new types, so we just have to overwrite them.

		    for(long j=0; j<newTypes; j++)
		    {
		      is >> cons;
              if(!is.good())
              {
                SetRestartStateInvalid("Restart state error: invalid bead-bead conservative parameter");
                return is;
              }
              m_vvConsInt.at(type).at(j) = cons;
//              std::cout << "setting Bead type " << type << " cons int with type " << j << " to " << cons << zEndl;
		    }
		    for(long j=0; j<newTypes; j++)
		    {
		      is >> diss;
              if(!is.good() || diss < 0.0)
              {
                SetRestartStateInvalid("Restart state error: invalid bead-bead dissipative parameter");
                return is;
              }
              m_vvDissInt.at(type).at(j) = diss;
//              std::cout << "setting Bead type " << type << " diss int with type " << j << " to " << diss << zEndl;
		    }

    #elif SimIdentifier == MD

    //		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
    //		{
    //			os << m_vvLJDepth.at((*iterBead)->GetType()).at(j) << " ";
    //		}
    //		os << zEndl;
    //		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
    //		{
    //			os << m_vvLJRange.at((*iterBead)->GetType()).at(j) << " ";
    //		}
    //		os << zEndl;

    //		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
    //		{
    //			os << m_vvSCDepth.at((*iterBead)->GetType()).at(j) << " ";
    //		}
    //		os << zEndl;
    //		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
    //		{
    //			os << m_vvSCRange.at((*iterBead)->GetType()).at(j) << " ";
    //		}
    //		os << zEndl;
    #endif
        }

    }

	// Copy the new bead-bead interaction parameters to the CCNTCell class 
	// in case the parameters have been modified by the user. This is 
	// redundant if no changes have been made, but it is more efficient 
	// than checking a (potentially-large) set of parameters for differences.
	// Note that the following CNTCell static member functions take pointers 
	// as arguments, and the CInputData functions return pointers.
	// We need to modify the loading of the bead-bead interaction parameters to 
	// allow the inclsuive restart states to modify them if necessary. This
	// constructor is (obviously) called before the Assemble function so we 
	// have overwrite the parameters in the Read() function if they have been 
	// modified.

// NOTE BD and MD not implemented yet

#if SimIdentifier == BD
//		CCNTCell::SetBDBeadStructure(rInputData.GetDPDBeadConsIntArray(),
//									 rInputData.GetDPDBeadDissIntArray());
#elif SimIdentifier == DPD

        m_pIRS->SetDPDBeadIntArrays(&m_vvConsInt, &m_vvDissInt);

#elif SimIdentifier == MD
		CCNTCell::SetMDBeadStructure(rInputData.GetMDBeadLJDepthArray(), 
									 rInputData.GetMDBeadLJRangeArray(),
									 rInputData.GetMDBeadSCDepthArray(), 
								     rInputData.GetMDBeadSCRangeArray());
#endif


    // Now the bond data

    const long oldBondTypes = vBondTypes.size();

    is >> newTypes;
    if(!is.good() || newTypes < 0)
    {
        SetRestartStateInvalid("Error reading restart state: invalid number of bond types");
        return is;
    }
//    std::cout << "Reading data for " << newTypes << " bond types" << zEndl;

    double k2, l0;

 	for(cBondVectorIterator iterBond=vBondTypes.begin(); iterBond!=vBondTypes.end(); iterBond++)
 	{
 		is >> type >> name >> k2 >> l0;   
        if(!is.good() || type < 0 || name.empty() || k2 < 0.0 || l0 < 0.0)
        {
            SetRestartStateInvalid("Error reading restart state: invalid Hookean bond type parameters");
            return is;
        }
        (*iterBond)->SetSpringConstant(k2);
        (*iterBond)->SetUnStretchedLength(l0);
//        std::cout << "setting Bond old/new type " << (*iterBond)->GetType() << " " << type << " " << name << " k2, l0 to " << k2 << " " << l0 << zEndl;
 	}

    // Now add a new template bond (no id) for any dynamically-created bond types that 
    // are not specified in the CDF

    CBond* pPolyBond = 0;

    if(newTypes != oldBondTypes)
    {
        for(jb=oldBondTypes; jb<newTypes; jb++)
        {
 		    is >> type >> name >> k2 >> l0;   
            if(!is.good() || type < 0 || type != jb || name.empty() || k2 < 0.0 || l0 < 0.0)
            {
                SetRestartStateInvalid("Error reading restart state: invalid dynamically-created Hookean bond type parameters");
                return is;
            }

 		    pPolyBond = new CBond(type, k2, l0);

            AddBondType(pPolyBond);
 //           std::cout << "adding new Bond type " << type << " " << name << k2 << " " << l0 << zEndl;
        }
    }

    // Now the bondpair data: note that the preferred angle is in radians here, we don't
    // convert it to degrees as the user is not supposed to manually modify it.

    double k3, phi0;

    is >> newTypes;
    if(!is.good() || newTypes < 0)
    {
        SetRestartStateInvalid("Error reading restart state: invalid number of stiff bond types");
        return is;
    }
//    std::cout << "Reading data for " << newTypes << " bondpair types" << zEndl;

 	for(cBondPairVectorIterator iterBP=vBondPairTypes.begin(); iterBP!=vBondPairTypes.end(); iterBP++)
 	{
 		is >> type >> name >> k3 >> phi0;   
        if(!is.good() || type < 0 || name.empty() || k3 < 0.0 || phi0 < 0.0 || phi0 > xxBase::m_globalPI)
        {
            SetRestartStateInvalid("Error reading restart state: invalid stiff bond type parameters");
            return is;
        }
        (*iterBP)->SetModulus(k3);
        (*iterBP)->SetPhi0(phi0);
//        std::cout << "setting BondPair old/new type " << (*iterBP)->GetType() << " " << type << " " << name << " k3, phi0 to " << k3 << " " << phi0 << zEndl;
 	}

    // Dynamically-created bonds used, for example, to tie polymers together in 
    // rigid targets. Each bond specifies the following data:
    // 
    // Bond type Bond name Spring const Unstretched length  Head index Tail index
    // e.g.  7 Unnamed  256 0.5  123 456
    //
    // We retrieve the actual bead pointers from vAllBeads using the head and tail 
    // indices, which relies upon the fact that a bead's index is its location in the
    // bead container, and create new bond instances. Note that we don't use the 
    // copy constructor as the user may have modified the bond parameters for each 
    // instance in the restart state. The polymerised bonds' ids start at the current
    // bond total.

    is >> newTypes;
    if(!is.good() || newTypes < 0)
    {
        SetRestartStateInvalid("Error reading restart state: invalid number of dynamic, polymerised bonds");
        return is;
    }
//    std::cout << "Reading data for " << newTypes << " polymerised bond types" << zEndl;

    long polyBondCounter = GetBondTotal();
    long headIndex, tailIndex;
    CBead* pPolyBondHead = 0;
    CBead* pPolyBondTail = 0;

    for(jb=0; jb < newTypes; jb++)
    {
        pPolyBondHead = 0;
        pPolyBondTail = 0;

 		is >> type >> name >> k2 >> l0 >> headIndex >> tailIndex;   
        if(!is.good() || type < 0 || name.empty() || k2 < 0.0 || l0 < 0.0 || headIndex < 0 || tailIndex < 0)
        {
            SetRestartStateInvalid("Error reading restart state: invalid dynamic, polymerised bond parameters");
            return is;
        }

        // Get pointers to the bond's head and tail beads and check that they exist
        // (it is possible that the user has changed the indices in the restart file,
        // although this should NOT be done); note the zero-based indexing of the container.

        pPolyBondHead = vAllBeads.at(headIndex-1);
        pPolyBondTail = vAllBeads.at(tailIndex-1);
        if(!pPolyBondHead || !pPolyBondTail || pPolyBondHead->GetId() != headIndex || pPolyBondTail->GetId() != tailIndex)
        {
            SetRestartStateInvalid("Error reading restart state: invalid dynamic, polymerised bond head or tail bead index");
            return is;
        }

 		pPolyBond = new CBond(type, k2, l0);
 		pPolyBond->SetId(polyBondCounter++);
		pPolyBond->SetBeads(pPolyBondHead, pPolyBondTail);
		pPolyBond->SetBeadIndex(pPolyBondHead->GetId(), pPolyBondTail->GetId());
        AddPolymerisedBond(pPolyBond);
        AddPolymerisedBondtoAllBonds(pPolyBond);
    }

    // Now we iterate over the command targets and decorators that have been
    // serialised to the restart state, and instantiate the appropriate classes
    // using the factory class to create each type of target. Then we import each
    // target's required data, and add them to the CSimBox's set of targets.

    // We distinguish targets from decorators by the fact that a target's inner label
    // is always the same as its own label, whereas a decorator's inner label is either
    // a target label or a different decorator label. Then we store targets and decorators
    // in two distinct containers for later copying to the SimBox.

    CCommandTargetNode* pTarget = 0;
    zString targetType;
    long targetTotal = 0;
    is >> targetTotal;
    if(!is.good() || targetTotal < 0)
    {
        SetRestartStateInvalid("Error reading restart state: invalid number of command targets");
        return is;
    }

    long j = 0;
    while(is.good() && j++<targetTotal)
    {
        is >> targetType >> name;
        if(!is.good() || targetType.empty() || name.empty())
        {
            SetRestartStateInvalid("Error reading restart state: invalid target type or name");
            return is;
        }
        else
        {
	        pTarget = acfTargetFactory::Instance()->Create(targetType, name);
            is >> (*pTarget);
            if(!is.good())
            {
                SetRestartStateInvalid("Error reading restart state: unable to read target data for "+ name);
                return is;
            }

            // Add a target to the local targets and  a decorator to the local decorators 
            // preparatory to copying them to the SimBox later. We cannot 
            // do it here as the SimBox does not yet exist. We distinguish targets from
            // decorators by using their inner decorator label: this must be the same
            // as the instance's own label for targets, but different for decorators.

            if(pTarget->GetInnerDecoratorName() == pTarget->GetLabel())
            {
                if(!AddCommandTarget(pTarget))
                {
                    SetRestartStateInvalid("Error reading restart state: unable to add command target (duplicate name?) " + name);
                    return is;
                }
            }
            else
            {
                if(!AddDecorator(pTarget))
                {
                    SetRestartStateInvalid("Error reading restart state: unable to add command target decorator (duplicate name?) " + name);
                    return is;
                }
            }
        }
    }

    // Now that all the targets have been instantiated, we connect them up according
    // to their inner and outer decorators and replace their set of proxy ids with 
    // actual entities (beads, polymers, active bonds, command targets (for composite
    // command targets) etc). If a required decorator is missing, we abort  the run.

    // Loop over each target, and replace the proxy name for each decorator with a pointer
    // to the actual target. We use the GetCommandTarget() helper function to retrieve
    // a target from the local store given its name.

    bool bTargetConnect = true;
    CommandTargetIterator iterTarget=m_CommandTargets.begin(); 

    while(bTargetConnect && iterTarget!=m_CommandTargets.end())
    {
        if(!ConnectTargetInnerOuterDecorators(*iterTarget))
        {
            SetRestartStateInvalid("Error reading restart state: invalid target inner/outer decorators");
            bTargetConnect = false;
        }
        else
        {
            (*iterTarget)->ReplaceProxyIds(this);
            iterTarget++;
        }
    }

    return is;
}

// Function to write this class' dynamically-created data to a restart state. 
// The data for a DPD simulation are the following:
//
// InitialState data
// *****************
//  Bead type, name, radius
//  Conservative force parameters (N*N not lower diagonal matrix)
//  Dissipative force parameters
//  Bond type, name, spring constant, unstretched length
//  BondPair type, name, bending constant, preferred angle (radians)
//
//  For BD and MD simulations, some of the bead parameters have a different 
//  interpretation. But the bond and bondpair data are the same.
//
//  Dynamically-created bonds used to bind polymers in targets together
//
//

zOutStream& CInitialState::Write(zOutStream& os) const
{
    // Bead data first: precede the data with the number of types as this can change
    // between runs

    os << zEndl; // Start with a new line
    os << vBeadTypes.size() << zEndl;
	for(cBeadVectorIterator iterBead=vBeadTypes.begin(); iterBead!=vBeadTypes.end(); iterBead++)
	{
	    os << (*iterBead)->GetType() << " " << m_pIRS->GetBeadNameFromType((*iterBead)->GetType()) << " " << (*iterBead)->GetRadius() << zEndl;

#if SimIdentifier == BD

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			os << m_vvConsInt.at((*iterBead)->GetType()).at(j) << " ";
		}
		os << zEndl;
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			os << m_vvDissInt.at((*iterBead)->GetType()).at(j) << " ";
		}
		os << zEndl;

#elif SimIdentifier == DPD

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			os << m_vvConsInt.at((*iterBead)->GetType()).at(j) << " ";
		}
		os << zEndl;
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			os << m_vvDissInt.at((*iterBead)->GetType()).at(j) << " ";
		}
		os << zEndl;

#elif SimIdentifier == MD

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			os << m_vvLJDepth.at((*iterBead)->GetType()).at(j) << " ";
		}
		os << zEndl;
		for(j=0; j<vBeadTypes.size(); j++)
		{
			os << m_vvLJRange.at((*iterBead)->GetType()).at(j) << " ";
		}
		os << zEndl;

		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			os << m_vvSCDepth.at((*iterBead)->GetType()).at(j) << " ";
		}
		os << zEndl;
		for(long unsigned int j=0; j<vBeadTypes.size(); j++)
		{
			os << m_vvSCRange.at((*iterBead)->GetType()).at(j) << " ";
		}
		os << zEndl;
#endif
	}

    // Now the bond data

    os << vBondTypes.size() << zEndl;
 	for(cBondVectorIterator iterBond=vBondTypes.begin(); iterBond!=vBondTypes.end(); iterBond++)
 	{
 		os << (*iterBond)->GetType() << " " << m_pIRS->GetBondNameFromType((*iterBond)->GetType()) << " " << (*iterBond)->GetSprConst() << " " << (*iterBond)->GetUnStrLength() << zEndl;
 	}

    // Now the bondpair data

    os << vBondPairTypes.size() << zEndl;
 	for(cBondPairVectorIterator iterBP=vBondPairTypes.begin(); iterBP!=vBondPairTypes.end(); iterBP++)
 	{
 		os << (*iterBP)->GetType() << " " << m_pIRS->GetBondPairNameFromType((*iterBP)->GetType()) << " " << (*iterBP)->GetModulus() << " " << (*iterBP)->GetPhi0() << zEndl;
 	}

    // Dynamically-created bonds used, for example, to tie polymers together in rigid targets

    os << vAllPolymerisedBonds.size() << zEndl;
 	for(cBondVectorIterator iterPolyBond=vAllPolymerisedBonds.begin(); iterPolyBond!=vAllPolymerisedBonds.end(); iterPolyBond++)
 	{
 		os << (*iterPolyBond)->GetType() << " " << m_pIRS->GetBondNameFromType((*iterPolyBond)->GetType()) << " " << (*iterPolyBond)->GetSprConst() << " " << (*iterPolyBond)->GetUnStrLength() << " " << (*iterPolyBond)->GetHeadIndex() << " " << (*iterPolyBond)->GetTailIndex() << zEndl;
 	}

    return os;
}

// Function to add a command target to the local store. It is used when importing
// a restart state that contains targets. If the target name has already been used,
// the new target is rejected.

bool CInitialState::AddCommandTarget(CCommandTargetNode* const pTarget)
{
	bool bExists = false;

	for(cCommandTargetIterator cIterTarget=m_CommandTargets.begin(); cIterTarget!=m_CommandTargets.end(); cIterTarget++)
	{
		if((*cIterTarget)->GetLabel() == pTarget->GetLabel())
			bExists = true;
	}

	if(!bExists)
	{
		m_CommandTargets.push_back(pTarget);
		return true;
	}
	else
	{
		return false;
	}
}

// Function to add a decorator instance to the local store. It is used when importing
// a restart state that contains targets and decorators. If the decorator name has
// already been used, the new one is rejected.

bool CInitialState::AddDecorator(CCommandTargetNode* const pTarget)
{
	bool bExists = false;

	for(cCommandTargetIterator cIterTarget=m_Decorators.begin(); cIterTarget!=m_Decorators.end(); cIterTarget++)
	{
		if((*cIterTarget)->GetLabel() == pTarget->GetLabel())
			bExists = true;
	}

	if(!bExists)
	{
		m_Decorators.push_back(pTarget);
		return true;
	}
	else
	{
		return false;
	}
}

// Function to return a pointer to a command target given its name.

CCommandTargetNode* CInitialState::GetCommandTarget(const zString name)
{
    CCommandTargetNode* pTarget = 0;

    CommandTargetIterator iterTarget = m_CommandTargets.begin();
	bool bFound = false;

	while(!bFound && iterTarget!=m_CommandTargets.end())
	{
		if((*iterTarget)->GetLabel() == name)
        {
			bFound = true;
            pTarget = *iterTarget;
        }
        else
        {
            iterTarget++;
        }
	}

    return pTarget;
}

// Function to return a pointer to a command target decorator given its name.

CCommandTargetNode* CInitialState::GetDecorator(const zString name)
{
    CCommandTargetNode* pTarget = 0;

    CommandTargetIterator iterTarget = m_Decorators.begin();
	bool bFound = false;

	while(!bFound && iterTarget!=m_Decorators.end())
	{
		if((*iterTarget)->GetLabel() == name)
        {
			bFound = true;
            pTarget = *iterTarget;
        }
        else
        {
            iterTarget++;
        }
	}

    return pTarget;
}

// Function to return the whole set of locally-stored command targets.

CommandTargetSequence CInitialState::GetCommandTargets()
{
    return m_CommandTargets;
}

// Function to return the whole set of locally-stored decorators.

CommandTargetSequence CInitialState::GetDecorators()
{
    return m_Decorators;
}

// Function to connect a target or decorator with its inner and outer decorators 
// using their names to retrieve them from the locally-stored targets. This function
// should only be explicitly called on a set of command targets as it makes 
// recursive calls to connect up all decorators that wrap a target. 

bool CInitialState::ConnectTargetInnerOuterDecorators(CCommandTargetNode* const pTarget)
{
    CCommandTargetNode* pDec = 0;

    const zString innerName = pTarget->GetInnerDecoratorName();
    const zString outerName = pTarget->GetOuterDecoratorName();

    if(outerName != pTarget->GetLabel()) // an outer decorator exists and must be connected
    {
        pDec = GetDecorator(outerName);
        if(pDec)
        {
            pTarget->SetOuterDecorator(pDec);
            pDec->SetInnerDecorator(pTarget);
            return ConnectTargetInnerOuterDecorators(pDec);
        }
        else
        {
            return false;
        }
    }

    return true;
}

// Function to set all beads of the specified type invisible. To allow for future
// extensions, we do not get a block of beads but set each bead instance's flag 
// individually.

void CInitialState::SetBeadTypeInvisible(long type)
{
    for(BeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
    {
        if((*iterBead)->GetType() == type)
        {
            (*iterBead)->SetInvisible();
        }
    }
}

void CInitialState::SetBeadTypeVisible(long type)
{
    for(BeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
    {
        if((*iterBead)->GetType() == type)
        {
            (*iterBead)->SetVisible();
        }
    }
}

// Function to return a flag showing if the density-dependent DPD force is included.
// If the feature is not enabled, we return false.

bool CInitialState::IsDPDLG() const
{
#if EnableDPDLG == ExperimentEnabled
    return m_bDPDLG;
#elif EnableDPDLG == ExperimentDisabled
    return false;
#endif
}

// Functions to return the total number of beads/bonds/bondpairs/polymers created on ALL processors. 
// These only differ from the equivalent single-processor functions when running a parallel simulation 
// on more than one processor. Because all processors may not have the same number of entities of a given type,
// we poll them to get the actual number.

long CInitialState::GetAllProcessorBeadTotal() const
{
    long total = 0;
	
#if EnableParallelSimBox == SimMPSEnabled
	pmInteger totalMsg;
	pmInteger totalMsgReturn;
	totalMsg.SetTag(1);	
	totalMsgReturn.SetTag(2);

	if(IsProcessZero())
	{
	    total = GetBeadTotal();  // start with number of entities on P0
		for(long pid=1; pid<xxParallelBase::GlobalGetWorld(); pid++)
		{
			totalMsg.Receive();
			total += totalMsg.GetValue();
		}
		
		totalMsgReturn.SetMessageData(total);
		totalMsgReturn.SendAllP();
	}
	else
	{
		totalMsg.SetMessageData(GetBeadTotal());
		totalMsg.SendP0();
		
		totalMsgReturn.Receive();
		total = totalMsgReturn.GetValue();
	}
#else
	total = GetBeadTotal();
#endif
	
    return total;
}

long CInitialState::GetAllProcessorBondTotal() const
{
    long total = 0;
	
#if EnableParallelSimBox == SimMPSEnabled
	pmInteger totalMsg;
	pmInteger totalMsgReturn;
	totalMsg.SetTag(1);	
	totalMsgReturn.SetTag(2);

	if(IsProcessZero())
	{
	    total = GetBondTotal();  // start with number of entities on P0
		for(long pid=1; pid<xxParallelBase::GlobalGetWorld(); pid++)
		{
			totalMsg.Receive();
			total += totalMsg.GetValue();
		}
		
		totalMsgReturn.SetMessageData(total);
		totalMsgReturn.SendAllP();
	}
	else
	{
		totalMsg.SetMessageData(GetBondTotal());
		totalMsg.SendP0();
		
		totalMsgReturn.Receive();
		total = totalMsgReturn.GetValue();
	}
#else
	total = GetBondTotal();
#endif
	
    return total;
}

long CInitialState::GetAllProcessorBondPairTotal() const
{
    long total = 0;
	
#if EnableParallelSimBox == SimMPSEnabled	
	pmInteger totalMsg;
	pmInteger totalMsgReturn;
	totalMsg.SetTag(1);	
	totalMsgReturn.SetTag(2);

	if(IsProcessZero())
	{
	    total = GetBondPairTotal();  // start with number of entities on P0
		for(long pid=1; pid<xxParallelBase::GlobalGetWorld(); pid++)
		{
			totalMsg.Receive();
			total += totalMsg.GetValue();
		}
		
		totalMsgReturn.SetMessageData(total);
		totalMsgReturn.SendAllP();
	}
	else
	{
		totalMsg.SetMessageData(GetBondPairTotal());
		totalMsg.SendP0();
		
		totalMsgReturn.Receive();
		total = totalMsgReturn.GetValue();
	}
#else
	total = GetBondPairTotal();
#endif
	
    return total;
}

long CInitialState::GetAllProcessorPolymerTotal() const
{
    long total = 0;
	
#if EnableParallelSimBox == SimMPSEnabled
	pmInteger totalMsg;
	pmInteger totalMsgReturn;
	totalMsg.SetTag(1);	
	totalMsgReturn.SetTag(2);

	if(IsProcessZero())
	{
	    total = GetPolymerTotal();  // start with number of entities on P0
		for(long pid=1; pid<xxParallelBase::GlobalGetWorld(); pid++)
		{
			totalMsg.Receive();
			total += totalMsg.GetValue();
		}
		
		totalMsgReturn.SetMessageData(total);
		totalMsgReturn.SendAllP();
	}
	else
	{
		totalMsg.SetMessageData(GetPolymerTotal());
		totalMsg.SendP0();
		
		totalMsgReturn.Receive();
		total = totalMsgReturn.GetValue();
	}
#else
	total = GetPolymerTotal();
#endif
	
    return total;
}

// The following functions return 0 if the type is invalid.

long CInitialState::GetAllProcessorBeadTotalForType(long type) const
{
    long total = 0;
	
#if EnableParallelSimBox == SimMPSEnabled	
	pmInteger totalMsg;
	pmInteger totalMsgReturn;
	totalMsg.SetTag(1);	
	totalMsgReturn.SetTag(2);

	if(IsProcessZero())
	{
	    total = GetBeadTotalForType(type);  // start with number of entities on P0
		for(long pid=1; pid<xxParallelBase::GlobalGetWorld(); pid++)
		{
			totalMsg.Receive();
			total += totalMsg.GetValue();
		}
		
		totalMsgReturn.SetMessageData(total);
		totalMsgReturn.SendAllP();
	}
	else
	{
		totalMsg.SetMessageData(GetBeadTotalForType(type));
		totalMsg.SendP0();
		
		totalMsgReturn.Receive();
		total = totalMsgReturn.GetValue();
	}
#else
	total = GetBeadTotalForType(type);
#endif
	
    return total;
}

long CInitialState::GetAllProcessorBondTotalForType(long type) const
{
    long total = 0;
	
#if EnableParallelSimBox == SimMPSEnabled
	pmInteger totalMsg;
	pmInteger totalMsgReturn;
	totalMsg.SetTag(1);	
	totalMsgReturn.SetTag(2);

	if(IsProcessZero())
	{
	    total = GetBondTotalForType(type);  // start with number of entities on P0
		for(long pid=1; pid<xxParallelBase::GlobalGetWorld(); pid++)
		{
			totalMsg.Receive();
			total += totalMsg.GetValue();
		}
		
		totalMsgReturn.SetMessageData(total);
		totalMsgReturn.SendAllP();
	}
	else
	{
		totalMsg.SetMessageData(GetBondTotalForType(type));
		totalMsg.SendP0();
		
		totalMsgReturn.Receive();
		total = totalMsgReturn.GetValue();
	}
#else
	total = GetBondTotalForType(type);
#endif
	
    return total;
}

long CInitialState::GetAllProcessorBondPairTotalForType(long type) const
{
    long total = 0;
	
#if EnableParallelSimBox == SimMPSEnabled
	pmInteger totalMsg;
	pmInteger totalMsgReturn;
	totalMsg.SetTag(1);	
	totalMsgReturn.SetTag(2);

	if(IsProcessZero())
	{
	    total = GetBondPairTotalForType(type);  // start with number of entities on P0
		for(long pid=1; pid<xxParallelBase::GlobalGetWorld(); pid++)
		{
			totalMsg.Receive();
			total += totalMsg.GetValue();
		}
		
		totalMsgReturn.SetMessageData(total);
		totalMsgReturn.SendAllP();
	}
	else
	{
		totalMsg.SetMessageData(GetBondPairTotalForType(type));
		totalMsg.SendP0();
		
		totalMsgReturn.Receive();
		total = totalMsgReturn.GetValue();
	}
#else
	total = GetBondPairTotalForType(type);
#endif
	
    return total;
}

long CInitialState::GetAllProcessorPolymerTotalForType(long type) const
{
    long total = 0;
	
#if EnableParallelSimBox == SimMPSEnabled
	pmInteger totalMsg;
	pmInteger totalMsgReturn;
	totalMsg.SetTag(1);	
	totalMsgReturn.SetTag(2);

	if(IsProcessZero())
	{
	    total = GetPolymerTotalForType(type);  // start with number of entities on P0
		for(long pid=1; pid<xxParallelBase::GlobalGetWorld(); pid++)
		{
			totalMsg.Receive();
			total += totalMsg.GetValue();
		}
		
		totalMsgReturn.SetMessageData(total);
		totalMsgReturn.SendAllP();
	}
	else
	{
		totalMsg.SetMessageData(GetPolymerTotalForType(type));
		totalMsg.SendP0();
		
		totalMsgReturn.Receive();
		total = totalMsgReturn.GetValue();
	}
#else
	total = GetPolymerTotalForType(type);
#endif
	
    return total;
}

// Functions to return the coordinates of the origin of the SimBox for
// each processor in the whole simulation Space. For a serial simulation, 
// this is just (0,0,0). For a parallel simulation, the origin of each 
// processor depends on its rank and the number of processors assigned to each
// dimension. Note that this assignment is also defined in the mpsSimBox class, 
// and any changes to either formula must be made in both classes.

double CInitialState::GetSimBoxXOrigin() const
{
#if EnableParallelSimBox == SimMPSEnabled
    return m_pParallelIS->GetSimBoxXOrigin();
#else
    return 0.0;
#endif
}

double CInitialState::GetSimBoxYOrigin() const
{
#if EnableParallelSimBox == SimMPSEnabled
    return m_pParallelIS->GetSimBoxYOrigin();
#else
    return 0.0;
#endif
}

double CInitialState::GetSimBoxZOrigin() const
{
#if EnableParallelSimBox == SimMPSEnabled
    return m_pParallelIS->GetSimBoxZOrigin();
#else
    return 0.0;
#endif
}
