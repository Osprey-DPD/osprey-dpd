// LogctFreezeBeadsInTarget.h: interface for the CLogctFreezeBeadsInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTFREEZEBEADSINTARGET_H__07680CD8_9062_4B78_8959_CD30F0E6F83E__INCLUDED_)
#define AFX_LOGCTFREEZEBEADSINTARGET_H__07680CD8_9062_4B78_8959_CD30F0E6F83E__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctFreezeBeadsInTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctFreezeBeadsInTarget(long time, const zString targetlabel);

	virtual ~CLogctFreezeBeadsInTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctFreezeBeadsInTarget& rMsg);

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

	CLogctFreezeBeadsInTarget(const CLogctFreezeBeadsInTarget& oldMessage);
	CLogctFreezeBeadsInTarget& operator=(const CLogctFreezeBeadsInTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
};

#endif // !defined(AFX_LOGCTFREEZEBEADSINTARGET_H__07680CD8_9062_4B78_8959_CD30F0E6F83E__INCLUDED_)
