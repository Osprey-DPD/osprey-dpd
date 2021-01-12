// ISimState.h: interface for the ISimState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISIMSTATE_H__A9140148_09A9_405A_A905_A6522576EA34__INCLUDED_)
#define AFX_ISIMSTATE_H__A9140148_09A9_405A_A905_A6522576EA34__INCLUDED_


// Forward declarations

class CSimState;
class ISimulation;
class IModifySimStateBeadTypes;
class IModifySimStateBondTypes;
class IModifySimStateIntegration;
class IModifySimStatePolymers;


#include "ISimEvent.h"

class ISimState : public ISimEvent
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~ISimState();

	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes.

    // Function to move a bead from its current CNT cell to another one
    
    virtual bool MoveBeadBetweenCNTCells(CAbstractBead* const pBead, double x, double y, double z) const = 0;

	// ****************************************
	// Public access functions
public:

    bool IsDPDLG() const;   // Function showing if the lg interactions are used

	const zString GetRunId() const;

	inline long   GetCurrentTime()			const {return m_SimTime;}
	inline long   GetTotalTime()			const {return m_TotalTime;}

	void AddProcess(xxProcess* const pProcess);
	void AddCommand(const xxCommand *const pCommand);
	void AddCommandToFront(const xxCommand *const pCommand);
	void AddCommandAfterFront(const xxCommand *const pCommand);

	// Forwarding function to the CSimState to give access to the simulation's
	// ISimulation interface

	ISimulation* GetISimulation() const;


	// General functions to return information about the SimBox for use in 
	// both the serial and parallel codes.

	double GetSimBoxXOrigin()     const;
	double GetSimBoxYOrigin()     const;        // SimBox origin in simulation space
	double GetSimBoxZOrigin()     const;
	double GetSimSpaceXLength()   const;        // Whole simulation Space size
	double GetSimSpaceYLength()   const;
	double GetSimSpaceZLength()   const;        

	// Functions querying the bead/bond/bondpair/polymers structures

	long GetBeadTypeTotal() const;
	long GetBondTypeTotal() const;
	long GetBondPairTypeTotal() const;
	long GetPolymerTypeTotal() const;

	long GetBeadTotalForType(long type) const;
	long GetBondTotalForType(long type) const;
	long GetBondPairTotalForType(long type) const;
	long GetPolymerTotalForType(long type) const;
	
	const BeadVector&     GetBeadTypes()     const;
	const BondVector&     GetBondTypes()     const;
	const BondPairVector& GetBondPairTypes() const;
	const PolymerVector&  GetPolymerTypes()  const;


	// Functions querying the polymer structures

	long GetBeadTypeFromName(const zString name) const;
	long GetBondTypeFromName(const zString name) const;
	long GetBondPairTypeFromName(const zString name) const;
	long GetPolymerTypeFromName(const zString name) const;
	long GetPolymerHeadType(long polyType) const;
	long GetPolymerTailType(long polyType) const;

	const zString GetBeadNameFromType(long type) const;
	const zString GetBondNameFromType(long type) const;
	const zString GetBondPairNameFromType(long type) const;
	const zString GetPolymerNameFromType(long type) const;

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	ISimState(CSimState& simState, const ISimBox* const pISimBox);

	// Forwarding functions to the CSimState interface classes that control 
	// access by objects wishing to modify the simulation state

	IModifySimStateBeadTypes*	 IModifyBeadTypes() const;
	IModifySimStateBondTypes*	 IModifyBondTypes() const;
	IModifySimStateIntegration*  IModifyIntegration() const;
	IModifySimStatePolymers*	 IModifyPolymers() const;

	// Functions used by the CSimBox to loop over the events and commands
	// to see if any should be executed in the current timestep

	void ExecuteEvents();
	void ExecuteCommands();

	// Functions to turn on/off constraints in the CSimState

	void SetActiveBondsOn(bool bActive);
	void SetBeadChargeOn(bool bCharge);
	void SetBeadStressContributionOn(bool bStress);
	void SetBondStressContributionOn(bool bStress);
	void SetBondPairStressContributionOn(bool bStress);
	void SetGravityOn(bool bGravity);
	void SetRenormaliseMomenta(bool bRenormalise);
	void SetWallOn(bool bWall);


	// Access functions to the CSimState that do not modify its state

	// Functions returning boolean information on the CSimState

	bool TimeToSample() const;
	bool TimeToDisplay() const;
	bool TimeToRestart() const;

	bool IsActiveBondsOn() const;
	bool IsBeadChargeOn() const;
	bool IsBeadStressAdded() const;
	bool IsBondStressAdded() const;
	bool IsBondPairStressAdded() const;
	bool IsEnergyOutputOn() const;
	bool IsGravityOn() const;
	bool IsRenormalisationOn() const;
	bool IsWallOn() const;
	bool IsWallPolymerFlexible() const;

	// Functions that return information about the physical state of the simulation

	long GetProcessorsXNo() const;
	long GetProcessorsYNo() const;              // No of processors in each dimension
	long GetProcessorsZNo() const;

	long GetGridXCellNo() const;                // No of grid cells in each dimension
	long GetGridYCellNo() const;
        long GetGridZCellNo() const;
    
	long GetCNTXCellNo() const;
	long GetCNTYCellNo() const;                 // No of CNT cells in each dimension
	long GetCNTZCellNo() const;					

	double GetCNTXCellWidth()     const;
	double GetCNTYCellWidth()     const;		// Width of CNT cells in each dimension
	double GetCNTZCellWidth()     const;
	double GetSimBoxXLength()     const;
	double GetSimBoxYLength()     const;		// Processor's SimBox size
	double GetSimBoxZLength()     const;
	double GetSimBoxBulkXLength() const;
	double GetSimBoxBulkYLength() const;		// Processor's (SimBox size - Wall width)
	double GetSimBoxBulkZLength() const;
	double GetWallXWidth()        const;
	double GetWallYWidth()        const;		// Wall width
	double GetWallZWidth()        const;
	double GetkT()                const;		// Temperature
	double GetEnergyScale()       const;		// MD energy scale
	double GetLengthScale()       const;		// MD length scale


	// Functions returning the number of types of bead, the numbers of
	// each bead type, etc.

	const BondVector GetAllBonds() const;
	const BondVector GetAllPolymerisedBonds() const;




    // Function to return the set of command targets stored in the 
    // initial state object. It is used when importing targets from
    // a restart state.

	CommandTargetSequence GetCommandTargets() const;

	// Functions inherited from the interface classes.
	//
	// ISimCmd
	// *******
	// ISimCmd provides access to functions that change the constraints on
	// the SimBox during a simulation. Each function takes an xxCommand-derived 
	// object as an argument that provides data needed for the constraint. 
	// We pass the commands as pointers to their base class and then cast 
	// them inside the function that implements the command.



	// ISimEvent
	// ***********

	void RefreshEvents();



	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:


	// ****************************************
	// Interfaces to the CSimState allowing the CSimBox to modify the
	// simulation in a controlled and specific way. Each interface
	// is via a wrapper class that exposes a subset of the CSimState
	// functions relating to a particular set of modifications.


	IModifySimStateBeadTypes*    const	m_pIBeadTypes;		// Interface allowing bead type changes
	IModifySimStateBondTypes*    const	m_pIBondTypes;		// Interface allowing bond type changes
	IModifySimStateIntegration*  const	m_pIIntegration;	// Interface to integration scheme
	IModifySimStatePolymers*     const	m_pIPolymers;		// Interface allowing polymer changes

	// ****************************************
	// Containers holding the set of events, commands etc that are
	// monitored during a simulation.

	EventSequence		  m_Events;				// Events monitored by the SimBox
	CommandSequence          m_Commands;			        // Commands used to change constraints on the SimBox


	// Data relating to the simulation's evolution and monitoring

	long m_SimTime;				// Current simulation time
	long m_TotalTime;				// Total time for simulation to run

	long m_TotalMCTime;				// Total time for pre-MD Monte Carlo relaxation
	double m_MaxMCStep;				// Max displacement for an MC step

	long m_RenormalisationPeriod;	// Period for renormalising momenta


private:

	CSimState&	m_rSimState;		// General reference to the SimState

};

#endif // !defined(AFX_ISIMSTATE_H__A9140148_09A9_405A_A905_A6522576EA34__INCLUDED_)
