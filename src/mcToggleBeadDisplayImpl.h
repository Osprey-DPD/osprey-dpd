// mcToggleBeadDisplayImpl.h: interface for the mcToggleBeadDisplayImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLEBEADDISPLAYIMPL_H__C6C5092E_ED46_475A_80BE_7AA8D49CA8FC__INCLUDED_)
#define AFX_MCTOGGLEBEADDISPLAYIMPL_H__C6C5092E_ED46_475A_80BE_7AA8D49CA8FC__INCLUDED_



// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcToggleBeadDisplayImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcToggleBeadDisplayImpl();

	virtual ~mcToggleBeadDisplayImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleBeadDisplay(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCTOGGLEBEADDISPLAYIMPL_H__C6C5092E_ED46_475A_80BE_7AA8D49CA8FC__INCLUDED_)
