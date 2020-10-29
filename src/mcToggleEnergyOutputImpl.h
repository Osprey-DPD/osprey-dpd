// mcToggleEnergyOutputImpl.h: interface for the mcToggleEnergyOutputImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLEENERGYOUTPUTIMPL_H__4399C9F0_1B9E_48FF_8AD2_157BC6582308__INCLUDED_)
#define AFX_MCTOGGLEENERGYOUTPUTIMPL_H__4399C9F0_1B9E_48FF_8AD2_157BC6582308__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcToggleEnergyOutputImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcToggleEnergyOutputImpl();

	virtual ~mcToggleEnergyOutputImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleEnergyOutput(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCTOGGLEENERGYOUTPUTIMPL_H__4399C9F0_1B9E_48FF_8AD2_157BC6582308__INCLUDED_)
