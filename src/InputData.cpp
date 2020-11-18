/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// InputData.cpp: implementation of the CInputData class.
//
//////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "SimAlgorithmFlags.h"
#include "SimStdlibFlags.h"
#include "InitialStateData.h"
#include "InitialStateRandom.h"     // Possible default initial states
#include "InitialStateLamella.h"
#include "InitialStateRestart.h"	// Needed to validate a restart state's data
#include "GravityData.h"
#include "WallData.h"
#include "InputData.h"
#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"
#include "Polymer.h"
#include "BeadType.h"
#include "BondType.h"
#include "BondPairType.h"
#include "PolymerType.h"
#include "InputBDFile.h"
#include "InputDPDFile.h"
#include "pmInputData.h"
#include "pmBeadData.h"
#include "pmBondData.h"
#include "pmBondPairData.h"
#include "pmPolymerData.h"
#include "pmCommandNames.h"
#include "mpmInitialState.h"

#if EnableDPDLG == ExperimentEnabled
#include "InputDPDLGFile.h"
#endif

#include "InputMDFile.h"
#include "BDBeadStructure.h"
#include "DPDBeadStructure.h"
#include "MDBeadStructure.h"

#include "xxProcessObject.h"
#include "xxProcess.h"
#include "AnalysisObject.h"
#include "Analysis.h"
#include "xxEventObject.h"
#include "xxEvent.h"
#include "xxCommandObject.h"
#include "xxCommand.h"

#include "xxParallelBase.h"

// STL using declarations

	typedef std::pair<long, CInputData::LoopTarget*>	idPairLongLoopTarget;
	typedef std::map<long, CInputData::LoopTarget*>::iterator	idLoopIterator;
	using std::find;

// Valid characters that can appear in bead/polymer names, operators and
// elements. Note that names must begin with a letter to distinguish them 
// from numeric multipliers. Also note that the space character must be 
// included in validChars but not in validNameChars.

const zString CInputData::spaceChar		  = " ";
const zString CInputData::branchChar	  = "*";
const zString CInputData::loopChar		  = "/";
const zString CInputData::validChars      = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789*/()-_ ";
const zString CInputData::validNameChars  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
const zString CInputData::validNumOpChars = "0123456789";
const zString CInputData::validSeparatorOpChars = "-_";


// Static function to check that a string identifier contains only 
// alphanumeric characters and begins with a letter.

