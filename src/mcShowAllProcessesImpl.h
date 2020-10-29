// mcShowAllProcessesImpl.h: interface for the mcShowAllProcessesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSHOWALLPROCESSESIMPL_H__1216805F_C4F8_4939_A2F7_29C68D64175A__INCLUDED_)
#define AFX_MCSHOWALLPROCESSESIMPL_H__1216805F_C4F8_4939_A2F7_29C68D64175A__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcShowAllProcessesImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcShowAllProcessesImpl();

	virtual ~mcShowAllProcessesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ShowAllProcesses(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSHOWALLPROCESSESIMPL_H__1216805F_C4F8_4939_A2F7_29C68D64175A__INCLUDED_)
