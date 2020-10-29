// LogctTargetInvisible.h: interface for the CLogctTargetInvisible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTARGETINVISIBLE_H__34E9CC4F_5955_418A_8463_671DC859256E__INCLUDED_)
#define AFX_LOGCTTARGETINVISIBLE_H__34E9CC4F_5955_418A_8463_671DC859256E__INCLUDED_


#include "LogInfoMessage.h"

class CLogctTargetInvisible : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctTargetInvisible(long time, const zString cmdTarget);

	virtual ~CLogctTargetInvisible();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTargetInvisible& rMsg);

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

	CLogctTargetInvisible(const CLogctTargetInvisible& oldMessage);
	CLogctTargetInvisible& operator=(const CLogctTargetInvisible& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
};

#endif // !defined(AFX_LOGCTTARGETINVISIBLE_H__34E9CC4F_5955_418A_8463_671DC859256E__INCLUDED_)
