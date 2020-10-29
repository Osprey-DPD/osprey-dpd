// mcTogglePolymerDisplayImpl.h: interface for the mcTogglePolymerDisplayImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLEPOLYMERDISPLAYIMPL_H__75C40D3B_53FC_4DA6_AC7A_3D2A5E2E5E19__INCLUDED_)
#define AFX_MCTOGGLEPOLYMERDISPLAYIMPL_H__75C40D3B_53FC_4DA6_AC7A_3D2A5E2E5E19__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcTogglePolymerDisplayImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcTogglePolymerDisplayImpl();

	virtual ~mcTogglePolymerDisplayImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void TogglePolymerDisplay(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCTOGGLEPOLYMERDISPLAYIMPL_H__75C40D3B_53FC_4DA6_AC7A_3D2A5E2E5E19__INCLUDED_)
