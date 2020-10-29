// LogctRemoveActiveCommandTarget.h: interface for the CLogctRemoveActiveCommandTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTREMOVEACTIVECOMMANDTARGET_H__F463E21D_90B0_4294_BE59_0CDEE4A30C7E__INCLUDED_)
#define AFX_LOGCTREMOVEACTIVECOMMANDTARGET_H__F463E21D_90B0_4294_BE59_0CDEE4A30C7E__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctRemoveActiveCommandTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctRemoveActiveCommandTarget(long time, const zString target, long total);

	virtual ~CLogctRemoveActiveCommandTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctRemoveActiveCommandTarget& rMsg);

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

	CLogctRemoveActiveCommandTarget(const CLogctRemoveActiveCommandTarget& oldMessage);
	CLogctRemoveActiveCommandTarget& operator=(const CLogctRemoveActiveCommandTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString m_TargetLabel;	// Label of command target to render inactive
	const long    m_TargetTotal;	// Number of command targets still active
};

#endif // !defined(AFX_LOGCTREMOVEACTIVECOMMANDTARGET_H__F463E21D_90B0_4294_BE59_0CDEE4A30C7E__INCLUDED_)
