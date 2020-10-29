// LogSetCommandTimer.h: interface for the CLogSetCommandTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETCOMMANDTIMER_H__33AA7472_7FB9_4533_9B17_993166C8C213__INCLUDED_)
#define AFX_LOGSETCOMMANDTIMER_H__33AA7472_7FB9_4533_9B17_993166C8C213__INCLUDED_


#include "LogConstraintMessage.h"

class CLogSetCommandTimer : public CLogConstraintMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetCommandTimer(long time, long delay, StringSequence& rCommands);

	virtual ~CLogSetCommandTimer();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetCommandTimer& rMsg);

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

	CLogSetCommandTimer(const CLogSetCommandTimer& oldMessage);
	CLogSetCommandTimer& operator=(const CLogSetCommandTimer& rhs);


	// ****************************************
	// Data members
private:

	const long		m_Delay;		// No of time steps before execution
	StringSequence	m_Commands;		// Names of commands to execute after delay
};


#endif // !defined(AFX_LOGSETCOMMANDTIMER_H__33AA7472_7FB9_4533_9B17_993166C8C213__INCLUDED_)
