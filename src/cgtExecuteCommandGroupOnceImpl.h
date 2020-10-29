// cgtExecuteCommandGroupOnceImpl.h: interface for the cgtExecuteCommandGroupOnceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTEXECUTECOMMANDGROUPONCEIMPL_H__B8B4CB11_B9F3_4825_9AB7_9985932464D3__INCLUDED_)
#define AFX_CGTEXECUTECOMMANDGROUPONCEIMPL_H__B8B4CB11_B9F3_4825_9AB7_9985932464D3__INCLUDED_



#include "ISimBoxCmd.h"

class cgtExecuteCommandGroupOnceImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgtExecuteCommandGroupOnceImpl();

	virtual ~cgtExecuteCommandGroupOnceImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ExecuteCommandGroupOnce(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGTEXECUTECOMMANDGROUPONCEIMPL_H__B8B4CB11_B9F3_4825_9AB7_9985932464D3__INCLUDED_)
