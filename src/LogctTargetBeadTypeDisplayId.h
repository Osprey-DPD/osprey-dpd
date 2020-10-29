// LogctTargetBeadTypeDisplayId.h: interface for the CLogctTargetBeadTypeDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTTARGETBEADTYPEDISPLAYID_H__08A06442_0471_4D9B_BE31_98F072388151__INCLUDED_)
#define AFX_LOGCTTARGETBEADTYPEDISPLAYID_H__08A06442_0471_4D9B_BE31_98F072388151__INCLUDED_


#include "LogInfoMessage.h"

class CLogctTargetBeadTypeDisplayId : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctTargetBeadTypeDisplayId(long time, const zString cmdTarget, long beadType, long displayId);

	virtual ~CLogctTargetBeadTypeDisplayId();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctTargetBeadTypeDisplayId& rMsg);

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

	CLogctTargetBeadTypeDisplayId(const CLogctTargetBeadTypeDisplayId& oldMessage);
	CLogctTargetBeadTypeDisplayId& operator=(const CLogctTargetBeadTypeDisplayId& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	long            m_BeadType;	    // Numeric identifier for bead type whose display id is modified
	long            m_DisplayId;	// Display id of all beads in the target
};

#endif // !defined(AFX_LOGCTTARGETBEADTYPEDISPLAYID_H__08A06442_0471_4D9B_BE31_98F072388151__INCLUDED_)
