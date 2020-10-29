// mcSetPolymerTypeDisplayIdImpl.h: interface for the mcSetPolymerTypeDisplayIdImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETPOLYMERTYPEDISPLAYIDIMPL_H__B18C2209_FA8C_4ED1_934C_462CD854C5EB__INCLUDED_)
#define AFX_MCSETPOLYMERTYPEDISPLAYIDIMPL_H__B18C2209_FA8C_4ED1_934C_462CD854C5EB__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetPolymerTypeDisplayIdImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetPolymerTypeDisplayIdImpl();

	virtual ~mcSetPolymerTypeDisplayIdImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetPolymerTypeDisplayId(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETPOLYMERTYPEDISPLAYIDIMPL_H__B18C2209_FA8C_4ED1_934C_462CD854C5EB__INCLUDED_)
