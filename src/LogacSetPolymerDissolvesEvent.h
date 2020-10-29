// LogacSetPolymerDissolvesEvent.h: interface for the CLogacSetPolymerDissolvesEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETPOLYMERDISSOLVESEVENT_H__E1EC5B75_B57B_4472_BD98_6E5FA56953C0__INCLUDED_)
#define AFX_LOGACSETPOLYMERDISSOLVESEVENT_H__E1EC5B75_B57B_4472_BD98_6E5FA56953C0__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetPolymerDissolvesEvent : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetPolymerDissolvesEvent(long time, const zString type, long duration,
		                       double spring, double length);

	virtual ~CLogacSetPolymerDissolvesEvent();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetPolymerDissolvesEvent& rMsg);

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

	CLogacSetPolymerDissolvesEvent(const CLogacSetPolymerDissolvesEvent& oldMessage);
	CLogacSetPolymerDissolvesEvent& operator=(const CLogacSetPolymerDissolvesEvent& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;			// Type of active cell network modified
	const long		m_Duration;			// Time steps allowed for polymer formation
	const double    m_SpringConstant;	// Final value of transient bond strength
	const double    m_Length;	        // Final value of transient bond unstretched length
};

#endif // !defined(AFX_LOGACSETPOLYMERDISSOLVESEVENT_H__E1EC5B75_B57B_4472_BD98_6E5FA56953C0__INCLUDED_)
