// cgcSetArgumentToIntegerConstantImpl.h: interface for the cgcSetArgumentToIntegerConstantImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOINTEGERCONSTANTIMPL_H__44FB203A_52A7_48BC_8BAB_65127D2304EF__INCLUDED_)
#define AFX_CGCSETARGUMENTTOINTEGERCONSTANTIMPL_H__44FB203A_52A7_48BC_8BAB_65127D2304EF__INCLUDED_



#include "ISimBoxCmd.h"

class cgcSetArgumentToIntegerConstantImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgcSetArgumentToIntegerConstantImpl();

	virtual ~cgcSetArgumentToIntegerConstantImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetArgumentToIntegerConstant(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGCSETARGUMENTTOINTEGERCONSTANTIMPL_H__44FB203A_52A7_48BC_8BAB_65127D2304EF__INCLUDED_)
