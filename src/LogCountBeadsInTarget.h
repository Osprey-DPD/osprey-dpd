// LogCountBeadsInTarget.h: interface for the CLogCountBeadsInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCOUNTBEADSINTARGET_H__4ACCF8F7_94FC_4A20_A5CD_E7B5CC0336F8__INCLUDED_)
#define AFX_LOGCOUNTBEADSINTARGET_H__4ACCF8F7_94FC_4A20_A5CD_E7B5CC0336F8__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogCountBeadsInTarget : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCountBeadsInTarget(long time, const zString cmdTarget, 
					   const zString oldName, long oldType, long total);

	virtual ~CLogCountBeadsInTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCountBeadsInTarget& rMsg);

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

	CLogCountBeadsInTarget(const CLogCountBeadsInTarget& oldMessage);
	CLogCountBeadsInTarget& operator=(const CLogCountBeadsInTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const zString	m_OldName;		// Bead type's original name
	const long      m_OldType;		// Original bead type
	const long      m_BeadTotal;	// Number of beads in target
};

#endif // !defined(AFX_LOGCOUNTBEADSINTARGET_H__4ACCF8F7_94FC_4A20_A5CD_E7B5CC0336F8__INCLUDED_)
