// ccChargeBeadByTypeImpl.h: interface for the ccChargeBeadByTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCCHARGEBEADBYTYPEIMPL_H__C3F2D44A_D9AF_4A50_B4B5_760168599664__INCLUDED_)
#define AFX_CCCHARGEBEADBYTYPEIMPL_H__C3F2D44A_D9AF_4A50_B4B5_760168599664__INCLUDED_


#include "ISimBoxCmd.h"

class ccChargeBeadByTypeImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccChargeBeadByTypeImpl();

	virtual ~ccChargeBeadByTypeImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ChargeBeadByType(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCCHARGEBEADBYTYPEIMPL_H__C3F2D44A_D9AF_4A50_B4B5_760168599664__INCLUDED_)
