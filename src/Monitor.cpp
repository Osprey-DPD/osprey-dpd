/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// Monitor.cpp: implementation of the CMonitor class.
//
//////////////////////////////////////////////////////////////////////
//
// Class Description
// *****************
//
// The singleton CMonitor class collects data from the CSimBox during the simulation
// and calculates all observables from the bead, bond and polymer coordinates.
// It stores the results in CObservable-derived objects and xxState-derived objects 
// for serialization to file. 
//
// Time series data are packaged in the class CTimeSeriesData and passed to the 
// CHistoryState object to record the progress of the simulation as well as for 
// analysing time-dependent observables. The CTimeSeriesData objects are stored in 
// the CHistoryState object and deleted by it when the simulation ends. This is
// to try and encapsulate the data inside the object that uses it and to reduce the
// amount of data stored in CMonitor.
//
// The CObservable objects contain data more conveniently averaged over (possibly) 
// long periods in the simulation, such as the density field display of bead motion.
//
// Simulation properties are divided into several categories:
//
//		single bead data
//		single bond data
//		polymer statistics
//		grid-based averages over bead data
//		averages over bead, bond or polymer types
//		averages over all beads in a given volume of space
//
//
// Each observable, Ob say, is calculated and stored locally in the CMonitor object 
// as m_meanOb, m_varOb before being written to a file.
//
// The CMonitor class is also responsible for saving bead and bond coordinates to
// file for display as a snapshot offline and the restart states used to repeat or
// alter a previous simulation. It can be the recipient of commands entered by the
// user in the control data file. These may be used to modify the analysis and display
// of data during a run. An example of such a command is the ToggleBeadDisplay 
// (Stored in mcToggleBeadDisplay.h, .cpp, the prefix mc indicates Monitor Command).
//
// xxState-derived classes
// ***********************
//
//	CSimState				- Composition of: CAnalysisState
//											  CInitialState
//											  CHistoryState
//							- This is used to package the data needed to start a simulation.
//						      A valid CSimState can only be created from these three objects,
//							  and is then used to define the CSimBox object in a valid initial
//							  configuration. It also provides the path by which the 
//							  CMonitor has access to all data about the simulation setup.
//											
//	CAnalysisState
//	CInitialState
//	CHistoryState
//
//	CCurrentState
//	CDensityState
//	CObservableState		- unused
//	CRestartState
//  CInclusiveRestartState  - updated version of the restart state class that allows
//                            dynamically-modified data (e.g., command targets) to be
//                            serialised to/from restart state files.
//	
//
//////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "SimDefs.h"
#include "SimMPSFlags.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimFunctionalFlags.h"
#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"
#include "Polymer.h"
#include "Monitor.h"
#include "CurrentState.h"
#include "DensityState.h"
#include "InclusiveRestartState.h"
#include "RestartState.h"
#include "SimState.h"
#include "ISimBox.h"

#include "aaScalarSingle.h"
#include "aaVectorSingle.h"
#include "aaTensorSingle.h"
#include "GridObservable.h"
#include "ScalarObservable.h"
#include "TensorObservable.h"
#include "VectorObservable.h"
#include "TimeSeriesData.h"
#include "xxProcess.h"

// Classes used to format the output for different visualisation tools

#include "PovrayFormat.h"
#include "AmiraFormat.h"
#include "ParaviewFormat.h"
#include "SolventFreeFormat.h"

// Parallel code include files

#if SimMPS == SimulationEnabled
#include "pmCurrentState.h"
#endif

#if EnableStressTensorSphere == SimMiscEnabled
#include "CNTCell.h"  // Needed for the stress tensor in curvilinear coordinates
#endif

// Aggregate analysis base class header file

#include "Analysis.h"

// Region analysis base class and related region analysis classes

#include "IRegionAnalysis.h"
#include "SliceEnergy.h"
#include "CNTCellSlice.h"

// Density field analysis classes

#include "Bead1dProfile.h"

// Log file message classes

#include "Logud.h"
#include "LogTextMessage.h"
#include "LogNegativeBeadDiffusionCoefficient.h"

// STL include files



//////////////////////////////////////////////////////////////////////
// Static member variable holding a pointer to the single instance of CMonitor.
// Note that it does not get assigned a value until the user creates the
// instance.

CMonitor* CMonitor::m_pInstance = NULL;

// Public member function to create a single instance of the CMonitor class.

