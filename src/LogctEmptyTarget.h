// LogctEmptyTarget.h: interface for the CLogctEmptyTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTEMPTYTARGET_H__DEEEBC97_FA18_41F7_8163_5E3C85396B47__INCLUDED_)
#define AFX_LOGCTEMPTYTARGET_H__DEEEBC97_FA18_41F7_8163_5E3C85396B47__INCLUDED_


#include "LogWarningMessage.h"

class CLogctEmptyTarget : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctEmptyTarget(long time, const zString label);

	virtual ~CLogctEmptyTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctEmptyTarget& rMsg);

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

	CLogctEmptyTarget(const CLogctEmptyTarget& oldMessage);
	CLogctEmptyTarget& operator=(const CLogctEmptyTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString m_TargetLabel;	// Command target's name
};

#endif // !defined(AFX_LOGCTEMPTYTARGET_H__DEEEBC97_FA18_41F7_8163_5E3C85396B47__INCLUDED_)