bool CInputData::GlobalIsExternalNameValid(const zString name)
{
	if(name.empty())
		return false;
	else if((name.find_first_not_of(CInputData::validNameChars) < name.length()) ||
		    (name.find_first_of(CInputData::validNumOpChars) == 0) ||
		    (name.find_first_of(CInputData::validSeparatorOpChars) == 0))
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// **********************************************************************
// Simulation data are set to sensible defaults here. This provides
// the basis for a typical run unless the user modifies the parameters.
// The exceptions are the bead, bond and polymer types which are set
// in the WriteDefaultControlDataFile() routine.
//
// The dimension of the simulation is set using the "SimDimension" 
// parameter in the include file "SimDefs.h". This affects the values  
// of the arrays and vectors relating to the CSimBox and CMonitor objects.
// But the xxState-derived objects do not need to know about the dimension.
//
//
// The pointer to the CInitialStateData-derived object is NULL until one of 
// WriteDefaultControlDataFile() or ReadControlDataFile() are called when
// the object is created to hold the default initial state parameters or
// the newly-read initial state.
//
// A wall may be specified around the simulation box in 1, 2 or 3 dimensions.
// An empty CWallData object is created and if the user specifies a wall its
// data are filled with the user-defined values. the CInitialState object
// reads the wall data and acts on it only if it is non-null.
//
// We include a density-dependent bead-bead interaction for liquid-gas systems
// if the input data file indicates it is being used. A boolean flag is read from
// the file reader class and stored locally for use in determining which
// bead-bead parameters to read or write. Note that the default CDF is still
// a standard DPD one: the user has to modify it manually to convert it to
// a liquid-gas one.
//
// **********************************************************************

CInputData::CInputData(const zString runid) : runId(runid),
											  m_pISD(0),
                                              Title(""), Date(""), Comment(""),
                                              AverageBeadDensity(0.0),
                                              Lambda(0.0),
                                              RCutOff(0.0),
                                              MCStepSize(0.0),
                                              kT(0.0),
                                              StepSize(0.0),
                                              CNTXCellWidth(0.0),
                                              CNTYCellWidth(0.0),
                                              CNTZCellWidth(0.0),
                                              AnalysisPeriod(0),
                                              DensityPeriod(0),
                                              DisplayPeriod(0),
                                              RestartPeriod(0),
                                              SamplePeriod(0),
                                              GridXCellNo(0),
                                              GridYCellNo(0),
                                              GridZCellNo(0),
                                              ProcXNo(0),
                                              ProcYNo(0),
                                              ProcZNo(0),
                                              CNTXCellNo(0),
                                              CNTYCellNo(0),
                                              CNTZCellNo(0),
                                              CurrentTime(0),
                                              TotalMCTime(0),
                                              TotalTime(0),
                                              RNGSeed(0),
                                              GravityXForce(0.0),
                                              GravityYForce(0.0),
                                              GravityZForce(0.0),
                                              WallPolymerName(""),
                                              WallBeadDensity(0.0),
                                              WallXWidth(0),
                                              WallYWidth(0),
                                              WallZWidth(0),
                                              m_bLG(false),
											  m_bGravity(false),
											  m_bWall(false),
											  m_bShear(false),
											  m_bBondTypeMissing(false),
                                              m_ExpectedBeadTypeTotal(0),
                                              m_ExpectedBondTypeTotal(0),
                                              m_ExpectedBondPairTypeTotal(0),
                                              m_ExpectedPolymerTypeTotal(0)
{
	// Make sure all maps and containers are empty and resize them if desired

	mBeadNames.clear();
	mBondNames.clear();
	mBondPairNames.clear();
	mPolymerNames.clear();

	m_mPolymerShapes.clear();
	m_mPolymerSizes.clear();
	m_mPolymerFractions.clear();
	m_mBondHeadTypes.clear();
	m_mBondTailTypes.clear();
	m_mBondPairHeadTypes.clear();
	m_mBondPairMiddleTypes.clear();
	m_mBondPairTailTypes.clear();

	mBeadTypes.clear();
	mPolymerTypes.clear();

	vBeadTypes.reserve(100);			// Guess not more than 100 bead, bond, bondpair and polymer types.
	vBondTypes.reserve(100);
	vBondPairTypes.reserve(100);
	vPolymerTypes.reserve(100);

    m_vvConsInt.clear();
    m_vvDissInt.clear();
    m_vvLGInt.clear();

    //Container of bead type instances that is only used on PN

    m_vBeadTypesPN.clear();

	// Specify the default initial state for each simulation type

#if SimIdentifier == BD
	m_pISD = new CInitialStateRandom;
#elif SimIdentifier == DPD
	m_pISD = new CInitialStateLamella;
#elif SimIdentifier == MD
	m_pISD = new CInitialStateLamella;
#endif


#if SimMPS == SimulationEnabled
    // Define a default number of processors if running a parallel job.
    ProcXNo = 1;
    ProcYNo = 1;
    ProcZNo = 1;
#endif

	// Default bead, bond and polymer types are defined in 
	// CInputData::WriteDefaultControlDataFile()

	// CNT cell sizes and analysis grid size 

	CNTXCellNo		= 32;
	CNTXCellWidth	= 1.0;

	CNTYCellNo		= 32;
	CNTYCellWidth	= 1.0;

	GridXCellNo		= 10;
	GridYCellNo		= 10;

#if SimDimension == 2
	CNTZCellNo		= 1;
	CNTZCellWidth	= 0.0;
	GridZCellNo		= 1
#elif SimDimension == 3
	CNTZCellNo		= 32;
	CNTZCellWidth	= 1.0;
	GridZCellNo		= 128;
#endif

	// Physical parameters and others: note that parameters whose
    // values depend on the type of simulation are initialised below
    // in a conditional compilation block.

	RNGSeed				= -60112;
	CurrentTime			= 0;
	TotalTime			= 20;

	SamplePeriod		= 1;
	AnalysisPeriod		= 10;
	DensityPeriod		= 20;
	DisplayPeriod		= 20;
	RestartPeriod		= 20;

	vGravityBeadNames.clear();
	GravityXForce		= 0.0;
	GravityYForce		= 0.0;
	GravityZForce		= 0.0;

	WallPolymerName		= "";
	WallBeadDensity		= 0.0;
	WallXWidth			= 0;
	WallYWidth			= 0;
	WallZWidth			= 0;

#if SimIdentifier == BD

	// BD-specific parameters or values for common parameters.  Irrelevant
	// parameters have been set to zero in the constructor's initialisation list.

	Lambda				= 0.0;
	StepSize			= 0.001;
	AverageBeadDensity	= 0.09156; // Creates 1000 actin monomers (A3-M-T3) in a 32**3 box
	kT					= 1.0;

#elif SimIdentifier == DPD

	// DPD-specific parameters.

	Lambda				= 0.5;
	StepSize			= 0.02;
	AverageBeadDensity	= 3.0;
	kT					= 1.0;

#elif SimIdentifier == MD

	// MD-specific parameters.

	Lambda				= 0.0;
	StepSize			= 0.02;
	AverageBeadDensity	= 0.666667;
	kT					= 1.35;
	RCutOff				= 2.5;
	MCStepSize			= 0.1;
	TotalMCTime			= 10000;

#endif

	// Ensure the containers of processes, aggregates, events, commands 
	// and force targets, command targets and command groups are empty 
    // because we use push_back() to add elements. 

	m_vProcesses.clear();	
	m_vAggregates.clear();	
	m_vEvents.clear();	
	m_lCommands.clear();
	m_vForceTargetNames.clear();
	m_vCommandTargetNames.clear();
	m_vACTNames.clear();
    m_vCommandGroupNames.clear();
    m_vCommandNames.clear();

}

// The destruction of the bead, bond, bondpair and polymers "types" is done in
// the CInitialState object even though they are created here. We view these types
// as being part of the initial state specification, the CInputData object just
// interprets the control data file and creates them, but they belong to the
// CInitialState class. Further types may be created during a simulation, for
// example, when an initial state is polymerised, and these new types are created
// by, and stored in, the CInitialState object.
											  
CInputData::~CInputData()
{
	// There must be a CInitialStateData pointer so delete it

	delete m_pISD;
	m_pISD = 0;

	// If any loop targets have been created then delete them

	if(!m_mLoopTargets.empty())
	{
		for(idLoopIterator iterLT = m_mLoopTargets.begin(); iterLT != m_mLoopTargets.end(); iterLT++)
		{
			delete (*iterLT).second;
		}
		m_mLoopTargets.clear();
	}

    // If this is a parallel run, destroy any types that were created when 
    // PN unpacked bead, bond, bondpair and polymer data from P0 messages
    // into local types in the ReceiveXXXData() functions. We don't need to
    // check we are on PN, as P0 will not store any data in these containers.

#if SimMPS == SimulationEnabled

    if(!m_vBeadTypesPN.empty())
    {
        for(BeadTypeVectorIterator iterBead = m_vBeadTypesPN.begin(); iterBead != m_vBeadTypesPN.end(); iterBead++)
        {
            delete *iterBead;
        }
        m_vBeadTypesPN.clear();
    }

#endif
}

// Function to add a new type of bead to the vector of bead templates.
// Notice that the type is implicit in the number of calls to this routine
// from one CInputData object. The id of these bead templates is set to -1
// to indicate that they should not appear in the actual simulation.
//
// We check that bead types do not already occur in the mBeadNames map before
// adding a new bead type, and exit with an error if a duplicate is found.
//
// The internal structure of DPD and MD beads is passed in using a pointer
// to the base class CBeadStructure. We distinguish between simulation types
// using conditional compilation, and dynamically cast the pointer to the
// appropriate class. The interaction parameters are then stripped out and 
// stored locally while the beads are created with just their name and type.
//
// BD Bead Structure
// *****************
// A Brownian Dynamics bead has a translational diffusion coefficient, a 
// rotational diffusion coefficient, and uses the same conservative force
// laws as a DPD bead. Hence, it also has an interaction radius.
//
// DPD Bead Structure
// ******************
// The i,jth element of m_vvConsInt and m_vvDissInt is the interaction of bead type j
// with the ith bead type. This means we have to update each existing bead's
// interaction parameters with those of the new bead every time a new bead
// is added. We use a vector of vector of doubles to hold the data so that
// we don't have to store the interaction constants in every bead.
//
// If the flag UseDPDBeadRadii is set the radius of interaction of the bead type
// is set in the newly-created CBead object.
//	
// MD Bead Structure
// ******************
// Similarly to the DPD beads the depth and range of a Lennard-Jones potential
// are stored in vectors and have to be copied into the currently-existing vectors
// when a new bead type is added.

bool CInputData::AddBeadType(zString name, const CBeadStructure* pStructure)
{
#if SimIdentifier == BD

	const CBDBeadStructure* pBeadStruct = dynamic_cast<const CBDBeadStructure*>(pStructure);

#elif SimIdentifier == DPD

	const CDPDBeadStructure* pBeadStruct = dynamic_cast<const CDPDBeadStructure*>(pStructure);

#elif SimIdentifier == MD

	const CMDBeadStructure* pBeadStruct   = dynamic_cast<const CMDBeadStructure*>(pStructure);

#endif

	if(!IsNameValid(name))
		return false;
	else if(mBeadNames.find(name) != mBeadNames.end())
		return false;
	else
	{
		long BeadTypeTotal = vBeadTypes.size();

		mBeadNames.insert(zPairSL(name,BeadTypeTotal));
		mBeadTypes.insert(zPairLS(BeadTypeTotal,name));

		// Create a template CBead object to hold the data for all beads 
		// of this type.

		CBead* pBead;

		if( vBeadTypes.empty() )	// first bead type defined
		{
			pBead = new CBead(BeadTypeTotal);
			vBeadTypes.push_back(pBead);

#if SimIdentifier == BD

			pBead->SetRadius(pBeadStruct->GetRadius());
		    pBead->SetTransDiff(pBeadStruct->GetTransDiff());
			pBead->SetRotDiff(pBeadStruct->GetRotDiff());
			m_vvConsInt.push_back(pBeadStruct->GetConsInt());
			m_vvDissInt.push_back(pBeadStruct->GetDissInt());

#elif SimIdentifier == DPD

			pBead->SetRadius(pBeadStruct->GetRadius());
			m_vvConsInt.push_back(pBeadStruct->GetConsInt());
			m_vvDissInt.push_back(pBeadStruct->GetDissInt());

#if EnableDPDLG == ExperimentEnabled
            if(IsDPDLG())
            {
			    pBead->SetLGRadius(pBeadStruct->GetLGRadius());
			    m_vvLGInt.push_back(pBeadStruct->GetLGInt());
            }
#endif

#elif SimIdentifier == MD

			m_vvLJDepth.push_back(pBeadStruct->GetLJDepth());
			m_vvLJRange.push_back(pBeadStruct->GetLJRange());
			m_vvSCDepth.push_back(pBeadStruct->GetSCDepth());
			m_vvSCRange.push_back(pBeadStruct->GetSCRange());
#endif
		}
		else	// previous bead types exist so add their interactions with the new bead
		{
			pBead = new CBead(BeadTypeTotal);
			vBeadTypes.push_back(pBead);

			// Update current beads before adding the new type
#if SimIdentifier == BD

			pBead->SetRadius(pBeadStruct->GetRadius());
			pBead->SetTransDiff(pBeadStruct->GetTransDiff());
			pBead->SetRotDiff(pBeadStruct->GetRotDiff());
			for(long beadType=0; beadType<BeadTypeTotal; beadType++)
			{
				m_vvConsInt.at(beadType).push_back((pBeadStruct->GetConsInt()).at(beadType));
				m_vvDissInt.at(beadType).push_back((pBeadStruct->GetDissInt()).at(beadType));
			}
			m_vvConsInt.push_back(pBeadStruct->GetConsInt());
			m_vvDissInt.push_back(pBeadStruct->GetDissInt());

#elif SimIdentifier == DPD

			pBead->SetRadius(pBeadStruct->GetRadius());
			for(long beadType=0; beadType<BeadTypeTotal; beadType++)
			{
				m_vvConsInt.at(beadType).push_back((pBeadStruct->GetConsInt()).at(beadType));
				m_vvDissInt.at(beadType).push_back((pBeadStruct->GetDissInt()).at(beadType));
			}
			m_vvConsInt.push_back(pBeadStruct->GetConsInt());
			m_vvDissInt.push_back(pBeadStruct->GetDissInt());

#if EnableDPDLG == ExperimentEnabled
            if(IsDPDLG())
            {
			    pBead->SetLGRadius(pBeadStruct->GetLGRadius());
			    for(long beadType1=0; beadType1<BeadTypeTotal; beadType1++)
			    {
				    m_vvLGInt.at(beadType1).push_back((pBeadStruct->GetLGInt()).at(beadType1));
			    }
			    m_vvLGInt.push_back(pBeadStruct->GetLGInt());
            }
#endif
#elif SimIdentifier == MD

			for(long beadType=0; beadType<BeadTypeTotal; beadType++)
			{
				m_vvLJDepth.at(beadType).push_back((pBeadStruct->GetLJDepth()).at(beadType));
				m_vvLJRange.at(beadType).push_back((pBeadStruct->GetLJRange()).at(beadType));
				m_vvSCDepth.at(beadType).push_back((pBeadStruct->GetSCDepth()).at(beadType));
				m_vvSCRange.at(beadType).push_back((pBeadStruct->GetSCRange()).at(beadType));
			}
			m_vvLJDepth.push_back(pBeadStruct->GetLJDepth());
			m_vvLJRange.push_back(pBeadStruct->GetLJRange());
			m_vvSCDepth.push_back(pBeadStruct->GetSCDepth());
			m_vvSCRange.push_back(pBeadStruct->GetSCRange());

#endif
		}
	}

	return true;
}

// Function to add a new type of bond to the set. A bond is a simple object
// with just two parameters: its spring constant and unstretched length. 
// The name of a bond is the names of the beads at its ends concatenated.
// The bead names must already have been added to the mBeadNames map; and the
// bond type must not already occur in the bondname map. This prevents the user
// declaring two bonds with the same pair of bead types at their ends.
//
// Note that we add a bond in twice if the beads at its ends are different
// to allow for access with the beads reversed.

bool CInputData::AddBondType(zString name1, zString name2, double spring, double length0)
{
	if( (mBeadNames.find(name1) == mBeadNames.end()) || 
		(mBeadNames.find(name2) == mBeadNames.end()) )
		return false;
	else if(spring < 0.0 || length0 < 0.0)
		return false;
	else if(mBondNames.find(name1+name2) != mBondNames.end())
		return false;
	else
	{
		// Create template CBond objects to hold the data for all bonds. We use
		// a CBond constructor designed to set only the type of the bond, its
		// parameters and its id to -1. The beads and indices into the bead array
		// are set separately in AddBeadtoPolymer().

		CBond* pBond;
		long BondTypeTotal = vBondTypes.size();
		zString bondname;
		bondname = name1 + name2;
		mBondNames.insert(zPairSL(bondname,BondTypeTotal));

		// Store the types of the beads at the bond's head and tail using
		// the bond type as the key

		m_mBondHeadTypes.insert(zPairLL(BondTypeTotal,(*mBeadNames.find(name1)).second));
		m_mBondTailTypes.insert(zPairLL(BondTypeTotal,(*mBeadNames.find(name2)).second));

		// If the beads at each end are distinct add the bond under the reversed
		// name so that it can be found whichever end the beads are read from.

		if(name1 != name2)
		{
			bondname = name2 + name1;
			mBondNames.insert(zPairSL(bondname,BondTypeTotal));
		}

		pBond = new CBond(BondTypeTotal, spring, length0);
		vBondTypes.push_back(pBond);
	}

	return true;
}

// Function to add a new bondpair object to the set. The CBondPair class represents
// 3-body forces between adjacent bonds in a polymer that try to maintain their
// relative angle to the preferred value. The orientation of the beads in the
// CBondPair object is:
//
//    bead1 ----- bead2 ----- bead3
//             
//           bond1       bond2
//
//
// The checks we perform before creating the new CBondPair object are:
//
//		the three beads defining the bonds must exist in the mBeadNames map
//
//		the two bonds defining the CBondPair object must exist in the mBondNames map.
//		these names are simply formed by concatenating the two bead names in the bond.
//
//		the CBondPair object's name must not already be defined in the 
//		mBondTypeNames map. This name is formed by concatenating the names of the 
//		three beads.
//
//		the bending modulus must be positive and the preferred angle postive. Note that
//		we have already checked in the CBondPairType constructor that the angle is
//		positive and less than 180 degrees. We just check for a non-negative value
//		here to avoid having to include the CBuilder header again to access pi.

bool CInputData::AddBondPairType(zString name1, zString name2, zString name3, double Strength, double Phi0)
{
	if( (mBeadNames.find(name1) == mBeadNames.end()) || 
		(mBeadNames.find(name2) == mBeadNames.end()) ||
		(mBeadNames.find(name3) == mBeadNames.end()) )
		return false;
	else if(mBondNames.find(name1+name2) == mBondNames.end())
		return false;
	else if(mBondNames.find(name2+name3) == mBondNames.end())
		return false;
	else if(mBondPairNames.find(name1+name2+name3) != mBondPairNames.end())
		return false;
	else if(Strength < 0.0 || Phi0 < 0.0)
		return false;
	else
	{
		// Create template CBondPair objects. We use a CBondPair constructor 
		// designed to set the type of the bondpair, its parameters: its id is
		// set to -1 to indicate that this is not an active CBondPair object yet.

		CBondPair* pBondPair;
		long BondPairTypeTotal = vBondPairTypes.size();

		zString bondPairName;
		bondPairName = name1 + name2 + name3;
		mBondPairNames.insert(zPairSL(bondPairName,BondPairTypeTotal));

		// Store the types of the beads at the bondpair's head, middle and tail using
		// the bondpair type as the key

		m_mBondPairHeadTypes.insert(zPairLL(BondPairTypeTotal,(*mBeadNames.find(name1)).second));
		m_mBondPairMiddleTypes.insert(zPairLL(BondPairTypeTotal,(*mBeadNames.find(name2)).second));
		m_mBondPairTailTypes.insert(zPairLL(BondPairTypeTotal,(*mBeadNames.find(name3)).second));

		// If the beads at each end are distinct add the bondpair under the reversed
		// name so that it can be found whichever end the beads are read from.

		if(name1 != name3)
		{
			bondPairName = name3 + name2 + name1;
			mBondPairNames.insert(zPairSL(bondPairName,BondPairTypeTotal));
		}

		pBondPair = new CBondPair(BondPairTypeTotal, Strength, Phi0);
		vBondPairTypes.push_back(pBondPair);
	}

	return true;
}

// Function to check that that all the bead names that appear in a polymer
// "shape" have been entered into the mBeadNames map. It assumes that
// the shape has been checked for illegal characters using IsShapeValid().
// It recursively checks that each element is not empty, is not an operator
// and is not compound (has brackets around it) and is a member of the bead
// names map.

bool CInputData::AreAllBeadNamesValid( const zString shape ) const
{
	zString element = CAR(shape);

	bool bValid;

	if(shape == "" || shape == "()")				// empty
		bValid = false;
	else if(element == CInputData::branchChar ||	// branch operator or multiplier
	 	  !(element.find_first_not_of(CInputData::validNumOpChars) < element.length()) )
		bValid = AreAllBeadNamesValid(CDR(shape));
	else if(element == CInputData::loopChar)		// loop operator		
		bValid = AreAllBeadNamesValid(LAST(shape));
	else if(RemoveBrackets(shape) != shape)			// compound element
	{
		if(CDR(shape) == "()")
			bValid = AreAllBeadNamesValid(element);
		else
		{
			if(AreAllBeadNamesValid(element) && AreAllBeadNamesValid(CDR(shape)))
				bValid = true;
			else
				bValid = false;
		}
	}
	else if(!IsBeadinMap(shape) )	// at this point we know we have a non-empty, 
		bValid = false;				// non-operator, non-compound element so we can
	else							// assume it is a bead name
		bValid = true;	
	
	return bValid;
}

// Function to check that that a bead or polymer name is valid.
// A valid name can consist only of an alphabetic character 
// (either case) or a number (listed in the static constant CInputData::validNameChars)
// and must begin with a letter: spaces are definitely excluded.

bool CInputData::IsNameValid( const zString name ) const
{
	if( name.empty() )
		return false;
	else if( (name.find_first_not_of(CInputData::validNameChars) < name.length()) ||
		     (name.find_first_of(CInputData::validNumOpChars) == 0) ||
		     (name.find_first_of(CInputData::validSeparatorOpChars) == 0))
		return false;

	return true;
}

// Function to check that the architecture specified for a polymer is valid.
// A polymer "shape" is valid if it satisfies the conditions:
//
//  It is not empty
//  The first character is "(" and the last is ")"
//  It is not just opening and closing brackets around whitespace
//  The number of left brackets equals the number of right brackets
//  Left and right brackets occur in the correct order (left before right)
//  Only numbers, letters and the key symbols "(", ")", "*", "/" " " occur in it
//  (notice that the space " " must be included as an allowed char).
//  The first element after the opening "(" is not an operator

bool CInputData::IsShapeValid( const zString shape ) const
{
	if( shape.empty() )
		return false;
	else if( shape.find_first_not_of(CInputData::validChars) < shape.length() )
		return false;
	else if( (*shape.begin() != '(') || (*shape.rbegin() != ')') )
		return false;
	else
	{
		// check that there is at least one non-whitespace character between
		// the brackets. We create iterators to the first character after the
		// opening bracket and the last before the closing bracket

		zString::const_iterator iterFirst = shape.begin();
		iterFirst++;
		zString::const_iterator iterLast  = shape.end();
		iterLast--;

		zString::const_iterator iterChar;

		bool bValid = false;
		for(iterChar = iterFirst; iterChar!=iterLast; iterChar++ )
		{
			if(*iterChar != ' ')
				bValid = true;
		}

		if( !bValid )
			return false;

		// count the number of left and right brackets
		// and make sure that they match. Note we exclude the first and
		// last characters. The number of right brackets at any
		// position in the shape must be less than or equal the number of 
		// left brackets.

		long leftbra  = 0;
		long rightbra = 0;

		bValid = true;
		for(iterChar = iterFirst; iterChar!=iterLast; iterChar++ )
		{
			if(*iterChar == '(')
			{			
				leftbra++;
			}
			else if(*iterChar == ')')
			{
				rightbra++;
				if( rightbra > leftbra )
					bValid = false;
			}
		}

		if( !bValid || leftbra != rightbra )
			return false;
	}
	return true;
}

// Function to expand the multiplicative operator in an element of the
// form: (nn token). It returns the sequence of bead names, or elements, 
// resulting from it. Note that token might be a compound element. This function 
// just concatenates "nn" of them, adds surrounding brackets to make them into 
// a valid element again, and returns the result. Also note that 0 as the operator 
// returns a space.

zString CInputData::ExpandElement(zString op, zString token)
{
	zString newString;

	int multi;
	multi = atoi(op.c_str());

	if(multi == 0)		// if multiplier is 0 return a space
		newString = CInputData::spaceChar;
	else if(multi == 1)
		newString = token;
	else
	{
		newString = token;
		for(int j=1; j<multi; j++)
		{
			newString+= ' ';
			newString+= token;
		}		
	}
	newString = '(' + newString + ')';	// add back surrounding brackets

	return newString;
}

// Function to return the head of "oldString". It is named after the
// LISP CAR function.
//
// First it checks for surrounding brackets and if they are there it ignores them,
// but if not it returns "oldString" as is.
// Then it looks for either a second left bracket, indicating a substring with its
// own brackets, or the first non-whitespace character, indicating a token, and 
// when it finds it it stores the position of the character. It loops from 
// this position forwards looking for the terminating character: either a right 
// bracket if the left bracket was found, or a whitespace character OR another
// left bracket if not.
// Once it has the terminating character it returns the characters between the
// first and last positions. Note that if the whitespace is the terminating 
// character then it is not included, but if the right bracket is the terminator 
// it is included.
//
// Examples
//
// CAR( "(A B C)" )			= A
// CAR( "(  A B C)" )		= A
// CAR( "((A) B C)" )		= A
// CAR( "(  (A) B C)" )		= (A)
// CAR( "((2 A) B C)" )		= (2 A)
// CAR( "( (2 A) B C)" )	= (2 A)
// CAR( "(A2) B C)"			= (A2)  NB A2 is a valid token name but 2A is not
// CAR( "(A (* (A B) ) B)"    = 



zString CInputData::CAR(const zString oldString) const
{
	if(!IsShapeValid(oldString))		// check shape is valid first
		return oldString;
	
	char endChar =' ';

	zString::const_iterator iterFirst= oldString.begin();
	iterFirst++;									// ignore opening bracket
	zString::const_iterator iterLast = oldString.end();
	iterLast--;										// ignore closing bracket

	zString::const_iterator iterChar = iterFirst;

	bool bFound = false;
	while(!bFound && iterChar != iterLast)
	{
		if(*iterChar == ' ')
		{
			iterChar++;
		}
		else if(*iterChar == '(')
		{
			bFound = true;
			endChar = ')';	// terminating character is right bracket
		}
		else
		{
			bFound = true;		// first alphanumeric character found
			endChar = ' ';		// terminating character is a space
		}
	}

	if( !bFound)
		return oldString;	// return the empty zString

	// now we have the ending condition and the first character find the 
	// end of the first element in the whole zString and return it. But make
	// sure that we match the number of left and right brackets if ")" is the
	// terminating character.

	int leftBrackets = 0;
	bFound = false;
	zString head = "";

	while(!bFound && iterChar != iterLast)
	{
		if(endChar == ')')
		{
			if(*iterChar == endChar)
			{
				leftBrackets--;
				if(leftBrackets == 0)
					bFound = true;
				head+= *iterChar;			// include endChar
				iterChar++;
			}
			else							// include spaces
			{
				if(*iterChar == '(')		// count left brackets to find matching
					leftBrackets++;			// right bracket to end element
				head+= *iterChar;
				iterChar++;
			}
		}
		else if( endChar == ' ')
		{
			if(*iterChar == endChar || *iterChar == '(')
				bFound = true;			// don't include spaces
			else
			{
				head+= *iterChar;
				iterChar++;
			}
		}
	}

// If bFound is still false this means that there were no spaces after the 
// characters before the end. This is not an error.

	return head;
}

// Function to return a zString with its head removed. It is named after
// the LISP CDR function. See the CAR function for the definition of the "head".
//
// If there is only one element in the zString then we return the empty zString.
// This complements CAR which would return the single element as the head.
// Because we call IsShapeValid() we know that the zString is not empty and
// has at least one non-whitespace character between the opening and closing
// brackets. Hence, it has at least a head or a head and a tail. We only have
// to check if there is a head and a tail.

zString CInputData::CDR(const zString oldString) const
{
	if(!IsShapeValid(oldString))		// check zString is valid first
		return oldString;
	
	zString head = CAR(oldString);
	zString tail	= oldString;
	long startPos;

	startPos = oldString.find(head);
	startPos+= head.length();

	tail = '(' + tail.erase(0,startPos);	// add opening bracket back

	if( !IsShapeValid(tail))
		return "()";

// The Modena string class does not have the erase() member function nor an
// iterator: we have to use remove() to remove the leading white space.


	zStringIterator iterChar=tail.begin();	// remove leading whitespace
	iterChar++;
	while(*iterChar == ' ')
	{
		tail.erase(iterChar);
	}


	return tail;
}

// Function to strip the bounding brackets off a zString if they exist.
// If not it returns the zString unchanged. Note that we do not remove
// whitespace.

zString CInputData::RemoveBrackets(zString oldString) const
{
	if( (*oldString.begin() != '(') || (*oldString.rbegin() != ')') )
		return oldString;
	else
	{
		oldString.erase(oldString.begin());
		zStringIterator iterLast = oldString.end();
		iterLast--;
		oldString.erase(iterLast);
	}

	return oldString;
}

// First overloaded function to construct a polymer type out of a shape.
//
// This version assigns the head and tail beads by default to the first and last
// elements in the shape. If these are not simple bead names an error occurs.
// They can both be the same bead to allow monomers to be handled.
// Other versions of this function allow the head and tail to be assigned to 
// arbitrary, but uniquely named, beads in the shape.
//
// Example
//
// (A B B A)  Head = A, Tail = A, Note that they don't have to have unique names 
//                                as their positions distinguish them
//
// The argument "fraction" is the number fraction of this polymer out of all
// species defined.

bool CInputData::AddPolymerType(zString name, zString shape, double fraction)
{
	if( !IsNameValid(name)			 || !IsShapeValid(shape) ||
		!IsNameValid(FIRST(shape))	 || 
	    (LAST(shape) != "" && !IsNameValid(LAST(shape)))  ||
		!AreAllBeadNamesValid(shape) ||
		!IsElementValid(shape)       ||  (fraction < 0.0) || (fraction > 1.0) )
		return false;
	else if(mPolymerNames.find(name) != mPolymerNames.end())
		return false;
	else
	{
		// Move through the elements in the shape, starting at the first,
		// adding beads and bonds. If an element is a simple bead name 
		// we add it to the polymer's vector of beads (creating a bond 
		// to attach it to the current bead if it is not the first bead), 
		// if it is a compound element we call
		// functions recursively to deal with each element in the compound.
		// They all end up by calling the AddBeadtoPolymer function.
		// For each polymer type we check to see if any CBondPair objects are
		// needed to account for stiff bonds (3-body forces between triplets of
		// beads connected by two adjacent bonds) and add them to the polymer
		// if so.

		BeadVector vNewBeads;
		BondVector vNewBonds;
		BondPairVector vNewBondPairs;

		// Note that current_element is not used in AddBeadtoPolymer() if we are
		// processing the first bead, and it is initialised if we are not.
		// Also the first bead must be a simple bead name in this version
		// of AddPolymerType(). The flag bLinear is used to indicate if the 
		// polymer has any side branches or not.

		zString current_element; 
		zString next_element; 

		long currentBeadIndex = -1;
		bool bLinear = true;
		zString tail = shape;

		while(tail != "()")
		{
			next_element = CAR(tail);

			// if next_element is a bead's name then add it directly to the polymer, 
			// otherwise parse the element looking for brackets or an operator at its 
			// head. The currentBeadIndex keeps track of which bead the next bond will
			// be added to. Because of side branches and loops this is not necessarily
			// the last bead that was added. We indicate whether to move the current bead
			// index by the last argument
			
			if(IsNameValid(next_element))
			{
				current_element = AddBeadtoPolymer(vNewBeads, vNewBonds, current_element, next_element, &currentBeadIndex, true);
			}
			else	// we have a compound element so parse it recursively
			{
				current_element = ParseElement(bLinear, vNewBeads, vNewBonds, current_element, next_element, &currentBeadIndex, true);
			}
			tail = CDR(tail);
		}

		// The new polymer type has been defined so create a new type and add 
		// it to the vector of polymer types passing the bead, and bondpair vectors, 
		// as arguments to its constructor. We also store the polymer shapes using
		// their type as the key into a map.

		long PolymerTypeTotal = vPolymerTypes.size();

		mPolymerNames.insert(zPairSL(name,PolymerTypeTotal));
		mPolymerTypes.insert(zPairLS(PolymerTypeTotal,name));
		m_mPolymerShapes.insert(zPairLS(PolymerTypeTotal,shape));
		m_mPolymerSizes.insert(zPairLL(PolymerTypeTotal,vNewBeads.size()));
		m_mPolymerFractions.insert(zPairLD(PolymerTypeTotal,fraction));

		CBead* pHead = vNewBeads.at(0);
		CBead* pTail = vNewBeads.at((vNewBeads.size()-1));

		// ****************************************
		// Add CBondPair objects to the polymer if it has the 3-body bond force.
		// We search through the bonds in the polymer looking for ones that connect
		// three beads whose names match any of the CBondPair types specified for
		// the simulation. If one if found we create a CBondPair object and add it
		// to the polymer for later use in CInitialState::CreatePolymers().

		long type1, type2, type3, bpType;
		zString firstName, secondName, thirdName;

		long firstBondIndex  = 0;
		for(BondVectorIterator iterBond=vNewBonds.begin(); iterBond!=vNewBonds.end(); iterBond++)
		{
			type1 = (*iterBond)->GetHead()->GetType();
			type2 = (*iterBond)->GetTail()->GetType();

			firstName  = (*mBeadTypes.find(type1)).second;
			secondName = (*mBeadTypes.find(type2)).second;

			// Find all bonds that have the currentIndex as their head index.
			// For each one concatenate the bead names from the first, second and
			// third beads: if this triplet exists in the mBondNames map it indicates
			// that the triplet represents a stiff bond so create a CBondPair object 
			// using the template  accessed via the name and associated type.
			//
			// Note that the order of storing the two bonds in the bondpair object
			// is opposite to the order in which they are encountered in the polymer
			// shape. This is necessary because the bonds' head and tail beads are
			// stored in the order they are added to the polymer, and the direction
			// of the bonds is thus from the tail of the bond added last to the head 
			// of that added before. We calculate the 3-body forces given that the 
			// bond vectors are pointing from the tail of a bond to its head. This
			// requires that we use the three beads defining the two bonds in the
			// reverse order to their occurrence in the polymer shape.

			long secondBondIndex = 0;
			for(BondVectorIterator iterBond2=vNewBonds.begin(); iterBond2!=vNewBonds.end(); iterBond2++)
			{
				if( (*iterBond2)->GetHeadIndex() == (*iterBond)->GetTailIndex())
				{
					type3 = (*iterBond2)->GetTail()->GetType();
					thirdName = (*mBeadTypes.find(type3)).second;

					if(mBondPairNames.find(firstName+secondName+thirdName) != mBondPairNames.end())
					{
						bpType = (*mBondPairNames.find(firstName+secondName+thirdName)).second;

						CBondPair* pBP = new CBondPair(*vBondPairTypes.at(bpType));
						pBP->SetBondIndex(secondBondIndex, firstBondIndex);
						pBP->SetBonds(*iterBond2, *iterBond);

						vNewBondPairs.push_back(pBP);
					}
				}
				secondBondIndex++;
			}
			firstBondIndex++;
		}
		// ****************************************
		// Add the new polymer to the vector of polymer types initialising its integer
		// type, linearity, number fraction, head, tail and bead and bond vectors.
		// If the polymer contains CBondPair objects we use a different constructor
		// to pass the vector of CBondPair objects.

		if(vNewBondPairs.empty())
		{
			vPolymerTypes.push_back(new CPolymer(PolymerTypeTotal, bLinear, fraction, 
									    pHead, pTail, vNewBeads, vNewBonds));
		}
		else
		{
			vPolymerTypes.push_back(new CPolymer(PolymerTypeTotal, bLinear, fraction, 
									    pHead, pTail, vNewBeads, vNewBonds, vNewBondPairs));
		}
		
	}
	
	if(m_bBondTypeMissing)
		return false;
	else
		return true;
}


// Second overloaded function to construct a new polymer type. This version
// requires the head and tail beads to be specified by their names. These
// must be unique in the set of bead names as they cannot be distinguished 
// by their position in the polymer shape. However they can have identical
// interactions and properties to other beads.
//
// The function IsBeadUniqueInPolymer() checks that the head and tail beads 
// occur uniquely in the polymer shape,but we check separately that they are 
// not the same.

bool CInputData::AddPolymerType(zString name, zString shape, double fraction, zString headBead, zString tailBead)
{
	if( !IsNameValid(name)	|| !IsShapeValid(shape) ||
		!(IsBeadUniqueInPolymer(headBead,shape))	|| 
		!(IsBeadUniqueInPolymer(tailBead,shape))	||
		(headBead == tailBead)       ||
		!AreAllBeadNamesValid(shape) ||  
		!IsElementValid(shape)       ||(fraction < 0.0) || (fraction > 1.0) )
		return false;
	else if(mPolymerNames.find(name) != mPolymerNames.end())
		return false;
	else
	{
		// Move through the elements in the shape, starting at the first,
		// adding beads and bonds. If an element is a simple bead name 
		// we add it to the polymer's vector of beads (creating a bond 
		// to attach it to the current bead if it is not the first bead), 
		// if it is a compound element we call
		// functions recursively to deal with each element in the compound.
		// They all end up by calling the AddBeadtoPolymer function.
		// For each polymer type we check to see if any CBondPair objects are
		// needed to account for stiff bonds (3-body forces between triplets of
		// beads connected by two adjacent bonds) and add them to the polymer
		// if so.

		BeadVector vNewBeads;
		BondVector vNewBonds;
		BondPairVector vNewBondPairs;

		// Note that current_element is not used in AddBeadtoPolymer() if we are
		// processing the first bead, and it is initialised if we are not.
		// Also the first bead must be a simple bead name in this version
		// of AddPolymerType(). The flag bLinear is used to indicate if the 
		// polymer has any side branches or not.

		zString current_element; 
		zString next_element; 

		long currentBeadIndex = -1;
		bool bLinear = true;

		zString tail = shape;

		while(tail != "()")
		{
			next_element = CAR(tail);

			// if next_element is a bead's name then add it directly to the polymer, 
			// otherwise parse the element looking for brackets or an operator at its 
			// head. The currentBeadIndex keeps track of which bead the next bond will
			// be added to. Because of side branches and loops this is not necessarily
			// the last bead that was added. We indicate whether to increment the current
			// bead index using the last argument.
			
			if(IsNameValid(next_element))
			{
				current_element = AddBeadtoPolymer(vNewBeads, vNewBonds, current_element, next_element, &currentBeadIndex, true);
			}
			else	// we have a compound element so parse it recursively
			{
				current_element = ParseElement(bLinear, vNewBeads, vNewBonds, current_element, next_element, &currentBeadIndex, true);
			}
			tail = CDR(tail);
		}

		// The new polymer type has been defined so create a new type and add 
		// it to the vector of polymer types passing the bead and bond lists 
		// as arguments to its constructor. We also store the polymer shapes
		// using their type as the key into a map.

		long PolymerTypeTotal = vPolymerTypes.size();

		mPolymerNames.insert(zPairSL(name,PolymerTypeTotal));
		mPolymerTypes.insert(zPairLS(PolymerTypeTotal,name));
		m_mPolymerShapes.insert(zPairLS(PolymerTypeTotal,shape));
		m_mPolymerSizes.insert(zPairLL(PolymerTypeTotal,vNewBeads.size()));
		m_mPolymerFractions.insert(zPairLD(PolymerTypeTotal,fraction));

		// Define the head and tail from the specified bead names. We know that
		// they must occur in the polymer so we loop over the vNewBeads vector 
		// looking for the (unique) head and tail beads whose types match
		// those the specified head and tail. Note that they can both be in the
		// same element (or branch) of the polymer.

		long headType = (*(GetBeadNamesMap().find(headBead))).second;
		long tailType = (*(GetBeadNamesMap().find(tailBead))).second;

		CBead* pHead = 0;
		CBead* pTail = 0;

		for(cBeadVectorIterator iterBead=vNewBeads.begin(); iterBead!=vNewBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == headType)
				pHead = (*iterBead);
			else if((*iterBead)->GetType() == tailType)
				pTail = (*iterBead);
		}

		// ****************************************
		// Add CBondPair objects to the polymer if it has the 3-body bond force.
		// We search through the bonds in the polymer looking for ones that connect
		// three beads whose names match any of the CBondPair types specified for
		// the simulation. If one if found we create a CBondPair object and add it
		// to the polymer for later use in CInitialState::CreatePolymers().
		//
		// Note that the order of storing the two bonds in the bondpair object
		// is opposite to the order in which they are encountered in the polymer
		// shape. This is necessary because the bonds' head and tail beads are
		// stored in the order they are added to the polymer, and the direction
		// of the bonds is thus from the tail of the bond added last to the head 
		// of that added before. We calculate the 3-body forces given that the 
		// bond vectors are pointing from the tail of a bond to its head. This
		// requires that we use the three beads defining the two bonds in the
		// reverse order to their occurrence in the polymer shape.

		long type1, type2, type3, bpType;
		zString firstName, secondName, thirdName;

		long firstBondIndex  = 0;
		for(BondVectorIterator iterBond=vNewBonds.begin(); iterBond!=vNewBonds.end(); iterBond++)
		{
			type1 = (*iterBond)->GetHead()->GetType();
			type2 = (*iterBond)->GetTail()->GetType();

			firstName  = (*mBeadTypes.find(type1)).second;
			secondName = (*mBeadTypes.find(type2)).second;

			// Find all bonds that have the currentIndex as their head index.
			// For each one concatenate the bead names from the first, second and
			// third beads: if this triplet exists in the mBondNames map it indicates
			// that the triplet represents a stif bond so create a CBondPair object 
			// using the template  accessed via the name and associated type.

			long secondBondIndex = 0;
			for(BondVectorIterator iterBond2=vNewBonds.begin(); iterBond2!=vNewBonds.end(); iterBond2++)
			{
				if( (*iterBond2)->GetHeadIndex() == (*iterBond)->GetTailIndex())
				{
					type3 = (*iterBond2)->GetTail()->GetType();
					thirdName = (*mBeadTypes.find(type3)).second;

					if(mBondPairNames.find(firstName+secondName+thirdName) != mBondPairNames.end())
					{
						bpType = (*mBondPairNames.find(firstName+secondName+thirdName)).second;

						CBondPair* pBP = new CBondPair(*vBondPairTypes.at(bpType));
						pBP->SetBondIndex(secondBondIndex, firstBondIndex);
						pBP->SetBonds(*iterBond2, *iterBond);

						vNewBondPairs.push_back(pBP);
					}
				}
				secondBondIndex++;
			}
			firstBondIndex++;
		}
		// ****************************************
		// Add the new polymer to the vector of polymer types initialising its integer
		// type, linearity, number fraction, head, tail and bead and bond vectors.
		// If the polymer contains CBondPair objects we use a different constructor
		// to pass the vector of CBondPair objects.

		if(vNewBondPairs.empty())
		{
			vPolymerTypes.push_back(new CPolymer(PolymerTypeTotal, bLinear, fraction, 
									    pHead, pTail, vNewBeads, vNewBonds));
		}
		else
		{
			vPolymerTypes.push_back(new CPolymer(PolymerTypeTotal, bLinear, fraction, 
									    pHead, pTail, vNewBeads, vNewBonds, vNewBondPairs));
		}
	}

	if(m_bBondTypeMissing)
		return false;
	else
		return true;

}

