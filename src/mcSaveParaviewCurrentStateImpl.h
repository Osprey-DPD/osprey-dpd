// mcSaveParaviewCurrentStateImpl.h: interface for the mcSaveParaviewCurrentStateImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEPARAVIEWCURRENTSTATEIMPL_H__5900F1F1_620F_4F88_8468_F700F419F176__INCLUDED_)
#define AFX_MCSAVEPARAVIEWCURRENTSTATEIMPL_H__5900F1F1_620F_4F88_8468_F700F419F176__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveParaviewCurrentStateImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveParaviewCurrentStateImpl();

	virtual ~mcSaveParaviewCurrentStateImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveParaviewCurrentState(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVEPARAVIEWCURRENTSTATEIMPL_H__5900F1F1_620F_4F88_8468_F700F419F176__INCLUDED_)
