// cgcSetArgumentToStringConstantImpl.h: interface for the cgcSetArgumentToStringConstantImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOSTRINGCONSTANTIMPL_H__125D7ABA_7C99_436E_904C_CF03859EBA15__INCLUDED_)
#define AFX_CGCSETARGUMENTTOSTRINGCONSTANTIMPL_H__125D7ABA_7C99_436E_904C_CF03859EBA15__INCLUDED_



#include "ISimBoxCmd.h"

class cgcSetArgumentToStringConstantImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgcSetArgumentToStringConstantImpl();

	virtual ~cgcSetArgumentToStringConstantImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetArgumentToStringConstant(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGCSETARGUMENTTOSTRINGCONSTANTIMPL_H__125D7ABA_7C99_436E_904C_CF03859EBA15__INCLUDED_)
