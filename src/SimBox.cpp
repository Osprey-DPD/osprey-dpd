/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// SimBox.cpp: implementation of the CSimBox class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "SimMathFlags.h"
#include "SimAlgorithmFlags.h"
#include "SimFunctionalFlags.h"
#include "SimMiscellaneousFlags.h"
#include "SimBox.h"
#include "ISimBox.h"
#include "IModifySimStateBeadTypes.h"
#include "IModifySimStateBondTypes.h"
#include "IModifySimStateIntegration.h"
#include "IModifySimStatePolymers.h"

// Force target base class needed for some functions

#include "ForceTarget.h"

#if EnableMiscClasses == SimMiscEnabled
  #include "BeadChargeWrapper.h"
  #include "ConstantForce.h"
  #include "LinearForce.h"
  #include "RadialForce.h"
  #include "SineForce.h"
  #include "SpringForce.h"

#endif

#include "Bead.h"
#include "Bond.h"
#include "BondPair.h"
#include "Polymer.h"
#include "CNTCell.h"
#include "CNTCellSlice.h"
#include "Cell.h"
#include "Row.h"
#include "Slice.h"
#include "StressGridCell.h"

// SimBox command headers: note that excluded commands are below

#include "ccExtendTotalTime.h"
#include "ccGravityOff.h"
#include "ccGravityOn.h"
#include "ccRenormaliseMomenta.h"
#include "ccSetBondStiffness.h"
#include "ccSetBondStrength.h"
#include "ccSetBondStrengthbyType.h"
#include "ccSetCommandTimer.h"
#include "ccSetDPDBeadConsInt.h"
#include "ccSetDPDBeadConsIntByType.h"
#include "ccSetDPDBeadDissInt.h"
#include "ccSetDPDBeadDissIntByType.h"
#include "ccSetTimeStepSize.h"
#include "ccStop.h"
#include "ccStopNoSave.h"
#include "ccToggleBeadStressContribution.h"
#include "ccToggleBondStressContribution.h"
#include "ccToggleBondPairStressContribution.h"
#include "ccWallOn.h"
#include "ccWallOff.h"
#include "ccZeroTotalMom.h"

#if EnableConstraintCommand == SimCommandEnabled
    #include "ccFreezeBeadsInSlice.h"
    #include "ccFreezeBeadsInSphericalShell.h"
    #include "ccRemoveCNTCellsInsideRadius.h"
    #include "ccRemoveCNTCellsOutsideRadius.h"
#endif

// Command group class definition

#include "tguCommandGroup.h"


// Command target class definition needed for return type of various functions

#include "CommandTargetNode.h"


#if EnableTargetCommand == SimCommandEnabled
// Command targets and associated classes

  #include "CommandTarget.h"
  #include "CommandTargetComposite.h"
  #include "CommandTargetPolymer.h"
  #include "Nanoparticle.h"

  #include "taConstantForce.h"
  #include "taLinearForce.h"
  #include "taRadialForce.h"
  #include "taSineForce.h"
  #include "taSpringForce.h"

  #include "ctAddTargetToComposite.h"
  #include "ctAssignExistingBeadType.h"
  #include "ctChangeBondPairType.h"
  #include "ctChangeBondType.h"
  #include "ctCompositeCommandTarget.h"
  #include "ctConstantForceOnTarget.h"
  #include "ctCountActiveCommandTargets.h"
  #include "ctCountCommandTargets.h"
  #include "ctCountBeadsInTarget.h"
  #include "ctCountBeadTypeInTarget.h"
  #include "ctCountTargetsInComposite.h"
  #include "ctFreezeBeadsInTarget.h"
  #include "ctLinearForceOnTarget.h"
  #include "ctPolymerisePolymerTripleInTarget.h"
  #include "ctRadialForceOnTarget.h"
  #include "ctRemoveActiveCommandTarget.h"
  #include "ctRemoveCommandTargetActivity.h"
  #include "ctRemoveTargetFromComposite.h"
  #include "ctRestoreBeadType.h"
  #include "ctRestoreOriginalBeadType.h"
  #include "ctSetBondStiffnessByPositionInTarget.h"
  #include "ctSetBondStiffnessInTarget.h"
  #include "ctSetBondStrengthByPositionInTarget.h"
  #include "ctSetBondStrengthInTarget.h"
  #include "ctSineForceOnTarget.h"
  #include "ctUnFreezeBeadsInTarget.h"
#endif

#if EnableShadowSimBox == SimACNEnabled
// Active elements

  #include "aeActiveSimBox.h"
  #include "aeActiveCellNetwork.h"
  #include "aeActiveBond.h"
  #include "aeActivePolymer.h"
  #include "CommandTargetACNFreeBonds.h"
  #include "CommandTargetACNFreePolymers.h"
#endif


#if EnableParallelSimBox == SimMPSEnabled
// Parallel SimBox
    #include "mpsLinearXSimBox.h"
    #include "mpsLinearYSimBox.h"
    #include "mpsLinearZSimBox.h"
    #include "mpsPlanarXSimBox.h"
    #include "mpsPlanarYSimBox.h"
    #include "mpsPlanarZSimBox.h"
    #include "mpsUnitSimBox.h"
    #include "mpsCubicSimBox.h"
#endif

#if EnableMiscClasses == SimMiscEnabled
// Miscellaneous classes and commands

  #include "ccAddBeadsInCell.h"
  #include "ccAddBeadsInRow.h"
  #include "ccAddBeadsInSlice.h"
  #include "ccAddPolymersInCell.h"
  #include "ccAddPolymersInRow.h"
  #include "ccAddPolymersInSlice.h"
  #include "ccChargeBeadType.h"
  #include "ccConstantForce.h"
  #include "ccLinearForce.h"
  #include "ccMovePolymersToSlice.h"
  #include "ccRadialForce.h"
  #include "ccSelectBeadsInCell.h"
  #include "ccSelectBeadsInRow.h"
  #include "ccSelectBeadsInSlice.h"
  #include "ccSelectBeadsInCylinder.h"
  #include "ccSelectPolymersInCell.h"
  #include "ccSelectPolymersInRow.h"
  #include "ccSelectPolymersInSlice.h"
  #include "ccSineForce.h"
  #include "ccSpringForce.h"
  #include "ccUnchargeBeadType.h"
#endif

// SimBox message headers

#include "LogActiveBondAdded.h"
#include "LogActiveBondRemoved.h"
#include "LogAssignBeadType.h"
#include "LogAssignExistingBeadType.h"
#include "LogBondStrengthChanged.h"
#include "LogBondStiffnessChanged.h"
#include "LogChargeBeadType.h"
#include "LogCommandFailed.h"			// Generic command failed message
#include "LogCountBeadsInTarget.h"
#include "LogctAddTargetToComposite.h"
#include "LogctChangeBondPairType.h"
#include "LogctChangeBondType.h"
#include "LogctCompositeCreated.h"
#include "LogctConstantForceOnTarget.h"
#include "LogctCountCommandTargets.h"
#include "LogctCountActiveCommandTargets.h"
#include "LogctCountTargetsInComposite.h"
#include "LogctFreezeBeadsInTarget.h"
#include "LogctNonExistentTarget.h"
#include "LogctPolymerisePolymerTripleInTarget.h"
#include "LogctRadialForceOnTarget.h"
#include "LogctRemoveActiveCommandTarget.h"
#include "LogctRemoveCommandTargetActivity.h"
#include "LogctRemoveTargetFromComposite.h"
#include "LogctSetBondStiffness.h"
#include "LogctSetBondStrength.h"
#include "LogctSineForceOnTarget.h"
#include "LogctUnFreezeBeadsInTarget.h"
#include "LogDPDBeadConsIntChanged.h"
#include "LogDPDBeadDissIntChanged.h"
#include "LogExtendTotalTime.h"
#include "LogFreezeBeadsInSlice.h"
#include "LogFreezeBeadsInSphericalShell.h"
#include "LogMCAcceptanceRate.h"
#include "LogRestoreBeadType.h"
#include "LogRestoreOriginalBeadType.h"
#include "LogSetCommandTimer.h"
#include "LogSetTimeStepSize.h"
#include "LogSimErrorTrace.h"
#include "LogStressContribution.h"

// Command and log message classes needed when some commands are directly 
// implemented in the SimBox. Note that ccStop and ccStopNoSave are already
// included above as we create instances of them explicitly in this class.

#if EnableDirectImplCommand == SimCommandEnabled

#include "ccToggleDPDBeadConservativeForces.h"
#include "ccToggleDPDBeadForces.h"
#include "ccToggleDPDBeadThermostat.h"

#include "LogStopSimulation.h"
#include "LogStopNoSaveSimulation.h"
#include "LogToggleDPDBeadConservativeForces.h"
#include "LogToggleDPDBeadForces.h"
#include "LogToggleDPDBeadThermostat.h"
#endif

#include <functional>

	using std::cout;

// Could do compile-time tricks to change alias, if compilers below C++11 need support?
using std::mem_fn; // Covert to std::mem_fn, as mem_fun deprecated in c++17


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Static member variable holding a pointer to the single instance of CSimBox.
// Note that it does not get assigned a value until the user creates the instance.

CSimBox* CSimBox::m_pInstance = 0;

// Public member function to create a single instance of the CSimBox class.

CSimBox* CSimBox::Instance(CSimState& simState, const ISimBox* const pISimBox)
{
	if( !m_pInstance)
		m_pInstance = new CSimBox(simState, pISimBox);

	return m_pInstance;
}

// Protected constructor for the CSimBox object. Because a CSimState object 
// is required to initialise the SimBox, and because it may be altered during 
// the simulation as commands are processed, we pass it by non-const reference. 
//
// Interface classes that wrap the CSimState are used by CSimBox commands
// to change the simulation state, e.g., the number of bead types. The 
// functions needed for the CSimBox to execute each type of modification 
// are grouped into distinct interface classes to prevent unintended access
// to commands that have potentially dangerous access to the simulation state.
// All such interface classes derive from IModifySimState that holds a 
// reference to the CSimState. We use const pointers to hold the interface
// classes instead of references because including the header files for 
// the interface classes in the SimBox.h generates a mass of compilation
// dependencies each time a new function is added to one of the interface
// classes.
//
//		commands destined for the CSimBox are declared in ISimCmd but implemented here;
//		commands destined for the CMonitor are passed via the ISimCmd to 
//		the ISimBox and then routed to the CMonitor. For this reason we store 
//		the pointer to the ISimBox in the ISimCmd base class.

// All constraints are initially turned off and commands are used to turn them on.
// The command sequence is stored locally so that we can remove commands from the
// set as they are executed without altering the original sequence. We set 
// a pointer to the first command so that we can loop over all commands 
// that might be executed in one timestep. However, the renormalisation of the
// momenta during a simulation is on by default and a command must be used to
// turn it off.

CSimBox::CSimBox(CSimState& simState, const ISimBox* const pISimBox) : ISimState(simState, pISimBox),
									m_CNTXCellNo(simState.GetCNTXCellNo()),
									m_CNTYCellNo(simState.GetCNTYCellNo()),
									m_CNTZCellNo(simState.GetCNTZCellNo()),
									m_CNTXYCellTotal(m_CNTXCellNo*m_CNTYCellNo),
									m_BeadTotal(simState.GetBeadTotal()),
									m_WallBeadTotal(simState.GetWallBeadTotal()),
									m_ChargedBeadTotal(0),
									m_PolymerTotal(simState.GetPolymerTotal()),
									m_WallPolymerTotal(simState.GetWallPolymerTotal()),
									m_CNTXCellWidth(simState.GetCNTXCellWidth()),
									m_CNTYCellWidth(simState.GetCNTYCellWidth()),
									m_CNTZCellWidth(simState.GetCNTZCellWidth()),
									m_GravityXForce(simState.GetGravityXForce()),
									m_GravityYForce(simState.GetGravityYForce()),
									m_GravityZForce(simState.GetGravityZForce()),
									m_StressCellMultiplier(2),
									m_StressCellXNo(m_StressCellMultiplier*m_CNTXCellNo),
									m_StressCellYNo(m_StressCellMultiplier*m_CNTYCellNo),
									m_StressCellZNo(m_StressCellMultiplier*m_CNTZCellNo),
									m_StressCellTotal(m_StressCellXNo*m_StressCellYNo*m_StressCellZNo),
									m_PointTotal(100), 
									m_InversePointTotal(1.0/static_cast<double>(m_PointTotal)),
									m_InversePointTotalMinusOne(1.0/static_cast<double>(m_PointTotal-1)),
									m_StressCellXWidth(m_CNTXCellWidth/static_cast<double>(m_StressCellMultiplier)),
									m_StressCellYWidth(m_CNTYCellWidth/static_cast<double>(m_StressCellMultiplier)),
									m_StressCellZWidth(m_CNTZCellWidth/static_cast<double>(m_StressCellMultiplier)),
									m_pOldCell(0),
	                                m_pNewCell(0),
	                                m_StressWeight(0.0)

{
#if EnableShadowSimBox == SimACNEnabled
	// Create the shadow SimBox that tracks active bonds and polymers.
	// We create it by default so that it can be the target of commands
	// without the user having to remember to create it. It can still be
	// destroyed and recreated by command.

	m_pShadow = aeActiveSimBox::Instance(GetISimBox());

	SetActiveBondsOn(true);
#endif

	// Clear the command group, force and target containers, then copy all 
    // command targets from the initial state (if any) in case this is a 
    // restarted run: we cannot do this earlier as the SimBox does not exist 
    // at the time the initial state reads the restart state file. 
    // Active command targets, which consist of the outermost decorator for each 
    // wrapped command target, are then copied into local storage. We don't copy the 
    // force targets as they are deprecated, and are not contained in restart states.
    // Nor do we copy the command groups as commands are not yet propagated via
    // restart states.

    m_Nanoparticles.clear();
    m_CommandGroups.clear();
    m_ActiveCommandGroups.clear();
	m_CommandTargets.clear();
	m_ActiveCommandTargets.clear();
	m_ForceTargets.clear();
	m_ActiveForceTargets.clear();

    // Note that it is crucial that the return value of the function be copied into 
    // a local variable before any iterators to the container are created, as they
    // don't operate correctly on the function's return value itself.

    m_CommandTargets = ISimState::GetCommandTargets();

	for(CommandTargetIterator iterTarget=m_CommandTargets.begin(); iterTarget!=m_CommandTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetOutermostDecorator() != *iterTarget)
        {
            m_ActiveCommandTargets.push_back((*iterTarget)->GetOutermostDecorator());
        }
	}

	// Define useful combinations of parameters

	m_SimBoxXLength = static_cast<double>(m_CNTXCellNo)*m_CNTXCellWidth;
	m_SimBoxYLength = static_cast<double>(m_CNTYCellNo)*m_CNTYCellWidth;

#if SimDimension == 2

	m_SimBoxZLength = 0.0;
	m_SimBoxVolume	= m_SimBoxXLength*m_SimBoxYLength;

	m_CNTCellTotal	= m_CNTXCellNo*m_CNTYCellNo;

#elif SimDimension == 3

	m_SimBoxZLength = static_cast<double>(m_CNTZCellNo)*m_CNTZCellWidth;
	m_SimBoxVolume	= m_SimBoxXLength*m_SimBoxYLength*m_SimBoxZLength;

	m_CNTCellTotal	= m_CNTXCellNo*m_CNTYCellNo*m_CNTZCellNo;

#endif

	m_HalfSimBoxXLength = 0.5*m_SimBoxXLength;
	m_HalfSimBoxYLength = 0.5*m_SimBoxYLength;
	m_HalfSimBoxZLength = 0.5*m_SimBoxZLength;

	// Allocate memory to the local vectors of polymers, wall polymers etc.
	// We don't allocate memory for charged beads because we expect only a few.

	 m_vAllPolymers.reserve(simState.GetPolymerTotal());
	m_vWallPolymers.reserve(simState.GetWallPolymerTotal());
	   m_vWallBeads.reserve(simState.GetWallBeadTotal());

	// Copy all polymers into a local vector for rapid access, and store local 
	// vectors of beads affected by the (optional) body force and the wall beads.
	// Note that the beads affected by gravity already occur in the polymers  
	// and no attempt should be made to destroy them separately.

	m_vAllPolymers	= simState.GetPolymers();
	m_vWallPolymers	= simState.GetWallPolymers();
	m_vWallBeads	= simState.GetWallBeads();
	m_vGravityBeads	= simState.GetGravityBeads();
    
    // BIOENG455 2025 Collect all polymers of type 1 (Helix) and store them locally to
    // allow the helix-forming forces to be applied to them.
    
    m_vHelixPolymers.clear();
    
    for(PolymerVectorIterator iterPoly=m_vAllPolymers.begin(); iterPoly!=m_vAllPolymers.end(); iterPoly++)
    {
        if( (*iterPoly)->GetType() == 1 )
        {
            m_vHelixPolymers.push_back(*iterPoly);
        }
    }

	// Construct the CNT cell network and partition the beads amongst the cells.
	// We distribute the wall beads here if the SimState shows that the wall has
	// been turned on. This is set by the CInputData object from the control data
	// file. The wall can also be turned off and on using commands.

	m_vCNTCells.reserve(m_CNTCellTotal);

	MakeCNTCells();

    // If any beads or wall beads cannot be assigned to a valid CNT cell.
    // we log an error message and terminate the simulation. Because we cannot yet issue a StopNoSave command in the parallel code,
	// we just reset the simulation time to 1 step so it stops soon.

    if(!AssignBeadstoCells(simState.GetInitialStateBeads()))
    {
#if EnableParallelSimBox == SimMPSEnabled
        std::cout << "Proc " << GetRank() << " unable to assign beads to CNT Cells - aborting run" << zEndl;
		m_SimTime = 1;
#else
        new CLogSimErrorTrace(m_SimTime, "Unable to assign beads to cells: stopping run");
	    AddCommandToFront(new ccStopNoSave(m_SimTime+1));
#endif
    }

	if(IsWallOn() && !AssignWallBeadstoCells())
    {
#if EnableParallelSimBox == SimMPSEnabled
        std::cout << "Proc " << GetRank() << " unable to assign wall beads to CNT Cells - aborting run" << zEndl;
		m_SimTime = 1;
#else
        new CLogSimErrorTrace(m_SimTime, "Unable to assign wall beads to cells: stopping run");
		AddCommandToFront(new ccStopNoSave(m_SimTime+1));
#endif
    }

#if EnableStressTensorSphere == SimMiscEnabled
    // Create the stress grid if the feature is compiled in.
	
    MakeStressCells();
	
	std::cout << "SimBox has created stress grid with multiplier:  " << m_StressCellMultiplier << zEndl;
	std::cout << m_StressCellXNo    << " " << m_StressCellYNo    << " " << m_StressCellZNo    << " " << m_StressCellTotal << zEndl;
	std::cout << m_StressCellXWidth << " " << m_StressCellYWidth << " " << m_StressCellZWidth << zEndl;
#endif
	
	
		
	for(long i=0; i<3; i++)
	{
	    m_rprime[i]          = 0.0;
		m_dr[i]              = 0.0;
		m_LocalStress[3*i]   = 0.0;
		m_LocalStress[3*i+1] = 0.0;
		m_LocalStress[3*i+2] = 0.0;
		m_StressOrigin[i]    = 0.0;
	}
	
			
#if EnableParallelSimBox == SimMPSEnabled
    // Create the parallel SimBox that handles all inter-processor communication.
    // We can only do this after the CNT cells have been created because we copy
    // the CNT cells into mpsSimBox so that it can partition them into various
    // mpsBorder regions to control the inter-processor communication.
    // We create an object of a given class depending on the geometry of the processors
    // in the simulation Space. We distinguish 3 types of geometry depending on
    // how many dimensions are spanned by only a single processor:
    //
    // Linear Space - All processors are arranged in the same dimension
    // Planar Space - All processors are arranged in a plane
    // Cubic  Space - All dimensions have more than one processor assigned to them
    //
    // Note that the case of a single processor is a bit pathological. It does not
    // need to use messaging, but using the parallel code on a single processor
    // results in slower execution than the serial code. We separate this into a
    // special case using mpsUnitSimBox.
   		
		m_pParallel = 0;		

        const long px = GetProcessorsXNo();
        const long py = GetProcessorsYNo();
        const long pz = GetProcessorsZNo();

        if(px == 1 && py == 1 && pz == 1)  
        {
            // Single-processor case
		    m_pParallel = new mpsUnitSimBox(m_CNTXCellNo,    m_CNTYCellNo,    m_CNTZCellNo,
                                            m_CNTXCellWidth, m_CNTYCellWidth, m_CNTZCellWidth,  
                                            m_vCNTCells, m_vAllPolymers);
        }
        else if(px == 1 && py == 1 && pz > 1)  
        {
            // Linear in Z
		    m_pParallel = new mpsLinearZSimBox(px, py, pz, 
                                    m_CNTXCellNo,    m_CNTYCellNo,    m_CNTZCellNo,
                                    m_CNTXCellWidth, m_CNTYCellWidth, m_CNTZCellWidth,  
                                    m_vCNTCells, m_vAllPolymers);
        }
        else if(px == 1 && py > 1 && pz == 1)
        {
            // Linear in Y
		    m_pParallel = new mpsLinearYSimBox(px, py, pz, 
                                    m_CNTXCellNo,    m_CNTYCellNo,    m_CNTZCellNo,
                                    m_CNTXCellWidth, m_CNTYCellWidth, m_CNTZCellWidth,  
                                    m_vCNTCells, m_vAllPolymers);
        }
        else if(px > 1 && py == 1 && pz == 1)
        {
            // Linear in X
		    m_pParallel = new mpsLinearXSimBox(px, py, pz, 
                                    m_CNTXCellNo,    m_CNTYCellNo,    m_CNTZCellNo,
                                    m_CNTXCellWidth, m_CNTYCellWidth, m_CNTZCellWidth,  
                                    m_vCNTCells, m_vAllPolymers);
        }
        else if(px > 1 && py > 1 && pz == 1)  
        {
            // Planar in Z
		    m_pParallel = new mpsPlanarZSimBox(px, py, pz, 
                                    m_CNTXCellNo,    m_CNTYCellNo,    m_CNTZCellNo,
                                    m_CNTXCellWidth, m_CNTYCellWidth, m_CNTZCellWidth,  
                                    m_vCNTCells, m_vAllPolymers);
        }
        else if(px > 1 && py == 1 && pz > 1)  
        {
            // Planar in Y
		    m_pParallel = new mpsPlanarYSimBox(px, py, pz, 
                                    m_CNTXCellNo,    m_CNTYCellNo,    m_CNTZCellNo,
                                    m_CNTXCellWidth, m_CNTYCellWidth, m_CNTZCellWidth,  
                                    m_vCNTCells, m_vAllPolymers);
        }
        else if(px == 1 && py > 1 && pz > 1)  
        {
            // Planar in X
		    m_pParallel = new mpsPlanarXSimBox(px, py, pz, 
                                    m_CNTXCellNo,    m_CNTYCellNo,    m_CNTZCellNo,
                                    m_CNTXCellWidth, m_CNTYCellWidth, m_CNTZCellWidth,  
                                    m_vCNTCells, m_vAllPolymers);
        }
        else // All other cases use the cubic SimBox
        {
		    m_pParallel = new mpsCubicSimBox(px, py, pz, 
                                    m_CNTXCellNo,    m_CNTYCellNo,    m_CNTZCellNo,
                                    m_CNTXCellWidth, m_CNTYCellWidth, m_CNTZCellWidth,  
                                    m_vCNTCells, m_vAllPolymers);
        }
#endif
}
 
CSimBox::~CSimBox()
{
	// Set the single CSimBox pointer to 0 to ensure that multiple simulations
	// use different objects.

	m_pInstance = 0;

#if EnableShadowSimBox == SimACNEnabled
	// Delete the shadow SimBox if it was created

	if(m_pShadow)
	{
		delete m_pShadow;
		m_pShadow = 0;
	}
#endif

#if EnableParallelSimBox == SimMPSEnabled
	// Delete the parallel SimBox if it was created

    std::cout << "Processor " << GetRank() << " being destroyed" << zEndl;
	
	if(m_pParallel)
	{
        std::cout << "Processor " << GetRank() << "  destroying its parallel simbox instance" << zEndl;
		delete m_pParallel;
		m_pParallel = 0;
	}
#endif

	// Delete all CNT cells created using new in the CSimBox::MakeCNTCells
	// function. As we store pointers to the cells and not the objects
	// themselves we only have to delete them once.

	// We don't delete the beads and bonds as they were created by the CInitialState
	// object and are destroyed by it.

	if(!m_vCNTCells.empty())
	{
		for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
		{
			delete (*iterCell);
		} 
		m_vCNTCells.clear();
	}

#if EnableStressTensorSphere == SimMiscEnabled
    // Delete the crvilinear stress cells if they were compiled in
	
	if(!m_vStressCells.empty())
	{
		for(StressCellIterator iterCell=m_vStressCells.begin(); iterCell!=m_vStressCells.end(); iterCell++)
		{
			delete (*iterCell);
		} 
		m_vStressCells.clear();
	}
#endif

#if EnableCommandGroups == SimCommandEnabled
	// Delete any command groups created: note that the active command groups container
    // only has copies of pointers in this container, so we must not delete the 
    // entries in that container as well.

	if(!m_CommandGroups.empty())
	{
		for(CommandGroupIterator iterGroup=m_CommandGroups.begin(); iterGroup!=m_CommandGroups.end(); iterGroup++)
		{
			delete *iterGroup;
		} 
		m_CommandGroups.clear();
	}

#endif


#if EnableTargetCommand == SimCommandEnabled
	// Delete any command targets created. We don't have to delete the
	// active command targets as they hold copies of pointers stored here;
	// and any decorator instances are deleted by the CCommandTargetNode's
	// destructor.

	if(!m_CommandTargets.empty())
	{
		for(CommandTargetIterator iterCmdTarget=m_CommandTargets.begin(); iterCmdTarget!=m_CommandTargets.end(); iterCmdTarget++)
		{
			delete *iterCmdTarget;
		} 
		m_CommandTargets.clear();
	}

    // Zero the command target counter so that successive runs use ids starting at 1

    CCommandTargetNode::ZeroCommandTargetTotal();

#endif

#if EnablMiscClasses == SimMiscEnabled
	// Delete any force targets created. We don't have to delete the
	// active force targets as they contain copies of pointers stored here.

	if(!m_ForceTargets.empty())
	{
		for(ForceTargetIterator iterForce=m_ForceTargets.begin(); iterForce!=m_ForceTargets.end(); iterForce++)
		{
			delete (*iterForce);
		} 
		m_ForceTargets.clear();
	}

#endif

#if EnablMiscClasses == SimMiscEnabled
	// Delete all the CBeadChargeWrapper objects if there are any. These wrap
	// the normal CAbstractBead objects but are not responsible for deleting them.
	// The wrapper objects are created in response to a ChargeBeadType command, so
	// it is appropriate that they are deleted here.

	if(!m_lAllChargedBeads.empty())
	{
		for(ChargedBeadListIterator iterBead=m_lAllChargedBeads.begin(); iterBead!=m_lAllChargedBeads.end(); iterBead++)
		{
			delete *iterBead;
		} 
		m_lAllChargedBeads.clear();
	}

#endif

	// Delete any CNanoparticles created in both the serial and parallel codes.
    
	if(!m_Nanoparticles.empty())
	{
		for(NanoparticleIterator iterNano=m_Nanoparticles.begin(); iterNano!=m_Nanoparticles.end(); iterNano++)
		{
			delete *iterNano;
		} 
		m_Nanoparticles.clear();
	}
}

