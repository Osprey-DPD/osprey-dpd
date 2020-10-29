// ctEdgeClampForceOnTargetImpl.h: interface for the ctEdgeClampForceOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTEDGECLAMPFORCEONTARGETIMPL_H__5d6ea441_d560_423f_8b6e_b915cb7ab989__INCLUDED_)
#define AFX_CTEDGECLAMPFORCEONTARGETIMPL_H__5d6ea441_d560_423f_8b6e_b915cb7ab989__INCLUDED_


#include "ISimBoxCmd.h"

class ctEdgeClampForceOnTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctEdgeClampForceOnTargetImpl();

	virtual ~ctEdgeClampForceOnTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void EdgeClampForceOnTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTEDGECLAMPFORCEONTARGETIMPL_H__5d6ea441_d560_423f_8b6e_b915cb7ab989__INCLUDED_)
