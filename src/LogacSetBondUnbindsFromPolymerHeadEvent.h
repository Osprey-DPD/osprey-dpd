// LogacSetBondUnbindsFromPolymerHeadEvent.h: interface for the CLogacSetBondUnbindsFromPolymerHeadEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETBONDUNBINDSFROMPOLYMERHEADEVENT_H__BBA85E0A_DE36_436F_B167_A7BB29B18819__INCLUDED_)
#define AFX_LOGACSETBONDUNBINDSFROMPOLYMERHEADEVENT_H__BBA85E0A_DE36_436F_B167_A7BB29B18819__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetBondUnbindsFromPolymerHeadEvent : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetBondUnbindsFromPolymerHeadEvent(long time, const zString type, long duration,
		                       double spring, double length);

	virtual ~CLogacSetBondUnbindsFromPolymerHeadEvent();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetBondUnbindsFromPolymerHeadEvent& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogacSetBondUnbindsFromPolymerHeadEvent(const CLogacSetBondUnbindsFromPolymerHeadEvent& oldMessage);
	CLogacSetBondUnbindsFromPolymerHeadEvent& operator=(const CLogacSetBondUnbindsFromPolymerHeadEvent& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;			// Type of active cell network modified
	const long		m_Duration;			// Time steps allowed for polymer formation
	const double    m_SpringConstant;	// Final value of transient bond strength
	const double    m_Length;	        // Final value of transient bond unstretched length
};

#endif // !defined(AFX_LOGACSETBONDUNBINDSFROMPOLYMERHEADEVENT_H__BBA85E0A_DE36_436F_B167_A7BB29B18819__INCLUDED_)
