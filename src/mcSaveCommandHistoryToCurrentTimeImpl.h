// mcSaveCommandHistoryToCurrentTimeImpl.h: interface for the mcSaveCommandHistoryToCurrentTimeImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVECOMMANDHISTORYTOCURRENTTIMEIMPL_H__DE2A7046_5403_4434_A6FA_17140950BAF7__INCLUDED_)
#define AFX_MCSAVECOMMANDHISTORYTOCURRENTTIMEIMPL_H__DE2A7046_5403_4434_A6FA_17140950BAF7__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveCommandHistoryToCurrentTimeImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveCommandHistoryToCurrentTimeImpl();

	virtual ~mcSaveCommandHistoryToCurrentTimeImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	void SaveCommandHistoryToCurrentTime(const xxCommand* const pCommand);

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_MCSAVECOMMANDHISTORYTOCURRENTTIMEIMPL_H__DE2A7046_5403_4434_A6FA_17140950BAF7__INCLUDED_)
