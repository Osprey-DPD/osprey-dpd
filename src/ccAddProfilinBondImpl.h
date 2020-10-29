// ccAddProfilinBondImpl.h: interface for the ccAddProfilinBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDPROFILINBONDIMPL_H__D7E8D140_0B3C_4E6F_8E47_A8200ACD7936__INCLUDED_)
#define AFX_CCADDPROFILINBONDIMPL_H__D7E8D140_0B3C_4E6F_8E47_A8200ACD7936__INCLUDED_


#include "ISimBoxCmd.h"

class ccAddProfilinBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccAddProfilinBondImpl();

	virtual ~ccAddProfilinBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void AddProfilinBond(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCADDPROFILINBONDIMPL_H__D7E8D140_0B3C_4E6F_8E47_A8200ACD7936__INCLUDED_)
