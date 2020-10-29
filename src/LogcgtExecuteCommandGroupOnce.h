// LogcgtExecuteCommandGroupOnce.h: interface for the CLogcgtExecuteCommandGroupOnce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTEXECUTECOMMANDGROUPONCE_H__2199C580_3B78_4A0C_A2B4_B16D9B256774__INCLUDED_)
#define AFX_LOGCGTEXECUTECOMMANDGROUPONCE_H__2199C580_3B78_4A0C_A2B4_B16D9B256774__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgtExecuteCommandGroupOnce : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtExecuteCommandGroupOnce(long time, const zString groupName);

	virtual ~CLogcgtExecuteCommandGroupOnce();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtExecuteCommandGroupOnce& rMsg);

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

	CLogcgtExecuteCommandGroupOnce(const CLogcgtExecuteCommandGroupOnce& oldMessage);
	CLogcgtExecuteCommandGroupOnce& operator=(const CLogcgtExecuteCommandGroupOnce& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group

};

#endif // !defined(AFX_LOGCGTEXECUTECOMMANDGROUPONCE_H__2199C580_3B78_4A0C_A2B4_B16D9B256774__INCLUDED_)
