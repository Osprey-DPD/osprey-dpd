// mcGetBuildIdImpl.h: interface for the mcGetBuildIdImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCGETBUILDIDIMPL_H__F261250E_35CA_4099_A590_EFA1DA7B2507__INCLUDED_)
#define AFX_MCGETBUILDIDIMPL_H__F261250E_35CA_4099_A590_EFA1DA7B2507__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcGetBuildIdImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcGetBuildIdImpl();

	virtual ~mcGetBuildIdImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void GetBuildId(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCGETBUILDIDIMPL_H__F261250E_35CA_4099_A590_EFA1DA7B2507__INCLUDED_)