CMonitor* CMonitor::Instance(CSimState *const psState, const ISimBox* const pISimBox)
{
	if( !m_pInstance)
	{
		m_pInstance = new CMonitor(psState, pISimBox);
	}

	return m_pInstance;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonitor::CMonitor(CSimState* const psState, const ISimBox* const pISimBox) : ISimBoxBase(pISimBox), 
													   m_pSimState(psState),
													   m_SimBoxXLength(psState->GetSimBoxXLength()),
													   m_SimBoxYLength(psState->GetSimBoxYLength()),
													   m_SimBoxZLength(psState->GetSimBoxZLength()),
													   m_GridXCellNo(psState->GetGridXCellNo()),
													   m_GridYCellNo(psState->GetGridYCellNo()),
													   m_GridZCellNo(psState->GetGridZCellNo()),
													   m_BeadTypeSize(psState->GetBeadTypeTotal()),
													   m_BondTypeSize(psState->GetBondTypeTotal()),
													   m_BondPairTypeSize(psState->GetBondPairTypeTotal()),
													   m_PolymerTypeSize(psState->GetPolymerTypeTotal()),
													   m_StressBeadPairTotal(m_BeadTypeSize*m_BeadTypeSize),
													   m_StressSliceTotal(0),
													   m_LowerStressSliceIndex(0),
													   m_UpperStressSliceIndex(0),
													   m_LowerStressSliceCoord(0.0),
													   m_UpperStressSliceCoord(0.0),
													   m_LowerStressSliceRem(0.0),
													   m_UpperStressSliceRem(0.0),
													   m_StressSliceWidth(0.0),
													   m_invSliceWidth(0.0),
													   m_FixedObservableNo(0),
													   m_RunCompleteInterval(0),
													   m_MinXFraction(0.0),
													   m_MinYFraction(0.0),
													   m_MinZFraction(0.0),
													   m_MaxXFraction(1.0),
													   m_MaxYFraction(1.0),
													   m_MaxZFraction(1.0),
                                                       m_DefaultCurrentStateFormat("Povray"),
                                                       m_bCurrentStateAnalysis(false),
													   m_bDisplayBox(true),
													   m_bRestrictCurrentStateCoords(false),
													   m_bDensityGridOutput(false),
													   m_bEnergyOutput(false),
													   m_bLogRestartWarningMessages(false),
													   m_bNormalizePerBead(false),
                                                       m_bInclusiveRestartStates(true)

{

	// **********************************************************************	
	// store some  variables locally for normalisation purposes. Note that there
	// must always be some beads and polymers (even if they consist only of
	// single beads) but there don't have to be any bonds.

	// We use the m_SamplesTaken counter to indicate when analysis should be
	// performed and averages calculated.

    // Local counters for initialisation only

    const long samplePeriod   = m_pSimState->GetSamplePeriod();
    const long analysisPeriod = m_pSimState->GetAnalysisPeriod();
    const long densityPeriod  = m_pSimState->GetDensityPeriod();

	m_SamplesTaken	    = 0;
	m_SampleNo		    = analysisPeriod/samplePeriod;
	m_SampleSize	    = static_cast<double>(m_SampleNo);
	m_GridSampleNo		= densityPeriod/samplePeriod;

	m_HalfSimBoxXLength = 0.5*m_SimBoxXLength;
	m_HalfSimBoxYLength = 0.5*m_SimBoxYLength;
	m_HalfSimBoxZLength = 0.5*m_SimBoxZLength;

	m_invSimBoxVolume	= 1.0/(m_SimBoxXLength*m_SimBoxYLength*m_SimBoxZLength);

	// Define the width of the analysis grid cells from the simulation box size

	m_GridXCellWidth = m_SimBoxXLength/static_cast<double>(m_GridXCellNo);
	m_GridYCellWidth = m_SimBoxYLength/static_cast<double>(m_GridYCellNo);
	m_GridZCellWidth = m_SimBoxZLength/static_cast<double>(m_GridZCellNo);

	// **************************************************
	// Set the vector of numbers of bead of each type initially to zero.
	// This is important because the copy algorithm does not increase the
	// container size to hold extra elements. Also store the number of
	// bead types locally to avoid having to fetch the value from the CInitialState
	// object throughout the simulation.
	// Note that the functions CInitialState::GetBeadTypeTotal() returns the number
	// of bead types, but the function CInitialState::GetBeadTypeTotals() returns
	// a vector holding the number of beads of each type created.

	m_BeadTotal	= static_cast<double>(m_pSimState->GetBeadTotal());

	// Copy the numbers of each bead type into a local vector 

	m_vBeadTypeTotal.resize(m_BeadTypeSize);

	std::copy( (m_pSimState->GetInitialState()).GetBeadTypeTotals().begin(), 
				(m_pSimState->GetInitialState()).GetBeadTypeTotals().end(), 
				m_vBeadTypeTotal.begin());

	// Initialise the map containing (beadId, displayId) pairs used in colouring
	// beads in the current state snapshots.

	CCurrentState::ClearBeadDisplayIdMap();

	AbstractBeadVector vAllBeads = GetSimBoxBeads();
	for(cAbstractBeadVectorIterator citerBead=vAllBeads.begin(); citerBead!=vAllBeads.end(); citerBead++)
	{
		CCurrentState::SetBeadDisplayId((*citerBead)->GetId(), (*citerBead)->GetType());
	}

	// **************************************************
	// Store the number of bonds (if any) locally

	if(m_pSimState->GetBondTotal() > 0)
	{
		m_vBondTypeTotal.resize(m_BondTypeSize);

		std::copy( (m_pSimState->GetInitialState()).GetBondTypeTotals().begin(), 
				   (m_pSimState->GetInitialState()).GetBondTypeTotals().end(), 
					m_vBondTypeTotal.begin());
	}

	// **************************************************
	// Store the number of bondpairs (if any) locally

	if(m_pSimState->GetBondPairTotal() > 0)
	{
		m_vBondPairTypeTotal.resize(m_BondPairTypeSize);

		std::copy( (m_pSimState->GetInitialState()).GetBondPairTypeTotals().begin(), 
				   (m_pSimState->GetInitialState()).GetBondPairTypeTotals().end(), 
					m_vBondPairTypeTotal.begin());
	}

	// **************************************************
	// Store the number of polymers of each type locally so that we can normalize
	// polymer observables without having to access the CInitialState object

	// Copy the numbers of each polymer type into a local vector 

	m_vPolymerTypeTotal.resize(m_PolymerTypeSize);

	copy( (m_pSimState->GetInitialState()).GetPolymerTypeTotals().begin(), 
			   (m_pSimState->GetInitialState()).GetPolymerTypeTotals().end(), 
				m_vPolymerTypeTotal.begin());

	// Zero the vectors holding the data on the bead, bond and polymer observables
	// that depend on the numbers of types of each object. Currently, these are:
	//
	//	Bead mean-square distance moved used to calculate the diffusion coefficient 
	//	for each bead type
	//
	//  Bond length for each kind of bond
	//
	//  Bondpair Legendre polynomial, bond lengths and ratio of mean-square to the
	//  square of the mean angle.
	//
	//	Polymer end-to-end distance

	m_vBeadMSD.resize(m_BeadTypeSize);

	m_vBondLength.resize(m_BondTypeSize);

	m_vBondPairFirstLength.resize(m_BondPairTypeSize);
	m_vBondPairSecondLength.resize(m_BondPairTypeSize);
	m_vBondPairP2.resize(m_BondPairTypeSize);
	m_vBondPairPhiRatio.resize(m_BondPairTypeSize);
	m_vBondPairPhiSq.resize(m_BondPairTypeSize);
	m_vBondPairPhi.resize(m_BondPairTypeSize);

	m_vPolymerEE.resize(m_PolymerTypeSize);

	// **********************************************************************
	// Store pointers to the CAnalysis-derived objects that represent the
	// aggregates specified in the control data file so that we can send 
	// them a message to update their state throughout the simulation. 
	// These objects correspond to the types of aggregate we are going to 
	// search for and analyse during the simulation.
	//
	// The following kinds of aggregate can be analysed:
	//
	//  CInterface		Planar interface between two polymeric fluids
	//  CBilayer		Bilayer formed from a specific polymer
	//
	// Also store the xxProcess-derived objects that use aggregates and
	// events to create data corresponding to physical processes in the 
	// simulation. Each process has an associated xxProcessState class that
	// is used to serialize its data to a process file. We store the
	// process state objects locally to the monitor so that we can pass
	// the runId and other data to the xxFile base when creating them.
	//

	m_vAggregates.clear();


	std::copy(m_pSimState->GetAnalysisState().GetAggregates().begin(), 
			  m_pSimState->GetAnalysisState().GetAggregates().end(), 
			  std::back_inserter(m_vAggregates));


	// **********************************************************************
	// Create the CObservable objects and store pointers to them in containers
	// for passing to other routines. There are several vectors of observables:
	//
	// m_vObservables			observables independent of bead, bond, polymer numbers
	// m_vBondObservables		observables for each bond type
	// m_vBondPairObservables	observables for each bondpair type
	// m_vPolymerObservables	observables for each polymer type
	//
	// The observables are accessed via their position in the vector, 
	// m_vObservables.at(i), so this must be known to get the observable's value.
	// 
	// To simplify serialising the data, we concatenate all the observables into
	// one container, m_vAllObservables, before passing it to the CAnalysisState.
	//
	// Id	Type	    Name				Definition
	// ***************************************************
	//
	// m_vObservables
	// *************
	//
	//  0	Scalar	Temperature				average bead kinetic energy
	//	1	Scalar	Pressure				virial equation
	//  2   Vector	CM momentum				averaged over all movable beads (not wall beads)
	//  3	Vector	CM position					"
	//	4	Tensor	Stress					averaged over all movable beads
	//	5	Tensor	Stress Sphere			averaged over all movable beads in spherical polar coordinates
	//  6	Tensor	Inertia					all elements of tensor
	//	7	Scalar	Bond length				averaged over all bonds
	//
	//  The following observables depend on the number of bond and polymer types.
	//  Because the arrays  are	zero-indexed we can use the length of the vector  
	//  as the position for the next observable to be stored.
	//
	//  m_vBondObservables
	//  ******************
	//
	//  0 to m_BondTypeSize									
	//      Scalar  Bond Length			bond length by type
	//
	//  m_vBondPairObservables
	//  **********************
	//
	//  0 to m_BondPairTypeSize
	//      Scalar						stiff bond lengths, angles and order parameters	
	//
	//  m_vPolymerObservables
	//  *********************
	//
	//  0 to m_PolymerTypeSize
	//      Scalar	EE						polymer head-tail distance
	//

	m_vObservables.clear();
	m_vBondObservables.clear();
	m_vBondPairObservables.clear();
	m_vPolymerObservables.clear();

	CScalarObservable* pTemp			= new CScalarObservable("Temperature",	analysisPeriod, samplePeriod);
	CScalarObservable* pPress			= new CScalarObservable("Pressure",		analysisPeriod, samplePeriod);
	CVectorObservable* pcmMom			= new CVectorObservable("CM Mom",		analysisPeriod, samplePeriod);
	CVectorObservable* pcmPos			= new CVectorObservable("CM Pos",		analysisPeriod, samplePeriod);
	CTensorObservable* pStress			= new CTensorObservable("Stress",		analysisPeriod, samplePeriod);
	CTensorObservable* pStressSphere	= new CTensorObservable("Stress Spherical",	 analysisPeriod, samplePeriod);
	CTensorObservable* pInertia			= new CTensorObservable("Inertia",		analysisPeriod, samplePeriod);
	CScalarObservable* pBondLength		= new CScalarObservable("Bond Length",	analysisPeriod, samplePeriod);

	m_vObservables.push_back(pTemp);			
	m_vObservables.push_back(pPress);
	m_vObservables.push_back(pcmMom);
	m_vObservables.push_back(pcmPos);
	m_vObservables.push_back(pStress);
	m_vObservables.push_back(pStressSphere);
	m_vObservables.push_back(pInertia);
	m_vObservables.push_back(pBondLength);

	// Set the fixed observable number to allow the observables depending on bond and
	// polymer types to be stored after them.

	m_FixedObservableNo = m_vObservables.size();

	// Add an observable for each bond type to hold the mean bond length data

	zString bondName;
	zString bondTitle;
	
	for(long bondType=0; bondType != m_BondTypeSize; bondType++)
	{
		bondName  = m_pSimState->GetAnalysisState().GetBondNameFromType(bondType);
		bondTitle = bondName + " bond length";
		CScalarObservable* pBL = new CScalarObservable(bondTitle, analysisPeriod, samplePeriod);
		m_vBondObservables.push_back(pBL);
	}

	// Add an observable for each polymer type to measure the polymer's head-tail distance.
	// Recall that the beads that represent the head and tail are by default the first and 
	// last in the polymer shape or are used-defined in the input data control file

	zString polymerName;
	zString polymerTitle;
	
	for(long polyType=0; polyType != m_PolymerTypeSize; polyType++)
	{
		polymerName  = m_pSimState->GetAnalysisState().GetPolymerNameFromType(polyType);
		polymerTitle = polymerName + " EE distance";
		CScalarObservable* pEEDist = new CScalarObservable(polymerTitle, analysisPeriod, samplePeriod);
		m_vPolymerObservables.push_back(pEEDist);
	}

	// Add an observable for each bond pair type. We add these after the polymer data
	// to avoid having to redo the order of the observables.

	zString bpName;
	zString bpTitle1, bpTitle2, bpTitle3, bpTitle4;
	
	for(long bpType=0; bpType<m_BondPairTypeSize; bpType++)
	{
		bpName  = m_pSimState->GetAnalysisState().GetBondPairNameFromType(bpType);
		bpTitle1 = bpName + " P2(x)";
		bpTitle2 = bpName + " first bond length";
		bpTitle3 = bpName + " second bond length";
		bpTitle4 = bpName + " <phi**2>/<phi>**2";

		CScalarObservable* pBPP2			= new CScalarObservable(bpTitle1, analysisPeriod, samplePeriod);
		CScalarObservable* pBPFirstLength	= new CScalarObservable(bpTitle2, analysisPeriod, samplePeriod);
		CScalarObservable* pBPSecondLength	= new CScalarObservable(bpTitle3, analysisPeriod, samplePeriod);
		CScalarObservable* pBPPhiRatio		= new CScalarObservable(bpTitle4, analysisPeriod, samplePeriod);

		m_vBondPairObservables.push_back(pBPP2);
		m_vBondPairObservables.push_back(pBPFirstLength);
		m_vBondPairObservables.push_back(pBPSecondLength);
		m_vBondPairObservables.push_back(pBPPhiRatio);
	}

	// Copy the observables into a single container and pass it to the 
	// CAnalysisState object for serialization. We pass a pointer to the 
	// vector to avoid calling the copy constructor.

	m_vAllObservables.clear();

	copy(m_vObservables.begin(),         m_vObservables.end(),         back_inserter(m_vAllObservables));
	copy(m_vBondObservables.begin(),     m_vBondObservables.end(),     back_inserter(m_vAllObservables));
	copy(m_vPolymerObservables.begin(),  m_vPolymerObservables.end(),  back_inserter(m_vAllObservables));
	copy(m_vBondPairObservables.begin(), m_vBondPairObservables.end(), back_inserter(m_vAllObservables));

	m_pSimState->GetAnalysisState().AddGlobalData(&m_vAllObservables);


	// Now create a CGridObservable for each bead type to hold the density fields.
	// Note that the last argument is a label to identify the output file for
	// each field and we use the bead name obtained from a map of bead types to
	// their names created in the CAnalysisState object.
	
	// Within the same loop we zero the elements in the excluded bead type vector.
	// When a RemoveBeadFromCurrentState command is received it puts a 1 in the
	// appropriate element of this vector to indicate that all subsequent
	// current states shoudl not output the coordinates of that bead type until
	// another command reverses the effect.

	m_vGridObservables.clear();

	for(long beadType=0; beadType != m_BeadTypeSize; beadType++)
	{
		zString beadName = m_pSimState->GetAnalysisState().GetBeadNameFromType(beadType);

		CGridObservable*   pDensity	= new CGridObservable("Bead Density", densityPeriod, samplePeriod,
														   m_GridXCellNo, m_GridYCellNo, m_GridZCellNo, 
														   m_GridXCellWidth, m_GridYCellWidth, m_GridZCellWidth, 
														   m_pSimState->GetBeadTotal(), beadName, beadType);
		m_vGridObservables.push_back(pDensity);

		m_ExcludedBeadTypes.push_back(0);
	}

	// **********************************************************************

	// Analysis classes derived from IRegionAnalysis are also stored in CMonitor, but
	// as they are only created as the result of mcToggleXXXOutput commands we
	// only zero the container here.

	m_vCurrentStates.clear();
	m_vDensityStates.clear();
	m_Regions.clear();
	m_DensityFields.clear();

	// zero local variables holding observable values

	m_meanTemp				= 0.0;
	m_meanPress				= 0.0;
	m_meanKineticEnergy		= 0.0;
	m_meanPotentialEnergy	= 0.0;

	for(short int i=0; i<3; i++)
	{
		m_cmPos[i]				    = 0.0;
		m_cmMom[i]				    = 0.0;
		m_totalSqMom[i]			    = 0.0;
		m_totalAngMom[i]		    = 0.0;
		m_totalStress[3*i]		    = 0.0;
		m_totalStress[3*i+1]	    = 0.0;
		m_totalStress[3*i+2]	    = 0.0;
		m_totalStressSphere[3*i]    = 0.0;
		m_totalStressSphere[3*i+1]	= 0.0;
		m_totalStressSphere[3*i+2]	= 0.0;
		m_totalInertia[3*i]		    = 0.0;
		m_totalInertia[3*i+1]	    = 0.0;
		m_totalInertia[3*i+2]	    = 0.0;
	}

	// Set up the storage for the stress tensor analysis on slices through the 
	// SimBox. This is used, for example,  by CBilayer aggregate objects to get 
	// the surface tension across a bilayer.

	InitialiseSliceStress();

	// Set the default data for CCurrentState: this is in PovRay format
	// by default and requires the camera and target coordinates to be specified 
	// in absolute coordinates. The commands specify the coordinates as multiples 
	// of the simulation SPace dimensions (for ease of changing SimBox size without
	// having to change the camera coordinates), and these are converted into 
	// absolute coordinates here and in the command handlers, SetCurrentStateCamera()
	// and SetCurrentStateLight().  We hold the coordinates of all SetCurrentStateLight()
	// commands received so that we can implement removal of lights later.
    // Note that for a serial run, the simulation Space reduced to the single SimBox.


	m_Camera[0]	=  0.5*m_pSimState->GetSimSpaceXLength();
	m_Camera[1]	= -1.5*m_pSimState->GetSimSpaceYLength();
	m_Camera[2]	=  0.5*m_pSimState->GetSimSpaceZLength();

	m_Target[0]	=  0.5*m_pSimState->GetSimSpaceXLength();
	m_Target[1]	=  0.5*m_pSimState->GetSimSpaceYLength();
	m_Target[2]	=  0.5*m_pSimState->GetSimSpaceZLength();

	// Define default lights for the PovRay output. The lights are located at:
	//
	// (30,30,30), (-30,0,0), (0,-30,0), (0,0,-30)

	m_vLightX.clear();
	m_vLightY.clear();
	m_vLightZ.clear();

	m_vLightX.push_back(30);
	m_vLightX.push_back(-30);
	m_vLightX.push_back(0);
	m_vLightX.push_back(0);

	m_vLightY.push_back(30);
	m_vLightY.push_back(0);
	m_vLightY.push_back(-30);
	m_vLightY.push_back(0);

	m_vLightZ.push_back(30);
	m_vLightZ.push_back(0);
	m_vLightZ.push_back(0);
	m_vLightZ.push_back(-30);

}

CMonitor::~CMonitor()
{
	// clear the static pointer to the singleton CMonitor object preparatory to
	// doing another simulation.

	m_pInstance = NULL;

	// delete the CObservable-derived objects that were created for fixed data, 
	// bond, bondpair and polymer data. We loop over the concatenated container.

	for(ObservableIterator iterOb = m_vAllObservables.begin(); iterOb != m_vAllObservables.end(); iterOb++)
	{
		delete (*iterOb);
	}
	m_vAllObservables.clear();

	// and the observables defined on the analysis grid

	for(GridObservableIterator iterGridOb = m_vGridObservables.begin(); iterGridOb != m_vGridObservables.end(); iterGridOb++)
	{
		delete (*iterGridOb);
	}
	m_vGridObservables.clear();

	// delete the current state snapshots if they were saved
	
	if(!m_vCurrentStates.empty())
	{
		for(CurrentStateIterator iterCur = m_vCurrentStates.begin(); iterCur != m_vCurrentStates.end(); iterCur++)
		{
			delete (*iterCur);
		}

		m_vCurrentStates.clear();
	}

	// delete the density field states if they were saved
	
	if(!m_vDensityStates.empty())
	{
		for(DensityStateIterator iterDen = m_vDensityStates.begin(); iterDen != m_vDensityStates.end(); iterDen++)
		{
			delete (*iterDen);
		}

		m_vDensityStates.clear();
	}

	// delete the IRegionAnalysis objects created and empty the container and the
	// one containing the currently-active regions.

	if(!m_Regions.empty())
	{
		for(RegionIterator iterRegion=m_Regions.begin(); iterRegion!=m_Regions.end(); iterRegion++)
		{
			delete (*iterRegion);
		}

		m_Regions.clear();
		m_ActiveRegions.clear();
	}

	if(!m_DensityFields.empty())
	{
		for(DensityFieldIterator iterDens=m_DensityFields.begin(); iterDens!=m_DensityFields.end(); iterDens++)
		{
			delete *iterDens;
		} 
		m_DensityFields.clear();
	}


}

// Function to calculate observables that depend only on properties of independent polymers.
//
// Current observables are:	Polymer end-to-end distance
//

void CMonitor::CalculateSinglePolymerData()
{
	// Zero the vector holding the EE distance for each polymer type

	for(zDoubleVectorIterator iter=m_vPolymerEE.begin(); iter!=m_vPolymerEE.end(); iter++)
	{
		(*iter) = 0.0;
	}

	// Loop over all polymers and calculate their end-to-end distance from the head
	// and tail beads and add it to the running totals for each polymer type.

	double dx[3];
	double rEEMag;

	for(cPolymerVectorIterator iterPoly=m_pSimState->GetPolymers().begin(); iterPoly!=m_pSimState->GetPolymers().end(); iterPoly++)
	{
		dx[0]	=  (*iterPoly)->GetHead()->m_Pos[0] - (*iterPoly)->GetTail()->m_Pos[0];
		dx[1]	=  (*iterPoly)->GetHead()->m_Pos[1] - (*iterPoly)->GetTail()->m_Pos[1];

#if SimDimension == 3
		dx[2]	=  (*iterPoly)->GetHead()->m_Pos[2] - (*iterPoly)->GetTail()->m_Pos[2];
#elif SimDimension == 2
		dx[2] = 0.0;
#endif

		if( dx[0] > m_HalfSimBoxXLength )
			dx[0] = dx[0] - m_SimBoxXLength;
		else if( dx[0] < -m_HalfSimBoxXLength )
			dx[0] = dx[0] + m_SimBoxXLength;

		if( dx[1] > m_HalfSimBoxYLength )
			dx[1] = dx[1] - m_SimBoxYLength;
		else if( dx[1] < -m_HalfSimBoxYLength )
			dx[1] = dx[1] + m_SimBoxYLength;

#if SimDimension == 3
		if( dx[2] > m_HalfSimBoxZLength )
			dx[2] = dx[2] - m_SimBoxZLength;
		else if( dx[2] < -m_HalfSimBoxZLength )
			dx[2] = dx[2] + m_SimBoxZLength;
#endif

		rEEMag	= sqrt(dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2]);
		m_vPolymerEE.at((*iterPoly)->GetType()) += rEEMag;
	}

	// Normalize the data by the number of polymers of each type. 
	// Because the wall polymers are included in the m_PolyerTypeSize but 
	// not in the m_vPolyerTypeTotal, we have to check that we do not divide 
	// by zero when normalising. Also note that the aaScalarSingle object fgoes
	// out of scope on exiting the if block, so a new one is created for
	// each polymer data item.
	// 

	double meanEEDist = 0.0;

	for(long polyType=0; polyType<m_PolymerTypeSize; polyType++)
	{
		if(m_vPolymerTypeTotal.at(polyType) > 0)
		{
			meanEEDist = m_vPolymerEE.at(polyType)/static_cast<double>(m_vPolymerTypeTotal.at(polyType));
			m_vPolymerEE.at(polyType) = meanEEDist;

			aaScalarSingle ssGlobalEEDist(meanEEDist);

			ssGlobalEEDist.AddData(m_vPolymerObservables.at(polyType));
		}	
	}
}


// Function to calculate observables that depend only on properties of independent beads.
//
// Current observables are:	Temperature
//							Pressure
//							Total linear momentum
//							Total angular momentum
//							Stress tensor (diagonal elements only)
//							Inertia tensor
//

void CMonitor::CalculateSingleBeadData()
{
	// Zero observables before performing sums over all beads

	m_meanTemp		= 0.0;
	m_meanPress		= 0.0;	// this holds the NkT term in the virial

	for(short int i=0; i<3; i++)	
	{
		m_cmPos[i]				    = 0.0;	// Centre of Mass location
		m_cmMom[i]				    = 0.0;	// Centre of Mass momentum
		m_totalSqMom[i]			    = 0.0;	// Total bead squared momentum
		m_totalAngMom[i]		    = 0.0;  // Total bead angular momentum
		m_totalStress[3*i]		    = 0.0;	// these hold the r.F terms in the virial
		m_totalStress[3*i+1]	    = 0.0;
		m_totalStress[3*i+2]	    = 0.0;
		m_totalStressSphere[3*i]	= 0.0;	// these hold the r.F terms in the virial calculated using curvilinear coords
		m_totalStressSphere[3*i+1]	= 0.0;
		m_totalStressSphere[3*i+2]	= 0.0;
		m_totalInertia[3*i]		    = 0.0;	
		m_totalInertia[3*i+1]	    = 0.0;
		m_totalInertia[3*i+2]	    = 0.0;
	}

	// Zero the vector holding the MSD for each bead type

	for(zDoubleVectorIterator iter=m_vBeadMSD.begin(); iter!=m_vBeadMSD.end(); iter++)
	{
		(*iter) = 0.0;
	}

	// loop over all beads and calculate distance moved, centre of mass position,
	// total momentum, total angular momentum and average kinetic energy. Then 
	// calculate the temperature, pressure and the stress and inertia tensors

	double dPos[3];
	double magPos;

	AbstractBeadVector vAllBeads = GetSimBoxBeads();
	for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		// We must use the un-periodic boundary positions for the diffusion coefficient
		// calculation

		dPos[0]	=  (*iterBead)->m_unPBCPos[0] - (*iterBead)->m_InitialPos[0];
		dPos[1]	=  (*iterBead)->m_unPBCPos[1] - (*iterBead)->m_InitialPos[1];
		dPos[2]	=  (*iterBead)->m_unPBCPos[2] - (*iterBead)->m_InitialPos[2];
		magPos	= dPos[0]*dPos[0] + dPos[1]*dPos[1] + dPos[2]*dPos[2];

		m_vBeadMSD.at((*iterBead)->GetType()) += magPos;

		m_cmPos[0]	+= (*iterBead)->m_Pos[0];
		m_cmPos[1]	+= (*iterBead)->m_Pos[1];
		m_cmPos[2]	+= (*iterBead)->m_Pos[2];

		m_cmMom[0] += (*iterBead)->m_Mom[0];
		m_cmMom[1] += (*iterBead)->m_Mom[1];
		m_cmMom[2] += (*iterBead)->m_Mom[2];

		m_totalSqMom[0] += (*iterBead)->m_Mom[0]*(*iterBead)->m_Mom[0];
		m_totalSqMom[1] += (*iterBead)->m_Mom[1]*(*iterBead)->m_Mom[1];
		m_totalSqMom[2] += (*iterBead)->m_Mom[2]*(*iterBead)->m_Mom[2];

		m_totalAngMom[0] +=  (*iterBead)->m_Pos[1]*(*iterBead)->m_Mom[2] - (*iterBead)->m_Pos[2]*(*iterBead)->m_Mom[1];
		m_totalAngMom[1] += -(*iterBead)->m_Pos[0]*(*iterBead)->m_Mom[2] + (*iterBead)->m_Pos[2]*(*iterBead)->m_Mom[0];
		m_totalAngMom[2] +=  (*iterBead)->m_Pos[0]*(*iterBead)->m_Mom[1] - (*iterBead)->m_Pos[1]*(*iterBead)->m_Mom[0];

#if SimDimension == 2

		m_totalStress[0] += (*iterBead)->m_Stress[0];
		m_totalStress[1] += (*iterBead)->m_Stress[1];
		m_totalStress[3] += (*iterBead)->m_Stress[3];
		m_totalStress[4] += (*iterBead)->m_Stress[4];

#elif SimDimension == 3

		for(short int i=0; i<9; i++)
		{
			m_totalStress[i] += (*iterBead)->m_Stress[i];
		}

		m_totalInertia[0] +=	(*iterBead)->m_Pos[1]*(*iterBead)->m_Pos[1] +
							    (*iterBead)->m_Pos[2]*(*iterBead)->m_Pos[2];

		m_totalInertia[1] +=	-(*iterBead)->m_Pos[0]*(*iterBead)->m_Pos[1];

		m_totalInertia[2] +=	-(*iterBead)->m_Pos[0]*(*iterBead)->m_Pos[2];

		m_totalInertia[4] +=	(*iterBead)->m_Pos[0]*(*iterBead)->m_Pos[0] +
							    (*iterBead)->m_Pos[2]*(*iterBead)->m_Pos[2];

		m_totalInertia[5] +=	-(*iterBead)->m_Pos[1]*(*iterBead)->m_Pos[2];

		m_totalInertia[8] +=	(*iterBead)->m_Pos[0]*(*iterBead)->m_Pos[0] +
							    (*iterBead)->m_Pos[1]*(*iterBead)->m_Pos[1];

#endif

		// Count the number of beads of each type in the grid cells covering the
		// simulation box. Note that the iz index is calculated within the
		// conditionally-compiled sections of code immediately above here.
		// We use the fact that the  CObservable objects are stored in the 
		// same order as the bead types to access the correct observable
		// for each type of bead

		long ix = static_cast<long>((*iterBead)->m_Pos[0]/m_GridXCellWidth);
		long iy = static_cast<long>((*iterBead)->m_Pos[1]/m_GridYCellWidth);

#if SimDimension == 2
		long iz = 0;	
#elif SimDimension == 3
		long iz = static_cast<long>((*iterBead)->m_Pos[2]/m_GridZCellWidth);	
#endif

		long index = m_GridXCellNo*(m_GridYCellNo*iz+iy) + ix;

		m_vGridObservables.at((*iterBead)->GetType())->m_vField.at(index) += 1.0;
	}

	// Normalize all observables according to the dimension of the simulation.
	// Note that because the wall bead type is included in m_BeadTypeSize but has no
	// entry in the m_vBeadTypeTotal (because the beads are CWallBeads and not CBeads)
	// we have to check for divide by zero in the normalisation.
	// Also note that the values output for the diffusion coefficient now
	// include the time-step and the dimensional factor of 2*D in D dimensions.
	//
	// From Rapaport, p116:   D = Lim     sum[(Xj(t)-Xj(0)]**2
	//							 t->inf	 j=1..N    6.N.t
	//

	// Store the current time locally to avoid making functions calls. Note
	// that because the current time can become very large, and the number of
	// beads of a given type can also be very large, their product may exceed the
	// maximum long integer storable. Hence we convert each term to a double before
	// multiplying them.

	const long currentTime = GetCurrentTime();

