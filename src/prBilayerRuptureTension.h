// prBilayerRuptureTension.h: interface for the prBilayerRuptureTension class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRBILAYERRUPTURETENSION_H__839C5B08_20A3_4748_9C27_26BA3F2874D0__INCLUDED_)
#define AFX_PRBILAYERRUPTURETENSION_H__839C5B08_20A3_4748_9C27_26BA3F2874D0__INCLUDED_


// Include files and forward declarations for template adaptor classes

class CBilayer;
class evLamellaPore;
class evLamellaPoreClosed;

#include "IBilayerRupture.h"


#include "xxProcess.h"

class prBilayerRuptureTension : public xxProcess 
{
	// ****************************************
	// Construction/Destruction
public:

	prBilayerRuptureTension();
	virtual ~prBilayerRuptureTension();

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
	
	// PVFs inherited from xxProcess

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	// Function to allow the process to monitor aggregates and check for events

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess() const;

	// Non-static member function to return the (static) process name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

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

	void SetAggregate(const CAnalysis* pAggregate);

	void SetEvent(xxEvent* pEvent);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions

	// ****************************************
	// Data members

private:							// Data specific to the process

	zString m_AggregateName;		// Name of CAnalysis-derived class to monitor
	long	m_AggregateId;			// Unique id of aggregate to monitor
	zString m_EventName;			// Name of xxEvent-derived class to monitor
	long	m_EventId;				// Unique id of event to monitor


	// Local data needed by the process

	// Pointers to wrapper class for the  CBilayer aggregate to give
	// access to its data; and interfaces for the events supported by the aggregates

	const IBilayerRupture<CBilayer>*  m_pBilayer;

	evLamellaPore*			m_pLamellaPore;				// Event to check
	evLamellaPoreClosed*	m_pLamellaPoreClosed;		// Event to check

	// Local data created by the process from its aggregates and events

	bool	m_bPoreOpened;		// Flag to indicate if the pore is open
	long	m_FirstOpenedTime;	// Simulation time at which pore first opens

	long	m_FlickerNo;		// No of times pore has opened

	double	m_PoreSize;			// No of grid points within pore
	double	m_PoreRg2;			// Rg**2 of pore
	double	m_PoreArea;			// Area of pore
	double	m_SurfaceTension;	// Bilayer tension obtained from aggregate


};

#endif // !defined(AFX_PRBILAYERRUPTURETENSION_H__839C5B08_20A3_4748_9C27_26BA3F2874D0__INCLUDED_)
