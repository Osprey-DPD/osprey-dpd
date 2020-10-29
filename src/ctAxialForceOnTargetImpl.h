// ctAxialForceOnTargetImpl.h: interface for the ctAxialForceOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTAXIALFORCEONTARGETIMPL_H__0AEA9E60_A796_4A5F_B17E_AE129E643DB0__INCLUDED_)
#define AFX_CTAXIALFORCEONTARGETIMPL_H__0AEA9E60_A796_4A5F_B17E_AE129E643DB0__INCLUDED_


#include "ISimBoxCmd.h"

class ctAxialForceOnTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctAxialForceOnTargetImpl();

	virtual ~ctAxialForceOnTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void AxialForceOnTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTAXIALFORCEONTARGETIMPL_H__0AEA9E60_A796_4A5F_B17E_AE129E643DB0__INCLUDED_)
