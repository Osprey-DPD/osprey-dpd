// mcShowModifiableProcessesImpl.h: interface for the mcShowModifiableProcessesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSHOWMODIFIABLEPROCESSESIMPL_H__6736500A_EF91_40EE_BBFC_B153BA1204BB__INCLUDED_)
#define AFX_MCSHOWMODIFIABLEPROCESSESIMPL_H__6736500A_EF91_40EE_BBFC_B153BA1204BB__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcShowModifiableProcessesImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcShowModifiableProcessesImpl();

	virtual ~mcShowModifiableProcessesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ShowModifiableProcesses(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSHOWMODIFIABLEPROCESSESIMPL_H__6736500A_EF91_40EE_BBFC_B153BA1204BB__INCLUDED_)
