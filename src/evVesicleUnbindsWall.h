// evVesicleUnbindsWall.h: interface for the evVesicleUnbindsWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVVESICLEUNBINDSWALL_H__6D3B8A7C_9C95_46DF_9A32_09767870E5D2__INCLUDED_)
#define AFX_EVVESICLEUNBINDSWALL_H__6D3B8A7C_9C95_46DF_9A32_09767870E5D2__INCLUDED_


// Forward declarations

class IEventVesicleUnbindsWall;
class CSimState;					// Needed for InternalValidateData() method


#include "xxEvent.h"

class evVesicleUnbindsWall : public xxEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	evVesicleUnbindsWall();
	evVesicleUnbindsWall(const evVesicleUnbindsWall& oldEvent);

	virtual ~evVesicleUnbindsWall();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the type of event

	static const zString m_Type;	// Identifier used in control data file
	
	// ****************************************
	// Public access functions
public:

	inline const zString GetAggregateName()		const {return m_AggregateName;}
	inline long			 GetAggregateId()		const {return m_AggregateId;}

	// ****************************************
	// PVFs inherited from xxEvent
public:

	zOutStream& put(zOutStream& os) const;
	zInStream&  get(zInStream& is);

	virtual bool Execute(long simTime, ISimEvent* const pISimEvent);

	virtual const zString GetEventType() const;

	virtual xxEvent* GetEvent() const;

	virtual bool ValidateData(const CInputData &riData);

	virtual bool InternalValidateData(const CSimState& rSimState, xxEvent* const pEvent);

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

	IEventVesicleUnbindsWall* m_pIEvent;		// Pointer to the IEventXXX event interface class

	zString m_AggregateName;			// Name of aggregate to monitor
	long	m_AggregateId;				// Unique id of aggregate
	long	m_WallXN;					// Normal to wall
	long	m_WallYN;
	long	m_WallZN;
	long	m_WallThickness;			// Wall thickness

	double	m_MaxCMSeparation;		// Max distance from vesicle CM to wall to be bound
	double	m_MaxEdgeSeparation;	// Max distance from vesicle membrane to wall to be bound

	// Local data

	double	m_CMDistance;			// Distance of vesicle CM from wall
	double  m_EdgeDistance;			// Distance of vesicle edge from wall

};

#endif // !defined(AFX_EVVESICLEUNBINDSWALL_H__6D3B8A7C_9C95_46DF_9A32_09767870E5D2__INCLUDED_)