#if SimDimension == 2
    const double diffusionPrefactor = 4.0*m_pSimState->GetIntegrationStep()*static_cast<double>(currentTime);
#elif SimDimension == 3
    const double diffusionPrefactor = 6.0*m_pSimState->GetIntegrationStep()*static_cast<double>(currentTime);
#endif

	for(long beadType=0; beadType!=m_BeadTypeSize; beadType++)
	{
		if(m_vBeadTypeTotal.at(beadType) > 0)
		{
			const double divisor = 	diffusionPrefactor*static_cast<double>(m_vBeadTypeTotal.at(beadType));

			m_vBeadMSD.at(beadType) /= divisor;

			// Trap any overflow errors resulting in negative MSDs

			if(m_vBeadMSD.at(beadType) < 0.0)
			{
				const zString beadName = m_pSimState->cGetAnalysisState().GetBeadNameFromType(beadType);
				const zString afterMsg = "MSD is negative after divide";

				new CLogNegativeBeadDiffusionCoefficient(currentTime, beadName, beadType);
				new CLogTextMessage(currentTime, afterMsg);
			}
		}
	}

	m_cmPos[0] /= m_BeadTotal;
	m_cmPos[1] /= m_BeadTotal;
	m_cmMom[0] /= m_BeadTotal;
	m_cmMom[1] /= m_BeadTotal;

#if SimDimension == 2

	m_cmPos[2] = 0.0;
	m_cmMom[2] = 0.0;

	m_meanTemp  = (m_totalSqMom[0] + m_totalSqMom[1])/m_BeadTotal;

	m_meanPress	= 0.5*m_invSimBoxVolume*(m_totalSqMom[0] + m_totalSqMom[1] + m_totalStress[0] + m_totalStress[4]);

	m_totalStress[0] = m_totalStress[0]*m_invSimBoxVolume;
	m_totalStress[1] = m_totalStress[1]*m_invSimBoxVolume;
	m_totalStress[3] = m_totalStress[3]*m_invSimBoxVolume;
	m_totalStress[4] = m_totalStress[4]*m_invSimBoxVolume;

#elif SimDimension == 3

	m_cmPos[2] /= m_BeadTotal;
	m_cmMom[2] /= m_BeadTotal;

	m_meanTemp = (m_totalSqMom[0] + m_totalSqMom[1] + m_totalSqMom[2])/(3.0*m_BeadTotal);

	m_meanPress	= 0.3333333333*m_invSimBoxVolume*(m_totalSqMom[0] + m_totalSqMom[1] + m_totalSqMom[2] + 
												  m_totalStress[0] + m_totalStress[4] + m_totalStress[8]);
	for(short int j=0; j<9; j++)
	{
		m_totalStress[j] = m_totalStress[j]*m_invSimBoxVolume;
	}

	// The inertia tensor is symmetric so we get the other elements free

	m_totalInertia[0] /= m_BeadTotal;
	m_totalInertia[1] /= m_BeadTotal;
	m_totalInertia[2] /= m_BeadTotal;
	m_totalInertia[4] /= m_BeadTotal;
	m_totalInertia[5] /= m_BeadTotal;
	m_totalInertia[8] /= m_BeadTotal;

	m_totalInertia[3] = m_totalInertia[1];
	m_totalInertia[6] = m_totalInertia[2];
	m_totalInertia[7] = m_totalInertia[5];

#endif

#if EnableStressTensorSphere == SimMiscEnabled
	// Tell the SimBox to transform the stress tensor into general curvilinear coordinates if it is compiled in, and retrieve the total stress
	// summed over the whole SimBox. We normalise by the SimBox volume and the sampling period as the stress has been accumulated since
	// that last time the Monitor calculated averages.
	
	for(long is=0; is<9; is++)
	{
	    m_totalStressSphere[is] = 0.0;
	}
	
	GetISimBox()->TransformStressSphere(m_totalStressSphere);
	
	for(long is1=0; is1<9; is1++)
	{
	    m_totalStressSphere[is1] *= m_invSimBoxVolume/static_cast<double>(m_pSimState->GetSamplePeriod());
	}
	
	
#endif	

	// Create aaScalarSingle, aaVectorSingle, etc objects to pass the data to the 
	// CObservables. 

	aaScalarSingle globalTemp(m_meanTemp);
	aaScalarSingle globalPress(m_meanPress);
	aaVectorSingle globalVCM(m_cmMom[0], m_cmMom[1], m_cmMom[2]);
	aaVectorSingle globalRCM(m_cmPos[0], m_cmPos[1], m_cmPos[2]);
	aaTensorSingle globalStress(m_totalStress);
	aaTensorSingle globalStressSphere(m_totalStressSphere);
	aaTensorSingle globalInertia(m_totalInertia);

	   globalTemp.AddData(m_vObservables.at(0));
	  globalPress.AddData(m_vObservables.at(1));
	    globalVCM.AddData(m_vObservables.at(2));
	    globalRCM.AddData(m_vObservables.at(3));
	 globalStress.AddData(m_vObservables.at(4));
	 globalStressSphere.AddData(m_vObservables.at(5));
	globalInertia.AddData(m_vObservables.at(6));
}

// Function to calculate averages over independent bonds. It should only be called
// if there definitely are some bonds as we don't check inside for divide by
// zero.
//
// The mean bond length for all bonds regardless of type is displayed as a single
// observable and the mean lengths for each bond type are stored sequentially
// as well. We loop over all bonds and sort them into the appropriate sums.
// 

void CMonitor::CalculateSingleBondData()
{
	// Zero the vector holding the mean length for each bond type

	for(zDoubleVectorIterator iter=m_vBondLength.begin(); iter!=m_vBondLength.end(); iter++)
	{
		(*iter) = 0.0;
	}

	// Loop over all bonds and calculate the length from the head and tail beads and 
	// add it to the running totals for each bond type and the overall bond length sum

	long bondTotal;
	double meanBondLength;
	double mean2BondLength;

	bondTotal       = 0;
	meanBondLength  = 0.0;
	mean2BondLength = 0.0;

	double dx[3];
	double rBondMag;

	BondVector vAllBonds = GetSimBoxBonds();
	for(cBondVectorIterator iterBond=vAllBonds.begin(); iterBond!=vAllBonds.end(); iterBond++)
	{
		dx[0]	=  (*iterBond)->GetHead()->m_Pos[0] - (*iterBond)->GetTail()->m_Pos[0];
		dx[1]	=  (*iterBond)->GetHead()->m_Pos[1] - (*iterBond)->GetTail()->m_Pos[1];

#if SimDimension == 3
		dx[2]	=  (*iterBond)->GetHead()->m_Pos[2] - (*iterBond)->GetTail()->m_Pos[2];
#elif SimDimension == 2
		dx[2] = 0.0;
#endif

		if( dx[0] > m_HalfSimBoxXLength )
			dx[0] = dx[0] - m_SimBoxXLength;
		else if( dx[0] < -m_HalfSimBoxXLength )
			dx[0] = dx[0] + m_SimBoxXLength;

		if( dx[1] > m_HalfSimBoxYLength )
			dx[1] = dx[1] - m_SimBoxYLength;
		else if( dx[1] < -m_HalfSimBoxYLength )
			dx[1] = dx[1] + m_SimBoxYLength;

#if SimDimension == 3
		if( dx[2] > m_HalfSimBoxZLength )
			dx[2] = dx[2] - m_SimBoxZLength;
		else if( dx[2] < -m_HalfSimBoxZLength )
			dx[2] = dx[2] + m_SimBoxZLength;
#endif

		rBondMag	= sqrt(dx[0]*dx[0] + dx[1]*dx[1] + dx[2]*dx[2]);

		m_vBondLength.at((*iterBond)->GetType()) += rBondMag;

		meanBondLength += rBondMag;
		mean2BondLength += rBondMag*rBondMag;

		bondTotal++;
	}

	// We only examine the mean bond length at the moment, not its square or
	// variance and we use the member function appropriate to the type of
	// observable.

	meanBondLength  /= static_cast<double>(bondTotal);
	mean2BondLength /= static_cast<double>(bondTotal);

	double varBondLength = mean2BondLength - meanBondLength*meanBondLength;

	if(varBondLength < 0.0 && fabs(varBondLength) > 0.000001)
	{
		Trace("Error: bond length has variance less than 0");
	}

	aaScalarSingle globalBondLength(meanBondLength);

    // Note that the index into the observables array depends on the number
    // of fixed observables defined in the constructor. After these are added
    // the observables that depend on the numbers of bead types, bond types, etc.

	globalBondLength.AddData(m_vObservables.at(m_FixedObservableNo-1));

	// Normalize the data by the number of polymers of each type and the number of data points
	// in the sample. Because the wall polymers are included in the m_PolyerTypeSize but 
	// not in the m_vPolyerTypeTotal, we have to check that we do not divide by zero when
	// normalising.
	//

	double meanBL;

	for(long bondType=0; bondType!=m_BondTypeSize; bondType++)
	{
		if(m_vBondTypeTotal.at(bondType) > 0)
		{
			meanBL = m_vBondLength.at(bondType)/static_cast<double>(m_vBondTypeTotal.at(bondType));

			m_vBondLength.at(bondType) = meanBL;

			aaScalarSingle globalBL(meanBL);

			globalBL.AddData(m_vBondObservables.at(bondType));
		}	
	}
}

// Function to calculate averages over independent bond pairs. Because not all
// simulations use CBondPair objects, it should only be called if there definitely 
// are CBondPairs in some polymers as we don't check for divide by zero if none exist
// and a loop is made on the assumption that some bond pairs exist.
//
// We store the following observables:
//
//	 second Lengendre polynomial, P2(phi-phi0)
//
//	 lengths of each bond in the CBondPair object
//
//	 ratio of the mean-square to the square of the mean angle.
//
// The last one may not always be calculated because it is expensive to do so.
// But it is useful in debugging because the ratio should be independent of the
// bending modulus and temperature.

void CMonitor::CalculateSingleBondPairData()
{
	// Zero the vectors holding the averages over bond pairs. If this routine is being
	// executed we assume that some CBondPairs exist.

	for(long bp=0; bp<m_BondPairTypeSize; bp++)
	{
		m_vBondPairP2.at(bp)			= 0.0;
		m_vBondPairFirstLength.at(bp)	= 0.0;
		m_vBondPairSecondLength.at(bp)	= 0.0;
		m_vBondPairPhiRatio.at(bp)		= 0.0;
		m_vBondPairPhiSq.at(bp)			= 0.0;
		m_vBondPairPhi.at(bp)			= 0.0;
	}

	// Calculate the averages over all bond pairs in the current configuration

	BondPairVector vAllBondPairs = GetSimBoxBondPairs();
	for(cBondPairVectorIterator iterBP=vAllBondPairs.begin(); iterBP!=vAllBondPairs.end(); iterBP++)
	{
		m_vBondPairP2.at((*iterBP)->GetType())				+= (*iterBP)->GetP2();
		m_vBondPairFirstLength.at((*iterBP)->GetType())		+= (*iterBP)->GetFirstLength();
		m_vBondPairSecondLength.at((*iterBP)->GetType())	+= (*iterBP)->GetSecondLength();
		m_vBondPairPhiSq.at((*iterBP)->GetType())			+= (*iterBP)->GetPhiDiffSq();
		m_vBondPairPhi.at((*iterBP)->GetType())				+= (*iterBP)->GetPhiDiff();
	}

	// Normalize the data by the number of bond pairs of each type. 

	for(long bpType=0; bpType<m_BondPairTypeSize; bpType++)
	{
		if(m_vBondPairTypeTotal.at(bpType) > 0)
		{
			const double bpInvTypeTotal = 1.0/static_cast<double>(m_vBondPairTypeTotal.at(bpType));

			m_vBondPairP2.at(bpType)			*=	bpInvTypeTotal;
			m_vBondPairFirstLength.at(bpType)	*=	bpInvTypeTotal;
			m_vBondPairSecondLength.at(bpType)	*=	bpInvTypeTotal;
			m_vBondPairPhi.at(bpType)			*=	bpInvTypeTotal;
			m_vBondPairPhiSq.at(bpType)			*=	bpInvTypeTotal;

			if(m_vBondPairPhi.at(bpType) > 0.0)
			{
				m_vBondPairPhiRatio.at(bpType) = m_vBondPairPhiSq.at(bpType)/(m_vBondPairPhi.at(bpType)*m_vBondPairPhi.at(bpType));
			}
			else
				m_vBondPairPhiRatio.at(bpType) = 0.0;


			aaScalarSingle globalP2(m_vBondPairP2.at(bpType));
			aaScalarSingle globalFirstLength(m_vBondPairFirstLength.at(bpType));
			aaScalarSingle globalSecondLength(m_vBondPairSecondLength.at(bpType));
			aaScalarSingle globalPhiRatio(m_vBondPairPhiRatio.at(bpType));

			globalP2.AddData(m_vBondPairObservables.at(4*bpType));
			globalFirstLength.AddData(m_vBondPairObservables.at(4*bpType + 1));
			globalSecondLength.AddData(m_vBondPairObservables.at(4*bpType + 2));
			globalPhiRatio.AddData(m_vBondPairObservables.at(4*bpType + 3));
		}	
	}
}


