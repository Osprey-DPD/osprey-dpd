// ccUnchargeBeadByTypeImpl.h: interface for the ccUnchargeBeadByTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCUNCHARGEBEADBYTYPEIMPL_H__39770F03_B17A_4C4B_8A9F_0CC77C70E8AC__INCLUDED_)
#define AFX_CCUNCHARGEBEADBYTYPEIMPL_H__39770F03_B17A_4C4B_8A9F_0CC77C70E8AC__INCLUDED_


#include "ISimBoxCmd.h"

class ccUnchargeBeadByTypeImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccUnchargeBeadByTypeImpl();

	virtual ~ccUnchargeBeadByTypeImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void UnchargeBeadByType(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCUNCHARGEBEADBYTYPEIMPL_H__39770F03_B17A_4C4B_8A9F_0CC77C70E8AC__INCLUDED_)
