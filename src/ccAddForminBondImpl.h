// ccAddForminBondImpl.h: interface for the ccAddForminBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCADDFORMINBONDIMPL_H__5E8E702D_CA73_4BE0_983A_9D11F439AC8E__INCLUDED_)
#define AFX_CCADDFORMINBONDIMPL_H__5E8E702D_CA73_4BE0_983A_9D11F439AC8E__INCLUDED_


#include "ISimBoxCmd.h"

class ccAddForminBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccAddForminBondImpl();

	virtual ~ccAddForminBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void AddForminBond(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCADDFORMINBONDIMPL_H__5E8E702D_CA73_4BE0_983A_9D11F439AC8E__INCLUDED_)
