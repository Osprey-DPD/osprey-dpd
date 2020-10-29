// LogctAntiTargetDisplay.h: interface for the CLogctAntiTargetDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTANTITARGETDISPLAY_H__77D29D83_53F1_4A78_BD9E_8DC0C22F7625__INCLUDED_)
#define AFX_LOGCTANTITARGETDISPLAY_H__77D29D83_53F1_4A78_BD9E_8DC0C22F7625__INCLUDED_


#include "LogInfoMessage.h"

class CLogctAntiTargetDisplay : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctAntiTargetDisplay(long time, const zString cmdTarget, const bool bDisplay);

	virtual ~CLogctAntiTargetDisplay();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctAntiTargetDisplay& rMsg);

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

	CLogctAntiTargetDisplay(const CLogctAntiTargetDisplay& oldMessage);
	CLogctAntiTargetDisplay& operator=(const CLogctAntiTargetDisplay& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const bool      m_bAntiDisplay;	// Visibility status of non-target beads
};
#endif // !defined(AFX_LOGCTANTITARGETDISPLAY_H__77D29D83_53F1_4A78_BD9E_8DC0C22F7625__INCLUDED_)
