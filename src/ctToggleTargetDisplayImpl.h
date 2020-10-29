// ctToggleTargetDisplayImpl.h: interface for the ctToggleTargetDisplayImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTTOGGLETARGETDISPLAYIMPL_H__FE1318B9_4785_4001_92E6_45EBEF80C8E2__INCLUDED_)
#define AFX_CTTOGGLETARGETDISPLAYIMPL_H__FE1318B9_4785_4001_92E6_45EBEF80C8E2__INCLUDED_


#include "ISimBoxCmd.h"

class ctToggleTargetDisplayImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctToggleTargetDisplayImpl();

	virtual ~ctToggleTargetDisplayImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleTargetDisplay(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTTOGGLETARGETDISPLAYIMPL_H__FE1318B9_4785_4001_92E6_45EBEF80C8E2__INCLUDED_)
