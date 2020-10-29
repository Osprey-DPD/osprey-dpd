// AnalysisState.h: interface for the CAnalysisState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSISSTATE_H__D70C3D01_3D5E_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_ANALYSISSTATE_H__D70C3D01_3D5E_11D3_820E_0060088AD300__INCLUDED_


// Forward declarations

class CInputData;


#include "xxState.h"
#include "ISerialiseInclusiveRestartState.h"

class CAnalysisState : public xxState, public ISerialiseInclusiveRestartState
{
	friend class CSimState;	// Allow commands to update data stored here

	// ****************************************
	// Construction/Destruction
public:

	CAnalysisState(const CInputData& rData);

	virtual ~CAnalysisState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	bool Serialize();

    // Implementation of the ISerialiseInclusiveRestartState interface
    // to allow this class to read/write data that can be modified
    // for restarts.

    virtual zInStream& Read(zInStream& is);
    virtual zOutStream& Write(zOutStream& is) const;

	// ****************************************
	// Public access functions
public:

	void AddProcess(xxProcess* pProcess);
	void AddEvent(xxEvent* pEvent);
	void AddCommand(const xxCommand* const pCmd);
	void AddGlobalData(ObservableSequencePointer pvObservables);

	// Functions to access the sets of processes, aggregates, events and 
	// commands that the simulation monitors.  We don't pass the command set 
	// by reference as commands may be removed from the sequence by the 
	// SimBox and we don't want to alter the original set stored here. 

	inline const ProcessSequence&   GetProcesses()   const {return m_Processes;}
	inline const AggregateSequence& GetAggregates()  const {return m_Analysis;}
	inline const EventSequence&		GetEvents()		 const {return m_Events;}
	inline		 CommandSequence	GetCommands()	 const {return m_Commands;}

	// Public access functions

	inline long GetGridXCellNo()	const {return m_GridXCellNo;}
	inline long GetGridYCellNo()	const {return m_GridYCellNo;}
	inline long GetGridZCellNo()	const {return m_GridZCellNo;}

	inline long GetTotalMCTime()	const {return m_TotalMCTime;}
	inline long GetTotalTime()		const {return m_TotalTime;}
	inline long GetSamplePeriod()	const {return m_SamplePeriod;}
	inline long GetAnalysisPeriod() const {return m_AnalysisPeriod;}
	inline long GetDensityPeriod()	const {return m_DensityPeriod;}
	inline long GetDisplayPeriod()	const {return m_DisplayPeriod;}
	inline long GetRestartPeriod()	const {return m_RestartPeriod;}

	inline double GetLambda()			const {return m_Lambda;}
	inline double GetMCStepSize()		const {return m_MCStepSize;}
	inline double GetIntegrationStep()	const {return m_StepSize;}

	// Bead maps

	bool IsBeadNameInMap(const zString name) const;
	bool IsBeadTypeInMap(long type) const;
	bool AddNamedBeadTypeToMap(long type, const zString name);

	long GetBeadTypeFromName(const zString& name) const;
	zString GetBeadNameFromType(long type) const;

	// Bond maps

	bool IsBondNameInMap(const zString name) const;
	bool IsBondTypeInMap(long type) const;
	bool AddNamedBondTypeToMap(long type, const zString name);

	long GetBondTypeFromName(const zString& name) const;
	zString GetBondNameFromType(long type) const;

	long GetBondHeadType(long bondType) const;
	long GetBondTailType(long bondType) const;

	// Bondpair maps

	bool IsBondPairNameInMap(const zString name) const;
	bool IsBondPairTypeInMap(long type) const;
	bool AddNamedBondPairTypeToMap(long type, const zString name);

    long GetBondPairTypeFromName(const zString& name) const;
	zString GetBondPairNameFromType(long type) const;

	long GetBondPairHeadType(long bondPairType) const;
	long GetBondPairMiddleType(long bondPairType) const;
	long GetBondPairTailType(long bondPairType) const;

	// Polymer maps