// Function to analyse the bead, bond and polymer configurations throughout the simulation.
// To speed up the processing of data the CMonitor has friend access to the following classes:
//
//		CAbstractBead
//		CBead
//		CBond
//		CCNTCell
//		CSimBox
//
// Data may be analysed and stored continuously using the CHistoryState object to hold
// packets of data stored in instances of the CTimeSeriesData class, or else averaged
// over equal-length periods throughout the simulation in CObservable-derived objects.
// These manage their own data normalisation and serialization.
//
// The data are sampled in the CSimBox once every "SamplePeriod" timesteps,
// and put into arrays here for later plotting. The various periods (sampling,
// analysis, etc) are set in the CMonitor's constructor. The current time is 
// obtained from the ISimBoxBase base class, except that CalculateSingleBeadData()
// stores it locally to calculate the bead MSDs. This avoids making a function call
// for every bead type. Using the base class function guarantees the monitor uses
// the same time as the SimBox.
//
// The following functions perform the various calculations:
//
//	CalculateSingleBeadData()		Averages over independent beads, e.g., T, P, CM pos	
//	CalculateSingleBondData()		Averages over independent bonds, e.g., Bond Length 								
//	CalculateSingleBondPairData()	Averages over independent bond pairss, e.g., Bond angles								
//	CalculateSinglePolymerData()	End-to-end distance for polymers
//
// We expect that correlation function measurements will be added that will require 
// calculations over pairs of beads or bonds. These will be calculated separately as 
// the data will not be stored in single beads or bonds but as a more global measurement.
// Measurements of the shape of the aggregates (micelles, lamellae) will be performed
// on "cells" that contain a set of beads and that need to be ordered to allow efficient
// calculations of shape fluctuations.

void CMonitor::Sample()
{
	m_SamplesTaken++;					// increment sample counter

	CalculateSingleBeadData();

	if(m_pSimState->GetBondTotal() > 0)	     // only calculate bond averages if they exist
		CalculateSingleBondData();

	if(m_pSimState->GetBondPairTotal() > 0)	// only calculate bond pair averages if they exist
		CalculateSingleBondPairData();

	CalculateSinglePolymerData();
	
	// Combine previous global analysis with a search for mesoscopic
	// aggregates such as micelles, bilayers, vesicles etc.

	FindAggregates();

	// Analyse geometric regions of the SimBox for observables depending on the
	// beads and polymers in those regions.

	FindRegions();

	// Generate density profiles of beads and polymers if required

	if(IsDensityFieldAnalysisOn())
	{
		CalculateDensityFields();
	}

	// debug output to screen if required

#ifdef TraceOn
	TraceInt(    "Time", GetCurrentTime());
	TraceDouble2("Temp,Press", m_meanTemp, m_meanPress);
	TraceVector( "vCM", m_cmMom[0], m_cmMom[1], m_cmMom[2]);
	TraceVector( "rCM", m_cmPos[0], m_cmPos[1], m_cmPos[2]);
	TraceVector( "Ang Mom", m_totalAngMom[0], m_totalAngMom[1], m_totalAngMom[2]);
#endif

	// Save data to CHistoryState object. This is every SamplePeriod steps
	// but it is written to file only every AnalysisPeriod steps. We do the 
	// check on whether to serialize the data inside the SaveHistoryState() function.

	SaveHistoryState();

	// Save messages stored in the CLogState object.

	SaveLogState();

	// Normalize and serialize the sampled data. Note that analysed data are
    // saved at multiples of the sample period, and the bead density grid data 
    // are saved at multiples of the density period.

	if(m_SamplesTaken%m_SampleNo == 0)
	{
		SaveAnalysisState();

		SaveAggregateState();	// Check whether to serialize aggregate data in the function

		SaveRegionState();
	}

    if(IsDensityGridAnalysisOn() && m_SamplesTaken%m_GridSampleNo == 0)
    {
		SaveDensityStates();
    }

}

// Function to perform the data sampling for a parallel simulation. Because most analysis now requires communication 
// between all the processors, we have to coordinate the sending and receiving of data from PN to P0, and make P0 responsible 
// for actually writing any data to file.

void CMonitor::SampleP()
{
	m_SamplesTaken++;					// increment sample counter



	// Save messages stored in the CLogState object.

	SaveLogStateP();



}

// Function to update the processes being monitored in the simulation. 
//
// This function is called from the SimBox directly at the same frequency
// (SamplePeriod) as the Sample() function above. Processes accumulate 
// their data every step and it is written to the process files every 
// SamplePeriod steps from the SaveProcessState() function below.
// 
// Because events cannot pass data into the CAnalysis-derived aggregate classes, 
// we use the processes to collect data from aggregates, and any associated events,
// and perform calculations that need both.

void CMonitor::SampleProcess()
{
	ProcessSequence vProcesses;

	vProcesses.clear();

	copy(m_pSimState->GetAnalysisState().GetProcesses().begin(), 
		 m_pSimState->GetAnalysisState().GetProcesses().end(), 
		 back_inserter(vProcesses));

	for(ProcessIterator iterProc=vProcesses.begin(); iterProc!=vProcesses.end(); iterProc++)
	{
		(*iterProc)->UpdateState(*m_pSimState, GetISimBox());
	}
}

// Function to write process data to file. Each xxProcess object contains an
// xxProcessState object that is serialized to a file with the name
//
// prefix.<process name>m.nnn 
//
// where "prefix" is the string defined in xxBase::GetPSPrefix() to name the process
// files, m identifies the process, and nnn is the runId. The process data is
// stored in the xxProcessState and written to file when told by the SimBox.
//

void CMonitor::SaveProcessState()
{
	ProcessSequence vProcesses;

	vProcesses.clear();

	copy(m_pSimState->GetAnalysisState().GetProcesses().begin(), 
		 m_pSimState->GetAnalysisState().GetProcesses().end(), 
		 back_inserter(vProcesses));

	for(ProcessIterator iterProc=vProcesses.begin(); iterProc!=vProcesses.end(); iterProc++)
	{
		(*iterProc)->SaveState();
	}
}

// Function to call the CAnalysisState object to normalize the sampled data 
// and serialize it. We have to set the current time in the CAnalysisState
// first so that it can correctly normalize the data.

void CMonitor::SaveAnalysisState() const
{
	m_pSimState->SetCurrentTime(GetCurrentTime());

	if(!m_pSimState->GetAnalysisState().Serialize())
		ErrorTrace("Error in CMonitor::SaveAnalysisState");
}

// Function to save an inclusive restart state without any bead coordinate data.
// This is for debug purposes to check the read/write of dynamically-created data.

void CMonitor::SaveNonBeadInclusiveRestartState() const
{
	CInclusiveRestartState rState(GetCurrentTime(), GetRunId(), true, true, m_bLogRestartWarningMessages, GetISimBox(), &m_pSimState->GetAnalysisState(), m_pSimState->GetInitialState());

    if(!rState.Serialize())
		ErrorTrace("Error in CMonitor::SaveNonBeadInclusiveRestartState");	
}

// Function to save the bead coordinates to a restart state file. We don't
// need to keep the the successive restart states created so we use the constructor
// that only writes out data. We get the current simulation time from 
// the ISimBox as the sample period of the monitor, which is used to increment 
// its time, may not be commensurate with the period of the restart state.
// The default type of restart state (coordinates only or inclusive) is set in the
// constructor and may be changed by command.

void CMonitor::SaveRestartState() const
{
    if(m_bInclusiveRestartStates)
    {
	    CInclusiveRestartState rState(GetCurrentTime(), GetRunId(), true, m_bLogRestartWarningMessages, GetISimBox(), &m_pSimState->GetAnalysisState(), m_pSimState->GetInitialState());
	    if(!rState.Serialize())
		    ErrorTrace("Error in CMonitor::SaveRestartState: inclusive restart state failed");	
    }
    else
    {
	    CRestartState rState(GetCurrentTime(), GetRunId(), true, m_bLogRestartWarningMessages, m_pSimState->GetInitialState());
	    if(!rState.Serialize())
		    ErrorTrace("Error in CMonitor::SaveRestartState: coordinates-only restart state failed");	
    }
}

// Function to save the bead coordinates in a CCurrentState object for use in
// displaying a snapshot of the simulation state. This cannot be used for
// starting a new simulation. We get the current simulation time from the ISimBox
// as the sample period of the monitor, which is used to increment its time, may
// not be commensurate with the period of the current state.
//
// The file is closed after it is written in the Serialize() member function,
// so that it can be viewed before the simulation has ended. But if the
// m_bCurrentStateAnalysis flag is false, the snapshots are discarded and no
// analysis performed.
//
// Data saved
// **********
//
//
// Bounding box of user-specified size
// Coordinates of camera and target for PovRay formatted output
// Bead Position Coordinates
//
//
// Beads whose types are set in the m_ExcludedBeadTypes vector are not
// displayed as their visibility flag is toggled off. However, beads that are
// assembled into targets can have their visibility changed according to the target's
// display state instead of their type.

void CMonitor::SaveCurrentState()
{
	// Replace the constructor call with the factory pattern that returns a new
	// format object

	CCurrentStateFormat* pFormat = 0;

	if( m_DefaultCurrentStateFormat == "Povray" )
	{
		pFormat = new CPovrayFormat(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength,
								    m_bDisplayBox, m_BeadTypeSize, 
									m_Camera, m_Target, m_vLightX, m_vLightY, m_vLightZ);
	}
	else if( m_DefaultCurrentStateFormat == "PovrayWithBonds" )
	{
		pFormat = new CPovrayFormat(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength,
								    m_bDisplayBox, m_BeadTypeSize, 
									m_Camera, m_Target, m_vLightX, m_vLightY, m_vLightZ, true);
	}
	else if( m_DefaultCurrentStateFormat == "Amira" )
	{
		pFormat = new CAmiraFormat(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength,
								   m_bDisplayBox, m_BeadTypeSize);
	}
	else if( m_DefaultCurrentStateFormat == "Paraview" )
	{
		pFormat = new CParaviewFormat(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength,
								   m_bDisplayBox, m_BeadTypeSize);
	}
    else if( m_DefaultCurrentStateFormat == "SolventFree" )
    {
        pFormat = new CSolventFreeFormat(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength,
                                    m_bDisplayBox, m_BeadTypeSize, GetISimBox());
    }
    else if( m_DefaultCurrentStateFormat == "SolventFreeAndPovray" )
    {
        pFormat = new CSolventFreeFormat(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength,
                                         m_bDisplayBox, m_BeadTypeSize, GetISimBox());

        CCurrentState* pcState = new CCurrentState(GetCurrentTime(), GetRunId(),
                                                   GetISimBox(), pFormat, m_bRestrictCurrentStateCoords,
                                                   m_MinXFraction, m_MinYFraction, m_MinZFraction,
                                                   m_MaxXFraction, m_MaxYFraction, m_MaxZFraction);
        
        // Here we write out both a povray snapshot and a solvent-free one, so we have to ensure we delete
        // the first one before creating the second.
        
        if(!pcState->Serialize())
        {
            ErrorTrace("Error in CMonitor::SaveCurrentState SolventFreeAndPovray");
        }
        
        delete pcState;
        
        // Now create the povray format instance for use outside the if-block
        
        pFormat = new CPovrayFormat(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength,
                                    m_bDisplayBox, m_BeadTypeSize,
                                    m_Camera, m_Target, m_vLightX, m_vLightY, m_vLightZ);
    }

	CCurrentState* pcState = new CCurrentState(GetCurrentTime(), GetRunId(), 
											   GetISimBox(), pFormat, m_bRestrictCurrentStateCoords,
											   m_MinXFraction, m_MinYFraction, m_MinZFraction,
											   m_MaxXFraction, m_MaxYFraction, m_MaxZFraction);

	if(!pcState->Serialize())
	{
		ErrorTrace("Error in CMonitor::SaveCurrentState");
	}

	if(m_bCurrentStateAnalysis)
	{
		m_vCurrentStates.push_back(pcState);
	}
	else
	{
		delete pcState;
	}
}

// Parallel version of the SaveCurrentState function that allows all PN to send their
// bead coordinates to P0 so that it can write them to file. Recall that we only allow
// P0 to do file IO. Also note that if the EnableParallelMonitor flag is not set, no
// output is created.
//
// Because of the potential size of the datasets, we do not allow current state snapshots 
// to be stored for analysis in a parallel simulation regardless of the setting of the flag.
//
// Within this function, some parameters have different meanings from those used
// in the serial code:
//
// GetSimSpaceXLength() = X size of simulation Space not one SimBox alone

