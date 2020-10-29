// mcWriteLogMessageImpl.h: interface for the mcWriteLogMessageImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCWRITELOGMESSAGEIMPL_H__787D806A_3C47_4793_AEE0_2F8B8DCDE254__INCLUDED_)
#define AFX_MCWRITELOGMESSAGEIMPL_H__787D806A_3C47_4793_AEE0_2F8B8DCDE254__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcWriteLogMessageImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcWriteLogMessageImpl();

	virtual ~mcWriteLogMessageImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void WriteLogMessage(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCWRITELOGMESSAGEIMPL_H__787D806A_3C47_4793_AEE0_2F8B8DCDE254__INCLUDED_)
