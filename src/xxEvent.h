// xxEvent.h: interface for the xxEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXEVENT_H__B1524EDB_A8AA_49EA_9A73_1AAE58A98FBB__INCLUDED_)
#define AFX_XXEVENT_H__B1524EDB_A8AA_49EA_9A73_1AAE58A98FBB__INCLUDED_

// Forward declarations

class ISimEvent;
class CInputData;
class CSimState;


// Include files

#include "acfEventFactory.h"	// Needed to create xxEvents


#include "xxBase.h"

class xxEvent : public xxBase  
{
	friend class xxProcess;			// Allows xxProcess to change xxEvent

	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	virtual ~xxEvent();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const xxEvent& event);
	friend zInStream&  operator>>(zInStream& is,        xxEvent& event);

	static long GetEventTotal();
	static void ZeroEventTotal();

private:

	static long m_EventTotal;		// Number of events created

	// ****************************************
	// PVS that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxEvent pointer,
	// respond to the SimBox's request to execute and return the name of the event,
	// and validate their data given the CInputData object to get access to the
	// SimBox definitions.

	virtual zOutStream& put(zOutStream& os) const;
	virtual zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimEvent* const pISimEvent) = 0;

	virtual const zString GetEventType() const = 0;

	virtual xxEvent* GetEvent() const = 0;

	// Function used by the CInputData to validate the event.
	// It cannot be const because the event keeps a pointer 
	// to its associated aggregate after validation.

	virtual bool ValidateData(const CInputData &riData) = 0;

	// VF used to validate events created during the simulation
	// that are not specified in the CDF. A default do-nothing body
	// is provided so that only events that need to be created on-the-fly
	// have to implement it.

	virtual bool InternalValidateData(const CSimState& rSimState, xxEvent* const pEvent);

protected:

	// Each derived class must store a pointer to its respective CAnalysis-derived 
	// class rather than doing dynamic_cast every time we want to access the aggregate.

	virtual bool SetAggregate(CAnalysis* const pAggregate) = 0;

	// ****************************************
	// Public access functions
public:

	inline long GetId()					const {return m_id;}
	inline long GetStartTime()			const {return m_Start;}
	inline long GetEndTime()			const {return m_End;}
	inline bool IsValid()				const {return m_bValid;}
	inline bool IsActive()				const {return m_bActive;}
	inline bool InitialiseEvent()		const {return m_bInitialise;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	xxEvent();
	xxEvent(const xxEvent& oldEvent);
	
	inline void SetDataValid(bool bValid)	       {m_bValid	  = bValid;}
	inline void ActivateEvent()				       {m_bActive     = true;}
	inline void InActivateEvent()			       {m_bActive     = false;}
	inline void ReInitialiseEvent()			       {m_bInitialise = true;}
	inline void UnInitialiseEvent()			       {m_bInitialise = false;}

    
	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	inline void SetStartTime(long start)	  {m_Start	= start;}
	inline void SetEndTime(long end)		  {m_End	= end;}

	// ****************************************
	// Data members

private:
	const long	m_id;				// Unique identifier for the event

	long  m_Start;					// Time at which event starts executing
	long  m_End;					// Time at which event stops executing
	
	bool m_bValid;					// Flag showing if event's data has been validated
	bool m_bActive;					// Flag showing if event is active
	bool m_bInitialise;				// Flag indicating whether to initialise event
};

#endif // !defined(AFX_XXEVENT_H__B1524EDB_A8AA_49EA_9A73_1AAE58A98FBB__INCLUDED_)

