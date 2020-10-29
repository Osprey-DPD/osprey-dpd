// prVesicleShear.h: interface for the prVesicleShear class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRVESICLESHEAR_H__1D288094_180F_4CB3_890A_0D28372152CC__INCLUDED_)
#define AFX_PRVESICLESHEAR_H__1D288094_180F_4CB3_890A_0D28372152CC__INCLUDED_


// Include file and forward declarations for template adaptor class

#include "IVesicleShear.h"

class CVesicle;
class evVesicleBindsWall;
class evVesicleUnbindsWall;



#include "xxProcess.h"

class prVesicleShear : public xxProcess
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	prVesicleShear();
	virtual ~prVesicleShear();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the process name

private:

	static const zString m_Type;	// Identifier used in control data file

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	virtual xxProcess* GetProcess() const;

	virtual bool ValidateData(const CInputData &riData);


	virtual const zString GetProcessType() const;	// Return the process name

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	// Functions to allow the xxProcess to store pointers to its 
	// required CAnalysis and xxEvent-derived class(es)
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
private:

	// ****************************************
	// Data members
private:								// Data specific to the process

	zString m_AggregateName;			// Name of CAnalysis-derived class to monitor
	long	m_AggregateId;				// Unique id of aggregate to monitor
	zString m_EventName;				// Name of xxEvent-derived class to monitor
	long	m_EventId;					// Unique id of event to monitor
	long	m_WallXN;					// Normal to wall
	long	m_WallYN;
	long	m_WallZN;
	long	m_WallThickness;			// Wall thickness
	double	m_MaxCMSeparation;			// Max distance from vesicle CM to wall to be bound
	double	m_MaxEdgeSeparation;		// Max distance from vesicle membrane to wall to be bound


	// Local data needed by the process

	const IVesicleShear<CVesicle>*  m_pVesicle;			// Aggregate to monitor

	evVesicleBindsWall*		m_pVesicleBinds;		// Event to check
	evVesicleUnbindsWall*	m_pVesicleUnbinds;		// Event to check

	// Local data created by the process from its aggregates and events

	bool	m_bVesicleWallMaxDistance;	// Indicates vesicle CM is closer to wall than m_MaxCMSeparation
	bool	m_bVesicleBound;			// Indicates vesicle is in contact with wall
	long	m_FirstBoundTime;			// Simulation time at which vesicle first binds
	double	m_VesicleCMWallDist;		// Distance of vesicle CM from wall
	double	m_VesicleEdgeWallDist;		// Distance of vesicle edge from wall

};

#endif // !defined(AFX_PRVESICLESHEAR_H__1D288094_180F_4CB3_890A_0D28372152CC__INCLUDED_)
