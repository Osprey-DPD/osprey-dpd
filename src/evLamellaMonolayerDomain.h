// evLamellaMonolayerDomain.h: interface for the evLamellaMonolayerDomain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLAMELLAMONOLAYERDOMAIN_H__F50B2CED_7FDB_4456_A7C4_3F888EC2ABC3__INCLUDED_)
#define AFX_EVLAMELLAMONOLAYERDOMAIN_H__F50B2CED_7FDB_4456_A7C4_3F888EC2ABC3__INCLUDED_


// Forward declarations

class IEventLamellaMonolayerDomain;


#include "xxEvent.h"

class evLamellaMonolayerDomain : public xxEvent
{
	// ****************************************
	// Construction/Destruction
public:

	evLamellaMonolayerDomain();
	evLamellaMonolayerDomain(const evLamellaMonolayerDomain& oldEvent);

	virtual ~evLamellaMonolayerDomain();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of event

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// Public access functions
public:

	inline const zString		GetAggregateName()		const {return m_AggregateName;}
	inline long					GetAggregateId()		const {return m_AggregateId;}
	inline double				GetMinSize()			const {return m_MinSize;}
	inline const StringSequence GetDomainPolymerNames()	const {return m_DomainPolymers;}
	inline const StringSequence GetSeaPolymerNames()	const {return m_DomainPolymers;}


	// ****************************************
	// PVS inherited from xxEvent base class
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

	IEventLamellaMonolayerDomain* m_pIEvent;	// Pointer to the IEventXXX event interface class

	zString m_AggregateName;			// Name of aggregate to monitor
	long	m_AggregateId;				// Unique id of aggregate
	bool	m_bOuterMonolayer;			// Flag showing which monolayer to monitor
	double	m_MinSize;					// Minimum domain size (# of polymers of all types)

	StringSequence	m_DomainPolymers;	// Polymers composing domains
	StringSequence	m_SeaPolymers;		// Polymers composing sea around domains

	// Local data

	zLongVector	m_DomainPolymerTypes;
	zLongVector	m_SeaPolymerTypes;
	zLongVector	m_DomainHeadBeadTypes;
	zLongVector	m_SeaHeadBeadTypes;

};

#endif // !defined(AFX_EVLAMELLAMONOLAYERDOMAIN_H__F50B2CED_7FDB_4456_A7C4_3F888EC2ABC3__INCLUDED_)