// Function to check that the bead name supplied corresponds to one that is
// defined in the map of currently existing beads. Note the syntax that the
// find member function returns an iterator to the map element, if found, or one
// to the end() of the map. But to access the element in the map we have to use
// the iterator->first and iterator->second members.

bool CInputData::IsBeadinMap(const zString name) const
{
	cStringLongIterator iterBead = mBeadNames.find(name);

	if(iterBead != mBeadNames.end())
		return true;

	return false;
}

// Function to check that the bond name supplied corresponds to one that is
// defined in the map of currently existing bonds. Note the syntax that the
// find member function returns an iterator to the map element, if found, or one
// to the end() of the map. But to access the element in the map we have to use
// the iterator->first and iterator->second members.

bool CInputData::IsBondinMap(const zString name) const
{
	cStringLongIterator iterBond = mBondNames.find(name);

	if(iterBond != mBondNames.end())
		return true;

	return false;
}

// Function to check that the polymer name supplied corresponds to one that is
// defined in the map of currently existing polymers. Note the syntax that the
// find member function returns an iterator to the map element, if found, or one
// to the end() of the map. But to access the element in the map we have to use
// the iterator->first and iterator->second members.

bool CInputData::IsPolymerinMap(const zString name) const
{
	cStringLongIterator iterPoly = mPolymerNames.find(name);

	if(iterPoly != mPolymerNames.end())
		return true;

	return false;
}

// Function to check that the bondpair name supplied corresponds to one that is
// defined in the map of currently existing CBondPairs. Note the syntax that the
// find member function returns an iterator to the map element, if found, or one
// to the end() of the map. But to access the element in the map we have to use
// the iterator->first and iterator->second members.

bool CInputData::IsBondPairinMap(const zString name) const
{
	cStringLongIterator iterBP = mBondPairNames.find(name);

	if(iterBP != mBondPairNames.end())
		return true;

	return false;
}

// Function to grow a polymer type by attaching a new bead to the current bead
// via a new bond. 
//
// If the appropriate bond type is not found in the map the
// function sets the flag m_bBondTypeMissing to signal the AddPolymerType()
// routine that it failed (we cannot use the return value of the function because 
// that is already used to point to the current bead).
//
// The current bead represents the local backbone of the polymer and is
// used recusively within AddPolymerType() to grow the side branches and 
// loops as well as the backbone itself. The bonds hold the indices into the 
// vNewBeads array of the beads at their head and tail. This is used when copying
// a polymer template into the actual polymers used in the simulation.
//
// If the vector of beads is empty this means we are starting the polymer at
// its head so there is no current_beadname and currentBeadIndex is -1. But these
// are not used when the first bead is added and are updated in ParseElement()
// before the second bead and a bond are added.
//
// Note that we pass the vectors of beads and bonds in by reference to allow
// us to add new elements to them. And the current bead index is passed as a pointer
// with the final argument showing whether it should be incremented or not.

