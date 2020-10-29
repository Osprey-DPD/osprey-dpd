// mcToggleDensityFieldOutputImpl.h: interface for the mcToggleDensityFieldOutputImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLEDENSITYFIELDOUTPUTIMPL_H__B024E119_7302_4628_839B_3004489032EB__INCLUDED_)
#define AFX_MCTOGGLEDENSITYFIELDOUTPUTIMPL_H__B024E119_7302_4628_839B_3004489032EB__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcToggleDensityFieldOutputImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcToggleDensityFieldOutputImpl();

	virtual ~mcToggleDensityFieldOutputImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleDensityFieldOutput(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCTOGGLEDENSITYFIELDOUTPUTIMPL_H__B024E119_7302_4628_839B_3004489032EB__INCLUDED_)
