// SimState.h: interface for the CSimState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMSTATE_H__9E3EA8E0_3DC3_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_SIMSTATE_H__9E3EA8E0_3DC3_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class ISimulation;


#include "AnalysisState.h"
#include "InitialState.h"
#include "HistoryState.h"
#include "LogState.h"


#include "xxBase.h"

class CSimState : public xxBase
{
    // Interface class used to construct a new initial state from a restart state

    friend class IInclusiveRestartState;

	// Interface classes used by the CSimBox to handle commands that
	// need access to the CSimState's private handler functions.

	friend class IModifySimStateBeadTypes;
	friend class IModifySimStateBondTypes;
	friend class IModifySimStateIntegration;
	friend class IModifySimStatePolymers;

public:

	// Constructor for use in stand-alone simulations

	CSimState(const CInputData& rInputData);

	// Constructor for use when the simulation is part of an experiment

	CSimState(ISimulation* pISim, const CInputData& rInputData);

	~CSimState();

	bool Assemble();  // Function to create the initial configuration for a serial simulation

	bool AssembleP(); // Function to create the parallel initial configuration

	inline const zString GetRunId()				const {return m_RunId;}
	inline const zString GetTitle()				const {return m_Title;}
	inline const zString GetDate()				const {return m_Date;}
	inline const zString GetComment()			const {return m_Comment;}

	// ****************************************
	// Access function to the ISimulation interface giving access to the
	// simulation and experiment objects.

	ISimulation* GetISimulation() const {return m_pISimulation;}

	// Access functions to xxState-derived objects. The first set allow the
	// objects to be altered while the second set return const references.

	CAnalysisState& GetAnalysisState();
	CInitialState&  GetInitialState();
	CHistoryState&  GetHistoryState();
	CLogState&		GetLogState();

	const CAnalysisState& cGetAnalysisState() const {return m_rAnalysisState;}
	const CInitialState&  cGetInitialState()  const {return m_rInitialState;}
	const CHistoryState&  cGetHistoryState()  const {return m_rHistoryState;}
	const CLogState&	  cGetLogState()      const {return m_rLogState;}

	// ****************************************
	// Access functions that wrap the enclosed xxState-derived objects.

	// CAnalysisState
	inline long   GetTotalTime()			const {return m_rAnalysisState.GetTotalTime();}
	inline long   GetTotalMCTime()			const {return m_rAnalysisState.GetTotalMCTime();}
	inline long	  GetSamplePeriod()			const {return m_rAnalysisState.GetSamplePeriod();}
	inline long	  GetAnalysisPeriod()		const {return m_rAnalysisState.GetAnalysisPeriod();}
	inline long   GetDensityPeriod()		const {return m_rAnalysisState.GetDensityPeriod();}
	inline long	  GetDisplayPeriod()		const {return m_rAnalysisState.GetDisplayPeriod();}
	inline long	  GetRestartPeriod()		const {return m_rAnalysisState.GetRestartPeriod();}
	inline long	  GetGridXCellNo()			const {return m_rAnalysisState.GetGridXCellNo();}
	inline long	  GetGridYCellNo()			const {return m_rAnalysisState.GetGridYCellNo();}
	inline long	  GetGridZCellNo()			const {return m_rAnalysisState.GetGridZCellNo();}
	inline double GetIntegrationStep()		const {return m_rAnalysisState.GetIntegrationStep();}
	inline double GetMCStepSize()			const {return m_rAnalysisState.GetMCStepSize();}
	inline double GetLambda()				const {return m_rAnalysisState.GetLambda();}
	
	inline ProcessSequence GetProcesses()   const {return m_rAnalysisState.GetProcesses();}
	inline EventSequence   GetEvents()		const {return m_rAnalysisState.GetEvents();}
	inline CommandSequence GetCommands()	const {return m_rAnalysisState.GetCommands();}

	// CInitialState

	inline const zString GetInitialStateType()	const {return m_rInitialState.GetInitialStateType();}

