// LogcgtExecuteCommandGroupFailed.h: interface for the CLogcgtExecuteCommandGroupFailed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTEXECUTECOMMANDGROUPFAILED_H__F4FED4E1_FA1C_41BB_BB65_FCB1A1F3CA57__INCLUDED_)
#define AFX_LOGCGTEXECUTECOMMANDGROUPFAILED_H__F4FED4E1_FA1C_41BB_BB65_FCB1A1F3CA57__INCLUDED_


#include "LogWarningMessage.h"

class CLogcgtExecuteCommandGroupFailed : public CLogWarningMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtExecuteCommandGroupFailed(long time, const zString groupName);

	virtual ~CLogcgtExecuteCommandGroupFailed();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtExecuteCommandGroupFailed& rMsg);

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

	CLogcgtExecuteCommandGroupFailed(const CLogcgtExecuteCommandGroupFailed& oldMessage);
	CLogcgtExecuteCommandGroupFailed& operator=(const CLogcgtExecuteCommandGroupFailed& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group

};

#endif // !defined(AFX_LOGCGTEXECUTECOMMANDGROUPFAILED_H__F4FED4E1_FA1C_41BB_BB65_FCB1A1F3CA57__INCLUDED_)