void CMonitor::SaveCurrentStateP()
{
#if EnableParallelMonitor == SimMPSEnabled

    pmCurrentState* pCS = dynamic_cast<pmCurrentState*>(acfParallelMessageFactory::Instance()->Create(pmCurrentState::GetType()));

    if(IsProcessZero())
    {
        // First, P0 writes the header information in the appropriate format
		
	    CCurrentStateFormat* pFormat = 0;

	    if( m_DefaultCurrentStateFormat == "Povray" )
	    {
		    pFormat = new CPovrayFormat(GetSimSpaceXLength(), GetSimSpaceYLength(), GetSimSpaceZLength(),
								        m_bDisplayBox, m_BeadTypeSize, 
									    m_Camera, m_Target, m_vLightX, m_vLightY, m_vLightZ);
	    }
	    else if( m_DefaultCurrentStateFormat == "Amira" )
	    {
		    pFormat = new CAmiraFormat(GetSimSpaceXLength(), GetSimSpaceYLength(), GetSimSpaceZLength(),
								       m_bDisplayBox, m_BeadTypeSize);
	    }
	    else if( m_DefaultCurrentStateFormat == "Paraview" )
	    {
		    pFormat = new CParaviewFormat(GetSimSpaceXLength(), GetSimSpaceYLength(), GetSimSpaceZLength(),
								          m_bDisplayBox, m_BeadTypeSize);
	    }

        CCurrentState* pcState = new CCurrentState(GetCurrentTime(), GetRunId(), 
								     GetISimBox(), pFormat, m_bRestrictCurrentStateCoords,
									 m_MinXFraction, m_MinYFraction, m_MinZFraction,
									 m_MaxXFraction, m_MaxYFraction, m_MaxZFraction);
									 
        // and stores a pointer to the current state instance in its 
        //  message instance

        pCS->SetCurrentStatePointer(pcState);

        // and adds its own bead coordinate data to the file

        if(!pcState->SerializeP0())
        {
		    ErrorTrace("Error writing P0 data in CMonitor::SaveCurrentStateP");
        }
		
        // Next P0 receives the bead coordinate messages from PN, and 
        // lets the messages pass their data to the current state instance they
        // received earlier.

        pCS->Receive();

        // Finish up

        if(!pcState->SerializeEnd())
        {
            // Error condition closing file

		    ErrorTrace("Error closing snapshot file in CMonitor::SaveCurrentStateP");
        }
		
        // We always delete the snapshot instances in a parallel run
        delete pcState;
    }
    else
    {
        // Here PN send its bead data to P0. To save time we only send beads
        // whose coordinates are within the display bead range (if set).
        // We take each bead's coordinate modulo the SimBox size in each dimension
        // and compare it to the fractional display ranges 
        // m_MinXFraction/m_MaxXFraction, etc.

        // Because all processors work in the same space (0,Lx),(0,Ly),(0,Lz),
        // we shift bead coordinates into the whole simulation Space 
        // before sending them to P0 inside the message's AddBead() function.

		AbstractBeadVector vAllBeads = GetISimBox()->GetBeads();
	    for(cAbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
        {
            pCS->AddBead(*iterBead);
        }

        if(pCS->Validate())
        {
            pCS->SendP0();
        }
        else
        {
            std::cout << "Processor " << GetRank() << " failed to validate its pmCurrentState message" << zEndl;
        }
    }

	delete pCS;  // Delete the locally-allocated message instance
#endif
}

// Function to save a single snapshot as a previsualization of the initial state 
// of a simulation. It can be configured to use any of the CCurrentStateFormat
// classes for this snapshot. This snapshot is destroyed immediately so it does
// not interfere with any analysis.

void CMonitor::SavePrevisCurrentState()
{
	CCurrentStateFormat* pFormat = 0;
    
	pFormat = new CPovrayFormat(m_SimBoxXLength, m_SimBoxYLength, m_SimBoxZLength,
								m_bDisplayBox, m_BeadTypeSize, 
								m_Camera, m_Target, m_vLightX, m_vLightY, m_vLightZ);

	CCurrentState* pcState = new CCurrentState(GetCurrentTime(), GetRunId(), 
											   GetISimBox(), pFormat, m_bRestrictCurrentStateCoords,
											   m_MinXFraction, m_MinYFraction, m_MinZFraction,
											   m_MaxXFraction, m_MaxYFraction, m_MaxZFraction);

	if(!pcState->Serialize())
	{
		ErrorTrace("Error in CMonitor::SavePrevisCurrentState");
	}

    delete pcState;
}

// Function to save bead density data calculated on the analysis grid. 
// Each bead type gives rise to a density field. The size of the grid and its 
// spacing are written to file first so that the grid can be read in by 
// other programs. 
//
// Each set of grid data is saved to a separate CDensityState object and serialized
// to its own file in the same format.

void CMonitor::SaveDensityStates()
{
	for(GridObservableIterator iterOb = m_vGridObservables.begin(); iterOb != m_vGridObservables.end(); iterOb++)
	{
		(*iterOb)->Normalize(GetCurrentTime());
//		(*iterOb)->DisplayDebug();
	
		CDensityState* pdState = new CDensityState(GetCurrentTime(), GetRunId(), (*iterOb));
		m_vDensityStates.push_back(pdState);

		if(!pdState->Serialize())
			ErrorTrace("Error in CMonitor::SaveDensityStates");	
	}
}

// Function to pass time-dependent data to the CHistoryState object packaged in a
// CTimeSeriesData object. This contains all data that needs to be analysed
// over a (possibly) long period of time during the simulation. The data is serialized
// if the current time coincides with the analysis period.
//
// We create a CTimeSeriesData object, fill it with data from the current simulation
// state and pass it to the CHistoryState object. Note that the CTimeSeriesData
// objects are destroyed by the CHistoryState object as it maintains a vector of 
// them for doing time-based analysis at the end of the simulation.
//
// Observables saved
// *****************
//	Time
//	Temperature
//	Pressure
//  KE/N		- note the missing factor of 1/2!
//  PE/N
//  Total E/N
//	Bead MSD	- depends on number of bead types including CWallBead
//  Polymer EE	- depends on the number of polymers including the wall polymer
//

void CMonitor::SaveHistoryState() const
{
	const long fixedDataTotal = 6;
	const long dataTotal      = fixedDataTotal + m_BeadTypeSize + m_PolymerTypeSize;

	// Put back the constants in the energy terms. We assume all the bead masses 
	// are unity and normalize by the total number of beads if the ToggleEnergyOutput
	// command indicates it is required. A factor of 1/2 is added to the kinetic 
	// energy as it is not done in CCNTCell::UpdateTotalEnergy to avoid O(N) divisions.
	
	CTimeSeriesData* pTSD = new CTimeSeriesData(dataTotal);

	pTSD->SetValue(0, GetCurrentTime(),	"Time");
	pTSD->SetValue(1, m_meanTemp,		"Temp");
	pTSD->SetValue(2, m_meanPress,		"Pressure");

	// Only output the energy if the user has requested it by command

	if(m_bEnergyOutput)
	{
		const double totalEnergy = 0.5*m_meanKineticEnergy + m_meanPotentialEnergy;

		if(m_bNormalizePerBead)
		{
			pTSD->SetValue(3, 0.5*m_meanKineticEnergy*static_cast<double>(m_BeadTotal), "Kinetic");
			pTSD->SetValue(4, m_meanPotentialEnergy*static_cast<double>(m_BeadTotal),   "Potential");
			pTSD->SetValue(5, totalEnergy*static_cast<double>(m_BeadTotal),			  "Total");
		}
		else
		{
			pTSD->SetValue(3, 0.5*m_meanKineticEnergy,	"Kinetic");
			pTSD->SetValue(4, m_meanPotentialEnergy,	"Potential");
			pTSD->SetValue(5, totalEnergy,				"Total");
		}
	}
	else
	{
			pTSD->SetValue(3, 0.0, "Kinetic");
			pTSD->SetValue(4, 0.0, "Potential");
			pTSD->SetValue(5, 0.0, "Total");
	}


	// Add the MSD's for all bead types taking care with the index into the CTimeSeriesData
	// vector

	for(long ib=0; ib<m_BeadTypeSize; ib++)
	{
		pTSD->SetValue(fixedDataTotal+ib, m_vBeadMSD.at(ib), "MSD");
	}

	// Add the polymer end-to-end distances taking care to start with an index
	// that allows for the prior 3 + No of Bead types

	for(long ip=0; ip<m_PolymerTypeSize; ip++)
	{
		pTSD->SetValue(fixedDataTotal+m_BeadTypeSize+ip, m_vPolymerEE.at(ip), "EE");
	}


	// Now add the CTimeSeriesData object to the history state

	(m_pSimState->GetHistoryState()).AddTimeSeriesData(pTSD);

	// If it is time to write analysis data to file, serialize the CHistoryState object.
	// Note that the same analysis period is used for the CHistoryState and CAnalysisState
	// objects.

	if(m_SamplesTaken%m_SampleNo == 0)
	{
		(m_pSimState->GetHistoryState()).Serialize();
	}

}

// Function to write out all messages to the log file.

void CMonitor::SaveLogState() const
{
	if(!m_pSimState->GetLogState().Serialize())
		ErrorTrace("Error in CMonitor::SaveLogState");
}

// Function to write out all messages to the log file for a parallel simulation.

void CMonitor::SaveLogStateP() const
{
#if EnableParallelMonitor == SimMPSEnabled

	if(!m_pSimState->GetLogState().SerializeP())
		ErrorTrace("Error in CMonitor::SaveLogStateP");

#endif
}

// Function to search the SimBox looking for the aggregates that are expected to be there
// and perform analysis on them. The UpdateState() message is sent to an analysis object
// if the current simulation time lies within its specified range of times.
//
// We store a vector of all the aggregates the user specified in the control data file, 
// and pass the CSimState to each of them to allow them to access all the data on beads, 
// bonds and polymers that they need. The one exception to
// this is that they also need the CNT cell structure to speed up the partitioning of beads
// into the various regions used for analysis (slices, rows and cells). To allow this
// we pass the ISimBox pointer in giving access to the CNT cells via the interface defined
// in that class. This is preferable to passing in the SimBox pointer because it decouples
// the actual SimBox implementation behind the ISimBox interface class.

void CMonitor::FindAggregates()
{
	for(AggregateIterator iterAgg=m_vAggregates.begin(); iterAgg!=m_vAggregates.end(); iterAgg++)
	{
		if((*iterAgg)->TimeToSample(GetCurrentTime()))
			(*iterAgg)->UpdateState(*m_pSimState, GetISimBox());
	}

}

// Function to serialize the aggregate state objects to file. These contain the
// time-dependent data obtained from all the CAnalysis-derived objects (aggregates)
// created during the run. To allow for the possibility of each aggregate having
// a different sample/analysis period we send the Serialize() message to each
// after checking if it is time to analyse.

void CMonitor::SaveAggregateState() const
{
	for(cAggregateIterator iterAgg=m_vAggregates.begin(); iterAgg!=m_vAggregates.end(); iterAgg++)
	{
		if( (*iterAgg)->TimeToAnalyse(GetCurrentTime(), m_pSimState->GetAnalysisPeriod()) &&
		   !(*iterAgg)->Serialize() )
			ErrorTrace("Error in CMonitor::SaveAggregateState");
	}
}

// Function used by the CCNTCell class to pass the bead-bead contributions to
// the stress tensor for analysis. We check the special cases for the two bead Z 
// coordinates, and adjust the f(z1,z2,zs) factor according to the results in 
// Appendix B of Goetz and Lipowsky, 1998.
//
// The data are stored in a set of 9 vectors using an index created from the bead types. 
// We store the inverse distance that is used to calculate the approximate number of 
// analysis slices between the two beads in the appropriate normal direction to avoid 
// having to do 9 divisions.
//
// Because we form a running sum of the stress contributions, the ZeroSliceStress() 
// function must be called before the first call of AddBeadStress() for each time-step.
//
// The function is currently hard-wired for z-axis slices only.
//

void CMonitor::AddBeadStress(const CAbstractBead* const pBead1, const CAbstractBead* const pBead2, const double force[3], const double dx[3])
{
	// Calculate the f(z1, z2, zs) factors that determine how much the bead interaction
	// contributes to each slice zs noting that more than one slice may be involved.
	// We first sort the two beads to find out which one is above the other or whether
	// they have equal z coordinates. In doing this we take the PBCs into account:
	// dx[2] has already been corrected for two beads that interact across the SimBox
	// boundaries, we only need to ensure that the slice index used for evaluating
	// the contributions to adjacent or multiple slices is correctly zeroed when it
	// goes beyond the grid size. 
	//
	// We store the contributions to each bead-pair in a matrix using the first bead
	// type followed by the second. There should not be a distinction between 
	// contributions to the bead-bead stresses from beads whose types are, e.g, 
	// head-water and water-head. Howevre, to avoid having to sort the types
	// (an order N*N/2 process) we store them separately and then sum them later
	// in the CAnalysis objects where they are used. This only requires calculations
	// that depend on the number of slices used in the stress profile.
	// This is an independent problem to the one of which bead lies above the other.
	// That problem affects the component of the actual stress tensor not the matrix
	// of contributions from the different bead-bead types.

	const long stressTypeIndex = pBead1->GetType()*m_BeadTypeSize + pBead2->GetType();

	// Store the product of force*distance for the current interaction. Note that 
	// because the force is central it does not change sign under reversal of the
	// order of the two bead indices. We do not need a minus sign because we have
	// already converted to force from the gradient of the potential.
	
	m_fdx[0] = force[0]*dx[0];
	m_fdx[1] = force[0]*dx[1];
	m_fdx[2] = force[0]*dx[2];
	m_fdx[3] = force[1]*dx[0];
	m_fdx[4] = force[1]*dx[1];
	m_fdx[5] = force[1]*dx[2];
	m_fdx[6] = force[2]*dx[0];
	m_fdx[7] = force[2]*dx[1];
	m_fdx[8] = force[2]*dx[2];

	double zDiff = dx[2];

	if(zDiff > 0.00001)			// Bead 1 above 2
	{
		m_UpperStressSliceCoord = pBead1->m_Pos[2]*m_invSliceWidth;
		m_LowerStressSliceCoord = pBead2->m_Pos[2]*m_invSliceWidth;
	}
	else if(zDiff < -0.00001)	// Bead 2 above 1
	{
		zDiff = -zDiff;	// Change sign so that calculation of f() uses a positive distance
		m_UpperStressSliceCoord = pBead2->m_Pos[2]*m_invSliceWidth;
		m_LowerStressSliceCoord = pBead1->m_Pos[2]*m_invSliceWidth;
	}
	else
	{
		m_UpperStressSliceCoord = pBead1->m_Pos[2]*m_invSliceWidth;
		m_LowerStressSliceCoord = m_UpperStressSliceCoord;
	}

	m_UpperStressSliceIndex = static_cast<long>(m_UpperStressSliceCoord);
	m_UpperStressSliceRem   = m_UpperStressSliceCoord - static_cast<double>(m_UpperStressSliceIndex);

	m_LowerStressSliceIndex = static_cast<long>(m_LowerStressSliceCoord);
	m_LowerStressSliceRem   = m_LowerStressSliceCoord - static_cast<double>(m_LowerStressSliceIndex);

	// The next case takes care of coincident and non-coincident beads within a slice and
	// those coincident on a slice border.

	if(m_UpperStressSliceIndex == m_LowerStressSliceIndex)	// Beads in same slice: assign f() = 1
	{
		if(m_UpperStressSliceCoord != m_LowerStressSliceCoord ||
		   m_UpperStressSliceRem > 0.00001)				// Beads not coincident or not on border
		{
			m_vvSliceStress11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0];
			m_vvSliceStress12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1];
			m_vvSliceStress13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2];
			m_vvSliceStress21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3];
			m_vvSliceStress22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4];
			m_vvSliceStress23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5];
			m_vvSliceStress31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6];
			m_vvSliceStress32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7];
			m_vvSliceStress33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8];
		}
		else	// Beads coincident on a slice border: assign f() = 0.5 to each slice
		{
			m_vvSliceStress11.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*0.5;
			m_vvSliceStress12.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*0.5;
			m_vvSliceStress13.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*0.5;
			m_vvSliceStress21.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*0.5;
			m_vvSliceStress22.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*0.5;
			m_vvSliceStress23.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*0.5;
			m_vvSliceStress31.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*0.5;
			m_vvSliceStress32.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*0.5;
			m_vvSliceStress33.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*0.5;

			// Check the pbcs for the slice beneath the one on whose border the two beads lie

			if(--m_LowerStressSliceIndex < 0)
				m_LowerStressSliceIndex += m_StressSliceTotal;

			m_vvSliceStress11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*0.5;
			m_vvSliceStress12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*0.5;
			m_vvSliceStress13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*0.5;
			m_vvSliceStress21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*0.5;
			m_vvSliceStress22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*0.5;
			m_vvSliceStress23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*0.5;
			m_vvSliceStress31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*0.5;
			m_vvSliceStress32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*0.5;
			m_vvSliceStress33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*0.5;
		}
	}
	else	// Multiple slices: f() = Slice width/zDiff for all inner slices and 
	{		// proportional for first and last
		// Do first and last slices manually and loop over the inner ones that all have
		// a contribution of the ratio of the slice width to the distance between the
		// two particles. Note that the lower and upper slices must be at least
		// two slices apart because adjacent slices have been dealt with above. 
		// When the upper bead coincides with a slice border it is handled by the
		// loop so we don't need a separate contribution. But when it falls within
		// a slice it must be calculated.

		// If the interaction crosses the PBCs, indicated by the upper slice having
		// an index smaller than the lower slice, we add m_StressSliceTotal to the 
		// upper limit and check the index for being greater or equal to the number of 
		// grid slices. When it passes through the maximum we subtract m_StressSliceTotal 
		// to bring it back within the correct range.

		const double innerInvComp = m_StressSliceWidth/zDiff;

		if(m_UpperStressSliceIndex > m_LowerStressSliceIndex)
		{
			for(long sliceIndex=m_LowerStressSliceIndex+1; sliceIndex<=m_UpperStressSliceIndex-1; sliceIndex++)
			{
				m_vvSliceStress11.at(sliceIndex).at(stressTypeIndex) += m_fdx[0]*innerInvComp;
				m_vvSliceStress12.at(sliceIndex).at(stressTypeIndex) += m_fdx[1]*innerInvComp;
				m_vvSliceStress13.at(sliceIndex).at(stressTypeIndex) += m_fdx[2]*innerInvComp;
				m_vvSliceStress21.at(sliceIndex).at(stressTypeIndex) += m_fdx[3]*innerInvComp;
				m_vvSliceStress22.at(sliceIndex).at(stressTypeIndex) += m_fdx[4]*innerInvComp;
				m_vvSliceStress23.at(sliceIndex).at(stressTypeIndex) += m_fdx[5]*innerInvComp;
				m_vvSliceStress31.at(sliceIndex).at(stressTypeIndex) += m_fdx[6]*innerInvComp;
				m_vvSliceStress32.at(sliceIndex).at(stressTypeIndex) += m_fdx[7]*innerInvComp;
				m_vvSliceStress33.at(sliceIndex).at(stressTypeIndex) += m_fdx[8]*innerInvComp;
			}
		}
		else
		{
			for(long sliceIndexPBC=m_LowerStressSliceIndex+1; sliceIndexPBC<=m_StressSliceTotal+m_UpperStressSliceIndex-1; sliceIndexPBC++)
			{
				long sliceIndex = sliceIndexPBC;

				if(sliceIndex >= m_StressSliceTotal)
					sliceIndex -= m_StressSliceTotal;

				m_vvSliceStress11.at(sliceIndex).at(stressTypeIndex) += m_fdx[0]*innerInvComp;
				m_vvSliceStress12.at(sliceIndex).at(stressTypeIndex) += m_fdx[1]*innerInvComp;
				m_vvSliceStress13.at(sliceIndex).at(stressTypeIndex) += m_fdx[2]*innerInvComp;
				m_vvSliceStress21.at(sliceIndex).at(stressTypeIndex) += m_fdx[3]*innerInvComp;
				m_vvSliceStress22.at(sliceIndex).at(stressTypeIndex) += m_fdx[4]*innerInvComp;
				m_vvSliceStress23.at(sliceIndex).at(stressTypeIndex) += m_fdx[5]*innerInvComp;
				m_vvSliceStress31.at(sliceIndex).at(stressTypeIndex) += m_fdx[6]*innerInvComp;
				m_vvSliceStress32.at(sliceIndex).at(stressTypeIndex) += m_fdx[7]*innerInvComp;
				m_vvSliceStress33.at(sliceIndex).at(stressTypeIndex) += m_fdx[8]*innerInvComp;
			}
		}

		if(m_LowerStressSliceRem > 0.000001)	// Lower bead not on slice border
		{
			const double lowerInvComp = (1.0 - m_LowerStressSliceRem)*innerInvComp;
			m_vvSliceStress11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*lowerInvComp;
			m_vvSliceStress12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*lowerInvComp;
			m_vvSliceStress13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*lowerInvComp;
			m_vvSliceStress21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*lowerInvComp;
			m_vvSliceStress22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*lowerInvComp;
			m_vvSliceStress23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*lowerInvComp;
			m_vvSliceStress31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*lowerInvComp;
			m_vvSliceStress32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*lowerInvComp;
			m_vvSliceStress33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*lowerInvComp;
		}
		else									// Lower bead is on slice border
		{
			m_vvSliceStress11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*innerInvComp;
			m_vvSliceStress12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*innerInvComp;
			m_vvSliceStress13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*innerInvComp;
			m_vvSliceStress21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*innerInvComp;
			m_vvSliceStress22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*innerInvComp;
			m_vvSliceStress23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*innerInvComp;
			m_vvSliceStress31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*innerInvComp;
			m_vvSliceStress32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*innerInvComp;
			m_vvSliceStress33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*innerInvComp;
		}

		if(m_UpperStressSliceRem > 0.000001)	// Upper bead not on slice border
		{
			const double upperInvComp = m_UpperStressSliceRem*innerInvComp;
			m_vvSliceStress11.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*upperInvComp;
			m_vvSliceStress12.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*upperInvComp;
			m_vvSliceStress13.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*upperInvComp;
			m_vvSliceStress21.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*upperInvComp;
			m_vvSliceStress22.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*upperInvComp;
			m_vvSliceStress23.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*upperInvComp;
			m_vvSliceStress31.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*upperInvComp;
			m_vvSliceStress32.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*upperInvComp;
			m_vvSliceStress33.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*upperInvComp;
		}
	}
}

