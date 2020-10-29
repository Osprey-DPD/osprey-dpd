// LogcgtCommandPackingFailed.h: interface for the CLogcgtCommandPackingFailed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCGTCOMMANDPACKINGFAILED_H__B50F7FE7_0991_41F5_ABFE_071F643EDBDB__INCLUDED_)
#define AFX_LOGCGTCOMMANDPACKINGFAILED_H__B50F7FE7_0991_41F5_ABFE_071F643EDBDB__INCLUDED_


#include "LogWarningMessage.h"

class CLogcgtCommandPackingFailed : public CLogWarningMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogcgtCommandPackingFailed(long time, const zString groupName, const zString cmdName, StringSequence vArgNames);

	virtual ~CLogcgtCommandPackingFailed();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogcgtCommandPackingFailed& rMsg);

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

	CLogcgtCommandPackingFailed(const CLogcgtCommandPackingFailed& oldMessage);
	CLogcgtCommandPackingFailed& operator=(const CLogcgtCommandPackingFailed& rhs);


	// ****************************************
	// Data members
private:

    const zString m_GroupName;        // Name of command group
    const zString m_CommandName;      // Name of command 
    const StringSequence m_ArgNames;  // Names of command's arguments
};

#endif // !defined(AFX_LOGCGTCOMMANDPACKINGFAILED_H__B50F7FE7_0991_41F5_ABFE_071F643EDBDB__INCLUDED_)