// Function to assign the beads to the CNT cells according to their position
// coordinates. If any bead is outside the SimBox, and therefore has a cell index
// that is out of range, we return false to allow the simulation to be terminaed.

bool CSimBox::AssignBeadstoCells(const BeadVector& rvAllBeads)
{
    bool bSuccess = true;

	long index, ix, iy, iz;

	for(cBeadVectorIterator citerBead=rvAllBeads.begin(); citerBead!=rvAllBeads.end(); citerBead++)
	{
#if EnableParallelSimBox == SimMPSEnabled

//		ix = static_cast<long>(((*citerBead)->GetXPos() - GetSimBoxXOrigin())/m_CNTXCellWidth);
//		iy = static_cast<long>(((*citerBead)->GetYPos() - GetSimBoxYOrigin())/m_CNTYCellWidth);
		ix = static_cast<long>((*citerBead)->GetXPos()/m_CNTXCellWidth);
		iy = static_cast<long>((*citerBead)->GetYPos()/m_CNTYCellWidth);

#if SimDimension == 2
		iz = 0;
#elif SimDimension == 3
//		iz = static_cast<long>(((*citerBead)->GetZPos() - GetSimBoxZOrigin())/m_CNTZCellWidth);		
		iz = static_cast<long>((*citerBead)->GetZPos()/m_CNTZCellWidth);		
#endif

#elif EnableParallelSimBox == SimMPSDisabled
		ix = static_cast<long>((*citerBead)->GetXPos()/m_CNTXCellWidth);
		iy = static_cast<long>((*citerBead)->GetYPos()/m_CNTYCellWidth);

#if SimDimension == 2
		iz = 0;
#elif SimDimension == 3
		iz = static_cast<long>((*citerBead)->GetZPos()/m_CNTZCellWidth);		
#endif

#else
        bSuccess = false;
#endif
		index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;
        if(index < m_CNTCellTotal)
        {
		    m_vCNTCells[index]->AddBeadtoCell(*citerBead);
        }
        else
        {
            bSuccess = false;
        }
	} 

    return bSuccess;
}

// Function to construct the CNT cells that handle all bead movement,
// nearest neighbour calculations and the periodic boundary conditions.

void CSimBox::MakeCNTCells()
{
	long index, coords[3];
	bool flag;

	CCNTCell* pCell;

#if SimDimension == 2
	long k = 0;
#elif SimDimension == 3
	for( long k=0; k<m_CNTZCellNo; k++ )
	{
#endif
		for( long j=0; j<m_CNTYCellNo; j++ )
		{
			for( long i=0; i<m_CNTXCellNo; i++ )
			{
				index = m_CNTXCellNo*(m_CNTYCellNo*k+j) + i;
				coords[0] = i;
				coords[1] = j;
				coords[2] = k;

#if SimDimension == 2
				if( i==0 || i==m_CNTXCellNo-1 ||
				    j==0 || j==m_CNTYCellNo-1 )
#elif SimDimension == 3
				if( i==0 || i==m_CNTXCellNo-1 ||
					j==0 || j==m_CNTYCellNo-1 ||
					k==0 || k==m_CNTZCellNo-1 )
#endif

				{
					flag = true;
				}
				else
				{
					flag = false;
				}

				pCell = new CCNTCell(index, coords, flag);
				m_vCNTCells.push_back(pCell);
			}
		}
#if SimDimension == 3

	}
#endif

	// Having created the cells we now link them into the nearest-neighbour
	// cell network. We map the coordinates of the cell (i,j,k) plus the change due
	// to a bead crossing cell boundaries (alpha,beta,gamma) into the 1D function 
	// given by the cell id's. There are 27 different directions (including the 
	// case of no change in the cell holding the bead) and each must be 
	// calculated for each cell. We use a helper function that maps the set
	// (gamma,beta,alpha) into [0,26] to allow storage as a 1d array of indices.
	//
	// We also store a separate array of cell pointers for those NN cells to be
	// used in calculating forces between beads. 

	for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		long nnIntIndex = 0;

#if SimDimension == 2
		long gamma = 0;
#elif SimDimension == 3
		for( long gamma=-1; gamma<2; gamma++ )
		{
#endif

			for( long  beta=-1;  beta<2;  beta++ )
			{
				for( long alpha=-1; alpha<2; alpha++ )
				{
					long newIndex = MapCelltoIndex( (*iterCell)->GetBLXIndex(), 
											   (*iterCell)->GetBLYIndex(), 
											   (*iterCell)->GetBLZIndex(), 
												alpha, beta, gamma );
					CCNTCell* pNNCell  = m_vCNTCells.at(newIndex);

					// map nn index to cell pointer
					long nnIndex  = MapCelltoNNIndex( alpha, beta, gamma );
					(*iterCell)->SetNNCellIndex( nnIndex, pNNCell );

					// map nn index to cell pointer for interacting cells.
					// These are those cells to the right and above the current 
					// cell in 2d and equivalently in 3d.

#if SimDimension == 2
					if( alpha==1 || (alpha==0 && beta==1) )
					{
						(*iterCell)->SetIntNNCellIndex( nnIntIndex, pNNCell );
						nnIntIndex++;
					}
#elif SimDimension == 3
					if( gamma==1 ||
					   (gamma==0 && (alpha==1 || (alpha==0 && beta==1))) )
					{
						(*iterCell)->SetIntNNCellIndex( nnIntIndex, pNNCell );
						nnIntIndex++;
					}
#endif

				}
			}
#if SimDimension == 3
		}
#endif
	} 

}

// Function to map the 3D cell coordinates (i,j,k) into a 1D index function when
// a bead moves from cell (i,j,k) to (i+alpha,j+beta,k+gamma). The index function
// returns an integer that corresponds to the label of the cell in the direction
// defined by the set (alpha,beta,gamma) taking the size of the simulation box,
// and the periodicity of the box into account. The order of storing the cells
// is X,Y,Z, that is, a plane of XY cells is filled first following by the next
// plane in the +Z direction.
// Also note that this function works for 2D and 3D simulations.

long CSimBox::MapCelltoIndex(long i, long j, long k, long alpha, long beta, long gamma) const
{
	long oldIndex, newIndex;

	oldIndex = i + j*m_CNTXCellNo + k*m_CNTXYCellTotal;

// X test
	newIndex = oldIndex + alpha;
	if( newIndex >= m_CNTXCellNo*(1 + j + k*m_CNTYCellNo) )
		newIndex -= m_CNTXCellNo;
	else if( newIndex < m_CNTXCellNo*(j + k*m_CNTYCellNo) )
		newIndex += m_CNTXCellNo;

// Y test
	newIndex += beta*m_CNTXCellNo;
	if( newIndex >= (1 + k)*m_CNTXYCellTotal )
		newIndex -= m_CNTXYCellTotal;
	else if( newIndex < k*m_CNTXYCellTotal )
		newIndex += m_CNTXYCellTotal;

// Z test
#if SimDimension == 3

	newIndex += gamma*m_CNTXYCellTotal;
	if( newIndex >= m_CNTCellTotal )
		newIndex -= m_CNTCellTotal;
	else if( newIndex < 0 )
		newIndex += m_CNTCellTotal;

#endif

	return newIndex;
}

// Function to map the set (alpha,beta,gamma) into a 1D function that gives
// the array index of an NN cell in 2D or 3D.

long CSimBox::MapCelltoNNIndex(long alpha, long beta, long gamma) const
{
	long newIndex;

#if SimDimension == 2

	newIndex = 4 + alpha + 3*beta;

#elif SimDimension == 3

	newIndex = 13 + alpha + 3*beta + 9*gamma;

#endif

	return newIndex;
}

// Private helper function to return a pointer to the CNT cell containing a given point.
// If the coordinates are within the SimBox we return a pointer to the cell, otherwise we return null.

CCNTCell* CSimBox::GetCNTCellFromCoords(double r[3]) const
{
    CCNTCell* pCell = 0;
	
	const long index = m_CNTXCellNo*(m_CNTYCellNo*static_cast<long>(r[2]/m_CNTZCellWidth) + static_cast<long>(r[1]/m_CNTYCellWidth)) + static_cast<long>(r[0]/m_CNTXCellWidth);

	if(index < m_CNTCellTotal)
	{
		pCell = m_vCNTCells[index];
	}
		
    return pCell;
}


// Function to evolve the state of all beads in a serial simulation forward 
// by one timestep. This consists of the following steps:
//
//  1   Calculate the new position of all beads from the current values of
//      force, velocity and position; zero current force  and store the
//      intermediate values of force and momentum (UpdatePos()). Note
//      that the current values of position, momentum and force are stored
//      in m_oldPos[], m_oldMom[] and m_oldForce[] in this routine.
//	
//  2	Calculate new forces on all beads due to bead-bead interactions,
//		bond forces and a body force if present (UpdateForce(), AddBondForces(),
//      AddBondPairForces(), Target forces, AddBodyForce(), AddChargedBeadForces()).
//  
//  3	Update velocity of all beads from old and new values of the force (UpdateMom())
//
//

void CSimBox::Evolve()
{
	CNTCellIterator iterCell;  // used in all three loops below

	for(iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdatePos();
	} 

	// Next calculate the forces between all pairs of beads in NN CNT cells
	// that can potentially interact. First, we tell the CMonitor to zero the
	// stress tensor arrays prior to generating new data. The function is
	// passed via the ISimBoxBase base class member holding a pointer to the
	// ISimBox that can then pass it to the CMonitor. Note that the stress
	// contributions from bonds and bondpairs are also zeroed within the
	// ZeroSliceStress() function but they are calculated and added serparately.

    // If the density-dependent DPDLG force is enabled and is being used
    // for the given input file, we first calculate the local bead density around
    // every bead in the SimBox and then call the UpdateLGForce() function to 
    // calculate the new force instead of the standard UpdateForce().

	ZeroSliceStress();

#if EnableDPDLG == ExperimentEnabled

    if(IsDPDLG())
    {
	    for(iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	    {
		    (*iterCell)->UpdateLGDensity();
	    } 
        
	    for(iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	    {
		    (*iterCell)->UpdateLGForce();
	    } 
    }
    else
    {
	    for(iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	    {
		    (*iterCell)->UpdateForce();
	    } 
    }

#elif EnableDPDLG == ExperimentDisabled

	for(iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdateForce();
	} 

#endif

	// Add in the forces between bonded beads and the stiff bond force. Note that
	// AddBondPairForces() must be called after AddBondForces() because it relies
	// on the bond lengths having already been calculated in CBond::AddForce().

	AddBondForces();
	AddBondPairForces();
    AddHelixForces();

#if EnableShadowSimBox == SimACNEnabled
	// Add in the forces due to active bonds and polymers if any exist.
	// We check the ISimState to see if active bonds are present in the simulation,
	// and if so, evolve their state via the shadow SimBox that holds all active
	// bond networks.

	if(IsActiveBondsOn())
		m_pShadow->Evolve();
#endif

	// If there are any charged bead types add in their forces to the beads

	if(IsBeadChargeOn())
		AddChargedBeadForces();

	// Execute any active command targets. These may be targetted by commands
	// that turn on an effect, such as applying a force, that is updated
	// at each timestep. The effect persists until another command turns it off.

	for(CommandTargetIterator iterComTarget=m_ActiveCommandTargets.begin(); iterComTarget!=m_ActiveCommandTargets.end();iterComTarget++)
	{
		(*iterComTarget)->Execute(m_SimTime);
	}

	// Add the forces dues to ccApplyForce commands to any active force targets,
	// after checking whether the target should be removed from the active list.
	// This does not destroy the target as it still exists in the m_ForceTargets
	// list. This will be destroyed in the SimBox's destructor.

	// Note the absence of an increment step in the for loop because the erase()
	// member function returns an iterator to the next element. However, because 
	// the Cray STL list<> class does not have an erase() member function we have 
	// to remove the element differently on that platform.


	for(ForceTargetIterator iterTarget=m_ActiveForceTargets.begin(); iterTarget!=m_ActiveForceTargets.end();)
	{
		if(!(*iterTarget)->AddForce(m_SimTime))
		{
			iterTarget = m_ActiveForceTargets.erase(iterTarget);
		}
		else
			iterTarget++;
	}

	// Only add gravity if a command to turn it on has been executed.

	if(IsGravityOn())
		AddBodyForce();

	// Finally update the velocities of the beads using the old and new values for
	// the forces. Note that even simulation types (such as Brownian Dynamics) that
    // do not use the bead velocities need to call this function as it calls the
    // SetMovable() function for each bead when it moves from one cell to another.

	for(iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdateMom();
	} 

	// Calculate the total KE and PE if required for output to the history state.
	// Zero the energy sums first and add the bond contributions using the monitor 
	// function ZeroTotalEnergy(), then iterate over all CNT cells adding 
	// the bead kinetic and potential energies. Note that the CNT cells call
	// the monitor function AddBeadEnergy() directly. We have to pass in two
	// dummy parameters for now

#if SimIdentifier != BD
	double kinetic = 0.0;
	double potential = 0.0;

	if(IsEnergyOutputOn())
	{
		ZeroTotalEnergy();

		for(cCNTCellIterator cIterCell=m_vCNTCells.begin(); cIterCell!=m_vCNTCells.end(); cIterCell++)
		{
			(*cIterCell)->UpdateTotalEnergy(&kinetic, &potential);
		} 
	}
#endif
}

// Function to evolve the state of all beads in a parallel simulation forward 
// by one timestep. To prevent any erros when a serial simulation runs, the function
// is compiled in only for the parallel executable.
//
// The sequence of updating the simulation's state for a parallel run differs from
// the serial case. We still update the bead coordinates in the order 

// Position - Force - Momenta
//
// but we divide the simulation Space into a Bulk space, in which bead-bead forces do not
// depend on beads owned by other processors, and Border spaces, where they do.
// Each processor carries out the following sequencec of steps:
//
// 1 Position update
// ******************
// 1.1 it updates bead positions in its Bulk space: no beads leave its Space or change
// ownership during this step
//
// 1.2 it updates bead positions in each of its Border regions (Faces, Edges and Corners);
// collecting beads that leave its own Space and packaging them into  
// containers to be sent to each neighbouring processor in the 26 directions
//
// 1.3 it sends messages to its 26 neighbour processors PN containing the complete 
// state information for all beads that are changing ownership to PN. We use a 
// Flyweight pattern here to avoid having to create/destroy CBead instances. Empty
// bead instances are stored ready to hold beads that move into this processor.
//
// 1.4 it receives messages from each PN containing bead states for beads that have 
// moved into its own Border regions, and fills up empty bead instances with the data.
// If it runs out of bead instances, it creates new ones and keeps track of them
// so that they can be deleted properly later.
//
// 2 Force update
// **************
// 2.1 it updates all non-bonded, short-ranged (ie, non-electrostatic) bead-bead forces
// between beads in its Bulk space. This includes forces between Bulk beads and 
// Border beads, but does not depend on beads owned by other processors. But this does
// require that all beads that should move between processors have already been moved
// before the forces are calculated. Hence, we need to synchronise the processors before
// this step.
//
// 2.2 it updates all bead-bead forces between beads in its Border regions and between
// these beads and neighbouring processors in its owned direction (U, T, R). To do this,
// it sends messages to PN to retrieve coordinates for beads in PNs border regions that it
// needs to calculate its forces. It stores the RNs needed for these forces and packages 
// them into messages that will later be sent to PN on request.
//
// 2.3 it receives messages from its non-owned neighbours PN containing the bead 
// coordinates and RNs needed to calculate bead-bead forces between its own Border region
// beads and those of its neighbours in its non-owned Border regions.
//
// 2.4 it calculates the forces between non-owned Border region beads and its Bulk using
// the data supplied by its non-owned neighbours.
//
// 2.5 it calculates all bonded forces between beads all of which are contained within its Bulk space 
//
// 2.6 it calculates all bonded forces between beads in its Bulk space and owned 
// Border regions and those in its owned neighbours PN Bulk spaces and Border regions.
//
// 2.7 it calculates all charged-bead forces between beads in its whose space;
// then it adds in the charged-bead forces between beads in its own Space and those of
// its owned neighbours PN.
//
// 2.8 it adds in any forces due to command targets that act on beads in its own Space.
//
// 2.9 it adds in the Gravity body force if it is turned on (deprecated).
//
// 3 Momenta update
// ****************
// 3.1 it updates the momenta of all beads in its Space as this calculation does not depend
// on any bead-bead interactions. But it does require that all force calculations be
// completed before starting the momenta calculations.
//


void CSimBox::EvolveP()
{
#if EnableParallelSimBox == SimMPSEnabled

    m_pParallel->UpdatePos();
    m_pParallel->UpdateForce();

	// Execute any active command targets. These may be targetted by commands
	// that turn on an effect, such as applying a force, that is updated
	// at each timestep. The effect persists until another command turns it off.

	for(CommandTargetIterator iterComTarget=m_ActiveCommandTargets.begin(); iterComTarget!=m_ActiveCommandTargets.end();iterComTarget++)
	{
		(*iterComTarget)->Execute(m_SimTime);
	}

        
	// Finally update the velocities of the beads using the old and new values for
	// the forces. Note that even simulation types (such as Brownian Dynamics) that
    // do not use the bead velocities need to call this function as it calls the
    // SetMovable() function for each bead when it moves from one cell to another.

    m_pParallel->UpdateMom();

#endif
}

// Function to execute the simulation of the fluid system occupying the simulation
// box. It handles both serial and parallel execution by calling distinct functions,
// and devolving the parallel work to the nested class mpsSimBox.
//
// Serial execution
// ****************
// The simulation state is incremented by calling Evolve() until the total
// number of timesteps, obtained from the CSimState object, has been 
// executed. Notice that we include the final timestep when deciding whether
// to sample the data. This allows us to print data from the last configuration
// in the simulation for use in subsequent restarts.
//
// Analysis is performed during the simulation at a frequency determined by 
// the period in the AnalysisState object, on data that is sampled at a rate
// set by the sample period. Some data is displayed as a time series by the
// CMonitor and other data is stored in CObservable-derived objects.
//
// We check that the bead lists for each CNT cell are correct by comparing each
// bead's coordinates with the coordinates of the bottom-left and top-right 
// corners of the cell. Because of the way the CNT update works if the step size
// is large a bead might cross a whole CNT cell in one time step and be added
// to the wrong cell.
//
// Events may be triggered by conditions in the SimBox so we check the sequence of
// events first. This preceeds the commands because an event may lead to commands
// being created which need to be executed after the event. Events persist for
// the whole simulation even if they become inactive so we do not delete them from
// the event sequence.
//
// Commands are used to alter the conditions in the simulation box. These commands 
// may turn the wall bead interactions on or off, impose an external body force, etc. 
// The commands are checked each time step to see if they are to be executed. 
// Any number of commands can be executed at one time step because we use a 
// successful execution of one command to determine if the next one is checked. 
// This assumes that the commands have
// been sorted into ascending order of time. When a command has executed it is
// removed from the sequence. Because they are stored by pointer we don't delete
// command objects here but leave that to the CAnalysisState object that created 
// them. 
//
// We also calculate the total kinetic and potential energy of the beads if the user 
// has issued the ToggleEnergyOutput command. This applies to both DPD and MD simulations.
//
// Parallel execution
// ******************
//
//

void CSimBox::Run()
{
	for(m_SimTime=1; m_SimTime<=m_TotalTime; m_SimTime++)
	{
        if(IsParallel())
        {
#if EnableParallelSimBox == SimMPSEnabled
//            MPI_Barrier(MPI_COMM_WORLD);  // Keep all processors synchronised each time-step

//            std::cout << "Processor " << GetRank() << " time step " << m_SimTime << zEndl;

            ExecuteCommands();

            EvolveP();
			
		    // Sample the parallel simulation state to construct observables.

		    if(TimeToSample())
		    {
    			Sample();
            }

		    if(TimeToDisplay())
		    {
    			SaveCurrentState();
		    }
#endif
        }
        else
        {
		    // Execute commands prior to updating simulation state because additional
		    // forces may be generated by the commands. First, we execute any active 
            // command groups so that they can add their payloads to the commands
            // container. Then we execute all commands whose execution time is the
            // current time step.

            ExecuteCommandGroups();

            ExecuteCommands();

            // Remove unnecessary features from each simulation type
#if SimIdentifier != BD
		    if(IsRenormalisationOn() && m_SimTime%m_RenormalisationPeriod == 0)
		    {
			    UpdateRenormalisedMom();
		    }
#endif

		    Evolve();

		    CNTCellCheck();		// check beads are in correct CNT cells

		    // Sample the simulation state to construct observables, check if any
		    // events have happened and update the state of all processes.
		    // Note that events that happen faster than SamplePeriod steps
		    // may not be noticed by the analysis until the next time the sampling
		    // is performed.

		    if(TimeToSample())
		    {
    			Sample();

    			ExecuteEvents();

			    // Call the Monitor to update the state of processes that depend on
			    // aggregates and events

    			SampleProcess();

    			SaveProcessState();
		    }

		    if(TimeToDisplay())
		    {
    			SaveCurrentState();
		    }

		    if(TimeToRestart())
		    {
    			SaveRestartState();
		    }
        }
	}
}

// Function to check that the beads in each CNT cell belong there. If the timestep
// is too large, or a bead's velocity is too high, it is possible for a bead to
// move across a whole cell in one timestep and have coordinates outside the extent
// of the cell it has been added to in CCNTCell::UpdataPos(). In that routine we
// only allow a bead to move from one cell to an adjacent cell.
// We write a warning message to the log state in the CCNTCell class but do nothing
// further here.

void CSimBox::CNTCellCheck()
{
	for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		(*iterCell)->CheckBeadsinCell();
	}
}

// Function to add the forces due to bonds to those beads that are in polymers of
// more than one bead. This function is only called in the serial code as the mpsSimBox 
// handles the bonds in polymers and extended polymers for the parallel code.
//
// If the command to toggle the bond stress contributions has been turned off then
// we only do the force calculation loop and ignore the stress. We repeat the whole
// loop to avoid checking the flag for every single bond in the simulation. This 
// means that either all bonds contribute or none of them do.

void CSimBox::AddBondForces()
{
	if(IsBondStressAdded())
	{
		for(PolymerVectorIterator iterPoly=m_vAllPolymers.begin(); iterPoly!=m_vAllPolymers.end(); iterPoly++)
		{
			(*iterPoly)->AddBondForces(this);
		}
	}
	else
	{
		for(PolymerVectorIterator iterPoly=m_vAllPolymers.begin(); iterPoly!=m_vAllPolymers.end(); iterPoly++)
		{
			(*iterPoly)->AddBondForces();
		}
	}
    
    // Add bond forces that bind polymers into nanoparticles. These are dynamically-created but stored in 
    // their owning CNanoparticle instances instead of in the general polymerised bond container.

	for(NanoparticleIterator iterNano=m_Nanoparticles.begin(); iterNano!=m_Nanoparticles.end(); iterNano++)
    {
        (*iterNano)->AddBondForces();
    }    

	// Next add in the forces due to dynamically-created bonds that are not contained
	// in any single polymer or nanoparticle. Note that we should check whether the bond stress is to
	// be added to the analysis of the stress profile.

	BondVector vBonds = GetAllPolymerisedBonds();

	for(BondVectorIterator iterBond=vBonds.begin(); iterBond!=vBonds.end(); iterBond++)
	{
		(*iterBond)->AddForce();
	}
}

// Function to add the 3-body forces due to stiff bonds to those beads that are 
// in polymers containing CBondPairs. We loop over the polymer vector similarly to
// the bond force calculation above.

void CSimBox::AddBondPairForces()
{
	if(IsBondPairStressAdded())
	{
		for(PolymerVectorIterator iterPoly=m_vAllPolymers.begin(); iterPoly!=m_vAllPolymers.end(); iterPoly++)
		{
			(*iterPoly)->AddBondPairForces(this);
		}
	}
	else
	{
		for(PolymerVectorIterator iterPoly=m_vAllPolymers.begin(); iterPoly!=m_vAllPolymers.end(); iterPoly++)
		{
			(*iterPoly)->AddBondPairForces();
		}
	}
}

// Function to add the Morse potential forces to a single polymer type (Helix), to create an alpha helical
// structure out of each polymer.

void CSimBox::AddHelixForces()
{
    for(PolymerVectorIterator iterPoly=m_vHelixPolymers.begin(); iterPoly!=m_vHelixPolymers.end(); iterPoly++)
    {
        (*iterPoly)->AddHelixForces();
    }
}

// Function to add a force due to a uniform field extending throughout the SimBox
// that we label gravity. 
//
// The force acts on specified bead types and may used to simulate the effects of 
// pulling on a particular bead type(s). Because this force is treated as arising
// within the SimBox itself, rather than being due to bead-bead interactions, we add it
// to the beads here instead of making the beads calculate it themselves. This avoids
// having to store the magnitude of the force in the CBead class.

void CSimBox::AddBodyForce()
{
	for(BeadVectorIterator iterBead=m_vGravityBeads.begin(); iterBead!=m_vGravityBeads.end(); iterBead++)
	{
		(*iterBead)->m_Force[0] += m_GravityXForce;
		(*iterBead)->m_Force[1] += m_GravityYForce;
		(*iterBead)->m_Force[2] += m_GravityZForce;
	}
}

// Function to add the forces due to the screened charges on specified bead types. 
// All beads of a given type hold the same charge. We apply the PBCs here and pass
// the bead-bead separation to the AddForce() function because the beads themselves
// have no knowledge of the boundaries of the SimBox. It is more appropriate to
// apply boundary checks inside the SimBox than pass its size into the bead class.

void CSimBox::AddChargedBeadForces()
{
#if EnableMiscClasses == SimMiscEnabled

    for(ChargedBeadListIterator iterBead1=m_lAllChargedBeads.begin(); iterBead1!=m_lAllChargedBeads.end(); iterBead1++)
	{
		for(rChargedBeadListIterator riterBead2=m_lAllChargedBeads.rbegin(); (*riterBead2)->GetId()!=(*iterBead1)->GetId(); ++riterBead2)
		{
			double dx[3];

			dx[0] = (*iterBead1)->GetXPos() - (*riterBead2)->GetXPos();
			dx[1] = (*iterBead1)->GetYPos() - (*riterBead2)->GetYPos();

#if SimDimension == 2
			dx[2] = 0.0;
#elif SimDimension == 3
			dx[2] = (*iterBead1)->GetZPos() - (*riterBead2)->GetZPos();
#endif

			// We have to check the PBCs because the charged bead forces are not like
			// bonds that maintain continuous contact

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

			(*iterBead1)->AddForce(*riterBead2, dx);
		}
	}

#endif
}



// Function to partition the wall beads amongst the CNT cells. Because the walls can 
// have arbitrary thicknesses (subject to not occupying the whole SimBox we simply
// copy the AssignBeadstoCells function's body.

bool CSimBox::AssignWallBeadstoCells()
{
    bool bSuccess = true;

	long index, ix, iy, iz;

	for(AbstractBeadVectorIterator iterBead=m_vWallBeads.begin(); iterBead!=m_vWallBeads.end(); iterBead++)
	{
		ix = static_cast<long>((*iterBead)->GetXPos()/m_CNTXCellWidth);
		iy = static_cast<long>((*iterBead)->GetYPos()/m_CNTYCellWidth);

#if SimDimension == 2
		iz = 0;
#elif SimDimension == 3
		iz = static_cast<long>((*iterBead)->GetZPos()/m_CNTZCellWidth);		
#endif

		index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;
        if(index < m_CNTCellTotal)
        {
		    m_vCNTCells[index]->AddBeadtoCell(*iterBead);
        }
        else
        {
            bSuccess = false;
        }
	} 

    return bSuccess;
}

// **********************************************************************
// Functions implementing the commands available to change the constraints 
// on the SimBox during a simulation.

// Function to freeze the motion of all beads in CNT cells within a specified
// planar slice. This prevents them from moving, but still includes them
// in calculating the forces on other particles. Hence, they provide a 
// "soft" wall of immovable beads to constrain the beads on both sides.
// The slice boundaries and thickness need not be an integer multiple of the 
// CNT cell width in the normal direction, but if not, all beads within all 
// CNT cells intersecting the slice are frozen.
//
// There are still some unresolved issues about whether to zero a bead's momentum
// and force when it is frozen. The DPD algorithm calculates bead momenta
// from the old value and the force and, unless it is zeroed at every time-step
// it will acquire a value even if the bead is nominally frozen. This may lead
// to erroneous values for the dissipative force on (movable) beads near the
// immovable ones. Currently, we zero the bead's internal data on a call to 
// CAbstractBead::SetFrozen() and then prevent its momenta being updated on
// subsequent time-steps by checking GetMovable() in CCNTCell::UpdateMom(). 

void CSimBox::FreezeBeadsInSlice(const xxCommand *const pCommand)
{
#if EnableConstraintCommand == SimCommandEnabled
	const ccFreezeBeadsInSlice* const pCmd = dynamic_cast<const ccFreezeBeadsInSlice*>(pCommand);

	const long xn			= pCmd->GetXNormal();
	const long yn			= pCmd->GetYNormal();
	const long zn			= pCmd->GetZNormal();
	const double lowerBound = pCmd->GetLowerBound();
	const double thickness  = pCmd->GetThickness();

	const double upperBound = lowerBound + thickness;

	// If the upper bound is larger than the SimBox in the normal direction,
	// restrict it to the SimBox length

	long firstSliceIndex = 0;
	long lastSliceIndex  = 0;

	if( xn == 1 )
	{
		firstSliceIndex = static_cast<long>(lowerBound/m_CNTXCellWidth);
		lastSliceIndex  = static_cast<long>(upperBound/m_CNTXCellWidth);

		if(lastSliceIndex > GetCNTXCellNo())
		   lastSliceIndex = GetCNTXCellNo();
	}
	else if( yn == 1 )
	{
		firstSliceIndex = static_cast<long>(lowerBound/m_CNTYCellWidth);
		lastSliceIndex  = static_cast<long>(upperBound/m_CNTYCellWidth);

		if(lastSliceIndex > GetCNTYCellNo())
		   lastSliceIndex = GetCNTYCellNo();
	}
	else
	{
		firstSliceIndex = static_cast<long>(lowerBound/m_CNTZCellWidth);
		lastSliceIndex  = static_cast<long>(upperBound/m_CNTZCellWidth);

		if(lastSliceIndex > GetCNTZCellNo())
		   lastSliceIndex = GetCNTZCellNo();
	}

	// Iterate over all the slices of CNT cells that lie within the specified range,
	// create a CCNTCellSlice object to hold the cells in each slice and iterate 
	// over their beads calling their SetFrozen() function to immobilise them.
	// We use the for_each algorithm to call the CAbstractBead::SetFrozen() member
	// function for each bead in the container (use SetNotFrozen() to unfreeze beads).
	// Two functions with no arguments are used instead of a single function with
	// an argument for speed.
	// We use a counter to see how many beads are frozen.

	long beadsFrozen = 0;

	for(long index = firstSliceIndex; index < lastSliceIndex; index++)
	{
		CCNTCellSlice slice1(index, xn, yn, zn, GetISimBox());

		for(long cellIndex = 0; cellIndex < slice1.GetSize(); cellIndex++)
		{
			const CCNTCell* const pCell = slice1.GetCell(cellIndex);

			BeadList beads = pCell->GetBeads();

			for_each(beads.begin(), beads.end(), mem_fn(&CAbstractBead::SetFrozen));

			beadsFrozen += beads.size();
		}
	}

	// Write a message to the log file showing how many beads have been frozen

	new CLogFreezeBeadsInSlice(m_SimTime, beadsFrozen, xn, yn, zn, lowerBound, thickness);
#endif
}


// Function to freeze the motion of all beads in CNT cells within a specified
// spherical shell. This prevents them from moving, but still includes them
// in calculating the forces on other particles. Hence, they provide a 
// "soft" wall of immovable beads to constrain the beads inside and outside
// the shell. The shell is centred on the middle of the SimBox and has an 
// arbitrary radius and thickness (even extending outside the SimBox). This
// allows cells in the corners of the SimBox to be collected leaving spaces
// in the centres of the SimBox faces if desired.

void CSimBox::FreezeBeadsInSphericalShell(const xxCommand *const pCommand)
{
#if EnableConstraintCommand == SimCommandEnabled

	const ccFreezeBeadsInSphericalShell* const pCmd = dynamic_cast<const ccFreezeBeadsInSphericalShell*>(pCommand);

	const double shellInnerRadius	= pCmd->GetInnerRadius();
	const double shellThickness		= pCmd->GetThickness();

	const double shellOuterRadius	= shellInnerRadius + shellThickness;

	double dx, dy, dz;
	double radius = 0.0;

	// For each bead check its distance from the SimBox centre, find the CCNTCell
	// that contains it from its coordinates and freeze it if it lies within
	// the spherical shell defined by the command. We start a counter to find
	// out how many beads are frozen for use in the message log.
	
	long beadsFrozen = 0;

    AbstractBeadVector vAllBeads = GetAllBeadsInCNTCells();
	for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		dx = (*iterBead)->GetXPos() - m_HalfSimBoxXLength;
		dy = (*iterBead)->GetYPos() - m_HalfSimBoxYLength;

#if SimDimension == 2
		radius = sqrt(dx*dx + dy*dy);
#elif SimDimension == 3
		dz = (*iterBead)->GetZPos() - m_HalfSimBoxZLength;
		radius = sqrt(dx*dx + dy*dy + dz*dz);
#endif

		if(radius > shellInnerRadius && radius < shellOuterRadius)
		{
			beadsFrozen++;
			(*iterBead)->SetFrozen();
		}
	} 

	// Write a message to the log file showing how many beads have been frozen

	new CLogFreezeBeadsInSphericalShell(m_SimTime, beadsFrozen, shellInnerRadius, shellThickness);
#endif
}

// Function to remove beads from CNT cells that are inside a
// specified radius of the centre of the SimBox. This reduces the number of beads
// that have to be integrated and hence speeds up the simulation.
//
// Note that the beads are not deleted only removed from the CNT cell that contained
// them. They will not contribute to the calculation of forces, nor have their 
// position and momentum updated. Also, any beads that move into the cells whose
// beads have been removed will still be included in the integration loop. Hence,
// a buffer shell of frozen beads should be provided to prevent beads migrating
// into the empty space.

void CSimBox::RemoveCNTCellsInsideRadius(const xxCommand *const pCommand)
{
#if EnableConstraintCommand == SimCommandEnabled

	const ccRemoveCNTCellsInsideRadius* const pCmd = dynamic_cast<const ccRemoveCNTCellsInsideRadius*>(pCommand);

 	const double shellRadius = pCmd->GetRadius();

	long index, ix, iy, iz;
	double dx, dy, dz;
	double radius = 0.0;

    AbstractBeadVector vAllBeads = GetAllBeadsInCNTCells();
	for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		dx = (*iterBead)->GetXPos() - m_HalfSimBoxXLength;
		dy = (*iterBead)->GetYPos() - m_HalfSimBoxYLength;

		ix = static_cast<long>((*iterBead)->GetXPos()/m_CNTXCellWidth);
		iy = static_cast<long>((*iterBead)->GetYPos()/m_CNTYCellWidth);

#if SimDimension == 2
		iz = 0;
		radius = sqrt(dx*dx + dy*dy);
#elif SimDimension == 3
		dz = (*iterBead)->GetZPos() - m_HalfSimBoxZLength;
		iz = static_cast<long>((*iterBead)->GetZPos()/m_CNTZCellWidth);	
		radius = sqrt(dx*dx + dy*dy + dz*dz);
#endif

		index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;

		if(radius < shellRadius)
		{
			m_vCNTCells[index]->RemoveBeadFromCell(*iterBead);
		}
	} 
#endif
}