zString CInputData::AddBeadtoPolymer(BeadVector& vNewBeads, BondVector& vNewBonds, zString current_beadname, zString next_beadname, long* pCurrentBeadIndex, bool bIncrementBeadIndex)
{
	StringLongIterator iterBeadType;
	long BeadType;
	CBead* pBead;

	if(vNewBeads.empty())	// if this is the first bead we don't need a bond
	{
		iterBeadType	= mBeadNames.find(next_beadname);
		BeadType		= (*iterBeadType).second;
		pBead			= new CBead(*vBeadTypes.at(BeadType));
		vNewBeads.push_back(pBead);
	}
	else					// otherwise we create a new bead and bond and attach them
	{
		iterBeadType	= mBeadNames.find(next_beadname);
		BeadType		= (*iterBeadType).second;
		pBead			= new CBead(*vBeadTypes.at(BeadType));
		vNewBeads.push_back(pBead);

		StringLongIterator iterBondType;	
		long BondType;
		zString bondname;		// we only ever need one bond

		bondname		= current_beadname + next_beadname;
		iterBondType	= mBondNames.find(bondname);
		if(iterBondType != mBondNames.end())
		{
			BondType		= (*iterBondType).second;
			CBond* pBond	= new CBond(*vBondTypes.at(BondType));		// bonds don't know about beads
			pBond->SetBeadIndex(*pCurrentBeadIndex, vNewBeads.size()-1);	// set indices of beads 
			pBond->SetBeads(vNewBeads.at(*pCurrentBeadIndex), pBead);	    // set bead pointers
			vNewBonds.push_back(pBond);
		}
		else
		{
			m_bBondTypeMissing = true;
		}
	}
	
	if(bIncrementBeadIndex)
		*pCurrentBeadIndex = vNewBeads.size()-1;

	return next_beadname;
}

// Function to return the last element in the shape. We remove
// any spaces at the beginning and end of the element so that simple bead names
// do not appear to have spaces in them even if they were surrounded by spaces
// in the polymer shape.

zString CInputData::LAST(const zString shape) const
{
	zString tail = CDR(shape); 
	zString previous_tail = tail;

	while(tail != "()")
	{
		previous_tail = tail;
		tail = CDR(tail);
	}
	previous_tail = RemoveBrackets(previous_tail);
	previous_tail = RemoveSpaces(previous_tail);											// strip whitespace off
	return previous_tail;
}

// Synonym for CAR(shape). It returns the first element in the polymer shape.
// It is added just for symmetry with LAST()

zString CInputData::FIRST(const zString shape) const
{
	return CAR(shape);
}

long CInputData::GetSamplePeriod() const
{
	return SamplePeriod;
}

long CInputData::GetAnalysisPeriod() const
{
	return AnalysisPeriod;
}

long CInputData::GetDisplayPeriod() const
{
	return DisplayPeriod;
}

long CInputData::GetRestartPeriod() const
{
	return RestartPeriod;
}

long CInputData::GetDensityPeriod() const
{
	return DensityPeriod;
}

long CInputData::GetProcessorsXNo() const
{
    return ProcXNo;
}

long CInputData::GetProcessorsYNo() const
{
    return ProcYNo;
}

long CInputData::GetProcessorsZNo() const
{
    return ProcZNo;
}

long CInputData::GetCNTXCellNo() const
{
	return CNTXCellNo;
}

long CInputData::GetCNTYCellNo() const
{
	return CNTYCellNo;

}

long CInputData::GetCNTZCellNo() const
{
	return CNTZCellNo;

}

long CInputData::GetCurrentTime() const
{
	return CurrentTime;
}

long CInputData::GetTotalTime() const
{
	return TotalTime;
}

long CInputData::GetRNGSeed() const
{
	return RNGSeed;
}

double CInputData::GetCNTXCellWidth() const
{
	return CNTXCellWidth;
}

double CInputData::GetCNTYCellWidth() const
{
	return CNTYCellWidth;
}

double CInputData::GetCNTZCellWidth() const
{
	return CNTZCellWidth;
}

double CInputData::GetkT() const
{
	return kT;
}

double CInputData::GetStepSize() const
{
	return StepSize;
}

// DPD function to get the integration constant lambda for the DPD algorithm.

double CInputData::GetLambda() const
{
	return Lambda;
}

// MD function to return the cut-off radius of all potentials

double CInputData::GetCutOffRadius() const
{
	return RCutOff;
}

double CInputData::GetAveBeadDensity() const
{
	return AverageBeadDensity;
}

double CInputData::GetSimBoxXLength() const
{
	return static_cast<double>(CNTXCellNo)*CNTXCellWidth;
}

double CInputData::GetSimBoxYLength() const
{
	return static_cast<double>(CNTYCellNo)*CNTYCellWidth;
}

double CInputData::GetSimBoxZLength() const
{
	return static_cast<double>(CNTZCellNo)*CNTZCellWidth;
}

// Function to return the size of the X dimension in simulation Space: this
// is the same as GetSimBoxXLength() for a serial simulation.

double CInputData::GetSimSpaceXLength() const
{
    return static_cast<double>(GetProcessorsXNo())*GetSimBoxXLength();
}

// Function to return the size of the Y dimension in simulation Space: this
// is the same as GetSimBoxYLength() for a serial simulation.

double CInputData::GetSimSpaceYLength() const
{
    return static_cast<double>(GetProcessorsYNo())*GetSimBoxYLength();
}

// Function to return the size of the Z dimension in simulation Space: this
// is the same as GetSimBoxZLength() for a serial simulation.

double CInputData::GetSimSpaceZLength() const
{
    return static_cast<double>(GetProcessorsZNo())*GetSimBoxZLength();
}

// Function to analyse the structure of an element and expand its tokens
// to allow the beads to be added to the growing polymer. Each bead is added
// using the AddBeadtoPolymer() function.
//
// Note that this function assumes that only valid bead names and operators 
// appear in the element, and that all bead names are in the beadnames map, 
// so it must be called after an application of AreAllBeadNamesValid().
//
// We indicate whether to move the current bead index with the bIncrementBeadIndex flag.
// If this is true, it shows that the polymer backbone will grow with the addition of a 
// bead or sequence of beads, but if it is false, then a sidebranch is added that does not
// reposition the place where the next bead will be added.

zString CInputData::ParseElement(bool& bLinear, BeadVector& vNewBeads, BondVector& vNewBonds, zString current_beadname, zString element, long* pCurrentBeadIndex, bool bIncrementBeadIndex)
{
	zString head = CAR(element);
	zString tail = CDR(element);
	zString token = RemoveBrackets(tail);

	zString dummy_beadname;	// used to avoid changing the current bead name

	if(!(head.find_first_not_of(CInputData::validNameChars) < head.length()) &&
		(head.find_first_of(CInputData::validNumOpChars) != 0) )	// simple bead name
	{		
		current_beadname = AddBeadtoPolymer(vNewBeads, vNewBonds, current_beadname, head, pCurrentBeadIndex, bIncrementBeadIndex);
		if(tail != "()")
			current_beadname = ParseElement(bLinear, vNewBeads, vNewBonds, current_beadname, tail, pCurrentBeadIndex, bIncrementBeadIndex);
	}
	else if(!(head.find_first_not_of(CInputData::validNumOpChars) < head.length()))	// multiplier operator
	{
		tail = ExpandElement(head, token);
		current_beadname = ParseElement(bLinear, vNewBeads, vNewBonds, current_beadname, tail, pCurrentBeadIndex, bIncrementBeadIndex);
	}	
	else if(head == CInputData::branchChar)			// side branch operator
	{
		bLinear = false;
		if(IsNameValid(token))
			dummy_beadname = AddBeadtoPolymer(vNewBeads, vNewBonds, current_beadname, token, pCurrentBeadIndex, false);
		else
		{
			long NewCurrentBeadIndex;

			NewCurrentBeadIndex = *pCurrentBeadIndex;
			dummy_beadname = ParseElement(bLinear, vNewBeads, vNewBonds, current_beadname, tail, &NewCurrentBeadIndex, true);

// Following line was an error
//			dummy_beadname = ParseElement(bLinear, vNewBeads, vNewBonds, current_beadname, tail, pCurrentBeadIndex, false);
		}
	}
	else if(head == CInputData::loopChar)		// loop operator
	{
		bLinear = false;
		zString loopId			= FIRST(tail);
		zString targetBeadName	= LAST(tail);
		int iLoopId				= atoi(loopId.c_str());	// convert string to a number

		// Unlike the branch operator we do move the current_beadname pointer when
		// adding a loop target. This is because loop targets are just normal beads
		// that can have extra bonds attached to them later.

		if(IsNameValid(targetBeadName))
		{
			current_beadname = AddBeadtoPolymer(vNewBeads, vNewBonds, current_beadname, targetBeadName, pCurrentBeadIndex, bIncrementBeadIndex);

			// If the target does not exist in the map we simply insert it, if
			// it is there then we add the current bead to it with a bond. 

			CAbstractBead* pTargetBead = vNewBeads.back();

			if(m_mLoopTargets.find(iLoopId) == m_mLoopTargets.end())
			{
				LoopTarget* pTarget = new LoopTarget(*pCurrentBeadIndex, targetBeadName, pTargetBead);

				m_mLoopTargets.insert(idPairLongLoopTarget(iLoopId,pTarget));
			}
			else
			{
				LoopTarget* pTarget = (*m_mLoopTargets.find(iLoopId)).second;

				long targetBeadIndex		= pTarget->GetId();
				zString targetBeadName		= pTarget->GetName();
				CAbstractBead* pTargetBead	= pTarget->GetBead();

				StringLongIterator iterBondType;	
				long BondType;
				zString bondname;		// we only ever need one bond

				bondname		= current_beadname + targetBeadName;
				iterBondType	= mBondNames.find(bondname);
				if(iterBondType != mBondNames.end())
				{
					BondType		= (*iterBondType).second;
					CBond* pBond	= new CBond(*vBondTypes.at(BondType));		// bonds don't know about beads
					pBond->SetBeadIndex(targetBeadIndex, vNewBeads.size()-1);	// set indices of beads 
					pBond->SetBeads(vNewBeads.at(*pCurrentBeadIndex), pTargetBead);	    // set bead pointers
					vNewBonds.push_back(pBond);
				}
				else
				{
					m_bBondTypeMissing = true;
				}
			}
		}
	}
	else		// head is a compound element so parse it further until tail is empty
	{
		if(tail =="()")
			current_beadname = ParseElement(bLinear, vNewBeads, vNewBonds, current_beadname, head, pCurrentBeadIndex, bIncrementBeadIndex);
		else
		{
			current_beadname = ParseElement(bLinear, vNewBeads, vNewBonds, current_beadname, head, pCurrentBeadIndex, bIncrementBeadIndex);
			current_beadname = ParseElement(bLinear, vNewBeads, vNewBonds, current_beadname, tail, pCurrentBeadIndex, bIncrementBeadIndex);
		}	
	}

	return current_beadname;
}

// Function to trim whitespace off the front and back of a zString.
// We don't check inside the zString because the spaces may be significant, e.g.,
// in separating bead names.

zString CInputData::RemoveSpaces(zString oldString) const
{
    if(!oldString.empty())
    {
        
	zStringIterator iterFor	 = oldString.begin();
	zStringIterator iterBack = oldString.end();
	iterBack--;

	for(long j=0; j<oldString.size(); j++)
	{
		if(*iterFor == ' ')
			oldString.erase(iterFor);	// erase automatically increments iterator
		if(*iterBack == ' ')
		{
			oldString.erase(iterBack);	
			iterBack--;					// move back from end()
		}
	}
    
        
    }
    
	return oldString;
}

void CInputData::SetAveBeadDensity(double dens)
{
	AverageBeadDensity = dens;
}

// Functions that set the number of processors per dimension for a parallel run.
// We conditionally compile out these functions so that the standard code does
// not get broken. For a non-parallel run, the number of processors per dimension
// is set to 1.

void CInputData::SetProcessorsXNo(long xproc)
{
#if SimMPS == SimulationEnabled
    ProcXNo	= xproc;
#else
    ProcXNo = 1;
#endif
}

void CInputData::SetProcessorsYNo(long yproc)
{
#if SimMPS == SimulationEnabled
    ProcYNo	= yproc;
#else
    ProcYNo = 1;
#endif
}

void CInputData::SetProcessorsZNo(long zproc)
{
#if SimMPS == SimulationEnabled
    ProcZNo	= zproc;
#else
    ProcZNo = 1;
#endif
}

void CInputData::SetCNTXCellNo(long xcells)
{
	CNTXCellNo	= xcells;
}

void CInputData::SetCNTYCellNo(long ycells)
{
	CNTYCellNo	= ycells;
}

// Set function to work for both 2 and 3 dimensions by conditionally compiling
// the number of CNT cells in the Z dimension. Note that this must set the 
// 2d number to 1 as it is used to divide the simulation box length. However, the
// width of the cells in the Z dimension is 0 so that the simulation box has no
// Z width.

void CInputData::SetCNTZCellNo(long zcells)
{

#if SimDimension == 2
	CNTZCellNo = 1
#elif SimDimension == 3
	CNTZCellNo = zcells;
#endif

}

void CInputData::SetCNTXCellWidth(double xcellwidth)
{
	CNTXCellWidth	= xcellwidth;
}

void CInputData::SetCNTYCellWidth(double ycellwidth)
{
	CNTYCellWidth	= ycellwidth;
}

// Set function to work for both 2 and 3 dimensions by conditionally compiling
// the width of CNT cells in the Z dimension. In two dimensions the width
// of the cells must be 0 so that the simulation box has no Z width.

void CInputData::SetCNTZCellWidth(double zcellwidth)
{

#if SimDimension == 2
	CNTZCellWidth = 0.0;
#elif SimDimension == 3
	CNTZCellWidth = zcellwidth;
#endif

}

// Function to open a stream to the CDF and call its Serialize() method to 
// write the default data appropriate to the simulation type. This is selected 
// by conditional compilation of the appropriate class. Note that for a DPD
// simulation we only write out a standard CDF not the liquid-gas one.

