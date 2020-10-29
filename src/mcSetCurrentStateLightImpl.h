// mcSetCurrentStateLightImpl.h: interface for the mcSetCurrentStateLightImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETCURRENTSTATELIGHTIMPL_H__B099CCD3_6952_49BE_8FCC_AF59C3B93C48__INCLUDED_)
#define AFX_MCSETCURRENTSTATELIGHTIMPL_H__B099CCD3_6952_49BE_8FCC_AF59C3B93C48__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetCurrentStateLightImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetCurrentStateLightImpl();

	virtual ~mcSetCurrentStateLightImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetCurrentStateLight(const xxCommand* const pCommand);


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
#endif // !defined(AFX_MCSETCURRENTSTATELIGHTIMPL_H__B099CCD3_6952_49BE_8FCC_AF59C3B93C48__INCLUDED_)
