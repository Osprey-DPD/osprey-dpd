// LogctTargetPolymerTypeDisplayId.h: interface for the CLogctTargetPolymerTypeDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTARGETPOLYMERTYPEDISPLAYID_H__462A23BB_44EC_49CC_A68E_A5A346819271__INCLUDED_)
#define AFX_LOGCTTARGETPOLYMERTYPEDISPLAYID_H__462A23BB_44EC_49CC_A68E_A5A346819271__INCLUDED_


#include "LogInfoMessage.h"

class CLogctTargetPolymerTypeDisplayId : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctTargetPolymerTypeDisplayId(long time, const zString cmdTarget, long polymerType, long displayId);

	virtual ~CLogctTargetPolymerTypeDisplayId();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTargetPolymerTypeDisplayId& rMsg);

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

	CLogctTargetPolymerTypeDisplayId(const CLogctTargetPolymerTypeDisplayId& oldMessage);
	CLogctTargetPolymerTypeDisplayId& operator=(const CLogctTargetPolymerTypeDisplayId& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	long            m_PolymerType;	// Numeric identifier for polymer type whose display id is modified
	long            m_DisplayId;	// Display id of all beads in the specified polymer type in the target
};

#endif // !defined(AFX_LOGCTTARGETPOLYMERTYPEDISPLAYID_H__462A23BB_44EC_49CC_A68E_A5A346819271__INCLUDED_)