// Function called by the CSimBox to zero the stress arrays prior to adding 
// new data in the CCNTCell::UpdateForce() and CSimBox functions.

void CMonitor::ZeroSliceStress()
{
	for(long i=0; i<m_StressSliceTotal; i++)
	{
		for(long j=0; j<m_StressBeadPairTotal; j++)
		{
			m_vvSliceStress11.at(i).at(j) = 0.0;
			m_vvSliceStress12.at(i).at(j) = 0.0;
			m_vvSliceStress13.at(i).at(j) = 0.0;
			m_vvSliceStress21.at(i).at(j) = 0.0;
			m_vvSliceStress22.at(i).at(j) = 0.0;
			m_vvSliceStress23.at(i).at(j) = 0.0;
			m_vvSliceStress31.at(i).at(j) = 0.0;
			m_vvSliceStress32.at(i).at(j) = 0.0;
			m_vvSliceStress33.at(i).at(j) = 0.0;
		}

		for(long k=0; k<m_BondTypeSize; k++)
		{
			m_vvSliceStressBond11.at(i).at(k) = 0.0;
			m_vvSliceStressBond12.at(i).at(k) = 0.0;
			m_vvSliceStressBond13.at(i).at(k) = 0.0;
			m_vvSliceStressBond21.at(i).at(k) = 0.0;
			m_vvSliceStressBond22.at(i).at(k) = 0.0;
			m_vvSliceStressBond23.at(i).at(k) = 0.0;
			m_vvSliceStressBond31.at(i).at(k) = 0.0;
			m_vvSliceStressBond32.at(i).at(k) = 0.0;
			m_vvSliceStressBond33.at(i).at(k) = 0.0;
		}

		for(long l=0; l<m_BondPairTypeSize; l++)
		{
			m_vvSliceStressBondPair11.at(i).at(l) = 0.0;
			m_vvSliceStressBondPair12.at(i).at(l) = 0.0;
			m_vvSliceStressBondPair13.at(i).at(l) = 0.0;
			m_vvSliceStressBondPair21.at(i).at(l) = 0.0;
			m_vvSliceStressBondPair22.at(i).at(l) = 0.0;
			m_vvSliceStressBondPair23.at(i).at(l) = 0.0;
			m_vvSliceStressBondPair31.at(i).at(l) = 0.0;
			m_vvSliceStressBondPair32.at(i).at(l) = 0.0;
			m_vvSliceStressBondPair33.at(i).at(l) = 0.0;
		}
	}

}

// Function to set up the storage needed to hold the stress tensor data.
// There are three contributions: bead-bead interactions calculated by the
// CCNTCell class; bond contributions calculated in the CSimBox; and
// bondpair contributions also calculated in the CSimBox. We use the analysis 
// grid size to set the slices used for the stress analysis, but note that the 
// analysis grid is not necessarily the same as the CNT grid.
//
// Separate arrays are used to hold the different contributions but they are
// all initialised here because they all represent contributions to the overall
// stress tensor.

void CMonitor::InitialiseSliceStress()
{
	// Set aside space for the stress tensor data for all beads. We need
	// space for all possible bead-bead pairs and the total number of
	// slices into which the SimBox is divided for the stress analysis. 
	// Given that there are m_BeadTypeSize bead types, we have
	//
	// m_BeadTypeSize*m_BeadTypeSize
	//
	// pairs. This is defined in the construdtor. The number of slices is 
	// obtained from the analysis grid size in the bilayer normal direction.  
	// It is hardwired to the Z axis at the moment but cannot be declared 
	// const incase we want to use the routine for the other axes.
	//
	// The bond and bondpair arrays are initialised from the number of 
	// bond and bondpair types, m_BondTypeSize, m_BondPairTypeSize.

	m_StressSliceTotal    = m_GridZCellNo;
	m_StressSliceWidth	  = m_GridZCellWidth;
	m_invSliceWidth		  = 1.0/m_StressSliceWidth;

	m_vvSliceStress11.resize(m_StressSliceTotal);
	m_vvSliceStress12.resize(m_StressSliceTotal);
	m_vvSliceStress13.resize(m_StressSliceTotal);
	m_vvSliceStress21.resize(m_StressSliceTotal);
	m_vvSliceStress22.resize(m_StressSliceTotal);
	m_vvSliceStress23.resize(m_StressSliceTotal);
	m_vvSliceStress31.resize(m_StressSliceTotal);
	m_vvSliceStress32.resize(m_StressSliceTotal);
	m_vvSliceStress33.resize(m_StressSliceTotal);

	// Now set aside space for the bond contributions to the stress tensor

	m_vvSliceStressBond11.resize(m_StressSliceTotal);
	m_vvSliceStressBond12.resize(m_StressSliceTotal);
	m_vvSliceStressBond13.resize(m_StressSliceTotal);
	m_vvSliceStressBond21.resize(m_StressSliceTotal);
	m_vvSliceStressBond22.resize(m_StressSliceTotal);
	m_vvSliceStressBond23.resize(m_StressSliceTotal);
	m_vvSliceStressBond31.resize(m_StressSliceTotal);
	m_vvSliceStressBond32.resize(m_StressSliceTotal);
	m_vvSliceStressBond33.resize(m_StressSliceTotal);


	// and the bondpair contributions

	m_vvSliceStressBondPair11.resize(m_StressSliceTotal);
	m_vvSliceStressBondPair12.resize(m_StressSliceTotal);
	m_vvSliceStressBondPair13.resize(m_StressSliceTotal);
	m_vvSliceStressBondPair21.resize(m_StressSliceTotal);
	m_vvSliceStressBondPair22.resize(m_StressSliceTotal);
	m_vvSliceStressBondPair23.resize(m_StressSliceTotal);
	m_vvSliceStressBondPair31.resize(m_StressSliceTotal);
	m_vvSliceStressBondPair32.resize(m_StressSliceTotal);
	m_vvSliceStressBondPair33.resize(m_StressSliceTotal);

	for(long i=0; i<m_StressSliceTotal; i++)
	{
		m_vvSliceStress11.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress12.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress13.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress21.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress22.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress23.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress31.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress32.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress33.at(i).resize(m_StressBeadPairTotal);

		m_vvSliceStressBond11.at(i).resize(m_BondTypeSize);
		m_vvSliceStressBond12.at(i).resize(m_BondTypeSize);
		m_vvSliceStressBond13.at(i).resize(m_BondTypeSize);
		m_vvSliceStressBond21.at(i).resize(m_BondTypeSize);
		m_vvSliceStressBond22.at(i).resize(m_BondTypeSize);
		m_vvSliceStressBond23.at(i).resize(m_BondTypeSize);
		m_vvSliceStressBond31.at(i).resize(m_BondTypeSize);
		m_vvSliceStressBond32.at(i).resize(m_BondTypeSize);
		m_vvSliceStressBond33.at(i).resize(m_BondTypeSize);

		m_vvSliceStressBondPair11.at(i).resize(m_BondPairTypeSize);
		m_vvSliceStressBondPair12.at(i).resize(m_BondPairTypeSize);
		m_vvSliceStressBondPair13.at(i).resize(m_BondPairTypeSize);
		m_vvSliceStressBondPair21.at(i).resize(m_BondPairTypeSize);
		m_vvSliceStressBondPair22.at(i).resize(m_BondPairTypeSize);
		m_vvSliceStressBondPair23.at(i).resize(m_BondPairTypeSize);
		m_vvSliceStressBondPair31.at(i).resize(m_BondPairTypeSize);
		m_vvSliceStressBondPair32.at(i).resize(m_BondPairTypeSize);
		m_vvSliceStressBondPair33.at(i).resize(m_BondPairTypeSize);
	}

	// Use the private member function to zero the stress arrays

	ZeroSliceStress();
}

// Function used by the CSimBox class to pass the bond contributions to
// the stress tensor for summation. There are two cases:
//
// 1) The stress is averaged over Z-normal slices across the SimBox - this function
// 2) The stress is averaged over CNT cells in 3 dimensions - deferred to CSimBox::AddBondStressSphere()
//
// The first calculation is relevant for planar membranes, and the second for spherical vesicles.
//
// Planar stress profile notes
// ***************************
// We check the special cases for the two bead Z 
// coordinates, and adjust the f(z1,z2,zs) factor according to the results in 
// Appendix B of Goetz and Lipowsky, 1998.
//
// The data are stored in a set of 9 vectors using an index created from the bead types. 
// We store the inverse distance that is used to calculate the approximate number of 
// analysis slices between the two beads in the appropriate normal direction to avoid 
// having to do 9 divisions.
//
//
// The function is currently hard-wired for z-axis slices only.
//

void CMonitor::AddBondStress(const CBond* const pBond)
{
	// Calculate the f(z1, z2, zs) factors that determine how much the bead interaction
	// contributes to each slice zs noting that more than one slice may be involved.
	// We first sort the two beads to find out which one is above the other or whether
	// they have equal z coordinates. In doing this we take the PBCs into account:
	// dx[2] has already been corrected for two beads that interact across the SimBox
	// boundaries, we only need to ensure that the slice index used for evaluating
	// the contributions to adjacent or multiple slices is correctly zeroed when it
	// goes beyond the grid size. 
	//
	// We store the contributions to each bead-pair in a matrix using the first bead
	// type followed by the second. There should not be a distinction between 
	// contributions to the bead-bead stresses from beads whose types are, e.g, 
	// head-water and water-head. Howevre, to avoid having to sort the types
	// (an order N*N/2 process) we store them separately and then sum them later
	// in the CAnalysis objects where they are used. This only requires calculations
	// that depend on the number of slices used in the stress profile.
	// This is an independent problem to the one of which bead lies above the other.
	// That problem affects the component of the actual stress tensor not the matrix
	// of contributions from the different bead-bead types.

	const long stressTypeIndex = pBond->GetType();

	// Store the product of force*distance for the current interaction. We choose
	// the beads in the order Head - Tail: hence the force is directed from the 
	// head back towards the tail but the length vector is in the reverse direction.

	double dx[3], force[3];

	dx[0]	 = pBond->m_dx;
	dx[1]	 = pBond->m_dy;
	dx[2]	 = pBond->m_dz;

	force[0] = pBond->m_fx;
	force[1] = pBond->m_fy;
	force[2] = pBond->m_fz;

	m_fdx[0] = force[0]*dx[0];
	m_fdx[1] = force[0]*dx[1];
	m_fdx[2] = force[0]*dx[2];
	m_fdx[3] = force[1]*dx[0];
	m_fdx[4] = force[1]*dx[1];
	m_fdx[5] = force[1]*dx[2];
	m_fdx[6] = force[2]*dx[0];
	m_fdx[7] = force[2]*dx[1];
	m_fdx[8] = force[2]*dx[2];
	
	double zDiff = dx[2];

	// Note that we have to use the PBC-corrected coordinates of the head and tail
	// not the unPBC ones returned by the CBond access functions. The Z coordinate is
	// used to identify the grid slices containing the head and tail.

	if(zDiff > 0.00001)			// Head Bead above Tail
	{
		m_UpperStressSliceCoord = pBond->GetHead()->GetZPos()*m_invSliceWidth;
		m_LowerStressSliceCoord = pBond->GetTail()->GetZPos()*m_invSliceWidth;
	}
	else if(zDiff < -0.00001)	// Tail Bead above Head
	{
		zDiff = -zDiff;	// Change sign so that calculation of f() uses a positive distance
		m_UpperStressSliceCoord = pBond->GetTail()->GetZPos()*m_invSliceWidth;
		m_LowerStressSliceCoord = pBond->GetHead()->GetZPos()*m_invSliceWidth;
	}
	else
	{
		m_UpperStressSliceCoord = pBond->GetHead()->GetZPos()*m_invSliceWidth;
		m_LowerStressSliceCoord = m_UpperStressSliceCoord;
	}

	m_UpperStressSliceIndex = static_cast<long>(m_UpperStressSliceCoord);
	m_UpperStressSliceRem   = m_UpperStressSliceCoord - static_cast<double>(m_UpperStressSliceIndex);

	m_LowerStressSliceIndex = static_cast<long>(m_LowerStressSliceCoord);
	m_LowerStressSliceRem   = m_LowerStressSliceCoord - static_cast<double>(m_LowerStressSliceIndex);

	// The next case takes care of coincident and non-coincident beads within a slice and
	// those coincident on a slice border.

	if(m_UpperStressSliceIndex == m_LowerStressSliceIndex)	// Beads in same slice: assign f() = 1
	{
		if(m_UpperStressSliceCoord != m_LowerStressSliceCoord ||
		   m_UpperStressSliceRem > 0.00001)				// Beads not coincident or not on border
		{
			m_vvSliceStressBond11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0];
			m_vvSliceStressBond12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1];
			m_vvSliceStressBond13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2];
			m_vvSliceStressBond21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3];
			m_vvSliceStressBond22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4];
			m_vvSliceStressBond23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5];
			m_vvSliceStressBond31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6];
			m_vvSliceStressBond32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7];
			m_vvSliceStressBond33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8];
		}
		else	// Beads coincident on a slice border: assign f() = 0.5 to each slice
		{
			m_vvSliceStressBond11.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*0.5;
			m_vvSliceStressBond12.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*0.5;
			m_vvSliceStressBond13.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*0.5;
			m_vvSliceStressBond21.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*0.5;
			m_vvSliceStressBond22.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*0.5;
			m_vvSliceStressBond23.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*0.5;
			m_vvSliceStressBond31.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*0.5;
			m_vvSliceStressBond32.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*0.5;
			m_vvSliceStressBond33.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*0.5;

			// Check the pbcs for the slice beneath the one on whose border the two beads lie

			if(--m_LowerStressSliceIndex < 0)
				m_LowerStressSliceIndex += m_StressSliceTotal;

			m_vvSliceStressBond11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*0.5;
			m_vvSliceStressBond12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*0.5;
			m_vvSliceStressBond13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*0.5;
			m_vvSliceStressBond21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*0.5;
			m_vvSliceStressBond22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*0.5;
			m_vvSliceStressBond23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*0.5;
			m_vvSliceStressBond31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*0.5;
			m_vvSliceStressBond32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*0.5;
			m_vvSliceStressBond33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*0.5;
		}
	}
	else	// Multiple slices: f() = Slice width/zDiff for all inner slices and 
	{		// proportional for first and last
		// Do first and last slices manually and loop over the inner ones that all have
		// a contribution of the ratio of the slice width to the distance between the
		// two particles. Note that the lower and upper slices must be at least
		// two slices apart because adjacent slices have been dealt with above. 
		// When the upper bead coincides with a slice border it is handled by the
		// loop so we don't need a separate contribution. But when it falls within
		// a slice it must be calculated.

		// If the interaction crosses the PBCs, indicated by the upper slice having
		// an index smaller than the lower slice, we add m_StressSliceTotal to the 
		// upper limit and check the index for being greater or equal to the number of 
		// grid slices. When it passes through the maximum we subtract m_StressSliceTotal 
		// to bring it back within the correct range.

		const double innerInvComp = m_StressSliceWidth/zDiff;

		if(m_UpperStressSliceIndex > m_LowerStressSliceIndex)
		{
			for(long sliceIndex=m_LowerStressSliceIndex+1; sliceIndex<=m_UpperStressSliceIndex-1; sliceIndex++)
			{
				m_vvSliceStressBond11.at(sliceIndex).at(stressTypeIndex) += m_fdx[0]*innerInvComp;
				m_vvSliceStressBond12.at(sliceIndex).at(stressTypeIndex) += m_fdx[1]*innerInvComp;
				m_vvSliceStressBond13.at(sliceIndex).at(stressTypeIndex) += m_fdx[2]*innerInvComp;
				m_vvSliceStressBond21.at(sliceIndex).at(stressTypeIndex) += m_fdx[3]*innerInvComp;
				m_vvSliceStressBond22.at(sliceIndex).at(stressTypeIndex) += m_fdx[4]*innerInvComp;
				m_vvSliceStressBond23.at(sliceIndex).at(stressTypeIndex) += m_fdx[5]*innerInvComp;
				m_vvSliceStressBond31.at(sliceIndex).at(stressTypeIndex) += m_fdx[6]*innerInvComp;
				m_vvSliceStressBond32.at(sliceIndex).at(stressTypeIndex) += m_fdx[7]*innerInvComp;
				m_vvSliceStressBond33.at(sliceIndex).at(stressTypeIndex) += m_fdx[8]*innerInvComp;
			}
		}
		else
		{
			for(long sliceIndexPBC=m_LowerStressSliceIndex+1; sliceIndexPBC<=m_StressSliceTotal+m_UpperStressSliceIndex-1; sliceIndexPBC++)
			{
				long sliceIndex = sliceIndexPBC;

				if(sliceIndex >= m_StressSliceTotal)
					sliceIndex -= m_StressSliceTotal;

				m_vvSliceStressBond11.at(sliceIndex).at(stressTypeIndex) += m_fdx[0]*innerInvComp;
				m_vvSliceStressBond12.at(sliceIndex).at(stressTypeIndex) += m_fdx[1]*innerInvComp;
				m_vvSliceStressBond13.at(sliceIndex).at(stressTypeIndex) += m_fdx[2]*innerInvComp;
				m_vvSliceStressBond21.at(sliceIndex).at(stressTypeIndex) += m_fdx[3]*innerInvComp;
				m_vvSliceStressBond22.at(sliceIndex).at(stressTypeIndex) += m_fdx[4]*innerInvComp;
				m_vvSliceStressBond23.at(sliceIndex).at(stressTypeIndex) += m_fdx[5]*innerInvComp;
				m_vvSliceStressBond31.at(sliceIndex).at(stressTypeIndex) += m_fdx[6]*innerInvComp;
				m_vvSliceStressBond32.at(sliceIndex).at(stressTypeIndex) += m_fdx[7]*innerInvComp;
				m_vvSliceStressBond33.at(sliceIndex).at(stressTypeIndex) += m_fdx[8]*innerInvComp;
			}
		}

		if(m_LowerStressSliceRem > 0.000001)	// Lower bead not on slice border
		{
			const double lowerInvComp = (1.0 - m_LowerStressSliceRem)*innerInvComp;
			m_vvSliceStressBond11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*lowerInvComp;
			m_vvSliceStressBond12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*lowerInvComp;
			m_vvSliceStressBond13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*lowerInvComp;
			m_vvSliceStressBond21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*lowerInvComp;
			m_vvSliceStressBond22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*lowerInvComp;
			m_vvSliceStressBond23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*lowerInvComp;
			m_vvSliceStressBond31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*lowerInvComp;
			m_vvSliceStressBond32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*lowerInvComp;
			m_vvSliceStressBond33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*lowerInvComp;
		}
		else									// Lower bead is on slice border
		{
			m_vvSliceStressBond11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*innerInvComp;
			m_vvSliceStressBond12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*innerInvComp;
			m_vvSliceStressBond13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*innerInvComp;
			m_vvSliceStressBond21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*innerInvComp;
			m_vvSliceStressBond22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*innerInvComp;
			m_vvSliceStressBond23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*innerInvComp;
			m_vvSliceStressBond31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*innerInvComp;
			m_vvSliceStressBond32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*innerInvComp;
			m_vvSliceStressBond33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*innerInvComp;
		}

		if(m_UpperStressSliceRem > 0.000001)	// Upper bead not on slice border
		{
			const double upperInvComp = m_UpperStressSliceRem*innerInvComp;
			m_vvSliceStressBond11.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*upperInvComp;
			m_vvSliceStressBond12.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*upperInvComp;
			m_vvSliceStressBond13.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*upperInvComp;
			m_vvSliceStressBond21.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*upperInvComp;
			m_vvSliceStressBond22.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*upperInvComp;
			m_vvSliceStressBond23.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*upperInvComp;
			m_vvSliceStressBond31.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*upperInvComp;
			m_vvSliceStressBond32.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*upperInvComp;
			m_vvSliceStressBond33.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*upperInvComp;
		}
	}

}

