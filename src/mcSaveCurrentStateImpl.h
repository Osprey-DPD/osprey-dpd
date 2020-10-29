// mcSaveCurrentStateImpl.h: interface for the mcSaveCurrentStateImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVECURRENTSTATEIMPL_H__2FE9446C_BCD5_45DC_A66B_89EEED1D1CBD__INCLUDED_)
#define AFX_MCSAVECURRENTSTATEIMPL_H__2FE9446C_BCD5_45DC_A66B_89EEED1D1CBD__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveCurrentStateImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveCurrentStateImpl();

	virtual ~mcSaveCurrentStateImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveCurrentStateCmd(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVECURRENTSTATEIMPL_H__2FE9446C_BCD5_45DC_A66B_89EEED1D1CBD__INCLUDED_)
