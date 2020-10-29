// LogcgtToggleCommandExecutionInGroup.h: interface for the CLogcgtToggleCommandExecutionInGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTTOGGLECOMMANDEXECUTIONINGROUP_H__84DAF572_8B05_4199_B2DD_FA0ED2D291FA__INCLUDED_)
#define AFX_LOGCGTTOGGLECOMMANDEXECUTIONINGROUP_H__84DAF572_8B05_4199_B2DD_FA0ED2D291FA__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgtToggleCommandExecutionInGroup : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtToggleCommandExecutionInGroup(long time, const zString groupName, const zString cmdName, 
                                         long cmdIndex, bool bActive);

	virtual ~CLogcgtToggleCommandExecutionInGroup();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtToggleCommandExecutionInGroup& rMsg);

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

	CLogcgtToggleCommandExecutionInGroup(const CLogcgtToggleCommandExecutionInGroup& oldMessage);
	CLogcgtToggleCommandExecutionInGroup& operator=(const CLogcgtToggleCommandExecutionInGroup& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_CommandName;   // Name of command 
    const long     m_CommandIndex;  // Index of command 
    const bool     m_bActive;       // Execution status of command
};

#endif // !defined(AFX_LOGCGTTOGGLECOMMANDEXECUTIONINGROUP_H__84DAF572_8B05_4199_B2DD_FA0ED2D291FA__INCLUDED_)
