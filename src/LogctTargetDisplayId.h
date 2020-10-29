// LogctTargetDisplayId.h: interface for the CLogctTargetDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTARGETDISPLAYID_H__15FC83F3_6027_4597_9B80_3EAAAD3B71CC__INCLUDED_)
#define AFX_LOGCTTARGETDISPLAYID_H__15FC83F3_6027_4597_9B80_3EAAAD3B71CC__INCLUDED_


#include "LogInfoMessage.h"

class CLogctTargetDisplayId : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctTargetDisplayId(long time, const zString cmdTarget, long displayId);

	virtual ~CLogctTargetDisplayId();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTargetDisplayId& rMsg);

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

	CLogctTargetDisplayId(const CLogctTargetDisplayId& oldMessage);
	CLogctTargetDisplayId& operator=(const CLogctTargetDisplayId& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	long            m_DisplayId;	// Display id of all beads in the target
};

#endif // !defined(AFX_LOGCTTARGETDISPLAYID_H__15FC83F3_6027_4597_9B80_3EAAAD3B71CC__INCLUDED_)
