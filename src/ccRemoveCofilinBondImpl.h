// ccRemoveCofilinBondImpl.h: interface for the ccRemoveCofilinBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCREMOVECOFILINBONDIMPL_H__53157324_9DA2_4191_B6C0_B6A4C92A3B01__INCLUDED_)
#define AFX_CCREMOVECOFILINBONDIMPL_H__53157324_9DA2_4191_B6C0_B6A4C92A3B01__INCLUDED_


#include "ISimBoxCmd.h"

class ccRemoveCofilinBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccRemoveCofilinBondImpl();

	virtual ~ccRemoveCofilinBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void RemoveCofilinBond(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCREMOVECOFILINBONDIMPL_H__53157324_9DA2_4191_B6C0_B6A4C92A3B01__INCLUDED_)
