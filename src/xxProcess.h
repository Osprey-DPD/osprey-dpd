// xxProcess.h: interface for the xxProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXPROCESS_H__34CD2269_1B08_4E00_9DD7_8C924EF6850D__INCLUDED_)
#define AFX_XXPROCESS_H__34CD2269_1B08_4E00_9DD7_8C924EF6850D__INCLUDED_

// Forward declarations

class CSimState;
class ISimState;
class ISimBox;
class CInputData;
class CTimeSeriesData;

// Include files that contain flags to control conditional compilation
// of derived classes, and the class factory that creates processes.

#include "SimProcessFlags.h"
#include "acfProcessFactory.h"	// Needed to create xxProcesses
#include "xxProcessState.h"		// Needed to make AddTimeSeriesData inline for speed

#include "xxBase.h"

class xxProcess : public xxBase
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~xxProcess();


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const xxProcess& process);
	friend zInStream&  operator>>(zInStream& is,        xxProcess& process);

	static long GetProcessTotal();
	static void ZeroProcessTotal();

private:

	static long m_ProcessTotal;	// Number of processes created

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxProcess pointer,
	// and respond to the SimBox's request to sample their data.

	virtual zOutStream& put(zOutStream& os) const = 0;
	virtual zInStream&  get(zInStream& is)        = 0;

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox) = 0;

	virtual xxProcess* GetProcess()  const = 0;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProcessType() const = 0;

	// Function to allow the CInputData object to check the process data:
	// it cannot be const because processes store data relating to the events
	// and commands they use.

	virtual bool ValidateData(const CInputData &riData) = 0;

	// Function to allow processes that are created on-the-fly to
	// validate their data and create their associated process states.
	// We provide a do-nothing body that can be overridden by any process
	// that needs to do so.

	virtual bool InternalValidateData(const ISimState* const pISimState);

	// ****************************************
	// Public access functions
public:

	inline long			  GetId()		const {return m_id;}
	inline const zString GetPid()		const {return m_pid;}
	inline long    GetStartTime()		const {return m_Start;}
	inline long      GetEndTime()		const {return m_End;}	

	inline bool  IsProcessValid()		const {return m_bProcessValid;}

	bool SaveState()					const;

	void SetStartTime(long start);
	void SetEndTime(long end);

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor and PVF's ensure that only derived classes can be 
	// instantiated. Only derived classes can store an xxStateObject and pass
	// time series data to it.

	xxProcess();
	xxProcess(bool bValid);
	xxProcess(const xxProcess& oldProcess);

	void SetDataValid(bool bValid);
	void SetState(xxProcessState* pState);

	inline bool InitialiseProcess()		const {return m_bInitialise;}
	inline void UnInitialiseProcess()		  {m_bInitialise = false;}

	// Functions to add a newly-created aggregate, command and event to the process.
	// The event function is not inline so that it can set some data.

	inline void AddAggregate(CAnalysis* pAgg) {m_Aggregates.push_back(pAgg);}
	inline void AddCommand(xxCommand* pCmd)	  {m_Commands.push_back(pCmd);}

	void AddEvent(xxEvent* pEvent, long start, long end);	

	// Functions to allow a process to modify its events

	void ActivateEvent(long id);
	void InActivateEvent(long id);
	void DeleteEvent(long id);

	inline void AddTimeSeriesData(CTimeSeriesData *pTSD) {m_pState->AddTimeSeriesData(pTSD);}


// ****************************************
// Implementation of the IModifyProcess interface
// Any derived class that implements the IModifyProcess interface must provide functions
// that forward to these internal functions.

	bool InternalIsModifiable() const;
	void InternalSetModifiable(const xxCommand* const pCommand);
	void InternalSetUnmodifiable(const xxCommand* const pCommand);
	void InternalSetIrreversibleUnmodifiable(const xxCommand* const pCommand);

	// Protected data accessible to all derived classes. 
	//
	// Note that all derived classes are responsible for specifying, and storing, 
	// their required aggregates, events and commands. We allow processes to 
	// create aggregates, events and commands on-the-fly during a 
	// simulation and store them here. This allows them to be destroyed when 
	// the process is destroyed, thus keeping them separate from the global 
	// aggregates, events, commands read in from the CDF file.
	// Messages are not stored here because they are automatically transferred
	// on creation to the CLogState object that stores and deletes them.

protected:

	AggregateSequence	m_Aggregates;	// Aggregates created by the process (not ones from CDF file)
	CommandSequence		m_Commands;		// Commands generated by the process	"
	EventSequence		m_Events;		// Events generated by the process		"
	
	xxProcessState* m_pState;	// Serialisation class for the process

	// ****************************************
	// Implementation

	// Data required to implement the IModifyProcess interface. 

	bool m_bIsModifiable;			// Flag showing if the process is currently modifiable
	bool m_bNeverModifiable;		// Flag showing if the process is never modifiable


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members

private:

	const long m_id;			// Unique identifier for the process
	
	zString   m_pid;			// user-assigned unique identifier for the process

	long  m_Start;				// Time at which process starts executing
	long  m_End;				// Time at which process stops executing

	bool  m_bProcessValid;		// Flag showing if process has been validated
	bool  m_bInitialise;		// Flag indicating whether to initialise

};

#endif // !defined(AFX_XXPROCESS_H__34CD2269_1B08_4E00_9DD7_8C924EF6850D__INCLUDED_)
