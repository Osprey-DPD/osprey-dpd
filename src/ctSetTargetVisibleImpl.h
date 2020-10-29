// ctSetTargetVisibleImpl.h: interface for the ctSetTargetVisibleImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETTARGETVISIBLEIMPL_H__729DDA00_19C5_4482_9D7C_68154760B975__INCLUDED_)
#define AFX_CTSETTARGETVISIBLEIMPL_H__729DDA00_19C5_4482_9D7C_68154760B975__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctSetTargetVisibleImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctSetTargetVisibleImpl();

	virtual ~ctSetTargetVisibleImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetTargetVisible(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTSETTARGETVISIBLEIMPL_H__729DDA00_19C5_4482_9D7C_68154760B975__INCLUDED_)
