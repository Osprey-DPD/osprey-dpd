// evLamellaPore.h: interface for the evLamellaPore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLAMELLAPORE_H__6A6BC548_DD9D_4C25_8FE7_61234ED3F368__INCLUDED_)
#define AFX_EVLAMELLAPORE_H__6A6BC548_DD9D_4C25_8FE7_61234ED3F368__INCLUDED_


// Forward declarations

class IEventLamellaPore;


#include "xxEvent.h"

class evLamellaPore : public xxEvent  
{
	// ****************************************
	// Construction/Destruction
public:
	evLamellaPore();
	evLamellaPore(const evLamellaPore& oldEvent);

	virtual ~evLamellaPore();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of event

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// Public access functions

	inline const zString GetAggregateName()			const {return m_AggregateName;}
	inline long		     GetAggregateId()			const {return m_AggregateId;}
	inline double		 GetMinSize()				const {return m_MinSize;}
	inline const StringSequence GetPolymerNames()	const {return m_Polymers;}


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

	IEventLamellaPore* m_pIEvent;	// Pointer to the IEventXXX event interface class

	zString m_AggregateName;		// Name of aggregate to monitor
	long	m_AggregateId;			// Unique id of aggregate
	double	m_MinSize;				// Minimum pore size (in grid points)

	StringSequence	m_Polymers;		// Polymers to check for pore

	// Local data

	zLongVector	m_PolymerTypes;
	zLongVector	m_HeadBeadTypes;

};

#endif // !defined(AFX_EVLAMELLAPORE_H__6A6BC548_DD9D_4C25_8FE7_61234ED3F368__INCLUDED_)