	bool IsPolymerNameInMap(const zString name) const;
	bool IsPolymerTypeInMap(long type) const;
    // Note that there is no function to add a new polymer type as we don't
    // allow this feature yet.

    long GetPolymerTypeFromName(const zString& name) const;
	zString GetPolymerNameFromType(long type) const;

	long GetPolymerHeadType(long polyType) const;
	long GetPolymerTailType(long polyType) const;

	const zString GetPolymerShapeFromType(long polyType) const;
	const zString GetSpaceWrappedPolymerShapeFromType(long polyType) const;

	// Process maps

	ProcessSequence GetModifiableProcesses() const;
	IModifyProcess* GetModifiableProcessFromName(const zString name) const;
	IModifyProcess* GetModifiableProcessFromPid(const zString pid) const;

	// ****************************************
	// Protected local functions
protected:

	// Functions used by CMonitor so that the CAnalysisState can be updated
	// in response to commands that change analysis parameters.
	// Note that the current time is protected and stored in the xxState base class.

	inline void SetCurrentTime(long currentTime) {m_CurrentTime = currentTime;}
	inline void	SetAnalysisPeriod(long period)   {m_AnalysisPeriod = period;}
	inline void	SetDensityPeriod(long period)    {m_DensityPeriod = period;}
	inline void	SetDisplayPeriod(long period)    {m_DisplayPeriod = period;}
	inline void	SetRestartPeriod(long period)    {m_RestartPeriod = period;}
	inline void	SetSamplePeriod(long period)     {m_SamplePeriod = period;}

	inline void SetIntegrationStep(double dt)	 {m_StepSize  = dt;}
	inline void SetTotalTime(long newTime)		 {m_TotalTime = newTime;}

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	ProcessSequence		m_Processes;
	AggregateSequence	m_Analysis;
	EventSequence		m_Events;				// Events may have their data changed
	CommandSequence		m_Commands;				// Commands are const

    StringSequence      m_CommandNames;         // Names of scheduled commands on PN

	ObservableSequencePointer m_pvObservables;	// pointer to a vector of global observables

	const long m_GridXCellNo;
	const long m_GridYCellNo;
	const long m_GridZCellNo;

	const long m_TotalMCTime;

	long m_TotalTime;			// May be changed by command
	long m_SamplePeriod;
	long m_AnalysisPeriod;
	long m_DensityPeriod;		// May be changed by command
	long m_DisplayPeriod;		// May be changed by command
	long m_RestartPeriod;		// May be changed by command

	const double m_Lambda;
	const double m_RCutOff;
	const double m_MCStepSize;
	double m_StepSize;			// May be changed by command

	StringLongMap	m_mBeadTypeFromName;		// Bead name/type maps
	StringLongMap	m_mBondTypeFromName;		// Bond name/type maps
	StringLongMap	m_mBondPairTypeFromName;	// Bond pair name/type maps
	StringLongMap	m_mPolymerTypeFromName;		// Polymer name/type maps

	LongStringMap	m_mBeadNameFromType;
	LongStringMap	m_mBondNameFromType;
	LongStringMap	m_mBondPairNameFromType;
	LongStringMap	m_mPolymerNameFromType;

	LongStringMap	m_mPolymerShapes;			// Map from polymer types to their shape (zString) strings

	LongLongMap		m_mBondHeadTypes;			// Head bead type/bond type map
	LongLongMap		m_mBondTailTypes;			// Tail bead type bond type map

	LongLongMap		m_mBondPairHeadTypes;		// Head bead type/bondpair type map
	LongLongMap		m_mBondPairMiddleTypes;		// Middle bead type/bondpair type map
	LongLongMap		m_mBondPairTailTypes;		// Tail bead type bondpair type map

	LongLongMap		m_mPolymerHeadTypes;		// Head bead type/polymer type map
	LongLongMap		m_mPolymerTailTypes;		// Tail bead type/polymer type map

	StringModifiableProcessMap m_mModifiableProcesses;	// Id/modifiable process map

};

#endif // !defined(AFX_ANALYSISSTATE_H__D70C3D01_3D5E_11D3_820E_0060088AD300__INCLUDED_)
