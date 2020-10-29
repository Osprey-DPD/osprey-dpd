// mcSetBeadTypeDisplayIdImpl.h: interface for the mcSetBeadTypeDisplayIdImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETBEADTYPEDISPLAYIDIMPL_H__074A1165_B67E_4074_A165_61C83B7B3255__INCLUDED_)
#define AFX_MCSETBEADTYPEDISPLAYIDIMPL_H__074A1165_B67E_4074_A165_61C83B7B3255__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetBeadTypeDisplayIdImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetBeadTypeDisplayIdImpl();

	virtual ~mcSetBeadTypeDisplayIdImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetBeadTypeDisplayId(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETBEADTYPEDISPLAYIDIMPL_H__074A1165_B67E_4074_A165_61C83B7B3255__INCLUDED_)
