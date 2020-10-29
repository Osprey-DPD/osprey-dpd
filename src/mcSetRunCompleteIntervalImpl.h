// mcSetRunCompleteIntervalImpl.h: interface for the mcSetRunCompleteIntervalImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETRUNCOMPLETEINTERVALIMPL_H__EB685D2A_7884_42B1_97A4_9FC79946F6C9__INCLUDED_)
#define AFX_MCSETRUNCOMPLETEINTERVALIMPL_H__EB685D2A_7884_42B1_97A4_9FC79946F6C9__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetRunCompleteIntervalImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetRunCompleteIntervalImpl();

	virtual ~mcSetRunCompleteIntervalImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetRunCompleteInterval(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETRUNCOMPLETEINTERVALIMPL_H__EB685D2A_7884_42B1_97A4_9FC79946F6C9__INCLUDED_)
