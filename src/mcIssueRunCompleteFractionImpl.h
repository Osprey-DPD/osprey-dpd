// mcIssueRunCompleteFractionImpl.h: interface for the mcIssueRunCompleteFractionImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCISSUERUNCOMPLETEFRACTIONIMPL_H__31F845D9_C135_4955_BAC7_73AB2872868D__INCLUDED_)
#define AFX_MCISSUERUNCOMPLETEFRACTIONIMPL_H__31F845D9_C135_4955_BAC7_73AB2872868D__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcIssueRunCompleteFractionImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcIssueRunCompleteFractionImpl();

	virtual ~mcIssueRunCompleteFractionImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void IssueRunCompleteFraction(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCISSUERUNCOMPLETEFRACTIONIMPL_H__31F845D9_C135_4955_BAC7_73AB2872868D__INCLUDED_)
