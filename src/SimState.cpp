/* **********************************************************************
Copyright 2020  Dr. J. C. Shillcock and Prof. Dr. R. Lipowsky, Director at the Max Planck Institute (MPI) of Colloids and Interfaces; Head of Department Theory and Bio-Systems.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************** */
// SimState.cpp: implementation of the CSimState class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SimDefs.h"
#include "ExperimentDefs.h"
#include "SimState.h"
#include "ISimulation.h"
#include "CNTCell.h"
#include "InputData.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Constructor for a CSimState object for use when the simulation is
// part of an experiment. The ISimulation interface is used to pass data 
// and control out to the simulation and experiment objects during the run.
//

CSimState::CSimState(ISimulation* pISim, const CInputData& rInputData) : m_pISimulation(pISim),
													m_rAnalysisState(rInputData),
													m_rInitialState(this, rInputData),
													m_rHistoryState(rInputData),
													m_rLogState(rInputData),
													m_RunId(rInputData.GetRunId()),
													m_Title(rInputData.GetTitle()),
													m_Date(rInputData.GetDate()),
													m_Comment(rInputData.GetComment()),
													m_ISType(m_rInitialState.GetInitialStateType()),
													m_bIsWallOn(rInputData.IsWallPresent()),
													m_bIsGravityOn(rInputData.IsGravityPresent()),
													m_bIsShearOn(rInputData.IsShearPresent()),
													m_bIsActiveBondsOn(false),
													m_bIsBeadChargeOn(false),
													m_bIsDPDBeadConsForceZero(false),
													m_bIsDPDBeadForceZero(false),
													m_bIsDPDBeadThermostatZero(false),
													m_bIsBeadStressAdded(true),
													m_bIsBeadChargeStressAdded(false),
													m_bIsBondStressAdded(true),
													m_bIsBondPairStressAdded(true),
													m_bEnergyOutput(false),
													m_bRenormaliseMomenta(false)
{
	// Initialise the random number generator prior to any possible use. A valid 
	// CSimulation object must have access to a sequence of random numbers that
	// are quasi-random but reproducible between independent simulations so that 
	// the results are reproducible. We use static member variables of the 
	// CNTCell class because that is where most random numbers are used and we want
	// fast access to the RNG. Certain initial states, and the distribution of beads
	// in a wall, need random numbers before the simulation proper begins.

	CCNTCell::SetRNGSeed(rInputData.GetRNGSeed());
	
	CCNTCell::SetTimeStepConstants(rInputData.GetStepSize(), 
								   rInputData.GetLambda(), 
								   rInputData.GetCutOffRadius(), 
								   rInputData.GetkT());

	CCNTCell::SetSimBoxLengths( rInputData.GetCNTXCellNo(), 
								rInputData.GetCNTYCellNo(), 
								rInputData.GetCNTZCellNo(),
								rInputData.GetCNTXCellWidth(),
								rInputData.GetCNTYCellWidth(), 
								rInputData.GetCNTZCellWidth() );

	// Store the bead-bead interaction parameters in the CCNTCell class
	// so that the integration scheme has efficient access to them when
	// calculating the forces between beads.

#if SimIdentifier == BD
// We use the same forces for BD as in DPD for now
		SetBDBeadIntArrays(rInputData.GetDPDBeadConsIntArray(), 
						   rInputData.GetDPDBeadDissIntArray());

#elif SimIdentifier == DPD

// We distringuish between a DPD simulation of a liquid-gas system and
// the standard liquid system. Even if the LG system is enabled, the input
// may be just the standard DPD one.

#if EnableDPDLG == ExperimentEnabled

        if(rInputData.IsDPDLG())
        {
            SetDPDBeadIntArrays(rInputData.GetDPDBeadConsIntArray(), 
							    rInputData.GetDPDBeadDissIntArray(),
                                rInputData.GetDPDBeadLGIntArray());
        }
        else
        {
            SetDPDBeadIntArrays(rInputData.GetDPDBeadConsIntArray(), 
							    rInputData.GetDPDBeadDissIntArray());
        }
#elif EnableDPDLG == ExperimentDisabled
        SetDPDBeadIntArrays(rInputData.GetDPDBeadConsIntArray(), 
							rInputData.GetDPDBeadDissIntArray());
#endif

#elif SimIdentifier == MD

        SetMDBeadIntArrays(rInputData.GetMDBeadLJDepthArray(), 
						   rInputData.GetMDBeadLJRangeArray(),
						   rInputData.GetMDBeadSCDepthArray(), 
						   rInputData.GetMDBeadSCRangeArray())

#endif


	// If we are doing an MD simulation then set the momenta renormalisation on, 
	// but turn it off for DPD. The default case can be overridden by using
	// a RenormaliseMomenta command.

#if SimIdentifier == BD
	m_bRenormaliseMomenta = false;
#elif SimIdentifier == DPD
	m_bRenormaliseMomenta = false;
#elif SimIdentifier == MD
	m_bRenormaliseMomenta = true;
#endif

}