// Function to add the contributions to the stress tensor of the stiff bonds 
// (CBondPair) defined as a linear sequence of three beads, e.g., B B B.
// There are three contributions from each stiff bond, corresponding to the
// three bead pairs. However, the force is not a simple sum of three two-body
// forces. The force on each bead in the triple is calculated when the equation
// of motion is integrated int eh CBondPair::AddForce() routine. It stores the
// forces on the three beads due to the bond stiffness so that they are available
// here. It also stores the force on the first bead twice so that the difference in
// forces can be easily calculated from the difference of adjacent elements in the array.
// The inter-bead distances are obtained from the beads at either end of the
// two bonds that define a CbondPair object. We use the coordinates that have not
// been corrected for the PBCs so that we obtain the correct relative positions
// when two beads are at opposite ends of the SimBox. Given the bead positions and 
// forces, this routine follows the same calculation of the f(zi, zj, zs) functions 
// as for the bead-bead and bond forces. However, we include the loop over the three
// bead-pairs within the routine instead of making three function calls.

void CMonitor::AddBondPairStress(const CBondPair* const pBondPair)
{
	// Calculate the f(z1, z2, zs) factors that determine how much the interaction
	// contributes to each slice, zs, noting that more than one slice may be involved.
	// We first sort the two beads to find out which one is above the other or whether
	// they have equal z coordinates. In doing this we take the PBCs into account:
	// The bead coordinates have already been corrected for two beads that interact 
	// across the SimBox boundaries, we only need to ensure that the slice index 
	// used for evaluating the contributions to adjacent or multiple slices is 
	// correctly zeroed when it goes beyond the grid size. 
	//
	// We add the contributions from each bead-pair and store them in a matrix using 
	// the bondpair type as the index. 

	const long stressTypeIndex = pBondPair->GetType();

	// Copy the bead pointers into a local array and repeat the first pointer at 
	// the end to make the calculation of adjacent pairs simpler.

	CAbstractBead* pBeadArray[4];

	pBeadArray[0] = pBondPair->GetFirst()->GetTail();
	pBeadArray[1] = pBondPair->GetFirst()->GetHead();
	pBeadArray[2] = pBondPair->GetSecond()->GetHead();
	pBeadArray[3] = pBondPair->GetFirst()->GetTail();

	for(short int j=0; j<3; j++)
	{
        const short int jp1 = static_cast<short int>(j+1);

		double dx[3];
		double force[3];

		dx[0] = pBeadArray[jp1]->GetunPBCXPos() - pBeadArray[j]->GetunPBCXPos();
		dx[1] = pBeadArray[jp1]->GetunPBCYPos() - pBeadArray[j]->GetunPBCYPos();
		dx[2] = pBeadArray[jp1]->GetunPBCZPos() - pBeadArray[j]->GetunPBCZPos();

		force[0] = 0.33333333*(pBondPair->GetXForce(jp1) - pBondPair->GetXForce(j));
		force[1] = 0.33333333*(pBondPair->GetYForce(jp1) - pBondPair->GetYForce(j));
		force[2] = 0.33333333*(pBondPair->GetZForce(jp1) - pBondPair->GetZForce(j));

		// Store the product of force*distance for the current interaction. 

		m_fdx[0] = force[0]*dx[0];
		m_fdx[1] = force[0]*dx[1];
		m_fdx[2] = force[0]*dx[2];
		m_fdx[3] = force[1]*dx[0];
		m_fdx[4] = force[1]*dx[1];
		m_fdx[5] = force[1]*dx[2];
		m_fdx[6] = force[2]*dx[0];
		m_fdx[7] = force[2]*dx[1];
		m_fdx[8] = force[2]*dx[2];

		double zDiff = dx[2];

		if(zDiff > 0.00001)			// Bead j+1 above j
		{
			m_UpperStressSliceCoord = pBeadArray[jp1]->GetZPos()*m_invSliceWidth;
			m_LowerStressSliceCoord = pBeadArray[j]->GetZPos()*m_invSliceWidth;
		}
		else if(zDiff < -0.00001)	// Bead j above j+1
		{
			zDiff = -zDiff;	// Change sign so that calculation of f() uses a positive distance
			m_UpperStressSliceCoord = pBeadArray[j]->GetZPos()*m_invSliceWidth;
			m_LowerStressSliceCoord = pBeadArray[jp1]->GetZPos()*m_invSliceWidth;
		}
		else
		{
			m_UpperStressSliceCoord = pBeadArray[jp1]->GetZPos()*m_invSliceWidth;
			m_LowerStressSliceCoord = m_UpperStressSliceCoord;
		}

		m_UpperStressSliceIndex = static_cast<long>(m_UpperStressSliceCoord);
		m_UpperStressSliceRem   = m_UpperStressSliceCoord - static_cast<double>(m_UpperStressSliceIndex);

		m_LowerStressSliceIndex = static_cast<long>(m_LowerStressSliceCoord);
		m_LowerStressSliceRem   = m_LowerStressSliceCoord - static_cast<double>(m_LowerStressSliceIndex);

		// The next case takes care of coincident and non-coincident beads within a slice and
		// those coincident on a slice border.

		if(m_UpperStressSliceIndex == m_LowerStressSliceIndex)	// Beads in same slice: assign f() = 1
		{
			if(m_UpperStressSliceCoord != m_LowerStressSliceCoord ||
			   m_UpperStressSliceRem > 0.00001)				// Beads not coincident or not on border
			{
				m_vvSliceStressBondPair11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0];
				m_vvSliceStressBondPair12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1];
				m_vvSliceStressBondPair13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2];
				m_vvSliceStressBondPair21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3];
				m_vvSliceStressBondPair22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4];
				m_vvSliceStressBondPair23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5];
				m_vvSliceStressBondPair31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6];
				m_vvSliceStressBondPair32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7];
				m_vvSliceStressBondPair33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8];
			}
			else	// Beads coincident on a slice border: assign f() = 0.5 to each slice
			{
				m_vvSliceStressBondPair11.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*0.5;
				m_vvSliceStressBondPair12.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*0.5;
				m_vvSliceStressBondPair13.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*0.5;
				m_vvSliceStressBondPair21.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*0.5;
				m_vvSliceStressBondPair22.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*0.5;
				m_vvSliceStressBondPair23.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*0.5;
				m_vvSliceStressBondPair31.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*0.5;
				m_vvSliceStressBondPair32.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*0.5;
				m_vvSliceStressBondPair33.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*0.5;

				// Check the pbcs for the slice beneath the one on whose border the two beads lie

				if(--m_LowerStressSliceIndex < 0)
					m_LowerStressSliceIndex += m_StressSliceTotal;

				m_vvSliceStressBondPair11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*0.5;
				m_vvSliceStressBondPair12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*0.5;
				m_vvSliceStressBondPair13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*0.5;
				m_vvSliceStressBondPair21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*0.5;
				m_vvSliceStressBondPair22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*0.5;
				m_vvSliceStressBondPair23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*0.5;
				m_vvSliceStressBondPair31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*0.5;
				m_vvSliceStressBondPair32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*0.5;
				m_vvSliceStressBondPair33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*0.5;
			}
		}
		else	// Multiple slices: f() = Slice width/zDiff for all inner slices and 
		{		// proportional for first and last
			// Do first and last slices manually and loop over the inner ones that all have
			// a contribution of the ratio of the slice width to the distance between the
			// two particles. Note that the lower and upper slices must be at least
			// two slices apart because adjacent slices have been dealt with above. 
			// When the upper bead coincides with a slice border it is handled by the
			// loop so we don't need a separate contribution. But when it falls within
			// a slice it must be calculated.

			// If the interaction crosses the PBCs, indicated by the upper slice having
			// an index smaller than the lower slice, we add m_StressSliceTotal to the 
			// upper limit and check the index for being greater or equal to the number of 
			// grid slices. When it passes through the maximum we subtract m_StressSliceTotal 
			// to bring it back within the correct range.

			const double innerInvComp = m_StressSliceWidth/zDiff;

			if(m_UpperStressSliceIndex > m_LowerStressSliceIndex)
			{
				for(long sliceIndex=m_LowerStressSliceIndex+1; sliceIndex<=m_UpperStressSliceIndex-1; sliceIndex++)
				{
					m_vvSliceStressBondPair11.at(sliceIndex).at(stressTypeIndex) += m_fdx[0]*innerInvComp;
					m_vvSliceStressBondPair12.at(sliceIndex).at(stressTypeIndex) += m_fdx[1]*innerInvComp;
					m_vvSliceStressBondPair13.at(sliceIndex).at(stressTypeIndex) += m_fdx[2]*innerInvComp;
					m_vvSliceStressBondPair21.at(sliceIndex).at(stressTypeIndex) += m_fdx[3]*innerInvComp;
					m_vvSliceStressBondPair22.at(sliceIndex).at(stressTypeIndex) += m_fdx[4]*innerInvComp;
					m_vvSliceStressBondPair23.at(sliceIndex).at(stressTypeIndex) += m_fdx[5]*innerInvComp;
					m_vvSliceStressBondPair31.at(sliceIndex).at(stressTypeIndex) += m_fdx[6]*innerInvComp;
					m_vvSliceStressBondPair32.at(sliceIndex).at(stressTypeIndex) += m_fdx[7]*innerInvComp;
					m_vvSliceStressBondPair33.at(sliceIndex).at(stressTypeIndex) += m_fdx[8]*innerInvComp;
				}
			}
			else
			{
				for(long sliceIndexPBC=m_LowerStressSliceIndex+1; sliceIndexPBC<=m_StressSliceTotal+m_UpperStressSliceIndex-1; sliceIndexPBC++)
				{
					long sliceIndex = sliceIndexPBC;

					if(sliceIndex >= m_StressSliceTotal)
						sliceIndex -= m_StressSliceTotal;

					m_vvSliceStressBondPair11.at(sliceIndex).at(stressTypeIndex) += m_fdx[0]*innerInvComp;
					m_vvSliceStressBondPair12.at(sliceIndex).at(stressTypeIndex) += m_fdx[1]*innerInvComp;
					m_vvSliceStressBondPair13.at(sliceIndex).at(stressTypeIndex) += m_fdx[2]*innerInvComp;
					m_vvSliceStressBondPair21.at(sliceIndex).at(stressTypeIndex) += m_fdx[3]*innerInvComp;
					m_vvSliceStressBondPair22.at(sliceIndex).at(stressTypeIndex) += m_fdx[4]*innerInvComp;
					m_vvSliceStressBondPair23.at(sliceIndex).at(stressTypeIndex) += m_fdx[5]*innerInvComp;
					m_vvSliceStressBondPair31.at(sliceIndex).at(stressTypeIndex) += m_fdx[6]*innerInvComp;
					m_vvSliceStressBondPair32.at(sliceIndex).at(stressTypeIndex) += m_fdx[7]*innerInvComp;
					m_vvSliceStressBondPair33.at(sliceIndex).at(stressTypeIndex) += m_fdx[8]*innerInvComp;
				}
			}

			if(m_LowerStressSliceRem > 0.000001)	// Lower bead not on slice border
			{
				const double lowerInvComp = (1.0 - m_LowerStressSliceRem)*innerInvComp;
				m_vvSliceStressBondPair11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*lowerInvComp;
				m_vvSliceStressBondPair12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*lowerInvComp;
				m_vvSliceStressBondPair13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*lowerInvComp;
				m_vvSliceStressBondPair21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*lowerInvComp;
				m_vvSliceStressBondPair22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*lowerInvComp;
				m_vvSliceStressBondPair23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*lowerInvComp;
				m_vvSliceStressBondPair31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*lowerInvComp;
				m_vvSliceStressBondPair32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*lowerInvComp;
				m_vvSliceStressBondPair33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*lowerInvComp;
			}
			else									// Lower bead is on slice border
			{
				m_vvSliceStressBondPair11.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*innerInvComp;
				m_vvSliceStressBondPair12.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*innerInvComp;
				m_vvSliceStressBondPair13.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*innerInvComp;
				m_vvSliceStressBondPair21.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*innerInvComp;
				m_vvSliceStressBondPair22.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*innerInvComp;
				m_vvSliceStressBondPair23.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*innerInvComp;
				m_vvSliceStressBondPair31.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*innerInvComp;
				m_vvSliceStressBondPair32.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*innerInvComp;
				m_vvSliceStressBondPair33.at(m_LowerStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*innerInvComp;
			}

			if(m_UpperStressSliceRem > 0.000001)	// Upper bead not on slice border
			{
				const double upperInvComp = m_UpperStressSliceRem*innerInvComp;
				m_vvSliceStressBondPair11.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[0]*upperInvComp;
				m_vvSliceStressBondPair12.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[1]*upperInvComp;
				m_vvSliceStressBondPair13.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[2]*upperInvComp;
				m_vvSliceStressBondPair21.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[3]*upperInvComp;
				m_vvSliceStressBondPair22.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[4]*upperInvComp;
				m_vvSliceStressBondPair23.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[5]*upperInvComp;
				m_vvSliceStressBondPair31.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[6]*upperInvComp;
				m_vvSliceStressBondPair32.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[7]*upperInvComp;
				m_vvSliceStressBondPair33.at(m_UpperStressSliceIndex).at(stressTypeIndex) += m_fdx[8]*upperInvComp;
			}
		}
	}
}

