// ctSetTargetDisplayIdImpl.h: interface for the ctSetTargetDisplayIdImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETTARGETDISPLAYIDIMPL_H__3B7C98B7_4B04_42E1_8218_447A94549EB6__INCLUDED_)
#define AFX_CTSETTARGETDISPLAYIDIMPL_H__3B7C98B7_4B04_42E1_8218_447A94549EB6__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctSetTargetDisplayIdImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctSetTargetDisplayIdImpl();

	virtual ~ctSetTargetDisplayIdImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetTargetDisplayId(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTSETTARGETDISPLAYIDIMPL_H__3B7C98B7_4B04_42E1_8218_447A94549EB6__INCLUDED_)
