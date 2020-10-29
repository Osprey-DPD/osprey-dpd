// mcToggleCurrentStateBoxImpl.h: interface for the mcToggleCurrentStateBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLECURRENTSTATEBOXIMPL_H__5DD2644C_E521_40CD_842C_7C6816725092__INCLUDED_)
#define AFX_MCTOGGLECURRENTSTATEBOXIMPL_H__5DD2644C_E521_40CD_842C_7C6816725092__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcToggleCurrentStateBoxImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcToggleCurrentStateBoxImpl();

	virtual ~mcToggleCurrentStateBoxImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleCurrentStateBox(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCTOGGLECURRENTSTATEBOXIMPL_H__5DD2644C_E521_40CD_842C_7C6816725092__INCLUDED_)
