// LogpcToggleProcessModificationStatus.h: interface for the CLogpcToggleProcessModificationStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCTOGGLEPROCESSMODIFICATIONSTATUS_H__32957C81_A48E_4B90_8159_21AE0469625B__INCLUDED_)
#define AFX_LOGPCTOGGLEPROCESSMODIFICATIONSTATUS_H__32957C81_A48E_4B90_8159_21AE0469625B__INCLUDED_


#include "LogInfoMessage.h"

class CLogpcToggleProcessModificationStatus : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcToggleProcessModificationStatus(long time, const zString pid, bool bModify);

	virtual ~CLogpcToggleProcessModificationStatus();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcToggleProcessModificationStatus& rMsg);

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

	CLogpcToggleProcessModificationStatus(const CLogpcToggleProcessModificationStatus& oldMessage);
	CLogpcToggleProcessModificationStatus& operator=(const CLogpcToggleProcessModificationStatus& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;	// Target process' id
	bool m_bModifiable;		// Flag showing if the process is modifiable after command execution
};

#endif // !defined(AFX_LOGPCTOGGLEPROCESSMODIFICATIONSTATUS_H__32957C81_A48E_4B90_8159_21AE0469625B__INCLUDED_)
