// LogSaveCommandHistoryToCurrentTime.h: interface for the CLogSaveCommandHistoryToCurrentTimeToCurrentTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVECOMMANDHISTORYTOCURRENTTIME_H__89F9ACB5_02A8_43FD_97BC_87D948840164__INCLUDED_)
#define AFX_LOGSAVECOMMANDHISTORYTOCURRENTTIME_H__89F9ACB5_02A8_43FD_97BC_87D948840164__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveCommandHistoryToCurrentTime : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveCommandHistoryToCurrentTime(long time, const zString name);

	virtual ~CLogSaveCommandHistoryToCurrentTime();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveCommandHistoryToCurrentTime& rMsg);

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

	CLogSaveCommandHistoryToCurrentTime(const CLogSaveCommandHistoryToCurrentTime& oldMessage);
	CLogSaveCommandHistoryToCurrentTime& operator=(const CLogSaveCommandHistoryToCurrentTime& rhs);


	// ****************************************
	// Data members
private:

	zString	 m_Name;	// Command document's name including ".xml" suffix
};

#endif // !defined(AFX_LOGSAVECOMMANDHISTORYTOCURRENTTIME_H__89F9ACB5_02A8_43FD_97BC_87D948840164__INCLUDED_)
