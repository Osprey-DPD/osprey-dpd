// evLamellaPoreClosed.h: interface for the evLamellaPoreClosed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLAMELLAPORECLOSED_H__95597E3F_9ED9_4EA3_9AE8_D3BBFEFB4574__INCLUDED_)
#define AFX_EVLAMELLAPORECLOSED_H__95597E3F_9ED9_4EA3_9AE8_D3BBFEFB4574__INCLUDED_


// Forward declarations

class IEventLamellaPoreClosed;
class CSimState;					// Needed for InternalValidateData() method


#include "xxEvent.h"

class evLamellaPoreClosed : public xxEvent  
{
	// ****************************************
	// Construction/Destruction
public:
	evLamellaPoreClosed();
	evLamellaPoreClosed(const evLamellaPoreClosed& oldEvent);

	virtual ~evLamellaPoreClosed();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of event

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// Public access functions
public:

	inline const zString GetAggregateName()	const {return m_AggregateName;}
	inline long		     GetAggregateId()			const {return m_AggregateId;}
	inline double		 GetMinSize()				const {return m_MinSize;}
	inline const StringSequence GetPolymerNames()	const {return m_Polymers;}

	// Validation for internally-created events

	bool InternalValidateData(const CSimState& rSimState, xxEvent* const pEvent);

	// ****************************************
	// PVS inherited from xxEvent base class
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	bool Execute(long simTime, ISimEvent* const pISimEvent);

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

	IEventLamellaPoreClosed* m_pIEvent;	// Pointer to the IEventXXX event interface class

	zString m_AggregateName;		// Name of CAnalysis-derived class to monitor
	long	m_AggregateId;			// Unique id of aggregate to monitor
	double	m_MinSize;				// Minimum pore size (in grid points)

	StringSequence	m_Polymers;		// Polymers to check for pore

	// Local data 

	zLongVector	m_PolymerTypes;
	zLongVector	m_HeadBeadTypes;

};

#endif // !defined(AFX_EVLAMELLAPORECLOSED_H__95597E3F_9ED9_4EA3_9AE8_D3BBFEFB4574__INCLUDED_)
