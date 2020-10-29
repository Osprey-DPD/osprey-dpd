// evLamellaCusp.h: interface for the evLamellaCusp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLAMELLACUSP_H__38282397_58E2_436E_A5B1_295CD7AEA887__INCLUDED_)
#define AFX_EVLAMELLACUSP_H__38282397_58E2_436E_A5B1_295CD7AEA887__INCLUDED_


// Forward declarations

class IEventLamellaCusp;



#include "xxEvent.h"

class evLamellaCusp : public xxEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	evLamellaCusp();
	evLamellaCusp(const evLamellaCusp& oldEvent);

	virtual ~evLamellaCusp();

	// ****************************************
	// Global functions, static member functions and variables
public:

		static const zString GetType();	// Return the type of event

		static const zString m_Type;	// Identifier used in control data file
	
	// ****************************************
	// Public access functions
public:

	inline const zString GetAggregateName()			const {return m_AggregateName;}
	inline long		     GetAggregateId()			const {return m_AggregateId;}
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

	IEventLamellaCusp* m_pIEvent;	// Pointer to the IEventXXX event interface class

	zString m_AggregateName;		// Name of aggregate to monitor
	long	m_AggregateId;			// Unique id of aggregate
	double  m_MinCuspCurvature;		// Minimum curvature for a cusp to register

	StringSequence	m_Polymers;		// Polymers to check for pore

	// Local data

	zLongVector	m_PolymerTypes;
	zLongVector	m_HeadBeadTypes;
};

#endif // !defined(AFX_EVLAMELLACUSP_H__38282397_58E2_436E_A5B1_295CD7AEA887__INCLUDED_)
