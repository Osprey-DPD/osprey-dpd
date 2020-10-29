// ctListActiveCommandTargetsImpl.h: interface for the ctListActiveCommandTargetsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTLISTACTIVECOMMANDTARGETSIMPL_H__2F305AD9_16E8_4593_BE32_FA87C1E9A928__INCLUDED_)
#define AFX_CTLISTACTIVECOMMANDTARGETSIMPL_H__2F305AD9_16E8_4593_BE32_FA87C1E9A928__INCLUDED_


// Forward declarations

class xxCommand;

#include "ISimBoxCmd.h"

class ctListActiveCommandTargetsImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctListActiveCommandTargetsImpl();

	virtual ~ctListActiveCommandTargetsImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ListActiveCommandTargets(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTLISTACTIVECOMMANDTARGETSIMPL_H__2F305AD9_16E8_4593_BE32_FA87C1E9A928__INCLUDED_)