// Function to remove beads from CNT cells that are outside a
// specified radius of the centre of the SimBox. This reduces the number of beads
// that have to be integrated and hence speeds up the simulation.
//
// Note that the beads are not deleted only removed from the CNT cell that contained
// them. They will not contribute to the calculation of forces, nor have their 
// position and momentum updated. Also, any beads that move into the cells whose
// beads have been removed will still be included in the integration loop. Hence,
// a buffer shell of frozen beads should be provided to prevent beads migrating
// into the empty space.

void CSimBox::RemoveCNTCellsOutsideRadius(const xxCommand *const pCommand)
{
#if EnableConstraintCommand == SimCommandEnabled

	const ccRemoveCNTCellsOutsideRadius* const pCmd = dynamic_cast<const ccRemoveCNTCellsOutsideRadius*>(pCommand);

 	const double shellRadius = pCmd->GetRadius();


	long index, ix, iy, iz;
	double dx, dy, dz;
	double radius = 0.0;

    AbstractBeadVector vAllBeads = GetAllBeadsInCNTCells();
	for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		dx = (*iterBead)->GetXPos() - m_HalfSimBoxXLength;
		dy = (*iterBead)->GetYPos() - m_HalfSimBoxYLength;

		ix = static_cast<long>((*iterBead)->GetXPos()/m_CNTXCellWidth);
		iy = static_cast<long>((*iterBead)->GetYPos()/m_CNTYCellWidth);

#if SimDimension == 2
		iz = 0;
		radius = sqrt(dx*dx + dy*dy);
#elif SimDimension == 3
		dz = (*iterBead)->GetZPos() - m_HalfSimBoxZLength;
		iz = static_cast<long>((*iterBead)->GetZPos()/m_CNTZCellWidth);	
		radius = sqrt(dx*dx + dy*dy + dz*dz);
#endif

		index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;

		if(radius > shellRadius)
		{
			m_vCNTCells[index]->RemoveBeadFromCell(*iterBead);
		}
	} 
#endif
}


//
// Functions to add/remove the wall from the SimBox. 
//
// To turn the wall on we simply distribute the wall beads amongst the CNT cells
// according to their coordinates. If the wall is already on we do nothing as we
// don't want to add the bead pointers to the cells twice.
// When turning the wall off we don't destroy the wall beads because we may 
// want to turn the wall back on at a later time. We simply remove them from the 
// CNTCell bead lists. They still exist in the SimBox's wall bead container and  
// can be redistributed through the simulation box if required.
// We loop over each wall bead finding the CNT cell that contains it from its
// coordinates: because wall beads don't move during the simulation this is easy,
// then we remove the bead pointer from the CNT cell. 

void CSimBox::WallOn(const xxCommand *const pCommand)
{
	const ccWallOn* const pCmd = dynamic_cast<const ccWallOn*>(pCommand);

	if(!IsWallOn())
	{
		// Update the CSimState to reflect the command's action

		SetWallOn(true);

		if(AssignWallBeadstoCells())
        {
 		    // If the command specifies to zero the total bead momentum issue a
		    // ZeroTotalMom command to execute at the next time step. See WallOff for
		    // a detailed description. 

		    if(pCmd->ZeroTotalMom())
		    {
			    AddCommand(new ccZeroTotalMom(m_SimTime+1));
		    }
        }
        else
        {
            new CLogSimErrorTrace(m_SimTime, "Unable to assign beads to cells: stopping run");
	        AddCommand(new ccStopNoSave(m_SimTime+1));
        }
	}
}

void CSimBox::WallOff(const xxCommand *const pCommand)
{
	const ccWallOff* const pCmd = dynamic_cast<const ccWallOff*>(pCommand);

	if(IsWallOn())
	{
		SetWallOn(false);

		for(cAbstractBeadVectorIterator iterBead=m_vWallBeads.begin(); iterBead!=m_vWallBeads.end(); iterBead++)
		{
			long ix = static_cast<long>((*iterBead)->GetXPos()/m_CNTXCellWidth);
			long iy = static_cast<long>((*iterBead)->GetYPos()/m_CNTYCellWidth);

#if SimDimension == 2
			long iz = 0;
#elif SimDimension == 3
			long iz = static_cast<long>((*iterBead)->GetZPos()/m_CNTZCellWidth);		
#endif

			long index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;
			m_vCNTCells[index]->RemoveBeadFromCell(*iterBead);
		}

		// If the command specifies to zero total momentum schedule a ZeroTotalMom command
		// to occur on the next time step to allow time for the forces to feed through
		// into the momentum before we subtract the CM momentum from all movable beads. 
		// We use a constructor that sets the command's valid flag because we are not 
		// reading its data in from the control data file where validation is performed 
		// and the valid flag set accordingly.

		// Because the current command has not been removed from the command list we 
		// insert the ZeroTotalMom command second in the list. We use the prefix
		// increment operator to insert after the first element otherwise the 
		// increment is applied after the function call. 

		if(pCmd->ZeroTotalMom())
		{
			AddCommand(new ccZeroTotalMom(m_SimTime+1));
		}
	}
}

// Functions to turn the external body force in the SimBox on and off.

void CSimBox::GravityOn(const xxCommand *const pCommand)
{
//	const ccGravityOn* const pCmd = dynamic_cast<const ccGravityOn*>(pCommand);

	if(!IsGravityOn())
	{
		SetGravityOn(true);
	}
}

void CSimBox::GravityOff(const xxCommand *const pCommand)
{
//	const ccGravityOff* const pCmd = dynamic_cast<const ccGravityOff*>(pCommand);

	if(IsGravityOn())
	{
		SetGravityOn(false);
	}
}

// Function to zero the total CM momentum of all movable beads (not wall polymer head
// beads but their body beads are included). We don't actually use the command object 
// here but provide a pointer for future development.

void CSimBox::ZeroTotalMom(const xxCommand *const pCommand)
{
//	const ccZeroTotalMom* const pCmd = dynamic_cast<const ccZeroTotalMom*>(pCommand);

	double vCMX = 0.0;
	double vCMY = 0.0;
	double vCMZ = 0.0;

    AbstractBeadVector vAllBeads = GetAllBeadsInCNTCells();

	AbstractBeadVectorIterator iterBead; // Used in two loops below
	for(iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		vCMX += (*iterBead)->GetXMom();
		vCMY += (*iterBead)->GetYMom();
		vCMZ += (*iterBead)->GetZMom();
	}

	vCMX /= static_cast<double>(m_BeadTotal);
	vCMY /= static_cast<double>(m_BeadTotal);
	vCMZ /= static_cast<double>(m_BeadTotal);

	for(iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		(*iterBead)->SetXMom( (*iterBead)->GetXMom() - vCMX );
		(*iterBead)->SetYMom( (*iterBead)->GetYMom() - vCMY );
		(*iterBead)->SetZMom( (*iterBead)->GetZMom() - vCMZ );
	}
}

// Function to implement a selection command that collects beads of the 
// specified types whose coordinates are within a given slice of CNT cells. 
// They are packaged into a CForceTarget aggregate that can receive ccApplyForce 
// commands.
//
// We assume that the data has been validated prior to this routine.

void CSimBox::SelectBeadsInSlice(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccSelectBeadsInSlice* const pCmd = dynamic_cast<const ccSelectBeadsInSlice*>(pCommand);

	// Data needed to define an aggregate

	const zString label = pCmd->GetLabel();	//	Label of the aggregate
	long xNormal = pCmd->GetX();
	long yNormal = pCmd->GetY();			//	Slice normal
	long zNormal = pCmd->GetZ();
	long sliceIndex = pCmd->GetSliceIndex();
	const StringSequence beadNames = pCmd->GetNames();	// Names of bead types to collect

	// Find the integer types of selected beads from their name strings

	zLongVector beadTypes;

	for(cStringSequenceIterator iterName=beadNames.begin(); iterName!=beadNames.end(); iterName++)
	{
		long type = GetBeadTypeFromName(*iterName);

		beadTypes.push_back(type);
	}

	// Create a CSlice, use its ability to collect all beads within its volume, 
	// and then search the bead list for those of the required types. We pass it the
	// coordinates identifying the slice and note that they must be validated for
	// use by a slice prior to this routine. Also, the GetBeads() function does not seem
	// to work when used inside the for loop statements, so I copy it to a local
	// variable before looping.

	CSlice targetSlice(sliceIndex, xNormal, yNormal, zNormal, GetISimBox());


	BeadVector aggregateBeads;

	aggregateBeads.clear();

	BeadList sliceBeads = targetSlice.GetBeads();

	for(cBeadListIterator iterBead=sliceBeads.begin(); iterBead!=sliceBeads.end(); iterBead++)
	{
		czLongVectorIterator iBeadType = find(beadTypes.begin(), beadTypes.end(), (*iterBead)->GetType());

		if(iBeadType != beadTypes.end())
		{
			CBead* pBead = dynamic_cast<CBead*>(*iterBead);
			if(pBead)
				aggregateBeads.push_back(pBead);
		}
	}

	// Create a CForceTarget object to hold the labelled bead aggregate ready to
	// receive commands to apply a force. The target stays on the m_ForceTarget 
	// list until a command is received to delete it or the SimBox destructor is
	// called.

	CForceTarget* pTarget = new CForceTarget(label, aggregateBeads);

	m_ForceTargets.push_back(pTarget);
#endif
}

// Function to select specified beads whose coordinates lie within a row of
// CNT cells in a user-defined slice.

void CSimBox::SelectBeadsInRow(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccSelectBeadsInRow* const pCmd = dynamic_cast<const ccSelectBeadsInRow*>(pCommand);

	// Data needed to define an aggregate

	const zString label = pCmd->GetLabel();	//	Label of the aggregate
	long xNormal = pCmd->GetX();
	long yNormal = pCmd->GetY();			//	Slice normal
	long zNormal = pCmd->GetZ();
	long sliceIndex = pCmd->GetSliceIndex();
	long rowIndex   = pCmd->GetRowIndex();		// Row location in slice

	const StringSequence beadNames = pCmd->GetNames();	// Names of bead types to collect

	// Find the integer types of selected beads from their name strings

	zLongVector beadTypes;

	for(cStringSequenceIterator iterName=beadNames.begin(); iterName!=beadNames.end(); iterName++)
	{
		long type = GetBeadTypeFromName(*iterName);

		beadTypes.push_back(type);
	}

	// Create a CSlice, use its ability to collect all beads within its volume, and
	// create a CRow from the slice. The row obtains the number of rows in the slice
	// and its size from the CSlice. Then search the row's bead list for those of the 
	// required types. The GetBeads() function does not seem to work when used inside the 
	// for loop statements, so I copy it to a local variable before looping.

	CSlice targetSlice(sliceIndex, xNormal, yNormal, zNormal, GetISimBox());

	CRow targetRow(targetSlice, rowIndex);

	BeadList rowBeads = targetRow.GetBeads();

	BeadVector aggregateBeads;

	aggregateBeads.clear();

	for(cBeadListIterator iterBead=rowBeads.begin(); iterBead!=rowBeads.end(); iterBead++)
	{
		czLongVectorIterator iBeadType = find(beadTypes.begin(), beadTypes.end(), (*iterBead)->GetType());

		if(iBeadType != beadTypes.end())
		{
			CBead* pBead = dynamic_cast<CBead*>(*iterBead);
			if(pBead)
				aggregateBeads.push_back(pBead);
		}
	}

	// Create a CForceTarget object to hold the labelled bead aggregate ready to
	// receive commands to apply a force. The target stays on the m_ForceTarget 
	// list until a command is received to delete it or the SimBox destructor is
	// called.

	CForceTarget* pTarget = new CForceTarget(label, aggregateBeads);

	m_ForceTargets.push_back(pTarget);

#endif
}

// This is the simplest routine as the beads are already divided into the
// CNT cells. We just use the indices to access the appropriate cell and 
// create the CForceTarget. Note that the slice normal data is not used.

void CSimBox::SelectBeadsInCell(const xxCommand *const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccSelectBeadsInCell* const pCmd = dynamic_cast<const ccSelectBeadsInCell*>(pCommand);

	// Data needed to define an aggregate

	const zString label = pCmd->GetLabel();		//	Label of the aggregate
	long xCellIndex = pCmd->GetXCellIndex();
	long yCellIndex = pCmd->GetYCellIndex();	// Cell coordinates
	long zCellIndex = pCmd->GetZCellIndex();

	const StringSequence beadNames = pCmd->GetNames();	// Names of bead types to collect

	// Find the integer types of selected beads from their name strings

	zLongVector beadTypes;

	for(cStringSequenceIterator iterName=beadNames.begin(); iterName!=beadNames.end(); iterName++)
	{
		long type = GetBeadTypeFromName(*iterName);

		beadTypes.push_back(type);
	}

	long index = m_CNTXCellNo*(m_CNTYCellNo*zCellIndex + yCellIndex) + xCellIndex;

	BeadList cellBeads = m_vCNTCells.at(index)->GetBeads();

	BeadVector aggregateBeads;

	aggregateBeads.clear();

	for(cBeadListIterator iterBead=cellBeads.begin(); iterBead!=cellBeads.end(); iterBead++)
	{
		czLongVectorIterator iBeadType = find(beadTypes.begin(), beadTypes.end(), (*iterBead)->GetType());

		if(iBeadType != beadTypes.end())
		{
			CBead* pBead = dynamic_cast<CBead*>(*iterBead);
			if(pBead)
				aggregateBeads.push_back(pBead);
		}
	}

	// Create a CForceTarget object to hold the labelled bead aggregate ready to
	// receive commands to apply a force. The target stays on the m_ForceTarget 
	// list until a command is received to delete it or the SimBox destructor is
	// called.

	CForceTarget* pTarget = new CForceTarget(label, aggregateBeads);

	m_ForceTargets.push_back(pTarget);

#endif
}

// Function to implement a selection command that collects beads of the 
// specified types whose coordinates are within a given cylinderical region of
// the SimBox. The radius of the region is specified in units of the bead diameter,
// and its upper and lower edges are specified as CNTCell slice indices.
// This means that if beads of different sizes are used in a simulation this
// command may have to be altered to take this into account.
//
// The beads are packaged into a CForceTarget aggregate that can receive subsequent 
// ccApplyForce commands.
//
// We assume that the input data have been validated prior to this routine.

void CSimBox::SelectBeadsInCylinder(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccSelectBeadsInCylinder* const pCmd = dynamic_cast<const ccSelectBeadsInCylinder*>(pCommand);

	// Data needed to define an aggregate

	const zString label = pCmd->GetLabel();			//	Label of the aggregate
	long xNormal		= pCmd->GetX();
	long yNormal		= pCmd->GetY();				//	Slice normal
	long zNormal		= pCmd->GetZ();
	long lowerIndex		= pCmd->GetLowerEdge();		// Top and bottom of cylinder
	long upperIndex		= pCmd->GetUpperEdge();
	double centre1      = pCmd->GetCentre1();		// Axes of cylinder
	double centre2		= pCmd->GetCentre2();
	double innerRadius  = pCmd->GetInnerRadius();	// Inner and out radii of cylinder
	double outerRadius  = pCmd->GetOuterRadius();

	const StringSequence beadNames = pCmd->GetNames();	// Names of bead types to collect

	// Find the integer types of selected beads from their name strings

	zLongVector beadTypes;

	for(cStringSequenceIterator iterName=beadNames.begin(); iterName!=beadNames.end(); iterName++)
	{
		long type = GetBeadTypeFromName(*iterName);

		beadTypes.push_back(type);
	}

	// Create a CSlice for each CNTCell slice in the cylinder's length, use its 
	// ability to collect all beads within its volume, and then search the 
	// bead list for those of the required types. We pass it the
	// coordinates identifying the slice and note that the cylinder length
	// indices are zero-indexed. For each bead that matches the types specified,
	// we check to see if its position lies withint the cylinder and add it to 
	// the target if so.

	// The GetBeads() function does not seem to work when used inside the 
	// for-loop statements, so I copy it to a local variable before looping.

	// Calculate the coordinates of the cylinder's central axis in the plane 
	// defined by its normal; and store the inner and outer radii squared to 
	// save time when comparing the bead distances from the cylinder's centre. 

	double rAxis1  = 0.0;
	double rAxis2  = 0.0;

	if(xNormal == 1)
	{
		rAxis1	= centre1*GetSimBoxYLength();
		rAxis2	= centre2*GetSimBoxZLength();

	}
	else if(yNormal == 1)
	{
		rAxis1	= centre1*GetSimBoxZLength();
		rAxis2	= centre2*GetSimBoxXLength();
	}
	else
	{
		rAxis1	= centre1*GetSimBoxXLength();
		rAxis2	= centre2*GetSimBoxYLength();
	}

	const double rMinSq = innerRadius*innerRadius;
	const double rMaxSq = outerRadius*outerRadius;

	double rDistSq = 0.0;

	BeadVector aggregateBeads;
	aggregateBeads.clear();

	for(long sliceIndex=lowerIndex; sliceIndex<=upperIndex; sliceIndex++)
	{
		CSlice targetSlice(sliceIndex, xNormal, yNormal, zNormal, GetISimBox());

		BeadList sliceBeads = targetSlice.GetBeads();

		for(cBeadListIterator iterBead=sliceBeads.begin(); iterBead!=sliceBeads.end(); iterBead++)
		{
			czLongVectorIterator iBeadType = find(beadTypes.begin(), beadTypes.end(), (*iterBead)->GetType());

			if(iBeadType != beadTypes.end())
			{
				CBead* pBead = dynamic_cast<CBead*>(*iterBead);

				if(pBead)
				{
					const double xBead = pBead->GetXPos();
					const double yBead = pBead->GetYPos();
					const double zBead = pBead->GetZPos();

					if(xNormal == 1)
					{
						rDistSq = (yBead - rAxis1)*(yBead - rAxis1) + 
								  (zBead - rAxis2)*(zBead - rAxis2);
					}
					else if(yNormal == 1)
					{
						rDistSq = (zBead - rAxis1)*(zBead - rAxis1) + 
								  (xBead - rAxis2)*(xBead - rAxis2);
					}
					else
					{
						rDistSq = (xBead - rAxis1)*(xBead - rAxis1) + 
								  (yBead - rAxis2)*(yBead - rAxis2);
					}

					if(rDistSq > rMinSq && rDistSq < rMaxSq)
						aggregateBeads.push_back(pBead);
				}
			}
		}
	}

	// Create a CForceTarget object to hold the labelled bead aggregate ready to
	// receive commands to apply a force. The target stays on the m_ForceTarget 
	// list until a command is received to delete it or the SimBox destructor is
	// called.

	std:: cout << "No of beads in ring = " << aggregateBeads.size() << zEndl;

	CForceTarget* pTarget = new CForceTarget(label, aggregateBeads);

	m_ForceTargets.push_back(pTarget);

#endif
}


// Function to implement the command to apply a constant force to a labelled target.
// 
// The target must be in the m_ForceTargets sequence and is copied to 
// m_ActiveForceTargets and a CForceLaw-derived object is created to hold the 
// force data. We postpone actually creating the data until the target has 
// been found: if it is not found the command has no effect.

