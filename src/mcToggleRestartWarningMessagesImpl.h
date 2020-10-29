// mcToggleRestartWarningMessagesImpl.h: interface for the mcToggleRestartWarningMessagesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLERESTARTWARNINGMESSAGESIMPL_H__751F3938_3F22_4A48_B7DA_F594F887D757__INCLUDED_)
#define AFX_MCTOGGLERESTARTWARNINGMESSAGESIMPL_H__751F3938_3F22_4A48_B7DA_F594F887D757__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"


class mcToggleRestartWarningMessagesImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcToggleRestartWarningMessagesImpl();

	virtual ~mcToggleRestartWarningMessagesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleRestartWarningMessages(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCTOGGLERESTARTWARNINGMESSAGESIMPL_H__751F3938_3F22_4A48_B7DA_F594F887D757__INCLUDED_)
