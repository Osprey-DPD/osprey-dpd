// LogacSetActivePolymerLimit.h: interface for the CLogacSetActivePolymerLimit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETACTIVEPOLYMERLIMIT_H__839D4B18_207D_4B8A_9E4F_8B303AAC6905__INCLUDED_)
#define AFX_LOGACSETACTIVEPOLYMERLIMIT_H__839D4B18_207D_4B8A_9E4F_8B303AAC6905__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetActivePolymerLimit : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetActivePolymerLimit(long time, const zString type, long max);

	virtual ~CLogacSetActivePolymerLimit();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetActivePolymerLimit& rMsg);

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

	CLogacSetActivePolymerLimit(const CLogacSetActivePolymerLimit& oldMessage);
	CLogacSetActivePolymerLimit& operator=(const CLogacSetActivePolymerLimit& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
	const long		m_MaxPolymers;	// Maximum number of polymers allowed
};

#endif // !defined(AFX_LOGACSETACTIVEPOLYMERLIMIT_H__839D4B18_207D_4B8A_9E4F_8B303AAC6905__INCLUDED_)
