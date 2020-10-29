// ctSetTargetPolymerTypeDisplayIdImpl.h: interface for the ctSetTargetPolymerTypeDisplayIdImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTSETTARGETPOLYMERTYPEDISPLAYIDIMPL_H__68911126_3535_4DFA_8F68_D3F22A2403AE__INCLUDED_)
#define AFX_CTSETTARGETPOLYMERTYPEDISPLAYIDIMPL_H__68911126_3535_4DFA_8F68_D3F22A2403AE__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctSetTargetPolymerTypeDisplayIdImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctSetTargetPolymerTypeDisplayIdImpl();

	virtual ~ctSetTargetPolymerTypeDisplayIdImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetTargetPolymerTypeDisplayId(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTSETTARGETPOLYMERTYPEDISPLAYIDIMPL_H__68911126_3535_4DFA_8F68_D3F22A2403AE__INCLUDED_)
