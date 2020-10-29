// ccRemoveForminBondImpl.h: interface for the ccRemoveForminBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCREMOVEFORMINBONDIMPL_H__6C0063A0_FFA3_4B49_9B5E_9C0DCDAD5A23__INCLUDED_)
#define AFX_CCREMOVEFORMINBONDIMPL_H__6C0063A0_FFA3_4B49_9B5E_9C0DCDAD5A23__INCLUDED_


#include "ISimBoxCmd.h"

class ccRemoveForminBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccRemoveForminBondImpl();

	virtual ~ccRemoveForminBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void RemoveForminBond(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCREMOVEFORMINBONDIMPL_H__6C0063A0_FFA3_4B49_9B5E_9C0DCDAD5A23__INCLUDED_)
