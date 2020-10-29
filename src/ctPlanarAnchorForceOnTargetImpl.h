// ctPlanarAnchorForceOnTargetImpl.h: interface for the ctPlanarAnchorForceOnTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTPLANARANCHORFORCEONTARGETIMPL_H__bf344c34_0247_403f_b09b_16221263b20d__INCLUDED_)
#define AFX_CTPLANARANCHORFORCEONTARGETIMPL_H__bf344c34_0247_403f_b09b_16221263b20d__INCLUDED_


#include "ISimBoxCmd.h"

class ctPlanarAnchorForceOnTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctPlanarAnchorForceOnTargetImpl();

	virtual ~ctPlanarAnchorForceOnTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void PlanarAnchorForceOnTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTPLANARANCHORFORCEONTARGETIMPL_H__bf344c34_0247_403f_b09b_16221263b20d__INCLUDED_)