void CSimBox::ConstantForce(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

    const ccConstantForce* const pCmd = dynamic_cast<const ccConstantForce*>(pCommand);
	
	// Find the target in m_ForceTargets, pass it a constant force object
	// and copy it to the active target sequence. We don't remove it from 
	// the m_ForceTargets list as it may be reused for another command later.

	for(ForceTargetIterator iterTarget=m_ForceTargets.begin(); iterTarget!=m_ForceTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetLabel() == pCmd->GetLabel())
		{
			long start = m_SimTime;
			long end   = m_SimTime + pCmd->GetDuration();
			double mag = pCmd->GetMagnitude();

			(*iterTarget)->SetForceLaw(new CConstantForce(start, end, pCmd->GetX(), pCmd->GetY(), pCmd->GetZ(), mag));
			m_ActiveForceTargets.push_back(*iterTarget);
		}
	}

#endif
}

// Function to implement the command to apply a linear force to a labelled target.
//
// The target must be in the m_ForceTargets sequence and is copied to 
// m_ActiveForceTargets and a CForceLaw-derived object is created to hold the force data. 
// We postpone actually creating the data until the target has been found: if it is not 
// found the command has no effect.

void CSimBox::LinearForce(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccLinearForce* const pCmd = dynamic_cast<const ccLinearForce*>(pCommand);
	
	// Find the target in m_ForceTargets, pass it a linear force object
	// and copy it to the active target sequence. We don't remove it from 
	// the m_ForceTargets list as it may be reused for another command later.

	for(ForceTargetIterator iterTarget=m_ForceTargets.begin(); iterTarget!=m_ForceTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetLabel() == pCmd->GetLabel())
		{
			long start = m_SimTime;
			long end   = m_SimTime + pCmd->GetDuration();

			double rate = pCmd->GetRate();

			(*iterTarget)->SetForceLaw(new CLinearForce(start, end, pCmd->GetX(), pCmd->GetY(), pCmd->GetZ(), rate));
			m_ActiveForceTargets.push_back(*iterTarget);
		}
	}

#endif
}

// Function to implement the command to apply a radial force to a labelled target.
//
void CSimBox::RadialForce(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

    const ccRadialForce* const pCmd = dynamic_cast<const ccRadialForce*>(pCommand);

	const double		cx = pCmd->GetXCentre();
	const double		cy = pCmd->GetYCentre();
	const double		cz = pCmd->GetZCentre();
	const double magnitude = pCmd->GetMagnitude();

	// Convert the force origin coordinates as fractions of the SimBox side lengths
	// into values relative to the bead diameter

	const double fcx = cx*GetSimBoxXLength();
	const double fcy = cy*GetSimBoxYLength();
	const double fcz = cz*GetSimBoxZLength();

	// Find the target in m_ForceTargets, pass it a radial force object
	// and copy it to the active target sequence. We don't remove it from 
	// the m_ForceTargets list as it may be reused for another command later.

	for(ForceTargetIterator iterTarget=m_ForceTargets.begin(); iterTarget!=m_ForceTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetLabel() == pCmd->GetLabel())
		{
			long start = m_SimTime;
			long end   = m_SimTime + pCmd->GetDuration();

			BeadVector beads = (*iterTarget)->GetBeads();

			if(beads.size() > 0)
			{
				(*iterTarget)->SetForceLaw(new CRadialForce(start, end, pCmd->GetX(), pCmd->GetY(), pCmd->GetZ(), 
															fcx, fcy, fcz, magnitude, beads));
				m_ActiveForceTargets.push_back(*iterTarget);
			}
		}
	}

#endif
}

// Function to implement the command to apply a sinusoidal force to a labelled target.
//
// The target must be in the m_ForceTargets sequence and is copied to 
// m_ActiveForceTargets and a CForceLaw-derived object is created to hold the force data. 
// We postpone actually creating the data until the target has been found: if it is not 
// found the command has no effect.

void CSimBox::SineForce(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

    const ccSineForce* const pCmd = dynamic_cast<const ccSineForce*>(pCommand);

	// Find the target in m_ForceTargets, pass it a sine force object
	// and copy it to the active target sequence. We don't remove it from 
	// the m_ForceTargets list as it may be reused for another command later.

	for(ForceTargetIterator iterTarget=m_ForceTargets.begin(); iterTarget!=m_ForceTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetLabel() == pCmd->GetLabel())
		{
			long start = m_SimTime;
			long end   = m_SimTime + pCmd->GetDuration();

			double amp    = pCmd->GetAmplitude();
			double period = pCmd->GetPeriod();

			(*iterTarget)->SetForceLaw(new CSineForce(start, end, pCmd->GetX(), pCmd->GetY(), pCmd->GetZ(), amp, period));
			m_ActiveForceTargets.push_back(*iterTarget);
		}
	}

#endif
}

// Function to implement the command to apply a force to a labelled target that
// represents a transducer moving at constant velocity away from the target and
// transmitting a force by a Hookean spring.
// 
// The target must be in the m_ForceTargets sequence and is copied to 
// m_ActiveForceTargets and a CForceLaw-derived object is created to hold the force data. 
// We postpone actually creating the data until the target has been found: if it is not 
// found the command has no effect.

void CSimBox::SpringForce(const xxCommand *const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

    const ccSpringForce* const pCmd = dynamic_cast<const ccSpringForce*>(pCommand);

	// Find the target in m_ForceTargets, pass it a spring force object
	// and copy it to the active target sequence. We don't remove it from 
	// the m_ForceTargets list as it may be reused for another command later.
	// We only copy the target across if there are beads in it, if it is empty
	// then we ignore the command.

	for(ForceTargetIterator iterTarget=m_ForceTargets.begin(); iterTarget!=m_ForceTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetLabel() == pCmd->GetLabel())
		{
			long start = m_SimTime;
			long end   = m_SimTime + pCmd->GetDuration();

			double k			= pCmd->GetSpringConst();
			double len			= pCmd->GetSpringLength();
			double vel			= pCmd->GetSpringVelocity();
			BeadVector beads	= (*iterTarget)->GetBeads();

			if(beads.size() > 0)
			{
				(*iterTarget)->SetForceLaw(new CSpringForce(start, end, pCmd->GetX(), pCmd->GetY(), pCmd->GetZ(), k, len, vel, beads));
				m_ActiveForceTargets.push_back(*iterTarget);
			}
		}
	}

#endif
}

// Function to collect all polymers of specified types that have their head beads
// within a certain slice into a CForceTarget aggregate that can have forces
// applied to it by commands. We assume that the data has been validated prior to 
// this routine.

void CSimBox::SelectPolymersInSlice(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled
//	const ccSelectPolymersInSlice* const pCmd = dynamic_cast<const ccSelectPolymersInSlice*>(pCommand);
#endif
}

void CSimBox::SelectPolymersInRow(const xxCommand *const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled
//	const ccSelectPolymersInRow* const pCmd = dynamic_cast<const ccSelectPolymersInRow*>(pCommand);
#endif
}

void CSimBox::SelectPolymersInCell(const xxCommand *const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled
//	const ccSelectPolymersInCell* const pCmd = dynamic_cast<const ccSelectPolymersInCell*>(pCommand);
#endif
}

// Function to add the beads in a CNT slice to those in a previously-defined
// CForceTarget. The data specified by the command is the same as for the
// SelectBeadsInxxx() commands but we use the label to access an existing
// CForceTarget object instead of creating a new one. 

void CSimBox::AddBeadsInSlice(const xxCommand *const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccAddBeadsInSlice* const pCmd = dynamic_cast<const ccAddBeadsInSlice*>(pCommand);

	// Data needed to define an aggregate

	const zString label = pCmd->GetLabel();	//	Label of the aggregate
	long xNormal = pCmd->GetX();
	long yNormal = pCmd->GetY();			//	Slice normal
	long zNormal = pCmd->GetZ();
	long sliceIndex = pCmd->GetSliceIndex();
	StringSequence beadNames = pCmd->GetNames();	// Names of bead types to collect

	// Find the integer types of selected beads from their name strings. We know
	// that the bead types do not occur in the target already because this is
	// checked in the ccAddxxxInyyy commands' IsDataValid() functions.

	zLongVector beadTypes;

	for(cStringSequenceIterator iterName=beadNames.begin(); iterName!=beadNames.end(); iterName++)
	{
		beadTypes.push_back(GetBeadTypeFromName(*iterName));
	}

	// Create a CSlice, use its ability to collect all beads within its volume, 
	// and then search the bead list for those of the required types. We pass it the
	// coordinates identifying the slice and note that they must be validated for
	// use by a slice prior to this routine. Also, the GetBeads() function does not seem
	// to work when used inside the for loop statements, so I copy it to a local
	// variable before looping.

	CSlice targetSlice(sliceIndex, xNormal, yNormal, zNormal, GetISimBox());

	BeadVector aggregateBeads;

	aggregateBeads.clear();

	BeadList sliceBeads = targetSlice.GetBeads();

	for(cBeadListIterator iterBead=sliceBeads.begin(); iterBead!=sliceBeads.end(); iterBead++)
	{
		czLongVectorIterator iBeadType = find(beadTypes.begin(), beadTypes.end(), (*iterBead)->GetType());

		if(iBeadType != beadTypes.end())
		{
			CBead* pBead = dynamic_cast<CBead*>(*iterBead);
			if(pBead)
				aggregateBeads.push_back(pBead);
		}
	}

	// Find the CForceTarget object to add the beads to. The target 
	// stays on the m_ForceTarget list until a command is received to delete 
	// it or the SimBox destructor is called. We cannot use find() here because
	// we need to search on the label property of CForceTarget objects.
	// Note that the command target name has already been validated in CInputData,
	// that is, a target with the correct name is known to exist.
	// Remove any duplicate beads in the target. We first sort the beads by their
	// pointer value and then remove adjacent duplicates

	for(cForceTargetIterator iterTarget=m_ForceTargets.begin(); iterTarget!=m_ForceTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetLabel() == label)
		{
			(*iterTarget)->AddBeads(aggregateBeads);

			(*iterTarget)->RemoveDuplicateBeads();
		}
	}

#endif
}

// Function to add the beads in a CNT row to those in a previously-defined
// CForceTarget. The data specified by the command is the same as for the
// SelectBeadsInxxx() commands but we use the label to access an existing
// CForceTarget object instead of creating a new one. 

void CSimBox::AddBeadsInRow(const xxCommand *const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccAddBeadsInRow* const pCmd = dynamic_cast<const ccAddBeadsInRow*>(pCommand);

	// Data needed to define an aggregate

	const zString label = pCmd->GetLabel();	//	Label of the aggregate
	long xNormal = pCmd->GetX();
	long yNormal = pCmd->GetY();			//	Slice normal
	long zNormal = pCmd->GetZ();
	long sliceIndex = pCmd->GetSliceIndex();
	long rowIndex   = pCmd->GetRowIndex();		// Row location in slice

	const StringSequence beadNames = pCmd->GetNames();	// Names of bead types to collect

	// Find the integer types of selected beads from their name strings

	zLongVector beadTypes;

	for(cStringSequenceIterator iterName=beadNames.begin(); iterName!=beadNames.end(); iterName++)
	{
		long type = GetBeadTypeFromName(*iterName);

		beadTypes.push_back(type);
	}

	// Create a CSlice, use its ability to collect all beads within its volume, and
	// create a CRow from the slice. The row obtains the number of rows in the slice
	// and its size from the CSlice. Then search the row's bead list for those of the 
	// required types. The GetBeads() function does not seem to work when used inside the 
	// for loop statements, so I copy it to a local variable before looping.

	CSlice targetSlice(sliceIndex, xNormal, yNormal, zNormal, GetISimBox());

	CRow targetRow(targetSlice, rowIndex);

	BeadList rowBeads = targetRow.GetBeads();

	BeadVector aggregateBeads;

	aggregateBeads.clear();

	for(cBeadListIterator iterBead=rowBeads.begin(); iterBead!=rowBeads.end(); iterBead++)
	{
		czLongVectorIterator iBeadType = find(beadTypes.begin(), beadTypes.end(), (*iterBead)->GetType());

		if(iBeadType != beadTypes.end())
		{
			CBead* pBead = dynamic_cast<CBead*>(*iterBead);
			if(pBead)
				aggregateBeads.push_back(pBead);
		}
	}

	// Find the CForceTarget object to add the beads to. The target 
	// stays on the m_ForceTarget list until a command is received to delete 
	// it or the SimBox destructor is called. We cannot use find() here because
	// we need to search on the label property of CForceTarget objects.
	// Note that the command target name has already been validated in CInputData,
	// that is, a target with the correct name is known to exist.

	for(cForceTargetIterator iterTarget=m_ForceTargets.begin(); iterTarget!=m_ForceTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetLabel() == label)
		{
			(*iterTarget)->AddBeads(aggregateBeads);
			(*iterTarget)->RemoveDuplicateBeads();
		}
	}

#endif
}

void CSimBox::AddBeadsInCell(const xxCommand *const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccAddBeadsInCell* const pCmd = dynamic_cast<const ccAddBeadsInCell*>(pCommand);

	// Data needed to define an aggregate. Note that we don't need the slice normal here.

	const zString label = pCmd->GetLabel();		//	Label of the aggregate
	long xCellIndex = pCmd->GetXCellIndex();
	long yCellIndex = pCmd->GetYCellIndex();	// Cell coordinates
	long zCellIndex = pCmd->GetZCellIndex();

	const StringSequence beadNames = pCmd->GetNames();	// Names of bead types to collect

	// Find the integer types of selected beads from their name strings

	zLongVector beadTypes;

	for(cStringSequenceIterator iterName=beadNames.begin(); iterName!=beadNames.end(); iterName++)
	{
		long type = GetBeadTypeFromName(*iterName);

		beadTypes.push_back(type);
	}

	// Find the CNT cell specified by the indices and add its beads to the target

	long index = m_CNTXCellNo*(m_CNTYCellNo*zCellIndex + yCellIndex) + xCellIndex;

	BeadList cellBeads = m_vCNTCells.at(index)->GetBeads();

	BeadVector aggregateBeads;

	aggregateBeads.clear();

	for(cBeadListIterator iterBead=cellBeads.begin(); iterBead!=cellBeads.end(); iterBead++)
	{
		czLongVectorIterator iBeadType = find(beadTypes.begin(), beadTypes.end(), (*iterBead)->GetType());

		if(iBeadType != beadTypes.end())
		{
			CBead* pBead = dynamic_cast<CBead*>(*iterBead);
			if(pBead)
				aggregateBeads.push_back(pBead);
		}
	}

	// Find the CForceTarget object to add the beads to. The target 
	// stays on the m_ForceTarget list until a command is received to delete 
	// it or the SimBox destructor is called. We cannot use find() here because
	// we need to search on the label property of CForceTarget objects.
	// Note that the command target name has already been validated in CInputData,
	// that is, a target with the correct name is known to exist.

	for(cForceTargetIterator iterTarget=m_ForceTargets.begin(); iterTarget!=m_ForceTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetLabel() == label)
		{
			(*iterTarget)->AddBeads(aggregateBeads);
			(*iterTarget)->RemoveDuplicateBeads();
		}
	}

#endif
}

void CSimBox::AddPolymersInSlice(const xxCommand *const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled
//	const ccAddPolymersInSlice* const pCmd = dynamic_cast<const ccAddPolymersInSlice*>(pCommand);
#endif
}

void CSimBox::AddPolymersInRow(const xxCommand *const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled
//	const ccAddPolymersInRow* const pCmd = dynamic_cast<const ccAddPolymersInRow*>(pCommand);
#endif
}

void CSimBox::AddPolymersInCell(const xxCommand *const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled
//	const ccAddPolymersInCell* const pCmd = dynamic_cast<const ccAddPolymersInCell*>(pCommand);
#endif
}

// Function to implement a selection command that collects polymers of the 
// specified types throughout the SimBox and moves them so that their heads
// lie within a specified CNT slice and their bodies are randomly distributed
// throughout that slice and its neighbours.
//
// They are packaged into a CForceTarget aggregate that can receive ccApplyForce 
// commands if required.
//
// We assume that the data has been validated prior to this routine.

void CSimBox::MovePolymersToSlice(const xxCommand* const pCommand)
{
#if EnableMiscClasses == SimMiscEnabled

	const ccMovePolymersToSlice* const pCmd = dynamic_cast<const ccMovePolymersToSlice*>(pCommand);

	// Data needed to define an aggregate

	const zString label = pCmd->GetLabel();	//	Label of the aggregate
	long xNormal = pCmd->GetX();
	long yNormal = pCmd->GetY();			//	Slice normal
	long zNormal = pCmd->GetZ();
	long sliceIndex = pCmd->GetSliceIndex();

	// Get the extent of the slice where the polymers are being moved to.
	// We have to take account of any walls when moving the polymers to the
	// new slice. We also position the beads in the boundary slices slightly 
	// inside SimBox so as to avoid any overlaps with the pbcs.


	double xMin, yMin, zMin, xWidth, yWidth, zWidth;
    xMin   = 0.0;
    yMin   = 0.0;
    zMin   = 0.0;
    xWidth = 0.0;
    yWidth = 0.0;
    zWidth = 0.0;

	if(xNormal == 1)
	{
		xMin = static_cast<double>(sliceIndex)*m_CNTXCellWidth;
		xWidth = m_CNTXCellWidth;

		if(xMin == 0.0)	// first slice in box 
		{
			xMin += 0.1;
		}
		else if(sliceIndex == m_CNTXCellNo-1)	// last slice in box
		{
			xWidth -= 0.1;
		}

		yMin   = GetWallYWidth();
		yWidth = GetSimBoxBulkYLength();

		if(yMin == 0.0)
		{
			yMin += 0.1;
			yWidth -= 0.1;
		}

		zMin   = GetWallZWidth();
		zWidth = GetSimBoxBulkZLength();

		if(zMin == 0.0)
		{
			zMin += 0.1;
			zWidth -= 0.1;
		}
	}
	else if(yNormal == 1)
	{
		yMin = static_cast<double>(sliceIndex)*m_CNTYCellWidth;
		yWidth = m_CNTYCellWidth;

		if(yMin == 0.0)	// first slice in box 
		{
			yMin += 0.1;
		}
		else if(sliceIndex == GetCNTYCellNo()-1)	// last slice in box
		{
			yWidth -= 0.1;
		}

		xMin   = GetWallXWidth();
		xWidth = GetSimBoxBulkXLength();

		if(xMin == 0.0)
		{
			xMin += 0.1;
			xWidth -= 0.1;
		}

		zMin   = GetWallZWidth();
		zWidth = GetSimBoxBulkZLength();

		if(zMin == 0.0)
		{
			zMin += 0.1;
			zWidth -= 0.1;
		}

	}
	else if(zNormal == 1)
	{
		zMin = static_cast<double>(sliceIndex)*m_CNTZCellWidth;
		zWidth = m_CNTZCellWidth;

		if(zMin == 0.0)	// first slice in box 
		{
			zMin += 0.1;
		}
		else if(sliceIndex == GetCNTZCellNo()-1)	// last slice in box
		{
			zWidth -= 0.1;
		}

		xMin   = GetWallXWidth();
		xWidth = GetSimBoxBulkXLength();

		if(xMin == 0.0)
		{
			xMin += 0.1;
			xWidth -= 0.1;
		}

		yMin   = GetWallYWidth();
		yWidth = GetSimBoxBulkYLength();

		if(yMin == 0.0)
		{
			yMin += 0.1;
			yWidth -= 0.1;
		}
	}

	const StringSequence polyNames = pCmd->GetNames();	// Names of polymer types to collect

	// Find the integer types of selected beads from their name strings

	zLongVector polyTypes;

	for(cStringSequenceIterator iterName=polyNames.begin(); iterName!=polyNames.end(); iterName++)
	{
		long type = GetPolymerTypeFromName(*iterName);

		polyTypes.push_back(type);
	}

	// Collect all beads in polymers of the specified types

	BeadVector targetBeads;
	BondVector targetBonds;

	for(zLongVectorIterator izPoly=polyTypes.begin(); izPoly!= polyTypes.end(); izPoly++)
	{
		cPolymerVectorIterator firstPoly = IModifyPolymers()->GetPolymers().begin();
		cPolymerVectorIterator lastPoly  = IModifyPolymers()->GetPolymers().end();

		for(cPolymerVectorIterator iterPoly=firstPoly; iterPoly!=lastPoly; iterPoly++)
		{
			if(find(polyTypes.begin(), polyTypes.end(), (*iterPoly)->GetType()) != polyTypes.end())
			{
				copy((*iterPoly)->GetBeads().begin(), (*iterPoly)->GetBeads().end(), back_inserter(targetBeads));
				copy((*iterPoly)->GetBonds().begin(), (*iterPoly)->GetBonds().end(), back_inserter(targetBonds));
			}
		}
	}

	// Remove the beads from their original CNT cells but wait until after the
	// bond lengths have been checked before adding to the new cells in the target
	// slice.

	double newX, newY, newZ;

	for(BeadVectorIterator iterBead = targetBeads.begin(); iterBead != targetBeads.end(); iterBead++)
	{
		long ix = static_cast<long>((*iterBead)->GetXPos()/m_CNTXCellWidth);
		long iy = static_cast<long>((*iterBead)->GetYPos()/m_CNTYCellWidth);

#if SimDimension == 2
		long iz = 0;
#elif SimDimension == 3
		long iz = static_cast<long>((*iterBead)->GetZPos()/m_CNTZCellWidth);		
#endif

		long index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;
		m_vCNTCells[index]->RemoveBeadFromCell(*iterBead);

		newX = xMin + xWidth*CCNTCell::GetRandomNo();
		newY = yMin + yWidth*CCNTCell::GetRandomNo();
		newZ = zMin + zWidth*CCNTCell::GetRandomNo();

		(*iterBead)->SetXPos(newX);
		(*iterBead)->SetYPos(newY);
		(*iterBead)->SetZPos(newZ);
	}

	// Loop over all bonds in the polymers adjusting those beads that are too far 
	// apart. Because we are moving all the beads in a polymer we know that the
	// bonds do not cross the periodic boundaries and so the distance between
	// the head and tail beads is the actual bond length.
	// We set the maximum bond length to the actual unstretched length, or 1
	// if the bond has zero length.

	double bondLength = 0.0;

	for(BondVectorIterator iterBond = targetBonds.begin(); iterBond != targetBonds.end(); iterBond++)
	{
		double xDist = (*iterBond)->GetTail()->GetXPos() - (*iterBond)->GetHead()->GetXPos();
		double yDist = (*iterBond)->GetTail()->GetYPos() - (*iterBond)->GetHead()->GetYPos();

		bondLength = xDist*xDist + yDist*yDist;

#if SimDimension == 3
		double zDist = (*iterBond)->GetTail()->GetZPos() - (*iterBond)->GetHead()->GetZPos();
		bondLength += zDist*zDist;
#endif
	
		bondLength = sqrt(bondLength);

		double maxBondLength = (*iterBond)->GetUnStrLength();

		if(maxBondLength < 1.0)
			maxBondLength = 1.0;

		if(bondLength > maxBondLength)
		{
			newX = (*iterBond)->GetHead()->GetXPos() + maxBondLength*xDist/bondLength;
			newY = (*iterBond)->GetHead()->GetYPos() + maxBondLength*yDist/bondLength;
			newZ = (*iterBond)->GetHead()->GetZPos() + maxBondLength*zDist/bondLength;

			(*iterBond)->GetTail()->SetXPos(newX);
			(*iterBond)->GetTail()->SetYPos(newY);
			(*iterBond)->GetTail()->SetZPos(newZ);
		}
	}

	// Add the beads to their new CNT cells within the target slice. Note that
	// we cannot do this in the bond loop above because polymers consisting of
	// single beads will not trigger the loop

	for(cBeadVectorIterator citerBead=targetBeads.begin(); citerBead!=targetBeads.end(); citerBead++)
	{
		long ix = static_cast<long>((*citerBead)->GetXPos()/m_CNTXCellWidth);
		long iy = static_cast<long>((*citerBead)->GetYPos()/m_CNTYCellWidth);

#if SimDimension == 2
		long iz = 0;
#elif SimDimension == 3
		long iz = static_cast<long>((*citerBead)->GetZPos()/m_CNTZCellWidth);		
#endif

		long index = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;
		m_vCNTCells[index]->AddBeadtoCell(*citerBead);
	}

	// Create a CForceTarget object to hold the labelled bead aggregate ready to
	// receive commands to apply a force. The target stays on the m_ForceTarget 
	// list until a command is received to delete it or the SimBox destructor is
	// called.

	CForceTarget* pTarget = new CForceTarget(label, targetBeads);

	m_ForceTargets.push_back(pTarget);

#endif
}

// Function to run an MC simulation prior to starting the main MD segment of 
// the code to relax the initial configuration of beads so that the Lennard-Jones 
// potentials do not lead to huge forces. It can be used when the initial state
// is random or an ordered distribution of polymers (e.g., a bilayer) although it 
// may not be necessary in the latter case if the particles are known to be 
// distributed such that there are no pairs separated by less than approximately 
// 0.8*sigma00.
//
// We need data to define the MC simulation fro the control data file.
//
// m_MaxMCStep		Maximum displacement of a bead in one MC step (assumed isotropic)
// m_TotalMCTime	Total number of MC steps to perform
//
// Note that if the number of MC steps is zero the equilibration is not performed
// but the assignment of various bead properties (momenta and forces) is still
// carried out at the end of this routine. If the number of MC steps is non-zero, 
// the acceptance rate of the moves is output to the log file via the
// LogMCAcceptanceRate message.

