// LogacSetBondUnbindsFromPolymerTailEvent.h: interface for the CLogacSetBondUnbindsFromPolymerTailEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETBONDUNBINDSFROMPOLYMERTAILEVENT_H__911111A2_CA30_4DC4_85CC_FBB81AF72F11__INCLUDED_)
#define AFX_LOGACSETBONDUNBINDSFROMPOLYMERTAILEVENT_H__911111A2_CA30_4DC4_85CC_FBB81AF72F11__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetBondUnbindsFromPolymerTailEvent : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetBondUnbindsFromPolymerTailEvent(long time, const zString type, long duration,
		                       double spring, double length);

	virtual ~CLogacSetBondUnbindsFromPolymerTailEvent();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetBondUnbindsFromPolymerTailEvent& rMsg);

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

	CLogacSetBondUnbindsFromPolymerTailEvent(const CLogacSetBondUnbindsFromPolymerTailEvent& oldMessage);
	CLogacSetBondUnbindsFromPolymerTailEvent& operator=(const CLogacSetBondUnbindsFromPolymerTailEvent& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;			// Type of active cell network modified
	const long		m_Duration;			// Time steps allowed for polymer formation
	const double    m_SpringConstant;	// Final value of transient bond strength
	const double    m_Length;	        // Final value of transient bond unstretched length
};

#endif // !defined(AFX_LOGACSETBONDUNBINDSFROMPOLYMERTAILEVENT_H__911111A2_CA30_4DC4_85CC_FBB81AF72F11__INCLUDED_)