bool CInputData::WriteDefaultControlDataFile(const zString iFileName)
{
#if SimIdentifier == BD

	CInputBDFile inFile(iFileName, true, *m_pISD);	// true = write to file

	inFile.Title	= "Actin monomers";
	inFile.Date		= "8/5/06";
	inFile.Comment	= "1000 actin monomers in a 32**3 box";

#elif SimIdentifier == DPD

	CInputDPDFile inFile(iFileName, true, *m_pISD);	// true = write to file

	inFile.Title	= "Water/surfactant";
	inFile.Date		= "2/7/01";
	inFile.Comment	= "Default test run of 2057h";

#elif SimIdentifier == MD

	CInputMDFile inFile(iFileName, true, *m_pISD);	

	inFile.Title	= "Default MD Oil/Water";
	inFile.Date		= "26/1/00";
	inFile.Comment	= "Under construction";

#endif

// Copy data common to all simulation types to the input file object.
// We conditionally compile in the processor numbers because they do
// not exist in the input file unless a parallel run is being done.

#if SimMPS == SimulationEnabled
    inFile.ProcXNo              = ProcXNo;
    inFile.ProcYNo              = ProcYNo;
    inFile.ProcZNo              = ProcZNo;
#endif

    inFile.CNTXCellNo			= CNTXCellNo;
	inFile.CNTYCellNo			= CNTYCellNo;
	inFile.CNTZCellNo			= CNTZCellNo;
	inFile.CNTXCellWidth		= CNTXCellWidth;
	inFile.CNTYCellWidth		= CNTYCellWidth;
	inFile.CNTZCellWidth		= CNTZCellWidth;
	inFile.AverageBeadDensity	= AverageBeadDensity;
	inFile.kT					= kT;
	inFile.RNGSeed				= RNGSeed;

#if SimIdentifier == BD
	inFile.Lambda				= Lambda;
#elif SimIdentifier == DPD
	inFile.Lambda				= Lambda;
#elif SimIdentifier == MD
	inFile.RCutOff				= RCutOff;
#endif

	inFile.StepSize				= StepSize;
	inFile.TotalTime			= TotalTime;
	inFile.SamplePeriod			= SamplePeriod;
	inFile.AnalysisPeriod		= AnalysisPeriod;
	inFile.DisplayPeriod		= DisplayPeriod;
	inFile.DensityPeriod		= DensityPeriod;
	inFile.RestartPeriod		= RestartPeriod;
	inFile.GridXCellNo			= GridXCellNo;
	inFile.GridYCellNo			= GridYCellNo;
	inFile.GridZCellNo			= GridZCellNo;

	// **********************************************************************
	// Data for the default run correspond to the control data file 2057h.
	// This is used to check the behaviour of the code after general modifications.
	// But note that it does not test any of the analysis options only the 
	// general properties of the simulation and global observable in CMonitor.
	//
	// Bead name	Conservative					Dissipative
	//	H			25.0						4.5
	//	A			25.0 25.0					4.5	4.5
	//	B			50.0 50.0 25.0				9   9   4.5
	//	T			50.0 50.0 25.0 25.0			9   9   4.5  4.5
	//	W			35.0 35.0 75.0 75.0 25.0	4.5 4.5 20.0 20.0 4.5

	// Create two bond types with names that correspond to the names of the
	// beads at each end concatenated. We pass in the names of the beads
	// at each end of the bond and the function creates the bond name.
	//
	// Type		Spring constant    Unstretched length
	//	HT			128.0					0.5
	//	TT			128.0					0.5
	//
	// Polymer shapes are defined as strings and are parsed within the CInputData
	// object overloaded functions AddPolymerType(). We define a grammar for 
	// the polymer structures that
	// allows the user to define branches and loops and repeated bead names.
	// The whole structure must be inclosed within brackets, and any operators
	// must be the first token after a left bracket.
	//
	// The only tokens that can appear in a polymer shape are:
	//
	// 1	"(" and ")" to denote the start and end of sequences of elements
	// 2	"ABC1234"	strings that represent bead names (must start a with letter)
	// 3	" "			spaces as separators for elements
	// 4    "*" "/"     operators defining a side branch and loop
	// 5	"123"		numbers representing a repeat multiplier
	//
	//
	// The only characters that can appear in a bead or polymer name are
	// alphanumeric ones. No underscores, spaces, dashes etc.
	//
	// Bond names are constructed by concatenating the names of the beads
	// at each end. No user-assigned name is allowed.
	//
	// We have tested the code on the following structures:
	//
	//	(A B B A)
	//  (A B (* B) B (* B) A)
	//  (A B (4 (* B)) B A)
	//  (A A (* (4 B)(*(A A A A)) (2 (* B)) B A)
	//  (A(B)B)
	//  (A(*B)(*B)B)
	//  (W)
	//
	// Note that spaces must separate bead names (and a numeric operator and
	// a bead name) otherwise the whole sequence of
	// of chars will be taken as one name. However, brackets may be adjacent
	// to a bead name or operator.

#if SimIdentifier == BD
	
		zString name;
        double radius, transDiff, rotDiff;
		zDoubleVector consInt, dissInt;

		name = "A3";
        radius = 0.5;
        transDiff = 1.0;
        rotDiff = 0.0;
		consInt.push_back(25.0);
		dissInt.push_back(4.5);
		inFile.AddNewBeadType(name, radius, transDiff, rotDiff, consInt, dissInt);

		name = "M";
        radius = 0.5;
        transDiff = 1.0;
        rotDiff = 0.0;
		consInt.clear();
		dissInt.clear();
		consInt.push_back(25.0);
		consInt.push_back(25.0);
		dissInt.push_back(4.5);
		dissInt.push_back(4.5);
		inFile.AddNewBeadType(name, radius, transDiff, rotDiff, consInt, dissInt);

		name = "T3";
        radius = 0.5;
        transDiff = 1.0;
        rotDiff = 0.0;
		consInt.clear();
		dissInt.clear();
		consInt.push_back(25.0);
		consInt.push_back(25.0);
		consInt.push_back(25.0);
		dissInt.push_back(4.5);
		dissInt.push_back(4.5);
		dissInt.push_back(4.5);
		inFile.AddNewBeadType(name, radius, transDiff, rotDiff, consInt, dissInt);

		inFile.AddNewBondType("A3", "M",  128.0, 0.75);
		inFile.AddNewBondType("A3", "T3", 128.0, 0.75);
		inFile.AddNewBondType("M",  "T3", 128.0, 0.75);

        // We have to explicitly specify the polymer's Head and Tail because of the 
        // loop tokens at the beginning and end of the polymer's shape.

		inFile.AddNewPolymerType("mActin", " ((/ 1 A3) M (/ 1 T3)) ", 1.0, "A3", "T3");

		// Add any commands present in the default state: none are included for now.

#elif SimIdentifier == DPD
	
		zString name;
		zDoubleVector consInt, dissInt;

		name = "H";
		consInt.push_back(25.0);
		dissInt.push_back(4.5);
		inFile.AddNewBeadType(name, consInt, dissInt);

		name = "A";
		consInt.clear();
		dissInt.clear();
		consInt.push_back(25.0);
		consInt.push_back(25.0);
		dissInt.push_back(4.5);
		dissInt.push_back(4.5);
		inFile.AddNewBeadType(name, consInt, dissInt);

		name = "B";
		consInt.clear();
		dissInt.clear();
		consInt.push_back(50.0);
		consInt.push_back(50.0);
		consInt.push_back(25.0);
		dissInt.push_back(9.0);
		dissInt.push_back(9.0);
		dissInt.push_back(4.5);
		inFile.AddNewBeadType(name, consInt, dissInt);

		name = "T";
		consInt.clear();
		dissInt.clear();
		consInt.push_back(50.0);
		consInt.push_back(50.0);
		consInt.push_back(25.0);
		consInt.push_back(25.0);
		dissInt.push_back(9.0);
		dissInt.push_back(9.0);
		dissInt.push_back(4.5);
		dissInt.push_back(4.5);
		inFile.AddNewBeadType(name, consInt, dissInt);

		name = "W";
		consInt.clear();
		dissInt.clear();
		consInt.push_back(35.0);
		consInt.push_back(35.0);
		consInt.push_back(75.0);
		consInt.push_back(75.0);
		consInt.push_back(25.0);
		dissInt.push_back(4.5);
		dissInt.push_back(4.5);
		dissInt.push_back(20.0);
		dissInt.push_back(20.0);
		dissInt.push_back(4.5);
		inFile.AddNewBeadType(name, consInt, dissInt);

		inFile.AddNewBondType("H", "T", 128.0, 0.5);
		inFile.AddNewBondType("T", "T", 128.0, 0.5);

		inFile.AddNewBondPairType("H", "T", "T", 10.0, 0.0);
		inFile.AddNewBondPairType("T", "T", "T", 10.0, 0.0);

		inFile.AddNewPolymerType("Water", " (W) ", 0.957628);
		inFile.AddNewPolymerType("Lipid", " (H T T T T T T) ", 0.042372);

		// Add any commands present in the default state: none are included for now.

#elif SimIdentifier == MD

		zString name;
		zDoubleVector ljDepth, ljRange;
		zDoubleVector scDepth, scRange;

		name = "A";
		ljDepth.clear();
		ljRange.clear();
		scDepth.clear();
		scRange.clear();
		ljDepth.push_back(2.0);
		ljRange.push_back(1.0);
		scDepth.push_back(2.5);
		scRange.push_back(1.5);

		inFile.AddNewBeadType(name, ljDepth, ljRange, scDepth, scRange);

		name = "B";
		ljDepth.clear();
		ljRange.clear();
		scDepth.clear();
		scRange.clear();
		ljDepth.push_back(4.0);
		ljDepth.push_back(4.0);
		ljRange.push_back(3.0);
		ljRange.push_back(3.0);
		scDepth.push_back(4.5);
		scDepth.push_back(4.5);
		scRange.push_back(3.5);
		scRange.push_back(3.5);

		inFile.AddNewBeadType(name, ljDepth, ljRange, scDepth, scRange);

		name = "W";
		ljDepth.clear();
		ljRange.clear();
		scDepth.clear();
		scRange.clear();
		ljDepth.push_back(6.0);
		ljDepth.push_back(6.0);
		ljDepth.push_back(6.0);
		ljRange.push_back(5.0);
		ljRange.push_back(5.0);
		ljRange.push_back(5.0);
		scDepth.push_back(6.5);
		scDepth.push_back(6.5);
		scDepth.push_back(6.5);
		scRange.push_back(5.5);
		scRange.push_back(5.5);
		scRange.push_back(5.5);

		inFile.AddNewBeadType(name, ljDepth, ljRange, scDepth, scRange);

		inFile.AddNewBondType("B", "B", 1.0, 0.4);
		inFile.AddNewBondType("A", "B", 2.0, 0.7);

		inFile.AddNewPolymerType("Water", "(W)", 0.86);
		inFile.AddNewPolymerType("Lipid",  "(B B A B B)", 0.14);

#endif

	return inFile.Serialize();
}

// Function to open a stream attached to the CDF and read all the data 
// needed for a simulation of the current type. It provides the inverse 
// operation to WriteDefaultControlDataFile().
//
// It is conditionally compiled to create an object of the appropriate 
// simulation type to read the data from a file. The Serialize() method 
// is called to get the data. Once the data has been read it is used to 
// fill up the CInputData object's parameter values. For DPD simulations
// there are two options: a standard DPD run with conservative and dissipative
// forces, and a run that also includes a density-dependent force to allow
// liquid-gas interfaces to occur. We read a boolean flag from the input
// file to determine which of these is being simulated, and then copy the
// state into a local flag for later use in reading the interaction parameters.
// Note that even if the EnableDPDLG flag is compiled in, we can still do 
// a standard DPD run by setting the input file appropriately.
//
// If an error occurs on reading the data the function returns false, if everything
// is successful it returns true.

bool CInputData::ReadControlDataFile(const zString iFileName)
{

#if SimIdentifier == BD

	CInputBDFile inFile(iFileName, false, *m_pISD);	// false = read from file

#elif SimIdentifier == DPD

#if EnableDPDLG == ExperimentEnabled
	CInputDPDLGFile inFile(iFileName, false, *m_pISD);
#elif EnableDPDLG == ExperimentDisabled
	CInputDPDFile inFile(iFileName, false, *m_pISD);	// false = read from file
#endif

#elif SimIdentifier == MD

	CInputMDFile inFile(iFileName, false, *m_pISD);	

#endif

	if(inFile.IsFileStateOk() && inFile.Serialize())	// if no IO errors copy data into CInputData object
	{
#if EnableDPDLG == ExperimentEnabled
        SetDPDLG(inFile.IsLG());
#endif
		SetTitle(inFile.Title);
		SetDate(inFile.Date);
		SetComment(inFile.Comment);
		SetISD(inFile.pISD);
		SetGravityData(inFile.pGD);
		SetWallData(inFile.pWD);
        SetProcessorsXNo(inFile.ProcXNo);
        SetProcessorsYNo(inFile.ProcYNo);
        SetProcessorsZNo(inFile.ProcZNo);
		SetCNTXCellNo(inFile.CNTXCellNo);
		SetCNTYCellNo(inFile.CNTYCellNo);
		SetCNTZCellNo(inFile.CNTZCellNo);
		SetCNTXCellWidth(inFile.CNTXCellWidth);
		SetCNTYCellWidth(inFile.CNTYCellWidth);
		SetCNTZCellWidth(inFile.CNTZCellWidth);
		SetAveBeadDensity(inFile.AverageBeadDensity);
		SetTemp(inFile.kT);
		SetRNGSeed(inFile.RNGSeed);

#if SimIdentifier == BD
		SetLambda(inFile.Lambda);
#elif SimIdentifier == DPD
		SetLambda(inFile.Lambda);
#elif SimIdentifier == MD
		SetCutOffRadius(inFile.RCutOff);
		SetMCStepSize(inFile.MCStepSize);
		SetTotalMCTime(inFile.TotalMCTime);
#endif

		SetStepSize(inFile.StepSize);
		SetTotalTime(inFile.TotalTime);
		SetSamplePeriod(inFile.SamplePeriod);
		SetAnalysisPeriod(inFile.AnalysisPeriod);
		SetDisplayPeriod(inFile.DisplayPeriod);
		SetDensityPeriod(inFile.DensityPeriod);
		SetRestartPeriod(inFile.RestartPeriod);
		SetGridXCellNo(inFile.GridXCellNo);
		SetGridYCellNo(inFile.GridYCellNo);
		SetGridZCellNo(inFile.GridZCellNo);

		// Normalize the MD interaction parameters prior to copying the
		// BeadTypes, BondTypes and BondPairTypes into CBead, CBond and
		// CBondPair objects. This is unnecessary for DPD because the 
		// values entered by the user are already dimensionless.

#if SimIdentifier == MD
			NormalizeMD(inFile.mNewBeadTypes, inFile.mNewBondTypes, inFile.mNewBondPairTypes, inFile.mNewPolymerTypes);
#endif

		// Create the beads, bonds and polymers from the representations
		// read from the input file. If these are valid check that the optional
		// simulation commands are valid.
		//
		// If the CInitialStateData object indicates a restart read the old control data
		// file and check that only allowed data has been changed between the previous
		// run and the current one. Check that commands are valid.
		//
		// The Initial State Data object is sent the ValidateData message to check
		// that it is consistent (a parallel simulation uses the ValidateDataP message). 
        // These are virtual functions that each class derived from CInitialStateData 
        // can override if it wants to perform specific validation. A similar message 
        // is sent to all the aggregates,
		// events and commands to check that the data the user specifies is valid.
		// Finally, we check that the processes are valid: this is done last so
		// that we are sure the aggregate and event specifications for the processes
		// are valid before we make them available via pointers in the 
		// xxProcess-derived classes.

		if( !CreateNewTypes(inFile.mNewBeadTypes, inFile.mNewBondTypes, inFile.mNewBondPairTypes, inFile.mNewPolymerTypes) )
			return ErrorTrace("Error creating beads, bonds, bondpairs or polymers");
		else if(!IsWallValid())
			return ErrorTrace("Error in Wall constraint");
		else if(!IsGravityValid())
			return ErrorTrace("Error in Gravity constraint");
		else if(!AreAggregatesValid(inFile.vAnalysisObjects))
			return ErrorTrace("Error in Analysis data");
		else if(!AreEventsValid(inFile.vEventObjects))
			return ErrorTrace("Error in Event data");
		else if(!AreCommandsValid(inFile.vCommandObjects))
			return ErrorTrace("Error in Command data");
		else if(!AreProcessesValid(inFile.vProcessObjects))
			return ErrorTrace("Error in Process data");
		else if( m_pISD->GetInitialStateType() == "restart" && 
			    !IsRestartDataValid(inFile.mNewBeadTypes, inFile.mNewBondTypes, inFile.mNewPolymerTypes) )
			return ErrorTrace("Restart data invalid");
		else if(!AreParallelProcessorsValid())
			return ErrorTrace("Invalid numbers of processors per dimension");
		else if(!IsInitialStateValid())
			return ErrorTrace("Initial state data invalid");
		return true;
	}
	else
		return false;
}

// Function to convert the CBeadType, CBondType, CBondPairType and CPolymerType 
// objects into the  internal representation needed by the CInputData object 
// so that it can create 
// multiple copies of beads, bonds and polymers to be used in the simulation. 
// We use #def to distinguish the different simulation types. This removes the 
// possibility of a mixture of DPD and MD beads created in the same
// simulation. The data on the bead-bead interactions is passed using the
// CBeadStructure-derived classes.
//
// Note the possible confusion between CBeadType and CBead* objects also referred 
// to "types" to distinguish them from the actual beads used in the simulation.
// The only place where the CBeadType objects occur is in the map holding the 
// definitions of all the beads, bonds and polymers.

