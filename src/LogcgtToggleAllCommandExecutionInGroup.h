// LogcgtToggleAllCommandExecutionInGroup.h: interface for the CLogcgtToggleAllCommandExecutionInGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTTOGGLEALLCOMMANDEXECUTIONINGROUP_H__7C7B8287_C880_4065_AD6D_B860CBA2A503__INCLUDED_)
#define AFX_LOGCGTTOGGLEALLCOMMANDEXECUTIONINGROUP_H__7C7B8287_C880_4065_AD6D_B860CBA2A503__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgtToggleAllCommandExecutionInGroup : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtToggleAllCommandExecutionInGroup(long time, const zString groupName, bool bActive);

	virtual ~CLogcgtToggleAllCommandExecutionInGroup();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtToggleAllCommandExecutionInGroup& rMsg);

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

	CLogcgtToggleAllCommandExecutionInGroup(const CLogcgtToggleAllCommandExecutionInGroup& oldMessage);
	CLogcgtToggleAllCommandExecutionInGroup& operator=(const CLogcgtToggleAllCommandExecutionInGroup& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const bool     m_bActive;       // Execution status of group
};

#endif // !defined(AFX_LOGCGTTOGGLEALLCOMMANDEXECUTIONINGROUP_H__7C7B8287_C880_4065_AD6D_B860CBA2A503__INCLUDED_)
