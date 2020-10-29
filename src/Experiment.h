// Experiment.h: interface for the CExperiment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPERIMENT_H__6D752554_4229_4BC1_88DB_209C9D7D9660__INCLUDED_)
#define AFX_EXPERIMENT_H__6D752554_4229_4BC1_88DB_209C9D7D9660__INCLUDED_


// Forward declarations

class xxProtocol;
class ISimulation;
class xxSimulationTool;
class xxSubstrateTool;
class xxSculptureTool;
class xxProbeFieldTool;
class xxExternalFieldTool;
class xxProcessTool;
class ISetProcessData;

// Include files for the experiment's nested interfaces

#include "ISecureExperimentImpl.h"
#include "IDocumentHandlerImpl.h"
#include "ICommandDocumentImpl.h"
#include "IProtocolDocumentImpl.h"
#include "IConnectSimulationImpl.h"


#include "IExperiment.h"

class CExperiment : public IExperiment  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~CExperiment();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// Protected constructor and the public Instance() functions ensure that
	// only a single CExperiment object can be created.  The bNormalRun flag
    // indicates if this is a normal experiment or a previsualization run
    // to generate a single initial state snapshot.

	static IExperiment* Instance(const zString protocolName, const zString runId, bool bNormalRun);


private:

	static IExperiment* m_pInstance;		

	
	// ****************************************
	// PVFs derived from base class: no classes should inherit from this class
	// as it is a singleton
public:

	bool ReadProtocolDocument();
	bool RunPrevis(StringSequence excludedPolymers);
	bool Run();

	ISecureExperiment* GetISecureExperiment();
	IDocumentHandler*  GetIDocumentHandler();
	ICommandDocument*  GetICommandDocument();
	IProtocolDocument* GetIProtocolDocument();

	// ****************************************
	// Public access functions
public:

	bool IsProtocolValid() const {return m_bProtocolValid;}
	bool IsExternalFieldActive() const;
	bool IsProbeFieldActive()	 const;


	// Functions that allow the experiment's interface implementations
	// to access the simulation's interfaces

	ISetProcessData* GetISetProcessData() const;



	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that the experiment can only be instantiated
	// using the static Instance member

	CExperiment(const zString protocolName, const zString runId);

	// Constructor for use when running a stand-alone simulation

	CExperiment(const zString runId);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

protected:




private:

	// Flags showing the state of the experiment's data and protocol requirements

	bool m_bProtocolValid;			// Flag showing if the protocol is recognised

	// Sub-objects created by the experiment

	xxProtocol*		m_pProtocol;		// "Script" to run the simulation
	ISimulation*	m_pISimulation;		// Interface to simulation instance
//	IProbeField*	m_pIProbeField;		// Interface to the probe field instance
//	IExternalField*	m_pIExternalField;	// Interface to the external field instance

	// xxTools used by the experiment to validate and store the data read 
	// from the protocol document. The protocol is divided into major sections
	// each of which is handled by a separate tool.

	xxSimulationTool*	  m_pSimTool;
	xxSubstrateTool*	  m_pSubstrateTool;
	xxSculptureTool*      m_pSculptureTool;
	xxProbeFieldTool*	  m_pProbeTool;
	xxExternalFieldTool*  m_pExternalTool;
	xxProcessTool*        m_pProcessTool;


	// Interfaces implemented by the experiment object

	ISecureExperimentImpl	m_ISecureExpt; //Secure incoming interface
	IDocumentHandlerImpl	m_IDocHandler;
	ICommandDocumentImpl	m_ICommandDoc;
	IProtocolDocumentImpl	m_IProtocolDoc;
	IConnectSimulationImpl	m_IConnectSim;


};

#endif // !defined(AFX_EXPERIMENT_H__6D752554_4229_4BC1_88DB_209C9D7D9660__INCLUDED_)
