// mcSaveBead1dDensityProfileImpl.h: interface for the mcSaveBead1dDensityProfileImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEBEAD1DDENSITYPROFILEIMPL_H__303277B3_894B_4A93_A485_B90185A5B073__INCLUDED_)
#define AFX_MCSAVEBEAD1DDENSITYPROFILEIMPL_H__303277B3_894B_4A93_A485_B90185A5B073__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveBead1dDensityProfileImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveBead1dDensityProfileImpl();

	virtual ~mcSaveBead1dDensityProfileImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveBead1dDensityProfile(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVEBEAD1DDENSITYPROFILEIMPL_H__303277B3_894B_4A93_A485_B90185A5B073__INCLUDED_)
