// mcSetDensityPeriodImpl.h: interface for the mcSetDensityPeriodImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETDENSITYPERIODIMPL_H__60F90E59_0778_43B6_8DD8_4BC42D17D214__INCLUDED_)
#define AFX_MCSETDENSITYPERIODIMPL_H__60F90E59_0778_43B6_8DD8_4BC42D17D214__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetDensityPeriodImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetDensityPeriodImpl();

	virtual ~mcSetDensityPeriodImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetDensityPeriod(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETDENSITYPERIODIMPL_H__60F90E59_0778_43B6_8DD8_4BC42D17D214__INCLUDED_)