bool CInputData::CreateNewTypes(LongBeadTypeMap     mNewBeadTypes,
								LongBondTypeMap     mNewBondTypes,
								LongBondPairTypeMap mNewBondPairTypes,
								LongPolymerTypeMap  mNewPolymerTypes)
{
	bool newTypeAdded = false;
	
	for(LongBeadTypeIterator iterBead = mNewBeadTypes.begin(); iterBead != mNewBeadTypes.end(); iterBead++)
	{
		CBeadType* pBead = (*iterBead).second;

#if SimIdentifier == BD

		newTypeAdded = AddBeadType(pBead->GetName(), pBead->GetBDStructure());

#elif SimIdentifier == DPD

		newTypeAdded = AddBeadType(pBead->GetName(), pBead->GetDPDStructure());

#elif SimIdentifier == MD

		newTypeAdded = AddBeadType(pBead->GetName(), pBead->GetMDStructure());

#endif

		if(!newTypeAdded)
			return ErrorTrace("Error adding bead type to CInputData");
	}

	for(LongBondTypeIterator iterBond = mNewBondTypes.begin(); iterBond != mNewBondTypes.end(); iterBond++)
	{
		CBondType* pBond = (*iterBond).second;

		newTypeAdded = AddBondType(pBond->GetHeadName(), pBond->GetTailName(), 
								   pBond->GetSpringConst(), pBond->GetLength());
		if(!newTypeAdded)
			return ErrorTrace("Error adding bond type to CInputData");
	}

	for(LongBondPairTypeIterator iterBP = mNewBondPairTypes.begin(); iterBP != mNewBondPairTypes.end(); iterBP++)
	{
		CBondPairType* pBondPair = (*iterBP).second;

		newTypeAdded = AddBondPairType(pBondPair->GetFirstName(), pBondPair->GetSecondName(), 
									   pBondPair->GetThirdName(),
								       pBondPair->GetModulus(), pBondPair->GetPhi0());
		if(!newTypeAdded)
			return ErrorTrace("Error adding bondpair type to CInputData");
	}

	// Call the appropriate AddPolymerType() function depending on whether the
	// CPolymerType has its head and tail specified or not. Note that both
	// must be specified or neither.

	for(LongPolymerTypeIterator iterPoly = mNewPolymerTypes.begin(); iterPoly != mNewPolymerTypes.end(); iterPoly++)
	{
		CPolymerType* pPolymer = (*iterPoly).second;

		if( (pPolymer->GetHeadName() != "") && (pPolymer->GetTailName() != "") )
		{
			newTypeAdded = AddPolymerType(pPolymer->GetName(), 
										  pPolymer->GetShape(), 
										  pPolymer->GetFraction(),
										  pPolymer->GetHeadName(),
										  pPolymer->GetTailName());
		}
		else
		{
			newTypeAdded = AddPolymerType(pPolymer->GetName(), 
										  pPolymer->GetShape(), 
										  pPolymer->GetFraction());
		}
		if(!newTypeAdded)
			return ErrorTrace("Error adding polymer type to CInputData");
	}

	return true;
}

// Function showing if a DPD simulation includes the density-dependent force
// that allows liquid-gas interfaces. If the EnableDPDLG flag is not set,
// this is automatically false.

bool CInputData::IsDPDLG() const
{
#if EnableDPDLG == ExperimentEnabled
    return m_bLG;
#elif EnableDPDLG == ExperimentDisabled
    return false;
#endif
}
// Function to set a flag showing if the density-dependent force is inclued in DPD

void CInputData::SetDPDLG(bool bLG)
{
#if EnableDPDLG == ExperimentEnabled
    m_bLG = bLG;
#elif EnableDPDLG == ExperimentDisabled
    m_bLG = false;
#endif
}

// Function to set the title of the current data set

void CInputData::SetTitle(zString title)
{
	Title = title;
}

void CInputData::SetDate(zString date)
{
	Date = date;
}

void CInputData::SetComment(zString comment)
{
	Comment	= comment;
}

void CInputData::SetExpectedBeadTypeTotal(long total)
{
    m_ExpectedBeadTypeTotal = total;
}

void CInputData::SetExpectedBondTypeTotal(long total)
{
    m_ExpectedBondTypeTotal = total;
}

void CInputData::SetExpectedBondPairTypeTotal(long total)
{
    m_ExpectedBondPairTypeTotal = total;
}

void CInputData::SetExpectedPolymerTypeTotal(long total)
{
    m_ExpectedPolymerTypeTotal = total;
}

void CInputData::SetRNGSeed(long seed)
{
	RNGSeed = seed;
}

void CInputData::SetTemp(double temp)
{
	kT = temp;
}

void CInputData::SetLambda(double lambda)
{
	Lambda = lambda;
}

void CInputData::SetCutOffRadius(double rcutoff)
{
	RCutOff = rcutoff;
}

void CInputData::SetMCStepSize(double step)
{
	MCStepSize = step;
}

void CInputData::SetTotalMCTime(long steps)
{
	TotalMCTime = steps;
}

void CInputData::SetStepSize(double step)
{
	StepSize = step;
}

void CInputData::SetTotalTime(long steps)
{
	TotalTime = steps;
}

void CInputData::SetSamplePeriod(long period)
{
	SamplePeriod = period;
}

void CInputData::SetAnalysisPeriod(long period)
{
	AnalysisPeriod = period;
}

void CInputData::SetDisplayPeriod(long period)
{
	DisplayPeriod = period;
}

void CInputData::SetDensityPeriod(long period)
{
	DensityPeriod = period;
}

void CInputData::SetRestartPeriod(long period)
{
	RestartPeriod = period;
}

void CInputData::SetGridXCellNo(long xcells)
{
	GridXCellNo = xcells;
}

void CInputData::SetGridYCellNo(long ycells)
{
	GridYCellNo	= ycells;
}

// Set function to work for both 2 and 3 dimensions by conditionally compiling
// the number of analysis grid cells in the Z dimension.

void CInputData::SetGridZCellNo(long zcells)
{

#if SimDimension == 2
	GridZCellNo	= 1;
#elif SimDimension == 3
	GridZCellNo	= zcells;
#endif

}


// Function to get the data required for the initial state of a simulation.
// If a default state is being used, it writes out the default data to a 
// control data file before reading the file back in. If the control data
// file exists it is just read back in.
//
bool CInputData::GetInputData(const zString iFileName)
{
	// If no file is found create a default data set

	if(!xxFile::DoesFileExistForReadOnly(iFileName))
	{
		if(!WriteDefaultControlDataFile(iFileName))
			return ErrorTrace("Error writing control data file");
	}

	if(!ReadControlDataFile(iFileName))
		return ErrorTrace("Error reading control data file");

	return true;
}

// Function to take the CInitialStateData-derived object created by the
// CInputDPDFile (or equivalent) and use it to replace the current pointer.
// This updates the default initial state data with that read from the control
// data file. It also copies the maps taking bead, bond and polymer names into 
// their types so that the ISD can access polymer types given only their names.

void CInputData::SetISD(const CInitialStateData *pISD)
{
	delete m_pISD;

	m_pISD = pISD->Copy();
}

// Function to check that a bead name occurs exactly once in a polymer shape.
// Actually, we only have to check if it is missing or occurs at least twice 
// for the test to fail. We assume that the polymer shape has been validated 
// by a call to IsShapeValid() so that we only have to look for bead name 
// substrings that occur in the shape.
// Because bead names can contain sub-strings that are valid bead names 
// (e.g., H, H1, beadH), we have to check that the characters on either side 
// of the bead names being tested are not valid name characters: this indicates 
// the bead name is isolated. Also, because elements can be nested, we check 
// every bead name in each element by searching for contiguous groups of
// characters that are in the validNameChars set.
//

bool CInputData::IsBeadUniqueInPolymer(const zString bead, const zString shape) const
{
	bool bValid = false;

	long BeadMatches = 0;

	zString prevChar = "";
	zString nextChar = "";

	zString element = shape;

	while(element != "()")
	{
		zString localElement = CAR(element);

		while(localElement.find(bead) < localElement.length())
		{
			if(localElement.find(bead) > 0)
				prevChar.assign(localElement, localElement.find(bead)-1, 1);
			else
				prevChar = "";

			if(localElement.find(bead) < localElement.length()-bead.size())
				nextChar.assign(localElement, localElement.find(bead)+bead.size(), 1);
			else
				nextChar = "";

			if( (prevChar.empty() || !(validNameChars.find(prevChar) < validNameChars.length())) &&
				(nextChar.empty() || !(validNameChars.find(nextChar) < validNameChars.length())) )
				BeadMatches++;

			// Remove all characters from the start of the localElement to 
			// the end of the (searched-for) bead name, and repeat the search. 
			// This relies on the find() algorithm returning the first occurrence 
			// of a sub-string.

			localElement.erase(0, localElement.find(bead)+bead.size());

			while(!nextChar.empty() && validNameChars.find(nextChar) < validNameChars.length())
			{
				localElement.erase(0, 1);
				nextChar.assign(localElement, 0, 1);
			}
		}
		element = CDR(element);
	}

	if(BeadMatches == 1)	// if a unique match is found return true
		bValid = true;

	return bValid;
}

// Function to check an element is valid: this implies
//
// CAR(element) is a bead, an operator or another element
// CAR(element) is branchChar and CDR(CDR(element)) is "()"
// CAR(element) is branchChar and CAR(CDR(element)) is not branchChar
//
// The function is called recursively until the element is empty or it returns
// false. An empty element returns false but the result is checked against the
// expected value and need not invalidate the element.
//
// We assume that IsShapeValid() has been called to validate the order of brackets
// so we only check the contents of the elements here.

bool CInputData::IsElementValid(zString element) const
{
	bool bValid;

	zString head = CAR(element);
	zString tail = CDR(element);

	if(element == "()")
		bValid = false;
	else if(IsBeadinMap(element))
		bValid = true;
	else if(head == CInputData::branchChar)
	{
		if(IsBeadinMap(CAR(tail)) && CDR(tail) == "()")
			bValid = true;
		else if ( *CAR(tail).begin() == '('   && 
				  CDR(tail)       == "()"  && 
				  IsBeadinMap(CAR(CAR(tail))) ) 
			return IsElementValid(CAR(tail));
		else
			bValid = false;
	}
	else if(head == CInputData::loopChar)
	{
		zString loopId			= FIRST(tail);
		zString loopBeadName	= LAST(tail);
		int iLoopId				= atoi(loopId.c_str());

		if(IsBeadinMap(loopBeadName) &&
		   !(loopId.find_first_not_of(CInputData::validNumOpChars) < loopId.length())
		   && iLoopId > 0)
			bValid = true;
		else
			bValid = false;
	}
	else if(!(head.find_first_not_of(CInputData::validNumOpChars) < head.length()) )
	{
		return IsElementValid(tail);
	}
	else if(*head.begin() == '(')					// compound element
	{
		if(IsElementValid(head) && (tail =="()" || IsElementValid(tail)))	
			bValid = true;
		else
			bValid = false;
	}
	else if(IsBeadinMap(head) && (tail =="()" || IsElementValid(tail)))	
		bValid = true;							
	else									
		bValid = false;

	return bValid;
}

// Function to copy the CWallData object from the input data file to a local data
// members. Note that we only set the data values if the user specified a wall
// otherwise they keep the null values set in the constructor.

void CInputData::SetWallData(const CWallData *pWD)
{
	if(pWD)
	{
		// Setting the m_bWall flag here causes the SimState object to set its
		// own flag which the SimBox will query on its creation using
		// CSimState::IsWallOn(). This will cause the wall beads to be assigned
		// to the CNT cells appropriately. Later in the simulation wall beads
		// may be removed and reassigned using commands.

		m_bWall			= true;
		WallPolymerName = pWD->GetWallPolymerName();
		WallBeadDensity = pWD->GetWallBeadDensity();
		WallXWidth		= pWD->GetWallXWidth();
		WallYWidth		= pWD->GetWallYWidth();

#if SimDimension == 2
		WallZWidth		= 0;
#elif SimDimension == 3
		WallZWidth		= pWD->GetWallZWidth();
#endif

	}
}

// Function to check that the wall data read from the input file is valid. We could
// not do this in the InputDPDFile() because the polymer and simulation box data were
// not known at the time the wall data was entered.
//
// The criteria are:  wall polymer name must be in the mPolymerNames map
//					  wall bead density must be greater than zero
//					  wall thickness (in numbers of CNT cells ) must be less than the 
//					  simulation box width
//
// Note that if there is no wall then we return true automatically.

bool CInputData::IsWallValid() const
{
	if(!m_bWall || (IsWallPolymerinMap()	 && 
 					IsWallBeadDensityValid() && 
 					IsWallThicknessValid()))
		return true;
 
	return false;
}

bool CInputData::IsWallPolymerinMap() const
{
	cStringLongIterator iterName = mPolymerNames.find(WallPolymerName);

 	if(iterName != mPolymerNames.end())
		return true;
 
	return ErrorTrace("Error wall polymer not in map");
}


bool CInputData::IsWallBeadDensityValid() const
{
 	if(WallBeadDensity > 0.0)
 		return true;
 	
	return ErrorTrace("Error wall bead density not greater than 0");
}

bool CInputData::IsWallThicknessValid() const
{
 
#if SimDimension == 2
 
	if(!(WallXWidth == 0 && WallYWidth == 0) &&	// not both zero
 	   !(WallXWidth < 0  || WallYWidth < 0)  && // neither negative
 		(WallXWidth < CNTXCellNo)			 && 
		(WallYWidth < CNTYCellNo))				// less than sim box 
		return true;

#elif SimDimension == 3
 
 	if(!(WallXWidth == 0 && WallYWidth == 0 && WallZWidth == 0) &&	
 	   !(WallXWidth < 0  || WallYWidth < 0  || WallZWidth < 0)  && 
 		(WallXWidth < CNTXCellNo) && 
		(WallYWidth < CNTYCellNo) &&
 	    (WallZWidth < CNTZCellNo))
 		return true;
 
#endif

 	return ErrorTrace("Error wall thickness negative or greater than simulation box");
}

// Function to copy the data relating to a body force acting throughout the SimBox
// from the CGravityData object into local variables. If the user did not specify
// that gravity be turned on the data members retain their null values set in the 
// constructor.

void CInputData::SetGravityData(const CGravityData* pGD)
{
	if(pGD)
	{
		m_bGravity			= true;
		vGravityBeadNames	= pGD->GetBeads();
		GravityXForce		= pGD->GetXForce();
		GravityYForce		= pGD->GetYForce();

#if SimDimension == 2
		GravityZForce	= 0.0;
#elif SimDimension == 3
		GravityZForce	= pGD->GetZForce();
#endif

	}

}

// Check that the body force data is valid. The only test we make is that the beads whose
// names are specified as being affected by the body force occur in the bead names map.

bool CInputData::IsGravityValid() const
{
	if(m_bGravity && !IsGravityBeadValid())
		return false;
 
	return true;
}

// Function to check that beads described as being affected by gravity occur in the 
// map of beads known to the simulation. The CGravityData object stores beads by their names
// but the map relates names (zString) to types (long). If the bead name is found we create
// a new vector of bead types so that the CInitialState object can use this to pass to the
// rest of the simulation which does not know about names of beads.

bool CInputData::IsGravityBeadValid() const
{
 	cStringLongIterator iterName;
 
 	for(cStringSequenceIterator iterBead=vGravityBeadNames.begin(); iterBead!=vGravityBeadNames.end(); iterBead++)
	{
 		iterName = mBeadNames.find(*iterBead);
		if(iterName == mBeadNames.end())
			return ErrorTrace("Error gravity bead not in map");
	}
 
 	return true;
}

// Function to return a vector of bead types for those beads whose names appear
// in the vector of bead names obtained from the CGravityData object. We translate from
// names to types because the CInitialState object and the rest of the simulation 
// do not know about the names of beads, bonds or polymers.

const zLongVector CInputData::GetGravityBeadTypes() const
{
	zLongVector vBeads;

	vBeads.clear();

	for(cStringSequenceIterator iterName=vGravityBeadNames.begin(); iterName!=vGravityBeadNames.end(); iterName++)
	{
		vBeads.push_back((*mBeadNames.find(*iterName)).second);	
	}
	return vBeads;
}

// Function to return the set of processes to be monitored and studied during
// the simulation.

const ProcessSequence& CInputData::GetProcesses() const
{
	return m_vProcesses;
}

// Function to return the set of aggregates to be monitored during a simulation.

const AggregateSequence& CInputData::GetAggregates() const
{
	return m_vAggregates;
}

// Function to return the set of events to be watched for during a simulation.

const EventSequence& CInputData::GetEvents() const
{
	return m_vEvents;
}

// Function to return the set of commands that the user has set to be executed
// during the simulation. Note that other commands may be generated as the 
// run progresses and they will be added to the command set.

const CommandSequence& CInputData::GetCommands() const
{
	return m_lCommands;
}


// Function to scale the dimensional quantities in an MD simulation by the 
// interaction depth and radius of the LJ potential for the first species. 
// The SimBox size and CNT cell sizes are assumed to be already expressed 
// in these units. To avoid having to normalize all the beads/bonds/bondpairs
// in each polymer type, this routine must be called before CreateNewType().
// But as a result, we have to use the maps of CBeadTypes, etc, not the 
// vectors of CBead*. Hence, we have to extract the bead structure to obtain
// the LJ depth and range parameters.
//
// In fact, we only normalize the bond and bondpair parameters here because
// the bead-bead interactions are copied into local arrays in the CCNTCell
// class for speed of access, and are normalized there. This is somewhat
// inconsistent as the CInitialState's bead types will contain the unnormalized
// values but the bonds and bondpair parameters will be normalized.
//
// If the first species has a potential depth or range of zero the bond 
// parameters are left unchanged. The bondpair interaction only has an
// energy scale as the preferred angle does not rescale.

