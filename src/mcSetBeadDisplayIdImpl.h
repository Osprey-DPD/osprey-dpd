// mcSetBeadDisplayIdImpl.h: interface for the mcSetBeadDisplayIdImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETBEADDISPLAYIDIMPL_H__E5627F13_BE82_4D00_B8F4_BFAD04B36D23__INCLUDED_)
#define AFX_MCSETBEADDISPLAYIDIMPL_H__E5627F13_BE82_4D00_B8F4_BFAD04B36D23__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetBeadDisplayIdImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetBeadDisplayIdImpl();

	virtual ~mcSetBeadDisplayIdImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetBeadDisplayId(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETBEADDISPLAYIDIMPL_H__E5627F13_BE82_4D00_B8F4_BFAD04B36D23__INCLUDED_)
