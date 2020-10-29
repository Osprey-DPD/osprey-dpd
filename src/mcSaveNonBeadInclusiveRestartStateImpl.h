// mcSaveNonBeadInclusiveRestartStateImpl.h: interface for the mcSaveNonBeadInclusiveRestartStateImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVENONBEADINCLUSIVERESTARTSTATEIMPL_H__D29D4F4F_DD04_42F1_9988_17038B0FA53C__INCLUDED_)
#define AFX_MCSAVENONBEADINCLUSIVERESTARTSTATEIMPL_H__D29D4F4F_DD04_42F1_9988_17038B0FA53C__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveNonBeadInclusiveRestartStateImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveNonBeadInclusiveRestartStateImpl();

	virtual ~mcSaveNonBeadInclusiveRestartStateImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveNonBeadInclusiveRestartStateCmd(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVENONBEADINCLUSIVERESTARTSTATEIMPL_H__D29D4F4F_DD04_42F1_9988_17038B0FA53C__INCLUDED_)
