// mcGetBuildDateImpl.h: interface for the mcGetBuildDateImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCGETBUILDDATEIMPL_H__29CB490F_4357_44F6_B4B8_F65186071465__INCLUDED_)
#define AFX_MCGETBUILDDATEIMPL_H__29CB490F_4357_44F6_B4B8_F65186071465__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcGetBuildDateImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcGetBuildDateImpl();

	virtual ~mcGetBuildDateImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void GetBuildDate(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCGETBUILDDATEIMPL_H__29CB490F_4357_44F6_B4B8_F65186071465__INCLUDED_)
