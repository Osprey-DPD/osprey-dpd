// evMotorATPase.h: interface for the evMotorATPase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVMOTORATPASE_H__1AC674A5_6F7A_4775_8801_9D77342DB487__INCLUDED_)
#define AFX_EVMOTORATPASE_H__1AC674A5_6F7A_4775_8801_9D77342DB487__INCLUDED_


// Forward declarations

class CMotor;


#include "xxEvent.h"

class evMotorATPase : public xxEvent  
{
	// ****************************************
	// Construction/Destruction
public:

	evMotorATPase();
	evMotorATPase(const evMotorATPase& oldEvent);

	virtual ~evMotorATPase();

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

	zString m_AggregateName;		// Name of CAnalysis-derived class to monitor
	long	m_AggregateId;			// Unique id of aggregate to monitor
	zString m_Motor;				// Polymer representing the motor protein
	zString	m_Fuel;					// Polymer representing fuel, i.e., ATP
	zString	m_Filament;				// Polymer representing the filament for motor to move on
	long    m_FuelPerStroke;		// Number of fuel polymers consumed per motor movement
	long	m_FuelBindingTime;		// Time during which fuel is bound to the motor
	long	m_FilamentBindingTime;	// Time during which the motor is bound to the filament
	double  m_FuelRange;			// Maximum distance to bind fuel polymer
	double  m_FilamentRange;		// Maximum distance to bind to filament
	double  m_BoundFuelRange;		// Separation of bound fuel from motor
	double  m_BoundFilamentRange;	// Separation of bound filament from motor
	double  m_BoundFuelSpringConstant;		// Hookean spring constant of bound fuel
	double  m_BoundFilamentSpringConstant;	// Hookean spring constant of bound filament
	double	m_DeltaHead;			// Change in preferred bond angle on binding fuel

	// Local data for event's analysis

	long m_MotorType;		// Polymer types for motor, fuel and filament polymers
	long m_FuelType;
	long m_FilamentType;

	CMotor* m_pMotor;						// Pointer to the event's associated aggregate
	const CAbstractBead*  m_pMotorHead;		// Pointer to the motor polymer's head bead
	const CNTCellVector* m_pCNTCells;		// Pointer to the CNT cells for bead coordinates

	long m_MotorTotal;						// Number of CMotor aggregates

	std::vector<long> m_vFuelBindingTimes;	// Timers for fuel binding for each motor
};

#endif // !defined(AFX_EVMOTORATPASE_H__1AC674A5_6F7A_4775_8801_9D77342DB487__INCLUDED_)
