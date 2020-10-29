// mcSetPolymerDisplayIdImpl.h: interface for the mcSetPolymerDisplayIdImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETPOLYMERDISPLAYIDIMPL_H__0316603B_D03A_4A8A_A3ED_ACFCC3229548__INCLUDED_)
#define AFX_MCSETPOLYMERDISPLAYIDIMPL_H__0316603B_D03A_4A8A_A3ED_ACFCC3229548__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetPolymerDisplayIdImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetPolymerDisplayIdImpl();

	virtual ~mcSetPolymerDisplayIdImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetPolymerDisplayId(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETPOLYMERDISPLAYIDIMPL_H__0316603B_D03A_4A8A_A3ED_ACFCC3229548__INCLUDED_)
