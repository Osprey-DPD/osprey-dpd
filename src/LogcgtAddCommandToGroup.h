// LogcgtAddCommandToGroup.h: interface for the CLogcgtAddCommandToGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTADDCOMMANDTOGROUP_H__BD72353E_EFB8_4FDD_97D3_80407517B23A__INCLUDED_)
#define AFX_LOGCGTADDCOMMANDTOGROUP_H__BD72353E_EFB8_4FDD_97D3_80407517B23A__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgtAddCommandToGroup : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtAddCommandToGroup(long time, const zString groupName, const zString cmdName,
                             StringSequence argNames);

	virtual ~CLogcgtAddCommandToGroup();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtAddCommandToGroup& rMsg);

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

	CLogcgtAddCommandToGroup(const CLogcgtAddCommandToGroup& oldMessage);
	CLogcgtAddCommandToGroup& operator=(const CLogcgtAddCommandToGroup& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;     // Name of command group
    const zString  m_CommandName;   // Name of command 
    StringSequence m_ArgNames;      // Placeholder names for command's arguments

};

#endif // !defined(AFX_LOGCGTADDCOMMANDTOGROUP_H__BD72353E_EFB8_4FDD_97D3_80407517B23A__INCLUDED_)
