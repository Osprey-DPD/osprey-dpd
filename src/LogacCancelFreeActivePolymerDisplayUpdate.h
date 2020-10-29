// LogacCancelFreeActivePolymerDisplayUpdate.h: interface for the CLogacCancelFreeActivePolymerDisplayUpdate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACCANCELFREEACTIVEPOLYMERDISPLAYUPDATE_H__FC38A783_480E_4B46_9085_F14D82E4E1A1__INCLUDED_)
#define AFX_LOGACCANCELFREEACTIVEPOLYMERDISPLAYUPDATE_H__FC38A783_480E_4B46_9085_F14D82E4E1A1__INCLUDED_


#include "LogInfoMessage.h"

class CLogacCancelFreeActivePolymerDisplayUpdate : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacCancelFreeActivePolymerDisplayUpdate(long time, const zString type);

	virtual ~CLogacCancelFreeActivePolymerDisplayUpdate();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacCancelFreeActivePolymerDisplayUpdate& rMsg);

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

	CLogacCancelFreeActivePolymerDisplayUpdate(const CLogacCancelFreeActivePolymerDisplayUpdate& oldMessage);
	CLogacCancelFreeActivePolymerDisplayUpdate& operator=(const CLogacCancelFreeActivePolymerDisplayUpdate& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACCANCELFREEACTIVEPOLYMERDISPLAYUPDATE_H__FC38A783_480E_4B46_9085_F14D82E4E1A1__INCLUDED_)
