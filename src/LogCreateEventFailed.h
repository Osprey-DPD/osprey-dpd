// LogCreateEventFailed.h: interface for the CLogCreateEventFailed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCREATEEVENTFAILED_H__596DA6A4_7C54_4E3B_AEE3_EE71AD6EC8DE__INCLUDED_)
#define AFX_LOGCREATEEVENTFAILED_H__596DA6A4_7C54_4E3B_AEE3_EE71AD6EC8DE__INCLUDED_


// Forward declarations

class xxEvent;

#include "LogWarningMessage.h"

class CLogCreateEventFailed : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCreateEventFailed(long time, const xxEvent* const pEvent);

	virtual ~CLogCreateEventFailed();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCreateEventFailed& rMsg);

	// ****************************************
	// Public access functions
public:

	// Function to get access to the command object. It is needed when 
	// serialising the event's parameters.

	inline const xxEvent* GetEvent() const {return m_pEvent;}

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

	CLogCreateEventFailed(const CLogCreateEventFailed& oldMessage);
	CLogCreateEventFailed& operator=(const CLogCreateEventFailed& rhs);

	// ****************************************
	// Data members
private:

	// We store the (failed) event object to write out its parameters using 
	// its put() function.

private:
	const xxEvent* const m_pEvent;

};

#endif // !defined(AFX_LOGCREATEEVENTFAILED_H__596DA6A4_7C54_4E3B_AEE3_EE71AD6EC8DE__INCLUDED_)

