// LogcgtInactiveCommandGroup.h: interface for the CLogcgtInactiveCommandGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTINACTIVECOMMANDGROUP_H__8C5C7262_5A2B_426C_8FDA_80BAB0DC87E6__INCLUDED_)
#define AFX_LOGCGTINACTIVECOMMANDGROUP_H__8C5C7262_5A2B_426C_8FDA_80BAB0DC87E6__INCLUDED_


#include "LogWarningMessage.h"

class CLogcgtInactiveCommandGroup : public CLogWarningMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtInactiveCommandGroup(long time, const zString groupName);

	virtual ~CLogcgtInactiveCommandGroup();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtInactiveCommandGroup& rMsg);

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

	CLogcgtInactiveCommandGroup(const CLogcgtInactiveCommandGroup& oldMessage);
	CLogcgtInactiveCommandGroup& operator=(const CLogcgtInactiveCommandGroup& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group

};

#endif // !defined(AFX_LOGCGTINACTIVECOMMANDGROUP_H__8C5C7262_5A2B_426C_8FDA_80BAB0DC87E6__INCLUDED_)
