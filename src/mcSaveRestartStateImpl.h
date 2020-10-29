// mcSaveRestartStateImpl.h: interface for the mcSaveRestartStateImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVERESTARTSTATEIMPL_H__07189415_83F1_4236_AD00_B1FD27687723__INCLUDED_)
#define AFX_MCSAVERESTARTSTATEIMPL_H__07189415_83F1_4236_AD00_B1FD27687723__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveRestartStateImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveRestartStateImpl();

	virtual ~mcSaveRestartStateImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveRestartStateCmd(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVERESTARTSTATEIMPL_H__07189415_83F1_4236_AD00_B1FD27687723__INCLUDED_)