// Constructor for a CSimState object for use during a stand-alone simulation.
// The ISimulation interface is unavailable during these runs.
//
// This object holds all the data needed to define the initial state of the 
// simulation. There should only be one call to this constructor because 
// it is used to pass data to the CCNTCell class that is needed to calculate 
// the bead-bead interactions throughout the simulation. The data is stored 
// in CCNTCell for speed of access as the loops over beads to calculate forces, 
// momenta and positions are performed there. 
//
// The CInputData object is used to initialise the analysis, initial, history
// and log state objects.
//
// The CInitialState object is given an interface pointer to this object to 
// allow it to build a new initial state when restarting a simulation. There
// are several types of restart state, and the following table shows which ones                                                    
// require access:
//
// CRestartState          - no access required; updates bead coordinates only.
//                          Bead, bond, bondpair and polymer types are unchangeable.
// CInclusiveRestartState - access required; updates bead coordinates, types,
//                          bond and bondpair data, command targets and decorators. 
//                          Polymers are unchangeable.
//
// Note that the contributions to the stress tensor analysis are turned on 
// by default, but because there are no charged beads by default, their 
// contribution is off.

CSimState::CSimState(const CInputData& rInputData) : m_pISimulation(0),
													m_rAnalysisState(rInputData),
													m_rInitialState(this, rInputData),
													m_rHistoryState(rInputData),
													m_rLogState(rInputData),
													m_RunId(rInputData.GetRunId()),
													m_Title(rInputData.GetTitle()),
													m_Date(rInputData.GetDate()),
													m_Comment(rInputData.GetComment()),
													m_ISType(m_rInitialState.GetInitialStateType()),
													m_bIsWallOn(rInputData.IsWallPresent()),
													m_bIsGravityOn(rInputData.IsGravityPresent()),
													m_bIsShearOn(rInputData.IsShearPresent()),
													m_bIsActiveBondsOn(false),
													m_bIsBeadChargeOn(false),
													m_bIsDPDBeadConsForceZero(false),
													m_bIsDPDBeadForceZero(false),
													m_bIsDPDBeadThermostatZero(false),
													m_bIsBeadStressAdded(true),
													m_bIsBeadChargeStressAdded(false),
													m_bIsBondStressAdded(true),
													m_bIsBondPairStressAdded(true),
													m_bEnergyOutput(false),
													m_bRenormaliseMomenta(false)
{
	// Initialise the random number generator prior to any possible use. A valid 
	// CSimulation object must have access to a sequence of random numbers that
	// are quasi-random but reproducible between independent simulations so that 
	// the results are reproducible. We use static member variables of the 
	// CNTCell class because that is where most random numbers are used and we want
	// fast access to the RNG. Certain initial states, and the distribution of beads
	// in a wall, need random numbers before the simulation proper begins.

	CCNTCell::SetRNGSeed(rInputData.GetRNGSeed());
	
	CCNTCell::SetTimeStepConstants(rInputData.GetStepSize(), 
								   rInputData.GetLambda(), 
								   rInputData.GetCutOffRadius(), 
								   rInputData.GetkT());

	CCNTCell::SetSimBoxLengths( rInputData.GetCNTXCellNo(), 
								rInputData.GetCNTYCellNo(), 
								rInputData.GetCNTZCellNo(),
								rInputData.GetCNTXCellWidth(),
								rInputData.GetCNTYCellWidth(), 
								rInputData.GetCNTZCellWidth() );

	// Store the bead-bead interaction parameters in the CCNTCell class
	// so that the integration scheme has efficient access to them when
	// calculating the forces between beads.

#if SimIdentifier == BD
// We use the same forces for BD as in DPD for now
		SetBDBeadIntArrays(rInputData.GetDPDBeadConsIntArray(), 
						   rInputData.GetDPDBeadDissIntArray());

#elif SimIdentifier == DPD

        SetDPDBeadIntArrays(rInputData.GetDPDBeadConsIntArray(), 
							rInputData.GetDPDBeadDissIntArray());

#elif SimIdentifier == MD

        SetMDBeadIntArrays(rInputData.GetMDBeadLJDepthArray(), 
						   rInputData.GetMDBeadLJRangeArray(),
						   rInputData.GetMDBeadSCDepthArray(), 
						   rInputData.GetMDBeadSCRangeArray())

#endif

	// If we are doing an MD simulation then set the momenta renormalisation on, 
	// but turn it off for DPD. The default case can be overridden by using
	// a RenormaliseMomenta command.

#if SimIdentifier == BD
	m_bRenormaliseMomenta = false;
#elif SimIdentifier == DPD
	m_bRenormaliseMomenta = false;
#elif SimIdentifier == MD
	m_bRenormaliseMomenta = true;
#endif

}

