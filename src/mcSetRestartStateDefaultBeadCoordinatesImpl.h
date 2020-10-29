// mcSetRestartStateDefaultBeadCoordinatesImpl.h: interface for the mcSetRestartStateDefaultBeadCoordinatesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETRESTARTSTATEDEFAULTBEADCOORDINATESIMPL_H__87FEA8F6_870F_4FC9_8602_775E2F89DE16__INCLUDED_)
#define AFX_MCSETRESTARTSTATEDEFAULTBEADCOORDINATESIMPL_H__87FEA8F6_870F_4FC9_8602_775E2F89DE16__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetRestartStateDefaultBeadCoordinatesImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetRestartStateDefaultBeadCoordinatesImpl();

	virtual ~mcSetRestartStateDefaultBeadCoordinatesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetRestartStateDefaultBeadCoordinates(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETRESTARTSTATEDEFAULTBEADCOORDINATESIMPL_H__87FEA8F6_870F_4FC9_8602_775E2F89DE16__INCLUDED_)
