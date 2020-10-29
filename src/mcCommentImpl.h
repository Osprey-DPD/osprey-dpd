// mcCommentImpl.h: interface for the mcCommentImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCCOMMENTIMPL_H__EF18E5D1_CB5A_4B36_A031_8718B4DA17BB__INCLUDED_)
#define AFX_MCCOMMENTIMPL_H__EF18E5D1_CB5A_4B36_A031_8718B4DA17BB__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcCommentImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcCommentImpl();

	virtual ~mcCommentImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void Comment(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCCOMMENTIMPL_H__EF18E5D1_CB5A_4B36_A031_8718B4DA17BB__INCLUDED_)
