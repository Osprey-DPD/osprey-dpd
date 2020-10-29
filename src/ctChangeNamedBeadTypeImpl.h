// ctChangeNamedBeadTypeImpl.h: interface for the ctChangeNamedBeadTypeImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCHANGENAMEDBEADTYPEIMPL_H__A56A9BC0_3BAA_4905_83FD_B2B37A13273E__INCLUDED_)
#define AFX_CTCHANGENAMEDBEADTYPEIMPL_H__A56A9BC0_3BAA_4905_83FD_B2B37A13273E__INCLUDED_


#include "ISimBoxCmd.h"

class ctChangeNamedBeadTypeImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctChangeNamedBeadTypeImpl();

	virtual ~ctChangeNamedBeadTypeImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ChangeNamedBeadType(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTCHANGENAMEDBEADTYPEIMPL_H__A56A9BC0_3BAA_4905_83FD_B2B37A13273E__INCLUDED_)
