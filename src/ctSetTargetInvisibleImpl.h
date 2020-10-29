// ctSetTargetInvisibleImpl.h: interface for the ctSetTargetInvisibleImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETTARGETINVISIBLEIMPL_H__7375C25E_BE2C_4E22_AC48_E1EE516F8A3C__INCLUDED_)
#define AFX_CTSETTARGETINVISIBLEIMPL_H__7375C25E_BE2C_4E22_AC48_E1EE516F8A3C__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctSetTargetInvisibleImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctSetTargetInvisibleImpl();

	virtual ~ctSetTargetInvisibleImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetTargetInvisible(const xxCommand* const pCommand);


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


#endif // !defined(AFX_CTSETTARGETINVISIBLEIMPL_H__7375C25E_BE2C_4E22_AC48_E1EE516F8A3C__INCLUDED_)
