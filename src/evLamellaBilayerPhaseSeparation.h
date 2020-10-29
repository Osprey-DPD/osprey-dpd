// evLamellaBilayerPhaseSeparation.h: interface for the evLamellaBilayerPhaseSeparation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLAMELLABILAYERPHASESEPARATION_H__BE349C5E_49CF_444C_94BF_EBE27C90236B__INCLUDED_)
#define AFX_EVLAMELLABILAYERPHASESEPARATION_H__BE349C5E_49CF_444C_94BF_EBE27C90236B__INCLUDED_


// Forward declarations

class CCompositeBilayer;


#include "xxEvent.h"

class evLamellaBilayerPhaseSeparation : public xxEvent  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	evLamellaBilayerPhaseSeparation();
	evLamellaBilayerPhaseSeparation(const evLamellaBilayerPhaseSeparation& oldEvent);

	virtual ~evLamellaBilayerPhaseSeparation();

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

	zString m_AggregateName;		// Name of aggregate to monitor
	long	m_AggregateId;			// Unique id of aggregate
	double	m_MinSize;				// Minimum pore size (in grid points)

	StringSequence	m_Polymers;		// Polymers to check for pore

	// Local data

	zLongVector	m_PolymerTypes;
	zLongVector	m_HeadBeadTypes;

	CCompositeBilayer* m_pBilayer;			// Pointer to CAnalysis-derived class to monitor
};


#endif // !defined(AFX_EVLAMELLABILAYERPHASESEPARATION_H__BE349C5E_49CF_444C_94BF_EBE27C90236B__INCLUDED_)
