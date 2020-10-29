// ctListCommandTargetsImpl.h: interface for the ctListCommandTargetsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTLISTCOMMANDTARGETSIMPL_H__08C7E550_E04D_4421_86D6_A3325636B39D__INCLUDED_)
#define AFX_CTLISTCOMMANDTARGETSIMPL_H__08C7E550_E04D_4421_86D6_A3325636B39D__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctListCommandTargetsImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctListCommandTargetsImpl();

	virtual ~ctListCommandTargetsImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ListCommandTargets(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTLISTCOMMANDTARGETSIMPL_H__08C7E550_E04D_4421_86D6_A3325636B39D__INCLUDED_)