    inline bool IsDPDLG()                   const {return m_rInitialState.IsDPDLG();}

	inline long	GetBeadTypeTotal()			const {return m_rInitialState.GetBeadTypeTotal();}
	inline long	GetBondTypeTotal()			const {return m_rInitialState.GetBondTypeTotal();}
	inline long	GetBondPairTypeTotal()		const {return m_rInitialState.GetBondPairTypeTotal();}
	inline long	GetPolymerTypeTotal()		const {return m_rInitialState.GetPolymerTypeTotal();}
	inline long	GetBeadTotal()				const {return m_rInitialState.GetBeadTotal();}
	inline long	GetBondTotal()				const {return m_rInitialState.GetBondTotal();}
	inline long	GetBondPairTotal()			const {return m_rInitialState.GetBondPairTotal();}
	inline long	GetPolymerTotal()			const {return m_rInitialState.GetPolymerTotal();}
	inline long GetGravityBeadTotal()		const {return m_rInitialState.GetGravityBeadTotal();}
	inline long	GetWallBeadTotal()			const {return m_rInitialState.GetWallBeadTotal();}
	inline long	GetWallPolymerTotal()		const {return m_rInitialState.GetWallPolymerTotal();}
	inline long	GetRNGSeed()				const {return m_rInitialState.GetRNGSeed();}
	inline long	GetProcessorsXNo()			const {return m_rInitialState.GetProcessorsXNo();}
	inline long	GetProcessorsYNo()			const {return m_rInitialState.GetProcessorsYNo();}
	inline long	GetProcessorsZNo()			const {return m_rInitialState.GetProcessorsZNo();}
	inline long	GetCNTXCellNo()				const {return m_rInitialState.GetCNTXCellNo();}
	inline long	GetCNTYCellNo()				const {return m_rInitialState.GetCNTYCellNo();}
	inline long	GetCNTZCellNo()				const {return m_rInitialState.GetCNTZCellNo();}

	inline double GetDPDBeadConsInt(long first, long second) const {return m_rInitialState.GetDPDBeadConsInt(first,second);}
	inline double GetDPDBeadDissInt(long first, long second) const {return m_rInitialState.GetDPDBeadDissInt(first,second);}
	inline double GetDPDBeadLGInt(long first, long second)   const {return m_rInitialState.GetDPDBeadLGInt(first,second);}

	inline double GetDensity()				const {return m_rInitialState.GetDensity();}
	inline double GetkT()					const {return m_rInitialState.GetkT();}
	inline double GetEnergyScale()			const {return m_rInitialState.GetEnergyScale();}
	inline double GetLengthScale()			const {return m_rInitialState.GetLengthScale();}
	inline double GetGravityXForce()		const {return m_rInitialState.GetGravityXForce();}
	inline double GetGravityYForce()		const {return m_rInitialState.GetGravityYForce();}
	inline double GetGravityZForce()		const {return m_rInitialState.GetGravityZForce();}
	inline double GetCNTXCellWidth()		const {return m_rInitialState.GetCNTXCellWidth();}
	inline double GetCNTYCellWidth()		const {return m_rInitialState.GetCNTYCellWidth();}
	inline double GetCNTZCellWidth()		const {return m_rInitialState.GetCNTZCellWidth();}
	inline double GetSimBoxXLength()		const {return m_rInitialState.GetSimBoxXLength();}
	inline double GetSimBoxYLength()		const {return m_rInitialState.GetSimBoxYLength();}
	inline double GetSimBoxZLength()		const {return m_rInitialState.GetSimBoxZLength();}

	double        GetSimBoxXOrigin() const;
	double        GetSimBoxYOrigin() const;
	double        GetSimBoxZOrigin() const;
    double        GetSimSpaceXLength() const;
	double        GetSimSpaceYLength() const;
	double        GetSimSpaceZLength() const;

