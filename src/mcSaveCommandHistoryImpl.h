// mcSaveCommandHistoryImpl.h: interface for the mcSaveCommandHistoryImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVECOMMANDHISTORYIMPL_H__72050047_D8ED_4D08_BAB5_3D763A27503B__INCLUDED_)
#define AFX_MCSAVECOMMANDHISTORYIMPL_H__72050047_D8ED_4D08_BAB5_3D763A27503B__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveCommandHistoryImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveCommandHistoryImpl();

	virtual ~mcSaveCommandHistoryImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:


	// ****************************************
	// Public access functions
public:

	void SaveCommandHistory(const xxCommand* const pCommand);

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

#endif // !defined(AFX_MCSAVECOMMANDHISTORYIMPL_H__72050047_D8ED_4D08_BAB5_3D763A27503B__INCLUDED_)
