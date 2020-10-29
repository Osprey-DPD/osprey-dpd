// mcSetRestartPeriodImpl.h: interface for the mcSetRestartPeriodImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETRESTARTPERIODIMPL_H__7B08F140_8C39_49CC_A6AD_B99E3B7393A1__INCLUDED_)
#define AFX_MCSETRESTARTPERIODIMPL_H__7B08F140_8C39_49CC_A6AD_B99E3B7393A1__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetRestartPeriodImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetRestartPeriodImpl();

	virtual ~mcSetRestartPeriodImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetRestartPeriod(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETRESTARTPERIODIMPL_H__7B08F140_8C39_49CC_A6AD_B99E3B7393A1__INCLUDED_)
