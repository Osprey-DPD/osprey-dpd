// LogcgcSetArgumentToArgument.h: interface for the CLogcgcSetArgumentToArgument class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGCSETARGUMENTTOARGUMENT_H__76409F5D_1BF1_420E_9AAD_2C8B94F7C34E__INCLUDED_)
#define AFX_LOGCGCSETARGUMENTTOARGUMENT_H__76409F5D_1BF1_420E_9AAD_2C8B94F7C34E__INCLUDED_


#include "LogInfoMessage.h"

class CLogcgcSetArgumentToArgument : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgcSetArgumentToArgument(long time, const zString groupName, 
                                 const zString destCmdName,   long destCmdNo,   const zString destArgName,
                                 const zString sourceCmdName, long sourceCmdNo, const zString sourceArgName);

	virtual ~CLogcgcSetArgumentToArgument();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgcSetArgumentToArgument& rMsg);

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

	CLogcgcSetArgumentToArgument(const CLogcgcSetArgumentToArgument& oldMessage);
	CLogcgcSetArgumentToArgument& operator=(const CLogcgcSetArgumentToArgument& rhs);


	// ****************************************
	// Data members
private:

    const zString  m_GroupName;         // Name of command group
    const zString  m_DestCommandName;   // Name of destination command being modified
    const zString  m_DestArgName;       // Placeholder name of argument being set
    const zString  m_SourceCommandName; // Name of source command 
    const zString  m_SourceArgName;     // Placeholder name of argument being copied

    const long     m_DestCommandNo;     // Index of destination command in group
    const long     m_SourceCommandNo;   // Index of source command in group

};

#endif // !defined(AFX_LOGCGCSETARGUMENTTOARGUMENT_H__76409F5D_1BF1_420E_9AAD_2C8B94F7C34E__INCLUDED_)
