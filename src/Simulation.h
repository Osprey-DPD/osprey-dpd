// Simulation.h: interface for the CSimulation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMULATION_H__A2FACF45_3F61_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_SIMULATION_H__A2FACF45_3F61_11D3_820E_0060088AD300__INCLUDED_


// Include file to distinguish parallel/serial codes and allow conditional
// compilation of parallelised classes.

#include "SimDefs.h"

// Forward declarations

class IConnectSimulation;
class CInputData;
class CSimState;
class ISimBox;

// Include files for the simulation's nested interfaces

#include "IExperimentAccessControlImpl.h"
#include "ISecureSimulationImpl.h"
#include "ISetSimulationDataImpl.h"
#include "ISetSubstrateDataImpl.h"
#include "ISetSculptureDataImpl.h"
#include "ISetProbeFieldDataImpl.h"
#include "ISetExternalFieldDataImpl.h"
#include "ISetProcessDataImpl.h"


#include "ISimulation.h"

class CSimulation : public ISimulation
{
	// ****************************************
	// Construction/Destruction
public:

	// Constructor for use in a stand-alone simulation

	CSimulation(const zString runId);	// A simulation needs a runId to identify the CDF		

	virtual ~CSimulation();					


	// ****************************************
	// Global functions, static member functions and variables
public:

	// Protected constructor and the public Instance() functions ensure that
	// only a single CSimulation object can be created. 

	static ISimulation* Instance(IConnectSimulation* pExpt, const zString runId);

private:

	static ISimulation* m_pInstance;		

	
	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// Implementation of ISimulation interface

	// External commands to begin simulation

	bool RunPrevis(StringSequence excludedPolymers);
	bool Run();							

	// Functions to access other interfaces

	IExperimentAccessControl*	GetIExperimentAccessControl();
	ISecureSimulation*			GetISecureSimulation();
	ISetExternalFieldData*		GetISetExternalFieldData();
	ISetProbeFieldData*			GetISetProbeFieldData();
	ISetSculptureData*			GetISetSculptureData();
	ISetSubstrateData*			GetISetSubstrateData();
	ISetSimulationData*			GetISetSimulationData();
	ISetProcessData*			GetISetProcessData();

	// ****************************************
	// Public access functions
public:

	double GetSimSpaceXLength() const;
	double GetSimSpaceYLength() const;
	double GetSimSpaceZLength() const;



	// ****************************************
	// Other interface class implementations

	// IExerimentAccessControl

	bool IsExperimentAccessible() const;
	IConnectSimulation*		GetIConnectSimulation();

	// ISecureSimulation




	// ISetProcessData

	bool AddProcess(xxProcess* const pProcess);





	// ****************************************
	// Protected local functions
protected:

	// Constructor for use in conjunction with the experiment

	CSimulation(IConnectSimulation* pExpt, const zString runId);

	// ****************************************
	// Implementation

	// ****************************************
	// Private functions
private:

	bool Initialize();	// Reads input data, creates SimBox, Monitor, etc

	bool BroadcastSimulationData();  // Pass simulation data from P0 to PN
	bool BroadcastPolymerData();     // Pass datatype data from P0 to PN
	bool BroadcastISData();          // Pass initial state data from P0 to PN
	bool BroadcastCommandData();

    bool SendBeadTypeMessage();
    bool SendBondTypeMessage(); 
    bool SendBondPairTypeMessage();
    bool SendPolymerTypeMessage();

	// ****************************************
	// Data members
private:

	const zString m_runId;				// id cannot change during simulation	
	const zString m_iFileName;			// input filename cannot change during simulation	

	// Pointers to key components in the simulation. These are not const as their states
	// change as the simulation evolves. However, only certain of them can alter the 
	// state of the others. 

	CInputData*			m_pInputData;
	CSimState*			m_pSimState;
	ISimBox*			m_pISimBox;

	// Interface to the experiment object allowing the simulation access during the run

	IConnectSimulation* m_pExpt;

	// Interfaces implemented by the simulation object

	IExperimentAccessControlImpl	m_IExptAccess;				// Controls access to experiment during a run
	ISecureSimulationImpl			m_ISecureSim;				// Secure incoming interface
	ISetSimulationDataImpl			m_ISetSimulationData;		// Simulation incoming interface
	ISetSubstrateDataImpl			m_ISetSubstrateData;		// Substrate incoming interface
	ISetSculptureDataImpl			m_ISetSculptureData;		// Sculpture incoming interface
	ISetProbeFieldDataImpl			m_ISetProbeFieldData;		// Probe field incoming interface
	ISetExternalFieldDataImpl		m_ISetExternalFieldData;	// External field incoming interface
	ISetProcessDataImpl				m_ISetProcessData;			// Process incoming interface

};

#endif // !defined(AFX_SIMULATION_H__A2FACF45_3F61_11D3_820E_0060088AD300__INCLUDED_)
