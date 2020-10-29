// evVesiclePore.h: interface for the evVesiclePore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVVESICLEPORE_H__0C3D81F0_C32F_4126_8AFF_29D04AA6BE14__INCLUDED_)
#define AFX_EVVESICLEPORE_H__0C3D81F0_C32F_4126_8AFF_29D04AA6BE14__INCLUDED_


// Forward declarations

class IEventVesiclePore;


#include "xxEvent.h"

class evVesiclePore : public xxEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	evVesiclePore();
	evVesiclePore(const evVesiclePore& oldEvent);

	virtual ~evVesiclePore();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of event

	static const zString m_Type;	// Identifier used in control data file
	
	// ****************************************
	// Public access functions
public:

	inline const zString GetAggregateName()		const {return m_AggregateName;}
	inline long			 GetAggregateId()		const {return m_AggregateId;}

	// ****************************************
	// PVFs inherited from xxEvent
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimEvent* const pISimEvent);

	virtual const zString GetEventType() const;

	virtual xxEvent* GetEvent() const;

	virtual bool ValidateData(const CInputData &riData);

protected:

	virtual bool SetAggregate(CAnalysis* const pAggregate);


	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation

	// ****************************************
	// Private functions
private:							

	// ****************************************
	// Data members
private:							// Local data specified by user

	IEventVesiclePore* m_pIEvent;		// Pointer to the IEventXXX event interface class

	zString m_AggregateName;			// Name of aggregate to monitor
	long	m_AggregateId;				// Unique id of aggregate


	// Local data


};

#endif // !defined(AFX_EVVESICLEPORE_H__0C3D81F0_C32F_4126_8AFF_29D04AA6BE14__INCLUDED_)