	// The following functions return the set of beads/bonds/bondpairs/polymers
	// created in the initial state only: these are not updated in parallel mode
	// to account for beads that move between processors.

	inline const BeadVector&     GetInitialStateBeads()	const {return m_rInitialState.GetBeads();}
	inline const BondVector&     GetBonds()				const {return m_rInitialState.GetBonds();}
	inline const BondPairVector& GetBondPairs()			const {return m_rInitialState.GetBondPairs();}
	inline const PolymerVector&  GetPolymers()			const {return m_rInitialState.GetPolymers();}
	inline const BeadVector&     GetGravityBeads()		const {return m_rInitialState.GetGravityBeads();}
	inline const AbstractBeadVector& GetWallBeads()		const {return m_rInitialState.GetWallBeads();}
	inline const PolymerVector&  GetWallPolymers()		const {return m_rInitialState.GetWallPolymers();}

	inline const BeadVector&	 GetBeadTypes()		const {return m_rInitialState.GetBeadTypes();}
	inline const BondVector&	 GetBondTypes()		const {return m_rInitialState.GetBondTypes();}
	inline const BondPairVector& GetBondPairTypes() const {return m_rInitialState.GetBondPairTypes();}
	inline const PolymerVector&	 GetPolymerTypes()	const {return m_rInitialState.GetPolymerTypes();}

	inline const BondVector&     GetPolymerisedBonds()	const {return m_rInitialState.GetPolymerisedBonds();}

	// Functions showing whether particular constraints are currently active

	inline bool IsWallOn()					const {return m_bIsWallOn;}
	inline bool IsWallPolymerFlexible()		const {return m_rInitialState.IsWallPolymerFlexible();}
	inline bool IsGravityOn()				const {return m_bIsGravityOn;}
	inline bool IsShearOn()					const {return m_bIsShearOn;}
	inline bool IsActiveBondsOn()			const {return m_bIsActiveBondsOn;}
	inline bool IsBeadChargeOn()			const {return m_bIsBeadChargeOn;}

	// Functions showing which contributions to the stress tensor are included

	inline bool IsBeadStressAdded()			const {return m_bIsBeadStressAdded;}
	inline bool IsBeadChargeStressAdded()	const {return m_bIsBeadChargeStressAdded;}
	inline bool IsBondStressAdded()			const {return m_bIsBondStressAdded;}
	inline bool IsBondPairStressAdded()		const {return m_bIsBondPairStressAdded;}

	// Function showing whether KE and PE are written to the history state

	inline bool IsEnergyOutputOn()			const {return m_bEnergyOutput;}

	// DPD only functions

	inline bool IsDPDBeadForceZero()		const {return m_bIsDPDBeadForceZero;}

	// MD only functions

	inline bool IsRenormalisationOn()		const {return m_bRenormaliseMomenta;}

	// ****************************************
	// Functions used by the CSimBox and CMonitor to update the CSimState
	// as a result of xxCommand objects

	void SetActiveBondsOn(bool bActive);
	void SetAnalysisPeriod(long period);
	void SetBeadChargeOn(bool bCharge);
	void SetBeadChargeStressContribution(bool bStressOn);
	void SetBeadStressContributionOn(bool bStressOn);
	void SetBondPairStressContributionOn(bool bStressOn);
	void SetBondStressContributionOn(bool bStressOn);
	void SetCurrentTime(long newTime);
	void SetDensityPeriod(long period);
	void SetDisplayPeriod(long period);
	void SetEnergyOutput(bool bEnergy);
	void SetGravityOn(bool bGravity);
	void SetRenormaliseMomenta(bool bRenormalise);
	void SetRestartPeriod(long period);
	void SetSamplePeriod(long period);
	void SetShearOn(bool bShear);
	void SetWallOn(bool bWall);

	// ****************************************
	// Functions used by the CSimBox to handle commands that change the
	// state of the simulation or analysis and output options. Such
	// commands are routed through wrapper classes to protect the 
	// CSimState from accidental modification: only functions explicitly
	// provided in the wrapper classes IModifySimStateXXX can be
	// executed.

private:

