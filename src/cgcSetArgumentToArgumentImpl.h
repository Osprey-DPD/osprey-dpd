// cgcSetArgumentToArgumentImpl.h: interface for the cgcSetArgumentToArgumentImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOARGUMENTIMPL_H__BE9DA57C_57C7_4A02_B2FC_7CE5C5814986__INCLUDED_)
#define AFX_CGCSETARGUMENTTOARGUMENTIMPL_H__BE9DA57C_57C7_4A02_B2FC_7CE5C5814986__INCLUDED_



#include "ISimBoxCmd.h"

class cgcSetArgumentToArgumentImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgcSetArgumentToArgumentImpl();

	virtual ~cgcSetArgumentToArgumentImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetArgumentToArgument(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGCSETARGUMENTTOARGUMENTIMPL_H__BE9DA57C_57C7_4A02_B2FC_7CE5C5814986__INCLUDED_)
