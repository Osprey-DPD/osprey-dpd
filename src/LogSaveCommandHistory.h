// LogSaveCommandHistory.h: interface for the CLogSaveCommandHistory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVECOMMANDHISTORY_H__1D9175E5_C20E_4818_AEF5_63588D63E0EE__INCLUDED_)
#define AFX_LOGSAVECOMMANDHISTORY_H__1D9175E5_C20E_4818_AEF5_63588D63E0EE__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveCommandHistory : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveCommandHistory(long time, const zString name);

	virtual ~CLogSaveCommandHistory();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveCommandHistory& rMsg);

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

	CLogSaveCommandHistory(const CLogSaveCommandHistory& oldMessage);
	CLogSaveCommandHistory& operator=(const CLogSaveCommandHistory& rhs);


	// ****************************************
	// Data members
private:

	zString	 m_Name;	// Command document's name including ".xml" suffix
};

#endif // !defined(AFX_LOGSAVECOMMANDHISTORY_H__1D9175E5_C20E_4818_AEF5_63588D63E0EE__INCLUDED_)
