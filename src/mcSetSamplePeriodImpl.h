// mcSetSamplePeriodImpl.h: interface for the mcSetSamplePeriodImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETSAMPLEPERIODIMPL_H__42FF3ACB_A946_4A9C_B670_11735EDF0009__INCLUDED_)
#define AFX_MCSETSAMPLEPERIODIMPL_H__42FF3ACB_A946_4A9C_B670_11735EDF0009__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetSamplePeriodImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetSamplePeriodImpl();

	virtual ~mcSetSamplePeriodImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetSamplePeriod(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETSAMPLEPERIODIMPL_H__42FF3ACB_A946_4A9C_B670_11735EDF0009__INCLUDED_)
