// ctChangeBondTypeImpl.h: interface for the ctChangeBondTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCHANGEBONDTYPEIMPL_H__4818CAB7_E6C8_4140_9627_141B2E525C06__INCLUDED_)
#define AFX_CTCHANGEBONDTYPEIMPL_H__4818CAB7_E6C8_4140_9627_141B2E525C06__INCLUDED_


#include "ISimBoxCmd.h"

class ctChangeBondTypeImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctChangeBondTypeImpl();

	virtual ~ctChangeBondTypeImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ChangeBondType(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTCHANGEBONDTYPEIMPL_H__4818CAB7_E6C8_4140_9627_141B2E525C06__INCLUDED_)
