// LogacSetEventExecutionPeriod.h: interface for the CLogacSetEventExecutionPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETEVENTEXECUTIONPERIOD_H__CA6727C8_3990_43CA_AA7E_220EA76D3FC8__INCLUDED_)
#define AFX_LOGACSETEVENTEXECUTIONPERIOD_H__CA6727C8_3990_43CA_AA7E_220EA76D3FC8__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetEventExecutionPeriod : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetEventExecutionPeriod(long time, const zString acnType, const zString eventType, const long period);

	virtual ~CLogacSetEventExecutionPeriod();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetEventExecutionPeriod& rMsg);

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

	CLogacSetEventExecutionPeriod(const CLogacSetEventExecutionPeriod& oldMessage);
	CLogacSetEventExecutionPeriod& operator=(const CLogacSetEventExecutionPeriod& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	    // Type of active cell network modified
	const zString	m_EventType;	// Type of event modified
	const long	    m_Period;	    // Period with which event's execution is checked
};


#endif // !defined(AFX_LOGACSETEVENTEXECUTIONPERIOD_H__CA6727C8_3990_43CA_AA7E_220EA76D3FC8__INCLUDED_)
