// ctAnchorForceOnTargetImpl.h: interface for the ctAnchorForceOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTANCHORFORCEONTARGETIMPL_H__b55a9cde_aecd_4ec0_801c_9263aeb8d7cd__INCLUDED_)
#define AFX_CTANCHORFORCEONTARGETIMPL_H__b55a9cde_aecd_4ec0_801c_9263aeb8d7cd__INCLUDED_


#include "ISimBoxCmd.h"

class ctAnchorForceOnTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctAnchorForceOnTargetImpl();

	virtual ~ctAnchorForceOnTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void AnchorForceOnTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTANCHORFORCEONTARGETIMPL_H__b55a9cde_aecd_4ec0_801c_9263aeb8d7cd__INCLUDED_)
