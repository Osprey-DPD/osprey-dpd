// prBilayerRaft.h: interface for the prBilayerRaft class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRBILAYERRAFT_H__7B2EF14F_1181_4DAB_AA60_AC06DA6F0C09__INCLUDED_)
#define AFX_PRBILAYERRAFT_H__7B2EF14F_1181_4DAB_AA60_AC06DA6F0C09__INCLUDED_


// Include files and forward declarations for template adaptor classes

class CCompositeBilayer;
class evLamellaMonolayerDomain;
class evLamellaMonolayerPhaseSeparation;
class evLamellaBilayerDomain;
class evLamellaBilayerPhaseSeparation;



#include "xxProcess.h"
#include "evLamellaMonolayerDomain.h"	// Added by ClassView

class prBilayerRaft : public xxProcess 
{
	// ****************************************
	// Construction/Destruction
public:

	prBilayerRaft();
	virtual ~prBilayerRaft();

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

	zString m_AggregateName;		// Name of CAnalysis-derived class to monitor (CCompositeBilayer)
	long	m_AggregateId;			// Unique id of aggregate to monitor
	zString m_EventName;			// Name of xxEvent-derived class to monitor (evLamellaMonolayerDomain)
	long	m_EventId;				// Unique id of event to monitor
	zString m_ProbeName;

	// Local data needed by the process

	const CCompositeBilayer*			m_pBilayer;		// Aggregate to monitor 

	evLamellaMonolayerDomain*			m_pMonolayerDomain;		// Events to check
	evLamellaMonolayerPhaseSeparation*	m_pMonolayerPhaseSep;
	evLamellaBilayerDomain*				m_pBilayerDomain;
	evLamellaBilayerPhaseSeparation*	m_pBilayerPhaseSep;

	// Local data created by the process from its aggregates and events

	long m_ProbeType;				// Polymer type for probe
	long m_ProbeHeadBeadType;		// Bead type for probe head
	bool m_bDomains;				// Flag showing if domains have formed
	bool m_bPhaseSeparation;		// Flag showing if lipids have phase separated




};

#endif // !defined(AFX_PRBILAYERRAFT_H__7B2EF14F_1181_4DAB_AA60_AC06DA6F0C09__INCLUDED_)
