// ctChangeBeadTypeImpl.h: interface for the ctChangeBeadTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCHANGEBEADTYPEIMPL_H__C0F73E38_1515_4AB3_862E_2623B2BACEAE__INCLUDED_)
#define AFX_CTCHANGEBEADTYPEIMPL_H__C0F73E38_1515_4AB3_862E_2623B2BACEAE__INCLUDED_


#include "ISimBoxCmd.h"

class ctChangeBeadTypeImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctChangeBeadTypeImpl();

	virtual ~ctChangeBeadTypeImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ChangeBeadType(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTCHANGEBEADTYPEIMPL_H__C0F73E38_1515_4AB3_862E_2623B2BACEAE__INCLUDED_)
