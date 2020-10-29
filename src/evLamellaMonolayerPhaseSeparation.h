// evLamellaMonolayerPhaseSeparation.h: interface for the evLamellaMonolayerPhaseSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLAMELLAMONOLAYERPHASESEPARATION_H__D9DE9161_A02F_4E66_92F6_F082F1A753E7__INCLUDED_)
#define AFX_EVLAMELLAMONOLAYERPHASESEPARATION_H__D9DE9161_A02F_4E66_92F6_F082F1A753E7__INCLUDED_


// Forward declarations

class IEventLamellaMonolayerPhaseSeparation;


#include "xxEvent.h"

class evLamellaMonolayerPhaseSeparation : public xxEvent  
{
	// ****************************************
	// Construction/Destruction

public:
	evLamellaMonolayerPhaseSeparation();
	evLamellaMonolayerPhaseSeparation(const evLamellaMonolayerPhaseSeparation& oldEvent);

	virtual ~evLamellaMonolayerPhaseSeparation();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of event

	static const zString m_Type;	// Identifier used in control data file

	
	// ****************************************
	// Public access functions
public:

	inline const zString GetAggregateName()				const {return m_AggregateName;}
	inline long		     GetAggregateId()				const {return m_AggregateId;}
	inline double		 GetMaxMissing()				const {return m_MaxMissing;}
	inline const StringSequence GetDomainPolymerNames()	const {return m_DomainPolymers;}
	inline const StringSequence GetSeaPolymerNames()	const {return m_DomainPolymers;}


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

	IEventLamellaMonolayerPhaseSeparation* m_pIEvent;	// Pointer to the IEventXXX event interface class

	zString m_AggregateName;		// Name of aggregate to monitor
	long	m_AggregateId;			// Unique id of aggregate
	bool	m_bOuterMonolayer;		// Flag showing which monolayer to monitor
	double	m_MaxMissing;			// Max percentage of polymers not in separated phase

	StringSequence	m_DomainPolymers;	// Polymers composing domains
	StringSequence	m_SeaPolymers;		// Polymers composing sea around domains

	// Local data

	zLongVector	m_DomainPolymerTypes;
	zLongVector	m_SeaPolymerTypes;
	zLongVector	m_DomainHeadBeadTypes;
	zLongVector	m_SeaHeadBeadTypes;

};

#endif // !defined(AFX_EVLAMELLAMONOLAYERPHASESEPARATION_H__D9DE9161_A02F_4E66_92F6_F082F1A753E7__INCLUDED_)
