// mcSetAllBeadsInvisibleImpl.h: interface for the mcSetAllBeadsInvisibleImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETALLBEADSINVISIBLEIMPL_H__A812C8E4_05B7_4D15_8ABB_E45C641879AC__INCLUDED_)
#define AFX_MCSETALLBEADSINVISIBLEIMPL_H__A812C8E4_05B7_4D15_8ABB_E45C641879AC__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetAllBeadsInvisibleImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetAllBeadsInvisibleImpl();

	virtual ~mcSetAllBeadsInvisibleImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetAllBeadsInvisible(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETALLBEADSINVISIBLEIMPL_H__A812C8E4_05B7_4D15_8ABB_E45C641879AC__INCLUDED_)