CSimState::~CSimState()
{

}

CAnalysisState& CSimState::GetAnalysisState()
{
	return m_rAnalysisState;
}

CInitialState& CSimState::GetInitialState()
{
	return m_rInitialState;
}

CHistoryState& CSimState::GetHistoryState()
{
	return m_rHistoryState;
}

CLogState& CSimState::GetLogState()
{
	return m_rLogState;
}

// Function to tell the CInitialState to assemble the beads, bonds and polymers
// defined in the CInputData object into the form specified by the CInitialStateData
// object. 

bool CSimState::Assemble()
{
	return m_rInitialState.Assemble();
}

// Function to tell the CInitialState to create the beads, bonds, polymers needed
// for the initial state on P0 and pass the numbers of each type to PN so that
// the other processors can create their pieces of the initial state.

bool CSimState::AssembleP()
{
	return m_rInitialState.AssembleP();
}

// Functions to update the SimState when the constraints on the SimBox are changed
// using xxCommand objects. 

void CSimState::SetAnalysisPeriod(long period)
{
	// Not implemented yet
}

void CSimState::SetDensityPeriod(long period)
{
	if(period > 0)
		m_rAnalysisState.SetDensityPeriod(period);
}

void CSimState::SetDisplayPeriod(long period)
{
	if(period > 0)
		m_rAnalysisState.SetDisplayPeriod(period);
}

void CSimState::SetSamplePeriod(long period)
{
	// not implemented yet
}

void CSimState::SetRestartPeriod(long period)
{
	if(period > 0)
		m_rAnalysisState.SetRestartPeriod(period);
}

void CSimState::SetWallOn(bool bWall)
{
	if(m_rInitialState.IsWallPresent())
		m_bIsWallOn = bWall;
}

void CSimState::SetGravityOn(bool bGravity)
{
	if(m_rInitialState.IsGravityPresent())
		m_bIsGravityOn = bGravity;
}

void CSimState::SetShearOn(bool bShear)
{
	if(m_rInitialState.IsShearPresent())
		m_bIsShearOn = bShear;
}

// Function to toggle a flag showing the presence of active bonds

void CSimState::SetActiveBondsOn(bool bActive)
{
	m_bIsActiveBondsOn = bActive;
}

// Function to toggle a flag showing the presence of charged beads

void CSimState::SetBeadChargeOn(bool bCharge)
{
	m_bIsBeadChargeOn = bCharge;
}

// Function to set a flag showing whether momenta renormalisation is
// performed on the particles in an MD simulation. We do not actually
// distinguish between MD and DPD but the use of the renormalisation 
// only makes sense in an MD simulation.

void CSimState::SetRenormaliseMomenta(bool bRenormalise)
{
	m_bRenormaliseMomenta = bRenormalise;
}

// Function to set a flag showing whether the total KE and PE should be
// written to the history state file. Because this is an expensive
// calculation, we allow it to be toggled on and off using the
// ToggleEnergyOutput command.

