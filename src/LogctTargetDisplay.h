// LogctTargetDisplay.h: interface for the CLogctTargetDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTARGETDISPLAY_H__73D111C0_0F70_42E7_916B_B60B0B675568__INCLUDED_)
#define AFX_LOGCTTARGETDISPLAY_H__73D111C0_0F70_42E7_916B_B60B0B675568__INCLUDED_


#include "LogInfoMessage.h"

class CLogctTargetDisplay : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctTargetDisplay(long time, const zString cmdTarget, const bool bDisplay);

	virtual ~CLogctTargetDisplay();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTargetDisplay& rMsg);

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

	CLogctTargetDisplay(const CLogctTargetDisplay& oldMessage);
	CLogctTargetDisplay& operator=(const CLogctTargetDisplay& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const bool      m_bDisplay;		// Visibility status of target
};

#endif // !defined(AFX_LOGCTTARGETDISPLAY_H__73D111C0_0F70_42E7_916B_B60B0B675568__INCLUDED_)
