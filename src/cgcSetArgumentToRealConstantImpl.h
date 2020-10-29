// cgcSetArgumentToRealConstantImpl.h: interface for the cgcSetArgumentToRealConstantImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOREALCONSTANTIMPL_H__6B07EBFF_41CD_4676_ABA7_C7310E6F0B80__INCLUDED_)
#define AFX_CGCSETARGUMENTTOREALCONSTANTIMPL_H__6B07EBFF_41CD_4676_ABA7_C7310E6F0B80__INCLUDED_



#include "ISimBoxCmd.h"

class cgcSetArgumentToRealConstantImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgcSetArgumentToRealConstantImpl();

	virtual ~cgcSetArgumentToRealConstantImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetArgumentToRealConstant(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGCSETARGUMENTTOREALCONSTANTIMPL_H__6B07EBFF_41CD_4676_ABA7_C7310E6F0B80__INCLUDED_)
