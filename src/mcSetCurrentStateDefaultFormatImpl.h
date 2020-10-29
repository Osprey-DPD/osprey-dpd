// mcSetCurrentStateDefaultFormatImpl.h: interface for the mcSetCurrentStateDefaultFormatImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETCURRENTSTATEDEFAULTFORMATIMPL_H__B744D483_4E6A_4C96_9F7C_D4EFF30EFBA8__INCLUDED_)
#define AFX_MCSETCURRENTSTATEDEFAULTFORMATIMPL_H__B744D483_4E6A_4C96_9F7C_D4EFF30EFBA8__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetCurrentStateDefaultFormatImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetCurrentStateDefaultFormatImpl();

	virtual ~mcSetCurrentStateDefaultFormatImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetCurrentStateDefaultFormat(const xxCommand* const pCommand);


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
#endif // !defined(AFX_MCSETCURRENTSTATEDEFAULTFORMATIMPL_H__B744D483_4E6A_4C96_9F7C_D4EFF30EFBA8__INCLUDED_)
