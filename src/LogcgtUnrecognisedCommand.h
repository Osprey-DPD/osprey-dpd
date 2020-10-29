// LogcgtUnrecognisedCommand.h: interface for the CLogcgtUnrecognisedCommand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTUNRECOGNISEDCOMMAND_H__CBF98CE5_1530_4580_9839_36A1A79D062B__INCLUDED_)
#define AFX_LOGCGTUNRECOGNISEDCOMMAND_H__CBF98CE5_1530_4580_9839_36A1A79D062B__INCLUDED_


#include "LogWarningMessage.h"

class CLogcgtUnrecognisedCommand : public CLogWarningMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtUnrecognisedCommand(long time, const zString groupName, const zString cmdName);

	virtual ~CLogcgtUnrecognisedCommand();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtUnrecognisedCommand& rMsg);

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

	CLogcgtUnrecognisedCommand(const CLogcgtUnrecognisedCommand& oldMessage);
	CLogcgtUnrecognisedCommand& operator=(const CLogcgtUnrecognisedCommand& rhs);


	// ****************************************
	// Data members
private:

    const zString m_GroupName;        // Name of command group
    const zString m_CommandName;      // Name of command 
};

#endif // !defined(AFX_LOGCGTUNRECOGNISEDCOMMAND_H__CBF98CE5_1530_4580_9839_36A1A79D062B__INCLUDED_)
