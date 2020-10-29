// mcSetAllBeadsVisibleImpl.h: interface for the mcSetAllBeadsVisibleImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETALLBEADSVISIBLEIMPL_H__D17BFF58_07F6_4E07_892F_CCCE79B005EC__INCLUDED_)
#define AFX_MCSETALLBEADSVISIBLEIMPL_H__D17BFF58_07F6_4E07_892F_CCCE79B005EC__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetAllBeadsVisibleImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetAllBeadsVisibleImpl();

	virtual ~mcSetAllBeadsVisibleImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetAllBeadsVisible(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETALLBEADSVISIBLEIMPL_H__D17BFF58_07F6_4E07_892F_CCCE79B005EC__INCLUDED_)
