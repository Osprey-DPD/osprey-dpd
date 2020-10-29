// evLamellaFlipflop.h: interface for the evLamellaFlipflop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVLAMELLAFLIPFLOP_H__C9C7192D_3516_4EDE_962A_B19508B415B6__INCLUDED_)
#define AFX_EVLAMELLAFLIPFLOP_H__C9C7192D_3516_4EDE_962A_B19508B415B6__INCLUDED_


// Forward declarations

class CBilayer;


#include "xxEvent.h"

class evLamellaFlipflop : public xxEvent  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	evLamellaFlipflop();
	evLamellaFlipflop(const evLamellaFlipflop& oldEvent);

	virtual ~evLamellaFlipflop();

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
	inline const StringSequence GetPolymerNames()	const {return m_Polymers;}


	// ****************************************
	// PVFs inherited from xxEvent
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
private:							// Data specific to the event

	zString m_AggregateName;		// Name of CAnalysis-derived class to monitor
	long	m_AggregateId;			// Unique id of aggregate to monitor
	StringSequence	m_Polymers;		// Polymers to check for flipping

	double	m_MinDistance;			// Minimum distance from monolayer heads


	// Local data

	CBilayer* m_pBilayer;			// Pointer to CAnalysis-derived class to monitor

	CellProfileSetVector  m_vBilayerProfiles;	// Cell profiles spanning bilayer

	long m_X;						// Normal to bilayer
	long m_Y;
	long m_Z;

	long m_SliceTotal;				// No of slices in SimBox profile
	long m_RowTotal;				// Number of rows used per slice in shape analysis
	long m_CellTotal;				// No of cells used per row in shape analysis


	zLongVector	m_PolymerTypes;		// Vector of polymer types
	zLongVector	m_HeadBeadTypes;	// Vector of polymer types

	LongBeadMap m_mBilayer;			// List of lipid head beads both monolayers
	LongBeadMap m_mUpper;			// List of lipid head beads in upper/lower monolayers
	LongBeadMap m_mLower;

	zLongVector m_UtoLCounter;		// Upper to lower flip counter
	zLongVector m_LtoUCounter;		// Lower to upper flip counter
	zLongVector m_DiffCounter;		// Difference counter


};

#endif // !defined(AFX_EVLAMELLAFLIPFLOP_H__C9C7192D_3516_4EDE_962A_B19508B415B6__INCLUDED_)
