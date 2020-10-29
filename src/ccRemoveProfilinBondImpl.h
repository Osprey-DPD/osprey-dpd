// ccRemoveProfilinBondImpl.h: interface for the ccRemoveProfilinBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCREMOVEPROFILINBONDIMPL_H__15FF8B78_705C_42C1_9C52_ECBC042AF4F5__INCLUDED_)
#define AFX_CCREMOVEPROFILINBONDIMPL_H__15FF8B78_705C_42C1_9C52_ECBC042AF4F5__INCLUDED_


#include "ISimBoxCmd.h"

class ccRemoveProfilinBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccRemoveProfilinBondImpl();

	virtual ~ccRemoveProfilinBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void RemoveProfilinBond(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCREMOVEPROFILINBONDIMPL_H__15FF8B78_705C_42C1_9C52_ECBC042AF4F5__INCLUDED_)
