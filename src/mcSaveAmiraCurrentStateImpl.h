// mcSaveAmiraCurrentStateImpl.h: interface for the mcSaveAmiraCurrentStateImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEAMIRACURRENTSTATEIMPL_H__94E48E61_15FD_4B86_8824_640B18E3012D__INCLUDED_)
#define AFX_MCSAVEAMIRACURRENTSTATEIMPL_H__94E48E61_15FD_4B86_8824_640B18E3012D__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveAmiraCurrentStateImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveAmiraCurrentStateImpl();

	virtual ~mcSaveAmiraCurrentStateImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveAmiraCurrentState(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVEAMIRACURRENTSTATEIMPL_H__94E48E61_15FD_4B86_8824_640B18E3012D__INCLUDED_)
