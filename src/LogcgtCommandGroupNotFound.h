// LogcgtCommandGroupNotFound.h: interface for the CLogcgtCommandGroupNotFound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTCOMMANDGROUPNOTFOUND_H__C8C8D31C_7FA6_4F14_B5DC_40376940A8E3__INCLUDED_)
#define AFX_LOGCGTCOMMANDGROUPNOTFOUND_H__C8C8D31C_7FA6_4F14_B5DC_40376940A8E3__INCLUDED_


#include "LogWarningMessage.h"

class CLogcgtCommandGroupNotFound : public CLogWarningMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtCommandGroupNotFound(long time, const zString groupName);

	virtual ~CLogcgtCommandGroupNotFound();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtCommandGroupNotFound& rMsg);

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

	CLogcgtCommandGroupNotFound(const CLogcgtCommandGroupNotFound& oldMessage);
	CLogcgtCommandGroupNotFound& operator=(const CLogcgtCommandGroupNotFound& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group

};

#endif // !defined(AFX_LOGCGTCOMMANDGROUPNOTFOUND_H__C8C8D31C_7FA6_4F14_B5DC_40376940A8E3__INCLUDED_)
