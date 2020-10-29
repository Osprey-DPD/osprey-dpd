// mcSetDisplayPeriodImpl.h: interface for the mcSetDisplayPeriodImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETDISPLAYPERIODIMPL_H__287F2A25_A2A8_45EA_8C23_B00A627E4C4A__INCLUDED_)
#define AFX_MCSETDISPLAYPERIODIMPL_H__287F2A25_A2A8_45EA_8C23_B00A627E4C4A__INCLUDED_

  
// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetDisplayPeriodImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetDisplayPeriodImpl();

	virtual ~mcSetDisplayPeriodImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetDisplayPeriod(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETDISPLAYPERIODIMPL_H__287F2A25_A2A8_45EA_8C23_B00A627E4C4A__INCLUDED_)
