// mcSetCurrentStateCameraImpl.h: interface for the mcSetCurrentStateCameraImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETCURRENTSTATECAMERAIMPL_H__8EF6BBC4_3D70_4E9B_9F09_3F9C7FD2EFA1__INCLUDED_)
#define AFX_MCSETCURRENTSTATECAMERAIMPL_H__8EF6BBC4_3D70_4E9B_9F09_3F9C7FD2EFA1__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetCurrentStateCameraImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetCurrentStateCameraImpl();

	virtual ~mcSetCurrentStateCameraImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetCurrentStateCamera(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETCURRENTSTATECAMERAIMPL_H__8EF6BBC4_3D70_4E9B_9F09_3F9C7FD2EFA1__INCLUDED_)
