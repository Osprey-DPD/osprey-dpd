// LogcgtCommandInstantiationFailed.h: interface for the CLogcgtCommandInstantiationFailed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTCOMMANDINSTANTIATIONFAILED_H__FAEFF33F_083A_4E71_9DAA_8CEB41A8B47C__INCLUDED_)
#define AFX_LOGCGTCOMMANDINSTANTIATIONFAILED_H__FAEFF33F_083A_4E71_9DAA_8CEB41A8B47C__INCLUDED_


#include "LogWarningMessage.h"

class CLogcgtCommandInstantiationFailed : public CLogWarningMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtCommandInstantiationFailed(long time, const zString groupName, const zString cmdName,  
                                      const zString argName, const zString argType, const zString argSource);

	virtual ~CLogcgtCommandInstantiationFailed();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtCommandInstantiationFailed& rMsg);

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

	CLogcgtCommandInstantiationFailed(const CLogcgtCommandInstantiationFailed& oldMessage);
	CLogcgtCommandInstantiationFailed& operator=(const CLogcgtCommandInstantiationFailed& rhs);


	// ****************************************
	// Data members
private:

    const zString m_GroupName;        // Name of command group
    const zString m_CommandName;      // Name of command 
    const zString m_ArgName;          // Name of failed argument
    const zString m_ArgType;          // Expected type of failed argument
    const zString m_ArgSource;        // Type of automatic generation of failed argument

};

#endif // !defined(AFX_LOGCGTCOMMANDINSTANTIATIONFAILED_H__FAEFF33F_083A_4E71_9DAA_8CEB41A8B47C__INCLUDED_)