	// IModifySimStateBeadTypes

	inline bool IsBeadNameInMap(const zString name) const {return m_rAnalysisState.IsBeadNameInMap(name);}
	inline bool IsBeadTypeInMap(long type)          const {return m_rAnalysisState.IsBeadTypeInMap(type);}

	void AddBeadType(long oldType);
	bool AddBeadType(long oldType, long newType, const zString name);

	// IModifySimStateBondTypes

	void AddBondType(CBond* const pBond);
	bool AddBondType(CBond* const pBond, long newType, const zString name);
	void AddPolymerisedBond(CBond* const pBond);

	// Functions to change the DPD bead-bead interaction parameters


	void SetDPDConsInt(long firstType, long secondType, double newValue);
	void SetDPDDissInt(long firstType, long secondType, double newValue);
	void SetDPDLGInt(long firstType, long secondType, double newValue);

	// IModifySimStateIntegration

	void SetTimeStep(double dt);		// Changes the integration time step
	void SetTotalTime(long newTime);	// Changes the total simulation time
	bool ToggleDPDBeadForces();			// Turns DPD bead-bead interactions on/off
	bool ToggleDPDBeadConservativeForces();
	bool ToggleDPDBeadThermostat();


	// IModifySimStatePolymers



    // IInclusiveRestartState

    // These functions are also used in constructing the initial state in 
    // the constructor

	void  SetBDBeadIntArrays(const zArray2dDouble* pvvConsInt, const zArray2dDouble* pvvDissInt);
    void SetDPDBeadIntArrays(const zArray2dDouble* pvvConsInt, const zArray2dDouble* pvvDissInt);
    void SetDPDBeadIntArrays(const zArray2dDouble* pvvConsInt, const zArray2dDouble* pvvDissInt,
                             const zArray2dDouble* pvvLGInt);
    void  SetMDBeadIntArrays(const zArray2dDouble* pvvLJDepth, const zArray2dDouble* pvvLJRange,
						     const zArray2dDouble* pvvSCDepth, const zArray2dDouble* pvvSCRange);


	// ****************************************
	// Data members
private:

	ISimulation* const   m_pISimulation;	// Pointer to the ISimulation interface

	CAnalysisState	m_rAnalysisState;
	CInitialState	m_rInitialState;
	CHistoryState	m_rHistoryState;
	CLogState		m_rLogState;

	const zString	m_RunId;				// Identifying string for the run
	const zString	m_Title;				// User-supplied title
	const zString	m_Date;					// User-supplied date
	const zString	m_Comment;				// User-supplied comment
	const zString   m_ISType;				// Initial state type identifier


	// Flags showing the state of constraints on the SimBox. These are used to
	// save state information in restart states.

	bool m_bIsWallOn;
	bool m_bIsGravityOn;
	bool m_bIsShearOn;

	// Further flags showing the state of the SimBox. These are not stored in 
	// a restart state.

	bool m_bIsActiveBondsOn;
	bool m_bIsBeadChargeOn;	

	// Flags showing which contributions to the stress tensor profile are included

	bool m_bIsBeadStressAdded;
	bool m_bIsBeadChargeStressAdded;
	bool m_bIsBondStressAdded;
	bool m_bIsBondPairStressAdded;

	// Flag showing whether the total kinetic and potential energies are output

	bool m_bEnergyOutput;

	// ****************************************
	// DPD only data

	bool m_bIsDPDBeadConsForceZero;
	bool m_bIsDPDBeadForceZero;
	bool m_bIsDPDBeadThermostatZero;

	// ****************************************
	// MD only data

	bool m_bRenormaliseMomenta;
};

#endif // !defined(AFX_SIMSTATE_H__9E3EA8E0_3DC3_11D3_820E_0060088AD300__INCLUDED_)
