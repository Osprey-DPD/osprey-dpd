// LogctUnFreezeBeadsInTarget.h: interface for the CLogctUnFreezeBeadsInTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTUNFREEZEBEADSINTARGET_H__4714D515_12B4_405C_B634_B9FA154772B5__INCLUDED_)
#define AFX_LOGCTUNFREEZEBEADSINTARGET_H__4714D515_12B4_405C_B634_B9FA154772B5__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctUnFreezeBeadsInTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctUnFreezeBeadsInTarget(long time, const zString targetlabel);

	virtual ~CLogctUnFreezeBeadsInTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctUnFreezeBeadsInTarget& rMsg);

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

	CLogctUnFreezeBeadsInTarget(const CLogctUnFreezeBeadsInTarget& oldMessage);
	CLogctUnFreezeBeadsInTarget& operator=(const CLogctUnFreezeBeadsInTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_TargetLabel;	// Command target label
};

#endif // !defined(AFX_LOGCTUNFREEZEBEADSINTARGET_H__4714D515_12B4_405C_B634_B9FA154772B5__INCLUDED_)
