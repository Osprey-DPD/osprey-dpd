// ctChangeNamedBeadTypeInCompositeTargetImpl.h: interface for the ctChangeNamedBeadTypeInCompositeTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCHANGENAMEDBEADTYPEINCOMPOSITETARGETIMPL_H__EE7F2FFD_3FEF_4C8D_9A01_82DA563AA6E2__INCLUDED_)
#define AFX_CTCHANGENAMEDBEADTYPEINCOMPOSITETARGETIMPL_H__EE7F2FFD_3FEF_4C8D_9A01_82DA563AA6E2__INCLUDED_


#include "ISimBoxCmd.h"

class ctChangeNamedBeadTypeInCompositeTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctChangeNamedBeadTypeInCompositeTargetImpl();

	virtual ~ctChangeNamedBeadTypeInCompositeTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ChangeNamedBeadTypeInCompositeTarget(const xxCommand* const pCommand);


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


#endif // !defined(AFX_CTCHANGENAMEDBEADTYPEINCOMPOSITETARGETIMPL_H__EE7F2FFD_3FEF_4C8D_9A01_82DA563AA6E2__INCLUDED_)