void CSimState::SetEnergyOutput(bool bEnergy)
{
	m_bEnergyOutput = bEnergy;
}

// Function to toggle the bead contribution to the stress tensor analysis
// on and off.

void CSimState::SetBeadStressContributionOn(bool bStressOn)
{
	m_bIsBeadStressAdded = bStressOn;
}

// Function to toggle the charged bead contribution to the stress tensor analysis
// on and off.

void CSimState::SetBeadChargeStressContribution(bool bStressOn)
{
	m_bIsBeadChargeStressAdded = bStressOn;
}

// Function to toggle the bond contribution to the stress tensor analysis
// on and off.

void CSimState::SetBondStressContributionOn(bool bStressOn)
{
	m_bIsBondStressAdded = bStressOn;
}

// Function to toggle the bondpair contribution to the stress tensor analysis
// on and off.

void CSimState::SetBondPairStressContributionOn(bool bStressOn)
{
	m_bIsBondPairStressAdded = bStressOn;
}

// Function to change the value of the conservative interaction between
// two DPD bead types. It changes one element in the matrix stored in the 
// static member variable CCNTCell::m_vvConsInt, and the copy in the 
// initial state instance.

void CSimState::SetDPDConsInt(long firstType, long secondType, double newValue)
{
	if(0 <= firstType  && firstType  < GetBeadTypeTotal() &&
	   0 <= secondType && secondType < GetBeadTypeTotal())
	{
		CCNTCell::SetDPDBeadConsInt(firstType, secondType, newValue);
        m_rInitialState.SetDPDBeadConsInt(firstType, secondType, newValue);
	}
}

// Function to change the value of the dissipative interaction between
// two DPD bead types. It changes one element in the matrix stored in the 
// static member variable CCNTCell::m_vvDissInt, and the copy in the 
// initial state instance.

void CSimState::SetDPDDissInt(long firstType, long secondType, double newValue)
{
	if(0 <= firstType  && firstType  < GetBeadTypeTotal() &&
	   0 <= secondType && secondType < GetBeadTypeTotal())
	{
		CCNTCell::SetDPDBeadDissInt(firstType, secondType, newValue);
        m_rInitialState.SetDPDBeadDissInt(firstType, secondType, newValue);
	}
}

// Function to change the value of the density-dependent interaction between
// two DPD bead types. It changes one element in the matrix stored in the 
// static member variable CCNTCell::m_vvLGInt, and the copy in the 
// initial state instance.

void CSimState::SetDPDLGInt(long firstType, long secondType, double newValue)
{
#if EnableDPDLG == ExperimentEnabled
	if(0 <= firstType  && firstType  < GetBeadTypeTotal() &&
	   0 <= secondType && secondType < GetBeadTypeTotal())
	{
		CCNTCell::SetDPDBeadLGInt(firstType, secondType, newValue);
        m_rInitialState.SetDPDBeadLGInt(firstType, secondType, newValue);
	}
#endif
}

// Function used to add a new bead type dynamically as the result of a
// ccChangeBeadType command, or other commands that alter the types
// associated with beads. Beads are not created as a result of this
// function, they only have their types changed from "oldType" to
// a new type equal to the previous number of bead types. Note also that 
// only a subset of "oldType" beads may be affected depending on the 
// details of the command that results in calling this function.
//
// To make this routine work with MD and DPD we have to use #if to
// distinguish the two cases.

void CSimState::AddBeadType(long oldType)
{
	// Tell the CInitialState to create a new bead type from beads
	// with the previous type "oldType".

	m_rInitialState.AddBeadType(oldType);

	// Set the bead-bead interactions in the CCNTCell static members 

	CCNTCell::AddDPDBeadType(oldType);
}

// Overloaded function to add a new bead type that has been explicitly named.
// If the name already occurs in the CAnalysisState's map, the function rejects
// the addition and returns false. Note that the old type is needed so that
// the original bead-bead interaction parameters can be copied into the new
// type.

