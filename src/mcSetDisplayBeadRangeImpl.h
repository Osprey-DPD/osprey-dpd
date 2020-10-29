// mcSetDisplayBeadRangeImpl.h: interface for the mcSetDisplayBeadRangeImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETDISPLAYBEADRANGEIMPL_H__B318528D_3B54_4978_8DA3_7055268AF164__INCLUDED_)
#define AFX_MCSETDISPLAYBEADRANGEIMPL_H__B318528D_3B54_4978_8DA3_7055268AF164__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetDisplayBeadRangeImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetDisplayBeadRangeImpl();

	virtual ~mcSetDisplayBeadRangeImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetDisplayBeadRange(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETDISPLAYBEADRANGEIMPL_H__B318528D_3B54_4978_8DA3_7055268AF164__INCLUDED_)
