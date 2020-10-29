// ccRemoveReceptorBondImpl.h: interface for the ccRemoveReceptorBondImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCREMOVERECEPTORBONDIMPL_H__2A1169CC_148D_472D_AC23_B45740B79D80__INCLUDED_)
#define AFX_CCREMOVERECEPTORBONDIMPL_H__2A1169CC_148D_472D_AC23_B45740B79D80__INCLUDED_


#include "ISimBoxCmd.h"

class ccRemoveReceptorBondImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccRemoveReceptorBondImpl();

	virtual ~ccRemoveReceptorBondImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void RemoveReceptorBond(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCREMOVERECEPTORBONDIMPL_H__2A1169CC_148D_472D_AC23_B45740B79D80__INCLUDED_)