void CSimBox::RunMCRelaxation()
{
	long  AcceptCount = 0;

	for(long MCSimTime=1; MCSimTime<=m_TotalMCTime; MCSimTime++)
	{
		AcceptCount+= MCPolymerRelaxation(m_vAllPolymers);

		// Now do the same for the wall polymers (if any).
		// Note that if no wall is present the function should return false.

		if(IsWallPolymerFlexible())
		{
			AcceptCount+= MCPolymerRelaxation(m_vWallPolymers);
		}
	}

	// Normalise the acceptance counter if m_TotalMCTime is non-zero. To avoid
	// the divisor exceeding the long integer limit we first divide by the total
	// number of beads then divide the total MC time into the result. As we only 
	// do this calculation once per simulation, we can afford two divide operations.

	if(m_TotalMCTime > 0)
	{
		const double AcceptFraction = (static_cast<double>(AcceptCount)/static_cast<double>(m_BeadTotal))/static_cast<double>(m_TotalMCTime);

		new CLogMCAcceptanceRate(m_SimTime, AcceptFraction);	
	}

	// Set the particle forces and momenta prior to starting the MD code. 
	// This ensures that the first call to UpdatePos() will use sensible values. 
	// We don't set anything for the wall beads as their positions are fixed and
	// their momenta and forces are always zero. However, the movable beads in 
	// polymers whose heads are fixed in the wall are included in the vector
	// constructed from all beads in CNT cells and so will have their properties set.
	//
	// We use the following initialisation scheme:
	//
	// 1	Store the positions generated by the MC equilibration in the
	//		m_unPBCPos[] and m_InitialPos[] arrays for use in calculating the
	//		diffusion coefficients. Zero m_Force[] and m_oldForce[] even though 
	//		they should be zero already. Store the momenta originally taken from 
	//		a Maxwell distribution in the m_oldMom[] array so that the new values 
	//		can be generated from them.
	//
	// 2	Given the positions generated by the MC equilibration call UpdateForce() 
	//		to calculate the force on each particle and add the bond and stiff bond
	//		forces for each bead.
	//
	// 3	Call UpdateMom() to generate new momenta out of the old ones and the 
	//		newly-calculated forces.
	//
	// The diffusion coefficients are calculated from the beginning of the MD stage
	// not the MC equilibration because the positions are not generated using
	// Newton's laws during MC.

    AbstractBeadVector vAllBeads = GetAllBeadsInCNTCells();
	for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		(*iterBead)->SetunPBCXPos((*iterBead)->GetXPos());
		(*iterBead)->SetunPBCYPos((*iterBead)->GetYPos());
		(*iterBead)->SetunPBCZPos((*iterBead)->GetZPos());

		(*iterBead)->SetInitialXPos((*iterBead)->GetXPos());
		(*iterBead)->SetInitialYPos((*iterBead)->GetYPos());
		(*iterBead)->SetInitialZPos((*iterBead)->GetZPos());

		(*iterBead)->SetXForce(0.0);
		(*iterBead)->SetYForce(0.0);
		(*iterBead)->SetZForce(0.0);

		(*iterBead)->m_oldForce[0] = (*iterBead)->GetXForce();
		(*iterBead)->m_oldForce[1] = (*iterBead)->GetYForce();
		(*iterBead)->m_oldForce[2] = (*iterBead)->GetZForce();

		(*iterBead)->m_oldMom[0] = (*iterBead)->GetXMom();
		(*iterBead)->m_oldMom[1] = (*iterBead)->GetYMom();
		(*iterBead)->m_oldMom[2] = (*iterBead)->GetZMom();
	}

	CNTCellIterator iterCell;

	for(iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdateForce();
	} 

	// Add in the forces between bonded beads and the stiff bond force. Note that
	// AddBondPairForces() must be called after AddBondForces() because it relies
	// on the bond lengths having already been calculated in CBond::AddForce().

	AddBondForces();
	AddBondPairForces();

	for(iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
		(*iterCell)->UpdateMom();
	} 
}

// Function to turn on or off renormalisation of the particle momenta during 
// an MD simulation. We don't actually prevent it from being used in a DPD
// run but it does not make much sense. We set the CSimState's flag showing that
// the renormalisation is on or off and copy the period locally to the CSimBox.
//

void CSimBox::RenormaliseMomenta(const xxCommand* const pCommand)
{
	const ccRenormaliseMomenta* const pCmd = dynamic_cast<const ccRenormaliseMomenta*>(pCommand);

	if(pCmd->ApplyRenormalisation())
	{
		SetRenormaliseMomenta(true);
		m_RenormalisationPeriod = pCmd->GetPeriod();
	}
	else
	{
		SetRenormaliseMomenta(false);
	}
}

// Function to renormalise all bead momenta to the specified temperature.
// Only the beads that result from a call to GetAllBeadsInCNTCells() are used 
// because the wall beads should have zero momenta at all times.

void CSimBox::UpdateRenormalisedMom()
{
	double magVSum = 0.0;

    AbstractBeadVector vAllBeads = GetAllBeadsInCNTCells();

    AbstractBeadVectorIterator iterBead; // Used in two loops
	for(iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		magVSum += (*iterBead)->GetXMom()*(*iterBead)->GetXMom();
		magVSum += (*iterBead)->GetYMom()*(*iterBead)->GetYMom();

#if SimDimension == 3
		magVSum += (*iterBead)->GetZMom()*(*iterBead)->GetZMom();
#endif
	}

	// Divide all momenta components by the expected value. Note that this 
	// depends on the dimension of the simulation
	//
	// Norm = d*N*kT/m
	//
	// but the mass of all beads is unity.

#if SimDimension == 2
	magVSum = sqrt(magVSum/(2.0*m_BeadTotal*GetkT()));
#elif SimDimension == 3
	magVSum = sqrt(magVSum/(3.0*m_BeadTotal*GetkT()));
#endif


	for(iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
	{
		(*iterBead)->SetXMom((*iterBead)->GetXMom()/magVSum);
		(*iterBead)->SetYMom((*iterBead)->GetYMom()/magVSum);
		(*iterBead)->SetZMom((*iterBead)->GetZMom()/magVSum);
	}
}

// Function to change the DPD conservative interaction parameter of a 
// specified pair of CBead objects in the simulation. This command
// updates the matrix of interaction parameters held in the CCNTCell class and 
// the value stored in the CInitialState instance. But it does not modify the
// CInputData object: that continues to hold the data as it was specified at the 
// start of the simulation.
// Note that the names of the bead types are not assumed to have been validated
// in the command as we want to be able to apply this command to dynamically
// created bead types whose names did not exist when the simulation began.
// Hence, we have to validate the bead types here.

void CSimBox::SetDPDBeadConsInt(const xxCommand *const pCommand)
{
	const ccSetDPDBeadConsInt* const pCmd = dynamic_cast<const ccSetDPDBeadConsInt*>(pCommand);

	// Unpack the data identifying the particular beads that are changed by
	// the command and the new value of their conservative interaction parameter.

	const zString firstName		= pCmd->GetFirstName();
	const zString secondName	= pCmd->GetSecondName();
	const double  consInt		= pCmd->GetConsInt();

	const long firstType  = GetBeadTypeFromName(firstName);
	const long secondType = GetBeadTypeFromName(secondName);

	// Check that the bead types exist, if not log an error message

	if(0 <= firstType  && firstType  < GetBeadTypeTotal() &&
	   0 <= secondType && secondType < GetBeadTypeTotal())
	{
		IModifyBeadTypes()->SetDPDConsInt(firstType, secondType, consInt);

		// Log a message indicating that a bead-bead conservative parameter has changed

		new CLogDPDBeadConsIntChanged(m_SimTime,firstName, secondName, firstType, secondType, consInt);
	}
	else	// Log a command failed message
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}
}

// Function to change the DPD conservative interaction parameter of a 
// specified pair of CBead objects in the simulation. It is a copy of the
// ccSetDPDBeadConsInt function but takes the beads' numeric types as the
// identifier instead of their names.
//
// This command updates the matrix of interaction parameters held in the 
// CCNTCell class and the value stored in the CInitialState instance. But it does 
// not modify the CInputData object: that continues to hold the data as it was 
// specified at the start of the simulation.
// Note that the numeric types of the beads are not assumed to have been validated
// in the command as we want to be able to apply this command to dynamically
// created bead types whose names did not exist when the simulation began.
// Hence, we have to validate the bead types here.

void CSimBox::SetDPDBeadConsIntByType(const xxCommand *const pCommand)
{
	const ccSetDPDBeadConsIntByType* const pCmd = dynamic_cast<const ccSetDPDBeadConsIntByType*>(pCommand);

	// Unpack the data identifying the particular beads that are changed by
	// the command and the new value of their conservative interaction parameter.

	const long firstType	= pCmd->GetFirstType();
	const long secondType	= pCmd->GetSecondType();
	const double  consInt	= pCmd->GetConsInt();

	// Check that the bead types exist, if not log an error message

	if(0 <= firstType  && firstType  < GetBeadTypeTotal() &&
	   0 <= secondType && secondType < GetBeadTypeTotal())
	{
		IModifyBeadTypes()->SetDPDConsInt(firstType, secondType, consInt);

		// Log a message indicating that a bead-bead conservative parameter 
		// has changed. We obtain the bead names from the CAnalysisState map: 
		// non-existent bead types are returned with the name "Unnamed".

		const zString firstName  = GetBeadNameFromType(firstType);
		const zString secondName = GetBeadNameFromType(secondType);

#if EnableParallelCommands == SimMPSEnabled
        // We only perform parallel processing here if the EnableParallelCommands
        // flag is set. 

        if(xxParallelBase::GlobalGetRank() == 0)
		{
		    new CLogDPDBeadConsIntChanged(m_SimTime, firstName, secondName, firstType, secondType, consInt);
		}
	}
	else
	{
        if(xxParallelBase::GlobalGetRank() == 0)
		{
			new CLogCommandFailed(m_SimTime, pCmd);
		}
	}
#else
		// Log a message indicating that a bead-bead conservative parameter 
		// has changed. We obtain the bead names from the CAnalysisState map: 
		// non-existent bead types are returned with the name "Unnamed".

		new CLogDPDBeadConsIntChanged(m_SimTime, firstName, secondName, firstType, secondType, consInt);
	}
	else	// Log a command failed message
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}
#endif
}

// Function to change the DPD dissipative interaction parameter of a 
// specified pair of CBead objects in the simulation. This command
// updates the matrix of interaction parameters held in the CCNTCell class and 
// the value stored in the CInitialState instance. But it does not modify the
// CInputData object: that continues to hold the data as it was specified at the 
// start of the simulation.
// Note that the names of the bead types are not assumed to have been validated
// in the command as we want to be able to apply this command to dynamically
// created bead types whose names did not exist when the simulation began.
// Hence, we have to validate the bead types here.

void CSimBox::SetDPDBeadDissInt(const xxCommand *const pCommand)
{
	const ccSetDPDBeadDissInt* const pCmd = dynamic_cast<const ccSetDPDBeadDissInt*>(pCommand);

	// Unpack the data identifying the particular beads that are changed by
	// the command and the new value of their dissipative interaction parameter.

	const zString firstName		= pCmd->GetFirstName();
	const zString secondName	= pCmd->GetSecondName();
	const double  dissInt		= pCmd->GetDissInt();

	const long firstType  = GetBeadTypeFromName(firstName);
	const long secondType = GetBeadTypeFromName(secondName);

	// Check that the bead types exist, if not log an error message

	if(0 <= firstType  && firstType  < GetBeadTypeTotal() &&
	   0 <= secondType && secondType < GetBeadTypeTotal())
	{
		IModifyBeadTypes()->SetDPDDissInt(firstType, secondType, dissInt);

		// Log a message indicating that a bead-bead dissipative parameter has changed

		new CLogDPDBeadDissIntChanged(m_SimTime, firstName, secondName, firstType, secondType, dissInt);
	}
	else	// Log a command failed message
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}
}

// Function to change the DPD dissipative interaction parameter of a 
// specified pair of CBead objects in the simulation. It is a copy of the
// ccSetDPDBeadDissInt function but takes the beads' numeric types as the
// identifier instead of their names.
//
// This command updates the matrix of interaction parameters held in the 
// CCNTCell class and the value stored in the CInitialState instance. But it does 
// not modify the CInputData object: that continues to hold the data as it was 
// specified at the start of the simulation.
// Note that the numeric types of the beads are not assumed to have been validated
// in the command as we want to be able to apply this command to dynamically
// created bead types whose names did not exist when the simulation began.
// Hence, we have to validate the bead types here.

void CSimBox::SetDPDBeadDissIntByType(const xxCommand *const pCommand)
{
	const ccSetDPDBeadDissIntByType* const pCmd = dynamic_cast<const ccSetDPDBeadDissIntByType*>(pCommand);

	// Unpack the data identifying the particular beads that are changed by
	// the command and the new value of their dissipative interaction parameter.

	const long firstType	= pCmd->GetFirstType();
	const long secondType	= pCmd->GetSecondType();
	const double  dissInt	= pCmd->GetDissInt();

	// Check that the bead types exist, if not log an error message

	if(0 <= firstType  && firstType  < GetBeadTypeTotal() &&
	   0 <= secondType && secondType < GetBeadTypeTotal())
	{
		IModifyBeadTypes()->SetDPDDissInt(firstType, secondType, dissInt);

		// Log a message indicating that a bead-bead dissipative parameter 
		// has changed. We obtain the bead names from the CAnalysisState map: 
		// non-existent bead types are returned with the name "Unnamed".

		const zString firstName  = GetBeadNameFromType(firstType);
		const zString secondName = GetBeadNameFromType(secondType);

#if EnableParallelCommands == SimMPSEnabled
        // We only perform parallel processing here if the EnableParallelCommands
        // flag is set. 

        if(xxParallelBase::GlobalGetRank() == 0)
		{
			new CLogDPDBeadDissIntChanged(m_SimTime,firstName, secondName, firstType, secondType, dissInt);
		}
	}
	else
	{
        if(xxParallelBase::GlobalGetRank() == 0)
		{
			new CLogCommandFailed(m_SimTime, pCmd);
		}
	}
#else
		// Log a message indicating that a bead-bead conservative parameter 
		// has changed. We obtain the bead names from the CAnalysisState map: 
		// non-existent bead types are returned with the name "Unnamed".

		new CLogDPDBeadDissIntChanged(m_SimTime, 
													  firstName, secondName, 
													  firstType, secondType, dissInt);
	}
	else	// Log a command failed message
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}
#endif
}

// Function to change the parameters of a specified type of CBondPair object in 
// the simulation.
//
// The vector of all bondpairs is iterated over and those whose type matches
// that specified in the command have their parameters set to the new values.
// If the simulation is MD we apply the normalisation before setting the values.

void CSimBox::SetBondStiffness(const xxCommand* const pCommand)
{
	const ccSetBondStiffness* const pCmd = dynamic_cast<const ccSetBondStiffness*>(pCommand);

	// Unpack the data identifying the particular bondpair that is changed by
	// the command and the new values of its bending modulues and preferred angle.
	// If the simulation type is MD we normalise the bending modulus by the
	// energy scale taken from the first species' LJ potential depth.

	const zString name	= pCmd->GetName();
	double modulus		= pCmd->GetBendingModulus();
	double phi0			= pCmd->GetPhi0();

	const long type	 = GetBondPairTypeFromName(name);

	// Check that the stiff bond type exists; if it does not we ignore the command 
	// and post a command failed message.

	if(0 <= type && type < GetBondPairTypeTotal())
	{

#if SimIdentifier == MD
		modulus /= GetEnergyScale();
#endif
		// Loop over all polymers changing the properties of those bondpairs
		// targetted by the command

		for(PolymerVectorIterator iterPoly=m_vAllPolymers.begin(); iterPoly!=m_vAllPolymers.end(); iterPoly++)
		{
			BondPairVector vBondPairs = (*iterPoly)->GetBondPairs();

			for(BondPairVectorIterator iterBP=vBondPairs.begin(); iterBP!=vBondPairs.end(); iterBP++)
			{
				if((*iterBP)->GetType() == type)
				{
					(*iterBP)->SetModulus(modulus);
					(*iterBP)->SetPhi0(phi0);
				}
			}
		}

		// Create a message indicating that the bondpair parameters have changed

		new CLogBondStiffnessChanged(m_SimTime, name, type, modulus, phi0);
	}
	else	// Log a command failed message
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}
}

// Function to change the parameters of a specified type of CBond object in 
// the simulation.
//
// The vector of all bonds is iterated over and those whose type matches
// that specified in the command have their parameters set to the new values.
// If the simulation is MD we apply the normalisation before setting the values.

void CSimBox::SetBondStrength(const xxCommand* const pCommand)
{
	const ccSetBondStrength* const pCmd = dynamic_cast<const ccSetBondStrength*>(pCommand);

	// Unpack the data identifying the particular bond that is changed by
	// the command and the new values of its spring constant and unstretched length.
	// If the simulation type is MD we normalise the spring constant by the
	// energy scale taken from the first species' LJ potential depth, and the lengths
	// by the first LJ potential range.

	const zString name		= pCmd->GetName();
	double springConstant	= pCmd->GetSpringConstant();
	double unStrLength		= pCmd->GetUnstretchedLength();

	const long type	= GetBondTypeFromName(name);

	// Check that the bond type exists; if it does not we ignore the command 
	// and post a command failed message.

	if(0 <= type && type < GetBondTypeTotal())
	{

#if SimIdentifier == MD
		springConstant *= (GetLengthScale()*GetLengthScale()/GetEnergyScale());
		unStrLength    /= GetLengthScale();
#endif
		// Loop over all polymers changing the properties of those bonds that are
		// targetted by the command

		for(PolymerVectorIterator iterPoly=m_vAllPolymers.begin(); iterPoly!=m_vAllPolymers.end(); iterPoly++)
		{
			BondVector vBonds = (*iterPoly)->GetBonds();

			for(BondVectorIterator iterBond=vBonds.begin(); iterBond!=vBonds.end(); iterBond++)
			{
				if((*iterBond)->GetType() == type)
				{
					(*iterBond)->SetSpringConstant(springConstant);
					(*iterBond)->SetUnStretchedLength(unStrLength);
				}
			}
		}

		// Create a message indicating that the bond parameters have changed

		new CLogBondStrengthChanged(m_SimTime, name, type, springConstant, unStrLength);
	}
	else	// Log a command failed message
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}
}

// Function to toggle the addition of the bead-bead contributions to the stress
// tensor on and off. 
//
// No data is required for this command, it just toggles a flag.

void CSimBox::ToggleBeadStressContribution(const xxCommand* const pCommand)
{
//	const ccToggleBeadStressContribution* const pCmd = dynamic_cast<const ccToggleBeadStressContribution*>(pCommand);

	if(IsBeadStressAdded())
	{
		SetBeadStressContributionOn(false);

		new CLogStressContribution(m_SimTime, "Bead", false);
	}
	else
	{
		SetBeadStressContributionOn(true);

		new CLogStressContribution(m_SimTime, "Bead", true);
	}

}

// Function to toggle the addition of the bond contributions to the stress
// tensor on and off. 
//
// No data is required for this command, it just toggles a flag.

void CSimBox::ToggleBondStressContribution(const xxCommand* const pCommand)
{
//	const ccToggleBondStressContribution* const pCmd = dynamic_cast<const ccToggleBondStressContribution*>(pCommand);

	if(IsBondStressAdded())
	{
		SetBondStressContributionOn(false);

		new CLogStressContribution(m_SimTime, "Bond", false);
	}
	else
	{
		SetBondStressContributionOn(true);

		new CLogStressContribution(m_SimTime, "Bond", true);
	}

}

// Function to toggle the addition of the bondpair contributions to the stress
// tensor on and off. 
//
// No data is required for this command, it just toggles a flag.

void CSimBox::ToggleBondPairStressContribution(const xxCommand* const pCommand)
{
//	const ccToggleBondPairStressContribution* const pCmd = dynamic_cast<const ccToggleBondPairStressContribution*>(pCommand);

	if(IsBondPairStressAdded())
	{
		SetBondPairStressContributionOn(false);

		new CLogStressContribution(m_SimTime, "BondPair", false);
	}
	else
	{
		SetBondPairStressContributionOn(true);

		new CLogStressContribution(m_SimTime, "BondPair", true);
	}

}

// Function to us a Metropolis algorithm to relax the energy of all the beads
// in a vector of polymers.
//
// This is called separately for the bulk SimBox polymers and the Wall polymers
// because the two types are stored in distinct vectors. Each bead in each 
// polymer is moved by a small 
// random amount and the change in bead-bead interaction energy, bond stretching
// energy and stiff bond bending energy calculated. The move is accepted or
// reject using the standard Metropolis algorithm. We use one trick: if a bead
// move is accepted then the final PE of the bonds in a polymer replaces the 
// initial bond PE for the next bead move, whilst if the move is rejected the 
// inital PE is left unchanged. This saves having to calculate the total bond 
// energy of a polymer twice. We cannot do this for the beads in a polymer 
// because we move one at a time and only calculate the interaction energy of 
// that bead. We calculate the energy of all bonds in a polymer because it is 
// easier than having to store information on which bonds a given bead is in.
//
// The function returns the fraction of accepted moves.

long CSimBox::MCPolymerRelaxation(PolymerVector& rPolymers)
{
	const double TwoMaxMCStep = 2.0*m_MaxMCStep;
	const double Beta		  = 1.0/GetkT();

	// Run the MC segment to find an "equilibrated" configuration.
	// We only have to update the positions here as the momenta and forces
	// are not used in MC.

	double oldPos[3];
	double newPos[3];
	double deltaPos[3];
	double oldunPBCPos[3];
	double initialBondPE	= 0.0;
	double finalBondPE		= 0.0;
	double initialPE		= 0.0;
	double finalPE			= 0.0;
	double deltaPE			= 0.0;

	long  AcceptCount		= 0;

	for(PolymerVectorIterator iterPoly=rPolymers.begin(); iterPoly!=rPolymers.end(); iterPoly++)
	{
		initialBondPE = (*iterPoly)->AddPotentialEnergy();

		for(BeadVectorIterator iterBead=(*iterPoly)->GetBeads().begin(); iterBead!=(*iterPoly)->GetBeads().end(); iterBead++)
		{
			// Store the original position in case the move is rejected, and the unPBC position
			// for the bond PE calculation

			oldPos[0] = (*iterBead)->GetXPos();
			oldPos[1] = (*iterBead)->GetYPos();
			oldPos[2] = (*iterBead)->GetZPos();

			oldunPBCPos[0] = (*iterBead)->GetunPBCXPos();
			oldunPBCPos[1] = (*iterBead)->GetunPBCYPos();
			oldunPBCPos[2] = (*iterBead)->GetunPBCZPos();

			// Find the cell containing the bead and remove it from the 
			// cell before calculating its potential energy to prevent 
			// any self-interactions.

			const long ixi = static_cast<long>(oldPos[0]/m_CNTXCellWidth);
			const long iyi = static_cast<long>(oldPos[1]/m_CNTYCellWidth);

	#if SimDimension == 2
			const long izi = 0;
	#elif SimDimension == 3
			const long izi = static_cast<long>(oldPos[2]/m_CNTZCellWidth);		
	#endif
			CCNTCell* const pInitialCell = m_vCNTCells.at(m_CNTXCellNo*(m_CNTYCellNo*izi+iyi) + ixi);
			pInitialCell->RemoveBeadFromCell(*iterBead);

			// Calculate the potential energy due to the bead-bead interactions,
			// and the the bond stretching and bending energy for all bonds in
			// the polymer. We calculate the total bond energy of the polymer
			// even though only one bead is moved at a time because the logic
			// is much simpler.

			initialPE  = pInitialCell->GetPotentialEnergy(*iterBead);
			initialPE += initialBondPE;

			// Generate a random change in position, find the new cell containing 
			// the bead and calculate its potential energy there. We don't add
			// the bead to the cell yet because the potential energy routine 
			// expects the bead not be in the cell.

			deltaPos[0] = TwoMaxMCStep*(0.5 - CCNTCell::GetRandomNo());
			deltaPos[1] = TwoMaxMCStep*(0.5 - CCNTCell::GetRandomNo());
			deltaPos[2] = TwoMaxMCStep*(0.5 - CCNTCell::GetRandomNo());

			newPos[0] = oldPos[0] + deltaPos[0];
			newPos[1] = oldPos[1] + deltaPos[1];
			newPos[2] = oldPos[2] + deltaPos[2];

			if( newPos[0] > GetSimBoxXLength() )
				newPos[0] -= GetSimBoxXLength();
			else if( newPos[0] < 0.0 )
				newPos[0] += GetSimBoxXLength();

			if( newPos[1] > GetSimBoxYLength() )
				newPos[1] -= GetSimBoxYLength();
			else if( newPos[1] < 0.0 )
				newPos[1] += GetSimBoxYLength();

			if( newPos[2] > GetSimBoxZLength() )
				newPos[2] -= GetSimBoxZLength();
			else if( newPos[2] < 0.0 )
				newPos[2] += GetSimBoxZLength();

			// Set the bead's new coordinates and unPBC coordinates

			(*iterBead)->SetXPos(newPos[0]);
			(*iterBead)->SetYPos(newPos[1]);	
			(*iterBead)->SetZPos(newPos[2]);

			(*iterBead)->SetunPBCXPos(oldunPBCPos[0]+deltaPos[0]);
			(*iterBead)->SetunPBCYPos(oldunPBCPos[1]+deltaPos[1]);	
			(*iterBead)->SetunPBCZPos(oldunPBCPos[2]+deltaPos[2]);

			const long ixf = static_cast<long>(newPos[0]/m_CNTXCellWidth);
			const long iyf = static_cast<long>(newPos[1]/m_CNTYCellWidth);

#if SimDimension == 2
			const long izf = 0;
#elif SimDimension == 3
			const long izf = static_cast<long>(newPos[2]/m_CNTZCellWidth);		
#endif
			CCNTCell* const pFinalCell	= m_vCNTCells.at(m_CNTXCellNo*(m_CNTYCellNo*izf+iyf) + ixf);

			finalPE			= pFinalCell->GetPotentialEnergy(*iterBead);
			finalBondPE		= (*iterPoly)->AddPotentialEnergy();
			finalPE		   += finalBondPE;

			deltaPE	 = Beta*(finalPE - initialPE);

			if(deltaPE > 0.0 && 1.0/exp(deltaPE) < CCNTCell::GetRandomNo())
			{
				(*iterBead)->SetXPos(oldPos[0]);			// Restore the bead to its old position
				(*iterBead)->SetYPos(oldPos[1]);
				(*iterBead)->SetZPos(oldPos[2]);

				(*iterBead)->SetunPBCXPos(oldunPBCPos[0]);	// and its unPBC coordinates
				(*iterBead)->SetunPBCYPos(oldunPBCPos[1]);
				(*iterBead)->SetunPBCZPos(oldunPBCPos[2]);
				pInitialCell->AddBeadtoCell(*iterBead);
			}
			else
			{
				AcceptCount++;							// Increment the acceptance count
				pFinalCell->AddBeadtoCell(*iterBead);	// Add bead to the new cell
				initialBondPE = finalBondPE;			// Replace initial PE with final PE
			}
		} 
	}

	return AcceptCount;
}

// Function to add charge to a specified bead type. It wraps each bead with a 
// CBeadChargeWrapper object so that all beads of the type experience an 
// interaction due to their charge. Several bead types can be charged, but the
// total number of charged beads should be small because we add the resulting 
// forces by performing an N(N-1)/2 loop over all charged beads. This is because
// we are not trying to model electrostatic interactions amongst all beads, but
// only to add a screened charge interaction to a selected few.
//
// The force law is: F(r) = F(0)exp(-kappa*r)/r
//
// where kappa = 1/range.
//
// Note that if we are doing an MD simulation we have to scale the strength and range
// of the force appropriately.

