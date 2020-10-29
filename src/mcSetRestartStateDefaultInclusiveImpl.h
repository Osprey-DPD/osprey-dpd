// mcSetRestartStateDefaultInclusiveImpl.h: interface for the mcSetRestartStateDefaultInclusiveImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETRESTARTSTATEDEFAULTINCLUSIVEIMPL_H__0DD08851_F873_4DE0_B10C_A56F59356768__INCLUDED_)
#define AFX_MCSETRESTARTSTATEDEFAULTINCLUSIVEIMPL_H__0DD08851_F873_4DE0_B10C_A56F59356768__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetRestartStateDefaultInclusiveImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetRestartStateDefaultInclusiveImpl();

	virtual ~mcSetRestartStateDefaultInclusiveImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetRestartStateDefaultInclusive(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETRESTARTSTATEDEFAULTINCLUSIVEIMPL_H__0DD08851_F873_4DE0_B10C_A56F59356768__INCLUDED_)
