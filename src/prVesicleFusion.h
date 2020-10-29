// prVesicleFusion.h: interface for the prVesicleFusion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRVESICLEFUSION_H__3F913F08_48FB_404B_ABD9_0295BF60846E__INCLUDED_)
#define AFX_PRVESICLEFUSION_H__3F913F08_48FB_404B_ABD9_0295BF60846E__INCLUDED_


// Include files and forward declarations for template adaptor classes

#include "IVesicleFusion.h"

class CVesicle;



#include "xxProcess.h"

class prVesicleFusion : public xxProcess
{
	// ****************************************
	// Construction/Destruction
public:

	prVesicleFusion();
	virtual ~prVesicleFusion();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the process name

private:

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// so that they may have data read into them given only an xxProcess pointer,
	// and respond to the SimBox's request to sample their data.

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual xxProcess* GetProcess() const;

	virtual const zString GetProcessType() const;	// Return the process name

	// Function to allow the CInputData object to check all processes' data

	virtual bool ValidateData(const CInputData &riData);

	// ****************************************
	// Public access functions
public:
	
	// ****************************************
	// Protected local functions
protected:

	// Functions to allow the xxProcess to store pointers to its 
	// required CAnalysis and xxEvents-derived class(es)
	// rather than doing dynamic_cast every time we want to access them.
	// We cannot provide such a function in the xxProcess base class
	// because different processes may need different numbers of aggregates
	// and events.

	void SetAggregate(const CAnalysis* pAggregate1, const CAnalysis* pAggregate2);

	void SetEvent();


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions

	// ****************************************
	// Data members

private:							// Data specific to the process

	zString m_AggregateName1;		// Name of CAnalysis-derived class to monitor
	zString m_AggregateName2;		// Name of CAnalysis-derived class to monitor
	long	m_AggregateId1;			// Unique id of aggregate to monitor
	long	m_AggregateId2;			// Unique id of aggregate to monitor

	double	m_MaxApproach;			// Distance at which vesicles are apposite
	double	m_MaxDistalTouch;		// Distance at which vesicle outer leaflets "touch"
	long    m_MinPoreSize;			// Minimum size (grid points) of single-membrane pore
	long    m_MinFusionPoreSize;	// Minimum size (grid points) of fusion pore


	// Local data needed by the process

	// Pointers to the two vesicle aggregates to access their data, and the
	// events supported by the aggregates

	const IVesicleFusion<CVesicle>*  m_pVesicle1;
	const IVesicleFusion<CVesicle>*  m_pVesicle2;

	// Local data created by the process from its aggregates and events

	// Flags showing which events on the path to fusion have occurred

	bool	m_bVesicleApproach;		// Indicates vesicles are closer than m_MaxApproach
	bool	m_bVesicleDistalMix;	// Indicates outer leaflets have mixed
	bool	m_bVesicleDistalTouch;	// Indicates that vesicle outer leaflets are closer than m_MaxDistalTouch
	bool    m_bVesicleFusionPore;	// Indicates a fusion pore connecting both vesicles
	bool	m_bVesiclePore1;		// Indicates that vesicle 1 has a pore
	bool	m_bVesiclePore2;		// Indicates that vesicle 2 has a pore
	bool	m_bVesicleProximalMix;	// Indicates inner leaflets have mixed


	double	m_CMSeparation;			// Distance between vesicle centres of mass

};

#endif // !defined(AFX_PRVESICLEFUSION_H__3F913F08_48FB_404B_ABD9_0295BF60846E__INCLUDED_)
