// aevActiveEvent.h: interface for the aevActiveEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEVACTIVEEVENT_H__C6AF905C_8B57_4AD5_AB2A_8A1EC94718CD__INCLUDED_)
#define AFX_AEVACTIVEEVENT_H__C6AF905C_8B57_4AD5_AB2A_8A1EC94718CD__INCLUDED_


// Forward declarations

class IActiveSimBox;
class aeActiveCellNetwork;
class aeActiveBond;
class aeActivePolymer;



// Include files

#include "acfActiveEventFactory.h"	// Needed to create aevActiveEvents


#include "xxBase.h"

class aevActiveEvent : public xxBase  
{
	friend class xxProcess;			// Allows xxProcess to change aevActiveEvent

	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~aevActiveEvent();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const aevActiveEvent& event);
	friend zInStream&  operator>>(zInStream& is,        aevActiveEvent& event);

	static long GetEventTotal();
	static void ZeroEventTotal();

private:

	static long m_EventTotal;		// Number of events created

	// ****************************************
	// PVS that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an aevActiveEvent pointer,
	// respond to the SimBox's request to execute and return the name of the event,
	// and validate their data given the CInputData object to get access to the
	// SimBox definitions.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual bool CheckPreconditions() = 0;
	virtual bool Execute(IActiveSimBox* const pShadow) = 0;

	virtual const zString GetEventType() const = 0;

	virtual aevActiveEvent* GetEvent() const = 0;


	// Function used to validate active events created during the simulation.
	// Note that these events cannot be created in the CDF so there is no
	// corresponding ValidateData() function as for xxEvents.

	virtual bool InternalValidateData();

protected:

    // VF that allows derived classes to inform their containing instance when their
    // state changes. A do-nothing implementation is provided here.

	virtual void BroadcastState();


	// ****************************************
	// Public access functions
public:

	aeCNTCell* GetCellFromBond(const aeActiveBond* const pBond) const;

	inline long GetId()					 const {return m_id;}
    inline long GetExecutionPeriod()     const {return m_ExecutionPeriod;}
    inline bool GetBroadcastState()     const {return m_bBroadcastState;}
	inline bool IsValid()				 const {return m_bValid;}
	inline bool IsDependent()		     const {return m_bDependent;}
	inline bool IsActive()               const {return m_bIsActive;}
	inline aeActiveCellNetwork* GetACN() const {return m_pACN;}

	bool IsAnyContraEventActive() const;
	bool AreAllContraEventsActive() const;
    bool IsTimerZero()const;

	void SetContainingACN(aeActiveCellNetwork* const pACN);

	void AddDependentEvent(aevActiveEvent* const pEvent);
	void AddContraEvent(aevActiveEvent* const pEvent);

    long GetCurrentTime() const;
	long GetSuccessCounter() const;
	long GetFailureCounter() const;
    void ResetAllCounters();
	void ResetSuccessCounter();
	void ResetFailureCounter();
 	void SetExecutionPeriod(long time);
	void ResetExecutionPeriod(long time);

    inline void SetBroadcastState(bool bNewState)  {m_bBroadcastState = bNewState;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	aevActiveEvent();
	aevActiveEvent(long period);
	aevActiveEvent(const aevActiveEvent& oldEvent);
	
	inline void SetDataValid(bool bValid)	  {m_bValid	= bValid;}
	inline void SetDependent(bool bDependent) {m_bDependent	= bDependent;}
	void SetActive();
	void SetInactive();

    // Functions used by derived classes to increment their counters.
    // Accessor functions are public to allow ACNs and processes access to them.

	void IncrementSuccesses();
	void IncrementFailures();

    // Functions used by derived classes to manipulate their execution timer.

	void ResetTimer();
    bool DecrementTimer();

	// Forwarding functions to the event's ACN

	void AddFreeBond(aeActiveBond* const pBond);
	void RemoveFreeBond(aeActiveBond* const pBond);
	ActiveBondListIterator RemoveFreeBondByIterator(ActiveBondListIterator iterBond);

	void AddFreePolymer(aeActivePolymer* pPolymer);
	void RemoveFreePolymer(aeActivePolymer* const pPolymer);

	aevActiveEvent* AddEvent(const zString type);



	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

private:
	const long	m_id;				// Unique identifier for the event

	bool m_bValid;					// Flag showing if event's data has been validated
	bool m_bDependent;				// Flag showing if the event is controlled by another event
	bool m_bIsActive;				// Flag showing if the event is currently active
    bool m_bBroadcastState;         // Flag indicating if event should broadcast its state
	aeActiveCellNetwork* m_pACN;	// Pointer to the event's containing ACN

	ActiveEventList m_DependentEvents;	// Set of events that should be destroyed when this event is destroyed
	
	ActiveEventList m_ContraEvents;		// Set of events that should be checked during this event's execution

    // Counters for events to keep track of their activation

    long m_SuccessCounter;           // Successful activations
    long m_FailureCounter;           // Failed activations

    long m_ExecutionPeriod;          // Period of event's execution checks
    long m_Timer;                    // Counter to check if execution is due
};

#endif // !defined(AFX_AEVACTIVEEVENT_H__C6AF905C_8B57_4AD5_AB2A_8A1EC94718CD__INCLUDED_)
