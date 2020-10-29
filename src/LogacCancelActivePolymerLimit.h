// LogacCancelActivePolymerLimit.h: interface for the CLogacCancelActivePolymerLimit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACCANCELACTIVEPOLYMERLIMIT_H__E5FD9435_8A32_4513_A545_1C170F576474__INCLUDED_)
#define AFX_LOGACCANCELACTIVEPOLYMERLIMIT_H__E5FD9435_8A32_4513_A545_1C170F576474__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacCancelActivePolymerLimit : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacCancelActivePolymerLimit(long time, const zString type);

	virtual ~CLogacCancelActivePolymerLimit();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacCancelActivePolymerLimit& rMsg);

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

	CLogacCancelActivePolymerLimit(const CLogacCancelActivePolymerLimit& oldMessage);
	CLogacCancelActivePolymerLimit& operator=(const CLogacCancelActivePolymerLimit& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACCANCELACTIVEPOLYMERLIMIT_H__E5FD9435_8A32_4513_A545_1C170F576474__INCLUDED_)