// Functions to allow the total kinetic and potential energies of the beads 
// in a simulation to be monitored. This is important for MD simulations to 
// check the integration scheme. 
//
// Currently only the bead-bead interactions, bond energies and bondpair energies
// are accumulated. The body force and any forces resulting from commands are not
// added to the total potential energy.
//
// Because a running sum over all the CNT cells is made, the total energies must be 
// zeroed appropriately before starting the summations by calling ZeroTotalEnergy().
// The contributions of the bond and bondpair energies are added in that routine,
// and the bead contributions are added using AddBeadEnergy().


void CMonitor::ZeroTotalEnergy()
{
	m_meanKineticEnergy		= 0.0;
	m_meanPotentialEnergy	= 0.0;

	// Add bond and bondpair potential energies

	for(cBondVectorIterator iterBond=(m_pSimState->GetBonds()).begin(); iterBond!=(m_pSimState->GetBonds()).end(); iterBond++)
	{
		m_meanPotentialEnergy += (*iterBond)->AddPotentialEnergy();
	}

	for(cBondPairVectorIterator iterBP=(m_pSimState->GetBondPairs()).begin(); iterBP!=(m_pSimState->GetBondPairs()).end(); iterBP++)
	{
		m_meanPotentialEnergy += (*iterBP)->AddPotentialEnergy();
	}


}

void CMonitor::AddBeadEnergy(double kinetic, double potential)
{
	m_meanKineticEnergy	  += kinetic;
	m_meanPotentialEnergy += potential;
}

// Function to iterate over all active IRegionAnalysis objects and perform averages
// over bead properties in specifed regions. These may be slices, rows or cells.
// Note that if the region is in the m_vRegions container it is active and is
// sampled. Toggling the command off removes it from the container so there is no
// need to store a lifetime in the commands.
//

void CMonitor::FindRegions()
{
	for(RegionIterator iterRegion=m_ActiveRegions.begin(); iterRegion!=m_ActiveRegions.end(); iterRegion++)
	{
		(*iterRegion)->UpdateState(*m_pSimState, GetISimBox());
	}
}

// Function to write the IRegionAnalysis data to file. Each class derived from
// IRegionAnalysis defines its own xxRegionState object that holds time series data.
// The data is written to file every SamplePeriod steps.

void CMonitor::SaveRegionState() const
{
	for(cRegionIterator iterReg=m_Regions.begin(); iterReg!=m_Regions.end(); iterReg++)
	{
		if( !(*iterReg)->Serialize() )
			ErrorTrace("Error in CMonitor::SaveRegionState");
	}

}

// Function used by the CSimBox to update the number of bead types in
// the CMonitor after a ccChangeBeadType command has been executed. 
// Other commands may also call this function. We assume that only one 
// new bead type has been created per call to this function, and the new bead
// type is assigned the next consecutive integer in the bead type count, so
// its type is one less than the total number of bead types. We get its name
// from the bead names map, but depending on which command invoked this
// function its name may be "Unnamed".
//
// This routine uses the CSimState and CInitialState to update its 
// local counters, and therefore they MUST be updated prior to calling 
// this routine. Note that the new bead type's display id is NOT set here.
// Any command that results in this function being called must update the
// affected beads' display ids itself. This is to avoid having to search for 
// the modified beads here, and change their display id when the calling command
// already stores the beads in its own container.

void CMonitor::UpdateBeadTypes()
{
	// Update the local bead type counter, and use it to resize
	// all other dependent local containers.

	m_BeadTypeSize = m_pSimState->GetBeadTypeTotal();

    const zString beadName = m_pSimState->cGetAnalysisState().GetBeadNameFromType(m_BeadTypeSize-1);

	// Update the container holding the number of beads of each type

	m_vBeadTypeTotal.clear();
	m_vBeadTypeTotal.resize(m_BeadTypeSize);

	copy( (m_pSimState->GetInitialState()).GetBeadTypeTotals().begin(), 
		  (m_pSimState->GetInitialState()).GetBeadTypeTotals().end(), 
				m_vBeadTypeTotal.begin());

	// Update the container of bead diffusion coefficients

	m_vBeadMSD.resize(m_BeadTypeSize);

	// Create a Grid observable to hold the new bead type's density field.
    // The observable's field id is set equal to the new bead's type,
    // which is 1 less than the total number of bead types.

	CGridObservable*   pDensity	= new CGridObservable("Bead Density", m_pSimState->GetDensityPeriod(), m_pSimState->GetSamplePeriod(),
													   m_GridXCellNo, m_GridYCellNo, m_GridZCellNo, 
													   m_GridXCellWidth, m_GridYCellWidth, m_GridZCellWidth, 
													   m_pSimState->GetBeadTotal(), beadName, m_BeadTypeSize-1);
	m_vGridObservables.push_back(pDensity);

	// Add an entry to the m_ExcludedBeadTypes container for any
	// new types created. Note that they are displayed by default
	// regardless of what their original exclusion status was.

	m_ExcludedBeadTypes.push_back(0);

	// Update the number of bead-bead interaction pairs for the stress
	// tensor calculation

	m_StressBeadPairTotal = m_BeadTypeSize*m_BeadTypeSize;

	for(short int i=0; i<m_StressSliceTotal; i++)
	{
		m_vvSliceStress11.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress12.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress13.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress21.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress22.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress23.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress31.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress32.at(i).resize(m_StressBeadPairTotal);
		m_vvSliceStress33.at(i).resize(m_StressBeadPairTotal);
	}

}

// Function used by the ISimBox to update the number of bond types in
// the CMonitor after a ctPolymerisePolymerTripleInTarget command has 
// been executed.  Other commands may also call this function. It could
// be modified to work for removing bond types as well by adding an else
// branch to the if statement and deleting the observables in the
// m_vBondObservables container, etc.
//
// This routine uses the CSimState and CInitialState to update its 
// local counters, and therefore they MUST be updated prior to calling 
// this routine.

void CMonitor::UpdateBondTypes()
{
	// Update the local bond type counter, and use it to resize
	// all other dependent local containers.

	const long oldBondTypeSize = m_BondTypeSize;

	m_BondTypeSize = m_pSimState->GetBondTypeTotal();

	// Add an observable for each newly-created bond type: we assume the type 
	// of each bobnd is just one greater than the previous highest bond type.

	if(m_BondTypeSize > oldBondTypeSize)
	{
		for(long ib=0; ib<m_BondTypeSize-oldBondTypeSize; ib++)
		{
			m_vBondObservables.push_back(new CScalarObservable("Unnamed bond length", m_pSimState->GetAnalysisPeriod(), m_pSimState->GetSamplePeriod()));
		}

		// Update the container holding the number of bonds of each type

		m_vBondTypeTotal.clear();
		m_vBondTypeTotal.resize(m_BondTypeSize);

		copy( (m_pSimState->GetInitialState()).GetBondTypeTotals().begin(), 
			  (m_pSimState->GetInitialState()).GetBondTypeTotals().end(), 
					m_vBondTypeTotal.begin());

		m_vBondLength.clear();
		m_vBondLength.resize(m_BondTypeSize);

		// Update the number of bond types for the stress tensor calculation

		for(short int i=0; i<m_StressSliceTotal; i++)
		{
			m_vvSliceStressBond11.at(i).resize(m_BondTypeSize);
			m_vvSliceStressBond12.at(i).resize(m_BondTypeSize);
			m_vvSliceStressBond13.at(i).resize(m_BondTypeSize);
			m_vvSliceStressBond21.at(i).resize(m_BondTypeSize);
			m_vvSliceStressBond22.at(i).resize(m_BondTypeSize);
			m_vvSliceStressBond23.at(i).resize(m_BondTypeSize);
			m_vvSliceStressBond31.at(i).resize(m_BondTypeSize);
			m_vvSliceStressBond32.at(i).resize(m_BondTypeSize);
			m_vvSliceStressBond33.at(i).resize(m_BondTypeSize);
		}
	}
}

// Private helper function to calculate bead density fields across specified regions
// of the SimBox. These may be 1-dimensional profiles or 2d and 3d fields.

void CMonitor::CalculateDensityFields()
{
	DensityFieldIterator iterDens=m_DensityFields.begin();

	while(iterDens != m_DensityFields.end())
	{
		if((*iterDens)->IsTimeToSample(GetCurrentTime()))
  		{
			(*iterDens)->Sample();
		}

		if((*iterDens)->IsTimeToNormalize())
		{
			(*iterDens)->Normalize();
			(*iterDens)->Serialize();
			iterDens = m_DensityFields.erase(iterDens);
		}
        else
        {
			iterDens++;
        }
	}

}

// Handler function to toggle the display of a polymer type on and off in response
// to an internal command. This executes the same actions as the similarly-named
// command (mcTogglePolymerDisplay) but does not require an execution time, and
// does not issue log messages. it is distasteful to have to replicate the code
// here, but there is no easy way to gain access to the implementation of the
// command.

void CMonitor::InternalTogglePolymerDisplay(const zString polymerName) const
{
	const long polymerType = m_pSimState->GetAnalysisState().GetPolymerTypeFromName(polymerName);
	
	if(polymerType >= 0 && polymerType < m_pSimState->GetPolymerTypeTotal())
	{
       PolymerVector vPolymers = m_pSimState->GetPolymers();

       // Find the current visibility status of the polymer type and store it.
       // We have to iterate over all polymers that precede the desired type,
       // as we don't know where they start in the polymer container.

       bool bNotFound = true;
       bool bNewDisplay = 0;

       PolymerVectorIterator iterPoly1=vPolymers.begin(); 
       
        while(bNotFound && iterPoly1!=vPolymers.end())
        {
            if((*iterPoly1)->GetType() == polymerType)
            {
	            bNotFound = false;
                bNewDisplay = !(*iterPoly1)->GetVisible();
            }
            iterPoly1++;
        }

        // Only modify the visibility status if the polymer type was found,
        // otherwise leave it unchanged. We indicate if the status was changed
        // in the log message. This should never fail, but if polymers have their
        // type changed dynamically in the future, it is possible for polymer types
        // to be out of order and perhaps some members of a type might get missed.

        if(!bNotFound)
        {
            for(PolymerVectorIterator iterPoly=vPolymers.begin(); iterPoly!=vPolymers.end(); iterPoly++)
            {
                if((*iterPoly)->GetType() == polymerType)
                {
		            (*iterPoly)->SetVisible(bNewDisplay);
                }
            }
        }
    }


}

void CMonitor::Saveud() const 
{
    new CLogud(GetCurrentTime(), "b814faa7-de6a-4d49-9296-4d3661916716");
}

// Function to return a pointer to the last saved density state. This can be used
// by other analysis features to calculate observables that are derived from the
// bead density grid data. If the analysis is off, or no state has yet been saved,
// it returns a null pointer which should be checked for by the calling routine.

const CDensityState* CMonitor::GetLastDensityState() const
{
    const CDensityState* pState = 0;

    if(IsDensityGridAnalysisOn() && !m_vDensityStates.empty())
    {
        pState = m_vDensityStates.back();
    }

    return pState;
}

// Function to return a copy of the container of density states held
// in the Monitor. These can be used to access grid data on the bead
// type distributions. Note that if the user has not issued the command
// to turn on grid observable analysis, the vector will be empty. This
// should be checked by the calling routine.

const DensityStateSequence& CMonitor::GetDensityStates() const
{
    return m_vDensityStates;
}

// Helper function showing if a bead type is excluded from current state snapshots
// or not.

bool CMonitor::IsBeadTypeVisible(long type) const
{
    if(type >= 0 && type < static_cast<long>(m_ExcludedBeadTypes.size()) && m_ExcludedBeadTypes.at(type) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Command handler function to change the analysis period for monitoring the simulation.
// We return a boolean flag showing if the change is accepted.
// This will invalidate any observables that are averaged on both sides of the change, 
// and for this reason is not implemented yet.

bool CMonitor::InternalSetAnalysisPeriod(long period)
{
    // Not implemented yet

    return false;
}

// Command handler function to change the period for saving the grid density data.
// We return a boolean flag showing if the change is accepted.

bool CMonitor::InternalSetDensityPeriod(long period)
{
    if(period > 0)
    {
        m_pSimState->SetDensityPeriod(period);
        return true;
    }
    else
    {
        return false;
    }
}

// Command handler function to change the period for saving current state snapshots.
// We return a boolean flag showing if the change is accepted.

bool CMonitor::InternalSetDisplayPeriod(long period)
{
    if(period > 0)
    {
        m_pSimState->SetDisplayPeriod(period);
        return true;
    }
    else
    {
        return false;
    }
}

// Command handler function to change the period for saving restart states.
// We return a boolean flag showing if the change is accepted.

bool CMonitor::InternalSetRestartPeriod(long period)
{
    if(period > 0)
    {
        m_pSimState->SetRestartPeriod(period);
        return true;
    }
    else
    {
        return false;
    }
}

// Command handler function to change the sampling period for monitoring the simulation.
// We return a boolean flag showing if the change is accepted.
// This will invalidate any observables that are averaged on both sides of the change, 
// and for this reason is not implemented yet. If this function is implemented, there
// must be a concommitant check on the analysis period to ensure they are still 
// commensurate. We also have to ensure that both the Monitor's internal period
// and the simulation state's period are consistent.

bool CMonitor::InternalSetSamplePeriod(long period)
{
    // Not implemented yet
    return false;
}

// Command handler function to write a current state snapshot to file. The snapshot
// is not saved for later analysis. This is in contrast to the usual function that 
// periodically writes current states to file. It requires an instance of a class 
// derived from CCurrentStateFormat that determines the type of snapshot, and all 
// other parameters are internal to the Monitor. We return a flag to the command 
// implementation class so that it can check if the write succeeded or not.

// Note that the format object is destroyed by its containing current state object.

bool CMonitor::InternalSaveCurrentState(CCurrentStateFormat* const pFormat)
{
	CCurrentState cState(GetCurrentTime(), GetRunId(), GetISimBox(), pFormat, 
						   m_bRestrictCurrentStateCoords,
						   m_MinXFraction, m_MinYFraction, m_MinZFraction,
						   m_MaxXFraction, m_MaxYFraction, m_MaxZFraction);

	return cState.Serialize();
}

// Command handler function to toggle the writing of the total KE and PE of the system
// to the history state file. The command cannot fail so there is no return value.

void CMonitor::InternalToggleEnergyOutput(bool bNormalizePerBead)
{
	m_bEnergyOutput     = !m_bEnergyOutput;
	m_bNormalizePerBead = bNormalizePerBead;

	m_pSimState->SetEnergyOutput(m_bEnergyOutput);
}