bool CSimState::AddBeadType(long oldType, long newType, const zString name)
{
	// Store the bead's name,type association in the CAnalysisState only
	// if the name does not already exist

	if(m_rAnalysisState.AddNamedBeadTypeToMap(newType, name))
	{
		// Tell the CInitialState to create a new bead type from beads
		// with the previous type "oldType".

		m_rInitialState.AddBeadType(oldType);

		// Set the bead-bead interactions in the CCNTCell static members 

		CCNTCell::AddDPDBeadType(oldType);

		return true;
	}
	else
		return false;
}

// Function used to add a new bond type dynamically as the result of a
// ctPolymerisePolymerTripleInTarget command, or other commands. This does not
// result in the creation of any new bond instances, it only creates a type.

void CSimState::AddBondType(CBond* const pBond)
{
	m_rInitialState.AddBondType(pBond);
}

// Overloaded function used to add a new bond type dynamically as the result of a
// ctPolymerisePolymerTripleInTarget command, or other commands. This version
// requires a unique bond name to be specified, and adds the name and numeric 
// type to the CAnalysisState maps.

bool CSimState::AddBondType(CBond* const pBond, long newType, const zString name)
{
	// Store the bond's name/type association in the CAnalysisState only
	// if the name does not already exist

	if(pBond && m_rAnalysisState.AddNamedBondTypeToMap(newType, name))
	{
	    m_rInitialState.AddBondType(pBond);

		return true;
	}
	else
		return false;
}

// Function to add a dynamically-created, polymerised bond to the CInitialState.
// These are stored separately from the ordinary bonds that bind beads into
// polymers because they connect different polymers, and so do not belong 
// in any one polymer. They are destroyed by the CInitialState's destructor.
// Note that this function differs from the bead and bond functions above in that
// a new bond instance is added to the simulation's bonds here, whereas
// the above functions only add a new type. 

void CSimState::AddPolymerisedBond(CBond* const pBond)
{
	m_rInitialState.AddPolymerisedBond(pBond);
	m_rInitialState.AddPolymerisedBondtoAllBonds(pBond);
}

// Function used to change the integration time step to a new value
// as the result of a ccSetTimeStepSize command. Although the command
// only allows positive values for the time step, we provide a further
// check here (and in the CCNTCell function) to prevent illegal values
// from being stored in CAnalysisState object.
//
// It passes the command to the CCNTCell class that stores the
// parameters that depend on the step size in static member variables;
// and updates the value stored in the CAnalysisState.

void CSimState::SetTimeStep(double dt)
{
	if(dt > 0.0)
	{
		m_rAnalysisState.SetIntegrationStep(dt);

		CCNTCell::ChangeTimeStepConstants(dt);
	}
}

// Function used to set the simulation time to a new value as the result of
// a ccExtendTotalTime command. The new time must be greater than or equal to 
// the current time or the command is ignored.

// The command is handled by the CSimBox that updates its own value for
// the simulation time and then updates the CSimState. No other class
// stores the time, so the CSimState simply updates its contained CAnalysisState object.
//
void CSimState::SetTotalTime(long newTime)
{
	if(newTime >= m_rAnalysisState.GetTotalTime())
	{
		m_rAnalysisState.SetTotalTime(newTime);
	}
}

// Function to set the current time in the CAnalysisState object so that
// it can normalise the observables correctly.

void CSimState::SetCurrentTime(long newTime)
{
	if(newTime > 0)
		m_rAnalysisState.SetCurrentTime(newTime);
}

// Function to turn the DPD bead-bead interactions on or off. It returns
// the new state of the force calculation.

bool CSimState::ToggleDPDBeadForces()
{
	// If the forces are currently zeroed, pass the time step to the
	// function to restore the constants to their normal values.
	// If the forces are currently calculated, pass zero in to turn them off.

	if(m_bIsDPDBeadForceZero)
	{
		m_bIsDPDBeadForceZero = false;

		CCNTCell::ToggleDPDBeadForces(m_rAnalysisState.GetIntegrationStep());
	}
	else
	{
		m_bIsDPDBeadForceZero = true;

		CCNTCell::ToggleDPDBeadForces(0.0);
	}

	return m_bIsDPDBeadForceZero;
}

// Function to turn the DPD conservative bead-bead interactions on or off. 