void CSimBox::ChargeBeadType(const xxCommand *const pCommand)
{
#if EnableConstraintCommand == SimCommandEnabled
#if EnableMiscClasses == SimMiscEnabled

	const ccChargeBeadType* const pCmd = dynamic_cast<const ccChargeBeadType*>(pCommand);

	// Unpack the data identifying the particular bead that is charged by the command.
	// If the simulation type is MD we normalise the force constant by the
	// energy scale taken from the first species' LJ potential depth, and the range
	// by the first LJ potential range.

	const zString name	= pCmd->GetName();
	double strength		= pCmd->GetStrength();
	double range		= pCmd->GetRange();
	double kappa		= 0.0;

	const long type		= GetBeadTypeFromName(name);

	// Only create charged bead wrappers if the specified bead type has not yet 
	// been charged. We use the find algorithm to see if the bead type occurs in the 
	// current set of charged bead types.

	if(find(m_vChargedBeadTypes.begin(), m_vChargedBeadTypes.end(), type) == m_vChargedBeadTypes.end())
	{
		m_vChargedBeadTypes.push_back(type);

		// How do I normalise the MD parameters???

#if SimIdentifier == MD
		strength *= (GetLengthScale()*GetLengthScale()/GetEnergyScale());
		range    /= GetLengthScale();
#endif

		// If the user has set the range to less than 10**-08, we treat this as 
		// having a strength of zero.

		if(range < 0.00000001)
		{
			strength = 0.0;
			kappa	 = 0.0;
		}
		else
		{
			kappa = 1.0/range;
		}

		// Loop over all beads adding those of the specified type to the m_lAllChargedBeads
		// container. We create a wrapper object from the CBeadChargeWrapper class,  
		// store the original CAbstractBead within it, and use forwarding functions 
		// to access the properties of the inner bead.

		long beadTotal = 0;

        AbstractBeadVector vAllBeads = GetAllBeadsInCNTCells();
	    for(AbstractBeadVectorIterator iterBead=vAllBeads.begin(); iterBead!=vAllBeads.end(); iterBead++)
		{
			if((*iterBead)->GetType() == type)
			{
				beadTotal++;

				CBeadChargeWrapper* pChargedBead = new CBeadChargeWrapper(*iterBead, strength, range, kappa);

				m_lAllChargedBeads.push_back(pChargedBead);
			}
		}

		m_vChargedBeadTotals.push_back(beadTotal);	// Store the number of newly-charged beads

		// Tell the SimState that charged beads are being used if this is the first time

		if(!IsBeadChargeOn())
			SetBeadChargeOn(true);

		// Log a message indicating that the bead type has been charged.

		new CLogChargeBeadType(m_SimTime, name, type, strength, range, true);
	}
#endif
#endif
}

// Function to remove the charge from all beads of a specified type.
// It is the complement of the ChargeBeadType() command above. We don't allow 
// beads to have their charge changed by command because we consider charge to 
// be an immutable property. However, removing all charged beads of a given type
// and then recharging them is allowed. 

void CSimBox::UnchargeBeadType(const xxCommand *const pCommand)
{
#if EnableConstraintCommand == SimCommandEnabled
#if EnableMiscClasses == SimMiscEnabled

	const ccUnchargeBeadType* const pCmd = dynamic_cast<const ccUnchargeBeadType*>(pCommand);

	// Unpack the data identifying the particular bead type that is being uncharged.

	const zString name	= pCmd->GetName();

	const long type		= GetBeadTypeFromName(name);

	// Only delete the bead type if it has already been charged

	if(find(m_vChargedBeadTypes.begin(), m_vChargedBeadTypes.end(), type) != m_vChargedBeadTypes.end())
	{
		m_vChargedBeadTypes.erase(find(m_vChargedBeadTypes.begin(), m_vChargedBeadTypes.end(), type));
	
		// Loop over the charged bead container removing those whose type matches 
		// that in the command. First we delete each CBeadChargeWrapper object with
		// the specified bead type, and then we erase the section of the container
		// that used to contain those beads. Because the bead pointers are stored 
		// in a list, we can delete a contiguous range of elements without invalidating
		// the remainder of the container.


		// Note the absence of the increment step because we use erase()  
		// to remove beads of the specified type, and increment it manually when
		// the type is different

		for(ChargedBeadListIterator iterBead=m_lAllChargedBeads.begin(); iterBead!=m_lAllChargedBeads.end(); )
		{
			if((*iterBead)->GetType() == type)
			{
				delete *iterBead;

				iterBead = m_lAllChargedBeads.erase(iterBead);
			}
			else
				iterBead++;
		}

		// If there are no longer any charged beads, toggle the SimState flag off

		if(m_lAllChargedBeads.empty())
			SetBeadChargeOn(false);
		
		// Log a message indicating that the charged bead type has been removed.
		// Note the zero values for the strength and range. These are not contained
		// in the ccUnchargeBeadType command.

		new CLogChargeBeadType(m_SimTime, name, type, 0.0, 0.0, false);
	}	
#endif
#endif

}

// Function to change the parameters of a CBond object in the simulation using 
// its type instead of its name to identify it. It is otherwise identical
// to the SetBondStrength command.
//
// The vector of all bonds is iterated over and those whose type matches
// that specified in the command have their parameters set to the new values.
// If the simulation is MD we apply the normalisation before setting the values.

void CSimBox::SetBondStrengthbyType(const xxCommand * const pCommand)
{
	const ccSetBondStrengthbyType* const pCmd = dynamic_cast<const ccSetBondStrengthbyType*>(pCommand);

	// Unpack the data identifying the particular bond that is changed by
	// the command and the new values of its spring constant and unstretched length.
	// If the simulation type is MD we normalise the spring constant by the
	// energy scale taken from the first species' LJ potential depth, and the lengths
	// by the first LJ potential range.

	const zString name		= pCmd->GetName();
	const long type 		= pCmd->GetBondType();
	double springConstant	= pCmd->GetSpringConstant();
	double unStrLength		= pCmd->GetUnstretchedLength();

	// Check that the bond type exists. This cannot be done earlier because the
	// initial state is not polymerised when CInputData checks the commands.
	// If it does not exist we ignore the command and post a command failed message.

	if(0 <= type && type <= GetBondTypeTotal())
	{
#if SimIdentifier == MD
		springConstant *= (GetLengthScale()*GetLengthScale()/GetEnergyScale());
		unStrLength    /= GetLengthScale();
#endif
		// Loop over all polymers changing the properties of those bonds that are
		// targetted by the command

		for(PolymerVectorIterator iterPoly=m_vAllPolymers.begin(); iterPoly!=m_vAllPolymers.end(); iterPoly++)
		{
			BondVector vBonds = (*iterPoly)->GetBonds();

			for(BondVectorIterator iterBond=vBonds.begin(); iterBond!=vBonds.end(); iterBond++)
			{
				if((*iterBond)->GetType() == type)
				{
					(*iterBond)->SetSpringConstant(springConstant);
					(*iterBond)->SetUnStretchedLength(unStrLength);
				}
			}
		}

		// Create a message indicating that the bond parameters have changed

		new CLogBondStrengthChanged(m_SimTime, name, type, springConstant, unStrLength);
	}
	else	// Log a command failed message
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}
}

// Function to apply a ctAssignExistingBeadType command to a command target.
// A set of beads, which have a specified type, whose centres lie within a 
// certain geometric region of the SimBox are stored in a CCommandTarget. 
// This can be the target of further commands that act on the beads. 
//
// This command changes the type of the beads to one that already exists. 
// The beads can be distinguished from others of the type by their pointers 
// held in the CCommandTarget.

