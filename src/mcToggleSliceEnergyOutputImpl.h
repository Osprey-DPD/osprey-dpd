// mcToggleSliceEnergyOutputImpl.h: interface for the mcToggleSliceEnergyOutputImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLESLICEENERGYOUTPUTIMPL_H__B30D1EBF_F3B4_40CD_A5D7_A71494D3BD45__INCLUDED_)
#define AFX_MCTOGGLESLICEENERGYOUTPUTIMPL_H__B30D1EBF_F3B4_40CD_A5D7_A71494D3BD45__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcToggleSliceEnergyOutputImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcToggleSliceEnergyOutputImpl();

	virtual ~mcToggleSliceEnergyOutputImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleSliceEnergyOutput(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCTOGGLESLICEENERGYOUTPUTIMPL_H__B30D1EBF_F3B4_40CD_A5D7_A71494D3BD45__INCLUDED_)
