// ctExternalWorkOnTargetImpl.h: interface for the ctExternalWorkOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTEXTERNALWORKONTARGETIMPL_H__90A1AB55_C69A_4128_86CC_254C8759795B__INCLUDED_)
#define AFX_CTEXTERNALWORKONTARGETIMPL_H__90A1AB55_C69A_4128_86CC_254C8759795B__INCLUDED_


#include "ISimBoxCmd.h"

class ctExternalWorkOnTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctExternalWorkOnTargetImpl();

	virtual ~ctExternalWorkOnTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ExternalWorkOnTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTEXTERNALWORKONTARGETIMPL_H__90A1AB55_C69A_4128_86CC_254C8759795B__INCLUDED_)