void CSimBox::AssignExistingBeadType(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctAssignExistingBeadType* const pCmd = dynamic_cast<const ctAssignExistingBeadType*>(pCommand);

	const zString label            = pCmd->GetTargetLabel();
	const zString assignedTypeName = pCmd->GetAssignedName();

	// Convert the assigned bead name to its numeric identifier: we use the ISimState
	// interface class to access the simulation state

	const long assignedType = GetBeadTypeFromName(assignedTypeName);

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(label);

	// Now we have the target, check it is not composite and that the new type
	// exists, and get the beads contained in it. Then change the type of the 
	// beads. We use the IModifySimStateBeadTypes interface class to safely alter the 
	// CSimState: the changes do not affect the Monitor or other parts of the 
	// simulation as the new bead type already exists, only the numbers of each
	// bead type are changed.

	if(pCmdTarget && !pCmdTarget->IsComposite() && assignedType < GetBeadTypeTotal())
	{
		BeadVector vBeads = pCmdTarget->GetBeads();

		for(BeadVectorIterator iterBead = vBeads.begin(); iterBead != vBeads.end(); iterBead++)
		{
			(*iterBead)->SetType(assignedType);
		}

		IModifyBeadTypes()->CalculateTypeTotals();

		// Update the command target with the (newType, oldType) pair,
		// and issue a log message indicating the successful command execution. 
		// Note that the previous name of the bead type is the original name
		// for the beads stored in the target. This allows for beads to have their
		// type changed to an unnamed type without breaking this routine.

		const long oldType = pCmdTarget->GetCurrentBeadType();
		const zString oldName = GetBeadNameFromType(pCmdTarget->GetOriginalBeadType());

		pCmdTarget->AddBeadTypePair(assignedType, oldType);

		new CLogAssignExistingBeadType(m_SimTime, label, oldName, oldType, assignedTypeName, assignedType);
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Function to implement a ctRestoreBeadType command to a command target.
// A set of beads, which have a specified type, whose centres lie within a 
// certain geometric region of the SimBox are stored in a CCommandTarget. 
// This can be the target of further commands that act on the beads. 
//
// This command changes the type of the beads to the immediately preceding
// value. Note that this command should only be called after a command that
// has altered the type of the beads in the target. Any number of matched 
// pairs of these two commands is allowed.
// Because beads elsewhere in the SimBox, or in other command targets, may
// share the type that is here being restored, we do not propagate the change
// out to the CMonitor, CCNTCell, etc. We just change the type of the beads in 
// the current target back to the previous value and recalculate the bead type
// totals to reflect the change.

void CSimBox::RestoreBeadType(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctRestoreBeadType* const pCmd = dynamic_cast<const ctRestoreBeadType*>(pCommand);

	const zString label = pCmd->GetTargetLabel();

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(label);

	// Now we have the target, check it is not composite and get the 
	// beads contained in it

	if(pCmdTarget && !pCmdTarget->IsComposite())
	{
		BeadVector vBeads = pCmdTarget->GetBeads();

		// Change the type of the beads back to its previous value.

		const long newType = pCmdTarget->GetCurrentBeadType();
		const long oldType = pCmdTarget->GetPreviousBeadType(newType);

		// Restore the previous type for all beads in the target

		for(BeadVectorIterator iterBead = vBeads.begin(); iterBead != vBeads.end(); iterBead++)
		{
			(*iterBead)->SetType(oldType);
		}

		// Recalculate the totals for each bead type

		IModifyBeadTypes()->CalculateTypeTotals();

		// Update the command target and issue a log message indicating 
		// the change of bead type. Note that we obtain the bead name
		// from the original type stored in the command target.

		pCmdTarget->RemoveBeadTypePair(newType);

		const zString oldName = GetBeadNameFromType(pCmdTarget->GetOriginalBeadType());

		new CLogRestoreBeadType(m_SimTime, label, oldName, oldType, newType);
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Function to implement a ctRestoreOriginalBeadType command to a command target.
// A set of beads, which have a specified type, whose centres lie within a 
// certain geometric region of the SimBox are stored in a CCommandTarget. 
// This can be the target of further commands that act on the beads. 
//
// This command changes the type of the beads to their original value. If the
// type of the beads has not been changed this command has no effect.
//
// Because beads elsewhere in the SimBox, or in other command targets, may
// share the type that is here being restored, we do not propagate the change
// out to the CMonitor, CCNTCell, etc. We just change the type of the beads in 
// the current target and recalculate the bead type totals to reflect the change.

void CSimBox::RestoreOriginalBeadType(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctRestoreOriginalBeadType* const pCmd = dynamic_cast<const ctRestoreOriginalBeadType*>(pCommand);

	const zString label = pCmd->GetTargetLabel();

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(label);

	// Now we have the target, check it is not composite and get the beads

	if(pCmdTarget && !pCmdTarget->IsComposite())
	{
		BeadVector vBeads = pCmdTarget->GetBeads();

		// Change the type of the beads back to the original value only if their
		// current value is different. If this is so, we still issue a successful
		// log message so that the failed message indicates a real failure and not
		// just a do-nothing case.

		const long newType = pCmdTarget->GetCurrentBeadType();
		const long oldType = pCmdTarget->GetOriginalBeadType();

		if(newType != oldType)
		{
			for(BeadVectorIterator iterBead = vBeads.begin(); iterBead != vBeads.end(); iterBead++)
			{
				(*iterBead)->SetType(oldType);
			}

			pCmdTarget->ClearBeadTypePairs();

			IModifyBeadTypes()->CalculateTypeTotals();
		}

		// Issue a log message indicating the change of bead type. Note that we 
		// obtain the bead name from the original type stored in the command target.

		const zString oldName = GetBeadNameFromType(pCmdTarget->GetOriginalBeadType());

		new CLogRestoreOriginalBeadType(m_SimTime, label, oldName, oldType, newType);
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Handler function to implement a ccSetTimeStepSize command that changes 
// the integration time step to a new value. The step size must be positive
// definite (this is checked in the command class), and takes effect from 
// the time step in which the command is executed. To avoid huge forces
// being generated because the stored values of force and momentum relate
// to the old time step, we zero previous values for each bead's force 
// and momentum.
//
// The command stores the new time step in the CCNTCell class and the
// CAnalysisState class. Several constants are defined in CCNTCell that 
// depend on the time step and these are updated accordingly. It then
// zeroes the old values of bead momenta and force to prevent huge forces
// being generated out of the time step change.
//

void CSimBox::SetTimeStepSize(const xxCommand* const pCommand)
{
	const ccSetTimeStepSize* const pCmd = dynamic_cast<const ccSetTimeStepSize*>(pCommand);

#if EnableParallelCommands == SimMPSEnabled
	
	IModifyIntegration()->SetTimeStep(pCmd->GetTimeStep());
	
	if(xxParallelBase::GlobalGetRank() == 0)
	{
	    new CLogSetTimeStepSize(m_SimTime, pCmd->GetTimeStep());
	}

#else

	IModifyIntegration()->SetTimeStep(pCmd->GetTimeStep());

	// Zero bead data - not implemented yet

	new CLogSetTimeStepSize(m_SimTime, pCmd->GetTimeStep());
#endif
}

// Command handler function to allow a set of commands to be scheduled for
// execution at a specified time in the future.

void CSimBox::SetCommandTimer(const xxCommand* const pCommand)
{
	const ccSetCommandTimer* const pCmd = dynamic_cast<const ccSetCommandTimer*>(pCommand);

	const long delay			= pCmd->GetDelay();
	CommandSequence commands	= pCmd->GetCommands();

	// Loop over the payload of commands scheduling each one for execution
	// after the timer runs out. Note that all commands in the payload must
	// have the same execution time, and it should be equal to the time at
	// which the ccSetCommandTimer executed plus the delay.

	// We store the names of the commands to write to the log state

	if(delay > 0 && !commands.empty())
	{
		StringSequence names;

		for(cCommandIterator cIterCmd=commands.begin(); cIterCmd!=commands.end(); cIterCmd++)
		{
			const xxCommand* const pNewCmd = *cIterCmd;

			names.push_back(pNewCmd->GetCommandType());

			AddCommand(pNewCmd);
		}

		// Log a message indicating successful execution

		new CLogSetCommandTimer(m_SimTime, delay, names);
	}
	else	// Log a command failed message
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}
}

// Handler function to implement a ccExtendTotalTime command that extends
// a simulation by adding an integer number of time steps to the current total.
// The command may specify an increment of zero, in which case it has no
// effect, but not a negative value.
//
// The new total simulation time is passed to the CSimState for storage. No other
// class stores the total time, as they obtain the value either from the 
// CSimBox or CSimState. We log the command's execution using the increment of 
// the time rather than the new total time because it is easier to see if the
// command specified a zero increment, and hence had no effect. If the command
// specifies a negative increment, a failed command message is issued.

void CSimBox::ExtendTotalTime(const xxCommand* const pCommand)
{
	const ccExtendTotalTime* const pCmd = dynamic_cast<const ccExtendTotalTime*>(pCommand);

	const long extraTime = pCmd->GetExtraTime();

	if(extraTime >= 0)
	{
		m_TotalTime += extraTime;

		IModifyIntegration()->SetTotalTime(m_TotalTime);

		new CLogExtendTotalTime(m_SimTime, extraTime);
	}
	else	// Log a command failed message
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}
}

// Handler function to count the number of beads in a command target. It works
// for CCommandTargets, which hold a single bead type, and returns the number
// of beads in the target, and also for CCommandTargetComposites, which hold
// other targets, and it returns the total number of beads in the composite target.
// For simple targets, which hold only a single bead type, it outputs the current 
// numeric bead type and the original bead name, whereas for composite targets it 
// outputs place-holder information for the types and names. Using the original
// name and current bead type allows future commands to be targetted at the new
// bead type, while still allowing the bead name to be identified.

void CSimBox::CountBeadsInTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

    const ctCountBeadsInTarget* const pCmd = dynamic_cast<const ctCountBeadsInTarget*>(pCommand);

	const zString label = pCmd->GetTargetLabel();

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(label);

	// Now we have the target, check if it is a composite and get the number of 
	// beads in it. For a simple command target we output the bead name and type, 
	// but for a composite target we output NULL values for these properties.

	if(pCmdTarget)
	{
		const long beadTotal = pCmdTarget->GetBeadTotal();

		if(pCmdTarget->IsComposite())
		{
			const long beadType  = -1;
			const zString beadName = "mixed";

			new CLogCountBeadsInTarget(m_SimTime, label, beadName, beadType, beadTotal);
		}
		else
		{
			const long beadType            = pCmdTarget->GetCurrentBeadType();
			const long originalBeadType    = pCmdTarget->GetOriginalBeadType();
			const zString beadName         = GetBeadNameFromType(originalBeadType);

			new CLogCountBeadsInTarget(m_SimTime, label, beadName, beadType, beadTotal);
		}
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Handler function to count the number of beads of a specified type in a 
// command target. If invoked on a simple target, and the type identifier matches
// that of the beads in the target, it counts the number of such beads. If invoked
// on a composite target, and the bead type is found in a lower-level target, it 
// counts the number of such beads. If the bead type is not found in either case
// it returns 0.

void CSimBox::CountBeadTypeInTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

    const ctCountBeadTypeInTarget* const pCmd = dynamic_cast<const ctCountBeadTypeInTarget*>(pCommand);

	const zString   label = pCmd->GetTargetLabel();
	const long soughtType = pCmd->GetBeadType();

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(label);

	// Now we have the target, get the number of beads of the sought type
	// using a function that works for both simple targets and composite ones. 
	// For a simple target we output the bead name, but for a composite target 
	// we output a generic name, "Unnamed", as it cannot easily be determined.
	// However, we output the current type of the beads in the target (soughtType)
	// in case a command has changed their type.

	if(pCmdTarget)
	{
		const long beadTotal = pCmdTarget->GetBeadTotalForType(soughtType);

		if(pCmdTarget->IsComposite())
		{
			new CLogCountBeadsInTarget(m_SimTime, label, "Unnamed", soughtType, beadTotal);
		}
		else
		{
			const zString beadName  = GetBeadNameFromType(soughtType);

			new CLogCountBeadsInTarget(m_SimTime, label, beadName, soughtType, beadTotal);
		}
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Handler function to create a new, empty composite command target. This acts as a
// container for other command targets allowing multiple bead types (one per
// simple command target) to be manipulated together.

void CSimBox::CompositeCommandTarget(const xxCommand* const pCommand) 
{
#if EnableTargetCommand == SimCommandEnabled

	const ctCompositeCommandTarget* const pCmd = dynamic_cast<const ctCompositeCommandTarget*>(pCommand);

	const zString label = pCmd->GetTargetLabel();

	if(CreateCompositeCommandTarget(label))
	{
		new CLogctCompositeCreated(m_SimTime, label);
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Handler function to add a single command target to a composite command target. 
// Both targets must exist before the command is executed. The destination command 
// must be a CCommandTargetComposite instance, but the source command can be an 
// instance of any class derived from CCommandTargetNode.

void CSimBox::AddTargetToComposite(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctAddTargetToComposite* const pCmd = dynamic_cast<const ctAddTargetToComposite*>(pCommand);

	const zString      source = pCmd->GetSourceTargetLabel();
	const zString destination = pCmd->GetDestinationTargetLabel();

	// Get the source and destination targets from the target list

	CCommandTargetNode*      pSourceTarget = GetCommandTarget(source);
	CCommandTargetNode* pDestinationTarget = GetCommandTarget(destination);

	// Check that the destination target is composite and add the source to it,
	// if the test fails log a command failed message.

	if(pSourceTarget && pDestinationTarget && pDestinationTarget->IsComposite() && pDestinationTarget->AddTarget(pSourceTarget))
	{
		new CLogctAddTargetToComposite(m_SimTime, destination, source, pDestinationTarget->GetTargetTotal());
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Handler function to remove a target from a composite command target. Both
// targets must exist before the command is executed.

void CSimBox::RemoveTargetFromComposite(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctRemoveTargetFromComposite* const pCmd = dynamic_cast<const ctRemoveTargetFromComposite*>(pCommand);

	const zString      source = pCmd->GetSourceTargetLabel();
	const zString destination = pCmd->GetDestinationTargetLabel();

	// Get the source and destination targets from the target list

	CCommandTargetNode*      pSourceTarget = GetCommandTarget(source);
	CCommandTargetNode* pDestinationTarget = GetCommandTarget(destination);

	// Check that the destination target is composite and remove the source from it,
	// if the test fails log a command failed message.

	if(pSourceTarget && pDestinationTarget && pDestinationTarget->IsComposite() && pDestinationTarget->RemoveTarget(source))
	{
		new CLogctRemoveTargetFromComposite(m_SimTime, destination, source, pDestinationTarget->GetTargetTotal());
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Function to create a new CCommandTarget to hold a set of beads that can have
// commands applied to them. It returns true if the target was successfully
// created, i.e., its name is unique and the bead type exists, and false otherwise.
//
// Note that this function only creates a simple target that holds beads; to 
// create a CCommandTargetComposite a different function must be used.

bool CSimBox::CreateCommandTarget(const zString label, long beadType, BeadVector vTargetBeads)
{
#if EnableTargetCommand == SimCommandEnabled

	bool bExists = false;

	for(cCommandTargetIterator cIterTarget=m_CommandTargets.begin(); cIterTarget!=m_CommandTargets.end(); cIterTarget++)
	{
		if((*cIterTarget)->GetLabel() == label)
			bExists = true;
	}

	if(!bExists && beadType >=0 && beadType < GetBeadTypeTotal())
	{
		CCommandTarget* pCmdTarget = new CCommandTarget(label, beadType, vTargetBeads);
		m_CommandTargets.push_back(pCmdTarget);
		return true;
	}
	else
	{
		return false;
	}

#else
    return false;
#endif
}

// Function to create a CCompositeCommandTarget that holds a set of other command
// targets so that a command can be directed at  multiple targets simultaneously.

bool CSimBox::CreateCompositeCommandTarget(const zString label)
{
#if EnableTargetCommand == SimCommandEnabled

    bool bExists = false;

	for(cCommandTargetIterator cIterTarget=m_CommandTargets.begin(); cIterTarget!=m_CommandTargets.end(); cIterTarget++)
	{
		if((*cIterTarget)->GetLabel() == label)
			bExists = true;
	}

	if(!bExists)
	{
		CCommandTargetComposite* pCmdTarget = new CCommandTargetComposite(label);
		m_CommandTargets.push_back(pCmdTarget);
		return true;
	}
	else
	{
		return false;
	}

#else
    return false;
#endif

}

// Function to create a new CCommandTarget to hold a set of polymers that can 
// have commands applied to them. It returns true if the target was successfully
// created, i.e., its name is unique and the polymer type exists, and false otherwise.
//
// Note that this function only creates a simple target that holds polymers; to 
// create a CCommandTargetComposite a different function must be used.

bool CSimBox::CreateCommandTargetPolymer(const zString label, long polyType, PolymerVector vTargetPolymers)
{
#if EnableTargetCommand == SimCommandEnabled

    bool bExists = false;

	for(cCommandTargetIterator cIterTarget=m_CommandTargets.begin(); cIterTarget!=m_CommandTargets.end(); cIterTarget++)
	{
		if((*cIterTarget)->GetLabel() == label)
			bExists = true;
	}

	if(!bExists && polyType >=0 && polyType < GetPolymerTypeTotal())
	{
		CCommandTargetPolymer* pCmdTarget = new CCommandTargetPolymer(label, polyType, vTargetPolymers);
		m_CommandTargets.push_back(pCmdTarget);
		return true;
	}
	else
	{
		return false;
	}

#else
    return false;
#endif
}

// Function to create a new CCommandTargetACNFreeBonds to hold a set of polymers  
// that represent the free bonds belonging to an ACN. Note that the word "bond" 
// is somewhat of a misnomer as it refers to the CPolymer type defined in the CDF
// that is wrapped by the ACN monomers. When these monomers bind to each other
// they form the ACN's "polymers." This target is equivalent to a CCommandTargetPolymer
// in that it holds a set of CPolymer instances.
//
// This creation function requires access to the associated ACN so that the new
// target can update its stored set of free bonds. 
//
// It returns true if the target was successfully created, i.e., its name is unique 
// and the polymer type exists, and false otherwise. Note that there may not be any
// free bonds in the ACN but this is not an error. They may all be bound into filaments.

bool CSimBox::CreateCommandTargetACNFreeBonds(const zString label, const zString acnType)
{
#if EnableShadowSimBox == SimACNEnabled
#if EnableTargetCommand == SimCommandEnabled

	bool bFails = false;

	// Check that the target name is not already in use, and the ACN exists

	for(cCommandTargetIterator cIterTarget=m_CommandTargets.begin(); cIterTarget!=m_CommandTargets.end(); cIterTarget++)
	{
		if((*cIterTarget)->GetLabel() == label)
			bFails = true;
	}

	aeActiveCellNetwork* pACN = GetActiveSimBox()->GetACNFromType(acnType);

	if(!bFails && pACN)
	{
		CCommandTargetACNFreeBonds* pCmdTarget = new CCommandTargetACNFreeBonds(label, pACN);
		m_CommandTargets.push_back(pCmdTarget);

		return true;
	}
	else
		return false;

#else
	return false;
#endif

#else
	return false;
#endif
}

// Function to create a new CCommandTargetACNFreePolymers to hold a set of polymers 
// that represent the free polymers (or filaments) belonging to an ACN. The 
// filaments consist of a set of ACN monomers each of which wraps a CPolymer. 
// This target is equivalent to a CCommandTargetPolymer in that it holds a set 
// of CPolymer instances, but it continually updates its contained polymers
// so that all filaments in the ACN are stored.
//
// This creation function requires access to the associated ACN so that the new
// target can update its stored set of free polymers. 
//
// It returns true if the target was successfully created, i.e., its name is unique 
// and the ACN exists, and false otherwise. Note that there may not be
// any free polymers in the ACN but this is not an error; all the monomers may be
// free at the time the target is created, but may assemble into polymers later.

bool CSimBox::CreateCommandTargetACNFreePolymers(const zString label, const zString acnType)
{
#if EnableShadowSimBox == SimACNEnabled
#if EnableTargetCommand == SimCommandEnabled

    bool bFails = false;

	// Check that the target name is not already in use, and the ACN exists

	for(cCommandTargetIterator cIterTarget=m_CommandTargets.begin(); cIterTarget!=m_CommandTargets.end(); cIterTarget++)
	{
		if((*cIterTarget)->GetLabel() == label)
			bFails = true;
	}

	aeActiveCellNetwork* pACN = GetActiveSimBox()->GetACNFromType(acnType);

	if(!bFails && pACN)
	{
		CCommandTargetACNFreePolymers* pCmdTarget = new CCommandTargetACNFreePolymers(label, pACN);
		m_CommandTargets.push_back(pCmdTarget);

		return true;
	}
	else
		return false;

#else
	return false;
#endif

#else
	return false;
#endif
}

// Function to find a command target given its name. If the target does not
// exist it returns a NULL pointer that should be checked by all calling 
// routines. Note that only command targets are stored here: the decorator
// instances only occur in the active command targets container.

CCommandTargetNode* CSimBox::GetCommandTarget(const zString label) const
{
	CCommandTargetNode* pCmdTarget = 0;

	for(cCommandTargetIterator iterTarget=m_CommandTargets.begin(); iterTarget!=m_CommandTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetLabel() == label)
		{
			pCmdTarget = *iterTarget;
		}
	}

	return pCmdTarget;
}

// Function to return the set of command targets.

const CommandTargetSequence& CSimBox::GetCommandTargets() const
{
    return m_CommandTargets;
}

// Function to return the set of active command targets. Note that this does NOT
// return the total number of active targets as some of them may be daisy-chained
// together: it returns the number of independently-wrapped targets. For the total
// number of targets plus all decorators use the GetCommandTargetNodeTotal() function.

const CommandTargetSequence&  CSimBox::GetActiveCommandTargets() const
{
    return m_ActiveCommandTargets;
}

// Function to return the total number of command targets plus decorators.
// This gives the total number of CommandTargetNode instances in the simulation.

long CSimBox::GetCommandTargetNodeTotal() const
{
    long total = m_CommandTargets.size();
    for(cCommandTargetIterator citerTarget=m_CommandTargets.begin(); citerTarget!=m_CommandTargets.end(); citerTarget++)
    {
        total += (*citerTarget)->GetDecoratorTotal();
    }

    return total;
}

// Function to find an active command target given its name. Note that
// this uses the name of the target itself not one of its decorator instances.
//
// We use the CCommandTargetNode::GetActiveCommandTarget() function to recurse
// through all the decorator instances that may wrap the active target
// until we get the actual command target at the centre. If none of the
// wrapped targets in the active command target container match the 
// search string it returns NULL.

CCommandTargetNode* CSimBox::GetActiveCommandTarget(const zString label) const
{
	CCommandTargetNode* pCmdTarget = 0;

	for(cCommandTargetIterator iterTarget=m_ActiveCommandTargets.begin(); iterTarget!=m_ActiveCommandTargets.end(); iterTarget++)
	{
		if((*iterTarget)->GetActiveCommandTarget()->GetLabel() == label)
		{
			pCmdTarget = (*iterTarget)->GetActiveCommandTarget();
		}
	}

	return pCmdTarget;
}

// Function to return a decorator instance that wraps a command target.
// Because active command targets are stored using their outermost 
// decorator instance, we have to recurse through all targets in the
// m_ActiveCommandTargets container, and through all decorator instances
// that wrap the targets, searching for a match of the decorator label 
// to the search string. If the decorator is not found, we return 0 which 
// should be checked by the calling routine.

CCommandTargetNode* CSimBox::GetCommandTargetActivity(const zString label) const
{
	cCommandTargetIterator iterTarget = m_ActiveCommandTargets.begin();
	CCommandTargetNode*   pCmdTarget  = 0;
	CCommandTargetNode*   pDec		  = 0;
	CCommandTargetNode*   pSoughtDec  = 0;

	bool bFound = false;

	while(!bFound && iterTarget != m_ActiveCommandTargets.end())
	{
		pCmdTarget = *iterTarget;
		pDec       = pCmdTarget;

		while(!bFound && pDec->GetInnerDecorator())
		{
			if(pDec->GetLabel() == label)
			{
				bFound = true;
				pSoughtDec = pDec;
			}
			else
			{
				pDec = pDec->GetInnerDecorator();
			}
		}

		iterTarget++;
	}

	return pSoughtDec;
}

// Function to return the outermost decorator, or the command target itself
// if it has no decorator instances. It searches the active command target
// container for the specified target label (not a decorator label). 
// If found, it means that the target is already active, so it returns its 
// outermost decorator. If the target is not found, it is currently inactive, 
// so it searches the command target container to find it.
//
// Note that it is possible this function returns a NULL pointer. This 
// should be checked by the calling routine.

CCommandTargetNode* CSimBox::GetActiveCommandTargetOutermost(const zString targetLabel) const
{
	CCommandTargetNode* pActiveTarget = GetActiveCommandTarget(targetLabel);
	CCommandTargetNode* pOuterDec     = 0;

	if(pActiveTarget)
	{
		pOuterDec = pActiveTarget->GetOutermostDecorator();
	}
	else
	{
		pOuterDec = GetCommandTarget(targetLabel);
	}

	return pOuterDec;
}

// ****************************************
// Function to count the number of currently active command targets.
// Because this command cannot fail, we don't try to issue a command
// failed message; and because there is no data, we don't store the
// actual command instance.

void CSimBox::CountActiveCommandTargets(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

//	const ctCountActiveCommandTargets* const pCmd = dynamic_cast<const ctCountActiveCommandTargets*>(pCommand);

	new CLogctCountActiveCommandTargets(m_SimTime, m_ActiveCommandTargets.size());

#endif
}

void CSimBox::CountCommandTargets(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

    //	const ctCountCommandTargets* const pCmd = dynamic_cast<const ctCountCommandTargets*>(pCommand);

	new CLogctCountCommandTargets(m_SimTime, m_CommandTargets.size());

#endif
}

// Function to count the number of targets contained in a named composite target.
// This only counts targets directly held in the composite target, not any targets 
// that they may contain if they are composite.
//
// If the named target does not exist or is not composite, we issue a warning message,
// but if it is empty we just return a count of zero.

void CSimBox::CountTargetsInComposite(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctCountTargetsInComposite* const pCmd = dynamic_cast<const ctCountTargetsInComposite*>(pCommand);

    const zString label = pCmd->GetTargetLabel();

    CCommandTargetNode* pTarget = GetCommandTarget(label);

    if(pTarget)
    {
        if(pTarget->IsComposite())
        {
            const long total = pTarget->GetTargetTotal();

	       new CLogctCountTargetsInComposite(m_SimTime, label, total);
        }
        else
        {
		    new CLogCommandFailed(m_SimTime, pCmd);
        }
    }
    else
    {
		new CLogctNonExistentTarget(m_SimTime, label);
    }

#endif
}

// Function to remove a command target from the active targets container.
// It requires the label for a command target, not a decorator instance.
// It does not delete the target just removes it from the set of active
// targets, and deletes all of its decorator instances.
// It can be made active again by sending another command to it.
//
// Note that if the target is composite, its contained targets are not
// removed if they have been independently added to the active command
// target container. That is, we don't recurse through a composite
// target when removing it, we just do a "shallow" removal. This also
// applies to the acitivities that wrap the targets: only those wrapping
// the composite target being removed are deleted.

void CSimBox::RemoveActiveCommandTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctRemoveActiveCommandTarget* const pCmd = dynamic_cast<const ctRemoveActiveCommandTarget*>(pCommand);

	const zString target = pCmd->GetTargetLabel();

	CCommandTargetNode* const pCmdTarget = GetActiveCommandTarget(target);

	// Remove the target from the active targets container first as it is
	// stored using a pointer to its outermost decorator instance, and
	// once the decorators are deleted we may lose access to the target.

	// Note that if all the target's decorators have been deleted, it will
	// already have been removed from the active target container and this 
	// command will issue a command failed message.

	if(pCmdTarget)
	{
		CCommandTargetNode* const pOutermostDec = pCmdTarget->GetOutermostDecorator();
		m_ActiveCommandTargets.remove(pOutermostDec);

//		Alternative code that deletes the command target completely, and its
//		decorator instances via the destructor
//		m_CommandTargets.remove(pCmdTarget);
//		delete pCmdTarget;

		// Delete all the decorator objects 

		CCommandTargetNode * pDec = pCmdTarget->GetOuterDecorator();

		while(pDec)
		{
			CCommandTargetNode * pOuterDec = pDec->GetOuterDecorator();
			delete pDec;
			pDec = pOuterDec;
		}

		new CLogctRemoveActiveCommandTarget(m_SimTime, target, m_ActiveCommandTargets.size());
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Function to remove one decorator instance from an active command target.
// It requires the label for a decorator instance not a command target.
// If the decorator instance is found, it is deleted, otherwise an error 
// message is issued. Other decorators wrapping the same command target 
// are unaffected, and their inner and outer decorator pointers are 
// rearranged to be consistent without the missing decorator.
// If the decorator was the only one wrapping the target, the target is
// automatically removed from the active targets container by the removal
// of the decorator. It can be made active again by issuing further commands.

void CSimBox::RemoveCommandTargetActivity(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctRemoveCommandTargetActivity* const pCmd = dynamic_cast<const ctRemoveCommandTargetActivity*>(pCommand);

	const zString activity = pCmd->GetTargetLabel();

	// First find the decorator instance. If it does not exist, as may 
	// happen if a RemoveActiveCommandTarget command is issued before 
	// a RemoveCommandTargetActivity, issue a command failed message.

	CCommandTargetNode* pDec = GetCommandTargetActivity(activity);

	if(pDec)
	{
		// Get the actual command target instance and the decorator's
		// inner and outer pointers.

		CCommandTargetNode* pTarget = pDec->GetActiveCommandTarget();
		CCommandTargetNode* pInner  = pDec->GetInnerDecorator();
		CCommandTargetNode* pOuter  = pDec->GetOuterDecorator();

		if(!pOuter)
		{
			// This is the outermost decorator instance so set 
			// the outer decorator pointer of its inner decorator
			// instance to 0, and replace the removed decorator
			// with its inner one unless there are no more decorators.

			pDec->GetInnerDecorator()->SetOuterDecorator(0);
			m_ActiveCommandTargets.remove(pDec);

			if(pInner->GetLabel() != pTarget->GetLabel())
				m_ActiveCommandTargets.push_back(pInner);
		}
		else
		{
			// Connect the inner and outer decorator instances to 
			// each other. Note that we don't have to replace the
			// stored deoorator instance as it remains the same.

			pInner->SetOuterDecorator(pOuter);
			pOuter->SetInnerDecorator(pInner);
		}

		delete pDec;

		new CLogctRemoveCommandTargetActivity(m_SimTime,  pTarget->GetLabel(), activity, pTarget->GetDecoratorTotal());
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Function to apply a constant force to the beads in a selected command target.
// The command creates a new decorator instance for the target that applies
// the constant force, and wraps it around the target. If other decorator
// classes already wrap the target, the new one is added on the outside.
// Note that the force is applied until a command explicitly turns it off.

void CSimBox::ConstantForceOnTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctConstantForceOnTarget* const pCmd = dynamic_cast<const ctConstantForceOnTarget*>(pCommand);

	const zString targetLabel	= pCmd->GetTargetLabel();
	const zString decLabel		= pCmd->GetDecLabel();
	const long xn			    = pCmd->GetXNormal();
	const long yn			    = pCmd->GetYNormal();
	const long zn			    = pCmd->GetZNormal();
	const double mag			= pCmd->GetMagnitude();

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(targetLabel);

	// Now we have the target, create a taConstantForce decorator object 
	// and wrap the target with it to enable the force to be applied to the
	// target's beads. Note that because we use a Composite pattern for the
	// command targets, we don't need to know whether it is composite or
	// simple as the GetBeads() function just returns all beads in the target.
	// However, the data on bead type, bead name and original bead type
	// is not applicable to composite targets because they may contain
	// a multitude of bead types.

	if(pCmdTarget)
	{
		const long originalBeadType    = pCmdTarget->GetOriginalBeadType();
		const zString beadName         = GetBeadNameFromType(originalBeadType);

		CCommandTargetNode* pActiveTarget = GetActiveCommandTargetOutermost(targetLabel);

		// If the target is already active, remove it before adding it back
		// using the new decorator: we just check if the label of the
		// outermost decorator instance differs from the target's label.
		// Note that because we now know that pCmdTarget exists, we don't 
		// have to check that pActiveTarget is non-NULL as it must either be
		// a pointer to a decorator instance or equal to pCmdTarget.

		if(pActiveTarget->GetLabel() != targetLabel)
		{
			m_ActiveCommandTargets.remove(pActiveTarget);
		}

		// Wrap the active target in a new constant force decorator instance
		// and add it to the active command targets container.

		taConstantForce* const pDec = new taConstantForce(decLabel, pActiveTarget, xn, yn, zn, mag);
		m_ActiveCommandTargets.push_back(pDec);

		new CLogctConstantForceOnTarget(m_SimTime, targetLabel, decLabel, xn, yn, zn, mag);
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

void CSimBox::LinearForceOnTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

#endif
}

// Function to apply a (constant magnitude) radial force to the beads in a 
// selected command target. The command creates a new decorator instance for 
// the target that applies the radial force, and wraps it around the target. 
// If other decorator classes already wrap the target, the new one is added 
// on the outside.
// Note that the force is applied until a command explicitly turns it off.

void CSimBox::RadialForceOnTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctRadialForceOnTarget* const pCmd = dynamic_cast<const ctRadialForceOnTarget*>(pCommand);

	const zString targetLabel	= pCmd->GetTargetLabel();
	const zString decLabel		= pCmd->GetDecLabel();
	const long xn			    = pCmd->GetXNormal();
	const long yn			    = pCmd->GetYNormal();
	const long zn			    = pCmd->GetZNormal();
	const double cx				= pCmd->GetXCentre();
	const double cy				= pCmd->GetYCentre();
	const double cz				= pCmd->GetZCentre();
	const double mag			= pCmd->GetMagnitude();

	// Convert the force origin coordinates as fractions of the SimBox side lengths
	// into values relative to the bead diameter

	const double fcx = cx*GetSimBoxXLength();
	const double fcy = cy*GetSimBoxYLength();
	const double fcz = cz*GetSimBoxZLength();

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(targetLabel);

	// Now we have the target, create a taRadialForce decorator object 
	// and wrap the target with it to enable the force to be applied to the
	// target's beads. Note that because we use a Composite pattern for the
	// command targets, we don't need to know whether it is composite or
	// simple as the GetBeads() function just returns all beads in the target.
	// However, the data on bead type, bead name and original bead type
	// is not applicable to composite targets because they may contain
	// a multitude of bead types.

	if(pCmdTarget)
	{
		const long originalBeadType    = pCmdTarget->GetOriginalBeadType();
		const zString beadName         = GetBeadNameFromType(originalBeadType);

		CCommandTargetNode* pActiveTarget = GetActiveCommandTargetOutermost(targetLabel);

		// If the target is already active, remove it before adding it back
		// using the new decorator: we just check if the label of the
		// outermost decorator instance differs from the target's label.
		// Note that because we now know that pCmdTarget exists, we don't 
		// have to check that pActiveTarget is non-NULL as it must either be
		// a pointer to a decorator instance or equal to pCmdTarget.

		if(pActiveTarget->GetLabel() != targetLabel)
		{
			m_ActiveCommandTargets.remove(pActiveTarget);
		}

		// Wrap the active target in a new constant force decorator instance
		// and add it to the active command targets container.

		taRadialForce* const pDec = new taRadialForce(decLabel, pActiveTarget, 
										xn, yn, zn, fcx, fcy, fcz, mag);
		m_ActiveCommandTargets.push_back(pDec);

		new CLogctRadialForceOnTarget(m_SimTime, targetLabel, decLabel, xn, yn, zn, cx, cy, cz, mag);
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Function to apply a (constant amplitude) sinusoidal force to the beads in a 
// selected command target. The command creates a new decorator instance for 
// the target that applies the sine force, and wraps it around the target. 
// If other decorator classes already wrap the target, the new one is added 
// on the outside. The target may be composite. The force is first applied 
// at the time the command execute and its amplitude starts from zero.
//
// Note that the force is applied until a command explicitly turns it off.

void CSimBox::SineForceOnTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctSineForceOnTarget* const pCmd = dynamic_cast<const ctSineForceOnTarget*>(pCommand);

	const zString targetLabel	= pCmd->GetTargetLabel();
	const zString decLabel		= pCmd->GetDecLabel();
	const long xn			    = pCmd->GetXNormal();
	const long yn			    = pCmd->GetYNormal();
	const long zn			    = pCmd->GetZNormal();
	const double amp			= pCmd->GetAmplitude();
	const double period			= pCmd->GetPeriod();

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(targetLabel);

	// Now we have the target, create a taSineForce decorator object 
	// and wrap the target with it to enable the force to be applied to the
	// target's beads. Note that because we use a Composite pattern for the
	// command targets, we don't need to know whether it is composite or
	// simple as the GetBeads() function just returns all beads in the target.
	// However, the data on bead type, bead name and original bead type
	// is not applicable to composite targets because they may contain
	// a multitude of bead types.

	if(pCmdTarget)
	{
		const long originalBeadType    = pCmdTarget->GetOriginalBeadType();
		const zString beadName         = GetBeadNameFromType(originalBeadType);

		CCommandTargetNode* pActiveTarget = GetActiveCommandTargetOutermost(targetLabel);

		// If the target is already active, remove it before adding it back
		// using the new decorator: we just check if the label of the
		// outermost decorator instance differs from the target's label.
		// Note that because we now know that pCmdTarget exists, we don't 
		// have to check that pActiveTarget is non-NULL as it must either be
		// a pointer to a decorator instance or equal to pCmdTarget.

		if(pActiveTarget->GetLabel() != targetLabel)
		{
			m_ActiveCommandTargets.remove(pActiveTarget);
		}

		// Wrap the active target in a new constant force decorator instance
		// and add it to the active command targets container.

		taSineForce* const pDec = new taSineForce(decLabel, pActiveTarget, GetCurrentTime(), xn, yn, zn, amp, period);
		m_ActiveCommandTargets.push_back(pDec);

		new CLogctSineForceOnTarget(m_SimTime, targetLabel, decLabel, xn, yn, zn, amp, period, GetCurrentTime());
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Function to freeze all the beads in a selected command target.
// If the target is composite, all beads in all targets contained in 
// the composite target are frozen.If other decorator classes already 
// wrap the target they are unaffected, although any that require the
// beads to move are, of course, negated. This command is a one-off 
// command that does not need to create a decorator instance.
//
// The beads are prevented from moving, but still contribute forces to
// movable beads that surround them. This allows them to apply a frictional
// force for example. Note that if the beads are already frozen it does 
// not generate an error message: only if the command target does not exist 
// is an error logged.

void CSimBox::FreezeBeadsInTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctFreezeBeadsInTarget* const pCmd = dynamic_cast<const ctFreezeBeadsInTarget*>(pCommand);

	const zString targetLabel	= pCmd->GetTargetLabel();

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(targetLabel);

	if(pCmdTarget)
	{
		const long originalBeadType = pCmdTarget->GetOriginalBeadType();
		const zString beadName      = GetBeadNameFromType(originalBeadType);

		BeadVector vBeads = pCmdTarget->GetBeads();

		for_each(vBeads.begin(), vBeads.end(), mem_fn(&CAbstractBead::SetFrozen));

		new CLogctFreezeBeadsInTarget(m_SimTime, targetLabel);
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Function to unfreeze all the beads in a selected command target.
// If the target is composite, all beads in all targets contained in 
// the composite target are also unfrozen. If other decorator classes 
// already wrap the target they are unaffected. This command is a one-off 
// command that does not need to create a decorator instance.
//
// Note that if the beads are already unfrozen it does not generate an
// error message: only if the command target does not exist is an error
// logged.

void CSimBox::UnFreezeBeadsInTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctUnFreezeBeadsInTarget* const pCmd = dynamic_cast<const ctUnFreezeBeadsInTarget*>(pCommand);

	const zString targetLabel	= pCmd->GetTargetLabel();

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(targetLabel);

	if(pCmdTarget)
	{
		const long originalBeadType = pCmdTarget->GetOriginalBeadType();
		const zString beadName      = GetBeadNameFromType(originalBeadType);

		BeadVector vBeads = pCmdTarget->GetBeads();

		for_each(vBeads.begin(), vBeads.end(), mem_fn(&CAbstractBead::SetNotFrozen));

		new CLogctUnFreezeBeadsInTarget(m_SimTime, targetLabel);
	}
	else
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

// Function to change the parameters of a specified bond type held in a
// command target. The bonds are part of a polymer type that is held
// by a CCommandTargetPolymer. Note that the bonds may occur in more than
// one polymer type and, if the target is composite, this could result
// in bonds beind modified in several polymer types.

void CSimBox::SetBondStrengthInTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

	const ctSetBondStrengthInTarget* const pCmd = dynamic_cast<const ctSetBondStrengthInTarget*>(pCommand);

	// Unpack the data identifying the bond type that is changed by the command 
	// and the new values of its spring constant and unstretched length.
	// If the simulation type is MD we normalise the spring constant by the
	// energy scale taken from the first species' LJ potential depth, and the 
	// lengths by the first LJ potential range.

	const zString label			= pCmd->GetTargetLabel();
	const zString bondName		= pCmd->GetName();
	const double springConstant	= pCmd->GetSpringConstant();
	const double unStrLength	= pCmd->GetUnstretchedLength();

	const long bondType	= GetBondTypeFromName(bondName);

	// Get the command target from the target list

	CCommandTargetNode* pCmdTarget = GetCommandTarget(label);

	// Now we have the target, check it is not composite, get the 
	// polymers contained in it and from them obtain the specified bonds;
	// if no bonds are found issue an error message.

	if(pCmdTarget && !pCmdTarget->IsComposite())
	{

#if SimIdentifier == MD
		springConstant *= (GetLengthScale()*GetLengthScale()/GetEnergyScale());
		unStrLength    /= GetLengthScale();
#endif

		PolymerVector vTargetPolymers = pCmdTarget->GetPolymers();

		BondVector vTargetBonds;
		vTargetBonds.clear();

		for(PolymerVectorIterator iterPoly=vTargetPolymers.begin(); iterPoly!=vTargetPolymers.end(); iterPoly++)
		{
			BondVector vBonds = (*iterPoly)->GetBonds();
			vTargetBonds.insert(vTargetBonds.end(), vBonds.begin(), vBonds.end());
		}

		// Count the number of bonds whose parameters are changed to enable
		// an error message if no bonds of the specified type are found

		long bondsChanged = 0;

		for(BondVectorIterator iterBond=vTargetBonds.begin(); iterBond!=vTargetBonds.end(); iterBond++)
		{
			if((*iterBond)->GetType() == bondType)
			{
				bondsChanged++;
				(*iterBond)->SetSpringConstant(springConstant);
				(*iterBond)->SetUnStretchedLength(unStrLength);
			}
		}

//		cout << "Changing parameters for " << bondsChanged << " bonds out of " << vTargetBonds.size() << " in " << vTargetPolymers.size() << " polymers" << zEndl;

		if(bondsChanged > 0)
		{
			// Create a message indicating that the bond parameters have changed

			new CLogctSetBondStrength(m_SimTime, label, bondName, bondType, bondsChanged, springConstant, unStrLength);
		}
		else
		{
			new CLogCommandFailed(m_SimTime, pCmd);
		}
	}
	else	// Log a command failed message
	{
		new CLogCommandFailed(m_SimTime, pCmd);
	}

#endif
}

void CSimBox::SetBondStrengthByPositionInTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

#endif
}

// Function to change the parameters of a specified bond pair type held in a
// command target. The bondpairs are part of a polymer type that is held
// by a CCommandTargetPolymer. Note that the bondspairs may occur in more than
// one polymer type and, if the target is composite, this could result
// in bondpairs beind modified in several polymer types.

void CSimBox::SetBondStiffnessInTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

    const ctSetBondStiffnessInTarget* const pCmd = dynamic_cast<const ctSetBondStiffnessInTarget*>(pCommand);

    // Unpack the data identifying the bondpair type that is changed by the command
    // and the new values of its strength and preferred angle.
    // If the simulation type is MD we normalise the strength by the
    // energy scale taken from the first species' LJ potential depth.

    const zString label            = pCmd->GetTargetLabel();
    const zString bondPairName     = pCmd->GetName();
    const double modulus           = pCmd->GetBendingModulus();
    const double phi0              = pCmd->GetPhi0();

    const long bondPairType        = GetBondPairTypeFromName(bondPairName);

    // Get the command target from the target list

    CCommandTargetNode* pCmdTarget = GetCommandTarget(label);

    // Now we have the target, check it is not composite, get the
    // polymers contained in it and from them obtain the specified bonds;
    // if no bonds are found issue an error message.

    if(pCmdTarget && !pCmdTarget->IsComposite())
    {

#if SimIdentifier == MD
        modulus /= GetEnergyScale();
#endif

        PolymerVector vTargetPolymers = pCmdTarget->GetPolymers();

        BondPairVector vTargetBondPairs;
        vTargetBondPairs.clear();

        for(PolymerVectorIterator iterPoly=vTargetPolymers.begin(); iterPoly!=vTargetPolymers.end(); iterPoly++)
        {
            BondPairVector vBondPairs = (*iterPoly)->GetBondPairs();
            vTargetBondPairs.insert(vTargetBondPairs.end(), vBondPairs.begin(), vBondPairs.end());
        }

        // Count the number of bondpairs whose parameters are changed to enable
        // an error message if no bonds of the specified type are found

        long bondsChanged = 0;

        for(BondPairVectorIterator iterBond=vTargetBondPairs.begin(); iterBond!=vTargetBondPairs.end(); iterBond++)
        {
            if((*iterBond)->GetType() == bondPairType)
            {
                bondsChanged++;
                (*iterBond)->SetModulus(modulus);
                (*iterBond)->SetPhi0(phi0);
            }
        }

        if(bondsChanged > 0)
        {
            // Create a message indicating that the bondpair parameters have changed

            new CLogctSetBondStiffness(m_SimTime, label, bondPairName, bondPairType, bondsChanged, modulus, phi0);
        }
        else
        {
            new CLogCommandFailed(m_SimTime, pCmd);
        }
    }
    else    // Log a command failed message
    {
        new CLogCommandFailed(m_SimTime, pCmd);
    }
#endif
}

void CSimBox::SetBondStiffnessByPositionInTarget(const xxCommand* const pCommand)
{
#if EnableTargetCommand == SimCommandEnabled

#endif
}


#if EnableShadowSimBox == SimACNEnabled

// Function to return a pointer to the shadow SimBox that handles active networks.

aeActiveSimBox* CSimBox::GetActiveSimBox() const
{
	return m_pShadow;
}

#endif

// ****************************************
// Set of commands that must be implemented directly in the SimBox in order
// to work properly on some platforms (e.g., gcc or Intel 9.1 on linux).
// On other platforms, the commands are implemented in separate classes that
// are virtual bases of the SimBox.

#if EnableDirectImplCommand == SimCommandEnabled

// Command handler function to stop the simulation gracefully.

void CSimBox::Stop(const xxCommand *const pCommand)
{
	const ccStop* const pCmd = dynamic_cast<const ccStop*>(pCommand);

    m_TotalTime = GetCurrentTime();

    SaveCurrentState();
    SaveRestartState();

    new CLogStopSimulation(GetCurrentTime(), m_TotalTime);
}

// Command handler function to stop the simulation immediately without 
// saving any files.

void CSimBox::StopNoSave(const xxCommand *const pCommand)
{
	const ccStopNoSave* const pCmd = dynamic_cast<const ccStopNoSave*>(pCommand);

    m_TotalTime = GetCurrentTime();

    new CLogStopNoSaveSimulation(GetCurrentTime(), m_TotalTime);
}

// Command handler function to toggle on and off the bead-bead conservative
// forces. This allows a dissipative ideal gas to be simulated.

void CSimBox::ToggleDPDBeadConservativeForces(const xxCommand *const pCommand)
{
	const ccToggleDPDBeadConservativeForces* const pCmd = dynamic_cast<const ccToggleDPDBeadConservativeForces*>(pCommand);

    bool bZeroForce = IModifyIntegration()->ToggleDPDBeadConservativeForces();

    new CLogToggleDPDBeadConservativeForces(GetCurrentTime(), bZeroForce);
}

// Command handler function to toggle on and off all bead-bead forces.
// This creates an ideal gas (apart from bond forces).

void CSimBox::ToggleDPDBeadForces(const xxCommand *const pCommand)
{
	const ccToggleDPDBeadForces* const pCmd = dynamic_cast<const ccToggleDPDBeadForces*>(pCommand);

    bool bZeroForce = IModifyIntegration()->ToggleDPDBeadForces();

    new CLogToggleDPDBeadForces(GetCurrentTime(), bZeroForce);
}

// Command handler function to toggle on and off all bead-bead forces.
// This creates an ideal gas (apart from bond forces).

void CSimBox::ToggleDPDBeadThermostat(const xxCommand *const pCommand)
{
	const ccToggleDPDBeadThermostat* const pCmd = dynamic_cast<const ccToggleDPDBeadThermostat*>(pCommand);

    bool bZeroForce = IModifyIntegration()->ToggleDPDBeadThermostat();

    new CLogToggleDPDBeadThermostat(GetCurrentTime(), bZeroForce);
}


#endif

// End of directly implemented command set
// ****************************************
// Handler function to return the number of active command groups; this is
// the number of groups that currently have a sequential execution pattern.

long CSimBox::CountActiveCommandGroups() const
{
#if EnableCommandGroups == SimCommandEnabled
    return m_ActiveCommandTargets.size();
#else
    return 0;
#endif
}

// Handler function to return the total number of command groups. This
// includes all groups whether in the active set or not.

long CSimBox::CountCommandGroups() const
{
#if EnableCommandGroups == SimCommandEnabled
    return m_CommandTargets.size();
#else
    return 0;
#endif
}

// Handler function to create an empty command group and store it for later
// execution. It is called directly from the command class (without using an
// implementation class) as its function is strongly bound to the SimBox.
// If the command groups feature is disabled, we just return false.

bool CSimBox::CreateCommandGroup(const zString name)
{
#if EnableCommandGroups == SimCommandEnabled

	bool bExists = false;

	for(cCommandGroupIterator citerGroup=m_CommandGroups.begin(); citerGroup!=m_CommandGroups.end(); citerGroup++)
	{
		if((*citerGroup)->GetName() == name)
			bExists = true;
	}

	if(!bExists)
	{
        tguCommandGroup* const pGroup = new tguCommandGroup(name);
		m_CommandGroups.push_back(pGroup);
		return true;
	}
	else
	{
		return false;
	}

#else
    return false;
#endif
}

// Handler function to delete a named command group. If the group exists, it is removed
// from the container, its destructor is called and the function returns true; if the
// group is not found, the function returns false.

bool CSimBox::DeleteCommandGroup(const zString name)
{
#if EnableCommandGroups == SimCommandEnabled

	bool bNotFound = true;

    CommandGroupIterator iterGroup=m_CommandGroups.begin();

	while(bNotFound && iterGroup!=m_CommandGroups.end())
	{
		if((*iterGroup)->GetName() == name)
        {
			bNotFound = false;
            tguCommandGroup* const pGroup = *iterGroup;
            m_CommandGroups.erase(iterGroup);
            delete pGroup;
        }
        iterGroup++;
	}

	return !bNotFound;

#else
    return false;
#endif
}

// Handler function to add an existing command group to the active set.
// This causes the group to be executed repeatedly according to its contained
// execution pattern. Typically, this results in the group being executed at
// a fixed period until the user stops it. The group remains in the command groups
// container, and a pointer to it is added to the active command groups container.
//
// If the specified group exists and is not in the active container, it is added
// and the function returns true; if the group is already in the active container
// the function still returns true; but if the group is not found, the function
// returns false.

bool CSimBox::AddActiveCommandGroup(const zString name)
{
#if EnableCommandGroups == SimCommandEnabled

    bool bFound = false;
	tguCommandGroup* pGroup = 0;
    CommandGroupIterator iterGroup=m_CommandGroups.begin();

	while(!pGroup && iterGroup!=m_CommandGroups.end())
	{
		if((*iterGroup)->GetName() == name)
        {
            bFound = true;
            pGroup = *iterGroup;
        }
        iterGroup++;
	}

    // We found the group so add it to the active set if it is not already there,
    // otherwise ignore it.

    bool bNotFound = true;
    iterGroup = m_ActiveCommandGroups.begin();

    while(bNotFound && iterGroup != m_ActiveCommandGroups.end())
    {
		if((*iterGroup)->GetName() == name)
        {
            bNotFound = false;
        }
        iterGroup++;
    }

    if(bNotFound)
    {
        m_ActiveCommandGroups.push_back(pGroup);
    }

	return bFound;

#else
    return false;
#endif
}

// Function to remove a command group from the active container. This does not
// delete the group itself, it just removes it from the set of active groups.
// If the group was active and is removed, the function returns true, otherwise
// it returns false.

bool CSimBox::RemoveActiveCommandGroup(const zString name)
{
#if EnableCommandGroups == SimCommandEnabled

	bool bNotFound = true;

    CommandGroupIterator iterGroup=m_ActiveCommandGroups.begin();

	while(bNotFound && iterGroup!=m_ActiveCommandGroups.end())
	{
		if((*iterGroup)->GetName() == name)
        {
			bNotFound = false;
            m_ActiveCommandGroups.erase(iterGroup);
        }
        iterGroup++;
	}

	return !bNotFound;

#else
    return false;
#endif
}

// Function to return a named command group. If no group is found, it returns a null pointer.
// Can this handle recursive command groups? How will it find a group that is contained
// within another group?

tguCommandGroup* CSimBox::GetCommandGroup(const zString name)
{
	tguCommandGroup* pGroup = 0;

#if EnableCommandGroups == SimCommandEnabled

    CommandGroupIterator iterGroup=m_CommandGroups.begin();

	while(!pGroup && iterGroup!=m_CommandGroups.end())
	{
		if((*iterGroup)->GetName() == name)
        {
            pGroup = *iterGroup;
        }
        iterGroup++;
	}

#endif

	return pGroup;
}

// Function to execute all active command groups. It iterates over the active command
// groups and checks if their Execute() function returns true. If it does, the group
// is left in for another execution, otherwise the group is removed from the container
// as it means that either its execution pattern has ended or there was an error
// instantiating one or more of its commands. Both cases requires that the group
// be removed from the active group container.

void CSimBox::ExecuteCommandGroups()
{
#if EnableCommandGroups == SimCommandEnabled

    CommandGroupIterator iterGroup=m_ActiveCommandGroups.begin();

	while(iterGroup!=m_ActiveCommandGroups.end())
	{
		if((*iterGroup)->TimeToExecute(GetCurrentTime()) && !(*iterGroup)->Execute(this))
        {
            iterGroup = m_ActiveCommandGroups.erase(iterGroup);
        }
        else
        {
            ++iterGroup;
        }
	}

#endif
}

#if EnableParallelSimBox == SimMPSEnabled

// Function to return a pointer to the parallel SimBox that handles 
// inter-processor communication.

mpsSimBox* CSimBox::GetParallelSimBox() const
{
    return m_pParallel;
}

#endif

// Private function to return a container of all beads held in the CNT cells.
// This is used to avoid having to store a container of bead pointers that
// must be updated continually during a parallel run. As reconstructing the
// container is expensive, it should only be used in commands or other infrequently
// executed functions.

AbstractBeadVector CSimBox::GetAllBeadsInCNTCells()
{
    AbstractBeadVector vBeads;
    vBeads.clear();

	for(CNTCellIterator iterCell=m_vCNTCells.begin(); iterCell!=m_vCNTCells.end(); iterCell++)
	{
        BeadList lBeads = (*iterCell)->GetBeads();
        copy(lBeads.begin(), lBeads.end(), back_inserter(vBeads));
    }

    return vBeads;
}

// Private helper function to return a pointer to the CNT cell containing a given point.
// If the coordinates are within the SimBox we return a pointer to the cell, otherwise we return null.

CStressGridCell* CSimBox::GetStressCellFromCoords(double r[3]) const
{
    CStressGridCell* pCell = 0;
	
#if EnableStressTensorSphere == SimMiscEnabled
	const long index = m_StressCellXNo*(m_StressCellYNo*static_cast<long>(r[2]/m_StressCellZWidth) + static_cast<long>(r[1]/m_StressCellYWidth)) + static_cast<long>(r[0]/m_StressCellXWidth);

	if(index < m_StressCellTotal)
	{
		pCell = m_vStressCells[index];
	}
#endif
		
    return pCell;
}


// Function to construct the grid of cells that holds the local stress tensor in curvilinear coordinates.
// This feature is normally not compiled in as it is extremely expensive to calculate. If it is needed, 
// the codebase must be recompiled with the compilation flag enabled, and the vesicle aggregate analysis 
// turned on.
// Note that the size of the grid is set by the const parameter m_StressCellMultiplier that specifies the number of 
// stress cells per CNT cell in each dimension in the constructor.

void CSimBox::MakeStressCells()
{
#if EnableStressTensorSphere == SimMiscEnabled

	long index, coords[3];
	bool flag;

	CStressGridCell* pCell;

	for( long k=0; k<m_StressCellZNo; k++ )
	{
		for( long j=0; j<m_StressCellYNo; j++ )
		{
			for( long i=0; i<m_StressCellXNo; i++ )
			{
				index = m_StressCellXNo*(m_StressCellYNo*k+j) + i;
				coords[0] = i;
				coords[1] = j;
				coords[2] = k;

				pCell = new CStressGridCell(index, coords, m_StressCellXWidth, m_StressCellYWidth, m_StressCellZWidth);
				m_vStressCells.push_back(pCell);
			}
		}
	}
#endif
}

// Private helper function to add a given stress tensor to each stress grid cell along a line joining two given points.
//
// The equation of the line is:  r(lambda) = rp + lambda*(rq - rp),  where lambda = 0 to 1
//
// We divide the line into a fixed number of segments and add the stress to the cells that contain each segment.  We include both end-points 
// explicitly so the actual number of segments is one less than the number of points.
//
// Double-counting is prevented by storing each CNT cell as we add the stress and only adding the stress to a cell if it is different from the old one. As we are
// iterating over a straight line that is shorter than the size of the simulation box this is ok.

void CSimBox::AddWeightedStressToLinearCells(double rp[3], double rq[3], double stress[9])
{
#if EnableStressTensorSphere == SimMiscEnabled

    long i;    // counter used several times below
    for(i=0; i<3; i++)
	{
	    m_rprime[i] = rp[i];
	    m_dr[i]     = (rq[i] - rp[i])*m_InversePointTotalMinusOne;
	}
	
	m_pOldCell = GetStressCellFromCoords(rp);
	m_pNewCell = GetStressCellFromCoords(rq);
			
	if(m_pNewCell != m_pOldCell)
	{
//		std::cout << " ******************** " << zEndl;
//		std::cout << "Starting iteration over vector joining cells " << m_pOldCell->GetId() << " and " << m_pNewCell->GetId() << zEndl;
		
	    m_StressWeight  = 0.0;
		
        for(i=0; i<m_PointTotal; i++)
	    {        
	        m_pNewCell = GetStressCellFromCoords(m_rprime);
		
		    if(m_pNewCell != m_pOldCell)
		    {
		        // Weight the stress tensor by the old cell's fraction of the vector and add it to the local stress tensor;
			    // then reset the weight counter
			    for(long j=0; j<9; j++)
			    {
			        m_LocalStress[j] = m_StressWeight*stress[j];
			    }
			
//			    std::cout << " Adding stress to cell " << m_pOldCell->GetId() << " with weight " << m_StressWeight << zEndl;
//				std::cout << m_LocalStress[0] << " " << m_LocalStress[1] << " " << m_LocalStress[2] << zEndl;
//				std::cout << m_LocalStress[3] << " " << m_LocalStress[4] << " " << m_LocalStress[5] << zEndl;
//				std::cout << m_LocalStress[6] << " " << m_LocalStress[7] << " " << m_LocalStress[8] << zEndl;
				
		        m_pOldCell->AddStress(m_LocalStress);
			    m_pOldCell = m_pNewCell;
			    m_StressWeight = m_InversePointTotal;
		    }
		    else
		    {
		        m_StressWeight += m_InversePointTotal;
		    }
	
	        m_rprime[0] += m_dr[0];
	        m_rprime[1] += m_dr[1];
	        m_rprime[2] += m_dr[2];
	    }
	
	    // Add the stress to the final cell on the vector as it will not have been done inside the loop
	
	    for(i=0; i<9; i++)
	    {
		    m_LocalStress[i] = m_StressWeight*stress[i];
	    }
		
//		std::cout << " Adding stress to cell " << m_pOldCell->GetId() << " with weight " << m_StressWeight << zEndl;
//		std::cout << m_LocalStress[0] << " " << m_LocalStress[1] << " " << m_LocalStress[2] << zEndl;
//		std::cout << m_LocalStress[3] << " " << m_LocalStress[4] << " " << m_LocalStress[5] << zEndl;
//		std::cout << m_LocalStress[6] << " " << m_LocalStress[7] << " " << m_LocalStress[8] << zEndl;
	    m_pOldCell->AddStress(m_LocalStress);
	}
	else
	{
	    // Both points are in the same cell so we add the whole stress
		
	    m_pOldCell->AddStress(stress);	
	}
		
#endif
}


// Function to add the stress from each bond instance to the local stress stored in each CNT cell along the bond's line of force. 
// We assume that the vesicle does not cross the PBCs so we do not correct for them. This means that this routine can only be used when
// it is known that no bonds cross the PBCs.

void CSimBox::AddBondStressSphere(const CBond *const pBond)
{
#if EnableStressTensorSphere == SimMiscEnabled

    double rp[3], rq[3];
	double dx[3], force[3], fdx[9];
	
	rp[0] = pBond->GetTail()->GetXPos();
	rp[1] = pBond->GetTail()->GetYPos();
	rp[2] = pBond->GetTail()->GetZPos();

	rq[0] = pBond->GetHead()->GetXPos();
	rq[1] = pBond->GetHead()->GetYPos();
	rq[2] = pBond->GetHead()->GetZPos();

	dx[0] = rq[0] - rp[0];
	dx[1] = rq[1] - rp[1];
	dx[2] = rq[2] - rp[2];
	
	// No minus sign needed here as we have converted from the gradient of the potential to the force, which removes the minus sign in Eqn. B13 of Goetz and Lipowsky, 
	// JCP 108:7397 (1998).

	force[0] = pBond->GetXForce();
	force[1] = pBond->GetYForce();
	force[2] = pBond->GetZForce();
		
	fdx[0] = dx[0]*force[0];
	fdx[1] = dx[1]*force[0];
	fdx[2] = dx[2]*force[0];
	fdx[3] = dx[0]*force[1];
	fdx[4] = dx[1]*force[1];
	fdx[5] = dx[2]*force[1];
	fdx[6] = dx[0]*force[2];
	fdx[7] = dx[1]*force[2];
	fdx[8] = dx[2]*force[2];
		
	AddWeightedStressToLinearCells(rp, rq, fdx);

#endif		
}

// Function used by the Monitor to add the stress from each bondpair instance to the stress grid cell's local store.  We assume that the stiff bonds 
// do not cross the PBCs, so we do not correct for them. This means that this routine can only be used when it is known that no bonds cross the PBCs.

void CSimBox::AddBondPairStressSphere(const CBondPair *const pBondPair)
{
#if EnableStressTensorSphere == SimMiscEnabled

	// Copy the bead pointers into a local array and repeat the first pointer at 
	// the end to make the calculation of adjacent pairs simpler.

	CAbstractBead* pBeadArray[4];

	pBeadArray[0] = pBondPair->GetFirst()->GetTail();
	pBeadArray[1] = pBondPair->GetFirst()->GetHead();
	pBeadArray[2] = pBondPair->GetSecond()->GetHead();
	pBeadArray[3] = pBondPair->GetFirst()->GetTail();
	
	CBond* pHeadBond = pBondPair->GetFirst();
	CBond* pTailBond = pBondPair->GetSecond();

	for(long j=0; j<3; j++)
	{
        const long jp1 = j+1;

        double rp[3], rq[3];
		double dx[3], force[3], fdx[9];
		
		rp[0] = pBeadArray[j]->GetunPBCXPos();
		rp[1] = pBeadArray[j]->GetunPBCYPos();
		rp[2] = pBeadArray[j]->GetunPBCZPos();
		
		rq[0] = pBeadArray[jp1]->GetunPBCXPos();
		rq[1] = pBeadArray[jp1]->GetunPBCYPos();
		rq[2] = pBeadArray[jp1]->GetunPBCZPos();

		dx[0] =  rq[0] - rp[0];
		dx[1] =  rq[1] - rp[1];
		dx[2] =  rq[2] - rp[2];

        // No minus sign needed here as we have already changed the order of the beads whose forces are subtracted from each each, 
		// which removes the minus sign in Eqn. B13 of the Goetz and Lipowsky references above.
		
		force[0] = 0.33333333*(pBondPair->GetXForce(jp1) - pBondPair->GetXForce(j));
		force[1] = 0.33333333*(pBondPair->GetYForce(jp1) - pBondPair->GetYForce(j));
		force[2] = 0.33333333*(pBondPair->GetZForce(jp1) - pBondPair->GetZForce(j));
	
	    fdx[0] = dx[0]*force[0];
	    fdx[1] = dx[1]*force[0];
	    fdx[2] = dx[2]*force[0];
	    fdx[3] = dx[0]*force[1];
	    fdx[4] = dx[1]*force[1];
	    fdx[5] = dx[2]*force[1];
	    fdx[6] = dx[0]*force[2];
	    fdx[7] = dx[1]*force[2];
	    fdx[8] = dx[2]*force[2];
						
		AddWeightedStressToLinearCells(rp, rq, fdx);
    }
	
#endif
}

// Function to set the origin for the transformation of the stress tensor in each stress grid cell from Cartesian to spherical polar coordinates.
// It is used, for example, by the CVesicle aggregate class.

void CSimBox::SetStressSphereOrigin(double rcm[3])
{
#if EnableStressTensorSphere == SimMiscEnabled
	m_StressOrigin[0] = rcm[0];
	m_StressOrigin[1] = rcm[1];
	m_StressOrigin[2] = rcm[2];
#endif
}

// Function used by the Monitor to calculate the local stress tensor for each stress grid cell. It is conditionally compiled because the calculation 
// is very expensive.  The function transforms the stress tensor that has already been calculated in Cartesian coordinates, so it must be called 
// after the original tensor has been calculated.
//
// As a check, we sum the stress over the whole SimBox and return the tensor in the argument. We assume this array has been zeroed in the calling routine.

void CSimBox::TransformStressSphere(double stress[9])
{
#if EnableStressTensorSphere == SimMiscEnabled
		
	for(StressCellIterator iterCell=m_vStressCells.begin(); iterCell!=m_vStressCells.end(); iterCell++)
	{
		stress[0] += (*iterCell)->GetStressComponent(0);
		stress[1] += (*iterCell)->GetStressComponent(1);
		stress[2] += (*iterCell)->GetStressComponent(2);
		stress[3] += (*iterCell)->GetStressComponent(3);
		stress[4] += (*iterCell)->GetStressComponent(4);
		stress[5] += (*iterCell)->GetStressComponent(5);
		stress[6] += (*iterCell)->GetStressComponent(6);
		stress[7] += (*iterCell)->GetStressComponent(7);
		stress[8] += (*iterCell)->GetStressComponent(8);
		
		(*iterCell)->TransformStressToSphCoords(m_StressOrigin);		
	}

#endif
}

// PVF that moves a bead instance from its current CNT cell to another one specified by the new coordinates passed in
// as arguments. We check that the bead pointer is not null and that the new coordinates lie within the simulation box.
// The calling routine must check the return value to see if the move succeeded.
//
// Parallel implementation notes. In a parallel simulation there are multiple SimBox spaces and we need to check 
// whether the bead is being moved within the owning processor's space or elsewhere. For now (4/12/12) 
// we only allow displacements within the owning processor's space.

bool CSimBox::MoveBeadBetweenCNTCells(CAbstractBead* const pBead, double x, double y, double z) const
{
    bool bValid = true;
    
#if EnableParallelSimBox == SimMPSEnabled
    
    // Parallel branch not enabled yet
    
#else
    
	if(pBead)
    {
        // Get current position and the index to the owning CNT cell
        
        long ix, iy, iz;
        
        ix = static_cast<long>(pBead->GetXPos()/m_CNTXCellWidth);
        iy = static_cast<long>(pBead->GetYPos()/m_CNTYCellWidth);
        iz = static_cast<long>(pBead->GetZPos()/m_CNTZCellWidth);
        
        const long oldIndex = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix;
        
        // Now get the new position and the index to the new CNT cell
        
        ix = static_cast<long>(x/m_CNTXCellWidth);
        iy = static_cast<long>(y/m_CNTYCellWidth);
        iz = static_cast<long>(z/m_CNTZCellWidth);
        
        const long newIndex = m_CNTXCellNo*(m_CNTYCellNo*iz+iy) + ix; 
        
        // Validate the indices 
        
        if(oldIndex >= 0 && oldIndex < m_CNTCellTotal && newIndex >= 0 && newIndex < m_CNTCellTotal)
        {
		    pBead->SetXPos(x);
		    pBead->SetYPos(y);
		    pBead->SetZPos(z);
		
            m_vCNTCells[oldIndex]->RemoveBeadFromCell(pBead);
            m_vCNTCells[newIndex]->AddBeadtoCell(pBead);
        }
        else
        {
            bValid = false;
        }
    }
    else
    {
        return false;
    }
#endif
    
    return bValid;
}


#if EnableParallelSimBox == SimMPSEnabled
// Access functions to the  parallel SimBox that need to be accessed by other classes. Because they are all only used in the 
// parallel simulation we conditionally compile them.

// Function to return a container of all the polymers in the owning processor's Bulk space. This does not include polymers that span the 
// processor's boundaries and also does not include any single-bead polymers.

PolymerVector CSimBox::GetAllBulkPolymers() const
{
    return m_pParallel->GetAllBulkPolymers();
}
    
// Function to return a container holding all polymers that have any beads within the specified 
// rectangular region. We ensure that the region lies wholly inside the Bulk region of the SimBox. First, we find the CNT cell 
// that contains the central point and then add all cells until we have covered the given region. Then we retrieve the beads 
// that lie within this region and their owning polymers.
//
// This function only works in the parallel code as it needs to be able to retrieve a polymer instance from a bead instance, and 
// this function is only compiled in for the parallel code.

PolymerVector CSimBox::GetPolymersInRectangularRegion(double x, double y, double z, double rx, double ry, double rz)
{
    // Find out how many CNT cells in each dimension we need and add them to the set of cells.
    // We divide the region into two halves along each dimension, and count the number of cells in each half.
    
    const long nx = 1 + 2*static_cast<long>(rx/m_CNTXCellWidth);
    const long ny = 1 + 2*static_cast<long>(ry/m_CNTYCellWidth);
    const long nz = 1 + 2*static_cast<long>(rz/m_CNTZCellWidth);
    
    // Find the CNT cell that holds the central point

    double r[3];
    r[0] = x;
    r[1] = y;
    r[2] = z;

 //   CCNTCell* const pCentralCell = GetCNTCellFromCoords(r);

    // And find the CNT cell at the origin (bottom left) of the rectangular region.

    r[0] = x - rx;
    r[1] = y - ry;
    r[2] = z - rz;

    CCNTCell* pPlanarOriginCell = GetCNTCellFromCoords(r);

    // Create containers for all the cells, for a row of cells in the X direction, and a plane of cells in the XY plane.

    CNTCellVector vCells;
    vCells.clear();

    CNTCellVector vXCells;
    vXCells.clear();

    CCNTCell* pRowOriginCell   = pPlanarOriginCell;
    CCNTCell* pNextCell        = pRowOriginCell;

    for(long iz = 0; iz < nz; ++iz)
    {
        vXCells.clear();
       
        for(long iy = 0; iy < ny; ++iy)
        {
            vXCells.clear();
        
            for(long ix = 0; ix < nx; ++ix)
            {
                vXCells.push_back(pNextCell);
                pNextCell = pNextCell->GetNNCell(14);
            }
        
            copy(vXCells.begin(), vXCells.end(), back_inserter(vCells));
        
            // Move the row origin cell one cell up in the Y direction
            pRowOriginCell = pRowOriginCell->GetNNCell(16);
            pNextCell = pRowOriginCell;
        }
        
        //Move the planar origin cell one cell up in Z direction and reset the row origin cell to it
        pPlanarOriginCell = pPlanarOriginCell->GetNNCell(22);
        pRowOriginCell = pPlanarOriginCell;
        pNextCell = pRowOriginCell;
    }

    // Get all beads in the selected CNT cells 

    AbstractBeadVector vBeads;
    vBeads.clear();
    
	for(CNTCellIterator iterCell=vCells.begin(); iterCell!=vCells.end(); iterCell++)
	{
        BeadList lBeads = (*iterCell)->GetBeads();
        copy(lBeads.begin(), lBeads.end(), back_inserter(vBeads));
    }
    
    // Now get the polymers from the beads. We should ensure that only a single instance of each polymer is kept in case 
    // a polymer consists of more than one bead. But for Water beads, which only have a single bead, this is not necessary.
    
    PolymerVector vPolymers;
    vPolymers.clear();
    
    for(AbstractBeadVectorIterator iterBead=vBeads.begin(); iterBead!=vBeads.end(); iterBead++)
    {
//        std::cout << "Testing bead " << (*iterBead)->GetId() << " with owning polymer " << (*iterBead)->GetPolymer()->GetId() << zEndl;
//        if(find(vPolymers.begin(), vPolymers.end(), (*iterBead)->GetPolymer()) == vPolymers.end())
        {
            vPolymers.push_back((*iterBead)->GetPolymer());
        }
    }

    // Return the polymers in a vector
    
    return vPolymers;
}
    
#endif

// Function to create a new CNanoparticle instance wrapped around a set of CBond instances. The nanoparticles are identified by 
// a unique integer that is just the number of nanoparticles created. Note that there is no way to modify a nanoparticle after 
// creation, unlike for Command Targets. Hence, we do not provide functions to delete or alter nanoparticles.

bool CSimBox::CreateNanoparticle(BondVector vBonds)
{
    CNanoparticle* pNanoparticle = new CNanoparticle(vBonds);
    
    if(pNanoparticle)
    {
        m_Nanoparticles.push_back(pNanoparticle);
        
#if EnableParallelSimBox == SimMPSEnabled
        // If this is a parallel run, we copy the nanoparticle to the mpsSimBox so that it can calculate the bond forces 
        // that hold the nanoparticle together when it crosses the processor boundaries.
        
        m_pParallel->AddNanoparticle(pNanoparticle);        
#endif        
        return true;
    }
    else
    {
        return false;
    }
}

// Function to return a CNanoparticle instance given its (unique) id.

CNanoparticle* CSimBox::GetNanoparticle(long id)
{
    CNanoparticle* pNanoparticle = 0;
    
	for(NanoparticleIterator iterNano=m_Nanoparticles.begin(); iterNano!=m_Nanoparticles.end(); iterNano++)
	{
		if((*iterNano)->GetId() == id)
		{
			pNanoparticle = *iterNano;
		}
	}

	return pNanoparticle;
}

// Function to return the number of nanoparticles created so far.

long CSimBox::GetNanoparticleTotal() const 
{
    return m_Nanoparticles.size();
}
