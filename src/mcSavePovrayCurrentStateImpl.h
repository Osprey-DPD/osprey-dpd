// mcSavePovrayCurrentStateImpl.h: interface for the mcSavePovrayCurrentStateImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEPOVRAYCURRENTSTATEIMPL_H__3CF63126_3931_4F4A_8BDE_6739CFAC70EE__INCLUDED_)
#define AFX_MCSAVEPOVRAYCURRENTSTATEIMPL_H__3CF63126_3931_4F4A_8BDE_6739CFAC70EE__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSavePovrayCurrentStateImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSavePovrayCurrentStateImpl();

	virtual ~mcSavePovrayCurrentStateImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SavePovrayCurrentState(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVEPOVRAYCURRENTSTATEIMPL_H__3CF63126_3931_4F4A_8BDE_6739CFAC70EE__INCLUDED_)
