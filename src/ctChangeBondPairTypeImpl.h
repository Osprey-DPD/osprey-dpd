// ctChangeBondPairTypeImpl.h: interface for the ctChangeBondPairTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCHANGEBONDPAIRTYPEIMPL_H__15A33162_7429_45E1_BA4A_BE1405522E66__INCLUDED_)
#define AFX_CTCHANGEBONDPAIRTYPEIMPL_H__15A33162_7429_45E1_BA4A_BE1405522E66__INCLUDED_


#include "ISimBoxCmd.h"

class ctChangeBondPairTypeImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctChangeBondPairTypeImpl();

	virtual ~ctChangeBondPairTypeImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ChangeBondPairType(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTCHANGEBONDPAIRTYPEIMPL_H__15A33162_7429_45E1_BA4A_BE1405522E66__INCLUDED_)