void CInputData::NormalizeMD(LongBeadTypeMap      mNewBeadTypes,
							 LongBondTypeMap      mNewBondTypes,
							 LongBondPairTypeMap  mNewBondPairTypes,
							 LongPolymerTypeMap   mNewPolymerTypes)
{
	const CBeadType* const pBead = (*mNewBeadTypes.begin()).second;

	const CMDBeadStructure* pMD   = dynamic_cast<const CMDBeadStructure*>(pBead->GetMDStructure());

	const double enorm = (pMD->GetLJDepth()).at(0);
	const double lnorm = (pMD->GetLJRange()).at(0);

	if(enorm > 0.0 && lnorm > 0.0)
	{
		double newSprConst			= 0.0;
		double newLength			= 0.0;
		double newBendingModulus	= 0.0;

		for(LongBondTypeIterator iterBond = mNewBondTypes.begin(); iterBond != mNewBondTypes.end(); iterBond++)
		{
			CBondType* pBond = (*iterBond).second;
			newSprConst  = pBond->GetSpringConst()*lnorm*lnorm/enorm;
			newLength	 = pBond->GetLength()/lnorm;

			pBond->SetSpringConst(newSprConst);
			pBond->SetLength(newLength);
		}

		for(LongBondPairTypeIterator iterBP = mNewBondPairTypes.begin(); iterBP != mNewBondPairTypes.end(); iterBP++)
		{
			CBondPairType* pBondPair = (*iterBP).second;
			newBendingModulus  = pBondPair->GetModulus()/enorm;

			pBondPair->SetModulus(newBendingModulus);
		}
	}
}

// Function to check that the data read from a control data file that specifies
// a restart initial configuration agrees with that in the original control data
// file. Only certain parameters are allowed to be altered when doing a restart.
// In particular,the names of beads, bonds and polymers must not be changed.
// 
// Parameters that can be altered are:
//
//	Bead interaction parameters
//
//	Bond unstretched length and spring constant
//
//  Polymer head and tail bead assignments
//
//	RNGSeed
//	Step size for integration scheme
//	Time
//	SamplePeriod
//	AnalysisPeriod
//	DensityPeriod
//	DisplayPeriod
//	RestartPeriod
//	Grid
//
// In addition, if a wall constraint was imposed on the original simulation it must
// also be imposed on the restart state. However, the gravity and shear constraints
// do not have to be the same. This is because they do not affect the coordinates of
// the beads in the simulation box.
//
// Note that the arguments passed into this routine come from the current
// CInputDPDFile or CInputMDFile and will go out of scope when those objects
// are destroyed. Hence, this function can only be called when one of those
// objects is in scope.

bool CInputData::IsRestartDataValid(LongBeadTypeMap    mNewBeadTypes,
									LongBondTypeMap    mNewBondTypes,
									LongPolymerTypeMap mNewPolymerTypes) const
{
	// Define a flag showing whether the control data files match

	bool bRestartOK = true;

    zString oldFileName = xxBase::GetCDFPrefix() + dynamic_cast<CInitialStateRestart*>(m_pISD)->GetRunId();

#if SimIdentifier == BD

	CInputBDFile oldinFile(oldFileName, false, *m_pISD);	// false = read from file

#elif SimIdentifier == DPD

	CInputDPDFile oldinFile(oldFileName, false, *m_pISD);	// false = read from file

#elif SimIdentifier == MD

	CInputMDFile oldinFile(oldFileName, false, *m_pISD);	

#endif

	// Compare critical data with new values to make sure only legitimate changes 
	// have been made. We set a flag true and then make it false if a discrepancy
	// is found.

	if(oldinFile.IsFileStateOk() && oldinFile.Serialize())
	{
		// Check that the wall constraint is consistent 

		if(  (m_bWall && !oldinFile.pWD) || 
			(!m_bWall && oldinFile.pWD)  ||
			(m_bWall &&
			(WallPolymerName != oldinFile.pWD->GetWallPolymerName() ||
			 WallBeadDensity != oldinFile.pWD->GetWallBeadDensity() ||
			 WallXWidth      != oldinFile.pWD->GetWallXWidth()      ||
			 WallYWidth      != oldinFile.pWD->GetWallYWidth()      ||
			 WallZWidth      != oldinFile.pWD->GetWallZWidth())) )
		{
			return ErrorTrace("Wall keyword error in restart control data file");
		}

		if(	CNTXCellNo != oldinFile.CNTXCellNo ||
			CNTYCellNo != oldinFile.CNTYCellNo ||
			CNTZCellNo != oldinFile.CNTZCellNo )
		{
			return ErrorTrace("Box size error in restart control data file");
		}

		if(	CNTXCellWidth != oldinFile.CNTXCellWidth ||
			CNTYCellWidth != oldinFile.CNTYCellWidth ||
			CNTZCellWidth != oldinFile.CNTZCellWidth )
		{
			return ErrorTrace("Box cell width error in restart control data file");
		}

		if( AverageBeadDensity != oldinFile.AverageBeadDensity )
		{
			return ErrorTrace("Density keyword error in restart control data file");
		}

		if( kT != oldinFile.kT )
		{
			return ErrorTrace("Temp keyword error in restart control data file");
		}

		// We don't check the RNGSeed because we may want to change it 
		// between runs

#if SimIdentifier == BD
        // We leave Lambda in a BD simulation for compatibility of the CDF's
		if( Lambda != oldinFile.Lambda )
		{
			return ErrorTrace("Lambda keyword error in restart control data file");
		}
#elif SimIdentifier == DPD
		if( Lambda != oldinFile.Lambda )
		{
			return ErrorTrace("Lambda keyword error in restart control data file");
		}
#elif SimIdentifier == MD
		if( RCutOff != oldinFile.RCutOff )
		{
			return ErrorTrace("RCutOff keyword error in restart control data file");
		}
#endif

		// Check the number of bead, bond and polymer types and the fraction,
		// shape and head/tail bead names for each polymer. The names are
		// important because we want to preserve the mapping from beads in 
		// the old polymers to those in the new polymers.

		if(mNewBeadTypes.size()    != oldinFile.mNewBeadTypes.size() ||
		   mNewBondTypes.size()    != oldinFile.mNewBondTypes.size() ||
		   mNewPolymerTypes.size() != oldinFile.mNewPolymerTypes.size())
		{
			return ErrorTrace("Bead/Bond/Polymer error in restart control data file");
		}

		// Check all bead names agree. Note that the iterators here have to be
		// const because this function is a const member function.
		// Also note that the counter starts at 1 because it is used in 
		// CInputDPDFile and CInputMDFile to indicate how many types have been
		// created so far. CInputData however counts from 0.

		for(long beadNo=1; beadNo<=mNewBeadTypes.size(); beadNo++)
		{
			cLongBeadTypeIterator iterNewBead =           mNewBeadTypes.find(beadNo);
			cLongBeadTypeIterator iterOldBead = oldinFile.mNewBeadTypes.find(beadNo);

// The following test has been changed because I had to put brackets around
// the map iterators to get it to compile on the Cray. I need to dereference the
// iterators before calling the operator==.

			if(iterOldBead == oldinFile.mNewBeadTypes.end() || !(*(*iterOldBead).second == *(*iterNewBead).second))
			{
				return ErrorTrace("Bead type error in restart control data file");
			}
		}

		// Check all bond names agree

		for(long bondNo=1; bondNo<=mNewBondTypes.size(); bondNo++)
		{
			cLongBondTypeIterator iterNewBond =           mNewBondTypes.find(bondNo);
			cLongBondTypeIterator iterOldBond = oldinFile.mNewBondTypes.find(bondNo);

			if(iterOldBond == oldinFile.mNewBondTypes.end() || !(*(*iterOldBond).second == *(*iterNewBond).second))
			{
				return ErrorTrace("Bond type error in restart control data file");
			}
		}
		// Check all polymer data agrees

		for(long polyNo=1; polyNo<=mNewPolymerTypes.size(); polyNo++)
		{
			cLongPolymerTypeIterator iterNewPolymer =           mNewPolymerTypes.find(polyNo);
			cLongPolymerTypeIterator iterOldPolymer = oldinFile.mNewPolymerTypes.find(polyNo);
			
			if(iterOldPolymer == oldinFile.mNewPolymerTypes.end() || !(*(*iterOldPolymer).second == *(*iterNewPolymer).second))
			{
				return ErrorTrace("Polymer type error in restart control data file");
			}
		}

	}
	else
		return ErrorTrace("Error reading restart control data file");

	return bRestartOK;
}

// Function to set the data needed to impose a uniaxial shear on the simulation box.

// NOT IMPLEMENTED YET.

void CInputData::SetShearData()
{
	m_bShear = false;
}

// Function to return the set of force target names that have been created 
// as a result of ccSelectxxxInyy commands or ccAddxxxInyyy commands.

const StringSequence CInputData::GetForceTargetNames() const
{
	return m_vForceTargetNames;
}

// Function to return the set of command target names that have been 
// created as a result of ccSelectBeadTypeInRegion commands.

const StringSequence CInputData::GetCommandTargetNames() const
{
	return m_vCommandTargetNames;
}

// Function to return the set of active command target names. These
// are command targets that have been scheduled to receive a command
// that requires continuous updating during a simulation. One example
// of this type of commands are those that apply a force to the bead
// in a command target, e.g., ctConstantForceOnTarget.

const StringSequence CInputData::GetActiveCommandTargetNames() const
{
	return m_vACTNames;
}

// Function to return the set of command group names. 

const StringSequence CInputData::GetCommandGroupNames() const
{
	return m_vCommandGroupNames;
}

// Function to add the name of a force target that has been specified as a 
// result of ccSelectxxxInyy commands or ccAddxxxInyyy commands, and whose name 
// does not already occur in the set of defined force target names.

void CInputData::AddForceTargetName(const zString name) 
{
	m_vForceTargetNames.push_back(name);
}

// Function to add the name of a command target that has been specified
// as the result of a ccSelectBeadTypeInRegion command, and whose name
// does not already occur in the set of defined command target names.

void CInputData::AddCommandTargetName(const zString name)
{
	m_vCommandTargetNames.push_back(name);
}

// Function to add the name of a command target to the set of targets
// that require continuous updating, and  whose name does not already 
// occur in the set of defined command target names.

void CInputData::AddActiveCommandTargetName(const zString name)
{
	m_vACTNames.push_back(name);
}

// Function to add the name of a command group to the existing set of groups.
// Note that if the group already exists, the new one is not added but no
// return value is set. The add just fails.

void CInputData::AddCommandGroupName(const zString name)
{
    if(!IsCommandGroupPresent(name))
    {
	    m_vCommandGroupNames.push_back(name);
    }
}

// Function to add a name to the bead names map. This is used, for example, when
// the type of a set of beads in a command target is changed by command. This function
// does not define a new "type" of bead, i.e., no new CBeadStructure is required as 
// for AddBeadType, and it associates the numeric identifier "-1" with the bead name.
// This means that the name cannot be searched for in the mBeadTypes map using "-1"
// as the key as there may be several bead names with this type.
// It stores the name solely to prove uniqueness when changing bead type names.

bool CInputData::AddBeadNameToMap(const zString name)
{
	if(!IsNameValid(name) || (mBeadNames.find(name) != mBeadNames.end()))
		return false;
	else
	{
		mBeadNames.insert(zPairSL(name,-1));
	}

	return true;
}

// Wrapper function to the private string checking function.

bool CInputData::IsExternalNameValid(const zString name) const
{
	return IsNameValid(name);
}

// Function to return the  number of MC steps to perform prior to doing an MD run.

long CInputData::GetTotalMCTime() const
{
	return TotalMCTime;
}

// Function to return the maximum displacement of a bead in an MC step.
// The MC moves are isotropic so the same step size is used for all dimensions.

double CInputData::GetMCStepSize() const
{
	return MCStepSize;
}

// Member functions of the enclosed class LoopTarget.
//
// Each LoopTarget object represents a bead that has been designated as the
// target for a loop via the (/ 1 A) element. The number indicates the target
// id and the name is that of the bead represented by the target.
//

CInputData::LoopTarget::LoopTarget(long id, zString name, CAbstractBead* pBead) : m_id(id), m_Name(name), m_pBead(pBead)
{
}

CInputData::LoopTarget::~LoopTarget()
{
}

long CInputData::LoopTarget::GetId() const
{
	return m_id;
}

zString CInputData::LoopTarget::GetName() const
{
	return m_Name;
}

CAbstractBead* CInputData::LoopTarget::GetBead() const
{
	return m_pBead;
}

// **********************************************************************
//
// Group of functions to check that user-defined entities (Processes, 
// Agrregates, Events and Commands) contain valid data.
//
//
// Function to check that user-defined process objects have complete 
// and valid data appropriate to each type. We also assign aggregates
// and events to each process as determined by their id's specified in the
// process data and the order of creation.

bool CInputData::AreProcessesValid(ProcessObjectSequence vProcessObjects)
{
	// Copy Processes to local storage first: this actually creates a new
	// copy of each process to decouple them from those in the xxProcessObject
	// class.

	for(cProcessObjectIterator iterPO=vProcessObjects.begin(); iterPO!=vProcessObjects.end(); iterPO++)
	{
		m_vProcesses.push_back((*iterPO)->GetProcess());
	}

	// Find the aggregates and events for each process and store pointers to them
	// inside the xxProcess-derived classes. We cannot use a const iterator 
	// because the xxProcesses store some data while doing their validation.

	bool bValid = true;

	for(ProcessIterator iterProc=m_vProcesses.begin(); iterProc!=m_vProcesses.end(); iterProc++)
	{
		if( bValid && !(*iterProc)->ValidateData(*this) )
			bValid = false;
	}
	return bValid;
}


// Function to check that user-defined analysis objects have complete 
// and valid data appropriate to each type, and to copy the data defining 
// the aggregate analysis from the CAnalysisData-derived
// objects into newly-created CAnalysis-derived objects that will be used 
// in CMonitor to do the actual analysis. By copying the information into 
// new objects here we decouple the CInputxxxFile-created objects from the 
// rest of the code.
//
// We pass the runId and SamplePeriod data to the aggregate state objects 
// so that the CAnalysis-derived objects can serialize their temporal evolution
// data to files whose names are identified by the runId. The sample period is 
// used to allocate space for the data in a vector.

bool CInputData::AreAggregatesValid(AnalysisObjectSequence vAnalysisObjects)
{
	// Copy the aggregate objects to local storage first

	for(cAnalysisObjectIterator iterAnOb=vAnalysisObjects.begin(); iterAnOb!=vAnalysisObjects.end(); iterAnOb++)
	{
		m_vAggregates.push_back((*iterAnOb)->CreateAnalysis(runId, SamplePeriod));
	}

	// now send the ValidateData message to each aggregate object to pass on
	// to its contained CAnalysisData object

	bool bValid = true;

	for(AnalysisObjectIterator iterAgg=vAnalysisObjects.begin(); iterAgg!=vAnalysisObjects.end(); iterAgg++)
	{
		if( bValid && !(*iterAgg)->ValidateData(*this) )
			bValid = false;
	}
	return bValid;
}

// Function check that the data specified for events is valid and complete. The xxEvents
// are copied to local storage and then checked for validity.

bool CInputData::AreEventsValid(EventObjectSequence vEventObjects)
{
	// Copy Events to local storage first

	for(cEventObjectIterator iterEO=vEventObjects.begin(); iterEO!=vEventObjects.end(); iterEO++)
	{
		m_vEvents.push_back((*iterEO)->GetEvent());
	}

	// now check their data. We cannot use a const iterator because the xxEvents
	// store some data while doing their validation.

	bool bValid = true;

	for(EventIterator iterEvent=m_vEvents.begin(); iterEvent!=m_vEvents.end(); iterEvent++)
	{
		if( bValid && !(*iterEvent)->ValidateData(*this) )
			bValid = false;
	}
	return bValid;
}