bool CSimState::ToggleDPDBeadConservativeForces()
{
	if(m_bIsDPDBeadConsForceZero)
	{
		m_bIsDPDBeadConsForceZero = false;

		CCNTCell::ToggleDPDBeadConservativeForces(false);
	}
	else
	{
		m_bIsDPDBeadConsForceZero = true;

		CCNTCell::ToggleDPDBeadConservativeForces(true);
	}

	return m_bIsDPDBeadConsForceZero;
}

// Function to turn the DPD random and dissipative bead-bead interactions on or off. 

bool CSimState::ToggleDPDBeadThermostat()
{
	if(m_bIsDPDBeadThermostatZero)
	{
		m_bIsDPDBeadThermostatZero = false;

		CCNTCell::ToggleDPDBeadThermostat(false);
	}
	else
	{
		m_bIsDPDBeadThermostatZero = true;

		CCNTCell::ToggleDPDBeadThermostat(true);
	}

	return m_bIsDPDBeadThermostatZero;
}

// Function to copy the BD bead-bead interaction parameters into the CCTNCell 
// class' local arrays.

void CSimState::SetBDBeadIntArrays(const zArray2dDouble* pvvConsInt, const zArray2dDouble* pvvDissInt)
{
// We use the same forces for BD as in DPD for now
    CCNTCell::SetBDBeadStructure(pvvConsInt, pvvDissInt);
}

// Function to copy the DPD bead-bead interaction parameters (for the conservative
// and dissipative forces) into the CCTNCell class' local arrays. This is done
// for speed of access. Note that the following CNTCell static member functions 
// take pointers as arguments, and the CInputData functions used in the constructor
// above return pointers. We also need to modify the loading of the bead-bead 
// interaction parameters to allow the inclusive restart states to modify them 
// if necessary. Hence, we use this local helper function to copy the pointers
// to the arrays to the CCNTCell class.
 
void CSimState::SetDPDBeadIntArrays(const zArray2dDouble* pvvConsInt, const zArray2dDouble* pvvDissInt)
{
	CCNTCell::SetDPDBeadStructure(pvvConsInt, pvvDissInt);
}

// Overloaded function that includes the density-dependent LG force parameters.
// Note that we don't allow for non-use of the density-dependent parameters here;
// this is assumed to be taken care of in the constructor where the rInputData 
// object is accessible.

void CSimState::SetDPDBeadIntArrays(const zArray2dDouble* pvvConsInt, const zArray2dDouble* pvvDissInt, const zArray2dDouble* pvvLGInt)
{
#if EnableDPDLG == ExperimentEnabled
	CCNTCell::SetDPDBeadStructure(pvvConsInt, pvvDissInt, pvvLGInt);
#endif
}

// Function to copy the MD bead-bead interaction parameters into the CCTNCell 
// class' local arrays.

void CSimState::SetMDBeadIntArrays(const zArray2dDouble* pvvLJDepth, const zArray2dDouble* pvvLJRange,
								   const zArray2dDouble* pvvSCDepth, const zArray2dDouble* pvvSCRange)
{
	CCNTCell::SetMDBeadStructure(pvvLJDepth, pvvLJRange, pvvSCDepth, pvvSCRange);
}

// Function to return the size of the X dimension in simulation Space: this
// is the same as GetSimBoxXLength() for a serial simulation.

double CSimState::GetSimSpaceXLength() const
{
    return static_cast<double>(GetProcessorsXNo())*GetSimBoxXLength();
}

// Function to return the size of the Y dimension in simulation Space: this
// is the same as GetSimBoxYLength() for a serial simulation.

double CSimState::GetSimSpaceYLength() const
{
    return static_cast<double>(GetProcessorsYNo())*GetSimBoxYLength();
}

// Function to return the size of the Z dimension in simulation Space: this
// is the same as GetSimBoxZLength() for a serial simulation.

double CSimState::GetSimSpaceZLength() const
{
    return static_cast<double>(GetProcessorsZNo())*GetSimBoxZLength();
}

// Functions to return the origin of  the owning processor's SimBox in the whole
// simulation space.

double CSimState::GetSimBoxXOrigin() const
{
    return m_rInitialState.GetSimBoxXOrigin();
}

double CSimState::GetSimBoxYOrigin() const
{
    return m_rInitialState.GetSimBoxYOrigin();
}

double CSimState::GetSimBoxZOrigin() const
{
    return m_rInitialState.GetSimBoxZOrigin();
}