// Function to copy the user-defined commands to the local command list and
// check that their data is complete and valid. It sends the IsDataValid 
// message to each command and leaves them to validate their own data.
// 
// Note that commands needed to turn the gravity, wall and shear constraints on 
// have already been added to the list in SetGravityData(), SetWallData() and 
// SetShearData(). We copy the objects rather than pointers to separate those
// objects created in the input data file and those stored in the simulation.
// They are deleted at different times and by different objects so cannot be
// pointers to the same set of objects.
// 
// Command validation falls into the following categories:
//
// Constraint commands:	if a constraint command has been entered check that the 
//                      constraint itself (Gravity, Wall or Shear) is active
//
// Selection commands:	if the user has defined a force target its name must be 
//						unique and it must specify a region within the SimBox.
//
// Add commands:		the target of an Add command must already exist
//
//
// ApplyForce commands: the target for the force command must exist
//

bool CInputData::AreCommandsValid(CommandObjectSequence vCommandObjects)
{
	// Copy commands to local storage first

	for(cCommandObjectIterator iterCO=vCommandObjects.begin(); iterCO!=vCommandObjects.end(); iterCO++)
	{
		m_lCommands.push_back((*iterCO)->GetCommand());
	}

	// now check their data

	bool bValid = true;

	for(cCommandIterator iterCom=m_lCommands.begin(); iterCom!=m_lCommands.end(); iterCom++)
	{
		if( bValid && !(*iterCom)->IsDataValid(*this) )
			bValid = false;
	}
	return bValid;
}

// Function that returns true if a specified force target already exists,
// which is checked by searching for the target's name, otherwise it returns false.

bool CInputData::IsForceTargetPresent(const zString label) const
{
	if(m_vForceTargetNames.empty() || 
        find(m_vForceTargetNames.begin(), m_vForceTargetNames.end(), label) == m_vForceTargetNames.end())
		return false;

	return true;
}

// Function that returns true if a specified command target already exists,
// which is checked by searching for the target's name, otherwise it returns 
// false. Note that we use the m_vCommandTargetNames container directly,
// rather than encapsulating it in the GetCommandTargetNames() function
// because the containers do not work when a function call is embedded in
// a find() algorithm.

bool CInputData::IsCommandTargetPresent(const zString label) const
{
	if(m_vCommandTargetNames.empty() || 
		find(m_vCommandTargetNames.begin(), m_vCommandTargetNames.end(), label) == m_vCommandTargetNames.end())
		return false;

	return true;
}

// Function that returns true if two active command targets, i.e., the
// decorator instances wrapping a command target, have the same identifier. 
// Note that a command target may be the recipient of many decorator commands, 
// such as several ctConstantForceOnTarget commands, but each decorator must 
// have a unique identifying label.

bool CInputData::IsActiveCommandTargetPresent(const zString label) const
{
	if(m_vACTNames.empty() || 
		find(m_vACTNames.begin(), m_vACTNames.end(), label) == m_vACTNames.end())
		return false;

	return true;
}

// Function that returns true if a new command group has the same name as
// an existing one.

bool CInputData::IsCommandGroupPresent(const zString label) const
{
	if(m_vCommandGroupNames.empty() || 
		find(m_vCommandGroupNames.begin(), m_vCommandGroupNames.end(), label) == m_vCommandGroupNames.end())
		return false;

	return true;
}

// Function to return the shape string of a specified polymer type.
// We check that the polymer type specified is valid before returning 
// a const zString holding the shape.

const zString CInputData::GetPolymerTypeShape(long polyType) const
{
	if(polyType >= 0 && polyType < m_mPolymerShapes.size())
	{
		cLongStringIterator iterPoly = m_mPolymerShapes.find(polyType);

		return (*iterPoly).second;
	}
	else
		return "";
}

// Overloaded function to return the shape string of a specified polymer
// type given its name. We check that the polymer name has been defined
// in the polymer types map and return an empty string if it has not.

const zString CInputData::GetPolymerTypeShape(const zString polyName) const
{
	if(IsPolymerinMap(polyName))
	{
		cStringLongIterator iterPoly = GetPolymerNamesMap().find(polyName);

		return GetPolymerTypeShape((*iterPoly).second);
	}
	else
		return "";
}

// Function to return the number of beads in a specified polymer type.
// We check that the polymer type specified is valid before returning 
// the number of beads in its shape.

long CInputData::GetPolymerTypeSize(long polyType) const
{
	if(polyType >= 0 && polyType < m_mPolymerSizes.size())
	{
		cLongLongIterator iterPoly = m_mPolymerSizes.find(polyType);

		return (*iterPoly).second;
	}
	else
		return 0;
}

// Overloaded function to return the number of beads in a polymer type
// whose name is given. We check that the polymer name has been defined
// in the polymer types map and return 0 for the size if it has not.

long CInputData::GetPolymerTypeSize(const zString polyName) const
{
	if(IsPolymerinMap(polyName))
	{
		cStringLongIterator iterPoly = GetPolymerNamesMap().find(polyName);

		return GetPolymerTypeSize((*iterPoly).second);
	}
	else
		return 0;
}

// Function to return the number fraction of a specified polymer type 
// given its numberic type. We check that the polymer type specified 
// is valid before returning the fraction.

double CInputData::GetPolymerTypeFraction(long polyType) const
{
	if(polyType >= 0 && polyType < m_mPolymerFractions.size())
	{
		cLongDoubleIterator iterPoly = m_mPolymerFractions.find(polyType);

		return (*iterPoly).second;
	}
	else
		return 0.0;
}

// Overloaded function to return the number fraction of a specified polymer type 
// given its string identifier. 

double CInputData::GetPolymerTypeFraction(const zString polyName) const
{
	if(IsPolymerinMap(polyName))
	{
		cStringLongIterator iterPoly = GetPolymerNamesMap().find(polyName);

		return GetPolymerTypeFraction((*iterPoly).second);
	}
	else
		return 0.0;
}


// Function used to fill up the instance with data retrieved from the parallel 
// code's pmInputData instance. Because this function is only called in the 
// parallel code, we only compile it in for that case. This function must be
// called before pmPolymerData so that the parameters defining the simulation
// space are available when creating polymers in the latter function, and 
// because the expected number of bead types, etc are required in order pass
// the datatypes to PN. These values are stored in local members called
// m_ExpectedBeadTypeTotal, etc, so that PN can distinguish the expected 
// number of datatypes from the actual number created.
//
// Note that we don't pass in the title, date and comment as these are only
// used when writing out the protocol file and all file IO is done on P0.
// We also don't try to pass the initial state data here: this is done
// by the ReceiveISData() function.

void CInputData::ReceiveInputData(const pmInputData* const pMsg)
{
#if SimMPS == SimulationEnabled
//	SetGravityData(inFile.pGD);
//	SetWallData(inFile.pWD);

    SetExpectedBeadTypeTotal(pMsg->GetBeadTypeTotal());
    SetExpectedBondTypeTotal(pMsg->GetBondTypeTotal());
    SetExpectedBondPairTypeTotal(pMsg->GetBondPairTypeTotal());
    SetExpectedPolymerTypeTotal(pMsg->GetPolymerTypeTotal());

    SetProcessorsXNo(pMsg->GetProcessorsXNo());
    SetProcessorsYNo(pMsg->GetProcessorsYNo());
    SetProcessorsZNo(pMsg->GetProcessorsZNo());

	SetCNTXCellNo(pMsg->GetCNTXCellNo());
	SetCNTYCellNo(pMsg->GetCNTYCellNo());
	SetCNTZCellNo(pMsg->GetCNTZCellNo());

	SetCNTXCellWidth(pMsg->GetCNTXCellWidth());
	SetCNTYCellWidth(pMsg->GetCNTYCellWidth());
	SetCNTZCellWidth(pMsg->GetCNTZCellWidth());

	SetAveBeadDensity(pMsg->GetBeadDensity());
	SetTemp(pMsg->GetTemperature());
	SetRNGSeed(pMsg->GetRNGSeed());

#if SimIdentifier == BD
	SetLambda(pMsg->GetLambda());
#elif SimIdentifier == DPD
	SetLambda(pMsg->GetLambda());
#elif SimIdentifier == MD
	SetCutOffRadius(pMsg->GetRCutOff());
	SetMCStepSize(pMsg->GetMCStepSize());
	SetTotalMCTime(pMsg->GetTotalMCTime());
#endif

	SetStepSize(pMsg->GetStepSize());
	SetTotalTime(pMsg->GetTotalTime());
	SetSamplePeriod(pMsg->GetSamplePeriod());
	SetAnalysisPeriod(pMsg->GetAnalysisPeriod());
	SetDensityPeriod(pMsg->GetDensityPeriod());
	SetDisplayPeriod(pMsg->GetDisplayPeriod());
	SetRestartPeriod(pMsg->GetRestartPeriod());
	SetGridXCellNo(pMsg->GetGridXCellNo());
	SetGridYCellNo(pMsg->GetGridYCellNo());
	SetGridZCellNo(pMsg->GetGridZCellNo());
#endif
}

// Functions used to create the beads, bonds, polymers etc, in a parallel simulation 
// on the PN processors. These functions replace the CreateNewTypes() function
// used in the serial code and on P0.
//
// They use data from the pmXXXData classes and require that pmInputData has 
// already been received and processed. Because these functions 
// are only called in the parallel code, we only compile them in for that case.
//
// A DPD Bead Type is defined by the following data items:
//
// String name
// Numeric type
// DPD cut-off radius
// DPD LG cut-off radius (for LG simulations only, otherwise ignored)
// Conservative interaction vector (with size equal to the number of existing bead types)
// Dissipative interaction vector (size as above)
// LG interaction vector (size as above, ignored except for LG DPD)
// 
// This function must only be called on PN as it relies on the CBeadType
// static member m_BeadTypeTotal being initially zero and incrementing it
// by one for each bead type received.

void CInputData::ReceiveBeadTypeData(const pmBeadData* const pMsg)
{
#if SimMPS == SimulationEnabled

    // First, unpack the bead data and then store it in a generic CBeadType instance

    const zString name = pMsg->GetBeadName();
    long          type = pMsg->GetBeadType();
    double      radius = pMsg->GetRadius();
    zDoubleVector vConsInt = pMsg->GetConsInt();
    zDoubleVector vDissInt = pMsg->GetDissInt();

    // Now add the new bead type using AddBeadType() that unpacks the CDPDBeadStructure
    // and handles the actual type creation. Note that we don't bother to check if
    // the bead data are valid as P0 has already done this before sending the data
    // to PN. 
    // We store the new bead type instances in a local container so that they can
    // be destoyed when CInputData is destroyed: in the serial code, the bead types
    // are owned by the CInputDPDFile instance, but PN never creates this object.

	CBeadType* pBead = CBeadType::AddDPDBeadType(name, radius, vConsInt, vDissInt);

    m_vBeadTypesPN.push_back(pBead);

    // To allow for extensions we check the simulation type here
#if SimIdentifier == DPD
	bool newTypeAdded = AddBeadType(name, pBead->GetDPDStructure());
#endif

    // Debug output to check messaging:

/*
    std::cout << "Processor " << xxParallelBase::GlobalGetRank() << " receiving bead data: " << name << " " << type << " " << radius << zEndl;
    for(long i=0; i<pMsg->GetTypeTotal(); i++)
    {
        std::cout << " " << vConsInt.at(i);
    }
    std::cout << zEndl;
    for(long j=0; j<pMsg->GetTypeTotal(); j++)
    {
        std::cout << " " << vDissInt.at(j);
    }
    std::cout << zEndl << zEndl;

*/
#endif
}

void CInputData::ReceiveBondTypeData(const pmBondData* const pMsg)
{
#if SimMPS == SimulationEnabled

    const zString headName = pMsg->GetHeadName();
    const zString tailName = pMsg->GetTailName();
    double          spring = pMsg->GetSpringConst();
    double          length = pMsg->GetLength();

//    std::cout << "Processor receiving bond data: " << headName << " " << tailName << " " << spring << " " << length << zEndl;

    AddBondType(headName, tailName, spring, length);
    
#endif
}

void CInputData::ReceiveBondPairTypeData(const pmBondPairData* const pMsg)
{
#if SimMPS == SimulationEnabled

    const zString  firstName = pMsg->GetFirstName();
    const zString secondName = pMsg->GetSecondName();
    const zString  thirdName = pMsg->GetThirdName();
    double           modulus = pMsg->GetModulus();
    double              phi0 = pMsg->GetPhi0();

//    std::cout << "Processor receiving bondpair data: " << firstName << " " << secondName << " " << thirdName << " " << modulus << " " << phi0 << zEndl;

    AddBondPairType(firstName, secondName, thirdName, modulus, phi0);
    
#endif
}

void CInputData::ReceivePolymerTypeData(const pmPolymerData* const pMsg)
{
#if SimMPS == SimulationEnabled

    // Add a new polymer type from the data in the message using AddPolymerType():
    // which overload we use depends on whether the polymer's head and tail
    // bead names have been specified or not.

    const zString      name = pMsg->GetName();
    const zString     shape = pMsg->GetShape();
    double         fraction = pMsg->GetFraction();
    const zString  headName = pMsg->GetHeadName();
    const zString  tailName = pMsg->GetTailName();

//    std::cout << "Processor receiving polymer data: " << name << " " << shape << " " << fraction << " ( head bead " << headName << " tail bead " << tailName << " )" << zEndl;

    // At this point we have to check if the head and tail bead names have been
    // specified; if so, they must be unique. Because polymers added to P0's
    // map will have already been checked, we only have to see if unique
    // head and tail bead names are specified, if so we call the appropriate overloaded
    // AddPolymerType function. Note that because of the way the polymer message
    // data is derived from the polymer type, head and tail bead names may
    // exist even if they are the same. This is not an error here as P0 must have
    // already checked that unique head/tail names were actually specified in 
    // the CDF, and the non-unique ones are created by the AddPolymerType functions.

    if(!headName.empty() && !tailName.empty()  && headName != tailName &&
        IsBeadUniqueInPolymer(headName,shape) && IsBeadUniqueInPolymer(tailName,shape))
    {
        if(!AddPolymerType(name, shape, fraction, headName, tailName))
        {
           // Handle error condition here
           std::cout << "CInputData failed to add polymer type/head-tail " << name << zEndl;
        }
    }
    else
    {
        if(!AddPolymerType(name, shape, fraction))
        {
           // Handle error condition here
           std::cout << "CInputData failed to add polymer type/no head-tail " << name << zEndl;
        }
    }

#endif
}

// Function to check that the number of processes assigned to each dimension is non-zero
// if a parallel simulation is being performed.

bool CInputData::AreParallelProcessorsValid() const
{
    bool bValid = true;

#if SimMPS == SimulationEnabled
    if(GetProcessorsXNo() < 1 || GetProcessorsYNo() < 1 || GetProcessorsZNo() < 1)
    {
        bValid = false;
    }
#endif

    return bValid;
}

// Function to perform validation on the initial state for both the serial 
// and parallel versions of the code. All classes derived from CInitial StateData
// that are parallel-aware must provide two validation functions that are 
// called from this function. Note that this function cannot be const as some
// of the ISD classes add information to the CInputData instance as they are
// validating the IS.

bool CInputData::IsInitialStateValid()
{
    bool bValid = true;

#if SimMPS == SimulationEnabled
    bValid = m_pISD->ValidateDataP(*this);
#else
    bValid = m_pISD->ValidateData(*this);
#endif

    return bValid;
}

// Function to receive the message that holds data defining the initial state and
// pass it to the appropriate ISD instance so that it can unpack its data.
// 
// All initial state messages derive from mpmInitialState, and we use the  
// string identifier stored in each concrete message class to create the appropriate 
// CInitialStateData instance and pass it the message instance so that the
// concrete ISD classes can unpack their data.
// 
// Note that we don't check for the validity of the IS here as P0 must have already
// done this.

void CInputData::ReceiveISData(const mpmInitialState* const pMsg)
{
#if SimMPS == SimulationEnabled
	CInitialStateData* const pISD = acfInitialStateFactory::Instance()->Create(pMsg->GetISType());

    pISD->SetData(pMsg);

    SetISD(pISD);
#endif
}

// Function to receive the set of command names for all commands that are
// scheduled for execution in the CDF.

void CInputData::ReceiveCommandNames(const pmCommandNames* const pMsg)
{
#if SimMPS == SimulationEnabled
    StringSequence names = pMsg->GetCommandNames();

    for(cStringSequenceIterator is=names.begin(); is!=names.end(); is++)
    {
        m_vCommandNames.push_back(*is);
    }
#endif
}

// Function to return the names of those commands that are scheduled for 
// execution in the CDF.

const StringSequence CInputData::GetCommandNames() const
{
    return m_vCommandNames;
}



